//서버 프로그램
#include<iostream>
#include<string>
#include<WinSock2.h>

using namespace std;

void ShowErrorMessage(string message) {
	cout << "[오류 발생]: " << message << '\n';
	system("pause");
	exit(1); //exit()함수 안에 0이 아닌 수가 입력되는 것은 비정상적인 종료를 의미
}

int main()
{
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;

	int serverPort = 9876;
	char received[256]; //주고 받을 '문자열 버퍼(received)'의 크기

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //(1) Winsock초기화->WSAStartup()함수가 성공적으로 완료되면 0을 반환
		ShowErrorMessage("WSAStartup()");

	serverSocket = socket(PF_INET, SOCK_STREAM, 0); //(2) 서버소켓(TCP) 생성

	if (serverSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress)); //서버 주소를 전부 0으로 초기화
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); //host to network(long) ->4바이트 정수를 네트워크 바이트 형식(Big Endian)으로 변환
	serverAddress.sin_port = htons(serverPort); //host to network(short) ->2바이트 정수를 네트워크 바이트 형식(Big Endian)으로 변환

	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) //(3) 서버 소켓에 IP와 포트번호 할당
		ShowErrorMessage("bind()");
	cout << "[현재 상태] bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR) //(4)클라이언트의 연결 요청을 기다림(최대 클라이언트 수:5)
		ShowErrorMessage("listen()");
	cout << "[현재 상태] listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress); //(5)클라이언트 연결 수락 후, 통신에 사용할 소켓(clientSocket)에 저장
	cout << "[현재 상태] accept()\n";

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()");

	while (1) {
		int length = recv(clientSocket, received, sizeof(received), 0); //(6)클라이언트의 메시지를 버퍼(received)에 전달받음
		received[length] = NULL; //문자열의 마지막을 NULL값으로 설정
		cout << "[클라이언트 메시지]: " << received << '\n';
		cout << "[메시지 전송]: " << received << '\n';
		if (strcmp(received, "[exit]") == 0) {
			send(clientSocket, received, sizeof(received) - 1, 0);
			cout << "[서버 종료]\n";
			break; //전달받은 메시지가 "[exit]"인 경우 종료
		}
		send(clientSocket, received, sizeof(received) - 1, 0); //(7)유효한 메시지인 경우, 클라이언트의 메시지를 그대로 전송(Eco Program)
	}

	closesocket(clientSocket); //(8-1)클라이언트의 소켓을 먼저 닫음
	closesocket(serverSocket); //(8-2)서버의 소켓을 닫음
	WSACleanup(); //(9)Winsock을 더이상 사용하지 않음
	system("pause");
	return 0;
}