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
	N_times_=0;
	for (int i_coord=0;i_coord<3; i_coord++)
		L_.push_back(0);
	
}

void strucXYZ::doReadXYZ(string input_file)
{
	string line;

	// Count N_times, N_atoms and retrieve L_[i_coord]box
	ifstream myfile(input_file);
	for (int i_jump=0;i_jump<4; i_jump++)	
		getline(myfile,line);  
	N_atoms_=stod(line);

	getline(myfile,line);
	for (int i_coord=0;i_coord<3; i_coord++)
	{
		getline(myfile,line);
		stringstream in(line);  
		double tmp1,tmp2;
		in >> tmp1;
		in >> tmp2;
		L_[i_coord]=tmp2-tmp1;
	}	
	cout<<"L="<<L_[0]<<"  "<<L_[1]<<"  "<<L_[2]<<endl;

	int N_lines=8;
	while (getline(myfile, line))
	    ++N_lines;
	N_times_=N_lines/(N_atoms_+9);
	myfile.close();
      
	// Read XYZ positions
	ifstream myfile2(input_file);
	for (int i_time=0;i_time<N_times_;i_time++)
	{
		for (int i_jump=0;i_jump<9; i_jump++)	
			getline(myfile2,line);	// HEAD 
		vector<vector<double>> XYZs;
		vector<int> type_onestep (N_atoms_);
		for (int i_atom=0;i_atom<N_atoms_;i_atom++)
		{
			vector<double> xyz(3,0);;
			getline(myfile2,line);
			stringstream in(line);	

        		string tmp;
			in >> tmp;
			in >> type_onestep[i_atom];
			for (int i_coord=0;i_coord<3; i_coord++)
				in >> xyz[i_coord];
			XYZs.push_back(xyz);
		}
		type_.push_back(type_onestep);
		pos_.push_back(XYZs);
	}
        myfile2.close();

}


void strucXYZ::doComputeDistances(double R_cut)
{
        vector<vector<vector<vector<double>>>> dx_TMP(N_times_, vector<vector<vector<double>>>(N_atoms_,vector<vector<double>>(N_atoms_,vector<double>(3))));
        vector<vector<vector<double>>> distance_TMP(N_times_, vector<vector<double>>(N_atoms_,vector<double>(N_atoms_)));
        vector<vector<vector<int>>> distanceTYPE_TMP(N_times_, vector<vector<int>>(N_atoms_,vector<int>(N_atoms_)));
	int i_true=0;
	#pragma omp parallel 
        for (int i_time=0;i_time<N_times_;i_time++)
	for (int i=0;i<N_atoms_-1;i++)
	for (int j=i+1;j<N_atoms_;j++)
	if( j!=i)
	{
		double d=0;
		for (int i_coord=0;i_coord<3; i_coord++)
	        {
			double dx=pos_[i_time][i][i_coord]-pos_[i_time][j][i_coord];
			if (abs(dx)>L_[i_coord]/2.0)	
			{
				dx=-(abs(abs(dx)-L_[i_coord]))*dx/abs(dx);
			}
			dx_TMP[i_time][i][j][i_coord]=dx;
			dx_TMP[i_time][j][i][i_coord]=-dx;
			d+=dx*dx;
		}

		if (d<R_cut*R_cut)
		{
			distance_TMP[i_time][i][j]=sqrt(d);
			distance_TMP[i_time][j][i]=sqrt(d);
			i_true++;
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


void strucXYZ::doComputeAnglesOLD(double R_cut)
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
			
			if ((pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]>0)&&(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]>0))
			{
				if ((pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]>L_[i_coord]/2.0)&&(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]>L_[i_coord]/2.0))
				{
					double V=(pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]-L_[i_coord]);
	                	        V=V*(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]-L_[i_coord]);
        		                W+=V;
				}
				if((pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]<L_[i_coord]/2.0)&&(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]<L_[i_coord]/2.0))
				{
					double V=(pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]);
                                        V=V*(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]);
                                        W+=V;
				}
				if((pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]<L_[i_coord]/2.0)&&(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]>L_[i_coord]/2.0))

				{
					double V=(pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]);
                                        V=V*(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]-L_[i_coord]);
                                        W+=V;
				}
				 if((pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]>L_[i_coord]/2.0)&&(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]<L_[i_coord]/2.0))
                                {
                                        double V=(pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]-L_[i_coord]);
                                        V=V*(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]);
                                        W+=V;
                                }
			}	
			if ((pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]<0)&&(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]<0))
			{
                                if((pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]<-L_[i_coord]/2.0)&&(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]<-L_[i_coord]/2.0))
                                {
					double V=(pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]+L_[i_coord]);
                                        V=V*(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]+L_[i_coord]);
                                        W+=V;
                                }
				if((pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]>-L_[i_coord]/2.0)&&(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]>-L_[i_coord]/2.0))
                                {
                                        double V=(pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]);
                                        V=V*(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]);
                                        W+=V;
                                }
				if((pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]<-L_[i_coord]/2.0)&&(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]>-L_[i_coord]/2.0))
                                {
                                        double V=(pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]+L_[i_coord]);
                                        V=V*(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]);
                                        W+=V;
                                }
				if((pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]>-L_[i_coord]/2.0)&&(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]<-L_[i_coord]/2.0))
                                {
                                        double V=(pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]);
                                        V=V*(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]+L_[i_coord]);
                                        W+=V;
                                }
			}
			if ((pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]>0)&&(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]<0))
			{
			if((pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]>L_[i_coord]/2.0)&&(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]<-L_[i_coord]/2.0))
                                {
                                        double V=(pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]-L_[i_coord]);
                                        V=V*(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]+L_[i_coord]);
                                        W+=V;
                                }
                                if((pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]>L_[i_coord]/2.0)&&(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]>-L_[i_coord]/2.0))
                                {
                                        double V=(pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]-L_[i_coord]);
                                        V=V*(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]);
                                        W+=V;
                                }
                                if((pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]<L_[i_coord]/2.0)&&(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]<-L_[i_coord]/2.0))
                                {
                                        double V=(pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]);
                                        V=V*(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]+L_[i_coord]);
                                        W+=V;
                                }
                                if((pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]<L_[i_coord]/2.0)&&(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]>-L_[i_coord]/2.0))
                                {
                                        double V=(pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]);
                                        V=V*(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]);
                                        W+=V;
				}
			}
			if ((pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]<0)&&(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]>0))			
			{
				if((pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]<-L_[i_coord]/2.0)&&(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]>L_[i_coord]/2.0))
                                {
                                        double V=(pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]+L_[i_coord]);
                                        V=V*(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]-L_[i_coord]);
                                        W+=V;
                                }
                                if((pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]<-L_[i_coord]/2.0)&&(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]<L_[i_coord]/2.0))
                                {
                                        double V=(pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]+L_[i_coord]);
                                        V=V*(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]);
                                        W+=V;
                                }       
                                if((pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]>-L_[i_coord]/2.0)&&(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]>L_[i_coord]/2.0))
                                {
                                        double V=(pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]);
                                        V=V*(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]-L_[i_coord]);
                                        W+=V;
                                }
                                if((pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]>-L_[i_coord]/2.0)&&(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]<L_[i_coord]/2.0))
                                {
                                        double V=(pos_[i_time][j][i_coord]-pos_[i_time][i][i_coord]);
                                        V=V*(pos_[i_time][k][i_coord]-pos_[i_time][i][i_coord]);
                                        W+=V;
                                }
			}
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
//		angleTYPE_TMP[i_time][i][j][k]=0;
//		angleTYPE_TMP[i_time][i][k][j]=0;
                angleTMP[i_time][i][j][k]=0;
                angleTMP[i_time][i][k][j]=0;
	}
	angles_=angleTMP;
	anglesTYPE_=angleTYPE_TMP;
}


