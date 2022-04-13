#include "socket.hpp"

int	error_handling(std::string error, int ret)
{
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

void	ft_usleep(double time)
{
	double	start;

	start = std::stod(get_ms_time());
	while (time >= std::stod(get_ms_time()) - start)
	{
		usleep(10);
	}
}

char	*ft_strdup(const char *s1)
{
	char	*str = 0;
	size_t	count;

	count = s1 == 0 ? 0 : strlen(s1);
	std::cout << count << std::endl;
	str = (char *)malloc(sizeof(char) * (count + 1));
	if (str == 0)
		return (0);
	for (int i = 0; i < count; i++)
		str[i] = s1[i];
	str[count] = 0;
	return (str);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	char	*copy_s1;
	char	*copy_s2;
	size_t	len1, len2, index, i;

	copy_s1 = ft_strdup(s1);
	copy_s2 = ft_strdup(s2);
	len1 = strlen(copy_s1);
	len2 = strlen(copy_s2);
	str = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
	if (str == NULL)
		return (NULL);
	index = 0;
	i = 0;
	while (i < len1)
		str[index++] = s1[i++];
	i = 0;
	while (i < len2)
		str[index++] = s2[i++];
	str[index] = '\0';
	free(copy_s1);
	free(copy_s2);
	return (str);
}