#include <state/include/precompiled.h>

#include <state/include/state.h>
#include <state/include/context.h>
#include <state/include/context_impl.h>

#include <boost/scoped_ptr.hpp>

boost::scoped_ptr< InitState > InitState::instatnce_( 0 );
boost::scoped_ptr< WorkingState > WorkingState::instatnce_( 0 );

void IState::ChangeState( IContext* context, IState* state )
{
     context->ChangeState( state );
}

void InitState::Request( IContext* context )
{
     context->Request();

     ChangeState( context, WorkingState::Instance() );
}

void WorkingState::Request( IContext* context )
{
     context->Request();
}




