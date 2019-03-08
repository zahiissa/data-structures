/*
 * AVLTree.h
 *
 *  Created on: Nov 22, 2015
 *      Author: Rush
 */

#ifndef AVLTREE_H_
#define AVLTREE_H_

#include <stdlib.h>
#include <stdexcept>
#include <iostream>

#define RROLL -2
#define LROLL 2
#define NULL_HEIGHT -1
//using namespace std;
class EXCEPTION: public std::exception {
};

class KEY_ALREADY_EXISTS: public EXCEPTION {
};

template<class K, class D>
class AVLTree;

template<class K, class D>
class AVLNode {
public:
	K key;
	D data;
	AVLNode* leftSon;
	AVLNode* rightSon;
	AVLNode* parent;
	int height;
	AVLNode() :
			key(), data(), leftSon(NULL), rightSon(NULL), parent(NULL), height(0)
	{
	}
	AVLNode(const K& key, const D& data, AVLNode* parent) :
			key(key), data(data), leftSon(NULL), rightSon(NULL), parent(parent), height(
					0)
	{
	}
	bool operator<(const AVLNode& node) const
	{
		return key < node.key;
	}
	bool operator>(const AVLNode& node) const
	{
		return key > node.key;
	}
	bool operator==(const AVLNode& node) const
	{
		return key == node.key;
	}
	bool operator!=(const AVLNode& node) const
	{
		return !(*this == node);
	}

	bool operator>=(const AVLNode& node) const
	{
		return key >= node.key;
	}
	bool operator<=(const AVLNode& node) const
	{
		return key <= node.key;
	}
	friend class AVLTree<K, D> ;
};

template<class K, class D>
class AVLTree {
public:
	AVLNode<K, D>* root;
	AVLNode<K, D>* max;
	int size;

	// find key if not exist return null
	AVLNode<K, D>* findKey(const K& key) const
	{
		AVLNode<K, D>* current = root;
		AVLNode<K, D>* parent = NULL;
		while (current != NULL) {
			if (key == current->key) {
				return current;
			}
			if (key > current->key) {
				parent = current;
				current = current->rightSon;
				continue;
			} else {
				parent = current;
				current = current->leftSon;
				continue;
			}
		}
		return parent;
	}
	///////////////////////////////////////////////////////////////////////////

	void printInOrder(AVLNode<K, D>* node)
	{
		if (node == NULL) {
			std::cout << std::endl;
			return;
		}
		printInOrder(node->leftSon);
		std::cout << "***" << node->key << "***";
		printInOrder(node->rightSon);
	}

	///////////////////////////////////////////////////////////////////////////
	bool removeAux(AVLNode<K, D>* node);
	void UpdateHeight(AVLNode<K, D>* v);
	void makeBalance(AVLNode<K, D>* parent);
	void LLrotate(AVLNode<K, D>& node);
	void RRrotate(AVLNode<K, D>& node);
	void LRrotate(AVLNode<K, D>& node);
	void RLrotate(AVLNode<K, D>& node);
	int nodeHeight(AVLNode<K, D>* node);
	int BF(AVLNode<K, D>* head);
	void SwapWithFollowing(AVLNode<K, D>& v);
//	void SwitchNodes(AVLNode<K, D>& node, AVLNode<K, D>& following);
	void SwitchSonFromv12V2(AVLNode<K, D>* v1, AVLNode<K, D>* v2);
public:
	AVLTree() :
			root(NULL), max(NULL), size(0)
	{
	}
	~AVLTree()
	{
		postOrderDelete(root);
	}
	D* insert(const K& key, const D& data);
	bool remove(const K& key);
	int getSize() const
	{
		return size;
	}
	D* find(const K& key);
	D* getMax()
	{
		if (max == NULL) {
			return NULL;
		}
		return &max->data;
	}
	void SwitchNodes(AVLNode<K, D>& node, AVLNode<K, D>& following);
	void getInBackOrder(AVLTree<K, D>& tree, D* arr)
	{
		int i = 0;
		getInBackOrderAux(tree.root, arr, &i);
	}
	void getInBackOrderAux(AVLNode<K, D>* v, D* arr, int* i)
	{
		if (!v) {
			return;
		}
		getInBackOrderAux(v->rightSon, arr, i);
		arr[*i] = v->data;
		(*i)++;
		getInBackOrderAux(v->leftSon, arr, i);
	}


	void getInInOrder(AVLTree<K, D>& tree, D* arr)
		{
			int i = 0;
			getInBackOrderAux(tree.root, arr, &i);
		}
		void getInInOrderAux(AVLNode<K, D>* v, D* arr, int* i)
		{
			if (!v) {
				return;
			}
			getInBackOrderAux(v->leftSon, arr, i);
			arr[*i] = v->data;
			(*i)++;
			getInBackOrderAux(v->rightSon, arr, i);

		}

	template<class Predicate>
	void DoInorder(Predicate p)
	{
		DoInorderAux(root, p);
	}
	template<class Predicate>
	void DoInorderAux(AVLNode<K, D>* node, Predicate p)
	{
		if (!node) {
			return;
		}
		DoInorderAux(node->leftSon, p);
		p(node->data);
		DoInorderAux(node->rightSon, p);
	}

//	template<class condition, class action>
//	void updateDataInTree(AVLNode<K, D>* node, condition c, action ac)
//	{
//		if (!node) {
//			return;
//		}
//		updateDataInTree(node->lefSon, c, ac);
//		if (c(node->data)) {
//			ac(node->data);
//		}
//		updateDataInTree(node->lefSon, c, ac);
//
//	}

	void postOrderDelete(AVLNode<K, D>* node)
	{
		if (!node) {
			return;
		}
		postOrderDelete(node->leftSon);
		postOrderDelete(node->rightSon);
		delete node;
	}

private:
	void updateMax();
};

/*
 * AVLTree Private functions
 */

template<class K, class D>
int AVLTree<K, D>::nodeHeight(AVLNode<K, D>* node)
{
	if (!node) {
		return NULL_HEIGHT;
	}
	return node->height;
}
template<class K, class D>
void AVLTree<K, D>::updateMax()
{
	if (!root) {
		max = NULL;
		return;
	}
	AVLNode<K, D>* tmpNode = root;
	while (tmpNode->rightSon) {
		tmpNode = tmpNode->rightSon;
	}
	max = tmpNode;
}

template<class K, class D>
void AVLTree<K, D>::UpdateHeight(AVLNode<K, D>* node)
{
	if (!node) {
		return;
	}
	node->height = 1
			+ std::max(nodeHeight(node->leftSon), nodeHeight(node->rightSon));
}

template<class K, class D>
void AVLTree<K, D>::RRrotate(AVLNode<K, D>& node)
{
	AVLNode<K, D>* righSon = node.rightSon;
	if (root->key == node.key) {
		root = righSon;
	}
	node.rightSon = righSon->leftSon;
	if (righSon->leftSon != NULL)
		righSon->leftSon->parent = &node;

	righSon->leftSon = &node;
	UpdateHeight(&node);
	UpdateHeight(righSon);
	AVLNode<K, D>* prevParent = node.parent;
	if (prevParent && prevParent->rightSon && (*prevParent->rightSon == node)) {
		prevParent->rightSon = righSon;
	} else if (prevParent && prevParent->leftSon
			&& (*prevParent->leftSon == node)) {
		prevParent->leftSon = righSon;
	} else {
		root = righSon;
	}
	righSon->parent = prevParent;
	UpdateHeight(prevParent);
	node.parent = righSon;
}

template<class K, class D>
void AVLTree<K, D>::LLrotate(AVLNode<K, D>& node)
{
	AVLNode<K, D>* leftSon = node.leftSon;
	if (root->key == node.key) {
		root = leftSon;
	}
	node.leftSon = leftSon->rightSon;
	if (leftSon->rightSon != NULL)
		leftSon->rightSon->parent = &node;

	leftSon->rightSon = &node;
	UpdateHeight(&node);
	UpdateHeight(leftSon);
	SwitchSonFromv12V2(&node, leftSon);
//	AVLNode<K, D>* prevParent = node.parent;
//	if (prevParent && prevParent->leftSon && (*prevParent->leftSon == node)) {
//		prevParent->leftSon = leftSon;
//	} else if (prevParent && prevParent->rightSon
//			&& (*prevParent->rightSon == node)) {
//		prevParent->rightSon = leftSon;
//	} else {
//		root = leftSon;
//	}
//	leftSon->parent = prevParent;
	UpdateHeight(node.parent);

	leftSon->parent = node.parent;
	node.parent = leftSon;

}

template<class K, class D>
void AVLTree<K, D>::LRrotate(AVLNode<K, D>& node)
{
	RRrotate(*node.leftSon);
	LLrotate(node);
}

template<class K, class D>
void AVLTree<K, D>::RLrotate(AVLNode<K, D>& node)
{
	LLrotate(*node.rightSon);
	RRrotate(node);
}

template<class K, class D>
void AVLTree<K, D>::makeBalance(AVLNode<K, D>* node)
{
	AVLNode<K, D>* tmpNode = node;
	while (tmpNode != NULL) {
		AVLNode<K, D>* parent = tmpNode->parent;
		int prevHeight = nodeHeight(tmpNode);
		UpdateHeight(tmpNode);
		int newHeight = nodeHeight(tmpNode);
		int bf = BF(tmpNode);
		switch (bf) {
		case 2:
			if (BF(tmpNode->leftSon) >= 0) {
				LLrotate(*tmpNode);
			} else if (BF(tmpNode->leftSon) == -1) {
				LRrotate(*tmpNode);
			}
			break;
		case -2:
			if (BF(tmpNode->rightSon) <= 0) {
				RRrotate(*tmpNode);
			} else if (BF(tmpNode->rightSon) == 1) {
				RLrotate(*tmpNode);
			}
			break;
		}
		if (prevHeight == newHeight) {
			return;
		}
		tmpNode = parent;
	}
	return;
}

template<class K, class D>
int AVLTree<K, D>::BF(AVLNode<K, D>* head)
{
	return nodeHeight(head->leftSon) - nodeHeight(head->rightSon);
}

template<class K, class D>
void AVLTree<K, D>::SwapWithFollowing(AVLNode<K, D>& v)
{
	AVLNode<K, D>* bigger = v.rightSon;
	if (bigger == NULL) {
		SwitchSonFromv12V2(&v, v.parent);
		return;
	}
	AVLNode<K, D>* following = bigger;
	while (bigger != NULL) {
		following = bigger;
		bigger = bigger->leftSon;
	}

	SwitchNodes(v, *following);

}

template<class K, class D>
void AVLTree<K, D>::SwitchSonFromv12V2(AVLNode<K, D>* v1, AVLNode<K, D>* v2)
{
	if (v1->parent) {
		if (v1->parent->rightSon == v1) {
			v1->parent->rightSon = v2;
		} else {
			v1->parent->leftSon = v2;
		}
	} else {
		root = v2;
	}
}

template<class K, class D>
void AVLTree<K, D>::SwitchNodes(AVLNode<K, D>& node, AVLNode<K, D>& following)
{
	if (&node == following.parent) {
		std::swap(node.height, following.height);

		if (node.leftSon == &following) {
			std::swap(node.rightSon, following.rightSon);
			node.leftSon = following.leftSon;
			following.leftSon = &node;
		} else {
			std::swap(node.leftSon, following.leftSon);
			node.rightSon = following.rightSon;
			following.rightSon = &node;
		}
		SwitchSonFromv12V2(&node, &following);
		following.parent = node.parent;
		if (following.leftSon) {
			following.leftSon->parent = &following;
		}
		if (following.rightSon) {
			following.rightSon->parent = &following;
		}
		if (node.rightSon) {
			node.rightSon->parent = &node;
		}
		if (node.leftSon) {
			node.leftSon->parent = &node;
		}
		return;
	}

	std::swap(node.height, following.height);
	std::swap(node.leftSon, following.leftSon);
	std::swap(node.rightSon, following.rightSon);
	SwitchSonFromv12V2(&node, &following);
	SwitchSonFromv12V2(&following,&node);
	AVLNode<K,D>* p = node.parent;
	node.parent = following.parent;
	following.parent = p;
	if (following.leftSon) {
		following.leftSon->parent = &following;
	}
	if (following.rightSon) {
		following.rightSon->parent = &following;
	}
	if (node.rightSon) {
		node.rightSon->parent = &node;
	}
	if (node.leftSon) {
		node.leftSon->parent = &node;
	}
}

/*
 * AVLTree public functions
 */

template<class K, class D>
bool AVLTree<K, D>::removeAux(AVLNode<K, D>* node)
{
	AVLNode<K, D>* parent;
	if (node->leftSon == NULL && node->rightSon == NULL) {

		parent = node->parent;
		if (parent && parent->leftSon == node) {
			parent->leftSon = NULL;
		} else if (parent && parent->rightSon == node) {
			parent->rightSon = NULL;
		} else {
			root = NULL;
		}
		delete node;
		makeBalance(parent);
		size--;
		return true;
	}
	if (node->rightSon == NULL || node->leftSon == NULL) {
		parent = node->parent;
		AVLNode<K, D>* son =
				(node->rightSon != NULL) ? node->rightSon : node->leftSon;
		son->parent = parent;
		if (node->parent == NULL) {
			root = son;
		} else {
			((node->parent)->leftSon == node) ?
					node->parent->leftSon = son : node->parent->rightSon = son;
		}

		delete node;
//			UpdateHeight(parent);
		makeBalance(parent);
		size--;
		return true;
	}
	SwapWithFollowing(*node);
	return removeAux(node);

}

template<class K, class D>
bool AVLTree<K, D>::remove(const K& key)
{
	AVLNode<K, D>* node = findKey(key);
	if (node == NULL || node->key != key) {
		return false;
	}
	if (max->key == key) {
		AVLNode<K, D>* prev = node->leftSon;
		if (prev == NULL) {
			max = node->parent;
		} else {
			AVLNode<K, D>* prev2 = prev;
			while (prev != NULL) {
				prev2 = prev;
				prev = prev->rightSon;
			}
			max = prev2;
		}
	}
	return removeAux(node);

}

template<class K, class D>
D* AVLTree<K, D>::insert(const K& key, const D& data)
{
	AVLNode<K, D>* node = findKey(key);
	if (node && node->key == key) {
		throw KEY_ALREADY_EXISTS();
	}
	AVLNode<K, D>* newNode = new AVLNode<K, D>(key, data, node);
	if (node == NULL) {
		root = newNode;
		max = newNode;
		size++;
		return &newNode->data;
	}
	if (key > node->key) {
		node->rightSon = newNode;
	} else {
		node->leftSon = newNode;
	}
	size++;
	newNode->parent = node;
	if (key > max->key) {
		max = newNode;
	}
	makeBalance(node);
	return &newNode->data;
}

template<class K, class D>
D* AVLTree<K, D>::find(const K& key)
{
	AVLNode<K, D>* node = findKey(key);
	if (node && node->key == key) {
		return &node->data;
	}
	return NULL;
}

#endif /* AVLTREE_H_ */
