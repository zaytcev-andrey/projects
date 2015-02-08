#pragma once

#include <state/include/i_context_impl.h>

class ContextImpl : public IContextImpl
{
public:

     void RequestForDataImpl();

     void GetResultImpl();
};