#include <state/include/precompiled.h>

#include <state/include/context.h>
#include <state/include/state.h>


Context::Context()
     : state_( InitState::Instance() )
{
}

Context::Context( IState* state )
     : state_( state )
{
}

void Context::RequestForData()
{
     state_->RequestForData( this );
}

void Context::GetResult()
{

}