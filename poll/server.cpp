#include "socket.hpp"

#define PORT 8080
#define BUFFER_SIZE 100
#define LISTEN_QUEUE_SIZE 5
#define theNumberOfFDs 100

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

 
    // // pollfd 배열 구조체 생성
	struct sockaddr_in	client_addr;
	socklen_t	client_addr_size = sizeof(client_addr);
    struct pollfd pollFDs[10];
	ssize_t strLen;
	int connectFD;
	char buffer[100];
 
    pollFDs[0].fd = server_socket_fd; // 0번째 배열에는 listen을 지정
    pollFDs[0].events = POLLIN; // 읽도록 만든다.
    pollFDs[0].revents = 0; // 처음에는 0으로 초기화 한다(아직 아무 일도 일어나지 않았으니)
 
    for (int i = 1; i < 10; i++)
        pollFDs[i].fd = -1; // 0번째 배열은 listen을 위한것이니 1번째부터 모두 -1로 초기화
 
    while (1)
    {
        int result = poll(pollFDs, 10, 1000); // -1 :: 무한 대기
		std::cout << result << std::endl;
        if (result > 0)
        {
            if (pollFDs[0].revents == POLLIN)
            {
                // 새로운 커넥션 요청이 들어왔을 때
                connectFD = accept(server_socket_fd, (struct sockaddr*)&client_addr, &client_addr_size);
				if (connectFD == -1)
					error_handling("server_socket: accept() error", 1);
				else
					std::cout << get_ms_time().substr(7) << "ms" << C_YLLW << "  [6] " << C_GREN
						<<  "accept(): " << C_NRML << "accept a connect() request from a client" << std::endl;
                for (int i = 1; i < 10; i++)
                {
                    if (pollFDs[i].fd == -1) // 비어있는 fd슬롯을 찾아서 넣어준다.
                    {
                        pollFDs[i].fd = connectFD;
                        pollFDs[i].events = POLLIN;
                        pollFDs[i].revents = 0;
                        break; // 모두 다 넣고 break를 통해 한번만 실행
                    }
                }
            }
 
            for (int i = 1; i < 10; i++)
            {
				std::cout << " revents = " <<  pollFDs[i].revents << std::endl;
                switch (pollFDs[i].revents)
                {
                    // no events
                    case 0:
                        break;
 
                    // data is ready
                    case POLLIN:
						// std::cout << " i = =  " << i << std::endl;
                        strLen = read(pollFDs[i].fd, buffer, BUFFER_SIZE);
                        printf("%lu bytes read\n", strLen);
						
                        buffer[strLen] = '\0';
                        write(1, buffer, strlen(buffer));
                        // fputs(buffer, stdout);
                        // fflush(stdout);
 
                    // 슬롯 초기화
                    default:
                        close(pollFDs[i].fd);
                        pollFDs[i].fd = -1;
                        pollFDs[i].revents = 0;
                        
                            
                }
            }
        }
 
        
    }
 
    close(server_socket_fd);
 
    return 0;
}
