#include <fstream>
#include <iostream>
using namespace std;

void QuickSort(float* array, int left, int right, int middle)
{
    int i = left;
    int j = right;
    float pivot = array[(i + j) / 2];
    int temp;
    while (i <= j)
    {
        while (array[i] < pivot)
            i++;
        while (array[j] > pivot)
            j--;
        if (i <= j)
        {
            swap(array[i], array[j]);
            i++;
            j--;
        }
    }
    if (j >= middle && j > left)
        QuickSort(array, left, j, middle);
    else if (i < right)
        QuickSort(array, i, right, middle);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        return 1;
    }
    std::ifstream inFile(argv[1]);
    if (!inFile) {
        return 2;
    }
    int n;
    float min, middle, max;
    int minInd, middleInd, maxInd;
    inFile >> n;
    float* M = new float[n];
    float* M_copy = new float[n];
    inFile >> M[0];
    M_copy[0] = M[0];
    min = M[0];
    minInd = 1;
    max = M[0];
    maxInd = 1;
    for (int i = 1; i < n; i++)
    {
        inFile >> M[i];
        M_copy[i] = M[i];
        if (min > M[i])
        {
            min = M[i];
            minInd = i + 1;
        }
        if (max < M[i])
        {
            max = M[i];
            maxInd = i + 1;
        }
    }
    QuickSort(M_copy, 0, n - 1, n / 2);
    middle = M_copy[n / 2];
    for (int i = 0; i < n; i++)
    {
        if (middle == M[i])
        {
            middleInd = i + 1;
            break;
        }
    }
    cout << minInd << " " << middleInd << " " << maxInd;
    delete[] M;
    delete[] M_copy;
    inFile.close();
    return 0;
}