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
