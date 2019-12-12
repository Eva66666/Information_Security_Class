import math
import random

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


# Compute the GCD with extended euclidean algorithm
def ext_euclid(x, y):
    x, y = abs(x), abs(y)
    a1, a2, b1, b2 = 1, 0, 0, 1
    while y != 0:
        q, x, y = x // y, y, x % y
        a1, a2 = a2, a1 - q * a2
        b1, b2 = b2, b1 - q * b2
    return x, a1, b1

#  CRT
def crt_function(p, q, d):
    _, q_inverse, _ = ext_euclid(q, p)
    return d % (p - 1), d % (q - 1), q_inverse % p

# Compute modular with Square-and-Multiply
def sq_and_mul(a, e, n):
    result = 1
    while e:
        if e & 1:
            result = result * a % n
        e >>= 1
        a = a * a % n
    return result

# Miller-Rabin test to varify if composite
def miller_rabin_test(n, rounds=40):
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


# Generate a random n-bit prime number
def generate_prime(n, rounds=40):
    if n < 2:
        return None
    while True:
        number = random.randrange(pow(2, n - 1) + 1, pow(2, n), 2)
        if not miller_rabin_test(number, rounds):
            return number


# Generates public and private key 
def rsa_keys(n, rounds=40):
    p = q = 0
    k1 = int(math.ceil(n / 2))
    k2 = int(math.floor(n / 2))
    if n < 5:
        return None
    while p == q:
        p = generate_prime(k1, rounds)
        q = generate_prime(k2, rounds)
    if q > p:
        p, q = q, p
    n = p * q
    phi = (p - 1) * (q - 1)
    while True:
        e = random.randrange(2, phi // 2)
        g, d, _ = ext_euclid(e, phi)
        if g == 1:
            public_key = (e, n)
            d = d % phi
            if d < 0:
                d += phi
            private_key = (p, q, d)
            return public_key, private_key


# encryption
def encryption(m, public_key):
    return sq_and_mul(m, public_key[0], public_key[1])


# decryption with CRT
def decryption(c, private_key):
    m1 = sq_and_mul(c, private_key[2], private_key[0])
    m2 = sq_and_mul(c, private_key[3], private_key[1])
    h = (private_key[4] * (m1 - m2)) % private_key[0]
    return m2 + h * private_key[1]

def generate_key():
    n = get_input("\n Input bits of n (in number): ")
    rounds = get_input("Input the number of rounds for Miller-Rabin: ")
    public_key, private_key = rsa_keys(n, rounds)
    print("\nPublic Key (e, n):", public_key)
    print("Private Key (p, q, d ):", private_key, "\n")

def start_encryption():
    m = str_to_num(input("\n Input plaintext m: "))
    e = get_input("Input e of the public key: ")
    n = get_input("Input n of the public key: ")
    
    # print("\nGenerated ciphertext c:")
    # for i in range(0,len(m)):
    #     #print(m[i])
    #     print(encryption(m[i], (e, n))," ")
    list_c = []
    for i in range(0,len(m)):
        #print(m[i])
        list_c.append(encryption(m[i], (e, n)))
    print("\nGenerated ciphertext c:",list_c , "\n")

def start_decryption():
    result = ""
    p = get_input("\nInput the first prime number p: ")
    q = get_input("Input the second prime number q: ")
    d = get_input("Input d of the private key: ")
    ciphertext = input("Input the ciphertext: ")
    c_num = [int(n) for n in ciphertext.split(", ")]
    dp, dq, q_inverse = crt_function(p, q, d)
    crt_private_key = (p, q, dp, dq, q_inverse)
    print("\nGenerated plaintext m:")
    for i in range(0,len(c_num)):
        #print(c_num[i])
        m_num = decryption(c_num[i], crt_private_key)
        #print(m_num)
        result += chr(m_num)
    print(result)

def main():
    a = ""
    print("\n------------ RSA ------------")
    print("1. Generate the key")
    print("2. RSA Encryption")
    print("3. RSA Decryption.")
    print("4. Exit.")
    # get input
    
    while a != "4":
        # get input
        a = input("\n\nplease input your choice with number:")
        if a == "1":
            generate_key()
        elif a=="2":
            start_encryption()
        elif a=="3":
            start_decryption()
        elif a=="4":
            print("exit!")
        else:
            print("please input the number with the legal form！")

if __name__ == '__main__':
    main()
