#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <fstream>
#include <iostream>
#include <random>
#include <cmath>
#include <math.h>
#include "mod_fonction.h"
#include "mod_XYZ.h"
#include "mod_xmat.h"
#include "mod_tool.h"


XMAT::XMAT(const strucXYZ &xyz,double a_init,double a_end,double a_step,double b_init,double b_end,double b_step,int i_nature,int check2B, int N_l3B,int N_lNB)
{
	i_nature_=i_nature;
	check2B_=check2B;
	N_l3B_=N_l3B;
	N_lNB_=N_lNB;

	cout<<"Working with "<<whichPot(i_nature)<<endl;
	cout<<"list_a= ";
        for (double a=a_init; a<a_end+a_step;a=a+a_step)
	{
		list_a_.push_back(a);
		cout<<a<<"  ";
	}
	cout<<endl;

	cout<<"list_b= ";
        for (double b=b_init; b<b_end+b_step;b=b+b_step)
	{
		cout<<b<<"  ";
		list_b_.push_back(b);
	}
	cout<<endl;

	int N_a=list_a_.size();
	int N_b=list_b_.size();

}

void XMAT::doXMAT(const strucXYZ &xyz,string output_file, double Rcut,double Rshort[3])
{
        int N_atoms=xyz.GetN_atoms();
        int N_times=xyz.GetN_times();
	int N_a=list_a_.size();
	int N_b=list_b_.size();
	int N_func=N_a*N_b;

	vector<vector<vector<vector<double>>>> dx=xyz.GetDX();
	vector<vector<vector<double>>> distance=xyz.GetDistance();
	vector<vector<vector<int>>> distanceTYPE=xyz.GetDistanceTYPE();
	vector<vector<vector<vector<double>>>> cos=xyz.GetAngle();
	vector<vector<vector<vector<int>>>> angleTYPE=xyz.GetAngleTYPE();
	vector<vector<int>> type=xyz.GetType();


	/*=== COMPUTE f(r) and f'(r) for all r  ===*/
	vector<vector<vector<vector<vector<double>>>>> frij(N_a,  vector<vector<vector<vector<double>>>>(N_b, vector<vector<vector<double>>>(N_times,vector<vector<double>>(N_atoms,vector<double> (N_atoms) )))); 
	vector<vector<vector<vector<vector<double>>>>> frijD(N_a, vector<vector<vector<vector<double>>>>(N_b, vector<vector<vector<double>>>(N_times,vector<vector<double>>(N_atoms,vector<double> (N_atoms) )))); 

	#pragma omp parallel for collapse(2)
	for (int i_a=0; i_a<N_a;i_a++)
	for (int i_b=0; i_b<N_b;i_b++)
	{
	        double a=list_a_[i_a];       //parameter 1 index
	        double b=list_b_[i_b];       //parameter 2 index
        	for (int i_time=0;i_time<N_times;i_time++)
		{
	
		
			for (int i=0; i<N_atoms; i++)
			for (int j=i+1; j<N_atoms; j++)
			{
				if (distanceTYPE[i_time][i][j]!=0)
				{
					frij[i_a][i_b][i_time][i][j] =doCalc(i_nature_,distance[i_time][i][j],a,b,Rcut);
					frijD[i_a][i_b][i_time][i][j]=doCalcD(i_nature_,distance[i_time][i][j],a,b,Rcut);
				
					frij[i_a][i_b][i_time][j][i] =frij[i_a][i_b][i_time][i][j];
					frijD[i_a][i_b][i_time][j][i]=frijD[i_a][i_b][i_time][i][j];
				}
			}
		}
	}

	/*=== COMPUTE 2B contribution  ===*/
	if (check2B_==1)
	{
		string output_file0=output_file;
		output_file0.append("_2B.bin");
		fstream fileStream;
		fileStream.open(output_file0);
		if (fileStream.fail()) 
		{
		        vector<vector<vector<vector<vector<vector<double>>>>>> force2B(3,  vector<vector<vector<vector<vector<double>>>>>(N_a, vector<vector<vector<vector<double>>>>(N_b,vector<vector<vector<double>>>(N_times,vector<vector<double>>(N_atoms,vector<double>(3) )))));
	
		        for (int i_time=0;i_time<N_times;i_time++)
			{
	
				for (int i_type=1;i_type<4;i_type++)
				#pragma omp parallel for collapse(2) 
				for (int i_a=0; i_a<N_a;i_a++)
				for (int i_b=0; i_b<N_b;i_b++)
				{
					
						for (int i=0; i<N_atoms; i++)
						for (int j=i+1; j<N_atoms; j++)
						if (distanceTYPE[i_time][i][j]==i_type)
						{
							double tmp=1.0/distance[i_time][i][j]*frijD[i_a][i_b][i_time][j][i];
							for (int beta=0;beta<3; beta++)
							{
								force2B[i_type-1][i_a][i_b][i_time][i][beta]-=tmp*dx[i_time][i][j][beta];  // There is a minus sign for F=-grad(V)
								force2B[i_type-1][i_a][i_b][i_time][j][beta]+=tmp*dx[i_time][i][j][beta];  // There is a minus sign for F=-tmp*dx;
							}
						}
				}
	
			}
		
			ofstream myfile2B(output_file0, ios::out | ios::trunc);
			for (int i_type=0;i_type<3;i_type++)
			for (int i_a=0; i_a<N_a;i_a++)
			for (int i_b=0; i_b<N_b;i_b++)
			{
				for (int i_time=0;i_time<N_times;i_time++)
				for (int i=0; i<N_atoms; i++)
				{
				        for (int beta=0;beta<3; beta++)
				        {
						myfile2B <<force2B[i_type][i_a][i_b][i_time][i][beta]<<"  ";
				        }
				}
				myfile2B<<endl;
			}
			cout<<"2B is done"<<endl;
		}
	}
	
	/*=== COMPUTE 3B contribution  ===*/
	if (N_l3B_!=0)
	{
        	string output_file0=output_file;
        	output_file0.append("_3B.bin");
		fstream fileStream;
		fileStream.open(output_file0);
		if (fileStream.fail()) 
		{
			vector<vector<vector<vector<vector<vector<vector<double>>>>>>> force3B(6,vector<vector<vector<vector<vector<vector<double>>>>>> (N_l3B_+1,  vector<vector<vector<vector<vector<double>>>>>(N_a, vector<vector<vector<vector<double>>>>(N_b,vector<vector<vector<double>>>(N_times,vector<vector<double>>(N_atoms,vector<double>(3) )))))); 

        		for (int i_time=0;i_time<N_times;i_time++)
			{
				for (int i_type=1;i_type<7;i_type++)
        			#pragma omp parallel for collapse(2) 
        			for (int i_a=0; i_a<N_a;i_a++)
        			for (int i_b=0; i_b<N_b;i_b++)
				for (int m=0; m<N_l3B_+1;m++)
				{

					for (int i=0;i<N_atoms;i++)
					for (int j=0;j<N_atoms;j++)
					for (int k=j+1;k<N_atoms;k++)
					if (angleTYPE[i_time][i][j][k]==i_type)
					{
					 	for (int beta=0;beta<3; beta++)
					 	{
							double tmp1,tmp2,tmp3;

					 		tmp1=-dx[i_time][j][i][beta]/distance[i_time][i][j]*frijD[i_a][i_b][i_time][i][j]*frij[i_a][i_b][i_time][i][k]*pow(cos[i_time][i][j][k],m);
							if (m!=0)
							{
						 		tmp2=frij[i_a][i_b][i_time][i][j]*frij[i_a][i_b][i_time][i][k]*m*pow(cos[i_time][i][j][k],m-1)/distance[i_time][i][j]*(cos[i_time][i][j][k]/distance[i_time][i][j]*dx[i_time][j][i][beta]);
						 		tmp3=frij[i_a][i_b][i_time][i][j]*frij[i_a][i_b][i_time][i][k]*m*pow(cos[i_time][i][j][k],m-1)/distance[i_time][i][j]*(-dx[i_time][k][i][beta]/distance[i_time][i][k]);
							}
							else
							{
						 		tmp2=0;
						 		tmp3=0;
							}
					 		double fj=tmp1+tmp2+tmp3;
					
					 		tmp1=-dx[i_time][k][i][beta]/distance[i_time][i][k]*frijD[i_a][i_b][i_time][i][k]*frij[i_a][i_b][i_time][i][j]*pow(cos[i_time][i][k][j],m);
							if (m!=0)
							{
					 		tmp2=frij[i_a][i_b][i_time][i][k]*frij[i_a][i_b][i_time][i][j]*m*pow(cos[i_time][i][j][k],m-1)/distance[i_time][i][k]*(cos[i_time][i][j][k]/distance[i_time][i][k]*dx[i_time][k][i][beta]);
					 		tmp3=frij[i_a][i_b][i_time][i][k]*frij[i_a][i_b][i_time][i][j]*m*pow(cos[i_time][i][j][k],m-1)/distance[i_time][i][k]*(-dx[i_time][j][i][beta]/distance[i_time][i][j]);
							}
							else
							{
						 		tmp2=0;
						 		tmp3=0;
							}
		
					 		double fk=tmp1+tmp2+tmp3;
					 		
							force3B[i_type-1][m][i_a][i_b][i_time][i][beta]-=fj+fk;
							force3B[i_type-1][m][i_a][i_b][i_time][j][beta]+=fj;
							force3B[i_type-1][m][i_a][i_b][i_time][k][beta]+=fk;
					 	}
					}
				}
			}
				
        		ofstream myfile3B(output_file0, ios::out | ios::trunc);

			for (int i_type=1;i_type<7;i_type++)
        		for (int i_a=0; i_a<N_a;i_a++)
        		for (int i_b=0; i_b<N_b;i_b++)
			for (int m=0; m<N_l3B_+1;m++)
			{
        		        for (int i_time=0;i_time<N_times;i_time++)
        		        for (int i=0; i<N_atoms; i++)
        		        {
        		                for (int beta=0;beta<3; beta++)
        		                {
						myfile3B <<force3B[i_type-1][m][i_a][i_b][i_time][i][beta]<<"  ";
        		                }
        		        }
        		        myfile3B<<endl;
			}
			cout<<"3B is done"<<endl;
		}
	}


        /*=== COMPUTE NB contribution  ===*/
        if (N_lNB_>3)
	{
	        #pragma omp parallel for collapse(2)
	        for (int i_a=0; i_a<N_a;i_a++)
	        for (int i_b=0; i_b<N_b;i_b++)
	        {
	                double a=list_a_[i_a];       //parameter 1 index
	                double b=list_b_[i_b];       //parameter 2 index
	                for (int i_time=0;i_time<N_times;i_time++)
	                {
	
	
	                        for (int i=0; i<N_atoms; i++)
	                        for (int j=i+1; j<N_atoms; j++)
	                        {
	                                if (distanceTYPE[i_time][i][j]!=0)
	                                {
	                                        double Rshort_tmp=Rshort[distanceTYPE[i_time][i][j]-1];
	                                        frij[i_a][i_b][i_time][i][j] =doCalc_Short(i_nature_,distance[i_time][i][j],a,b,Rcut,Rshort_tmp);
	                                        frijD[i_a][i_b][i_time][i][j]=doCalcD_Short(i_nature_,distance[i_time][i][j],a,b,Rcut,Rshort_tmp);
	
	                                        frij[i_a][i_b][i_time][j][i] =frij[i_a][i_b][i_time][i][j];
	                                        frijD[i_a][i_b][i_time][j][i]=frijD[i_a][i_b][i_time][i][j];
	                                }
	                        }
	                }
	        }

        	string output_file0=output_file;
        	output_file0.append("_NB.bin");
		fstream fileStream;
		fileStream.open(output_file0);
		if (fileStream.fail()) 
		{
		      	vector<vector<vector<vector<vector<double>>>>> rho(4, vector<vector<vector<vector<double>>>>(N_a,vector<vector<vector<double>>> (N_b,vector<vector<double>> (N_times, vector<double> (N_atoms) )))); 
		        #pragma omp parallel for collapse(2)
		        for (int i_a=0; i_a<N_a;i_a++)
		        for (int i_b=0; i_b<N_b;i_b++)
		        {
				for (int i_time=0;i_time<N_times;i_time++)
				{
		                	for (int i=0;i<N_atoms;i++)
		                	for (int j=0;j<N_atoms; j++)
			      		{
						if (whichTypeNB(type[i_time][i],distanceTYPE[i_time][i][j])>0)
						{	
							rho[whichTypeNB(type[i_time][i],distanceTYPE[i_time][i][j])-1][i_a][i_b][i_time][i]+=frij[i_a][i_b][i_time][i][j];
						}
		      			}
				}
			}
	
			vector<vector<vector<vector<vector<vector<vector<double>>>>>>> forceNB(4, vector<vector<vector<vector<vector<vector<double>>>>>> (N_lNB_-3,  vector<vector<vector<vector<vector<double>>>>>(N_a, vector<vector<vector<vector<double>>>>(N_b,vector<vector<vector<double>>>(N_times,vector<vector<double>>(N_atoms,vector<double>(3) )))))); 
	
	        	for (int i_time=0;i_time<N_times;i_time++)
			{
				for (int i_type=1;i_type<5;i_type++)
	                	#pragma omp parallel for collapse(2)
	                	for (int i_a=0; i_a<N_a;i_a++)
	                	for (int i_b=0; i_b<N_b;i_b++)
				for (int m=4;m<N_lNB_+1;m++)
				{
					for (int i=0;i<N_atoms;i++)
					for (int j=0;j<N_atoms;j++)
					{
						if (whichTypeNB(type[i_time][i],distanceTYPE[i_time][i][j])==i_type)
						{
							double tmp=1.0/distance[i_time][i][j]*m*(pow(rho[i_type-1][i_a][i_b][i_time][i],m-1)+pow(rho[i_type-1][i_a][i_b][i_time][j],m-1))*frijD[i_a][i_b][i_time][i][j];
							for (int beta=0;beta<3; beta++)
							{
								forceNB[i_type-1][m-4][i_a][i_b][i_time][i][beta]-=tmp*dx[i_time][i][j][beta];
								if (type[i_time][j]!=type[i_time][i])
									forceNB[i_type-1][m-4][i_a][i_b][i_time][j][beta]+=tmp*dx[i_time][i][j][beta];
							}
						}
					}
				}
			}
	
	                ofstream myfileNB(output_file0, ios::out | ios::trunc);
			for (int i_type=1;i_type<5;i_type++)
			for (int m=4;m<N_lNB_+1;m++)
		        for (int i_a=0; i_a<N_a;i_a++)
		        for (int i_b=0; i_b<N_b;i_b++)
			{
	                	for (int i_time=0;i_time<N_times;i_time++)
	                	for (int i=0; i<N_atoms; i++)
	                	{
	                	        for (int beta=0;beta<3; beta++)
	                	        {
						myfileNB <<forceNB[i_type-1][m-4][i_a][i_b][i_time][i][beta]<<"  ";
	                	        }
	                	}
	                	myfileNB<<endl;
			}      	
			cout<<"NB is done"<<endl;
		}
	}
}


void XMAT::doComputePot(vector<vector<double>> &Pot2B,vector<vector<double>> &Force2B,vector<double> r,int &i_func,vector<double>coeffS,double Rcut,double Rshort[3])
{
	int N_a=list_a_.size();
	int N_b=list_b_.size();
	int N_selected=0;
	if (check2B_==1)
	{
		int N_selected2B=0;
		int N_2B=0;
		for (int i_type=0;i_type<3;i_type++)
		for (int i_a=0; i_a<N_a;i_a++)
		for (int i_b=0; i_b<N_b;i_b++)
		{
                	if (abs(coeffS[i_func])>1e-10)
			{
	                	double a=list_a_[i_a];       //parameter 1 index
	                	double b=list_b_[i_b];       //parameter 2 index
				for (int i=0;i<r.size();i++)
				{
					Pot2B[i_type][i]+=coeffS[i_func]*doCalc(i_nature_,r[i],a,b,Rcut);
					Force2B[i_type][i]-=coeffS[i_func]*doCalcD(i_nature_,r[i],a,b,Rcut);
				}
				cout<<"2B     "<<i_type<<"   "<<coeffS[i_func]<<"  "<<a<<"  "<<b<<endl;
				N_selected++;
				N_selected2B++;
			}
			i_func++;
		}
	}

	int N_max=30;
        if (N_l3B_!=0)                                                                                                
        {                                                                                                             
		ofstream myfile3B("FeAu_f"+to_string(i_nature_)+".sw", ios::out | ios::trunc);

		int N_selected3B=0;
                for (int i_type=1;i_type<7;i_type++)                                                                  
		{
			int N_selected3B_pertype=0;
			string selected;
                	for (int i_a=0; i_a<N_a;i_a++)                                                                        
                	for (int i_b=0; i_b<N_b;i_b++)                                                                        
			for (int m=0; m<N_l3B_+1;m++)
                	{                                                                                                     
				if (abs(coeffS[i_func])>1e-10)
				{
			                double a=list_a_[i_a];       //parameter 1 index
			                double b=list_b_[i_b];       //parameter 2 index
					ostringstream streamObj;
					streamObj << coeffS[i_func];
					string strObj = streamObj.str();
					selected=selected+"  "+strObj+"  "+to_string(a)+"  "+to_string(b)+"  "+to_string(m);
					cout<<"3B     "<<i_type<<"   "<<strObj<<"  "<<a<<"  "<<b<<"  "<<m<<endl;
					N_selected3B++;
					N_selected3B_pertype++;
					N_selected++;
				}
				i_func++;
                	} 
			if (N_selected3B_pertype>N_max)
			{
				cout<<"Lammps software must be recompiled!"<<endl;
			}
			else
			{
				for (int i_missing=N_selected3B_pertype;i_missing<N_max;i_missing++)
					selected=selected+"  0  0 0 0";
			}
			myfile3B <<whichType3B(i_type,"Fe", "Au")<<"   "<<N_selected3B_pertype<<"  "<<Rcut<<selected<<endl;
			if (i_type==4)
				myfile3B <<whichType3B_BIS(i_type,"Fe", "Au")<<"   "<<N_selected3B_pertype<<"  "<<Rcut<<selected<<endl;
			if (i_type==6)
				myfile3B <<whichType3B_BIS(i_type,"Fe", "Au")<<"   "<<N_selected3B_pertype<<"  "<<Rcut<<selected<<endl;

		}
		myfile3B.close();
	}

		
		


        if (N_lNB_!=0)                                                                                                
        {                                                                                                             
		int N_selectedNB=0;
                for (int i_type=1;i_type<5;i_type++)
                for (int m=4; m<N_lNB_+1;m++)
                for (int i_a=0; i_a<N_a;i_a++)
                for (int i_b=0; i_b<N_b;i_b++)
                {                                                                                                     
			if (abs(coeffS[i_func])>1e-10)
			{
				double Rshort_tmp=whichShort(Rshort,i_type);
				ostringstream streamObj;
				streamObj << coeffS[i_func];
				string strObj = streamObj.str();
				string command="./GenEAM.py  "+to_string(N_selectedNB)+"  "+to_string(i_type)+"  "+to_string(m)+"  "+to_string(list_a_[i_a])+"  "+to_string(list_b_[i_b])+"  "+strObj+"  "+to_string(Rcut)+"  "+to_string(i_nature_)+"  "+to_string(Rshort_tmp);
				system(command.c_str());
				cout<<"NB     "<<i_type<<"   "<<coeffS[i_func]<<"   "<<list_a_[i_a]<<"  "<<list_b_[i_b]<<"  "<<m<<endl;
				N_selectedNB++;
				N_selected++;
			}
			i_func++;
                } 
        }                                                                                                             

}



