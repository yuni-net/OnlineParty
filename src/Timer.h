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
		void begin_sync();
		void update();
		float get_elapsed_sec();
		unsigned long long get_now_time();




	private:
		static const int8_t request_code_sync_time = 0;

		// I show the plan.
		// (1)I synchronize current time with server.
		// (2)I measure elapsed seconds since (1) with std::chrono.
		// (3)I calclate recent time based on (1) and (2).

		// 'count' have recent time.
		unsigned long long count;
		// 'base_count' have the time that I got from server.
		unsigned long long base_count;
		// 'first' have system-time-point with std::chrono that I got first.
		std::chrono::system_clock::time_point first;
		// 'last' have recent system-time-point with std::chrono.
		std::chrono::system_clock::time_point last;
		// 'elapsed_sec' have elapsed seconds since last frame.
		float elapsed_sec;

		bool is_syncing;


		/**
		@brief I synchronize current time with server.
		@detail
			In this function, I communicate with the server through internet.
			About accuracy:
			The maximam milli seconds of error is half of the time that
			sending_time - receiving_time.
		*/
		static void sync_time_with_server(void * parameter);

		/**
		@brief I convert the system time to the milli second.
		@param Set the system time (UTC) which you want convert.
		@return Milli second is returned.
		@detail
			The elapsed milli second from Jan 1st 1970 is returned.
			Due to the specification of time system of general OS.
		*/
		static unsigned long long time_to_ms(const SYSTEMTIME & systime);

		/**
		@brief I convert the system time to the value as time_t type.
		@param Set the system time (UTC) which you want convert.
		@return The value as time_t type is returned.
		*/
		static time_t make_time_t(const SYSTEMTIME & systime);

		static void send_request_sync_current_time(const fw::P2P & p2p);
		static unsigned long long get_current_time(fw::Bindata & data);
		static bool is_valid_data(fw::Bindata & data);
	};
}