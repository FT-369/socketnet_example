#include "socket.hpp"

int main()
{
	struct sockaddr_in server_addr;
	socklen_t	server_addr_len = sizeof(server_addr);

	memset(&server_addr, 0, server_addr_len);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);

	int server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock == -1)
		exit(error_handling("server: socket() error", 1));

	if (bind(server_sock, (struct sockaddr*)&server_addr, server_addr_len) == -1)
		exit(error_handling("bind() error", 1));
	if (listen(server_sock, 20) == -1)
		exit(error_handling("listen() error", 1));
	fcntl(server_sock, F_SETFL, O_NONBLOCK);


	// init kqueue
	int kq = kqueue();
	if (kq == -1)
		exit(error_handling("kqueue() error", 1));
	
	std::vector<struct kevent>	change_list_vec;
	struct kevent	event_list_arr[8];
	struct kevent	change_list_server;

	// kqueue에 서버 소켓 추가
	EV_SET(&change_list_server, server_sock, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	change_list_vec.push_back(change_list_server);
	//


	// struct timeval timeout;
	std::map<int, std::string> client_message;
	while (true)
	{
		// timeout.tv_sec = 5;
		// timeout.tv_usec = 5000;
		int new_events = kevent(kq, &change_list_vec[0], change_list_vec.size(), event_list_arr, 8, NULL);
		if (new_events == -1)
			exit(error_handling("kevent() error", 1));
		change_list_vec.clear();

		for (int i = 0; i < new_events; i++)
		{
			struct kevent* curr_event = &event_list_arr[i];

			if (curr_event->flags & EV_ERROR) // 에러가 발생했다면
			{
				if (curr_event->ident == server_sock) { // 에러가 발생한 곳이 서버 소켓
					exit(error_handling("curr_event error at server socket", 1));
				} else { // 에러가 발생한 곳이 클라이언트 소켓
					error_handling("curr_event error at client socket", 1);
					std::cout << C_YLLW << "disconnnect client fd: " << C_NRML << curr_event->ident << std::endl;
					close(curr_event->ident);
					client_message.erase(curr_event->ident);
				}
				continue;
			}

			// curr_event->ident로 부터 전송받을 데이터가 있다면,
			if (curr_event->filter == EVFILT_READ)
			{
				if (curr_event->ident == server_sock) // 클라이언트에서 connect 요청이 들어온 상황
				{
					struct sockaddr_in client_addr;
					socklen_t client_addr_size = sizeof(client_addr);
					int client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_size);
					if (client_sock == -1)
						exit(error_handling("accept() error", 1));
					std::cout << C_AQUA << "connnect client fd: " << C_NRML << client_sock << std::endl;
					fcntl(client_sock, F_SETFL, O_NONBLOCK);
					
					// kqueue에 클라이언트 소켓 추가
					struct kevent	change_list_client_read, change_list_client_write;
					EV_SET(&change_list_client_read, client_sock, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
					change_list_vec.push_back(change_list_client_read);
					EV_SET(&change_list_client_write, client_sock, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
					change_list_vec.push_back(change_list_client_write);

					client_message[client_sock] = "";
				}
				else if (client_message.find(curr_event->ident) != client_message.end())
				{
					char buf[BUFFER_SIZE];
					ssize_t read_len = read(curr_event->ident, buf, BUFFER_SIZE - 1);
					if (read_len <= 0) {
						if (read_len < 0) {
							error_handling("read() error", 1);
						}
						std::cout << C_YLLW << "disconnnect client fd: " << C_NRML << curr_event->ident << std::endl;
						close(curr_event->ident);
						client_message.erase(curr_event->ident);
					} else {
						buf[read_len] = 0;
						client_message[curr_event->ident] += buf;
						std::cout << C_GREN << "Message from client[" << C_NRML << curr_event->ident << C_GREN << "]: "
							<< C_NRML << client_message[curr_event->ident] << std::endl;
					}
				}
			}
			
			if (curr_event->filter == EVFILT_WRITE) // 쓰기 이벤트
			{
				if (client_message.find(curr_event->ident) != client_message.end())
				{
					if (client_message[curr_event->ident] != "")
					{
						int fd = curr_event->ident;
						int write_len = write(fd, client_message[fd].c_str(), client_message[fd].size());
						if (write_len < 0) {
							error_handling("write() error", 1);
							std::cout << C_YLLW << "disconnnect client fd: " << C_NRML << fd << std::endl;
							close(fd);
							client_message.erase(fd);
						} else {
							client_message[fd] = "";
						}
					}
				}
			}
		}
		
	}
	close(server_sock);
	close(kq);
	return 0;
}