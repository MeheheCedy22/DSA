#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

using std::cout;
using std::endl;
using std::string;
using std::vector;

void generate()
{
    int N = 1000 * 1000;

    FILE *fp;
    if ((fp = fopen("dataset_1M.txt", "w")) == NULL)
    {
        cout << "Error opening file" << endl;
        return;
    }

    int *input_array = new int[N];
    vector<string> string_array;

    for (int i = 0; i < N; i++)
    {
        input_array[i] = i + 1;

        // Generate string data for each integer
        string str = "";
        int val = i;
        for (int j = 0; j < 5; j++)
        {
            str += 'a' + (val % 26);
            val /= 26;
        }
        string_array.push_back(str);
    }

    // Shuffle the integer array
    std::random_shuffle(&input_array[0], &input_array[N]);

    // Write shuffled integer and string data to the file
    for (int i = 0; i < N; i++)
    {
        fprintf(fp, "%d %s\n", input_array[i], string_array[input_array[i] - 1].c_str());
    }

    fclose(fp);
    delete[] input_array;
}