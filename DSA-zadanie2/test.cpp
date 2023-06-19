// input will be in DNF form (i.e. a+bc+D == A+BC+!D)
// lowercase letters represent variable without negation
// uppercase letters represent variable with negation

#include "bdd.h"
#include "expression-generator.cpp"
#include <bitset>
#include <cmath>
#include <fstream>
#include <chrono>

using namespace std::chrono;

using std::ifstream;

char evaluate(string bfunkcia, string input, string poradie)
{
    string binary_bfunction;
    int index;

    if (input.length() != poradie.length())
    {
        cout << "Wrong input" << endl;
        return -1;
    }
    // make binary bfunction based on input i.e. bfunction is a+b+c, input is 111 and poradie is abc then binary_bfunction will be 1+1+1

    bool isClauseTrue = false;
    for (size_t i = 0; i <= bfunkcia.length(); i++)
    {
        char c = bfunkcia[i];

        if (c == '+' || i == bfunkcia.length())
        {
            if (isClauseTrue)
                return '1';
            continue;
        }

        index = poradie.find(tolower(c));

        if (index == -1)
        {
            cout << "Wrong input" << endl;
            return -1;
        }

        char inputChar = input[index];

        if ((islower(c) && inputChar == '0') || (isupper(c) && inputChar == '1'))
        {
            isClauseTrue = false;
            while (bfunkcia[i] != '+' && ++i != bfunkcia.length())
                ;

            continue;
        }
        isClauseTrue = true;
    }

    return '0';
}

bool testBDD(BDD *bdd, string bfunkcia, string poradie)
{

    // int maxInputov = 1 << poradie.length();
    // 2^n inputs because of n variables and it is doing truth table
    int maxInputov = pow(2, poradie.length());

    for (int i = 0; i < maxInputov; i++)
    {
        string input = std::bitset<64>(i).to_string();
        input = input.substr(input.size() - poradie.length());
        char eval = evaluate(bfunkcia, input, poradie);
        char bddEval = BDD_use(bdd, input);
        if (eval != bddEval)
        {
            cout << " BDD WRONG EVALUATION" << endl;
            cout << "Failed on input: " << input << "| GOT: " << bddEval << " | EXPECTED: " << eval << endl;
            return false;
        }
    }
    return true;
}

int main()
{

    // string func = "Ch+hj+ach+Jha";
    // string order = "cajh";
    // BDD *bdd = BDD_create(func, order);
    // bdd->print();

    // delete bdd;

    // return 0;

    FILE *fw;

    if ((fw = fopen("expressions-DNF.txt", "w")) == NULL)
    {
        cout << "Error opening file expressions-DNF.txt" << endl;
        return -1;
    }

    for (int i = 11; i <= 15; i++) // default from 11 to 15; how many variables in bfunction
    {
        for (int j = 0; j < 100; j++)
        {
            string bfunction = generate_bfunction(i);
            fprintf(fw, "%s\n", bfunction.c_str());
        }
    }

    fclose(fw);

    string bfunkcia = "";
    string poradie = "";

    ifstream f("expressions-DNF.txt");
    if (!f)
    {
        cout << "File expressions-DNF.txt not opened" << endl;
        return 1;
    }

    auto sum_duration = 0;
    auto best_sum_duration = 0;
    int sum_space = 0;
    int best_sum_space = 0;
    double sum_ratio = 0;
    double best_sum_ratio = 0;
    int runs = 0;

    bool breaking = false;
    // breaking=true;

    while (getline(f, bfunkcia))
    {

        // break; //for user demonstration

        if (runs == 99)
        {
            runs = 0;
        }
        cout << "runs: " << runs + 1 << endl;

        if (bfunkcia[0] == '/')
        {
            continue;
        }
        poradie = "";

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
        // Remove everything other than letters and '+'
        bfunkcia.erase(remove_if(bfunkcia.begin(), bfunkcia.end(), [](char c)
                                 { return !isalpha(c) && c != '+'; }),
                       bfunkcia.end());

        // first poradie (order) will be how the function have the variables in order
        // i.e. ab+d+c will be abdc

        /*uncomment*/
        // cout << "------" << endl;
        // cout << "------" << endl;
        // cout << "Poradie: " << poradie << endl;
        // cout << "Bfunckia: " << bfunkcia << endl;
        // cout << "------" << endl;

        auto start = high_resolution_clock::now();

        BDD *bdd = BDD_create(bfunkcia, poradie);

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
        sum_duration += duration.count();

        cout << "Space taken by function: " << bdd->size * sizeof(Node) + sizeof(BDD) << endl;
        sum_space += bdd->size * sizeof(Node) + sizeof(BDD);

        /*uncomment 1*/
        cout << "BDD created" << endl;

        // cout << "Displaying" << endl;
        // bdd->print();
        if (testBDD(bdd, bfunkcia, poradie))
        {
            cout << "BDD is correct" << endl;
        }
        else
        {
            cout << "BDD is not correct" << endl;
            return 1;
        }

        cout << "size: " << bdd->size << endl;
        cout << "max possible size: " << pow(2, poradie.length() + 1) - 1 << endl;
        double reduction = (bdd->size / (pow(2, poradie.length() + 1) - 1)) * 100;
        cout << "reduced to : \t" << reduction << " %" << endl;

        sum_ratio += reduction;

        cout << endl;
        cout << "------" << endl;

        cout << "Doing best order: " << endl;

        auto start_best = high_resolution_clock::now();

        BDD *bdd_best = BDD_create_with_best_order(bfunkcia);

        auto stop_best = high_resolution_clock::now();
        auto duration_best = duration_cast<microseconds>(stop_best - start_best);
        cout << "Time taken by function (best): " << duration_best.count() << " microseconds" << endl;
        best_sum_duration += duration_best.count();

        cout << "Space taken by function (best): " << bdd_best->size * sizeof(Node) + sizeof(BDD) << endl;
        best_sum_space += bdd_best->size * sizeof(Node) + sizeof(BDD);

        // bdd_best->print();
        if (testBDD(bdd, bfunkcia, poradie))
        {
            cout << "BDD is correct" << endl;
        }
        else
        {
            cout << "BDD is not correct" << endl;
            return 1;
        }
        cout << "size of best order: " << bdd_best->size << endl;
        cout << "max possible size: " << pow(2, poradie.length() + 1) - 1 << endl;
        double reduction_best = (bdd_best->size / (pow(2, poradie.length() + 1) - 1)) * 100;
        cout << "reduced to : \t" << reduction_best << " %" << endl;

        best_sum_ratio += reduction_best;

        cout << endl;
        double reduction_best_to_reduction = (reduction_best / reduction) * 100;
        cout << "reduced comparison: \t" << reduction_best_to_reduction << " %" << endl;
        cout << "------" << endl;
        cout << "------" << endl;

        delete bdd_best;
        delete bdd;

        runs++;
    }
    cout << "---------------------------------------------------------------" << endl;

    if (!breaking)
    {
        cout << "Average time for creating BDD: " << sum_duration / runs << " microseconds" << endl;
        cout << "Average time for creating BDD with best order: " << best_sum_duration / runs << " microseconds" << endl;
        cout << "Average space for creating BDD: " << sum_space / runs << endl;
        cout << "Average space for creating BDD with best order: " << best_sum_space / runs << endl;
        cout << "Average reduction: " << (sum_ratio / runs) / 100 << " %" << endl;
        cout << "Average reduction with best order: " << (best_sum_ratio / runs) / 100 << " %" << endl;
    }

    f.close();
    cout << "File closed" << endl;

    cout << "---------------------------------------------------------------" << endl;

    cout << "User input: " << endl
         << endl;

    while (true)
    {
        // break; //for canceling user demonstration and faster end, for debugging

        string user_bfunkcia;
        string user_poradie;

        cout << "Enter bfunkcia (bfunction): ";
        cin >> user_bfunkcia;
        getchar();
        cout << "Enter poradie (order): ";
        cin >> user_poradie;
        getchar();

        cout << endl;

        auto user_start = high_resolution_clock::now();

        BDD *user_bdd = BDD_create(user_bfunkcia, user_poradie);

        auto user_stop = high_resolution_clock::now();
        auto user_duration = duration_cast<microseconds>(user_stop - user_start);
        cout << "Time taken by function: " << user_duration.count() << " microseconds" << endl;

        cout << "BDD created" << endl;
        cout << "Displaying" << endl;
        user_bdd->print();
        if (testBDD(user_bdd, user_bfunkcia, user_poradie))
        {
            cout << "BDD is correct" << endl;
        }
        else
        {
            cout << "BDD is not correct" << endl;
        }
        cout << "size: " << user_bdd->size << endl;
        cout << "max possible size: " << pow(2, user_poradie.length() + 1) - 1 << endl;
        double user_reduction = (user_bdd->size / (pow(2, user_poradie.length() + 1) - 1)) * 100;
        cout << "reduced to : \t" << user_reduction << " %" << endl;

        cout << endl;
        cout << "------" << endl;

        cout << "User best order: " << endl;

        auto user_start_best = high_resolution_clock::now();

        BDD *user_bdd_best = BDD_create_with_best_order(user_bfunkcia);

        auto user_stop_best = high_resolution_clock::now();
        auto user_duration_best = duration_cast<microseconds>(user_stop_best - user_start_best);
        cout << "Time taken by function (best): " << user_duration_best.count() << " microseconds" << endl;

        user_bdd_best->print();
        if (testBDD(user_bdd, user_bfunkcia, user_poradie))
        {
            cout << "BDD is correct" << endl;
        }
        else
        {
            cout << "BDD is not correct" << endl;
        }
        cout << "size of best order: " << user_bdd_best->size << endl;
        cout << "max possible size: " << pow(2, user_poradie.length() + 1) - 1 << endl;
        double user_reduction_best = (user_bdd_best->size / (pow(2, user_poradie.length() + 1) - 1)) * 100;
        cout << "reduced to : \t" << user_reduction_best << " %" << endl;
        cout << endl;
        double user_reduction_best_to_reduction = (user_reduction_best / user_reduction) * 100;
        cout << "reduced comparison: \t" << user_reduction_best_to_reduction << " %" << endl;

        // User BDD_use
        string user_input;
        char user_eval;
        char user_out = 'y';
        cout << "------" << endl;
        cout << "\t\t" << user_bdd->poradie << "\tporadie classic" << endl;
        cout << "What is on\t";
        cin >> user_input;
        getchar();
        cout << "------" << endl;

        user_out = BDD_use(user_bdd, user_input);
        if (user_out == -1)
        {
            cout << "Output: " << -1 << endl;
        }
        else if (user_out == 'y')
        {
            cout << "Output: "
                 << "error" << endl;
        }
        else
        {
            cout << "Output: " << user_out << endl;
        }
        cout << "------" << endl;

        user_eval = evaluate(user_bfunkcia, user_input, user_poradie);

        cout << "Eval: " << user_eval << endl;
        if (user_eval == user_out)
        {
            cout << "Correct" << endl;
        }

        else
        {
            cout << "Wrong" << endl;
        }

        delete user_bdd;
        delete user_bdd_best;

        char user_continue;
        cout << "Do you want to continue? (y/n)" << endl;
        cin >> user_continue;
        getchar();
        if (user_continue == 'n')
        {
            break;
        }
    }

    return 0;
}