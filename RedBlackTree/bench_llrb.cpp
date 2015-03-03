#include "stdafx.h"
#include "llrb.h"
#include "bench.h"
#include "random_data.h"
#include <algorithm>

template<class T>
class llrb_set : public llrb < T, T >
{
public:
	typedef llrb<T, T> BaseType;
	void insert(const T& k) {
		BaseType::insert(k, k);
	}
};

void VerifyLLRB()
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
	//插入
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

	//查找
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

	//删除
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

struct BenchLLRB : public BenchContext
{
public:
	BenchLLRB() {
		insert = std::bind(&BenchLLRB::insert_wapper, this, std::placeholders::_1);
		search = std::bind(&BenchLLRB::search_wapper, this, std::placeholders::_1);
		erase = std::bind(&BenchLLRB::erase_wapper, this, std::placeholders::_1);
	}

	void insert_wapper(int i)
	{
		tree.insert(i);
	}

	bool search_wapper(int i)
	{
		return tree.contains(i);
	}

	void erase_wapper(int i)
	{
		tree.erase(i);
	}

private:
	llrb_set<int> tree;
	int ss;
};

REGISTER_BENCH("LLRB", BenchLLRB);