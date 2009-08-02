
#include <cxxtest/TestSuite.h>

#include <ChainableMockMethodContainer.h>
#include <ChainableMockMethodCore.h>
#include <InvocationMockBuilderGetter.h>


class TestChainableMockMethodContainer : public CxxTest::TestSuite
{
   struct InvocationMockerNamespaceStub : public InvocationMockerNamespace
   {
      InvocationMockerNamespaceStub()
			: name("InvocationMockerNamespaceStub")
      {}

      InvocationMocker* getInvocationMocker(const std::string& id)  const
      {
         return 0;
      }

      std::string& getName(void) const
      {
          return const_cast<std::string&>(name);
      } 

      std::string name;
   };

private:

    MOCKCPP_NS::ChainableMockMethodNameKey* key;
    InvocationMockerNamespaceStub ns;
    ChainableMockMethodCore* method;

public:
   
	void setUp()
   {
      key = new ChainableMockMethodNameKey("method");
		method = new ChainableMockMethodCore(key, &ns);
   }

	void tearDown()
   {
   }

	/////////////////////////////////////////////////////////

	void testShouldBeAbleToAddAMethod()
	{
      MOCKCPP_NS::ChainableMockMethodContainer container;
      container.addMethod(key, method);
      TS_ASSERT_EQUALS(method, container.getMethod(key));
	}

   void testShouldReturnNullIfMethodWithTheSpecifiedKeyWasNotAddedToContainer()
   {
      MOCKCPP_NS::ChainableMockMethodContainer container;
      TS_ASSERT(0 == container.getMethod(key));
   }

   void testShouldThrowExceptionIfAMethodWithTheSameKeyHasBeedAdded()
   {
      MOCKCPP_NS::ChainableMockMethodContainer container;
      container.addMethod(key, method);

      TS_ASSERT_THROWS(container.addMethod(key, method), MOCKCPP_NS::Exception);
   }

   void testYouCannotGetTheMethodAnyLongerAfterTheContainerIsReset()
   {
      MOCKCPP_NS::ChainableMockMethodContainer container;
      container.addMethod(key, method);
      container.reset();
      TS_ASSERT(0 == container.getMethod(key));
   }

   void testShouldBeAbleToVerifyEveryMethodInTheContainer()
   {
      InvocationMockBuilderGetter builderGetter(method, method);

      builderGetter.expects(once());

      MOCKCPP_NS::ChainableMockMethodContainer container;
      container.addMethod(key, method);

      TS_ASSERT_THROWS(container.verify(), MOCKCPP_NS::Exception);
   }
  
   void testShouldBeAbleToGetInvocationMockerById()
   {
      InvocationMockBuilderGetter builderGetter(method, method);

      builderGetter.expects(once()).id("1");

      MOCKCPP_NS::ChainableMockMethodContainer container;
      container.addMethod(key, method);

      TS_ASSERT(container.findInvocationMocker("1") != 0);
   }
   
   void testShouldReturnNullIfAInvocationMockerWithSpecifiedIdWasNeverBuilt()
   {
      InvocationMockBuilderGetter builderGetter(method, method);

      builderGetter.expects(once()).id("1");

      MOCKCPP_NS::ChainableMockMethodContainer container;
      container.addMethod(key, method);

      TS_ASSERT(container.findInvocationMocker("0") == 0);
   }
};
