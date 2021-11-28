//���� ���α׷�
#include<iostream>
#include<string>
#include<WinSock2.h>

using namespace std;

void ShowErrorMessage(string message) {
	cout << "[���� �߻�]: " << message << '\n';
	system("pause");
	exit(1); //exit()�Լ� �ȿ� 0�� �ƴ� ���� �ԷµǴ� ���� ���������� ���Ḧ �ǹ�
}

int main()
{
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;

	int serverPort = 9876;
	char received[256]; //�ְ� ���� '���ڿ� ����(received)'�� ũ��

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //(1) Winsock�ʱ�ȭ->WSAStartup()�Լ��� ���������� �Ϸ�Ǹ� 0�� ��ȯ
		ShowErrorMessage("WSAStartup()");

	serverSocket = socket(PF_INET, SOCK_STREAM, 0); //(2) ��������(TCP) ����

	if (serverSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress)); //���� �ּҸ� ���� 0���� �ʱ�ȭ
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); //host to network(long) ->4����Ʈ ������ ��Ʈ��ũ ����Ʈ ����(Big Endian)���� ��ȯ
	serverAddress.sin_port = htons(serverPort); //host to network(short) ->2����Ʈ ������ ��Ʈ��ũ ����Ʈ ����(Big Endian)���� ��ȯ

	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) //(3) ���� ���Ͽ� IP�� ��Ʈ��ȣ �Ҵ�
		ShowErrorMessage("bind()");
	cout << "[���� ����] bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR) //(4)Ŭ���̾�Ʈ�� ���� ��û�� ��ٸ�(�ִ� Ŭ���̾�Ʈ ��:5)
		ShowErrorMessage("listen()");
	cout << "[���� ����] listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress); //(5)Ŭ���̾�Ʈ ���� ���� ��, ��ſ� ����� ����(clientSocket)�� ����
	cout << "[���� ����] accept()\n";

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()");

	while (1) {
		int length = recv(clientSocket, received, sizeof(received), 0); //(6)Ŭ���̾�Ʈ�� �޽����� ����(received)�� ���޹���
		received[length] = NULL; //���ڿ��� �������� NULL������ ����
		cout << "[Ŭ���̾�Ʈ �޽���]: " << received << '\n';
		cout << "[�޽��� ����]: " << received << '\n';
		if (strcmp(received, "[exit]") == 0) {
			send(clientSocket, received, sizeof(received) - 1, 0);
			cout << "[���� ����]\n";
			break; //���޹��� �޽����� "[exit]"�� ��� ����
		}
		send(clientSocket, received, sizeof(received) - 1, 0); //(7)��ȿ�� �޽����� ���, Ŭ���̾�Ʈ�� �޽����� �״�� ����(Eco Program)
	}

	closesocket(clientSocket); //(8-1)Ŭ���̾�Ʈ�� ������ ���� ����
	closesocket(serverSocket); //(8-2)������ ������ ����
	WSACleanup(); //(9)Winsock�� ���̻� ������� ����
	system("pause");
	return 0;
}