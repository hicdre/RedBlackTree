// RedBlackTree.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <set>
#include <time.h>
#include "timehelp.h"
#include "RedBlackTree.h"


int _tmain(int argc, _TCHAR* argv[])
{
	unsigned randSeed = (unsigned)time(NULL);
	unsigned numTest = 50000;
	{//stlÀ„∑®
		std::set<int> tree;
		srand(randSeed);

		start_time();
		for (unsigned i = 0; i < numTest; ++i)
		{
			tree.insert(rand());
		}
		std::cout << "STL set Finished : " << elapse_millseconds() << "ms" << std::endl;
	}
	{//RedBlackTreeÀ„∑®
		RedBlackTree tree;
		srand(randSeed);

		start_time();
		for (unsigned i = 0; i < numTest; ++i)
		{
			tree.insert(rand());
		}
		std::cout << "RedBlackTree Finished : " << elapse_millseconds() << "ms" << std::endl;
		bool success = tree.verify();
		std::cout << "RedBlackTree Verify : " << (success ? "success" : "failed") << std::endl;

		while (true)
		{
			int testNum = rand();
			bool contains = tree.contains(testNum);
			if (contains)
				std::cout << "RedBlackTree contains: " << testNum << std::endl;
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
		}
		
		
	}
	return 0;
}

