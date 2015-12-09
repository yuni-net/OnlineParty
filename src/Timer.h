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
		std::chrono::system_clock::time_point last;
		float elapsed_sec;

		std::unique_ptr<time_t> download_criteria_time();

		/**
		 @brief time_t型データを生成する。
		 @param 年月日を表すデータを指定する。
		 @warnning 生成されたデータは年月日以外の情報は持たない。
		 */
		time_t make_time_t(const SYSTEMTIME & st);
	};
}