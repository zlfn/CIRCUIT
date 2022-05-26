#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <ws2tcpip.h>
#include "Network.h"

#pragma comment(lib, "ws2_32")

#define PACKET_SIZE 100
#define BUFFER_SIZE 256

using namespace std;

void sendDataTCP()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET hListen;
	hListen = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN tListenAddr = {};
	tListenAddr.sin_family = AF_INET;
	tListenAddr.sin_port = htons(1102);
	tListenAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(hListen, (SOCKADDR*)&tListenAddr, sizeof(tListenAddr));
	listen(hListen, SOMAXCONN);

	SOCKADDR_IN tClntAddr = {};
	int iClntSize = sizeof(tClntAddr);
	SOCKET hClient = accept(hListen, (SOCKADDR*)&tClntAddr, &iClntSize);

	char cBuffer[PACKET_SIZE] = {};
	//recv(hClient, cBuffer, PACKET_SIZE, 0);
	char cMsg[] = "Server Send";
	send(hClient, cMsg, strlen(cMsg), 0);

	closesocket(hClient);
	closesocket(hListen);

	WSACleanup();
}

int sendUDPBroadcast(const char* message ,int port)
{
	WSADATA wsaData;
	int fBroadcast = 1;
	sockaddr_in sockaddr1;
	sockaddr_in sockaddr2;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (const char*)&fBroadcast, sizeof(fBroadcast));
	memset(&sockaddr1, 0, sizeof(sockaddr_in));
	sockaddr1.sin_family = AF_INET;
	sockaddr1.sin_addr.S_un.S_addr = inet_addr("255.255.255.255");
	sockaddr1.sin_port = htons(port);
	
	sendto(sock, message, strlen(message), 0, (const sockaddr*)&sockaddr1, sizeof(sockaddr_in));
	closesocket(sock);
	WSACleanup();

	return 0;
}

int receiveUDPBroadcast(char* buffer, IPV4* ip, int limitTime, int port)
{
	SSIZE_T bufferSize;

	WSADATA wsaData;
	SOCKET sock;
	timeval timeout;
	socklen_t socklen1;
	sockaddr_in sockaddr1;
	socklen_t socklen2;
	sockaddr_in sockaddr2;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	memset(&timeout, 0, sizeof(timeval));
	timeout.tv_sec = limitTime;
	timeout.tv_usec = 0;

	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeval));
	memset(&sockaddr1, 0, sizeof(sockaddr_in));
	socklen1 = sizeof(struct sockaddr_in);
	sockaddr1.sin_family = AF_INET;
	sockaddr1.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	sockaddr1.sin_port = htons(port);

	//정해진 시간만큼 수신을 기다립니다.
	bind(sock, (sockaddr*)&sockaddr1, sizeof(sockaddr_in));
	memset(buffer, 0, BUFFER_SIZE);
	memset(&sockaddr2, 0, sizeof(struct sockaddr_in));
	socklen2 = sizeof(sockaddr_in);
	bufferSize = recvfrom(sock, buffer, BUFFER_SIZE, 0, (sockaddr*)&sockaddr2, (int*)&socklen2);
	closesocket(sock);

	if (bufferSize < 0) return -1;
	
	ip->b1 = sockaddr2.sin_addr.S_un.S_un_b.s_b1;
	ip->b2 = sockaddr2.sin_addr.S_un.S_un_b.s_b2;
	ip->b3 = sockaddr2.sin_addr.S_un.S_un_b.s_b3;
	ip->b4 = sockaddr2.sin_addr.S_un.S_un_b.s_b4;

	WSACleanup();

	return 0;
}
