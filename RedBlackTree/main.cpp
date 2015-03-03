// RedBlackTree.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <functional>
#include "timehelp.h"
#include "bench.h"
#include "random_data.h"

class BenchData
{
public:
	BenchData(size_t count) {
		Build(count);
	}

	std::vector<int> insertSet;
	std::vector<int> searchSet;
	std::vector<int> eraseSet;
private:
	void Build(size_t count) {
		GenRandomSet(insertSet, count);
		searchSet = insertSet;
		eraseSet = insertSet;
		std::random_shuffle(insertSet.begin(), insertSet.end());
		std::random_shuffle(searchSet.begin(), searchSet.end());
		std::random_shuffle(eraseSet.begin(), eraseSet.end());
	}
};

double BenchOnce(const std::function<void(void)>& benchFunction)
{
	start_time();

	benchFunction();

	return elapse_millseconds();
}

void BenchDataForeach(const std::vector<int>& dataSet, const std::function<void(int)>& func)
{
	for (int i : dataSet)
	{
		func(i);
	}
}

void BenchSearchForeach(const std::vector<int>& dataSet, const std::function<bool(int)>& func)
{
	int k = 0;
	for (int i : dataSet)
	{
		if (func(i))
			k++;
	}
}

void BenchCase(const Bench* b, BenchData* data)
{
	//TreeType tree;
	std::cout << "=========================" << std::endl;
	std::cout << "Begin " << b->name << ":" << std::endl;
	std::cout << b->name << " insert : "
		<< BenchOnce(std::bind(BenchDataForeach, data->insertSet, b->context->insert))
		<< "ms" << std::endl;

	std::cout << b->name << " search : "
		<< BenchOnce(std::bind(BenchSearchForeach, data->searchSet, b->context->search))
		<< "ms" << std::endl;

	std::cout << b->name << " erase : "
		<< BenchOnce(std::bind(BenchDataForeach, data->eraseSet, b->context->erase))
		<< "ms" << std::endl;
	std::cout << "End " << b->name << ":" << std::endl;
	std::cout << "=========================" << std::endl;
}

void BenchAll()
{
#ifdef _DEBUG
	BenchData data(100000);
#else
	BenchData data(1000000);
#endif

	for (const Bench* b : BenchManager::Get()->benches())
	{
		BenchCase(b, &data);
	}

// 	BenchCase<std::set<int>>("STL", &data);
// 	BenchCase<llrb_set<int>>("LLRB", &data);
}




int _tmain(int argc, _TCHAR* argv[])
{
	//VerifyAll();
	BenchAll();
	return 0;
}

