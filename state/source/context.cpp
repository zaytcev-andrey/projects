#include <state/include/precompiled.h>

#include <state/include/context.h>
#include <state/include/state.h>

class Context::ContextImpl
{
public:
     void ContextInternalFunction()
     {
     }
};

Context::Context()
     : impl_( new ContextImpl() )
     , state_( InitState::Instance() )
{
}

void Context::Request()
{
     state_->Request( this );
}