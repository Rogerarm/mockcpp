
#include <ReturnObjectList.h>
#include <Asserter.h>

#include <vector>

MOCKCPP_NS_START

struct ReturnObjectListImpl
{
    typedef std::vector<Any> Objects;

    unsigned int firstUnused;

    Objects objects;

    ReturnObjectListImpl() : firstUnused(0) {}

    unsigned int numberOfValidObjects() const;

    Any& invoke();

    std::string toString() const;

	const std::type_info& type() const;
};

///////////////////////////////////////////////////////
const std::type_info& ReturnObjectListImpl::type() const
{
    if (firstUnused >= objects.size()) return getEmptyAny().type();

    return objects[firstUnused].type();
}

///////////////////////////////////////////////////////
std::string ReturnObjectListImpl::toString() const
{
    oss_t oss;

    unsigned int n = numberOfValidObjects();
    for (unsigned int i = 0; i < n; i++)
    {
      if (i > 0) oss << ",";
      oss << objects[i].toTypeAndValueString();
    }

    return oss.str();
}

///////////////////////////////////////////////////////
unsigned int ReturnObjectListImpl::numberOfValidObjects() const
{
    for (int i = objects.size()-1; i >= 0; i--)
    {
      if(!objects[i].empty()) return i+1;
    }

    return 0;
}

///////////////////////////////////////////////////////
Any& ReturnObjectListImpl::invoke()
{
    if(firstUnused < numberOfValidObjects())
    {
      return objects[firstUnused++];
    }
    
    MOCKCPP_FAIL("All objects has been returned");

    return getEmptyAny();
}

///////////////////////////////////////////////////////
#define STORE_OBJECTS(i) This->objects.push_back(o##i)

ReturnObjectList::ReturnObjectList(
                      const Any& o1 
                    , const Any& o2 
                    , const Any& o3
                    , const Any& o4 
                    , const Any& o5
                    , const Any& o6 
                    , const Any& o7
                    , const Any& o8
                    , const Any& o9 
                    , const Any& o10 
                    , const Any& o11 
                    , const Any& o12 
    ) : This(new ReturnObjectListImpl)
{
    STORE_OBJECTS(1);
    STORE_OBJECTS(2);
    STORE_OBJECTS(3);
    STORE_OBJECTS(4);
    STORE_OBJECTS(5);
    STORE_OBJECTS(6);
    STORE_OBJECTS(7);
    STORE_OBJECTS(8);
    STORE_OBJECTS(9);
    STORE_OBJECTS(10);
    STORE_OBJECTS(11);
    STORE_OBJECTS(12);
}

///////////////////////////////////////////////////
ReturnObjectList::~ReturnObjectList()
{
    delete This;
}

///////////////////////////////////////////////////
Any& ReturnObjectList::invoke(void)
{
    return This->invoke();
}

///////////////////////////////////////////////////
std::string ReturnObjectList::toString(void) const
{
    oss_t oss;

    oss << "returnObjectList(" << This->toString() << ")";

    return oss.str();
}

///////////////////////////////////////////////////
const std::type_info& ReturnObjectList::type() const
{
    return This->type();   
}

///////////////////////////////////////////////////

MOCKCPP_NS_END
