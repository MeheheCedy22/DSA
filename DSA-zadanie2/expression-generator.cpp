#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>
#include <random>
#include <ctime>

using std::cout;
using std::endl;
using std::string;
using std::vector;

string generate_bfunction(int n)
{
    srand(time(NULL) * n * rand());

    // Generate an alphabet string of size n
    string alphabetLower = "abcdefghijklmnopqrstuvwxyz";
    string alphabetUpper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    alphabetLower.resize(n);
    alphabetUpper.resize(n);
    string alphabet = alphabetLower + alphabetUpper;
    string bfunction = "";

    // duplicate the alphabet string n times
    for (int i = 0; i < n; i++)
    {
        bfunction += alphabet;
    }

    // shuffle the string
    std::random_shuffle(bfunction.begin(), bfunction.end());

    // cout << bfunction << endl;

    vector<string> parts;

    // split the string into n parts
    size_t start = 0;
    size_t end = start;
    while (end < bfunction.length())
    {
        int part_size = rand() % (n - 1) + n / 2;
        end = start + part_size;
        string part = bfunction.substr(start, part_size);

        // remove duplicate variables from the part
        for (size_t j = 0; j < part.size(); j++)
        {
            for (size_t k = j + 1; k < part.size(); k++)
            {
                if (toupper(part[j]) == toupper(part[k]))
                {
                    part.erase(k, 1);
                    k--;
                }
            }
        }

        parts.push_back(part);
        start = end;
    }

    // Concatenate the parts
    string result = "";
    for (size_t i = 0; i < parts.size(); i++)
    {
        result += parts[i];
        result += "+";
    }
    result.pop_back();

    return result;
}