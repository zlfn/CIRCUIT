//GNU GPL 3.0 lisence
/*
 * 정보 수행평가 게임
 * Copyright (C) 2022 박찬웅, 김진서, 박지환
 *
 * 이 프로그램은 자유 소프트웨어입니다. 소프트웨어의 피양도자는 자유 소프트웨어
 * 재단이 공표한 GNU 일반 공중 사용 허가서 3판 혹은 그 이후 판을 임의로 선택하여
 * 그 규정에 따라 프로그램을 개작하거나 재배포할 수 있습니다.
 *
 * 이 프로그램은 유용하게 사용될 수 있으리라는 희망에서 배포되고 있지만, 특정한
 * 목적에 맞는 적합성 여부나 판매용으로 사용할 수 있으리라는 묵시적인 보증을 포함한
 * 어떠한 형태의 보증도 제공하지 않습니다. 보다 자세한 사항에 대해서는
 * GNU 일반 공중 허가서를 참고하시기 바랍니다.
 *
 * GNU 일반 공중 사용 허가서는 이 프로그램과 함께 제공됩니다. 만약 문서가 누락되어있다면
 * <http://www.gnu.org/licenses/>을 참조하시기 바랍니다.
 */

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define BUFFER_SIZE 256
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include "Network.h"

#pragma comment(lib, "ws2_32")

using namespace std;

int serveTCP(const char* message, char* buffer, int bufferSize, IPV4* clientIP, int port)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET hListen;
	hListen = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in tListenAddr;
	tListenAddr.sin_family = AF_INET;
	tListenAddr.sin_port = htons(port);
	tListenAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(hListen, (SOCKADDR*)&tListenAddr, sizeof(tListenAddr));
	listen(hListen, SOMAXCONN);

	SOCKADDR_IN tClntAddr;
	int iClntSize = sizeof(tClntAddr);
	SOCKET hClient = accept(hListen, (SOCKADDR*)&tClntAddr, &iClntSize);

	recv(hClient, buffer, bufferSize, 0);
	send(hClient, message, strlen(message), 0);

	clientIP->b1 = tClntAddr.sin_addr.S_un.S_un_b.s_b1;
	clientIP->b2 = tClntAddr.sin_addr.S_un.S_un_b.s_b2;
	clientIP->b3 = tClntAddr.sin_addr.S_un.S_un_b.s_b3;
	clientIP->b4 = tClntAddr.sin_addr.S_un.S_un_b.s_b4;

	closesocket(hClient);
	closesocket(hListen);

	WSACleanup();
	return 0;
}

int requestTCP(const char* message, char* buffer, int bufferSize, IPV4 serverIP, int port)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET hSocket;
	hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN tAddr;
	tAddr.sin_family = AF_INET;
	tAddr.sin_port = htons(port);

	char cServerIP[100];
	sprintf_s(cServerIP, sizeof(cServerIP), "%d.%d.%d.%d", serverIP.b1, serverIP.b2, serverIP.b3, serverIP.b4);
	tAddr.sin_addr.s_addr = inet_addr(cServerIP);

	connect(hSocket, (SOCKADDR*)&tAddr, sizeof(tAddr));

	send(hSocket, message, strlen(message), 0);
	recv(hSocket, buffer, bufferSize, 0);

	closesocket(hSocket);

	WSACleanup();
	return 0;
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

int sendUDPMessage(const char* message, IPV4 destination, int port)
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
	char cDestination[100];
	sprintf_s(cDestination, sizeof(cDestination), "%d.%d.%d.%d", destination.b1, destination.b2, destination.b3, destination.b4);
	sockaddr1.sin_addr.S_un.S_addr = inet_addr(cDestination);
	sockaddr1.sin_port = htons(port);
	
	sendto(sock, message, strlen(message), 0, (const sockaddr*)&sockaddr1, sizeof(sockaddr_in));
	closesocket(sock);
	WSACleanup();

	return 0;

}

int receiveUDPMessage(char* buffer, IPV4* ip, int limitTime, int port)
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
