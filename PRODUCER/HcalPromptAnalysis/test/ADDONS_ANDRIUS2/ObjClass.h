#ifndef ObjClass_H
#define ObjClass_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <TH1.h>
#include <TH2.h>
#include <TLatex.h>
#include <TFrame.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TMath.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TLegend.h>
#include <TText.h>
#include <TAxis.h>
#include <TFile.h>
#include <TLine.h>
#include <TGraph.h>
#include <TString.h>

// constants

const Int_t NMAX = 40;
const TDatime T0_2012(2012,01,01,00,00,00);
const TDatime T0_2015(2015,01,01,00,00,00);
const TDatime time0=T0_2012;


struct InputRuns_t;
struct InputCards_t;

//______________________________________________________________________________

//     1-   A,     2-TSn,        3-TSx,        4-W,        5-R,
//     6-  <A>,    7-<TSn>,      8-<TSx>,      9-<W>,     10-<R>,
//    11- <P0>,   12-<P1>,      13-<P2>,      14-<P3>,
//    15-<PW0>,   16-<PW1>,     17-<PW2>,     18-<PW3>,
//    19-  no,     20-no,
//    21-A_calib, 22-TSn_calib, 23-TSx_calib, 24-W_calib, 25-R_calib,
//______________________________________________________________________________

int LoadInputRuns(const std::string &fname, std::vector<InputRuns_t> &vec,
		  InputRuns_t *inpRunMin=NULL, InputRuns_t *inpRunMax=NULL);

TCanvas* MakePlot(Int_t Method, const InputCards_t &ic);

int checkDetector(std::string detectorName, int depth);

//______________________________________________________________________________

//---------------------------------------------------------------------------

inline
std::string twoDigitStr(int i) {
  const char* format= (i<10) ? "0%d" : "%d";
  return std::string(Form(format,i));
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

struct InputRuns_t {
  int fYear,fMonth,fDay,fHour,fMin,fSec;
  int fRunNo;
  std::string fDate; // a helper field
public:
  InputRuns_t();
  InputRuns_t(const InputRuns_t &r);
  std::string sqlTimeStr() const;
  void assign(int year, int month, int day, int hour, int min,
	      int sec, int runNo);
  int assign(const std::string &inpLine);
  int setDate(const std::string &inpLine);
  void setRun(int runNo) { fRunNo=runNo; }
  int runNo() const { return fRunNo; }

  // comparison operators compare either dateStr (if both not empty) or runNo
  friend int operator>(const InputRuns_t &r1, const InputRuns_t &r2);
  friend int operator<(const InputRuns_t &r1, const InputRuns_t &r2);
  friend int operator<=(const InputRuns_t &r1, const InputRuns_t &r2);
  friend int operator==(const InputRuns_t &r1, const InputRuns_t &r2);

  friend std::ostream& operator<< (std::ostream& out, const InputRuns_t &r);
};

//---------------------------------------------------------------------------

struct InputCards_t {
  std::vector<int> fIEta;
  std::string fDetector;
  int fDepth;
  std::vector<int> fIPhi;
  std::vector<InputRuns_t> fRunData;
  std::string fCalibType;
  std::string fFName;
public:
  InputCards_t(std::string set_calibType="LED") :
    fIEta(),fDetector("HF"),fDepth(-1),fIPhi(),fRunData(),
       fCalibType(set_calibType), fFName() {}
  InputCards_t(const InputCards_t &ic) :
    fIEta(ic.fIEta), fDetector(ic.fDetector),
    fDepth(ic.fDepth), fIPhi(ic.fIPhi),
    fRunData(ic.fRunData), fCalibType(ic.fCalibType), fFName()
    {}
  void clear();

  int size() const { return int(fRunData.size()); }
  int depth() const { return fDepth; }
  void depth(int set_depth) { fDepth=set_depth; }
  std::string calibType() const { return fCalibType; }
  void calibType(std::string set_type) { fCalibType=set_type; }
  std::string detector() const { return fDetector; }
  int detector(std::string set_detector); // adjust, if needed { fDetector=set_detector; }
  const std::vector<int> &ieta() const { return fIEta; }
  template<class idx_t>
  int ieta(idx_t idx) const { return fIEta[idx]; }
  const std::vector<int> &iphi() const { return fIPhi; }
  template<class idx_t>
  int iphi(idx_t idx) const { return fIPhi[idx]; }
  std::vector<int> &edit_iphi() { return fIPhi; }
  std::vector<int> &edit_ieta() { return fIEta; }
  const std::vector<InputRuns_t> &runData() const { return fRunData; }
  template<class idx_t>
  const InputRuns_t& runData(idx_t idx) const { return fRunData[idx]; }
  const InputRuns_t& firstRunData() const { return fRunData[0]; }
  const InputRuns_t& lastRunData() const { return fRunData.back(); }

  int load(const std::string &fname,
	   InputRuns_t *inpRunMin=NULL, InputRuns_t *inpRunMax=NULL);

  int prepareRootFileNamesAndTimer(std::vector<std::string> &fnames,
				   Float_t **timeArr) const;

  friend std::ostream& operator<< (std::ostream& out, const InputCards_t &ic);
};

//---------------------------------------------------------------------------

inline void HERE(const char *msg) { std::cout << msg << std::endl; }
template<class type_t>
inline void HERE(const char *msg, type_t val) { std::cout << msg << " " << val << std::endl; }

//---------------------------------------------------------------------------

void printHisto(TString msg, const TH2F* h2);

//---------------------------------------------------------------------------

#endif
