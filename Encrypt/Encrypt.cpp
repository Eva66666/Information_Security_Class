#include <stdio.h>
#include <iostream> 
#include <string.h>
#include <math.h>
using namespace std; 
int main(){
    void playfair(string,string);
    void caesar(string,string);
    void vernam(string,string);
    void row(string,string);
    void rail_fence(string,string);
    string key;
    string plaint;
    char method[20];
    cin >> method >> key >> plaint;
    if(strcmp(method,"caesar") == 0){
        caesar(key,plaint);
    }
    else if(strcmp(method,"playfair") == 0){
        playfair(key,plaint);
    }
    else if(strcmp(method,"vernam") == 0){
        vernam(key,plaint);
    }
    else if(strcmp(method,"row") == 0){
        row(key,plaint);
    }
    else if(strcmp(method,"rail_fence") == 0){
        rail_fence(key,plaint);
    }
    else{
        std::cout << "請輸入正確格式！" << std::endl;
    }
    return 0;
}

//Caesar cipher
    void caesar(string key,string plaint){
        char cipher[40];
        int caesar_key = atoi(key.c_str());
        for(int i = 0 ; i < plaint.length(); i++){
            cipher[i] = ((plaint[i]-'a')+caesar_key)%26+'A';
        }
        cout << cipher;
    }
    
    //Playfair cipher
    void playfair(string p_key,string p_plaint){
        char the_key[5][5];
        for(int i = 0 ; i < 5; i++){
            for(int j = 0; j < 5; j++){
                the_key[i][j]='j';
            }
        }
        int lb = 0;
        char p_list[25]={'A','B','C','D','E','F','G','H','I','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
        for(int i = 0; i < p_key.length(); i++){
            if (p_key[i] =='j'){
                p_key[i] = 'i';
            }
            int j = (i-lb)/5;
            int k = (i-lb)%5;
            int p_l = p_key[i]-'A';
            if(p_l >= 10){
                p_l = p_l - 1;
            }
           if (the_key[j][k] == 'j' && p_list[p_l] != 'j'){
                the_key[j][k] = p_key[i];
            }else{
                lb++;
            }
            p_list[p_l] = 'j';
        }
        
        
        int p = 0;
        for(int j = (p_key.length()-lb)/5, k = (p_key.length()-lb)%5; k < 5; k++){
            if(p_list[p] == 'j'){
                while(p_list[p] == 'j'){
                        p++;
                    }
                the_key[j][k] = p_list[p];
                p++;
                }else{
                    the_key[j][k] = p_list[p];
                    p++;
                }
        }
        for(int j = ((p_key.length()-lb)/5)+1;j<5;j++){
            for(int k = 0;k<5;k++){
                if(p_list[p] == 'j'){
                    while(p_list[p] == 'j'){
                        p++;
                    }
                    the_key[j][k] = p_list[p];
                    p++;
                }else{
                    the_key[j][k] = p_list[p];
                    p++;
                }
            }
        }
        int q =0;
        char s[40];
        for(int i = 0; i <p_plaint.length(); i=i+2){
            s[q]=p_plaint[i]-'a'+'A';
            s[q+1]=p_plaint[i+1] - 'a' +'A';
            if(s[q]=='J'){
                s[q]='I';
            }
            if(s[q+1]=='J'){
                s[q+1]='I';
            }
            if(s[q]==s[q+1]){
                s[q+1]='X';
                i--;
            }
            q=q+2;
        }
        if(q%2 ==1){
            q++;
            s[q] = 'X';
        }
        //step 3
        int a,b,c,d;
        char cipher[30];
        for(int k = 0; k < q; k=k+2){
            for(int i = 0; i < 5; i++){
                for(int j = 0; j <5 ;j++){
                    if (the_key[i][j] == s[k]){
                        a = i;
                        b = j;
                    }
                    if(the_key[i][j] == s[k+1] ){
                        c = i;
                        d = j;
                    }
                }
            }
            if(a != c && b != d){
                    cipher[k] = the_key[a][d];
                    cipher[k+1] = the_key[c][b];
                }else if(a == c){
                    b = (b+1)%5;
                    d = (d + 1) % 5;
                    cipher[k] = the_key[a][b];
                    cipher[k+1] = the_key[c][d];
                }else if(b==d){
                    a = (a+1)%5;
                    c = (c+1)%5;
                    cipher[k] = the_key[a][b];
                    cipher[k+1] = the_key[c][d];
                }
        }
        for(int o = 0; o<30;o++){
            printf("%c",cipher[o]);
        }
    }
//vernam    
void vernam(string key,string plaint){
    int p = key.length();
    int q = plaint.length();
    char the_key[30];
    int row,col;
    if(p<q){
        for(int i = 0; i < q; i++){
            if(i<key.length()){
                the_key[i]=key[i];
                row = the_key[i] - 'A';
            }
            else{
                the_key[i] = plaint[i-p];
                row = the_key[i] - 'a';
            }
            col = plaint[i] - 'a';
            int ccc = row^col;
            char kkk = 'A'+ ccc;
            cout << kkk;
        }
    }else{
        for(int i = 0; i < q; i++){
                int row = key[i] - 'A';
                int col = plaint[i] - 'a';
                int ccc = row^col;
                char kkk = 'A'+ ccc;
                cout << kkk;
        }
    }
}

//row
void row(string key,string plaint){
    int p = key.length();
    int q = plaint.length();
    int a,b;
    char plaintext[40][40];
    int row_key[q];
    int r_l = atoi(key.c_str());
    int rr= r_l;
    for(int i = 0; i<p; i++){
        int r_i = int(pow10(p-i-1));
        int r = rr/r_i;
        if(r>0){
            row_key[i] = r;
        }else{
            row_key[i] = r_l % 10;
        }
        rr = rr%r_i;
    }
    for(int i = 0; i < q; i++){
        a = i/p;
        b = i%p;
        plaintext[a][b] = plaint[i] - 'a' + 'A';
    }
    if(b<p){
        int k = p - b;
        for(int i = k -2,j = 0; i >= 0; i--,j++ ){
            plaintext[a][k+j] = 'Z' - i;
        }
    }
    if(b > 0){
        a++;
    }
    for(int i = 0; i < a; i++){
        for(int j = 0; j < p; j++){
        }
    }
    char cipher[40][40];
    for(int i = 0; i<p; i++){
        for(int j = 0; j < a; j++){
            int s = row_key[i] - 1;
            cipher[s][j] = plaintext[j][i];
        }
    }
    for(int o = 0 ; o < p; o++){
            for(int ll = 0; ll < a; ll++){
                 printf("%c",cipher[o][ll]);
            }
        }
}

void rail_fence(string key,string plaint){
    int k = 0, n = 0,w = 0;
    int r_l = atoi(key.c_str());
    string s[r_l];
    for(int i = 0; i<r_l; i++){
        s[i] = "";
    }
    for(int i = 0; i < plaint.length(); i++){
        if((w)%(2*r_l-2)==0){
            n = 0;
            w = 0;
        }
        //
        if(w+1==r_l){
            n = 1;
        }
        char x = plaint[i]-'a'+'A';
        s[k] = s[k] + x;
        
        if(n == 0){
            k++;
        }
        if(n==1){
            k--;
            
        }
        w++;
    }
    for(int i = 0; i<r_l; i++){
        cout<<s[i];
    }
}
