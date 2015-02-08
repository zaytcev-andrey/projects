#pragma once

class IContextImpl
{
public:
     
     ~IContextImpl() {}

     virtual void RequestForDataImpl() = 0;

     virtual void GetResultImpl() = 0;
};