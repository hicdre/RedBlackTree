// RedBlackTree.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <random>
#include <functional>
#include "timehelp.h"
#include "llrb.h"

template<class T>
class llrb_set : public llrb < T, T >
{
public:
	typedef llrb<T, T> BaseType;
	void insert(const T& k) {
		BaseType::insert(k, k);
	}
};

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

template<class TreeType>
void BenchFunction_insert(TreeType* tree, BenchData* data)
{
	for (int i : data->insertSet)
	{
		tree->insert(i);
	}
}

template<class TreeType>
void BenchFunction_search(TreeType* tree, BenchData* data)
{
	for (int i : data->searchSet)
	{
		tree->find(i);
	}
}

template<class TreeType>
void BenchFunction_erase(TreeType* tree, BenchData* data)
{
	for (int i : data->eraseSet)
	{
		tree->erase(i);
	}
}

template<class TreeType>
void BenchCase(const char* caseName, BenchData* data)
{
	TreeType tree;
	std::cout << "=========================" << std::endl;
	std::cout << "Begin " << caseName << ":" << std::endl;
	std::cout << caseName <<" insert : " 
		<< BenchOnce(std::bind(BenchFunction_insert<TreeType>, &tree, data))
		<< "ms" << std::endl;

	std::cout << caseName << " search : "
		<< BenchOnce(std::bind(BenchFunction_search<TreeType>, &tree, data))
		<< "ms" << std::endl;

	std::cout << caseName << " erase : "
		<< BenchOnce(std::bind(BenchFunction_erase<TreeType>, &tree, data))
		<< "ms" << std::endl;
	std::cout << "End " << caseName << ":" << std::endl;
	std::cout << "=========================" << std::endl;
}

void BenchAll()
{
	BenchData data(100000);

	BenchCase<std::set<int>>("STL", &data);
	BenchCase<llrb_set<int>>("LLRB", &data);
}

void VerifyAll()
{
	std::vector<int> data;
	GenRandomSetII(data, 3000);
	std::vector<int> fist_data;
	std::vector<int> second_data;
	std::vector<int> third_data;
	for (unsigned i = 0; i < data.size(); ++i)
	{
		switch (i % 3)
		{
		case 0:
			fist_data.push_back(data[i]);
			break;
		case 1:
			second_data.push_back(data[i]);
			break;
		case 2:
			third_data.push_back(data[i]);
			break;
		default:
			break;
		}
	}

	
	llrb_set<int> tree;
	//≤Â»Î
	for (unsigned i = 0; i < fist_data.size(); ++i)
	{
		tree.insert(fist_data[i]);
		assert(tree.verify());
	}
	assert(tree.verify());
	for (int i : second_data)
	{
		tree.insert(i);
	}
	assert(tree.verify());

	//≤È’“
	std::random_shuffle(fist_data.begin(), fist_data.end());
	std::random_shuffle(second_data.begin(), second_data.end());
	for (int i : fist_data)
	{
		assert(tree.contains(i));
	}
	for (int i : second_data)
	{
		assert(tree.contains(i));
	}
	for (int i : third_data)
	{
		assert(!tree.contains(i));
	}

	//…æ≥˝
	std::random_shuffle(third_data.begin(), third_data.end());
	for (int i : second_data)
	{
		tree.erase(i);
		assert(!tree.contains(i));
	}
	assert(tree.verify());
	for (int i : third_data)
	{
		tree.erase(i);
		assert(!tree.contains(i));
	}
	for (int i : fist_data)
	{
		assert(tree.contains(i));
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	VerifyAll();
	BenchAll();
	return 0;
}

