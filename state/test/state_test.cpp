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
     MOCK_METHOD0( RequestForData, void() );
     MOCK_METHOD0( GetResult, void() );
     MOCK_METHOD1( ChangeState, void( IState* state ) );
};

class MockInitState : public InitState
{
public:
     MOCK_METHOD2( ChangeState, void( IContext* context, IState* state ) );
};


TEST( InitStateTest, Request )
{
     MockContext moc_context;
     IState* new_state = WorkingState::Instance();
 
     MockInitState state;     
     EXPECT_CALL( moc_context, RequestForData() ).Times( 1 );
     EXPECT_CALL( state, ChangeState( &moc_context, new_state ) ).Times( 1 );

     state.RequestForData( &moc_context );
}
