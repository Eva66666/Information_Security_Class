from Crypto.Cipher import AES
#from Crypto import Random
from PIL import Image
import binascii

key = 'this is keykeyke'.encode('ASCII')
iv = 'This is an IV123'.encode('ASCII')


def xor(x,y):
    ans=""
    for i in range(0,128):
        if ((x[i] == '0' and y[i] == '1') or (x[i] == '1' and y[i] == '0')):
            ans += "1" 
        else: 
            ans += "0" 
    ##print(ans)
    return ans
    #return format(int(x, 16) ^ int(y, 16), '032x')

def byte_to_bit(s):
    ans = ""
    #print("leng:",len(s))
    for i in range(0,len(s)):
        ##print(s[i])
        ss = bin(s[i]).replace('0b', '')
        sss = ss.zfill(8)
        ans += sss
    return ans

def bit_to_byte(s):
    ans = b""
   # #print(len(s))
    for i in range(0,len(s),8):
        ss = s[i:i+8]
        sss = int(ss,2)
        #print("sss:",sss)
        ssss = str(hex(sss)).replace('0x', '')
        if len(ssss)<2:
            ssss="0"+ssss
        #print("ssss:",ssss)
        sssss =bytes.fromhex(ssss)
        #print("sssss:",sssss)
        ans+=sssss
    ##print(ans)
    return ans

def shift_key(t,k):
    ans = b""
    kk = ''
    global shift_k
    for i in range(0,len(k),8):
        k1 = k[i:i+8]
        for j in range(0,8):
            kk1 = (t+shift_k)%8
            kk += k1[kk1]
        k2 = int(kk,2)
        k3 = str(hex(k2)).replace('0x', '')
        if len(k3)<2:
            k3="0"+k3
        k4 =bytes.fromhex(k3)
        ans+=k4
    return ans

def unpad(s):
    """
    Remove trailing zeros"""
    return s.rstrip(b"\0")

# get image
input_file = open("./DBU_image.ppm", 'rb')
input_data = input_file.read()
input_file.close()

# get ppm header: {P6}\n {width} {height} \n{color} \n
splits = input_data.split(b"\n", 3)
data_for_decryption = splits[3]
style = splits[0]
size = splits[1]
color = splits[2]


# mode3: get shift number
key0 = xor(byte_to_bit(key),byte_to_bit(iv))
shift_k0 = ""
for i in range(0,128,16):
    shift_k0 += key0[i]

shift_k = int(shift_k0,2)%8

# decryt with DBU
THIS_MODE = AES.MODE_ECB
decrypt_data_dbu = b""
out_dbu = b""
after_xor = b""
for i in range(0,len(data_for_decryption),16):
    ciphertext = data_for_decryption[i:i+16]
    #print(plaintext)
    the_key = shift_key(i,byte_to_bit(key))
    out_dbu = after_xor
    k_bit = byte_to_bit(the_key)
    c_bit = byte_to_bit(ciphertext)
    after_xor = bit_to_byte(xor(k_bit,c_bit))
    if i == 0:
        dbu_cipher = AES.new(iv, THIS_MODE)
        out_dbu = dbu_cipher.decrypt(after_xor)
    else:
        dbu_cipher = AES.new(out_dbu, THIS_MODE)
        out_dbu = dbu_cipher.decrypt(after_xor)
    decrypt_data_dbu += out_dbu


# save file after adding ppm_header P6}\n {width} {height} \n{color} \n
output_file = open("./dbu_decrypt.ppm", "wb")
output_file.write(b"\n".join([style, size, color, unpad(decrypt_data_dbu)]))
output_file.close()
im = Image.open('./DBU_image2.ppm')
im.save('./DBU_image2.jpg')
im.show()
