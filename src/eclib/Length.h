/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File Length.h
// Baudouin Raoult - ECMWF Jul 96

#ifndef Length_H
#define Length_H

#include "eclib/Stream.h"

class Bless;
class DumpLoad;


// But because the compiler aligns long longs
// on 64bits boundaries and longs on 32 bits boundaries,
// we need the help of a little pragma here, to make ObjectStore happy

#ifdef AIX
#pragma options align=twobyte
#endif

class Length {
public:
	
	friend ostream& operator<<(ostream& s,const Length& x)
		{ return s << x.value_; }

	friend Stream& operator<<(Stream& s,const Length& x)
		{ return s << x.value_; }

	friend Stream& operator>>(Stream& s,Length& x)
		{ return s >> x.value_; }

// -- Contructors

	Length(long long l = 0) : value_(l) {}
	Length(const Length& other) : value_(other.value_) {}

#include "eclib/Length.b"

// -- Operators

	Length& operator=(const Length& other) 
		{ value_ = other.value_; return *this;}

	Length operator+(const Length& other) const
		{ return Length(value_ + other.value_);}

	Length& operator+=(const Length& other) 
		{ value_ += other.value_; return *this;}

	bool operator==(const Length& other) const
		{ return value_ == other.value_; }

	bool operator<(const Length& other) const
		{ return value_ < other.value_; }

	operator long long() const { return value_; }

// -- 
	long long operator-(const Length& other) const
		{ return value_ - other.value_;}

	void dump(DumpLoad&) const;
	void load(DumpLoad&);

// -- Class methods


private:


// -- Members

	long long value_;
	
	friend class Offset;
};

typedef vector<Length> LengthList;


#ifdef AIX
#pragma options align=reset
#endif


#endif
