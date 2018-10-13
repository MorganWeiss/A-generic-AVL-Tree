#include <iostream>
#include "AVL.h"

int main() {

	AVL<int> tree;
	std::unique_ptr<AVL<int>::TreeNode> root = nullptr;

	root = nullptr;
	root = tree.insert(root, 9);
	root = tree.insert(root, 5);
	root = tree.insert(root, 10);
	root = tree.insert(root, 0);
	root = tree.insert(root, 6);
	root = tree.insert(root, 11);
	root = tree.insert(root, -1);
	root = tree.insert(root, 1);
	root = tree.insert(root, 2);

	/* The constructed AVL Tree would be
			9
		   /  \
		  1    10
		/  \     \
	   0    5     11
	  /    /  \
	 -1   2    6
	*/

	std::cout << "Preorder traversal of the constructed AVL " "tree is \n";
	tree.preorderTraversal(root);
	std::cout << "\n";

	std::cout << "Postorder traversal of the constructed AVL " "tree is \n";
	tree.postorderTraversal(root);
	std::cout << "\n";

	std::cout << "Inorder traversal of the constructed AVL " "tree is \n";
	tree.inorderTraversal(root);
	std::cout << "\n";

	std::cout << "DFS" << std::endl;
	tree.DFS(root);
	std::cout << "\n";

	std::cout << "BFS" << std::endl;
	tree.BFS(root);
	std::cout << "\n";

	root = tree.deleteNode(root, 10);

	/* The AVL Tree after deletion of 10
			1
		   /  \
		  0    9
		/     /  \
	   -1    5     11
		   /  \
		  2    6
	*/

	std::cout << "Preorder traversal of the tree after deletion is \n";
	tree.preorderTraversal(root);
	std::cout << "\n";

	std::cout << "Inorder \n";
	tree.inorderTraversal(root);
	std::cout << "\n";



	tree.exists(root, 4) ? std::cout << "Yes" << std::endl : std::cout << "No"  << "\n";

	std::cin.get();
}
