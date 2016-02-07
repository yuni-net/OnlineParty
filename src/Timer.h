#pragma once

#include <simplect3D.h>
#include <ctime>
#include <memory>
#include <chrono>

namespace OnlineParty
{
	class Timer
	{
	public:
		Timer();
		void update();
		float get_elapsed_sec();
		unsigned long long get_now_time();




	private:
		unsigned long long count;
		unsigned long long base_count;
		std::chrono::system_clock::time_point first;
		std::chrono::system_clock::time_point last;
		float elapsed_sec;

		/**
		@brief I convert the system time to the milli second.
		@param Set the system time (UTC) which you want convert.
		@return Milli second is returned.
		@detail
			The elapsed milli second from Jan 1st 1970 is returned.
			Due to the specification of time system of general OS.
		*/
		unsigned long long time_to_ms(const SYSTEMTIME & systime) const;

		/**
		@brief I convert the system time to the value as time_t type.
		@param Set the system time (UTC) which you want convert.
		@return The value as time_t type is returned.
		*/
		time_t make_time_t(const SYSTEMTIME & systime) const;
	};
}