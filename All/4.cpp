#include <fstream>
#include <iostream>
using namespace std;

void QuickSort(int* x, int left, int right, int k1, int k2)
{
    int i = left;
    int j = right;
    int pivot = x[(i + j) / 2];
    while (i <= j)
    {
        while (x[i] < pivot) ++i;
        while (x[j] > pivot) --j;
        if (i <= j)
        {
            swap(x[i], x[j]);
            i++;
            j--;
        }
    }
    if (j >= k2 && j > left)
        QuickSort(x, left, j, k1, k2);
    else if (j < k1 && i < right)
        QuickSort(x, i, right, k1, k2);
    else if (j >= k1 && j < k2)
    {
        if (j > left)
            QuickSort(x, left, j, k1, j);
        if (i < right)
            QuickSort(x, i, right, i, k2);
    }
}


#include <fstream>

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
    int n, k1, k2, a, b, c, x_1, x_2;
    inFile >> n >> k1 >> k2;
    inFile >> a >> b >> c >> x_1 >> x_2;
    int* x = new int [n];
    x[0] = x_1;
    x[1] = x_2;
    for (int i = 2; i < n; i++)
    {
        x[i] = a * x[i - 2] + b * x[i - 1] + c;
    }
    QuickSort(x, 0, n - 1, k1-1, k2-1);
    for (int i = k1 - 1; i < k2-1; i++)
    {
        outFile << x[i] << " ";
    }
    outFile << x[k2-1];
    delete[] x;
    inFile.close();
    outFile.close();
    return 0;
}