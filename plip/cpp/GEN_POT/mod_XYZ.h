#include <vector>
#include <fstream>
#include <iostream>
#include <random>
#include <cmath>

using namespace std;

class strucXYZ 
{
    private:
        int N_atoms_; 	
        int N_times_; 	
	vector<vector<vector<double>>>  pos_;
	vector<vector<vector<vector<double>>>>  dx_;
	vector<vector<vector<double>>>  distance_;
	vector<vector<vector<int>>>  distanceTYPE_;
	vector<vector<int>>  type_;
	vector<vector<vector<vector<double>>>> angles_;
	vector<vector<vector<vector<int>>>> anglesTYPE_;
	vector<double> L_;
    public:
	strucXYZ();
	void doReadXYZ(string input_file);
	void doComputeDistances(double R_cut);
	void doComputeAngles(double R_cut);
	void doComputeAnglesOLD(double R_cut);

	int GetN_atoms() const {return N_atoms_;};
	int GetN_times() const {return N_times_;};


	vector<double> GetL() const {return L_;};

//	vector<vector<double>>  	GetPos(int i_time) 	  const {return pos_[i_time];};
//	vector<vector<vector<double>>>	GetDX(int i_time) const {return dx_[i_time];};
//	vector<vector<double>>		GetDistance(int i_time) const {return distance_[i_time];};
//	vector<vector<int>>		GetDistanceTYPE(int i_time) const {return distanceTYPE_[i_time];};
//	vector<vector<vector<double>>>	GetAngle(int i_time) const {return angles_[i_time];};
//	vector<vector<vector<int>>>	GetAngleTYPE(int i_time) const {return anglesTYPE_[i_time];};
//	vector<int>			GetType(int i_time) const {return type_[i_time];};

	vector<vector<vector<double>>>  	GetPos() 	  const {return pos_;};
	vector<vector<vector<vector<double>>>>	GetDX() const {return dx_;};
	vector<vector<vector<double>>>		GetDistance() const {return distance_;};
	vector<vector<vector<int>>>		GetDistanceTYPE() const {return distanceTYPE_;};
	vector<vector<vector<vector<double>>>>	GetAngle() const {return angles_;};
	vector<vector<vector<vector<int>>>>	GetAngleTYPE() const {return anglesTYPE_;};
	vector<vector<int>>			GetType() const {return type_;};
};


