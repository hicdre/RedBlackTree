#include "stdafx.h"
#include "another_rb.h"
#include "bench.h"

struct node {
	LLRB_ENTRY(node) entry;
	int i;
};

int
static intcmp(struct node *e1, struct node *e2)
{
	return (e1->i - e2->i);
}

LLRB_HEAD(inttree, node);
LLRB_GENERATE(inttree, node, entry, intcmp);

struct BenchAnother : public BenchContext
{
public:
	BenchAnother() {
		insert = std::bind(&BenchAnother::insert_wapper, this, std::placeholders::_1);
		search = std::bind(&BenchAnother::search_wapper, this, std::placeholders::_1);
		erase = std::bind(&BenchAnother::erase_wapper, this, std::placeholders::_1);
		tree = LLRB_INITIALIZER(&tree);

	}

	~BenchAnother() {
		node *var, *nxt;
		for (var = LLRB_MIN(inttree, &tree); var != NULL; var = nxt) {
			nxt = LLRB_NEXT(inttree, &tree, var);
			LLRB_REMOVE(inttree, &tree, var);
			delete var;
		}
	}

	void insert_wapper(int i)
	{
		node* n = new node;
		n->i = i;
		LLRB_INSERT(inttree, &tree, n);
	}

	bool search_wapper(int i)
	{
		node n;
		n.i = i;
		return LLRB_FIND(inttree, &tree, &n) != NULL;
	}

	void erase_wapper(int i)
	{
		node n;
		n.i = i;
		LLRB_REMOVE(inttree, &tree, &n);
	}

private:
	inttree tree;
	int ss;
};

REGISTER_BENCH("AnotherLLRB", BenchAnother);