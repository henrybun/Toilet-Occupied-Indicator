# -*- coding: utf-8 -*-
"""
Created on Sun Feb  6 19:58:05 2022

@author: Eric Kwong
"""


import numpy as np

arr = []
var = "00 11 22 33 44 55 66 77 88 99"
i = 0
f = 0
cnt = 0
for x in var:
    f = f + 1
    if (x.isspace() == True):
        arr.append(var[i:f-1])
        i = f
    if (len(var)==f):
        arr.append(var[i:f+1])