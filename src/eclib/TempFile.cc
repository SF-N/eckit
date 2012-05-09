/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */


#include "eclib/FileSpace.h"
#include "eclib/TempFile.h"

TempFile::TempFile():
	PathName(dir("temp"))
{
}

TempFile::TempFile(const string& name):
	PathName(dir(name))
{
}

TempFile::~TempFile()
{
	unlink();
}

PathName TempFile::dir(const string& s)
{
	return PathName::unique(FileSpace::lookUp(s).selectFileSystem()+ "/tmp");
}
