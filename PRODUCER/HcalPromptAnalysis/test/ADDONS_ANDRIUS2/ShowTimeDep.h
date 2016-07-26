#ifndef ShowTimeDep_H
#define ShowTimeDep_H

#include "ObjClass.h"
#include "HistoDef.h"


// --------------------------------------------------------

struct PlotOptions_t;

std::vector<TCanvas*>* MakeTimeDepPlot(HistoDef_t hd,
				       const InputCards_t &ic,
				       const PlotOptions_t &opt);

int createWebPage(std::vector<TCanvas*> &canvasV,
		  const HistoDef_t &hd, const InputCards_t &ic,
		  const PlotOptions_t&opt);

// --------------------------------------------------------
// --------------------------------------------------------

struct PlotOptions_t {
  int fFixedYRange;
  int fSuppressErrorInPlots;
  int fSaveEveryPlot;
  TString fOutFigFormat;
  TString fOutDirName;
  TString fLegPlace;   // (not used) Values: default
public:
  PlotOptions_t() :
      fFixedYRange(1),
      fSuppressErrorInPlots(0),
      fSaveEveryPlot(0),  // whether MakeTimeDepPlot should save every plot in destination folder (not webPage)
      fOutFigFormat("gif"),
      fOutDirName("dir-figs"),
      fLegPlace("default")
  {}

  TString outDirName() const { return fOutDirName; }
  void outDirName(TString set_dir) { fOutDirName=set_dir; }
};

// --------------------------------------------------------

#endif
