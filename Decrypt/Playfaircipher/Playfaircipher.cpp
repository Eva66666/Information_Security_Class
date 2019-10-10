

#include <iostream>
#include <string>
#include <cctype>
#include <set>
using namespace std;
char matrix[5][5];
int r(char x)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (matrix[i][j] == x)
			{
				return i;
			}
			else if ((x == 'I' || x == 'J') && (matrix[i][j]=='I'|| matrix[i][j]=='J'))
			{
				return i;
			}
		}
	}
}
int c(char x)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (matrix[i][j] == x)
			{
				return j;
			}
			else if ((x == 'I' || x == 'J') && (matrix[i][j] == 'I' || matrix[i][j] == 'J'))
			{
				return j;
			}
		}
	}
}
void Decrypt_Playfair(string& key, string& Ciphertext)
{	
	set<char> s;
	int t = 0;
	for (int i = 0; i < key.size(); i++)//将key依次填入到矩阵当中，set记录已存在字母
	{
		if (!s.count(key[i]))
		{
			if ((key[i] == 'J' && s.count('I')) || (key[i] == 'I' && s.count('J')))
			{
				continue;
			}
			s.insert(key[i]);
			matrix[t / 5][t % 5] = key[i];
			t++;			
		}
	}
	for (char c = 'A'; c <= 'Z'; c++)//剩余字母依次填充
	{
		if (!s.count(c))
		{
			if ((c == 'J' && s.count('I')) || (c == 'I' && s.count('J')))
			{
				continue;
			}
			s.insert(c);
			matrix[t / 5][t % 5] = c;
			t++;
		}
	}
	for (int i = 0; i < Ciphertext.size()-1; i+=2)//按密文输出明文
	{
		if (r(Ciphertext[i]) == r(Ciphertext[i+1]))
		{
			int p1c = c(Ciphertext[i]) - 1, p2c = c(Ciphertext[i + 1]) - 1;
			while (p1c < 0)
			{
				p1c += 5;
			}
			while (p2c < 0)
			{
				p2c += 5;
			}
			char p1 = tolower(matrix[r(Ciphertext[i])][p1c]);
			char p2 = tolower(matrix[r(Ciphertext[i])][p2c]);
			cout << p1 << p2;
		}
		else if (c(Ciphertext[i]) == c(Ciphertext[i + 1]))
		{
			int p1r = r(Ciphertext[i]) - 1, p2r = r(Ciphertext[i + 1]) - 1;
			while (p1r < 0)
			{
				p1r += 5;
			}
			while (p2r < 0)
			{
				p2r += 5;
			}
			char p1 = tolower(matrix[p1r][c(Ciphertext[i])]);
			char p2 = tolower(matrix[p2r][c(Ciphertext[i])]);
			cout << p1 << p2;
		}
		else
		{
			char p1 = tolower(matrix[r(Ciphertext[i])][c(Ciphertext[i + 1])]);
			char p2 = tolower(matrix[r(Ciphertext[i + 1])][c(Ciphertext[i])]);
			cout << p1 << p2;
		}
	}
}
int main()
{
	string str1, key, Ciphertext;
	while (cin >> str1  >> key >> Ciphertext)//./Decrypt Playfair HIT MCCROUALFOCXMXDBXCPQ
	{
		Decrypt_Playfair(key, Ciphertext);
	}
}

