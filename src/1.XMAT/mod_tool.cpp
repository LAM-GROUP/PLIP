#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <fstream>
#include <iostream>
#include <random>
#include <cmath>
#include "mod_tool.h"
using namespace std;


string replace(std::string& str, const std::string& from, const std::string& to)
{
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return str;
    string output=str.replace(start_pos, from.length(), to);
    return output;
}

string whichType3B (int type_3B, string type1, string type2)
{
	if (type_3B==1)
		return type1+"  "+type1+"  "+type1;
	else if	(type_3B==2)
		return type2+"  "+type2+"  "+type2; 
	else if	(type_3B==3)
		return type1+"  "+type2+"  "+type2; 
	else if	(type_3B==4)
		return type1+"  "+type1+"  "+type2; 
	else if	(type_3B==5)
		return type2+"  "+type1+"  "+type1; 
	else if	(type_3B==6)
		return type2+"  "+type2+"  "+type1; 
}
string whichType3B_BIS (int type_3B, string type1, string type2)
{
	if (type_3B==4)
		return type1+"  "+type2+"  "+type1;
	else if	(type_3B==6)
		return type2+"  "+type1+"  "+type2; 
}

int whichTypeNB (int type_at, int type_distance)
{
	if (type_distance!=0)
	{
		if (type_at==1)             
		{                           
		        if (type_distance==1)
		                return 1;
		        else        
		                return 2;               
		}                   
		else if (type_at==2)               
		{                   
		        if (type_distance==2)
		                return 3;
		        else
		                return 4;
		}
	}
	else
		return 0;
}


double whichShort (double Rshort[3] , int i_type)
{
        if (i_type!=0)
        {
                if (i_type==1)
			return Rshort[0];
                else if (i_type==2)
			return Rshort[2];
                else if (i_type==3)
			return Rshort[1];
                else if (i_type==4)
			return Rshort[2];
        }
}



vector<double> doReadCoeff(string input_file)
{
        vector<double>  coeffS;
        string line;
        double tmp;
        ifstream myfile(input_file);

        while (getline(myfile, line))
        {
                stringstream in(line);
                in>>tmp>>tmp;
                coeffS.push_back(tmp);
        }
        return coeffS;
}



void doPrint2B(vector<double> r, vector<vector<double>> Pot2B, vector<vector<double>> Force2B)
{
	for (int i_type=0;i_type<3;i_type++)
	{
		string output_file="out2B_"+to_string(i_type)+".txt";
		ofstream myfile(output_file, ios::out | ios::trunc);
	
		myfile<<"LARS_2B"<<endl;
		myfile<<"N  "<<r.size()<<"  R  "<<r[0]<<"  "<<r[r.size()-1]<<endl<<endl;
		for (int i=0;i<r.size();i++)
		{
			myfile<<i<<"  "<<r[i]<<"  "<<Pot2B[i_type][i]<<"  "<<Force2B[i_type][i]<<endl;
		}
	}
}



void doPrint3B(vector<double> r, vector<double>theta, vector<vector<vector<vector<double>>>> Pot3B,vector<vector<vector<vector<double>>>> Force3B) 
{
	string output_file="out3B.txt";
	ofstream myfile(output_file, ios::out | ios::trunc);

	for (int i=0;i<theta.size();i++)
	{
		for (int k=0;k<r.size();k++)
		{
			myfile<<theta[i]<<"  "<<r[k]<<"  "<<r[k]<<"  ";
		        for (int i_type=0;i_type<6;i_type++)
		                myfile<<Pot3B[i_type][i][k][k]<<"  ";
			myfile<<endl;
		}
		myfile<<endl;
	}
}



