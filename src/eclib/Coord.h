/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File Coord.h
// Claude  Gibert - ECMWF Dec 97

#ifndef Coord_H
#define Coord_H

#include "eclib/Stream.h"

class DumpLoad;

//---------------------------------------------------------
// Coord
//---------------------------------------------------------
class Coord {
public:
	
	friend ostream& operator<<(ostream& s,const Coord& x)
		{ x.print(s); return s; }

	friend Stream& operator>>(Stream& s,Coord& x)
		{ return s >> x.value_; }

// -- Contructors

	Coord(double l = 0) : value_(l) {}
	Coord(const Coord& other) : value_(other.value_) {}


// -- Operators

	Coord& operator=(const Coord& other) 
		{ value_ = other.value_; return *this;}

	Coord operator+(const Coord& other) const
		{ return Coord(value_ + other.value_);}

	Coord& operator+=(const Coord& other) 
		{ value_ += other.value_; return *this;}

	bool operator==(const Coord& other) const
		{ return value_ == other.value_; }

	bool operator<(const Coord& other) const
		{ return value_ < other.value_; }

	bool operator<=(const Coord& other) const
		{ return value_ <= other.value_; }

	bool operator>(const Coord& other) const
		{ return value_ > other.value_; }

	bool operator>=(const Coord& other) const
		{ return value_ >= other.value_; }

// -- 
#if 0
	double operator-(const Coord& other) const
		{ return value_ - other.value_;}
#endif

// -- Methods

	void print(ostream& s) const { s << value_; }

	void dump(DumpLoad&) const;
	void load(DumpLoad&);

// -- Class methods

	double value() { return value_; }

protected:


// -- Members

	double value_;


	friend class Area;
};

typedef vector<Coord> CoordList;

#endif

//---------------------------------------------------------
// Longitude
//---------------------------------------------------------
#ifndef Longitude_H
#define Longitude_H

class Longitude: public Coord {
public:
	
// -- Contructors

	Longitude(double l = 0);

};

#endif

//---------------------------------------------------------
// Latitude
//---------------------------------------------------------
#ifndef Latitude_H
#define Latitude_H

class Latitude: public Coord {
public:
	
// -- Contructors

	Latitude(double l = 0);

};

#endif

//---------------------------------------------------------
// Point
//---------------------------------------------------------
#ifndef Point_H
#define Point_H

class Point {

public:
	Point(const Latitude& lat, const Longitude& longi):
			lat_(lat), long_(longi) {}

	const Latitude& latitude() const { return lat_; }
	const Longitude& longitude() const { return long_; }

private:

	Latitude	lat_;
	Longitude	long_;

	friend class Area;

};

#endif
