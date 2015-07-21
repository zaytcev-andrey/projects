// iocp_server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "iocp_server.h"

#include <string>


int _tmain(int argc, _TCHAR* argv[])
{
	if ( argc < 2 )
     {
          std::cout << "must have one parameter" << std::endl;
          return 0;
     }
     
     const std::string address = argv[ 1 ];
     iocp::iocp_server server( address );
     server.run();
     
     return 0;
}

