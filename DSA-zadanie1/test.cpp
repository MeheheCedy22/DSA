#include "avl.h"
#include "chaining_hash.h"
#include "openAddressing_hash.h"
#include "RNG_for_DSA_plus_strings.h"
#include <iomanip>

class Test
{
public:
    int data;
    int in_key;
    char in_data[6];
    char I[6];

    Node *node;
    NodeHashChain *nodeHashChain;
    NodeHashOpen *nodeHashOpen;

    AVL *tree;
    HashTableChain *hashTableCh;
    HashTableOpen *hashTableO;

    Test(int Tablesize)
    {
        this->data = 0;
        this->in_key = 0;

        this->node = nullptr;
        this->nodeHashChain = nullptr;
        this->nodeHashOpen = nullptr;

        this->tree = new AVL;
        this->hashTableCh = new HashTableChain(Tablesize);
        this->hashTableO = new HashTableOpen(Tablesize);
    }

    ~Test()
    {
        delete node;
        delete nodeHashChain;
        delete nodeHashOpen;

        delete tree;
        delete hashTableCh;
        delete hashTableO;
    }

    long long insertAVL(FILE *f, int nodeCount)
    {
        auto start = high_resolution_clock::now();
        for (int x = 0; x < nodeCount; x++)
        {
            fscanf(f, "%d%s", &in_key, &in_data);
            in_data[5] = '\0';
            string insertData = string(in_data);
            tree->root = tree->insert(tree->root, in_key, insertData);
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(stop - start);
        return duration.count();
    }

    long long insertHashChain(FILE *f, int nodeCount)
    {
        auto start = high_resolution_clock::now();
        for (int x = 0; x < nodeCount; x++)
        {
            fscanf(f, "%d%s", &data, &I);
            I[5] = '\0';
            string insertKey = string(I);
            hashTableCh->insert(insertKey, data);
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(stop - start);
        return duration.count();
    }

    long long insertHashOpen(FILE *f, int nodeCount)
    {
        auto start = high_resolution_clock::now();
        for (int x = 0; x < nodeCount; x++)
        {
            fscanf(f, "%d%s", &data, &I);
            I[5] = '\0';
            string insertKey = string(I);
            hashTableO->insert(insertKey, data);
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(stop - start);
        return duration.count();
    }

    long long searchAVL(FILE *f, int nodeCount)
    {
        auto start = high_resolution_clock::now();
        for (int x = 0; x < nodeCount; x++)
        {
            fscanf(f, "%d%s", &in_key, &in_data);
            tree->search(tree->root, in_key);
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(stop - start);
        return duration.count();
    }

    long long searchHashChain(FILE *f, int nodeCount)
    {
        auto start = high_resolution_clock::now();
        for (int x = 0; x < nodeCount; x++)
        {
            fscanf(f, "%d%s", &data, &I);
            I[5] = '\0';
            string insertKey = string(I);
            hashTableCh->search(insertKey);
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(stop - start);
        return duration.count();
    }

    long long searchHashOpen(FILE *f, int nodeCount)
    {
        auto start = high_resolution_clock::now();
        for (int x = 0; x < nodeCount; x++)
        {
            fscanf(f, "%d%s", &data, &I);
            I[5] = '\0';
            string insertKey = string(I);
            hashTableO->search(insertKey);
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(stop - start);
        return duration.count();
    }

    long long deleteAVL(FILE *f, int nodeCount)
    {
        auto start = high_resolution_clock::now();
        for (int x = 0; x < nodeCount; x++)
        {
            fscanf(f, "%d%s", &in_key, &in_data);
            tree->root = tree->deleteNode(tree->root, in_key);
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(stop - start);
        return duration.count();
    }

    long long deleteHashChain(FILE *f, int nodeCount)
    {
        auto start = high_resolution_clock::now();
        for (int x = 0; x < nodeCount; x++)
        {
            fscanf(f, "%d%s", &data, &I);
            I[5] = '\0';
            string insertKey = string(I);
            hashTableCh->deleteKey(insertKey);
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(stop - start);
        return duration.count();
    }

    long long deleteHashOpen(FILE *f, int nodeCount)
    {
        auto start = high_resolution_clock::now();
        for (int x = 0; x < nodeCount; x++)
        {
            fscanf(f, "%d%s", &data, &I);
            I[5] = '\0';
            string insertKey = string(I);
            hashTableO->deleteKey(insertKey);
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(stop - start);
        return duration.count();
    }
};

int main()
{
    generate();
    int N = 10;
    Test test(N);

    FILE *f;
    if ((f = fopen("dataset_1M.txt", "r")) == NULL)
    {
        cout << "Error opening file dataset_1M.txt" << endl;
        return 1;
    }

    cout << "1 AVL" << endl;
    cout << "2 Chaining" << endl;
    cout << "3 Open Addressing" << endl;
    cout << endl;

    cout << "N\tInsert\tSearch\tDelete\tSpace" << endl;

    for (int i = 0; N < 1000000; i++)
    {
        cout << N << "\t";
        long long timeAVL = test.insertAVL(f, N);
        cout << timeAVL / N << "\t";
        rewind(f);
        timeAVL = test.searchAVL(f, N);
        cout << timeAVL / N << "\t";
        rewind(f);
        timeAVL = test.deleteAVL(f, N);
        cout << timeAVL / N << "\t";
        cout << (sizeof(Node) + 5) * N + sizeof(AVL) << "\t";
        cout << endl;
        N = N * 1.7f + 7000;
    }

    N = 10;
    cout << endl
         << endl;

    for (int i = 0; N < 1000000; i++)
    {
        cout << N << "\t";
        rewind(f);
        long long timeHashChain = test.insertHashChain(f, N);
        cout << timeHashChain / N << "\t";
        int bucketCount = test.hashTableCh->size;
        rewind(f);
        timeHashChain = test.searchHashChain(f, N);
        cout << timeHashChain / N << "\t";
        rewind(f);
        timeHashChain = test.deleteHashChain(f, N);
        cout << timeHashChain / N << "\t";
        cout << bucketCount * sizeof(NodeHashChain *) + N * (sizeof(NodeHashChain) + 5) << "\t";
        cout << endl;
        N = N * 1.7f + 7000;
    }

    N = 10;
    cout << endl
         << endl;

    for (int i = 0; N < 1000000; i++)
    {
        cout << N << "\t";
        rewind(f);
        long long timeHashOpen = test.insertHashOpen(f, N);
        cout << timeHashOpen / N << "\t";
        int space = test.hashTableO->size;
        rewind(f);
        timeHashOpen = test.searchHashOpen(f, N);
        cout << timeHashOpen / N << "\t";
        rewind(f);
        timeHashOpen = test.deleteHashOpen(f, N);
        cout << timeHashOpen / N << "\t";
        cout << space * sizeof(NodeHashOpen *) + N * (sizeof(NodeHashOpen) + 5) << "\t";
        cout << endl;
        N = N * 1.7f + 7000;
    }

    fclose(f);

    return 0;
}