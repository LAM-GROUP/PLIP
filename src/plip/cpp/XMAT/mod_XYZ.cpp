#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <fstream>
#include <iostream>
#include <random>
#include <cmath>
#include "mod_XYZ.h"
// Constructor
strucXYZ::strucXYZ()
{
	N_atoms_=0;
	N_times_=1;
	for (int i=0;i<3; i++)
	for (int j=0;j<3; j++)
		L_[i][j]=0;
}

void strucXYZ::doReadXYZ(string input_file, vector<string> species)
{
	string line;
	ifstream myfile(input_file);

	// Read Box lattice
	getline(myfile,line);
	getline(myfile,line);
	for (int i_basis=0;i_basis<3; i_basis++)
	{
		getline(myfile,line);
		stringstream in(line);  
		double tmp1,tmp2,tmp3;
		for (int i_coord=0;i_coord<3;i_coord++)
			in >> L_[i_basis][i_coord];
	}	
	cout<<L_[0][0]<<"  "<<L_[0][1]<<"  "<<L_[0][2]<<endl;
	cout<<L_[1][0]<<"  "<<L_[1][1]<<"  "<<L_[1][2]<<endl;
	cout<<L_[2][0]<<"  "<<L_[2][1]<<"  "<<L_[2][2]<<endl;

	// Read number of A and B
	getline(myfile,line);
	string linetype=line;
	cout<<"Size="<<linetype.length()<<endl;
	int NA,NB;
	NA=0;
	NB=0;
	getline(myfile,line);
	stringstream in(line); 
	cout <<"Atom types:"<<species.size ()<<endl;

	if (species.size ()== 2)
	{

	if (linetype.find(species[0]) != std::string::npos) 
	{		
		if (linetype.find(species[1]) != std::string::npos) 
			in>>NA>>NB;
		else
			in>>NA;
	}
	else
		in>>NB;

	cout<<species[0]<<":"<<NA<<" "<<species[1]<<":"<<NB<<endl;
	}
	else if (species.size () == 1)
	{
		if (linetype.find(species[0]) != std::string::npos) 
			in>>NA;
		cout<<species[0]<<":"<<NA<<endl;
	}
	else
	{
		cout<<"Number of atom types greater than 2 not supported"<<endl;
		exit(1);
	}
//	cout<<"Na="<<NA<<"  Nb="<<NB<<endl;


//
//
//	if (abs(double(NB))>200)
//		NB=0;
	N_atoms_=NA+NB;
	getline(myfile,line);
	string coord_type=line;
	if (coord_type != "Cartesian") {
        cout << "POSCAR format: " << coord_type << endl;
        cout << "FileError: POSCAR not in Cartesian format" << endl;
        exit(1); 
    }


	
      
	vector<vector<double>> XYZs;
	vector<int> type_onestep (N_atoms_);
	for (int i_atom=0;i_atom<N_atoms_;i_atom++)
	{
		vector<double> xyz(3,0);;
		getline(myfile,line);
		stringstream in(line);	
		for (int i_coord=0;i_coord<3; i_coord++)
			in >> xyz[i_coord];
		if (i_atom<int(NA))
		{
			type_onestep[i_atom]=1;
		}
		else
		{
			type_onestep[i_atom]=2;
		}
		XYZs.push_back(xyz);
	}
	type_.push_back(type_onestep);
	pos_.push_back(XYZs);
	myfile.close();
}


void strucXYZ::doComputeDistances(double R_cut)
{

        vector<vector<vector<vector<double>>>> dx_TMP(N_times_, vector<vector<vector<double>>>(N_atoms_,vector<vector<double>>(N_atoms_,vector<double>(3))));
        vector<vector<vector<double>>> distance_TMP(N_times_, vector<vector<double>>(N_atoms_,vector<double>(N_atoms_)));
        vector<vector<vector<int>>> distanceTYPE_TMP(N_times_, vector<vector<int>>(N_atoms_,vector<int>(N_atoms_)));
	int i_time=0;


//	#pragma omp parallel 
	for (int i=0;i<N_atoms_-1;i++)
	for (int j=i+1;j<N_atoms_;j++)
	{   
	        double d=1E10;
	
                double d_tmp=0;
                double dx_TMP2[3] {0,0,0};
                for (int i_coord=0;i_coord<3; i_coord++)
                {
                        dx_TMP2[i_coord]=pos_[i_time][i][i_coord]-pos_[i_time][j][i_coord];
                        d_tmp+=dx_TMP2[i_coord]*dx_TMP2[i_coord];
                }
                if (d_tmp<d)
                {
                        d=d_tmp;
                        for (int i_coord=0;i_coord<3; i_coord++)
                        {
                                dx_TMP[i_time][i][j][i_coord]=dx_TMP2[i_coord];
                                dx_TMP[i_time][j][i][i_coord]=-dx_TMP2[i_coord];
                        }
                }




		for (int i_shift=-1;i_shift<2; i_shift++)
		for (int j_shift=-1;j_shift<2; j_shift++)
		for (int k_shift=-1;k_shift<2; k_shift++)
		if ( not (i_shift==0 && j_shift==0 && k_shift==0)) 
		{
		        d_tmp=0;
		        double dx_TMP2[3] {0,0,0};
			for (int i_coord=0;i_coord<3; i_coord++)
			{
				dx_TMP2[i_coord]=pos_[i_time][i][i_coord]-(pos_[i_time][j][i_coord]+i_shift*L_[0][i_coord]+j_shift*L_[1][i_coord]+k_shift*L_[2][i_coord] ); 
				d_tmp+=dx_TMP2[i_coord]*dx_TMP2[i_coord];
			}


			if (d_tmp<d)
			{
				d=d_tmp;
				for (int i_coord=0;i_coord<3; i_coord++)
				{
					dx_TMP[i_time][i][j][i_coord]=dx_TMP2[i_coord];
					dx_TMP[i_time][j][i][i_coord]=-dx_TMP2[i_coord];
				}
			}
			

		}


		if (d<R_cut*R_cut)
	        {
	            distance_TMP[i_time][i][j]=sqrt(d);
	            distance_TMP[i_time][j][i]=sqrt(d);
	
	            if ((type_[i_time][i]==1) && (type_[i_time][j]==1))
	                distanceTYPE_TMP[i_time][i][j]=1;
	            else if ( (type_[i_time][i]==2) && (type_[i_time][j]==2))
	                distanceTYPE_TMP[i_time][i][j]=2;
	            else
	                distanceTYPE_TMP[i_time][i][j]=3;
	        }
	        else
	            distanceTYPE_TMP[i_time][i][j]=0;
	        distanceTYPE_TMP[i_time][j][i]=distanceTYPE_TMP[i_time][i][j];
        }
            
	distance_=distance_TMP;
	distanceTYPE_=distanceTYPE_TMP;
	dx_=dx_TMP;
}

void strucXYZ::doComputeAngles(double R_cut)
{

	vector<vector<vector<vector<double>>>> angleTMP(N_times_, vector<vector<vector<double>>>(N_atoms_,vector<vector<double>>(N_atoms_, vector<double>(N_atoms_))));
	vector<vector<vector<vector<int>>>> angleTYPE_TMP(N_times_, vector<vector<vector<int>>>(N_atoms_,vector<vector<int>>(N_atoms_, vector<int>(N_atoms_,0))));

	for (int i_time=0;i_time<N_times_;i_time++)
	#pragma omp parallel for collapse(2)
	for (int i=0;i<N_atoms_;i++)
	for (int j=0;j<N_atoms_;j++)
	if  (j!=i)
	for (int k=j+1;k<N_atoms_;k++)
	if  (k!=i)
	if (distanceTYPE_[i_time][i][j]!=0 && distanceTYPE_[i_time][i][k]!=0)
	{
		double W=0;
		double rinvijk=1.0/(distance_[i_time][i][j]*distance_[i_time][i][k]);
		for (int i_coord=0;i_coord<3; i_coord++)
		{
			W+=dx_[i_time][j][i][i_coord]*dx_[i_time][k][i][i_coord];
		}
		angleTMP[i_time][i][j][k]=rinvijk*W;
		angleTMP[i_time][i][k][j]=angleTMP[i_time][i][j][k];

		angleTYPE_TMP[i_time][i][j][k]=0;
		angleTYPE_TMP[i_time][i][k][j]=0;
		if (distanceTYPE_[i_time][i][j]!=0)
		if (distanceTYPE_[i_time][i][k]!=0)
		{
			if ((type_[i_time][i]==1) && (type_[i_time][j]==1) && (type_[i_time][k]==1) )
				angleTYPE_TMP[i_time][i][j][k]=1;
			else if ((type_[i_time][i]==2) && (type_[i_time][j]==2) && (type_[i_time][k]==2) )
				angleTYPE_TMP[i_time][i][j][k]=2;
			else if ((type_[i_time][i]==1) && (type_[i_time][j]==2) && (type_[i_time][k]==2) )
				angleTYPE_TMP[i_time][i][j][k]=3;
			else if ((type_[i_time][i]==1) && (type_[i_time][j]==1) && (type_[i_time][k]==2) )
				angleTYPE_TMP[i_time][i][j][k]=4;
			else if ((type_[i_time][i]==1) && (type_[i_time][j]==2) && (type_[i_time][k]==1) )
				angleTYPE_TMP[i_time][i][j][k]=4;
			else if ((type_[i_time][i]==2) && (type_[i_time][j]==1) && (type_[i_time][k]==1) )
				angleTYPE_TMP[i_time][i][j][k]=5;
			else if ((type_[i_time][i]==2) && (type_[i_time][j]==1) && (type_[i_time][k]==2) )
				angleTYPE_TMP[i_time][i][j][k]=6;
			else if ((type_[i_time][i]==2) && (type_[i_time][j]==2) && (type_[i_time][k]==1) )
				angleTYPE_TMP[i_time][i][j][k]=6;
		}
		angleTYPE_TMP[i_time][i][k][j]=angleTYPE_TMP[i_time][i][j][k];
	}
	else
	{
                angleTMP[i_time][i][j][k]=0;
                angleTMP[i_time][i][k][j]=0;
	}
	angles_=angleTMP;
	anglesTYPE_=angleTYPE_TMP;
}


