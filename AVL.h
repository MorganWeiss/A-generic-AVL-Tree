#pragma once

#include <algorithm>  
#include <iostream>  
#include <memory>
#include <stack>
#include <utility>
#include <queue>



template <class T>
class AVL {
public:

	struct TreeNode {
		T data;
		int height;
		std::unique_ptr<TreeNode> left = nullptr;
		std::unique_ptr<TreeNode> right = nullptr;

		TreeNode(const T& x, 
			const int& y,
			std::unique_ptr<TreeNode>&& left = nullptr,
			std::unique_ptr<TreeNode>&& right = nullptr) :
			data(x),
			height(y),
			left(std::move(left)),
			right(std::move(right)) {}

		TreeNode(const int& theData) : data(theData), height(1) {}
	};
	std::unique_ptr<TreeNode> root = nullptr;

	AVL() = default;
	AVL(AVL const& source);
	AVL(AVL && move) noexcept;
	AVL& operator=(AVL &&move) noexcept;
	~AVL();

	AVL& operator=(AVL const &rhs);

	// Member functions
	bool empty() { return root == nullptr; }
	int height(std::unique_ptr<TreeNode>& root) const;
	int heightDiff(std::unique_ptr<TreeNode>& root) const;
	std::unique_ptr<TreeNode> rightRotate(std::unique_ptr<TreeNode>& y);
	std::unique_ptr<TreeNode> leftRotate(std::unique_ptr<TreeNode>& x);
	std::unique_ptr<TreeNode> insert(std::unique_ptr<TreeNode>& root, const T& theData);
	std::unique_ptr<TreeNode> insert(std::unique_ptr<TreeNode>& root, T&& theData);
	auto findMin(std::unique_ptr<TreeNode>& root) const;
	std::unique_ptr<TreeNode> deleteNode(std::unique_ptr<TreeNode>& root, const T& theData);
	int depth(std::unique_ptr<TreeNode>& root) const;
	bool exists(std::unique_ptr<TreeNode>& root, const T& d) const;
	bool validate(std::unique_ptr<TreeNode>& root, TreeNode* previous) const;
	void inorderTraversal(std::unique_ptr<TreeNode>& root) const;
	void preorderTraversal(std::unique_ptr<TreeNode>& root) const;
	void postorderTraversal(std::unique_ptr<TreeNode>& root) const;
	void DFS(std::unique_ptr<TreeNode>& root) const;
	void BFS(std::unique_ptr<TreeNode>& root) const;

private:

	// This is used for the copy constructor
	void copyTree(std::unique_ptr<TreeNode>& thisRoot, const TreeNode *sourceRoot) {
		if (!sourceRoot) thisRoot = nullptr;
		else {
			thisRoot = std::make_unique<TreeNode>(sourceRoot->data);
			thisRoot->data = sourceRoot->data;
			copyTree(thisRoot->left, sourceRoot->left.get());
			copyTree(thisRoot->right, sourceRoot->right.get());
		}
	}
};

template <class T>
AVL<T>::AVL(AVL<T> const& source) {
	if (!source.root) root = nullptr;
	else {
		copyTree(this->root, source.root.get());
	}
}

template <class T>
AVL<T>::AVL(AVL<T> && move) noexcept {
	move.swap(*this);
}

template <class T>
AVL<T>& AVL<T>::operator=(AVL<T> && move) noexcept {
	move.swap(*this);
	return *this;
}

template <class T>
AVL<T>::~AVL() {

}

template <class T>
AVL<T>& AVL<T>::operator=(AVL<T> const &rhs) {
	AVL copy(rhs);
	swap(copy);
	return *this;
}

template <class T>
int AVL<T>::height(std::unique_ptr<TreeNode>& root) const {
	if (!root) return 0;
	return root->height;
}

template <class T>
int AVL<T>::heightDiff(std::unique_ptr<TreeNode>& root) const {
	if (!root) return 0;
	return height(root->left) - height(root->right);
}

template <class T>
auto AVL<T>::rightRotate(std::unique_ptr<TreeNode>& y) -> std::unique_ptr<TreeNode> {
	std::unique_ptr<TreeNode> x = std::move(y->left);
	std::unique_ptr<TreeNode> T2 = std::move(x->right);

	// Perform rotation
	x->right = std::move(y);
	x->right->left = std::move(T2);

	// Update heights
	x->right->height = std::max(height(x->right->left), height(x->right->right)) + 1;
	x->height = std::max(height(x->left), height(x->right)) + 1;
	
	return std::move(x);
}

template <class T>
auto AVL<T>::leftRotate(std::unique_ptr<TreeNode>& x) -> std::unique_ptr<TreeNode> {
	std::unique_ptr<TreeNode> y = std::move(x->right);
	std::unique_ptr<TreeNode> T2 = std::move(y->left);

	// Perform rotation
	y->left = std::move(x);
	y->left->right = std::move(T2);

	// Update heights
	y->left->height = std::max(height(y->left->left), height(y->left->right)) + 1;
	y->height = std::max(height(y->left), height(y->right)) + 1;

	return std::move(y);
}

template <class T>
auto AVL<T>::insert(std::unique_ptr<TreeNode>& root, const T& theData) -> std::unique_ptr<TreeNode> {
	std::unique_ptr<TreeNode> newNode = std::make_unique<TreeNode>(theData);
	// Perform normal BST insertion
	if (root == nullptr) {
		return std::move(newNode);
	}

	else if (theData < root->data) {
		root->left = insert(root->left, theData);
	}

	else {
		root->right = insert(root->right, theData);
	}

	// Update height of this ancestor node
	root->height = 1 + std::max(height(root->left), height(root->right));

	// Get the balance factor of this ancestor node to check whether this node became unbalanced
	int balance = heightDiff(root);

	// If this node become unbalaced, then we have 4 cases

	// Left Left Case
	if (balance > 1 && root->left && theData < root->left->data)
		return rightRotate(root);

	// Right Right Case
	if (balance < -1 && root->right && theData > root->right->data)
		return leftRotate(root);

	// Left Right Case
	if (balance > 1 && root->left && theData > root->left->data) {
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}

	// Right Left Case
	if (balance < -1 && root->right && theData < root->right->data) {
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}

	return std::move(root);
}

template <class T>
auto AVL<T>::insert(std::unique_ptr<TreeNode>& root, T&& theData) -> std::unique_ptr<TreeNode> {
	std::unique_ptr<TreeNode> newNode = std::make_unique<TreeNode>(std::move(theData));
	// Perform normal BST insertion
	if (root == nullptr) {
		return std::move(newNode);
	}

	else if (theData < root->data) {
		root->left = insert(root->left, std::move(theData));
	}

	else {
		root->right = insert(root->right, std::move(theData));
	}

	// Update height of this ancestor node
	root->height = 1 + std::max(height(root->left), height(root->right));

	// Get the balance factor of this ancestor node to check whether this node became unbalanced
	int balance = heightDiff(root);

	// If this node become unbalaced, then we have 4 cases

	// Left Left Case
	if (balance > 1 && root->left && theData < root->left->data)
		return rightRotate(root);

	// Right Right Case
	if (balance < -1 && root->right && theData > root->right->data)
		return leftRotate(root);

	// Left Right Case
	if (balance > 1 && root->left && theData > root->left->data) {
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}

	// Right Left Case
	if (balance < -1 && root->right && theData < root->right->data) {
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}

	return std::move(root);
}

template <class T>
auto AVL<T>::findMin(std::unique_ptr<TreeNode>& root) const {
	while (root->left) root = std::move(root->left);
	return root.get();
}

template <class T>
auto AVL<T>::deleteNode(std::unique_ptr<TreeNode>& root, const T& theData) -> std::unique_ptr<TreeNode> {
	if (!root)
		return std::move(root);

	if (theData < root->data)
		root->left = deleteNode(root->left, theData);

	else if (theData > root->data)
		root->right = deleteNode(root->right, theData);

	else {
		// Case 1: No child
		if (root->left == nullptr && root->right == nullptr) {
			root = nullptr;
		}

		// Case 2: One child
		else if (root->left == nullptr) {
			root->data = root->right->data;
			root->right = nullptr;
		}

		else if (root->right == nullptr) {
			root->data = root->left->data;
			root->left = nullptr;
		}

		// Case 3: Two children
		else {
			auto temp = findMin(root->right);
			temp->data = root->data;
			root->right = deleteNode(root->right, temp->data);
		}
	}

	// Step 2: Update height of the current node
	root->height = 1 + std::max(height(root->left), height(root->right));

	// Step 3: Get the balalce factor of the this node (to
	// check whether this node became unbalanced)
	int balance = heightDiff(root);

	// If this node becomes unbalanced, then there are 4 cases

	// Left Left Case
	if (balance > 1 && heightDiff(root->left) >= 0)
		return rightRotate(root);

	// Left Right Case
	if (balance > 1 && heightDiff(root->left) < 0) {
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}

	// Right Right Case
	if (balance < -1 && heightDiff(root->right) <= 0)
		return leftRotate(root);

	// Right Left Case
	if (balance < -1 && heightDiff(root->right) > 0) {
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}

	return std::move(root);
}

template <class T>
int AVL<T>::depth(std::unique_ptr<TreeNode>& root) const {
	if (!root) return 0;
	else {
		int l = getDepth(root->left);
		int r = getDepth(root->right);
		return std::max(l, r) + 1;
	}
}

template <class T>
bool AVL<T>::exists(std::unique_ptr<TreeNode>& root, const T& d) const {
	auto temp = root.get();
	while (temp != nullptr) {
		if (temp->data == d) {
			return true;
		}
		else {
			if (d > temp->data) {
				temp = temp->right.get();
			}
			else {
				temp = temp->left.get();
			}
		}
	}
	return false;
}

template <class T>
bool AVL<T>::validate(std::unique_ptr<TreeNode>& root, TreeNode* previous) const {
	if (!root) return true;
	else if (!validate(root->left, previous)) return false;
	if (previous && previous->data >= root->data) return false;
	previous = root.get();
	return validate(root->right, previous);
}

template <class T>
void AVL<T>::inorderTraversal(std::unique_ptr<TreeNode>& root) const {
	if (root != nullptr) {
		inorderTraversal(root->left);
		std::cout << root->data << " ";
		inorderTraversal(root->right);
	}
}

template <class T>
void AVL<T>::preorderTraversal(std::unique_ptr<TreeNode>& root) const {
	if (root != nullptr) {
		std::cout << root->data << " ";
		preorderTraversal(root->left);
		preorderTraversal(root->right);
	}
}

template <class T>
void AVL<T>::postorderTraversal(std::unique_ptr<TreeNode>& root) const {
	if (root != nullptr) {
		postorderTraversal(root->left);
		postorderTraversal(root->right);
		std::cout << root->data << " ";
	}
}

template <class T>
void AVL<T>::DFS(std::unique_ptr<TreeNode>& root) const {
	if (!root) return;
	std::stack<TreeNode const *> s;
	s.push(root.get());

	while (!s.empty()) {
		auto p = s.top();
		s.pop();
		std::cout << p->data << " ";

		if (p->right) s.push(p->right.get());
		if (p->left) s.push(p->left.get());
	}
}

template <class T>
void AVL<T>::BFS(std::unique_ptr<TreeNode>& root) const {
	if (!root) return;
	std::queue<TreeNode const *> q;
	q.push(root.get());

	while (!q.empty()) {
		auto p = q.front();
		q.pop();
		std::cout << p->data << " ";

		if (p->left) q.push(p->left.get());
		if (p->right) q.push(p->right.get());
	}
}