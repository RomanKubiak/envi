/*
  ==============================================================================

    EnviDataStore.h
    Created: 5 Dec 2013 1:56:08pm
    Author:  rk09631

  ==============================================================================
*/

#ifndef ENVIDATASTORE_H_INCLUDED
#define ENVIDATASTORE_H_INCLUDED

#include "EnviIncludes.h"
#include "EnviDataSource.h"

class EnviDataStore
{
	public:
		EnviDataStore(EnviApplication &_owner) : owner(_owner) {}
		virtual ~EnviDataStore() {}
		virtual const Result openStore()														= 0;
		virtual const Result closeStore()														= 0;
        virtual const Result storeData(const EnviData &dataToStore)								= 0;
        virtual const Result rotate()															= 0;
		virtual const bool isValid()															= 0;
		virtual const Result flush()															= 0;
		virtual const Result registerSources()													{ return Result::ok(); }
	protected:
		EnviApplication &owner;
};


#endif  // ENVIDATASTORE_H_INCLUDED
