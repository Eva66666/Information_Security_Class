

#include <iostream>
#include <string>
using namespace std;
void Decrypt_Verman(string& key, string& Ciphertext)
{
	string plaintext = "";
	for (int i = 0; i < Ciphertext.length(); i++) {
		plaintext += 'a' + ((Ciphertext[i] - 'a') ^ (tolower(key[i]) - 'a'));
		if (plaintext[i] > 'z')
			plaintext[i] -= 26;
		key += plaintext[i];
	}
	for (int i = 0; i < plaintext.size(); i++)
	{
		char tmp = tolower(plaintext[i]);
		cout << tmp;
	}
	cout << '\n';
}
int main()
{
	string str1, key, Ciphertext;
	while (cin >> str1 >> key >> Ciphertext)// ./Decrypt Verman CON IKJFCKHLIFJTJVTMEC][ 
	{
		Decrypt_Verman(key, Ciphertext);
	}
}


