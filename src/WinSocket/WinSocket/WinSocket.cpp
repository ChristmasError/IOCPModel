#include<thread>
#include<Windows.h>
#include<iostream>
#include"WinSocket.h"
#define socklen_t int

WinSocket::WinSocket()
{
	static bool first = true;
	if (first)
	{
		WSADATA ws;
		WSAStartup(MAKEWORD(2, 2), &ws);
		first = false;
	}
}
int WinSocket::CreateSocket()
{
	socket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (socket == -1)
		std::cout << "服务器创建端口失败！\n";
	return socket;
}
bool WinSocket::Bind(unsigned short port)
{
	if (socket <= 0)
		CreateSocket();
	//创建端口成功后
	sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = htonl(0);
	//绑定
	if (::bind(socket, (sockaddr*)&saddr, sizeof(saddr)) != 0)
	{
		printf("绑定 %d 端口失败!\n", port);
		return false;
	}
	printf("绑定 %d 端口成功!\n", port);
	//监听
	listen(socket, 10);//监听服务器套接字
	return true;
}
WinSocket WinSocket::Accept()
{
	sockaddr_in caddr;
	socklen_t len = sizeof(caddr);
	WinSocket accilent;
	int clientsock = accept(socket, (sockaddr*)&caddr, &len); //成功返回一个新的socket
	if (clientsock <= 0)
		return accilent;
	printf("连接客户端 [%d] !\n", clientsock);
	char *ip = inet_ntoa(caddr.sin_addr);
	strcpy(accilent.ip, ip);
	accilent.port = ntohs(caddr.sin_port);
	accilent.socket = clientsock;
	printf("客户端ip：%s , 端口号：%d\n", accilent.ip, accilent.port);
	return accilent;
}

int WinSocket::Recv(char* buf, int bufsize)
{
	return recv(socket, buf, bufsize, 0);
}

int WinSocket::Send(const char* buf, int size)
{
	int s = 0;
	while (s != size)
	{
		int len = send(socket, buf + s, size - s, 0);
		if (len <= 0)
			break;
		s += len;
	}
	return s;
}

bool WinSocket::SetBlock(bool isblock)
{
	if (socket <= 0)
		return false;
	unsigned long ul = 0;
	if (!isblock)
		ul = 1;
	ioctlsocket(socket, FIONBIO, &ul);
	return true;
}

bool WinSocket::Connect(const char* ip, unsigned short port, int timeout)
{
	if (socket <= 0)
		CreateSocket();
	sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = inet_addr(ip);
	SetBlock(false);
	fd_set set;
	if (connect(socket, (sockaddr*)&saddr, sizeof(saddr)) != 0)
	{
		FD_ZERO(&set);
		FD_SET(socket, &set);
		timeval tm;
		tm.tv_sec = 0;
		tm.tv_usec = timeout * 1000;
		if (select(socket + 1, 0, &set, 0, &tm) <= 0)
		{
			printf("连接失败或超时!\n");
			printf("连接 %s : %d 失败!: %s\n", ip, port, strerror(errno));
			return false;
		}
	}
	SetBlock(true);
	printf("连接 %s : %d 成功!\n", ip, port);
	return true;
}
void WinSocket::Close()
{
	if (socket <= 0)
		return;
	std::cout << "连接关闭！\n";
	closesocket(socket);
}
WinSocket::~WinSocket()
{
}
