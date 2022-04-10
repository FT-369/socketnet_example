#include "socket.hpp"

int main(int argc, char **argv)
{
	if (argc > 2)
		exit(error_handling("TOO MANY ATGUMENT", 1));
	
	struct sockaddr_in	socket_addr;
	socklen_t	socket_addr_len = sizeof(socket_addr);

	memset(&socket_addr, 0, socket_addr_len);
	socket_addr.sin_family = AF_INET;
	socket_addr.sin_addr.s_addr = inet_addr(LOCALHOST);
	socket_addr.sin_port = htons(8080);


	// 소켓 생성
	int	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1)
		exit(error_handling("client: socket() error", 1));

	// 서버에 연결요청
	if (connect(socket_fd, (struct sockaddr *)&socket_addr, sizeof(socket_addr)) == -1)
		exit(error_handling("client: connect() error", 1));
	// fcntl(socket_fd, F_SETFL, O_NONBLOCK);

	// send()로 서버로 메세지 보냄
	const char	*message = argc == 1 ? "DEFAULT MESSAGE" : argv[1];
	if ((write(socket_fd, message, strlen(message))) == -1)
		exit(error_handling("client: send() error", 1));
	else
		std::cout << C_GREN << "Message to Server : " << C_NRML << message << std::endl;

	// recv()로 서버로 메세지 받음
	char	buffer[BUFFER_SIZE];
	std::cout << C_AQUA << "ECHO : " << C_NRML;
	ssize_t	recv_len;

	// while ((recv_len = read(socket_fd, buffer, BUFFER_SIZE - 1)) == BUFFER_SIZE - 1) {
	// 	buffer[BUFFER_SIZE - 1] = 0;
	// 	std::cout << buffer;
	// }
	recv_len = read(socket_fd, buffer, BUFFER_SIZE - 1);
	if (recv_len < 0) {
		std::cout << std::endl;
		error_handling("client: recv() error", 1);
	}
	else {
		buffer[recv_len] = 0;
		std::cout << buffer << std::endl;
	}

	// client 여러개 돌리기 위함
	ft_usleep(10000);
	close(socket_fd);
	return (0);
}