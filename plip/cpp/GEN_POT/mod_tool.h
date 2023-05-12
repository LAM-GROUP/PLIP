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


string replace(std::string& str, const std::string& from, const std::string& to);


int whichTypeNB (int type_at, int type_distance);
string whichType3B (int type_3B, string type1, string type2);
string whichType3B_BIS (int type_3B, string type1, string type2);
double whichShort (double Rshort[3] , int i_type);

vector<double> doReadCoeff(string input_file);



void doPrint2B(vector<double> r, vector<vector<double>> Pot2B, vector<vector<double>> Force2B);
void doPrint3B(vector<double> r, vector<double>theta, vector<vector<vector<vector<double>>>> Pot3B, vector<vector<vector<vector<double>>>> Force3B);




