#pragma once
#include <cassert>
#include <iostream>

template<class KeyType>
struct KeyCompare
{
	int operator()(const KeyType& left, const KeyType& right) const
	{	// apply operator< to operands
		if (left < right)
			return -1;
		if (left == right)
			return 0;
		return 1;
	}
};

template<class KeyType, class ValueType,
class KeyCompareType = KeyCompare<KeyType>>
class llrb
{
public:
	struct Node
	{
		KeyType key;
		ValueType value;
		Node* left;
		Node* right;
		Node* parent;
		bool red;

		Node(const KeyType& k, const ValueType& v)
			: key(k), value(v), left(NULL), right(NULL), parent(NULL), red(true) {
		}
	};
	
	llrb() 
		: root(NULL), key_comp(KeyCompareType()) {
	}
	~llrb() {
		clear();
	}

	void insert(const KeyType& k, const ValueType& v) {
		root = insert(root, k, v);
		root->red = false;
	}

	void clear() {
		_deleteNode(root);
	}

	bool contains(const KeyType& n) {
		return find(root, n) != NULL;
	}

	Node* find(const KeyType& n) {
		return find(root, n);
	}

	Node* notGreater(const KeyType& n) {
		return lowerNode(root, n);
	}

	Node* notLess(const KeyType& n) {
		return upperNode(root, n);
	}

	void erase(const KeyType& n) {
		if (root == NULL)
			return;
		root = eraseNode(root, n);
		if (root)
			root->red = false;
	}

	bool verify() {
		if (root == NULL)
			return true;
		Node* n = first(root);
		size_t pathLength = blackPathLength(n);
		while (n)
		{
			Node* b = next(n);
			if (b && compare(b->key, n->key) <= 0) {
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
	bool isRed(Node* tree) {
		if (tree == NULL)
			return false;
		return tree->red;
	}

	Node* insert(Node* tree, const KeyType& k, const ValueType& v) {
		if (tree == NULL)
			return new Node(k, v);

		int result = compare(k, tree->key);
		if (result == 0){//assignment
			tree->key = k;
			tree->value = v;
		}
		else if (result < 0) {
			setLeftTree(tree, insert(tree->left, k, v));
		}
		else {
			setRightTree(tree, insert(tree->right, k, v));
		}

		return fixup(tree);
	}

	Node* fixup(Node* tree) {
		if (isRed(tree->right))
			tree = roateLeft(tree);

		if (isRed(tree->left) && isRed(tree->left->left))
			tree = roateRight(tree);

		if (isRed(tree->left) && isRed(tree->right))
			flipColor(tree);
		return tree;
	}

	Node* roateLeft(Node* tree){
		//左转
		assert(isRed(tree->right));
		Node* newTree = tree->right;
		newTree->parent = tree->parent;

		setRightTree(tree, newTree->left);
		setLeftTree(newTree, tree);
		newTree->red = tree->red;
		tree->red = true;
		return newTree;
	}

	Node* roateRight(Node* tree){
		//右转
		assert(isRed(tree->left));
		Node* newTree = tree->left;
		newTree->parent = tree->parent;

		setLeftTree(tree, newTree->right);
		setRightTree(newTree, tree);

		newTree->red = tree->red;
		tree->red = true;
		return newTree;
	}

	Node* flipColor(Node* tree){
		tree->left->red = !tree->left->red;
		tree->right->red = !tree->right->red;
		tree->red = !tree->red;
		return tree;
	}

	void _deleteNode(Node* tree) {
		if (tree == NULL)
			return;
		_deleteNode(tree->left);
		_deleteNode(tree->right);
		delete tree;
	}

	Node* next(Node* tree)
	{
		if (tree == NULL)
			return NULL;
		if (tree->right != NULL)
			return first(tree->right);
		else
			return leftAncestor(tree);
	}

	Node* prev(Node* tree)
	{
		if (tree == NULL)
			return NULL;
		if (tree->left != NULL)
			return last(tree->right);
		else
			return rightAncestor(tree);
	}

	Node* first(Node* tree)
	{
		assert(tree);
		if (tree->left)
			return first(tree->left);
		else
			return tree;
	}

	Node* last(Node* tree)
	{
		assert(tree);
		if (tree->right)
			return last(tree->right);
		else
			return tree;
	}

	Node* leftAncestor(Node* tree) {
		while (tree->parent && tree->parent->left != tree) {
			tree = tree->parent;
		}
		return tree->parent;
	}

	Node* rightAncestor(Node* tree) {
		while (tree->parent && tree->parent->right != tree) {
			tree = tree->parent;
		}
		return tree->parent;
	}

	Node* find(Node* tree, const KeyType& n) {
		if (tree == NULL)
			return NULL;
		int result = compare(n, tree->key);
		if (result == 0)
			return tree;
		else if (result < 0)
			return find(tree->left, n);
		else
			return find(tree->right, n);
	}

	Node* lowerNode(Node* tree, const KeyType& n) {
		if (tree == NULL)
			return NULL;
		int result = compare(n, tree->key);
		if (result == 0)
			return tree;
		else if (result < 0) {
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

	int compare(const KeyType& a, const KeyType& b) {
		return key_comp(a, b);
	}

	Node* upperNode(Node* tree, const KeyType& n) {
		if (tree == NULL)
			return NULL;
		int result = compare(n, tree->key);
		if (result == 0)
			return tree;
		else if (result < 0) {
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
	Node* eraseNode(Node* tree, const KeyType& n)
	{
		//不存在
		if (compare(n, tree->key) != 0 && tree->left == NULL && tree->right == NULL)
			return tree;

		if (compare(n, tree->key) < 0)
		{
			if (!isRed(tree->left) && !isRed(tree->left->left))
				tree = moveRedLeft(tree);
			setLeftTree(tree, eraseNode(tree->left, n));
		}
		else
		{
			if (isRed(tree->left))
				tree = roateRight(tree);
			if (compare(n, tree->key) == 0 && (tree->right == NULL))
				return NULL;
			if (!isRed(tree->right) && !isRed(tree->right->left))
				tree = moveRedRight(tree);

			//经过旋转后tree可能已经变化
			if (compare(n, tree->key) == 0)
			{
				Node* firstNode = first(tree->right);
				tree->key = firstNode->key;//assignment
				setRightTree(tree, eraseFirstNode(tree->right));
			}
			else 
				setRightTree(tree, eraseNode(tree->right, n));
		}
		return fixup(tree);
	}

	Node* eraseLastNode(Node* tree)
	{
		if (isRed(tree->left)) {
			tree = roateRight(tree);
			assert(tree->right);
			assert(isRed(tree->right));
		}

		if (tree->right == NULL) {
			delete tree;
			return NULL;
		}

		if (!isRed(tree->right) && !isRed(tree->right->left)) {
			tree = moveRedRight(tree);
		}

		setRightTree(tree, eraseLastNode(tree->right));
		return fixup(tree);
	}

	Node* eraseFirstNode(Node* tree)
	{
		if (tree->left == NULL) {
			delete tree;
			return NULL;
		}

		if (!isRed(tree->left) && !isRed(tree->left->left)) {
			tree = moveRedLeft(tree);
		}

		setLeftTree(tree, eraseFirstNode(tree->left));
		return fixup(tree);
	}

	Node* moveRedLeft(Node* tree)
	{
		flipColor(tree);
		if (isRed(tree->right->left)) {
			setRightTree(tree, roateRight(tree->right));
			tree = roateLeft(tree);
			flipColor(tree);
		}
		return tree;
	}

	Node* moveRedRight(Node* tree)
	{
		flipColor(tree);
		if (isRed(tree->left->left)) {
			tree = roateRight(tree);
			flipColor(tree);
		}
		return tree;
	}

	void setLeftTree(Node* tree, Node* sub) {
		tree->left = sub;
		if (sub)
			sub->parent = tree;
	}

	void setRightTree(Node* tree, Node* sub) {
		tree->right = sub;
		if (sub)
			sub->parent = tree;
	}

	//红结点左右儿子都是黑的
	bool verifyRedNode(Node* tree){
		if (isRed(tree))
			return !isRed(tree->left) && !isRed(tree->right);
		return true;
	}

	bool verifyBlackPathLength(Node* tree, size_t length) {
		if (tree->left || tree->right)
			return true;

		return blackPathLength(tree) == length;
	}

	size_t blackPathLength(Node* tree) {
		size_t pathLength = 0;;
		while (tree)
		{
			if (!isRed(tree))
				++pathLength;
			tree = tree->parent;
		}
		return pathLength;
	}
private:
	Node* root;
	KeyCompareType key_comp;

};
