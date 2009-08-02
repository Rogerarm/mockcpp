
#include <InvocationMocker.h>
#include <ChainableMockMethodCore.h>
#include <Matcher.h>
#include <Stub.h>
#include <Any.h>
#include <Asserter.h>
#include <InvocationId.h>
#include <Method.h>
#include <StubContainer.h>

#include <list>
#include <algorithm>

MOCKCPP_NS_START

namespace
{
   const std::string normalSpace = "     ";

   const std::string& space()
   {
	    return normalSpace;
   }
}
///////////////////////////////////////////////////////////
class InvocationMockerImpl
{
public:
    typedef std::list<Matcher*>  List;
    typedef List::iterator Iterator;
    typedef List::const_iterator ConstIterator;

	///////////////////////////////////////////////////
    List matchers;
    bool hasBeenInvoked;
    Method* belongedMethod;
    InvocationId* id ;

    StubContainer stubs;
	///////////////////////////////////////////////////
    InvocationMockerImpl(Method* method)
		: belongedMethod(method), hasBeenInvoked(false), id(0), stubs(space())
    {}

    ~InvocationMockerImpl()
    { reset(); }

    bool matches(const Invocation& inv) const;
    void increaseInvoked(const Invocation& inv);
    Any& invoke(const Invocation& inv);

    void reset();
    void verify();
    std::string toString() const;
};

/////////////////////////////////////////////////////////


std::string
InvocationMockerImpl::toString() const
{
    std::ostringstream ss;

    ss << "method(" << belongedMethod->getName() << ")";

    for (ConstIterator i = matchers.begin(); i != matchers.end(); ++i)
    {
      ss << "\n" << space() << (*i)->toString();
    }

    ss << stubs.toString();

    if(id != 0)
    {
      ss << "\n" << space() << id->toString();
    }
   
    ss << ";";

    return ss.str();
}

/////////////////////////////////////////////////////////
namespace
{
    void verifyMatcher(Matcher* matcher)
    {
      matcher->verify();
    }
}

/////////////////////////////////////////////////////////
void InvocationMockerImpl::verify()
{
    for_each(matchers.begin(), matchers.end(), verifyMatcher);
}

/////////////////////////////////////////////////////////
namespace
{
    void deleteMatcher(Matcher* matcher)
    {
      delete matcher;
    }
}

///////////////////////////////////////////////////////////
void InvocationMockerImpl::reset()
{
    for_each(matchers.begin(), matchers.end(), deleteMatcher);

    matchers.clear();

    stubs.reset();

    if (id != 0)
    {
      delete id;
      id = 0;
    }
}

///////////////////////////////////////////////////////////
bool
InvocationMockerImpl::matches(const Invocation& inv) const
{
    ConstIterator i = matchers.begin();
    for (; i != matchers.end(); ++i)
    {
      if (!(*i)->matches(inv))
      {
        return false;
      }
    }

    return true;
}

///////////////////////////////////////////////////////////
void
InvocationMockerImpl::increaseInvoked(const Invocation& inv) 
{
    for (Iterator i = matchers.begin(); i != matchers.end(); ++i)
    {
      (*i)->increaseInvoked(inv);
    }
}

///////////////////////////////////////////////////////////
Any&
InvocationMockerImpl::invoke(const Invocation& inv)
{
    increaseInvoked(inv);

    hasBeenInvoked = true;

    Stub* stub = stubs.getStub();
    if (stub != 0)
    {
      return stub->invoke(inv);
    } 

    return getVoidAny();
}

///////////////////////////////////////////////////////////
InvocationMocker::InvocationMocker(Method* method)
   : This(new InvocationMockerImpl(method))
{
}

///////////////////////////////////////////////////////////
InvocationMocker::~InvocationMocker()
{
    delete This;
}

///////////////////////////////////////////////////////////
Method*
InvocationMocker::getMethod() const
{
    return This->belongedMethod;
}

///////////////////////////////////////////////////////////
void InvocationMocker::setId(InvocationId* id)
{
    if (This->id != 0) delete This->id;

    This->id = id;
}

///////////////////////////////////////////////////////////
void InvocationMocker::addMatcher(Matcher* matcher)
{
    This->matchers.push_back(matcher);
}

///////////////////////////////////////////////////////////
bool InvocationMocker::hasBeenInvoked(void) const
{
    return This->hasBeenInvoked;
}

///////////////////////////////////////////////////////////
void InvocationMocker::addStub(Stub* stub) 
{
    This->stubs.addStub(stub);
}

///////////////////////////////////////////////////////////
const InvocationId* const InvocationMocker::getId(void) const
{
    return This->id;
}

///////////////////////////////////////////////////////////
bool InvocationMocker::matches(const Invocation& inv) const
{
    return This->matches(inv);
}

///////////////////////////////////////////////////////////
std::string InvocationMocker::toString() const
{
    return This->toString();
}

///////////////////////////////////////////////////////////
Any& InvocationMocker::invoke(const Invocation& inv)
{
    try
    {
      return This->invoke(inv);
    }
    catch (Exception& ex)
    {
      MOCKCPP_FAIL(ex.getMessage() + "\n" +
                    This->toString());
    }

	return getEmptyAny();
}

///////////////////////////////////////////////////////////
void InvocationMocker::verify()
{
    try
    {
      This->verify();
    }
    catch (Exception& ex)
    {
      MOCKCPP_FAIL(ex.getMessage() + "\n" +
                    This->toString());
    }
}

///////////////////////////////////////////////////////////

MOCKCPP_NS_END
