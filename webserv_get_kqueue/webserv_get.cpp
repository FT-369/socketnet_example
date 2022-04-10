#include "socket.hpp"
# include <sys/event.h>
# include <vector>
# include <map>

#define BUF_SIZE 1024
#define SMALL_BUF 100
#define PORT 8080

class data
{
public:
	FILE*	_client_write;
	std::string	_method;
	std::string	_content_type;
	std::string	_file_name;
public:
	data(FILE* client_write, std::string method, std::string content_type, std::string file_name)
		: _client_write(client_write), _method(method), _content_type(content_type), _file_name(file_name) {}
};

char *get_content_type(char *file)
{
	char file_name[SMALL_BUF];
	char extension[SMALL_BUF];
	char *html = ft_strdup("text/html");
	char *plain = ft_strdup("text/plain");

	strcpy(file_name, file);
	strtok(file_name, ".");
	strcpy(extension, strtok(NULL, "."));

	if (!strcmp(extension, "html") || !strcmp(extension, "htm"))
		return html;
	else
		return plain;
}

void send_data(FILE *fp, char* ct, char* file_name)
{
	char	protocol[] = "HTTP/1.0 200 OK\r\n";
	char	server[] = "Server: Mac Web Server \r\n";
	char	cnt_len[] = "Content-length:2048\r\n";
	char	cnt_type[SMALL_BUF];
	char	buf[BUF_SIZE];
	FILE*	send_file;

	std::cout << "send data!" << std::endl;
	sprintf(cnt_type, "Content-type:%s\r\n\r\n", ct);
	send_file = fopen(file_name, "r");

	if (send_file == NULL)
	{
		// send_error(fp);
		return;
	}

	// 헤더 정보 전송
	fputs(protocol, fp);
	fputs(server, fp);
	fputs(cnt_len, fp);
	fputs(cnt_type, fp);

	// 요청 데이터 전송
	while (fgets(buf, BUF_SIZE, send_file) != NULL)
	{
		fputs(buf, fp);
		fflush(fp);
	}
	fflush(fp);
	fclose(fp);
}

data *request_handler(int client_socket)
{
	char request_line[SMALL_BUF];


	FILE* client_read = fdopen(client_socket, "r");
	FILE* client_write = fdopen(dup(client_socket), "w");
	fgets(request_line, SMALL_BUF, client_read);
	std::cout << request_line << std::endl;

	if (strstr(request_line, "HTTP/") == NULL)
	{
		// send_error(client_write);
		fclose(client_read);
		fclose(client_write);
		return 0;
	}

	char method[10];
	char file_name[30];
	char content_type[15];

	strcpy(method, strtok(request_line, " /"));
	strcpy(file_name, strtok(NULL, " /"));
	strcpy(content_type, get_content_type(file_name));

	if (strcmp(method, "GET") != 0)
	{
		// send_error(client_write);
		fclose(client_read);
		fclose(client_write);
		return 0;
	}

	fclose(client_read);
	// send_data(client_write, content_type, file_name);
	return new data(client_write, method, content_type, file_name);
}


int main(int argc, char **argv)
{
	// if (argc != 2)
	// 	exit(error_handling("arg error!", 1));
	
	int server_sock, client_sock;
	struct sockaddr_in server_addr, client_addr;
	socklen_t	client_addr_size;
	pthread_t	t_id;

	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);

	if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
		error_handling("bind() error", 1);
	if (listen(server_sock, 20) == -1)
		error_handling("listen() error", 1);
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
	// std::map<int, std::string> client_message;
	// data* request;
	std::map<int, data*> request;
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
					request.erase(curr_event->ident);
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
					// EV_SET(&change_list_client_write, client_sock, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
					// change_list_vec.push_back(change_list_client_write);

					//
					request[client_sock] = 0;
				}
				else if (request.find(curr_event->ident) != request.end())
				{
					data* req = request_handler(curr_event->ident);
					if (req == 0)
						exit(error_handling("request_handler() error", 1));
					request[curr_event->ident] = req;
					send_data(req->_client_write, (char *)(req->_content_type).c_str(), (char *)(req->_file_name).c_str());
				}
			}
		}
		
	}
	close(server_sock);
	close(kq);
	return 0;
}