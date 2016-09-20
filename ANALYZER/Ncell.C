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
