from Crypto.Cipher import AES
from Crypto.Util.number import long_to_bytes
from Crypto.Util.Padding import unpad

p = 
g = 
u = 
v = 
b = 
s = pow(u, b, p)
s_in_bytes = long_to_bytes(s)
aes_key = s_in_bytes[:16]

iv = bytes.fromhex("")
encrypted_password=bytes.fromhex("")
cipher = AES.new(aes_key, AES.MODE_CBC, iv)
decrypted_password = cipher.decrypt(encrypted_password)
print(decrypted_password, len(iv))