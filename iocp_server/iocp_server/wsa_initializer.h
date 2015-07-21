#ifndef wsa_initializer_h__
#define wsa_initializer_h__

#include <winsock2.h>
#include <string>
#include <exception>
#include <iostream>
#include <sstream>

class systemproxy
{
public:
     int WSAStartup( WORD version_requested, LPWSADATA wsa_data )
     {
          return ::WSAStartup( version_requested, wsa_data );
     }

     int WSACleanup()
     {
          return ::WSACleanup();
     }
};


namespace testing
{
     class mock_system_proxy
     {
     public:
          int WSAStartup( WORD version_requested, LPWSADATA wsa_data )
          {
               wsa_data->wVersion = 2;
               wsa_data->wVersion |= ( 2 << 8 );

               return 0;
          }

          int WSACleanup()
          {
               return 0;
          }
     };
}

template < class system_proxy >
class wsa_initializer : private system_proxy
{
public:
     wsa_initializer()
     {
          const WORD version_requested = MAKEWORD( 2, 2 );
          WSADATA wsa_data = {};
          const int err = system_proxy::WSAStartup( version_requested, &wsa_data );
          if ( err != 0 ) 
          {                               
               throw std::exception( "could not find a usable WinSock DLL" );
          }

          const int low_ver = LOBYTE( wsa_data.wVersion );
          const int high_ver = HIBYTE( wsa_data.wVersion );
          if ( low_ver != 2 || high_ver != 2 )
          {
               system_proxy::WSACleanup();
               std::ostringstream err;
               err << "could not find proper version of a WinSock DLL. " <<
                    "version of an existing WinSock DLL is: " << 
                    low_ver << "." << high_ver << ". " <<
                    "require 2.2 version of an WinSock DLL or high";
               throw std::exception( err.str().c_str() );
          }

          std::cout << "version of WinSock DLL is: " << low_ver << "." << high_ver << std::endl;
     }
     ~wsa_initializer()
     {
          system_proxy::WSACleanup();
     }
};

#endif // wsa_initializer_h__