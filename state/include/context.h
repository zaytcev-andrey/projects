#pragma once

class Data
{
public:
     Data( int val = 10 )
          : value_( 10 )
     {
     }

     void SetValue( int val )
     {
          value_ = val;
     }

     int GetValue() const
     {
          return value_;
     }
private:
     int value_;
};

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
     friend class State;

     void ChangeState( State* state )
     {
          state_ = state;
     }
private:
     Data data_;
     State* state_;
};