#ifndef SOCKET_HPP

# include <iostream>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <cstring>	// memset
# include <unistd.h>	// write, close
# include <iomanip>	// setw, setfill
# include <sys/time.h> // gettimeofday
#include <fcntl.h>

# define C_NRML "\033[0m"
# define C_BLCK "\033[30m"
# define C_RED  "\033[31m"
# define C_GREN "\033[32m"
# define C_YLLW "\033[33m"
# define C_BLUE "\033[34m"
# define C_PRPL "\033[35m"
# define C_AQUA "\033[36m"

int	error_handling(std::string error, int ret);
std::string	get_ms_time();

#endif