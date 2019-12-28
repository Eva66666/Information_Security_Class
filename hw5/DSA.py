import sys
import random
import hashlib
import math

#change string into number
def str_to_num(str):
    list_num = []
    for i in range(0,len(str)):
        list_num.append(ord(str[i])) 
    # num = np.fromstring(str, dtype=np.uint8)
    return list_num

# change input into int
def get_input(message):
    choice = int(input(message))
    return choice

# Compute modular with Square-and-Multiply
def sq_and_mul(a, e, n):
    result = 1
    while e:
        if e & 1:
            result = result * a % n
        e >>= 1
        a = a * a % n
    return result

#compute inverse with extended Euclid
def ext_euclid(a,m):
    if gcd(a,m)!=1:
        return None
    u1,u2,u3 = 1,0,a
    v1,v2,v3 = 0,1,m
    while v3!=0:
        q = u3//v3
        v1,v2,v3,u1,u2,u3 = (u1-q*v1),(u2-q*v2),(u3-q*v3),v1,v2,v3
    return u1%m


def gcd(a,b):
    if a > b:    
        while(b):     
            a, b = b, a % b
        return a
    else:
        while(a):
            b , a = a , b % a
        return b

# Miller-Rabin test to varify if composite
def miller_rabin_test(n):
    rounds = 20
    if n == 2 or n == 3:
        return False
    if n < 2 or n % 2 == 0:
        return True
    r, s = 0, n - 1
    while s % 2 == 0:
        r += 1
        s //= 2
    for _ in range(rounds):
        a = random.randrange(2, n - 1)
        x = sq_and_mul(a, s, n)
        if x == 1 or x == n - 1:
            continue
        for _ in range(r - 1):
            x = sq_and_mul(x, 2, n)
            if x == n - 1:
                break
        else:
            return True
    return False

def generate_prime(n):
    if n < 2:
        return None
    while True:
        number = random.randrange(pow(2, n - 1) + 1, pow(2, n), 2)
        if not miller_rabin_test(number):
            return number


def sha_hash(m):
    hasher = hashlib.sha1(m.encode("utf-8"))
    hex = "0x"+hasher.hexdigest()
    return int(hex,0)

def key_generation():
    loop = True
    bit = get_input('Please insert the bits number of the prime: ')
    print('generating keys,please wait a moment...')
    while loop:
        k=random.randrange(2**(863), 2**(864))
        q=generate_prime(bit)
        p=(k*q)+1
        L = p.bit_length()
        t = random.randint(1,p-1)
        g = sq_and_mul(t,(p-1)//q,p)
        if(L>=1023 and L<=1024 and L%64 == 0 and (gcd(p-1,q)) > 1 and sq_and_mul(g,q,p) == 1):
            
            loop = False
            a = random.randint(2,q-1)
            h = sq_and_mul(g,a,p)
            print('Finish!')
            print('public key:')
            print("p = ",p)
            print("q = ",q)
            print("g = ",g)
            print("h = ",h)
            print("\nprivate key:")
            print("a = ",a)
            file1 = open("public_key.txt","w")
            file1.truncate()
            file1.write(str(p))
            file1.write("\n")
            file1.write(str(q))
            file1.write("\n")
            file1.write(str(g))
            file1.write("\n")
            file1.write(str(h))
            file1.close()
            file2 = open("private_key.txt","w")
            file2.truncate()
            file2.write(str(a))
            file2.close()
            print("\nkeys have been stored at public_key.txt and private_key.txt")
            
	
def sign():
    # print('Reading key...')
    file1 = open("public_key.txt","r")
    file2 = open("private_key.txt","r")
    p = int(file1.readline().rstrip())
    q = int(file1.readline().rstrip())
    g = int(file1.readline().rstrip())
    h = int(file1.readline().rstrip())
    a = int(file2.readline().rstrip())
    print('Finish reading key!\n')
    # p = get_input('please insert the p:')
    # q = get_input('please insert the q:')
    # g = get_input('please insert the g:')
    # a = get_input('please insert the a:')
    m = input('please insert the message:')
    loop = True
    while loop:
        r = random.randint(1,q-1)
        c1 = (sq_and_mul(g,r,p))%q
        c2 = sha_hash(m) + (a*c1)
        r_inverse = ext_euclid(r,q)
        c2 = (c2*r_inverse)%q
        if(c1 != 0 and c2 != 0):
            loop = False
    print('\nsignature has finished!')
    print('message:',m)
    print('c1:',c1)
    print('c2:',c2)
    # file = open("signature.txt","w")
    # file.write(str(c1))
    # file.write("\n")
    # file.write(str(c2))
    # print("cipher has been stored at signature.txt")


def varify():
    m = input('please insert the message:')
    # p = get_input('please insert the p:')
    # q = get_input('please insert the q:')
    # g = get_input('please insert the g:')
    # h = get_input('please insert the h:')
    # print('\nReading key...')
    file1 = open("public_key.txt","r")
    p = int(file1.readline().rstrip())
    q = int(file1.readline().rstrip())
    g = int(file1.readline().rstrip())
    h = int(file1.readline().rstrip())
    print('Finish reading key!')
    c1 = get_input('please insert the c1:')
    c2 = get_input('please insert the c2:')
    t1 = sha_hash(m)
    c2_inverse = ext_euclid(c2,q)
    t1 = (t1*c2_inverse)%q
    t2 = ext_euclid(c2,q)
    t2 = (t2*c1)%q
    v1 = sq_and_mul(g,t1,p)
    v2 = sq_and_mul(h,t2,p)
    v = ((v1*v2)%p)%q
    if(v == c1):
        print("\n------Result: Valid signature!------\n\n")
    else:
        print("\n------Result: Invalid signature!------\n\n")

def main():
    a = ""
    print("\n------------ DSA ------------")
    # get input
    
    while a != "4":
        # get input
        print("\n1. Generate the key")
        print("2. DSA Sign")
        print("3. DSA Verify.")
        print("4. Exit.")
        a = input("\nplease input your choice with number:")
        if a == "1":
            key_generation()
        elif a=="2":
            sign()
        elif a=="3":
            varify()
        elif a=="4":
            print("exit!")
        else:
            print("please input the number with the legal form！")

if __name__ == '__main__':
    main()
        

