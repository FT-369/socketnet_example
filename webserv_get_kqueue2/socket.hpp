#ifndef SOCKET_HPP

# include <iostream>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <cstring>
# include <unistd.h>
# include <iomanip>
# include <sys/time.h>
# include <fcntl.h>
# include <sys/event.h>

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
std::string	get_ms_time();
void	ft_usleep(double time);
char	*ft_strdup(const char *s1);
char	*ft_strjoin(char const *s1, char const *s2);

#endif