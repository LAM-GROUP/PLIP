#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <fstream>
#include <iostream> 
#include <random>
#include <vector>
#include <fstream>
#include <iostream>
#include <random>
#include <cmath>
#include <math.h>
#include "mod_fonction.h"

using namespace std;

//Basic functions
string whichPot(int i_nature)
{
        if (i_nature==1)
        	return "Gaus";
        if (i_nature==2)
        	return "LJ";
	if (i_nature==3)
        	return "ExpSW";
	if (i_nature==4)
        	return "STO";
	if (i_nature==5)
        	return "POW";
        if (i_nature==6)
                return "GTO";
        if (i_nature==7)
                return "Lorentz";
        if (i_nature==8)
                return "ALogNormal";
}


//double fcut(double rcut, double r)
//{
//	double tmp=r/rcut;
//	return 2*tmp*tmp*tmp-3*tmp*tmp+1;
//}
//double fcutD(double rcut, double r)
//{
//	double tmp=r/rcut;
//	return 6*tmp*tmp-6*tmp;
//
//}


//double fcut(double rcut, double r)
//{
//	return -1.49039973e-04*r*r*r*r*r+2.23553448e-03*r*r*r*r-1.51078107e-03*r*r*r-6.68806489e-02*r*r-7.41094448e-04*r+1.00007838e+00;
//}
//double fcutD(double rcut, double r)
//{
//	return -0.00073217*r*r*r*r+0.00878573*r*r*r-0.00390615*r*r-0.13470144*r-0.00033814;
//}

double fcut(double rcut, double r)
{
	double pi=3.1415926535897932;
	return 0.5*(1+cos(pi*(r/rcut)));
}
double fcutD(double rcut, double r)
{
	double pi=3.1415926535897932;
	return -pi/rcut*0.5*sin(pi*(r/rcut));
}

double fshort(double rshort, double r)
{
        double r1=0.95*rshort;
        double r2=1.05*rshort;

        if (r>r2)
                return 1;
        else if (r<r1)
                return 0;
        else
        {
                double u=(r-r1)/(r2-r1);
                return 6.0*u*u*u*u*u-15.0*u*u*u*u+10.0*u*u*u;
        }

}
double fshortD(double rshort, double r)
{
        double r1=0.95*rshort;
        double r2=1.05*rshort;

        if (r>r2)
                return 0;
        else if (r<r1)
                return 0;
        else
        {
                double u=(r-r1)/(r2-r1);
                return 1.0/(r2-r1)*(30.0*u*u*u*u-60.0*u*u*u+30.0*u*u);
        }
}


//potentials
double LJ(double sigma,double r)
{
	return 4.0*(pow(sigma/r,12)-pow(sigma/r,6));
}

double LJ_D(double sigma,double r)
{
	return 4.0*(-12.0/r*pow(sigma/r,12)+6.0/r*pow(sigma/r,6));
}

double Gaus(double p,double q,double r)
{
        return exp(-p*(r-q)*(r-q));
}
double Gaus_D(double p,double q,double r)
{
        return -2.0*p*(r-q)*exp(-p*(r-q)*(r-q));
}

double expSW(double A,double B,double r)
{
	if (r<=B+0.1)
		return 0;
	else;
	        return exp(A/(r-B));
}
double expSW_D(double A,double B,double r)
{
	if (r<=B+0.1)
		return 0;
	else;
        	return -A/((r-B)*(r-B))*exp(A/(r-B)); 
}


double STO(double A,double B,double r)
{
	if (A!=0)
	        return pow(r,A)*exp(-B*r);
	else
		return exp(-B*r);



}
double STO_D(double A,double B,double r)
{
	if (A!=0)
        	return (A-B*r)*pow(r,A-1)*exp(-B*r);
	else
	        return -B*exp(-B*r);
}

double GTO(double A,double B,double r)
{       
        if (A!=0)
                return pow(r,A)*exp(-B*r*r);
        else    
                return exp(-B*r*r);



}
double GTO_D(double A,double B,double r)
{       
        if (A!=0)
                return (A-2*B*r*r)*pow(r,A-1)*exp(-B*r*r);
        else    
                return -2*B*r*exp(-B*r*r);
}



double POW(double A,double B,double r)
{
	if (A>1)
	        return pow(r,A);
	else if (A==1)
		return r;
	else
		return 1; 



}
double POW_D(double A,double B,double r)
{
	if (A>2)
        	return A*pow(r,A-1);
	else
	        return 1; 
}

double Lorentz(double A, double B, double r)
{
	return 1/(pow((r-A),B)+1);
}

double Lorentz_D(double A, double B, double r)
{
	return -1/((pow((r-A),B)+1)*(pow((r-A),B)+1))*B*pow((r-A),B-1);
}

double ALogNormal(double A, double B, double r)
{
	if (r-A<=0)
		return 0;
	else
		return exp(-log((r-A)/B)*log((r-A)/B));
}

double ALogNormal_D(double A, double B, double r)
{
	if (r-A<=0)
		return 0;
	else
		return 2*exp(-log((r-A)/B)*log((r-A)/B))*log((r-A)/B)/(A-r);
}



//makes the link between the potential function  and i_nature
double doCalc(int i_nature, double r, double param1 , double param2,double Rcut)
{
        if (i_nature==1)
                return Gaus(param1,param2,r)*fcut(Rcut,r);
        else if (i_nature==2)
        	return LJ(param1,r);
        else if (i_nature==3)
        	return expSW(param1,param2,r)*fcut(Rcut,r);
        else if (i_nature==4)
        	return STO(param1,param2,r)*fcut(Rcut,r)/STO(param1,param2,2);
        else if (i_nature==5)
       		return POW(param1,param2,r)*fcut(Rcut,r);
        else if (i_nature==6)
                return GTO(param1,param2,r)*fcut(Rcut,r)/GTO(param1,param2,2);
        else if (i_nature==7)
                return Lorentz(param1,param2,r)*fcut(Rcut,r);
        else if (i_nature==8)
                return ALogNormal(param1,param2,r)*fcut(Rcut,r);
}

double doCalcD(int i_nature, double r, double param1 , double param2,double Rcut)
{
	double fn=0;
	double fnD=0; 
	double fc=fcut(Rcut,r);
	double fcD=fcutD(Rcut,r);

        if (i_nature==1)
	{
		fn=Gaus(param1,param2,r);
		fnD=Gaus_D(param1,param2,r);
	}
        else if (i_nature==2)
	{
		return LJ_D(param1,r);
	}
	else if  (i_nature==3)
	{
		fn=expSW(param1,param2,r);
		fnD=expSW_D(param1,param2,r);
	}
        else if (i_nature==4)
	{
		fn=STO(param1,param2,r)/STO(param1,param2,2);
		fnD=STO_D(param1,param2,r)/STO(param1,param2,2);
	}
        else if (i_nature==5)
	{
                fn=POW(param1,param2,r);
                fnD=POW_D(param1,param2,r);
	}
        else if (i_nature==6)
	{
                fn=GTO(param1,param2,r)/GTO(param1,param2,2);
                fnD=GTO_D(param1,param2,r)/GTO(param1,param2,2);
	}
        else if (i_nature==7)
	{
                fn=Lorentz(param1,param2,r);
                fnD=Lorentz_D(param1,param2,r);
	}
        else if (i_nature==8)
	{
                fn=ALogNormal(param1,param2,r);
                fnD=ALogNormal_D(param1,param2,r);
	}
	return fnD*fc+fn*fcD;
}



//makes the link between the potential function  and i_nature
double doCalc_Short(int i_nature, double r, double param1 , double param2,double Rcut,double rshort)
{
        if (i_nature==1)
                return Gaus(param1,param2,r)*fcut(Rcut,r)*fshort(rshort,r);
        else if (i_nature==2)
        	return LJ(param1,r);
        else if (i_nature==3)
        	return expSW(param1,param2,r)*fcut(Rcut,r)*fshort(rshort,r);
        else if (i_nature==4)
        	return STO(param1,param2,r)*fcut(Rcut,r)*fshort(rshort,r)/STO(param1,param2,2);
        else if (i_nature==5)
       		return POW(param1,param2,r)*fcut(Rcut,r)*fshort(rshort,r);
        else if (i_nature==6)
                return GTO(param1,param2,r)*fcut(Rcut,r)*fshort(rshort,r)/GTO(param1,param2,2);
        else if (i_nature==7)
                return Lorentz(param1,param2,r)*fcut(Rcut,r)*fshort(rshort,r);
        else if (i_nature==8)
                return ALogNormal(param1,param2,r)*fcut(Rcut,r)*fshort(rshort,r);
}

double doCalcD_Short(int i_nature, double r, double param1 , double param2,double Rcut,double rshort)
{
	double fn=0;
	double fnD=0; 
	double fc=fcut(Rcut,r);
	double fcD=fcutD(Rcut,r);
	double fs=fshort(rshort,r);
	double fsD=fshortD(rshort,r);

        if (i_nature==1)
	{
		fn=Gaus(param1,param2,r);
		fnD=Gaus_D(param1,param2,r);
	}
        else if (i_nature==2)
	{
		return LJ_D(param1,r);
	}
	else if  (i_nature==3)
	{
		fn=expSW(param1,param2,r);
		fnD=expSW_D(param1,param2,r);
	}
        else if (i_nature==4)
	{
		fn=STO(param1,param2,r)/STO(param1,param2,2);
		fnD=STO_D(param1,param2,r)/STO(param1,param2,2);
	}
        else if (i_nature==5)
	{
                fn=POW(param1,param2,r);
                fnD=POW_D(param1,param2,r);
	}
        else if (i_nature==6)
	{
                fn=GTO(param1,param2,r)/GTO(param1,param2,2);
                fnD=GTO_D(param1,param2,r)/GTO(param1,param2,2);
	}
        else if (i_nature==7)
	{
                fn=Lorentz(param1,param2,r);
                fnD=Lorentz_D(param1,param2,r);
	}
        else if (i_nature==8)
	{
                fn=ALogNormal(param1,param2,r);
                fnD=ALogNormal_D(param1,param2,r);
	}
	return fnD*fc*fs+fn*fcD*fs+fn*fc*fsD;
}


