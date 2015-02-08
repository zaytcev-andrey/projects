#include <state/include/precompiled.h>

#include <state/include/context.h>
#include <state/include/context_impl.h>
#include <state/include/state.h>


Context::Context()
     : state_( InitState::Instance() )
     , impl_( new ContextImpl() )
{
}

Context::Context( IState* state, boost::shared_ptr< IContextImpl > impl )
     : state_( state )
     , impl_( impl )
{
}

void Context::RequestForData()
{
     state_->RequestForData( this, impl_.get() );

     boost::shared_ptr< IState > holder( shared_state_ );
     {
          boost::shared_ptr< IState > new_state( new WorkingState() );
          shared_state_.swap( new_state );
     }
}

void Context::GetResult()
{
     state_->GetResult( this, impl_.get() );
}