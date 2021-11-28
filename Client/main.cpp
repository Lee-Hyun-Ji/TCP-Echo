//Ŭ���̾�Ʈ ���α׷�
#define _WINSOCK_DEPRECATED_NO_WARNINGS //deprecatedó���� �Լ�(ex.inet_addr())�� ����ϰڴٰ� ���
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
	SOCKET clientSocket;
	SOCKADDR_IN serverAddress;

	int serverPort = 9876; //(*)������ ������ ��Ʈ��ȣ�� ����ؾ� ��
	char received[256];
	string sent;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //(1)Winsock�� �ʱ�ȭ
		ShowErrorMessage("WSAStartup()");

	clientSocket = socket(PF_INET, SOCK_STREAM, 0); //(2)TCP ���� ����

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress)); //�����ּҸ� ���� 0���� �ʱ�ȭ
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); //inet_addr: ���ڿ�ip�� ��Ʈ��ũ �������� ��ȯ(�Ű������� 'IP�ּ� ���·� �� ���ڿ�'�� ���޹���)
	serverAddress.sin_port = htons(serverPort); //2����Ʈ ����(��Ʈ��ȣ)�� ��Ʈ��ũ ����Ʈ ����(Big Endian)���� ��ȯ

	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) //(3)������ ���� ��û
		ShowErrorMessage("connect()");
	cout << "[���� ����] connect()\n";

	while (1) {
		cout << "[�޽��� ����]: ";
		getline(cin, sent); //sent���ڿ��� ������ �޽����� �Է�
		if (sent == " ")continue;
		send(clientSocket, sent.c_str(), sent.length(), 0); //(4)��Ʈ���� ���ڿ� ���� ���·� ��ȯ(.c_str())�Ͽ� ������ ����
		int length = recv(clientSocket, received, sizeof(received), 0); //(5)�����κ��� �ٽ� ���ƿ� �����͸� ���ڿ�(received)�� ����
		received[length] = '\0'; //���ڿ��� �������� NULL������ ����
		if (strcmp(received, "[exit]") == 0) {
			cout << "[���� ����]\n";
			break; //���޹��� �޽����� "[exit]"�� ��� ����
		}
		cout << "[���� �޽���]: " << received << '\n';
	}
	closesocket(clientSocket); //(6)Ŭ���̾�Ʈ ���� ����
	WSACleanup(); //(7)Winsock�� ���̻� ������� ����
	system("pause");
	return 0;
}