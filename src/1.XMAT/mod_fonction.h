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


/**
 * @brief The cut-off function definition.
 * 
 * @param rcut The cut-off radii in Å.
 * @param r  The value of the distance.
 * @return double  The cut-off function value
 */
double fcut(double rcut, double r);

/**
 * @brief The derivative of function @see fcut
 * 
 * @param rcut The cut-off radii in Å
 * @param r  The value of distance at which derivative taken.
 * @return double The cut-off function derivative value
 */
double fcutD(double rcut, double r);

/**
 * @brief The lennard-jones potential 
 * 
 * @param sigma  See lennard jones potential
 * @param r Distance value
 * @return double Function value
 */
double LJ(double sigma,double r);

/**
 * @brief The derivative of lennard jones potential @see LJ
 * 
 * @param sigma See lennard jones potential
 * @param r Distance value
 * @return double Function value
 */
double LJ_D(double sigma,double r);

/**
 * @brief  Gaussian function
 * 
 * @param p Width of gaussian
 * @param q Position of gaussian
 * @param r Distance value
 * @return double Derivative value
 */
double Gaus(double p,double q,double r);

/**
 * @brief Derivative of gaussian function @see Gaus
 * 
 * @param p Width of gaussian.
 * @param q Position of gaussian.
 * @param r Distance value.
 * @return double Derivative value
 */
double Gaus_D(double p,double q,double r);

double expSW(double A,double B,double r);
double expSW_D(double A,double B,double r);


/**
 * @brief  The function used to link potential function and user input.
 * 
 * @param i_nature The integer value representing the choice of potential.
 * @param r        The distance value.
 * @param param1   The first  parameter of the chosen potential function.
 * @param param2   The second  parameter of the chosen potential function.
 * @param Rcut     The cut-off value for distance.
 * @return double  The calculated potential value.
 */
double doCalc(int i_nature, double r, double param1 , double param2,double Rcut);

/**
 * @brief Returns the derivative of potential function 
 * 
 * @param i_nature The integer value representing the choice of potential.
 * @param r The distance value.
 * @param param1 The first  parameter of the chosen potential function.
 * @param param2 The second  parameter of the chosen potential function.
 * @param Rcut The cut-off value for distance.
 * @return double  The calculated derivative value.
 */
double doCalcD(int i_nature, double r, double param1 , double param2,double Rcut);

/**
 * @brief  The function is similar to @see doCalcD but with an additional short range part.
 * 
 * @param i_nature The integer value representing the choice of potential.
 * @param r The distance value.
 * @param param1 The first  parameter of the chosen potential function.
 * @param param2 The second  parameter of the chosen potential function.
 * @param Rcut The cut-off value for distance.
 * @param Rshort The value of short-range distance for Nbody conribution.
 * @return double The calculated potential value.
 */
double doCalc_Short(int i_nature, double r, double param1 , double param2,double Rcut,double Rshort);

/**
 * @brief Returns derivative of @see doCalc_Short
 * 
 * @param i_nature The integer value representing the choice of potential.
 * @param r 
 * @param param1 The first  parameter of the chosen potential function
 * @param param2 The second  parameter of the chosen potential function
 * @param Rcut The cut-off value for distance
 * @param Rshort The value of short-range distance for Nbody conribution.
 * @return double The calculated derivative value
 */
double doCalcD_Short(int i_nature, double r, double param1 , double param2,double Rcut,double Rshort);

/**
 * @brief Function to check the choice of potential given by user.
 * 
 * @param i_nature The integer number.
 * @return string  The type of potential.
 */
string whichPot(int i_nature);

