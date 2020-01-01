#ifndef DSA_HPP
#define DSA_HPP
#include <iostream>
#include <string>
#include <openssl/bn.h>
#include <openssl/sha.h>
#include<fstream>

//打印生成的参数并且写入文件
int PrintMyDSAInfo(BIGNUM *p,BIGNUM *q,BIGNUM *g,BIGNUM *x,BIGNUM *y) {
    char *prt;
    prt = BN_bn2dec(p);
    std::cout << "p: " << prt << std::endl;
    std::ofstream key_file;
    key_file.open("./key.txt");
    for(int i=0; prt[i] != 0;i++){
        key_file << prt[i];   
    }
    key_file << std::endl;
    prt = BN_bn2dec(q);
    std::cout << "q: " << prt << std::endl;
    for(int i=0; prt[i] != 0;i++){
        key_file << prt[i];   
    }
    key_file << std::endl;
    prt = BN_bn2dec(g);
    std::cout << "g: " << prt << std::endl;
    for(int i=0; prt[i] != 0;i++){
        key_file << prt[i];   
    }
    key_file << std::endl;
    prt = BN_bn2dec(x);
    std::cout << "x: " << prt << std::endl;
    for(int i=0; prt[i] != 0;i++){
        key_file << prt[i];   
    }
    key_file << std::endl;
    prt = BN_bn2dec(y);
    std::cout << "y: " << prt << std::endl;
    for(int i=0; prt[i] != 0;i++){
        key_file << prt[i];   
    }
    key_file << std::endl;
    return 1;
}
//生成参数
void keygen() {
    // allocate big numbers
    BIGNUM *p = BN_new();
    BIGNUM *q = BN_new();
    BIGNUM *g = BN_new();
    BIGNUM *y = BN_new();
    BIGNUM *x = BN_new();
    BIGNUM *h = BN_new();

    BN_CTX *ctx = BN_CTX_new();

    BIGNUM *one = BN_new();
    BN_one(one);

    // 生成 p, q, g, x, y。
    // p 和 q 之间的关系比较特殊，需要满足 p % q == 1 。且 p 和 q 都为素数。
    // 其实这个要自己生成时有点复杂的，好在 OpenSSL 提供了这个功能。
    // q
    BN_generate_prime_ex(q, 160, true, NULL, NULL, NULL);
    // p
    BN_generate_prime_ex(p, 1024, false, q, one, NULL);

    // g = h^((p-1)/q) mod p
    BIGNUM *p_minus_1 = BN_new();
    BN_sub(p_minus_1, p, one);
    do {
        BN_rand_range(h, p_minus_1);
    } while (BN_is_zero(h) || BN_is_one(h));

    BIGNUM *p_minus_1_div_q = BN_new();
    BN_div(p_minus_1_div_q, NULL, p_minus_1, q, ctx);

    BN_mod_exp(g, h, p_minus_1_div_q, p, ctx);

    // x
    do {
        BN_rand_range(x, q);
    } while (BN_is_zero(x));

    // y = g^x mod p
    BN_mod_exp(y, g, x, p, ctx);

    //打印并写入
    PrintMyDSAInfo(p,q,g,x,y); 
    // Clear and free temporary BIGNUM instances
    BN_clear_free(one);
    BN_clear_free(p_minus_1);
    BN_clear_free(p_minus_1_div_q);
    BN_clear_free(p);
    BN_clear_free(q);
    BN_clear_free(g);
    BN_clear_free(y);
    BN_clear_free(x);
    BN_clear_free(h);    
    BN_CTX_free(ctx);   
}
//签名
void Signature(const std::string msg) {
     // allocate big numbers
    BIGNUM *p = BN_new();
    BIGNUM *q = BN_new();
    BIGNUM *g = BN_new();
    BIGNUM *y = BN_new();
    BIGNUM *x = BN_new();
    BN_CTX *ctx = BN_CTX_new();

    std::ifstream key_file;
    key_file.open("./key.txt");

    std::string p_str;
    getline(key_file,p_str);
    BN_dec2bn(&(p), p_str.c_str());
    //std::cout << p_str << std::endl;

    std::string q_str;
    getline(key_file,q_str);
    BN_dec2bn(&(q), q_str.c_str());
    //std::cout << q_str << std::endl;

    std::string g_str;
    getline(key_file,g_str);
    BN_dec2bn(&(g), g_str.c_str());
    //std::cout << g_str << std::endl;

    std::string x_str;
    getline(key_file,x_str);
    BN_dec2bn(&(x), x_str.c_str());
    //std::cout << x_str << std::endl;
   
    std::string y_str;
    getline(key_file,y_str);
    BN_dec2bn(&(y), y_str.c_str());
    //std::cout << y_str << std::endl;
    // k
    BIGNUM *k = BN_new();
    do {
        BN_rand_range(k, q);
    } while(BN_is_zero(k));

    // r = (g^k mod p) mod q
    BIGNUM *r = BN_new();
    BIGNUM *g_k_mod_p = BN_new();
    BN_mod_exp(g_k_mod_p, g, k, p, ctx);
    BN_mod(r, g_k_mod_p, q, ctx);

    // s = (k^-1 (H(m) + xr)) mod q
    BIGNUM *s = BN_new();
    BIGNUM *k_inverse = BN_new();
    BN_mod_inverse(k_inverse, k, q, ctx);
    // H(m)
    BIGNUM *digest = BN_new();
    const char* msg_char = msg.c_str();
    // 消息使用 SHA1 生成散列码
    int length = msg.length();
    unsigned char *digest_char = SHA1((const unsigned char*)msg_char, length, NULL); 
    BN_bin2bn(digest_char, length, digest);

    char *pt_digest = BN_bn2dec(digest);
    std::cout << "SHA1 Digest: " << pt_digest << std::endl;

        // xr
    BIGNUM *xr = BN_new();
    BN_mul(xr, x, r, ctx);
        // H(m) + xr
    BN_add(digest, digest, xr);

    BN_mod_mul(s, k_inverse, digest, q, ctx);

    char *prt;
    prt = BN_bn2dec(r);
    std::cout << "r: " << prt << std::endl;
    prt = BN_bn2dec(s);
    std::cout << "s: " << prt << std::endl;

    // Clear and free all the BIGNUM instance
    BN_clear_free(k);
    BN_clear_free(r);
    BN_clear_free(g_k_mod_p);
    BN_clear_free(s);
    BN_clear_free(k_inverse);
    BN_clear_free(digest);
    BN_clear_free(xr);
    BN_clear_free(p);
    BN_clear_free(q);
    BN_clear_free(g);
    BN_clear_free(y);
    BN_clear_free(x);
    BN_CTX_free(ctx);

}
//验证
int Verification(const std::string msg,BIGNUM *r ,BIGNUM *s ) {
     // allocate big numbers
    BIGNUM *p = BN_new();
    BIGNUM *q = BN_new();
    BIGNUM *g = BN_new();
    BIGNUM *y = BN_new();
    BIGNUM *x = BN_new();
    BN_CTX *ctx = BN_CTX_new();

    std::ifstream key_file;
    key_file.open("./key.txt");

    std::string p_str;
    getline(key_file,p_str);
    BN_dec2bn(&(p), p_str.c_str());
    //std::cout << p_str << std::endl;

    std::string q_str;
    getline(key_file,q_str);
    BN_dec2bn(&(q), q_str.c_str());
    //std::cout << q_str << std::endl;

    std::string g_str;
    getline(key_file,g_str);
    BN_dec2bn(&(g), g_str.c_str());
    //std::cout << g_str << std::endl;

    std::string x_str;
    getline(key_file,x_str);
    BN_dec2bn(&(x), x_str.c_str());
    //std::cout << x_str << std::endl;
   
    std::string y_str;
    getline(key_file,y_str);
    BN_dec2bn(&(y), y_str.c_str());
    //std::cout << y_str << std::endl;

    BIGNUM *omega = BN_new();
    BIGNUM *u1 = BN_new();
    BIGNUM *u2 = BN_new();

    // omega = s^-1 mod q
    BIGNUM *s_inverse = BN_new();
    BN_mod_inverse(s_inverse, s, q, ctx);

    BN_mod(omega, s_inverse, q, ctx);

    // u1 = (H(m) omega) mod q
    // H(m)
    BIGNUM *digest = BN_new();
    const char* msg_char = msg.c_str();
    int length = msg.length();
    unsigned char *digest_char = SHA1((const unsigned char*)msg_char, length, NULL); 
    BN_bin2bn(digest_char, length, digest);

    BN_mod_mul(u1, digest, omega, q, ctx);

    // u2 = r omega mod q
    BN_mod_mul(u2, r, omega, q, ctx);

    // compare
    BIGNUM *re_cal = BN_new();
    
    BIGNUM *g_u1 = BN_new();
    BIGNUM *y_u2 = BN_new();
    // 求 g^u1 和 y^u2，直接 exp 量级过于庞大（L**160 的量级），因此先 mod p 缩小量级再进行计算，否则无法在多项式时间内得出结果。
    BN_mod_exp(g_u1, g, u1, p, ctx);
    BN_mod_exp(y_u2, y, u2, p, ctx);

    BIGNUM *gu1_mul_yu2_mod_p = BN_new();
    BN_mod_mul(gu1_mul_yu2_mod_p, g_u1, y_u2, p, ctx);
    
    BN_mod(re_cal, gu1_mul_yu2_mod_p, q, ctx);
    
    int result = -1;
    // 比较计算结果和信息中所带签名信息，若相同即为认证。
    if (BN_cmp(re_cal, r) == 0) {
        result = 1;
    } else {
        result = 0;
    }
    // clear and free all the BIGNUM instance
    BN_clear_free(omega);
    BN_clear_free(u1);
    BN_clear_free(u2);
    BN_clear_free(s_inverse);
    BN_clear_free(digest);
    BN_clear_free(g_u1);
    BN_clear_free(y_u2);
    BN_clear_free(gu1_mul_yu2_mod_p);
    BN_clear_free(re_cal);

    BN_clear_free(p);
    BN_clear_free(q);
    BN_clear_free(g);
    BN_clear_free(y);
    BN_clear_free(x);
    BN_CTX_free(ctx);
    return result;

}
#endif
