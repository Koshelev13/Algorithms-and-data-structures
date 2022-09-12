#include <fstream>
#include <iostream>
using namespace std;

void Merge(long long* M, int first, int middle, int last, int size)
{
    long long* tmp = new long long[last - first + 1];
    int start1 = first, end1 = middle, start2 = middle + 1, end2 = last;
    int ind = 0;
    while (start1 <= end1 && start2 <= end2)
    {
        if (M[start1] <= M[start2])
        {
            tmp[ind++] = M[start1++];
        }
        else
        {
            M[size] += end1 - start1;
            M[size]++;
            tmp[ind++] = M[start2++];
        }
    }
    while (start1 <= end1) tmp[ind++] = M[start1++];
    while (start2 <= end2) tmp[ind++] = M[start2++];
    ind = 0;
    while (first <= last)
        M[first++] = tmp[ind++];
    delete[]tmp;
}

void Split(long long* M, int first, int last, int size)
{
    if (first < last)
    {
        int middle = first - 1 + (last - first + 1) / 2;
        Split(M, middle + 1, last, size);
        Split(M, first, middle, size);
        Merge(M, first, middle, last, size);
    }
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
    int n;
    inFile >> n;
    long long* M = new long long[n + 1];
    for (int i = 0; i < n; i++)
        inFile >> M[i];
    M[n] = 0;
    Split(M, 0, n - 1, n);
    outFile << M[n];
    delete [] M;
    inFile.close();
    outFile.close();
    return 0;
}