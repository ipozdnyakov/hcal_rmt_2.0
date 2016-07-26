

#include "ObjClass.h"
#include "HistoDef.h"
#include "ShowTimeDep.h"

//const Int_t Method = 1;
//     1-   A,     2-TSn,        3-TSx,        4-W,        5-R,
//     6-  <A>,    7-<TSn>,      8-<TSx>,      9-<W>,     10-<R>,
//    11- <P0>,   12-<P1>,      13-<P2>,      14-<P3>,
//    15-<PW0>,   16-<PW1>,     17-<PW2>,     18-<PW3>,
//    19-  no,     20-no,
//    21-A_calib, 22-TSn_calib, 23-TSx_calib, 24-W_calib, 25-R_calib,
//______________________________________________________________________________

int main(int argc, char *argv[])
{
  if (argc<2) {
    std::cout << "use: ./zamt_hf5.exe inputCardsFile [method] [-calib-VALUE] [-detector NAMEDEPTH] [-run-range runNumMin runNumMax] [-run-period runDateMin runDateMax] [other_options]\n";
    std::cout << "  VALUE for calib LED (default), LASER or PEDESTAL\n";
    std::cout << "  date format year/month/day\n";
    ListMethods(1);
    //std::cout << "\n\nInternal check\n"; CheckMethodConversion(1);
    std::cout << "  other_options=\n";
    std::cout << "     -fixed-yrangeX      (here X=0,1)\n";
    std::cout << "     -suppress-errorX    (here X=0,1)\n";
    std::cout << "     -output-format FORMAT (here FORMAT='gif','png',etc.)\n";
    std::cout << "     -output-dir NAME\n";
    std::cout << "     -output-file-name NAME  (file for canvas collection)\n";
    std::cout << "     -iphi-value VALUE\n";
    std::cout << "     -ieta-value VALUE\n";
    std::cout << "     -iphi-range valueMin valueMax valueStep\n";
    std::cout << "     -ieta-range valueMin valueMax valueStep\n";
    std::cout << "     -iphi-set count val1 ... valCount\n";
    std::cout << "     -ieta-set count val1 ... valCount\n";
    std::cout << "     -spec-method-avgPhi\n";
    std::cout << "  VALUE for iphi = iphiMin .. iphiMax (TBD)\n";
    std::cout << "  VALUE for ieta = iphiMin .. iphiMax (TBD)\n";
    gSystem->Exit(1);
  }
  std::string fname=argv[1];
  TMethod_t method=_calcAmp;
  if ((argc>=3) && (argv[2][0]!='-')) {
    method=GetMethod(argv[2]);
    if (method==_calcUnknown) {
      std::cout << "error in the method name\n";
      gSystem->Exit(1);
    }
  }

  PlotOptions_t plotOpt;
  std::string user_calibration="undef";
  std::string user_detector;
  int user_depth=-1;
  InputRuns_t *inpRunMin=NULL, *inpRunMax=NULL;
  std::vector<int> user_ieta,user_iphi;
  TString rootFileName="collection.root";
  TSpecMethod_t user_specMethod=_specNone;
  for (int ia=2; ia<argc; ia++) {
    std::string s(argv[ia]);
    if (s.find("-calib-")!=std::string::npos) {
      user_calibration=s.substr(7,s.size());
      std::cout << " ** calibration=" << user_calibration << "\n";
    }
    else if (s.find("-detector")!=std::string::npos) {
      if (ia+1>=argc) {
	std::cout << " error: keyword '-detector' needs 1 additional argument -- detector with the depth number at the end of the string\n";
	gSystem->Exit(1);
      }
      user_detector=argv[ia+1];
      user_depth=atoi(user_detector.c_str()+user_detector.size()-1);
      std::cout << " ** detector=<" << user_detector << ">, depth="
		<< user_depth << "\n";
      if (!checkDetector(user_detector,user_depth)) {
	std::cout << "error: detector check failed\n";
	gSystem->Exit(1);
      }
    }
    else if (s.find("-run-range")!=std::string::npos) {
      if (ia+2>=argc) {
	std::cout << " error: keyword '-run-range' needs 2 following numbers\n";
	gSystem->Exit(1);
      }
      int runMin=atoi(argv[ia+1]);
      int runMax=atoi(argv[ia+2]);
      std::cout << " ** run range = " << runMin << " .. " << runMax << "\n";
      if (runMax<runMin) {
	std::cout << "error runMax<runMin\n";
	gSystem->Exit(1);
      }
      if (inpRunMin) {
	std::cout << "error: specify either 'run range' or 'run period'\n";
	gSystem->Exit(1);
      }
      ia+=2;
      inpRunMin=new InputRuns_t();
      inpRunMin->setRun(runMin);
      inpRunMax=new InputRuns_t();
      inpRunMax->setRun(runMax);
    }
    else if (s.find("-run-period")!=std::string::npos) {
      if (ia+2>=argc) {
	std::cout << " error: keyword '-run-period' needs 2 following date-strings\n";
	gSystem->Exit(1);
      }
      std::string runMin=argv[ia+1];
      std::string runMax=argv[ia+2];

      std::cout << " ** run period = " << runMin << " .. " << runMax << "\n";
      if (runMax<runMin) {
	std::cout << "error runMax<runMin\n";
	gSystem->Exit(1);
      }
      if (inpRunMin) {
	std::cout << "error: specify either 'run range' or 'run period'\n";
	gSystem->Exit(1);
      }
      ia+=2;
      inpRunMin=new InputRuns_t();
      inpRunMin->setDate(runMin);
      inpRunMax=new InputRuns_t();
      inpRunMax->setDate(runMax);
    }
    else if (s.find("-fixed-yrange")!=std::string::npos) {
      int value=0;
      if (s.size()==strlen("-fixed-yrange")) value=1;
      else value=atoi(s.c_str() + strlen("-fixed-yrange"));
      plotOpt.fFixedYRange=value;
      std::cout << " ** fixed_yrange = " << value << "\n";
    }
    else if (s.find("-suppress-err")!=std::string::npos) {
      int value=0;
      if (s.size()==strlen("-suppress-err")) value=1;
      else value=atoi(s.c_str() + strlen("-suppress-err"));
      plotOpt.fSuppressErrorInPlots=value;
      std::cout << " ** suppressError = " << value << "\n";
    }
    else if (s.find("-output-format")!=std::string::npos) {
      if (ia+1>=argc) { std::cout << "missing argument for -output-format\n"; gSystem->Exit(1); }
      plotOpt.fOutFigFormat=argv[ia+1];
      std::cout << " ** plot output format = <" << plotOpt.fOutFigFormat << ">\n";
      ia++;
    }
    else if (s.find("-output-dir")!=std::string::npos) {
      if (ia+1>=argc) { std::cout << "missing argument for -output-dir\n"; gSystem->Exit(1); }
      plotOpt.fOutDirName=argv[ia+1];
      std::cout << " ** plot output directory = <" << plotOpt.fOutDirName << ">\n";
      ia++;
    }
    else if (s.find("-output-file-name")!=std::string::npos) {
      if (ia+1>=argc) { std::cout << "missing argument for -output-file-name\n"; gSystem->Exit(1); }
      rootFileName=argv[ia+1];
      std::cout << " ** save canvases to <" << rootFileName << ">\n";
      ia++;
    }
    else if (s.find("-iphi-value")!=std::string::npos) {
      if (ia+1>=argc) { std::cout << "missing argument for -iphi-value\n"; gSystem->Exit(1); }
      int value=atoi(argv[ia+1]);
      user_iphi.push_back(value);
      std::cout << " ** user_iphi=" << value << "\n";
      ia++;
    }
    else if (s.find("-ieta-value")!=std::string::npos) {
      if (ia+1>=argc) { std::cout << "missing argument for -ieta-value\n"; gSystem->Exit(1); }
      int value=atoi(argv[ia+1]);
      user_ieta.push_back(value+41); // convert to index
      std::cout << " ** user_ieta=" << value << " (converted to ieta_idx="
		<< user_ieta.back() << ")\n";
      ia++;
    }
    else if (s.find("-iphi-range")!=std::string::npos) {
      if (ia+3>=argc) { std::cout << "missing arguments for -iphi-range\n"; gSystem->Exit(1); }
      int valMin=atoi(argv[ia+1]);
      int valMax=atoi(argv[ia+2]);
      int valStep=atoi(argv[ia+3]);
      for (int value= valMin; value<=valMax; value+=valStep) {
	user_iphi.push_back(value);
	std::cout << " ** user_iphi=" << value << "\n";
      }
      ia+=3;
    }
    else if (s.find("-ieta-range")!=std::string::npos) {
      if (ia+3>=argc) { std::cout << "missing arguments for -ieta-range\n"; gSystem->Exit(1); }
      int valMin=atoi(argv[ia+1]);
      int valMax=atoi(argv[ia+2]);
      int valStep=atoi(argv[ia+3]);
      for (int value= valMin; value<=valMax; value+=valStep) {
	user_ieta.push_back(value+41);
	std::cout << " ** user_ieta=" << value << " (converted to ieta_idx="
		  << user_ieta.back() << ")\n";
      }
      ia+=3;
    }
    else if (s.find("-iphi-set")!=std::string::npos) {
      if (ia+2>=argc) { std::cout << "missing arguments for -iphi-set\n"; gSystem->Exit(1); }
      int valCount=atoi(argv[ia+1]);
      int iaMax=ia+2+valCount;
      for (ia=ia+2; ia<iaMax; ia++) {
	int value=atoi(argv[ia]);
	user_iphi.push_back(value);
	std::cout << " ** user_iphi=" << value << "\n";
      }
      std::sort(user_iphi.begin(),user_iphi.end());
      ia--;
    }
    else if (s.find("-ieta-set")!=std::string::npos) {
      if (ia+2>=argc) { std::cout << "missing arguments for -ieta-set\n"; gSystem->Exit(1); }
      int valCount=atoi(argv[ia+1]);
      int iaMax=ia+2+valCount;
      for (ia=ia+2; ia<iaMax; ia++) {
	int value=atoi(argv[ia]);
	user_ieta.push_back(value+41);
	std::cout << " ** user_ieta=" << value << " (converted to ieta_idx="
		  << user_ieta.back() << ")\n";
      }
      std::sort(user_ieta.begin(),user_ieta.end());
      ia--;
    }
    else if (s.find("-spec-method-avgphi")!=std::string::npos) {
      std::cout << " ** spec_method_avgphi=1\n";
      user_specMethod=_specAvgPhi;
    }
  }

  InputCards_t ic;
  int ok=ic.load(fname,inpRunMin,inpRunMax);
  HERE("load ok=",ok);
  if (ic.size()==0) {
    std::cout << "no runs to work with\n";
    ok=0;
  }
  if (user_iphi.size()) { // hack the object
    ic.edit_iphi() = user_iphi;
  }
  if (user_ieta.size()) { // hack the object
    ic.edit_ieta() = user_ieta;
  }
  if (user_calibration!="undef") ic.calibType(user_calibration);
  if (user_depth!=-1) ic.depth(user_depth);
  if (ok && (user_detector.size()>0)) ok=ic.detector(user_detector); // may change the depth
  if (!ok) gSystem->Exit(1);

  HERE("try to make the plot");

  HistoDef_t hd(ic.calibType(),ic.detector().c_str(),method,ic.depth());
  //std::cout << "hd(0) " << hd;
  if (!hd.setSpecMethod(user_specMethod,ic)) {
    std::cout << "failed to prepare spec method\n";
    gSystem->Exit(1);
  }
  //std::cout << "hd(1) " << hd;

  std::vector<TCanvas*>* cV= MakeTimeDepPlot(hd, ic, plotOpt);
  if (!cV) std::cout << "Failed to produce time-dependency plots\n";
  else {
    std::cout << "MakeTimeDepPlot OK\n";
    TFile fout(rootFileName,"recreate");
    if (!fout.IsOpen()) {
      std::cout << "Failed to create a file <" << rootFileName << ">\n";
    }
    else {
      for (unsigned int iCanv=0; iCanv<cV->size(); iCanv++) {
	cV->at(iCanv) -> Write();
      }
      fout.Close();
      std::cout << "File <" << fout.GetName() << "> created\n";
    }
  }

  if (cV) {
    int res= createWebPage(*cV,hd,ic,plotOpt);
  }

  std::cout << "exiting\n";
  gSystem->Exit(0);

}
//______________________________________________________________________________
