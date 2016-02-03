#pragma once

namespace OnlineParty
{
	class NeckMeasure
	{
	public:
		static void init();
		static void each_frame();
		static void set(const unsigned int index, const std::string & str, const double & val);



	private:
		struct strval
		{
			std::string str;
			double val;
			double max_per;
			double min_per;
		};

		static NeckMeasure & get_instance();
		void init_dynamic();
		void each_frame_dynamic();
		void set_dynamic(const unsigned int index, const std::string & str, const double & val);

		double last_time;
		double max;
		fw::Array<strval> requests;


		NeckMeasure();
	};
}