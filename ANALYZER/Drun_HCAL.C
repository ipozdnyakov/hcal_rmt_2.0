void Drun_HCAL(TString run1 = "271961", TString run2 = "273961", double threshold = 0.03){

cout << "Run-" << run2 << "/Run-" << run1 << "-t-" << threshold << ":\t";

//declaration of reference and analysed files and TH2D histos for the data from them, declaration of other variables

TFile *run_ref = new TFile( "/afs/cern.ch/work/k/kodolova/public/RDMweb/histos/LED_" + run1 + ".root", "READ");
TFile *run_ana = new TFile( "/afs/cern.ch/work/k/kodolova/public/RDMweb/histos/LED_" + run2 + ".root", "READ");
TH2F	*ampl_ref[8], *ampl_ana[8], *ampl_ratio[8];
TH1F	*ratio_distr[8];
int chan_count = 0; 	//all channels
int cal_count = 0;  	//channels need to be calibrated in subdetector
int tot_cal_count = 0;  //channels need to be calibrated in total
double drift = 0.;
TString	hist_name, hist_title;
TString	titles[] = {"HB1", "HB2", "HE1", "HE2", "HE3", "HF1", "HF2", "HO4"};

//loop over all subdetectors

for(int subd = 0; subd < 8; subd++){
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

						cout << subd << "\t";

					        if( xi > 41){
					                cout << xi - 41 << "\t";
					        }else{
					              	cout << xi - 42 << "\t";
					        }
						cout << yi << "\n";

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

	//output the results
	
	if(chan_count > 0){
//		cout << cal_count << "\t";
	}else{
//		cout << "empty_run\t";
	}

	//return counters to zero
	
	cal_count = 0;
	chan_count = 0;
}

//cout << ":\t" << tot_cal_count << "\n";

	//write all histos to .root file and to .gif files
	//by default .gif files printing is commented to avoid unwanted output to the terminal - one can uncomment them if needed

	TFile *output = new TFile("/afs/cern.ch/user/i/ivanp/2016_HCAL_gains_monitoring/output/Drun_" + run1 + "_" + run2 + ".root", "RECREATE");
	TCanvas *cnvs;
	for(int i = 0; i < 8; i++){
		cnvs = new TCanvas(titles[i]);
		ampl_ratio[i]->Draw("colz");
//		cnvs->Print("/afs/cern.ch/user/i/ivanp/2016_HCAL_gains_monitoring/output/Drun_" + run1 + "_" + run2 + "_2D" + titles[i] + ".gif");
		ratio_distr[i]->Draw("");
//		cnvs->Print("/afs/cern.ch/user/i/ivanp/2016_HCAL_gains_monitoring/output/Drun_" + run1 + "_" + run2 + "_1D" + titles[i] + ".gif");
		delete cnvs;
		ampl_ratio[i]->Write();
		ratio_distr[i]->Write();
	}
}
