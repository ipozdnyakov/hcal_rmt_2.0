#include<iostream>
#include<iomanip>
#include<cstdio>
#include<fstream>
#include<vector>
#include<TCanvas.h>
#include<TString.h>
#include<TFile.h>
#include<TH2F.h>

#include"input.h"

using namespace std;

int main(){

	streambuf *console = cout.rdbuf();
	ofstream gain_drifts_SiMP_HE_s("./output/gain_drift_cells_of_SiMP_HE_from_mapping");

	cout.rdbuf(gain_drifts_SiMP_HE_s.rdbuf());
	Ncell("cells_of_SiMP_HE_from_mapping");
	gain_drifts_SiMP_HE_s.close();
	cout.rdbuf(console);

	cout << "Cells of interest done\n";

        return (EXIT_SUCCESS);
}

//--------------------------------------------------
//BELOW THE SOURCE CODE FOR FUNCTIONS
//--------------------------------------------------

void Ncell(TString file_name){

	int subd = 0, ieta = 0, iphi = 0;
	vector<vector<double> > drifts;
	vector<vector<int> > cells;
	vector<int> cell;

	FILE *file = fopen("./output/"+file_name,"r");

	while(!feof(file)){

     		fscanf(file, "%d %d %d", &subd, &ieta, &iphi);	

		//position of a cell in eta is translated to the bin number
		if(ieta > 0){
			ieta = ieta + 41;
		}else{
			ieta = ieta + 42;
		}

		cell.push_back(subd);
		cell.push_back(ieta);
		cell.push_back(iphi);
		if(!feof(file)) cells.push_back(cell);
		cell.clear();
	}

	for(int i = 0; i < cells.size(); i++){
		cout << setw(12) << "(" << cells[i][0] << ";" << cells[i][1] << ";" << cells[i][2] << ")\t";
	}
		cout << "\n";

	for(int i = 0; i < nruns; i++){
		drifts.push_back(Nrun_cell(cells, i));
	}

	TFile   *out  = new TFile("./plots/HCAL_evol_" + file_name + ".root", "RECREATE");
	TCanvas *cnvs =  new TCanvas("HCAL_evol");
	TH1D	*evol; 
	TString name = "cell_";
	int color = 1;

	for(int i = 0; i < drifts[0].size(); i++){
		color = i%50;
		if((color == 0)||(color == 10)||(color == 19)) color = 2;
		evol = new TH1D("Gain_Drifts","Gain_Drifts", nruns, 0, nruns);
		evol->Sumw2();
		evol->SetStats(0);
		name += i;
		evol->SetTitle("");
		evol->SetName(name);
		name = "cell_";
		evol->SetLineColor(color);
		evol->SetLineWidth(1);
		evol->SetMarkerColor(color);
		evol->SetMarkerStyle(7);
		for(int j = 0; j < drifts.size(); j++){
			evol->Fill(j+0.5, drifts[j][i]);
			if(j%1 == 0){
				evol->GetXaxis()->SetBinLabel(j+1,dates[j]);
			}else{
				evol->GetXaxis()->SetBinLabel(j+1,"");
			}
		}
		evol->DrawClone("samehistp0l");
		evol->Write();
		delete evol;
	}
	cnvs->Write();
	delete out;
}

vector<double> Nrun_cell(vector<vector<int> > cells, int run){

//loop over all cells for the run with regard to the first run

	vector<double> drifts;
	int subd = 0, ieta = 0, iphi = 0;

//declaration of reference and analysed files and TH2D histos for the data from them

	TFile *run_ref = new TFile( "/afs/cern.ch/work/k/kodolova/public/RDMweb/histos/LED_" + runs[0] + ".root", "READ");
	TFile *run_ana = new TFile( "/afs/cern.ch/work/k/kodolova/public/RDMweb/histos/LED_" + runs[run] + ".root", "READ");
	TH2F  *ampl_ref, *ampl_ana;
	TString	hist_name;

	double drift = 1.;

	for(int i = 0; i < cells.size(); i++){
		subd = cells[i][0];
		ieta = cells[i][1];
		iphi = cells[i][2];

		//choosing data (histograms) from files by names according to input subdetector number
		hist_name = subd_depth_name[subd];

		//reading data from files, normalyzing and calculation of gain drift
		//and output of result

		if ((ampl_ref = (TH2F*)run_ref->Get(hist_name))&&(ampl_ana = (TH2F*)run_ana->Get(hist_name))){
			ampl_ana->Scale(1./ampl_ana->GetEntries());
			ampl_ref->Scale(1./ampl_ref->GetEntries());

			if (ampl_ref->GetBinContent(ieta, iphi) != 0.){
				drift = ampl_ana->GetBinContent(ieta, iphi)/ampl_ref->GetBinContent(ieta, iphi);
				if((drift > 0.)&&(drift < 2.)){
					cout << setw(12) << drift << "\t";
				}else{
					cout << setw(12) << "%%%" << "\t";
					drift = 1;
				}
			}else{
				cout << "ref_to_zero\t";
				drift = 1;
			}
		}

		drifts.push_back(drift);
	}

	cout << "\n";

	run_ref->Close();
	run_ana->Close();
	delete run_ref;
	delete run_ana;
	return drifts;
}
