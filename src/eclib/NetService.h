/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File NetService.h
// Baudouin Raoult - ECMWF Jun 96

#ifndef NetService_H
#define NetService_H

#include "eclib/TCPServer.h"
#include "eclib/Thread.h"

class NetUser;

class NetService : public Thread {
public:

// -- Contructors

	NetService(int);

// -- Destructor

	~NetService();

// -- Methods

	int port() const { return server_.localPort(); }

// -- Overriden methods

	// From Task

	virtual void run();

private:

// No copy allowed

	NetService(const NetService&);
	NetService& operator=(const NetService&);

// -- Members

	TCPServer server_;

// -- Methods

	virtual NetUser* newUser(TCPSocket&) = 0;	
	virtual string   name() = 0;



};

#endif

