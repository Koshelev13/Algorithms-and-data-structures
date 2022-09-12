#include <fstream>
#include <iostream>
#include <string>
using namespace std;

template <typename T>
class AVL {
public:
    class node {
    public:
        T key;
        int height;
        node* left;
        node* right;
        node(T k) {
            height = 1;
            key = k;
            left = nullptr;
            right = nullptr;
        }
    };
    node* root = nullptr;

    int balance()
    {
        return GetBalance(root);
    }

    void insert(T x) {
        root = insertInside(root, x);
    }
    void remove(T x) {
        root = removeInside(root, x);
    }
    bool search(T x) {
        return searchInside(root, x);
    }

private:
    int GetBalance(node* head)
    {
        if (head == nullptr) return 0;
        return GetHeight(head->right) - GetHeight(head->left);
    }
    int GetHeight(node* head) {
        if (head == nullptr) return 0;
        return head->height;
    }
    node* rightRotation(node* head) {
        node* newhead = head->left;
        head->left = newhead->right;
        newhead->right = head;
        head->height = max(GetHeight(head->left), GetHeight(head->right)) + 1;
        newhead->height = max(GetHeight(newhead->left), GetHeight(newhead->right)) + 1;
        return newhead;
    }

    node* leftRotation(node* head) {
        node* newhead = head->right;
        head->right = newhead->left;
        newhead->left = head;
        head->height = max(GetHeight(head->left), GetHeight(head->right)) + 1;
        newhead->height = max(GetHeight(newhead->left), GetHeight(newhead->right)) + 1;
        return newhead;
    }

    node* insertInside(node* head, T x) {
        if (head == nullptr) {
            node* temp = new node(x);
            return temp;
        }
        if (x < head->key) head->left = insertInside(head->left, x);
        else if (x > head->key) head->right = insertInside(head->right, x);
        head->height = max(GetHeight(head->left), GetHeight(head->right)) + 1;
        if (GetBalance(head) > 1)
        {
            if (GetBalance(head->right) < 0)
                head->right = rightRotation(head->right);
            return leftRotation(head);
        }
        else if (GetBalance(head) < -1)
        {
            if (GetBalance(head->left) > 0)
                head->left = leftRotation(head->left);
            return rightRotation(head);
        }
        return head;
    }
    node* removeInside(node* head, T x) {
        if (head == nullptr) return nullptr;
        if (x < head->key) {
            head->left = removeInside(head->left, x);
        }
        else if (x > head->key) {
            head->right = removeInside(head->right, x);
        }
        else {
            node* r = head->right;
            if (head->right == nullptr) {
                node* l = head->left;
                delete(head);
                head = l;
            }
            else if (head->left == nullptr) {
                delete(head);
                head = r;
            }
            else {
                while (r->left != nullptr) r = r->left;
                head->key = r->key;
                head->right = removeInside(head->right, r->key);
            }
        }
        if (head == nullptr) return head;
        head->height = max(GetHeight(head->left), GetHeight(head->right)) + 1;
        if (GetBalance(head) > 1)
        {
            if (GetBalance(head->right) < 0)
                head->right = rightRotation(head->right);
            return leftRotation(head);
        }
        else if (GetBalance(head) < -1)
        {
            if (GetBalance(head->left) > 0)
                head->left = leftRotation(head->left);
            return rightRotation(head);
        }
        return head;
    }
    bool searchInside(node* head, T x) {
        if (head == nullptr) return false;
        if (head->key == x) return true;
        if (x < head->key) return searchInside(head->left, x);
        if (x > head->key) return searchInside(head->right, x);
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
    /*ifstream inFile;
    inFile.open("in.txt");
    ofstream outFile;
    outFile.open("out.txt");*/
    AVL<int> avl;
    int n;
    inFile >> n;
    char command;
    int a;
    for (int i = 0; i < n; i++)
    {
        inFile >> command;
        inFile >> a;
        if (command == '+')
        {
            avl.insert(a);
            int height = avl.root->height;
            outFile << avl.balance() << endl;
        }
        else if (command == '-')
        {
            avl.remove(a);
            outFile << avl.balance() << endl;
        }
        else if (command == '?')
        {
            if (avl.search(a))
                outFile << "true" << endl;
            else
                outFile << "false" << endl;
        }
    }
    inFile.close();
    outFile.close();
    return 0;
}