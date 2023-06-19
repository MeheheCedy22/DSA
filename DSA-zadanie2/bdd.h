// input will be in DNF form (i.e. a+bc+D == A+BC+!D)
// lowercase letters represent variable without negation
// uppercase letters represent variable with negation

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

struct Node
{
    char data;
    string bfunction;
    Node *l;
    Node *r;

    Node()
    {
        this->data = '\0';
        l = nullptr;
        r = nullptr;
    }

    Node(char data)
    {
        this->data = data;
        l = nullptr;
        r = nullptr;
    }
};

class BDD
{
public:
    Node *root = nullptr;
    int var_count;   // get value in bdd create
    size_t size = 2; // number of nodes in BDD 2 because of 0 and 1 nodes
    string poradie;

    Node *false_node = new Node('0');
    Node *true_node = new Node('1');

    // Map to store the function and root pointer
    std::unordered_map<string, Node *> function_map;
    std::unordered_map<Node *, char> printedNodes;
    std::unordered_map<Node *, char> deletedNodes;

    ~BDD()
    {
        if (root == nullptr || root->data == '0' || root->data == '1')
        {
            deleteNode(false_node);
            deleteNode(true_node);
            return;
        }

        deleteNode(root);
        root = nullptr;
    }

    void deleteNode(Node *node)
    {
        if (node == nullptr)
        {
            return;
        }

        bool found = (deletedNodes.find(node) != deletedNodes.end());

        if (found)
        {
            return;
        }

        deleteNode(node->r);
        deleteNode(node->l);

        // If node was already deleted, return
        found = (deletedNodes.find(node) != deletedNodes.end());
        if (found)
        {
            return;
        }

        deletedNodes[node] = node->data;
        delete node;
    }

    void printNode(Node *node, unsigned int level)
    {
        if (node == nullptr)
        {
            return;
        }
        // Last 5 chars of the address of node node is used as its ID
        string id = std::to_string((long long)node).substr(5);

        // If node was already printed, return
        bool found = (printedNodes.find(node) != printedNodes.end());

        if (!found)
        {
            if (node->data != '0' && node->data != '1')
            {
                printedNodes[node] = node->data;
            }

            printNode(node->r, level + 1);
        }

        if (level == 0U)
        {
            cout << node->data << " " << node->bfunction << " " << id << "\n";
        }
        else
        {

            for (unsigned int i = 0; i < level - 1; i++)
            {
                cout << "|\t";
            }

            if (found)
            {
                cout << "|-------" << node->data << "* " << id << "\n";
                return;
            }
            else if (node->data == '0' || node->data == '1')
            {
                cout << "|-------" << node->data << "\n";
            }
            else
            {
                cout << "|-------" << node->data << " " << node->bfunction << " " << id << "\n";
            }
        }
        printNode(node->l, level + 1);
    }

    void print()
    {
        printNode(root, 0);
        cout << endl;
    }

    void pushToSide(vector<string> *side, string part)
    {
        // if part was only 1 variable (e.g. a) then it is not needed to be added to the side and we know where to put the 1 beacause we use certain side
        if (part.length() == 0)
        {
            side->clear();
            side->push_back("1");
            return;
        }

        // Check if the part doesent already exist (duplicate resolution of parts - klauzul)
        bool exists = false;
        for (size_t j = 0; j < side->size(); j++)
        {
            if (side->at(j) == part)
            {
                exists = true;
                break;
            }
        }
        if (exists)
        {
            return;
        }

        side->push_back(part);
    }

    void doShannonExpansion(const string bfunkcia, const char var, string *left, string *right)
    {
        string part = "";
        vector<string> left_side;
        vector<string> right_side;
        vector<string> *current_side = nullptr; // Pointer to the side that is currently being filled

        for (size_t i = 0; i <= bfunkcia.length(); i++)
        {
            const char c = bfunkcia[i];
            if (c == tolower(var)) // bolo by aj tak lower case
            {
                // verify (aA) case and it can be deleted because it is a contradiction
                if (current_side == &left_side)
                {
                    // skip the part
                    part = "";
                    current_side = nullptr;
                    while (bfunkcia[i] != '+' && bfunkcia[i] != '\0')
                    {
                        i++;
                    }
                    continue;
                }
                current_side = &right_side;
                continue;
            }

            if (c == toupper(var))
            {
                // to iste ako vyssie
                if (current_side == &right_side)
                {
                    // skip the part
                    part = "";
                    current_side = nullptr;
                    while (bfunkcia[i] != '+' && bfunkcia[i] != '\0')
                    {
                        i++;
                    }
                    continue;
                }

                current_side = &left_side;
                continue;
            }

            if (c == '+' || c == '\0')
            {
                // If the variables was not found in the expression, push part to both sides
                if (current_side == nullptr)
                {
                    pushToSide(&left_side, part);
                    pushToSide(&right_side, part);
                    part = "";
                    current_side = nullptr;
                    continue;
                }
                // Otherwise push to the current side
                pushToSide(current_side, part);
                part = "";
                current_side = nullptr;
                continue;
            }

            part += c;
        }

        *left = "";
        *right = "";

        //-----------left side----------------

        // If the side is empty, set it to 0
        if (left_side.empty())
        {
            *left = "0";
        }
        // If the side is 1 or 0, set it to 1 or 0
        else if (left_side.at(0) == "1" || left_side.at(0) == "0") // left_side[0]
        {
            *left = left_side.at(0);
        }
        else
        {
            // Sort the side by length and concatenate it (lambda function for sorting)
            std::sort(left_side.begin(), left_side.end(), [](const string &a, const string &b)
                      { return a.length() < b.length(); });

            for (size_t i = 0; i < left_side.size(); i++)
            {
                (*left).append(left_side.at(i));
                if (i != left_side.size() - 1)
                {
                    (*left).append("+");
                }
            }
        }

        //-----------right side----------------

        // If the side is empty, set it to 0
        if (right_side.empty())
        {
            *right = "0";
        }
        // If the side is 1 or 0, set it to 1 or 0
        else if (right_side.at(0) == "1" || right_side.at(0) == "0")
        {
            *right = right_side.at(0);
        }
        else
        {
            // Sort the side by length and concatenate it
            std::sort(right_side.begin(), right_side.end(), [](const string &a, const string &b)
                      { return a.length() < b.length(); });
            for (size_t i = 0; i < right_side.size(); i++)
            {
                (*right).append(right_side.at(i));
                if (i != right_side.size() - 1)
                {
                    (*right).append("+");
                }
            }
        }
    }

    Node *recursive_create(string bfunkcia, const size_t orderIndex)
    {

        // terminate end nodes
        if (bfunkcia == "0")
        {
            return false_node;
        }
        if (bfunkcia == "1")
        {
            return true_node;
        }

        const char var = poradie[orderIndex];
        const string key = bfunkcia + "|" + var; // basically a hash

        // Check if the function is in the function_map
        if (function_map.find(key) != function_map.end())
        {
            return function_map[key];
        }

        // Expand
        string bfunkcia_0 = "";
        string bfunkcia_1 = "";
        doShannonExpansion(bfunkcia, var, &bfunkcia_0, &bfunkcia_1);
        // cout << "bfunkcia: " << bfunkcia << endl;
        // cout << "var: " << var << endl;
        // cout << "bfunkcia_0: " << bfunkcia_0 << endl;
        // cout << "bfunkcia_1: " << bfunkcia_1 << endl;
        // cout << "======================" << endl;
        // If the functions are the same, create only one root in it's place
        if (bfunkcia_0 == bfunkcia_1)
        {
            return recursive_create(bfunkcia_0, orderIndex + 1);
        }

        // Create the node
        Node *node = new Node();
        node->data = var;
        node->bfunction = bfunkcia;
        size++;
        function_map[key] = node;
        node->r = recursive_create(bfunkcia_1, orderIndex + 1);
        node->l = recursive_create(bfunkcia_0, orderIndex + 1);

        // If the nodes are the same, return one of them
        if (node->r == node->l)
        {
            function_map[key] = node->r;
            delete node;

            size--;
            return function_map[key];
        }

        return node;
    }
};

BDD *BDD_create(string bfunkcia, string poradie)
{
    BDD *bdd = new BDD();
    bdd->poradie = poradie;
    bdd->var_count = poradie.length();
    bdd->root = bdd->recursive_create(bfunkcia, 0);
    if (bdd->root->data == '1' || bdd->root->data == '0') // if it is tautology or contradiction
    {
        bdd->size = 1;
    }
    return bdd;
}

string getPoradie(string bfunkcia)
{
    string poradie = "";

    for (char ch : bfunkcia)
    {
        if (isalpha(ch))
        {
            ch = tolower(ch);
            for (size_t i = 0; i < poradie.length(); i++)
            {
                if (poradie[i] == ch)
                {
                    ch = '$';
                    break;
                }
            }
            if (ch != '$')
            {
                poradie += ch;
            }
        }
    }

    return poradie;
}

BDD *BDD_create_with_best_order(string bfunkcia)
{
    // finding the best order
    // using n^2 permutations

    srand(10);
    string poradie = getPoradie(bfunkcia);

    /*uncomment 1*/
    // cout << "getPoradie: " << poradie << endl;

    BDD *best_bdd = nullptr;

    for (size_t i = 0; i < poradie.length() * poradie.length(); i++)
    {
        string temp_poradie = poradie;

        std::random_shuffle(temp_poradie.begin(), temp_poradie.end());

        BDD *temp_bdd = BDD_create(bfunkcia, temp_poradie);
        size_t best_bdd_size = best_bdd == nullptr ? 999999999 : best_bdd->size;
        // cout << "bfunkcia: " << bfunkcia << endl;
        // cout << "poradie: " << temp_poradie << endl;
        // cout << "size: " << temp_bdd->size << endl;
        if (temp_bdd->size < best_bdd_size)
        {
            // cout << "--------------------------" << endl;
            // cout << "MAM LEPSIE PORADIE" << endl;
            // cout << "--------------------------" << endl;
            delete best_bdd;
            best_bdd = temp_bdd;
            continue;
        }
        delete temp_bdd;
    }

    /*uncomment*/
    // cout << "--------------------------" << endl;
    // cout << "BEST" << endl;
    // cout << "best poradie: " << best_bdd->poradie << endl;

    return best_bdd;
}

char BDD_use(BDD *bdd, string vstupy)
{
    if (vstupy.length() != (size_t)bdd->var_count)
    {
        // Incorrect input size
        return -1;
    }

    Node *current_node = bdd->root;

    for (size_t i = 0; i < (size_t)vstupy.length(); i++)
    {

        if (current_node->data == '1' || current_node->data == '0')
        {
            return current_node->data;
        }

        if (current_node->data != bdd->poradie[i])
        {
            continue;
        }

        if (vstupy[i] == '0')
        {
            current_node = current_node->l;
        }
        else if (vstupy[i] == '1')
        {
            current_node = current_node->r;
        }
        else
        {
            // Incorrect input value
            return -1;
        }
    }

    return current_node->data;
}