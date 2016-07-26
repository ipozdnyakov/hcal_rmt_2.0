#ifndef helpWebPage_H
#define helpWebPage_H

#include <TCanvas.h>
#include <TString.h>
#include <vector>
#include <fstream>
#include <iostream>

// -------------------------------------------------------------------------

inline
void HEREmsg(int debug, const char *msg)
{ if (debug) std::cout << msg << std::endl; }

template<class type_t>
inline
void HEREmsg(int debug, const char *format, type_t val)
{ if (debug) std::cout <<Form(format,val) << std::endl; }


// -------------------------------------------------------------------------

inline
int CreateWebPage(std::vector<TCanvas*> &canvasV,
		  TString webPageTitle,
		  TString destDir,
		  const std::vector<TString> &infoLines,
		  TString htmlFileName="index.html",
		  TString multiColumnSubdir="",
		  int saveFigs=1,
		  TString setFigWidth="100%")
{
  int debug=0;

  HEREmsg(debug,"CreateWebPage. Here are %d canvases",int(canvasV.size()));

  int badVector=0;
  for (unsigned int i=0; i<canvasV.size(); ++i) {
    if (canvasV[i]==NULL) {
      std::cout << "canvasV[" << i << "] is null\n";
      badVector=1;
    }
  }
  if (badVector) {
    std::cout << "CreateWebPage: bad pointer in vector of size="
	      << canvasV.size() << " detected\n";
    return 0;
  }

  int flag_hasEffSigma=0;
  for (unsigned int i=0; !flag_hasEffSigma && (i<canvasV.size()); ++i) {
    if (i+1>=canvasV.size()) continue;
    if (TString(canvasV[i]->GetName())+TString("_effSigma") ==
	TString(canvasV[i+1]->GetName())) {
      flag_hasEffSigma=1;
    }
  }

  HEREmsg(debug,"flag_hasEffSigma=%d",flag_hasEffSigma);

  // create directories
  gSystem->mkdir(destDir,kTRUE);

  // open the file
  std::ofstream web_page(destDir+TString("/")+htmlFileName);

  // if asked, save to a specific subdirectory
  if (multiColumnSubdir.Length()) {
    gSystem->mkdir(destDir + TString("/") + multiColumnSubdir,kTRUE);
    destDir.Append(TString("/")+multiColumnSubdir);
  }

  web_page
    <<"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2 Final//EN\">\n"
    <<"<html>\n"
    <<"<head>\n"
    <<"<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\" />\n"
    <<"<title>"<<webPageTitle<<"</title>\n"
    <<"</head>\n";
  //<<"<h1><a href=\"../index.html\">Up</a>&nbsp;&nbsp;"<< webPageTitle <<"</h1>\n" ;


  HEREmsg(debug,Form("print infoLines[%d]",int(infoLines.size())));

  if (infoLines.size()) {
    web_page << "<p>\n";
    for (unsigned int i=0; i<infoLines.size(); ++i) {
      web_page << infoLines[i] << "<br>\n";
    }
    web_page << "</p><br>\n";
  }

  web_page
    <<"<br><table border=\"1\" cellpadding=\"5\" width=\"100%\">"
    <<"<tr valign=\"top\"><td width=\"20%\">\n"
    <<"<h2>Plot name</h2></td>";
  web_page << "<td width=\"30%\"><h2>Plot</h2></td>";
  if (flag_hasEffSigma) {
    web_page <<"<td width=\"30%\"><h2>Eff.&sigma;</h2></td>";
  }
  web_page << "</tr>\n";

  HEREmsg(debug,"save canvases");

  int verbose= (canvasV.size() > 30) ? 1:0;
  for (unsigned int i=0; i<canvasV.size(); ++i) {
    int hasEffSigma=0;
    if (i+1<canvasV.size()) {
      if (TString(canvasV[i]->GetName())+TString("_effSigma") ==
	  TString(canvasV[i+1]->GetName())) {
	hasEffSigma=1;
	std::cout << "hasEffSigma! "
		  << canvasV[i]->GetName() << "  "
		  << canvasV[i+1]->GetName() << "\n";
      }
    }
    TString canvName= canvasV[i]->GetName();
    TString figName= canvName;
    figName.ReplaceAll("canv_","fig_");
    figName.ReplaceAll("file1_","");
    if (multiColumnSubdir.Length()) {
      figName.Prepend(multiColumnSubdir + TString("/"));
    }
    TString figNameEffSigma=figName + TString("_effSigma.png");
    figName.Append(".png");
    TString info= canvName;
    info.ReplaceAll("canv_","");
    web_page << "<tr><td>" << info << "</td>";
    for (int iPlot=0; iPlot < 1+hasEffSigma; ++iPlot) {
      TString plotFName= (iPlot==0) ? figName : figNameEffSigma;
      web_page << "<td><a href=\"" << plotFName << "\">"
	       << "<img border=\"0\" class=\"image\" width=\""
	       << setFigWidth << "\" src=\""
	       << plotFName << "\"></a></td>";
    }
    web_page << "</tr>\n";

    if (saveFigs) {
      if (verbose) {
	std::cout << Form("... saving %d/%d: ",i,int(canvasV.size()))
		  << figName << "\n";
      }
      canvasV[i]->SaveAs(destDir + TString("/") + figName);

      if (hasEffSigma) {
	if (verbose) {
	  std::cout << Form("... saving %d/%d: ",i+1,int(canvasV.size()))
		    << figName << "\n";
	}
	canvasV[i+1]->SaveAs(destDir + TString("/") + figNameEffSigma);
      }
    }
    if (hasEffSigma) i++;
  }

  HEREmsg(debug,"print time stamp");

  // prepare time stamp
  time_t ltime;
  ltime=time(NULL);
  TString str = TString(asctime(localtime(&ltime)));
  if (str[str.Length()-1]=='\n') str.Remove(str.Length()-1,1);


  web_page << "</table><br>\n\n";
  web_page << "Created on " << str << "\n";
  web_page << "</html>\n";
  web_page.close();

  HEREmsg(debug,"file closed");

  return 1;
}

// -------------------------------------------------------------------------

inline
int CreateWebPageMultiColumn(std::vector<std::vector<TCanvas*>*> &canvasV,
			     const std::vector<TString> &labelV_inp,
			     TString webPageTitle,
			     TString destDir,
			     const std::vector<TString> &infoLines,
			     TString htmlFileName="index.html",
			     int minglePlots=10,
			     TString setFigWidth="100%",
			     int cmpFitPlotCorrection=0,
			     int matchAtLeast2=0,
	     const std::vector<std::vector<TString>*>* endNotes=NULL)
{

  unsigned int columnCount=canvasV.size();
  std::vector<TString> labelV(labelV_inp);
  if (canvasV.size()!=labelV.size()) {
    std::cout << "CreateWebPageMultiColumn:  different number of "
	      << "columns and labels\n";
    return 0;
  }
  if (canvasV.size()==0) {
    std::cout << "no canvases supplied\n";
    return 0;
  }
  unsigned int rowCount=canvasV[0]->size();
  {
    TString info="row counts: ";
    int ok=1;
    for (unsigned int iCol=0; iCol<canvasV.size(); iCol++) {
      info.Append(Form(" %d",int(canvasV[iCol]->size())));
      if (canvasV[iCol]->size()!=rowCount) ok=0;
    }
    if (!ok) {
      std::cout << "CreateWebPageMultiColumn:"
		<< " different number of plots supplied\n";
      std::cout << info << "\n";
      if (!minglePlots) return 0;
      else minglePlots=10;
    }
  }

  int flag_hasControlPlots=-1;
  int flag_hasBaseQuantitiesPlots=-1;
  unsigned int idx_controlPlots=0;
  unsigned int idx_baseQuantities=0;

  std::vector<std::vector<int>*> indV;
  indV.reserve(canvasV.size());
  for (unsigned int i=0; i<canvasV.size(); i++) {
    indV.push_back(new std::vector<int>());
  }
  if (minglePlots!=10) {
    for (unsigned int i=0; i<indV.size(); i++) {
      indV[i]->reserve(rowCount);
      for (unsigned int ir=0; ir<rowCount; ir++) {
	indV[i]->push_back(ir);
      }
    }
  }
  else {
    const int debug=0;
    // complicated case
    std::vector<int> idx;
    for(unsigned int ic=0; ic<columnCount; ic++) idx.push_back(0);


    for (int plotType=0; plotType<3; plotType++) {
      if (plotType==1) {
	idx_controlPlots=indV[0]->size();
	if (flag_hasControlPlots!=1) continue;
      }
      else if (plotType==2) {
	idx_baseQuantities=indV[0]->size();
	if (flag_hasBaseQuantitiesPlots!=1) continue;
      }

    // collect indices
    for (unsigned int iColTarget=0; iColTarget+1<canvasV.size(); iColTarget++){
      // check whether all plots are already paired
      int floating=1;
      if (iColTarget>0) {
	floating=0;
	for (unsigned int iCol=iColTarget+1; !floating && (iCol<canvasV.size());
	     ++iCol) {
	  std::vector<int> *currIdx= indV[iCol];
	  for (unsigned int ir=0; ir<canvasV[iCol]->size(); ir++) {
	    if (std::find(currIdx->begin(),currIdx->end(),int(ir))
		== currIdx->end() ) {
	      // plot was not included
	      floating=1;
	      HEREmsg(debug,Form("floating plot iCol=%d, ir=%d",
				 int(iCol),int(ir)));
	    }
	  }
	}
      }
      if (!floating) break;
      HEREmsg(debug,Form("\niColTarget=%d\n",int(iColTarget)));

      unsigned int rowCount_local= canvasV[iColTarget]->size();
      for (unsigned int ir=0; ir<rowCount_local; ir++) {
	int haveCount=1;
	int correctionApplied=0;
	TString target= canvasV[iColTarget]->at(ir)->GetName();
	if (plotType==0) {
	  if (target.Index("control")!=-1) {
	    flag_hasControlPlots=1;
	    continue;
	  }
	  else if (target.Index("BQ")!=-1) {
	    flag_hasBaseQuantitiesPlots=1;
	    continue;
	  }
	}
	else if ((plotType==1) && (target.Index("control")==-1)) continue;
	else if ((plotType==2) && (target.Index("BQ")==-1)) continue;
	target.ReplaceAll(Form("file%d_",iColTarget+1),"");
	target.ReplaceAll("cv_","");
	if ((target[0]=='c') && (target[1]=='_')) target.Remove(0,2);
	else if (cmpFitPlotCorrection && (target[0]=='c')) {
	  target.Remove(0,1);
	  correctionApplied=1;
	}
	if (debug) std::cout << "\n" << std::string(50,'-') << "\n";
	HEREmsg(debug,Form("ir=%d, target=<%s>",int(ir),target.Data()));
	// check that the plot is not included
	if (iColTarget>0) {
	  std::vector<int> *currIdx= indV[iColTarget];
	  if (std::find(currIdx->begin(),currIdx->end(),int(ir))
	      != currIdx->end()) {
	    // plot was included
	    HEREmsg(debug,"... plot was included");
	    continue;
	  }
	  HEREmsg(debug,"loop ended");
	}

	HEREmsg(debug,"finalize idx for iColTarget=%d",iColTarget);

	for (unsigned int ii=0; ii<iColTarget; ii++) idx[ii]=-1;
	idx[iColTarget]=int(ir);

	HEREmsg(debug,"work with selected idx");
	for (unsigned int iCol=iColTarget+1; (iCol<canvasV.size()); iCol++) {
	  const std::vector<TCanvas*> *cV= canvasV[iCol];
	  int found=0;
	  for (unsigned int ii=0; !found && (ii<cV->size()); ii++) {
	    TString probe= cV->at(ii)->GetName();
	    probe.ReplaceAll("canv_","");
	    if (probe.Index("file")!=-1) {
	      for (int iFile=1; iFile<int(canvasV.size()+1); iFile++) {
		probe.ReplaceAll(Form("file%d_",iFile),"");
	      }
	    }
	    probe.ReplaceAll("cv_","");
	    probe.ReplaceAll("h1D_","");
	    HEREmsg(debug,Form("- iCol=%d, ii=%d, raw=<%s>, probe<%s>",
		       int(iCol),int(ii),cV->at(ii)->GetName(),probe.Data()));
	    if ((probe[0]=='c') && (probe[1]=='_')) probe.Remove(0,2);
	    else {
	      HEREmsg(debug,Form("-- %d %d %c",cmpFitPlotCorrection,
				 correctionApplied,probe[0]));
	      if (cmpFitPlotCorrection && correctionApplied &&
		  ((probe[0]=='c') || (probe[0]=='h'))) {
		int rmCnt=((probe[0]=='h') && (probe[1]=='1')) ? 2:1;
		probe.Remove(0,rmCnt);
	      }
	    }
	    if (target == probe) {
	      found=1;
	      idx[iCol]=ii;
	    }
	    HEREmsg(debug,Form("iCol=%d, ii=%d, probe=<%s>, raw=<%s>, found=%d",
			       int(iCol),int(ii),probe.Data(),
			       cV->at(ii)->GetName(),found));
	  }
	  haveCount+=found;
	  if (!found) idx[iCol]=-1;
	  else HEREmsg(debug,"found!");
	}
	const int allowNoPlotCount=(matchAtLeast2) ? (columnCount-2) : 1;
	if (haveCount+allowNoPlotCount>=int(columnCount)) {
	  HEREmsg(debug,Form("haveCount=%d, missing=%d",
			     haveCount,columnCount-haveCount));
	  for (unsigned int iCol=0; iCol<idx.size(); ++iCol) {
	    indV[iCol]->push_back(idx[iCol]);
	  }
	}
      }
    }
    // add the missing plots
    for (unsigned int iCol=0; iCol<canvasV.size(); ++iCol) {
      std::vector<int> *currIdx= indV[iCol];
      for (unsigned int ir=0; ir<canvasV[iCol]->size(); ir++) {
	if (std::find(currIdx->begin(),currIdx->end(),int(ir))
	    == currIdx->end() ) {
	  TString cName=canvasV[iCol]->at(ir)->GetName();
	  if (plotType==0) {
	    if ((cName.Index("control")!=-1) ||
		(cName.Index("BQ")!=-1)) continue;
	  }
	  else if ((plotType==1) && (cName.Index("control")==-1)) continue;
	  else if ((plotType==2) && (cName.Index("BQ")==-1)) continue;
	  // plot was not included
	  HEREmsg(debug,Form("missing plot iCol=%d, ir=%d",int(iCol),int(ir)));
	  for (unsigned int ic=0; ic<indV.size(); ++ic) {
	    indV[ic]->push_back( (ic==iCol) ? ir : -1 );
	  }
	}
      }
    }
    } // plotType

    HEREmsg(debug,"final check for the missing plots");
  // final check for the missing plots
  int hasMissingPlots=0;
  for (unsigned int iCol=0; iCol<canvasV.size(); ++iCol) {
    std::vector<int> *currIdx= indV[iCol];
    for (unsigned int ir=0; ir<canvasV[iCol]->size(); ir++) {
      if (std::find(currIdx->begin(),currIdx->end(),int(ir))
	  == currIdx->end() ) {
	hasMissingPlots=1;
      }
    }
  }
  if (hasMissingPlots) {
    std::cout << "\n\nMissing plots detected after all steps. Check the code\n";
    return 0;
  }
  }

  rowCount= indV[0]->size();

  std::cout << "flags: ControlPlots " << flag_hasControlPlots << " (idx="
	    << idx_controlPlots << "), BQ "
	    << flag_hasBaseQuantitiesPlots << " (idx="
	    << idx_baseQuantities << ")\n";

  if (1) {
    for (unsigned int ir=0; ir<rowCount; ir++) {
      std::cout << "ir= " << ir << "  ";
      for (unsigned int ic=0; ic<indV.size(); ++ic) {
	std::cout << indV[ic]->at(ir) << " ";
      }
      if (indV[0]->at(ir)!=-1) {
	std::cout << canvasV[0]->at(indV[0]->at(ir))->GetName();
      }
      std::cout << "\n";
    }
  }

  gSystem->mkdir(destDir,kTRUE);
  std::ofstream web_page(destDir+TString("/")+htmlFileName);

  web_page
    <<"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2 Final//EN\">\n"
    <<"<html>\n"
    <<"<head>\n"
    <<"<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\" />\n"
    <<"<title>"<<webPageTitle<<"</title>\n"
    <<"</head>\n";
  //<<"<h1><a href=\"../index.html\">Up</a>&nbsp;&nbsp;"<< webPageTitle <<"</h1>\n" ;

  if (infoLines.size()) {
    web_page << "<p>\n";
    for (unsigned int i=0; i<infoLines.size(); ++i) {
      web_page << infoLines[i] << "<br>\n";
    }
    web_page << "</p><br>\n";
  }

  // create directories for plots
  for (unsigned int i=0; i<labelV.size(); ++i) {
    TString dirName= destDir + TString("/") + labelV[i];
    dirName.ReplaceAll("#","");
    gSystem->mkdir(dirName,true);
  }

  // to prevent canvases from splashing on the screen
  // switch to batch mode
  bool isBatch = gROOT->IsBatch();
  gROOT->SetBatch(true);


  for (int plotType=0; plotType<3; plotType++) {

    unsigned int iRowMin=0;
    unsigned int iRowMax=rowCount;
    if (plotType==0) {
      if (flag_hasControlPlots==1) iRowMax=idx_controlPlots;
      else if (flag_hasBaseQuantitiesPlots==1) iRowMax=idx_baseQuantities;
    }

    if (plotType!=0) {
      //if ((plotType==1) && (flag_hasControlPlots!=1)) continue;
      //if ((plotType==2) && (flag_hasBaseQuantitiesPlots!=1)) continue;

      if (plotType==1) {
	if (flag_hasControlPlots!=1) continue;
	iRowMin= idx_controlPlots;
	if (flag_hasBaseQuantitiesPlots==1) iRowMax= idx_baseQuantities;
	web_page << "<br><b><h2>Control plots</h2></b><br>\n";
      }
      else if (plotType==2) {
	if (flag_hasBaseQuantitiesPlots!=1) continue;
	iRowMin=idx_baseQuantities;
	web_page << "<br><b><h2>Base quantities</h2></b><br>\n";
      }
    }

  web_page
    <<"<br><table border=\"1\" cellpadding=\"5\" width=\"100%\">"
    <<"<tr valign=\"top\"><td width=\"10%\">\n"
    <<"<h2>Plot name</h2></td>";
  for (unsigned int iCol=0; iCol<columnCount; ++iCol) {
    web_page << "<td width=\"20%\" align=\"center\"><h2>"
	     << labelV[iCol] << "</h2></td>";
    labelV[iCol].ReplaceAll(" ","_");
  }
  web_page << "</tr>\n";


  // create the table and the respective plots
  const int debugTbl=0;
  if (debugTbl) std::cout << "\nplotType=" << plotType << " : "
			  << iRowMin << ".." << iRowMax << "\n";
  for (unsigned int iRow=iRowMin; iRow<iRowMax; ++iRow) {
    web_page << "<tr>";
    if (debugTbl) {
      std::cout << "iRow=" << iRow << ". Indices: ";
      for (unsigned int i=0; i<canvasV.size(); i++) {
	std::cout << " " << indV[i]->at(iRow);
      }
      std::cout << "\n";
    }
    for (unsigned int i=0; i<canvasV.size(); ++i) {
      int verbose=((i==0) && (rowCount>20)) ? 1:0;
      int canvIdx= indV[i]->at(iRow);
      TCanvas *canv= (canvIdx==-1) ? NULL : canvasV[i]->at(canvIdx);
      TString canvName= (canv) ? canv->GetName() : "";
      TString figName= canvName;
      figName.ReplaceAll("file1_","");
      figName.ReplaceAll("canv_","fig_");
      figName.Prepend(labelV[i]+TString("/"));
      figName.ReplaceAll("#","");
      figName.Append(".png");
      if (i==0) {
	TString info= canvName;
	if (canvIdx==-1) {
	  for (unsigned int ic=1; ic<canvasV.size(); ++ic) {
	    if (indV[ic]->at(iRow)!=-1) {
	      info= canvasV[ic]->at(indV[ic]->at(iRow))->GetName();
	    }
	  }
	}
	info.ReplaceAll("canv_","");
	info.ReplaceAll("file1_","");
	web_page << "<td>" << info << "</td>";
      }
      if (canvIdx==-1) {
	web_page << "<td></td>";
      }
      else {
	web_page << "<td><a href=\"" << figName << "\">"
		 << "<img border=\"0\" class=\"image\" width=\""
		 << setFigWidth << "\" src=\""
		 << figName << "\"></a></td>";
	if (verbose) std::cout << " ... saving " << iRow << "/" << rowCount
			       << ": " << figName << "\n";
	const int save=1;
	if (save) {
	  canv->Draw();
	  canv->SaveAs(destDir + TString("/") + figName);
	}
      }
    }
    web_page << "</tr>\n";
  }

  web_page << "</table><br>\n\n";
  }

  // restore batch mode
  gROOT->SetBatch(isBatch);

  // prepare time stamp
  time_t ltime;
  ltime=time(NULL);
  TString str = TString(asctime(localtime(&ltime)));
  if (str[str.Length()-1]=='\n') str.Remove(str.Length()-1,1);

  if (endNotes) {
    web_page << "<br><hr><br><b>End Notes</b><br>\n";
    for (unsigned int i=0; i<endNotes->size(); i++) {
      web_page << "<br><br>\n";
      if (endNotes->size() == labelV_inp.size()) {
	web_page << "Associated to '" << labelV_inp[i] << "':<br>\n";
      }
      const std::vector<TString> *notes= endNotes->at(i);
      for (unsigned int iNote=0; iNote<notes->size(); ++iNote) {
	web_page << notes->at(iNote) << "<br>\n";
      }
    }
    web_page << "<hr><br>\n";
  }

  web_page << "Created on " << str << "\n";
  web_page << "</html>\n";
  web_page.close();
  return 1;
}

// -------------------------------------------------------------------------

#endif
