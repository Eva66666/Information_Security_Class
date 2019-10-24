#include <stdio.h>
#include <stdlib.h>
#include <iostream>       
#include <string>  


using namespace std;

int IP[64]={
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9,  1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};
int IP_1[64]={
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
	37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28,
	35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26,
	33, 1, 41,  9, 49, 17, 57, 25
};
int E[48]={
    32,  1,  2,  3,  4,  5,
	4,  5,  6,  7,  8,  9,
	8,  9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32,  1
};
int PC_1[56] = {
    57, 49, 41, 33, 25, 17, 9,
	 1, 58, 50, 42, 34, 26, 18,
	10,  2, 59, 51, 43, 35, 27,
	19, 11,  3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
	 7, 62, 54, 46, 38, 30, 22,
	14,  6, 61, 53, 45, 37, 29,
	21, 13,  5, 28, 20, 12,  4
};
int PC_2[48] = {
    14, 17, 11, 24,  1,  5,
	 3, 28, 15,  6, 21, 10,
	23, 19, 12,  4, 26,  8,
	16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
};
int P[32] ={
    16,  7, 20, 21,29, 12, 28, 17,
	 1, 15, 23, 26, 5, 18, 31, 10,
	 2,  8, 24, 14,32, 27,  3,  9,
	19, 13, 30,  6,22, 11,  4, 25
};
int S_BOX[8][4][16] = {
	{  
		{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},  
		{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},  
		{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0}, 
		{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13} 
	},
	{  
		{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},  
		{3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5}, 
		{0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},  
		{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}  
	}, 
	{  
		{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},  
		{13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},  
		{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},  
		{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}  
	}, 
	{  
		{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},  
		{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},  
		{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},  
		{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}  
	},
	{  
		{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},  
		{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},  
		{4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},  
		{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}  
	},
	{  
		{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},  
		{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},  
		{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},  
		{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}  
	}, 
	{  
		{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},  
		{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},  
		{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},  
		{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}  
	}, 
	{  
		{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},  
		{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},  
		{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},  
		{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}  
	} 
};
int test32[]{
	 1, 1, 1, 1, 0, 0, 1, 0, 
	 1, 1, 0, 0, 1, 0, 1, 1, 
	 0, 1, 1, 1, 1, 1, 0, 0, 
	 0, 0, 0, 1, 1, 0, 1, 0
};
int test48[]{
	1, 1, 1, 1, 0, 0, 1, 0, 
	1, 1, 0, 0, 1, 0, 1, 1, 
	0, 1, 1, 1, 1, 1, 0, 0, 
	0, 0, 0, 1, 1, 0, 1, 0,
	1, 1, 1, 1, 0, 0, 1, 0,
	1, 1, 1, 1, 0, 0, 1, 0
};
int shift[]={1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
int* toBits(string);
int* f_function(int*,int*);
int* key_schedule(int *);
int* left_shift(int *,int);

int main(){

    string cipher;
    string key;
    cin >> key >>cipher;
    //轉成二進制形式
    int* b_key = toBits(key);
    int* b_cipher = toBits(cipher);
    //test1
    /*std::cout << "test tobit" << std::endl;
    for(int i =1;i<=64;i++){
    	cout<<b_cipher[i-1];
    	if(i%8==0){
    		std::cout << "" << std::endl;
    	}
    }
    std::cout << "test tobit" << std::endl;*/
    
    
    int b_cipher1[64];
    int init_L[32],left[32];
    int init_R[32];
    //置換IP, get left,right
    for(int i = 0;i<64;i++){
    	b_cipher1[i]=b_cipher[IP[i]-1];
    	if(i<32){
			init_L[i]=b_cipher1[i];
		}else{
			init_R[i-32]=b_cipher1[i];
		}
    }
    
    //test2
    /*std::cout <<"\n"<< "test ip" << std::endl;
    for(int i =1;i<=64;i++){
    	cout<<b_cipher1[i-1];
    	if(i%8==0){
    		std::cout << "" << std::endl;
    	}
    }
    std::cout << "test ip" << std::endl;*/
    
    //test3
    /*std::cout <<"\n"<< "test l\r" << std::endl;
    for(int i = 1;i<32;i++){
    	cout<<init_L[i-1];
    	if(i%8==0)
    	std::cout << "" << std::endl;
    }
    std::cout << "" << std::endl;
    for(int i = 0;i<32;i++){
    	cout<<init_R[i];
    	if(i%8==0)
    	std::cout << "" << std::endl;
    }
    std::cout << "test l\r" << std::endl;*/
    
    //get subkey
    int* subkey = key_schedule(b_key);
    //test4
    /*std::cout <<"\n"<< "ori key" << std::endl;
    for(int i =1;i<=64;i++){
    	cout<<b_key[i-1];
    	if(i%8==0){
    		std::cout << "" << std::endl;
    	}
    }
    std::cout << "ori key" << std::endl;
    std::cout <<"\n"<< "test key" << std::endl;
    for(int i =1;i<=768;i++){
    	cout<<subkey[i-1];
    	if(i%8==0){
    		std::cout << "" << std::endl;
    	}
    	if(i%48==0){
    		std::cout << "" << std::endl;
    	}
    }
    std::cout << "test key" << std::endl;*/
    
    
    int sub_key[48];
    
	//16 rounds
    for(int turn=0;turn<16;turn++){
    	for(int i=0;i<48;i++){
    		sub_key[i]=subkey[768-((turn+1)*48)+i];
    	}
    	//test5
	    /*std::cout <<"\n"<< "sub key" << std::endl;
	    for(int i =1;i<=64;i++){
	    	cout<<sub_key[i-1];
	    	if(i%8==0){
	    		std::cout << "" << std::endl;
	    	}
	    }
	    std::cout << "sub key" << std::endl;*/
	    
    	for(int i = 0;i<32;i++){
			left[i] = init_R[i];
		}
		int* right=f_function(init_R,sub_key);
		for(int i = 0;i<32;i++){
			//cout<<right[i];
			//if(i%8==0)
			//std::cout << "" << std::endl;
			init_R[i]=init_L[i]^right[i];
		}
		
		//std::cout << "" << std::endl;
		for(int i = 0;i<32;i++){
			init_L[i]=left[i];
		}
    }
    
    
    //combine right&left
    int plain[64];
    for(int i =0;i<64;i++){
    	if(i<32){
    		plain[i]=init_L[i];
    	}else{
    		plain[i]=init_R[i-32];
    	}
    }
    /*std::cout << "\n" << std::endl;
    for(int i =1 ;i<=64;i++){
    	cout<<plain[i];
    	if(i%8==0){
    		std::cout << "" << std::endl;
    	}
    }*/
    
    //IP-1
    int bitplain[64];
    //std::cout << "\n" << std::endl;
    for(int i = 0;i<64;i++){
    	bitplain[i]=plain[IP_1[i]-1];
    }
    
    //test
    /*std::cout <<"\n"<< "test ip-1" << std::endl;
    for(int i =1;i<=64;i++){
    	cout<<bitplain[i-1];
    	if(i%8==0){
    		std::cout << "" << std::endl;
    	}
    }
    std::cout << "test ip-1" << std::endl;*/
    
    //tohex
    std::cout << "\n\n" << std::endl;
    char out_plain[16];
    for(int i =0;i<64;i=i+4){
            //k->the number of round
            int k = i/4;
            int dec=8*bitplain[i]+4*bitplain[i+1]+2*bitplain[i+2]+bitplain[i+3];
            if(dec>9){
                int w=dec-10;
                out_plain[k]='a'+w;
                cout<<out_plain[k];
            }else{
                out_plain[k]='0'+ dec;
                cout<<out_plain[k];
            }
    }
    //int* test = f_function(test32,test48);
    /*for(int i =0;i<6;i++){
    	std::cout << test[i] << std::endl;
    }*/
    return 0;
}
int* toBits(string input){
	int *b = new int[64];
	for(int i =2, n = -1; input[i] != '\0';i++){
        if(input[i]>='0' && input[i]<='9'){
            int a = input[i]-'0';
            int k = 0;
            while(a != 0){
                b[n+4-k]=a%2;
                a /= 2;
                k++;
            }
            n+=4;
            if(k<4){
                for(int i = 1;i<(4-k);i++){
                        b[n+i-4]=0;
                }
            }
        }
        if(input[i]>='A' && input[i]<='F'){
            int a = input[i]-'A'+10;
            int p = 0;
            while(a != 0){
                b[n+4-p]=a%2;
                a /= 2;
                p++;
                
            }
            n+=4;
        }
    }
    return b;
}

int* f_function(int R[32],int key[48]){
	//Expansion E
	int R1[48];
	for(int i=0;i<48;i++){
		int a = E[i]-1;
		R1[i] = R[a];
	}
	//XOR with round key
	int R2[48];
	for(int i=0;i<48;i++){
		R2[i] = R1[i]^key[i];
	}
	//test informations
	/*for(int i=0;i<32;i++){
    	cout<<" "<< R[i];
    	if((i+1)%4==0)
    	std::cout << " ";
    	if((i+1)%8==0)
    	std::cout << " " << std::endl;
    }
	std::cout << " " << std::endl;
	std::cout << "R1 " << std::endl;
	for(int i=0;i<48;i++){
    	cout<<" "<< R1[i];
    	if((i+1)%6==0)
    	std::cout << " " << std::endl;
    }
    std::cout << " " << std::endl;
    std::cout << "key " << std::endl;
    for(int i=0;i<48;i++){
    	cout<<" "<< key[i];
    	if((i+1)%6==0)
    	std::cout << " " << std::endl;
    }
    std::cout << " " << std::endl;
    std::cout << "R2 " << std::endl;
	for(int i=0;i<48;i++){
    	cout<<" "<< R2[i];
    	if((i+1)%6==0)
    	std::cout << " " << std::endl;
    }*/
    
	//S-box substitution
	int R3[32];
	for(int j=1,k=1; j<=48; j=j+6){
		int i = (k-1)*6;
		int t = (k-1)*4;
		int row = 2*R2[i]+R2[i+5];
		int col = 8*R2[i+1]+4*R2[i+2]+2*R2[i+3]+R2[i+4];
		int r= S_BOX[k][row][col];
		int r1[4];
		//cout<<"j:"<<j<<" turns:"<<k<<" rows:"<<row<<" col:"<<col<<" r:"<<r<<endl;
		for(int a = 0;a<4 ;a++){
			if(r !=0){
				r1[a]=r%2;
				r /= 2;	
			}else{
				r1[a]=0;	
			}
		}
		for(int a = 0; a<4;a++){
			R3[t+a]=r1[3-a];
			//cout<<" R3["<<t+a<<"]:"<<R3[t+a];
		}
		//cout<<""<<endl;
		k++;
	}
	//Permutation
	int *R4 = new int[32];
	for(int i = 0; i<32;i++){
		R4[i]= R3[P[i]-1];
	}
	
	return R4;
}

int* left_shift(int subkey[28],int s){
	int* out = new int[28];
	for(int i=0; i<28; i++){
		out[i]=subkey[i+s%28];
	}
	return out;
}

int* key_schedule(int key[64]){
	int C[28],D[28];
	int key2[56],key3[48];
	int *key4 = new int[768];
	//P_1
	for(int i=0;i<56;i++){
		/*if(i<28){
			C[i]=key[PC_1[i]-1];
		}else{
			D[i-28]=key[PC_1[i]-1];
		}*/
		key2[i]=key[PC_1[i]-1];
	}
	
	
	
	for(int key_round = 0;key_round<16;key_round++){
		for(int i=0;i<56;i++){
			if(i<28){
				C[i]=key2[i];
			}else{
				D[i-28]=key2[i];
			}
		}
		int* C1 = left_shift(C,shift[key_round]);
		int* D1 = left_shift(D,shift[key_round]);
		for(int i=0;i<28;i++){
			C[i]=C1[i];
			D[i]=D1[i];
			key2[i]=C1[i];
			key2[i+28]=D1[i];
		}
		//PC_2
		for(int i =0;i<48;i++){
			key4[key_round*48+i]=key2[PC_2[i]-1];
			//cout<<i<<" key2["<<PC_2[i]-1<<"]"<<key2[PC_2[i]-1]<<endl;
		}
		
	}
	return key4;
}
