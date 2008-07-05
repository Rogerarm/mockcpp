
#include <mockcpp.h>
#include <PendingMatcher.h>
#include <InvocationMocker.h>
#include <OrderingMatcher.h>
#include <Invocation.h>
#include <Namespace.h>
#include <Asserter.h>

#include <string>

MOCKCPP_NS_START

#define SETUP_ORDERING_MATCHER() do { \
	setUpOrderingMatcher(); \
	oss_t oss; \
	oss << "There isn't an expectation whose id is \"" \
		 << id << "\" in namespace " << scope->getName() \
       << ", which is from : \n" << mocker->toString(); \
	MOCKCPP_ASSERT_TRUE_MESSAGE(oss.str(), matcher->isComplete()); \
} while(0)

/////////////////////////////////////////////////////////////////
PendingMatcher::PendingMatcher(OrderingMatcher* orderingMatcher
                 , Namespace* ns
                 , const std::string& identity
                 , InvocationMocker* ThisMocker)
	: matcher(orderingMatcher)
   , scope(ns)
	, id(identity)
   , mocker(ThisMocker)
{
}

/////////////////////////////////////////////////////////////////
PendingMatcher::~PendingMatcher()
{
    delete matcher;
}

/////////////////////////////////////////////////////////////////
bool PendingMatcher::matches(const Invocation& inv) const
{
    SETUP_ORDERING_MATCHER();
    return matcher->matches(inv);
}

/////////////////////////////////////////////////////////////////
void PendingMatcher::increaseInvoked(const Invocation& inv)
{
    SETUP_ORDERING_MATCHER();
    return matcher->increaseInvoked(inv);
}

/////////////////////////////////////////////////////////////////
std::string PendingMatcher::toString() const
{
	oss_t oss;
	oss << "." << matcher->toString() 
       << "("
       << scope->getName() << ", "
       << "\"" << id << "\""
       << ")" ;

	return oss.str();
}

/////////////////////////////////////////////////////////////////
void PendingMatcher::verify()
{
    return matcher->verify();
}

/////////////////////////////////////////////////////////////////
void PendingMatcher::setUpOrderingMatcher() const
{
    if(matcher->isComplete()) 
      return;

    InvocationMocker* mocker = scope->getInvocationMocker(id);
    if(mocker != 0) {
      matcher->setOrderingInvocationMocker(mocker);
    }
}

MOCKCPP_NS_END

