#!/usr/bin/python3.6

import cProfile
import numpy as np

def startA (n) :
    
    A = np.zeros((n,n))
    A.fill(10)
    A[0] = 100
    A[1] = 100
    
    Anew = A.copy()
    
    return A, Anew

def makeMV(Anew, A):
	Anew[1:-1,1:-1] = 0.25*(A[:-2,1:-1] + A[2:,1:-1] + A[1:-1,:-2] + A[1:-1,2:])

def error(A,Anew):
	return np.max(np.abs(Anew[1:-1,1:-1]) - np.abs(A[1:-1,1:-1]))

def copy(A, Anew):
	A = Anew.copy()

pr = cProfile.Profile()
pr.enable()

n = 100

A, Anew = startA(n)

for i in range(1000):
	makeMV(Anew, A)

	err = error(A, Anew)

# print(Anew)
	A = Anew.copy()

	if i%100 ==0: print(err)

pr.disable()
pr.print_stats(sort='time')


