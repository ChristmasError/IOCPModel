#ifndef SOCKET_H
#define SOCKET_H

#ifdef WINSOCKET_EXPORTS
#define WINSOCKET_API __declspec(dllexport)
#else
#define WINSOCKET_API __declspec(dllimport)
#endif
#define socklen_t int

class WINSOCKET_API WinSocket
{
public:
	int CreateSocket();
	bool Bind(unsigned short port);
	bool Connect(const char*ip, unsigned short port, int timeout = 1000);
	WinSocket Accept();
	int Recv(char* buf, int bufsize);
	int Send(const char* buf, int sendsize);
	void Close();
	bool SetBlock(bool isblock);

	WinSocket();
	virtual ~WinSocket();
	int socket = 0;
	unsigned short port = 0;
	char ip[16];
};

#endif // ! SOCKET_H
