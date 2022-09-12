#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

class BTree
{
private:
	class Node {
	public:
		vector<int> keys;
		vector<Node*> children;

		int searchPos(int key)
		{
			for (int i = 0; i < keys.size(); ++i)
			{
				if (key <= keys[i])
					return i;
			}
			return keys.size();
		}

		bool findKey(int pos, int key)
		{
			if (pos < keys.size())
				return (keys[pos] == key);
			return false;
		}

		bool enoughSize(int t)
		{
			return (keys.size() >= t);
		}
	};

public:

	Node* root;
	int parametr_t;

	BTree(int t)
	{
		parametr_t = t;
		root = new Node();
	}

	bool isEmpty()
	{
		if (root == nullptr)
			return true;
		if (root->keys.size() == 0)
			return true;
		return false;
	}

	void insert(int key)
	{
		Node* node = root;
		Node* parent = nullptr;
		while (true)
		{
			if (node->keys.size() == 2 * parametr_t - 1)
				node = splitNode(parent, node);
			if (node->children.size() == 0)
				break;
			else
			{
				int pos = node->searchPos(key);
				Node* left = node->children[pos];
				parent = node;
				node = left;
			}
		}
		int pos = node->searchPos(key);
		node->keys.insert(node->keys.begin() + pos, key);
	}

	bool remove(int key)
	{
		Node* node = root;
		while (true)
		{
			int pos = node->searchPos(key);
			if (node->children.size() == 0)
			{
				if (!node->findKey(pos, key))
					return false;
				else
				{
					deleteToLeaf(node, pos);
					return true;
				}
			}
			Node* left = node->children[pos];
			if (node->findKey(pos, key))
			{
				deleteToNotLeaf(node, pos);
				return true;
			}
			else
			{
				if (!left->enoughSize(parametr_t))
				{
					Pair result = makeMinSize(node, left, pos);
					node = result.parent;
					left = result.node;
				}
				node = left;
			}
		}
	}

	bool search(int key)
	{
		if (root == nullptr)
			return false;

		Node* node = root;
		Node* parent = nullptr;

		while (true)
		{
			int pos = node->searchPos(key);
			if (node->children.size() == 0)
			{
				if (!node->findKey(pos, key))
					return false;
				else
					return true;
			}
			Node* left = node->children[pos];
			if (node->findKey(pos, key))
				return true;
			else
			{
				parent = node;
				node = left;
			}
		}
	}

	~BTree()
	{
		if (root == nullptr)
			return;
		clear();
	}

	void clear()
	{
		while (root != nullptr && root->keys.size() != 0)
		{
			remove(root->keys[0]);
		}
		delete root;
		root = nullptr;
	}

private:
	class Pair
	{
	public:
		Node* parent;
		Node* node;

		Pair() {}

		Pair(Node* parent, Node* node) : parent(parent), node(node) {}
	};

	Node* splitNode(Node* parent, Node* node)
	{
		if (parent == nullptr)
		{
			parent = new Node();
			root = parent;
		}

		Node* left = new Node();
		Node* right = new Node();

		if (node->children.size() == 0)
		{
			for (int i = 0; i < parametr_t - 1; ++i)
			{
				left->keys.push_back(node->keys[i]);
			}
			for (int i = parametr_t; i < node->keys.size(); ++i)
			{
				right->keys.push_back(node->keys[i]);
			}
		}
		else
		{
			for (int i = 0; i < parametr_t - 1; ++i)
			{
				left->keys.push_back(node->keys[i]);
				left->children.push_back(node->children[i]);
			}
			left->children.push_back(node->children[parametr_t - 1]);
			for (int i = parametr_t; i < node->keys.size(); ++i)
			{
				right->keys.push_back(node->keys[i]);
				right->children.push_back(node->children[i]);
			}
			right->children.push_back(node->children[node->keys.size()]);
		}

		int key = node->keys[parametr_t - 1];
		int pos = parent->searchPos(key);

		if (parent->keys.size() == 0)
		{
			parent->keys.push_back(key);
			parent->children.push_back(left);
			parent->children.push_back(right);
		}
		else
		{
			parent->keys.insert(parent->keys.begin() + pos, key);
			parent->children.insert(parent->children.begin() + pos, left);
			parent->children[pos + 1] = right;
		}
		delete node;
		return parent;
	}

	void deleteToNotLeaf(Node* node, int pos)
	{
		Node* leftChild = node->children[pos];
		Node* rightChild = node->children[pos + 1];
		int replaceValue;

		if (leftChild->enoughSize(parametr_t))
		{
			replaceValue = changeToMaxLeft(leftChild);
			node->keys[pos] = replaceValue;
		}
		else if (rightChild->enoughSize(parametr_t))
		{
			replaceValue = changeToMinRight(rightChild);
			node->keys[pos] = replaceValue;
		}
		else
		{
			int newPos = leftChild->keys.size();

			Pair result = mergeNodes(node, leftChild, rightChild, pos);
			node = result.parent;
			leftChild = result.node;
			if (leftChild->children.size() == 0)
				deleteToLeaf(leftChild, newPos);
			else
				deleteToNotLeaf(leftChild, newPos);
		}
	}

	int changeToMaxLeft(Node* parent)
	{
		int replaceValue;
		if (parent->children.size() == 0)
		{
			replaceValue = parent->keys.back();
			deleteToLeaf(parent, parent->keys.size() - 1);
			return replaceValue;
		}

		Node* node = parent->children.back();
		int posParent = parent->children.size() - 1;

		while (true)
		{
			if (!node->enoughSize(parametr_t))
			{
				Pair result = makeMinSize(parent, node, posParent);
				parent = result.parent;
				node = result.node;
			}

			if (node->children.size() == 0)
				break;

			parent = node;
			posParent = parent->children.size() - 1;
			node = node->children.back();
		}

		replaceValue = node->keys.back();
		deleteToLeaf(node, node->keys.size() - 1);
		return replaceValue;
	}

	int changeToMinRight(Node* parent)
	{
		int replaceValue;
		if (parent->children.size() == 0)
		{
			replaceValue = parent->keys.front();
			deleteToLeaf(parent, 0);
			return replaceValue;
		}

		Node* node = parent->children.front();
		int posParent = 0;

		while (true)
		{
			if (!node->enoughSize(parametr_t))
			{
				Pair result = makeMinSize(parent, node, posParent);
				parent = result.parent;
				node = result.node;
			}

			if (node->children.size() == 0)
				break;

			parent = node;
			node = node->children.front();
		}

		replaceValue = node->keys.front();
		deleteToLeaf(node, 0);
		return replaceValue;
	}

	void deleteToLeaf(Node* node, int pos)
	{
		node->keys.erase(node->keys.begin() + pos);
	}

	Pair makeMinSize(Node* parent, Node* nodeLeft, int posParent)
	{
		Node* leftChild;
		if (posParent > 0)
			leftChild = parent->children[posParent - 1];
		else
			leftChild = nullptr;
		Node* rightChild;
		if (posParent < parent->keys.size())
			rightChild = parent->children[posParent + 1];
		else
			rightChild = nullptr;

		if (leftChild == nullptr)
		{
			if (rightChild->enoughSize(parametr_t))
				fromRightToLeft(parent, nodeLeft, rightChild, posParent);
			else
				return mergeNodes(parent, nodeLeft, rightChild, posParent);
		}
		else if (rightChild == nullptr)
		{
			if (leftChild->enoughSize(parametr_t))
				fromLeftToRight(parent, leftChild, nodeLeft, posParent - 1);
			else
				return mergeNodes(parent, leftChild, nodeLeft, posParent - 1);
		}
		else
		{
			if (leftChild->enoughSize(parametr_t) && rightChild->enoughSize(parametr_t))
				if (rightChild->keys.size() > leftChild->keys.size())
					fromRightToLeft(parent, nodeLeft, rightChild, posParent);
				else
					fromLeftToRight(parent, leftChild, nodeLeft, posParent - 1);
			else if (leftChild->enoughSize(parametr_t))
				fromLeftToRight(parent, leftChild, nodeLeft, posParent - 1);
			else if (rightChild->enoughSize(parametr_t))
				fromRightToLeft(parent, nodeLeft, rightChild, posParent);
			else
				return mergeNodes(parent, leftChild, nodeLeft, posParent - 1);
		}

		return Pair(parent, nodeLeft);
	}

	Pair mergeNodes(Node* parent, Node* left, Node* right, int posParent)
	{
		int keyParent = parent->keys[posParent];

		left->keys.push_back(keyParent);
		if (right->children.size() == 0)
		{
			for (int i = 0; i < right->keys.size(); ++i)
			{
				left->keys.push_back(right->keys[i]);
			}
		}
		else
		{
			for (int i = 0; i < right->keys.size(); ++i)
			{
				left->keys.push_back(right->keys[i]);
				left->children.push_back(right->children[i]);
			}
			left->children.push_back(right->children[right->keys.size()]);
		}

		delete right;
		parent->keys.erase(parent->keys.begin() + posParent);
		parent->children.erase(parent->children.begin() + posParent);

		if ((parent == root) && (parent->keys.size() == 0))
		{
			delete parent;
			root = left;
			parent = nullptr;
		}
		else
			parent->children[posParent] = left;
		return Pair(parent, left);
	}

	void fromLeftToRight(Node* parent, Node* left, Node* right, int posParent)
	{
		int posMaxLeft = left->keys.size() - 1;
		int keyParent = parent->keys[posParent];

		right->keys.insert(right->keys.begin(), keyParent);

		if (!left->children.size() == 0)
		{
			Node* child = left->children[posMaxLeft + 1];
			right->children.insert(right->children.begin(), child);
			left->children.erase(left->children.begin() + (left->children.size() - 1));
		}

		int maxLeft = left->keys.back();
		left->keys.erase(left->keys.begin() + (left->keys.size() - 1));

		parent->keys[posParent] = maxLeft;
	}

	void fromRightToLeft(Node* parent, Node* left, Node* right, int posParent)
	{
		int posMinRight = 0;
		int keyParent = parent->keys[posParent];

		left->keys.push_back(keyParent);

		if (!right->children.size() == 0)
		{
			Node* child = right->children[posMinRight];
			left->children.push_back(child);
			right->children.erase(right->children.begin());
		}

		int minRight = right->keys.front();
		right->keys.erase(right->keys.begin());

		parent->keys[posParent] = minRight;
	}
};



int main(int argc, char* argv[]) {
	if (argc < 3) {
		return 1;
	}
	std::ifstream inFile(argv[1]);
	if (!inFile) {
		return 2;
	}
	std::ofstream outFile(argv[2]);
	if (!outFile) {
		return 3;
	}

	/*ifstream inFile("in.txt");
	ofstream outFile("out.txt");*/


	int t, n;
	inFile >> t >> n;
	BTree btree(t);
	char command;
	int value;

	for (int i = 0; i < n; ++i)
	{
		command = 0;
		inFile >> command;

		if (command == '+')
		{
			inFile >> value;
			btree.insert(value);

			int size = btree.root->keys.size();
			outFile << size << " ";
			for (int i = 0; i < size - 1; ++i)
				outFile << btree.root->keys[i] << " ";
			outFile << btree.root->keys[size - 1];
			outFile << '\n';
		}
		else if (command == '-')
		{
			inFile >> value;
			btree.remove(value);
			if (btree.isEmpty())
				outFile << "0" << '\n';
			else
			{
				int size = btree.root->keys.size();
				outFile << size << " ";
				for (int i = 0; i < size - 1; ++i)
					outFile << btree.root->keys[i] << " ";
				outFile << btree.root->keys[size - 1];
				outFile << '\n';
			}
		}
		else if (command == '?')
		{
			inFile >> value;
			if (btree.search(value))
			{
				outFile << "true" << '\n';
			}
			else
			{
				outFile << "false" << '\n';
			}
		}
	}

	inFile.close();
	outFile.close();
	return 0;
}