#include "stdafx.h"
#include "bench.h"

struct BenchNone : public BenchContext
{
public:
	BenchNone() {
		insert = std::bind(&BenchNone::insert_wapper, this, std::placeholders::_1);
		search = std::bind(&BenchNone::search_wapper, this, std::placeholders::_1);
		erase = std::bind(&BenchNone::erase_wapper, this, std::placeholders::_1);
	}

	void insert_wapper(int i)
	{
		ss = i;
	}

	bool search_wapper(int i)
	{
		return ss % 2 == 0;
	}

	void erase_wapper(int i)
	{
		ss = i;
	}

private:
	int ss;
};

REGISTER_BENCH("None", BenchNone);