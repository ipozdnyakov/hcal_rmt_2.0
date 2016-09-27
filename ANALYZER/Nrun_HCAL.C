void Nrun_HCAL(double threshold = 0.03) {

//array of numbers of runs - from list of led runs in the "/input" directory

TString runs[] = {
/*"271961","272303","272312","272513","272533","272581",*/"272847","272946","272947","273117","273234","273316",
"273455","273557","273604","273616","273648","273757","273763","273770","273894","273961",
"274752","274890","275013","275265","275430","275457","275697","275807","275864","276081","276402","276416",
"276461","276462","276463","276476","276520","276531","276554","276600","276678",
"277132","277172","277336","277511","278596","278897","279058","279096","279607","279700","279723,"279785","279800","279908","280200","280262","280379"};



//calculate the size of the array

Int_t n_of_runs = sizeof(runs)/sizeof(runs[0]);

//cout monitoring parameters and names for columns

cout << "RefRun=" << runs[0] << " threshold=" << threshold << ":\n";
cout << "\t\tHB1\tHB2\tHB_tot\tHE1\tHE2\tHE3\tHE_tot\tHF1\tHF2\tHF_tot\tHO4\t:\tTOT\n";

//loop over all runs

	for(int i = 1; i < n_of_runs; i++){
		cout << i << ") ";
		Drun_HCAL(runs[0], runs[i], threshold, true);
	}

}
