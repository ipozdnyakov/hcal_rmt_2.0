void Nrun_cell(int subd, int ieta, int iphi) {

//array of runs numbers and total nuber of them

	TString runs[] = {"271961","272303","272312","272513","272533","272581","272847","272946","272947","273117","273234","273316",
			  "273455","273557","273604","273616","273648","273757","273763","273770","273894","273961",
			  "274752","274890","275013","275265","275430","275457","275697","275807","275864","276081","276402","276416"};
	Int_t n_of_runs = sizeof(runs)/sizeof(runs[0]);

//loop over all runs with regard to the first one

	cout << "Time evol of gain drift for cell in subdetector " << subd << " with ieta=" << ieta << " iphi=" << iphi << " : ";

	for(int i = 1; i < n_of_runs; i++){
		Drun_cell(runs[0], runs[i], subd, ieta, iphi);
	}
cout << "\n";
}
