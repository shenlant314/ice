// **********************************************************************
//
// Copyright (c) 2003-2005 ZeroC, Inc. All rights reserved.
//
// This copy of Ice-E is licensed to you under the terms described in the
// ICEE_LICENSE file included in this distribution.
//
// **********************************************************************

#include <IceE/Proxy.h>
#include <IceE/ProxyFactory.h>
#include <IceE/Outgoing.h>
#include <IceE/Connection.h>
#include <IceE/Reference.h>
#include <IceE/Instance.h>
#include <IceE/BasicStream.h>
#include <IceE/LocalException.h>
#ifdef ICEE_HAS_ROUTER
#   include <IceE/RouterInfo.h>
#endif

using namespace std;
using namespace Ice;
using namespace IceInternal;

void IceInternal::incRef(::IceProxy::Ice::Object* p) { p->__incRef(); }
void IceInternal::decRef(::IceProxy::Ice::Object* p) { p->__decRef(); }

void
Ice::__write(::IceInternal::BasicStream* __os, const ::Ice::Context& v, ::Ice::__U__Context)
{
    __os->writeSize(::Ice::Int(v.size()));
    ::Ice::Context::const_iterator p;
    for(p = v.begin(); p != v.end(); ++p)
    {
	__os->write(p->first);
	__os->write(p->second);
    }
}

void
Ice::__read(::IceInternal::BasicStream* __is, ::Ice::Context& v, ::Ice::__U__Context)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    while(sz--)
    {
	::std::pair<const  ::std::string, ::std::string> pair;
	__is->read(const_cast< ::std::string&>(pair.first));
	::Ice::Context::iterator __i = v.insert(v.end(), pair);
	__is->read(__i->second);
    }
}

::Ice::ObjectPrx
IceInternal::checkedCastImpl(const ObjectPrx& b, const string& f, const string& typeId)
{
//
// COMPILERBUG: Without this work-around, release VC7.0 and VC7.1
// build crash when FacetNotExistException is raised
//
#if defined(_MSC_VER) && (_MSC_VER >= 1300) && (_MSC_VER <= 1310)
    ObjectPrx fooBar;
#endif

    if(b)
    {
	ObjectPrx bb = b->ice_newFacet(f);
	try
	{
	    if(bb->ice_isA(typeId))
	    {
		return bb;
	    }
#ifndef NDEBUG
	    else
	    {
		assert(typeId != "::Ice::Object");
	    }
#endif
	}
	catch(const FacetNotExistException&)
	{
	}
    }
    return 0;
}

::Ice::ObjectPrx
IceInternal::checkedCastImpl(const ObjectPrx& b, const string& f, const string& typeId, const Context& ctx)
{
//
// COMPILERBUG: Without this work-around, release VC7.0 build crash
// when FacetNotExistException is raised
//
#if defined(_MSC_VER) && (_MSC_VER == 1300)
    ObjectPrx fooBar;
#endif

    if(b)
    {
	ObjectPrx bb = b->ice_newFacet(f);
	try
	{
	    if(bb->ice_isA(typeId, ctx))
	    {
		return bb;
	    }
#ifndef NDEBUG
	    else
	    {
		assert(typeId != "::Ice::Object");
	    }
#endif
	}
	catch(const FacetNotExistException&)
	{
	}
    }
    return 0;
}

bool
IceProxy::Ice::Object::operator==(const Object& r) const
{
    return _reference == r._reference;
}

bool
IceProxy::Ice::Object::operator!=(const Object& r) const
{
    return _reference != r._reference;
}

bool
IceProxy::Ice::Object::operator<(const Object& r) const
{
    return _reference < r._reference;
}

Int
IceProxy::Ice::Object::ice_hash() const
{
    return _reference->hash();
}

CommunicatorPtr
IceProxy::Ice::Object::ice_communicator() const
{
    return _reference->getCommunicator();
}

string
IceProxy::Ice::Object::ice_toString() const
{
    return _reference->toString();
}

bool
IceProxy::Ice::Object::ice_isA(const string& __id)
{
    return ice_isA(__id, _reference->getContext());
}

bool
IceProxy::Ice::Object::ice_isA(const string& __id, const Context& __context)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly("ice_isA");
	    static const string __operation("ice_isA");
	    __checkConnection();
#ifdef ICEE_BLOCKING_CLIENT
#  ifndef ICEE_PURE_BLOCKING_CLIENT
            if(_connection->blocking())
#  endif
            {
                Outgoing __og(_connection.get(), _reference.get(), __operation, ::Ice::Nonmutating, __context);
                return __ice_isA(__id, __og);
            }
#  ifndef ICEE_PURE_BLOCKING_CLIENT
            else
#  endif
#endif
#ifndef ICEE_PURE_BLOCKING_CLIENT
            {
                OutgoingM __og(_connection.get(), _reference.get(), __operation, ::Ice::Nonmutating, __context);
                return __ice_isA(__id, __og);
            }
#endif
	}
	catch(const NonRepeatable& __ex)
	{
	    __handleException(*__ex.get(), __cnt);
	}
	catch(const LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
#if defined(_MSC_VER) && (_MSC_VER == 1201) && defined(_M_ARM) // EVC4 SP4 bug.
	catch(...)
	{
	    throw;
	}
#endif
    }
}

bool
IceProxy::Ice::Object::__ice_isA(const string& __id, Outgoing& __og)
{
    try
    {
        BasicStream* __os = __og.os();
        __os->write(__id);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ret;
    bool __ok = __og.invoke();
    try
    {
        BasicStream* __is = __og.is();
        if(!__ok)
        {
            __is->throwException();
        }
        __is->read(__ret);
    }
    catch(const ::Ice::UserException&)
    {
        throw ::Ice::UnknownUserException(__FILE__, __LINE__);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::NonRepeatable(__ex);
    }
#if defined(_MSC_VER) && (_MSC_VER == 1201) && defined(_M_ARM) // EVC4 SP4 bug.
    catch(...)
    {
        throw;
    }
#endif
    return __ret;
}

void
IceProxy::Ice::Object::ice_ping()
{
    ice_ping(_reference->getContext());
}

void
IceProxy::Ice::Object::ice_ping(const Context& __context)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
            static const string __operation("ice_ping");
	    __checkConnection();
#ifdef ICEE_BLOCKING_CLIENT
#  ifndef ICEE_PURE_BLOCKING_CLIENT
            if(_connection->blocking())
#  endif
            {
                Outgoing __og(_connection.get(), _reference.get(), __operation, ::Ice::Nonmutating, __context);
                __ice_ping(__og);
            }
#  ifndef ICEE_PURE_BLOCKING_CLIENT
            else
#  endif
#endif
#ifndef ICEE_PURE_BLOCKING_CLIENT
            {
                OutgoingM __og(_connection.get(), _reference.get(), __operation, ::Ice::Nonmutating, __context);
                __ice_ping(__og);
            }
#endif
	    return;
	}
	catch(const NonRepeatable& __ex)
	{
	    __handleException(*__ex.get(), __cnt);
	}
	catch(const LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
#if defined(_MSC_VER) && (_MSC_VER == 1201) && defined(_M_ARM) // EVC4 SP4 bug.
	catch(...)
	{
	    throw;
	}
#endif
    }
}

void
IceProxy::Ice::Object::__ice_ping(Outgoing& __og)
{
    bool __ok = __og.invoke();
    try
    {
        BasicStream* __is = __og.is();
        if(!__ok)
        {
            __is->throwException();
        }
    }
    catch(const ::Ice::UserException&)
    {
        throw ::Ice::UnknownUserException(__FILE__, __LINE__);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::NonRepeatable(__ex);
    }
#if defined(_MSC_VER) && (_MSC_VER == 1201) && defined(_M_ARM) // EVC4 SP4 bug.
    catch(...)
    {
        throw;
    }
#endif
}



vector<string>
IceProxy::Ice::Object::ice_ids()
{
    return ice_ids(_reference->getContext());
}

vector<string>
IceProxy::Ice::Object::ice_ids(const Context& __context)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly("ice_ids");
            static const string __operation("ice_ids");
	    __checkConnection();
#ifdef ICEE_BLOCKING_CLIENT
#  ifndef ICEE_PURE_BLOCKING_CLIENT
            if(_connection->blocking())
#  endif
            {
                Outgoing __og(_connection.get(), _reference.get(), __operation, ::Ice::Nonmutating, __context);
                return __ice_ids(__og);
            }
#  ifndef ICEE_PURE_BLOCKING_CLIENT
            else
#  endif
#endif
#ifndef ICEE_PURE_BLOCKING_CLIENT
            {
                OutgoingM __og(_connection.get(), _reference.get(), __operation, ::Ice::Nonmutating, __context);
                return __ice_ids(__og);
            }
#endif
	}
	catch(const NonRepeatable& __ex)
	{
	    __handleException(*__ex.get(), __cnt);
	}
	catch(const LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
#if defined(_MSC_VER) && (_MSC_VER == 1201) && defined(_M_ARM) // EVC4 SP4 bug.
	catch(...)
	{
	    throw;
	}
#endif
    }
}

vector<string>
IceProxy::Ice::Object::__ice_ids(Outgoing& __og)
{
    vector<string> __ret;
    bool __ok = __og.invoke();
    try
    {
        BasicStream* __is = __og.is();
        if(!__ok)
        {
            __is->throwException();
        }
        __is->read(__ret);
    }
    catch(const ::Ice::UserException&)
    {
        throw ::Ice::UnknownUserException(__FILE__, __LINE__);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::NonRepeatable(__ex);
    }
#if defined(_MSC_VER) && (_MSC_VER == 1201) && defined(_M_ARM) // EVC4 SP4 bug.
    catch(...)
    {
        throw;
    }
#endif
    return __ret;
}

string
IceProxy::Ice::Object::ice_id()
{
    return ice_id(_reference->getContext());
}

string
IceProxy::Ice::Object::ice_id(const Context& __context)
{
    int __cnt = 0;
    while(true)
    {
	try
	{
	    __checkTwowayOnly("ice_id");
            static const string __operation("ice_id");
	    __checkConnection();
#ifdef ICEE_BLOCKING_CLIENT
#ifndef ICEE_PURE_BLOCKING_CLIENT
            if(_connection->blocking())
#endif
            {
                Outgoing __og(_connection.get(), _reference.get(), __operation, ::Ice::Nonmutating, __context);
                return __ice_id(__og);
            }
#  ifndef ICEE_PURE_BLOCKING_CLIENT
            else
#  endif
#endif
#ifndef ICEE_PURE_BLOCKING_CLIENT
            {
                OutgoingM __og(_connection.get(), _reference.get(), __operation, ::Ice::Nonmutating, __context);
                return __ice_id(__og);
            }
#endif
	}
	catch(const NonRepeatable& __ex)
	{
	    __handleException(*__ex.get(), __cnt);
	}
	catch(const LocalException& __ex)
	{
	    __handleException(__ex, __cnt);
	}
#if defined(_MSC_VER) && (_MSC_VER == 1201) && defined(_M_ARM) // EVC4 SP4 bug.
	catch(...)
	{
	    throw;
	}
#endif
    }
}

string
IceProxy::Ice::Object::__ice_id(Outgoing& __og)
{
    string __ret;
    bool __ok = __og.invoke();
    try
    {
        BasicStream* __is = __og.is();
        if(!__ok)
        {
            __is->throwException();
        }
        __is->read(__ret);
    }
    catch(const ::Ice::UserException&)
    {
        throw ::Ice::UnknownUserException(__FILE__, __LINE__);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::NonRepeatable(__ex);
    }
#if defined(_MSC_VER) && (_MSC_VER == 1201) && defined(_M_ARM) // EVC4 SP4 bug.
    catch(...)
    {
        throw;
    }
#endif
    return __ret;
}


Context
IceProxy::Ice::Object::ice_getContext() const
{
    return _reference->getContext();
}

ObjectPrx
IceProxy::Ice::Object::ice_newContext(const Context& newContext) const
{
    ObjectPrx proxy(new ::IceProxy::Ice::Object());
    proxy->setup(_reference->changeContext(newContext));
    return proxy;
}

ObjectPrx
IceProxy::Ice::Object::ice_defaultContext() const
{
    ObjectPrx proxy(new ::IceProxy::Ice::Object());
    proxy->setup(_reference->defaultContext());
    return proxy;
}

Identity
IceProxy::Ice::Object::ice_getIdentity() const
{
    return _reference->getIdentity();
}

ObjectPrx
IceProxy::Ice::Object::ice_newIdentity(const Identity& newIdentity) const
{
    if(newIdentity == _reference->getIdentity())
    {
	return ObjectPrx(const_cast< ::IceProxy::Ice::Object*>(this));
    }
    else
    {
	ObjectPrx proxy(new ::IceProxy::Ice::Object());
	proxy->setup(_reference->changeIdentity(newIdentity));
	return proxy;
    }
}

const string&
IceProxy::Ice::Object::ice_getFacet() const
{
    return _reference->getFacet();
}

ObjectPrx
IceProxy::Ice::Object::ice_newFacet(const string& newFacet) const
{
    if(newFacet == _reference->getFacet())
    {
	return ObjectPrx(const_cast< ::IceProxy::Ice::Object*>(this));
    }
    else
    {
	ObjectPrx proxy(new ::IceProxy::Ice::Object());
	proxy->setup(_reference->changeFacet(newFacet));
	return proxy;
    }
}

ObjectPrx
IceProxy::Ice::Object::ice_twoway() const
{
    ReferencePtr ref = _reference->changeMode(Reference::ModeTwoway);
    if(ref == _reference)
    {
	return ObjectPrx(const_cast< ::IceProxy::Ice::Object*>(this));
    }
    else
    {
	ObjectPrx proxy(new ::IceProxy::Ice::Object());
	proxy->setup(ref);
	return proxy;
    }
}

bool
IceProxy::Ice::Object::ice_isTwoway() const
{
    return _reference->getMode() == Reference::ModeTwoway;
}

ObjectPrx
IceProxy::Ice::Object::ice_oneway() const
{
    ReferencePtr ref = _reference->changeMode(Reference::ModeOneway);
    if(ref == _reference)
    {
	return ObjectPrx(const_cast< ::IceProxy::Ice::Object*>(this));
    }
    else
    {
	ObjectPrx proxy(new ::IceProxy::Ice::Object());
	proxy->setup(ref);
	return proxy;
    }
}

bool
IceProxy::Ice::Object::ice_isOneway() const
{
    return _reference->getMode() == Reference::ModeOneway;
}

#ifdef ICEE_HAS_BATCH
ObjectPrx
IceProxy::Ice::Object::ice_batchOneway() const
{
    ReferencePtr ref = _reference->changeMode(Reference::ModeBatchOneway);
    if(ref == _reference)
    {
	return ObjectPrx(const_cast< ::IceProxy::Ice::Object*>(this));
    }
    else
    {
	ObjectPrx proxy(new ::IceProxy::Ice::Object());
	proxy->setup(ref);
	return proxy;
    }
}

bool
IceProxy::Ice::Object::ice_isBatchOneway() const
{
    return _reference->getMode() == Reference::ModeBatchOneway;
}
#endif

ObjectPrx
IceProxy::Ice::Object::ice_timeout(int t) const
{
    ReferencePtr ref = _reference->changeTimeout(t);
    if(ref == _reference)
    {
	return ObjectPrx(const_cast< ::IceProxy::Ice::Object*>(this));
    }
    else
    {
	ObjectPrx proxy(new ::IceProxy::Ice::Object());
	proxy->setup(ref);
	return proxy;
    }
}

#ifdef ICEE_HAS_ROUTER

ObjectPrx
IceProxy::Ice::Object::ice_router(const RouterPrx& router) const
{
    ReferencePtr ref = _reference->changeRouter(router);
    if(ref == _reference)
    {
	return ObjectPrx(const_cast< ::IceProxy::Ice::Object*>(this));
    }
    else
    {
	ObjectPrx proxy(new ::IceProxy::Ice::Object());
	proxy->setup(ref);
	return proxy;
    }
}

#endif

#ifdef ICEE_HAS_LOCATOR

ObjectPrx
IceProxy::Ice::Object::ice_locator(const LocatorPrx& locator) const
{
    ReferencePtr ref = _reference->changeLocator(locator);
    if(ref == _reference)
    {
	return ObjectPrx(const_cast< ::IceProxy::Ice::Object*>(this));
    }
    else
    {
	ObjectPrx proxy(new ::IceProxy::Ice::Object());
	proxy->setup(ref);
	return proxy;
    }
}

#endif

ConnectionPtr
IceProxy::Ice::Object::ice_connection()
{
    return _connection;
}

ReferencePtr
IceProxy::Ice::Object::__reference() const
{
    return _reference;
}

void
IceProxy::Ice::Object::__copyFrom(const ObjectPrx& from)
{
    ReferencePtr ref;
    ConnectionPtr con;

    {
	::IceUtil::Mutex::Lock sync(*from.get());

	ref = from->_reference;
	con = from->_connection;
    }

    //
    // No need to synchronize "*this", as this operation is only
    // called upon initialization.
    //

    assert(!_reference);
    assert(!_connection);

    _reference = ref;
    _connection = con;
}

void
IceProxy::Ice::Object::__handleException(const LocalException& ex, int& cnt)
{
    //
    // Only _connection needs to be mutex protected here.
    //
    {
        ::IceUtil::Mutex::Lock sync(*this);
        _connection = 0;
    }

    ProxyFactoryPtr proxyFactory = _reference->getInstance()->proxyFactory();
    if(proxyFactory)
    {
	proxyFactory->checkRetryAfterException(ex, _reference, cnt);
    }
    else
    {
	//
	// The communicator is already destroyed, so we cannot retry.
	//
        ex.ice_throw();
    }
}

void
IceProxy::Ice::Object::__rethrowException(const LocalException& ex)
{
    //
    // Only _connection needs to be mutex protected here.
    //
    {
        ::IceUtil::Mutex::Lock sync(*this);
        _connection = 0;
    }

    ex.ice_throw();
}

void
IceProxy::Ice::Object::__checkTwowayOnly(const char* name) const
{
    //
    // No mutex lock necessary, there is nothing mutable in this
    // operation.
    //

    if(!ice_isTwoway())
    {
        TwowayOnlyException ex(__FILE__, __LINE__);
	ex.operation = name;
	throw ex;
    }
}

const Context&
IceProxy::Ice::Object::__defaultContext() const
{
    return _reference->getContext();
}

void
IceProxy::Ice::Object::__checkConnection() 
{
    ::IceUtil::Mutex::Lock sync(*this);

    if(!_connection)
    {
        _connection = _reference->getConnection();

        //
        // If this proxy is for a non-local object, and we are
        // using a router, then add this proxy to the router info
        // object.
        //
#ifdef ICEE_HAS_ROUTER
        RoutableReferencePtr rr = RoutableReferencePtr::dynamicCast(_reference);
        if(rr && rr->getRouterInfo())
        {
            rr->getRouterInfo()->addProxy(this);
        }
#endif
    }
}


void
IceProxy::Ice::Object::setup(const ReferencePtr& ref)
{
    //
    // No need to synchronize "*this", as this operation is only
    // called upon initialization.
    //

    assert(!_reference);
    assert(!_connection);

    _reference = ref;
}

bool
Ice::proxyIdentityLess(const ObjectPrx& lhs, const ObjectPrx& rhs)
{
    if(!lhs && !rhs)
    {
	return false;
    }
    else if(!lhs && rhs)
    {
	return true;
    }
    else if(lhs && !rhs)
    {
	return false;
    }
    else
    {
	return lhs->ice_getIdentity() < rhs->ice_getIdentity();
    }
}

bool
Ice::proxyIdentityEqual(const ObjectPrx& lhs, const ObjectPrx& rhs)
{
    if(!lhs && !rhs)
    {
	return true;
    }
    else if(!lhs && rhs)
    {
	return false;
    }
    else if(lhs && !rhs)
    {
	return false;
    }
    else
    {
	return lhs->ice_getIdentity() == rhs->ice_getIdentity();
    }
}

bool
Ice::proxyIdentityAndFacetLess(const ObjectPrx& lhs, const ObjectPrx& rhs)
{
    if(!lhs && !rhs)
    {
	return false;
    }
    else if(!lhs && rhs)
    {
	return true;
    }
    else if(lhs && !rhs)
    {
	return false;
    }
    else
    {
	Identity lhsIdentity = lhs->ice_getIdentity();
	Identity rhsIdentity = rhs->ice_getIdentity();
	
	if(lhsIdentity < rhsIdentity)
	{
	    return true;
	}
	else if(rhsIdentity < lhsIdentity)
	{
	    return false;
	}
	
	string lhsFacet = lhs->ice_getFacet();
	string rhsFacet = rhs->ice_getFacet();
	
	if(lhsFacet < rhsFacet)
	{
	    return true;
	}
	else if(rhsFacet < lhsFacet)
	{
	    return false;
	}
	
	return false;
    }
}

bool
Ice::proxyIdentityAndFacetEqual(const ObjectPrx& lhs, const ObjectPrx& rhs)
{
    if(!lhs && !rhs)
    {
	return true;
    }
    else if(!lhs && rhs)
    {
	return false;
    }
    else if(lhs && !rhs)
    {
	return false;
    }
    else
    {
	Identity lhsIdentity = lhs->ice_getIdentity();
	Identity rhsIdentity = rhs->ice_getIdentity();
	
	if(lhsIdentity == rhsIdentity)
	{
	    string lhsFacet = lhs->ice_getFacet();
	    string rhsFacet = rhs->ice_getFacet();
	    
	    if(lhsFacet == rhsFacet)
	    {
		return true;
	    }
	}
	
	return false;
    }
}
