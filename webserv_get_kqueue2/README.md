# kqueue를 이용한 Multiplexing WebServer 구현

## 실행방법

1. `webserver` 프로그램 만들기

```sh
make
```

<br>

2. `webserver` 프로그램 실행
```sh
./server
```
- 포트번호는 `8080` 으로 고정
- 포트번호를 바꾸고 싶다면 `socket.hpp` 16번째줄 `# define PORT` 변경

<br>

3. 웹브라우저에서 접속
```sh
http://localhost:[포트번호]/[파일이름].html
```
- 포트번호는 `8080` 으로 고정
- 파일은 같은 루트에 있는 `index.html` 혹은, `index2.html` 이용
- 다른 파일을 보고 싶다면 루트에 다른 `.html` 파일 저장

<br>

## 기타 참고 사항

1. 3번을 실행할 때 클라이언트 socket fd가 여러개 생김
2. 불안정함, 같은 행동을 해도 html이 전송 안될 때가 있음
