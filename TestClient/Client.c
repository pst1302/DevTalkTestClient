#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Windows.h>
#include<process.h>

#define BUF_SIZE 100

void ErrorHandling(char* message);

int main(int argc, char* argv[]) {

	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAdr;

	char message[BUF_SIZE];
	int strLen;

	if (argc != 3) {
		printf("Usage : %s <ip> <port>\n", argv[0]);
		getchar();
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WSAStartup() error!");
	}

	// ���� ����
	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET) {
		ErrorHandling("socket() error!");
	}

	// �޸� ������ ����
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr(argv[1]);
	servAdr.sin_port = htons(atoi(argv[2]));

	// ���� ���Ͽ� ����
	if (connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR) {
		ErrorHandling("connect() error!");
	}
	else
		puts("Connect........");

	while (1)
	{
		fputs("Input message(Q to quit) : ", stdout);
		fgets(message, BUF_SIZE, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;


		send(hSocket, message, strlen(message), 0);
		strLen = recv(hSocket, message, BUF_SIZE - 1, 0);
		message[strLen] = 0;
		printf("Message from server: %s", message);
	}

	// ���� ����
	closesocket(hSocket);
	WSACleanup();


	printf("�����Ͻ÷��� �ƹ�Ű�� �����ּ���.");
	getchar();
	return 0;
}

void ErrorHandling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}