#include "stdafx.h"
#include "bench.h"
#include <unordered_set>

struct BenchHashSet : public BenchContext
{
public:
	BenchHashSet() {
		insert = std::bind(&BenchHashSet::insert_wapper, this, std::placeholders::_1);
		search = std::bind(&BenchHashSet::search_wapper, this, std::placeholders::_1);
		erase = std::bind(&BenchHashSet::erase_wapper, this, std::placeholders::_1);

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
	std::unordered_set<int> tree;
	int ss;
};

REGISTER_BENCH("STL unordered_set", BenchHashSet);