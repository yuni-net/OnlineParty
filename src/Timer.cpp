#include "Timer.h"
#include "God.h"
#include <simplect3D.h>

namespace OnlineParty
{
	Timer::Timer()
	{
		std::unique_ptr<time_t> criteria_time(download_criteria_time());
		SYSTEMTIME st;
		GetSystemTime(&st);
		double gap_sec = difftime(make_time_t(st), *criteria_time);
		count = static_cast<unsigned long long>(gap_sec*1000);
		// 年月日の差分しかカウントできていないから、それ以下の分もカウントする
		count += st.wHour * 60 * 60 * 1000 + st.wMinute * 60 * 1000 + st.wSecond * 1000 + st.wMilliseconds;

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





	std::unique_ptr<time_t> Timer::download_criteria_time()
	{
		std::unique_ptr<time_t> criteria_time(new time_t());

		tm time_data;
		time_data.tm_year = 2015;
		time_data.tm_mon = 10;
		time_data.tm_mday = 21;
		(*criteria_time) = mktime(&time_data);

		// todo
		/*
		fw::P2P p2p;
		fw::Bindata request;
		request.add("{\"signature\": \"OnlineParty\", \"version\": 0, \"request\": [\"Give me the criteria time\"]}");
		p2p.send(God::get_surver_surfer(), request);
		if (p2p.are_there_any_left_datas() == false)
		{
			Sleep(1);
		}
		fw::NetSurfer surfer;
		fw::Bindata reply;
		p2p.pop_received_data(reply, surfer);
		reply >> criteria_time->year;
		reply >> criteria_time->month;
		reply >> criteria_time->day;
		reply >> criteria_time->hour;
		reply >> criteria_time->minute;
		reply >> criteria_time->second;
		reply >> criteria_time->ms;
		//*/
		return criteria_time;
	}


	time_t Timer::make_time_t(const SYSTEMTIME & st)
	{
		tm time_data;
		time_data.tm_year = st.wYear;
		time_data.tm_mon = st.wMonth;
		time_data.tm_mday = st.wDay;
		return mktime(&time_data);
	}


}