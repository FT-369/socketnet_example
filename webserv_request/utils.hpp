#ifndef SOCKET_HPP

# include <arpa/inet.h>
# include <sys/socket.h>
# include <sys/event.h>
# include <sys/time.h>
# include <iostream>
# include <cstring>
# include <iomanip>
# include <unistd.h>
# include <fcntl.h>
# include <vector>
# include <map>

# define PORT 8080
# define LOCALHOST "127.0.0.1"
# define BUFFER_SIZE 5

# define C_NRML "\033[0m"
# define C_BLCK "\033[30m"
# define C_RED  "\033[31m"
# define C_GREN "\033[32m"
# define C_YLLW "\033[33m"
# define C_BLUE "\033[34m"
# define C_PRPL "\033[35m"
# define C_AQUA "\033[36m"

int	error_handling(std::string error, int ret);
std::vector<std::string>	split(std::string const line, std::string const delimiter);

#endif