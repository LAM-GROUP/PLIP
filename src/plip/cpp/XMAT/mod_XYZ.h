#include <vector>
#include <fstream>
#include <iostream>
#include <random>
#include <cmath>

using namespace std;

/**
 * @brief This class stores the x,y,z coordinates of the atoms and computes pairwise distances and angles.
 * 
 */
class strucXYZ 
{
    private:
        int N_atoms_; 								/**< Get number of atoms */
        int N_times_; 								///< XXXX
	vector<vector<vector<double>>>  pos_;			/**< Position vectors*/
	vector<vector<vector<vector<double>>>>  dx_;
	vector<vector<vector<double>>>  distance_;
	vector<vector<vector<int>>>  distanceTYPE_;
	vector<vector<int>>  type_;
	vector<vector<vector<vector<double>>>> angles_;
	vector<vector<vector<vector<int>>>> anglesTYPE_;
	double L_[3][3] {}; ///< Lattice parameter values
    public:
	/// Constructor
	strucXYZ();
	                        
	/**
	 * @brief The function is used to read the input file
	 * 
	 * @param input_file  The filename for the input 
	 * 
	 * @param species Atom types present
	 */
	void doReadXYZ(string input_file, vector<string> species);
	/**
	 * @brief Function used to compute pairwise distances
	 * 
	 * @param R_cut The cutoff in Å for distances
	 */
	void doComputeDistances(double R_cut);
	/**
	 * @brief Function to compute angles
	 * 
	 * @param R_cut The cutoff in Å for distances
	 */
	void doComputeAngles(double R_cut);

	/**
	 * @brief Function to return number of atoms
	 * 
	 * @return int 
	 */
	int GetN_atoms() const {return N_atoms_;};
	/**
	 * @brief  Get number of atoms*
	 * 
	 * @return int 
	 */
	int GetN_times() const {return N_times_;};



//	vector<vector<double>>  	GetPos(int i_time) 	  const {return pos_[i_time];};
//	vector<vector<vector<double>>>	GetDX(int i_time) const {return dx_[i_time];};
//	vector<vector<double>>		GetDistance(int i_time) const {return distance_[i_time];};
//	vector<vector<int>>		GetDistanceTYPE(int i_time) const {return distanceTYPE_[i_time];};
//	vector<vector<vector<double>>>	GetAngle(int i_time) const {return angles_[i_time];};
//	vector<vector<vector<int>>>	GetAngleTYPE(int i_time) const {return anglesTYPE_[i_time];};
//	vector<int>			GetType(int i_time) const {return type_[i_time];};

	/**
	 * @brief Get the Pos object
	 * 
	 * @return vector<vector<vector<double>>> 
	 */
	vector<vector<vector<double>>>  	GetPos() 	  const {return pos_;};
	/**
	 * @brief Get the position vectors
	 * 
	 * @return vector<vector<vector<vector<double>>>> 
	 */
	vector<vector<vector<vector<double>>>>	GetDX() const {return dx_;};
	/**
	 * @brief Get the Distance object
	 * 
	 * @return vector<vector<vector<double>>> 
	 */
	vector<vector<vector<double>>>		GetDistance() const {return distance_;};
	/**
	 * @brief Get the Distance T Y P E object
	 * 
	 * @return vector<vector<vector<int>>> 
	 */
	vector<vector<vector<int>>>		GetDistanceTYPE() const {return distanceTYPE_;};
	/**
	 * @brief Get the Angle object
	 * 
	 * @return vector<vector<vector<vector<double>>>> 
	 */
	vector<vector<vector<vector<double>>>>	GetAngle() const {return angles_;};
	/**
	 * @brief Get the Angle T Y P E object
	 * 
	 * @return vector<vector<vector<vector<int>>>> 
	 */
	vector<vector<vector<vector<int>>>>	GetAngleTYPE() const {return anglesTYPE_;};
	/**
	 * @brief Get the Type object
	 * 
	 * @return vector<vector<int>> 
	 */
	vector<vector<int>>			GetType() const {return type_;};
};


