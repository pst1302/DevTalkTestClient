#include<stdio.h>
#include<stdlib.h>
#include<WinSock2.h>

#define BUF_SIZE 1024

void ErrorHandling(char* message);

int main(int argc, char* argv[]) {

	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char message[30];
	int strLen;

	if (argc != 3) {
		printf("Usage : %s <ip> <port> \n", argv[0]);
		getchar();
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WSAStartup() error!");
	}

	// 소켓 생성
	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET) {
		ErrorHandling("socket() error!");
	}

	// 메모리 설정후 셋팅
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	// 서버 소켓에 연결
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		ErrorHandling("connect() error!");
	}

	// message 계속 보냄
	while (1) {
		fputs("Input message(Q to quit) : ", stdout);
		fgets(message, BUF_SIZE, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		send(hSocket, message, strlen(message), 0);
		strLen = recv(hSocket, message, BUF_SIZE - 1, 0);
		message[strLen] = 0;
		printf("Message from server : %s", message);
	}

	// 소켓 종료
	closesocket(hSocket);
	WSACleanup();


	printf("종료하시려면 아무키나 눌러주세요.");
	getchar();
	return 0;
}

void ErrorHandling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}