#include "stdafx.h"
#include "random_data.h"
#include <algorithm>
#include <random>
#include <set>

void GenRandomSet(std::vector<int>& randomSet, size_t count)
{
	std::random_device rd;
	for (unsigned i = 0; i < count; ++i)
	{
		randomSet.push_back(rd());
	}
}

void GenRandomSetII(std::vector<int>& randomSet, size_t count)
{
	std::random_device rd;
	std::set<int> tset;
	while (tset.size() < count)
	{
		tset.insert(rd());
	}
	for (int i : tset)
	{
		randomSet.push_back(i);
	}
	std::random_shuffle(randomSet.begin(), randomSet.end());
}
