#include <fstream>
#include <iostream>
using namespace std;

int* RadixSort(char** a, int n, int m, int k)
{
    int* b = new int[n];
    for (int i = 0; i < n; i++)
    {
        b[i] = i;
    }
    int* b1 = new int[n];
    for (int i = 0; i < n; i++)
    {
        b1[i] = i;
    }
    int c[52];
    for (int i = m - 1; i >= m - k; i--)
    {
        for (int j = 0; j < 52; j++)
        {
            c[j] = 0;
        }
        for (int j = 0; j < n; j++)
        {
            int d;
            if (int(a[i][b[j]]) < 91)
                d = int(a[i][b[j]]) - 65;
            else
                d = int(a[i][b[j]]) - 71;
            c[d]++;
        }
        int count = 0;
        for (int j = 0; j < 52; j++)
        {
            int temp = c[j];
            c[j] = count;
            count += temp;
        }
        for (int j = 0; j < n; j++)
        {
            int d;
            if (int(a[i][b[j]]) < 91)
                d = int(a[i][b[j]]) - 65;
            else
                d = int(a[i][b[j]]) - 71;
            b1[c[d]] = b[j];
            c[d]++;
        }
        for (int j = 0; j < n; j++)
        {
            b[j] = b1[j];
        }
    }
    delete[] b1;
    return b;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        return -1;
    }

    std::ifstream inFile(argv[1]);
    if (!inFile) {
        return -2;
    }

    std::ofstream outFile(argv[2]);
    if (!outFile) {
        return -3;
    }
    int n, m, k;
    inFile >> n >> m >> k;
    char **a = new char* [m];
    for (int i = 0; i < m; i++)
    {
        a[i] = new char[n];
    }
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            inFile >> a[i][j];
        }
    }
    int* b;
    b = RadixSort(a, n, m, k);
    for (int i = 0; i < n-1; i++)
    {
        outFile << b[i] + 1 << " ";
    }
    outFile << b[n - 1] + 1;
    inFile.close();
    outFile.close();
    for (int i = 0; i < m; i++)
    {
        delete[] a[i];
    }
    delete[] a;
    delete[] b;
    return 0;
}