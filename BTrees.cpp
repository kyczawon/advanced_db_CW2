#include <cstdlib>
#include <utility>
#include <iostream>
#include <optional>
#include "BTrees.hpp"
using namespace std;

/**
 * A Node in the Tree,
 *
 * NodeSize is the out-degree (the number of children),
 *
 * T is the type of the held value
 */
template <size_t NodeSize, typename T> struct BTreeNode {
	/**
	 * An element in the node consisting of a pivot and the subtree to its left
	 */
	struct NodeElement {
		/**
		 * The node to the left of the pivot
		 */
		BTreeNode<NodeSize, T>* childToLeft{};
		/**
		 * The pivot, we're using the optional template to encode pivots slots that don't contain a
		 * value
		 */
		optional<T> pivot{};
	};

	/**
	 * A struct to (temporarily) hold newly created splits
	 */
	struct NewSplit {
		/**
		 * the newly introduced pivot, this needs merging into a node
		 */
		T newPivot;
		/**
		 * the newly create subtree to the right of the pivot
		 */
		BTreeNode<NodeSize, T>* childToTheRightOfPivot;
	};

	/**
	 * since the out-degree is NodeSize, we have NodeSize-1 pivots
	 */
	NodeElement pivots[NodeSize - 1]{};
	/**
	 * the child to the very right of the node, it is not associated/paired with a pivot
	 */
	BTreeNode<NodeSize, T>* rightMost = nullptr;

	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	////////// There is no need for you to change anything above this line//////////
	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////

	/**
	 * This is a convenience function that Holger used to implement this,
	 * feel free to ignore it and implement your own structure
	 */
	NewSplit splitNode(T first, BTreeNode<NodeSize, T>* newChild) {
		// you may or may not implement this function
		return {};
	}

	/**
	 * This is a convenience function that Holger used to implement this,
	 * feel free to ignore it and implement your own structure
	 */
	NewSplit mergeValueIntoNode(T first, BTreeNode<NodeSize, T>* second, bool isLeaf = false) {
		// you may or may not implement this function
		return {};
	}

	/**
	 * Inserts the value into the Node (or any of it children)
	 *
	 * !!!!! You *must* implement this function !!!!!
	 *
	 * If the node was split, returns a new pivot and a new right child
	 */
	NewSplit insert(T v) { return {}; }

	/**
	 * Count the number of occurences of the value v
	 *
	 * !!!!! You *must* implement this function !!!!!
	 *
	 * You can assume uniqueness of the values v, thus this function
	 * returns either 0 of 1
	 */
	size_t count(T v) { return {}; }
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////// There is no need for you to change anything below this line//////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template <size_t NodeSize, typename T> void BTree<NodeSize, T>::insert(T v) {
	auto newNode = root->insert(v);
	auto oldRoot = root;
	if(newNode.childToTheRightOfPivot) {
		root = new BTreeNode<NodeSize, T>{};
		root->pivots[0].pivot = newNode.newPivot;
		root->pivots[0].childToLeft = oldRoot;
		root->pivots[1].childToLeft = newNode.childToTheRightOfPivot;
	}
}

template <size_t NodeSize, typename T>
BTree<NodeSize, T>::BTree(T v)
		: root(new BTreeNode<NodeSize, T>) {
	root->pivots[0].pivot = v;
}

template <size_t NodeSize, typename T> size_t BTree<NodeSize, T>::count(T v) {
	return root->count(v);
}

template <size_t NodeSize, typename T>
ostream& operator<<(ostream& o, typename BTreeNode<NodeSize, T>::NodeElement const& v) {
	return v.childToLeft ? (o << *(v.childToLeft) << ", " << *v.pivot) : (o << *v.pivot);
}

template <size_t NodeSize, typename T> ostream& operator<<(ostream& o, BTreeNode<NodeSize, T> const& v) {
	o << "[";
	operator<<<NodeSize, T>(o, v.pivots[0]);
	for(auto it = next(begin(v.pivots)); it != end(v.pivots); ++it) {
		if(it->pivot)
			operator<<<NodeSize, T>(o << ", ", *it);
		else if(it->childToLeft)
			o << ", " << *it->childToLeft;
	}
	return (v.rightMost ? (o << ", " << *v.rightMost) : o) << "]";
}

template <size_t NodeSize, typename T>
ostream& operator<<(ostream& o, BTree<NodeSize, T> const& v) {
	return o << *v.root << endl;
}

template class BTree<4>;
template ostream& operator<<(ostream& o, BTree<4, long> const& v);
template class BTree<5>;
template ostream& operator<<(ostream& o, BTree<5, long> const& v);
template class BTree<6>;
template ostream& operator<<(ostream& o, BTree<6, long> const& v);
