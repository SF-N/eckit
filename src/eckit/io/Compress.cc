/*
 * (C) Copyright 1996-2017 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */


#include "eckit/io/Compress.h"
#include "eckit/io/DataHandle.h"
#include "eckit/io/BitIO.h"

// This code is written for readibility, not speed
// See https://users.cs.cf.ac.uk/Dave.Marshall/Multimedia/node214.html
// for a description of the algorythm


namespace eckit {

enum {
    RESET_TABLE = 256,
    END_MARKER  = 257,
    FIRST_CODE  = 258,

    BITS_MIN =  9,
    BITS_MAX = 16   ,

};


static void print_code(std::ostream& out, size_t s) {

    switch (s) {

    case RESET_TABLE : out << "(reset)"; break;
    case END_MARKER : out << "(end)"; break;

    default:
        if (::isprint(s)) {
            out << char(s);
        }
        else {
            if (s < 256)
            {out << std::hex << "0x" << s << std::dec;}
            else {
                out << '(' << s << ')';
            }
        }
        break;
    }
}

class Entry  {

    void print(std::ostream& out) const;

    std::vector<unsigned char> chars_;
    size_t code_;

public:

    Entry(size_t = END_MARKER);

    Entry operator+(unsigned char) const;
    Entry& operator=(unsigned char);
    bool operator<(const Entry& other) const;
    bool empty() const;

    void code(size_t c) {
        code_ = c;
    }

    unsigned char firstChar() const {
        ASSERT(chars_.size());
        return chars_[0];
    }

    friend std::ostream& operator<<(std::ostream& out, const Entry& e) {
        e.print(out);
        return out;
    }

    void output(BitIO& out) const;
    void output(BitIO& out, size_t nbits) const;

};

Entry::Entry(size_t code) :
    code_(code) {

    if (code < 256) {
        chars_.push_back(code) ;
    }
}

Entry Entry::operator+(unsigned char c) const {
    Entry result = *this;
    result.chars_.push_back(c);
    return result;
}

Entry& Entry::operator=(unsigned char c) {
    chars_.clear();
    chars_.push_back(c);
    code_ = c;
    return *this;
}

bool Entry::operator<(const Entry& other) const {
    return chars_ < other.chars_;
}

void Entry::output(eckit::BitIO& out) const {
    std::cout << "Output " << *this << std::endl;

    for (unsigned char c : chars_) {
        out.write(c, 8);
    }
}

void Entry::print(std::ostream& out) const {
    out << '[';
    print_code(out, code_);
    out << " -> ";
    for (unsigned char c : chars_) {
        print_code(out, c);
    }
    out << ']';

}

bool Entry::empty() const {
    return chars_.empty();
}

void Entry::output(eckit::BitIO& out, size_t nbits) const {
    std::cout << "Send "
              << *this
              << " (nbits="
              << nbits
              << ")"
              << std::endl;

    out.write(code_, nbits);
}
//----------------------------------------------------------------------------------------------------------------------

inline static size_t MAX_CODE(size_t nbits) {
    return (1 << nbits) - 1;
}

static inline size_t next_byte(BitIO& in) {
    size_t byte = in.read(8, END_MARKER);
    std::cout << "Read ";
    print_code(std::cout, byte);
    std::cout << std::endl;
    return byte;
}

static void init_table(std::set<Entry>& table) {
    table.clear();
    for (size_t c = 0; c < 256; ++c) {
        table.insert(Entry(c));
    }
}

size_t Compress::encode(DataHandle& in, DataHandle& out)
{
    Entry eoi(END_MARKER);
    Entry reset(RESET_TABLE);

    std::set<Entry> code_table;
    init_table(code_table);

    BitIO bin(in);
    BitIO bout(out);

    size_t nbits = BITS_MIN;
    size_t next_code = FIRST_CODE;

    reset.output(bout, nbits);

    Entry w;
    for (;;)
    {
        size_t k = next_byte(bin);

        if (k == END_MARKER) {
            break;
        }

        Entry wk = w + k;

        auto j = code_table.find(wk);

        // Sequence in table
        if (j != code_table.end()) {
            // '*j' is the same as 'wp'
            // but should contain a valid code
            w = *j;
            continue;
        }

        wk.code(next_code++);
        code_table.insert(wk);

        w.output(bout, nbits);

        w = k;

    }

    if (!w.empty()) {
        w.output(bout, nbits);
    }

    eoi.output(bout, nbits);
    bout.flush();

    return bout.count();

}
//----------------------------------------------------------------------------------------------------------------------


static void init_table( std::vector<Entry>& table) {
    table.clear();
    for (size_t i = 0; i < 256; ++i) {
        table.push_back(Entry(i));
    }

    table.push_back(RESET_TABLE);
    table.push_back(END_MARKER);

}

size_t Compress::decode(DataHandle& in, DataHandle& out)
{
    std::vector<Entry> table;
    init_table(table);

    BitIO bin(in);
    BitIO bout(out);

    size_t nbits = BITS_MIN;

    Entry w;

    for (;;) {

        size_t k = bin.read(nbits, END_MARKER);

        std::cout << "Got code ";
        print_code(std::cout, k);
        std::cout << std::endl;

        if (k == END_MARKER) {
            break;
        }

        /* This should be the first code */
        if (k == RESET_TABLE)
        {

            nbits = BITS_MIN;
            // max_code = MAX_CODE(nbits);
            // next_code = FIRST_CODE;
            init_table(table);
            // prev = END_MARKER;

            k = bin.read(nbits, END_MARKER);
            if (k == END_MARKER) {
                break;
            }

            std::cout << "Got code ";
            print_code(std::cout, k);
            std::cout << std::endl;

            w = k;

            w.output(bout);

            continue;
        }

        Entry e = table.at(k);
        e.output(bout);

        Entry n = w + e.firstChar();
        n.code(table.size());
        table.push_back(n);

        std::cout << "Add code " << n << std::endl;

        w = e;

    }

    bout.flush();
    return bout.count();
}
//----------------------------------------------------------------------------------------------------------------------
} // namespace eckit
