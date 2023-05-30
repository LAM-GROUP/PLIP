#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <fstream>
#include <iostream>
#include <random>
#include <cmath>

using namespace std;

/**
 * @brief A search and replace function
 * 
 * @param str The input string.
 * @param from Search sub-string.
 * @param to Replacement sub-string.
 * @return string 
 */
string replace(std::string& str, const std::string& from, const std::string& to);

/**
 * @brief 
 * 
 * @param type_at 
 * @param type_distance 
 * @return int 
 */
int whichTypeNB (int type_at, int type_distance);
/**
 * @brief 
 * 
 * @param type_3B 
 * @param type1 
 * @param type2 
 * @return string 
 */
string whichType3B (int type_3B, string type1, string type2);
/**
 * @brief 
 * 
 * @param type_3B 
 * @param type1 
 * @param type2 
 * @return string 
 */
string whichType3B_BIS (int type_3B, string type1, string type2);
/**
 * @brief 
 * 
 * @param Rshort 
 * @param i_type 
 * @return double 
 */
double whichShort (double Rshort[3] , int i_type);
/**
 * @brief  A function to read coefficients.
 * 
 * @param input_file  The filename with coefficients
 * @return vector<double>  A vector of coefficients
 */
vector<double> doReadCoeff(string input_file);


/**
 * @brief  Print two-body potential.
 * 
 * @param r  interatomic distance values. 
 * @param Pot2B  Computed two-body potential values.
 * @param Force2B Computed two-body force values
 */
void doPrint2B(vector<double> r, vector<vector<double>> Pot2B, vector<vector<double>> Force2B);
/**
 * @brief Print three-body potential.
 * 
 * @param r  interatomic distance values.
 * @param Pot3B Computed two-body potential values.
 * @param Force3B Computed three-body force values.
 */
void doPrint3B(vector<double> r, vector<double>theta, vector<vector<vector<vector<double>>>> Pot3B, vector<vector<vector<vector<double>>>> Force3B);




