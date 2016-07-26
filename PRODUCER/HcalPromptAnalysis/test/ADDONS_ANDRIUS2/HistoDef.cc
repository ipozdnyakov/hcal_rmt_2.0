#include "HistoDef.h"


// -------------------------------------------------------------
// -------------------------------------------------------------

TString GetMethodName(TMethod_t method, int shortVersion) {
  TString name;
  switch(method) {
  case _calcNone: name="none"; break;
  case _calcAmp: name="Amp"; break;
  case _calcTSn: name="TSn"; break;
  case _calcTSx: name="TSx"; break;
  case _calcWidth: name="Width"; break;
  case _calcRatio: name="Ratio"; break;
  case _calcAvgAmp: name="AvgAmp"; break;
  case _calcAvgTSn: name="AvgTSn"; break;
  case _calcAvgTSx: name="AvgTSx"; break;
  case _calcAvgWidth: name="AvgWidth"; break;
  case _calcAvgRatio: name="AvgRatio"; break;
  case _calcAvgP0: name="AvgP0"; break;
  case _calcAvgP1: name="AvgP1"; break;
  case _calcAvgP2: name="AvgP2"; break;
  case _calcAvgP3: name="AvgP3"; break;
  case _calcAvgPW0: name="AvgPW0"; break;
  case _calcAvgPW1: name="AvgPW1"; break;
  case _calcAvgPW2: name="AvgPW2"; break;
  case _calcAvgPW3: name="AvgPW3"; break;
  case _calcACalib: name="ACalib"; break;
  case _calcTSnCalib: name="TSnCalib"; break;
  case _calcTSxCalib: name="TSxCalib"; break;
  case _calcWCalib: name ="WCalib"; break;
  case _calcRCalib: name="RCalib"; break;
  case _calcLast: name="Last"; break;
  case _calcSpec: name="Spec"; break;
  case _calcUnknown: name="Unknown"; break;
  default:
    std::cout << "GetName(TMethod_t): missing entry\n";
    name="UNDEFINED";
  }
  if (!shortVersion) name.Prepend("Method_");
  return name;
}

// -------------------------------------------------------------

TMethod_t GetMethod(TString str) {
  TMethod_t method=_calcUnknown;
  str.ToLower();
  // change to the lower-case letters
  // amp, tsn, tsx, width, ratio are at the end
  if (str.Index("avgamp")!=-1) method=_calcAvgAmp;
  else if (str.Index("avgtsn")!=-1) method=_calcAvgTSn;
  else if (str.Index("avgtsx")!=-1) method=_calcAvgTSx;
  else if (str.Index("avgwidth")!=-1) method=_calcAvgWidth;
  else if (str.Index("avgratio")!=-1) method=_calcAvgRatio;
  else if (str.Index("avgp0")!=-1) method=_calcAvgP0;
  else if (str.Index("avgp1")!=-1) method=_calcAvgP1;
  else if (str.Index("avgp2")!=-1) method=_calcAvgP2;
  else if (str.Index("avgp3")!=-1) method=_calcAvgP3;
  else if (str.Index("avgpw0")!=-1) method=_calcAvgPW0;
  else if (str.Index("avgpw1")!=-1) method=_calcAvgPW1;
  else if (str.Index("avgpw2")!=-1) method=_calcAvgPW2;
  else if (str.Index("avgpw3")!=-1) method=_calcAvgPW3;
  else if (str.Index("acalib")!=-1) method=_calcACalib;
  else if (str.Index("tsncalib")!=-1) method=_calcTSnCalib;
  else if (str.Index("tsxcalib")!=-1) method=_calcTSxCalib;
  else if (str.Index("wcalib")!=-1) method=_calcWCalib;
  else if (str.Index("rcalib")!=-1) method=_calcRCalib;
  else if (str.Index("unknown")!=-1) method=_calcUnknown;
  else if (str.Index("spec")!=-1) method=_calcSpec;
  else if (str.Index("last")!=-1) method=_calcLast;
  else if (str.Index("amp")!=-1) method=_calcAmp;
  else if (str.Index("tsn")!=-1) method=_calcTSn;
  else if (str.Index("tsx")!=-1) method=_calcTSx;
  else if (str.Index("width")!=-1) method=_calcWidth;
  else if (str.Index("ratio")!=-1) method=_calcRatio;
  else {
    std::cout << "cannot identify method from <" << str << ">\n";
    method=_calcUnknown;
  }
  return method;
}

// -------------------------------------------------------------

void ListMethods(int shortVersion)
{
  std::cout << "available methods (as strings): ";
  for (TMethod_t m=_calcAmp; m!=_calcLast; next(m)) {
    std::cout << " " << GetMethodName(m,shortVersion);
  }
  std::cout << "\n";
}

// -------------------------------------------------------------

void CheckMethodConversion(int shortVersion)
{
  std::cout << "available methods (as strings):\n";
  for (TMethod_t m=_calcAmp; m!=_calcLast; next(m)) {
    std::cout << "i=" << (int(m)-int(_calcAmp)) << "\n";
    std::cout << " " << GetMethodName(m,shortVersion) << "\n";
    std::cout << " " << GetMethodName(GetMethod(GetMethodName(m,shortVersion)),shortVersion) << "\n";
  }
  std::cout << "\n";
}

// -------------------------------------------------------------
// -------------------------------------------------------------

TString GetSpecMethodName(TSpecMethod_t method, int shortVersion) {
  TString name;
  switch(method) {
  case _specNone: name="specNone"; break;
  case _specAvgPhi: name="AvgPhi"; break;
  case _specLast: name="specLast"; break;
  case _specUnknown: name="specUnknown"; break;
  default:
    std::cout << "GetName(TSpecMethod_t): missing entry\n";
    name="specUNDEFINED";
  }
  if (!shortVersion) name.Prepend("SpecMethod_");
  return name;
}

// -------------------------------------------------------------

TSpecMethod_t GetSpecMethod(TString str) {
  TSpecMethod_t method=_specUnknown;
  str.ToLower();
  // change to the lower-case letters
  // amp, tsn, tsx, width, ratio are at the end
  if (str.Index("avgphi")!=-1) method=_specAvgPhi;
  else if (str.Index("specnone")!=-1) method=_specNone;
  else if (str.Index("spec-none")!=-1) method=_specNone;
  else if (str.Index("speclast")!=-1) method=_specLast;
  else if (str.Index("specunknown")!=-1) method=_specUnknown;
  else {
    std::cout << "cannot identify special method from <" << str << ">\n";
    method=_specUnknown;
  }
  return method;
}

// -------------------------------------------------------------

void ListSpecMethods(int shortVersion)
{
  std::cout << "available spec methods (as strings): ";
  for (TSpecMethod_t m=_specNone; m!=_specLast; next(m)) {
    std::cout << " " << GetSpecMethodName(m,shortVersion);
  }
  std::cout << "\n";
}

// -------------------------------------------------------------

void CheckSpecMethodConversion(int shortVersion)
{
  std::cout << "available special methods (as strings):\n";
  for (TSpecMethod_t m=_specNone; m!=_specLast; next(m)) {
    std::cout << "i=" << (int(m)-int(_specAvgPhi)) << "\n";
    std::cout << " " << GetSpecMethodName(m,shortVersion) << "\n";
    std::cout << " " << GetSpecMethodName(GetSpecMethod(GetSpecMethodName(m,shortVersion)),shortVersion) << "\n";
  }
  std::cout << "\n";
}

// -------------------------------------------------------------
// -------------------------------------------------------------

TString GetSpecMethodName(SpecMethodDef_t method, int shortVersion) {
  TString name;
  if (shortVersion) {
    name= GetSpecMethodName(method.method(), shortVersion) +
      TString("_") + method.name();
  }
  else {
    name= GetSpecMethodName(method.method(), shortVersion) +
      TString(" name: ") + method.name();
  }
  return name;
}

// -------------------------------------------------------------

/*
SpecMethodDef_t GetSpecMethodDef(TString str) {
  int idx= str.Index("_");
  TString name= (idx==-1) ? "undef" : str.Substr(idx,str.Length());
  TSpecMethod_t m=GetSpecMethod(str);
  SpecMethodDef_t method(name,m,imin,imax);
  if (m==_specUnknown) {
    std::cout << "GetSpecMethodDef: failed to determine the special method\n";
  }
  return method;
}
*/
// -------------------------------------------------------------
// -------------------------------------------------------------

TString GetHistoDimName(THistoDim_t hd) {
  TString name;
  switch(hd) {
  case _histo0: name="h0"; break;
  case _histo1F: name="h1F"; break;
  case _histo1D: name="h1D"; break;
  case _histo2F: name="h2F"; break;
  case _histo2D: name="h2D"; break;
  default:
    std::cout << "GetHistoDimName cannot handle this case int(hd)="
	      << int(hd) << "\n";
    name="hXD";
  }
  return name;
}

// -------------------------------------------------------------

THistoDim_t GetHistoDim(TString str)
{
  THistoDim_t hd=_histo0;
  str.ToLower();
  if (str.Index("h0")!=-1) hd=_histo0;
  else if (str.Index("h1f")!=-1) hd=_histo1F;
  else if (str.Index("h1d")!=-1) hd=_histo1D;
  else if (str.Index("h2f")!=-1) hd=_histo2F;
  else if (str.Index("h2d")!=-1) hd=_histo2D;
  else {
    std::cout << "GetHistoDim could not identify THistoDim_t from <"
	      << str << ">\n";
  }
  return hd;
}

// -------------------------------------------------------------
// -------------------------------------------------------------

TString GetSubDetFullName(TSubDet_t sd) {
  TString name="unknown";
  switch(sd) {
  case _sdHB1: name="HB1"; break;
  case _sdHB2: name="HB2"; break;
  case _sdHE1: name="HE1"; break;
  case _sdHE2: name="HE2"; break;
  case _sdHE3: name="HE3"; break;
  case _sdHO4: name="HO4"; break;
  case _sdHF1: name="HF1"; break;
  case _sdHF2: name="HF2"; break;
  case _sdLast: name="sdLast"; break;
  case _sdUnknown: name="sdUnknown"; break;
  default:
    std::cout << "GetSubDetFullName: unknown subdetector int(sb)="
	      << int(sd) << "\n";
  }
  return name;
}

// -------------------------------------------------------------

TString GetSubDetName(TSubDet_t sd) {
  TString name="unknown";
  switch(sd) {
  case _sdHB1: case _sdHB2:              name="HB"; break;
  case _sdHE1: case _sdHE2: case _sdHE3: name="HE"; break;
  case _sdHF1: case _sdHF2:              name="HF"; break;
  case _sdHO4:                           name="HO"; break;
  case _sdLast: name="sdLast"; break;
  case _sdUnknown: name="sdUnknown"; break;
  default:
    std::cout << "GetSubDetName is not ready for int(subdet)="
	      << int(sd) << "\n";
  }
  return name;
}

// -------------------------------------------------------------

int GetSubDetDepth(TSubDet_t sd) {
  int depth=-1;
  switch(sd) {
  case _sdHB1:  case _sdHE1:  case _sdHF1:    depth=1;    break;
  case _sdHB2:  case _sdHE2:  case _sdHF2:    depth=2;    break;
                case _sdHE3:                  depth=3;    break;
  case _sdHO4:                                depth=4;    break;
  default:
    std::cout << "GetSubDetDepth(sd=" << GetSubDetFullName(sd) << ") "
      "is not ready for this subdet\n";
  }
  return depth;
}

// -------------------------------------------------------------

TSubDet_t GetSubDet(TString str) {
  TSubDet_t sd=_sdUnknown;
  str.ToLower();
  if (str.Index("hb1")!=-1) sd=_sdHB1;
  else if (str.Index("hb2")!=-1) sd=_sdHB2;
  else if (str.Index("he1")!=-1) sd=_sdHE1;
  else if (str.Index("he2")!=-1) sd=_sdHE2;
  else if (str.Index("he3")!=-1) sd=_sdHE3;
  else if (str.Index("ho4")!=-1) sd=_sdHO4;
  else if (str.Index("hf1")!=-1) sd=_sdHF1;
  else if (str.Index("hf2")!=-1) sd=_sdHF2;
  else if (str.Index("sdlast")!=-1) sd=_sdLast;
  else if (str.Index("sdunknown")!=-1) sd=_sdUnknown;
  else {
    std::cout << "GetSubDet could not identify subdetector in <"
	      << str << ">\n";
  }
  return sd;
}

// -------------------------------------------------------------

TSubDet_t GetSubDet(TString str_inp, int depth) {
  TSubDet_t sd=_sdUnknown;
  TString str=str_inp;
  str.ToLower();
  str.Append(Form("%d",depth));
  if (str.Index("hb1")!=-1) sd=_sdHB1;
  else if (str.Index("hb2")!=-1) sd=_sdHB2;
  else if (str.Index("he1")!=-1) sd=_sdHE1;
  else if (str.Index("he2")!=-1) sd=_sdHE2;
  else if (str.Index("he3")!=-1) sd=_sdHE3;
  else if (str.Index("ho4")!=-1) sd=_sdHO4;
  else if (str.Index("hf1")!=-1) sd=_sdHF1;
  else if (str.Index("hf2")!=-1) sd=_sdHF2;
  else if (str.Index("sdlast")!=-1) sd=_sdLast;
  else if (str.Index("sdunknown")!=-1) sd=_sdUnknown;
  else {
    std::cout << "GetSubDet(str,depth) could not identify subdetector from <"
	      << str_inp << "> and depth=" << depth << "\n";
  }
  return sd;
}

// -------------------------------------------------------------
// -------------------------------------------------------------

SpecMethodDef_t::SpecMethodDef_t(TString set_name) :
  fMethod(_specNone), fIVec(), fName(set_name)
{}

// -------------------------------------------------------------

SpecMethodDef_t::SpecMethodDef_t(TString set_name, TSpecMethod_t set_method,
				 const std::vector<int> &set_idx) :
  fMethod(set_method), fIVec(set_idx), fName(set_name)
{}

// -------------------------------------------------------------

int SpecMethodDef_t::assign(TSpecMethod_t set_method,
			    int set_imin, int set_imax, int set_step)
{
  fMethod=set_method;
  fIVec.clear();

  if ((set_imin > set_imax) || (set_step==0)) {
    std::cout << "SpecMethodDef_t::assign set_imin=" << set_imin
	      << ", set_imax=" << set_imax << ", set_step=" << set_step
	      << ": values are not correctly ordered\n";
    return 0;
  }

  int ok=1;
  switch(set_method) {
  case _specNone: ok=1; break;
  case _specAvgPhi:
    if ((set_imin<=0) || (set_imax<=0) || (set_imin>=72) || (set_imax>72)) {
      std::cout << Form("SpecMethodDef_t::assign(specAvgPhi,%d,%d,%d):",
			set_imin,set_imax,set_step)
		<< " expected values are from 0 to 72(max)\n";
      ok=0;
    }
    fIVec.reserve(set_imax-set_imin);
    for (int iphi=set_imin; iphi<set_imax; iphi+=set_step) {
      fIVec.push_back(iphi);
    }
    break;
  case _specLast: ok=1; break;
  default:
    std::cout << "SpecMethodDef_t::assign is not ready for this special method\n";
    ok=0;
  }
  return ok;
}

// -------------------------------------------------------------

void SpecMethodDef_t::assign(const SpecMethodDef_t &s, TString new_name)
{
  fMethod=s.fMethod;
  fIVec=s.fIVec;
  if (new_name.Length()) fName=new_name; else fName=s.fName;
}

// -------------------------------------------------------------

TString SpecMethodDef_t::explainIdx() const
{
  TString explain;
  int allPhi=0;
  if (fIVec.size()==cIPhiMax) {
    allPhi=1;
    for (unsigned int i=0; allPhi && (i<fIVec.size()); i++) {
      if (fIVec[i]!=i+1)  allPhi=0;
    }
  }
  if (allPhi==1) explain="-ALLIPhi";
  else {
    explain="iphi=";
    int cont=0;
    for (unsigned int i=0; i<fIVec.size(); i++) {
      if ((i<fIVec.size()-1) && (fIVec[i]+1==fIVec[i+1])) {
	if (cont==0) {
	  if (i!=0) explain.Append(",");
	  explain.Append(Form("%d-",fIVec[i]));
	}
	cont++;
      }
      else {
	if ((i!=0) && !cont) explain.Append(",");
	explain.Append(Form("%d",fIVec[i]));
	cont=0;
      }
    }
  }
  return explain;
}

// -------------------------------------------------------------

SpecMethodDef_t& SpecMethodDef_t::operator=(const SpecMethodDef_t &s) {
  if (this==&s) return *this;
  fMethod=s.fMethod;
  fIVec=s.fIVec;
  fName=s.fName;
  return *this;
}

// -------------------------------------------------------------

std::ostream& operator<<(std::ostream& out, const SpecMethodDef_t &s)
{
  out << "SpecMethodDef: " << GetSpecMethodName(s) << "."
      << Form(" iVec[%d]: ",s.fIVec.size());
  for (unsigned int i=0; i<s.fIVec.size(); i++) {
    out << " " << s.fIVec[i];
  }
  return out;
}

// -------------------------------------------------------------
// -------------------------------------------------------------

HistoDef_t::HistoDef_t(TString set_calibtype, TString set_detector,
		       TMethod_t set_method, int set_depth) :
  fCalibType(set_calibtype),
  fDetector((set_detector.Length()) ? set_detector : "HF"),
  fMethod(set_method),
  fSpecMethod(),
  fDepth(set_depth),
  fPlotTitle("title"),
  fInpHistoName("inphisto"),
  fInpHistoName2("inphisto2"),
  fHistoDim(_histo0),
  fH1F(NULL), fH2F(NULL), fH2F_2(NULL), fH2F_ratio(NULL)
{
  if (set_method != _calcNone) this->setMethod(set_method,set_depth);
  if (set_calibtype.Length()==0) {
    std::cout << "Error: HistoDef::HistoDef set_calibtype cannot be empty\n"
	      << "Setting to LED\n";
    fCalibType="LED";
  }
}

// -------------------------------------------------------------

HistoDef_t::HistoDef_t(TString set_calibtype, TSubDet_t subdet,
		       TMethod_t set_method) :
  fCalibType(set_calibtype),
  fDetector(GetSubDetName(subdet)),
  fMethod(set_method),
  fSpecMethod(),
  fDepth(GetSubDetDepth(subdet)),
  fPlotTitle("title"),
  fInpHistoName("inphisto"),
  fInpHistoName2("inphisto2"),
  fHistoDim(_histo0),
  fH1F(NULL), fH2F(NULL), fH2F_2(NULL), fH2F_ratio(NULL)
{
  if (set_method != _calcNone) this->setMethod(set_method,fDepth);
  if (set_calibtype.Length()==0) {
    std::cout << "Error: HistoDef::HistoDef set_calibtype cannot be empty\n"
	      << "Setting to LED\n";
    fCalibType="LED";
  }
  if (!checkDetector(std::string(fDetector.Data()),fDepth)) {
    std::cout << "\n\tHistoDef_t::HistoDef_t error\n";
  }
}

// -------------------------------------------------------------

HistoDef_t::HistoDef_t(const InputCards_t &ic, TMethod_t set_method) :
  fCalibType(ic.calibType()),
  fDetector(ic.detector().c_str()),
  fMethod(set_method),
  fSpecMethod(),
  fDepth(ic.depth()),
  fPlotTitle("title"),
  fInpHistoName("inphisto"),
  fInpHistoName2("inphisto2"),
  fHistoDim(_histo0),
  fH1F(NULL), fH2F(NULL), fH2F_2(NULL), fH2F_ratio(NULL)
{
  if (set_method != _calcNone) this->setMethod(set_method,fDepth);
  if (!checkDetector(std::string(fDetector.Data()),fDepth)) {
    std::cout << "\n\tHistoDef_t::HistoDef_t(InputCards_t) error\n";
  }
}

// -------------------------------------------------------------

HistoDef_t::HistoDef_t(const HistoDef_t &d, TString cloneTag) :
  fCalibType(d.fCalibType),
  fDetector(d.fDetector),
  fMethod(d.fMethod),
  fSpecMethod(d.fSpecMethod),
  fDepth(d.fDepth),
  fPlotTitle(d.fPlotTitle),
  fInpHistoName(d.fInpHistoName),
  fInpHistoName2(d.fInpHistoName2),
  fHistoDim(d.fHistoDim),
  fH1F(d.fH1F),
  fH2F(d.fH2F),
  fH2F_2(d.fH2F_2),
  fH2F_ratio(d.fH2F_ratio)
{
  if (cloneTag.Length()) {
    TString hname;
    if (d.fH1F) {
      hname=TString(d.fH1F->GetName()) + cloneTag;
      fH1F=(TH1F*)d.fH1F->Clone(hname);
    }
    if (d.fH2F) {
      hname=TString(d.fH2F->GetName()) + cloneTag;
      fH2F=(TH2F*)d.fH2F->Clone(hname);
    }
    if (d.fH2F_2) {
      hname=TString(d.fH2F_2->GetName()) + cloneTag;
      fH2F_2=(TH2F*)d.fH2F_2->Clone(hname);
    }
    if (d.fH2F_ratio) {
      hname=TString(d.fH2F_ratio->GetName()) + cloneTag;
      fH2F_ratio=(TH2F*)d.fH2F_ratio->Clone(hname);
    }
  }
}

// -------------------------------------------------------------

TString HistoDef_t::getMethodName() const
{
  if (fSpecMethod.method()==_specNone) return GetMethodName(fMethod,1);
  TString name = fSpecMethod.name() + TString("_") + GetMethodName(fMethod,1);
  return name;
}

// -------------------------------------------------------------

int HistoDef_t::setMethod(TMethod_t set_method, int set_depth) {
  fMethod=set_method;
  fDepth=set_depth;
  fInpHistoName2="";

  TString histMapD= Form("h_mapDepth%d_HF",fDepth);
  TString histMap_ = "h_map_HF";

  switch(set_method) {
  case _calcAmp:
    fPlotTitle=Form("Time dependence of full Amplitude averaged over events of each %s run for HF channels",fCalibType.Data());
    fInpHistoName=Form("h_mapDepth%dADCAmpl_HF",fDepth);
    fInpHistoName2= histMapD;
    fHistoDim=_histo2F;
    break;
  case _calcTSn:
    fPlotTitle=Form("Time dependence of TSn averaged over events of each %s run for some(=9) random HF channels",fCalibType.Data());
    fInpHistoName=Form("h_mapDepth%dTSmeanA_HF",fDepth);
    fInpHistoName2= histMapD;
    fHistoDim=_histo2F;
    break;
  case _calcTSx:
    fPlotTitle=Form("Time dependence of TSx averaged over events of each %s run for some(=9) random HF channels",fCalibType.Data());
    fInpHistoName=Form("h_mapDepth%dTSmaxA_HF",fDepth);
    fInpHistoName2= histMapD;
    fHistoDim=_histo2F;
    break;
  case _calcWidth:
    fPlotTitle=Form("Time dependence of RMS averaged over events of each %s run for some(=9) random HF channels",fCalibType.Data());
    fInpHistoName=Form("h_mapDepth%dAmplitude_HF",fDepth);
    fInpHistoName2= histMapD;
    fHistoDim=_histo2F;
    break;
  case _calcRatio:
    fPlotTitle=Form("Time dependence of amplitude Ratio averaged over events of each %s run for some(=9) random HF channels",fCalibType.Data());
    fInpHistoName=Form("h_mapDepth%dAmpl_HF",fDepth);
    fInpHistoName2= histMapD;
    fHistoDim=_histo2F;
    break;
  case _calcAvgAmp:
    fPlotTitle=Form("Time dependence of full Amplitude averaged over events of each %s run and over all channels of HF",fCalibType.Data());
    fInpHistoName=Form("h_ADCAmpl_HF");
    fHistoDim=_histo1F;
    break;
  case _calcAvgTSn:
    fPlotTitle=Form("Time dependence of TSn averaged over events of each %s run and over all channels of HF",fCalibType.Data());
    fInpHistoName=Form("h_TSmeanA_HF");
    fHistoDim=_histo1F;
    break;
  case _calcAvgTSx:
    fPlotTitle=Form("Time dependence of TSx averaged over events of each %s run and over all channels of HF",fCalibType.Data());
    fInpHistoName=Form("h_TSmaxA_HF");
    fHistoDim=_histo1F;
    break;
  case _calcAvgWidth:
    fPlotTitle=Form("Time dependence of RMS averaged over events of each %s run and over all channels of HF",fCalibType.Data());
    fInpHistoName=Form("h_Amplitude_HF");
    fHistoDim=_histo1F;
    break;
  case _calcAvgRatio:
    fPlotTitle=Form("Time dependence of amplitude Ratio averaged over events of each %s run and over all channels of HF",fCalibType.Data());
    fInpHistoName=Form("h_Ampl_HF");
    fHistoDim=_histo1F;
    break;
  case _calcAvgP0:
    fPlotTitle=Form("Time dependence of Pedestal(cap0) averaged over events of each %s run and over all channels of HF",fCalibType.Data());
    fInpHistoName=Form("h_pedestal0_HF");
    fHistoDim=_histo1F;
    break;
  case _calcAvgP1:
    fPlotTitle=Form("Time dependence of Pedestal(cap1) averaged over events of each %s run and over all channels of HF",fCalibType.Data());
    fInpHistoName=Form("h_pedestal1_HF");
    fHistoDim=_histo1F;
    break;
  case _calcAvgP2:
    fPlotTitle=Form("Time dependence of Pedestal(cap2) averaged over events of each %s run and over all channels of HF",fCalibType.Data());
    fInpHistoName=Form("h_pedestal2_HF");
    fHistoDim=_histo1F;
    break;
  case _calcAvgP3:
    fPlotTitle=Form("Time dependence of Pedestal(cap3) averaged over events of each %s run and over all channels of HF",fCalibType.Data());
    fInpHistoName=Form("h_pedestal3_HF");
    fHistoDim=_histo1F;
    break;
  case _calcAvgPW0:
    fPlotTitle=Form("Time dependence of PedestalWidth(cap0) averaged over events of each %s run and over all channels of HF",fCalibType.Data());
    fInpHistoName=Form("h_pedestalw0_HF");
    fHistoDim=_histo1F;
    break;
  case _calcAvgPW1:
    fPlotTitle=Form("Time dependence of PedestalWidth(cap1) averaged over events of each %s run and over all channels of HF",fCalibType.Data());
    fInpHistoName=Form("h_pedestalw1_HF");
    fHistoDim=_histo1F;
    break;
  case _calcAvgPW2:
    fPlotTitle=Form("Time dependence of PedestalWidth(cap2) averaged over events of each %s run and over all channels of HF",fCalibType.Data());
    fInpHistoName=Form("h_pedestalw2_HF");
    fHistoDim=_histo1F;
    break;
  case _calcAvgPW3:
    fPlotTitle=Form("Time dependence of PedestalWidth(cap3) averaged over events of each %s run and over all channels of HF",fCalibType.Data());
    fInpHistoName=Form("h_pedestalw3_HF");
    fHistoDim=_histo1F;
    break;
  case _calcACalib:
    fPlotTitle=Form("Time dependence of A_calib averaged over events of each %s run for some(=9) random HF channels",fCalibType.Data());
    fInpHistoName=Form("h_mapADCCalib_HF");
    fInpHistoName2= histMap_;
    fHistoDim=_histo2F;
    break;
  case _calcTSnCalib:
    fPlotTitle=Form("Time dependence of TSn_calib averaged over events of each %s run for some(=9) random HF channels",fCalibType.Data());
    fInpHistoName=Form("h_mapTSmeanCalib_HF");
    fInpHistoName2= histMap_;
    fHistoDim=_histo2F;
    break;
  case _calcTSxCalib:
    fPlotTitle=Form("Time dependence of TSx_calib averaged over events of each %s run for some(=9) random HF channels",fCalibType.Data());
    fInpHistoName=Form("h_mapTSmaxCalib_HF");
    fInpHistoName2= histMap_;
    fHistoDim=_histo2F;
    break;
  case _calcWCalib:
    fPlotTitle=Form("Time dependence of W_calib averaged over events of each %s run for some(=9) random HF channels",fCalibType.Data());
    fInpHistoName=Form("h_mapWidthCalib_HF");
    fInpHistoName2= histMap_;
    fHistoDim=_histo2F;
    break;
  case _calcRCalib:
    fPlotTitle=Form("Time dependence of Ratio_calib averaged over events of each %s run for some(=9) random HF channels",fCalibType.Data());
    fInpHistoName=Form("h_mapRatioCalib_HF");
    fInpHistoName2= histMap_;
    fHistoDim=_histo2F;
    break;
  default:
    std::cout << "method HistoDef_t::setMethod() is not ready for this method\n";
    std::cout <<   "set_method=" << GetMethodName(set_method)
	      << ", set_depth=" << set_depth << "\n";
  }

  if (fDetector!="HF") {
    fPlotTitle.ReplaceAll("HF",fDetector);
    fInpHistoName.ReplaceAll("HF",fDetector);
    fInpHistoName2.ReplaceAll("HF",fDetector);
  }
  return 1;
}

// -------------------------------------------------------------

int HistoDef_t::setMethod(TString reset_calibtype, TMethod_t set_method,
			  int set_depth)
{
  if (reset_calibtype.Length()==0) {
    std::cout << "Error: HistoDef::setMethod reset_calibtype cannot be empty\n";
    return 0;
  }
  fCalibType=reset_calibtype;
  return this->setMethod(set_method,set_depth);
}

// -------------------------------------------------------------

int HistoDef_t::setSpecMethod(TSpecMethod_t specM, InputCards_t &ic,
			      TString set_specMethodTag)
{
  if (specM==_specNone) {
    fSpecMethod.clear();
    return 1;
  }

  if (fMethod==_calcNone) {
    std::cout << "HistoDef_t::setSpecMethod should be called after setMethod\n";
    return 0;
  }

  // Get iphi values, set a special flag in InputCardds
  std::vector<int> iphiVec=ic.iphi();
// ok fix
  ic.edit_iphi().clear();
  ic.edit_iphi().push_back(-1111);
  if (iphiVec.size()) std::sort(iphiVec.begin(),iphiVec.end());

  //  initialize fSpecMethod. The final name is formed in the 2nd step
  if (!fSpecMethod.assign(specM,iphiVec,set_specMethodTag)) {
    std::cout << "error in HistoDef_t::setSpecMethod\n";
    return 0;
  }
  TString explain= fSpecMethod.explainIdx();
  TString forNameTag=explain;
  forNameTag.ReplaceAll("iphi=","");
  fSpecMethod.nameTag(forNameTag + set_specMethodTag);

  fPlotTitle.ReplaceAll("averaged","averaged " + explain);
  std::cout << "updated title <" << fPlotTitle << ">\n";

  return 1;
}

// -------------------------------------------------------------

int HistoDef_t::setSpecMethod(const SpecMethodDef_t &specM)
{
  if (specM.method()==_specNone) {
    fSpecMethod.clear();
    return 1;
  }

  if (fMethod==_calcNone) {
    std::cout << "HistoDef_t::setSpecMethod should be called after setMethod\n";
    return 0;
  }

  fSpecMethod=specM;
  return 1;
}

// -------------------------------------------------------------

THistoDim_t HistoDef_t::useHistoDim() const
{
  if (fSpecMethod.method()==_specNone) return fHistoDim;
  else return _histo1F;
}

// -------------------------------------------------------------

int HistoDef_t::loadHisto(TFile &f, TString setTag)
{
  if (!f.IsOpen()) {
    std::cout << "HistoDef_t::loadHisto: file <" << f.GetName() << "> is not open\n";
    return 0;
  }
  fH1F=NULL; fH2F=NULL; fH2F_2=NULL; fH2F_ratio=NULL;

  if (fHistoDim==_histo1F) {
    fH1F=(TH1F*)f.Get(fInpHistoName);
    if (!fH1F) {
      std::cout << "HistoDef_t::loadHisto: loading of <"
		<< fInpHistoName << "> from file <" << f.GetName()
		<< "> failed\n";
      return 0;
    }
    if (fH1F->GetNbinsX()>2000) {
      std::cout << "HistoDef_t::loadHisto: loaded 1D histogram "
		<< fH1F->GetName()
		<< " with an unexpectedly large number of bins : "
		<< fH1F->GetNbinsX() << "\n";
      return 0;
    }
    if (setTag.Length()) {
      TH1F *h1=fH1F;
      fH1F=(TH1F*)h1->Clone(h1->GetName() + setTag);
      delete h1;
      if (!fH1F) {
	std::cout << "HistoDef_t::loadHisto: clone of 1F histo failed\n";
	return 0;
      }
    }
    fH1F->SetDirectory(0);
    return 1;
  }
  else if (fHistoDim==_histo2F) {
    std::cout << "Loading histo2F: " << fInpHistoName << " and "
	      << fInpHistoName2 << "\n";

    fH2F=(TH2F*)f.Get(fInpHistoName);
    if (!fH2F) {
      std::cout << "HistoDef_t::loadHisto: loading of <"
		<< fInpHistoName << "> from file <" << f.GetName()
		<< "> failed\n";
      return 0;
    }
    if ((fH2F->GetNbinsX()>2000) || (fH2F->GetNbinsY()>2000)) {
      std::cout << "HistoDef_t::loadHisto: loaded 2D histogram "
		<< fH2F->GetName()
		<< " with an unexpectedly large number of bins : "
		<< fH2F->GetNbinsX() << " x " << fH2F->GetNbinsY() << "\n";
      return 0;
    }
    //std::cout << "fH2F " << fH2F->GetName() << " dim: "<< fH2F->GetNbinsX() << " x " << fH2F->GetNbinsY() << "\n";
    if (fInpHistoName2.Length()) {
      fH2F_2=(TH2F*)f.Get(fInpHistoName2);
      if (!fH2F_2) {
	std::cout << "HistoDef_t::loadHisto: loading of <"
		  << fInpHistoName2 << "> from file <" << f.GetName()
		  << "> failed\n";
	return 0;
      }
      if (setTag.Length()) {
	TH2F *h2=fH2F_2;
	fH2F_2=(TH2F*)h2->Clone(h2->GetName() + setTag);
	delete h2;
	if (!fH2F_2) {
	  std::cout << "HistoDef_t::loadHisto: clone of 2F_2 histo failed\n";
	  return 0;
	}
      }
      fH2F_2->SetDirectory(0);
    }
    if (setTag.Length()) {
      TH2F *h2=fH2F;
      fH2F=(TH2F*)h2->Clone(h2->GetName() + setTag);
      delete h2;
      if (!fH2F) {
	std::cout << "HistoDef_t::loadHisto: clone of 2F histo failed\n";
	return 0;
      }
    }
    fH2F->SetDirectory(0);

    if (fH2F && fH2F_2) {
      fH2F_ratio= (TH2F*)fH2F->Clone(TString("Ceff_") + fH2F->GetName());
      if (!fH2F_ratio) {
	std::cout << "HistoDef_t::loadHisto: failed to clone fH2F for fH2F_ratio\n";
	return 0;
      }
      fH2F_ratio->Divide(fH2F, fH2F_2, 1, 1, "B");
      fH2F_ratio->SetDirectory(0);
    }

    switch (fSpecMethod.method()) {
    case _specNone: break;
    case _specAvgPhi: {
      if (!fH2F_ratio) {
	std::cout << "HistoDef_t::loadHisto for specAvgPhi requires fH2F_ratio\n";
	return 0;
      }
      if (fH2F_ratio->GetNbinsX()!=cIEtaMax) {
	std::cout << "HistoDef_t::loadHisto for specAvgPhi expects x-axis to have " << cIEtaMax << " bins\n";
	return 0;
      }
      TString hname=fH2F_ratio->GetName() + TString("_avgPhi");
      fH1F= new TH1F(hname,hname,cIEtaMax, -41.,41.);
      if (!fH1F) {
	std::cout << "HistoDef_t::loadHisto: failed to create histo <" << hname
		  << ">\n";
	return 0;
      }
      fH1F->SetDirectory(0);

        std::cout<<" Spec method "<<_specAvgPhi<<std::endl;

      if (fSpecMethod.iVec().size()>0) {
	for (int ietaBin=1; ietaBin<=fH1F->GetNbinsX(); ++ietaBin) {
	  Float_t sum=0;
	  Float_t sumErr2=0;
	  for (unsigned int i=0; i<fSpecMethod.iVec().size(); ++i) {
	    int iphiBin= fSpecMethod.i(i) + 1;

              std::cout<<" iphiBin "<<iphiBin<<std::endl;

	    sum += fH2F_ratio->GetBinContent(ietaBin,iphiBin);
	    sumErr2 += pow(fH2F_ratio->GetBinError(ietaBin,iphiBin), 2);
	  }
	  sum /= Float_t(fSpecMethod.iVec().size());
	  sumErr2= sqrt(sumErr2)/Float_t(fSpecMethod.iVec().size());

            std::cout<<" Sum "<<ietaBin<<" "<<sum<<std::endl;

	  fH1F->SetBinContent(ietaBin, sum);
	  fH1F->SetBinError(ietaBin, sumErr2);
	}
      }
    } // spec method AvgPhi
      break;
    default:
      std::cout << "HistoDef_t::loadHisto cannot handle special method "
		<< fSpecMethod << "\n";
      return 0;
    }

    return 1;
  }
  std::cout << "HistoDef_t::loadHisto error: object is not prepared\n";
  return 0;
}

// -------------------------------------------------------------

std::ostream& operator<< (std::ostream &out, const HistoDef_t &hd) {
  out << "HistoDef_t:: calibType=" << hd.fCalibType
      << ", detector=" << hd.fDetector
      << ", depth=" << hd.fDepth << "\n";
  out << " method=" << hd.fMethod << ", specMethod=(" << hd.fSpecMethod;
  out << "),\n plotTile=<" << hd.fPlotTitle << ">\n";
  out << " inpHistoName =<" << hd.fInpHistoName  << ">\n";
  out << " inpHistoName2=<" << hd.fInpHistoName2 << ">\n";
  out << " histoDim=" << hd.fHistoDim << "\n";
  return out;
}

// -------------------------------------------------------------
// -------------------------------------------------------------

TCanvas *MakePlotHD(HistoDef_t inpHD, const InputCards_t &ic)
{
  if (ic.runData().size()==0) {
    std::cout << "MakePlotHD: nothing to display\n";
    return NULL;
  }
  if (std::string(inpHD.calibType().Data())!=ic.calibType()) {
    std::cout << "Error in calibType mismatch:\n"
	      << "  HistoDef calibType=" << inpHD.calibType() << "\n"
	      << "  InputCards calibType=" << ic.calibType() << "\n";
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

  Float_t *timeArr=NULL;
  std::vector<std::string> filesv;

  if (!ic.prepareRootFileNamesAndTimer(filesv,&timeArr)) {
    std::cout << "preparation failed\n";
    gStyle->SetTimeOffset(storeTimeOffset.Convert());
  }

  Float_t timeMin= timeArr[0];
  Float_t timeMax= timeArr[filesv.size()-1];

  int iphi=ic.iphi()[0];
  int numdates = int(filesv.size());
  int numetas = int(ic.ieta().size());
  std::cout << "MakePlotHD:Period: " << timeMinStr << " -- " << timeMaxStr
	    << " (" << numdates << " values)\n";
  std::cout << "MakePlotHD:Run range: " << ic.runData()[0].runNo() << " -- " << ic.runData().back().runNo() << "\n";
  std::cout << std::endl;


  TString outfile=Form("fig_%s_outDepth%d_phi%d_HF.gif",
		       GetMethodName(inpHD.method(),1).Data(), ic.depth(),iphi);
  TString outleg=Form("HF depth %d, iphi=%d",ic.depth(), iphi);


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
  t->DrawLatex(0.9, 15.5, inpHD.plotTitle());
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

  // Load histograms
  std::vector<TH1F*> h1fV;
  std::vector<TH2F*> h2fV;
  std::vector<TH2F*> h2f2V;

  for (unsigned int i=0; i<filesv.size(); ++i) {
    TFile *f= new TFile(filesv[i].c_str(), "READ");
    if (!f || !f->IsOpen()) {
      std::cout << "failed to open the file <" << filesv[i] << ">\n";
    }
    else {
      if (!inpHD.loadHisto(*f,Form("_file%d",i))) {
	std::cout << "error loading from file\n";
	return NULL;
      }
      h1fV.push_back(inpHD.getHisto());
      h2fV.push_back(inpHD.getHisto2F());
      h2f2V.push_back(inpHD.getHisto2F_2());
    }
    delete f;
  }

  Int_t NP = int(h1fV.size());

  for(std::vector<int>::const_iterator meta=ic.ieta().begin();
      meta<ic.ieta().end(); meta++) {

    // for loop over time points(LED runs)
    for (unsigned int k1=0; k1<h1fV.size(); k1++) {

      std::cout << "for " << ic.calibType() << " run k1=" << k1
		<<"  Number of imposed points = " << NP <<" "<<*meta<<std::endl;

      switch(inpHD.method()) {
      case _calcAvgAmp:
      case _calcAvgTSn:
      case _calcAvgTSx:
      case _calcAvgWidth:
      case _calcAvgRatio:
      case _calcAvgP0: case _calcAvgP1: case _calcAvgP2: case _calcAvgP3:
      case _calcAvgPW0: case _calcAvgPW1: case _calcAvgPW2: case _calcAvgPW3:
	std::cout << "get mean of " << h1fV[k1]->GetName() << "\n";
	avgW[k1] = h1fV[k1]->GetMean();
	break;
      case _calcAmp:
      case _calcTSn:
      case _calcTSx:
      case _calcWidth:
      case _calcRatio:
      case _calcACalib:
      case _calcTSnCalib:
      case _calcTSxCalib:
      case _calcWCalib:
      case _calcRCalib: {
	TH2F *twod1 = h2fV[k1];
	TH2F *twod0 = h2f2V[k1];
	TH2F *Ceff= (TH2F*)twod1->Clone("Ceff");
	if (!Ceff) {
	  std::cout << "failed to clone " << twod1->GetName() << "\n";
	  return NULL;
	}
        //std::cout<<" Bin content "<<twod1->GetBinContent(28,1)<<std::endl;
	std::cout << "divide " << twod1->GetName() << " by " << twod0->GetName() << "\n";
	Ceff->Divide(twod1,twod0, 1, 1, "B");
	if (k1==0) {
	  std::cout << "for 1st " << ic.calibType() << " run:"
		    << " nx = " << Ceff->GetXaxis()->GetNbins()
		    << ", ny = " << Ceff->GetYaxis()->GetNbins() << "\n";
	}
	Int_t i = (*meta);
	Int_t j = iphi;
	avgW[k1] = Ceff->GetBinContent(i,j);
	if(i == 2) std::cout<<j<<" RRRR "<<avgW[k1]<<" "<<i<<std::endl;
	delete Ceff;
      } break;
      default:
	std::cout << "not ready for this method\n";
	return NULL;
      }
    } // loop over periods (k1)

    // normalization on 1st time point
    for(int k2 = 0; k2<NP; k2++) {
      if (avgW[0]<=0.) { ratio[k2] = 0.; }
      else {
	ratio[k2] = avgW[k2]/avgW[0];
      }
      std::cout<<"Point:k2="<<k2<<" avgW[k2]="<<avgW[k2]<<" avgW[0]="<<avgW[0]<<" ratio[k2]="<<ratio[k2]<<std::endl;
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

  }// meta

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

// -------------------------------------------------------------
// -------------------------------------------------------------
