#include <fstream>
#include <iostream>
#include <string>
using namespace std;

long long* buildPref(string substring) {
	long long* p = new long long[substring.length() + 1];
	p[1] = 0;
	long long i = 1;
	long long j = 0;
	while (i < substring.length()) {
		if (substring[i] == substring[j]) {
			p[i + 1] = j + 1;
			i++;
			j++;
		}
		else {
			if (j > 0) {
				j = p[j];
			}
			else {
				p[i + 1] = 0;
				i++;
			}
		}
	}
	return p;
}


long long findSubstring(string mainstring, string substring)
{
	long long* p = buildPref(substring);
	long long i = 0, j = 0;
	while (i < mainstring.length() && j < substring.length())
	{
		if (mainstring[i] == substring[j])
		{
			i++;
			j++;
		}
		else if (j > 0)
			j = p[j];
		else
			i++;
	}
	delete[] p;
	if (j == substring.length())
		return i - substring.length();
	else
		return -1;
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

	/*ifstream inFile;
	inFile.open("in.txt");
	ofstream outFile;
	outFile.open("out.txt");*/

	string s1;
	string s2;
	inFile >> s1;
	inFile >> s2;
	long long count = 0;
	long long* result = new long long[s2.length()];
	long long res = findSubstring(s2, s1);
	long long pos = 0;
	while (res != -1)
	{
		pos += res + 1;
		result[count] = pos;
		count++;
		s2.erase(0, res + 1);
		res = findSubstring(s2, s1);
	}
	outFile << count << endl;
	if (count > 0)
	{
		outFile << result[0];
		for (int i = 1; i < count; i++)
		{
			outFile << " " << result[i];
		}
	}
	delete[] result;
	inFile.close();
	outFile.close();
	return 0;
}