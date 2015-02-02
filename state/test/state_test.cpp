// state_test.cpp : Defines the entry point for the console application.
//

#include <state/test/precompiled.h>

#include <state/include/state.h>
#include <state/include/context.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace testing;

class MockState : public IState
{
public:
     MOCK_METHOD1( RequestForData, void( IContext* context ) );
     MOCK_METHOD1( GetResult, void( IContext* context ) );
};

class MockContext : public IContext
{
public:
     MOCK_METHOD0( RequestForData, void() );
     MOCK_METHOD0( GetResult, void() );
     MOCK_METHOD1( ChangeState, void( IState* state ) );
};

class TestContext: public Context
{
public:
     TestContext()
          : Context() 
     {
     }
     TestContext( IState* state )
          : Context( state ) 
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


TEST( InitStateTest, RequestForData )
{
     MockContext moc_context;
     IState* new_state = WorkingState::Instance();
 
     TestInitState testing_state;     
     EXPECT_CALL( moc_context, RequestForData() ).Times( 1 );
     EXPECT_CALL( testing_state, ChangeState( &moc_context, new_state ) ).Times( 1 );

     testing_state.RequestForData( &moc_context );
}

TEST( WorkingStateTest, RequestForData )
{
     using ::testing::_;

     MockContext moc_context;

     TestWorkingState testing_state;     
     EXPECT_CALL( moc_context, RequestForData() ).Times( 1 );
     EXPECT_CALL( testing_state, ChangeState( &moc_context, _ ) ).Times( 0 );

     testing_state.RequestForData( &moc_context );
}

TEST( WorkingStateTest, GetResult )
{
     MockContext moc_context;
     IState* new_state = DoneState::Instance();

     TestWorkingState state;     
     EXPECT_CALL( moc_context, GetResult() ).Times( 1 );
     EXPECT_CALL( state, ChangeState( &moc_context, new_state ) ).Times( 1 );

     state.GetResult( &moc_context );
}

TEST( DoneStateTest, RequestForData )
{
     MockContext moc_context;
     IState* new_state = WorkingState::Instance();

     TestDoneState state;     
     EXPECT_CALL( moc_context, RequestForData() ).Times( 1 );
     EXPECT_CALL( state, ChangeState( &moc_context, new_state ) ).Times( 1 );

     state.RequestForData( &moc_context );
}

TEST( DoneStateTest, GetResult )
{
     using ::testing::_;
     
     MockContext moc_context;

     TestDoneState state;    
     EXPECT_CALL( moc_context, GetResult() ).Times( 1 );
     EXPECT_CALL( state, ChangeState( &moc_context, _ ) ).Times( 0 );

     state.GetResult( &moc_context );
}

TEST( Context, RequestForData )
{
     MockState state;
     TestContext test_context( &state );
    
     EXPECT_CALL( state, RequestForData( &test_context ) ).Times( 1 );
     test_context.RequestForData();

     EXPECT_CALL( state, GetResult( &test_context ) ).Times( 1 );
     test_context.GetResult();
}