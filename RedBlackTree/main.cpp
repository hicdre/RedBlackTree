// RedBlackTree.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <set>
#include <time.h>
#include <vector>
#include "timehelp.h"
#include "RedBlackTree.h"


int _tmain(int argc, _TCHAR* argv[])
{
	unsigned randSeed = (unsigned)time(NULL);
	unsigned numTest = 10;
	{//stlÀ„∑®
		std::set<int> tree;
		srand(randSeed);

		start_time();
		for (unsigned i = 0; i < numTest; ++i)
		{
			tree.insert(rand());
		}
		std::cout << "STL set Finished : " << elapse_millseconds() << "ms" << std::endl;

		std::set<int>::iterator it = tree.begin();
		++it;
		tree.erase(it);
	}
	{//RedBlackTreeÀ„∑®
		RedBlackTree tree;
		srand(randSeed);

		start_time();
		for (unsigned i = 0; i < numTest; ++i)
		{
			int k = rand();
			std::cout << k << std::endl;
			tree.insert(k);
		}
		std::cout << "RedBlackTree Finished : " << elapse_millseconds() << "ms" << std::endl;
		//bool success = tree.verify();
		//std::cout << "RedBlackTree Verify : " << (success ? "success" : "failed") << std::endl;

		while (true)
		{
			int testNum = rand();
			bool contains = tree.contains(testNum);
			if (contains)
			{
				std::cout << "RedBlackTree contains: " << testNum << std::endl;
			}
			else
			{
				{
					RedBlackTree::Node* node = tree.notGreater(testNum);
					std::cout << "RedBlackTree not Greater than " << testNum << " is ";
					if (node)
						std::cout << node->key << std::endl;
					else
						std::cout << "null" << std::endl;
				}
				{
					RedBlackTree::Node* node = tree.notLess(testNum);
					std::cout << "RedBlackTree not Less than " << testNum << " is ";
					if (node)
						std::cout << node->key << std::endl;
					else
						std::cout << "null" << std::endl;
				}
				break;
			}
			//assert(tree.verify());
		}
		
		//tree.print();
		{
			std::vector<int> v{ 27269, 13489, 23205, 11215, 15860, 19079, 8180, 8619, 31807, 23629 };
			RedBlackTree ntree;
			for (int i : v)
			{
				ntree.insert(i);
			}
			assert(ntree.verify());
			ntree.erase(23205);
			assert(!ntree.contains(23205));
		}

		
		size_t eraseCount = 10;
		while (eraseCount)
		{
			int testNum = rand();
			bool c = tree.contains(testNum);
			if (c)
			{
				std::cout << "RedBlackTree erase: " << testNum << std::endl;
				tree.erase(testNum);
				bool b = tree.contains(testNum);
				assert(b != c);
				--eraseCount;
				assert(tree.verify());
			}
		}
	}
	return 0;
}

