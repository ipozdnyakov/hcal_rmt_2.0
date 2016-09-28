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
