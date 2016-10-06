#include<vector>
#include"TH1D.h"

#ifndef _CELL_H
#define	_CELL_H
using namespace std;

class Cell {

private:
	vector<int> event_count;
	int run_count;
public:
	int subd, depth, eta, phi;
	TString subd_name;

	vector<TH1D> sum_ampl;	 	
	vector<double> drift;	 	
	bool change_trend;

	Cell();
	Cell(int subd, int eta, int phi);
	Cell(TString subd_name, int depth, int eta, int phi);

	void AddEvent(double);
	void AddRun();
};
#endif	/* _CELL_H */
