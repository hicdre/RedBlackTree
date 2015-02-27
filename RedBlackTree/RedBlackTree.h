#pragma once
#include <cassert>
#include <functional>

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
	}

	void clear() {
		deleteNode(root);
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

	bool verify() {
		Node* n = first(root);
		while (n)
		{
			Node* b = next(n);
			if (b && b->key <= n->key) {
				return false;
			}
			n = b;
		}
		return true;
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
			tree->left = insert(tree->left, n);
			tree->left->parent = tree;
		}
		else {
			tree->right = insert(tree->right, n);
			tree->right->parent = tree;
		}

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
		Node* newTree = tree->right;
		newTree->parent = tree->parent;
		tree->parent = newTree;

		tree->right = newTree->left;
		if (tree->right)
			tree->right->parent = tree;
		newTree->left = tree;

		newTree->red = tree->red;
		tree->red = true;
		assert(tree->parent != tree);
		return newTree;
	}

	static Node* roateRight(Node* tree){
		//右转
		Node* newTree = tree->left;
		newTree->parent = tree->parent;
		tree->parent = newTree;

		tree->left = newTree->right;
		if (tree->left)
			tree->left->parent = tree;
		newTree->right = tree;

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

	static void deleteNode(Node* tree) {
		if (tree == NULL)
			return;
		deleteNode(tree->left);
		deleteNode(tree->right);
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
private:
	Node* root;

};