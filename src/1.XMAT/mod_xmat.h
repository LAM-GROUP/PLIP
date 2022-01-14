#include <vector>
#include <fstream>
#include <iostream>
#include <random>
#include <cmath>


using namespace std;

/**
 * @brief 
 * 
 */
class XMAT
{
    private:
        int i_nature_; 
        int check2B_; 
	int N_l3B_;	
	int N_lNB_;	

        vector<double> list_a_;  // List of function parameters
	vector<double> list_b_;  // List of function parameters

    public:
	XMAT(const strucXYZ &xyz,double a_init,double a_end,double a_step,double b_init,double b_end,double b_step,int i_nature,int check2B, int N_l3B,int N_lNB);
	void doXMAT(const strucXYZ &xyz,string output_file,  double Rcut,double Rshort[3]);
	void doComputePot(vector<vector<double>> &Pot2B,vector<vector<double>> &Force2B,vector<double> r,int &i_func,vector<double>coeffS,double Rcut,double Rshort[3]);

	int getCheck2B() const {return check2B_;};
	int getN_l3B()   const {return N_l3B_;};
	int getInat()    const {return i_nature_;};
	vector<double> getA()       const {return list_a_;};
	vector<double> getB()       const {return list_b_;};
};

