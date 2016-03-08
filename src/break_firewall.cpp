#include <simplect3D.h>

namespace OnlineParty
{
	void break_firewall()
	{
		fw::NetWork::init_ifneed();
		SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		sockaddr_in recv_addr;
		recv_addr.sin_family = AF_INET;
		recv_addr.sin_addr.S_un.S_addr = INADDR_ANY;

		unsigned short port = 9696;
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

		sockaddr_in sendaddr;
		memset(&sendaddr, 0, sizeof(sendaddr));
		sendaddr.sin_family = AF_INET;
		sendaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		sendaddr.sin_port = htons(port);
		char damy = 0;
		sendto(sock, &damy, sizeof(damy), 0, (sockaddr *) &sendaddr, sizeof(sendaddr));

		const int bufsize = 256;
		char buffer [bufsize];
		sockaddr_in ano_addr;
		int addr_len = sizeof(sockaddr_in);
		recvfrom(sock, buffer, bufsize, 0, reinterpret_cast<sockaddr *>(&ano_addr), &addr_len);

		closesocket(sock);
	}
}