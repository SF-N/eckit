/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File NumberContent.h
// Manuel Fuentes - ECMWF Jun 97

#ifndef NumberContent_H
#define NumberContent_H

#include "eclib/Content.h"
#include "eclib/Value.h"

class NumberContent : public Content {

protected:

// -- Constructor

	NumberContent(long long);
	NumberContent(Stream&);

// -- Destructor

	virtual ~NumberContent();

// -- Overridden methods

	// -- From Content
	virtual void value(long long&) const;
	virtual void value(string& n) const;
	virtual void value(vector<Value>& n) const { Content::value(n); }
	virtual void value(Date& n) const          { Content::value(n); }
	virtual void value(Time& n) const          { Content::value(n); }
	virtual void value(DateTime& n) const      { Content::value(n); }

	virtual int compare(const Content&)             const;

	virtual int  compareNil(const NilContent&) const       { return  1; }
    virtual int  compareNumber(const NumberContent&) const;
	virtual int  compareString(const StringContent&) const { return  -1; }
	virtual int  compareList(const ListContent&) const     { return  -1; }
	virtual int  compareTime(const TimeContent&) const     { return  -1; }
	virtual int  compareDate(const DateContent&) const     { return  -1; }
	virtual int  compareDateTime(const DateTimeContent&) const { return  -1; }

	virtual Content* add(const Content&)             const;
	virtual Content* sub(const Content&) const;
	virtual Content* mul(const Content&) const;
	virtual Content* div(const Content&) const;

	virtual Content* addNumber(const NumberContent&) const;
	virtual Content* subNumber(const NumberContent&) const;
	virtual Content* mulNumber(const NumberContent&) const;
	virtual Content* divNumber(const NumberContent&) const;

	virtual void    print(ostream&) const;
	virtual string  typeName()      const { return "Number"; }
	virtual bool    isNumber()      const { return true; }

    // -- From Streamable

    virtual void encode(Stream&) const;
    virtual const ReanimatorBase& reanimator() const { return reanimator_; }

// -- Class methods

    static  const ClassSpec&  classSpec()            { return classSpec_;}

private:

	NumberContent(const NumberContent&);
	NumberContent& operator=(const NumberContent&);

// -- Members

	long long num_;

// -- Class Members

	static  ClassSpec                  classSpec_;
    static  Reanimator<NumberContent>  reanimator_;

// -- Friends

	friend class Reanimator<NumberContent>;
	friend class Value;
	friend class DateContent;
};

#endif
