#ifndef iocp_server_h__
#define iocp_server_h__

#include <winsock2.h>
#include <process.h>
#include <string>
#include <exception>
#include <iostream>
#include <memory>
#include <sstream>

#include "wsa_initializer.h"

#define DATA_BUFSIZE 4096

namespace iocp
{

typedef struct _PER_HANDLE_DATA
{
     SOCKET Socket;
     // Other information useful to be associated with the handle
     SOCKADDR_STORAGE ClientAddr;
} PER_HANDLE_DATA, * LPPER_HANDLE_DATA;

enum IO_OPERATION { RECV_POSTED, SEND_POSTED };

typedef struct _PER_IO_DATA
{
     OVERLAPPED Overlapped;
     WSABUF WSABuffer;
     char Buffer[ DATA_BUFSIZE ];
     int	BufferLen;
     IO_OPERATION OperationType;
} PER_IO_DATA, * LPPER_IO_DATA;


class iocp_server
{
public:
     iocp_server( const std::string& address );

     ~iocp_server();

     void run();

private:

     HANDLE create_new_iocp();

     void associate_socket_with_iocp( SOCKET accepted_socket, sockaddr_in* remote_point, int remote_len );

     void create_working_threads();

     void join_threads();

     void create_listening_socket();

     void listen();

     static void async_receive( SOCKET accepted_socket, LPPER_IO_DATA per_io_data );

     static void process_received_bytes( DWORD number_of_bytes_recvd, LPPER_IO_DATA per_io_data, SOCKET accepted_socket );

     static unsigned __stdcall server_working_thread( void* iocp );

     static void check_completion_for_critical_error( BOOL res, LPPER_IO_DATA per_io_data );

     static bool is_timeout( BOOL res, LPPER_IO_DATA per_io_data );

private:

     wsa_initializer< systemproxy > wsa_init_;

     std::string address_;
     HANDLE iocp_;
     SOCKET listen_socket_;
     static DWORD data_wait_timeout;

     HANDLE* threads_;
     DWORD thread_count;
};

}

#endif // iocp_server_h__