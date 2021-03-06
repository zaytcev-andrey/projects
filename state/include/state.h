#pragma once

#include <boost/scoped_ptr.hpp>

class IContext;
class IContextImpl; 

class IState
{
public:
     
     virtual ~IState() {}

     virtual void RequestForData( IContext* context, IContextImpl* impl ) = 0;

     virtual void GetResult( IContext* context, IContextImpl* impl ) = 0;

protected:
     virtual void ChangeState( IContext* context, IState* state );
};


class InitState : public IState
{
public:
     void RequestForData( IContext* context, IContextImpl* impl );
     void GetResult( IContext* context, IContextImpl* impl );

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

class WorkingState : public IState
{
public:
     void RequestForData( IContext* context, IContextImpl* impl );
     void GetResult( IContext* context, IContextImpl* impl );

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

class DoneState : public IState
{
public:
     void RequestForData( IContext* context, IContextImpl* impl );
     void GetResult( IContext* context, IContextImpl* impl );

     static DoneState* Instance()
     {
          if ( !instatnce_.get() )
          {
               instatnce_.reset( new DoneState() );
          }

          return instatnce_.get();
     }

private:
     static boost::scoped_ptr< DoneState > instatnce_;
};