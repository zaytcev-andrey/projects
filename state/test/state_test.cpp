// state_test.cpp : Defines the entry point for the console application.
//

#include <state/test/precompiled.h>

#include <state/include/state.h>
#include <state/include/context.h>
#include <state/include/i_context_impl.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace testing;

class MockState : public IState
{
public:
     MOCK_METHOD2( RequestForData, void( IContext* context, IContextImpl* context_impl ) );
     MOCK_METHOD2( GetResult, void( IContext* context, IContextImpl* context_impl ) );
};

class TestState : public MockState
{
public:
     using IState::ChangeState;
};

class MockContextImpl : public IContextImpl
{
public:
     MOCK_METHOD0( RequestForDataImpl, void() );
     MOCK_METHOD0( GetResultImpl, void() );
};

class MockContext : public IContext
{
public:
     MOCK_METHOD0( RequestForData, void() );
     MOCK_METHOD0( GetResult, void() );
     MOCK_METHOD1( ChangeState, void( IState* state ) );
     MOCK_METHOD1( ChangeState, void( boost::shared_ptr< IState > state ) );
};

class TestContext: public Context
{
public:
     TestContext()
          : Context() 
     {
     }
     TestContext( IState* state
          , boost::shared_ptr< IContextImpl > impl )
          : Context( state, impl ) 
     {
     }
     MOCK_METHOD1( ChangeState, void( IState* state ) );
};

class TestInitState : public InitState
{
public:
     MOCK_METHOD2( ChangeState, void( IContext* context, IState* state ) );
};

class TestWorkingState : public WorkingState
{
public:
     MOCK_METHOD2( ChangeState, void( IContext* context, IState* state ) );
};

class TestDoneState : public DoneState
{
public:
     MOCK_METHOD2( ChangeState, void( IContext* context, IState* state ) );
};

TEST( BaseStateTest, ChangeState )
{
     using namespace ::testing;

     MockContext moc_context;
     MockState next_state;

     TestState testing_state;     
     
     EXPECT_CALL( moc_context, ChangeState( Matcher< IState* >( Eq( &next_state ) ) ) ).Times( 1 );

     testing_state.ChangeState( &moc_context, &next_state );
}

TEST( InitStateTest, RequestForData )
{
     MockContext moc_context;
     MockContextImpl moc_context_impl;

     IState* new_state = WorkingState::Instance();
 
     TestInitState testing_state;     
     EXPECT_CALL( moc_context_impl, RequestForDataImpl() ).Times( 1 );
     EXPECT_CALL( testing_state, ChangeState( &moc_context, new_state ) ).Times( 1 );

     testing_state.RequestForData( &moc_context, &moc_context_impl );
}

TEST( InitStateTest, GetResult )
{
     using ::testing::_;
     
     MockContext moc_context;
     MockContextImpl moc_context_impl;

     TestInitState testing_state;     
     EXPECT_CALL( moc_context_impl, GetResultImpl() ).Times( 1 );
     EXPECT_CALL( testing_state, ChangeState( &moc_context, _ ) ).Times( 0 );

     testing_state.GetResult( &moc_context, &moc_context_impl );
}

TEST( WorkingStateTest, RequestForData )
{
     using ::testing::_;

     MockContext moc_context;
     MockContextImpl moc_context_impl;

     TestWorkingState testing_state;     
     EXPECT_CALL( moc_context_impl, RequestForDataImpl() ).Times( 1 );
     EXPECT_CALL( testing_state, ChangeState( &moc_context, _ ) ).Times( 0 );

     testing_state.RequestForData( &moc_context, &moc_context_impl );
}

TEST( WorkingStateTest, GetResult )
{
     MockContext moc_context;
     MockContextImpl moc_context_impl;

     IState* new_state = DoneState::Instance();

     TestWorkingState state;     
     EXPECT_CALL( moc_context_impl, GetResultImpl() ).Times( 1 );
     EXPECT_CALL( state, ChangeState( &moc_context, new_state ) ).Times( 1 );

     state.GetResult( &moc_context, &moc_context_impl );
}

TEST( DoneStateTest, RequestForData )
{
     MockContext moc_context;
     MockContextImpl moc_context_impl;

     IState* new_state = WorkingState::Instance();

     TestDoneState state;     
     EXPECT_CALL( moc_context_impl, RequestForDataImpl() ).Times( 1 );
     EXPECT_CALL( state, ChangeState( &moc_context, new_state ) ).Times( 1 );

     state.RequestForData( &moc_context, &moc_context_impl );
}

TEST( DoneStateTest, GetResult )
{
     using ::testing::_;
     
     MockContext moc_context;
     MockContextImpl moc_context_impl;

     TestDoneState state;    
     EXPECT_CALL( moc_context_impl, GetResultImpl() ).Times( 1 );
     EXPECT_CALL( state, ChangeState( &moc_context, _ ) ).Times( 0 );

     state.GetResult( &moc_context, &moc_context_impl );
}

TEST( Context, ContextBehaviourTest )
{
     MockState state;
     boost::shared_ptr< IContextImpl > moc_context_impl( new MockContextImpl() );

     TestContext test_context( &state, moc_context_impl );
    
     EXPECT_CALL( state, RequestForData( &test_context, moc_context_impl.get() ) ).Times( 1 );
     test_context.RequestForData();

     EXPECT_CALL( state, GetResult( &test_context, moc_context_impl.get() ) ).Times( 1 );
     test_context.GetResult();
}