#include "Driver.hpp"

// Driver::Driver(std::mutex &chunk_mutex): data_mutex(chunk_mutex)
// {
// 	producer = new Producer(chunk_mutex);
// 	consumer = new Consumer(chunk_mutex);
// 	copy = new std::map<std::pair<int, int>, obj::Chunk *>();
// }

// Driver::~Driver()
// {
// 	delete producer;
// 	delete consumer;
// 	delete copy;
// }

// // Function to produce an item and
// // add it to the buffer
// void	Driver::producer(T *copy)
// {
// 	// Decrease mutex value by 1
// 	--mutex;

// 	// Increase the number of full
// 	// slots by 1
// 	++full;

// 	// Decrease the number of empty
// 	// slots by 1
// 	--empty;

// 	// Item produced
// 	x++;

// 	std::cout << "\nProducer produces item " << x << std::endl;
// 	producer.produce(data);

// 	// Increase mutex value by 1
// 	++mutex;
// }

// // Function to consume an item and
// // remove it from buffer
// template<class T>
// void	Driver::consumer(T *copy)
// {
// 	// Decrease mutex value by 1
// 	--mutex;

// 	// Decrease the number of full
// 	// slots by 1
// 	--full;

// 	// Increase the number of empty
// 	// slots by 1
// 	++empty;
// 	std::cout << "\nConsumer consumes item " << x << std::endl;
// 	consumer.consume(data);
// 	x--;

// 	// Increase mutex value by 1
// 	++mutex;
// }

// template<class T>
// void	Driver:transfer(T *data, T *copy)
// {
// 	swap(data, copy);
// 	copy.clear();
// }

// // Driver Code
// template<class T>
// void	Driver::driver(T data)
// {
// 	#pragma omp critical

// 	bool produce = true;
// 	bool consume = false;

// 	while ()
// 	{
// 		if (produce)
// 		{
// 			if ((mutex == 1) && (empty != 0))
// 				producer(copy);
// 			else
// 				std::cout << "Buffer is full!" << std::endl;
// 		}
// 		if (consume)
// 		{
// 			if ((mutex == 1) && (full != 0))
// 				consumer(copy);
// 			else
// 				std::cout << "Buffer is empty!" << std::endl;
// 		}
// 		if (!produce && !consume)
// 		{
// 			transfer(data, copy);
// 		}
// 	}
// }