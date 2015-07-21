// tcp_client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#pragma comment( lib, "Ws2_32.lib" )

#include <winsock2.h>
#include <process.h>
#include <string>
#include <exception>
#include <iostream>
#include <memory>
#include <sstream>

class wsa_initializer
{
public:
     wsa_initializer()
     {
          WORD version_requested = MAKEWORD( 2, 2 );
          WSADATA wsa_data;
          int err = WSAStartup( version_requested, &wsa_data );
          if ( err != 0 ) 
          {                               
               throw std::exception( "could not find a usable WinSock DLL" );
          }
     }
     ~wsa_initializer()
     {
          WSACleanup();
     }
};

class tcp_client
{
public:

     tcp_client( const std::string& address )
          : address_( address )
     {
          create_socket();
     }

     ~tcp_client()
     {
          closesocket( connected_socket_ );
     }

     void connect()
     {
          sockaddr_in inetaddr;
          inetaddr.sin_family = AF_INET;
          inetaddr.sin_addr.s_addr = inet_addr( address_.c_str() );
          inetaddr.sin_port = htons( 27015 );

          int res = ::connect( connected_socket_
               , (SOCKADDR*)&inetaddr, sizeof( inetaddr ) );

          if ( res == SOCKET_ERROR )
          {
               std::ostringstream error;
               error << "Failed to connect with WSAGetLastError = " << WSAGetLastError();
               throw std::exception( error.str().c_str() );
          }

          std::cout << "tcp_client connected to " << address_ << std::endl;
     }

     void send_data( const char* data, size_t data_len )
     {
          std::cout << "send: ";
          std::cout.write( data, data_len ) << std::endl;
          int send_res = ::send( connected_socket_, data, data_len, 0 );

          if ( send_res == SOCKET_ERROR )
          {
               std::ostringstream error;
               error << "send failed with WSAGetLastError = " << WSAGetLastError();
               throw std::exception( error.str().c_str() );
          }
     }

private:
     
     void create_socket()
     {
          connected_socket_ = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

          if ( connected_socket_ == INVALID_SOCKET )
          {
               std::ostringstream error;
               error << "socket failed with WSAGetLastError = " << WSAGetLastError();
               throw std::exception( error.str().c_str() );
          }
     }

private:
     
     wsa_initializer wsa_init_;
     std::string address_;
     SOCKET connected_socket_;
};

int _tmain(int argc, _TCHAR* argv[])
{
     try
     {
          if ( argc < 2 )
          {
               std::cout << "must have one parameter" << std::endl;
               return 0;
          }

          const std::string address = argv[ 1 ];
          
          tcp_client client( address );
          client.connect();

          std::string message( "tcp client message" );
          while ( true )
          {
               client.send_data( message.c_str(), message.length() );
               ::Sleep( 60000 );
          }
     }
     catch( const std::exception& e )
     {
          std::cout << "error: " << e.what() << std::endl;
     }
     
     return 0;
}

