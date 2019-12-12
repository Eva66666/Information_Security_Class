#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <string>
#define MaxSize  1024
typedef long long ll;
using namespace std;
// 筛选法生成伪素数
ll producePrime()
{
    srand((unsigned)time(NULL));
    bool isprime = false;
    ll a;
    int arr[15] = {2,3,5,7,11,13,17,19,23,2,31,37,41,43,47};   //50以内的素数 
    while (!isprime)
    {       
        isprime = true;       
        a = rand()%1000+1; //生成一个范围在1～10001里的奇数
        for (int j = 0; j<15; ++j)
        {
            if (a%arr[j] == 0)
            {
                isprime = false;
                break;
            }
        }
    }
    return a;
}
//平方乘法求模(a^b)%mode
ll Mode(ll a, ll b, ll mode)
{
    ll sum = 1;
    while (b > 0) {
        if (b % 2 == 1)		//判断是否是奇数，是奇数的话将多出来的数事先乘如sum
            sum = (sum * a) % mode;
        b /= 2;
        a = (a * a) % mode;// 不断的两两合并再取模，减小a和b的规模
    }
    return sum;
}
bool Miller_Rabin(ll n, int repeat)//n是测试的大数，repeat是测试重复次数
{
    //将n-1分解成2^s*d
    ll d = n - 1;
    int s = 0;
    while(!(d & 1)) ++s, d >>= 1;
    srand((unsigned)time(NULL));
    for(int i = 0; i < repeat; i++)//重复repeat次
    {
        ll a = rand() % (n - 3) + 2;//取一个随机数,[2,n-1)
        ll x = Mode(a, d, n);
        ll y = 0;
        for(int j = 0; j < s; j++)
        {
           y = Mode(x, 2, n);
           if(y == 1 && x != 1 && x != (n - 1))return false;
           x = y;
        }
       if(y != 1)return false;//费马小定理
    }
    return true;
}
//辗转相除法求gcd
int gcd(int a, int b) {
    int temp;
    while(b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}
//获得e
int randExponent(ll phi, ll n) {
    ll e = rand() % 1000;
    while(1) {
        if(gcd(e, phi) == 1) return e;
        e = (e + 1) % n;
        if(e <= 2) e = 3;
    }
}
ll exgcd(ll a,ll b,ll &x,ll &y)//扩展欧几里得算法 
{
    if(b==0)
    {
        x=1,y=0;
        return a;
    }
    ll ret=exgcd(b,a%b,y,x);
    y-=a/b*x;
    return ret;
}
ll getInv(ll a,ll mod)//求a在mod下的逆元
{
    ll x,y;
    ll d=exgcd(a,mod,x,y);
    return (x%mod+mod)%mod;
}
void generateKey(){
    ll p,q;
    bool findP = false;
    bool findQ = false;
    while(!findP){
	p = producePrime();
	if (Miller_Rabin(p,20)){
	    findP = true;
	}
        usleep(1000000);
    }  
    while(!findQ){
	q = producePrime();
	if (Miller_Rabin(q,20)){
	    findQ = true;
	}
        usleep(1000000);
    }
    ll n = p*q;
    ll phi = (p-1)*(q-1);
    ll e = randExponent(phi,n);
    ll d = getInv(e,phi);
    cout << "p = " << p << endl;
    cout << "q = " << q << endl;
    cout << "n = " << n << endl;
    cout << "e = " << e << endl;
    cout << "d = " << d << endl;
}
void encrypt(char* plainText,ll e, ll n){
    cout<<"ciphertext is:";
    string str = plainText;    
    for(int i=0;i<str.length();i++){
	cout << Mode(plainText[i],e,n) << " ";
    }
    cout << endl;
}
void decrypt(int* cipherText,ll d,ll n){
    char plaintext[MaxSize] = {0};
    for(int i=0;;i++){
        if(cipherText[i] != 0){
	    plaintext[i] = Mode(cipherText[i],d,n);
	}else
	    break;
    }
    cout << "plaintext is: "<< plaintext<< endl;
}
int main(int argc, char* argv[])
{
    if(argv[1] == NULL)
	cout << "Invalid input!"<<endl; 
    else{
	string s = argv[1];
	if(s == "init") 
            generateKey();
        else if(s == "encrypt" && argv[4] != NULL){
	    ll e = atol(argv[2]);
	    ll n = atol(argv[3]);
            string plainText = argv[4];
            int i = 5;
            while(argv[i] != NULL){
                string tmp = argv[i];
	        plainText = plainText + " " + tmp;	
	        i++;
	    }
            char* p = const_cast<char*>(plainText.c_str());
	    encrypt(p,e,n);
        }
        else if(s == "decrypt" && argv[4] != NULL){
	    ll d = atol(argv[2]);
	    ll n = atol(argv[3]);
            int c[MaxSize] = {0};
	    int i = 4;
 	    while(argv[i] != NULL){
		c[i-4] = atoi(argv[i]);                
		i++;
            }
	    decrypt(c,d,n);
        }
        else
	    cout << "Invalid input!"<<endl;
    }    
    return 0;
}
