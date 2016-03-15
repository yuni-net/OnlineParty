#include <simplect3D.h>
#include "FirewallInstallHelper.h"

namespace OnlineParty
{
	void break_firewall()
	{
		static const int bufsize = 1024;
		char mypath[bufsize];
		GetModuleFileName(NULL, mypath, bufsize);
		const bool registered = CanLaunchMultiplayerGameA(mypath) != 0;
		if (registered)
		{
			return;
		}

		system(fw::cnct()<<".\\data\\FirewallBreaker.exe \""<<mypath<"\" -friend OnlineParty_debug");
	}
}