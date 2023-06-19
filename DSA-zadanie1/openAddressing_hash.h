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

struct NodeHashOpen
{
public:
    int data;
    string key;

    NodeHashOpen(int data, string key) /*constructor*/
    {
        this->data = data;
        this->key = key;
    }
};

// class HashTableOpen which is hash table with open addressing colision resolution method with linear probing
class HashTableOpen
{
public:
    int size;
    int count;
    bool resizing;
    int found;
    int notFound;
    vector<NodeHashOpen *> table;

    HashTableOpen(int size) /* constructor */
    {
        this->size = size;
        this->count = 0;
        this->found = 0;
        this->notFound = 0;
        table = vector<NodeHashOpen *>(size);
        this->resizing = false;
    }

    ~HashTableOpen() /* destructor */
    {
        clear();
    }

    // clear the hash table by iterating through the whole vector and deleting the nodes
    void clear()
    {
        for (int i = 0; i < size; i++)
        {
            if (table[i] != nullptr)
            {
                delete table[i];
            }
        }
    }

    // Jenskin's hash function for linear probing
    int hash(string key)
    {
        int hash = 0;
        for (size_t i = 0; i < key.length(); i++)
        {
            hash += key[i];
            hash += (hash << 10);
            hash ^= (hash >> 6);
        }
        hash += (hash << 3);
        hash ^= (hash >> 11);
        hash += (hash << 15);
        return abs(hash % size);
    }

    // function to get load factor of the hash table
    float loadFactor()
    {
        return (float)count / (float)size;
    }

    // insert a new node into the hash table
    void insert(string key, int data)
    {
        int index = hash(key);
        if (table[index] == nullptr)
        {
            table[index] = new NodeHashOpen(data, key);
        }
        else if (table[index]->key == "deleted")
        {
            table[index]->key = key;
            table[index]->data = data;
        }
        else
        {
            int i = 1;
            while (table[index] != nullptr)
            {
                index = (index + i) % size;
                i++;
            }
            table[index] = new NodeHashOpen(data, key);
        }

        if (resizing)
        {
            return;
        }
        count++;
        if (loadFactor() > 0.75)
        {
            resize(size * 2);
        }
    }

    // resize the hash table
    void resize(int newSize)
    {
        resizing = true;
        vector<NodeHashOpen *> oldTable = table;
        int oldSize = size;
        size = newSize;
        table = vector<NodeHashOpen *>(size);

        for (int i = 0; i < oldSize; i++)
        {
            if (oldTable[i] != nullptr && oldTable[i]->key != "deleted")
            {
                insert(oldTable[i]->key, oldTable[i]->data);
            }
        }

        // delete old table
        for (int i = 0; i < oldSize; i++)
        {
            if (oldTable[i] != nullptr)
            {
                delete oldTable[i];
            }
        }

        resizing = false;
    }

    // search for a node in the hash table
    NodeHashOpen *search(string key)
    {
        int index = hash(key);
        int i = 1;
        while (table[index] != nullptr)
        {
            if (table[index]->key == key)
            {
                found++;
                return table[index];
            }
            index = (index + i) % size;
            i++;
        }
        notFound++;
        return nullptr;
    }

    // delete a node from the hash table
    void deleteKey(string key)
    {
        int index = hash(key);
        int i = 1;

        if (search(key) != nullptr)
        {
            while (table[index]->key != key)
            {
                index = (index + i) % size;
                i++;
            }
            table[index]->key = "deleted";
            table[index]->data = 0;
            count--;
            if (loadFactor() < 0.25)
            {
                resize(size / 2);
            }
        }
        else
        {
            cout << "key not found:" << key << endl;
        }
    }

    // print the hash table for debugging
    void print()
    {
        for (int i = 0; i < size; i++)
        {
            if (table[i] != nullptr)
            {
                cout << i << ": " << table[i]->key << " " << table[i]->data << endl;
            }
            else
            {
                cout << i << ": " << endl;
            }
        }
    }

    // function to reset the found and notFound counters when debugging
    void resetFound()
    {
        found = 0;
        notFound = 0;
    }
};