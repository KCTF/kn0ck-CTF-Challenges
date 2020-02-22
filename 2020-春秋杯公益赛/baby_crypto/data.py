#!/usr/bin/env python
import os, random, sys, string
from hashlib import sha256


SZ = 8


def keyout(stats):
    res = ''
    res += '{'
    for i in range(3):
        res += '{'
        for j in range(8):
            res += '0x'+stats[i*8+j]
            if j != 7:
                res += ','
        res += '}'
        if i != 2:
            res += ','
    res += '}' + ';'
    return res


f = open('data.txt', 'r')
input = []
statsp0 = []
statsc0 = []
statsp1 = []
statsc1 = []
for a in range(3):
    for i in range(8):
        f.readline()
        tmp = f.readline()[:16]
        statsp0.append(tmp)
        tmp = f.readline()[:16]
        statsc0.append(tmp)
        f.readline()
        tmp = f.readline()[:16]
        statsp1.append(tmp)
        tmp = f.readline()[:16]
        statsc1.append(tmp)
tmp = ''
tmp += 'unsigned long long statsp0[3][8] = '
tmp += keyout(statsp0)
print(tmp)
tmp = ''
tmp += 'unsigned long long statsc0[3][8] = '
tmp += keyout(statsc0)
print(tmp)
tmp = ''
tmp += 'unsigned long long statsp1[3][8] = '
tmp += keyout(statsp1)
print(tmp)
tmp = ''
tmp += 'unsigned long long statsc1[3][8] = '
tmp += keyout(statsc1)
print(tmp)