#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>

#include <chrono>
using namespace std::chrono;

using std::cin;
using std::cout;
using std::endl;
using std::string;

// class node for binary search tree using AVL algorithm
class Node
{
public:
    int value;
    int height;
    string data;
    Node *left;
    Node *right;

    Node(int value, string data) /*constructor*/
    {
        this->value = value;
        this->data = data;
        this->height = 1;
        this->left = nullptr;
        this->right = nullptr;
    }
    ~Node() /*destructor*/
    {
        delete left;
        delete right;
    }
};

// class AVL tree
class AVL
{
public:
    Node *root;
    AVL() /* constructor */
    {
        root = nullptr;
    }

    ~AVL() /* destructor */
    {
        delete root;
    }

    // function to get the height of the tree
    int Height(Node *node)
    {
        if (node == nullptr)
        {
            return 0;
        }

        return node->height;
    }

    // function to get the balance factor of the tree
    int balanceFactor(Node *node)
    {
        if (node == nullptr)
        {
            return 0;
        }

        return (Height(node->left) - Height(node->right));
    }

    // function to update the height of the tree
    int updateHeight(Node *node)
    {
        if (node == nullptr)
        {
            return 0;
        }

        return (Height(node->left) > Height(node->right) ? Height(node->left) + 1 : Height(node->right) + 1);
    }

    // function to rotate the tree to the right
    Node *rotateRight(Node *node)
    {
        Node *left = node->left;
        Node *leftRight = left->right;

        left->right = node;
        node->left = leftRight;

        node->height = updateHeight(node);
        left->height = updateHeight(left);

        return left;
    }

    // function to rotate the tree to the left
    Node *rotateLeft(Node *node)
    {
        Node *right = node->right;
        Node *rightLeft = right->left;

        right->left = node;
        node->right = rightLeft;

        node->height = updateHeight(node);
        right->height = updateHeight(right);

        return right;
    }

    // function to insert new node into the tree
    Node *insert(Node *node, int value, string data)
    {
        if (node == nullptr)
        {
            return new Node(value, data);
        }

        if (value < node->value)
        {
            node->left = insert(node->left, value, data);
        }
        else if (value > node->value)
        {
            node->right = insert(node->right, value, data);
        }
        else /*ignoring duplicates*/
        {
            return node;
        }

        /*balancing using rotations*/

        node->height = updateHeight(node);

        int balance = balanceFactor(node);

        if (balance > 1 && value < node->left->value)
        {
            return rotateRight(node);
        }

        if (balance < -1 && value > node->right->value)
        {
            return rotateLeft(node);
        }

        if (balance > 1 && value > node->left->value)
        {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        if (balance < -1 && value < node->right->value)
        {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    // function to delete node from the tree
    Node *deleteNode(Node *node, int value)
    {
        if (node == nullptr)
        {
            return node;
        }

        if (value < node->value)
        {
            node->left = deleteNode(node->left, value);
        }
        else if (value > node->value)
        {
            node->right = deleteNode(node->right, value);
        }
        else
        {
            if (node->left == nullptr || node->right == nullptr)
            {
                Node *temp = node->left ? node->left : node->right; /*if left not null then left, else right*/

                if (temp == nullptr)
                {
                    temp = node;
                    node = nullptr;
                }
                else
                    *node = *temp;
                delete temp;
            }
            else
            {
                Node *temp = minValueNode(node->right);

                node->value = temp->value;
                node->data = temp->data;

                node->right = deleteNode(node->right, temp->value);
            }
        }

        /*balancing using rotations*/

        if (node == nullptr)
            return node;

        node->height = updateHeight(node);

        int balance = balanceFactor(node);

        if (balance > 1 && balanceFactor(node->left) >= 0)
        {
            return rotateRight(node);
        }

        if (balance > 1 && balanceFactor(node->left) < 0)
        {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        if (balance < -1 && balanceFactor(node->right) <= 0)
        {
            return rotateLeft(node);
        }

        if (balance < -1 && balanceFactor(node->right) > 0)
        {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    // function to find the minimum value of the tree
    Node *minValueNode(Node *node)
    {
        Node *current = node;

        while (current->left != nullptr)
        {
            current = current->left;
        }

        return current;
    }

    // function to search for a node in the tree
    Node *search(Node *node, int value)
    {
        if (node == nullptr || node->value == value)
        {
            return node;
        }

        if (node->value < value)
        {
            return search(node->right, value);
        }

        return search(node->left, value);
    }

    // print function for debugging
    void print(const string &prefix, Node *node, bool isLeft)
    {
        if (node != nullptr)
        {
            cout << prefix;

            cout << (isLeft ? "|--" : "L--");

            // print the value of the node
            cout << node->value << " data: " << node->data << endl;

            // enter the next tree level - left and right branch
            print(prefix + (isLeft ? "|   " : "    "), node->left, true);
            print(prefix + (isLeft ? "|   " : "    "), node->right, false);
        }
    }
};