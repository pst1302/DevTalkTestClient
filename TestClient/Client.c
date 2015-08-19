#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Windows.h>
#include<process.h>

#define BUF_SIZE 100
#define NAME_SIZE 20

unsigned WINAPI SendMsg(void* arg);
unsigned WINAPI RecvMsg(void* arg);
void ErrorHandling(char* message);

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];

int main(int argc, char* argv[]) {

	WSADATA wsaData;
	SOCKET hSock;
	SOCKADDR_IN servAddr;
	HANDLE hSndThread, hRcvThread;
	
	if (argc != 4) {
		printf("Usage : %s <ip> <port> <name>\n", argv[0]);
		getchar();
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WSAStartup() error!");
	}


	sprintf_s(name,sizeof(name), "[%s]", argv[3]);
	
	// 소켓 생성
	hSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hSock == INVALID_SOCKET) {
		ErrorHandling("socket() error!");
	}

	// 메모리 설정후 셋팅
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	// 서버 소켓에 연결
	if (connect(hSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		ErrorHandling("connect() error!");
	}

	hSndThread = (HANDLE)_beginthreadex(NULL, 0, SendMsg, (void*)&hSock, 0, NULL);
	hRcvThread = (HANDLE)_beginthreadex(NULL, 0, RecvMsg, (void*)&hSock, 0, NULL);

	WaitForSingleObject(hSndThread, INFINITE);
	WaitForSingleObject(hRcvThread, INFINITE);

	// 소켓 종료
	closesocket(hSock);
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

// 보내는 쓰레드 Main 함수
unsigned WINAPI SendMsg(void* arg) {
	SOCKET hSock = *((SOCKET*)arg);
	char nameMsg[NAME_SIZE + BUF_SIZE];

	while (1) {

		fgets(msg, BUF_SIZE, stdin);
		if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))
		{
			closesocket(hSock);
			exit(0);
		}
		sprintf_s(nameMsg, sizeof(nameMsg), "%s %s", name, msg);
		send(hSock, nameMsg, strlen(nameMsg), 0);
	}

	return 0;
}

// 받는 쓰레드 Main 함수
unsigned WINAPI RecvMsg(void* arg) {
	int hSock = *((SOCKET*)arg);
	char nameMsg[NAME_SIZE + BUF_SIZE];
	int strLen;

	while (1){
		strLen = recv(hSock, nameMsg, NAME_SIZE + BUF_SIZE - 1, 0);

		if (strLen == -1)
			return 0;

		nameMsg[strLen] = 0;
		fputs(nameMsg, stdout);
	}
}