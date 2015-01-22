#pragma once

#include <boost/scoped_ptr.hpp>
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
};

class Context : public IContext
{
public:

     Context();

     void RequestForData();

     void GetResult();

private:

     void ChangeState( IState* state )
     {
          state_ = state;
     }
 
protected:

     Context( IState* state );

private:     
     
     IState* state_;
};