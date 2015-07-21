#include "stdafx.h"

#include "wsa_initializer.h"

#include <winsock2.h>
#include <string>
#include <exception>
#include <iostream>
#include <sstream>

#pragma comment( lib, "Ws2_32.lib" )

/*
wsa_initializer::wsa_initializer()
{
     const WORD version_requested = MAKEWORD( 2, 2 );
     WSADATA wsa_data = {};
     const int err = WSAStartup( version_requested, &wsa_data );
     if ( err != 0 ) 
     {                               
          throw std::exception( "could not find a usable WinSock DLL" );
     }

     const int low_ver = LOBYTE( wsa_data.wVersion );
     const int high_ver = HIBYTE( wsa_data.wVersion );
     if ( low_ver != 2 || high_ver != 2 )
     {
          WSACleanup();
          std::ostringstream err;
          err << "could not find proper version of a WinSock DLL. " <<
               "version of an existing WinSock DLL is: " << 
               low_ver << "." << high_ver << ". " <<
               "require 2.2 version of an WinSock DLL or high";
          throw std::exception( err.str().c_str() );
     }

     std::cout << "version of WinSock DLL is: " << low_ver << "." << high_ver << std::endl;
}

wsa_initializer::~wsa_initializer()
{
     WSACleanup();
}*/