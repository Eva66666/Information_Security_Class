#include "dsa.hpp"
using namespace std;
int main(int argc,char* argv[]) {
    std::string s = argv[1];
    if(s == "-keygen"){
	 keygen(); 
    }
    else if(s == "-sign"){
        string msg = argv[2];
    	Signature(msg);
    }
    else if(s == "-verify"){
	const string msg = argv[2];
        BIGNUM *r = BN_new();
	BIGNUM *s = BN_new();
	const string r_str = argv[3];
	const string s_str = argv[4];
	BN_dec2bn(&(r), r_str.c_str());
        BN_dec2bn(&(s), s_str.c_str());
        int verified = -1;
        verified = Verification(msg,r,s);
        switch (verified) {
        case -1:
            cout << "Error" << endl;
            break;
        case 1:
            cout << "Verified!" << endl;
            break;
        case 0:
            cout << "Unverified!" << endl;
            break;
        }
	BN_clear_free(r);
        BN_clear_free(s);
    }
    return 0;
}


