# kqueue를 이용한 Multiplexing Echo Server 구현

## 실행방법

1. `server` 및 `client` 프로그램 만들기

```sh
make
```

<br>

2. `server` 프로그램 실행
```sh
./server
```
- 포트번호는 `8080` 으로 고정
- 포트번호를 바꾸고 싶다면 `socket.hpp` 16번째줄 `# define PORT` 변경

<br>

3. `client` 프로그램 실행
```sh
./client [서버에 전송하고 싶은 문자열]
```
- 포트번호는 `8080`, IP 주소는 `127.0.0.1` 으로 고정
- IP 주소를 바꾸고 싶다면 `socket.hpp` 17번째줄 `# define LOCALHOST` 변경

<br>

## 기타 참고 사항

1. `client` 프로그램은 `kqueue_client.cpp` 를 기반으로 작동합니다.
