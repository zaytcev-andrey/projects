#pragma once

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include <state/include/i_context.h>


class IContextImpl;

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
 
     void ChangeState( boost::shared_ptr< IState > state )
     {
          // shared_state_.swap( state );
     }
 
protected:

     Context( IState* state, boost::shared_ptr< IContextImpl > impl );

private:     
     
     IState* state_;
     boost::shared_ptr< IState > shared_state_;
     boost::shared_ptr< IContextImpl > impl_;
};