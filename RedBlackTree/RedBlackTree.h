#pragma once
#include <cassert>
#include <functional>
#include <iostream>

class RedBlackTree
{
public:
	struct Node
	{
		int key;
		Node* left;
		Node* right;
		Node* parent;
		bool red;

		Node(int k)
			: key(k), left(NULL), right(NULL), parent(NULL), red(true) {
		}
		int compare(int k) {
			return key - k;
		}
		void set(int k) {
			key = k;
		}
	};
	
	RedBlackTree() 
		: root(NULL) {
	}
	~RedBlackTree() {
		clear();
	}

	void insert(int n) {
		root = insert(root, n);
		root->red = false;
	}

	void clear() {
		_deleteNode(root);
	}

	bool contains(int n) {
		return find(root, n) != NULL;
	}

	Node* notGreater(int n) {
		return lowerNode(root, n);
	}

	Node* notLess(int n) {
		return upperNode(root, n);
	}

	void erase(int n) {
		root = eraseNode(root, n);
		if (root)
			root->red = false;
	}

	bool verify() {
		Node* n = first(root);
		size_t pathLength = blackPathLength(n);
		while (n)
		{
			Node* b = next(n);
			if (b && b->key <= n->key) {
				return false;
			}

			if (!verifyRedNode(n))
				return false;

			if (!verifyBlackPathLength(n, pathLength))
				return false;

			n = b;
		}
		return true;
	}

	void print() {
		Node* n = first(root);
		while (n)
		{
			std::cout << n->key << "," << std::endl;
			n = next(n);
		}
	}

private:
	static bool IsRed(Node* tree) {
		if (tree == NULL)
			return false;
		return tree->red;
	}

	static Node* insert(Node* tree, int n) {
		if (tree == NULL)
			return new Node(n);

		int result = tree->compare(n);
		if (result == 0)
			tree->set(n);
		else if (result > 0) {
			setLeftTree(tree, insert(tree->left, n));
		}
		else {
			setRightTree(tree, insert(tree->right, n));
		}

		return fixup(tree);
	}

	static Node* fixup(Node* tree) {
		if (IsRed(tree->right))
			tree = roateLeft(tree);

		if (IsRed(tree->left) && IsRed(tree->left->left))
			tree = roateRight(tree);

		if (IsRed(tree->left) && IsRed(tree->right))
			flipColor(tree);
		return tree;
	}

	static Node* roateLeft(Node* tree){
		//左转
		assert(IsRed(tree->right));
		Node* newTree = tree->right;
		newTree->parent = tree->parent;

		setRightTree(tree, newTree->left);
		setLeftTree(newTree, tree);
		newTree->red = tree->red;
		tree->red = true;
		return newTree;
	}

	static Node* roateRight(Node* tree){
		//右转
		assert(IsRed(tree->left));
		Node* newTree = tree->left;
		newTree->parent = tree->parent;

		setLeftTree(tree, newTree->right);
		setRightTree(newTree, tree);

		newTree->red = tree->red;
		tree->red = true;
		return newTree;
	}

	static Node* flipColor(Node* tree){
		tree->left->red = !tree->left->red;
		tree->right->red = !tree->right->red;
		tree->red = !tree->red;
		return tree;
	}

	static void _deleteNode(Node* tree) {
		if (tree == NULL)
			return;
		_deleteNode(tree->left);
		_deleteNode(tree->right);
		delete tree;
	}

	static Node* next(Node* tree)
	{
		if (tree == NULL)
			return NULL;
		if (tree->right != NULL)
			return first(tree->right);
		else
			return leftAncestor(tree);
	}

	static Node* prev(Node* tree)
	{
		if (tree == NULL)
			return NULL;
		if (tree->left != NULL)
			return last(tree->right);
		else
			return rightAncestor(tree);
	}

	static Node* first(Node* tree)
	{
		assert(tree);
		if (tree->left)
			return first(tree->left);
		else
			return tree;
	}

	static Node* last(Node* tree)
	{
		assert(tree);
		if (tree->right)
			return last(tree->right);
		else
			return tree;
	}

	static Node* leftAncestor(Node* tree) {
		while (tree->parent && tree->parent->left != tree) {
			tree = tree->parent;
		}
		return tree->parent;
	}

	static Node* rightAncestor(Node* tree) {
		while (tree->parent && tree->parent->right != tree) {
			tree = tree->parent;
		}
		return tree->parent;
	}

	static Node* find(Node* tree, int n) {
		if (tree == NULL)
			return NULL;
		int result = tree->compare(n);
		if (result == 0)
			return tree;
		else if (result > 0)
			return find(tree->left, n);
		else
			return find(tree->right, n);
	}

	static Node* lowerNode(Node* tree, int n) {
		if (tree == NULL)
			return NULL;
		int result = tree->compare(n);
		if (result == 0)
			return tree;
		else if (result > 0) {
			if (tree->left)
				return lowerNode(tree->left, n);
			else
				return prev(tree);
		}
		else {
			if (tree->right)
				return lowerNode(tree->right, n);
			else
				return tree;
		}
	}

	static Node* upperNode(Node* tree, int n) {
		if (tree == NULL)
			return NULL;
		int result = tree->compare(n);
		if (result == 0)
			return tree;
		else if (result > 0) {
			if (tree->left)
				return upperNode(tree->left, n);
			else
				return tree;
		}
		else {
			if (tree->right)
				return upperNode(tree->right, n);
			else
				return next(tree);
		}
	}

	//移除一个节点
	static Node* eraseNode(Node* tree, int n)
	{
		int result = tree->compare(n);
		if (result > 0)
		{
			if (!IsRed(tree->left) && !IsRed(tree->left->left))
				tree = moveRedLeft(tree);
			setLeftTree(tree, eraseNode(tree->left, n));
		}
		else
		{
			if (IsRed(tree->left))
				tree = roateRight(tree);
			if (result == 0 && (tree->right == NULL))
				return NULL;
			if (!IsRed(tree->right) && !IsRed(tree->right->left))
				tree = moveRedRight(tree);
			if (result == 0)
			{
				Node* firstNode = first(tree->right);
				tree->key = firstNode->key;
				setRightTree(tree, eraseFirstNode(tree->right));
			}
			else 
				setRightTree(tree, eraseNode(tree->right, n));
		}
		return fixup(tree);
	}

	static Node* eraseLastNode(Node* tree)
	{
		if (IsRed(tree->left)) {
			tree = roateRight(tree);
			assert(tree->right);
			assert(IsRed(tree->right));
		}

		if (tree->right == NULL) {
			delete tree;
			return NULL;
		}

		if (!IsRed(tree->right) && !IsRed(tree->right->left)) {
			tree = moveRedRight(tree);
		}

		setRightTree(tree, eraseLastNode(tree->right));
		return fixup(tree);
	}

	static Node* eraseFirstNode(Node* tree)
	{
		if (tree->left == NULL) {
			delete tree;
			return NULL;
		}

		if (!IsRed(tree->left) && !IsRed(tree->left->left)) {
			tree = moveRedLeft(tree);
		}

		setLeftTree(tree, eraseFirstNode(tree->left));
		return fixup(tree);
	}

	static Node* moveRedLeft(Node* tree)
	{
		flipColor(tree);
		if (IsRed(tree->right->left)) {
			setRightTree(tree, roateRight(tree->right));
			tree = roateLeft(tree);
			flipColor(tree);
		}
		return tree;
	}

	static Node* moveRedRight(Node* tree)
	{
		flipColor(tree);
		if (IsRed(tree->left->left)) {
			tree = roateRight(tree);
			flipColor(tree);
		}
		return tree;
	}

	static void setLeftTree(Node* tree, Node* sub) {
		tree->left = sub;
		if (sub)
			sub->parent = tree;
	}

	static void setRightTree(Node* tree, Node* sub) {
		tree->right = sub;
		if (sub)
			sub->parent = tree;
	}

	//红结点左右儿子都是黑的
	static bool verifyRedNode(Node* tree){
		if (IsRed(tree))
			return !IsRed(tree->left) && !IsRed(tree->right);
		return true;
	}

	static bool verifyBlackPathLength(Node* tree, size_t length) {
		if (tree->left || tree->right)
			return true;

		return blackPathLength(tree) == length;
	}

	static size_t blackPathLength(Node* tree) {
		size_t pathLength = 0;;
		while (tree)
		{
			if (!IsRed(tree))
				++pathLength;
			tree = tree->parent;
		}
		return pathLength;
	}
private:
	Node* root;

};