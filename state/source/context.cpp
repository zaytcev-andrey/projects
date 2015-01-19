#include <state/include/precompiled.h>

#include <state/include/context.h>
#include <state/include/state.h>

Context::Context()
: state_( InitState::Instance() )
{
}

void Context::Request()
{
     state_->Request( this );
}