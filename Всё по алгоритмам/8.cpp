#include <fstream>
#include <iostream>
#include <ctime>
using namespace std;

class HashTable {
private:

    struct Cell {
        int key;
        bool isEmpty = true;
        int next;
    };

    Cell* array;
    int a, b, p, size;
    int count;

    int hashFunc(int key) {
        return(((a * key + b) % p) % size);
    }

    int getRandom(int min, int max)
    {
        srand(time(NULL));
        int i = min + rand() % (max - min + 1);
        return i;
    }

    int getPrime(int n) const //поиск самого маленького простого числа >= 2*n
    {
        int i = 2 * n;
        while (!isPrime(i))
            i++;
        return i;
    }

    bool isPrime(int n) const // проверка на простоту числа
    {
        bool isPrime = true;
        for (int count = 2; count < n && isPrime; count++)
            if (n % count == 0)
                isPrime = false;
        return isPrime;
    }
public:

    HashTable(int n) // конструктор
    {
        /*size = getPrime(n);*/
        size = n;
        array = new Cell[size];
        p = getPrime(size);
        a = getRandom(1, p - 1);
        b = getRandom(0, p - 1);
        for (int i = 0; i < size; i++)
        {
            array[i].next = -1;
            array[i].isEmpty = true;
        }
        count = 0;
    }

    ~HashTable() // деструктор
    {
        delete[] array;
    }

    int GetCount()
    {
        return count;
    }
    void insert(int x) // поиск элемента в таблице с помощью индекса на следующий
    {
        int hash = hashFunc(x);
        int origHash = hash;
        bool found = false;
        if (!array[hash].isEmpty)
        {
            while (array[hash].next != -1) // ищем конец хвоста
                hash = array[hash].next;

            for (int i = hash+1; i < size; i++) // ищем свободное место в таблице от конца хвоста до конца массива
            {
                if (array[i].isEmpty)
                {
                    array[i].key = x;
                    array[i].isEmpty = false;
                    array[hash].next = i;
                    found = true;
                    break;
                }
            }
            if (!found) // ищем свободное место в таблице от 0 до начала хвоста
            {
                for (int i = 0; i < origHash; i++)
                {
                    if (array[i].isEmpty)
                    {
                        array[i].key = x;
                        array[i].isEmpty = false;
                        array[hash].next = i;
                        found = true;
                        break;
                    }
                }
            }
        }
        else
        {
            array[hash].key = x;
            array[hash].isEmpty = false;
            found = true;
        }
        if (!found)
            cout << "Нет места в хэш-таблице";
        else
            count++;
    }

    int search(int x)
    {
        int hash = hashFunc(x);
        if (!array[hash].isEmpty)
        {
            while (array[hash].key != x && array[hash].next != -1) // ищем элемент равный x или доходим до конца хвоста
            {
                hash = array[hash].next;
            }
            if (array[hash].key == x && !array[hash].isEmpty) // проверка на пустой элемент, потому что можем не затереть key
                return hash;
            else
                return -1;
        }
        else
            return -1;
    }

    void remove(int x) // удаление с помощью сдвига хвоста вверх
    {
        int found = search(x);
        int parent = found;
        if (found != -1)
        {
            int hash = found;
            while (array[hash].next != -1)
            {
                array[hash].key = array[array[hash].next].key;
                parent = hash;
                hash = array[hash].next;
            }
            array[hash].isEmpty = true;
            array[parent].next = -1;
            count--;
        }
    }
};

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
    /*ifstream inFile;
    inFile.open("in.txt");
    ofstream outFile;
    outFile.open("out.txt");*/

    int n;
    inFile >> n;
    HashTable ht(n);
    char command;
    int a;
    for (int i = 0; i < n; i++)
    {
        inFile >> command;
        inFile >> a;
        if (command == '+')
        {
            ht.insert(a);
        }
        else if (command == '-')
        {
            ht.remove(a);
        }
        else if (command == '?')
        {
            if (ht.search(a) != -1)
                outFile << "true\n";
            else
                outFile << "false\n";
        }
    }
    inFile.close();
    outFile.close();
    return 0;
}