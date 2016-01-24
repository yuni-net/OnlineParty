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
		@param Set the system time which you want convert.
		@return Milli second is returned.
		@detail
			The elapsed milli second from Jan 1st 2015 is returned.
			Cuz I couldn't calclate the elapsed time from A.D. 0 Jan 1st
			due to the technical problem of the function "mktime."
		*/
		unsigned long long time_to_ms(const SYSTEMTIME & systime) const;

		/**
		@brief I convert the system time to the value as time_t type.
		@param Set the system time which you want convert.
		@return The value as time_t type is returned.
		@warning I consider just year, month and day.
			I DONT consider hour, minute, second, milli second and so on.
		*/
		time_t make_time_t(const SYSTEMTIME & systime) const;

		/**
		@brief I make the value of criteria time as time_t type.
		@return The value as time_t type is returned.
		@detail The criteria time is Oct. 21st 2015 (at present).
		*/
		time_t make_criteria_time_t() const;
	};
}