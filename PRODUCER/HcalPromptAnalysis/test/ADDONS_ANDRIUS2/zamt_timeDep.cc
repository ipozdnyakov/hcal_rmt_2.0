

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
    std::cout << "use: ./zamt_timeDep.exe inputCardsFile [method] [-calib-VALUE] [-detector NAMEDEPTH] [-run-range runNumMin runNumMax] [-run-period runDateMin runDateMax] [other_options]\n";
    std::cout << "  VALUE for calib LED (default), LASER or PEDESTAL\n";
    std::cout << "  date format year/month/day\n";
    ListMethods(1);
    //std::cout << "\n\nInternal check\n"; CheckMethodConversion(1);
    std::cout << "  other_options=\n";
    std::cout << "     -fixed-yrangeX      (here X=0,1)\n";
    std::cout << "     -suppress-errorX    (here X=0,1)\n";
    std::cout << "     -output-format FORMAT (here FORMAT='gif','png',etc.)\n";
    std::cout << "     -output-dir NAME\n";
    std::cout << "     -output-file-name NAME  (file name for canvas collection)\n";
    std::cout << "     -iphi-all\n";
    std::cout << "     -iphi-value VALUE\n";
    std::cout << "     -ieta-value VALUE\n";
    std::cout << "     -iphi-range valueMin valueMax valueStep\n";
    std::cout << "     -ieta-range valueMin valueMax valueStep\n";
    std::cout << "     -iphi-set count val1 ... valCount\n";
    std::cout << "     -ieta-set count val1 ... valCount\n";
    std::cout << "     -spec-avgPhi (activated by default)\n";
    std::cout << "     -spec-none   (deactivate avgPhi)\n";
    std::cout << "     -save-all    (whether to save all produced canvases)\n";
    std::cout << "  VALUE for iphi = iphiMin .. iphiMax (TBD)\n";
    std::cout << "  VALUE for ieta = iphiMin .. iphiMax (TBD)\n";
    gSystem->Exit(1);
  }
  std::string fname=argv[1];
  TMethod_t user_method=_calcNone;
  if ((argc>=3) && (argv[2][0]!='-')) {
    user_method=GetMethod(argv[2]);
    if (user_method==_calcUnknown) {
      std::cout << "error in the method name\n";
      gSystem->Exit(1);
    }
  }

  PlotOptions_t plotOpt;
  std::string user_calibration="undef";
  std::string user_detector;
  int user_depth=-1;
  int user_saveAll=-1;
  InputRuns_t *inpRunMin=NULL, *inpRunMax=NULL;
  std::vector<int> user_ieta,user_iphi;
  TString rootFileName;
  TSpecMethod_t user_specMethod=_specUnknown;
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
    else if (s.find("-iphi-all")!=std::string::npos) {
      std::cout << " ** iphi-all detected\n";
      for (int i=1; i<=cIPhiMax; i++) {
	user_iphi.push_back(i);
      }
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
    else if (s.find("-spec")!=std::string::npos) {
      user_specMethod=GetSpecMethod(s);
      std::cout << " ** spec_method=<" << GetSpecMethodName(user_specMethod)
		<< ">\n";
      if (user_specMethod==_specUnknown) {
	std::cout << " -- detection error\n";
	ListSpecMethods(1);
	gSystem->Exit(3);
      }
    }
    else if (s.find("-save-all")!=std::string::npos) {
      user_saveAll=1;
      std::cout << " ** saveAll detected\n";
    }
  }

  // Check if date auto-set is needed
  if (0) if (!inpRunMin && !inpRunMax) {
    TDatime now;
    UInt_t timeNow=now.Convert();
    // start period - 90 days ago
    UInt_t nDays=90;
    UInt_t timeStart= timeNow - 60 * 60 * 24 * nDays;
    TDatime start(timeStart);
    inpRunMin= new InputRuns_t();
    inpRunMax= new InputRuns_t();
    inpRunMin->setDate(start.AsSQLString());
    inpRunMax->setDate(now.AsSQLString());
    std::cout << " +-+ auto set period for " << nDays << " days: from "
	      << *inpRunMin << " to " << *inpRunMax << "\n";
  }

  InputCards_t ic;
  int ok=ic.load(fname,inpRunMin,inpRunMax);
  HERE("load ok=",ok);
  if (ic.size()==0) {
    std::cout << "no runs to work with\n";
    ok=0;
  }

   std::cout<<" Load from cards: "<<ic.fIPhi.size()<<" "<<ic.fIPhi[0]<<" "<<ic.fIEta[0]<<std::endl;

  if (user_iphi.size()) { // hack the object
    ic.edit_iphi() = user_iphi;
  }
  if (user_ieta.size()) { // hack the object
    ic.edit_ieta() = user_ieta;
  }

   std::cout<<" 0 Load from cards: "<<ic.fIPhi.size()<<" "<<ic.fIPhi[0]<<" "<<ic.fIEta[0]<<std::endl;

  if (user_calibration!="undef") ic.calibType(user_calibration);
  if (user_depth!=-1) ic.depth(user_depth);
  if (ok && (user_detector.size()>0)) ok=ic.detector(user_detector); // may change the depth
  if (!ok) gSystem->Exit(1);

   std::cout<<" 1 Load from cards: "<<ic.fIPhi.size()<<" "<<ic.fIPhi[0]<<" "<<ic.fIEta[0]<<std::endl;

  // Container for the avg-phi canvases
  std::vector<TCanvas*> cV_avgPhi;
  HistoDef_t *global_hd=NULL;

  // loop over the subdetectors
  for (TSubDet_t sd=_sdHB1; sd<_sdLast; next(sd)) {
    if (user_depth!=-1) {
      std::cout << "user_depth=" << user_depth << " is fixed. Probing...";
      if (user_depth!=GetSubDetDepth(sd)) {
	std::cout << " skip " << GetSubDetFullName(sd) << "\n";
	continue;
      }
    }
    if (user_detector.size()>0) {
      std::cout << "user_detector=<" << user_detector << "> is fixed. Probing...";
      if (user_detector!=GetSubDetName(sd)) {
	std::cout << " skip " << GetSubDetFullName(sd) << "\n";
	continue;
      }
    }

    std::cout << "Working with " << GetSubDetFullName(sd) << "\n";
    ic.depth(GetSubDetDepth(sd));
    ic.detector(GetSubDetName(sd).Data());

   std::cout<<" 2 Load from cards: "<<ic.fIPhi.size()<<" "<<ic.fIPhi[0]<<" "<<ic.fIEta[0]<<std::endl;

    TMethod_t methodStart=(user_method==_calcNone) ? _calcAmp : user_method;
    TMethod_t methodLast =(user_method==_calcNone) ? _calcRatio : user_method;
    std::cout << "methodStart=" << GetMethodName(methodStart)
	      << ", methodLast=" << GetMethodName(methodLast) << "\n";
    for (TMethod_t method= methodStart; method<=methodLast; next(method)) {
      std::cout << "work with method=" << GetMethodName(method) << "\n";

   std::cout<<" 2.1 Load from cards: "<<ic.fIPhi.size()<<" "<<ic.fIPhi[0]<<" "<<ic.fIEta[0]<<std::endl;

      HistoDef_t hd(ic,method);
      //std::cout << "hd(0) " << hd;
      TSpecMethod_t specMethodStart=
	(user_specMethod==_specUnknown) ? _specAvgPhi : user_specMethod;
      TSpecMethod_t specMethodLast =
	(user_specMethod==_specUnknown) ? _specAvgPhi : user_specMethod;
      std::cout << "specMethodStart=" << GetSpecMethodName(specMethodStart)
		<< ", specMethodLast=" << GetSpecMethodName(specMethodLast)
		<< "\n";
      std::vector<int> storeIPhiVec= ic.iphi();

   std::cout<<" 2.2 Load from cards: "<<ic.fIPhi.size()<<" "<<ic.fIPhi[0]<<" "<<ic.fIEta[0]<<std::endl;

      TString storeOutDir= plotOpt.outDirName();
      TString storeRootFileName= rootFileName;
      // loop over special methods
      for (TSpecMethod_t specMethod= specMethodStart;
	   specMethod<=specMethodLast; next(specMethod)) {

   std::cout<<" 2.3 Load from cards: "<<ic.fIPhi.size()<<" "<<ic.fIPhi[0]<<" "<<ic.fIEta[0]<<std::endl;


	if (!hd.setSpecMethod(specMethod,ic)) {
	  std::cout << "failed to prepare spec method\n";
	  gSystem->Exit(1);
	}

   std::cout<<" 2.4 Load from cards: "<<ic.fIPhi.size()<<" "<<ic.fIPhi[0]<<" "<<ic.fIEta[0]<<std::endl;

	if (global_hd!=NULL) delete global_hd;
	global_hd= new HistoDef_t(hd,"global_");
	//std::cout << "hd(1) " << hd;
   std::cout<<" 2.5 Load from cards: "<<ic.fIPhi.size()<<" "<<ic.fIPhi[0]<<" "<<ic.fIEta[0]<<std::endl;
	// Construct output directory
	TString extraTag=TString("-") +
	                 GetSubDetFullName(sd) + TString("-")
	                 + hd.getMethodName();
	TString outDir=plotOpt.outDirName() + TString("/dir") + extraTag;

   std::cout<<" 2.6 Load from cards: "<<ic.fIPhi.size()<<" "<<ic.fIPhi[0]<<" "<<ic.fIEta[0]<<std::endl;

	plotOpt.outDirName(outDir);

   std::cout<<" 2.7 Load from cards: "<<ic.fIPhi.size()<<" "<<ic.fIPhi[0]<<" "<<ic.fIEta[0]<<std::endl;

	rootFileName.ReplaceAll(".root",extraTag+TString(".root"));

   std::cout<<" 2.8 Load from cards: "<<ic.fIPhi.size()<<" "<<ic.fIPhi[0]<<" "<<ic.fIEta[0]<<std::endl;

	std::cout << "outDir=" << outDir << ", rootFileName=<"
		  << rootFileName << ">\n";

	HERE("try to make the plot");

   std::cout<<" 3 Load from cards: "<<ic.fIPhi.size()<<" "<<ic.fIPhi[0]<<" "<<ic.fIEta[0]<<std::endl;

	std::vector<TCanvas*>* cV= MakeTimeDepPlot(hd, ic, plotOpt);
	if (!cV) std::cout << "Failed to produce time-dependency plots\n";
	else {
	  std::cout << "MakeTimeDepPlot OK\n";
	}

	// if needed save the canvases
	if (cV && (rootFileName.Length()>0)) {
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

	// create index pages for each case
	if (cV && (user_saveAll==1)) {
	  int res= createWebPage(*cV,hd,ic,plotOpt);
	  if (!res) {
	    std::cout << "failed to created the webPage\n";
	    gSystem->Exit(1);
	  }
	}

	// collect only interesting canvases
	if (cV && (specMethod==_specAvgPhi)) {
	  for (unsigned int i=0; i<cV->size(); i++) {
	    TString chkName= cV->at(i)->GetName();
	    if (chkName.Index("iphi-1111")!=-1) {
	      cV_avgPhi.push_back(cV->at(i));
	    }
	    else {
	      delete cV->at(i);
	    }
	  }
	}

	// restore initial values
	ic.edit_iphi()= storeIPhiVec;
	plotOpt.outDirName(storeOutDir);
	rootFileName= storeRootFileName;
      } // specMethod
    } // method
  } // subdet

  if (cV_avgPhi.size()!=0) {
    if (!global_hd) {
      std::cout << "would save cV_avgPhi, but global_hd is null\n";
    }
    else {
      int res= createWebPage(cV_avgPhi,*global_hd,ic,plotOpt);
      if (!res) {
	std::cout << "failed to created the webPage\n";
	gSystem->Exit(1);
      }
    }
  }

  std::cout << "exiting\n";
  gSystem->Exit(0);

}
//______________________________________________________________________________
