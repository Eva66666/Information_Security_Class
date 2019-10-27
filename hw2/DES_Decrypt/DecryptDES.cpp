#include <stdio.h>
#include <iostream>
#include <bitset>
#include <string>

using namespace std;
 
bitset<64> key;                // key
bitset<48> subKey[16];         // subkey
 
int IP[] = {
	58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17,  9, 1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7
};

int IP_1[] = {
	40, 8, 48, 16, 56, 24, 64, 32,
	39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30,
	37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28,
	35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26,
	33, 1, 41,  9, 49, 17, 57, 25
};

int E[] = {
	32,  1,  2,  3,  4,  5,
	 4,  5,  6,  7,  8,  9,
	 8,  9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32, 1
};
 
int PC_1[] = {
	57, 49, 41, 33, 25, 17, 9,
	1, 58, 50, 42, 34, 26, 18,
	10, 2, 59, 51, 43, 35, 27,
	19, 11, 3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
	7, 62, 54, 46, 38, 30, 22,
	14, 6, 61, 53, 45, 37, 29,
	21, 13, 5, 28, 20, 12, 4
}; 

int PC_2[] = {
	14, 17, 11, 24,  1,  5,
	 3, 28, 15,  6, 21, 10,
	23, 19, 12,  4, 26,  8,
	16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
};
 
int shift[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
 
int P[] = {
	16,  7, 20, 21, 29, 12, 28, 17,
	 1, 15, 23, 26,  5, 18, 31, 10,
	 2,  8, 24, 14, 32, 27,  3,  9,
	19, 13, 30,  6, 22, 11,  4, 25
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

//change into bits
bitset<64> toBits(const char s[18])
{
	bitset<64> bits;
		for(int i =2, n = -1; i<18;i++){
			
        if(s[i]>='0' && s[i]<='9'){
            int a = s[i]-'0';
            int k = 0;
            while(a != 0){
                bits[n+4-k]=a%2;
                a /= 2;
                k++;
            }
            n+=4;
            if(k<4){
                for(int i = 1;i<(4-k);i++){
                        bits[n+i-4]=0;
                }
            }
        }
        if(s[i]>='A' && s[i]<='F'){
            int a = s[i]-'A'+10;
            int p = 0;
            while(a != 0){
                bits[n+4-p]=a%2;
                a /= 2;
                p++;
                
            }
            n+=4;
        }
    }
    // bitset<64> bits2;
    // for(int i = 0;i<64;i++){
    // 	bits2[i]=bits[63-i];
    // }
	return bits;
}

//leftshift

bitset<28> leftShift(bitset<28> k, int s)
{
	bitset<28> tmp;
	for(int i=0; i<28; i++){
		tmp[i]=k[(i+s)%28];
	}
	return tmp;
}
 
//generate subkey
void key_schedule() 
{
	bitset<56> key2;
	bitset<28> C;	
	bitset<28> D;
	bitset<48> key3;
	// PC-1
	for (int i=0; i<56; i++)
		key2[i] = key[PC_1[i]-1];
	
	for(int key_round=0; key_round<16; key_round++) 
	{
		//left&right
		for(int i=0; i<28; i++)
			C[i] = key2[i];
		for(int i=28; i<56; i++)
			D[i-28] = key2[i];
		
		// //test
		// std::cout <<"\n"<< "test shift" << std::endl;
		// std::cout << "L"<<key_round<<":" ;
	 //   for(int i =0;i<28;i++){
	 //   	cout<<C[i];	
	 //   	/*if(i%8==0){
	 //   		std::cout << "" << std::endl;
	 //   	}*/
	 //   }
	 //   std::cout << "" << std::endl;
	 //   std::cout << "R"<<key_round<<":";
	 //   for(int i =0;i<28;i++){
	 //   	cout<<D[i];	
	 //   	/*if(i%8==0){
	 //   		std::cout << "" << std::endl;
	 //   	}*/
	 //   }
	 //   std::cout << "\ntest shift" << std::endl;	
		
		// left shift
		C = leftShift(C, shift[key_round]);
		D = leftShift(D, shift[key_round]);
		
		//PC-2
		for(int i=28; i<56; i++)
			key2[i] = D[i-28];
		for(int i=0; i<28; i++)
			key2[i] = C[i];
		for(int i=0; i<48; i++)
			key3[i] = key2[PC_2[i]-1];
		subKey[key_round] = key3;
	}
	//test
	// std::cout <<"\n"<< "test key" << std::endl;
 //   for(int i =0;i<16;i++){
 //   	cout<<"subkey"<<i<<":";
 //   	for(int j=0;j<48;j++){
    		
 //   		cout<<subKey[i][j];
 //   	}
 //   	std::cout << "" << std::endl;
 //   	/*if(i%8==0){
 //   		std::cout << "" << std::endl;
 //   	}*/
 //   }
 //   std::cout << "test key" << std::endl;
}

bitset<32> f_function(bitset<32> R, bitset<48> k)
{
	bitset<48> R1;
	for(int i=0; i<48; i++)
		R1[i] = R[E[i]-1];
	//for(int i=0; i<48; i++)
		//R1[i] = R1[i] ^ k[i];
	R1 = R1 ^ k;
	
	bitset<32> R3;
	int x = 0,t=0;
	for(int i=0; i<48; i=i+6)
	{
		int row = R1[i]*2 + R1[i+5];
		int col = R1[i+1]*8 + R1[i+2]*4 + R1[i+3]*2 + R1[i+4];
		int r = S_BOX[t][row][col];
		
		//test
		/*std::cout <<"\n"<< "test R2" << std::endl;
		for(int a =0;a<6;a++){
			cout<<R1[a+i];
		}
		std::cout << "S_BOX[t][row][col]:" <<r <<" t:"<<t<< " row:"<<row<<" col:"<<col<<endl;
	    std::cout << "test R2" << std::endl;*/
		
		bitset<4> R2;
		for(int a = 0;a<4 ;a++){
			if(r !=0){
				R2[a]=r%2;
				r /= 2;	
			}else{
				R2[a]=0;	
			}
		}
		
		for(int a = 0; a<4;a++){
			R3[x+a]=R2[3-a];
		}
		x += 4;
		t++;
	}
	//test
	/*std::cout <<"\n"<< "test output0" << std::endl;
    for(int i =1;i<=32;i++){
    	cout<<R3[i-1];
    	if(i%8==0){
    		std::cout << "" << std::endl;
    	}
    }
    std::cout << "test output0" << std::endl;*/
    
	// P
	bitset<32> R4;
	for(int i=0; i<32; i++)
		R4[i] = R3[P[i]-1];
		
	//test
	/*std::cout <<"\n"<< "test R3" << std::endl;
    for(int i =1;i<=32;i++){
    	cout<<R3[i-1];
    	if(i%8==0){
    		std::cout << "" << std::endl;
    	}
    }
    std::cout << "test R3" << std::endl;*/
	
	return R4;
}

bitset<64> decrypt(bitset<64>& cipher)
{
	bitset<64> plain;
	bitset<64> currentBits;
	bitset<32> left;
	bitset<32> right;
	bitset<32> newLeft;
	// IP
	for(int i=0; i<64; i++)
		currentBits[i] = cipher[IP[i]-1];
	// left&R
	for(int i=0; i<32;i++)
		left[i] = currentBits[i];
	for(int i=0; i<32; i++)
		right[i] = currentBits[32+i];
	// 16 rounds
	for(int round=0; round<16; round++)
	{
		newLeft = right;
		right = left ^ f_function(right,subKey[15-round]);
		left = newLeft;
	}
	// combine l&r
	for(int i=0; i<32; i++)
		plain[i] = right[i];
	for(int i=32; i<64; i++)
		plain[i] = left[i-32];
	// IP-1
	currentBits = plain;
	for(int i=0; i<64; i++)
		plain[i] = currentBits[IP_1[i]-1];

	return plain;
}

int main() {
	string c_i;
    string k_i;
    cin >> k_i >>c_i;
    
	bitset<64> cipher = toBits(c_i.c_str());
	key = toBits(k_i.c_str());
	//test
	//std::cout <<"\n"<< "test tobit" << std::endl;
    /*for(int i =1;i<=64;i++){
    	cout<<cipher[i-1];
    	if(i%8==0){
    		std::cout << "" << std::endl;
    	}
    }*/
    // std::cout << "" << std::endl;
    /*for(int i =1;i<=64;i++){
    	cout<<key[i-1];
    	if(i%8==0){
    		std::cout << "" << std::endl;
    	}
    }*/
    //std::cout << cipher << std::endl;
    // std::cout << "" << std::endl;
    // std::cout << "test tobit" << std::endl;

	key_schedule();
	bitset<64> temp_plain = decrypt(cipher);
	//test
	/*std::cout <<"\n"<< "test decrypt" << std::endl;
    for(int i =1;i<=64;i++){
    	cout<<temp_plain[i-1];
    	if(i%8==0){
    		std::cout << "" << std::endl;
    	}
    }
    std::cout << temp_plain << std::endl;
    std::cout << "test decrypt" << std::endl;*/
    char out_plain[16];
    cout<<"0x";
    for(int i =0;i<64;i=i+4){
            //k->the number of round
            int k = i/4;
            int dec=8*temp_plain[i]+4*temp_plain[i+1]+2*temp_plain[i+2]+temp_plain[i+3];
            if(dec>9){
                int w=dec-10;
                out_plain[k]='a'+w;
                cout<<out_plain[k];
            }else{
                out_plain[k]='0'+ dec;
                cout<<out_plain[k];
            }
    }
    
	return 0;
}
