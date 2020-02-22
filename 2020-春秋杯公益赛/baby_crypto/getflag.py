

subkey = [ 0x1d4c40c1 , 0x4a282833 , 0x0d52377f, 0x08510b7c, 0x8bb10b11, 0x1ce6da6b]
cflag = '66a3bf441b7c16da1754294fcb9fbdac7f1e954f9262d30005cf1880b651a51a3fa8e8c5d2189085'
key = []
for i in subkey:
    keyi = []
    tmp = i >> 24
    keyi.append(tmp)
    tmp = i>>16 & 0xff
    keyi.append(tmp)
    tmp = i>>8 & 0xff
    keyi.append(tmp)
    tmp = i & 0xff
    keyi.append(tmp)
    key.append(keyi)
cflag = cflag.decode('hex')

SZ = 8

def gbox(a,b,mode):
    x = (a+b+mode)%256
    return ((x<<2)|(x>>6))&0xff

def fbox(plain):
    t0 = (plain[3] ^ plain[0])
    y1 = gbox(plain[1] ^ plain[2], t0, 1)
    y0 = gbox(plain[1], y1, 0)
    y2 = gbox(t0, y1, 0)
    y3 = gbox(plain[0], y2, 1)
    return [y3, y0, y1, y2]


def doxor(m,n):
    return map(lambda x: x[0]^x[1], zip(m, n))



def decrypt_block(pt, ks):
    l = pt[:4]
    r = pt[4:]
    for i in range(4):
        l, r = r, doxor(l, r)
        l, r = doxor(l, fbox(doxor(r, ks[3-i]))), r
    l = doxor(l, ks[4])
    r = doxor(r, ks[5])
    return l+r

def decrypt(pt, k):
    ct = ''
    for i in range(0, len(pt), SZ):
        res = decrypt_block(map(ord, pt[i:i+SZ]), k)
        ct += ''.join(map(chr, res))
    return ct

flag = decrypt(cflag, key)
print(flag)