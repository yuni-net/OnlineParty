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
		do
		{
			recv_addr.sin_port = htons(port);
			int result = bind(sock, reinterpret_cast<sockaddr *>(&recv_addr), sizeof(recv_addr));
			if (result == -1)
			{
				++port;
				continue;
			}
			else
			{
				break;
			}
		} while (true);

		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(sock, &fds);

		timeval timev;
		timev.tv_sec = 0;
		timev.tv_usec = 8;

		select(0, &fds, NULL, NULL, &timev);

		closesocket(sock);
	}
}