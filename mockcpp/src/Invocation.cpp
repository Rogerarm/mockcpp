

#include <vector>

#include <OutputStringStream.h>
#include <Invocation.h>
#include <RefAny.h>

MOCKCPP_NS_START

////////////////////////////////////////////////////////////////
namespace
{
    const static unsigned int maxNumOfParameters = 12;
};

////////////////////////////////////////////////////////////////
class InvocationImpl
{
public:
    std::vector<RefAny> parameters;
	std::string nameOfCaller;
    std::string toString(void) const;

	InvocationImpl(const std::string& name);
};

////////////////////////////////////////////////////////////////
InvocationImpl::InvocationImpl(const std::string &name)
: nameOfCaller(name)
{
}
////////////////////////////////////////////////////////////////
std::string InvocationImpl::toString() const
{
    oss_t oss;

    for (unsigned int i=0; i<maxNumOfParameters; i++)
    {
      if (parameters[i].empty())
          return oss.str();

      if (i > 0) oss << ", ";

      oss << parameters[i].toTypeAndValueString();
    }

    return oss.str();
}

////////////////////////////////////////////////////////////////

#define INIT_PARAMETER(i) This->parameters.push_back(p##i)

Invocation::Invocation(
                     const std::string name
                   , const RefAny& p1
				   , const RefAny& p2
				   , const RefAny& p3
				   , const RefAny& p4
				   , const RefAny& p5
				   , const RefAny& p6
				   , const RefAny& p7
				   , const RefAny& p8
				   , const RefAny& p9
				   , const RefAny& p10
				   , const RefAny& p11
				   , const RefAny& p12
   )
	: This(new InvocationImpl(name))
{
	INIT_PARAMETER(1);
	INIT_PARAMETER(2);
	INIT_PARAMETER(3);
	INIT_PARAMETER(4);
	INIT_PARAMETER(5);
	INIT_PARAMETER(6);
	INIT_PARAMETER(7);
	INIT_PARAMETER(8);
	INIT_PARAMETER(9);
	INIT_PARAMETER(10);
	INIT_PARAMETER(11);
	INIT_PARAMETER(12);
}

////////////////////////////////////////////////////////////////
Invocation::~Invocation()
{
	delete This;
}

////////////////////////////////////////////////////////////////
RefAny& Invocation::getParameter(const unsigned int i) const
{
    if (i < 1 || i > maxNumOfParameters )
    {
		return getEmptyRefAny();
    }

    return This->parameters[i-1];
}

////////////////////////////////////////////////////////////////
std::string Invocation::toString(void) const
{
    return std::string("(") + This->toString() + std::string(")");
}

////////////////////////////////////////////////////////////////

std::string Invocation::getNameOfCaller() const
{
	return This->nameOfCaller;
}

////////////////////////////////////////////////////////////////

MOCKCPP_NS_END
