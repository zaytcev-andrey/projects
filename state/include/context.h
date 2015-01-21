#pragma once

#include <boost/scoped_ptr.hpp>

class State;

class IContext
{
public:

     virtual void Request() = 0;
private:
     friend class State;

     virtual void ChangeState( State* state ) = 0;
};

class Context: public IContext
{
public:
     Context();

     void Request();
private:

     void ChangeState( State* state )
     {
          state_ = state;
     }
private:
     
     class ContextImpl;

     boost::scoped_ptr< ContextImpl > impl_;
     
     State* state_;
};