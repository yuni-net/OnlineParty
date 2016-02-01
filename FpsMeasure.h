#pragma once

namespace OnlineParty
{
	class FpsMeasure
	{
	public:
		FpsMeasure(const unsigned long long now_time);
		void update();



	private:
		int frame_count;
		unsigned long long last_time_show;

		float calc_fps(const float elapsed_ms) const;
		void show(const float fps) const;
	};
}