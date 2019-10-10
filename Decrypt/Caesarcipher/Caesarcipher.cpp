
#include <iostream>
#include <string>
#include <cctype>
using namespace std;
int main()
{
	//./Decrypt caesar 7 RLLWNVPUNULCLYNPCLBW 
	string str1, Ciphertext;
	int key;
	while (cin >> str1  >> key >> Ciphertext)
	{
		for (int i = 0; i < Ciphertext.size(); i++)
		{
			Ciphertext[i] = Ciphertext[i] - key;
			while(Ciphertext[i] < 'A')
			{
				Ciphertext[i] += 26;
			}
			Ciphertext[i] = tolower(Ciphertext[i]);
		}
		cout << Ciphertext;
	}
}

