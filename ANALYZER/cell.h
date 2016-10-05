#include<vector>
#include"TH1D.h"

#ifndef _CELL_H
#define	_CELL_H
using namespace std;

class Cell {
//Event is a set of jets and parameters
private:
	vector<int> event_count;
	int run_count = 0;
public:
	int subd = -1, depth = -1, eta = 0, phi = -1;
	TSTring subd_name = "not defined";

	vector<TH1D> gain;	 	
	vector<double> drift;	 	
	bool change_trend = false;

	Cell(int subd, int eta, int phi): subd(subd), eta(eta), phi(phi) { }
	Cell(TString subd_name, int depth, int eta, int phi): subd_name(subd_name), depth(depth), eta(eta), phi(phi) { }

	void AddEvent();
	void AddRun();
};
#endif	/* _CELL_H */
