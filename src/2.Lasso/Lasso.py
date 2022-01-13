#!//usr/bin/python3


########################   
####### MODULES ########
########################

import re
import csv
from decimal import Decimal
from scipy import misc
from scipy.io import FortranFile
import sklearn
from sklearn import linear_model
from sklearn.feature_selection import chi2
from sklearn.metrics import mean_squared_error
from sklearn.metrics import mean_absolute_error
import math
from statistics import stdev, mean
import numpy as np
import glob
import os
import random
import time 
import subprocess
import sys

inputArgs=sys.argv
list_itype=[]
str_type=""
for i in np.arange(len(inputArgs))[1:]:
        list_itype=np.append(list_itype,sys.argv[i])
        str_type=str_type+str(sys.argv[i])
print (list_itype)
print (str_type)




##########################   
####### FUNCTIONS ########
##########################
 
#### Ljfunction ####
def Lj(r,E,sigma):
    return 4.0*E*((r/sigma)**(-12.0) - (r/sigma)**(-6.0))

#### GaussianFunction ####
def gaussian(r,p,q):
    return np.exp(-p*(r-q)*(r-q))
#### CUTOFF ####
def fcut(r,rcut):
    return 0.5*(1+np.cos(np.pi*(r/rcut))) 

#### read functions  #####
def getN_atoms(input_file):
        return  sum(1 for line in open(input_file))

def readForces(input_file):
    tmp=np.genfromtxt(input_file,invalid_raise=False, usecols=[0,1,2],dtype=float)
    return(tmp)

def readFile(input_file):
        input_lmp=input_file
        print (input_file)
        Y=readForces(input_lmp)
        Y=Y.reshape(1, 3*np.shape(Y)[0])
        Y=Y[0]
        
        
        N_atoms=getN_atoms(input_lmp)
        
        list_bin=[]
        for i_type in list_itype: 
                search=input_file[:-6]+"poscar_f"+str(i_type)+"_*bin"        # changing poscar fomat
                search=search.replace("Input","Input_"+str(i_type))
                print (search)
                list_bin=np.append(list_bin,sorted(glob.glob(search)))
        
        print(list_bin) 
        
        
        XMAT=np.genfromtxt(list_bin[0])
        XMAT=np.transpose(XMAT)
        
        for input_bin in list_bin[1:]: 
                XMAT_tmp=np.genfromtxt(input_bin)
                XMAT_tmp=np.transpose(XMAT_tmp)
                XMAT=np.concatenate((XMAT,XMAT_tmp),axis=1)
    
        return(XMAT,Y) 

#### Trace Curve ####
def computeV(r,list_w,list_r0,coeff,i_start):
    i=i_start
    gtot= np.zeros(r.size) 
    for k in range(list_w.size):
        for j in range(list_r0.size):
            gtot=gtot+coeff[i]*gaussian(r,list_w[k],list_r0[j])
            i=i+1
    return gtot




###############################   
####### LASSO PROCESS #########
###############################
N_sample=100


i_select=1
#### Data reading #### 
search="Input/*forces"
input_file=sorted(glob.glob(search))[0]
XMAT,Yin=readFile(input_file)
print(XMAT.shape, Yin.shape)

for input_file in sorted(glob.glob(search))[1:]: 
        XMAT_tmp,Yin_tmp=readFile(input_file)
        XMAT=np.concatenate((XMAT,XMAT_tmp),axis=0)
        Yin=np.append(Yin,Yin_tmp)
        if (np.shape(XMAT)[0]!=np.shape(Yin)[0]):
                print ("WARNING!!! ",input_file, np.shape(XMAT)[0],np.shape(Yin)[0])        

filter_=abs(Yin)<15
XMAT=XMAT[filter_]
Yin=Yin[filter_]
    
## Split data in Train/Test
N_sample=np.shape(Yin)[0]
list_Train=random.sample(range(N_sample),int(0.95*N_sample))
list_Test=np.delete(np.arange(N_sample),list_Train)
XMAT_Train=XMAT[list_Train]
Y_Train=Yin[list_Train]
XMAT_Test=XMAT[list_Test]
Y_Test=Yin[list_Test]

with open("Out_"+str_type+".log",'w') as fw0:    
        for exponent in np.arange(4.0,8.0)[::-1]:
                for number in np.arange(1.0,10.0)[::5]:
                        alpha=pow(10,-exponent)*number
                        alpha=round(alpha,int(exponent))
                        str_alpha=str('%.0e' % Decimal(alpha))
                        # Fit
                        clf =linear_model.LassoLars(alpha=alpha)
                        clf.fit(XMAT_Train, Y_Train)
                        coeff=np.copy(clf.coef_)
                    
                        # Score/RMSE/MAS
                        Y_OUT_Train=clf.predict(XMAT_Train)
                        Y_OUT_Test=clf.predict(XMAT_Test)
                        scoreTrain=clf.score(XMAT_Train, Y_Train)
                        scoreTest=clf.score(XMAT_Test, Y_Test)
                        RMSE_Train=np.sqrt(mean_squared_error(Y_OUT_Train,Y_Train))
                        RMSE_Test=np.sqrt(mean_squared_error(Y_OUT_Test,Y_Test))
                        MAS_Train=mean_absolute_error(Y_OUT_Train,Y_Train)
                        MAS_Test=mean_absolute_error(Y_OUT_Test,Y_Test)
                        
                        print (alpha, scoreTrain,scoreTest,RMSE_Train,RMSE_Test,MAS_Train,MAS_Test, sum(abs(coeff)>1E-10), np.size(coeff),np.mean(abs(Y_Train))) 
                    
                        output_file="Coeff"+str_alpha+"_"+str_type+".txt"
                        with open(output_file,'w') as fw:    
                            for i in np.arange(np.size(coeff)):
                                fw.write("%g %g\n"%(i,coeff[i]))
                    
                        output_file="ResultsTest"+str_alpha+"_"+str_type+".txt"
                        with open(output_file,'w') as fw:    
                            for i in np.arange(np.size(Y_Test)):
                                fw.write("%g %g %g \n"%(i,Y_Test[i],Y_OUT_Test[i]))
                        
                        output_file="ResultsTrain"+str_alpha+"_"+str_type+".txt"
                        with open(output_file,'w') as fw:    
                            for i in np.arange(np.size(Y_Train)):
                                fw.write("%g %g %g \n"%(i,Y_Train[i],Y_OUT_Train[i]))
                    
                        fw0.write("%g %g %g %g %g %g %g %g %g %g  \n"%(   alpha, scoreTrain,scoreTest,RMSE_Train,RMSE_Test,MAS_Train,MAS_Test, sum(abs(coeff)>1E-10), np.size(coeff),np.mean(abs(Y_Train))) )
                                
