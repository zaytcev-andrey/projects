#include <state/include/precompiled.h>

#include <state/include/state.h>
#include <state/include/i_context.h>
#include <state/include/i_context_impl.h>

#include <boost/scoped_ptr.hpp>

boost::scoped_ptr< InitState > InitState::instatnce_( 0 );
boost::scoped_ptr< WorkingState > WorkingState::instatnce_( 0 );
boost::scoped_ptr< DoneState > DoneState::instatnce_( 0 );

void IState::ChangeState( IContext* context, IState* state )
{
     context->ChangeState( state );
}

void InitState::RequestForData( IContext* context, IContextImpl* impl )
{
     impl->RequestForDataImpl();

     ChangeState( context, WorkingState::Instance() );
}

void InitState::GetResult( IContext* context, IContextImpl* impl )
{
     impl->GetResultImpl();
}

void WorkingState::RequestForData( IContext* context, IContextImpl* impl )
{
     impl->RequestForDataImpl();
}

void WorkingState::GetResult( IContext* context, IContextImpl* impl )
{
     impl->GetResultImpl();
     
     ChangeState( context, DoneState::Instance() );
}

void DoneState::RequestForData( IContext* context, IContextImpl* impl )
{
     impl->RequestForDataImpl();

     ChangeState( context, WorkingState::Instance() );
}

void DoneState::GetResult( IContext* context, IContextImpl* impl )
{
     impl->GetResultImpl();
}


