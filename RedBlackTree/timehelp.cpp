#include "stdafx.h"
#include "timehelp.h"
#include <chrono>


static std::chrono::time_point<std::chrono::system_clock> s_last_time_point;

void start_time()
{
	s_last_time_point = std::chrono::system_clock::now();
}

double elapse_millseconds()
{
	std::chrono::time_point<std::chrono::system_clock> current_time_point = std::chrono::system_clock::now();
	std::chrono::duration<double> d = current_time_point - s_last_time_point;
	s_last_time_point = current_time_point;
	return std::chrono::duration_cast<std::chrono::milliseconds>(d).count();
}
