#include "socket.hpp"

/*
typedef __uint8_t               sa_family_t;
typedef __uint16_t              in_port_t;
typedef __uint32_t              in_addr_t;
*/

/*
 * Internet address (a structure for historical reasons)

struct in_addr {
	in_addr_t s_addr;
};
*/

/*
 * Socket address, internet style.

// IPv4 주소 체계에서 사용하는 구조체

struct sockaddr_in (16 byte)
{
	__uint8_t       sin_len;        // 전체 sockaddr_in 구조체 길이  (1 byte)
	sa_family_t     sin_family;     // 주소 체계  (1 byte)
	in_port_t       sin_port;       // 포트 번호, uint16_t, network byte order  (2 byte)
	struct  in_addr sin_addr;       // IP 주소, uint32_t, network byte order  (4 byte)
	char            sin_zero[8];    // struct sockaddr과 패딩 맞추기용, 보통 0으로 설정  (8 byte)
};
*/

/*
 * [XSI] Structure used by kernel to store most addresses.
struct sockaddr (16 byte)
{
	__uint8_t       sa_len;         // total length  (1 byte)
	sa_family_t     sa_family;      // [XSI] address family  (1 byte)
	char            sa_data[14];    // [XSI] addr value (actually larger), IP 주소 + 포트  (14byte)
};
*/

int main(int argc, char **argv)
{
	if (argc != 2)
		exit(error_handling("arg error", 1));


	// 소켓 생성
	int	server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket_fd == -1)
		exit(error_handling("server_socket: socket() error", 1));
	else
		std::cout << get_ms_time().substr(7) << "ms" << C_YLLW << "  [1] " << C_GREN
			<< "socket(): " << C_NRML << "create server socket!" << std::endl;


	struct sockaddr_in	server_addr;

	// 구조체의 모든 값을 0으로 초기화
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(std::atoi(argv[1]));
	
	// 소켓에 주소 할당
	if (bind(server_socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
		error_handling("server_socket: bind() error", 1);
	else
		std::cout << get_ms_time().substr(7) << "ms" << C_YLLW << "  [2] " << C_GREN
			<< "bind()  : " << C_NRML << "bind address to server socket." << std::endl;

	// 16 byte로 사이즈가 똑같기 때문에 sockaddr_in 에서 sockaddr 로 캐스팅이 가능
	// sockaddr 대신 sockaddr_in 을 선언하여 사용하는 것은 사용자 편의 때문
	// sockaddr -> IP + port 를 한번에 저장, sockaddr_in -> IP와 port를 나눠서 저장

	// 소켓이 연결 요청을 받을 수 있는 상태가 됨
	if (listen(server_socket_fd, 5) == -1)
		error_handling("server_socket: listen() error", 1);
	else
		std::cout << get_ms_time().substr(7) << "ms" << C_YLLW << "  [3] " << C_GREN
			<< "listen(): " << C_NRML  << "server can listen to client's requests, waiting for client's request..." << std::endl;


	struct sockaddr_in	client_addr;
	socklen_t	client_addr_size = sizeof(client_addr);

	// 클라이언트의 연결 요청을 수락, 연결 요청이 있을때까지 함수는 반환하지 않고 기다린다.
	int	client_socket_fd = accept(server_socket_fd, (struct sockaddr *)&client_addr, &client_addr_size);
	if (client_socket_fd == -1)
		error_handling("server_socket: accept() error", 1);
	else
		std::cout << get_ms_time().substr(7) << "ms" << C_YLLW << "  [6] " << C_GREN
			<<  "accept(): " << C_NRML << "accept a connect() request from a client" << std::endl;
	

	// accept 함수를 지났다는 것은, 연결요청이 들어왔다는 뜻. 즉, write로 데이터 전송 가능
	char	message1[] = "Hello Client! I'm Sever.";
	write(client_socket_fd, message1, sizeof(message1));
	std::cout << std::endl;
	std::cout << get_ms_time().substr(7) << "ms" << C_YLLW << "  [1] " << C_AQUA
		<< "Write Message to Client : " << C_NRML << message1 << std::endl;

	// read()로 클라이언트가 보낸 메세지를 읽어옴
	char buffer[30];
	int response_len = read(client_socket_fd, buffer, sizeof(buffer) - 1);
	if (response_len == -1)
		error_handling("server_socket: read() error", 1);
	std::cout << get_ms_time().substr(7) << "ms" << C_YLLW << "  [4] " << C_AQUA
		<< "Read Message from Client: " << C_NRML << buffer << std::endl;

	// send()로 클라이언트가에게 메세지를 보냄
	char	message2[] = "Nice to meet you, client.";
	send(client_socket_fd, message2, sizeof(message2), 0);
	std::cout << get_ms_time().substr(7) << "ms" << C_YLLW << "  [5] " << C_AQUA
		<< "Send Message to Client  : " << C_NRML << message2 << std::endl;

	// recv()로 클라이언트가 보낸 메세지를 읽어옴
	char buf[30];
	response_len = recv(client_socket_fd, buf, sizeof(buf) - 1, 0);
	if (response_len == -1)
		error_handling("server_socket: recv() error", 1);
	std::cout << get_ms_time().substr(7) << "ms" << C_YLLW << "  [8] " << C_AQUA
		<< "Recv Message from Client: " << C_NRML << buffer << std::endl;

	close(client_socket_fd);
	close(server_socket_fd);
	return (0);
}