/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */


#include "eclib/MarsFSClient.h"
#include "eclib/MarsFSPath.h"
#include "eclib/ThreadSingleton.h"

//#define X(a) Log::userInfo() << #a << endl; Log::info() << #a << endl; 
#define X(a) /**/

// TODO: Return errno
//


class MarsFSClientSettings {
	bool forever_;
public:
	MarsFSClientSettings() :
		forever_(true)
	{
	}

	bool retry() const
	{
		return forever_;
	}

	void retry(bool on)
	{
		forever_ = on;
	}
};

static ThreadSingleton<MarsFSClientSettings> settings;

inline static bool retry()
{
	return settings.instance().retry();
}

MarsFSClientRetry::MarsFSClientRetry(bool on) :
	old_(settings.instance().retry())
{
	settings.instance().retry(on);
}

MarsFSClientRetry::~MarsFSClientRetry()
{
	settings.instance().retry(old_);
}

MarsFSClient::MarsFSClient(const MarsFSPath& path) :
	connector_(Connector::service("marsfs", path.node()))
{
}

MarsFSClient::~MarsFSClient()
{
}

Length MarsFSClient::size(const string& path)
{
	static const char *here = __FUNCTION__;
	X(MarsFSClient::size);

	for(;;)
	{

		try
		{
			AutoMemoize m(connector_, 1);

			Stream& s = connector_;
			unsigned long long len;
			s << "size";
			s << path;
			s >> len;
			return len;
		}
		catch(ConnectorException& e)
		{
			if(!retry())
				throw;
			Log::error() << "** " << e.what() << " Caught in " << here << endl;
			Log::error() << "** Exception is ignored" << endl;
		}
	}
}

bool MarsFSClient::exists(const string& path)
{
	static const char *here = __FUNCTION__;

	X(MarsFSClient::exit);

	for(;;)
	{

		try
		{
			//AutoMemoize m(connector_, 10);

			bool ok;
			Stream& s = connector_;

			s << "exists";
			s << path;
			s >> ok;
			return ok;
		}
		catch(ConnectorException& e)
		{
			if(!retry())
				throw;
			Log::error() << "** " << e.what() << " Caught in " << here << endl;
			Log::error() << "** Exception is ignored" << endl;
		}
	}
}

string MarsFSClient::mountPoint(const string& path)
{
	static const char *here = __FUNCTION__;
	X(MarsFSClient::mountPoint);

	for(;;)
	{

		try
		{
			AutoMemoize m(connector_, 60);

			string p;
			Stream& s = connector_;
			s << "mountp";
			s << path;
			s >> p;
			return p;
		}
		catch(ConnectorException& e)
		{
			if(!retry())
				throw;
			Log::error() << "** " << e.what() << " Caught in " << here << endl;
			Log::error() << "** Exception is ignored" << endl;
		}
	}
}

string MarsFSClient::baseName(const string& path, bool ext)
{
	static const char *here = __FUNCTION__;
	X(MarsFSClient::baseName);

	for(;;)
	{

		try
		{
			AutoMemoize m(connector_, 60);

			string b;
			Stream& s = connector_;
			s << "baseName";
			s << path;
			s << ext;
			s >> b;
			return b;
		}
		catch(ConnectorException& e)
		{
			if(!retry())
				throw;
			Log::error() << "** " << e.what() << " Caught in " << here << endl;
			Log::error() << "** Exception is ignored" << endl;
		}
	}
}

string MarsFSClient::dirName(const string& path)
{
	static const char *here = __FUNCTION__;
	X(MarsFSClient::dirName);

	for(;;)
	{

		try
		{
			AutoMemoize m(connector_, 60);

			string d;
			Stream& s = connector_;
			s << "dirName";
			s << path;
			s >> d;
			return d;
		}
		catch(ConnectorException& e)
		{
			if(!retry())
				throw;
			Log::error() << "** " << e.what() << " Caught in " << here << endl;
			Log::error() << "** Exception is ignored" << endl;
		}
	}
}

string MarsFSClient::fullName(const string& path)
{
	static const char *here = __FUNCTION__;
	X(MarsFSClient::fullName);

	for(;;)
	{

		try
		{
			AutoMemoize m(connector_, 60);

			string f;
			Stream& s = connector_;
			s << "fullName";
			s << path;
			s >> f;
			return f;
		}
		catch(ConnectorException& e)
		{
			if(!retry())
				throw;
			Log::error() << "** " << e.what() << " Caught in " << here << endl;
			Log::error() << "** Exception is ignored" << endl;
		}
	}
}

string MarsFSClient::unique(const string& path)
{
	static const char *here = __FUNCTION__;
	X(MarsFSClient::unique);

	for(;;)
	{

		try
		{
			string u;
			Stream& s = connector_;
			s << "unique";
			s << path;
			s >> u;
			return u;
		}
		catch(ConnectorException& e)
		{
			if(!retry())
				throw;
			Log::error() << "** " << e.what() << " Caught in " << here << endl;
			Log::error() << "** Exception is ignored" << endl;
		}
	}
}

bool MarsFSClient::sameAs(const string& path1, const string& path2)
{
	static const char *here = __FUNCTION__;
	X(MarsFSClient::sameAs);

	for(;;)
	{

		try
		{
			AutoMemoize m(connector_, 10);

			bool ok;
			Stream& s = connector_;
			s << "sameas";
			s << path1;
			s << path2;
			s >> ok;
			return ok;
		}
		catch(ConnectorException& e)
		{
			if(!retry())
				throw;
			Log::error() << "** " << e.what() << " Caught in " << here << endl;
			Log::error() << "** Exception is ignored" << endl;
		}
	}
}

void MarsFSClient::mkdir(const string& path, short mode)
{
	static const char *here = __FUNCTION__;
	X(MarsFSClient::mkdir);

	for(;;)
	{

		try
		{
			bool ok;
			Stream& s = connector_;
			s << "mkdir";
			s << path;
			s << mode;
			s >> ok;
			return;
		}
		catch(ConnectorException& e)
		{
			if(!retry())
				throw;
			Log::error() << "** " << e.what() << " Caught in " << here << endl;
			Log::error() << "** Exception is ignored" << endl;
		}
	}
}

void MarsFSClient::match(const string& path, vector<string>& result, bool recurse)
{
	static const char *here = __FUNCTION__;
	X(MarsFSClient::match);

	for(;;)
	{

		try
		{
			AutoMemoize m(connector_, 2);

			Stream& s = connector_;
			s << "match";
			s << path;
			s << recurse;

			result.clear();

			long n;
			s >> n;

			string r;

			for(long i = 0; i < n; i++)
			{
				s >> r;
				result.push_back(r);
			}
			return;
		}
		catch(ConnectorException& e)
		{
			if(!retry())
				throw;
			Log::error() << "** " << e.what() << " Caught in " << here << endl;
			Log::error() << "** Exception is ignored" << endl;
		}
	}
}

void MarsFSClient::children(const string& path, vector<string>& dirs, vector<string>& files)
{
	static const char *here = __FUNCTION__;
	X(MarsFSClient::children);

	for(;;)
	{

		try
		{
			AutoMemoize m(connector_, 2);

			Stream& s = connector_;
			s << "children";
			s << path;

			dirs.clear();

			long n;
			s >> n;

			string r;

			for(long i = 0; i < n; i++)
			{
				s >> r;
				dirs.push_back(r);
			}

			files.clear();

			s >> n;

			for(long i = 0; i < n; i++)
			{
				s >> r;
				files.push_back(r);
			}

			return;
		}
		catch(ConnectorException& e)
		{
			if(!retry())
				throw;
			Log::error() << "** " << e.what() << " Caught in " << here << endl;
			Log::error() << "** Exception is ignored" << endl;
		}
	}
}

vector<string> MarsFSClient::getFileSpaces()
{
	static const char *here = __FUNCTION__;
	X(MarsFSClient::getFileSpaces);
	for(;;)
	{

		try
		{

			AutoMemoize m(connector_, 20);

			Stream& s = connector_;
			s << "getFileSpaces";

			vector<string> result;

			long n;
			s >> n;

			string r;

			for(long i = 0; i < n; i++)
			{
				s >> r;
				result.push_back(r);
			}

			return result;
		}
		catch(ConnectorException& e)
		{
			if(!retry())
				throw;
			Log::error() << "** " << e.what() << " Caught in " << here << endl;
			Log::error() << "** Exception is ignored" << endl;
		}
	}
}

vector<string> MarsFSClient::getFileSystems(const string& name)
{
	static const char *here = __FUNCTION__;
	X(MarsFSClient::getFileSystems);

	for(;;)
	{

		try
		{
			AutoMemoize m(connector_, 20);

			Stream& s = connector_;
			s << "getFileSystems";
			s << name;

			vector<string> result;

			long n;
			s >> n;

			string r;

			for(long i = 0; i < n; i++)
			{
				s >> r;
				result.push_back(r);
			}

			return result;
		}
		catch(ConnectorException& e)
		{
			if(!retry())
				throw;
			Log::error() << "** " << e.what() << " Caught in " << here << endl;
			Log::error() << "** Exception is ignored" << endl;
		}
	}

}

void MarsFSClient::rename(const string& from, const string& to)
{
	static const char *here = __FUNCTION__;
	X(MarsFSClient::rename);
	for(;;)
	{

		try
		{
			bool ok;
			Stream& s = connector_;
			s << "rename";
			s << from;
			s << to;
			s >> ok;
			return;
		}
		catch(ConnectorException& e)
		{
			if(!retry())
				throw;
			Log::error() << "** " << e.what() << " Caught in " << here << endl;
			Log::error() << "** Exception is ignored" << endl;
		}
	}
}

void MarsFSClient::link(const string& from, const string& to)
{
	static const char *here = __FUNCTION__;
	X(MarsFSClient::link);
	for(;;)
	{

		try
		{

			bool ok;
			Stream& s = connector_;
			s << "link";
			s << from;
			s << to;
			s >> ok;
			return;
		}
		catch(ConnectorException& e)
		{
			if(!retry())
				throw;
			Log::error() << "** " << e.what() << " Caught in " << here << endl;
			Log::error() << "** Exception is ignored" << endl;
		}
	}
}

void MarsFSClient::unlink(const string& path)
{
	static const char *here = __FUNCTION__;
	X(MarsFSClient::unlink);
	for(;;)
	{

		try
		{

			bool ok;
			Stream& s = connector_;
			s << "unlink";
			s << path;
			s >> ok;
			return;
		}
		catch(ConnectorException& e)
		{
			if(!retry())
				throw;
			Log::error() << "** " << e.what() << " Caught in " << here << endl;
			Log::error() << "** Exception is ignored" << endl;
		}
	}
}

void MarsFSClient::rmdir(const string& path)
{
	static const char *here = __FUNCTION__;
	X(MarsFSClient::rmdir);
	for(;;)
	{

		try
		{

			bool ok;
			Stream& s = connector_;
			s << "rmdir";
			s << path;
			s >> ok;
			return;
		}
		catch(ConnectorException& e)
		{
			if(!retry())
				throw;
			Log::error() << "** " << e.what() << " Caught in " << here << endl;
			Log::error() << "** Exception is ignored" << endl;
		}
	}
}

void MarsFSClient::touch(const string& path)
{
	static const char *here = __FUNCTION__;
	X(MarsFSClient::touch);
	for(;;)
	{

		try
		{

			bool ok;
			Stream& s = connector_;
			s << "touch";
			s << path;
			s >> ok;
			return;
		}
		catch(ConnectorException& e)
		{
			if(!retry())
				throw;
			Log::error() << "** " << e.what() << " Caught in " << here << endl;
			Log::error() << "** Exception is ignored" << endl;
		}
	}
}

void MarsFSClient::fileSystemSize(const string& path, FileSystemSize& fs)
{
	static const char *here = __FUNCTION__;
	X(MarsFSClient::fileSystemSize);
	for(;;)
	{

		try
		{

			AutoMemoize m(connector_, 10);

			Stream& s = connector_;
			s << "statfs";
			s << path;

			s >> fs.available;
			s >> fs.total;
			return;
		}
		catch(ConnectorException& e)
		{
			if(!retry())
				throw;
			Log::error() << "** " << e.what() << " Caught in " << here << endl;
			Log::error() << "** Exception is ignored" << endl;
		}
	}
}

time_t MarsFSClient::created(const string& path)
{
	static const char *here = __FUNCTION__;
	X(MarsFSClient::created);
	for(;;)
	{

		try
		{

			AutoMemoize m(connector_, 60);

			Stream& s = connector_;
			unsigned long long len;
			s << "created";
			s << path;
			s >> len;
			return len;
		}
		catch(ConnectorException& e)
		{
			if(!retry())
				throw;
			Log::error() << "** " << e.what() << " Caught in " << here << endl;
			Log::error() << "** Exception is ignored" << endl;
		}
	}
}

time_t MarsFSClient::lastAccess(const string& path)
{
	static const char *here = __FUNCTION__;
	X(MarsFSClient::lastAccess);
	for(;;)
	{

		try
		{

			AutoMemoize m(connector_, 1);

			Stream& s = connector_;
			unsigned long long len;
			s << "lastAccess";
			s << path;
			s >> len;
			return len;
		}
		catch(ConnectorException& e)
		{
			if(!retry())
				throw;
			Log::error() << "** " << e.what() << " Caught in " << here << endl;
			Log::error() << "** Exception is ignored" << endl;
		}
	}
}

time_t MarsFSClient::lastModified(const string& path)
{
	static const char *here = __FUNCTION__;
	X(MarsFSClient::lastModified);
	for(;;)
	{

		try
		{

			AutoMemoize m(connector_, 1);

			Stream& s = connector_;
			unsigned long long len;
			s << "lastModified";
			s << path;
			s >> len;
			return len;
		}
		catch(ConnectorException& e)
		{
			if(!retry())
				throw;
			Log::error() << "** " << e.what() << " Caught in " << here << endl;
			Log::error() << "** Exception is ignored" << endl;
		}
	}
}
