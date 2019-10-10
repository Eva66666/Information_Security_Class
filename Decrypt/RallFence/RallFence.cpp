

#include <iostream>
#include <string>
#include <cctype>
#include <set>
#include <vector>
#include <cmath>
using namespace std;

void Decrypt_RallFence(int& key, string& Ciphertext)
{
	const int size = ceil((float)Ciphertext.size() / key);
	vector<vector<char> > v(key);
	for (int i = 0; i < Ciphertext.size(); i++)//将密文放入k行中
	{
		v[i / size].push_back(Ciphertext[i]);
	}
	for (int i = 0; i < size; i++)//交叉提出，变成明文
	{
		for (int j = 0; j < key; j++)
		{
			if (i < v[j].size())
			{
				char tmp = tolower(v[j][i]);
				cout << tmp;
			}
		}
	}
}
int main()
{
	string str1, Ciphertext;
	int key;
	while (cin >> str1 >> key >> Ciphertext)//./RallFence Decrypt 2 TEOGSDYUTAENNHLNETAMSHVAED
	{
		Decrypt_RallFence(key, Ciphertext);
	}
	
}


