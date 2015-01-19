// state_test.cpp : Defines the entry point for the console application.
//

#include <state/test/precompiled.h>

#include <state/include/state.h>
#include <state/include/context.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace testing;


class MockContext : public IContext
{
public:
     MOCK_METHOD0( Request, void() );
     MOCK_METHOD1( ChangeState, void( State* state ) );
};

class MockInitState : public InitState
{
public:
     MOCK_METHOD2( ChangeState, void( IContext* context, State* state ) );
};


TEST( InitStateTest, Request )
{
     MockContext moc_context;
     State* new_state = WorkingState::Instance();
 
     MockInitState state;     
     EXPECT_CALL( moc_context, Request() ).Times( 1 );
     EXPECT_CALL( state, ChangeState( &moc_context, new_state ) ).Times( 1 );

     state.Request( &moc_context );
}
