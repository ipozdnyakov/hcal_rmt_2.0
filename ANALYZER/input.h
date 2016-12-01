#ifndef _INPUT_H
#define _INPUT_H
using namespace std;

vector<vector<Int_t> > Nrun_HCAL(double threshold);
vector<Int_t> Drun_HCAL(TString run1, TString run2, double threshold, bool from_nrun);

void Ncell(TString);
void Nrun_cell(int subd, int ieta, int iphi);
void Drun_cell(TString run1, TString run2, int subd, int ieta, int iphi);

//array of runs numbers and total nuber of them

TString runs[] = {
	"271961","272303","272312","272513","272533","272581","272847","272946","272947","273117",
	"273234","273316","273455","273557","273604","273616","273648","273757","273763","273770",
	"273894","273961","274752","274890","275013","275265","275430","275457","275697","275807",
	"275864","276081","276402","276416","276461","276462","276463","276476","276520","276531",
	"276554","276600","276678","277132","277172","277336","277511","278596","278897","279058",
	"279096","279607","279700","279723","279785","279800","279908","280200","280262","280379",
	"281059","281362","281458","281645","281735","282462","282726","282775","282776","282847",
	"283010","283013","283098","283099","283101","283102","283103","283104","283111","283296",
	"283370","283398","283440","283442","283536","284124","284184","284188","284250","284307",
	"284320","284321","284324","284339","284829","284832","284878","285101","285224","285349"
	/*,"285356"*/};

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

int subd_depth[] = {1,2,1,2,3,1,2,4};

Int_t subd_depth_cells[] = {2304,288,1124,1080,288,864,864,2160};
Int_t subd_cells[] = {2592,2592,1728,2160}; //total 9072

#endif  /* _INPUT_H */
