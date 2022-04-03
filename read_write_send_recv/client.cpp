#include "socket.hpp"

int main(int argc, char **argv)
{
	if (argc != 3)
		exit(error_handling("arg error", 1));


	// 소켓 생성
	// 소켓을 생성하는 순간에는 서버와 클라이언트의 구분이 없다.
	// bind, listen 함수의 호출이 이어지면 서버 소켓이, connect 함수의 호출이 이어지면 클라이언트 소켓이 된다.
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1)
		exit(error_handling("client_socket: socket() error", 1));
	else
		std::cout << get_ms_time().substr(7) << "ms" << C_YLLW << "  [4] " << C_GREN
			<< "socket() : " << C_NRML << "create client socket!" << std::endl;
	// fcntl(socket_fd, F_SETFL, O_NONBLOCK);
	
	struct sockaddr_in	server_addr;

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(std::atoi(argv[2]));

	// 서버에 연결요청
	if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
		error_handling("client_socket: connect() error", 1);
	else
		std::cout << get_ms_time().substr(7) << "ms" << C_YLLW << "  [5] " << C_GREN
			<< "connect(): " << C_NRML << "send a connection request to the server." << std::endl;

	// read()로 서버가 보낸 메세지를 읽어옴
	char buffer[30];
	ssize_t read_len = read(socket_fd, buffer, sizeof(buffer) - 1);
	if (read_len == -1)
		error_handling("client_socket: read() error", 1);
	std::cout << std::endl;
	std::cout << get_ms_time().substr(7) << "ms" << C_YLLW << "  [2] " << C_AQUA
		<<  "Read Message from Server: " << C_NRML << buffer << std::endl;

	// write()로 서버에게 메세지를 보냄
	char	message[] = "Hi Server! I'm Client.";
	write(socket_fd, message, sizeof(message));
	std::cout << get_ms_time().substr(7) << "ms" << C_YLLW << "  [3] " << C_AQUA
		<<  "Write Message to Server : " << C_NRML << message << std::endl;


	// recv()로 서버가 보낸 메세지를 읽어옴
	char buf[5]; ssize_t recv_len;
	std::cout << get_ms_time().substr(7) << "ms" << C_YLLW << "  [6] " << C_AQUA <<  "Recv Message from Server: " << C_NRML;
	
	while ((recv_len = recv(socket_fd, buf, sizeof(buf) - 1, 0)) == sizeof(buf) - 1)
		std::cout << buf;

	if (recv_len < 0)
		error_handling("client_socket: recv() error", 1);
	else
		std::cout << buf << C_RED << " [eof]" <<  C_NRML << std::endl;

	// send()로 서버가 보낸 메세지를 읽어옴
	char	message2[] = "Nice meet to you, too!\n";
	if ((send(socket_fd, message2, sizeof(message2), 0)) == -1)
		exit(error_handling("client_socket: send() error", 1));
	else
		std::cout << get_ms_time().substr(7) << "ms" << C_YLLW << "  [7] " << C_AQUA
			<<  "Send Message to Server  : " << C_NRML << message2 << std::endl;

	close(socket_fd);
	return (0);
}