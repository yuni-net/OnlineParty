#include <simplect3D.h>

#include "NeckMeasure.h"

namespace OnlineParty
{
	void NeckMeasure::init()
	{
		get_instance().init_dynamic();
	}

	void NeckMeasure::each_frame()
	{
		get_instance().each_frame_dynamic();
	}

	void NeckMeasure::set(const unsigned int index, const std::string & str, const double & val)
	{
		get_instance().set_dynamic(index, str, val);
	}


	NeckMeasure & NeckMeasure::get_instance()
	{
		static NeckMeasure measure;
		return measure;
	}

	void NeckMeasure::init_dynamic()
	{
		last_time = fw::gettimeofday();
		max = 0.000;
		requests.setsize(20);
		for (unsigned int index = 0; index < requests.size(); ++index)
		{
			requests[index].max_per = 0.00000000;
			requests[index].min_per = 8192;
		}
	}

	void NeckMeasure::each_frame_dynamic()
	{
		const double now_time = fw::gettimeofday();
		const double gap = now_time - last_time;
		if (max < gap)
		{
			max = gap;
		}
		fw::view().set(fw::cnct() << "このフレームにかかった時間: " << gap << "秒"<<"　最大"<<max<<"秒", 1);
		for (unsigned int index = 0; index < requests.size(); ++index)
		{
			if (requests[index].str == ""){ continue; }
			const double percent = requests[index].val / gap;
			if (requests[index].max_per < percent)
			{
				requests[index].max_per = percent;
			}
			if (requests[index].min_per > percent)
			{
				requests[index].min_per = percent;
			}
			fw::view().set(fw::cnct() << requests[index].str << "　全体の" << percent << "%　Max"<<requests[index].max_per<<"%　Min"<<requests[index].min_per<<"%", 2 + index);
		}

		last_time = now_time;
	}

	void NeckMeasure::set_dynamic(const unsigned int index, const std::string & str, const double & val)
	{
		if (index >= requests.size()){ return; }

		requests[index].str = str;
		requests[index].val = val;
	}



	NeckMeasure::NeckMeasure()
	{
		// Nothing
	}

}