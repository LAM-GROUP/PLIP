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

using namespace std;

//Basic functions
double fcut(double rcut, double r);
double fcutD(double rcut, double r);

double LJ(double sigma,double r);
double LJ_D(double sigma,double r);

double Gaus(double p,double q,double r);
double Gaus_D(double p,double q,double r);

double expSW(double A,double B,double r);
double expSW_D(double A,double B,double r);



double doCalc(int i_nature, double r, double param1 , double param2,double Rcut);
double doCalcD(int i_nature, double r, double param1 , double param2,double Rcut);

double doCalc_Short(int i_nature, double r, double param1 , double param2,double Rcut,double Rshort);
double doCalcD_Short(int i_nature, double r, double param1 , double param2,double Rcut,double Rshort);


string whichPot(int i_nature);

