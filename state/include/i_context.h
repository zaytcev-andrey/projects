#pragma once

#include <boost/shared_ptr.hpp>

class IState;

class IContext
{
public:

     virtual ~IContext() {}

     virtual void RequestForData() = 0;

     virtual void GetResult() = 0;

private:

     friend class IState;

     virtual void ChangeState( IState* state ) = 0;

     virtual void ChangeState( boost::shared_ptr< IState > state ) = 0;
};