#include "Timer.h"
#include "God.h"
#include <simplect3D.h>

namespace OnlineParty
{
	Timer::Timer()
	{
		SYSTEMTIME sys_time;
		GetSystemTime(&sys_time);
		first = std::chrono::system_clock::now();
		base_count = time_to_ms(sys_time);
		count = base_count;
		last = first;
		is_syncing = false;
	}

	void Timer::begin_sync()
	{
		if (is_syncing)
		{
			return;
		}

		is_syncing = true;
		fw::newthread(sync_time_with_server, this);
	}

	void Timer::update()
	{
		auto now = std::chrono::system_clock::now();

		const auto & gap_from_last = now - last;
		last = now;
		const long long & elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(gap_from_last).count();
		if (elapsed_ms <= 0)
		{
			// 経過時間ゼロが連発するとゲームの進行速度がほかの端末と大きくずれてしまうから
			Sleep(2);
			update();
			return;
		}
		elapsed_sec = static_cast<float>(elapsed_ms)*0.001f;

		const auto & gap_from_first = now - first;
		const long long & gap_ms_first = std::chrono::duration_cast<std::chrono::milliseconds>(gap_from_first).count();
		count = base_count + gap_ms_first;
	}

	float Timer::get_elapsed_sec()
	{
		return elapsed_sec;
	}

	unsigned long long Timer::get_now_time()
	{
		return count;
	}







	void Timer::sync_time_with_server(void * parameter)
	{
		auto & self = *reinterpret_cast<Timer *>(parameter);
		static const int try_qty = 10;
		fw::P2P p2p;
		fw::Bindata data;
		int total_offset = 0;
		SYSTEMTIME beg_time;
		SYSTEMTIME end_time;

		for (int i = 0; i < try_qty; ++i)
		{
			send_request_sync_current_time(p2p);
			GetSystemTime(&beg_time);
			auto beg = std::chrono::system_clock::now();
			while (true)
			{
				if (p2p.are_there_any_left_datas())
				{
					GetSystemTime(&end_time);
					fw::NetSurfer surfer;
					p2p.pop_received_data(data, surfer);
					if (is_valid_data(data))
					{
						break;
					}
				}


				const auto & gap = std::chrono::system_clock::now() - beg;
				const long long & elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(gap).count();
				if (elapsed_ms >= 6000)
				{
					send_request_sync_current_time(p2p);
					GetSystemTime(&beg_time);
					beg = std::chrono::system_clock::now();
				}
			}
			const auto & current_time_ms = get_current_time(data);
			const auto & sent_ms = time_to_ms(beg_time);
			const auto & received_ms = time_to_ms(end_time);
			auto estimated_current_time = sent_ms + (received_ms - sent_ms)/2;

			if (current_time_ms >= estimated_current_time)
			{
				total_offset += static_cast<int>(current_time_ms - estimated_current_time);
			}
			else
			{
				total_offset -= static_cast<int>(estimated_current_time - current_time_ms);
			}
		}

		int offset = total_offset / try_qty;
		self.base_count += offset;
		self.count += offset;
		self.is_syncing = false;
	}

	unsigned long long Timer::time_to_ms(const SYSTEMTIME & systime)
	{
		FILETIME file_time;
		SystemTimeToFileTime(&systime, &file_time);
		FILETIME local_ft;
		FileTimeToLocalFileTime(&file_time, &local_ft);
		SYSTEMTIME local_time;
		FileTimeToSystemTime(&local_ft, &local_time);
		time_t seconds = make_time_t(local_time);
		return seconds * 1000 + systime.wMilliseconds;
	}

	time_t Timer::make_time_t(const SYSTEMTIME & st)
	{
		tm time_data;
		ZeroMemory(&time_data, sizeof(time_data));
		time_data.tm_year = st.wYear - 1900;
		time_data.tm_mon = st.wMonth - 1;
		time_data.tm_mday = st.wDay;
		time_data.tm_hour = st.wHour;
		time_data.tm_min = st.wMinute;
		time_data.tm_sec = st.wSecond;
		time_data.tm_isdst = 0;
		return mktime(&time_data);
	}

	void Timer::send_request_sync_current_time(const fw::P2P & p2p)
	{
		fw::Bindata request;
		request.add(std::string("OnlineParty"));
		static const int8_t version = 0;
		request.add(version);
		request.add(request_code_sync_time);
		p2p.send(God::get_synchronizer().get_server_surfer(), request);
	}

	unsigned long long Timer::get_current_time(fw::Bindata & data)
	{
		unsigned long long time;

		if (fw::is_big_endian())
		{
			int64_t t;
			data >> t;
			time = t;
		}
		else
		{
			int8_t a, b, c, d;
			data >> a >> b >> c >> d;
			time = d;
			time <<= 8;
			time += c;
			time <<= 8;
			time += b;
			time <<= 8;
			time += a;
		}
		
		return time;
	}

	bool Timer::is_valid_data(fw::Bindata & data)
	{
		std::string signature;
		data >> signature;
		if (signature != std::string("OnlineParty"))
		{
			return false;
		}
		int8_t version;
		data >> version;
		if (version == 0)
		{
			int8_t request_code;
			data >> request_code;
			if (request_code == request_code_sync_time)
			{
				return true;
			}
		}
		else
		{
			// Unsupported version
		}
		
		return false;
	}
}