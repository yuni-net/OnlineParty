#include <simplect3D.h>

namespace OnlineParty
{
	void break_firewall_actually(void * parameter)
	{
		SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		sockaddr_in recv_addr;
		recv_addr.sin_family = AF_INET;
		recv_addr.sin_addr.S_un.S_addr = INADDR_ANY;

		unsigned short port = 9697;
		while (true)
		{
			recv_addr.sin_port = htons(port);
			int result = bind(sock, reinterpret_cast<sockaddr *>(&recv_addr), sizeof(recv_addr));
			if (result != -1)
			{
				break;
			}
			++port;
		}

		const int bufsize = 256;
		char buffer[bufsize];
		sockaddr_in ano_addr;
		int addr_len = sizeof(sockaddr_in);
		recvfrom(sock, buffer, bufsize, 0, reinterpret_cast<sockaddr *>(&ano_addr), &addr_len);

		closesocket(sock);
	}

	void break_firewall()
	{
		fw::NetWork::init_ifneed();
		fw::newthread(break_firewall_actually, nullptr);
	}

}