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
	ofstream bad_runs_s("./output/bad_runs");
	ofstream bad_cells_s("./output/bad_cells");
	ofstream gain_drifts_s("./output/gain_drifts");
	ofstream gain_drifts_interest_s("./output/gain_drift_cells_of_interest");
	ofstream gain_drifts_HB_s("./output/gain_drift_HB");
	ofstream gain_drifts_HE_s("./output/gain_drift_HE");
	ofstream gain_drifts_HF_s("./output/gain_drift_HF");
	ofstream gain_drifts_HO_s("./output/gain_drift_HO");

	//return vector of vectors with number of bad cells for each run for each subdetector
	//only for runs with atleast 1 subdetector has more than 50% bad cells
	vector<vector<Int_t> > bad_runs;
	bad_runs = Nrun_HCAL(0.05);

cout << "Bad runs are found\n";

	//print list of bad runs into file
	cout.rdbuf(bad_runs_s.rdbuf());
	for(int i = 0; i < bad_runs.size(); i++){
		for(int j = 0; j < bad_runs[i].size(); j++){
			cout << bad_runs[i][j] << "\t";
		}
		cout << "\n";
	}
	bad_runs_s.close();
	cout.rdbuf(console);

cout << "Bad runs are written\n";

/*	cout.rdbuf(bad_cells_s.rdbuf());
	Drun_HCAL(0, nruns-1, 0.05, false);
	bad_cells_s.close();
	cout.rdbuf(console);

cout << "Bad cells are found\n";

	cout.rdbuf(gain_drifts_s.rdbuf());
	Ncell("bad_cells");
	gain_drifts_s.close();
	cout.rdbuf(console);

cout << "Bad cells are done\n";*/

/*	cout.rdbuf(gain_drifts_interest_s.rdbuf());
	Ncell("cells_of_interest");
	gain_drifts_interest_s.close();
	cout.rdbuf(console);

cout << "Cells of interest done\n";*/

	cout.rdbuf(gain_drifts_HB_s.rdbuf());
	Ncell("cells_of_HB");
	gain_drifts_HB_s.close();
	cout.rdbuf(console);

cout << "HB is done\n";

	cout.rdbuf(gain_drifts_HE_s.rdbuf());
	Ncell("cells_of_HE");
	gain_drifts_HE_s.close();
	cout.rdbuf(console);

cout << "HE is done\n";

	cout.rdbuf(gain_drifts_HF_s.rdbuf());
	Ncell("cells_of_HF");
	gain_drifts_HF_s.close();
	cout.rdbuf(console);

cout << "HF is done\n";

	cout.rdbuf(gain_drifts_HO_s.rdbuf());
	Ncell("cells_of_HO");
	gain_drifts_HO_s.close();
	cout.rdbuf(console);

cout << "HO is done\n";


        return (EXIT_SUCCESS);
}

//--------------------------------------------------
//BELOW THE SOURCE CODE FOR FUNCTIONS
//--------------------------------------------------

vector<vector<Int_t> > Nrun_HCAL(double threshold = 0.03) {

	vector<vector<Int_t> > bad_runs;
	vector<Int_t> bads;

	//cout monitoring parameters and names for columns
	cout << "RefRun=" << runs[0] << " threshold=" << threshold << ":\n";
	cout << "\t\tHB1\tHB2\tHB_tot\tHE1\tHE2\tHE3\tHE_tot\tHF1\tHF2\tHF_tot\tHO4\t:\tTOT\n";

	//loop over all runs
	for(int i = 0; i < nruns; i++){
		cout << i << ") ";
		//return vector of number of bad cells in each subdetector
		bads = Drun_HCAL(0, i, threshold, true);
		if((bads[1]+bads[2]+bads[3]+bads[4]+bads[5]+bads[6]+bads[7]+bads[8]) > 0) bad_runs.push_back(bads);
	}

	return bad_runs;
}

vector<Int_t> Drun_HCAL(int run1_i = 0, int run2_i = 1, double threshold = 0.03, bool from_nrun = false){

	bool contrast = true; // set for drift > 1+threshold high value (1.2),
			      // for for < 1-threshold low value (0.8)
			      // make plots more contrast

	vector<Int_t> bads;
	bads.push_back(runs_numbers[run2_i]);

	if(!from_nrun) cout << "Threshold= " << threshold << "\tRefRun= " << runs[run1_i] << "\tAnalazedRun= ";
	cout << " " << runs[run2_i] << ":\t";
	if(!from_nrun) cout << "\n";

	//declaration of reference and analysed files and TH2D histos for the data from them, declaration of other variables
	TFile *run_ref[8];
	for(int subd = 0; subd < 8; subd++){
		run_ref[subd] = new TFile("/afs/cern.ch/work/k/kodolova/public/RDMweb/histos/LED_" + ref_runs[subd] + ".root", "READ");
	}
	TFile *run_ana = new TFile( "/afs/cern.ch/work/k/kodolova/public/RDMweb/histos/LED_" + runs[run2_i] + ".root", "READ");
	TFile *output = new TFile("./plots/Drun_" + runs[run1_i] + "_" + runs[run2_i] + ".root", "RECREATE");
	TCanvas *cnvs;
	TH2F	*ampl_ref[8], *ampl_ana[8], *ampl_ratio[8];
	TH1F	*ratio_distr[8];
	int chan_count = 0; 		//all channels
	int subd_cal_count = 0;  	//channels need to be calibrated in subdetector
	int cal_count = 0;  		//channels need to be calibrated in subdetector X depth
	int tot_cal_count = 0;  	//channels need to be calibrated in total
	double drift = 0.;
	TString	hist_name, hist_title;
	TString	titles[] = {"HB1", "HB2", "HE1", "HE2", "HE3", "HF1", "HF2", "HO4"};

	//loop over all subdetectors
	for(int subd = 0; subd < 8; subd++){
		//run_ref = new TFile("/afs/cern.ch/work/k/kodolova/public/RDMweb/histos/LED_" + ref_runs[subd] + ".root", "READ");
		hist_name = subd_depth_name[subd];

		//if requiered histograms are presented in the LED files - read them
		if ((ampl_ref[subd] = (TH2F*)run_ref[subd]->Get(hist_name))&&(ampl_ana[subd] = (TH2F*)run_ana->Get(hist_name))){
			//normalisation and calculation of gain drift (ratio)	
			ampl_ana[subd]->Scale(1./ampl_ana[subd]->GetEntries());
			ampl_ref[subd]->Scale(1./ampl_ref[subd]->GetEntries());
			ampl_ratio[subd] = (TH2F*)ampl_ref[subd]->Clone("Ratio");
      			ampl_ratio[subd]->Divide(ampl_ana[subd], ampl_ref[subd], 1, 1, "B");

			//declaration 1D histo for drifts distribution
			ratio_distr[subd] = new TH1F("drift_" + subd, "drift_" + subd, 50, 0.8, 1.2);

			//loop over all cells			
			for(int xi = 1; xi <= ampl_ref[subd]->GetNbinsX(); xi++){
				for(int yi = 1; yi <= ampl_ref[subd]->GetNbinsY(); yi++){

					//analysing if drift is non-zero and large
					drift = ampl_ratio[subd]->GetBinContent(xi, yi);
					if(drift != 0.){

						//increase counters for channels counted in this subdetector
						chan_count++;

						//analysing if drift beyond the threshold
						if( fabs(drift - 1) > threshold){

							if(contrast){
								if(drift > (1 + threshold)) ampl_ratio[subd]->SetBinContent(xi, yi, 1.2);
								if(drift < (1 - threshold)) ampl_ratio[subd]->SetBinContent(xi, yi, 0.8);
							}

							//increase counters for channels what need to be calibrated
							cal_count++;
							//output bad cells (and converting from bin nuber to eta)
							if(!from_nrun) cout << subd << "\t";
						        if( xi > 41){
						                if(!from_nrun) cout << xi - 41 << "\t";
							}else{
						              	if(!from_nrun) cout << xi - 42 << "\t";
						        }
							if(!from_nrun) cout << yi << "\n";

						};

						//fill 1D histo for drifts distribution
						ratio_distr[subd]->Fill(drift);
					};
				};
			};

			//naming and refining histograms
			hist_title = titles[subd];
			ampl_ratio[subd]->GetZaxis()->SetRange(0.8,1.2);
			ampl_ratio[subd]->SetMinimum(0.8);
			ampl_ratio[subd]->SetMaximum(1.2);
			ampl_ratio[subd]->SetStats(0000);
			ampl_ratio[subd]->SetTitle(hist_title);
			ampl_ratio[subd]->SetName(hist_title+"_ratio");
			ratio_distr[subd]->SetTitle(hist_title);					
			ratio_distr[subd]->SetName(hist_title+"_distr");
		}

		//increase total number of channels what need to be calibrated
		tot_cal_count += cal_count;
		subd_cal_count += cal_count;

		//output the results for each subdetector X depth
		if(runs[run2_i] <= ref_runs[subd]){
				if((from_nrun)&&(runs[run2_i] < ref_runs[subd])) cout << "---\t";
				if((from_nrun)&&(runs[run2_i] == ref_runs[subd])) cout << "ref\t";
				tot_cal_count -= cal_count;
				subd_cal_count -= cal_count;
				cal_count = 0;
		}else{
			if(chan_count > 0){
				if(from_nrun) cout << cal_count << "\t";
			}else{
				cout << "empty_run\t";
			}
		}
		//check if run is bad
		if((double)cal_count/subd_depth_cells[subd] > 0.5){
			bads.push_back(cal_count);
		}else{
			bads.push_back(0);
		}
		// output results for subdetector
		if((subd == 1)||(subd == 4)||(subd == 6)){
			if(from_nrun) cout << subd_cal_count << "\t";
			subd_cal_count = 0;
		}

		//return counters to zero
		cal_count = 0;
		chan_count = 0;


		//write all histos to .root file and to .gif files

		cnvs = new TCanvas(titles[subd]);
		ampl_ratio[subd]->Draw("colz");
		if(!from_nrun) cnvs->Print("./plots/Drun_" + runs[run1_i] + "_" + runs[run2_i] + "_2D" + titles[subd] + ".gif");
		ratio_distr[subd]->Draw("");
		if(!from_nrun) cnvs->Print("./plots/Drun_" + runs[run1_i] + "_" + runs[run2_i] + "_1D" + titles[subd] + ".gif");
		delete cnvs;

		ampl_ratio[subd]->Write();
		ratio_distr[subd]->Write();

		run_ref[subd]->Close();
		delete run_ref[subd];
	}

	run_ana->Close();
	delete run_ana;
	output->Close();
	delete output;

	if(from_nrun) cout << ":\t" << tot_cal_count << "\n";

	return bads;
}

void Ncell(TString file_name){

	int subd = 0, ieta = 0, iphi = 0;
	vector<vector<double> > drifts;
	vector<vector<int> > cells;
	vector<int> cell;

	FILE *file = fopen("./output/"+file_name,"r");

     	fscanf(file, "Threshold= %*f RefRun= 271961 AnalazedRun=  "+ runs[nruns-1] + ":");	

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
		cells.push_back(cell);
		cell.clear();
	}

	for(int i = 0; i < cells.size(); i++){
		cout << setw(11) << "(" << cells[i][0] << ";" << cells[i][1] << ";" << cells[i][2] << ")\t";
	}

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
			if(j%4 == 0){
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
					cout << setw(8) << drift << "\t";
				}else{
					cout << setw(8) << "%%%" << "\t";
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
