/***
    mockcpp is a generic C/C++ mock framework.
    Copyright (C) <2009>  <Darwin Yuan: darwin.yuan@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
***/

#include <mockcpp/mockcpp.h>

#include <mockcpp/HookMockObject.h>
#include <mockcpp/ChainableMockMethodCore.h>
#include <mockcpp/InvocationMockBuilderGetter.h>
#include <mockcpp/InvocationMockerNamespace.h>
#include <mockcpp/CApiHookKey.h>
#include <mockcpp/ReportFailure.h>

#include <iostream>

MOCKCPP_NS_START

//////////////////////////////////////////////////////////////
struct HookMockObjectImpl
{
   HookMockObjectImpl(ChainableMockMethodContainer* c)
      : container(c)
   {}

   ChainableMockMethodCore*
   getMethod(const std::string& name, const void* api, 
         const void* stub, ThunkCode *thunkTemplate, InvocationMockerNamespace* ns);
   
   ChainableMockMethodCore*
   getMethod(const void* api);
   
   void reset();

   ChainableMockMethodContainer* container;

private:

   ChainableMockMethodCore* 
   addMethod(const std::string& name, 
       const void* api, 
	   const void* stub,
       ThunkCode *thunkTemplate, 
	   InvocationMockerNamespace* ns); 

};

//////////////////////////////////////////////////////////////
void
HookMockObjectImpl::reset()
{
    container->reset();
}

//////////////////////////////////////////////////////////////
ChainableMockMethodCore*
HookMockObjectImpl::
addMethod(const std::string& name, 
    const void* api, 
	const void* stub,
    ThunkCode *thunkTemplate, 
	InvocationMockerNamespace* ns) 
{
    CApiHookKey* key = new CApiHookKey(api, stub, thunkTemplate);
    ChainableMockMethodCore* method = new ChainableMockMethodCore(name, ns);

    container->addMethod(key, method);

    return method;
}
//////////////////////////////////////////////////////////////
ChainableMockMethodCore*
HookMockObjectImpl::
getMethod(const std::string& name, const void* api
         , const void* stub
         , ThunkCode *thunkTemplate
         , InvocationMockerNamespace* ns)
{
    ChainableMockMethodCore* method = getMethod(api);
    if (method != 0)
    {
      return method;
    }

    return addMethod(name, api, stub, thunkTemplate, ns);
}

//////////////////////////////////////////////////////////////
ChainableMockMethodCore*
HookMockObjectImpl::
getMethod(const void* api) 
{
    CApiHookKey key(api);
    return container->getMethod(&key);
}

//////////////////////////////////////////////////////////////
HookMockObject::HookMockObject(const std::string& name)
   : ChainableMockObjectBase(name)
{
	This = new HookMockObjectImpl(this->getMethodContainer());
}

//////////////////////////////////////////////////////////////
HookMockObject::~HookMockObject()
{
   delete This;
}

//////////////////////////////////////////////////////////////
InvocationMockBuilderGetter
HookMockObject::method(const std::string& name, const void* api, const void* stub, ThunkCode *thunkTemplate)
{
    ChainableMockMethodCore* core = This->getMethod(name, api, stub, thunkTemplate, this);
    return InvocationMockBuilderGetter(core, core);
}

//////////////////////////////////////////////////////////////
// Private
Invokable*
HookMockObject::getInvokable(const void* api) 
{
   ChainableMockMethodCore* method = This->getMethod(api);
   MOCKCPP_ASSERT_TRUE(
	   "mockcpp internal error (3001): not able to get api pointer",
	   method != 0);
   return method;
}

//////////////////////////////////////////////////////////////
void HookMockObject::reset()
{
   This->reset();
}

//////////////////////////////////////////////////////////////

MOCKCPP_NS_END


