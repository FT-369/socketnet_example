#include "socket.hpp"
# include <sys/event.h>
# include <vector>
# include <map>

#define BUF_SIZE 1024
#define SMALL_BUF 100
#define PORT 8080

struct kevent_r{
    int     kq;
    int     nchanges;
    int     nevents;
    std::vector<struct kevent> changeList;
    struct kevent   eventList[10];
    std::map<int, std::string> saved_fd; // 클래스에 넣어야 할까요

    kevent_r() : kq(0), nchanges(0), nevents(10) {}

};



class event{
    private :

    public  :
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
        event() {};

        void    changeEvent(std::vector<struct kevent> &k, uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata, int *nchange)
        {
            struct kevent temp_event;
            EV_SET(&temp_event, ident, filter, flags, fflags, data, udata);
            k.push_back(temp_event);
            (*nchange)++;
        };

        void    changes_clear(struct kevent_r &ke)
        {
            ke.changeList.clear();
            ke.nchanges = 0;
        }

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
			// std::cout << request_line << std::endl;

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

        void    accept_get_client_fd(struct kevent_r &ke, int server_socket_fd, struct sockaddr_in client_addr, socklen_t client_addr_size)
        {
            int connectFD = 0;

            connectFD = accept(server_socket_fd, (struct sockaddr*)&client_addr, &client_addr_size);
            if (connectFD == -1)
                error_handling("server_socket: accept() error", 1);
            else
                std::cout << get_ms_time().substr(7) << "ms" << C_YLLW << "  [6] " << C_GREN
                    <<  "accept(): " << C_NRML << "accept a connect() request from a client" << std::endl;
            fcntl(connectFD, F_SETFL, O_NONBLOCK);
            changeEvent(ke.changeList, connectFD, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL, &ke.nchanges);
            changeEvent(ke.changeList, connectFD, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL, &ke.nchanges);
            ke.saved_fd[connectFD] = "";
        }

        void    kevent_get_event(struct kevent_r &ke, int server_socket_fd)
        {
            int new_event = 0;
			std::map<int, data*> request;

            ke.kq = kqueue();
            if (ke.kq == -1)
                std::cout << " errorororororo" << std::endl;
            changeEvent(ke.changeList, server_socket_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL, &ke.nchanges);
            struct sockaddr_in	client_addr;
            socklen_t	client_addr_size = sizeof(client_addr);
            ssize_t strLen;
            char buffer[100];
			data* req;

            while (1)
            {
                // std::cout << ke.nchanges << std::endl;
                new_event = kevent(ke.kq, &ke.changeList[0], ke.nchanges, ke.eventList, ke.nevents, 0);
                changes_clear(ke);
                if (new_event == -1)
                {
                    std::cout << "errrororroroor" << std::endl;
                    exit(1);
                }
                memset(buffer, 0x00, 100);
                for (int i = 0; i < new_event; i++)
                {
                    switch (ke.eventList[i].filter)
                    {
                        case EV_ERROR:
                            std::cout << "errorororor" << std::endl;

                        case EVFILT_READ:
                            if (ke.eventList[i].ident == (uintptr_t)server_socket_fd)
                            {
                                accept_get_client_fd(ke, server_socket_fd, client_addr, client_addr_size);
                                // std::cout << ke.nchanges << std::endl;
                            }
                            else if(ke.saved_fd.find(ke.eventList[i].ident) != ke.saved_fd.end())
                            {
								req = request_handler(ke.eventList[i].ident);
								if (req == 0)
									exit(error_handling("request_handler() error", 1));
								request[ke.eventList[i].ident] = req;
                                    ke.saved_fd[ke.eventList[i].ident] += "hello";
                                // strLen = read(ke.eventList[i].ident, buffer, 100);
                                // if (strLen > 0)
                                // {
                                //     write(1, buffer, strlen(buffer));
                                // }
                            }

                        case EVFILT_WRITE:
                            if (ke.saved_fd[ke.eventList[i].ident] != "")
                            {
								send_data(req->_client_write, (char *)(req->_content_type).c_str(), (char *)(req->_file_name).c_str());
                                // write(ke.eventList[i].ident, buffer, strlen(buffer));
                                ke.saved_fd.erase(ke.eventList[i].ident);
                                close(ke.eventList[i].ident);
                                std::cout << "i'm here" << std::endl;
                            }
                            // std::cout << "write" << std::endl;
                        // 구현
                    }
                }
            }
        }
        
};


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
	// kqueue에 서버 소켓 추가
	
	//


	// struct timeval timeout;
	// std::map<int, std::string> client_message;
	// data* request;
	struct kevent_r ev;
    event ke;
    ke.kevent_get_event(ev, server_sock);
	close(server_sock);
	// close(kq);
	return 0;
}