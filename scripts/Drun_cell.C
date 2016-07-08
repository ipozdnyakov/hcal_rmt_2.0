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
