#include "stdafx.h"

#include "iocp_server.h"

namespace iocp
{

LPPER_IO_DATA create_per_io_data()
{
     LPPER_IO_DATA per_io_data = new PER_IO_DATA();
     
     per_io_data->BufferLen = DATA_BUFSIZE;
     per_io_data->WSABuffer.buf = per_io_data->Buffer;
     per_io_data->WSABuffer.len = per_io_data->BufferLen;

     return per_io_data;
}

iocp_server::iocp_server( const std::string& address )
     : address_( address )
     , iocp_( INVALID_HANDLE_VALUE )
     , listen_socket_( INVALID_SOCKET )
     , thread_count( 0 )
     , threads_( 0 )
{
     iocp_ = create_new_iocp();
     create_listening_socket();

     std::cout << "created tcp server" << std::endl;
}

iocp_server::~iocp_server()
{
     CloseHandle( iocp_ );
}

void iocp_server::run()
{                    
     create_working_threads();

     listen();

     join_threads();
}


HANDLE iocp_server::create_new_iocp()
{
     HANDLE iocp = CreateIoCompletionPort( 
          INVALID_HANDLE_VALUE, 0, 0, 0 );

     if ( !iocp )
     {
          throw std::exception( "could not create new completion port" );
     }

     return iocp;
}

void iocp_server::associate_socket_with_iocp( SOCKET accepted_socket, sockaddr_in* remote_point, int remote_len )
{
     LPPER_HANDLE_DATA per_handle_data = new PER_HANDLE_DATA();
     per_handle_data->Socket = accepted_socket;
     memcpy( &per_handle_data->ClientAddr, &remote_point, remote_len );

     HANDLE associated_iocp = CreateIoCompletionPort( ( HANDLE )accepted_socket, iocp_, ( ULONG_PTR )per_handle_data, 0 );
     if ( associated_iocp == 0 )
     {
          std::ostringstream error;
          error << "could not associate socket with iocp GetLastError = " << GetLastError();
          throw std::exception( error.str().c_str() );
     }
}

void iocp_server::create_working_threads()
{
     SYSTEM_INFO lpSystemInfo;
     GetSystemInfo( &lpSystemInfo );

     thread_count = lpSystemInfo.dwNumberOfProcessors;
     threads_ = new HANDLE[ thread_count ];

     for ( DWORD idx = 0; idx < thread_count; idx++ )
     {
          unsigned int thread_id = 0;
          HANDLE thread = ( HANDLE )_beginthreadex( 
               0, 0, server_working_thread, iocp_, 0, &thread_id );
     }
}

void iocp_server::join_threads()
{
     WaitForMultipleObjects( thread_count, threads_, TRUE, INFINITE );

     for ( DWORD idx = 0; idx < thread_count; idx++ )
     {
          CloseHandle( threads_[ idx ] );
     }
}

void iocp_server::create_listening_socket()
{
     listen_socket_ = WSASocket(
          AF_INET, SOCK_STREAM, 0, 0, 0, WSA_FLAG_OVERLAPPED );

     if ( listen_socket_ == INVALID_SOCKET )
     {
          std::ostringstream error;
          error << "WSASocket failed with WSAGetLastError = " << WSAGetLastError();
          throw std::exception( error.str().c_str() );
     }

     sockaddr_in inetaddr;
     inetaddr.sin_family = AF_INET;
     inetaddr.sin_addr.s_addr = inet_addr( address_.c_str() );
     inetaddr.sin_port = htons( 27015 );

     const int bind_res = bind( listen_socket_, (SOCKADDR*)&inetaddr, sizeof( inetaddr ) );
     if ( bind_res == SOCKET_ERROR )
     {
          std::ostringstream error;
          error << "could not bind socket, WSAGetLastError = " << WSAGetLastError();
          throw std::exception( error.str().c_str() );
     }
}

void iocp_server::listen()
{          
     ::listen( listen_socket_, SOMAXCONN );

     std::cout << "tcp server start listening, wait for new client" << std::endl;

     while( true )
     {
          sockaddr_in remote_point;
          int remote_len = sizeof( remote_point );
          SOCKET accepted_socket = WSAAccept( listen_socket_, ( sockaddr* )&remote_point, &remote_len, 0, 0 );

          if ( accepted_socket == INVALID_SOCKET )
          {
               std::ostringstream error;
               error << "WSAAccept failed with WSAGetLastError = " << WSAGetLastError();
               throw std::exception( error.str().c_str() );
          }

          std::cout << "new client accepted, associate socket with iocp" << std::endl;

          associate_socket_with_iocp( accepted_socket, &remote_point, remote_len );

          LPPER_IO_DATA per_io_data = create_per_io_data();

          async_receive( accepted_socket, per_io_data );
     }
}

DWORD iocp_server::data_wait_timeout = 1000;

void iocp_server::async_receive( SOCKET accepted_socket, LPPER_IO_DATA per_io_data )
{
     ZeroMemory( &per_io_data->Overlapped, sizeof( per_io_data->Overlapped ) );
     per_io_data->OperationType = RECV_POSTED;
     
     DWORD number_of_bytes_recvd = 0;
     DWORD flags = 0;
     int res = WSARecv( accepted_socket, &per_io_data->WSABuffer, 1
          , &number_of_bytes_recvd, &flags, &per_io_data->Overlapped, 0 );

     if ( res == 0 )
     {
          // already receive data from client
          process_received_bytes( 
               number_of_bytes_recvd
               , per_io_data
               , accepted_socket );
     }

     if ( res == SOCKET_ERROR )
     {
          const DWORD error = WSAGetLastError();
          
          if ( error != WSA_IO_PENDING )
          {
               std::ostringstream error;
               error << "WSARecv failed with WSAGetLastError = " << WSAGetLastError();
               throw std::exception( error.str().c_str() );
          }
     }
}

void iocp_server::process_received_bytes( 
     DWORD number_of_bytes_recvd
     , LPPER_IO_DATA per_io_data
     , SOCKET accepted_socket )
{
     std::cout << "received " << number_of_bytes_recvd << 
          " bytes from socket " << int( accepted_socket ) << ":" << std::endl;

     std::cout.write( per_io_data->Buffer, number_of_bytes_recvd ) << std::endl;
}

unsigned __stdcall iocp_server::server_working_thread( void* iocp )
{
     try
     {
          HANDLE io_cp = ( HANDLE )iocp;
          DWORD bytes_transfered = 0;
          LPPER_HANDLE_DATA per_handle_data = 0;
          LPPER_IO_DATA per_io_data = 0;          
          
          while ( true )
          {
               BOOL res = GetQueuedCompletionStatus(
                    io_cp
                    , &bytes_transfered
                    , ( PULONG_PTR )&per_handle_data
                    , ( LPOVERLAPPED* )&per_io_data
                    , data_wait_timeout );

               check_completion_for_critical_error( res, per_io_data );

               if ( !is_timeout( res, per_io_data ) )
               {
                    if ( bytes_transfered == 0 &&
                         ( per_io_data->OperationType == RECV_POSTED || per_io_data->OperationType ) )
                    {
                         std::cout << "GetQueuedCompletionStatus return 0 bytes" << std::endl;
                         std::cout << "socket " << int( per_handle_data->Socket ) << " closing" << std::endl;

                         closesocket( per_handle_data->Socket );

                         delete per_handle_data;
                         delete per_io_data;

                         continue;
                    }

                    if ( bytes_transfered != 0 )
                    {
                         process_received_bytes( 
                              bytes_transfered
                              , per_io_data
                              , per_handle_data->Socket );

                         async_receive( per_handle_data->Socket, per_io_data );
                    }     
               }
               else
               {
                    std::cout << "GetQueuedCompletionStatus return timeout" << std::endl;
                    std::cout << "GetQueuedCompletionStatus return timeout " << bytes_transfered << " bytes" << std::endl;

                    continue;
               }              
          }
     }
     catch( const std::exception& e )
     {
          std::cout << " server_working_thread throw exception: " << e.what() << std::endl;

          return 1;
     }
     
     return 0;          
}

void iocp_server::check_completion_for_critical_error( 
     BOOL res
     , LPPER_IO_DATA per_io_data )
{
     const DWORD last_error = GetLastError();
     if ( res == 0 && per_io_data == 0 && last_error != WAIT_TIMEOUT )
     {
          std::ostringstream error_strm;
          error_strm << "Completion port error: GetQueuedCompletionStatus failed with GetLastError = " << GetLastError();
          throw std::exception( error_strm.str().c_str() ); 
     }
}

bool iocp_server::is_timeout( 
     BOOL res
     , LPPER_IO_DATA per_io_data )
{
     const DWORD last_error = GetLastError();
     return ( res == 0 && per_io_data == 0 && last_error == WAIT_TIMEOUT );         
}

}