#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>
#include <random>
#include <cmath>
#include <math.h>
#include <chrono>
#include <ctime>
#include <stdio.h>
#include <dirent.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include "mod_fonction.h"
#include "mod_tool.h"
#include "mod_XYZ.h"
#include "mod_xmat.h"

using namespace std;


int main(int argc,char* argv[])
{
        cout<<""<<endl;
        cout<<"==================================================================="<<endl;
        cout<<"=====================         READ XMAT       ====================="<<endl;
        cout<<"==================================================================="<<endl<<endl<<endl;

	std::chrono::time_point<std::chrono::system_clock> start, end;
        start = std::chrono::system_clock::now();	
	string init_file="input.txt";
///////////////////////////////////////////////////////////////////////////
	
	ifstream myfile(init_file, ios::in);
	double Rcut;	
	double Rshort[3];	
	string input_folder;
	string output_folder;

        string line;
	std::getline( myfile, line );
	string tmp; 
	myfile >> tmp>>Rcut>>tmp>>input_folder>>tmp>>Rshort[0]>>tmp>>Rshort[1]>>tmp>>Rshort[2];
	myfile.close();




        cout<<"=====================     INITIALIZATION     ====================="<<endl;
	cout<<"Input_folder="<<input_folder<<endl;
	cout<<"Rcut="<<Rcut<<endl;
	cout<<"R_AA="<<Rshort[0]<<endl;
	cout<<"R_BB="<<Rshort[1]<<endl;
	cout<<"R_AB="<<Rshort[2]<<endl;
        cout<<endl<<endl<<endl;

//////////////////////////////////////////////////////////////////////////////
	
	vector<double> r;
	int N_i=1000;
	for (int i=0;i<N_i+1;i++)
		r.push_back((Rcut-0.5)*i/N_i+0.5);


	string input_coeff="Coeff.txt";
	if ( argc == 2 )
		input_coeff=argv[1];

	vector<double> coeffS=doReadCoeff(input_coeff);

	int i_func=0;
	vector<vector<double>> Pot2B (3,vector<double> (r.size()));
	vector<vector<double>> Force2B (3,vector<double> (r.size()));

	ifstream myfile2(init_file, ios::in);	
	int ii = 0;
	string line2;
        while( std::getline( myfile2,line2 ))
        {
        	if(ii > 10 )
        	{
			int i_nature,i_include,i_bodies;
			double a_min,a_step,a_max;
			double b_min,b_step,b_max;
			int check2B, N_l3B,N_lNB;
	                stringstream in(line2);
        	        string tmp;
        		in >> tmp>> i_nature;
			in >> tmp>> i_include;
			if (i_include!=0)
			{	
				in >> tmp>> a_min>>a_max>>a_step;
				in >> tmp>> b_min>>b_max>>b_step;
				in >> tmp>> check2B;
				in >> tmp>> N_l3B;
				in >> tmp>> N_lNB;

				strucXYZ xyz;
				XMAT XMAT(xyz, a_min,a_max,a_step,b_min,b_max,b_step,i_nature,check2B,N_l3B,N_lNB);	
				XMAT.doComputePot(Pot2B,Force2B,r,i_func,coeffS,Rcut,Rshort);
				cout<<"i_func="<<i_func<<endl;
			}
        	}
	        ii++;
        }
	myfile2.close();


	doPrint2B(r,Pot2B,Force2B);
        
        
        cout<<"==================================================================="<<endl<<endl;

	end = std::chrono::system_clock::now();
        int elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>
                             (end-start).count();
	cout<<"time_run="<<elapsed_seconds<<"µs"<<endl<<endl;
        cout<<"==================================================================="<<endl;
        cout<<"==================================================================="<<endl;
        cout<<"==================================================================="<<endl;
	return 0;
}

