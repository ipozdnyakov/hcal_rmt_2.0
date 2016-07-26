#ifndef HistoDef_H
#define HistoDef_H

#include "ObjClass.h"
#include <TString.h>

// -------------------------------------------------------------

struct HistoDef_t;
TCanvas *MakePlotHD(HistoDef_t hd, const InputCards_t &ic);

const int cIEtaMax=82;
const int cIPhiMax=72;


// -------------------------------------------------------------

  //     1-  Amp,  2 - TSn,  3 - TSx,  4 - Width,        5-Ratio,
  //     6-  <A>,    7-<TSn>,      8-<TSx>,      9-<W>,     10-<R>,
  //    11- <P0>,   12-<P1>,      13-<P2>,      14-<P3>,
  //    15-<PW0>,   16-<PW1>,     17-<PW2>,     18-<PW3>,
  //    19-  no,     20-no,
  //    21-A_calib, 22-TSn_calib, 23-TSx_calib, 24-W_calib, 25-R_calib,

typedef enum {
  _calcNone=0, _calcAmp, _calcTSn, _calcTSx, _calcWidth, _calcRatio,
  _calcAvgAmp, _calcAvgTSn, _calcAvgTSx, _calcAvgWidth, _calcAvgRatio,
  _calcAvgP0, _calcAvgP1, _calcAvgP2, _calcAvgP3,
  _calcAvgPW0, _calcAvgPW1, _calcAvgPW2, _calcAvgPW3,
  _calcACalib, _calcTSnCalib, _calcTSxCalib, _calcWCalib, _calcRCalib,
  _calcLast,
  _calcSpec,
  _calcUnknown
} TMethod_t;

typedef enum { _histo0, _histo1F, _histo1D, _histo2F, _histo2D } THistoDim_t;

typedef enum { _sdHB1, _sdHB2,
	       _sdHE1, _sdHE2, _sdHE3,
	       _sdHO4,
	       _sdHF1, _sdHF2,
	       _sdLast,
	       _sdUnknown }
  TSubDet_t;

typedef enum { _specNone=0, _specAvgPhi,
	       _specLast,
	       _specUnknown
} TSpecMethod_t;


// -------------------------------------------------------------

// define dictionaries ( enum <-> TString )

TString GetMethodName(TMethod_t method, int shortVersion=0);
TMethod_t GetMethod(TString str);
void ListMethods(int shortVersion=0);
void CheckMethodConversion(int shortVersion=0);

TString GetSpecMethodName(TSpecMethod_t method, int shortVersion=0);
TSpecMethod_t GetSpecMethod(TString str);
void ListSpecMethods(int shortVersion=0);
void CheckSpecMethodConversion(int shortVersion=0);

TString GetHistoDimName(THistoDim_t hd);
THistoDim_t GetHistoDim(TString str);

TString GetSubDetFullName(TSubDet_t sd);
TString GetSubDetName(TSubDet_t sd);
int GetSubDetDepth(TSubDet_t sd);
TSubDet_t GetSubDet(TString str);
TSubDet_t GetSubDet(TString str, int depth);

// -------------------------------------------------------------

inline TMethod_t nextMethod(TMethod_t m) {
  if (m<_calcLast) m=TMethod_t(int(m)+1);
  return m;
}

inline void next(TMethod_t &m) { m=nextMethod(m); }

// -------------------------------------------------------------

inline TSpecMethod_t nextSpecMethod(TSpecMethod_t m) {
  if (m<_specLast) m=TSpecMethod_t(int(m)+1);
  return m;
}

inline void next(TSpecMethod_t &m) { m=nextSpecMethod(m); }

// -------------------------------------------------------------

inline TSubDet_t nextSubDet(TSubDet_t sd) {
  if (sd<_sdLast) sd=TSubDet_t(int(sd)+1);
  return sd;
}

inline void next(TSubDet_t &sd) { sd=nextSubDet(sd); }

// -------------------------------------------------------------

struct SpecMethodDef_t {
  TSpecMethod_t fMethod;
  std::vector<int> fIVec; // keep indices (0,1,...,maxDim-1).
  TString fName;
public:
  SpecMethodDef_t(TString set_name="undef");
  SpecMethodDef_t(TString set_name, TSpecMethod_t set_method, const std::vector<int> &set_idx);
  SpecMethodDef_t(const SpecMethodDef_t &s) :
    fMethod(s.fMethod), fIVec(s.fIVec), fName(s.fName) {}
  void clear() { fMethod=_specNone; fIVec.clear(); }
  TString name() const { return fName; }
  void name(TString set_name) { fName=set_name; }
  void nameTag(TString set_name_tag) { fName=GetSpecMethodName(fMethod,1) + set_name_tag; }
  TSpecMethod_t method() const { return fMethod; }
  const std::vector<int> &iVec() const { return fIVec; }
  template<class idx_t>
  int i(idx_t idx) const { return fIVec[idx]; }

  int assign(TSpecMethod_t set_method, int set_imin, int set_imax, int set_step);

  int assign(TSpecMethod_t set_method, const std::vector<int> &set_idx,
	     TString set_name_tag="")
  {
    fMethod=set_method; fIVec=set_idx; this->nameTag(set_name_tag);
    return 1;
  }

  void assign(const SpecMethodDef_t &s, TString newName="");
  TString explainIdx() const;

  SpecMethodDef_t& operator=(const SpecMethodDef_t &s);
  friend std::ostream& operator<<(std::ostream& out, const SpecMethodDef_t &s);
};

// -------------------------------------------------------------

TString GetSpecMethodName(SpecMethodDef_t method, int shortVersion=0);
//SpecMethodDef_t GetSpecMethodDef(TString str);

// -------------------------------------------------------------

struct HistoDef_t {
  TString fCalibType, fDetector;
  TMethod_t fMethod;
  SpecMethodDef_t fSpecMethod;
  int fDepth;
  TString fPlotTitle;
  TString fInpHistoName, fInpHistoName2;
  THistoDim_t fHistoDim;
  TH1F *fH1F;
  TH2F *fH2F, *fH2F_2, *fH2F_ratio;
public:
  HistoDef_t(TString set_calibtype="LED", TString set_detector="HF",
	     TMethod_t set_method=_calcNone, int set_depth=-1);
  HistoDef_t(TString set_calibtype="LED", TSubDet_t sd=_sdHF1,
	     TMethod_t set_method=_calcNone);
  HistoDef_t(const InputCards_t &ic, TMethod_t set_method=_calcNone);
  HistoDef_t(const HistoDef_t &d, TString cloneTag="");

  TString calibType() const { return fCalibType; }
  TString detector() const { return fDetector; }
  TMethod_t method() const { return fMethod; }
  SpecMethodDef_t specMethod() const { return fSpecMethod; }
  THistoDim_t histoDim() const { return fHistoDim; }
  int depth() const { return fDepth; }
  TString plotTitle() const { return fPlotTitle; }
  TH1F *getHisto() { return fH1F; }
  TH2F *getHisto2F() { return fH2F; }
  TH2F *getHisto2F_2() { return fH2F_2; }
  TH2F *getHisto2F_ratio() { return fH2F_ratio; }
  TString getMethodName() const;

  // Main methods that initialize the histogram names and the types
  int setMethod(TMethod_t set_method, int set_depth);
  int setMethod(TString reset_calibtype, TMethod_t set_method, int set_depth); // calls setMethod

  // define spec method, remove iphi values from InputCards_t
  int setSpecMethod(TSpecMethod_t specM, InputCards_t &ic,
		    TString set_specMethodTag="");
  int setSpecMethod(const SpecMethodDef_t &specM);
  TMethod_t useMethod() const // defines which histograms to use
  { return (this->applySpecMethod()) ? _calcSpec : fMethod; }
  THistoDim_t useHistoDim() const; // defines which histograms to use

  // whether special handling is needed
  int applySpecMethod() const {
    return ((fSpecMethod.method()!=_specNone) &&
	    (fSpecMethod.method()!=_specUnknown)) ? 1:0;
  }

  // Load histograms from a file and put them in the TH1F or TH2F fields
  // SpecMethod comes into effect there
  int loadHisto(TFile &f, TString setTag="");

  Float_t getMean() const {
    if (fH1F) return fH1F->GetMean();
    std::cout << "getMean fH1F is not ready (fInpHistoName=" << fInpHistoName
	      << ", fPlotTitle=" << fPlotTitle << ")\n";
    return -999.;
  }

  friend std::ostream& operator<< (std::ostream &out, const HistoDef_t &hd);
};

// -------------------------------------------------------------
// -------------------------------------------------------------

#endif
