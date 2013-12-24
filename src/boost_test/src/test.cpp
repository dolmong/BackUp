#include <boost/thread.hpp>
#include <iostream>

void wait(
	int seconds
	)
{
	boost::this_thread::sleep( boost::posix_time::seconds(seconds) );
}

void thread()
{
	try
	{
		for( int i=0; i<5; i++ )
		{
			wait( 1 );
			std::cout << "thread: " << boost::this_thread::get_id() << " " << i << std::endl;
		}
	}
	catch( boost::thread_interrupted & )
	{
		std::cout << "thread interrupted" << std::endl;
	}
}

int main(
	int argc,
	char **argv
	)
{
	std::cout << boost::thread::hardware_concurrency() << std::endl;
	std::cout << "(main)thread main id: " << boost::this_thread::get_id() << std::endl;

	boost::thread t( thread );
	std::cout << boost::thread::hardware_concurrency() << std::endl;
	std::cout << "(main)thread id: " << t.get_id() << std::endl;

	wait( 3 );

	t.interrupt();
	t.join();

	return 0;
}
