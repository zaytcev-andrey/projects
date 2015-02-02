#include <state/include/precompiled.h>

#include <state/include/context.h>

#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/locks.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

void print(const boost::system::error_code& e )
{
     if ( e != boost::asio::error::operation_aborted )
     {
          std::cout << "Hello, world!\n";
     }
     else
     {
          std::cout << "timer canceled!\n";
     }
}

void run( boost::asio::io_service& io )
{
     io.run();
}

int _tmain(int argc, _TCHAR* argv[])
{
     Context context;
     
     boost::asio::io_service io;

     boost::asio::deadline_timer t( io, boost::posix_time::seconds( 5 ) );

     t.async_wait(print);

     boost::scoped_ptr<boost::thread> trd( new boost::thread( run, boost::ref(io) ) );

     trd->join();

     t.async_wait(print);

     trd.reset( new boost::thread( run, boost::ref(io) ) );

     io.reset();

     t.cancel();
     
     trd->join();
     
     return 0;
}