/*
 * (C) Copyright 1996- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 *
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */


#include <iostream>
#include <numeric>

#include "griditer/util.h"


template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
    const auto* sep = "";
    for (const auto& value : v) {
        out << sep << value;
        sep = ", ";
    }
    return out;
}


template <typename X>
struct iterator_t {
    X& cnt;
    size_t pos;

    bool operator!=(const iterator_t& other) const { return &cnt != &other.cnt || pos != other.pos; }

    iterator_t& operator++() {
        pos++;
        return *this;
    }

    iterator_t operator++(int) {
        auto old = *this;
        operator++();
        return old;
    }

    typename X::value_type& operator*() { return cnt.at(pos); }

    const typename X::value_type& operator*() const { return cnt.at(pos); }
};


class iterable_t : public std::vector<double> {

    using iterator       = iterator_t<iterable_t>;
    using const_iterator = iterator_t<const iterable_t>;

public:
    using vector::vector;

    iterator begin() { return {*this, 0}; }

    iterator end() { return {*this, this->size()}; }

    const_iterator cbegin() const { return {*this, 0}; }

    const_iterator cend() const { return {*this, this->size()}; }

    const_iterator begin() const { return cbegin(); }

    const_iterator end() const { return cend(); }
};


int main(int argc, char* argv[]) {
    //    std::cout << grit::util::linspace(1, 2, 2, true) << std::endl;
    //    std::cout << grit::util::arange(1, 2, 0.5) << std::endl;
    //    std::cout << grit::util::gaussian_latitudes(64, false) << std::endl;


    iterable_t it{1, 2, 3, 4};

    for (const auto& v : it) {
        std::cout << v << std::endl;
    }

    std::cout << std::accumulate(it.begin(), it.end(), 0) << std::endl;
}
