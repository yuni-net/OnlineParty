#include "FpsMeasure.h"
#include "src\God.h"

namespace OnlineParty
{
	FpsMeasure::FpsMeasure(const unsigned long long now_time)
	{
		last_time_show = now_time;
		frame_count = 0;
	}

	void FpsMeasure::update()
	{
		++frame_count;
		const auto now = God::get_now_time();
		const float elapsed_ms = static_cast<float>(now - last_time_show);
		if (elapsed_ms < 1000)
		{
			return;
		}

		show(calc_fps(elapsed_ms));
		frame_count = 0;
		last_time_show = now;
	}


	float FpsMeasure::calc_fps(const float elapsed_ms) const
	{
		return frame_count / (elapsed_ms * 0.001f);
	}

	void FpsMeasure::show(const float fps) const
	{
		fw::view().set(fw::cnct() << fps, 0);
	}
}