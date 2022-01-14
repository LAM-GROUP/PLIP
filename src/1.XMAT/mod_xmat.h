#include <vector>
#include <fstream>
#include <iostream>
#include <random>
#include <cmath>


using namespace std;

/**
 * @brief This class is used to compute and manipulate the description matrix.
 * 
 * 
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
	/**
	 * @brief Constructor for XMAT
	 * 
	 * @param xyz  Object of class strucXYZ. @see strucXYZ 
	 * @param a_init Minimum value for parameter a.
	 * @param a_end  Max value for parameter a.
	 * @param a_step  step size for parameter a.
	 * @param b_init Minimum value for parameter b.
	 * @param b_end Max value for parameter b.
	 * @param b_step step size for parameter b.
	 * @param i_nature  type of potential used. @see 
	 * @param check2B  value for 2-body.
	 * @param N_l3B   value for 3-body.
	 * @param N_lNB  value for N-body.
	 */
	XMAT(const strucXYZ &xyz,double a_init,double a_end,double a_step,double b_init,double b_end,double b_step,int i_nature,int check2B, int N_l3B,int N_lNB);
	
	/**
	 * @brief Generates XMAT
	 * 
	 * @param xyz  Object of class strucXYZ.
	 * @param output_file  filename for output.
	 * @param Rcut Cut-off value for 2-body.
	 * @param Rshort Cut-off value for N-body.
	 */
	void doXMAT(const strucXYZ &xyz,string output_file,  double Rcut,double Rshort[3]);
	/**
	 * @brief  Generates potential.
	 * 
	 * @param Pot2B  Storage for 2B potential values
	 * @param Force2B Storage for 2B potential derivative values
	 * @param r  One dimensional grid of interatomic distances
	 * @param i_func XXXX
	 * @param coeffS coefficiemt values after Lasso lars.
	 * @param Rcut Cut-off value for 2-body.
	 * @param Rshort Cut-off value for N-body.
	 */
	void doComputePot(vector<vector<double>> &Pot2B,vector<vector<double>> &Force2B,vector<double> r,int &i_func,vector<double> coeffS,double Rcut,double Rshort[3]);
	/**
	 * @brief Get the value for check2B_
	 * 
	 * @return int 
	 */
	int getCheck2B() const {return check2B_;};
	/**
	 * @brief Get the value for number of three-body
	 * 
	 * @return int 
	 */
	int getN_l3B()   const {return N_l3B_;};
	/**
	 * @brief get value for i_nature_
	 * 
	 * @return int 
	 */
	int getInat()    const {return i_nature_;};
	/**
	 * @brief a parameter values
	 * 
	 * @return vector<double> 
	 */
	vector<double> getA()       const {return list_a_;};
	/**
	 * @brief b parameter values
	 * 
	 * @return vector<double> 
	 */
	vector<double> getB()       const {return list_b_;};
};

