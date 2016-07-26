

#include "ObjClass.h"
#include "HistoDef.h"

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
    std::cout << "use: ./zamt_hf3.exe inputCardsFile [method] [-iphi-VALUE] [-calib-VALUE] [-run-range runNumMin runNumMax] [-run-period runDateMin runDateMax]\n";
    std::cout << "  VALUE for calib LED (default), LASER or PEDESTAL\n";
    std::cout << "  VALUE for iphi = iphiMin .. iphiMax (TBD)\n";
    std::cout << "  date format year/month/day\n";
    ListMethods(1);
    gSystem->Exit(1);
  }
  std::string fname=argv[1];
  TMethod_t method=_calcAmp;
  if (argc>=3) {
    method=GetMethod(argv[2]);
    if (method==_calcUnknown) {
      std::cout << "error in the method name\n";
      gSystem->Exit(1);
    }
  }

  std::string calibration="undef";
  int user_iphi=-1;
  InputRuns_t *inpRunMin=NULL, *inpRunMax=NULL;
  for (int ia=2; ia<argc; ia++) {
    std::string s(argv[ia]);
    if (s.find("-calib-")!=std::string::npos) {
      calibration=s.substr(7,s.size());
      std::cout << " ** calibration=" << calibration << "\n";
    }
    else if (s.find("-iphi-")!=std::string::npos) {
      user_iphi=atoi(s.c_str()+6);
      std::cout << " ** user_iphi=" << user_iphi << "\n";
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
  }

  InputCards_t ic;
  int ok=ic.load(fname,inpRunMin,inpRunMax);
  HERE("load ok=",ok);
  if (user_iphi!=-1) { // hack the object
    ic.edit_iphi().clear();
    ic.edit_iphi().push_back(user_iphi);
  }
  if (calibration!="undef") ic.calibType(calibration);

  if (!ok) gSystem->Exit(1);

  HERE("try to make the plot");

  HistoDef_t hd(ic.calibType(),method,ic.depth());

  TCanvas *c= MakePlotHD(hd, ic);
  HERE("makePlot ok");
  gSystem->Exit(0);

}
//______________________________________________________________________________
