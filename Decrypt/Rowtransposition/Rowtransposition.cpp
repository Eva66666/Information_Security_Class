

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cmath>
using namespace std;
void Decrypt_Rowtransposition(string &key, string &Ciphertext)
{
	const int  minr = Ciphertext.size() / key.size();
	map<char,vector<char> > m;
	for (int i = 0; i < key.size(); i++)//按密文建立char对vector的映射
	{
		vector<char> tmp(minr);
		m[key[i]] = tmp;
	}
	for (int i = 0, tmp = Ciphertext.size() % key.size(); i < key.size() && tmp; i++, tmp--)//确定每个char向量的长度
	{
		m[key[i]].resize(minr + 1);
	}
	int i = 0;
	for (char j = '1'; j <= (char)('0' + key.size()); j++)//讲密文依次填入map中
	{
		for (int k = 0; k < m[j].size(); k++)
		{
			m[j][k] = Ciphertext[i++];
		}
	}
	for (int i = 0; i < minr+1; i++)//输出明文
	{
		for (int j = 0; j < key.size(); j++)
		{
			if (i < m[key[j]].size())
			{
				char tmp = tolower(m[key[j]][i]);
				cout << tmp;
			}
		}
	}
}
int main()
{
	string str1, key, Ciphertext;
	while (cin >> str1 >> key >> Ciphertext)// ./Decrypt Rowtransposition 31562487 ENEGEKGVOREEUPVPNIIG

	{
		Decrypt_Rowtransposition(key, Ciphertext);
	}
}

