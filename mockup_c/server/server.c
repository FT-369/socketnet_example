// Server side C/C++ program to demonstrate Socket programming
// 코드 출처 : geeksforgeeks

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
       
    // 소켓을 생성합니다.
    // AF_INET 으로 IPv4 기반으로 설정하고
    // SOCK_STREAM 으로 연결기반 소켓을 생성합니다.
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    // addr 구조체에 타입에 맞는 소켓 구조체를 가져옵니다
    // if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    // {
    //     perror("setsockopt");
    //     exit(EXIT_FAILURE);
    // }
    // Address Family를 설정하고
    // htons(8080)으로 포트를 설정합니다.
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
       
    // 소켓 바인딩 부분입니다.
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    // 이제 설정한 소켓을 활성화합니다.
    // 이 상태에서 비로소 클라이언트로 부터 오는 요청을 받아들일 준비가 됩니다.
    // listen을 실행시키면 서버는 클라이언트의 커넥션을 받는 대기상태로 전환합니다.
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    // 클라이언트로부터 오는 요청을 받아들이고, 클라이언트와의 데이터 전송을 위해 connection_fd를 생성합니다.
    // 생성된 connection_fd를 통해 데이터를 파이프처럼 주고 받습니다.
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    // 버퍼에 전송받은 값을 카피하고 띄웁니다.
    valread = read( new_socket , buffer, 1024);
    printf("%s\n",buffer );
    send(new_socket , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    return 0;
}