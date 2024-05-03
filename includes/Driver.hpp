#ifndef __DRIVER_HPP__
# define __DRIVER_HPP__

# include "Producer.hpp"
# include "Consumer.hpp"

// class Driver
// {
// 	public:
// 		// Initialize a mutex to 1
// 		int mutex = 1;
// 		// Number of full slots as 0
// 		int full = 0;
// 		// Number of empty slots as size of buffer
// 		int empty = 10, x = 0;

// 		Producer	*producer;
// 		Consumer	*consumer;

// 		std::mutex	&data_mutex;
// 		std::map<std::pair<int, int>, obj::Chunk *>	*copy;

// 		Driver(std::mutex &chunk_mutex);
// 		~Driver();

// 		void	base_producer(std::map<std::pair<int, int>, obj::Chunk *> *copy);
// 		void	base_consumer(std::map<std::pair<int, int>, obj::Chunk *> *copy);
// 		void	transfer(std::map<std::pair<int, int>, obj::Chunk *> *data, std::map<std::pair<int, int>, obj::Chunk *> *copy);
// 		void	driver(std::map<std::pair<int, int>, obj::Chunk *> *data);
// };

#endif
