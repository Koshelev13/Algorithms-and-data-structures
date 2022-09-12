#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
using namespace std;

class PriorityQueue
{
private:
    int array[100000];
    int last;
public:
    
    bool isEmpty()
    {
        if (last >= 0)
            return false;
        else
            return true;
    }

    int GetMin()
    {
        if (last >= 0)
        {
            return array[0];
        }
    }

    PriorityQueue()
    {
        last = -1;
    }

    void push(int x)
    {
        array[last + 1] = x;
        last++;
        int tempLast = last;
        while (array[tempLast] < array[(tempLast -1)/2] && tempLast != 0)
        {
            int temp = array[(tempLast - 1) / 2];
            array[(tempLast - 1) / 2] = array[tempLast];
            array[tempLast] = temp;
            tempLast = (tempLast - 1) / 2;
        }
    }

    void extractMin()
    {
        if (last >= 0)
        {
            array[0] = array[last];
            last--;
            int index = 0;
            int minTemp;
            do
            {
                if (2 * (index + 1) - 1 == last)
                    minTemp = 2 * (index + 1) - 1;
                else if (2 * (index + 1) - 1 < last)
                {
                    if (array[2 * (index + 1) - 1] < array[2 * (index + 1)])
                        minTemp = 2 * (index + 1) - 1;
                    else
                        minTemp = 2 * (index + 1);
                }
                else
                    break;
                if (array[index] > array[minTemp])
                {
                    int temp = array[minTemp];
                    array[minTemp] = array[index];
                    array[index] = temp;
                    index = minTemp;
                }
                else
                    break;
            } while (index <= last);
        }
    }
    
    void decreaseKey(int x, int y)
    {
        int index;
        for (int i = 0; i < 100000; i++)
        {
            if (array[i] == x)
            {
                array[i] = y;
                index = i;
                break;
            }
        }
        while (array[index] < array[(index - 1) / 2] && index != 0)
        {
            int temp = array[(index - 1) / 2];
            array[(index - 1) / 2] = array[index];
            array[index] = temp;
            index = (index - 1) / 2;
        }
    }
};


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

    /*ifstream inFile;
    inFile.open("in.txt");
    ofstream outFile;
    outFile.open("out.txt");*/
    PriorityQueue pq;
    int n = 100000;
    int p[100000];
    for (int i = 0; i < n; i++)
    {
        string s;
        getline(inFile, s);
        if (s[0] == ' ' || s == "")
        {
            break;
        }
        if (s[0] == 'p')
        {
            int pos = s.find(" ") + 1;
            string num1;
            num1.assign(s, pos, s.length());
            int num = atoi(num1.c_str());
            p[i] = num;
            pq.push(num);
        }
        else if (s[0] == 'e')
        {
            if (!pq.isEmpty())
            {
                int min = pq.GetMin();
                outFile << min << endl;
            }
            else
                outFile << '*' << endl;
            pq.extractMin();
        }
        else if (s[0] == 'd')
        {
            int pos1 = s.find(" ") + 1;
            int pos2 = s.rfind(" ");
            string num1;
            num1 = s.substr(pos1, pos2-pos1);
            int num_1 = atoi(num1.c_str());
            string num2;
            num2 = s.substr(pos2 + 1, s.length()-pos2-1);
            int num_2 = atoi(num2.c_str());    
            pq.decreaseKey(p[num_1-1], num_2);
        }
    }
    inFile.close();
    outFile.close();
    return 0;
}