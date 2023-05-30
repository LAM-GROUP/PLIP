#!//usr/bin/python3


import numpy as np
import math
import sys
import os

from atsim.potentials import Potential
from atsim.potentials import EAMPotential
from atsim.potentials import writeSetFLFinnisSinclair

##i_type   m  i_a  i_b   coeffS[i_func]
tmp=[float(i) for i in sys.argv[1:]]
(i_selected,pot_type,m,p,q,coeff,cut,i_nature,rshort)=tmp
pot_type=int(pot_type)
i_selected=int(i_selected)
i_nature=int(i_nature)

r1=0.95*rshort
r2=1.05*rshort

def fcut(r):
    return 0.5*(1+np.cos(np.pi*(r/cut)))

def fshort(r):
    if (r>r2):
        phi=1
    elif (r<r1):
        phi=0
    else:
        u=(r-r1)/(r2-r1)
        phi=6.0*u*u*u*u*u-15.0*u*u*u*u+10.0*u*u*u
    return phi


def gaussian(r,p,q):
    return np.exp(-p*(r-q)*(r-q))

def STO(r,p,q):
    if (r!=0):
        if (p!=0):
            tmp=r**p*np.exp(-q*r)
            return tmp 
        else:
            tmp=np.exp(-q*r)
            return tmp
    else:
        return 1E10


def GTO(r,p,q):
    if (r!=0):
        if (p!=0):
                return r**p*np.exp(-q*r*r)
        else:
                return np.exp(-q*r*r)
    else:
        return 1E10

def POW(r,p,q):        
        if (p>1): 
                return r**p
        else:    
                return 1

def Lorentz(r,p,q):
    return 1/((r-p)**q+1)

def Lognormal(r,p,q):
    return np.exp(-np.log((r-p)/q)*np.log((r-p)/q)) 






def doCalc(r,p,q,i_nature):
    if (i_nature==1):
            return gaussian(r,p,q)*fshort(r)*fcut(r) 
    if (i_nature==4):
            return STO(r,p,q)*fshort(r)*fcut(r)#/STO(2,p,q)
    if (i_nature==5):
            return POW(r,p,q)*fshort(r)*fcut(r)
    if (i_nature==6):
            return GTO(r,p,q)*fshort(r)*fcut(r)#/GTO(2,p,q)
    if (i_nature==7):
            return Lorentz(r,p,q)*fshort(r)*fcut(r)
    if (i_nature==8):
            return Lognormal(r,p,q)*fshort(r)*fcut(r)

    


#Fe
def FeEmbedFunction(rho):
    if (pot_type==1 or pot_type ==2):
        return coeff*rho**m
    else:
        return 0

def FeFeDensityFunction(r):
    if (pot_type==1):
        return doCalc(r,p,q,i_nature) 
    else:
        return 0

def FeAuDensityFunction(r):
    if (pot_type ==2):
        return doCalc(r,p,q,i_nature) 
    else:
        return 0


#Au
def AuEmbedFunction(rho):
    if (pot_type==3 or pot_type ==4):
        return coeff*rho**m
    else:
        return 0

def AuAuDensityFunction(r):
    if (pot_type==3  ):
        return doCalc(r,p,q,i_nature)
    else:
        return 0

def AuFeDensityFunction(r):
    if (pot_type==4):
        return doCalc(r,p,q,i_nature) 
    else:
        return 0



#Pair potentials
def ppfuncFeFe(r):
  return 0 

def ppfuncFeAu(r):
  return 0 

def ppfuncAuAu(r):
  return 0 

def main():
  # Define list of pair potentials
  pairPotentials = [
    Potential('Fe', 'Fe', ppfuncFeFe),
    Potential('Fe', 'Au', ppfuncFeAu),
    Potential('Au', 'Au', ppfuncAuAu)]

  # Assemble the EAMPotential objects
  eamPotentials = [
    #Fe
    EAMPotential('Fe', 26, 55.845, FeEmbedFunction,
      { 'Fe' : FeFeDensityFunction,
        'Au' : AuFeDensityFunction },
      latticeConstant = 2.855312, 
      latticeType = 'bcc'),
    #Au
    EAMPotential('Au', 79, 196.97, AuEmbedFunction,
      { 'Fe': FeAuDensityFunction,
        'Au' : AuAuDensityFunction},
      latticeConstant = 4.068,
      latticeType = 'fcc') ]
  # Number of grid points and cut-offs for tabulation.
  nrho = 10000
  nr   = 10000
  dr   = 6.0000000000000E-4
  drho = 6.0000000000000E-4

  output_file="LassoLars_"+str(i_selected)+"_"+str(i_nature)+".eam.fs"
  with open(output_file, "wb") as outfile:
    writeSetFLFinnisSinclair(
      nrho, drho,
      nr, dr,
      eamPotentials,
      pairPotentials,
      outfile)
  os.system("sed -i '5s/$/  "+str(int(m))+"  "+str(coeff)+"/' "+output_file)

#  output_file="LassoLars_"+str(i_selected)+"_"+str(i_nature)+".BIS"
#  with open(output_file,'w') as fw:
#    for i in np.arange(0,nr,1):
#        fw.write("%g %g  \n"%(dr*i,doCalc(dr*i,p,q,i_nature)))


if __name__ == '__main__':
    main()

