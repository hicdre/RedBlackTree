#include "stdafx.h"
#include "bench.h"
#include "normal_redblacktree.h"

struct BenchSecondRedBlackTree : public BenchContext
{
public:
	BenchSecondRedBlackTree() {
		insert = std::bind(&BenchSecondRedBlackTree::insert_wapper, this, std::placeholders::_1);
		search = std::bind(&BenchSecondRedBlackTree::search_wapper, this, std::placeholders::_1);
		erase = std::bind(&BenchSecondRedBlackTree::erase_wapper, this, std::placeholders::_1);

	}

	void insert_wapper(int i)
	{
		tree.insert(i);
	}

	bool search_wapper(int i)
	{
		return  tree.search(i)->key == i;
	}

	void erase_wapper(int i)
	{
		tree.remove(i);
	}

private:
	RBTree<int> tree;
	int ss;
};

REGISTER_BENCH("Normal redblacktree", BenchSecondRedBlackTree);