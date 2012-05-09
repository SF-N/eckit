/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "eclib/Exceptions.h"
#include "eclib/ThreadSingleton.h"

template<class T> pthread_once_t ThreadSingleton<T>::once_ = PTHREAD_ONCE_INIT;
template<class T> pthread_key_t ThreadSingleton<T>::key_;

template<class T> ThreadSingleton<T>::ThreadSingleton()
{
}

template<class T> ThreadSingleton<T>::~ThreadSingleton()
{
}

template<class T> 
T& ThreadSingleton<T>::instance()
{
	pthread_once(&once_,init);

	T* value = (T*)pthread_getspecific(key_);
	if(!value)
	{

		value = new T();
		THRCALL(pthread_setspecific(key_,value));
	}
	return *value;
}

template<class T> void ThreadSingleton<T>::cleanUp(void* data)
{
	delete (T*)data;
	pthread_setspecific(key_,0);
}

template<class T> void ThreadSingleton<T>::init()
{
	pthread_key_create(&key_,cleanUp);
}
