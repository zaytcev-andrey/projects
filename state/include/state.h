#pragma once

#include <boost/scoped_ptr.hpp>

class IContext;

class State
{
public:
     virtual void Request( IContext* context ) = 0;
protected:
     virtual void ChangeState( IContext* context, State* state );
};


class InitState : public State
{
public:
     void Request( IContext* context );

     static InitState* Instance()
     {
          if ( !instatnce_.get() )
          {
               instatnce_.reset( new InitState() );                   
          }    

          return instatnce_.get();
     }
private:
     static boost::scoped_ptr< InitState > instatnce_;
};

class WorkingState : public State
{
public:
     void Request( IContext* context );

     static WorkingState* Instance()
     {
          if ( !instatnce_.get() )
          {
               instatnce_.reset( new WorkingState() );                   
          }    

          return instatnce_.get();
     }

private:
     static boost::scoped_ptr< WorkingState > instatnce_;
};
