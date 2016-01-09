#include "Timer.h"
#include "God.h"
#include <simplect3D.h>

namespace OnlineParty
{
	Timer::Timer()
	{
		SYSTEMTIME st;
		GetSystemTime(&st);
		count = time_to_ms(st);
		last = std::chrono::system_clock::now();
	}

	void Timer::update()
	{
		auto now = std::chrono::system_clock::now();
		const auto & gap = now - last;

		const long long & gap_ms = std::chrono::duration_cast<std::chrono::milliseconds>(gap).count();
		count += gap_ms;

		elapsed_sec = static_cast<float>(std::chrono::duration_cast<std::chrono::seconds>(gap).count());
	}

	float Timer::get_elapsed_sec()
	{
		return elapsed_sec;
	}

	unsigned long long Timer::get_now_time()
	{
		return count;
	}






	unsigned long long Timer::time_to_ms(const SYSTEMTIME & systime) const
	{
		time_t daystime = make_time_t(systime);
		time_t criteria = make_criteria_time_t();
		double gap_days_as_sec = difftime(daystime, criteria);
		unsigned long long gap_days_as_ms = static_cast<unsigned long long>(gap_days_as_sec) * 1000;
		return gap_days_as_ms +
			static_cast<unsigned long long>(systime.wHour) * 3600*1000 +
			static_cast<unsigned long long>(systime.wMinute) * 60 * 1000 +
			static_cast<unsigned long long>(systime.wSecond) * 1000 +
			systime.wMilliseconds;
	}

	time_t Timer::make_time_t(const SYSTEMTIME & st) const
	{
		tm time_data;
		ZeroMemory(&time_data, sizeof(time_data));
		time_data.tm_year = st.wYear-1900;
		time_data.tm_mon = st.wMonth-1;
		time_data.tm_mday = st.wDay;
		return mktime(&time_data);
	}

	time_t Timer::make_criteria_time_t() const
	{
		tm time_data;
		ZeroMemory(&time_data, sizeof(time_data));
		time_data.tm_year = 2015 - 1900;
		time_data.tm_mon = 10 - 1;
		time_data.tm_mday = 1;
		return mktime(&time_data);
	}
}