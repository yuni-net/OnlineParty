#include <simplect3D.h>
#include "God.h"
#include "../NeckMeasure.h"

using namespace OnlineParty;

int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lpC, int nC)
{
	fw::xrandom_init(static_cast<unsigned long>(time(NULL)));

	if (si3::Manager::init(960, 600) == false)
	{
		fw::popup("failed");
		return -1;
	}

	auto & displaym = si3::Manager::display_manager();
	displaym.context_color(1.0f, 1.0f, 1.0f);


	NeckMeasure::init();

	while (true)
	{
		double beg = fw::gettimeofday();
		const bool should_continue = si3::Manager::begin_frame();
		double end = fw::gettimeofday();
		double gap = end - beg;
		NeckMeasure::set(2, fw::cnct() << "si3::Manager::begin_frame()‚É‚©‚©‚Á‚½ŽžŠÔ: " << gap << "•b", gap);
		if (should_continue == false)
		{
			return -1;
		}

		beg = fw::gettimeofday();
		God::update();
		end = fw::gettimeofday();
		gap = end - beg;
		NeckMeasure::set(3, fw::cnct() << "God::update()‚É‚©‚©‚Á‚½ŽžŠÔ: " << gap << "•b", gap);

		beg = fw::gettimeofday();
		si3::Manager::show();
		end = fw::gettimeofday();
		gap = end - beg;
		NeckMeasure::set(4, fw::cnct() << "si3::Manager::show()‚É‚©‚©‚Á‚½ŽžŠÔ: " << gap << "•b", gap);


		NeckMeasure::each_frame();
	}


	return 0;
}



