#include "ShowTimeDep.h"
#include <TGraphErrors.h>
#include "helpWebPage.h"


// -------------------------------------------------------------

std::vector<TCanvas*>* MakeTimeDepPlot(HistoDef_t inpHD, const InputCards_t &ic,
				       const PlotOptions_t &opt)
{
  int tolerateFileFailure=0;

  if (ic.runData().size()==0) {
    std::cout << "MakeTimeDepPlot: nothing to display\n";
    return NULL;
  }
  if (std::string(inpHD.calibType().Data())!=ic.calibType()) {
    std::cout << "MakeTimeDepPlot detected error in calibType mismatch:\n"
	      << "  HistoDef calibType=" << inpHD.calibType() << "\n"
	      << "  InputCards calibType=" << ic.calibType() << "\n";
    return NULL;
  }

  TDatime storeTimeOffset(gStyle->GetTimeOffset());
  const Double_t startTime=Double_t(time0.Convert());
  gStyle->SetTimeOffset(startTime);

  Float_t *timeArr=NULL;
  std::vector<std::string> filesv;

  if (!ic.prepareRootFileNamesAndTimer(filesv,&timeArr)) {
    std::cout << "preparation failed\n";
    gStyle->SetTimeOffset(storeTimeOffset.Convert());
  }

  // announce the period
  {
    std::string timeMinStr= ic.firstRunData().sqlTimeStr();
    std::string timeMaxStr= ic.lastRunData().sqlTimeStr();
    std::cout << "Period: " << timeMinStr << " -- " << timeMaxStr
	      << " (" << filesv.size() << " snapshots)\n";
    std::cout << "Run range: " << ic.runData()[0].runNo() << " -- " << ic.runData().back().runNo() << "\n";
    std::cout << std::endl;
  }


  // Load histograms. The loaded histograms will depend on the method
  // which is given in inpHD
  std::vector<TH1F*> h1fV;
  std::vector<TH2F*> h2fV;
  std::vector<TH2F*> h2f2V;
  std::vector<TH2F*> h2fEffV;

  std::cout << "loading histograms for the method="
	    << GetMethodName(inpHD.method()) << "; full method name="
	    << inpHD.getMethodName() << "\n";

  for (unsigned int i=0; i<filesv.size(); ++i) {
    HERE("opening file",filesv[i].c_str());
    TFile f(filesv[i].c_str(), "READ");
    if (!f.IsOpen()) {
      std::cout << "failed to open the file <" << filesv[i] << ">" << std::endl;
    }
    else {
      HERE("loading histos from",filesv[i].c_str());
      if (!inpHD.loadHisto(f,Form("_file%d",i))) {
	std::cout << "error loading from file <" << filesv[i] << ">\n";
	if (!tolerateFileFailure) { return NULL; }
	std::cout << " ... tolerating this one\n";
	std::cout << "   ... oops not implemented yet! (timeArr is not adapted)\n";
	return NULL;
      }
      else {
	HERE("loaded ok");
	//if (!inpHD.getHisto()) std::cout << "getHisto is null\n";
	//if (!inpHD.getHisto2F()) std::cout << "getHisto2F is null\n";
	//if (!inpHD.getHisto2F_2()) std::cout << "getHisto2F_2 is null\n";
	//if (!inpHD.getHisto2F_ratio()) std::cout << "getHisto2F_ratio is null\n";
	h1fV.push_back(inpHD.getHisto());
	h2fV.push_back(inpHD.getHisto2F());
	h2f2V.push_back(inpHD.getHisto2F_2());
	h2fEffV.push_back(inpHD.getHisto2F_ratio());

	//printHisto("inpHD.getHisto2F",inpHD.getHisto2F());
	//printHisto("inpHD.getHisto2F_2",inpHD.getHisto2F_2());
	//printHisto("inpHD.getHisto2F_ratio",inpHD.getHisto2F_ratio());
      }
    }
    //delete f; // close the file
    f.Close();
  }
  HERE("histograms loaded");

  unsigned int numRuns= h1fV.size();
  if (numRuns==0) {
    std::cout << "MakeTimeDepPlot: failed to load any good files\n";
    return NULL;
  }

  Float_t timeMin= timeArr[0];
  Float_t timeMax= timeArr[filesv.size()-1];

  std::vector<TCanvas*>* canvV= new std::vector<TCanvas*>();
  std::vector<std::vector<TGraphErrors*>*> grVV;

  TCanvas *c1=NULL;
  std::cout << "numRuns=" << numRuns << "\n";
  Float_t *avgW=new Float_t[numRuns];// width averaged
  Float_t *ratio=new Float_t[numRuns];// ratio
  Float_t *avgWErr=new Float_t[numRuns];// width averaged uncertainty
  Float_t *ratioErr=new Float_t[numRuns];// ratio uncertainty (only scaled)

  // no time uncertainty
  Float_t *timeArrErr= new Float_t[numRuns];
  for (int i=0; i<numRuns; i++) timeArrErr[i]=0;

  if (!avgW || !ratio || !avgWErr || !ratioErr) {
    std::cout << "MakeTimeDepPlot: failed to allocate the arrays\n";
    return NULL;
  }

  // Make plots in profiles:
  //  -- iProfile=0: fixed iphi
  //                 it is possible if the plots are not TH1F (fixed eta)
  //  -- iProfile=1: fixed ieta


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (opt.fSaveEveryPlot==1) {
    gSystem->mkdir(opt.fOutDirName,kTRUE); // kTRUE-recursive
  }


  std::cout << "Loop over profiles\n";
  for (int iProfile=0; iProfile<2; iProfile++) {
    if ((iProfile==0) && ((inpHD.histoDim()==_histo1F) ||
			  (inpHD.histoDim()==_histo1D))) {
      std::cout << " iProfile (fix IPhi) is not possible for histo1F/histo1D."
		<< " Continuing\n";
      continue;
    }

    TString outFile, outLeg;
    // TargetIEta loop is needed to be able to construct profile
    // with fixed eta. It is active only in iProfile=1, and short-cuts the
    // loop over eta
    unsigned int targetIEtaMin= ic.ieta().size() + 1;
    unsigned int targetIEtaMax= ic.ieta().size() + 2;
    if (iProfile==1) { targetIEtaMin=0; targetIEtaMax=ic.ieta().size(); }
    std::vector<TGraphErrors*> *grV=NULL;

    for (unsigned int targetIEta=targetIEtaMin; targetIEta<targetIEtaMax;
	 targetIEta++) {
      for (unsigned int idxIPhi=0; idxIPhi<ic.iphi().size(); idxIPhi++) {

	const char* fixedVarName= (iProfile==0) ? "iphi" : "ieta";
	int fixedVarVal =
	  (iProfile==0) ? ic.iphi(idxIPhi) : ic.ieta(targetIEta);
	const int iPhi=ic.iphi(idxIPhi);

	std::cout << "\ncycle. iProfile=" << iProfile << ", targetIEta="
		  << targetIEta << "(" << ic.ieta(targetIEta)
		  << "), idxIPhi=" << idxIPhi << "(" << ic.iphi(idxIPhi)<<")\n";

	// Prepare variables
	if ((iProfile==0) ||
	    ((iProfile==1) && (idxIPhi==0))) {

	  grV=new std::vector<TGraphErrors*>();
	  grVV.push_back(grV);

	  outFile=Form("%s/fig_%s_outDepth%d_%s%d_%s.%s",
		       opt.fOutDirName.Data(),
		       //GetMethodName(inpHD.method(),1).Data(),
		       inpHD.getMethodName().Data(),
		       ic.depth(),
		       fixedVarName,
		       fixedVarVal,
		       ic.detector().c_str(),
		       opt.fOutFigFormat.Data());
	  std::cout << " -- setup outFile=<" << outFile << ">\n";
	  outLeg=Form("HF depth %d, %s=%d",ic.depth(),fixedVarName,fixedVarVal);
	}


	// Accumulate data
	for (unsigned int idxIEta=0; idxIEta<ic.ieta().size(); idxIEta++) {
	  // allow only 1 value if the profile goes over phi
	  if ((iProfile==1) && (idxIEta!=targetIEta)) continue;

	  const int iEta=ic.ieta(idxIEta);

	  // clear the arrays
	  for (unsigned int irun=0; irun<numRuns; irun++) {
	    avgW[irun]=0;
	    avgWErr[irun]=0;
	    ratio[irun]=0;
	    ratioErr[irun]=0;
	  }

	  // for loop over time points)
	  for (unsigned int irun=0; irun<numRuns; irun++) {

	    std::cout << "profile: " << iProfile
		      << Form(", idxIPhi=%d(iPhiBin=%d)",idxIPhi,iPhi)
		      << Form(", idxIEta=%d(iEtaBin=%d)",idxIEta,iEta)
		      << ", irun=" << irun
		      << "\n";
	    std::cout << "inpHD.useMethod=" << GetMethodName(inpHD.useMethod()) << "\n";

	    switch(inpHD.useMethod()) {
	    case _calcAvgAmp:
	    case _calcAvgTSn:
	    case _calcAvgTSx:
	    case _calcAvgWidth:
	    case _calcAvgRatio:
	    case _calcAvgP0: case _calcAvgP1:
	    case _calcAvgP2: case _calcAvgP3:
	    case _calcAvgPW0: case _calcAvgPW1:
	    case _calcAvgPW2: case _calcAvgPW3:
	      std::cout << "get mean of " << h1fV[irun]->GetName() << "\n";
	      avgW[irun] = h1fV[irun]->GetMean();
	      avgWErr[irun] = h1fV[irun]->GetMeanError();
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
	      int oldCode=0;
	      if (oldCode) {
		TH2F *twod1 = h2fV[irun];
		TH2F *twod0 = h2f2V[irun];
		if (!twod1 || !twod0) {
		  if (!twod1) std::cout << "twod1 is null\n";
		  if (!twod0) std::cout << "twod0 is null\n";
		  std::cout << "error detected" << std::endl;
		  return NULL;
		}
		TH2F *Ceff= (TH2F*)twod1->Clone("Ceff");
		if (!Ceff) {
		  std::cout << "failed to clone " << twod1->GetName() << "\n";
		  return NULL;
		}
		//std::cout<<" Bin content "<<twod1->GetBinContent(28,1)<<std::endl;
		std::cout << "divide " << twod1->GetName() << " by " << twod0->GetName() << "\n";
		Ceff->Divide(twod1,twod0, 1, 1, "B");
		if (irun==0) {
		  std::cout << "for 1st " << ic.calibType() << " run:"
			    << " nx = " << Ceff->GetXaxis()->GetNbins()
			    << ", ny = " << Ceff->GetYaxis()->GetNbins() << "\n";
		}
		avgW[irun] = Ceff->GetBinContent(iEta,iPhi);
		if(iEta == 2) std::cout<<iPhi<<" RRRR "<<avgW[irun]<<" "<<iEta<<std::endl;
		delete Ceff;
	      } // end of old code
	      else
	      {
		// new code
		//std::cout << "get value for irun=" << irun << " at "
		//	  << Form("(iEta,iPhi)=(%d,%d)",iEta,iPhi) << std::endl;
		const TH2F *Ceff= h2fEffV[irun];
		if (!Ceff) {
		  std::cout << "Ceff is null" << std::endl;
		  return NULL;
		}
		avgW[irun] = Ceff->GetBinContent(iEta,iPhi);
		avgWErr[irun] = Ceff->GetBinError(iEta,iPhi);
		std::cout << "irun=" << irun << " value=" << avgW[irun] << " +- " << avgWErr[irun] << "\n";
		if(iEta == 2) std::cout<<iPhi<<" RRRR "<<avgW[irun]<<" "<<iEta<<std::endl;
	      }
	    } break;
	    case _calcSpec:
	      std::cout << "get spec value from " << h1fV[irun]->GetName() << "\n";
	      avgW[irun] = h1fV[irun]->GetBinContent(41+iEta);
	      avgWErr[irun] = h1fV[irun]->GetBinError(41+iEta);
               std::cout<<" CalcSpec "<<irun<<" "<<41+iEta<<" "<<h1fV[irun]->GetBinContent(41+iEta)<<
                                                           " "<<h1fV[irun]->GetBinContent(27)<<std::endl;

	      break;
	    default:
	      std::cout << "not ready for this method\n";
	      return NULL;
	    }
	  } // loop over periods (irun)

	  // normalization
	  Float_t norm=avgW[0];
	  if (1) {
	    // normalize to the first non-zero point
	    if (norm<=Float_t(0)) {
	      for (unsigned int ir=0; ir<numRuns; ir++) {
		if (avgW[ir]>Float_t(0)) {
		  norm=avgW[ir];
		  break;
		}
	      }
	    }
	  }
	  for(unsigned int ir = 0; ir<numRuns; ir++) {
	    if (norm<=0.) {
	      ratio[ir] = 0;
	      ratioErr[ir] = 0;
	    }
	    else {
	      ratio[ir] = avgW[ir]/norm;
	      ratioErr[ir] = avgWErr[ir]/norm;
	    }
	    std::cout<<"Point2:ir="<<ir<<"  avgW[ir]="<<avgW[ir]<<" avgW[0]="<<avgW[0]<<" ratio[ir]="<<ratio[ir]<<std::endl;
	  }//


	  if (opt.fSuppressErrorInPlots) {
	    for (unsigned int ir=0; ir<numRuns; ir++) ratioErr[ir]=0;
	  }

	  TGraphErrors *gr = new TGraphErrors(numRuns,timeArr,ratio,
					      timeArrErr,ratioErr);
	  grV->push_back(gr);
	  //gPad->SetGridy();
	  //gr->Draw("PLE1");

	} // idxIEta


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// Prepare and finalize the plot
	if ((iProfile==0) ||
	    ((iProfile==1) && (idxIPhi==ic.iphi().size()-1))) {
	  std::cout<<"\nNumber of graphs "<< grV->size() << std::endl;

	  TString canvName=outFile;
	  canvName.ReplaceAll(opt.fOutDirName+TString("/fig_"),"canv_");
	  canvName.ReplaceAll(TString(".") + opt.fOutFigFormat,"");
	  TString canvTitle=canvName;
	  canvTitle.ReplaceAll("canv_","");

	  //__________________________________________________________________
	  // Create a new canvas.
	  c1 = new TCanvas(canvName,canvTitle,10,40,800,600);
	  canvV->push_back(c1);
	  c1->Range(0,0,25,18);
	  c1->SetFillColor(40);
	  TString pad1Name=TString("pad1_") + canvTitle;
	  TPad* pad1 = new TPad(pad1Name,"This is pad1",0.02,0.02,0.98,0.83,33);
	  TLatex *t = new TLatex();
	  t->SetTextFont(32);
	  t->SetTextColor(4);
	  t->SetTextSize(0.03);
	  t->SetTextAlign(12);
	  t->DrawLatex(0.9, 15.5, inpHD.plotTitle());
	  pad1->Draw();
	  pad1->SetGridy();

	  //__________________________________________________________________
	  pad1->cd();
	  pad1->Range(-0.255174,-19.25,2.29657,-6.75);

	  Float_t yMin_hh2=0.8;
	  Float_t yMax_hh2=1.2;

	  if (!opt.fFixedYRange) {
	    yMin_hh2=1e6; yMax_hh2=-1e6;
	    Double_t val;
	    for (unsigned int igr=0; igr<grV->size(); igr++) {
	      const TGraphErrors *gr= grV->at(igr);
	      for (int i=0; i<gr->GetN(); i++) {
		val= gr->GetY()[i] + 1.1*gr->GetEY()[i];
		if (val > yMax_hh2) yMax_hh2=val;
		val= gr->GetY()[i] - 1.1*gr->GetEY()[i];
		if (val < yMin_hh2) yMin_hh2=val;
	      }
	    }
	    if (yMin_hh2==yMax_hh2) yMax_hh2=yMin_hh2+1;
	    std::cout << " setting yRange to "
		      << yMin_hh2 << " .. " << yMax_hh2 << "\n";
	  }

	  TString hh2Name=TString("hh2_") + canvName;
	  TH2F* hh2=new TH2F(hh2Name,"",2,timeMin,timeMax,100,yMin_hh2,yMax_hh2);
	  hh2->SetStats(0);
	  hh2->GetXaxis()->SetTimeDisplay(1);
	  hh2->GetXaxis()->SetLabelSize(0.03);
	  hh2->GetXaxis()->SetTimeFormat("%D");
	  hh2->GetXaxis()->SetTitle("Time");
	  hh2->Draw();

	  //pad1->DrawFrame(t0[0]-10.,   0.8,  t0[6]+10.,   1.1);// 1-A,
	  //pad1->SetTimeDisplay(1);
	  //pad1->SetTimeFormat("%d\/%H:%M");
	  //__________________________________________________________________
	  //__________________________________________________________________

	  pad1->GetFrame()->SetFillColor(19);
	  //__________________________________________________________________

	  // Draw the graphs
	  for(unsigned int m=0; m<grV->size(); m++) {
	    TGraphErrors *gr= grV->at(m);
	    gr->SetMarkerStyle(20);
	    gr->SetMarkerSize(0.9);
	    int icolor=m+1;
	    gr->SetLineColor(icolor);
	    gr->SetMarkerColor(icolor);
	    if (icolor == 10) {
	      gr->SetMarkerStyle(24);
	      gr->SetMarkerColor(kBlack);
	    }
	    gr->Draw("LPE1");
	  }

	  // Display legend
	  TLegend * leg1 = new TLegend(0.20,0.75,0.45,0.95,outLeg);
	  TLegend * leg2 = new TLegend(0.50,0.75,0.75,0.95);
	  int split=6;
	  // -- orphan control
	  if (grV->size()==split+1) split++;

	  for(unsigned int m=0; m<grV->size(); m++) {
	    TString desc="desc"; // dummy assignment
	    if (iProfile==0) desc=Form("ieta = %d",-41+ic.ieta(m));
	    else desc=Form("iphi = %d",ic.iphi(m));
	    TLegend *legLoc= (m<split) ? leg1 : leg2;
	    legLoc->AddEntry(grV->at(m),desc,"p");
	  }
	  leg1->Draw();
	  if (leg2->GetNRows()>0) leg2->Draw();

	  c1->Modified();
	  c1->Update();
	  //
	  if (opt.fSaveEveryPlot==1) {
	    std::cout << "Saving the plot to <" << outFile << ">\n\n";
	    c1->Print(outFile);
	  }
	}

      } // idxIPhi
    } // targetIEta
  } // iProfile

  //_________________________________________________________________________
  //_________________________________________________________________________

  gStyle->SetTimeOffset(storeTimeOffset.Convert());

  return canvV;
}
//___________________________________________________________________________

// --------------------------------------------------------------------------

int createWebPage(std::vector<TCanvas*> &canvasV,
		  const HistoDef_t &hd, const InputCards_t &ic,
		  const PlotOptions_t &opt)
{
  if (canvasV.size()==0) {
    std::cout << "createWebPage canvasV.size=0\n";
    return 0;
  }
  std::vector<TString> infolines;
  TString destDir=TString("webPage_") + opt.fOutDirName;
  int res=CreateWebPage(canvasV,"Time dependency",destDir,infolines,
			"index.html","",1,"300px");
  return res;
}

// --------------------------------------------------------------------------
