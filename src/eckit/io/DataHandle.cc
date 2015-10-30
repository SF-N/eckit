/*
 * (C) Copyright 1996-2013 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */


#include "eckit/io/Buffer.h"
#include "eckit/log/Bytes.h"
#include "eckit/io/DataHandle.h"
#include "eckit/io/DblBuffer.h"
#include "eckit/io/MoverTransfer.h"
#include "eckit/filesystem/PathName.h"
#include "eckit/log/Progress.h"
#include "eckit/config/Resource.h"
#include "eckit/log/Timer.h"

//-----------------------------------------------------------------------------

namespace eckit {

//-----------------------------------------------------------------------------

ClassSpec DataHandle::classSpec_ = {&Streamable::classSpec(),"DataHandle",};
Reanimator<DataHandle> DataHandle::reanimator_;

DataHandle::DataHandle()
{
}

DataHandle::DataHandle(Stream& s):
    Streamable(s)
{
}

AutoClose::~AutoClose() 
{ 
    bool fail = !Exception::throwing();

    try {
        handle_.close(); 
    }
    catch(std::exception& e)
    {
        Log::error() << "** " << e.what() << " Caught in " << Here() << std::endl;
        if(fail)
        {
            Log::error() << "** Exception is re-thrown" << std::endl;
            throw;
        }
        Log::error() << "** An exception is already in progress" << std::endl;
        Log::error() << "** Exception is ignored" << std::endl;
    }
}

static const char *b="KMGPH";

static double rate(double x,char& c)
{
    const char* p = b;
    c = ' ';
    while(x > 100)
    {
        x /= 1024;	
        c = *p++;
    }
    if(x>=10)
        return long(x+0.5);
    else
        return long(x*10+0.5)/10.0;
}

void DataHandle::encode(Stream& s) const
{
    Streamable::encode(s);
}

void DataHandle::flush()
{
    Log::error() << *this << std::endl;
    NOTIMP;
}

Length DataHandle::saveInto(DataHandle& other,TransferWatcher& watcher)
{
    static bool moverTransfer = Resource<bool>("-mover;moverTransfer",0);


    compress();

    Log::status() << Bytes(estimate()) << " " << title() << " => " << other.title() << std::endl;

    if(moverTransfer && moveable() && other.moveable())
    {
        //Log::info() << "Using MoverTransfer" << std::endl;
        MoverTransfer mover(watcher);
        return mover.transfer(*this,other);
    }


    static bool doubleBuffer = Resource<bool>("doubleBuffer",0);

    if(doubleBuffer)
    {
        long bufsize = Resource<long>("doubleBufferSize",10*1024*1024/20);
        long count   = Resource<long>("doubleBufferCount",20);

        DblBuffer buf(count,bufsize,watcher);
        return buf.copy(*this,other);
    }
    else
    {

        long bufsize = Resource<long>("bufferSize",64*1024*1024);

        Buffer buffer(bufsize);
        //ResizableBuffer buffer(bufsize);


		watcher.watch(0,0);

        Length estimate = openForRead(); AutoClose closer1(*this);
        other.openForWrite(estimate);    AutoClose closer2(other);

        Progress progress("Moving data",0,estimate);

        Length total = 0;
        long length = -1;
        double readTime = 0;
        double lastRead = 0;
        double writeTime = 0;
        double lastWrite = 0;
        Timer timer("Save into");
        bool more = true;

        while(more)
        {
            more = false;
            try {
                while( (length = read(buffer,buffer.size())) > 0)
                {
                    double r = timer.elapsed() - lastRead;
                    readTime += r;
                    lastWrite = timer.elapsed();

                    if(other.write((const char*)buffer,length) != length) 
                        throw WriteError(name() + " into " + other.name());

                    double w = timer.elapsed() - lastWrite;
                    writeTime += w;
                    total += length;
                    progress(total);
                    watcher.watch(buffer,length);
                    lastRead = timer.elapsed();

                    char c1,c2;
                    //			Log::message() << rate(length/r,c1)  << c1 << " " << rate(length/w,c2) << c2 << std::endl;
                    Log::message() << rate(total/readTime,c1)  << c1 << " " << rate(total/writeTime,c2) << c2 << std::endl;
                }
            }
            catch(RestartTransfer& retry)
            {
                Log::warning() << "Retrying transfer from " << retry.from() << " (" << Bytes(retry.from()) << ")" << std::endl;
                restartReadFrom(retry.from());
                other.restartWriteFrom(retry.from());
                Log::warning() << "Total so far " << total << std::endl;
                total = Length(0) + retry.from();
                Log::warning() << "New total " << total << std::endl;
                more = true;
            }
        }

        Log::message() <<  "" << std::endl;


        Log::info() << "Read  rate: " << Bytes(total/readTime)  << "/s" << std::endl;
        Log::info() << "Write rate: " << Bytes(total/writeTime) << "/s" << std::endl;

        if(length < 0)
            throw ReadError(name() + " into " + other.name());

        if(estimate != 0 && estimate != total)
        {
            std::ostringstream os;
            os << "DataHandle::saveInto got " << total << " bytes out of " << estimate;
            throw ReadError(name() + " into " + other.name() + " " + os.str());
        }

        return total;
    }
}

Length DataHandle::saveInto(const PathName& path,TransferWatcher& w)
{
    std::auto_ptr<DataHandle> file(path.fileHandle());
    return saveInto(*file,w);
}

std::string DataHandle::name() const
{ 
    std::ostringstream s;
    s << *this;
    return s.str();
}

std::string DataHandle::title() const
{
	return className();
}

#ifndef IBM
template<>
Streamable* Reanimator<DataHandle>::ressucitate(Stream& s) const 
{ 
    return 0; 
}
#endif


bool DataHandle::compare(DataHandle& other)
{

    long bufsize = Resource<long>("compareBufferSize",10*1024*1024);

    Buffer buffer1(bufsize);
    Buffer buffer2(bufsize);

    DataHandle& self = *this;

    Length estimate1 = self.openForRead();  AutoClose closer1(self);
    Length estimate2 = other.openForRead(); AutoClose closer2(other);

    if(estimate1 != estimate2) {
        Log::error() << "DataHandle::compare(" << self << "," << other <<") failed: openForRead() returns " <<
            estimate1 << " and " << estimate2 << std::endl;
        return false;
    }

    Log::status() << "Comparing data" << std::endl;

    Progress progress("Comparing data",0,estimate1);
    Length total = 0;

    for(;;)
    {
        long len1 = self.read(buffer1,buffer1.size());
        long len2 = other.read(buffer2,buffer2.size());

        if(len1 != len2) {
            Log::error() << "DataHandle::compare(" << self << "," << other <<") failed: read() returns " <<
                len1 << " and " << len2 << std::endl;
            return false;
        }

        if(len1 <= 0 && len2 <= 0) 
        {
            Log::info() << "DataHandle::compare(" << self << "," << other <<") is successful" << std::endl;
            return true;
        }

        total += len1;
        progress(total);

        if(::memcmp(buffer1,buffer2,len1))
        {
            Log::error() << "DataHandle::compare(" << self << "," << other <<") failed: memcmp() returns non-zero value" << std::endl;
            return false;
        }

    }

    return false; // keep linter happy
}

Offset DataHandle::position() {
    Log::error() << *this << std::endl;
    NOTIMP;
}

void DataHandle::rewind() {
    Log::error() << *this << std::endl;
    std::ostringstream os;
    os << "NOT IMPLEMENTED DataHandle::rewind(" << *this << ")";
    throw SeriousBug(os.str());
    //NOTIMP;
}

Offset DataHandle::seek(const Offset& from)
{
    Log::error() << *this << std::endl;
    NOTIMP;
}

void DataHandle::skip(const Length& len)
{
    seek(position() + len);
}


void DataHandle::restartReadFrom(const Offset& from)
{
    Log::error() << *this << std::endl;
    NOTIMP;
}

void DataHandle::restartWriteFrom(const Offset&)
{
    Log::error() << *this << std::endl;
    NOTIMP;
}

void DataHandle::toLocal(Stream& s) const
{
    s << *this;
}

DataHandle* DataHandle::toLocal()
{
    return this;
}

void DataHandle::toRemote(Stream& s) const 
{
    s << *this;
}

void DataHandle::cost(std::map<std::string,Length>& c, bool read) const
{
}

DataHandle* DataHandle::clone() const
{
    Log::error() << *this << std::endl;
    NOTIMP;
}
//-----------------------------------------------------------------------------

} // namespace eckit
