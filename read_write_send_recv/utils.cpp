#include "socket.hpp"

int	error_handling(std::string error, int ret) {
	std::cerr << error << std::endl;
	return(ret);
}

std::string	get_ms_time()
{
	struct timeval	time;
	double			time_ms;

	gettimeofday(&time, 0);
	time_ms = time.tv_sec * 1000 + ((double)time.tv_usec / 1000);
	return (std::to_string(time_ms));
}
