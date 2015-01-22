#include <state/include/precompiled.h>

#include <state/include/state.h>
#include <state/include/context.h>

#include <boost/scoped_ptr.hpp>

boost::scoped_ptr< InitState > InitState::instatnce_( 0 );
boost::scoped_ptr< WorkingState > WorkingState::instatnce_( 0 );
boost::scoped_ptr< DoneState > DoneState::instatnce_( 0 );

void IState::ChangeState( IContext* context, IState* state )
{
     context->ChangeState( state );
}

void InitState::RequestForData( IContext* context )
{
     context->RequestForData();

     ChangeState( context, WorkingState::Instance() );
}

void InitState::GetResult( IContext* context )
{
     // do nothing
}

void WorkingState::RequestForData( IContext* context )
{
     context->RequestForData();
}

void WorkingState::GetResult( IContext* context )
{
     context->GetResult();
     
     ChangeState( context, DoneState::Instance() );
}

void DoneState::RequestForData( IContext* context )
{
     context->RequestForData();

     ChangeState( context, WorkingState::Instance() );
}

void DoneState::GetResult( IContext* context )
{
     context->GetResult();
}


