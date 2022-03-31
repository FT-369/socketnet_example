// Client side C/C++ program to demonstrate Socket programming
// 코드 출처 : geeksforgeeks

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 8080

int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};

    // 클라이언트 측에서 소켓을 생성합니다.
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    // Address family와 port 를 설정합니다.
    // 클라이언트는 통신하려는 서버와 똑같은 address family 와 port로 설정되어 있어야합니다.
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
       
    // 소켓 통신을 위해 주어진 주소를 바이너리로 전환합니다.
    // 전환된 바이너리 주소를 addr 구조체의 sin_addr에 할당합니다.
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    // 주어진 정보들을 토대로 서버에 커넥션 요청을 합니다.
    // 서버는 listen 상태라면 해당 요청을 받아 처리할 것입니다.
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    send(sock , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
    return 0;
}