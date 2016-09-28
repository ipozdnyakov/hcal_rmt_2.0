#include<iostream>
#include<fstream>
#include<TCanvas.h>
#include<TString.h>
#include<TFile.h>
#include<TH2F.h>

using namespace std;

TString Choose_subdetector(int subd);
void Drun_cell(TString run1, TString run2, int subd, int ieta, int iphi);
void Nrun_cell(int subd, int ieta, int iphi);
void Nrun_HCAL(double threshold);
void Drun_HCAL(TString run1, TString run2, double threshold, bool from_nrun);
void Cells_evol();
void Ncell();

//array of runs numbers and total nuber of them

TString runs[] = {
"271961","272303","272312","272513","272533","272581","272847","272946","272947","273117",
"273234","273316","273455","273557","273604","273616","273648","273757","273763","273770",
"273894","273961","274752","274890","275013","275265","275430","275457","275697","275807",
"275864","276081","276402","276416","276461","276462","276463","276476","276520","276531",
"276554","276600","276678","277132","277172","277336","277511","278596","278897","279058",
"279096","279607","279700","279723","279785","279800","279908","280200","280262","280379"
};

Int_t nruns = sizeof(runs)/sizeof(runs[0]);

TString ref_runs[] = {"271961","271961","271961","271961","271961","272303","272303","272303"};

//arrays of names and numbers of cells in subdetectors

TString subd_depth_name[] = {
	"h_mapDepth1ADCAmpl_HB",
	"h_mapDepth2ADCAmpl_HB",
	"h_mapDepth1ADCAmpl_HE",
	"h_mapDepth2ADCAmpl_HE",
	"h_mapDepth3ADCAmpl_HE",
	"h_mapDepth1ADCAmpl_HF",
	"h_mapDepth2ADCAmpl_HF",
	"h_mapDepth4ADCAmpl_HO",
};

Int_t subd_depth_cells[] = {2304,288,1124,1080,288,864,864,2160};
Int_t subd_cells[] = {2592,2592,1728,2160};

int main(){

	streambuf *console = cout.rdbuf();
	ofstream bad_runs("./input/bad_runs");
	ofstream bad_cells("./input/bad_cells");

	Nrun_HCAL(0.05);

	cout.rdbuf(bad_cells.rdbuf());
	Drun_HCAL(runs[0], runs[nruns-1], 0.05, false);
	cout.rdbuf(console);

        return (EXIT_SUCCESS);
}

//--------------------------------------------------
//BELOW THE SOURCE CODE FOR FUNCTIONS
//--------------------------------------------------

void Nrun_HCAL(double threshold = 0.03) {

	//cout monitoring parameters and names for columns
	cout << "RefRun=" << runs[0] << " threshold=" << threshold << ":\n";
	cout << "\t\tHB1\tHB2\tHB_tot\tHE1\tHE2\tHE3\tHE_tot\tHF1\tHF2\tHF_tot\tHO4\t:\tTOT\n";

	//loop over all runs
	for(int i = 0; i < nruns; i++){
		cout << i << ") ";
		Drun_HCAL(runs[0], runs[i], threshold, true);
	}
}

void Drun_HCAL(TString run1 = "271961", TString run2 = "276678", double threshold = 0.03, bool from_nrun = false){

	if(!from_nrun) cout << "Threshold= " << threshold << "\tRefRun= " << run1 << "\tAnalazedRun= ";
	cout << " " << run2 << ":\t";
	if(!from_nrun) cout << "\n";

	//declaration of reference and analysed files and TH2D histos for the data from them, declaration of other variables
	TFile *run_ref;// = new TFile( "/afs/cern.ch/work/k/kodolova/public/RDMweb/histos/LED_" + run1 + ".root", "READ");
	TFile *run_ana = new TFile( "/afs/cern.ch/work/k/kodolova/public/RDMweb/histos/LED_" + run2 + ".root", "READ");
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
		run_ref = new TFile("/afs/cern.ch/work/k/kodolova/public/RDMweb/histos/LED_" + ref_runs[subd] + ".root", "READ");
		hist_name = Choose_subdetector(subd);

		//if requiered histograms are presented in the LED files - read them
		if ((ampl_ref[subd] = (TH2F*)run_ref->Get(hist_name))&&(ampl_ana[subd] = (TH2F*)run_ana->Get(hist_name))){
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
		if(run2 <= ref_runs[subd]){
				if((from_nrun)&&(run2 < ref_runs[subd])) cout << "---\t";
				if((from_nrun)&&(run2 == ref_runs[subd])) cout << "ref\t";
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
		// output results for subdetector
		if((subd == 1)||(subd == 4)||(subd == 6)){
			if(from_nrun) cout << subd_cal_count << "\t";
			subd_cal_count = 0;
		}

		//return counters to zero
		cal_count = 0;
		chan_count = 0;
	}

	if(from_nrun) cout << ":\t" << tot_cal_count << "\n";

	//write all histos to .root file and to .gif files
	//by default .gif files printing is commented to avoid unwanted output to the terminal - one can uncomment them if needed

	TFile *output = new TFile("./output/Drun_" + run1 + "_" + run2 + ".root", "RECREATE");
	TCanvas *cnvs;
	for(int i = 0; i < 8; i++){
		cnvs = new TCanvas(titles[i]);
		ampl_ratio[i]->Draw("colz");
		if(!from_nrun) cnvs->Print("./output/Drun_" + run1 + "_" + run2 + "_2D" + titles[i] + ".gif");
		ratio_distr[i]->Draw("");
		if(!from_nrun) cnvs->Print("./output/Drun_" + run1 + "_" + run2 + "_1D" + titles[i] + ".gif");
		delete cnvs;
		ampl_ratio[i]->Write();
		ratio_distr[i]->Write();
	}
}

void Ncell(){

	int subd = 0, ieta = 0, iphi = 0;

	// for all bad cellc from file ./input/bad_cells

	FILE *file = fopen("./input/bad_cells","r");
	
	while( fscanf(file,"%i %i %i",&subd,&ieta,&iphi) == 3 ){
		Nrun_cell(subd, ieta, iphi);
		if(feof(file)) break;
	}

	cout << "\n";
}

void Cells_evol(){

//read list of bad cells from file

	FILE *file;
	file = fopen("/afs/cern.ch/user/i/ivanp/2016_HCAL_gains_monitoring/input/bad_cells","r");

//loop over all bad cells from the file

	int subd, ieta, iphi;
	while(!feof(file)){
     		fscanf(file, "%d\t%d\t%d", &subd, &ieta, &iphi);

	//loop over all runs with regard to the first one

		cout << "Time evol of gain drift for cell in subdetector " << subd << " with ieta=" << ieta << " iphi=" << iphi << " : ";
		Nrun_cell(subd, ieta, iphi);
	}
}

void Nrun_cell(int subd, int ieta, int iphi){

//loop over all runs with regard to the first one

	cout << "(" << subd << ";" << ieta << ";" << iphi << ")\t";

	for(int i = 1; i < nruns; i++){
		Drun_cell(runs[0], runs[i], subd, ieta, iphi);
	}
cout << "\n";
}

void Drun_cell(TString run1 = "271961", TString run2 = "273961", int subd = 0, int ieta = 0, int iphi = 0){

//position of a cell in eta is translated to the bin number

	if(ieta > 0){
		ieta = ieta + 41;
	}else{
		ieta = ieta + 42;
	}

//declaration of reference and analysed files and TH2D histos for the data from them

	TFile *run_ref = new TFile( "/afs/cern.ch/work/k/kodolova/public/RDMweb/histos/LED_" + run1 + ".root", "READ");
	TFile *run_ana = new TFile( "/afs/cern.ch/work/k/kodolova/public/RDMweb/histos/LED_" + run2 + ".root", "READ");
	TH2F  *ampl_ref, *ampl_ana;
	TString	hist_name;

//choosing data (histograms) from files by names according to input subdetector number

	hist_name = Choose_subdetector(subd);

//reading data from files, normalyzing and calculation of gain drift
//and output of result

	double drift = 0.;

	if ((ampl_ref = (TH2F*)run_ref->Get(hist_name))&&(ampl_ana = (TH2F*)run_ana->Get(hist_name))){
		ampl_ana->Scale(1./ampl_ana->GetEntries());
		ampl_ref->Scale(1./ampl_ref->GetEntries());

		if (ampl_ref->GetBinContent(ieta, iphi) != 0.){
			drift = ampl_ana->GetBinContent(ieta, iphi)/ampl_ref->GetBinContent(ieta, iphi);
			cout << drift << "\t";
		}else{
			cout << "ref_to_zero\t";
		}
	}
//	cout << "\n";
run_ref->Close();
run_ana->Close();
}

TString Choose_subdetector(int subd = 0){
TString hist_name;
        if (subd == 0){
                hist_name = "h_mapDepth1ADCAmpl_HB";
        }else if(subd == 1){
                hist_name = "h_mapDepth2ADCAmpl_HB";
        }else if(subd == 2){
                hist_name = "h_mapDepth1ADCAmpl_HE";
        }else if(subd == 3){
                hist_name = "h_mapDepth2ADCAmpl_HE";
        }else if(subd == 4){
                hist_name = "h_mapDepth3ADCAmpl_HE";
        }else if(subd == 5){
                hist_name = "h_mapDepth1ADCAmpl_HF";
        }else if(subd == 6){
                hist_name = "h_mapDepth2ADCAmpl_HF";
        }else if(subd == 7){
                hist_name = "h_mapDepth4ADCAmpl_HO";
        }
return hist_name;
}
