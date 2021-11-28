//클라이언트 프로그램
#define _WINSOCK_DEPRECATED_NO_WARNINGS //deprecated처리된 함수(ex.inet_addr())를 사용하겠다고 명시
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
	SOCKET clientSocket;
	SOCKADDR_IN serverAddress;

	int serverPort = 9876; //(*)서버와 동일한 포트번호를 사용해야 함
	char received[256];
	string sent;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //(1)Winsock을 초기화
		ShowErrorMessage("WSAStartup()");

	clientSocket = socket(PF_INET, SOCK_STREAM, 0); //(2)TCP 소켓 생성

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress)); //서버주소를 전부 0으로 초기화
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); //inet_addr: 문자열ip를 네트워크 형식으로 변환(매개변수로 'IP주소 형태로 된 문자열'을 전달받음)
	serverAddress.sin_port = htons(serverPort); //2바이트 정수(포트번호)를 네트워크 바이트 형식(Big Endian)으로 변환

	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) //(3)서버에 연결 요청
		ShowErrorMessage("connect()");
	cout << "[현재 상태] connect()\n";

	while (1) {
		cout << "[메시지 전송]: ";
		getline(cin, sent); //sent문자열에 전송할 메시지를 입력
		if (sent == " ")continue;
		send(clientSocket, sent.c_str(), sent.length(), 0); //(4)스트링을 문자열 버퍼 형태로 변환(.c_str())하여 서버로 전송
		int length = recv(clientSocket, received, sizeof(received), 0); //(5)서버로부터 다시 돌아온 데이터를 문자열(received)에 담음
		received[length] = '\0'; //문자열의 마지막을 NULL값으로 설정
		if (strcmp(received, "[exit]") == 0) {
			cout << "[서버 종료]\n";
			break; //전달받은 메시지가 "[exit]"인 경우 종료
		}
		cout << "[서버 메시지]: " << received << '\n';
	}
	closesocket(clientSocket); //(6)클라이언트 소켓 닫음
	WSACleanup(); //(7)Winsock을 더이상 사용하지 않음
	system("pause");
	return 0;
}