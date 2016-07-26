#include "ObjClass.h"

using std::cout;
using std::endl;

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

int checkDetector(std::string name, int depth) {
  int ok=1;
  if (name=="HF") {
    if ((depth!=1) && (depth!=2)) ok=0;
  }
  else if (name=="HB") {
    if ((depth<1) || (depth>2)) ok=0;
  }
  else if (name=="HE") {
    if ((depth<1) || (depth>3)) ok=0;
  }
  else if (name=="HO") {
    if (depth!=4) ok=0;
  }
  std::cout << "checkDetector(name=" << name << ", depth=" << depth << ")\n";
  if (!ok) std::cout << " -- failed\n";
  else std::cout << " -- passed\n";
  return ok;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

InputRuns_t::InputRuns_t() : fYear(0),fMonth(0),fDay(0),
			     fHour(0),fMin(0),fSec(0),
			     fRunNo(0), fDate("")
{}

// ------------------------------------------------------

InputRuns_t::InputRuns_t(const InputRuns_t &r)
  : fYear(r.fYear), fMonth(r.fMonth), fDay(r.fDay),
    fHour(r.fHour), fMin(r.fMin), fSec(r.fSec),
    fRunNo(r.fRunNo),
    fDate(r.fDate)
{}

// ------------------------------------------------------

std::string InputRuns_t::sqlTimeStr() const
{
  std::string d("-"), sp(" "), s(":");
  std::string str = Form("%d",fYear) + d +
    twoDigitStr(fMonth) + d + twoDigitStr(fDay) + sp +
    twoDigitStr(fHour) + s + twoDigitStr(fMin) + s + twoDigitStr(fSec);
  return str;
}

// ------------------------------------------------------

void InputRuns_t::assign(int year, int month, int day, int hour, int min,
			 int sec, int runNo)
{
  fYear=year; fMonth=month; fDay=day;
  fHour=hour; fMin=min; fSec=sec;
  fDate= Form("%d",fYear) + twoDigitStr(fMonth) + twoDigitStr(fDay)
    + twoDigitStr(fHour) + twoDigitStr(fMin) + twoDigitStr(fSec);
  fRunNo=runNo;
}

// ------------------------------------------------------

int InputRuns_t::assign(const std::string &inpLine)
{
  std::istringstream ss(inpLine);
  std::string temp;
  int year,month,day,hour,minute,second,nrun;
  ss>>temp>>year>>month>>day>>hour>>minute>>second>>nrun;
  this->assign(year,month,day,hour,minute,second,nrun);
  return 1;
}

// ------------------------------------------------------

int InputRuns_t::setDate(const std::string &inpLine)
{
  std::istringstream ss(inpLine);
  char c1,c2;
  int year,month,day;
  ss>>year>>c1>>month>>c2>>day;
  this->assign(year,month,day,0,0,0, 0);
  return 1;
}

// ------------------------------------------------------

int operator>(const InputRuns_t &r1, const InputRuns_t &r2)
{
  if ((r1.fDate.size()>0) && (r2.fDate.size()>0))
    return (r1.fDate > r2.fDate) ? 1:0;
  return (r1.fRunNo > r2.fRunNo) ? 1:0;
}

int operator<(const InputRuns_t &r1, const InputRuns_t &r2)
{
  if ((r1.fDate.size()>0) && (r2.fDate.size()>0))
    return (r1.fDate < r2.fDate) ? 1:0;
  return (r1.fRunNo < r2.fRunNo) ? 1:0;
}

int operator<=(const InputRuns_t &r1, const InputRuns_t &r2)
{
  if ((r1.fDate.size()>0) && (r2.fDate.size()>0))
    return (r1.fDate <= r2.fDate) ? 1:0;
  return (r1.fRunNo <= r2.fRunNo) ? 1:0;
}

int operator==(const InputRuns_t &r1, const InputRuns_t &r2)
{
  if ((r1.fDate.size()>0) && (r2.fDate.size()>0))
    return (r1.fDate == r2.fDate) ? 1:0;
  return (r1.fRunNo == r2.fRunNo) ? 1:0;
}


// ------------------------------------------------------

std::ostream& operator<<(std::ostream& out, const InputRuns_t &r) {
  out << "InputRun: ";
  if (1) {
    out << r.sqlTimeStr();
  }
  else {
    out << r.fYear << "-" << twoDigitStr(r.fMonth) << "-" << twoDigitStr(r.fDay)
	<< " "
	<< twoDigitStr(r.fHour) << ":" << twoDigitStr(r.fMin) << ":"
	<< twoDigitStr(r.fSec);
  }
  out << " (dateStr=" << r.fDate << ")"
      << ", runNo=" << r.fRunNo;
  return out;
}

// ------------------------------------------------------


// ------------------------------------------------------

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void InputCards_t::clear()
{
  fIEta.clear();
  fDepth=-1;
  fIPhi.clear();
  fRunData.clear();
  fCalibType="unknown";
  fFName="none";
}

// ------------------------------------------------------

int InputCards_t::detector(std::string set_detector) {
  if (set_detector.size()==2) {
    fDetector=set_detector.c_str();
  }
  else if (set_detector.size()==3) {
    char c= set_detector[set_detector.size()-1];
    if ((c>='1') && (c<='4')) {
      fDepth= int(c)-int('1') + 1;
      fDetector=Form("%c%c",set_detector[0],set_detector[1]);
    }
    else {
      std::cout << "InputCards_t::detector: 3 characters supplied in <"
		<< set_detector << "> but the last one is not a number\n";
      return 0;
    }
  }
  int res=checkDetector(fDetector,fDepth);
  if (!res) {
    std::cout << "InputCards_t::detector got improper value <"
	      << set_detector << ">\n";
    return 0;
  }
  return 1;
}

// ------------------------------------------------------

int InputCards_t::load(const std::string &fname,
		       InputRuns_t *inpRunMin, InputRuns_t *inpRunMax)
{
  this->clear();
  fFName=fname;
  std::cout << "InputCards_t::load(" << fname << ") ..." << std::endl;

  std::ifstream fin(fname.c_str());
  if (!fin.is_open()) {
    std::cout << "failed to open the file <" << fname << ">\n";
    return 0;
  }

  std::string line;
  InputRuns_t r;
  std::string test;
  int val;

  while ( std::getline( fin, line ) ) {
    if ( !line.size() || line[0]=='#' ) { continue; }
    std::istringstream ss(line);
    ss >> test;

    if (test == "eta") {
      do {
	ss >> val;
           std::cout<<" Read eta "<<val<<std::endl;
	if ((fIEta.size()==0) || (fIEta.back()!=val)) fIEta.push_back(val);
      } while (!ss.eof());
    }
    else if (test == "calibration") {
      ss >> fCalibType;
           std::cout<<" calibration "<<fCalibType<<std::endl;
    }
    else if (test == "depth") {
      ss >> fDepth;
           std::cout<<" Read depth "<<fDepth<<std::endl;
    }
    else if (test == "iphi" ) {
      do {
	ss >> val;
	if ((fIPhi.size()==0) || (fIPhi.back()!=val)) fIPhi.push_back(val);
       std::cout<<" Read iphi "<<val<<" "<<fIPhi.size()<<std::endl; 
      } while (!ss.eof());
    }
    else if (test == "inputruns") {
      if (!r.assign(line)) {
	std::cout << "failed to assign from line <" << line << ">\n";
      }
      else {
	if (inpRunMin && (r<*inpRunMin)) continue;
	if (inpRunMax && (*inpRunMax<r)) continue;
         std::cout<<" Run "<<r<<std::endl;
	fRunData.push_back(r);
      }
    }
    else {
      std::cout << "ignored line <" << line << ">\n";
    }
  }
  fin.close();

  if (fIEta.size()>1) std::sort(fIEta.begin(),fIEta.end());
  if (fIPhi.size()>1) std::sort(fIPhi.begin(),fIPhi.end());

  return 1;
}

// ------------------------------------------------------

int InputCards_t::prepareRootFileNamesAndTimer(std::vector<std::string> &fnames,
					       Float_t **timeArr) const
{
  fnames.clear();
  (*timeArr)=NULL;
  if (fRunData.size()==0) return 0;

  const Double_t startTime=Double_t(time0.Convert());

  int count=int(fRunData.end()-fRunData.begin());
  (*timeArr)= new Float_t[count+1];

  for (std::vector<InputRuns_t>::const_iterator it= fRunData.begin();
       it!=fRunData.end(); it++) {
    int idx=int( it - fRunData.begin() );
    TDatime t( it->sqlTimeStr().c_str() );
    (*timeArr)[idx] = Float_t( Double_t(t.Convert()) - startTime );
    const char *histoFileNameT="/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMweb/histos/%s_%d.root";
    //const char *histoFileNameT="/home/andriusj/temp/%s_%d.root";
    std::string fname=Form(histoFileNameT,fCalibType.c_str(),it->runNo());
    fnames.push_back(fname);
  }
  return int(fnames.size());
}

// ------------------------------------------------------

std::ostream& operator<<(std::ostream& out, const InputCards_t &ic) {
  out << "InputCards:  " << ic.fCalibType << "\n";
  out << "(from file <" << ic.fFName << ">)\n";
  out << "Depth=" << ic.fDepth << "\n";
  out << Form("iEtas [%d]: ",ic.fIEta.size());
  for (unsigned int i=0; i<ic.fIEta.size(); i++) {
    out << " " << ic.fIEta[i];
  }
  out << Form("iPhis [%d]: ",ic.fIPhi.size());
  for (unsigned int i=0; i<ic.fIPhi.size(); i++) {
    out << " " << ic.fIPhi[i];
  }
  out << "\n";
  out << Form("Runs [%d]:",ic.fRunData.size()) << "\n";
  for (unsigned int i=0; i<ic.fRunData.size(); i++) {
    out << i << ") " << ic.fRunData[i] << "\n";
  }
  return out;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void printHisto(TString msg, const TH2F *h2) {
  std::cout << "printHisto: msg=" << msg << std::endl;
  if (!h2) { std::cout << "histoPtr is null\n"; return; }
  std::cout << " histo " << h2->GetName() << " "
	    << h2->GetNbinsX() << " x " << h2->GetNbinsY() << "\n";
  for (int ibin=1; ibin<=h2->GetNbinsX(); ++ibin) {
    for (int jbin=1; jbin<=h2->GetNbinsY(); ++jbin) {
      if ((ibin>100) || (jbin>100)) std::cout << ".";
      std::cout << Form(" (%d,%d) ",ibin,jbin)
		<< h2->GetBinContent(ibin,jbin) << "\n";
    }
  }
  std::cout << "\n";
  return;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

int LoadInputRuns(const std::string &fname, std::vector<InputRuns_t> &vec,
		  InputRuns_t *inpRunMin, InputRuns_t *inpRunMax) {
  vec.clear();

  std::ifstream fin(fname.c_str());
  if (!fin.is_open()) {
    std::cout << "failed to open the file <" << fname << ">\n";
    return 0;
  }

  std::string line;
  InputRuns_t r;

  while ( std::getline( fin, line ) ) {
    if ( !line.size() || line[0]=='#' ) { continue; }
    if (line.find("inputruns")==std::string::npos) continue;

    if (!r.assign(line)) {
      std::cout << "failed to read from <" << line << ">\n";
    }
    else {
      if (inpRunMin && (r<*inpRunMin)) continue;
      if (inpRunMax && (*inpRunMax<r)) continue;
      vec.push_back(r);
    }
  }
  fin.close();
  return 1;
}


//---------------------------------------------------------------------------


//______________________________________________________________________________
//     1-   A,     2-TSn,        3-TSx,        4-W,        5-R,
//     6-  <A>,    7-<TSn>,      8-<TSx>,      9-<W>,     10-<R>,
//    11- <P0>,   12-<P1>,      13-<P2>,      14-<P3>,
//    15-<PW0>,   16-<PW1>,     17-<PW2>,     18-<PW3>,
//    19-  no,     20-no,
//    21-A_calib, 22-TSn_calib, 23-TSx_calib, 24-W_calib, 25-R_calib,
//______________________________________________________________________________

TCanvas *MakePlot(Int_t Method, const InputCards_t &ic)
{
  if (ic.runData().size()==0) {
    std::cout << "MakePlot: nothing to display\n";
    return NULL;
  }

  TCanvas *c1=NULL;
  std::vector<TGraph*> grV;

  Float_t avgW[NMAX];// width averaged
  Float_t ratio[NMAX];// ratio

  TDatime storeTimeOffset(gStyle->GetTimeOffset());
  const Double_t startTime=Double_t(time0.Convert());
  gStyle->SetTimeOffset(startTime);

  std::string timeMinStr= ic.firstRunData().sqlTimeStr();
  std::string timeMaxStr= ic.lastRunData().sqlTimeStr();
  //TDatime timeMinDT(timeMinStr.c_str());
  //TDatime timeMaxDT(timeMaxStr.c_str());
  //Float_t timeMin=Float_t(timeMinDT.Convert());
  //Float_t timeMax=Float_t(timeMaxDT.Convert());

  Float_t timeArr[NMAX];
  std::vector<std::string> filesv;
  filesv.reserve(20);

  for (std::vector<InputRuns_t>::const_iterator it= ic.runData().begin();
       it!=ic.runData().end(); it++) {
    int idx=int( it - ic.runData().begin() );
    TDatime t( it->sqlTimeStr().c_str() );
    timeArr[idx] = Float_t( Double_t(t.Convert()) - startTime );
    const char *histoFileNameT="/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMweb/histos/%s_%d.root";
    std::string fname=Form(histoFileNameT,ic.calibType().c_str(),it->runNo());
    filesv.push_back(fname);
  }

  Float_t timeMin= timeArr[0];
  Float_t timeMax= timeArr[filesv.size()-1];

  int iphi=ic.iphi()[0];
  int numdates = int(filesv.size());
  int numetas = int(ic.ieta().size());
  std::cout << "Period: " << timeMinStr << " -- " << timeMaxStr
	    << " (" << numdates << " values)\n";
  std::cout << "Run range: " << ic.runData()[0].runNo() << " -- " << ic.runData().back().runNo() << "\n";
  std::cout << std::endl;


  char histADCAmpl[100];
  char histTSmean[100];
  char histTSmax[100];
  char histAmplitude[100];
  char histAmpl[100];
  char histMap[100];
  char outfile[100];
  char outleg[100];
  const int depth=ic.depth();
  sprintf(histADCAmpl,"h_mapDepth%dADCAmpl_HF",depth);
  sprintf(histTSmean,"h_mapDepth%dTSmeanA_HF",depth);
  sprintf(histTSmax,"h_mapDepth%dTSmaxA_HF",depth);
  sprintf(histAmplitude,"h_mapDepth%dAmplitude_HF",depth);
  sprintf(histAmpl,"h_mapDepth%dAmpl_HF",depth);
  sprintf(histMap,"h_mapDepth%d_HF",depth);
  sprintf(outfile,"outDepth%d_phi%d_HF.gif",depth,iphi);
  sprintf(outleg,"HF depth %d, iphi=%d",depth, iphi);


  //______________________________________________________________________________
  // Create a new canvas.
  c1 = new TCanvas("zamt4","Monte Carlo Study of Z scaling",10,40,800,600);
  c1->Range(0,0,25,18);
  c1->SetFillColor(40);
  TPad* pad1 = new TPad("pad1","This is pad1",0.02,0.02,0.98,0.83,33);
  TLatex *t = new TLatex();
  t->SetTextFont(32);
  t->SetTextColor(4);
  t->SetTextSize(0.03);
  t->SetTextAlign(12);
  if(Method == 1) t->DrawLatex(0.9,15.5,"Time dependence of full Amplitude averaged over events of each LED run for HF channels");
  if(Method == 2) t->DrawLatex(0.9,15.5,"Time dependence of TSn averaged over events of each LED run for some(=9) random HB-depth1 channels");
  if(Method == 3) t->DrawLatex(0.9,15.5,"Time dependence of TSx averaged over events of each LED run for some(=9) random HB-depth1 channels");
  if(Method == 4) t->DrawLatex(0.9,15.5,"Time dependence of RMS averaged over events of each LED run for some(=9) random HB-depth1 channels");
  if(Method == 5) t->DrawLatex(0.9,15.5,"Time dependence of amplitude Ratio averaged over events of each LED run for some(=9) random HB-depth1 channels");
  if(Method == 6) t->DrawLatex(0.9,15.5,"Time dependence of full Amplitude averaged over events of each LED run and over all channels of HB-depth1");
  if(Method == 7) t->DrawLatex(0.9,15.5,"Time dependence of TSn averaged over events of each LED run and over all channels of HB-depth1");
  if(Method == 8) t->DrawLatex(0.9,15.5,"Time dependence of TSx averaged over events of each LED run and over all channels of HB-depth1");
  if(Method == 9) t->DrawLatex(0.9,15.5,"Time dependence of RMS averaged over events of each LED run and over all channels of HB-depth1");
  if(Method ==10) t->DrawLatex(0.9,15.5,"Time dependence of amplitude Ratio averaged over events of each LED run and over all channels of HB-depth1");
  if(Method ==11) t->DrawLatex(0.9,15.5,"Time dependence of Pedestal(cap0) averaged over events of each LED run and over all channels of HB-depth1");
  if(Method ==12) t->DrawLatex(0.9,15.5,"Time dependence of Pedestal(cap1) averaged over events of each LED run and over all channels of HB-depth1");
  if(Method ==13) t->DrawLatex(0.9,15.5,"Time dependence of Pedestal(cap2) averaged over events of each LED run and over all channels of HB-depth1");
  if(Method ==14) t->DrawLatex(0.9,15.5,"Time dependence of Pedestal(cap3) averaged over events of each LED run and over all channels of HB-depth1");
  if(Method ==15) t->DrawLatex(0.9,15.5,"Time dependence of PedestalWidth(cap0) averaged over events of each LED run and over all channels of HB-depth1");
  if(Method ==16) t->DrawLatex(0.9,15.5,"Time dependence of PedestalWidth(cap1) averaged over events of each LED run and over all channels of HB-depth1");
  if(Method ==17) t->DrawLatex(0.9,15.5,"Time dependence of PedestalWidth(cap2) averaged over events of each LED run and over all channels of HB-depth1");
  if(Method ==18) t->DrawLatex(0.9,15.5,"Time dependence of PedestalWidth(cap3) averaged over events of each LED run and over all channels of HB-depth1");
  if(Method ==19) t->DrawLatex(0.9,15.5,"no suach a method");
  if(Method ==20) t->DrawLatex(0.9,15.5,"no suach a method");
  if(Method == 21) t->DrawLatex(0.9,15.5,"Time dependence of A_calib averaged over events of each LED run for some(=9) random HB channels");
  if(Method == 22) t->DrawLatex(0.9,15.5,"Time dependence of TSn_calib averaged over events of each LED run for some(=9) random HB channels");
  if(Method == 23) t->DrawLatex(0.9,15.5,"Time dependence of TSx_calib averaged over events of each LED run for some(=9) random HB channels");
  if(Method == 24) t->DrawLatex(0.9,15.5,"Time dependence of W_calib averaged over events of each LED run for some(=9) random HB channels");
  if(Method == 25) t->DrawLatex(0.9,15.5,"Time dependence of R_calib averaged over events of each LED run for some(=9) random HB channels");


  pad1->Draw();
  //______________________________________________________________________________
  pad1->cd();
  pad1->Range(-0.255174,-19.25,2.29657,-6.75);

  TH2F* hh2=new TH2F("hh2","",2,timeMin,timeMax,100,0.8,1.2);
  hh2->SetStats(0);
  hh2->GetXaxis()->SetTimeDisplay(1);
  hh2->GetXaxis()->SetLabelSize(0.03);
  hh2->GetXaxis()->SetTimeFormat("%D");
  hh2->GetXaxis()->SetTitle("Time");
  hh2->Draw();

  //pad1->DrawFrame(t0[0]-10.,   0.8,  t0[6]+10.,   1.1);// 1-A,
  //pad1->SetTimeDisplay(1);
  //pad1->SetTimeFormat("%d\/%H:%M");
  //______________________________________________________________________________
  //______________________________________________________________________________
  pad1->GetFrame()->SetFillColor(19);
  //______________________________________________________________________________
  //______________________________________________________________________________
  //
  //

  TFile* hfile=NULL;
  TH2F* twod1=NULL;
  TH2F* twod0=NULL;

  for(std::vector<int>::const_iterator meta=ic.ieta().begin();
      meta<ic.ieta().end(); meta++)
    {

    Int_t NP = numdates;  // for loop over time points(LED runs)

    for(int k1 = 0; k1<NP; k1++) {

      std::cout << "for LED run k1=" << k1   <<"      Number of imposed points = " << NP <<" "<<*meta<<std::endl;

      hfile= new TFile(filesv[k1].c_str(),"READ");

      cout << "OPENED" <<endl;
      //h_mapTSmaxCalib_HB
      // ADCAmpl   - fullAmplitude
      if(Method == 1) twod1= (TH2F*)hfile->Get(histADCAmpl);
      // TSmeanA   -    TSn
      if(Method == 2) twod1= (TH2F*)hfile->Get(histTSmean);
      // TSmaxA    -   TSx
      if(Method == 3) twod1= (TH2F*)hfile->Get(histTSmax);
      // Amplitude - W
      if(Method == 4) twod1= (TH2F*)hfile->Get(histAmplitude);
      // Ampl      - Ratio
      if(Method == 5) twod1= (TH2F*)hfile->Get(histAmpl);
      //
      //
      // Calibration channels:
      //
      //  - fullAmplitude
      if(Method == 21) twod1= (TH2F*)hfile->Get("h_mapADCCalib_HF");
      //  -    TSn
      if(Method == 22) twod1= (TH2F*)hfile->Get("h_mapTSmeanCalib_HF");
      //  -   TSx
      if(Method == 23) twod1= (TH2F*)hfile->Get("h_mapTSmaxCalib_HF");
      //  - W
      if(Method == 24) twod1= (TH2F*)hfile->Get("h_mapWidthCalib_HF");
      //  - Ratio
      if(Method == 25) twod1= (TH2F*)hfile->Get("h_mapRatioCalib_HF");
      //
      //
      //
      //
      if(Method == 6) {
	TH1F *Ceff6= (TH1F*)hfile->Get("h_ADCAmpl_HF");
	avgW[k1]  =  Ceff6->GetMean();
	// if(k1==0 ) cout << "for LED run k1=" << k1   <<"GetMean = " << avgW[k1] <<endl;
      }
      else if(Method == 7) {
	TH1F *Ceff7= (TH1F*)hfile->Get("h_TSmeanA_HF");
	avgW[k1]  =  Ceff7->GetMean();
      }
      else if(Method == 8) {
	TH1F *Ceff8= (TH1F*)hfile->Get("h_TSmaxA_HF");
	avgW[k1]  =  Ceff8->GetMean();
      }
      else if(Method == 9) {
	TH1F *Ceff9= (TH1F*)hfile->Get("h_Amplitude_HF");
	avgW[k1]  =  Ceff9->GetMean();
      }
      else if(Method == 10) {
	TH1F *Ceff10= (TH1F*)hfile->Get("h_Ampl_HF");
	avgW[k1]  =  Ceff10->GetMean();
      }
      else if(Method == 11) {
	TH1F *Ceff11= (TH1F*)hfile->Get("h_pedestal0_HF");
	avgW[k1]  =  Ceff11->GetMean();
      }
      else if(Method == 12) {
	TH1F *Ceff12= (TH1F*)hfile->Get("h_pedestal1_HF");
	avgW[k1]  =  Ceff12->GetMean();
      }
      else if(Method == 13) {
	TH1F *Ceff13= (TH1F*)hfile->Get("h_pedestal2_HF");
	avgW[k1]  =  Ceff13->GetMean();
      }
      else if(Method == 14) {
	TH1F *Ceff14= (TH1F*)hfile->Get("h_pedestal3_HF");
	avgW[k1]  =  Ceff14->GetMean();
      }
      else if(Method == 15) {
	TH1F *Ceff15= (TH1F*)hfile->Get("h_pedestalw0_HF");
	avgW[k1]  =  Ceff15->GetMean();
      }
      else if(Method == 16) {
	TH1F *Ceff16= (TH1F*)hfile->Get("h_pedestalw1_HF");
	avgW[k1]  =  Ceff16->GetMean();
      }
      else if(Method == 17) {
	TH1F *Ceff17= (TH1F*)hfile->Get("h_pedestalw2_HF");
	avgW[k1]  =  Ceff17->GetMean();
      }
      else if(Method == 18) {
	TH1F *Ceff18= (TH1F*)hfile->Get("h_pedestalw3_HF");
	avgW[k1]  =  Ceff18->GetMean();
      }
      else {
	//
	//
	if(Method < 6) twod0= (TH2F*)hfile->Get(histMap);
	if(Method > 20) twod0= (TH2F*)hfile->Get("h_map_HF");
	//
	//	if(twod0->IsA()->InheritsFrom("TH2F")) {
	TH2F* Ceff = (TH2F*)twod1->Clone("Ceff");

        //std::cout<<" Bin content "<<twod1->GetBinContent(28,1)<<std::endl;

	Ceff->Divide(twod1,twod0, 1, 1, "B");
	//	}
	int nx = Ceff->GetXaxis()->GetNbins();
	int ny = Ceff->GetYaxis()->GetNbins();
	if(k1==0 ) {
         cout << "for 1st LED run: nx = " << nx <<" ny = " << ny <<endl;
        }
	//
	Int_t i = (*meta);
	Int_t j = iphi;
	avgW[k1]  =  Ceff->GetBinContent(i,j);
	if(i == 2) std::cout<<j<<" RRRR "<<avgW[k1]<<" "<<i<<std::endl;
      }// else
    }//for k1 loop over time points(LED runs)

    // normalization on 1st time point

    for(int k2 = 0; k2<NP; k2++) {
      if(avgW[0]<=0.) {ratio[k2] = 0.;}
       else
      {ratio[k2] = avgW[k2]/avgW[0];}
            cout<<"Point:k2="<<k2<<" avgW[k2]="<<avgW[k2]<<" avgW[0]="<<avgW[0]<<" ratio[k2]="<<ratio[k2]<<endl;
    }//



    TGraph *gr = new TGraph(NP,timeArr,ratio);
    grV.push_back(gr);
    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(0.9);
    int r1=int(grV.size());
    gr->SetLineColor(r1+1);
    gr->SetMarkerColor(r1+1);
    gPad->SetGridy();

    gr->Draw("PL");

  }//r1 -few random point sets

  //______________________________________________________________________________
  //______________________________________________________________________________
  //______________________________________________________________________________
  //
  std::cout<<"Number of graphs "<< grV.size() <<std::endl;
  TLegend * leg1 = new TLegend(0.20,0.75,0.55,0.95,outleg);
  TLegend * leg2 = new TLegend(0.60,0.75,0.95,0.95);
  int split=6;

  for(unsigned int m=0; m<grV.size(); m++) {
    TString desc=Form("ieta = -%d",41-m);
    TLegend *legLoc= (m<split) ? leg1 : leg2;
    legLoc->AddEntry(grV[m],desc,"p");
  }
  leg1->Draw();
  if (grV.size() >= split) leg2->Draw();

  c1->Modified();
  c1->Update();
  //
  c1->Print(outfile);
  //gSystem->Exit(0);
  //______________________________________________________________________________

  gStyle->SetTimeOffset(storeTimeOffset.Convert());

  return c1;
}
//______________________________________________________________________________
