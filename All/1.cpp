#include <fstream>
#include<iostream>
using namespace std;
int** increaseArr(int** current, int size)
{

    int** temp = new int* [size * 2];
    for (int i = 0; i < size; ++i)
    {
        temp[i] = new int[2];
        temp[i][0] = current[i][0];
        temp[i][1] = current[i][1];
    }
    for (int i = size; i < size * 2; i++) temp[i] = new int[2];
    return temp;
}
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
    int** stack_1 = new int* [1];
    stack_1[0] = new int[2];
    int size_1 = 1;
    int end_1 = -1;
    int** stack_2 = new int* [1];
    stack_2[0] = new int[2];
    int size_2 = 1;
    int end_2 = -1;
    int k = 0;
    inFile >> k;
    char symbol;
    int number;
    for (int i = 1; i <= k; ++i)
    {
        inFile >> symbol;
        if (symbol == '+')
        {
            inFile >> number;
            if (end_1 >= size_1 - 1)
            {
                while (end_1 >= size_1 - 1) {
                    stack_1 = increaseArr(stack_1, size_1);
                    size_1 *= 2;
                }
            }
            end_1++;
            stack_1[end_1][0] = number;
            if (i == 1 || end_1 == 0) stack_1[end_1][1] = number;
            else {
                if (stack_1[end_1 - 1][1] > number) stack_1[end_1][1] = number;
                else stack_1[end_1][1] = stack_1[end_1 - 1][1];
            }
        }
        if (symbol == '-')
        {
            if (end_2 == -1)
            {
                while (size_2 < size_1) {
                    stack_2 = increaseArr(stack_2, size_2);
                    size_2 *= 2;
                }
                for (int j = end_1; j >= 0; --j) {
                    stack_2[end_1 - j][0] = stack_1[j][0];
                    if (j == end_1) stack_2[end_1 - j][1] = stack_1[j][0];
                    else {
                        if (stack_2[end_1 - j - 1][1] < stack_1[j][0]) stack_2[end_1 - j][1] = stack_2[end_1 - j - 1][1];
                        else stack_2[end_1 - j][1] = stack_1[j][0];
                    }
                    ++end_2;
                }
                end_1 = -1;
            }
            --end_2;
        }
        if (symbol == '?') {
            if (end_1 == -1) outFile << stack_2[end_2][1] << '\n';
            else if (end_2 == -1) outFile << stack_1[end_1][1] << '\n';
            else {
                if (stack_1[end_1][1] < stack_2[end_2][1]) { outFile << stack_1[end_1][1] << '\n'; }
                else {
                    outFile << stack_2[end_2][1] << '\n';
                }
            }
        }
    }

    inFile.close();
    outFile.close();
    return 0;
}