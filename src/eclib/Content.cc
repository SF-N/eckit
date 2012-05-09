/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */


#include "eclib/Content.h"
#include "eclib/StrStream.h"
#include "eclib/Value.h"

// =============================================================

class BadConversion:  public Exception { 
	public: 
		BadConversion(const string& w):
			Exception(string("Bad Conversion: ") + w)   {  }
};

class BadComparison:  public Exception { 
	public: 	
		BadComparison(const string& w):
			Exception(string("Bad Comparison: ") + w)   {  }
};

class BadOperator:  public Exception { 
	public: 	
		BadOperator(const string& w):
			Exception(string("Bad operator: ") + w)   {  }
};

// =============================================================

ClassSpec Content::classSpec_ = {&Streamable::classSpec(),"Content",};
Reanimator<Content> Content::reanimator_;

void Content::encode(Stream& s) const
{
	Streamable::encode(s);
}

Content::Content(Stream& s):
	Streamable(s)
{
}

Content::~Content()
{
}

void Content::badConvertion(const string& to) const
{
	StrStream s;
	s << "Cannot convert " << *this << " (" << typeName() << ") to " << to << StrStream::ends;
	string str(s);
	throw BadConversion(str);
}

void Content::badComparison(const string& to) const
{
	StrStream s;
	s << "Cannot compare " << *this << " (" << typeName() << ") with " << to << StrStream::ends;
	string str(s);
	throw BadComparison(str);
}

void Content::badOperator(const string& op, const string& to) const
{
	StrStream s;
	s << *this << " (" << typeName() << ") " << op << " " << to << StrStream::ends;
	string str(s);
	throw BadOperator(str);
}

void Content::value(long long&) const 
{ 
	badConvertion("long long");
}

void Content::value(string&) const
{
	badConvertion("string");
}

void Content::value(Date&) const
{
	badConvertion("Date");
}

void Content::value(Time&) const
{
	badConvertion("Time");
}

void Content::value(DateTime&) const
{
	badConvertion("DateTime");
}

void Content::value(vector<Value>& v) const
{
	// Cast away constness, so the Contnt can be attached by the value
	v.push_back(Value((Content*)this));
}

bool Content::operator==(const Content& other) const
{
	Log::debug() << *this << " == " << other << endl;
	return (this->compare(other) == 0);
}

bool Content::operator<(const Content& other) const
{
	return (this->compare(other) < 0);
}

int Content::compareNumber(const NumberContent&) const
{
	badComparison("Number");
	return 0;
}

int Content::compareString(const StringContent&) const
{
	badComparison("String");
	return 0;
}

int Content::compareNil(const NilContent&) const
{
	badComparison("Nil");
	return 0;
}

int Content::compareList(const ListContent&) const
{
	badComparison("List");
	return 0;
}

int Content::compareDate(const DateContent&) const
{
	badComparison("Date");
	return 0;
}

int Content::compareTime(const TimeContent&) const
{
	badComparison("Time");
	return 0;
}

int Content::compareDateTime(const DateTimeContent&) const
{
	badComparison("DateTime");
	return 0;
}

Content* Content::operator+(const Content& other) const
{
	return add(other);
}

Content* Content::add(const Content& other) const
{
	badOperator("+",other.typeName());
	return 0;
}

Content* Content::addNumber(const NumberContent&) const
{
	badOperator("+","Number");
	return 0;
}

Content* Content::addString(const StringContent&) const
{
	badOperator("+","String");
	return 0;
}

Content* Content::addNil(const NilContent&) const
{
	badOperator("+","Nil");
	return 0;
}

Content* Content::addList(const ListContent&) const
{
	badOperator("+","List");
	return 0;
}

Content* Content::addDate(const DateContent&) const
{
	badOperator("+","Date");
	return 0;
}

Content* Content::addTime(const TimeContent&) const
{
	badOperator("+","Time");
	return 0;
}

Content* Content::addDateTime(const DateTimeContent&) const
{
	badOperator("+","DateTime");
	return 0;
}

Content* Content::operator-(const Content& other) const
{
	return sub(other);
}

Content* Content::sub(const Content& other) const
{
	badOperator("-",other.typeName());
	return 0;
}

Content* Content::subNumber(const NumberContent&) const
{
	badOperator("-","Number");
	return 0;
}

Content* Content::subString(const StringContent&) const
{
	badOperator("-","String");
	return 0;
}

Content* Content::subNil(const NilContent&) const
{
	badOperator("-","Nil");
	return 0;
}

Content* Content::subList(const ListContent&) const
{
	badOperator("-","List");
	return 0;
}

Content* Content::subDate(const DateContent&) const
{
	badOperator("-","Date");
	return 0;
}

Content* Content::subTime(const TimeContent&) const
{
	badOperator("-","Time");
	return 0;
}

Content* Content::subDateTime(const DateTimeContent&) const
{
	badOperator("-","DateTime");
	return 0;
}

Content* Content::operator*(const Content& other) const
{
	return mul(other);
}

Content* Content::mul(const Content& other) const
{
	badOperator("*",other.typeName());
	return 0;
}

Content* Content::mulNumber(const NumberContent&) const
{
	badOperator("*","Number");
	return 0;
}

Content* Content::mulString(const StringContent&) const
{
	badOperator("*","String");
	return 0;
}

Content* Content::mulNil(const NilContent&) const
{
	badOperator("*","Nil");
	return 0;
}

Content* Content::mulList(const ListContent&) const
{
	badOperator("*","List");
	return 0;
}

Content* Content::mulDate(const DateContent&) const
{
	badOperator("*","Date");
	return 0;
}

Content* Content::mulTime(const TimeContent&) const
{
	badOperator("*","Time");
	return 0;
}

Content* Content::mulDateTime(const DateTimeContent&) const
{
	badOperator("*","DateTime");
	return 0;
}

Content* Content::operator/(const Content& other) const
{
	return div(other);
}

Content* Content::div(const Content& other) const
{
	badOperator("/",other.typeName());
	return 0;
}

Content* Content::divNumber(const NumberContent&) const
{
	badOperator("/","Number");
	return 0;
}

Content* Content::divString(const StringContent&) const
{
	badOperator("/","String");
	return 0;
}

Content* Content::divNil(const NilContent&) const
{
	badOperator("/","Nil");
	return 0;
}

Content* Content::divList(const ListContent&) const
{
	badOperator("/","List");
	return 0;
}

Content* Content::divDate(const DateContent&) const
{
	badOperator("/","Date");
	return 0;
}

Content* Content::divTime(const TimeContent&) const
{
	badOperator("/","Time");
	return 0;
}

Content* Content::divDateTime(const DateTimeContent&) const
{
	badOperator("/","DateTime");
	return 0;
}

#ifndef IBM
template<>
Streamable* Reanimator<Content>::ressucitate(Stream& s) const 
{ 
	return 0; 
}
#endif
