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
using std::vector;

struct NodeHashChain
{
public:
    int data;
    string key;
    NodeHashChain *next;

    NodeHashChain(int data, string key) /*constructor*/
    {
        this->data = data;
        this->key = key;
        this->next = nullptr;
    }
};

// class Hash table using vector of linked lists
class HashTableChain
{
public:
    int size;
    int count;
    bool resizing;
    int found;
    int notFound;
    vector<NodeHashChain *> table;

    HashTableChain(int size) /* constructor */
    {
        this->size = size;
        this->count = 0;
        this->found = 0;
        this->notFound = 0;
        table = vector<NodeHashChain *>(size);
        this->resizing = false;
    }

    ~HashTableChain() /* destructor */
    {
        clear();
    }

    // clear the hash table by iterating through the vector and deleting the linked lists
    void clear()
    {
        for (int i = 0; i < size; i++)
        {
            NodeHashChain *temp = table[i];
            while (temp != nullptr)
            {
                NodeHashChain *temp2 = temp;
                temp = temp->next;
                delete temp2;
            }
        }
        count = 0;
    }

    // function to get the hash value of the key
    // djb2 hash function for string keys
    int hash(string key)
    {
        int hashValue = 5381;
        for (size_t i = 0; i < key.length(); i++)
        {
            hashValue = ((hashValue << 5) + hashValue) + key[i];
        }
        return hashValue % size;
    }

    // function to get load factor of the hash table
    float loadFactor()
    {
        return (float)count / (float)size;
    }

    // function to insert the key and data into the hash table using chaining method
    void insert(string key, int data)
    {

        int hashValue = hash(key);

        // if the hash value is empty, insert the key and data
        if (table[hashValue] == nullptr)
        {
            table[hashValue] = new NodeHashChain(data, key);
        }
        else
        {
            // if the hash value is not empty, insert the key and data at the end of the linked list
            NodeHashChain *temp = table[hashValue];
            while (temp->next != nullptr)
            {
                temp = temp->next;
            }
            temp->next = new NodeHashChain(data, key);
        }

        if (resizing)
        {
            return;
        }
        count++;
        if (loadFactor() > 0.7)
        {
            resize(size * 2);
        }
    }

    // function to resize the hash table
    void resize(int capacity)
    {
        resizing = true;
        // backup data and keys
        vector<NodeHashChain *> temp = table;
        int tempSize = size;
        // resize the hash table
        table = vector<NodeHashChain *>(capacity);
        size = capacity;

        // rehash the data
        for (int i = 0; i < tempSize; i++)
        {
            NodeHashChain *temp2 = temp[i];
            while (temp2 != nullptr)
            {
                insert(temp2->key, temp2->data);
                temp2 = temp2->next;
            }
        }

        // delete the old hash table
        for (int i = 0; i < tempSize; i++)
        {
            NodeHashChain *temp2 = temp[i];
            while (temp2 != nullptr)
            {
                NodeHashChain *temp3 = temp2;
                temp2 = temp2->next;
                delete temp3;
            }
        }

        resizing = false;
    }

    // function to search the key in the hash table
    NodeHashChain *search(string key)
    {
        int hashValue = hash(key);
        NodeHashChain *temp = table[hashValue];
        while (temp != nullptr)
        {
            if (temp->key == key)
            {
                found++;
                return temp;
            }
            temp = temp->next;
        }
        notFound++;
        return nullptr;
    }

    // function to delete the key from the hash table using chaining method
    void deleteKey(string key)
    {
        int hashValue = hash(key);
        NodeHashChain *temp = table[hashValue];
        NodeHashChain *prev = nullptr;
        while (temp != nullptr)
        {
            if (temp->key == key)
            {
                if (prev == nullptr)
                {
                    table[hashValue] = temp->next;
                }
                else
                {
                    prev->next = temp->next;
                }
                count--;
                delete temp;
                if (loadFactor() < 0.25)
                {
                    resize(size / 2);
                }
                return;
            }
            prev = temp;
            temp = temp->next;
        }
    }

    // print the hash table for debugging
    void print()
    {
        for (int i = 0; i < size; i++)
        {
            cout << i << ": ";
            NodeHashChain *temp = table[i];
            while (temp != nullptr)
            {
                cout << temp->key << " " << temp->data << "->";
                temp = temp->next;
            }
            cout << endl;
        }
    }

    // function to reset the found and notFound counters when debugging
    void resetFound()
    {
        found = 0;
        notFound = 0;
    }
};