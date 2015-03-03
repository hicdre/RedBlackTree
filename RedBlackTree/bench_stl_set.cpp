#include "stdafx.h"
#include "bench.h"
#include "random_data.h"
#include <set>

struct BenchSTL : public BenchContext
{
public:
	BenchSTL() {
		insert = std::bind(&BenchSTL::insert_wapper, this, std::placeholders::_1);
		search = std::bind(&BenchSTL::search_wapper, this, std::placeholders::_1);
		erase = std::bind(&BenchSTL::erase_wapper, this, std::placeholders::_1);
	}

	void insert_wapper(int i)
	{
		tree.insert(i);
	}

	bool search_wapper(int i)
	{
		return  tree.find(i) != tree.end();
	}

	void erase_wapper(int i)
	{
		tree.erase(i);
	}

private:
	std::set<int> tree;
	int ss;
};

REGISTER_BENCH("STL set", BenchSTL);