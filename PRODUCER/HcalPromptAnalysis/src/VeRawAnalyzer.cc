// -*- C++ -*-
//
// Package:    VeRawAnalyzer
// Class:      VeRawAnalyzer
// 
/**\class VeRawAnalyzer VeRawAnalyzer.cc Hcal/VeRawAnalyzer/src/VeRawAnalyzer.cc
   
Description: <one line class summary>

Implementation:          
<Notes on implementation>
*/
//
// $Id: VeRawAnalyzer.cc,v 1.10 2013/04/28 19:01:16 dtlisov Exp $ 
// 

// system include files
#include <fstream> 
#include <iostream>
#include <cmath>
#include <iosfwd>
#include <bitset>
#include <memory>

//#include "TFile.h"
//#include "TH1F.h"
//#include "TH2F.h"
//#include "TTree.h"

using namespace std;
// user include files   

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

using namespace edm;
// this is to retrieve HCAL digi's  

#include "DataFormats/HcalDetId/interface/HcalElectronicsId.h"
#include "DataFormats/HcalDetId/interface/HcalGenericDetId.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalDigi/interface/HcalQIESample.h"
#include "DataFormats/HcalDetId/interface/HcalCalibDetId.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"

#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalDigi/interface/HBHEDataFrame.h"
#include "DataFormats/HcalDigi/interface/HFDataFrame.h"
#include "DataFormats/HcalDigi/interface/HODataFrame.h"
#include "DataFormats/HcalDigi/interface/HcalCalibrationEventTypes.h"

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h" 

//to gnerate logical and electrical mapping
#include "CalibCalorimetry/HcalAlgos/interface/HcalLogicalMapGenerator.h"
#include "CondFormats/HcalObjects/interface/HcalLogicalMap.h"
#include "CondFormats/HcalObjects/interface/HcalElectronicsMap.h"

#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h" 
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CondFormats/HcalObjects/interface/HcalQIECoder.h"

#include "EventFilter/HcalRawToDigi/interface/HcalDCCHeader.h" 

#include "CondFormats/DataRecord/interface/L1GtTriggerMaskAlgoTrigRcd.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMaskTechTrigRcd.h"

//to get to BX 
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h" 
#include "DataFormats/FEDRawData/interface/FEDHeader.h"
#include "DataFormats/FEDRawData/interface/FEDTrailer.h"
#include "DataFormats/FEDRawData/interface/FEDNumbering.h"

#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"

#include "Geometry/Records/interface/HcalRecNumberingRecord.h"
#include "Geometry/CaloTopology/interface/HcalTopology.h"
//#include "Geometry/Records/interface/CaloGeometryRecord.h"
//#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
//#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"


// ROOT
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"

// Private include files from Eric
//#include "FedGet.hh" 

static const float adc2fC[128]={-0.5,0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5, 10.5,11.5,12.5,
				13.5,15.,17.,19.,21.,23.,25.,27.,29.5,32.5,35.5,38.5,42.,46.,50.,54.5,59.5,
				64.5,59.5,64.5,69.5,74.5,79.5,84.5,89.5,94.5,99.5,104.5,109.5,114.5,119.5,
				124.5,129.5,137.,147.,157.,167.,177.,187.,197.,209.5,224.5,239.5,254.5,272.,
				292.,312.,334.5,359.5,384.5,359.5,384.5,409.5,434.5,459.5,484.5,509.5,534.5,
				559.5,584.5,609.5,634.5,659.5,684.5,709.5,747.,797.,847.,897.,947.,997.,
				1047.,1109.5,1184.5,1259.5,1334.5,1422.,1522.,1622.,1734.5,1859.5,1984.5,
				1859.5,1984.5,2109.5,2234.5,2359.5,2484.5,2609.5,2734.5,2859.5,2984.5,
				3109.5,3234.5,3359.5,3484.5,3609.5,3797.,4047.,4297.,4547.,4797.,5047.,
				5297.,5609.5,5984.5,6359.5,6734.5,7172.,7672.,8172.,8734.5,9359.5,9984.5};		   		   

class VeRawAnalyzer : public edm::EDAnalyzer 
{
public:
  explicit VeRawAnalyzer(const edm::ParameterSet&);
  ~VeRawAnalyzer();
  virtual void beginJob();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  virtual void beginRun( const edm::Run& r, const edm::EventSetup& iSetup);
  virtual void endRun( const edm::Run& r, const edm::EventSetup& iSetup);
  virtual void fillMAP(); 
private:
edm::EDGetTokenT<HcalCalibDigiCollection> tok_calib_;
edm::EDGetTokenT<HBHEDigiCollection> tok_hbhe_;
edm::EDGetTokenT<HODigiCollection> tok_ho_;
edm::EDGetTokenT<HFDigiCollection> tok_hf_;  

  ////////////////////////////////////
  // simple test
  ////////////////////////////////////
  double dR(double eta1, double phi1, double eta2, double phi2);
  double phi12(double phi1, double en1, double phi2, double en2);
  double dPhiWsign(double phi1,double phi2);  
  ////////////////////////////////////
  std::string  fOutputFileName;
  std::string  MAPOutputFileName;
  
  edm::InputTag inputTag_;
  
  edm::ESHandle<HcalDbService> conditions;
  const HcalQIEShape* shape;
  const HcalTopology* topo; 

  /////////////////////////////////////////////
  int verbosity;
  int MAPcreation;

  /////////////////////////////////////////////
  bool recordNtuples_;
  int maxNeventsInNtuple_;
  bool recordHistoes_;
  bool studyRunDependenceHist_;
  bool studyCapIDErrorsHist_;
  bool studyRMSshapeHist_;
  bool studyRatioShapeHist_;
  bool studyTSmaxShapeHist_;
  bool studyTSmeanShapeHist_;
  bool studyDiffAmplHist_;
  bool studyCalibCellsHist_;
  bool studyADCAmplHist_;
  bool studyPedestalsHist_;
  bool studyPedestalCorrelations_;
  bool usePedestalSubtraction_;
  bool useADCmassive_;
  bool useADCfC_;
  bool useADCcounts_;
  bool usecontinuousnumbering_;
  /////////////////////////////////////////////
  double ratioHBMin_;
  double ratioHBMax_;
  double ratioHEMin_;
  double ratioHEMax_;
  double ratioHFMin_;
  double ratioHFMax_;
  double ratioHOMin_;
  double ratioHOMax_;

  /////////////////////////////////////////////
  int flagtoaskrunsorls_;
  int flagtodefinebadchannel_;
  int howmanybinsonplots_;
  int splashesUpperLimit_;

  int flagabortgaprejected_;
  int bcnrejectedlow_;
  int bcnrejectedhigh_;

  int flagestimatornormalization_;
  int flagcpuoptimization_;
//  int nbadchannels1_;
//  int nbadchannels2_;
//  int nbadchannels3_;
  int lsdep_cut1_peak_HBdepth1_;
  int lsdep_cut1_peak_HBdepth2_;
  int lsdep_cut1_peak_HEdepth1_;
  int lsdep_cut1_peak_HEdepth2_;
  int lsdep_cut1_peak_HEdepth3_;
  int lsdep_cut1_peak_HFdepth1_;
  int lsdep_cut1_peak_HFdepth2_;
  int lsdep_cut1_peak_HOdepth4_;
  //
  int lsdep_cut3_max_HBdepth1_;
  int lsdep_cut3_max_HBdepth2_;
  int lsdep_cut3_max_HEdepth1_;
  int lsdep_cut3_max_HEdepth2_;
  int lsdep_cut3_max_HEdepth3_;
  int lsdep_cut3_max_HFdepth1_;
  int lsdep_cut3_max_HFdepth2_;
  int lsdep_cut3_max_HOdepth4_;

  double lsdep_estimator1_HBdepth1_;
  double lsdep_estimator1_HBdepth2_;
  double lsdep_estimator1_HEdepth1_;
  double lsdep_estimator1_HEdepth2_;
  double lsdep_estimator1_HEdepth3_;
  double lsdep_estimator1_HFdepth1_;
  double lsdep_estimator1_HFdepth2_;
  double lsdep_estimator1_HOdepth4_;

  double lsdep_estimator2_HBdepth1_;
  double lsdep_estimator2_HBdepth2_;
  double lsdep_estimator2_HEdepth1_;
  double lsdep_estimator2_HEdepth2_;
  double lsdep_estimator2_HEdepth3_;
  double lsdep_estimator2_HFdepth1_;
  double lsdep_estimator2_HFdepth2_;
  double lsdep_estimator2_HOdepth4_;

  double lsdep_estimator3_HBdepth1_;
  double lsdep_estimator3_HBdepth2_;
  double lsdep_estimator3_HEdepth1_;
  double lsdep_estimator3_HEdepth2_;
  double lsdep_estimator3_HEdepth3_;
  double lsdep_estimator3_HFdepth1_;
  double lsdep_estimator3_HFdepth2_;
  double lsdep_estimator3_HOdepth4_;

  double lsdep_estimator4_HBdepth1_;
  double lsdep_estimator4_HBdepth2_;
  double lsdep_estimator4_HEdepth1_;
  double lsdep_estimator4_HEdepth2_;
  double lsdep_estimator4_HEdepth3_;
  double lsdep_estimator4_HFdepth1_;
  double lsdep_estimator4_HFdepth2_;
  double lsdep_estimator4_HOdepth4_;

  double lsdep_estimator5_HBdepth1_;
  double lsdep_estimator5_HBdepth2_;
  double lsdep_estimator5_HEdepth1_;
  double lsdep_estimator5_HEdepth2_;
  double lsdep_estimator5_HEdepth3_;
  double lsdep_estimator5_HFdepth1_;
  double lsdep_estimator5_HFdepth2_;
  double lsdep_estimator5_HOdepth4_;

  double forallestimators_amplitude_bigger_;

  /////////////////////////////////////////////
  double rmsHBMin_;
  double rmsHBMax_;
  double rmsHEMin_;
  double rmsHEMax_;
  double rmsHFMin_;
  double rmsHFMax_;
  double rmsHOMin_;
  double rmsHOMax_;

  /////////////////////////////////////////////
  double TSpeakHBMin_;
  double TSpeakHBMax_;
  double TSpeakHEMin_;
  double TSpeakHEMax_;
  double TSpeakHFMin_;
  double TSpeakHFMax_;
  double TSpeakHOMin_;
  double TSpeakHOMax_;

  double TSmeanHBMin_;
  double TSmeanHBMax_;
  double TSmeanHEMin_;
  double TSmeanHEMax_;
  double TSmeanHFMin_;
  double TSmeanHFMax_;
  double TSmeanHOMin_;
  double TSmeanHOMax_;

  /////////////////////////////////////////////
  double ADCAmplHBMin_;
  double ADCAmplHEMin_;
  double ADCAmplHOMin_;
  double ADCAmplHFMin_;
  double ADCAmplHBMax_;
  double ADCAmplHEMax_;
  double ADCAmplHOMax_;
  double ADCAmplHFMax_;

  double calibrADCHBMin_;
  double calibrADCHEMin_;
  double calibrADCHOMin_;
  double calibrADCHFMin_;
  double calibrADCHBMax_;
  double calibrADCHEMax_;
  double calibrADCHOMax_;
  double calibrADCHFMax_;

  double calibrRatioHBMin_;
  double calibrRatioHEMin_;
  double calibrRatioHOMin_;
  double calibrRatioHFMin_;
  double calibrRatioHBMax_;
  double calibrRatioHEMax_;
  double calibrRatioHOMax_;
  double calibrRatioHFMax_;

  double calibrTSmaxHBMin_;
  double calibrTSmaxHEMin_;
  double calibrTSmaxHOMin_;
  double calibrTSmaxHFMin_;
  double calibrTSmaxHBMax_;
  double calibrTSmaxHEMax_;
  double calibrTSmaxHOMax_;
  double calibrTSmaxHFMax_;

  double calibrTSmeanHBMin_;
  double calibrTSmeanHEMin_;
  double calibrTSmeanHOMin_;
  double calibrTSmeanHFMin_;
  double calibrTSmeanHBMax_;
  double calibrTSmeanHEMax_;
  double calibrTSmeanHOMax_;
  double calibrTSmeanHFMax_;

  double calibrWidthHBMin_;
  double calibrWidthHEMin_;
  double calibrWidthHOMin_;
  double calibrWidthHFMin_;
  double calibrWidthHBMax_;
  double calibrWidthHEMax_;
  double calibrWidthHOMax_;
  double calibrWidthHFMax_;

  /////////////////////////////////////////////
  int nevent;
  int nevent50;
  int nnnnnn;
  int counter;
  int counterho;

  int nnnnnn1;
  int nnnnnn2;
  int nnnnnn3;
  int nnnnnn4;
  int nnnnnn5;
  int nnnnnn6;
  int nnnnnn7;
  int nnnnnn8;

  double pedestalwHBMax_;
  double pedestalwHEMax_;
  double pedestalwHFMax_;
  double pedestalwHOMax_;
  double pedestalHBMax_;
  double pedestalHEMax_;
  double pedestalHFMax_;
  double pedestalHOMax_;
  /////////////////////////////////////////////


  TH1F* h_errorGeneral;
  TH1F* h_error1;
  TH1F* h_error2;
  TH1F* h_error3;
  TH1F* h_amplError;
  TH1F* h_amplFine;
  
  TH1F* h_errorGeneral_HB;
  TH1F* h_error1_HB;
  TH1F* h_error2_HB;
  TH1F* h_error3_HB;
  TH1F* h_error4_HB;
  TH1F* h_error5_HB;
  TH1F* h_error6_HB;
  TH1F* h_error7_HB;
  TH1F* h_amplError_HB;
  TH1F* h_amplFine_HB;
  TH2F* h_mapDepth1Error_HB;
  TH2F* h_mapDepth2Error_HB;
  TH2F* h_mapDepth3Error_HB;
  TH1F* h_fiber0_HB;
  TH1F* h_fiber1_HB;
  TH1F* h_fiber2_HB;
  TH1F* h_repetedcapid_HB;

  TH1F* h_errorGeneral_HE;
  TH1F* h_error1_HE;
  TH1F* h_error2_HE;
  TH1F* h_error3_HE;
  TH1F* h_error4_HE;
  TH1F* h_error5_HE;
  TH1F* h_error6_HE;
  TH1F* h_error7_HE;
  TH1F* h_amplError_HE;
  TH1F* h_amplFine_HE;
  TH2F* h_mapDepth1Error_HE;
  TH2F* h_mapDepth2Error_HE;
  TH2F* h_mapDepth3Error_HE;
  TH1F* h_fiber0_HE;
  TH1F* h_fiber1_HE;
  TH1F* h_fiber2_HE;
  TH1F* h_repetedcapid_HE;

  TH1F* h_errorGeneral_HF;
  TH1F* h_error1_HF;
  TH1F* h_error2_HF;
  TH1F* h_error3_HF;
  TH1F* h_error4_HF;
  TH1F* h_error5_HF;
  TH1F* h_error6_HF;
  TH1F* h_error7_HF;
  TH1F* h_amplError_HF;
  TH1F* h_amplFine_HF;
  TH2F* h_mapDepth1Error_HF;
  TH2F* h_mapDepth2Error_HF;
  TH2F* h_mapDepth3Error_HF;
  TH1F* h_fiber0_HF;
  TH1F* h_fiber1_HF;
  TH1F* h_fiber2_HF;
  TH1F* h_repetedcapid_HF;

  TH1F* h_errorGeneral_HO;
  TH1F* h_error1_HO;
  TH1F* h_error2_HO;
  TH1F* h_error3_HO;
  TH1F* h_error4_HO;
  TH1F* h_error5_HO;
  TH1F* h_error6_HO;
  TH1F* h_error7_HO;
  TH1F* h_amplError_HO;
  TH1F* h_amplFine_HO;
  TH2F* h_mapDepth4Error_HO;
  TH1F* h_fiber0_HO;
  TH1F* h_fiber1_HO;
  TH1F* h_fiber2_HO;
  TH1F* h_repetedcapid_HO;

  /////////////////////////////////////////////
  TH2F*     h_mapDepth1ADCAmpl225Copy_HB  ;
  TH2F*     h_mapDepth2ADCAmpl225Copy_HB  ;
  TH2F*     h_mapDepth1ADCAmpl225Copy_HE  ;
  TH2F*     h_mapDepth2ADCAmpl225Copy_HE  ;
  TH2F*     h_mapDepth3ADCAmpl225Copy_HE  ;
  TH2F*     h_mapDepth1ADCAmpl225Copy_HF  ;
  TH2F*     h_mapDepth2ADCAmpl225Copy_HF  ;
  TH2F*     h_mapDepth4ADCAmpl225Copy_HO  ;


  TH1F* h_ADCAmpl345Zoom_HB;
  TH1F* h_ADCAmpl345Zoom1_HB;
  TH1F* h_ADCAmpl345_HB;
  TH1F*   h_ADCAmpl_HBCapIdNoError;
  TH1F*   h_ADCAmpl345_HBCapIdNoError;
  TH1F*   h_ADCAmpl_HBCapIdError;
  TH1F*   h_ADCAmpl345_HBCapIdError;

  TH1F* h_ADCAmplZoom_HB;
  TH1F* h_ADCAmplZoom1_HB;
  TH1F* h_ADCAmpl_HB;
  TH2F* h_mapDepth1ADCAmpl_HB;
  TH2F* h_mapDepth2ADCAmpl_HB;
  TH2F* h_mapDepth1ADCAmpl225_HB;
  TH2F* h_mapDepth2ADCAmpl225_HB;

  TH1F* h_TSmeanA_HB;
  TH2F* h_mapDepth1TSmeanA_HB;
  TH2F* h_mapDepth2TSmeanA_HB;
  TH2F* h_mapDepth1TSmeanA225_HB;
  TH2F* h_mapDepth2TSmeanA225_HB;

  TH1F* h_TSmaxA_HB;
  TH2F* h_mapDepth1TSmaxA_HB;
  TH2F* h_mapDepth2TSmaxA_HB;
  TH2F* h_mapDepth1TSmaxA225_HB;
  TH2F* h_mapDepth2TSmaxA225_HB;
  TH1F* h_Amplitude_HB;
  TH2F* h_mapDepth1Amplitude_HB;
  TH2F* h_mapDepth2Amplitude_HB;
  TH2F* h_mapDepth1Amplitude225_HB;
  TH2F* h_mapDepth2Amplitude225_HB;
  TH1F* h_Ampl_HB;
  TH2F* h_mapDepth1Ampl047_HB;
  TH2F* h_mapDepth2Ampl047_HB;
  TH2F* h_mapDepth1Ampl_HB;
  TH2F* h_mapDepth2Ampl_HB;
  TH2F* h_mapDepth1AmplE34_HB;
  TH2F* h_mapDepth2AmplE34_HB;
  TH2F* h_mapDepth1_HB;
  TH2F* h_mapDepth2_HB;

  TH2F* h_mapDepth1ADCAmpl12_HB;
  TH2F* h_mapDepth2ADCAmpl12_HB;
  TH2F* h_mapDepth1ADCAmpl12_HE;
  TH2F* h_mapDepth2ADCAmpl12_HE;
  TH2F* h_mapDepth3ADCAmpl12_HE;
  TH2F* h_mapDepth1ADCAmpl12_HF;
  TH2F* h_mapDepth2ADCAmpl12_HF;
  TH2F* h_mapDepth4ADCAmpl12_HO;

  /////////////////////////////////////////////
  TH1F* h_ADCAmpl_HF;
  TH1F* h_ADCAmplZoom1_HF;
  TH2F* h_mapDepth1ADCAmpl_HF;
  TH2F* h_mapDepth2ADCAmpl_HF;
  TH2F* h_mapDepth1ADCAmpl225_HF;
  TH2F* h_mapDepth2ADCAmpl225_HF;

  TH1F* h_TSmeanA_HF;
  TH2F* h_mapDepth1TSmeanA_HF;
  TH2F* h_mapDepth2TSmeanA_HF;
  TH2F* h_mapDepth1TSmeanA225_HF;
  TH2F* h_mapDepth2TSmeanA225_HF;

  TH1F* h_TSmaxA_HF;
  TH2F* h_mapDepth1TSmaxA_HF;
  TH2F* h_mapDepth2TSmaxA_HF;
  TH2F* h_mapDepth1TSmaxA225_HF;
  TH2F* h_mapDepth2TSmaxA225_HF;
  TH1F* h_Amplitude_HF;
  TH2F* h_mapDepth1Amplitude_HF;
  TH2F* h_mapDepth2Amplitude_HF;
  TH2F* h_mapDepth1Amplitude225_HF;
  TH2F* h_mapDepth2Amplitude225_HF;
  TH1F* h_Ampl_HF;
  TH2F* h_mapDepth1Ampl047_HF;
  TH2F* h_mapDepth2Ampl047_HF;
  TH2F* h_mapDepth1Ampl_HF;
  TH2F* h_mapDepth2Ampl_HF;
  TH2F* h_mapDepth1AmplE34_HF;
  TH2F* h_mapDepth2AmplE34_HF;
  TH2F* h_mapDepth1_HF;
  TH2F* h_mapDepth2_HF;
  /////////////////////////////////////////////
  TH1F* h_ADCAmpl_HO;
  TH1F* h_ADCAmplZoom1_HO;
  TH1F* h_ADCAmpl_HO_copy;
  TH2F* h_mapDepth4ADCAmpl_HO;
  TH2F* h_mapDepth4ADCAmpl225_HO;

  TH1F* h_TSmeanA_HO;
  TH2F* h_mapDepth4TSmeanA_HO;
  TH2F* h_mapDepth4TSmeanA225_HO;

  TH1F* h_TSmaxA_HO;
  TH2F* h_mapDepth4TSmaxA_HO;
  TH2F* h_mapDepth4TSmaxA225_HO;
  TH1F* h_Amplitude_HO;
  TH2F* h_mapDepth4Amplitude_HO;
  TH2F* h_mapDepth4Amplitude225_HO;
  TH1F* h_Ampl_HO;
  TH2F* h_mapDepth4Ampl047_HO;
  TH2F* h_mapDepth4Ampl_HO;
  TH2F* h_mapDepth4AmplE34_HO;
  TH2F* h_mapDepth4_HO;
  /////////////////////////////////////////////
  TH1F* h_nbadchannels_depth1_HB;
  TH1F* h_runnbadchannels_depth1_HB;
  TH1F* h_runnbadchannelsC_depth1_HB;
  TH1F* h_runbadrate_depth1_HB;
  TH1F* h_runbadrateC_depth1_HB;
  TH1F* h_runbadrate0_depth1_HB;

  TH1F* h_nbadchannels_depth2_HB;
  TH1F* h_runnbadchannels_depth2_HB;
  TH1F* h_runnbadchannelsC_depth2_HB;
  TH1F* h_runbadrate_depth2_HB;
  TH1F* h_runbadrateC_depth2_HB;
  TH1F* h_runbadrate0_depth2_HB;

  TH1F* h_nbadchannels_depth1_HE;
  TH1F* h_runnbadchannels_depth1_HE;
  TH1F* h_runnbadchannelsC_depth1_HE;
  TH1F* h_runbadrate_depth1_HE;
  TH1F* h_runbadrateC_depth1_HE;
  TH1F* h_runbadrate0_depth1_HE;

  TH1F* h_nbadchannels_depth2_HE;
  TH1F* h_runnbadchannels_depth2_HE;
  TH1F* h_runnbadchannelsC_depth2_HE;
  TH1F* h_runbadrate_depth2_HE;
  TH1F* h_runbadrateC_depth2_HE;
  TH1F* h_runbadrate0_depth2_HE;

  TH1F* h_nbadchannels_depth3_HE;
  TH1F* h_runnbadchannels_depth3_HE;
  TH1F* h_runnbadchannelsC_depth3_HE;
  TH1F* h_runbadrate_depth3_HE;
  TH1F* h_runbadrateC_depth3_HE;
  TH1F* h_runbadrate0_depth3_HE;

  TH1F* h_nbadchannels_depth4_HO;
  TH1F* h_runnbadchannels_depth4_HO;
  TH1F* h_runnbadchannelsC_depth4_HO;
  TH1F* h_runbadrate_depth4_HO;
  TH1F* h_runbadrateC_depth4_HO;
  TH1F* h_runbadrate0_depth4_HO;

  TH1F* h_nbadchannels_depth1_HF;
  TH1F* h_runnbadchannels_depth1_HF;
  TH1F* h_runnbadchannelsC_depth1_HF;
  TH1F* h_runbadrate_depth1_HF;
  TH1F* h_runbadrateC_depth1_HF;
  TH1F* h_runbadrate0_depth1_HF;

  TH1F* h_nbadchannels_depth2_HF;
  TH1F* h_runnbadchannels_depth2_HF;
  TH1F* h_runnbadchannelsC_depth2_HF;
  TH1F* h_runbadrate_depth2_HF;
  TH1F* h_runbadrateC_depth2_HF;
  TH1F* h_runbadrate0_depth2_HF;

TH1F*     h_bcnnbadchannels_depth1_HB;
TH1F*     h_bcnnbadchannels_depth2_HB;
TH1F*     h_bcnnbadchannels_depth1_HE;
TH1F*     h_bcnnbadchannels_depth2_HE;
TH1F*     h_bcnnbadchannels_depth3_HE;
TH1F*     h_bcnnbadchannels_depth4_HO;
TH1F*     h_bcnnbadchannels_depth1_HF;
TH1F*     h_bcnnbadchannels_depth2_HF;
TH1F*     h_bcnbadrate0_depth1_HB;
TH1F*     h_bcnbadrate0_depth2_HB;
TH1F*     h_bcnbadrate0_depth1_HE;
TH1F*     h_bcnbadrate0_depth2_HE;
TH1F*     h_bcnbadrate0_depth3_HE;
TH1F*     h_bcnbadrate0_depth4_HO;
TH1F*     h_bcnbadrate0_depth1_HF;
TH1F*     h_bcnbadrate0_depth2_HF;


TH1F*         h_Amplitude_forCapIdErrors_HB1;
TH1F*         h_Amplitude_forCapIdErrors_HB2;
TH1F*         h_Amplitude_forCapIdErrors_HE1;
TH1F*         h_Amplitude_forCapIdErrors_HE2;
TH1F*         h_Amplitude_forCapIdErrors_HE3;
TH1F*         h_Amplitude_forCapIdErrors_HF1;
TH1F*         h_Amplitude_forCapIdErrors_HF2;
TH1F*         h_Amplitude_forCapIdErrors_HO4;

TH1F*         h_Amplitude_notCapIdErrors_HB1;
TH1F*         h_Amplitude_notCapIdErrors_HB2;
TH1F*         h_Amplitude_notCapIdErrors_HE1;
TH1F*         h_Amplitude_notCapIdErrors_HE2;
TH1F*         h_Amplitude_notCapIdErrors_HE3;
TH1F*         h_Amplitude_notCapIdErrors_HF1;
TH1F*         h_Amplitude_notCapIdErrors_HF2;
TH1F*         h_Amplitude_notCapIdErrors_HO4;

  /////////////////////////////////////////////
  TH1F* h_ADCAmpl_HE;
  TH1F* h_ADCAmplZoom1_HE;
  TH2F* h_mapDepth1ADCAmpl_HE;
  TH2F* h_mapDepth2ADCAmpl_HE;
  TH2F* h_mapDepth3ADCAmpl_HE;
  TH2F* h_mapDepth1ADCAmpl225_HE;
  TH2F* h_mapDepth2ADCAmpl225_HE;
  TH2F* h_mapDepth3ADCAmpl225_HE;

  TH1F* h_TSmeanA_HE;
  TH2F* h_mapDepth1TSmeanA_HE;
  TH2F* h_mapDepth2TSmeanA_HE;
  TH2F* h_mapDepth3TSmeanA_HE;
  TH2F* h_mapDepth1TSmeanA225_HE;
  TH2F* h_mapDepth2TSmeanA225_HE;
  TH2F* h_mapDepth3TSmeanA225_HE;

  TH1F* h_TSmaxA_HE;
  TH2F* h_mapDepth1TSmaxA_HE;
  TH2F* h_mapDepth2TSmaxA_HE;
  TH2F* h_mapDepth3TSmaxA_HE;
  TH2F* h_mapDepth1TSmaxA225_HE;
  TH2F* h_mapDepth2TSmaxA225_HE;
  TH2F* h_mapDepth3TSmaxA225_HE;

  TH1F* h_Amplitude_HE;
  TH2F* h_mapDepth1Amplitude_HE;
  TH2F* h_mapDepth2Amplitude_HE;
  TH2F* h_mapDepth3Amplitude_HE;
  TH2F* h_mapDepth1Amplitude225_HE;
  TH2F* h_mapDepth2Amplitude225_HE;
  TH2F* h_mapDepth3Amplitude225_HE;

  TH1F* h_Ampl_HE;
  TH2F* h_mapDepth1Ampl047_HE;
  TH2F* h_mapDepth2Ampl047_HE;
  TH2F* h_mapDepth3Ampl047_HE;
  TH2F* h_mapDepth1Ampl_HE;
  TH2F* h_mapDepth2Ampl_HE;
  TH2F* h_mapDepth3Ampl_HE;
  TH2F* h_mapDepth1AmplE34_HE;
  TH2F* h_mapDepth2AmplE34_HE;
  TH2F* h_mapDepth3AmplE34_HE;
  TH2F* h_mapDepth1_HE;
  TH2F* h_mapDepth2_HE;
  TH2F* h_mapDepth3_HE;

  /////////////////////////////////////////////
  /*
  TH1F* h_GetRMSOverNormalizedSignal_HB;
  TH1F* h_GetRMSOverNormalizedSignal_HE;
  TH1F* h_GetRMSOverNormalizedSignal_HO;
  TH1F* h_GetRMSOverNormalizedSignal_HF;
  TH1F* h_GetRMSOverNormalizedSignal3_HB;
  TH1F* h_GetRMSOverNormalizedSignal3_HE;
  TH1F* h_GetRMSOverNormalizedSignal3_HO;
  TH1F* h_GetRMSOverNormalizedSignal3_HF;
*/

  TH2F* h_FullSignal3D_HB;
  TH2F* h_FullSignal3D0_HB;
  TH2F* h_FullSignal3D_HE;
  TH2F* h_FullSignal3D0_HE;
  TH2F* h_FullSignal3D_HO;
  TH2F* h_FullSignal3D0_HO;
  TH2F* h_FullSignal3D_HF;
  TH2F* h_FullSignal3D0_HF;

  /////////////////////////////////////////////
  TH2F* h_mapCapCalib047_HB;
  TH2F* h_mapCapCalib047_HE;
  TH2F* h_mapCapCalib047_HO;
  TH2F* h_mapCapCalib047_HF;

  TH1F* h_ADCCalib_HB;
  TH1F* h_ADCCalib1_HB;
  TH2F* h_mapADCCalib047_HB;
  TH2F* h_mapADCCalib_HB;
  TH1F* h_RatioCalib_HB;
  TH2F* h_mapRatioCalib047_HB;
  TH2F* h_mapRatioCalib_HB;
  TH1F* h_TSmaxCalib_HB;
  TH2F* h_mapTSmaxCalib047_HB;
  TH2F* h_mapTSmaxCalib_HB;
  TH1F* h_TSmeanCalib_HB;
  TH2F* h_mapTSmeanCalib047_HB;
  TH2F* h_mapTSmeanCalib_HB;
  TH1F* h_WidthCalib_HB;
  TH2F* h_mapWidthCalib047_HB;
  TH2F* h_mapWidthCalib_HB;
  TH2F* h_map_HB;
  TH1F* h_ADCCalib_HE;
  TH1F* h_ADCCalib1_HE;
  TH2F* h_mapADCCalib047_HE;
  TH2F* h_mapADCCalib_HE;
  TH1F* h_RatioCalib_HE;
  TH2F* h_mapRatioCalib047_HE;
  TH2F* h_mapRatioCalib_HE;
  TH1F* h_TSmaxCalib_HE;
  TH2F* h_mapTSmaxCalib047_HE;
  TH2F* h_mapTSmaxCalib_HE;
  TH1F* h_TSmeanCalib_HE;
  TH2F* h_mapTSmeanCalib047_HE;
  TH2F* h_mapTSmeanCalib_HE;
  TH1F* h_WidthCalib_HE;
  TH2F* h_mapWidthCalib047_HE;
  TH2F* h_mapWidthCalib_HE;
  TH2F* h_map_HE;
  TH1F* h_ADCCalib_HO;
  TH1F* h_ADCCalib1_HO;
  TH2F* h_mapADCCalib047_HO;
  TH2F* h_mapADCCalib_HO;
  TH1F* h_RatioCalib_HO;
  TH2F* h_mapRatioCalib047_HO;
  TH2F* h_mapRatioCalib_HO;
  TH1F* h_TSmaxCalib_HO;
  TH2F* h_mapTSmaxCalib047_HO;
  TH2F* h_mapTSmaxCalib_HO;
  TH1F* h_TSmeanCalib_HO;
  TH2F* h_mapTSmeanCalib047_HO;
  TH2F* h_mapTSmeanCalib_HO;
  TH1F* h_WidthCalib_HO;
  TH2F* h_mapWidthCalib047_HO;
  TH2F* h_mapWidthCalib_HO;
  TH2F* h_map_HO;
  TH1F* h_ADCCalib_HF;
  TH1F* h_ADCCalib1_HF;
  TH2F* h_mapADCCalib047_HF;
  TH2F* h_mapADCCalib_HF;
  TH1F* h_RatioCalib_HF;
  TH2F* h_mapRatioCalib047_HF;
  TH2F* h_mapRatioCalib_HF;
  TH1F* h_TSmaxCalib_HF;
  TH2F* h_mapTSmaxCalib047_HF;
  TH2F* h_mapTSmaxCalib_HF;
  TH1F* h_TSmeanCalib_HF;
  TH2F* h_mapTSmeanCalib047_HF;
  TH2F* h_mapTSmeanCalib_HF;
  TH1F* h_WidthCalib_HF;
  TH2F* h_mapWidthCalib047_HF;
  TH2F* h_mapWidthCalib_HF;
  TH2F* h_map_HF;

  TH1F*     h_nls_per_run ;
  TH1F*     h_nls_per_run10;
  TH1F*    h_nevents_per_LS;
  TH1F*    h_nevents_per_LSzoom;
  TH1F*    h_nevents_per_eachLS;
  TH1F*    h_nevents_per_eachRealLS;
  TH1F*    h_lsnumber_per_eachLS;

  // LS dependencies for estimator0:
  TH1F*    h_sumPedestalLS1 ;
  TH2F*    h_2DsumPedestalLS1;
  TH1F*    h_sumPedestalperLS1;
  TH2F*    h_2D0sumPedestalLS1;
  TH1F*    h_sum0PedestalperLS1;

  TH1F*    h_sumPedestalLS2         ;
  TH2F*    h_2DsumPedestalLS2         ;
  TH1F*    h_sumPedestalperLS2         ;
  TH2F*    h_2D0sumPedestalLS2         ;
  TH1F*    h_sum0PedestalperLS2         ;

  TH1F*    h_sumPedestalLS3         ;
  TH2F*    h_2DsumPedestalLS3         ;
  TH1F*    h_sumPedestalperLS3         ;
  TH2F*    h_2D0sumPedestalLS3         ;
  TH1F*    h_sum0PedestalperLS3         ;

  TH1F*    h_sumPedestalLS4         ;
  TH2F*    h_2DsumPedestalLS4         ;
  TH1F*    h_sumPedestalperLS4         ;
  TH2F*    h_2D0sumPedestalLS4         ;
  TH1F*    h_sum0PedestalperLS4         ;

  TH1F*    h_sumPedestalLS5         ;
  TH2F*    h_2DsumPedestalLS5         ;
  TH1F*    h_sumPedestalperLS5         ;
  TH2F*    h_2D0sumPedestalLS5         ;
  TH1F*    h_sum0PedestalperLS5         ;

  TH1F*    h_sumPedestalLS6         ;
  TH2F*    h_2DsumPedestalLS6         ;
  TH1F*    h_sumPedestalperLS6         ;
  TH2F*    h_2D0sumPedestalLS6         ;
  TH1F*    h_sum0PedestalperLS6         ;

  TH1F*    h_sumPedestalLS7         ;
  TH2F*    h_2DsumPedestalLS7         ;
  TH1F*    h_sumPedestalperLS7         ;
  TH2F*    h_2D0sumPedestalLS7         ;
  TH1F*    h_sum0PedestalperLS7         ;

  TH1F*    h_sumPedestalLS8         ;
  TH2F*    h_2DsumPedestalLS8         ;
  TH1F*    h_sumPedestalperLS8        ;
  TH2F*    h_2D0sumPedestalLS8         ;
  TH1F*    h_sum0PedestalperLS8         ;

  // LS dependencies for estimator1:
  TH1F*    h_sumADCAmplLS1copy1    ;
  TH1F*    h_sumADCAmplLS1copy2    ;
  TH1F*    h_sumADCAmplLS1copy3    ;
  TH1F*    h_sumADCAmplLS1copy4    ;
  TH1F*    h_sumADCAmplLS1copy5    ;
  TH1F*    h_sumADCAmplLS1         ;
  TH2F*    h_2DsumADCAmplLS1         ;
  TH2F*    h_2DsumADCAmplLS1_LSselected;
  TH1F*    h_sumADCAmplperLS1         ;
  TH1F*    h_sumCutADCAmplperLS1         ;
  TH2F*    h_2D0sumADCAmplLS1         ;
  TH1F*    h_sum0ADCAmplperLS1         ;

  TH1F*    h_sumADCAmplLS2         ;
  TH2F*    h_2DsumADCAmplLS2         ;
  TH2F*    h_2DsumADCAmplLS2_LSselected;
  TH1F*    h_sumADCAmplperLS2         ;
  TH1F*    h_sumCutADCAmplperLS2         ;
  TH2F*    h_2D0sumADCAmplLS2         ;
  TH1F*    h_sum0ADCAmplperLS2         ;
  
  TH1F*    h_sumADCAmplLS3         ;
  TH2F*    h_2DsumADCAmplLS3         ;
  TH2F*    h_2DsumADCAmplLS3_LSselected;
  TH1F*    h_sumADCAmplperLS3         ;
  TH1F*    h_sumCutADCAmplperLS3         ;
  TH2F*    h_2D0sumADCAmplLS3         ;
  TH1F*    h_sum0ADCAmplperLS3         ;
  
  TH1F*    h_sumADCAmplLS4         ;
  TH2F*    h_2DsumADCAmplLS4         ;
  TH2F*    h_2DsumADCAmplLS4_LSselected;
  TH1F*    h_sumADCAmplperLS4         ;
  TH1F*    h_sumCutADCAmplperLS4         ;
  TH2F*    h_2D0sumADCAmplLS4         ;
  TH1F*    h_sum0ADCAmplperLS4         ;
  
  TH1F*    h_sumADCAmplLS5         ;
  TH2F*    h_2DsumADCAmplLS5         ;
  TH2F*    h_2DsumADCAmplLS5_LSselected;
  TH1F*    h_sumADCAmplperLS5         ;
  TH1F*    h_sumCutADCAmplperLS5         ;
  TH2F*    h_2D0sumADCAmplLS5         ;
  TH1F*    h_sum0ADCAmplperLS5         ;
  
  TH1F*    h_sumADCAmplLS6         ;
  TH2F*    h_2DsumADCAmplLS6         ;
  TH2F*    h_2DsumADCAmplLS6_LSselected;
  TH1F*    h_sumADCAmplperLS6         ;
  TH1F*    h_sumCutADCAmplperLS6         ;
  TH2F*    h_2D0sumADCAmplLS6         ;
  TH1F*    h_sum0ADCAmplperLS6         ;

  TH1F*    h_sumADCAmplperLS1_P1         ;
  TH1F*    h_sum0ADCAmplperLS1_P1         ;
  TH1F*    h_sumADCAmplperLS1_P2         ;
  TH1F*    h_sum0ADCAmplperLS1_P2         ;
  TH1F*    h_sumADCAmplperLS1_M1         ;
  TH1F*    h_sum0ADCAmplperLS1_M1         ;
  TH1F*    h_sumADCAmplperLS1_M2         ;
  TH1F*    h_sum0ADCAmplperLS1_M2         ;

  TH1F*    h_sumADCAmplperLS3_P1         ;
  TH1F*    h_sum0ADCAmplperLS3_P1         ;
  TH1F*    h_sumADCAmplperLS3_P2         ;
  TH1F*    h_sum0ADCAmplperLS3_P2         ;
  TH1F*    h_sumADCAmplperLS3_M1         ;
  TH1F*    h_sum0ADCAmplperLS3_M1         ;
  TH1F*    h_sumADCAmplperLS3_M2         ;
  TH1F*    h_sum0ADCAmplperLS3_M2         ;

  TH1F*    h_sumADCAmplperLS6_P1         ;
  TH1F*    h_sum0ADCAmplperLS6_P1         ;
  TH1F*    h_sumADCAmplperLS6_P2         ;
  TH1F*    h_sum0ADCAmplperLS6_P2         ;
  TH1F*    h_sumADCAmplperLS6_M1         ;
  TH1F*    h_sum0ADCAmplperLS6_M1         ;
  TH1F*    h_sumADCAmplperLS6_M2         ;
  TH1F*    h_sum0ADCAmplperLS6_M2         ;

  TH1F*    h_sumADCAmplperLS8_P1         ;
  TH1F*    h_sum0ADCAmplperLS8_P1         ;
  TH1F*    h_sumADCAmplperLS8_P2         ;
  TH1F*    h_sum0ADCAmplperLS8_P2         ;
  TH1F*    h_sumADCAmplperLS8_M1         ;
  TH1F*    h_sum0ADCAmplperLS8_M1         ;
  TH1F*    h_sumADCAmplperLS8_M2         ;
  TH1F*    h_sum0ADCAmplperLS8_M2         ;


  
  TH1F*    h_sumADCAmplLS7         ;
  TH2F*    h_2DsumADCAmplLS7         ;
  TH2F*    h_2DsumADCAmplLS7_LSselected;
  TH1F*    h_sumADCAmplperLS7         ;
  TH1F*    h_sumCutADCAmplperLS7         ;
  TH2F*    h_2D0sumADCAmplLS7         ;
  TH1F*    h_sum0ADCAmplperLS7         ;
  
  TH1F*    h_sumADCAmplLS8         ;
  TH2F*    h_2DsumADCAmplLS8         ;
  TH2F*    h_2DsumADCAmplLS8_LSselected;
  TH1F*    h_sumADCAmplperLS8         ;
  TH1F*    h_sumCutADCAmplperLS8         ;
  TH2F*    h_2D0sumADCAmplLS8         ;
  TH1F*    h_sum0ADCAmplperLS8         ;
  
  // LS dependencies for estimator2:
  TH1F*    h_sumTSmeanALS1         ;
  TH2F*    h_2DsumTSmeanALS1         ;
  TH1F*    h_sumTSmeanAperLS1         ;
  TH1F*    h_sumTSmeanAperLS1_LSselected         ;
  TH1F*    h_sumCutTSmeanAperLS1         ;
  TH2F*    h_2D0sumTSmeanALS1         ;
  TH1F*    h_sum0TSmeanAperLS1         ;

  TH1F*    h_sumTSmeanALS2         ;
  TH2F*    h_2DsumTSmeanALS2         ;
  TH1F*    h_sumTSmeanAperLS2         ;
  TH1F*    h_sumCutTSmeanAperLS2         ;
  TH2F*    h_2D0sumTSmeanALS2         ;
  TH1F*    h_sum0TSmeanAperLS2         ;
  
  TH1F*    h_sumTSmeanALS3         ;
  TH2F*    h_2DsumTSmeanALS3         ;
  TH1F*    h_sumTSmeanAperLS3         ;
  TH1F*    h_sumCutTSmeanAperLS3         ;
  TH2F*    h_2D0sumTSmeanALS3         ;
  TH1F*    h_sum0TSmeanAperLS3         ;
  
  TH1F*    h_sumTSmeanALS4         ;
  TH2F*    h_2DsumTSmeanALS4         ;
  TH1F*    h_sumTSmeanAperLS4         ;
  TH1F*    h_sumCutTSmeanAperLS4         ;
  TH2F*    h_2D0sumTSmeanALS4         ;
  TH1F*    h_sum0TSmeanAperLS4         ;
  
  TH1F*    h_sumTSmeanALS5         ;
  TH2F*    h_2DsumTSmeanALS5         ;
  TH1F*    h_sumTSmeanAperLS5         ;
  TH1F*    h_sumCutTSmeanAperLS5         ;
  TH2F*    h_2D0sumTSmeanALS5         ;
  TH1F*    h_sum0TSmeanAperLS5         ;
  
  TH1F*    h_sumTSmeanALS6         ;
  TH2F*    h_2DsumTSmeanALS6         ;
  TH1F*    h_sumTSmeanAperLS6         ;
  TH1F*    h_sumCutTSmeanAperLS6         ;
  TH2F*    h_2D0sumTSmeanALS6         ;
  TH1F*    h_sum0TSmeanAperLS6         ;
  
  TH1F*    h_sumTSmeanALS7         ;
  TH2F*    h_2DsumTSmeanALS7         ;
  TH1F*    h_sumTSmeanAperLS7         ;
  TH1F*    h_sumCutTSmeanAperLS7         ;
  TH2F*    h_2D0sumTSmeanALS7         ;
  TH1F*    h_sum0TSmeanAperLS7         ;
  
  TH1F*    h_sumTSmeanALS8         ;
  TH2F*    h_2DsumTSmeanALS8         ;
  TH1F*    h_sumTSmeanAperLS8         ;
  TH1F*    h_sumCutTSmeanAperLS8         ;
  TH2F*    h_2D0sumTSmeanALS8         ;
  TH1F*    h_sum0TSmeanAperLS8         ;
  
  // LS dependencies for estimator3:
  TH1F*    h_sumTSmaxALS1         ;
  TH2F*    h_2DsumTSmaxALS1         ;
  TH1F*    h_sumTSmaxAperLS1         ;
  TH1F*    h_sumTSmaxAperLS1_LSselected         ;
  TH1F*    h_sumCutTSmaxAperLS1         ;
  TH2F*    h_2D0sumTSmaxALS1         ;
  TH1F*    h_sum0TSmaxAperLS1         ;

  TH1F*    h_sumTSmaxALS2         ;
  TH2F*    h_2DsumTSmaxALS2         ;
  TH1F*    h_sumTSmaxAperLS2         ;
  TH1F*    h_sumCutTSmaxAperLS2         ;
  TH2F*    h_2D0sumTSmaxALS2         ;
  TH1F*    h_sum0TSmaxAperLS2         ;
  
  TH1F*    h_sumTSmaxALS3         ;
  TH2F*    h_2DsumTSmaxALS3         ;
  TH1F*    h_sumTSmaxAperLS3         ;
  TH1F*    h_sumCutTSmaxAperLS3         ;
  TH2F*    h_2D0sumTSmaxALS3         ;
  TH1F*    h_sum0TSmaxAperLS3         ;
  
  TH1F*    h_sumTSmaxALS4         ;
  TH2F*    h_2DsumTSmaxALS4         ;
  TH1F*    h_sumTSmaxAperLS4         ;
  TH1F*    h_sumCutTSmaxAperLS4         ;
  TH2F*    h_2D0sumTSmaxALS4         ;
  TH1F*    h_sum0TSmaxAperLS4         ;
  
  TH1F*    h_sumTSmaxALS5         ;
  TH2F*    h_2DsumTSmaxALS5         ;
  TH1F*    h_sumTSmaxAperLS5         ;
  TH1F*    h_sumCutTSmaxAperLS5         ;
  TH2F*    h_2D0sumTSmaxALS5         ;
  TH1F*    h_sum0TSmaxAperLS5         ;
  
  TH1F*    h_sumTSmaxALS6         ;
  TH2F*    h_2DsumTSmaxALS6         ;
  TH1F*    h_sumTSmaxAperLS6         ;
  TH1F*    h_sumCutTSmaxAperLS6         ;
  TH2F*    h_2D0sumTSmaxALS6         ;
  TH1F*    h_sum0TSmaxAperLS6         ;
  
  TH1F*    h_sumTSmaxALS7         ;
  TH2F*    h_2DsumTSmaxALS7         ;
  TH1F*    h_sumTSmaxAperLS7         ;
  TH1F*    h_sumCutTSmaxAperLS7         ;
  TH2F*    h_2D0sumTSmaxALS7         ;
  TH1F*    h_sum0TSmaxAperLS7         ;
  
  TH1F*    h_sumTSmaxALS8         ;
  TH2F*    h_2DsumTSmaxALS8         ;
  TH1F*    h_sumTSmaxAperLS8         ;
  TH1F*    h_sumCutTSmaxAperLS8         ;
  TH2F*    h_2D0sumTSmaxALS8         ;
  TH1F*    h_sum0TSmaxAperLS8         ;
  
  // LS dependencies for estimator4:
  TH1F*    h_sumAmplitudeLS1         ;
  TH2F*    h_2DsumAmplitudeLS1         ;
  TH1F*    h_sumAmplitudeperLS1         ;
  TH1F*    h_sumAmplitudeperLS1_LSselected         ;
  TH1F*    h_sumCutAmplitudeperLS1         ;
  TH2F*    h_2D0sumAmplitudeLS1         ;
  TH1F*    h_sum0AmplitudeperLS1         ;

  TH1F*    h_sumAmplitudeLS2         ;
  TH2F*    h_2DsumAmplitudeLS2         ;
  TH1F*    h_sumAmplitudeperLS2         ;
  TH1F*    h_sumCutAmplitudeperLS2         ;
  TH2F*    h_2D0sumAmplitudeLS2         ;
  TH1F*    h_sum0AmplitudeperLS2         ;
  
  TH1F*    h_sumAmplitudeLS3         ;
  TH2F*    h_2DsumAmplitudeLS3         ;
  TH1F*    h_sumAmplitudeperLS3         ;
  TH1F*    h_sumCutAmplitudeperLS3         ;
  TH2F*    h_2D0sumAmplitudeLS3         ;
  TH1F*    h_sum0AmplitudeperLS3         ;
  
  TH1F*    h_sumAmplitudeLS4         ;
  TH2F*    h_2DsumAmplitudeLS4         ;
  TH1F*    h_sumAmplitudeperLS4         ;
  TH1F*    h_sumCutAmplitudeperLS4         ;
  TH2F*    h_2D0sumAmplitudeLS4         ;
  TH1F*    h_sum0AmplitudeperLS4         ;
  
  TH1F*    h_sumAmplitudeLS5         ;
  TH2F*    h_2DsumAmplitudeLS5         ;
  TH1F*    h_sumAmplitudeperLS5         ;
  TH1F*    h_sumCutAmplitudeperLS5         ;
  TH2F*    h_2D0sumAmplitudeLS5         ;
  TH1F*    h_sum0AmplitudeperLS5         ;
  
  TH1F*    h_sumAmplitudeLS6         ;
  TH2F*    h_2DsumAmplitudeLS6         ;
  TH1F*    h_sumAmplitudeperLS6         ;
  TH1F*    h_sumCutAmplitudeperLS6         ;
  TH2F*    h_2D0sumAmplitudeLS6         ;
  TH1F*    h_sum0AmplitudeperLS6         ;
  
  TH1F*    h_sumAmplitudeLS7         ;
  TH2F*    h_2DsumAmplitudeLS7         ;
  TH1F*    h_sumAmplitudeperLS7         ;
  TH1F*    h_sumCutAmplitudeperLS7         ;
  TH2F*    h_2D0sumAmplitudeLS7         ;
  TH1F*    h_sum0AmplitudeperLS7         ;
  
  TH1F*    h_sumAmplitudeLS8         ;
  TH2F*    h_2DsumAmplitudeLS8         ;
  TH1F*    h_sumAmplitudeperLS8         ;
  TH1F*    h_sumCutAmplitudeperLS8         ;
  TH2F*    h_2D0sumAmplitudeLS8         ;
  TH1F*    h_sum0AmplitudeperLS8         ;
  
  // LS dependencies for estimator5:
  TH1F*    h_sumAmplLS1         ;
  TH2F*    h_2DsumAmplLS1         ;
  TH1F*    h_sumAmplperLS1         ;
  TH1F*    h_sumAmplperLS1_LSselected         ;
  TH1F*    h_sumCutAmplperLS1         ;
  TH2F*    h_2D0sumAmplLS1         ;
  TH1F*    h_sum0AmplperLS1         ;

  TH1F*    h_sumAmplLS2         ;
  TH2F*    h_2DsumAmplLS2         ;
  TH1F*    h_sumAmplperLS2         ;
  TH1F*    h_sumCutAmplperLS2         ;
  TH2F*    h_2D0sumAmplLS2         ;
  TH1F*    h_sum0AmplperLS2         ;
  
  TH1F*    h_sumAmplLS3         ;
  TH2F*    h_2DsumAmplLS3         ;
  TH1F*    h_sumAmplperLS3         ;
  TH1F*    h_sumCutAmplperLS3         ;
  TH2F*    h_2D0sumAmplLS3         ;
  TH1F*    h_sum0AmplperLS3         ;
  
  TH1F*    h_sumAmplLS4         ;
  TH2F*    h_2DsumAmplLS4         ;
  TH1F*    h_sumAmplperLS4         ;
  TH1F*    h_sumCutAmplperLS4         ;
  TH2F*    h_2D0sumAmplLS4         ;
  TH1F*    h_sum0AmplperLS4         ;
  
  TH1F*    h_sumAmplLS5         ;
  TH2F*    h_2DsumAmplLS5         ;
  TH1F*    h_sumAmplperLS5         ;
  TH1F*    h_sumCutAmplperLS5         ;
  TH2F*    h_2D0sumAmplLS5         ;
  TH1F*    h_sum0AmplperLS5         ;
  
  TH1F*    h_sumAmplLS6         ;
  TH2F*    h_2DsumAmplLS6         ;
  TH1F*    h_sumAmplperLS6         ;
  TH1F*    h_sumCutAmplperLS6         ;
  TH2F*    h_2D0sumAmplLS6         ;
  TH1F*    h_sum0AmplperLS6         ;

  TH1F*        h_RatioOccupancy_HBP         ;
  TH1F*        h_RatioOccupancy_HBM         ;
  TH1F*        h_RatioOccupancy_HEP         ;
  TH1F*        h_RatioOccupancy_HEM         ;
  TH1F*        h_RatioOccupancy_HOP         ;
  TH1F*        h_RatioOccupancy_HOM         ;
  TH1F*        h_RatioOccupancy_HFP         ;
  TH1F*        h_RatioOccupancy_HFM         ;


  
  TH1F*    h_sumAmplLS7         ;
  TH2F*    h_2DsumAmplLS7         ;
  TH1F*    h_sumAmplperLS7         ;
  TH1F*    h_sumCutAmplperLS7         ;
  TH2F*    h_2D0sumAmplLS7         ;
  TH1F*    h_sum0AmplperLS7         ;
  
  TH1F*    h_sumAmplLS8         ;
  TH2F*    h_2DsumAmplLS8         ;
  TH1F*    h_sumAmplperLS8         ;
  TH1F*    h_sumCutAmplperLS8         ;
  TH2F*    h_2D0sumAmplLS8         ;
  TH1F*    h_sum0AmplperLS8         ;

  TH1F*  h_pedestal0_HB ;
  TH1F*  h_pedestal1_HB ;
  TH1F*  h_pedestal2_HB ;
  TH1F*  h_pedestal3_HB ;
  TH1F*  h_pedestalaver4_HB ;
  TH1F*  h_pedestalaver9_HB ;
  TH1F*  h_pedestalw0_HB ;
  TH1F*  h_pedestalw1_HB ;
  TH1F*  h_pedestalw2_HB ;
  TH1F*  h_pedestalw3_HB ;
  TH1F*  h_pedestalwaver4_HB ;
  TH1F*  h_pedestalwaver9_HB ;

  TH1F*  h_pedestal0_HE ;
  TH1F*  h_pedestal1_HE ;
  TH1F*  h_pedestal2_HE ;
  TH1F*  h_pedestal3_HE ;
  TH1F*  h_pedestalaver4_HE ;
  TH1F*  h_pedestalaver9_HE ;
  TH1F*  h_pedestalw0_HE ;
  TH1F*  h_pedestalw1_HE ;
  TH1F*  h_pedestalw2_HE ;
  TH1F*  h_pedestalw3_HE ;
  TH1F*  h_pedestalwaver4_HE ;
  TH1F*  h_pedestalwaver9_HE ;

  TH1F*  h_pedestal0_HF ;
  TH1F*  h_pedestal1_HF ;
  TH1F*  h_pedestal2_HF ;
  TH1F*  h_pedestal3_HF ;
  TH1F*  h_pedestalaver4_HF ;
  TH1F*  h_pedestalaver9_HF ;
  TH1F*  h_pedestalw0_HF ;
  TH1F*  h_pedestalw1_HF ;
  TH1F*  h_pedestalw2_HF ;
  TH1F*  h_pedestalw3_HF ;
  TH1F*  h_pedestalwaver4_HF ;
  TH1F*  h_pedestalwaver9_HF ;

  TH1F*  h_pedestal0_HO ;
  TH1F*  h_pedestal1_HO ;
  TH1F*  h_pedestal2_HO ;
  TH1F*  h_pedestal3_HO ;
  TH1F*  h_pedestalaver4_HO ;
  TH1F*  h_pedestalaver9_HO ;
  TH1F*  h_pedestalw0_HO ;
  TH1F*  h_pedestalw1_HO ;
  TH1F*  h_pedestalw2_HO ;
  TH1F*  h_pedestalw3_HO ;
  TH1F*  h_pedestalwaver4_HO ;
  TH1F*  h_pedestalwaver9_HO ;

  TH2F*     h_mapDepth1pedestalw_HB;
  TH2F*     h_mapDepth2pedestalw_HB;
  TH2F*     h_mapDepth1pedestalw_HE;
  TH2F*     h_mapDepth2pedestalw_HE;
  TH2F*     h_mapDepth3pedestalw_HE;
  TH2F*     h_mapDepth1pedestalw_HF;
  TH2F*     h_mapDepth2pedestalw_HF;
  TH2F*     h_mapDepth4pedestalw_HO;

  TH2F*     h_mapDepth1pedestal_HB;
  TH2F*     h_mapDepth2pedestal_HB;
  TH2F*     h_mapDepth1pedestal_HE;
  TH2F*     h_mapDepth2pedestal_HE;
  TH2F*     h_mapDepth3pedestal_HE;
  TH2F*     h_mapDepth1pedestal_HF;
  TH2F*     h_mapDepth2pedestal_HF;
  TH2F*     h_mapDepth4pedestal_HO;

  TH1F*  h_pedestal00_HB;
  TH1F*  h_gain_HB;
  TH1F*  h_respcorr_HB;
  TH1F*  h_timecorr_HB;
  TH1F*  h_lutcorr_HB;
  TH1F*  h_difpedestal0_HB;
  TH1F*  h_difpedestal1_HB;
  TH1F*  h_difpedestal2_HB;
  TH1F*  h_difpedestal3_HB;

  TH1F*  h_pedestal00_HE;
  TH1F*  h_gain_HE;
  TH1F*  h_respcorr_HE;
  TH1F*  h_timecorr_HE;
  TH1F*  h_lutcorr_HE;
  
  TH1F*  h_pedestal00_HF;
  TH1F*  h_gain_HF;
  TH1F*  h_respcorr_HF;
  TH1F*  h_timecorr_HF;
  TH1F*  h_lutcorr_HF;
  
  TH1F*  h_pedestal00_HO;
  TH1F*  h_gain_HO;
  TH1F*  h_respcorr_HO;
  TH1F*  h_timecorr_HO;
  TH1F*  h_lutcorr_HO;
  
  TH2F*      h2_pedvsampl_HB;
  TH2F*      h2_pedwvsampl_HB;
  TH1F*      h_pedvsampl_HB;
  TH1F*      h_pedwvsampl_HB;
  TH1F*      h_pedvsampl0_HB;
  TH1F*      h_pedwvsampl0_HB;
  TH2F*      h2_amplvsped_HB;
  TH2F*      h2_amplvspedw_HB;
  TH1F*      h_amplvsped_HB;
  TH1F*      h_amplvspedw_HB;
  TH1F*      h_amplvsped0_HB;

  TH2F*      h2_pedvsampl_HE;
  TH2F*      h2_pedwvsampl_HE;
  TH1F*      h_pedvsampl_HE;
  TH1F*      h_pedwvsampl_HE;
  TH1F*      h_pedvsampl0_HE;
  TH1F*      h_pedwvsampl0_HE;
  TH2F*      h2_pedvsampl_HF;
  TH2F*      h2_pedwvsampl_HF;
  TH1F*      h_pedvsampl_HF;
  TH1F*      h_pedwvsampl_HF;
  TH1F*      h_pedvsampl0_HF;
  TH1F*      h_pedwvsampl0_HF;
  TH2F*      h2_pedvsampl_HO;
  TH2F*      h2_pedwvsampl_HO;
  TH1F*      h_pedvsampl_HO;
  TH1F*      h_pedwvsampl_HO;
  TH1F*      h_pedvsampl0_HO;
  TH1F*      h_pedwvsampl0_HO;

  TH1F*      h_shape_Ahigh_HB0;
  TH1F*      h_shape0_Ahigh_HB0;
  TH1F*      h_shape_Alow_HB0;
  TH1F*      h_shape0_Alow_HB0;
  TH1F*      h_shape_Ahigh_HB1;
  TH1F*      h_shape0_Ahigh_HB1;
  TH1F*      h_shape_Alow_HB1;
  TH1F*      h_shape0_Alow_HB1;
  TH1F*      h_shape_Ahigh_HB2;
  TH1F*      h_shape0_Ahigh_HB2;
  TH1F*      h_shape_Alow_HB2;
  TH1F*      h_shape0_Alow_HB2;
  TH1F*      h_shape_Ahigh_HB3;
  TH1F*      h_shape0_Ahigh_HB3;
  TH1F*      h_shape_Alow_HB3;
  TH1F*      h_shape0_Alow_HB3;

  TH1F*  h_shape_bad_channels_HB;
  TH1F*  h_shape0_bad_channels_HB;
  TH1F*  h_shape_good_channels_HB;
  TH1F*  h_shape0_good_channels_HB;
  TH1F*  h_shape_bad_channels_HE;
  TH1F*  h_shape0_bad_channels_HE;
  TH1F*  h_shape_good_channels_HE;
  TH1F*  h_shape0_good_channels_HE;
  TH1F*  h_shape_bad_channels_HF;
  TH1F*  h_shape0_bad_channels_HF;
  TH1F*  h_shape_good_channels_HF;
  TH1F*  h_shape0_good_channels_HF;
  TH1F*  h_shape_bad_channels_HO;
  TH1F*  h_shape0_bad_channels_HO;
  TH1F*  h_shape_good_channels_HO;
  TH1F*  h_shape0_good_channels_HO;
 
  TH1F*  h_sumamplitude_depth1_HB;
  TH1F*  h_sumamplitude_depth2_HB;
  TH1F*  h_sumamplitude_depth1_HE;
  TH1F*  h_sumamplitude_depth2_HE;
  TH1F*  h_sumamplitude_depth3_HE;
  TH1F*  h_sumamplitude_depth1_HF;
  TH1F*  h_sumamplitude_depth2_HF;
  TH1F*  h_sumamplitude_depth4_HO;

  TH1F*  h_sumamplitude_depth1_HB0;
  TH1F*  h_sumamplitude_depth2_HB0;
  TH1F*  h_sumamplitude_depth1_HE0;
  TH1F*  h_sumamplitude_depth2_HE0;
  TH1F*  h_sumamplitude_depth3_HE0;
  TH1F*  h_sumamplitude_depth1_HF0;
  TH1F*  h_sumamplitude_depth2_HF0;
  TH1F*  h_sumamplitude_depth4_HO0;

  TH1F*  h_sumamplitude_depth1_HB1;
  TH1F*  h_sumamplitude_depth2_HB1;
  TH1F*  h_sumamplitude_depth1_HE1;
  TH1F*  h_sumamplitude_depth2_HE1;
  TH1F*  h_sumamplitude_depth3_HE1;
  TH1F*  h_sumamplitude_depth1_HF1;
  TH1F*  h_sumamplitude_depth2_HF1;
  TH1F*  h_sumamplitude_depth4_HO1;

    TH1F*      h_ADC_HB;
    TH1F*      h_ADC_HE;
    TH1F*      h_ADC_HF;
    TH1F*      h_ADC_HO;

    TH1F*      h_ADC_HBdepth1;
    TH1F*      h_ADC_HBdepth2;
    TH1F*      h_ADC_HEdepth1;
    TH1F*      h_ADC_HEdepth2;
    TH1F*      h_ADC_HEdepth3;
    TH1F*      h_ADC_HFdepth1;
    TH1F*      h_ADC_HFdepth2;
    TH1F*      h_ADC_HOdepth4;


    TH1F*  h_ADC_HBdepth1_TS0;
    TH1F*  h_ADC_HBdepth1_TS1;
    TH1F*  h_ADC_HBdepth1_TS2;
    TH1F*  h_ADC_HBdepth1_TS3;
    TH1F*  h_ADC_HBdepth1_TS4;
    TH1F*  h_ADC_HBdepth1_TS5;
    TH1F*  h_ADC_HBdepth1_TS6;
    TH1F*  h_ADC_HBdepth1_TS7;
    TH1F*  h_ADC_HBdepth1_TS8;
    TH1F*  h_ADC_HBdepth1_TS9;
    TH1F*  h_ADC_HBdepth2_TS0;
    TH1F*  h_ADC_HBdepth2_TS1;
    TH1F*  h_ADC_HBdepth2_TS2;
    TH1F*  h_ADC_HBdepth2_TS3;
    TH1F*  h_ADC_HBdepth2_TS4;
    TH1F*  h_ADC_HBdepth2_TS5;
    TH1F*  h_ADC_HBdepth2_TS6;
    TH1F*  h_ADC_HBdepth2_TS7;
    TH1F*  h_ADC_HBdepth2_TS8;
    TH1F*  h_ADC_HBdepth2_TS9;
    
    TH1F*  h_ADC_HEdepth1_TS0;
    TH1F*  h_ADC_HEdepth1_TS1;
    TH1F*  h_ADC_HEdepth1_TS2;
    TH1F*  h_ADC_HEdepth1_TS3;
    TH1F*  h_ADC_HEdepth1_TS4;
    TH1F*  h_ADC_HEdepth1_TS5;
    TH1F*  h_ADC_HEdepth1_TS6;
    TH1F*  h_ADC_HEdepth1_TS7;
    TH1F*  h_ADC_HEdepth1_TS8;
    TH1F*  h_ADC_HEdepth1_TS9;
    TH1F*  h_ADC_HEdepth2_TS0;
    TH1F*  h_ADC_HEdepth2_TS1;
    TH1F*  h_ADC_HEdepth2_TS2;
    TH1F*  h_ADC_HEdepth2_TS3;
    TH1F*  h_ADC_HEdepth2_TS4;
    TH1F*  h_ADC_HEdepth2_TS5;
    TH1F*  h_ADC_HEdepth2_TS6;
    TH1F*  h_ADC_HEdepth2_TS7;
    TH1F*  h_ADC_HEdepth2_TS8;
    TH1F*  h_ADC_HEdepth2_TS9;
    TH1F*  h_ADC_HEdepth3_TS0;
    TH1F*  h_ADC_HEdepth3_TS1;
    TH1F*  h_ADC_HEdepth3_TS2;
    TH1F*  h_ADC_HEdepth3_TS3;
    TH1F*  h_ADC_HEdepth3_TS4;
    TH1F*  h_ADC_HEdepth3_TS5;
    TH1F*  h_ADC_HEdepth3_TS6;
    TH1F*  h_ADC_HEdepth3_TS7;
    TH1F*  h_ADC_HEdepth3_TS8;
    TH1F*  h_ADC_HEdepth3_TS9;
    
    TH1F*  h_ADC_HFdepth1_TS0;
    TH1F*  h_ADC_HFdepth1_TS1;
    TH1F*  h_ADC_HFdepth1_TS2;
    TH1F*  h_ADC_HFdepth1_TS3;
    TH1F*  h_ADC_HFdepth1_TS4;
    TH1F*  h_ADC_HFdepth1_TS5;
    TH1F*  h_ADC_HFdepth1_TS6;
    TH1F*  h_ADC_HFdepth1_TS7;
    TH1F*  h_ADC_HFdepth1_TS8;
    TH1F*  h_ADC_HFdepth1_TS9;
    TH1F*  h_ADC_HFdepth2_TS0;
    TH1F*  h_ADC_HFdepth2_TS1;
    TH1F*  h_ADC_HFdepth2_TS2;
    TH1F*  h_ADC_HFdepth2_TS3;
    TH1F*  h_ADC_HFdepth2_TS4;
    TH1F*  h_ADC_HFdepth2_TS5;
    TH1F*  h_ADC_HFdepth2_TS6;
    TH1F*  h_ADC_HFdepth2_TS7;
    TH1F*  h_ADC_HFdepth2_TS8;
    TH1F*  h_ADC_HFdepth2_TS9;
    
    TH1F*  h_ADC_HOdepth4_TS0;
    TH1F*  h_ADC_HOdepth4_TS1;
    TH1F*  h_ADC_HOdepth4_TS2;
    TH1F*  h_ADC_HOdepth4_TS3;
    TH1F*  h_ADC_HOdepth4_TS4;
    TH1F*  h_ADC_HOdepth4_TS5;
    TH1F*  h_ADC_HOdepth4_TS6;
    TH1F*  h_ADC_HOdepth4_TS7;
    TH1F*  h_ADC_HOdepth4_TS8;
    TH1F*  h_ADC_HOdepth4_TS9;

  TH2F* h_mapDepth1Ped0_HB;
  TH2F* h_mapDepth1Ped1_HB;
  TH2F* h_mapDepth1Ped2_HB;
  TH2F* h_mapDepth1Ped3_HB;
  TH2F* h_mapDepth1Pedw0_HB;
  TH2F* h_mapDepth1Pedw1_HB;
  TH2F* h_mapDepth1Pedw2_HB;
  TH2F* h_mapDepth1Pedw3_HB;
  TH2F* h_mapDepth2Ped0_HB;
  TH2F* h_mapDepth2Ped1_HB;
  TH2F* h_mapDepth2Ped2_HB;
  TH2F* h_mapDepth2Ped3_HB;
  TH2F* h_mapDepth2Pedw0_HB;
  TH2F* h_mapDepth2Pedw1_HB;
  TH2F* h_mapDepth2Pedw2_HB;
  TH2F* h_mapDepth2Pedw3_HB;
  
  TH2F* h_mapDepth1Ped0_HE;
  TH2F* h_mapDepth1Ped1_HE;
  TH2F* h_mapDepth1Ped2_HE;
  TH2F* h_mapDepth1Ped3_HE;
  TH2F* h_mapDepth1Pedw0_HE;
  TH2F* h_mapDepth1Pedw1_HE;
  TH2F* h_mapDepth1Pedw2_HE;
  TH2F* h_mapDepth1Pedw3_HE;
  TH2F* h_mapDepth2Ped0_HE;
  TH2F* h_mapDepth2Ped1_HE;
  TH2F* h_mapDepth2Ped2_HE;
  TH2F* h_mapDepth2Ped3_HE;
  TH2F* h_mapDepth2Pedw0_HE;
  TH2F* h_mapDepth2Pedw1_HE;
  TH2F* h_mapDepth2Pedw2_HE;
  TH2F* h_mapDepth2Pedw3_HE;
  TH2F* h_mapDepth3Ped0_HE;
  TH2F* h_mapDepth3Ped1_HE;
  TH2F* h_mapDepth3Ped2_HE;
  TH2F* h_mapDepth3Ped3_HE;
  TH2F* h_mapDepth3Pedw0_HE;
  TH2F* h_mapDepth3Pedw1_HE;
  TH2F* h_mapDepth3Pedw2_HE;
  TH2F* h_mapDepth3Pedw3_HE;
  
  TH2F* h_mapDepth1Ped0_HF;
  TH2F* h_mapDepth1Ped1_HF;
  TH2F* h_mapDepth1Ped2_HF;
  TH2F* h_mapDepth1Ped3_HF;
  TH2F* h_mapDepth1Pedw0_HF;
  TH2F* h_mapDepth1Pedw1_HF;
  TH2F* h_mapDepth1Pedw2_HF;
  TH2F* h_mapDepth1Pedw3_HF;
  TH2F* h_mapDepth2Ped0_HF;
  TH2F* h_mapDepth2Ped1_HF;
  TH2F* h_mapDepth2Ped2_HF;
  TH2F* h_mapDepth2Ped3_HF;
  TH2F* h_mapDepth2Pedw0_HF;
  TH2F* h_mapDepth2Pedw1_HF;
  TH2F* h_mapDepth2Pedw2_HF;
  TH2F* h_mapDepth2Pedw3_HF;
  
  TH2F* h_mapDepth4Ped0_HO;
  TH2F* h_mapDepth4Ped1_HO;
  TH2F* h_mapDepth4Ped2_HO;
  TH2F* h_mapDepth4Ped3_HO;
  TH2F* h_mapDepth4Pedw0_HO;
  TH2F* h_mapDepth4Pedw1_HO;
  TH2F* h_mapDepth4Pedw2_HO;
  TH2F* h_mapDepth4Pedw3_HO;
  
  TH2F* h_mapGetRMSOverNormalizedSignal_HB;
  TH2F* h_mapGetRMSOverNormalizedSignal0_HB;
  TH2F* h_mapGetRMSOverNormalizedSignal_HE;
  TH2F* h_mapGetRMSOverNormalizedSignal0_HE;
  TH2F* h_mapGetRMSOverNormalizedSignal_HF;
  TH2F* h_mapGetRMSOverNormalizedSignal0_HF;
  TH2F* h_mapGetRMSOverNormalizedSignal_HO;
  TH2F* h_mapGetRMSOverNormalizedSignal0_HO;



  TH2F* h_2D0sumErrorBLS1;
  TH2F* h_2DsumErrorBLS1;
  TH1F*  h_sumErrorBLS1;
  TH1F* h_sumErrorBperLS1 ;
  TH1F* h_sum0ErrorBperLS1;
  TH2F* h_2D0sumErrorBLS2;
  TH2F* h_2DsumErrorBLS2;
  TH1F*  h_sumErrorBLS2;
  TH1F* h_sumErrorBperLS2 ;
  TH1F* h_sum0ErrorBperLS2;
  
  TH2F* h_2D0sumErrorBLS3;
  TH2F* h_2DsumErrorBLS3;
  TH1F*  h_sumErrorBLS3;
  TH1F* h_sumErrorBperLS3 ;
  TH1F* h_sum0ErrorBperLS3;
  TH2F* h_2D0sumErrorBLS4;
  TH2F* h_2DsumErrorBLS4;
  TH1F*  h_sumErrorBLS4;
  TH1F* h_sumErrorBperLS4 ;
  TH1F* h_sum0ErrorBperLS4;
  TH2F* h_2D0sumErrorBLS5;
  TH2F* h_2DsumErrorBLS5;
  TH1F*  h_sumErrorBLS5;
  TH1F* h_sumErrorBperLS5 ;
  TH1F* h_sum0ErrorBperLS5;
  
  TH2F* h_2D0sumErrorBLS6;
  TH2F* h_2DsumErrorBLS6;
  TH1F*  h_sumErrorBLS6;
  TH1F* h_sumErrorBperLS6 ;
  TH1F* h_sum0ErrorBperLS6;
  TH2F* h_2D0sumErrorBLS7;
  TH2F* h_2DsumErrorBLS7;
  TH1F*  h_sumErrorBLS7;
  TH1F* h_sumErrorBperLS7 ;
  TH1F* h_sum0ErrorBperLS7;
  
  TH2F* h_2D0sumErrorBLS8;
  TH2F* h_2DsumErrorBLS8;
  TH1F*  h_sumErrorBLS8;
  TH1F* h_sumErrorBperLS8 ;
  TH1F* h_sum0ErrorBperLS8;
  
  TH1F* h_averSIGNALoccupancy_HB;
  TH1F* h_averSIGNALoccupancy_HE;
  TH1F* h_averSIGNALoccupancy_HF;
  TH1F* h_averSIGNALoccupancy_HO;

  TH1F*   h_averSIGNALsumamplitude_HB;
  TH1F*   h_averSIGNALsumamplitude_HE;
  TH1F*   h_averSIGNALsumamplitude_HF;
  TH1F*   h_averSIGNALsumamplitude_HO;

  TH1F* h_averNOSIGNALoccupancy_HB;
  TH1F* h_averNOSIGNALoccupancy_HE;
  TH1F* h_averNOSIGNALoccupancy_HF;
  TH1F* h_averNOSIGNALoccupancy_HO;

  TH1F*   h_averNOSIGNALsumamplitude_HB;
  TH1F*   h_averNOSIGNALsumamplitude_HE;
  TH1F*   h_averNOSIGNALsumamplitude_HF;
  TH1F*   h_averNOSIGNALsumamplitude_HO;

  TH1F* h_maxxSUMAmpl_HB;
  TH1F* h_maxxSUMAmpl_HE;
  TH1F* h_maxxSUMAmpl_HF;
  TH1F* h_maxxSUMAmpl_HO;

  TH1F* h_maxxOCCUP_HB;
  TH1F* h_maxxOCCUP_HE;
  TH1F* h_maxxOCCUP_HF;
  TH1F* h_maxxOCCUP_HO;

  TH1F* h_sumamplitudechannel_HB;
  TH1F* h_sumamplitudechannel_HE;
  TH1F* h_sumamplitudechannel_HF;
  TH1F* h_sumamplitudechannel_HO;

  TH1F* h_eventamplitude_HB;
  TH1F* h_eventamplitude_HE;
  TH1F* h_eventamplitude_HF;
  TH1F* h_eventamplitude_HO;

  TH1F* h_eventoccupancy_HB;
  TH1F* h_eventoccupancy_HE;
  TH1F* h_eventoccupancy_HF;
  TH1F* h_eventoccupancy_HO;

  TH2F*    h_2DAtaildepth1_HB;
  TH2F*    h_2D0Ataildepth1_HB;
  TH2F*    h_2DAtaildepth2_HB;
  TH2F*    h_2D0Ataildepth2_HB;



  /////////////////////////////////////////////
  // for ROOT
  // Readouts
  
  float hb15[2][2][72][10];
  float hb16[2][2][72][10];
  float he16[2][1][72][10];
  float hb[29][1][72][10];
  float he[26][2][72][10]; 
  
  // Calibration channels (corresponded to 1 RBX,counted 1-35).
  
  float hb_calibration0[40][15];
  float hb_calibration1[40][15];
  float he_calibration0[40][15];
  float he_calibration1[40][15];
  float hf_calibration0[40][15];
  float hf_calibration1[40][15];
  float ho_calibration0[40][15];
  float ho_calibration1[40][15];
  
  int myCalEta[5][40];
  int myCalPhi[5][40];
  
  int calibcapiderror[4][82][72];
  float calibt[4][82][72];
  double caliba[4][82][72];
  double calibw[4][82][72];

  double calib0[4][82][72];
  double signal[4][82][72];
  double calib3[4][82][72];
  double signal3[4][82][72];
  double calib2[4][82][72];
  int badchannels[4][4][82][72];
  double sumEstimator0[4][4][82][72];
  double sumEstimator1[4][4][82][72];
  double sumEstimator2[4][4][82][72];
  double sumEstimator3[4][4][82][72];
  double sumEstimator4[4][4][82][72];
  double sumEstimator5[4][4][82][72];
  double sumEstimator6[4][4][82][72];
  double sum0Estimator[4][4][82][72];

  double amplitudechannel[4][4][82][72];

  int Nevent;
  int Run;
  int run0 ;
  //int run1 ;
  int runcounter ;
  int eventcounter;

  int lumi;
  int ls0 ;
  int lscounter ;
  int lscounterM1 ;
  int lscounter10 ;
  int nevcounter ;
  int lscounterrun ;
  int lscounterrun10 ;
  int nevcounter0 ;
  int nevcounter00 ;

  double averSIGNALoccupancy_HB;
  double averSIGNALoccupancy_HE;
  double averSIGNALoccupancy_HF;
  double averSIGNALoccupancy_HO;

  double averSIGNALsumamplitude_HB;
  double averSIGNALsumamplitude_HE;
  double averSIGNALsumamplitude_HF;
  double averSIGNALsumamplitude_HO;

  double averNOSIGNALoccupancy_HB;
  double averNOSIGNALoccupancy_HE;
  double averNOSIGNALoccupancy_HF;
  double averNOSIGNALoccupancy_HO;

  double averNOSIGNALsumamplitude_HB;
  double averNOSIGNALsumamplitude_HE;
  double averNOSIGNALsumamplitude_HF;
  double averNOSIGNALsumamplitude_HO;

  double maxxSUM1;
  double maxxSUM2;
  double maxxSUM3;
  double maxxSUM4;

  double maxxOCCUP1;
  double maxxOCCUP2;
  double maxxOCCUP3;
  double maxxOCCUP4;



  TTree*    myTree;
  TFile*    hOutputFile;
  ofstream MAPfile;
  ///////////////////////////////////////// 
  // Get RBX number from 1-35 for Calibration box
  int getRBX(int& i, int& j, int& k);

  void fillDigiErrors(HBHEDigiCollection::const_iterator& digiItr);
  void fillDigiErrorsHF(HFDigiCollection::const_iterator& digiItr);
  void fillDigiErrorsHO(HODigiCollection::const_iterator& digiItr);

  void fillDigiAmplitude(HBHEDigiCollection::const_iterator& digiItr);
  void fillDigiAmplitudeHF(HFDigiCollection::const_iterator& digiItr);
  void fillDigiAmplitudeHO(HODigiCollection::const_iterator& digiItr);

  int local_event;
  int eta,phi,depth,nTS,cap_num;
  int numOfTS;
  float TS_data[10];
  float TS_cal[10];
  int numOfLaserEv;

  int testmetka;
  
};


VeRawAnalyzer::VeRawAnalyzer(const edm::ParameterSet& iConfig)
{
  verbosity     =  iConfig.getUntrackedParameter<int>("Verbosity");
  MAPcreation   =  iConfig.getUntrackedParameter<int>("MapCreation");
  //
  recordNtuples_=iConfig.getUntrackedParameter<bool>("recordNtuples");
  maxNeventsInNtuple_ = iConfig.getParameter<int>("maxNeventsInNtuple");
  //
  //

//  tok_calib_ = consumes<HcalCalibDigiCollection>(edm::InputTag("hcalDigis"));
  tok_calib_ = consumes<HcalCalibDigiCollection>(iConfig.getParameter<edm::InputTag>("hcalCalibDigiCollectionTag"));  //
  
  tok_hbhe_ = consumes<HBHEDigiCollection>(iConfig.getParameter<edm::InputTag>("hbheDigiCollectionTag"));
  tok_ho_ = consumes<HODigiCollection>(iConfig.getParameter<edm::InputTag>("hoDigiCollectionTag"));
  tok_hf_ = consumes<HFDigiCollection>(iConfig.getParameter<edm::InputTag>("hfDigiCollectionTag"));  //
  //

  recordHistoes_=iConfig.getUntrackedParameter<bool>("recordHistoes");
  studyRunDependenceHist_=iConfig.getUntrackedParameter<bool>("studyRunDependenceHist"); 
  studyCapIDErrorsHist_=iConfig.getUntrackedParameter<bool>("studyCapIDErrorsHist"); 
  studyRMSshapeHist_=iConfig.getUntrackedParameter<bool>("studyRMSshapeHist"); 
  studyRatioShapeHist_=iConfig.getUntrackedParameter<bool>("studyRatioShapeHist"); 
  studyTSmaxShapeHist_=iConfig.getUntrackedParameter<bool>("studyTSmaxShapeHist"); 
  studyTSmeanShapeHist_=iConfig.getUntrackedParameter<bool>("studyTSmeanShapeHist"); 
  studyDiffAmplHist_=iConfig.getUntrackedParameter<bool>("studyDiffAmplHist"); 
  studyCalibCellsHist_=iConfig.getUntrackedParameter<bool>("studyCalibCellsHist"); 
  studyADCAmplHist_=iConfig.getUntrackedParameter<bool>("studyADCAmplHist");
  studyPedestalsHist_=iConfig.getUntrackedParameter<bool>("studyPedestalsHist");
  studyPedestalCorrelations_=iConfig.getUntrackedParameter<bool>("studyPedestalCorrelations");
  useADCmassive_=iConfig.getUntrackedParameter<bool>("useADCmassive");
  useADCfC_=iConfig.getUntrackedParameter<bool>("useADCfC");
  useADCcounts_=iConfig.getUntrackedParameter<bool>("useADCcounts");
  usePedestalSubtraction_=iConfig.getUntrackedParameter<bool>("usePedestalSubtraction");
  usecontinuousnumbering_=iConfig.getUntrackedParameter<bool>("usecontinuousnumbering");
  // 
  flagabortgaprejected_ = iConfig.getParameter<int>("flagabortgaprejected");//
  bcnrejectedlow_ = iConfig.getParameter<int>("bcnrejectedlow");//
  bcnrejectedhigh_ = iConfig.getParameter<int>("bcnrejectedhigh");//
  //
  ratioHBMin_      = iConfig.getParameter<double>("ratioHBMin");//
  ratioHBMax_      = iConfig.getParameter<double>("ratioHBMax");//
  ratioHEMin_      = iConfig.getParameter<double>("ratioHEMin");//
  ratioHEMax_      = iConfig.getParameter<double>("ratioHEMax");//
  ratioHFMin_      = iConfig.getParameter<double>("ratioHFMin");//
  ratioHFMax_      = iConfig.getParameter<double>("ratioHFMax");//
  ratioHOMin_      = iConfig.getParameter<double>("ratioHOMin");//
  ratioHOMax_      = iConfig.getParameter<double>("ratioHOMax");//
  //
  flagtodefinebadchannel_ = iConfig.getParameter<int>("flagtodefinebadchannel");//
  howmanybinsonplots_ = iConfig.getParameter<int>("howmanybinsonplots");//
  splashesUpperLimit_ = iConfig.getParameter<int>("splashesUpperLimit");//

  flagtoaskrunsorls_ = iConfig.getParameter<int>("flagtoaskrunsorls");//
  flagestimatornormalization_ = iConfig.getParameter<int>("flagestimatornormalization");//
  flagcpuoptimization_ = iConfig.getParameter<int>("flagcpuoptimization");//

//  nbadchannels1_      = iConfig.getParameter<int>("nbadchannels1");//
//  nbadchannels2_      = iConfig.getParameter<int>("nbadchannels2");//
//  nbadchannels3_      = iConfig.getParameter<int>("nbadchannels3");//
  lsdep_cut1_peak_HBdepth1_ = iConfig.getParameter<int>("lsdep_cut1_peak_HBdepth1");
  lsdep_cut1_peak_HBdepth2_ = iConfig.getParameter<int>("lsdep_cut1_peak_HBdepth2");
  lsdep_cut1_peak_HEdepth1_ = iConfig.getParameter<int>("lsdep_cut1_peak_HEdepth1");
  lsdep_cut1_peak_HEdepth2_ = iConfig.getParameter<int>("lsdep_cut1_peak_HEdepth2");
  lsdep_cut1_peak_HEdepth3_ = iConfig.getParameter<int>("lsdep_cut1_peak_HEdepth3");
  lsdep_cut1_peak_HFdepth1_ = iConfig.getParameter<int>("lsdep_cut1_peak_HFdepth1");
  lsdep_cut1_peak_HFdepth2_ = iConfig.getParameter<int>("lsdep_cut1_peak_HFdepth2");
  lsdep_cut1_peak_HOdepth4_ = iConfig.getParameter<int>("lsdep_cut1_peak_HOdepth4");
  //
  lsdep_cut3_max_HBdepth1_ = iConfig.getParameter<int>("lsdep_cut3_max_HBdepth1");
  lsdep_cut3_max_HBdepth2_ = iConfig.getParameter<int>("lsdep_cut3_max_HBdepth2");
  lsdep_cut3_max_HEdepth1_ = iConfig.getParameter<int>("lsdep_cut3_max_HEdepth1");
  lsdep_cut3_max_HEdepth2_ = iConfig.getParameter<int>("lsdep_cut3_max_HEdepth2");
  lsdep_cut3_max_HEdepth3_ = iConfig.getParameter<int>("lsdep_cut3_max_HEdepth3");
  lsdep_cut3_max_HFdepth1_ = iConfig.getParameter<int>("lsdep_cut3_max_HFdepth1");
  lsdep_cut3_max_HFdepth2_ = iConfig.getParameter<int>("lsdep_cut3_max_HFdepth2");
  lsdep_cut3_max_HOdepth4_ = iConfig.getParameter<int>("lsdep_cut3_max_HOdepth4");

  lsdep_estimator1_HBdepth1_ = iConfig.getParameter<double>("lsdep_estimator1_HBdepth1");
  lsdep_estimator1_HBdepth2_ = iConfig.getParameter<double>("lsdep_estimator1_HBdepth2");
  lsdep_estimator1_HEdepth1_ = iConfig.getParameter<double>("lsdep_estimator1_HEdepth1");
  lsdep_estimator1_HEdepth2_ = iConfig.getParameter<double>("lsdep_estimator1_HEdepth2");
  lsdep_estimator1_HEdepth3_ = iConfig.getParameter<double>("lsdep_estimator1_HEdepth3");
  lsdep_estimator1_HFdepth1_ = iConfig.getParameter<double>("lsdep_estimator1_HFdepth1");
  lsdep_estimator1_HFdepth2_ = iConfig.getParameter<double>("lsdep_estimator1_HFdepth2");
  lsdep_estimator1_HOdepth4_ = iConfig.getParameter<double>("lsdep_estimator1_HOdepth4");

  lsdep_estimator2_HBdepth1_ = iConfig.getParameter<double>("lsdep_estimator2_HBdepth1");
  lsdep_estimator2_HBdepth2_ = iConfig.getParameter<double>("lsdep_estimator2_HBdepth2");
  lsdep_estimator2_HEdepth1_ = iConfig.getParameter<double>("lsdep_estimator2_HEdepth1");
  lsdep_estimator2_HEdepth2_ = iConfig.getParameter<double>("lsdep_estimator2_HEdepth2");
  lsdep_estimator2_HEdepth3_ = iConfig.getParameter<double>("lsdep_estimator2_HEdepth3");
  lsdep_estimator2_HFdepth1_ = iConfig.getParameter<double>("lsdep_estimator2_HFdepth1");
  lsdep_estimator2_HFdepth2_ = iConfig.getParameter<double>("lsdep_estimator2_HFdepth2");
  lsdep_estimator2_HOdepth4_ = iConfig.getParameter<double>("lsdep_estimator2_HOdepth4");

  lsdep_estimator3_HBdepth1_ = iConfig.getParameter<double>("lsdep_estimator3_HBdepth1");
  lsdep_estimator3_HBdepth2_ = iConfig.getParameter<double>("lsdep_estimator3_HBdepth2");
  lsdep_estimator3_HEdepth1_ = iConfig.getParameter<double>("lsdep_estimator3_HEdepth1");
  lsdep_estimator3_HEdepth2_ = iConfig.getParameter<double>("lsdep_estimator3_HEdepth2");
  lsdep_estimator3_HEdepth3_ = iConfig.getParameter<double>("lsdep_estimator3_HEdepth3");
  lsdep_estimator3_HFdepth1_ = iConfig.getParameter<double>("lsdep_estimator3_HFdepth1");
  lsdep_estimator3_HFdepth2_ = iConfig.getParameter<double>("lsdep_estimator3_HFdepth2");
  lsdep_estimator3_HOdepth4_ = iConfig.getParameter<double>("lsdep_estimator3_HOdepth4");

  lsdep_estimator4_HBdepth1_ = iConfig.getParameter<double>("lsdep_estimator4_HBdepth1");
  lsdep_estimator4_HBdepth2_ = iConfig.getParameter<double>("lsdep_estimator4_HBdepth2");
  lsdep_estimator4_HEdepth1_ = iConfig.getParameter<double>("lsdep_estimator4_HEdepth1");
  lsdep_estimator4_HEdepth2_ = iConfig.getParameter<double>("lsdep_estimator4_HEdepth2");
  lsdep_estimator4_HEdepth3_ = iConfig.getParameter<double>("lsdep_estimator4_HEdepth3");
  lsdep_estimator4_HFdepth1_ = iConfig.getParameter<double>("lsdep_estimator4_HFdepth1");
  lsdep_estimator4_HFdepth2_ = iConfig.getParameter<double>("lsdep_estimator4_HFdepth2");
  lsdep_estimator4_HOdepth4_ = iConfig.getParameter<double>("lsdep_estimator4_HOdepth4");

  lsdep_estimator5_HBdepth1_ = iConfig.getParameter<double>("lsdep_estimator5_HBdepth1");
  lsdep_estimator5_HBdepth2_ = iConfig.getParameter<double>("lsdep_estimator5_HBdepth2");
  lsdep_estimator5_HEdepth1_ = iConfig.getParameter<double>("lsdep_estimator5_HEdepth1");
  lsdep_estimator5_HEdepth2_ = iConfig.getParameter<double>("lsdep_estimator5_HEdepth2");
  lsdep_estimator5_HEdepth3_ = iConfig.getParameter<double>("lsdep_estimator5_HEdepth3");
  lsdep_estimator5_HFdepth1_ = iConfig.getParameter<double>("lsdep_estimator5_HFdepth1");
  lsdep_estimator5_HFdepth2_ = iConfig.getParameter<double>("lsdep_estimator5_HFdepth2");
  lsdep_estimator5_HOdepth4_ = iConfig.getParameter<double>("lsdep_estimator5_HOdepth4");
  forallestimators_amplitude_bigger_=iConfig.getParameter<double>("forallestimators_amplitude_bigger");

  //
  rmsHBMin_      = iConfig.getParameter<double>("rmsHBMin");//
  rmsHBMax_      = iConfig.getParameter<double>("rmsHBMax");//
  rmsHEMin_      = iConfig.getParameter<double>("rmsHEMin");//
  rmsHEMax_      = iConfig.getParameter<double>("rmsHEMax");//
  rmsHFMin_      = iConfig.getParameter<double>("rmsHFMin");//
  rmsHFMax_      = iConfig.getParameter<double>("rmsHFMax");//
  rmsHOMin_      = iConfig.getParameter<double>("rmsHOMin");//
  rmsHOMax_      = iConfig.getParameter<double>("rmsHOMax");//
  //
  ADCAmplHBMin_ = iConfig.getParameter<double>("ADCAmplHBMin");//
  ADCAmplHEMin_ = iConfig.getParameter<double>("ADCAmplHEMin");//
  ADCAmplHOMin_ = iConfig.getParameter<double>("ADCAmplHOMin");//
  ADCAmplHFMin_ = iConfig.getParameter<double>("ADCAmplHFMin");//
  ADCAmplHBMax_ = iConfig.getParameter<double>("ADCAmplHBMax");//
  ADCAmplHEMax_ = iConfig.getParameter<double>("ADCAmplHEMax");//
  ADCAmplHOMax_ = iConfig.getParameter<double>("ADCAmplHOMax");//
  ADCAmplHFMax_ = iConfig.getParameter<double>("ADCAmplHFMax");//
  //
  pedestalwHBMax_ = iConfig.getParameter<double>("pedestalwHBMax");//
  pedestalwHEMax_ = iConfig.getParameter<double>("pedestalwHEMax");//
  pedestalwHFMax_ = iConfig.getParameter<double>("pedestalwHFMax");//
  pedestalwHOMax_ = iConfig.getParameter<double>("pedestalwHOMax");//
  //
  pedestalHBMax_ = iConfig.getParameter<double>("pedestalHBMax");//
  pedestalHEMax_ = iConfig.getParameter<double>("pedestalHEMax");//
  pedestalHFMax_ = iConfig.getParameter<double>("pedestalHFMax");//
  pedestalHOMax_ = iConfig.getParameter<double>("pedestalHOMax");//
  //
  calibrADCHBMin_ = iConfig.getParameter<double>("calibrADCHBMin");//
  calibrADCHEMin_ = iConfig.getParameter<double>("calibrADCHEMin");//
  calibrADCHOMin_ = iConfig.getParameter<double>("calibrADCHOMin");//
  calibrADCHFMin_ = iConfig.getParameter<double>("calibrADCHFMin");//
  calibrADCHBMax_ = iConfig.getParameter<double>("calibrADCHBMax");//
  calibrADCHEMax_ = iConfig.getParameter<double>("calibrADCHEMax");//
  calibrADCHOMax_ = iConfig.getParameter<double>("calibrADCHOMax");//
  calibrADCHFMax_ = iConfig.getParameter<double>("calibrADCHFMax");//
  //
  calibrRatioHBMin_ = iConfig.getParameter<double>("calibrRatioHBMin");//
  calibrRatioHEMin_ = iConfig.getParameter<double>("calibrRatioHEMin");//
  calibrRatioHOMin_ = iConfig.getParameter<double>("calibrRatioHOMin");//
  calibrRatioHFMin_ = iConfig.getParameter<double>("calibrRatioHFMin");//
  calibrRatioHBMax_ = iConfig.getParameter<double>("calibrRatioHBMax");//
  calibrRatioHEMax_ = iConfig.getParameter<double>("calibrRatioHEMax");//
  calibrRatioHOMax_ = iConfig.getParameter<double>("calibrRatioHOMax");//
  calibrRatioHFMax_ = iConfig.getParameter<double>("calibrRatioHFMax");//
  //
  calibrTSmaxHBMin_ = iConfig.getParameter<double>("calibrTSmaxHBMin");//
  calibrTSmaxHEMin_ = iConfig.getParameter<double>("calibrTSmaxHEMin");//
  calibrTSmaxHOMin_ = iConfig.getParameter<double>("calibrTSmaxHOMin");//
  calibrTSmaxHFMin_ = iConfig.getParameter<double>("calibrTSmaxHFMin");//
  calibrTSmaxHBMax_ = iConfig.getParameter<double>("calibrTSmaxHBMax");//
  calibrTSmaxHEMax_ = iConfig.getParameter<double>("calibrTSmaxHEMax");//
  calibrTSmaxHOMax_ = iConfig.getParameter<double>("calibrTSmaxHOMax");//
  calibrTSmaxHFMax_ = iConfig.getParameter<double>("calibrTSmaxHFMax");//
  //
  calibrTSmeanHBMin_ = iConfig.getParameter<double>("calibrTSmeanHBMin");//
  calibrTSmeanHEMin_ = iConfig.getParameter<double>("calibrTSmeanHEMin");//
  calibrTSmeanHOMin_ = iConfig.getParameter<double>("calibrTSmeanHOMin");//
  calibrTSmeanHFMin_ = iConfig.getParameter<double>("calibrTSmeanHFMin");//
  calibrTSmeanHBMax_ = iConfig.getParameter<double>("calibrTSmeanHBMax");//
  calibrTSmeanHEMax_ = iConfig.getParameter<double>("calibrTSmeanHEMax");//
  calibrTSmeanHOMax_ = iConfig.getParameter<double>("calibrTSmeanHOMax");//
  calibrTSmeanHFMax_ = iConfig.getParameter<double>("calibrTSmeanHFMax");//
  //
  calibrWidthHBMin_ = iConfig.getParameter<double>("calibrWidthHBMin");//
  calibrWidthHEMin_ = iConfig.getParameter<double>("calibrWidthHEMin");//
  calibrWidthHOMin_ = iConfig.getParameter<double>("calibrWidthHOMin");//
  calibrWidthHFMin_ = iConfig.getParameter<double>("calibrWidthHFMin");//
  calibrWidthHBMax_ = iConfig.getParameter<double>("calibrWidthHBMax");//
  calibrWidthHEMax_ = iConfig.getParameter<double>("calibrWidthHEMax");//
  calibrWidthHOMax_ = iConfig.getParameter<double>("calibrWidthHOMax");//
  calibrWidthHFMax_ = iConfig.getParameter<double>("calibrWidthHFMax");//
  //
  fOutputFileName   = iConfig.getUntrackedParameter<std::string>("HistOutFile"); 
  MAPOutputFileName = iConfig.getUntrackedParameter<std::string>("MAPOutFile");
  // inputTag_ = iConfig.getUntrackedParameter<edm::InputTag>("DigiCollectionLabel");
  //
  TSpeakHBMin_      = iConfig.getParameter<double>("TSpeakHBMin");//
  TSpeakHBMax_      = iConfig.getParameter<double>("TSpeakHBMax");//
  TSpeakHEMin_      = iConfig.getParameter<double>("TSpeakHEMin");//
  TSpeakHEMax_      = iConfig.getParameter<double>("TSpeakHEMax");//
  TSpeakHFMin_      = iConfig.getParameter<double>("TSpeakHFMin");//
  TSpeakHFMax_      = iConfig.getParameter<double>("TSpeakHFMax");//
  TSpeakHOMin_      = iConfig.getParameter<double>("TSpeakHOMin");//
  TSpeakHOMax_      = iConfig.getParameter<double>("TSpeakHOMax");//
  
  TSmeanHBMin_      = iConfig.getParameter<double>("TSmeanHBMin");//
  TSmeanHBMax_      = iConfig.getParameter<double>("TSmeanHBMax");//
  TSmeanHEMin_      = iConfig.getParameter<double>("TSmeanHEMin");//
  TSmeanHEMax_      = iConfig.getParameter<double>("TSmeanHEMax");//
  TSmeanHFMin_      = iConfig.getParameter<double>("TSmeanHFMin");//
  TSmeanHFMax_      = iConfig.getParameter<double>("TSmeanHFMax");//
  TSmeanHOMin_      = iConfig.getParameter<double>("TSmeanHOMin");//
  TSmeanHOMax_      = iConfig.getParameter<double>("TSmeanHOMax");//
  
  std::cout<<" Look on parameters you booked:" << std::endl;   
  std::cout<<" recordNtuples_ = " <<recordNtuples_ << std::endl;  
  std::cout<<" maxNeventsInNtuple_ = " <<maxNeventsInNtuple_ << std::endl;  
  std::cout<<" recordHistoes_ = " <<recordHistoes_ << std::endl; 
  std::cout<<" studyRunDependenceHist_ = " <<studyRunDependenceHist_ << std::endl; 
  std::cout<<" studyCapIDErrorsHist_ = " <<studyCapIDErrorsHist_ << std::endl; 
  std::cout<<" studyRMSshapeHist_ = " <<studyRMSshapeHist_ << std::endl; 
  std::cout<<" studyRatioShapeHist_ = " <<studyRatioShapeHist_ << std::endl; 
  std::cout<<" studyTSmaxShapeHist_ = " <<studyTSmaxShapeHist_ << std::endl; 
  std::cout<<" studyTSmeanShapeHist_ = " <<studyTSmeanShapeHist_ << std::endl; 
  std::cout<<" studyDiffAmplHist_ = " <<studyDiffAmplHist_ << std::endl; 
  std::cout<<" studyCalibCellsHist_ = " <<studyCalibCellsHist_ << std::endl; 
  std::cout<<" studyADCAmplHist_ = " <<studyADCAmplHist_ << std::endl; 
  std::cout<<" studyPedestalsHist_ = " <<studyPedestalsHist_ << std::endl; 
  std::cout<<" studyPedestalCorrelations_ = " <<studyPedestalCorrelations_ << std::endl; 
  std::cout<<" useADCmassive_ = " <<useADCmassive_ << std::endl; 
  std::cout<<" useADCfC_ = " <<useADCfC_ << std::endl; 
  std::cout<<" useADCcounts_ = " <<useADCcounts_ << std::endl; 
  std::cout<<" usePedestalSubtraction_ = " <<usePedestalSubtraction_ << std::endl; 
  std::cout<<" usecontinuousnumbering_ = " <<usecontinuousnumbering_ << std::endl; 

  std::cout<<" ratioHBMin_ = " <<ratioHBMin_ << std::endl;   
  std::cout<<" ratioHBMax_ = " <<ratioHBMax_ << std::endl;   
  std::cout<<" ratioHEMin_ = " <<ratioHEMin_ << std::endl;   
  std::cout<<" ratioHEMax_ = " <<ratioHEMax_ << std::endl;   
  std::cout<<" ratioHFMin_ = " <<ratioHFMin_ << std::endl;   
  std::cout<<" ratioHFMax_ = " <<ratioHFMax_ << std::endl;   
  std::cout<<" ratioHOMin_ = " <<ratioHOMin_ << std::endl;   
  std::cout<<" ratioHOMax_ = " <<ratioHOMax_ << std::endl;   
  std::cout<<" flagtoaskrunsorls_ = " <<flagtoaskrunsorls_ << std::endl;
  std::cout<<" flagestimatornormalization_ = " <<flagestimatornormalization_ << std::endl;
  std::cout<<" flagcpuoptimization_ = " <<flagcpuoptimization_ << std::endl;
  std::cout<<" flagtodefinebadchannel_ = " <<flagtodefinebadchannel_ << std::endl;
  std::cout<<" howmanybinsonplots_ = " <<howmanybinsonplots_ << std::endl;
  std::cout<<" splashesUpperLimit_ = " <<splashesUpperLimit_ << std::endl;
  std::cout<<" flagabortgaprejected_ = " <<flagabortgaprejected_ << std::endl;
  std::cout<<" bcnrejectedlow_ = " <<bcnrejectedlow_ << std::endl;
  std::cout<<" bcnrejectedhigh_ = " <<bcnrejectedhigh_ << std::endl;
  //  std::cout<<" nbadchannels1_ = " <<nbadchannels1_ << std::endl;   
  //  std::cout<<" nbadchannels2_ = " <<nbadchannels2_ << std::endl;   
  //  std::cout<<" nbadchannels3_ = " <<nbadchannels3_ << std::endl;   
  std::cout<<" rmsHBMin_ = " <<rmsHBMin_ << std::endl;   
  std::cout<<" rmsHBMax_ = " <<rmsHBMax_ << std::endl;   
  std::cout<<" rmsHEMin_ = " <<rmsHEMin_ << std::endl;   
  std::cout<<" rmsHEMax_ = " <<rmsHEMax_ << std::endl;   
  std::cout<<" rmsHFMin_ = " <<rmsHFMin_ << std::endl;   
  std::cout<<" rmsHFMax_ = " <<rmsHFMax_ << std::endl;   
  std::cout<<" rmsHOMin_ = " <<rmsHOMin_ << std::endl;   
  std::cout<<" rmsHOMax_ = " <<rmsHOMax_ << std::endl;   

  std::cout<<" ADCAmplHBMin_ = " <<ADCAmplHBMin_ << std::endl;   
  std::cout<<" ADCAmplHEMin_ = " <<ADCAmplHEMin_ << std::endl;   
  std::cout<<" ADCAmplHOMin_ = " <<ADCAmplHOMin_ << std::endl;   
  std::cout<<" ADCAmplHFMin_ = " <<ADCAmplHFMin_ << std::endl;   
  std::cout<<" ADCAmplHBMax_ = " <<ADCAmplHBMax_ << std::endl;   
  std::cout<<" ADCAmplHEMax_ = " <<ADCAmplHEMax_ << std::endl;   
  std::cout<<" ADCAmplHOMax_ = " <<ADCAmplHOMax_ << std::endl;   
  std::cout<<" ADCAmplHFMax_ = " <<ADCAmplHFMax_ << std::endl;   

  std::cout<<" pedestalwHBMax_ = " <<pedestalwHBMax_ << std::endl;   
  std::cout<<" pedestalwHEMax_ = " <<pedestalwHEMax_ << std::endl;   
  std::cout<<" pedestalwHFMax_ = " <<pedestalwHFMax_ << std::endl;   
  std::cout<<" pedestalwHOMax_ = " <<pedestalwHOMax_ << std::endl;   

  std::cout<<" pedestalHBMax_ = " <<pedestalHBMax_ << std::endl;   
  std::cout<<" pedestalHEMax_ = " <<pedestalHEMax_ << std::endl;   
  std::cout<<" pedestalHFMax_ = " <<pedestalHFMax_ << std::endl;   
  std::cout<<" pedestalHOMax_ = " <<pedestalHOMax_ << std::endl;   

  std::cout<<" calibrADCHBMin_ = " <<calibrADCHBMin_ << std::endl;   
  std::cout<<" calibrADCHEMin_ = " <<calibrADCHEMin_ << std::endl;   
  std::cout<<" calibrADCHOMin_ = " <<calibrADCHOMin_ << std::endl;   
  std::cout<<" calibrADCHFMin_ = " <<calibrADCHFMin_ << std::endl;   
  std::cout<<" calibrADCHBMax_ = " <<calibrADCHBMax_ << std::endl;   
  std::cout<<" calibrADCHEMax_ = " <<calibrADCHEMax_ << std::endl;   
  std::cout<<" calibrADCHOMax_ = " <<calibrADCHOMax_ << std::endl;   
  std::cout<<" calibrADCHFMax_ = " <<calibrADCHFMax_ << std::endl;   

  std::cout<<" calibrRatioHBMin_ = " <<calibrRatioHBMin_ << std::endl;   
  std::cout<<" calibrRatioHEMin_ = " <<calibrRatioHEMin_ << std::endl;   
  std::cout<<" calibrRatioHOMin_ = " <<calibrRatioHOMin_ << std::endl;   
  std::cout<<" calibrRatioHFMin_ = " <<calibrRatioHFMin_ << std::endl;   
  std::cout<<" calibrRatioHBMax_ = " <<calibrRatioHBMax_ << std::endl;   
  std::cout<<" calibrRatioHEMax_ = " <<calibrRatioHEMax_ << std::endl;   
  std::cout<<" calibrRatioHOMax_ = " <<calibrRatioHOMax_ << std::endl;   
  std::cout<<" calibrRatioHFMax_ = " <<calibrRatioHFMax_ << std::endl;   
  std::cout<<" calibrTSmaxHBMin_ = " <<calibrTSmaxHBMin_ << std::endl;   
  std::cout<<" calibrTSmaxHEMin_ = " <<calibrTSmaxHEMin_ << std::endl;   
  std::cout<<" calibrTSmaxHOMin_ = " <<calibrTSmaxHOMin_ << std::endl;   
  std::cout<<" calibrTSmaxHFMin_ = " <<calibrTSmaxHFMin_ << std::endl;   
  std::cout<<" calibrTSmaxHBMax_ = " <<calibrTSmaxHBMax_ << std::endl;   
  std::cout<<" calibrTSmaxHEMax_ = " <<calibrTSmaxHEMax_ << std::endl;   
  std::cout<<" calibrTSmaxHOMax_ = " <<calibrTSmaxHOMax_ << std::endl;   
  std::cout<<" calibrTSmaxHFMax_ = " <<calibrTSmaxHFMax_ << std::endl;   
  std::cout<<" calibrTSmeanHBMin_ = " <<calibrTSmeanHBMin_ << std::endl;   
  std::cout<<" calibrTSmeanHEMin_ = " <<calibrTSmeanHEMin_ << std::endl;   
  std::cout<<" calibrTSmeanHOMin_ = " <<calibrTSmeanHOMin_ << std::endl;   
  std::cout<<" calibrTSmeanHFMin_ = " <<calibrTSmeanHFMin_ << std::endl;   
  std::cout<<" calibrTSmeanHBMax_ = " <<calibrTSmeanHBMax_ << std::endl;   
  std::cout<<" calibrTSmeanHEMax_ = " <<calibrTSmeanHEMax_ << std::endl;   
  std::cout<<" calibrTSmeanHOMax_ = " <<calibrTSmeanHOMax_ << std::endl;   
  std::cout<<" calibrTSmeanHFMax_ = " <<calibrTSmeanHFMax_ << std::endl;   
   std::cout<<" calibrWidthHBMin_ = " <<calibrWidthHBMin_ << std::endl;   
  std::cout<<" calibrWidthHEMin_ = " <<calibrWidthHEMin_ << std::endl;   
  std::cout<<" calibrWidthHOMin_ = " <<calibrWidthHOMin_ << std::endl;   
  std::cout<<" calibrWidthHFMin_ = " <<calibrWidthHFMin_ << std::endl;   
  std::cout<<" calibrWidthHBMax_ = " <<calibrWidthHBMax_ << std::endl;   
  std::cout<<" calibrWidthHEMax_ = " <<calibrWidthHEMax_ << std::endl;   
  std::cout<<" calibrWidthHOMax_ = " <<calibrWidthHOMax_ << std::endl;   
  std::cout<<" calibrWidthHFMax_ = " <<calibrWidthHFMax_ << std::endl;   
 std::cout<<" TSpeakHBMin_ = " <<TSpeakHBMin_ << std::endl;   
  std::cout<<" TSpeakHBMax_ = " <<TSpeakHBMax_ << std::endl;   
  std::cout<<" TSpeakHEMin_ = " <<TSpeakHEMin_ << std::endl;   
  std::cout<<" TSpeakHEMax_ = " <<TSpeakHEMax_ << std::endl;   
  std::cout<<" TSpeakHFMin_ = " <<TSpeakHFMin_ << std::endl;   
  std::cout<<" TSpeakHFMax_ = " <<TSpeakHFMax_ << std::endl;   
  std::cout<<" TSpeakHOMin_ = " <<TSpeakHOMin_ << std::endl;   
  std::cout<<" TSpeakHOMax_ = " <<TSpeakHOMax_ << std::endl;   
  
  std::cout<<" TSmeanHBMin_ = " <<TSmeanHBMin_ << std::endl;   
  std::cout<<" TSmeanHBMax_ = " <<TSmeanHBMax_ << std::endl;   
  std::cout<<" TSmeanHEMin_ = " <<TSmeanHEMin_ << std::endl;   
  std::cout<<" TSmeanHEMax_ = " <<TSmeanHEMax_ << std::endl;   
  std::cout<<" TSmeanHFMin_ = " <<TSmeanHFMin_ << std::endl;   
  std::cout<<" TSmeanHFMax_ = " <<TSmeanHFMax_ << std::endl;   
  std::cout<<" TSmeanHOMin_ = " <<TSmeanHOMin_ << std::endl;   
  std::cout<<" TSmeanHOMax_ = " <<TSmeanHOMax_ << std::endl;   
  
  //
  numOfLaserEv=0;
  local_event=0;
  numOfTS=10;
  run0=-1 ;
  //run1=-1 ;
  runcounter=0 ;
  eventcounter=0 ;

  lumi=0;
  ls0=-1 ;
  lscounter=0 ;
  lscounterM1=0 ;
  lscounter10=0 ;
  nevcounter=0 ;
  lscounterrun=0 ;
  lscounterrun10=0 ;
  nevcounter0=0 ;
  nevcounter00=0 ;
  for(int k0 = 0; k0<4; k0++) {
    for(int k1 = 0; k1<4; k1++) {
      for(int k2 = 0; k2<82; k2++) {
	for(int k3 = 0; k3<72; k3++) {
	  sumEstimator0[k0][k1][k2][k3] = 0.;
	  sumEstimator1[k0][k1][k2][k3] = 0.;
	  sumEstimator2[k0][k1][k2][k3] = 0.;
	  sumEstimator3[k0][k1][k2][k3] = 0.;
	  sumEstimator4[k0][k1][k2][k3] = 0.;
	  sumEstimator5[k0][k1][k2][k3] = 0.;
	  sumEstimator6[k0][k1][k2][k3] = 0.;
	  sum0Estimator[k0][k1][k2][k3] = 0.;
	}//for  
      }//for  
    }//for  
  }//for  
  

  averSIGNALoccupancy_HB = 0.;
  averSIGNALoccupancy_HE = 0.;
  averSIGNALoccupancy_HF = 0.;
  averSIGNALoccupancy_HO = 0.;

  averSIGNALsumamplitude_HB = 0.;
  averSIGNALsumamplitude_HE = 0.;
  averSIGNALsumamplitude_HF = 0.;
  averSIGNALsumamplitude_HO = 0.;

  averNOSIGNALoccupancy_HB = 0.;
  averNOSIGNALoccupancy_HE = 0.;
  averNOSIGNALoccupancy_HF = 0.;
  averNOSIGNALoccupancy_HO = 0.;

  averNOSIGNALsumamplitude_HB = 0.;
  averNOSIGNALsumamplitude_HE = 0.;
  averNOSIGNALsumamplitude_HF = 0.;
  averNOSIGNALsumamplitude_HO = 0.;


  maxxSUM1 = 0.;
  maxxSUM2 = 0.;
  maxxSUM3 = 0.;
  maxxSUM4 = 0.;

  maxxOCCUP1 = 0.;
  maxxOCCUP2 = 0.;
  maxxOCCUP3 = 0.;
  maxxOCCUP4 = 0.;

  testmetka=0;

}

VeRawAnalyzer::~VeRawAnalyzer()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------ method called to for each event  ------------
void VeRawAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){


  if (MAPcreation>0) {
    edm::ESHandle<HcalTopology> topo_;
    iSetup.get<HcalRecNumberingRecord>().get(topo_);
    topo = &*topo_;
    fillMAP();
    MAPcreation=0;
  }
 //std::cout<<" Start analyze "<<nevent<<std::endl;

  if(verbosity > 0) std::cout<<" Start analyze "<<std::endl;    

  nevent++;
  nevent50 = nevent/50;
  Run=iEvent.id().run();
  Nevent=iEvent.id().event();    // event number 
  lumi=iEvent.luminosityBlock(); // lumi section
  int bcn = iEvent.bunchCrossing();

   if(verbosity > 0 ) std::cout  <<  " Run =  "  << Run  <<  " bcn =  "  << bcn  <<  " LS =  "  << lumi  <<  " event =  "  << Nevent  <<  endl;

  int outabortgap = 1;
  if(bcn >= bcnrejectedlow_ && bcn <= bcnrejectedhigh_ ) outabortgap = 0; //  if(bcn>=3446 && bcn<=3564) 

  if((flagabortgaprejected_ == 1 && outabortgap == 1) || 
            (flagabortgaprejected_ == 0 && outabortgap == 0) || flagabortgaprejected_ == 2 ) {
  // to get counters:
  // to get runcounter:
  if(run0 != Run) {
    ++runcounter;
    if( runcounter != 1) {
      nevcounter00=eventcounter;
      cout  <<  " --------------------------------------- "  <<  endl;
      cout<<" for Run = "<<run0<<" with runcounter = "<< runcounter-1 <<" #ev = "<<eventcounter<<endl;
      cout  << " #LS =  "  << lscounterrun << " #LS10 =  "  << lscounterrun10 << " Last LS =  "  << ls0 <<  endl;
      cout  <<  " --------------------------------------------- "  <<  endl;
      h_nls_per_run->Fill(float(lscounterrun) );
      h_nls_per_run10->Fill(float(lscounterrun10) );
      lscounterrun =0;lscounterrun10 =0;
    } // runcounter > 1
    cout  <<  " ---------***********************------------- "  <<  endl;
    cout  <<  " New Run =  "  << Run <<  " runcounter =  "  << runcounter <<  endl;
    cout  <<  " ------- "  <<  endl;
    run0 = Run;
    eventcounter = 0;
    ls0 =-1;
  } // new run
  else {
    nevcounter00=0;
  }//else new run
  ++eventcounter;

  // to get new LS:

  if(ls0 != lumi) {
    if(ls0 != -1) {
      h_nevents_per_eachLS->Fill( float(lscounter) ,float(nevcounter) );//
      nevcounter0=nevcounter;
      if(verbosity > 0 ) std::cout  <<  " nevcounter0 =  "  << nevcounter0  <<  endl;
    } // ls0>-1
    lscounter++;lscounterrun++;

      if(verbosity > 0 ) std::cout  <<  " lscounter =  "  << lscounter  <<  endl;

    ////////////////////////////////////////////////////////////////

    if(usecontinuousnumbering_) { lscounterM1 = lscounter - 1; }
    else {lscounterM1 = ls0; }
    if(ls0 != -1) h_nevents_per_eachRealLS->Fill( float(lscounterM1) ,float(nevcounter) );//

    /////////////////////////////////////////////////////////////////
    h_lsnumber_per_eachLS->Fill( float(lscounter) ,float(lumi) );
    if(nevcounter > 10.) {++lscounter10; ++lscounterrun10; }
    if (verbosity == -79 && lscounter == 76) 
          cout  <<  " for old LS =  "  << ls0 <<  " nevents =  "  << nevcounter <<  endl;
    if (verbosity == -79 && lscounter == 77) 
          cout  <<  " for old LS =  "  << ls0 <<  " nevents =  "  << nevcounter <<  endl;
    h_nevents_per_LS->Fill(float(nevcounter) );
    h_nevents_per_LSzoom->Fill(float(nevcounter) );
    nevcounter=0;
    if (verbosity == -79 && lscounter == 76) 
            cout  <<  " New LS =  "  << lumi <<  "  lscounter =  "  << 
                   lscounter <<  "  lscounter10 =  "  << lscounter10 <<  endl;
    if (verbosity == -79 && lscounter == 77) cout  <<  " New LS =  "  << lumi <<  
          "  lscounter =  "  << lscounter <<  "  lscounter10 =  "  << lscounter10 <<  endl;
    ls0 = lumi;
  }// new lumi
  else {
    nevcounter0=0;
  }//else new lumi
  ++nevcounter;// #ev in LS
  //////
  //11.04.14 - to study time dependencies  
  if(flagtoaskrunsorls_ == 0 ) {
    lscounterM1 = runcounter;
    nevcounter0=nevcounter00;
  }
  // for following use: lscounter, nevcounter0 :=! 0 if new LS started
  if (verbosity == -79 && lscounter == 76) 
              std::cout << "   nevcounter0=  " << nevcounter0 <<" lscounter= " << 
                   lscounter <<" nevcounter= " << nevcounter <<" sumEstimator4= " << 
                           (double)sumEstimator4[3][1][40][50] << std::endl;
  if (verbosity == -84 ) std::cout << "==================>>>    nevcounter0 = " <<nevcounter0 << "in LS nevcounter = " <<nevcounter << "in run: eventcounter = " <<eventcounter << "  lscounter = " <<lscounter << "  lscounterrun = " <<lscounterrun << "  runcounter = " <<runcounter<< "  lscounterM1 = " <<lscounterM1 << std::endl;
  if (verbosity == -5555 ) std::cout << "==================>>>    nevcounter0 = " <<nevcounter0 << "in LS nevcounter = " <<nevcounter << "in run: eventcounter = " <<eventcounter << "  lscounter = " <<lscounter << "  lscounterrun = " <<lscounterrun << "  runcounter = " <<runcounter<< "  lscounterM1 = " <<lscounterM1 << std::endl;
  if (verbosity == -7777 ) std::cout << "==================>>>    nevcounter0 = " <<nevcounter0 << "in LS nevcounter = " <<nevcounter << "in run: eventcounter = " <<eventcounter << "  lscounter = " <<lscounter << "  lscounterrun = " <<lscounterrun << "  runcounter = " <<runcounter<< "  lscounterM1 = " <<lscounterM1 << std::endl;
  if (verbosity == -7778 && nevcounter0 != 0) std::cout << "==================>>>    nevcounter0 = " <<nevcounter0 << "in run: eventcounter = " <<eventcounter << "  lscounter = " <<lscounter << "  lscounterM1 = " <<lscounterM1 << "  lumi = " <<lumi << std::endl;
  
  
  if(nevcounter0 != 0 || nevcounter > 99999 ) {  
    if(nevcounter > 99999 ) nevcounter0 = 1;
 
      std::cout<<" We are here "<<std::endl;
   
    ///////  int sub= cell.subdet();  1-HB, 2-HE, 3-HO, 4-HF
    ////////////            k0(sub): =0 HB; =1 HE; =2 HO; =3 HF;
    ////////////         k1(depth-1): = 0 - 3 or depth: = 1 - 4;
    unsigned long int pcountall1 = 0;
    unsigned long int pcountall3 = 0;
    unsigned long int pcountall6 = 0;
    unsigned long int pcountall8 = 0;

    int pcountmin1 = 0;
    int pcountmin3 = 0;
    int pcountmin6 = 0;
    int pcountmin8 = 0;

    unsigned long int mcountall1 = 0;
    unsigned long int mcountall3 = 0;
    unsigned long int mcountall6 = 0;
    unsigned long int mcountall8 = 0;

    int mcountmin1 = 0;
    int mcountmin3 = 0;
    int mcountmin6 = 0;
    int mcountmin8 = 0;


    int pnnbins1 = 0;
    int pnnbins3 = 0;
    int pnnbins6 = 0;
    int pnnbins8 = 0;

    int pnnmin1 = 999999999;
    int pnnmin3 = 999999999;
    int pnnmin6 = 999999999;
    int pnnmin8 = 999999999;

    int mnnbins1 = 0;
    int mnnbins3 = 0;
    int mnnbins6 = 0;
    int mnnbins8 = 0;

    int mnnmin1 = 999999999;
    int mnnmin3 = 999999999;
    int mnnmin6 = 999999999;
    int mnnmin8 = 999999999;

    for(int k0 = 0; k0<4; k0++) {
      for(int k1 = 0; k1<4; k1++) {
	for(int k3 = 0; k3<72; k3++) {
	  for(int k2 = 0; k2<82; k2++) {
	    int ieta = k2-41;
	    
	    // ------------------------------------------------------------sumEstimator0
	    if (verbosity == -81 ) std::cout << "sumEstimator0 = " <<sumEstimator0[k0][k1][k2][k3]<< std::endl;
	    if(sumEstimator0[k0][k1][k2][k3] != 0. ) {

	      // fill histoes:
	      double bbbc=0.;
	      if(flagestimatornormalization_ == 0 ) bbbc = sumEstimator0[k0][k1][k2][k3]/nevcounter0;
	      if(flagestimatornormalization_ == 1 ) bbbc = sumEstimator0[k0][k1][k2][k3]/sum0Estimator[k0][k1][k2][k3];
	      double bbb1=1.;
	      if(flagestimatornormalization_ == 2 ) {bbbc = sumEstimator0[k0][k1][k2][k3]; bbb1 = sum0Estimator[k0][k1][k2][k3];}
	      
	      // HB:
	      if(k0==0) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumPedestalLS1->Fill(bbbc/bbb1);
		  h_2DsumPedestalLS1->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumPedestalLS1->Fill(double(ieta), double(k3), bbb1);
		  h_sumPedestalperLS1->Fill( float(lscounterM1) ,bbbc);
		  h_sum0PedestalperLS1->Fill( float(lscounterM1) ,bbb1);
		}
		if(k1+1  ==2) {
		  h_sumPedestalLS2->Fill(bbbc/bbb1);
		  h_2DsumPedestalLS2->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumPedestalLS2->Fill(double(ieta), double(k3), bbb1);
		  h_sumPedestalperLS2->Fill( float(lscounterM1) ,bbbc);
		  h_sum0PedestalperLS2->Fill( float(lscounterM1) ,bbb1);
		}
	      }
	      // HE:
	      if(k0==1) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumPedestalLS3->Fill(bbbc/bbb1);
		  h_2DsumPedestalLS3->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumPedestalLS3->Fill(double(ieta), double(k3), bbb1);
		  h_sumPedestalperLS3->Fill( float(lscounterM1) ,bbbc);
		  h_sum0PedestalperLS3->Fill( float(lscounterM1) ,bbb1);
		}
		if(k1+1  ==2) {
		  h_sumPedestalLS4->Fill(bbbc/bbb1);
		  h_2DsumPedestalLS4->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumPedestalLS4->Fill(double(ieta), double(k3), bbb1);
		  h_sumPedestalperLS4->Fill( float(lscounterM1) ,bbbc);
		  h_sum0PedestalperLS4->Fill( float(lscounterM1) ,bbb1);
		}
		if(k1+1  ==3) {
		  h_sumPedestalLS5->Fill(bbbc/bbb1);
		  h_2DsumPedestalLS5->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumPedestalLS5->Fill(double(ieta), double(k3), bbb1);
		  h_sumPedestalperLS5->Fill( float(lscounterM1) ,bbbc);
		  h_sum0PedestalperLS5->Fill( float(lscounterM1) ,bbb1);
		}
	      }
	      // HF:
	      if(k0==3) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumPedestalLS6->Fill(bbbc/bbb1);
		  h_2DsumPedestalLS6->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumPedestalLS6->Fill(double(ieta), double(k3), bbb1);
		  h_sumPedestalperLS6->Fill( float(lscounterM1) ,bbbc);
		  h_sum0PedestalperLS6->Fill( float(lscounterM1) ,bbb1);
		}
		if(k1+1  ==2) {
		  h_sumPedestalLS7->Fill(bbbc/bbb1);
		  h_2DsumPedestalLS7->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumPedestalLS7->Fill(double(ieta), double(k3), bbb1);
		  h_sumPedestalperLS7->Fill( float(lscounterM1) ,bbbc);
		  h_sum0PedestalperLS7->Fill( float(lscounterM1) ,bbb1);
		}
	      }
	      // HO:
	      if(k0==2) {
		// HBdepth1
		if(k1+1  ==4) {
		  h_sumPedestalLS8->Fill(bbbc/bbb1);
		  h_2DsumPedestalLS8->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumPedestalLS8->Fill(double(ieta), double(k3), bbb1);
		  h_sumPedestalperLS8->Fill( float(lscounterM1) ,bbbc);
		  h_sum0PedestalperLS8->Fill( float(lscounterM1) ,bbb1);
		}
	      }
	    }//if(sumEstimator0[k0][k1][k2][k3] != 0.
	    
	    // ---------------------------------------------------sumEstimator1
	    if (verbosity == -81 ) std::cout << "sumEstimator1 = " <<sumEstimator1[k0][k1][k2][k3]<< std::endl;
	    if(sumEstimator1[k0][k1][k2][k3] != 0. ) {
	      
	      // fill histoes:
	      double bbbc=0.;
	      if(flagestimatornormalization_ == 0 ) bbbc = sumEstimator1[k0][k1][k2][k3]/nevcounter0;
	      if(flagestimatornormalization_ == 1 ) bbbc = sumEstimator1[k0][k1][k2][k3]/sum0Estimator[k0][k1][k2][k3];
	      double bbb1=1.;
	      if(flagestimatornormalization_ == 2 ) {bbbc = sumEstimator1[k0][k1][k2][k3]; bbb1 = sum0Estimator[k0][k1][k2][k3];}
	      
	      // HB:
	      if(k0==0) {
		// HBdepth1
		if(k1+1  ==1) {
	  	  if (verbosity == -5555 && k2==55&&k3==17) std::cout << "***sumEstimator1= "  << sumEstimator1[k0][k1][k2][k3] << "sum0Estimator= "  << sum0Estimator[k0][k1][k2][k3] <<" nevcounter0= " << nevcounter0 <<" bbbc= " << bbbc << std::endl;
		  
		  h_sumADCAmplLS1copy1->Fill(bbbc/bbb1);
		  h_sumADCAmplLS1copy2->Fill(bbbc/bbb1);
		  h_sumADCAmplLS1copy3->Fill(bbbc/bbb1);
		  h_sumADCAmplLS1copy4->Fill(bbbc/bbb1);
		  h_sumADCAmplLS1copy5->Fill(bbbc/bbb1);
		  h_sumADCAmplLS1->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 > lsdep_estimator1_HBdepth1_  || bbbc/bbb1 < 0. ) h_2DsumADCAmplLS1->Fill(double(ieta), double(k3), bbbc);
		  if(bbbc/bbb1 > 2.*lsdep_estimator1_HBdepth1_ ) h_2DsumADCAmplLS1_LSselected->Fill(double(ieta), double(k3), bbbc);
//		  if( ( lscounterM1 == 734 || lscounterM1 == 758 || lscounterM1 == 759 || lscounterM1 == 853 || lscounterM1 == 1436 ) ) h_2DsumADCAmplLS1_LSselected->Fill(double(ieta), double(k3), bbbc);
		  //		  if( ( lscounterM1 == 504 || lscounterM1 == 867 ) ) h_2DsumADCAmplLS1_LSselected->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumADCAmplLS1->Fill(double(ieta), double(k3), bbb1);
		  h_sumADCAmplperLS1->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 > lsdep_estimator1_HBdepth1_  || bbbc/bbb1 < 0. ) h_sumCutADCAmplperLS1->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0ADCAmplperLS1->Fill( float(lscounterM1) ,bbb1);
		  if(ieta>0 ){
		    if(k3<36) {
		      h_sumADCAmplperLS1_P1->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS1_P1->Fill( float(lscounterM1) ,bbb1);
		    }
		    else {
		      h_sumADCAmplperLS1_P2->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS1_P2->Fill( float(lscounterM1) ,bbb1);
		    }
		    ////////////////////////////// P
		    if(bbbc/bbb1> 25.) {
		      pcountall1 += bbb1 ;
		      pcountmin1 += bbb1;
		    }
		    //////////////////////////////
		    
		  }
		  else {
		    if(k3<36) {
		      h_sumADCAmplperLS1_M1->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS1_M1->Fill( float(lscounterM1) ,bbb1);
		    }
		    else {
		      h_sumADCAmplperLS1_M2->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS1_M2->Fill( float(lscounterM1) ,bbb1);
		    }
		    ////////////////////////////// M
		    if(bbbc/bbb1> 25.) {
		      mcountall1 += bbb1 ;
		      mcountmin1 += bbb1;
		    }
		    //////////////////////////////
		    
		  }
		  
		}
		// HBdepth2
		if(k1+1  ==2) {
		  h_sumADCAmplLS2->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 > lsdep_estimator1_HBdepth2_   || bbbc/bbb1 < 0. ) h_2DsumADCAmplLS2->Fill(double(ieta), double(k3), bbbc);
		  if(bbbc/bbb1 > 2.*lsdep_estimator1_HBdepth2_  ) h_2DsumADCAmplLS2_LSselected->Fill(double(ieta), double(k3), bbbc);
//		  if( ( lscounterM1 == 734 || lscounterM1 == 758 || lscounterM1 == 759 || lscounterM1 == 853 || lscounterM1 == 1436 ) ) h_2DsumADCAmplLS2_LSselected->Fill(double(ieta), double(k3), bbbc);
		  //		  if( ( lscounterM1 == 504 || lscounterM1 == 867 ) ) h_2DsumADCAmplLS2_LSselected->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumADCAmplLS2->Fill(double(ieta), double(k3), bbb1);
		  h_sumADCAmplperLS2->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 > lsdep_estimator1_HBdepth2_   || bbbc/bbb1 < 0. ) h_sumCutADCAmplperLS2->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0ADCAmplperLS2->Fill( float(lscounterM1) ,bbb1);
		  if(ieta>0 ){
		    if(k3<36) {
		      h_sumADCAmplperLS1_P1->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS1_P1->Fill( float(lscounterM1) ,bbb1);
		    }
		    else {
		      h_sumADCAmplperLS1_P2->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS1_P2->Fill( float(lscounterM1) ,bbb1);
		    }
		  }
		  else {
		    if(k3<36) {
		      h_sumADCAmplperLS1_M1->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS1_M1->Fill( float(lscounterM1) ,bbb1);
		    }
		    else {
		      h_sumADCAmplperLS1_M2->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS1_M2->Fill( float(lscounterM1) ,bbb1);
		    }
		  }

		}
	      }
	      // HE:
	      if(k0==1) {
		// HEdepth1
		if(k1+1  ==1) {
		  h_sumADCAmplLS3->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 > lsdep_estimator1_HEdepth1_   || bbbc/bbb1 < 0. ) h_2DsumADCAmplLS3->Fill(double(ieta), double(k3), bbbc);
		  if(bbbc/bbb1 > 2.*lsdep_estimator1_HEdepth1_  ) h_2DsumADCAmplLS3_LSselected->Fill(double(ieta), double(k3), bbbc);
		  //		    if( ( lscounterM1 == 146 || lscounterM1 == 515 ) ) h_2DsumADCAmplLS3_LSselected->Fill(double(ieta), double(k3), bbbc);
		  //		    if( ( lscounterM1 == 504 || lscounterM1 == 867 ) ) h_2DsumADCAmplLS3_LSselected->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumADCAmplLS3->Fill(double(ieta), double(k3), bbb1);
		  h_sumADCAmplperLS3->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 > lsdep_estimator1_HEdepth1_   || bbbc/bbb1 < 0. ) h_sumCutADCAmplperLS3->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0ADCAmplperLS3->Fill( float(lscounterM1) ,bbb1);
		  if(ieta>0 ){
		    if(k3<36) {
		      h_sumADCAmplperLS3_P1->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS3_P1->Fill( float(lscounterM1) ,bbb1);
		    }
		    else {
		      h_sumADCAmplperLS3_P2->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS3_P2->Fill( float(lscounterM1) ,bbb1);
		    }
		    ////////////////////////////// P
		    //		    if(bbbc/bbb1> 25. ) {
		    if(bbbc/bbb1> 15. && k3%2 == 0) {
		    //if(bbbc/bbb1> 30. && k3%2 != 0) {
		      pcountall3 += bbb1 ;
		      pcountmin3 += bbb1;
		    }
		    //////////////////////////////
		  }
		  else {
		    if(k3<36) {
		      h_sumADCAmplperLS3_M1->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS3_M1->Fill( float(lscounterM1) ,bbb1);
		    }
		    else {
		      h_sumADCAmplperLS3_M2->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS3_M2->Fill( float(lscounterM1) ,bbb1);
		    }
		    ////////////////////////////// M
		    //		    if(bbbc/bbb1> 25. ) {
		    if(bbbc/bbb1> 15. && k3%2 == 0) {
		    //if(bbbc/bbb1> 30. && k3%2 != 0) {
		      mcountall3 += bbb1 ;
		      mcountmin3 += bbb1;
		    }
		    //////////////////////////////
		  }

		}
		// HEdepth2
		if(k1+1  ==2) {
		  h_sumADCAmplLS4->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 > lsdep_estimator1_HEdepth2_   || bbbc/bbb1 < 0. ) h_2DsumADCAmplLS4->Fill(double(ieta), double(k3), bbbc);
		  if(bbbc/bbb1 > 2.*lsdep_estimator1_HEdepth2_  ) h_2DsumADCAmplLS4_LSselected->Fill(double(ieta), double(k3), bbbc);
		  //		    if( ( lscounterM1 == 146 || lscounterM1 == 515 ) ) h_2DsumADCAmplLS4_LSselected->Fill(double(ieta), double(k3), bbbc);
		  //		    if( ( lscounterM1 == 504 || lscounterM1 == 867 ) ) h_2DsumADCAmplLS4_LSselected->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumADCAmplLS4->Fill(double(ieta), double(k3), bbb1);
		  h_sumADCAmplperLS4->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 > lsdep_estimator1_HEdepth2_   || bbbc/bbb1 < 0. ) h_sumCutADCAmplperLS4->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0ADCAmplperLS4->Fill( float(lscounterM1) ,bbb1);
		  if(ieta>0 ){
		    if(k3<36) {
		      h_sumADCAmplperLS3_P1->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS3_P1->Fill( float(lscounterM1) ,bbb1);
		    }
		    else {
		      h_sumADCAmplperLS3_P2->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS3_P2->Fill( float(lscounterM1) ,bbb1);
		    }
		  }
		  else {
		    if(k3<36) {
		      h_sumADCAmplperLS3_M1->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS3_M1->Fill( float(lscounterM1) ,bbb1);
		    }
		    else {
		      h_sumADCAmplperLS3_M2->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS3_M2->Fill( float(lscounterM1) ,bbb1);
		    }
		  }

		}
		// HEdepth3
		if(k1+1  ==3) {
		  h_sumADCAmplLS5->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 > lsdep_estimator1_HEdepth3_   || bbbc/bbb1 < 0. ) h_2DsumADCAmplLS5->Fill(double(ieta), double(k3), bbbc);
		  if(bbbc/bbb1 > 2.*lsdep_estimator1_HEdepth3_  ) h_2DsumADCAmplLS5_LSselected->Fill(double(ieta), double(k3), bbbc);
		  //		    if( ( lscounterM1 == 146 || lscounterM1 == 515 ) ) h_2DsumADCAmplLS5_LSselected->Fill(double(ieta), double(k3), bbbc);
		  //		    if( ( lscounterM1 == 504 || lscounterM1 == 867 ) ) h_2DsumADCAmplLS5_LSselected->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumADCAmplLS5->Fill(double(ieta), double(k3), bbb1);
		  h_sumADCAmplperLS5->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 > lsdep_estimator1_HEdepth3_   || bbbc/bbb1 < 0. ) h_sumCutADCAmplperLS5->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0ADCAmplperLS5->Fill( float(lscounterM1) ,bbb1);
		  if(ieta>0 ){
		    if(k3<36) {
		      h_sumADCAmplperLS3_P1->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS3_P1->Fill( float(lscounterM1) ,bbb1);
		    }
		    else {
		      h_sumADCAmplperLS3_P2->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS3_P2->Fill( float(lscounterM1) ,bbb1);
		    }
		  }
		  else {
		    if(k3<36) {
		      h_sumADCAmplperLS3_M1->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS3_M1->Fill( float(lscounterM1) ,bbb1);
		    }
		    else {
		      h_sumADCAmplperLS3_M2->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS3_M2->Fill( float(lscounterM1) ,bbb1);
		    }
		  }

		}
	      }
	      // HF:
	      if(k0==3) {
		// HFdepth1
		if(k1+1  ==1) {
	  	  if (verbosity == -7777 ) std::cout << "HF***sumEstimator1= "  << sumEstimator1[k0][k1][k2][k3] << "sum0Estimator= "  << sum0Estimator[k0][k1][k2][k3] <<" nevcounter0= " << nevcounter0 <<" bbbc= " << bbbc << std::endl;
		  h_sumADCAmplLS6->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 > lsdep_estimator1_HFdepth1_ || bbbc/bbb1 < 0. ) h_2DsumADCAmplLS6->Fill(double(ieta), double(k3), bbbc);
		  if(bbbc/bbb1 > 2.*lsdep_estimator1_HFdepth1_  ) h_2DsumADCAmplLS6_LSselected->Fill(double(ieta), double(k3), bbbc);
		  //		    if( ( lscounterM1 == 1810 ) ) h_2DsumADCAmplLS6_LSselected->Fill(double(ieta), double(k3), bbbc);
		  //		    if( ( lscounterM1 == 504 || lscounterM1 == 867 ) ) h_2DsumADCAmplLS6_LSselected->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumADCAmplLS6->Fill(double(ieta), double(k3), bbb1);
		  h_sumADCAmplperLS6->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 > lsdep_estimator1_HFdepth1_  || bbbc/bbb1 < 0.  ) h_sumCutADCAmplperLS6->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0ADCAmplperLS6->Fill( float(lscounterM1) ,bbb1);

		  ///////////////////////////////////////////////////////// error-A
 		  if(ieta>0 ){
		    if(k3<36) {
		      h_sumADCAmplperLS6_P1->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS6_P1->Fill( float(lscounterM1) ,bbb1);
		    }
		    else {
		      h_sumADCAmplperLS6_P2->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS6_P2->Fill( float(lscounterM1) ,bbb1);
		    }
		  ////////////////////////////// P
		    if(bbbc/bbb1> 20.) {
		      pcountall6 += bbb1 ;
		      pcountmin6 += bbb1;
		    }
		  //////////////////////////////

		  }
		  else {
		    if(k3<36) {
		      h_sumADCAmplperLS6_M1->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS6_M1->Fill( float(lscounterM1) ,bbb1);
		    }
		    else {
		      h_sumADCAmplperLS6_M2->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS6_M2->Fill( float(lscounterM1) ,bbb1);
		    }
		  ////////////////////////////// M
		    if(bbbc/bbb1> 20.) {
		      mcountall6 += bbb1 ;
		      mcountmin6 += bbb1;
		    }
		  //////////////////////////////

		  }
		  /////////////////////////////////////////////////////////
		  
		  
		}
		// HFdepth2
		if(k1+1  ==2) {
		  h_sumADCAmplLS7->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 > lsdep_estimator1_HFdepth2_   || bbbc/bbb1 < 0. ) h_2DsumADCAmplLS7->Fill(double(ieta), double(k3), bbbc);
		  if(bbbc/bbb1 > 2.*lsdep_estimator1_HFdepth2_  ) h_2DsumADCAmplLS7_LSselected->Fill(double(ieta), double(k3), bbbc);
		  //		    if( ( lscounterM1 == 1810 ) ) h_2DsumADCAmplLS7_LSselected->Fill(double(ieta), double(k3), bbbc);
		  //		    if( ( lscounterM1 == 504 || lscounterM1 == 867 ) ) h_2DsumADCAmplLS7_LSselected->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumADCAmplLS7->Fill(double(ieta), double(k3), bbb1);
		  h_sumADCAmplperLS7->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 > lsdep_estimator1_HFdepth2_   || bbbc/bbb1 < 0. ) h_sumCutADCAmplperLS7->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0ADCAmplperLS7->Fill( float(lscounterM1) ,bbb1);


		  if(ieta>0 ){
		    if(k3<36) {
		      h_sumADCAmplperLS6_P1->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS6_P1->Fill( float(lscounterM1) ,bbb1);
		    }
		    else {
		      h_sumADCAmplperLS6_P2->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS6_P2->Fill( float(lscounterM1) ,bbb1);
		    }
		  }
		  else {
		    if(k3<36) {
		      h_sumADCAmplperLS6_M1->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS6_M1->Fill( float(lscounterM1) ,bbb1);
		    }
		    else {
		      h_sumADCAmplperLS6_M2->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS6_M2->Fill( float(lscounterM1) ,bbb1);
		    }
		  }



		}
	      }
	      // HO:
	      if(k0==2) {
		// HOdepth4
		if(k1+1  ==4) {
		  h_sumADCAmplLS8->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 > lsdep_estimator1_HOdepth4_   || bbbc/bbb1 < 0. ) h_2DsumADCAmplLS8->Fill(double(ieta), double(k3), bbbc);
		  if(bbbc/bbb1 > 2.*lsdep_estimator1_HOdepth4_  ) h_2DsumADCAmplLS8_LSselected->Fill(double(ieta), double(k3), bbbc);
		  //		  if( ( lscounterM1 == 14 ) ) h_2DsumADCAmplLS8_LSselected->Fill(double(ieta), double(k3), bbbc);
		  //		  if( ( lscounterM1 == 504 || lscounterM1 == 867 ) ) h_2DsumADCAmplLS8_LSselected->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumADCAmplLS8->Fill(double(ieta), double(k3), bbb1);
		  h_sumADCAmplperLS8->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 > lsdep_estimator1_HOdepth4_   || bbbc/bbb1 < 0. ) h_sumCutADCAmplperLS8->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0ADCAmplperLS8->Fill( float(lscounterM1) ,bbb1);

		  ///////////////////////////////////////////////////////// error-A
 		  if(ieta>0 ){
		    if(k3<36) {
		      h_sumADCAmplperLS8_P1->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS8_P1->Fill( float(lscounterM1) ,bbb1);
		    }
		    else {
		      h_sumADCAmplperLS8_P2->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS8_P2->Fill( float(lscounterM1) ,bbb1);
		    }
		  ////////////////////////////// P
		    if(bbbc/bbb1> 80.) {
		      pcountall8 += bbb1 ;
		      pcountmin8 += bbb1;
		    }
		  //////////////////////////////

		  }
		  else {
		    if(k3<36) {
		      h_sumADCAmplperLS8_M1->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS8_M1->Fill( float(lscounterM1) ,bbb1);
		    }
		    else {
		      h_sumADCAmplperLS8_M2->Fill( float(lscounterM1) ,bbbc);
		      h_sum0ADCAmplperLS8_M2->Fill( float(lscounterM1) ,bbb1);
		    }
		  ////////////////////////////// M
		    if(bbbc/bbb1> 80.) {
		      mcountall8 += bbb1 ;
		      mcountmin8 += bbb1;
		    }
		  //////////////////////////////

		  }
		  /////////////////////////////////////////////////////////

		}
	      }
	    }//if(sumEstimator1[k0][k1][k2][k3] != 0.


	    // ------------------------------------------------------------------------------------------------------------------------sumEstimator2
	    if (verbosity == -81 ) std::cout << "sumEstimator2 = " <<sumEstimator2[k0][k1][k2][k3]<< std::endl;

                 std::cout << "sumEstimator2 = " <<sumEstimator2[k0][k1][k2][k3]<< std::endl;

	    if(sumEstimator2[k0][k1][k2][k3] != 0. ) {

	      // fill histoes:
	      double bbbc=0.;
	      if(flagestimatornormalization_ == 0 ) bbbc = sumEstimator2[k0][k1][k2][k3]/nevcounter0;
	      if(flagestimatornormalization_ == 1 ) bbbc = sumEstimator2[k0][k1][k2][k3]/sum0Estimator[k0][k1][k2][k3];
	      double bbb1=1.;
	      if(flagestimatornormalization_ == 2 ) {bbbc = sumEstimator2[k0][k1][k2][k3]; bbb1 = sum0Estimator[k0][k1][k2][k3];}
	      
	      // HB:
	      if(k0==0) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumTSmeanALS1->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator2_HBdepth1_  )h_2DsumTSmeanALS1->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmeanALS1->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmeanAperLS1->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator2_HBdepth1_  ) h_sumCutTSmeanAperLS1->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmeanAperLS1->Fill( float(lscounterM1) ,bbb1);
		  if(bbbc/bbb1> 2.*lsdep_estimator2_HBdepth1_  ) h_sumTSmeanAperLS1_LSselected->Fill( float(lscounterM1) ,bbbc); 
//		  if( ( lscounterM1 == 734 || lscounterM1 == 758 || lscounterM1 == 759 || lscounterM1 == 853 || lscounterM1 == 1436 ) ) h_sumTSmeanAperLS1_LSselected->Fill( float(lscounterM1) ,bbbc); 
		  //		  if( ( lscounterM1 == 504 || lscounterM1 == 867 ) ) h_sumTSmeanAperLS1_LSselected->Fill( float(lscounterM1) ,bbbc); 

		}
		if(k1+1  ==2) {
		  h_sumTSmeanALS2->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator2_HBdepth2_  )h_2DsumTSmeanALS2->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmeanALS2->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmeanAperLS2->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator2_HBdepth2_  ) h_sumCutTSmeanAperLS2->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmeanAperLS2->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	      // HE:
	      if(k0==1) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumTSmeanALS3->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator2_HEdepth1_  )h_2DsumTSmeanALS3->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmeanALS3->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmeanAperLS3->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator2_HEdepth1_  ) h_sumCutTSmeanAperLS3->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmeanAperLS3->Fill( float(lscounterM1) ,bbb1);

		}
		if(k1+1  ==2) {
		  h_sumTSmeanALS4->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator2_HEdepth2_  )h_2DsumTSmeanALS4->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmeanALS4->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmeanAperLS4->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator2_HEdepth2_  ) h_sumCutTSmeanAperLS4->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmeanAperLS4->Fill( float(lscounterM1) ,bbb1);

		}
		if(k1+1  ==3) {
		  h_sumTSmeanALS5->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator2_HEdepth3_  )h_2DsumTSmeanALS5->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmeanALS5->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmeanAperLS5->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator2_HEdepth3_  ) h_sumCutTSmeanAperLS5->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmeanAperLS5->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	      // HF:
	      if(k0==3) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumTSmeanALS6->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator2_HFdepth1_  )h_2DsumTSmeanALS6->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmeanALS6->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmeanAperLS6->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator2_HFdepth1_  ) h_sumCutTSmeanAperLS6->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmeanAperLS6->Fill( float(lscounterM1) ,bbb1);

		}
		if(k1+1  ==2) {
		  h_sumTSmeanALS7->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator2_HFdepth2_  )h_2DsumTSmeanALS7->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmeanALS7->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmeanAperLS7->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator2_HFdepth2_  ) h_sumCutTSmeanAperLS7->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmeanAperLS7->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	      // HO:
	      if(k0==2) {
		// HBdepth1
		if(k1+1  ==4) {
		  h_sumTSmeanALS8->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator2_HOdepth4_  )h_2DsumTSmeanALS8->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmeanALS8->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmeanAperLS8->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator2_HOdepth4_  ) h_sumCutTSmeanAperLS8->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmeanAperLS8->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	    }//if(sumEstimator2[k0][k1][k2][k3] != 0.


	    // ------------------------------------------------------------------------------------------------------------------------sumEstimator3
	    if (verbosity == -81 ) std::cout << "sumEstimator3 = " <<sumEstimator3[k0][k1][k2][k3]<< std::endl;

 std::cout << "sumEstimator3 = " <<sumEstimator3[k0][k1][k2][k3]<< std::endl;


	    if(sumEstimator3[k0][k1][k2][k3] != 0. ) {

	      // fill histoes:
	      double bbbc=0.;
	      if(flagestimatornormalization_ == 0 ) bbbc = sumEstimator3[k0][k1][k2][k3]/nevcounter0;
	      if(flagestimatornormalization_ == 1 ) bbbc = sumEstimator3[k0][k1][k2][k3]/sum0Estimator[k0][k1][k2][k3];
	      double bbb1=1.;
	      if(flagestimatornormalization_ == 2 ) {bbbc = sumEstimator3[k0][k1][k2][k3]; bbb1 = sum0Estimator[k0][k1][k2][k3];}
	      
	      // HB:
	      if(k0==0) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumTSmaxALS1->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator3_HBdepth1_  )h_2DsumTSmaxALS1->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmaxALS1->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmaxAperLS1->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator3_HBdepth1_  ) h_sumCutTSmaxAperLS1->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmaxAperLS1->Fill( float(lscounterM1) ,bbb1);
		  if(bbbc/bbb1> 2.*lsdep_estimator3_HBdepth1_  ) h_sumTSmaxAperLS1_LSselected->Fill( float(lscounterM1) ,bbbc); 
//		  if( ( lscounterM1 == 734 || lscounterM1 == 758 || lscounterM1 == 759 || lscounterM1 == 853 || lscounterM1 == 1436 ) ) h_sumTSmaxAperLS1_LSselected->Fill( float(lscounterM1) ,bbbc); 
		  //		  if( ( lscounterM1 == 504 || lscounterM1 == 867 ) ) h_sumTSmaxAperLS1_LSselected->Fill( float(lscounterM1) ,bbbc); 

		}
		if(k1+1  ==2) {
		  h_sumTSmaxALS2->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator3_HBdepth2_  )h_2DsumTSmaxALS2->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmaxALS2->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmaxAperLS2->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator3_HBdepth2_  ) h_sumCutTSmaxAperLS2->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmaxAperLS2->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	      // HE:
	      if(k0==1) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumTSmaxALS3->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator3_HEdepth1_  )h_2DsumTSmaxALS3->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmaxALS3->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmaxAperLS3->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator3_HEdepth1_  ) h_sumCutTSmaxAperLS3->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmaxAperLS3->Fill( float(lscounterM1) ,bbb1);

		}
		if(k1+1  ==2) {
		  h_sumTSmaxALS4->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator3_HEdepth2_  )h_2DsumTSmaxALS4->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmaxALS4->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmaxAperLS4->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator3_HEdepth2_  ) h_sumCutTSmaxAperLS4->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmaxAperLS4->Fill( float(lscounterM1) ,bbb1);

		}
		if(k1+1  ==3) {
		  h_sumTSmaxALS5->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator3_HEdepth3_  )h_2DsumTSmaxALS5->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmaxALS5->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmaxAperLS5->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator3_HEdepth3_  ) h_sumCutTSmaxAperLS5->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmaxAperLS5->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	      // HF:
	      if(k0==3) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumTSmaxALS6->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator3_HFdepth1_  )h_2DsumTSmaxALS6->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmaxALS6->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmaxAperLS6->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator3_HFdepth1_  ) h_sumCutTSmaxAperLS6->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmaxAperLS6->Fill( float(lscounterM1) ,bbb1);

		}
		if(k1+1  ==2) {
		  h_sumTSmaxALS7->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator3_HFdepth2_  )h_2DsumTSmaxALS7->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmaxALS7->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmaxAperLS7->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator3_HFdepth2_  ) h_sumCutTSmaxAperLS7->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmaxAperLS7->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	      // HO:
	      if(k0==2) {
		// HBdepth1
		if(k1+1  ==4) {
		  h_sumTSmaxALS8->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator3_HOdepth4_  )h_2DsumTSmaxALS8->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmaxALS8->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmaxAperLS8->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator3_HOdepth4_  ) h_sumCutTSmaxAperLS8->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmaxAperLS8->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	    }//if(sumEstimator3[k0][k1][k2][k3] != 0.


	    // ------------------------------------------------------------------------------------------------------------------------sumEstimator4
	    if (verbosity == -81 ) std::cout << "sumEstimator4 = " <<sumEstimator4[k0][k1][k2][k3]<< std::endl;

              std::cout << "sumEstimator4 = " <<sumEstimator4[k0][k1][k2][k3]<< std::endl;

	    if(sumEstimator4[k0][k1][k2][k3] != 0. ) {

	      // fill histoes:
	      double bbbc=0.;
	      if(flagestimatornormalization_ == 0 ) bbbc = sumEstimator4[k0][k1][k2][k3]/nevcounter0;
	      if(flagestimatornormalization_ == 1 ) bbbc = sumEstimator4[k0][k1][k2][k3]/sum0Estimator[k0][k1][k2][k3];
	      double bbb1=1.;
	      if(flagestimatornormalization_ == 2 ) {bbbc = sumEstimator4[k0][k1][k2][k3]; bbb1 = sum0Estimator[k0][k1][k2][k3];}
	      
	      // HB:
	      if(k0==0) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumAmplitudeLS1->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator4_HBdepth1_  )h_2DsumAmplitudeLS1->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplitudeLS1->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplitudeperLS1->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator4_HBdepth1_  ) h_sumCutAmplitudeperLS1->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplitudeperLS1->Fill( float(lscounterM1) ,bbb1);
		  if(bbbc/bbb1> 2.*lsdep_estimator4_HBdepth1_  ) h_sumAmplitudeperLS1_LSselected->Fill( float(lscounterM1) ,bbbc); 
		  //		  if( ( lscounterM1 == 734 || lscounterM1 == 758 || lscounterM1 == 759 || lscounterM1 == 853 || lscounterM1 == 1436 ) ) h_sumAmplitudeperLS1_LSselected->Fill( float(lscounterM1) ,bbbc); 
				  //		  if( ( lscounterM1 == 504 || lscounterM1 == 867 ) ) h_sumAmplitudeperLS1_LSselected->Fill( float(lscounterM1) ,bbbc); 

		}
		if(k1+1  ==2) {
		  h_sumAmplitudeLS2->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator4_HBdepth2_  )h_2DsumAmplitudeLS2->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplitudeLS2->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplitudeperLS2->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator4_HBdepth2_  ) h_sumCutAmplitudeperLS2->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplitudeperLS2->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	      // HE:
	      if(k0==1) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumAmplitudeLS3->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator4_HEdepth1_  )h_2DsumAmplitudeLS3->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplitudeLS3->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplitudeperLS3->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator4_HEdepth1_  ) h_sumCutAmplitudeperLS3->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplitudeperLS3->Fill( float(lscounterM1) ,bbb1);

		}
		if(k1+1  ==2) {
		  h_sumAmplitudeLS4->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator4_HEdepth2_  )h_2DsumAmplitudeLS4->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplitudeLS4->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplitudeperLS4->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator4_HEdepth2_  ) h_sumCutAmplitudeperLS4->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplitudeperLS4->Fill( float(lscounterM1) ,bbb1);

		}
		if(k1+1  ==3) {
		  h_sumAmplitudeLS5->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator4_HEdepth3_  )h_2DsumAmplitudeLS5->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplitudeLS5->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplitudeperLS5->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator4_HEdepth3_  ) h_sumCutAmplitudeperLS5->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplitudeperLS5->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	      // HF:
	      if(k0==3) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumAmplitudeLS6->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator4_HFdepth1_   )h_2DsumAmplitudeLS6->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplitudeLS6->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplitudeperLS6->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator4_HFdepth1_   ) h_sumCutAmplitudeperLS6->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplitudeperLS6->Fill( float(lscounterM1) ,bbb1);

		}
		if(k1+1  ==2) {
		  if (verbosity == -79 &&  k2==40 && k3 == 50 && lscounterM1 == 76) std::cout << "======>>>>>   nevcounter0=  " << nevcounter0 <<" bbbc= " << bbbc <<" lscounterM1= " << lscounterM1 <<" nevcounter= " << nevcounter <<" sumEstimator4= " << (double)sumEstimator4[k0][k1][k2][k3] << std::endl;
		  h_sumAmplitudeLS7->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator4_HFdepth2_   )h_2DsumAmplitudeLS7->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplitudeLS7->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplitudeperLS7->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator4_HFdepth2_   ) h_sumCutAmplitudeperLS7->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplitudeperLS7->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	      // HO:
	      if(k0==2) {
		// HBdepth1
		if(k1+1  ==4) {
		  h_sumAmplitudeLS8->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator4_HOdepth4_   )h_2DsumAmplitudeLS8->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplitudeLS8->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplitudeperLS8->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator4_HOdepth4_   ) h_sumCutAmplitudeperLS8->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplitudeperLS8->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	    }//if(sumEstimator4[k0][k1][k2][k3] != 0.


	    // ------------------------------------------------------------------------------------------------------------------------sumEstimator5
	    if (verbosity == -81 ) std::cout << "sumEstimator5 = " <<sumEstimator5[k0][k1][k2][k3]<< std::endl;

                    std::cout << "sumEstimator5 = " <<sumEstimator5[k0][k1][k2][k3]<< std::endl;

	    if(sumEstimator5[k0][k1][k2][k3] != 0. ) {

	      // fill histoes:
	      double bbbc=0.;
	      if(flagestimatornormalization_ == 0 ) bbbc = sumEstimator5[k0][k1][k2][k3]/nevcounter0;
	      if(flagestimatornormalization_ == 1 ) bbbc = sumEstimator5[k0][k1][k2][k3]/sum0Estimator[k0][k1][k2][k3];
	      double bbb1=1.;
	      if(flagestimatornormalization_ == 2 ) {bbbc = sumEstimator5[k0][k1][k2][k3]; bbb1 = sum0Estimator[k0][k1][k2][k3];}
	      
	      // HB:
	      if(k0==0) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumAmplLS1->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator5_HBdepth1_  )h_2DsumAmplLS1->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplLS1->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplperLS1->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator5_HBdepth1_  ) h_sumCutAmplperLS1->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplperLS1->Fill( float(lscounterM1) ,bbb1);
		  if(bbbc/bbb1> 2.*lsdep_estimator5_HBdepth1_  ) h_sumAmplperLS1_LSselected->Fill( float(lscounterM1) ,bbbc); 
		  //		  if( ( lscounterM1 == 734 || lscounterM1 == 758 || lscounterM1 == 759 || lscounterM1 == 853 || lscounterM1 == 1436 ) ) h_sumAmplperLS1_LSselected->Fill( float(lscounterM1) ,bbbc); 
		  //		  if( ( lscounterM1 == 504 || lscounterM1 == 867 ) ) h_sumAmplperLS1_LSselected->Fill( float(lscounterM1) ,bbbc); 

		}
		if(k1+1  ==2) {
		  h_sumAmplLS2->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator5_HBdepth2_  )h_2DsumAmplLS2->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplLS2->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplperLS2->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator5_HBdepth2_  ) h_sumCutAmplperLS2->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplperLS2->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	      // HE:
	      if(k0==1) {
		// HEdepth1
		if(k1+1  ==1) {
		  h_sumAmplLS3->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator5_HEdepth1_  )h_2DsumAmplLS3->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplLS3->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplperLS3->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator5_HEdepth1_  ) h_sumCutAmplperLS3->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplperLS3->Fill( float(lscounterM1) ,bbb1);

		}
		if(k1+1  ==2) {
		  h_sumAmplLS4->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator5_HEdepth2_  )h_2DsumAmplLS4->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplLS4->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplperLS4->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator5_HEdepth2_  ) h_sumCutAmplperLS4->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplperLS4->Fill( float(lscounterM1) ,bbb1);

		}
		if(k1+1  ==3) {
		  h_sumAmplLS5->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator5_HEdepth3_  )h_2DsumAmplLS5->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplLS5->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplperLS5->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator5_HEdepth3_  ) h_sumCutAmplperLS5->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplperLS5->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	      // HF:
	      if(k0==3) {
		// HFdepth1
		if(k1+1  ==1) {
		  h_sumAmplLS6->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator5_HFdepth1_  )h_2DsumAmplLS6->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplLS6->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplperLS6->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator5_HFdepth1_  ) h_sumCutAmplperLS6->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplperLS6->Fill( float(lscounterM1) ,bbb1);

		}
		if(k1+1  ==2) {
		  h_sumAmplLS7->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator5_HFdepth2_  )h_2DsumAmplLS7->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplLS7->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplperLS7->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator5_HFdepth2_  ) h_sumCutAmplperLS7->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplperLS7->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	      // HO:
	      if(k0==2) {
		// HOdepth4
		if(k1+1  ==4) {
		  h_sumAmplLS8->Fill(bbbc/bbb1);
		  if(bbbc/bbb1>lsdep_estimator5_HOdepth4_  )h_2DsumAmplLS8->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplLS8->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplperLS8->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1> lsdep_estimator5_HOdepth4_  ) h_sumCutAmplperLS8->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplperLS8->Fill( float(lscounterM1) ,bbb1);
		  
		}
	      }
	    }//if(sumEstimator5[k0][k1][k2][k3] != 0.
	    
	    // ------------------------------------------------------------------------------------------------------------------------sumEstimator6 (Error-B)
	    if (verbosity == -81 ) std::cout << "sumEstimator6 = " <<sumEstimator6[k0][k1][k2][k3]<< std::endl;

                  std::cout << "sumEstimator6 = " <<sumEstimator6[k0][k1][k2][k3]<< std::endl;


	    if(sumEstimator6[k0][k1][k2][k3] != 0. ) {
	      
	      // fill histoes:
	      double bbbc=0.;
	      if(flagestimatornormalization_ == 0 ) bbbc = sumEstimator6[k0][k1][k2][k3]/nevcounter0;
	      if(flagestimatornormalization_ == 1 ) bbbc = sumEstimator6[k0][k1][k2][k3]/sum0Estimator[k0][k1][k2][k3];
	      double bbb1=1.;
	      if(flagestimatornormalization_ == 2 ) {bbbc = sumEstimator6[k0][k1][k2][k3]; bbb1 = sum0Estimator[k0][k1][k2][k3];}
	      
	      // HB:
	      if(k0==0) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumErrorBLS1->Fill(bbbc/bbb1);
		  h_2DsumErrorBLS1->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumErrorBLS1->Fill(double(ieta), double(k3), bbb1);
		  h_sumErrorBperLS1->Fill( float(lscounterM1) ,bbbc);
		  h_sum0ErrorBperLS1->Fill( float(lscounterM1) ,bbb1);
		}
		if(k1+1  ==2) {
		  h_sumErrorBLS2->Fill(bbbc/bbb1);
		  h_2DsumErrorBLS2->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumErrorBLS2->Fill(double(ieta), double(k3), bbb1);
		  h_sumErrorBperLS2->Fill( float(lscounterM1) ,bbbc);
		  h_sum0ErrorBperLS2->Fill( float(lscounterM1) ,bbb1);
		}
	      }
	      // HE:
	      if(k0==1) {
		// HEdepth1
		if(k1+1  ==1) {
		  h_sumErrorBLS3->Fill(bbbc/bbb1);
		  h_2DsumErrorBLS3->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumErrorBLS3->Fill(double(ieta), double(k3), bbb1);
		  h_sumErrorBperLS3->Fill( float(lscounterM1) ,bbbc);
		  h_sum0ErrorBperLS3->Fill( float(lscounterM1) ,bbb1);
		}
		if(k1+1  ==2) {
		  h_sumErrorBLS4->Fill(bbbc/bbb1);
		  h_2DsumErrorBLS4->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumErrorBLS4->Fill(double(ieta), double(k3), bbb1);
		  h_sumErrorBperLS4->Fill( float(lscounterM1) ,bbbc);
		  h_sum0ErrorBperLS4->Fill( float(lscounterM1) ,bbb1);
		}
		if(k1+1  ==3) {
		  h_sumErrorBLS5->Fill(bbbc/bbb1);
		  h_2DsumErrorBLS5->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumErrorBLS5->Fill(double(ieta), double(k3), bbb1);
		  h_sumErrorBperLS5->Fill( float(lscounterM1) ,bbbc);
		  h_sum0ErrorBperLS5->Fill( float(lscounterM1) ,bbb1);
		}
	      }
	      // HF:
	      if(k0==3) {
		// HFdepth1
		if(k1+1  ==1) {
		  h_sumErrorBLS6->Fill(bbbc/bbb1);
		  h_2DsumErrorBLS6->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumErrorBLS6->Fill(double(ieta), double(k3), bbb1);
		  h_sumErrorBperLS6->Fill( float(lscounterM1) ,bbbc);
		  h_sum0ErrorBperLS6->Fill( float(lscounterM1) ,bbb1);
		}
		if(k1+1  ==2) {
		  h_sumErrorBLS7->Fill(bbbc/bbb1);
		  h_2DsumErrorBLS7->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumErrorBLS7->Fill(double(ieta), double(k3), bbb1);
		  h_sumErrorBperLS7->Fill( float(lscounterM1) ,bbbc);
		  h_sum0ErrorBperLS7->Fill( float(lscounterM1) ,bbb1);
		}
	      }
	      // HO:
	      if(k0==2) {
		// HOdepth4
		if(k1+1  ==4) {
		  h_sumErrorBLS8->Fill(bbbc/bbb1);
		  h_2DsumErrorBLS8->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumErrorBLS8->Fill(double(ieta), double(k3), bbb1);
		  h_sumErrorBperLS8->Fill( float(lscounterM1) ,bbbc);
		  h_sum0ErrorBperLS8->Fill( float(lscounterM1) ,bbb1);
		}
	      }
	      ///
	    }//if(sumEstimator6[k0][k1][k2][k3] != 0.


	    /////////////////////////////////////////////////////////////////////	    
	    
	  }//for k2


	  // occupancy distributions for error-A:
	  // HB
	  if(k0 == 0 && k1 == 0) {
	    if(pcountmin1> 0) {
	      //	  cout<<" HB+    - pcountmin1 = "<<   pcountmin1    <<" pcountall1 = "<<   pcountall1   <<endl;
	      if(pcountmin1 < pnnmin1 ) pnnmin1 = pcountmin1;
	      pcountmin1=0;
	      pnnbins1++;
	    }
	    if(mcountmin1> 0) {
	      //	  cout<<" HB-    - mcountmin1 = "<<   mcountmin1    <<" mcountall1 = "<<   mcountall1   <<endl;
	      if(mcountmin1 < mnnmin1 ) mnnmin1 = mcountmin1;
	      mcountmin1=0;
	      mnnbins1++;
	    }
	  }//
	  // HE
	  if(k0 == 1 && k1 == 0) {
	    if(pcountmin3> 0) {
	      //	  cout<<" HE+    - pcountmin3 = "<<   pcountmin3    <<" pcountall3 = "<<   pcountall3   <<endl;
	      if(pcountmin3 < pnnmin3 ) pnnmin3 = pcountmin3;
	      pcountmin3=0;
	      pnnbins3++;
	    }
	    if(mcountmin3> 0) {
	      //	  cout<<" HE-    - mcountmin3 = "<<   mcountmin3    <<" mcountall3 = "<<   mcountall3   <<endl;
	      if(mcountmin3 < mnnmin3 ) mnnmin3 = mcountmin3;
	      mcountmin3=0;
	      mnnbins3++;
	    }
	  }//
	  // HO
	  if(k0 == 2 && k1 == 3) {
	    if(pcountmin8> 0) {
	      //	  cout<<" HO+    - pcountmin8 = "<<   pcountmin8    <<" pcountall8 = "<<   pcountall8   <<endl;
	      if(pcountmin8 < pnnmin8 ) pnnmin8 = pcountmin8;
	      pcountmin8=0;
	      pnnbins8++;
	    }
	    if(mcountmin8> 0) {
	      //	  cout<<" HO-    - mcountmin8 = "<<   mcountmin8    <<" mcountall8 = "<<   mcountall8   <<endl;
	      if(mcountmin8 < mnnmin8 ) mnnmin8 = mcountmin8;
	      mcountmin8=0;
	      mnnbins8++;
	    }
	  }//
	  // HF
	  if(k0 == 3 && k1 == 0) {
	    if(pcountmin6> 0) {
	      //	  cout<<" HF+    - pcountmin6 = "<<   pcountmin6    <<" pcountall6 = "<<   pcountall6   <<endl;
	      if(pcountmin6 < pnnmin6 ) pnnmin6 = pcountmin6;
	      pcountmin6=0;
	      pnnbins6++;
	    }
	    if(mcountmin6> 0) {
	      //	  cout<<" HF-    - mcountmin6 = "<<   mcountmin6    <<" mcountall6 = "<<   mcountall6   <<endl;
	      if(mcountmin6 < mnnmin6 ) mnnmin6 = mcountmin6;
	      mcountmin6=0;
	      mnnbins6++;
	    }
	  }//
	  
	}//for k3
      }//for k1
    }//for k0
    ///////  int sub= cell.subdet();  1-HB, 2-HE, 3-HO, 4-HF
    ////////////            k0(sub): =0 HB; =1 HE; =2 HO; =3 HF;
    ////////////         k1(depth-1): = 0 - 3 or depth: = 1 - 4;
    
    //   cout<<"=============================== lscounterM1 = "<<   (float)lscounterM1    <<endl;

    //      cout<<" HB+    - pcountall1 = "<<   pcountall1    <<" pnnmin1= "<<   pnnmin1    <<" pnnbins1= "<<   pnnbins1    <<" Ave= "<<   pcountall/pnnbins1    <<endl;
    float patiooccupancy1=  0.;
    if(pcountall1 != 0 ) patiooccupancy1 =  (float)pnnmin1*mnnbins1/pcountall1;
    //   if(pnnmin1 > 0.) cout<<" HB+  - pcountall1/pnnbins1/pnnmin1 = "<<   (float)pcountall1/pnnbins1/pnnmin1    <<" patiooccupancy1 = "<<   patiooccupancy1 <<endl;
     h_RatioOccupancy_HBM->Fill( float(lscounterM1) ,patiooccupancy1); 
    //      cout<<" HB-    - mcountall1 = "<<   mcountall1    <<" mnnmin1= "<<   mnnmin1    <<" mnnbins1= "<<   mnnbins1    <<" Ave= "<<   mcountall1/mnnbins1    <<endl;
    float matiooccupancy1=  0.;
    if(mcountall1 != 0 ) matiooccupancy1=  (float)mnnmin1*mnnbins1/mcountall1;
    //   if(mnnmin1 > 0.) cout<<" HB-    - mcountall1/mnnbins1/mnnmin1 = "<<   (float)mcountall1/mnnbins1/mnnmin1    <<" matiooccupancy1 = "<<   matiooccupancy1 <<endl;
     h_RatioOccupancy_HBP->Fill( float(lscounterM1) ,matiooccupancy1); 
     

    //      cout<<" HE+    - pcountall3 = "<<   pcountall3    <<" pnnmin3= "<<   pnnmin3    <<" pnnbins3= "<<   pnnbins3    <<" Ave= "<<   pcountal3/pnnbins3    <<endl;
    float patiooccupancy3=  0.;
    if(pcountall3 != 0 ) patiooccupancy3 =  (float)pnnmin3*mnnbins3/pcountall3;
    //   if(pnnmin3 > 0.) cout<<" HE+  - pcountall3/pnnbins3/pnnmin3 = "<<   (float)pcountall3/pnnbins3/pnnmin3    <<" patiooccupancy3 = "<<   patiooccupancy3 <<endl;
     h_RatioOccupancy_HEM->Fill( float(lscounterM1) ,patiooccupancy3); 
    //      cout<<" HE-    - mcountall3 = "<<   mcountall3    <<" mnnmin3= "<<   mnnmin3    <<" mnnbins3= "<<   mnnbins3    <<" Ave= "<<   mcountall3/mnnbins3    <<endl;
    float matiooccupancy3=  0.;
    if(mcountall3 != 0 ) matiooccupancy3=  (float)mnnmin3*mnnbins3/mcountall3;
    //   if(mnnmin3 > 0.) cout<<" HE-    - mcountall3/mnnbins3/mnnmin3 = "<<   (float)mcountall3/mnnbins3/mnnmin3    <<" matiooccupancy3 = "<<   matiooccupancy3 <<endl;
     h_RatioOccupancy_HEP->Fill( float(lscounterM1) ,matiooccupancy3); 
     

    //      cout<<" HF+    - pcountall6 = "<<   pcountall6    <<" pnnmin6= "<<   pnnmin6    <<" pnnbins6= "<<   pnnbins6    <<" Ave= "<<   pcountal6/pnnbins6    <<endl;
    float patiooccupancy6=  0.;
    if(pcountall6 != 0 ) patiooccupancy6 =  (float)pnnmin6*mnnbins6/pcountall6;
    //   if(pnnmin6 > 0.) cout<<" HF+  - pcountall6/pnnbins6/pnnmin6 = "<<   (float)pcountall6/pnnbins6/pnnmin6    <<" patiooccupancy6 = "<<   patiooccupancy6 <<endl;
     h_RatioOccupancy_HFM->Fill( float(lscounterM1) ,patiooccupancy6); 
    //      cout<<" HF-    - mcountall6 = "<<   mcountall6    <<" mnnmin6= "<<   mnnmin6    <<" mnnbins6= "<<   mnnbins6    <<" Ave= "<<   mcountall6/mnnbins6    <<endl;
    float matiooccupancy6=  0.;
    if(mcountall6 != 0 ) matiooccupancy6=  (float)mnnmin6*mnnbins6/mcountall6;
    //   if(mnnmin6 > 0.) cout<<" HF-    - mcountall6/mnnbins6/mnnmin6 = "<<   (float)mcountall6/mnnbins6/mnnmin6    <<" matiooccupancy6 = "<<   matiooccupancy6 <<endl;
     h_RatioOccupancy_HFP->Fill( float(lscounterM1) ,matiooccupancy6); 
     

    //      cout<<" HO+    - pcountall8 = "<<   pcountall8    <<" pnnmin8= "<<   pnnmin8    <<" pnnbins8= "<<   pnnbins8    <<" Ave= "<<   pcountal8/pnnbins8    <<endl;
    float patiooccupancy8=  0.;
    if(pcountall8 != 0 ) patiooccupancy8 =  (float)pnnmin8*mnnbins8/pcountall8;
    //   if(pnnmin8 > 0.) cout<<" HO+  - pcountall8/pnnbins8/pnnmin8 = "<<   (float)pcountall8/pnnbins8/pnnmin8    <<" patiooccupancy8 = "<<   patiooccupancy8 <<endl;
     h_RatioOccupancy_HOM->Fill( float(lscounterM1) ,patiooccupancy8); 
    //      cout<<" HO-    - mcountall8 = "<<   mcountall8    <<" mnnmin8= "<<   mnnmin8    <<" mnnbins8= "<<   mnnbins8    <<" Ave= "<<   mcountall8/mnnbins8    <<endl;
    float matiooccupancy8=  0.;
    if(mcountall8 != 0 ) matiooccupancy8=  (float)mnnmin8*mnnbins8/mcountall8;
    //   if(mnnmin8 > 0.) cout<<" HO-    - mcountall8/mnnbins8/mnnmin8 = "<<   (float)mcountall8/mnnbins8/mnnmin8    <<" matiooccupancy8 = "<<   matiooccupancy8 <<endl;
     h_RatioOccupancy_HOP->Fill( float(lscounterM1) ,matiooccupancy8); 
     

    
    if (verbosity == -5555 ) std::cout << "********************** My NULLing " << std::endl;
    for(int k0 = 0; k0<4; k0++) {
      for(int k1 = 0; k1<4; k1++) {
	for(int k2 = 0; k2<82; k2++) {
	  for(int k3 = 0; k3<72; k3++) {
	    // reset massives:
	    sumEstimator0[k0][k1][k2][k3] = 0.;
	    sumEstimator1[k0][k1][k2][k3] = 0.;
	    sumEstimator2[k0][k1][k2][k3] = 0.;
	    sumEstimator3[k0][k1][k2][k3] = 0.;
	    sumEstimator4[k0][k1][k2][k3] = 0.;
	    sumEstimator5[k0][k1][k2][k3] = 0.;
	    sumEstimator6[k0][k1][k2][k3] = 0.;
	    sum0Estimator[k0][k1][k2][k3] = 0.;
	  }//for  
	}//for  
      }//for  
    }//for 
    

    //------------------------------------------------------                        averSIGNAL
    averSIGNALoccupancy_HB /= float(nevcounter0);
    h_averSIGNALoccupancy_HB->Fill( float(lscounterM1) ,averSIGNALoccupancy_HB);
    averSIGNALoccupancy_HE /= float(nevcounter0);
    h_averSIGNALoccupancy_HE->Fill( float(lscounterM1) ,averSIGNALoccupancy_HE);
    averSIGNALoccupancy_HF /= float(nevcounter0);
    h_averSIGNALoccupancy_HF->Fill( float(lscounterM1) ,averSIGNALoccupancy_HF);
    averSIGNALoccupancy_HO /= float(nevcounter0);
    h_averSIGNALoccupancy_HO->Fill( float(lscounterM1) ,averSIGNALoccupancy_HO);

    averSIGNALoccupancy_HB = 0.;
    averSIGNALoccupancy_HE = 0.;
    averSIGNALoccupancy_HF = 0.;
    averSIGNALoccupancy_HO = 0.;
    
    //------------------------------------------------------
    averSIGNALsumamplitude_HB /= float(nevcounter0);
    h_averSIGNALsumamplitude_HB->Fill( float(lscounterM1) ,averSIGNALsumamplitude_HB);
    averSIGNALsumamplitude_HE /= float(nevcounter0);
    h_averSIGNALsumamplitude_HE->Fill( float(lscounterM1) ,averSIGNALsumamplitude_HE);
    averSIGNALsumamplitude_HF /= float(nevcounter0);
    h_averSIGNALsumamplitude_HF->Fill( float(lscounterM1) ,averSIGNALsumamplitude_HF);
    averSIGNALsumamplitude_HO /= float(nevcounter0);
    h_averSIGNALsumamplitude_HO->Fill( float(lscounterM1) ,averSIGNALsumamplitude_HO);

    averSIGNALsumamplitude_HB = 0.;
    averSIGNALsumamplitude_HE = 0.;
    averSIGNALsumamplitude_HF = 0.;
    averSIGNALsumamplitude_HO = 0.;
    
    //------------------------------------------------------                        averNOSIGNAL
    averNOSIGNALoccupancy_HB /= float(nevcounter0);
    h_averNOSIGNALoccupancy_HB->Fill( float(lscounterM1) ,averNOSIGNALoccupancy_HB);
    averNOSIGNALoccupancy_HE /= float(nevcounter0);
    h_averNOSIGNALoccupancy_HE->Fill( float(lscounterM1) ,averNOSIGNALoccupancy_HE);
    averNOSIGNALoccupancy_HF /= float(nevcounter0);
    h_averNOSIGNALoccupancy_HF->Fill( float(lscounterM1) ,averNOSIGNALoccupancy_HF);
    averNOSIGNALoccupancy_HO /= float(nevcounter0);
    h_averNOSIGNALoccupancy_HO->Fill( float(lscounterM1) ,averNOSIGNALoccupancy_HO);

    averNOSIGNALoccupancy_HB = 0.;
    averNOSIGNALoccupancy_HE = 0.;
    averNOSIGNALoccupancy_HF = 0.;
    averNOSIGNALoccupancy_HO = 0.;
    
    //------------------------------------------------------
    averNOSIGNALsumamplitude_HB /= float(nevcounter0);
    h_averNOSIGNALsumamplitude_HB->Fill( float(lscounterM1) ,averNOSIGNALsumamplitude_HB);
    averNOSIGNALsumamplitude_HE /= float(nevcounter0);
    h_averNOSIGNALsumamplitude_HE->Fill( float(lscounterM1) ,averNOSIGNALsumamplitude_HE);
    averNOSIGNALsumamplitude_HF /= float(nevcounter0);
    h_averNOSIGNALsumamplitude_HF->Fill( float(lscounterM1) ,averNOSIGNALsumamplitude_HF);
    averNOSIGNALsumamplitude_HO /= float(nevcounter0);
    h_averNOSIGNALsumamplitude_HO->Fill( float(lscounterM1) ,averNOSIGNALsumamplitude_HO);

    averNOSIGNALsumamplitude_HB = 0.;
    averNOSIGNALsumamplitude_HE = 0.;
    averNOSIGNALsumamplitude_HF = 0.;
    averNOSIGNALsumamplitude_HO = 0.;
    
    //------------------------------------------------------   maxxSA and maxxOccupancy
    if (verbosity == -9812 ) std::cout << "************* lscounterM1= " <<lscounterM1 << "   maxxSUM1 =  " <<maxxSUM1 << "   nevcounter0 =  " <<nevcounter0 << std::endl;
    if (verbosity == -9812 ) std::cout << "after dividing   maxxSUM1 =  " <<maxxSUM1 << std::endl;
    h_maxxSUMAmpl_HB->Fill( float(lscounterM1) ,maxxSUM1);
    h_maxxSUMAmpl_HE->Fill( float(lscounterM1) ,maxxSUM2);
    h_maxxSUMAmpl_HO->Fill( float(lscounterM1) ,maxxSUM3);
    h_maxxSUMAmpl_HF->Fill( float(lscounterM1) ,maxxSUM4);
    maxxSUM1 = 0.;
    maxxSUM2 = 0.;
    maxxSUM3 = 0.;
    maxxSUM4 = 0.;
    //------------------------------------------------------
    h_maxxOCCUP_HB->Fill( float(lscounterM1) ,maxxOCCUP1);
    h_maxxOCCUP_HE->Fill( float(lscounterM1) ,maxxOCCUP2);
    h_maxxOCCUP_HO->Fill( float(lscounterM1) ,maxxOCCUP3);
    h_maxxOCCUP_HF->Fill( float(lscounterM1) ,maxxOCCUP4);
    maxxOCCUP1 = 0.;
    maxxOCCUP2 = 0.;
    maxxOCCUP3 = 0.;
    maxxOCCUP4 = 0.;
    
    
    
    //------------------------------------------------------
  }//if(nevcounter0 != 0)
//  POINT1  
 
  ///////////////////////////////////////////////////
  // get conditions
  if(verbosity > 0 ) std::cout  <<  " get conditions   "  <<  endl;

  iSetup.get<HcalDbRecord>().get(conditions);
  //  shape = conditions->getHcalShape (); //generic
  if(verbosity > 0 ) std::cout  <<  " get conditions done  "  <<  endl;
    
  /////////////////////////////////////////////////// over DigiCollections:
  for(int k1 = 0; k1<4; k1++) {
    for(int k2 = 0; k2<82; k2++) {
      for(int k3 = 0; k3<72; k3++) {
	if(studyCalibCellsHist_) {
	  signal[k1][k2][k3] = 0.;
	  calibt[k1][k2][k3] = 0.;
	  calibcapiderror[k1][k2][k3] = 0;
	  caliba[k1][k2][k3] = 0.;
	  calibw[k1][k2][k3] = 0.;
	  calib0[k1][k2][k3] = 0.;
	  signal3[k1][k2][k3] = 0.;
	  calib3[k1][k2][k3] = 0.;
	  calib2[k1][k2][k3] = 0.;
	}
	if(studyRunDependenceHist_) {
	  for(int k0 = 0; k0<4; k0++) {
	    badchannels[k0][k1][k2][k3] = 0;
	  }//for
	}//if
	
      }//for  
    }//for  
  }//for  
  for(int k0 = 0; k0<4; k0++) {
    for(int k1 = 0; k1<4; k1++) {
      for(int k2 = 0; k2<82; k2++) {
	for(int k3 = 0; k3<72; k3++) {
	  amplitudechannel[k0][k1][k2][k3] = 0. ;
	}//k3
      }//k2
    }//k1
  }//k0
  /////////////////////////////////////////////// HFDigiCollection
  if(verbosity > 0 ) std::cout  <<  " HFDigiCollection start  "  <<  endl;

//   std::cout  <<  " HFDigiCollection start  "  <<  endl;
//   verbosity = 1;


  edm::Handle<HFDigiCollection> hf;
  if(verbosity > 0 ) std::cout  <<  " 1  "  <<  endl;
  
  iEvent.getByToken(tok_hf_,hf);
  if(verbosity > 0 ) std::cout  <<  " 2  "  <<  endl;
  bool gotHFDigis=true;
  if (!(iEvent.getByToken(tok_hf_,hf)))
    {
      gotHFDigis=false; //this is a boolean set up to check if there are HFgigis in input root file
    }
  if (!(hf.isValid()))
    {
      gotHFDigis=false; //if it is not there, leave it false
    }
  if(!gotHFDigis) {
    //  if(!hf.isValid()) {
    cout<<" No HFDigiCollection collection is found "<<endl;
  } else {

  if(verbosity > 0 ) std::cout  <<  " 3  "  <<  endl;
    int qwert7= 0; int qwert8= 0;
    for(HFDigiCollection::const_iterator digi=hf->begin();digi!=hf->end();digi++){
      eta=digi->id().ieta(); phi=digi->id().iphi(); depth=digi->id().depth(); nTS=digi->size();
      ///////////////////
      counter++;
      if(verbosity > 0 ) std::cout  <<  " counter= "  << counter <<  endl;
      
      if(depth == 1 && qwert7==0 ){nnnnnn7++;qwert7=1;}
      if(depth == 2 && qwert8==0 ){nnnnnn8++;qwert8=1;}    
      ////////////////////////////////////////////////////////////  for zerrors.C script:
      if(recordHistoes_ && studyCapIDErrorsHist_) fillDigiErrorsHF(digi); 
  if(verbosity > 0 ) std::cout  <<  " 4  "  <<  endl;
      
      //////////////////////////////////////  for ztsmaxa.C,zratio34.C,zrms.C & zdifampl.C scripts:
      if(recordHistoes_) fillDigiAmplitudeHF(digi); 
      if(verbosity > 0 ) std::cout  <<  " 5  "  <<  endl;
      ///////////////////

      if(recordHistoes_ && studyCalibCellsHist_) {
	int iphi  = phi-1;
	int ieta  = eta;
	if(ieta > 0) ieta -= 1;
	if (verbosity == -13) std::cout<<"******************   HFDigiCollection nTS= "<<nTS<<std::endl;

	if(nTS<=numOfTS) for(int i=0;i<nTS;i++) {

	  TS_data[i]=adc2fC[digi->sample(i).adc()];
	  signal[3][ieta+41][iphi] += TS_data[i];

	  if(i>1&&i<6) signal3[3][ieta+41][iphi] += TS_data[i];

	  if (verbosity == -13) {
	    cout<< "HF phi= " << phi << " eta= " << eta << endl;
	    cout<< "HF iphi= " << iphi << " ieta= " << ieta << endl;
	    cout<< "HF TS0= " << adc2fC[digi->sample(0).adc()] 
		<< " TS1= " << adc2fC[digi->sample(1).adc()] 
		<< " TS2= " << adc2fC[digi->sample(2).adc()] 
		<< " TS3= " << adc2fC[digi->sample(3).adc()] 
		<< " TS4= " << adc2fC[digi->sample(4).adc()] 
		<< " TS5= " << adc2fC[digi->sample(5).adc()] << endl;
	  }// Verbosity

	}// TS 
      }// if(recordHistoes_ && studyCalibCellsHist_) 
    }// for
  } // hf.isValid

  /////////////////////////////////////////////// HBHEDigiCollection
  if(verbosity > 0 ) std::cout  <<  " 6  "  <<  endl;

  edm::Handle<HBHEDigiCollection> hbhe; 
  iEvent.getByToken(tok_hbhe_,hbhe);
  
  bool gotHBHEDigis=true;
  if (!(iEvent.getByToken(tok_hbhe_,hbhe)))
    {
      gotHBHEDigis=false; //this is a boolean set up to check if there are HBHEgigis in input root file
    }
  if (!(hbhe.isValid()))
    {
      gotHBHEDigis=false; //if it is not there, leave it false
    }
  
  if(!gotHBHEDigis) {
    //  if(!hbhe.isValid()) {
    cout<<" No HBHEDigiCollection collection is found "<<endl;
  } else {
    
    
    unsigned int N =  hbhe->size();
    if (verbosity == -22) std::cout << " HBHEDigiCollection size : " << N << std::endl;

    int qwert1= 0; int qwert2= 0;int qwert3= 0;int qwert4= 0;int qwert5= 0;
    for(HBHEDigiCollection::const_iterator digi=hbhe->begin();digi!=hbhe->end();digi++)
      {
	eta=digi->id().ieta(); phi=digi->id().iphi(); depth=digi->id().depth(); nTS=digi->size();      
	
	/////////////////////////////////////// counters of event*digis
	nnnnnn++;  
	//////////////////////////////////  counters of event for subdet & depth
	if(digi->id().subdet()==HcalBarrel && depth == 1 && qwert1==0 ){nnnnnn1++;qwert1=1;}
	if(digi->id().subdet()==HcalBarrel && depth == 2 && qwert2==0 ){nnnnnn2++;qwert2=1;}    
	if(digi->id().subdet()==HcalEndcap && depth == 1 && qwert3==0 ){nnnnnn3++;qwert3=1;}    
	if(digi->id().subdet()==HcalEndcap && depth == 2 && qwert4==0 ){nnnnnn4++;qwert4=1;}    
	if(digi->id().subdet()==HcalEndcap && depth == 3 && qwert5==0 ){nnnnnn5++;qwert5=1;}    
	////////////////////////////////////////////////////////////  for zerrors.C script:
	if(recordHistoes_ && studyCapIDErrorsHist_) fillDigiErrors(digi); 
	//////////////////////////////////////  for ztsmaxa.C,zratio34.C,zrms.C & zdifampl.C scripts:
	if(recordHistoes_) fillDigiAmplitude(digi); 
	  
	if(recordHistoes_ && studyCalibCellsHist_) {
	  int iphi  = phi-1;
	  int ieta  = eta;
	  if(ieta > 0) ieta -= 1;
	  if(digi->id().subdet()==HcalBarrel) 
	    {                 
	      if(nTS<=numOfTS) for(int i=0;i<nTS;i++) {
		TS_data[i]=adc2fC[digi->sample(i).adc()];
		signal[0][ieta+41][iphi] += TS_data[i];
		if(i>1&&i<6) signal3[0][ieta+41][iphi] += TS_data[i];
	      } 
	    } 
	  
	  if(digi->id().subdet()==HcalEndcap)
	    {  
	      if(nTS<=numOfTS) for(int i=0;i<nTS;i++) {
		TS_data[i]=adc2fC[digi->sample(i).adc()];
		signal[1][ieta+41][iphi] += TS_data[i];
		if(i>1&&i<6) signal3[1][ieta+41][iphi] += TS_data[i];
	      } 
	    } 
	}//if(recordHistoes_ && studyCalibCellsHist_) 
	
	////////////////////////////////////////////////////////////////  for Olga's script:
	if(recordNtuples_ && nevent50 < maxNeventsInNtuple_) {
	  
	  if(digi->id().subdet()==HcalBarrel) 
	    {                 
	      if(nTS<=numOfTS) for(int i=0;i<nTS;i++) {
		TS_data[i]=adc2fC[digi->sample(i).adc()];
// Note: ieta+-15 and +-16 are treated for HB separately.
		if(abs(eta) == 15 || abs(eta) == 16) {
		  if(eta == 15) hb15[1][depth-1][phi-1][i] = TS_data[i];
		  if(eta == 16) hb16[1][depth-1][phi-1][i] = TS_data[i];
		  if(eta == -15) hb15[0][depth-1][phi-1][i] = TS_data[i];
		  if(eta == -16) hb16[0][depth-1][phi-1][i] = TS_data[i];
		} else {
		  hb[eta+14][0][phi-1][i] = TS_data[i];
		}
	      } // TS
	    } // HBDigi  
	  
	  if(digi->id().subdet()==HcalEndcap)
	    {  
	      if(nTS<=numOfTS) for(int i=0;i<nTS;i++) {
		TS_data[i]=adc2fC[digi->sample(i).adc()];
		if( abs(eta) == 16 ) {
// Note: eta +-16 is treated for HE separately
// For eta +-16 there is one depth in HE (behind 2 HB depths): 3 depths in HBHE in total
		  if(eta == 16) he16[1][0][phi-1][i] = TS_data[i];
		  if(eta == -16) he16[0][0][phi-1][i] = TS_data[i];
		} else { // eta =16
		  int jeta = abs(eta);
		  if(eta>0) jeta = eta-4;
		  if(eta<0) jeta = eta+29;
		  if (verbosity == -22) std::cout<<" HE "<<eta<<" "<<jeta<<std::endl;
		  he[jeta][depth-1][phi-1][i] = TS_data[i];
		} // eta!=16
	      } // TS
	    }// HcalEndcap                                                                    
	}//if(recordNtuples_)
      }// for HBHE digis
  }//hbhe.isValid
  
  ///////////////////////////////////////////////////   HODigiCollection
  edm::Handle<HODigiCollection> ho; 
  iEvent.getByToken(tok_ho_,ho);
  
  bool gotHODigis=true;
  if (!(iEvent.getByToken(tok_ho_,ho)))
    {
      gotHODigis=false; //this is a boolean set up to check if there are HOgigis in input root file
    }
  if (!(ho.isValid()))
    {
      gotHODigis=false; //if it is not there, leave it false
    }
  
  if(!gotHODigis) {
    //  if(!ho.isValid()) {
    cout<<" No HO collection is found "<<endl;
  } else {
    
    int qwert6= 0; 
    for(HODigiCollection::const_iterator digi=ho->begin();digi!=ho->end();digi++){
      eta=digi->id().ieta(); phi=digi->id().iphi(); depth=digi->id().depth(); nTS=digi->size();
      ///////////////////
      counterho++;
	//////////////////////////////////  counters of event
      if(qwert6==0 ){nnnnnn6++;qwert6=1;}    
      ////////////////////////////////////////////////////////////  for zerrors.C script:
      if(recordHistoes_ && studyCapIDErrorsHist_) fillDigiErrorsHO(digi); 

      //////////////////////////////////////  for ztsmaxa.C,zratio34.C,zrms.C & zdifampl.C scripts:
      if(recordHistoes_) fillDigiAmplitudeHO(digi); 
      
      ///////////////////
      if(recordHistoes_ && studyCalibCellsHist_) {
	int iphi  = phi-1;
	int ieta  = eta;
	if(ieta > 0) ieta -= 1;
	//      double amplitude = 0;
	if(nTS<=numOfTS) for(int i=0;i<nTS;i++) {
	  TS_data[i]=adc2fC[digi->sample(i).adc()];
	  //	amplitude += TS_data[i];
	  signal[2][ieta+41][iphi] += TS_data[i];
	  if(i>1&&i<6) signal3[2][ieta+41][iphi] += TS_data[i];
	}//if for
      }//if(recordHistoes_ && studyCalibCellsHist_) 
    }//for HODigiCollection
  }//ho.isValid(

  ///////////////////////////////////////////////  
  ///////////////////////////////////////////////  
  //////////// k0(sub): =0 HB; =1 HE; =2 HO; =3 HF;
  //////////// k1(depth-1): = 0 - 3 or depth: = 1 - 4;

  int maxtestmetka=550;
  int testcount1=0;
  int testcount2=0;
  int testcount3=0;

  for(int k0 = 0; k0<4; k0++) {
    int sumofchannels = 0;
    double sumamplitudesubdet = 0.;	
    int sumofchannels0 = 0;
    double sumamplitudesubdet0 = 0.;	
    if (verbosity == -9129 && k0==3) testmetka++;
    if (verbosity == -9129 && k0==3 && testmetka== maxtestmetka) std::cout << " ==================================================================================== =  " << std::endl;

    //  for(int k1 = 0; k1<4; k1++) {
    for(int k2 = 0; k2<82; k2++) {
      if (verbosity == -9814 && k0==3) std::cout << "===loop   lscounterM1 =  " <<lscounterM1 <<"k3=70 for   sumamplitudechannel_HF depth1 =  " <<amplitudechannel[k0][0][k2][70] <<"k3=70 for   sumamplitudechannel_HF depth2=  " <<amplitudechannel[k0][1][k2][70] << "   k2 =  " <<k2 << std::endl;

      for(int k3 = 0; k3<72; k3++) {

	if (verbosity == -9129 && k0==3 && testmetka== maxtestmetka && (amplitudechannel[k0][0][k2][k3] !=0. || amplitudechannel[k0][1][k2][k3] !=0.)  ) {
	  std::cout << "===loop   lscounterM1 =  " <<lscounterM1 <<" sumamplitudechannel_HF depth1 =  " <<amplitudechannel[k0][0][k2][k3] <<" sumamplitudechannel_HF depth2=  " <<amplitudechannel[k0][1][k2][k3] << " k2 =  " <<k2 << " k3 =  " <<k3 << std::endl;
	  
	  if(amplitudechannel[k0][0][k2][k3]!=0. && amplitudechannel[k0][1][k2][k3]!=0.) testcount1++;
	  
	  if(amplitudechannel[k0][0][k2][k3]!=0. && amplitudechannel[k0][1][k2][k3]==0.) testcount2++;
	  if(amplitudechannel[k0][0][k2][k3]!=0. && amplitudechannel[k0][1][k2][k3]==0.) std::cout << "===CASE 2:   lscounterM1 =  " <<lscounterM1 <<" sumamplitudechannel_HF depth1 =  " <<amplitudechannel[k0][0][k2][k3] <<" sumamplitudechannel_HF depth2=  " <<amplitudechannel[k0][1][k2][k3] << " k2 =  " <<k2 << " k3 =  " <<k3 << std::endl;
	  
	  if(amplitudechannel[k0][0][k2][k3]==0. && amplitudechannel[k0][1][k2][k3]!=0.) testcount3++;
	  if(amplitudechannel[k0][0][k2][k3]==0. && amplitudechannel[k0][1][k2][k3]!=0.) std::cout << "===CASE 3:   lscounterM1 =  " <<lscounterM1 <<" sumamplitudechannel_HF depth1 =  " <<amplitudechannel[k0][0][k2][k3] <<" sumamplitudechannel_HF depth2=  " <<amplitudechannel[k0][1][k2][k3] << " k2 =  " <<k2 << " k3 =  " <<k3 << std::endl;
	}       
	// HB
	if( k0 == 0 ) {
	  double sumamplitudechannel_HB = amplitudechannel[k0][0][k2][k3]+amplitudechannel[k0][1][k2][k3];
	  h_sumamplitudechannel_HB->Fill(sumamplitudechannel_HB);
	  if( sumamplitudechannel_HB   >      80.) {
	    sumamplitudesubdet += sumamplitudechannel_HB;
	    sumofchannels++;
	  }
	  else {
	    if( sumamplitudechannel_HB   >  0.) {
	      sumamplitudesubdet0 += sumamplitudechannel_HB;
	      sumofchannels0++;
	    }
	  }
	}//
	// HE
	if( k0 == 1 ) {
	  double sumamplitudechannel_HE = amplitudechannel[k0][0][k2][k3]+amplitudechannel[k0][1][k2][k3]+amplitudechannel[k0][2][k2][k3];
	  h_sumamplitudechannel_HE->Fill(sumamplitudechannel_HE);
	  if( sumamplitudechannel_HE   >     200.) {
	    sumamplitudesubdet += sumamplitudechannel_HE;
	    sumofchannels++;
	  }
	  else {
	    if( sumamplitudechannel_HE   >  0.) {
	      sumamplitudesubdet0 += sumamplitudechannel_HE;
	      sumofchannels0++;
	    }
	  }
	}//
	// HO
	if( k0 == 2 ) {
	  double sumamplitudechannel_HO = amplitudechannel[k0][3][k2][k3];
	  h_sumamplitudechannel_HO->Fill(sumamplitudechannel_HO);
	  if( sumamplitudechannel_HO   >    1200.) {
	    sumamplitudesubdet += sumamplitudechannel_HO;
	    sumofchannels++;
	  }
	  else {
	    if( sumamplitudechannel_HO   >  0.) {
	      sumamplitudesubdet0 += sumamplitudechannel_HO;
	      sumofchannels0++;
	    }
	  }
	}//
	// HF
	if( k0 == 3 ) {
	  double sumamplitudechannel_HF = amplitudechannel[k0][0][k2][k3]+amplitudechannel[k0][1][k2][k3];
	  h_sumamplitudechannel_HF->Fill(sumamplitudechannel_HF);
	  if( sumamplitudechannel_HF   >     600.) {
	    sumamplitudesubdet += sumamplitudechannel_HF;
	    sumofchannels++;
	  }
	  else {
      if (verbosity == -9814 ) std::cout << "===loop   lscounterM1 =  " <<lscounterM1 <<"sumamplitudechannel_HF =  " <<sumamplitudechannel_HF << "   k2 =  " <<k2 << "   k3 =  " <<k3 << "   sumofchannels0 =  " <<sumofchannels0 << std::endl;

	    if( sumamplitudechannel_HF   >  0.) {
	      sumamplitudesubdet0 += sumamplitudechannel_HF;
	      sumofchannels0++;
	    }
	  }
	}//
	
	
      }//k3
    }//k2

    if (verbosity == -9129 && k0==3 && testmetka== maxtestmetka) {
      std::cout << " ==================================================================================== =  " << std::endl;
      std::cout << "===FINAL:   testcount1 =  " <<testcount1 <<" testcount2 =  " <<testcount2 <<" testcount3=  " <<testcount3 << std::endl;
    }
      
    //  }//k1


    // SA of each sub-detector DONE. Then: summarize or find maximum throught events of LS
    if( k0 == 0 ) { 
      h_eventamplitude_HB->Fill((sumamplitudesubdet+sumamplitudesubdet0));
      h_eventoccupancy_HB->Fill((sumofchannels+sumofchannels0));
      if( (sumamplitudesubdet+sumamplitudesubdet0) > maxxSUM1) maxxSUM1 = sumamplitudesubdet+sumamplitudesubdet0;
      if( (sumofchannels+sumofchannels0) > maxxOCCUP1) maxxOCCUP1 = sumofchannels+sumofchannels0;
      averSIGNALoccupancy_HB+=sumofchannels;
      averSIGNALsumamplitude_HB+=sumamplitudesubdet;
      averNOSIGNALoccupancy_HB+=sumofchannels0;
      averNOSIGNALsumamplitude_HB+=sumamplitudesubdet0;
      //	if (verbosity == -9812 ) std::cout << "===loop   lscounterM1 =  " <<lscounterM1 <<"   maxxSUM1 =  " <<maxxSUM1 << "   sumamplitudesubdet0 =  " <<sumamplitudesubdet0 << std::endl;
      if((sumamplitudesubdet+sumamplitudesubdet0)>60000) {
	for(int k2 = 0; k2<82; k2++) {
	  for(int k3 = 0; k3<72; k3++) {
	    int ieta = k2-41;
	    /// HB depth1:
	    if(amplitudechannel[k0][0][k2][k3]!=0.){h_2DAtaildepth1_HB->Fill(double(ieta),double(k3),amplitudechannel[k0][0][k2][k3]);h_2D0Ataildepth1_HB->Fill(double(ieta),double(k3),1.);}
	    /// HB depth2:
	    if(amplitudechannel[k0][1][k2][k3]!=0.){h_2DAtaildepth2_HB->Fill(double(ieta),double(k3),amplitudechannel[k0][1][k2][k3]);h_2D0Ataildepth2_HB->Fill(double(ieta),double(k3),1.);}
	  }//for
	}//for
      }//>60000
    }//HB
    if( k0 == 1 ) { 
      h_eventamplitude_HE->Fill((sumamplitudesubdet+sumamplitudesubdet0));
      h_eventoccupancy_HE->Fill((sumofchannels+sumofchannels0));
      if( (sumamplitudesubdet+sumamplitudesubdet0) > maxxSUM2) maxxSUM2 = sumamplitudesubdet+sumamplitudesubdet0;
      if( (sumofchannels+sumofchannels0) > maxxOCCUP2) maxxOCCUP2 = sumofchannels+sumofchannels0;
      averSIGNALoccupancy_HE+=sumofchannels;
      averSIGNALsumamplitude_HE+=sumamplitudesubdet;
      averNOSIGNALoccupancy_HE+=sumofchannels0;
      averNOSIGNALsumamplitude_HE+=sumamplitudesubdet0;
    }//HE
    if( k0 == 2 ) { 
      h_eventamplitude_HO->Fill((sumamplitudesubdet+sumamplitudesubdet0));
      h_eventoccupancy_HO->Fill((sumofchannels+sumofchannels0));
      if( (sumamplitudesubdet+sumamplitudesubdet0) > maxxSUM3) maxxSUM3 = sumamplitudesubdet+sumamplitudesubdet0;
      if( (sumofchannels+sumofchannels0) > maxxOCCUP3) maxxOCCUP3 = sumofchannels+sumofchannels0;
      averSIGNALoccupancy_HO+=sumofchannels;
      averSIGNALsumamplitude_HO+=sumamplitudesubdet;
      averNOSIGNALoccupancy_HO+=sumofchannels0;
      averNOSIGNALsumamplitude_HO+=sumamplitudesubdet0;
    }//HO
    if( k0 == 3 ) { 
      h_eventamplitude_HF->Fill((sumamplitudesubdet+sumamplitudesubdet0));
      h_eventoccupancy_HF->Fill((sumofchannels+sumofchannels0));
      if( (sumamplitudesubdet+sumamplitudesubdet0) > maxxSUM4) maxxSUM4 = sumamplitudesubdet+sumamplitudesubdet0;
      if( (sumofchannels+sumofchannels0) > maxxOCCUP4) maxxOCCUP4 = sumofchannels+sumofchannels0;
      averSIGNALoccupancy_HF+=sumofchannels;
      averSIGNALsumamplitude_HF+=sumamplitudesubdet;
      averNOSIGNALoccupancy_HF+=sumofchannels0;
      averNOSIGNALsumamplitude_HF+=sumamplitudesubdet0;
    }//HF
    if (verbosity == -9814 && k0==3) std::cout << "===loop   lscounterM1 =  " <<lscounterM1 <<"    sumofchannels0 =  " <<sumofchannels0 <<"    maxxSUM4 =  " <<maxxSUM4 << std::endl;
    
  }//k0
  
  ///////////////////// ///////////////////// //////////////////////////////////////////
  ///////////////////////////////////////////////  for zRunRatio34.C & zRunNbadchan.C scripts:
  if(recordHistoes_ && studyRunDependenceHist_) {
    int eeeeee;
    eeeeee = lscounterM1;
    if(flagtoaskrunsorls_ == 0 ) eeeeee = runcounter;
    //////////// k0(sub): =0 HB; =1 HE; =2 HO; =3 HF;
    //////////// k1(depth-1): = 0 - 3 or depth: = 1 - 4;
    for(int k0 = 0; k0<4; k0++) {
      for(int k1 = 0; k1<4; k1++) {
//	if(k0 ==0 && k1>=2) break;
//	if(k0 ==1 && k1==3) break;
//	if(k0 ==2 && k1< 3) break;
//	if(k0 ==3 && k1>=2) break;
	
	//////////
	int nbadchannels = 0;	
	for(int k2 = 0; k2<82; k2++) {
	  for(int k3 = 0; k3<72; k3++) {
	    if(badchannels[k0][k1][k2][k3] !=0) ++nbadchannels;
	  }//k3
	}//k2
	if (verbosity == -12 && k0 == 2 && k1 == 3 ) {
//	  std::cout << " ********   k0 = " << k0 << "  k1 = " << k1 << std::endl;
	  std::cout << " nbadchannels = " << nbadchannels << std::endl;
	}
	//////////
	//HB
	if(k0 == 0) {
	  if(k1 == 0) {
	    if(nbadchannels>lsdep_cut1_peak_HBdepth1_ && verbosity == -77) {
	      cout  <<  " HB-depth1:************  nbadchannels =  "  << nbadchannels <<  endl;
	      cout  <<  " Run =  "  << Run <<  " runcounter =  "  << runcounter <<  endl;
	      cout  <<  " LS  =  "  << lumi <<  " lscounterrun =  "  << lscounterrun <<  " lscounterM1 =  "  << lscounterM1 <<  endl;
	      cout  <<  " eventNumber =  "  << Nevent <<  " eventcounterLS =  "  << nevcounter <<  " eventcounterRUN =  "  << eventcounter <<  endl;
	      cout  <<  " ---------------------------------------------------------------  "  <<  endl;
	    }
	    h_nbadchannels_depth1_HB->Fill(float(nbadchannels) );
	    h_runnbadchannels_depth1_HB->Fill( float(eeeeee) ,float(nbadchannels) );
	    if(nbadchannels > lsdep_cut1_peak_HBdepth1_) h_runnbadchannelsC_depth1_HB->Fill(float(eeeeee) ,float(nbadchannels) );
	    if(nbadchannels != 0 ) h_runbadrate_depth1_HB->Fill(float(eeeeee) , 1. );
	    if(nbadchannels > lsdep_cut3_max_HBdepth1_) h_runbadrateC_depth1_HB->Fill(float(eeeeee),1.);
	    h_bcnnbadchannels_depth1_HB->Fill( float(bcn) ,float(nbadchannels) );
	    h_runbadrate0_depth1_HB->Fill( float(eeeeee),1.);
	    h_bcnbadrate0_depth1_HB->Fill( float(bcn) , 1. );
	  }
	  if(k1 == 1) { 
	    if(nbadchannels>lsdep_cut1_peak_HBdepth2_ && verbosity == -77) {
	      cout  <<  " HB-depth2:************  nbadchannels =  "  << nbadchannels <<  endl;
	      cout  <<  " Run =  "  << Run <<  " runcounter =  "  << runcounter <<  endl;
	      cout  <<  " LS  =  "  << lumi <<  " lscounterrun =  "  << lscounterrun <<  " lscounterM1 =  "  << lscounterM1 <<  endl;
	      cout  <<  " eventNumber =  "  << Nevent <<  " eventcounterLS =  "  << nevcounter <<  " eventcounterRUN =  "  << eventcounter <<  endl;
	      cout  <<  " ---------------------------------------------------------------  "  <<  endl;
	    }
	    h_nbadchannels_depth2_HB->Fill( float(nbadchannels) );
	    h_runnbadchannels_depth2_HB->Fill( float(eeeeee) , float(nbadchannels) );
	    if(nbadchannels > lsdep_cut1_peak_HBdepth2_) h_runnbadchannelsC_depth2_HB->Fill(float(eeeeee) ,float(nbadchannels) );
	    if(nbadchannels != 0 ) h_runbadrate_depth2_HB->Fill( float(eeeeee) , 1.);
	    if(nbadchannels > lsdep_cut3_max_HBdepth2_) h_runbadrateC_depth2_HB->Fill(float(eeeeee),1.);
	    h_bcnnbadchannels_depth2_HB->Fill( float(bcn) ,float(nbadchannels) );
	    h_runbadrate0_depth2_HB->Fill( float(eeeeee) , 1. );
	    h_bcnbadrate0_depth2_HB->Fill( float(bcn) , 1. );
	  }
	}////if(k0 == 0)
	//HE
	if(k0 == 1) {
	  if(k1 == 0) { 
	    if(nbadchannels>lsdep_cut1_peak_HEdepth1_ && verbosity == -77) {
	      cout  <<  " HE-depth1:************  nbadchannels =  "  << nbadchannels <<  endl;
	      cout  <<  " Run =  "  << Run <<  " runcounter =  "  << runcounter <<  endl;
	      cout  <<  " LS  =  "  << lumi <<  " lscounterrun =  "  << lscounterrun <<  " lscounterM1 =  "  << lscounterM1 <<  endl;
	      cout  <<  " eventNumber =  "  << Nevent <<  " eventcounterLS =  "  << nevcounter <<  " eventcounterRUN =  "  << eventcounter <<  endl;
	      cout  <<  " ---------------------------------------------------------------  "  <<  endl;
	    }
	    h_nbadchannels_depth1_HE->Fill( float(nbadchannels) );
	    h_runnbadchannels_depth1_HE->Fill( float(eeeeee) , float(nbadchannels) );
	    if(nbadchannels > lsdep_cut1_peak_HEdepth1_) h_runnbadchannelsC_depth1_HE->Fill(float(eeeeee) ,float(nbadchannels) );
	    if(nbadchannels != 0 ) h_runbadrate_depth1_HE->Fill( float(eeeeee) , 1.);
	    if(nbadchannels > lsdep_cut3_max_HEdepth1_) h_runbadrateC_depth1_HE->Fill(float(eeeeee),1.);
	    h_bcnnbadchannels_depth1_HE->Fill( float(bcn) ,float(nbadchannels) );
	    h_runbadrate0_depth1_HE->Fill( float(eeeeee) , 1. );
	    h_bcnbadrate0_depth1_HE->Fill( float(bcn) , 1. );
	  }
	  if(k1 == 1) { 
	    if(nbadchannels>lsdep_cut1_peak_HEdepth2_ && verbosity == -77) {
	      cout  <<  " HE-depth2:************  nbadchannels =  "  << nbadchannels <<  endl;
	      cout  <<  " Run =  "  << Run <<  " runcounter =  "  << runcounter <<  endl;
	      cout  <<  " LS  =  "  << lumi <<  " lscounterrun =  "  << lscounterrun <<  " lscounterM1 =  "  << lscounterM1 <<  endl;
	      cout  <<  " eventNumber =  "  << Nevent <<  " eventcounterLS =  "  << nevcounter <<  " eventcounterRUN =  "  << eventcounter <<  endl;
	      cout  <<  " ---------------------------------------------------------------  "  <<  endl;
	    }
	    h_nbadchannels_depth2_HE->Fill( float(nbadchannels) );
	    h_runnbadchannels_depth2_HE->Fill( float(eeeeee) , float(nbadchannels) );
	    if(nbadchannels > lsdep_cut1_peak_HEdepth2_) h_runnbadchannelsC_depth2_HE->Fill(float(eeeeee) ,float(nbadchannels) );
	    if(nbadchannels != 0 ) h_runbadrate_depth2_HE->Fill( float(eeeeee) , 1.);
	    if(nbadchannels > lsdep_cut3_max_HEdepth2_) h_runbadrateC_depth2_HE->Fill(float(eeeeee),1.);
	    h_bcnnbadchannels_depth2_HE->Fill( float(bcn) ,float(nbadchannels) );
	    h_runbadrate0_depth2_HE->Fill( float(eeeeee) , 1. );
	    h_bcnbadrate0_depth2_HE->Fill( float(bcn) , 1. );
	  }
	  if(k1 == 2) { 
	    if(nbadchannels>lsdep_cut1_peak_HEdepth3_ && verbosity == -77) {
	      cout  <<  " HE-depth3:************  nbadchannels =  "  << nbadchannels <<  endl;
	      cout  <<  " Run =  "  << Run <<  " runcounter =  "  << runcounter <<  endl;
	      cout  <<  " LS  =  "  << lumi <<  " lscounterrun =  "  << lscounterrun <<  " lscounterM1 =  "  << lscounterM1 <<  endl;
	      cout  <<  " eventNumber =  "  << Nevent <<  " eventcounterLS =  "  << nevcounter <<  " eventcounterRUN =  "  << eventcounter <<  endl;
	      cout  <<  " ---------------------------------------------------------------  "  <<  endl;
	    }
	    h_nbadchannels_depth3_HE->Fill( float(nbadchannels) );
	    h_runnbadchannels_depth3_HE->Fill( float(eeeeee) , float(nbadchannels) );
	    if(nbadchannels > lsdep_cut1_peak_HEdepth3_) h_runnbadchannelsC_depth3_HE->Fill(float(eeeeee) ,float(nbadchannels) );
	    if(nbadchannels != 0 ) h_runbadrate_depth3_HE->Fill( float(eeeeee) , 1.);
	    if(nbadchannels > lsdep_cut3_max_HEdepth3_) h_runbadrateC_depth3_HE->Fill(float(eeeeee),1.);
	    h_bcnnbadchannels_depth3_HE->Fill( float(bcn) ,float(nbadchannels) );
	    h_runbadrate0_depth3_HE->Fill( float(eeeeee) , 1. );
	    h_bcnbadrate0_depth3_HE->Fill( float(bcn) , 1. );
	  }
	}////if(k0 == 1)
	//HO
	if(k0 == 2) {
	  if(k1 == 3) { 
	    h_nbadchannels_depth4_HO->Fill( float(nbadchannels) );
	    h_runnbadchannels_depth4_HO->Fill( float(eeeeee) , float(nbadchannels) );
	    if(nbadchannels > lsdep_cut1_peak_HOdepth4_) h_runnbadchannelsC_depth4_HO->Fill(float(eeeeee) ,float(nbadchannels) );
	    if(nbadchannels != 0 ) h_runbadrate_depth4_HO->Fill( float(eeeeee) , 1.);
	    if(nbadchannels > lsdep_cut3_max_HOdepth4_) h_runbadrateC_depth4_HO->Fill(float(eeeeee),1.);
	    h_bcnnbadchannels_depth4_HO->Fill( float(bcn) ,float(nbadchannels) );
	    h_runbadrate0_depth4_HO->Fill( float(eeeeee) , 1. );
	    h_bcnbadrate0_depth4_HO->Fill( float(bcn) , 1. );
	  }
	}////if(k0 == 2)
	//HF
	if(k0 == 3) {
	  if(k1 == 0) { 
	    h_nbadchannels_depth1_HF->Fill( float(nbadchannels) );
	    h_runnbadchannels_depth1_HF->Fill( float(eeeeee) , float(nbadchannels) );
	    if(nbadchannels > lsdep_cut1_peak_HFdepth1_) h_runnbadchannelsC_depth1_HF->Fill(float(eeeeee) ,float(nbadchannels) );
	    if(nbadchannels != 0 ) h_runbadrate_depth1_HF->Fill( float(eeeeee) , 1.);
	    if(nbadchannels > lsdep_cut3_max_HFdepth1_) h_runbadrateC_depth1_HF->Fill(float(eeeeee),1.);
	    h_bcnnbadchannels_depth1_HF->Fill( float(bcn) ,float(nbadchannels) );
	    h_runbadrate0_depth1_HF->Fill( float(eeeeee) , 1. );
	    h_bcnbadrate0_depth1_HF->Fill( float(bcn) , 1. );
	  }
	  if(k1 == 1) { 
	    h_nbadchannels_depth2_HF->Fill( float(nbadchannels) );
	    h_runnbadchannels_depth2_HF->Fill( float(eeeeee) , float(nbadchannels) );
	    if(nbadchannels > lsdep_cut1_peak_HFdepth2_) h_runnbadchannelsC_depth2_HF->Fill(float(eeeeee) ,float(nbadchannels) );
	    if(nbadchannels != 0 ) h_runbadrate_depth2_HF->Fill( float(eeeeee) , 1.);
	    if(nbadchannels > lsdep_cut3_max_HFdepth2_) h_runbadrateC_depth2_HF->Fill(float(eeeeee),1.);
	    h_bcnnbadchannels_depth2_HF->Fill( float(bcn) ,float(nbadchannels) );
	    h_runbadrate0_depth2_HF->Fill( float(eeeeee) , 1. );
	    h_bcnbadrate0_depth2_HF->Fill( float(bcn) , 1. );
	  }
	}////if(k0 == 3)
	
	//////////
      }//k1
    }//k0
    ////////////
  }//if(recordHistoes_&& studyRunDependenceHist_) 
  
  /////////////////////////////////////////////////// HcalCalibDigiCollection

  for(int k1 = 0; k1<40; k1++) {
    for(int k2 = 0; k2<5; k2++ ) {
      myCalEta[k2][k1] = 0.;
      myCalPhi[k2][k1] = 0.;
    }
    for(int k2 = 0; k2<15; k2++) {  
      
      hb_calibration0[k1][k2] = 0.;
      hb_calibration1[k1][k2] = 0.;    
      
      he_calibration0[k1][k2] = 0.;
      he_calibration1[k1][k2] = 0.;
      
      ho_calibration0[k1][k2] = 0.;
      ho_calibration1[k1][k2] = 0.;
      
      hf_calibration0[k1][k2] = 0.;
      hf_calibration1[k1][k2] = 0.;
      
    } // k2
  } // k1 
  edm::Handle<HcalCalibDigiCollection> calib; 
  iEvent.getByToken(tok_calib_,calib);
  
  bool gotCALIBDigis=true;
  if (!(iEvent.getByToken(tok_calib_,calib)))
    {
      gotCALIBDigis=false; //this is a boolean set up to check if there are CALIBgigis in input root file
    }
  if (!(calib.isValid()))
    {
      gotCALIBDigis=false; //if it is not there, leave it false
    }
  if(!gotCALIBDigis) {
    cout<<" No HcalCalibDigiCollection collection is found "<<endl;
  } else {
    for(HcalCalibDigiCollection::const_iterator digi=calib->begin();digi!=calib->end();digi++){
      int cal_det=digi->id().hcalSubdet(); // 1-HB,2-HE,3-HO,4-HF
      int cal_phi=digi->id().iphi();
      int cal_eta=digi->id().ieta();
      int cal_cbox=digi->id().cboxChannel();

      /////////////////////////////////////////////     
      if(recordHistoes_ && studyCalibCellsHist_) {
	if(cal_det>0 && cal_det<5 && cal_cbox == 0 ) {
	  int iphi  = cal_phi-1;
	  int ieta  = cal_eta;
	  if(ieta > 0) ieta -= 1;
	  nTS=digi->size();
	  double max_signal = -100.;
	  int ts_with_max_signal = -100;
	  double timew = 0.;

	  //
	  if(nTS<=numOfTS) for(int i=0;i<nTS;i++) {
	    double ampldefault = adc2fC[digi->sample(i).adc()&0xff];
	    if(max_signal < ampldefault ) {
	      max_signal = ampldefault;
	      ts_with_max_signal = i;
	    }
	    if(i>1&&i<6) calib3[cal_det-1][ieta+41][iphi] += ampldefault;
	    calib0[cal_det-1][ieta+41][iphi] += ampldefault;
	    timew += (i+1)*ampldefault;
	  }// for
	  //

	  double amplitude = calib0[cal_det-1][ieta+41][iphi];
	  double aveamplitude = -100.;
	  if(amplitude >0 && timew >0)  aveamplitude = timew/amplitude;// average_TS +1
	  double aveamplitude1 = aveamplitude-1;// means iTS=0-9
	  caliba[cal_det-1][ieta+41][iphi] = aveamplitude1;

	  double rmsamp = 0.;
	  for (int ii=0; ii<10; ii++) {  
	    double ampldefault = adc2fC[digi->sample(ii).adc()&0xff];
	    double aaaaaa = (ii+1)-aveamplitude;
	    double aaaaaa2 = aaaaaa*aaaaaa;
	    rmsamp+=(aaaaaa2*ampldefault);// fC
	  }//for 2
	  double rmsamplitude = -100.;
	  if( (amplitude >0 && rmsamp >0) || (amplitude <0 && rmsamp <0))  rmsamplitude = sqrt( rmsamp/amplitude );
	  calibw[cal_det-1][ieta+41][iphi] = rmsamplitude;
	  //
	  calibt[cal_det-1][ieta+41][iphi] = ts_with_max_signal;
	  //

	  if(ts_with_max_signal > -1 && ts_with_max_signal < nTS) calib2[cal_det-1][ieta+41][iphi] = adc2fC[digi->sample(ts_with_max_signal).adc()&0xff];
	  if(ts_with_max_signal+1 > -1 && ts_with_max_signal+1 < nTS) calib2[cal_det-1][ieta+41][iphi] += adc2fC[digi->sample(ts_with_max_signal+1).adc()&0xff];
	  if(ts_with_max_signal+2 > -1 && ts_with_max_signal+2 < nTS) calib2[cal_det-1][ieta+41][iphi] += adc2fC[digi->sample(ts_with_max_signal+2).adc()&0xff];
	  if(ts_with_max_signal-1 > -1 && ts_with_max_signal-1 < nTS) calib2[cal_det-1][ieta+41][iphi] += adc2fC[digi->sample(ts_with_max_signal-1).adc()&0xff];
	  if(ts_with_max_signal-2 > -1 && ts_with_max_signal-2 < nTS) calib2[cal_det-1][ieta+41][iphi] += adc2fC[digi->sample(ts_with_max_signal-2).adc()&0xff];
	  //
	  if (verbosity == -11) {
	    cout<<"*******  det= " << cal_det << " cbox = " << cal_cbox << endl;
	    cout<< " phi= " << cal_phi << " eta= " << cal_eta << endl;
	    cout<< " iphi= " << iphi << " ieta= " << ieta << endl;
	    cout<< " TS0= " << adc2fC[digi->sample(0).adc()&0xff] 
		<< " TS1= " << adc2fC[digi->sample(1).adc()&0xff] 
		<< " TS2= " << adc2fC[digi->sample(2).adc()&0xff] 
		<< " TS3= " << adc2fC[digi->sample(3).adc()&0xff] 
		<< " TS4= " << adc2fC[digi->sample(4).adc()&0xff] 
		<< " TS5= " << adc2fC[digi->sample(5).adc()&0xff] << endl;
	  }

	  bool anycapid   =  true;
	  bool anyer      =  false;
	  bool anydv      =  true;
	  int error1=0,error2=0,error3=0; 
	  int lastcapid=0, capid=0;
	  for  (int ii=0;ii<(*digi).size();ii++) {
	    capid = (*digi)[ii].capid(); // capId (0-3, sequential)
	    bool er    = (*digi)[ii].er();   // error
	    bool dv    = (*digi)[ii].dv();  // valid data
	    if(ii!=0 && ((lastcapid+1)%4)!=capid) anycapid =  false;    
	    lastcapid=capid;  
	    if(er) anyer =  true;    
	    if(!dv) anydv =  false;    
	  }
	  if( !anycapid ) error1 = 1; 
	  if( anyer )     error2 = 1; 
	  if( !anydv )    error3 = 1; 
	  if(error1 !=0 || error2 !=0 || error3 !=0 ) calibcapiderror[cal_det-1][ieta+41][iphi] = 100;
	  if(verbosity == -233 &&cal_det==1 && calibcapiderror[cal_det-1][ieta+41][iphi] ==100) {
	    cout << "in Coolection  ieta+41= " << ieta+41 << " iphi= " << iphi << endl;
	  }
	  
	}// if(cal_det>0 && cal_det<5
      }//if(recordHistoes_ && studyCalibCellsHist_)
      ///////////////////////////////////////////// 
      
      
      if(recordNtuples_ && nevent50 < maxNeventsInNtuple_) {    
	//      if(recordNtuples_ && nevent < maxNeventsInNtuple_) {    
	if (cal_cbox<2){
	  nTS=digi->size();
	  if(nTS<=numOfTS) for(int i=0;i<nTS;i++) {
	    TS_cal[i]=adc2fC[digi->sample(i).adc()&0xff];   
	    int cal_RBX = getRBX(cal_det,cal_eta,cal_phi);
	    if( cal_det == 0 ) continue;   // HcalEmpty - what it is? skip
	    
	    myCalEta[cal_det][cal_RBX] = cal_eta;
	    myCalPhi[cal_det][cal_RBX] = cal_phi;
	    
	    if(cal_det == 1 ) {  
	      if(cal_cbox == 0)  hb_calibration0[cal_RBX][i] = adc2fC[digi->sample(i).adc()&0xff];
	      if(cal_cbox == 1)  hb_calibration1[cal_RBX][i] = adc2fC[digi->sample(i).adc()&0xff];
	    }
	    if(cal_det == 2 ) {
	      if(cal_cbox == 0)  he_calibration0[cal_RBX][i] = adc2fC[digi->sample(i).adc()&0xff];
	      if(cal_cbox == 1)  he_calibration1[cal_RBX][i] = adc2fC[digi->sample(i).adc()&0xff];
	    }
	    if(cal_det == 3 ) {
	      if(cal_cbox == 0) ho_calibration0[cal_RBX][i] = adc2fC[digi->sample(i).adc()&0xff];
	      if(cal_cbox == 1) ho_calibration1[cal_RBX][i] = adc2fC[digi->sample(i).adc()&0xff];
	    }
	  }
	}
	
	if(local_event==5){
	  if (verbosity == -22) {
	    cout<<cal_det<<" "<<cal_cbox<<" "<<cal_phi<<" "<<cal_eta<<" "<<digi->id().cboxChannelString();
	    for(int i=0; i<numOfTS; i++) cout<<" "<<TS_cal[i]<<" ";
	    cout<<endl;
	  }//if
	}// if 
      }//if(recordNtuples_) {

    }//for(HcalCalibDigiCollection
  }//if(calib.isValid(

  if(recordHistoes_ && studyCalibCellsHist_) {
    ////////////////////////////////////for loop for zcalib.C and zgain.C scripts:
    for(int k1 = 0; k1<4; k1++) {
      for(int k2 = 0; k2<82; k2++) {
	for(int k3 = 0; k3<72; k3++) {
	  int k2plot = k2-41;
	  if(flagcpuoptimization_== 0 ) {
	    ////////////////////////////////////////////////////////////////  for zgain.C script:
	    
	    if(signal[k1][k2][k3]>0.) {
	      if(k1==0) {
		h_FullSignal3D_HB->Fill(double(k2plot), double(k3), signal[k1][k2][k3]);
		h_FullSignal3D0_HB->Fill(double(k2plot), double(k3), 1.);    
	      }
	      if(k1==1) {
		h_FullSignal3D_HE->Fill(double(k2plot), double(k3), signal[k1][k2][k3]);
		h_FullSignal3D0_HE->Fill(double(k2plot), double(k3), 1.);    
	      }
	      if(k1==2) {
		h_FullSignal3D_HO->Fill(double(k2plot), double(k3), signal[k1][k2][k3]);
		h_FullSignal3D0_HO->Fill(double(k2plot), double(k3), 1.);    
	      }
	      if(k1==3) {
		h_FullSignal3D_HF->Fill(double(k2plot), double(k3), signal[k1][k2][k3]);
		h_FullSignal3D0_HF->Fill(double(k2plot), double(k3), 1.);    
	      }
	    }

	  }// optimization
	  ////////////////////////////////////////////////////////////////

	  ////////////////////////////////////////////////////////////////  for zcalib.C script:
	  // k2 = 0-81, k3= 0-71
	  // return to real indexes in eta and phi ( k20 and k30)
	  int k20=k2-41;    // k20 = -41 - 40
	  if(k20>0 || k20==0) k20 += 1;// k20 = -41 - -1 and +1 - +41
	  int k30=k3+1; // k30= 1-72
	  
	  // find calibration indexes in eta and phi ( kk2 and kk3) 
	  int kk2=0, kk3=0;
	  if(k1==0 || k1==1) {
	    if(k20>0 ) kk2=1; else kk2=-1;
	    if(k30==71 ||k30==72 || k30==1 || k30==2) kk3=71; else kk3=((k30-3)/4)*4+3;
	  }
	  else if(k1==2){
	    if(abs(k20)<=4){
	      kk2=0;
	      if(k30==71||k30==72||k30==1||k30==2||k30==3||k30==4) kk3=71; else kk3=((k30-5)/6)*6+5;
	    }
	    else{
	      if(abs(k20)>4  && abs(k20)<=10)  kk2=1;
	      if(abs(k20)>10 && abs(k20)<=15)  kk2=2;
	      if(k20<0) kk2=-kk2;
	      if(k30==71 ||k30==72 || (k30>=1 && k30<=10)) kk3=71; else kk3=((k30-11)/12)*12+11;
	    }
	  }
	  else if(k1==3){
	    if(k20>0) kk2=1; else kk2=-1;
	    if(k30>=1  && k30<=18) kk3=1;
	    if(k30>=19 && k30<=36) kk3=19;
	    if(k30>=37 && k30<=54) kk3=37;
	    if(k30>=55 && k30<=72) kk3=55;
	  }
	  // return to indexes in massiv
	  int kkk2  = kk2+41;
	  if(kk2 > 0) kkk2 -= 1;  
	  int kkk3  = kk3;
	  kkk3 -=1;
	  	  
	  if(flagcpuoptimization_== 0 ) {
	    
	    double GetRMSOverNormalizedSignal =-1.;
	    if(signal[k1][k2][k3]>0.&& calib0[k1][kkk2][kkk3]>0.) {
	      GetRMSOverNormalizedSignal = signal[k1][k2][k3]/calib0[k1][kkk2][kkk3];
	      if(k1==0) {
		//		h_GetRMSOverNormalizedSignal_HB->Fill(GetRMSOverNormalizedSignal,1.);
		h_mapGetRMSOverNormalizedSignal_HB->Fill(double(k2plot),double(k3),GetRMSOverNormalizedSignal);
		h_mapGetRMSOverNormalizedSignal0_HB->Fill(double(k2plot),double(k3),1.);
	      }
	      if(k1==1) {
		//		h_GetRMSOverNormalizedSignal_HE->Fill(GetRMSOverNormalizedSignal,1.);
		h_mapGetRMSOverNormalizedSignal_HE->Fill(double(k2plot),double(k3),GetRMSOverNormalizedSignal);
		h_mapGetRMSOverNormalizedSignal0_HE->Fill(double(k2plot),double(k3),1.);
	      }
	      if(k1==2) {
		//		h_GetRMSOverNormalizedSignal_HO->Fill(GetRMSOverNormalizedSignal,1.);
		h_mapGetRMSOverNormalizedSignal_HO->Fill(double(k2plot),double(k3),GetRMSOverNormalizedSignal);
		h_mapGetRMSOverNormalizedSignal0_HO->Fill(double(k2plot),double(k3),1.);
	      }
	      if(k1==3) {
		//		h_GetRMSOverNormalizedSignal_HF->Fill(GetRMSOverNormalizedSignal,1.);
		h_mapGetRMSOverNormalizedSignal_HF->Fill(double(k2plot),double(k3),GetRMSOverNormalizedSignal);
		h_mapGetRMSOverNormalizedSignal0_HF->Fill(double(k2plot),double(k3),1.);
	      }
	    }
	  }//optimization
	  ////////////////////////////////////////////////////////////////  for zcalib....C script:
	  if(signal[k1][k2][k3]>0.) {
	    // ADC
	    double adc =0.;
	    if(calib0[k1][kkk2][kkk3]>0. ) adc = calib0[k1][kkk2][kkk3];
	    // Ratio
	    double ratio =2.;
	    if(calib0[k1][kkk2][kkk3]>0. ) ratio = calib2[k1][kkk2][kkk3]/calib0[k1][kkk2][kkk3];
	    // TSmax
	    float calibtsmax = calibt[k1][kkk2][kkk3]; 
	    // TSmean
	    float calibtsmean = caliba[k1][kkk2][kkk3]; 
	    // Width
	    float calibwidth = calibw[k1][kkk2][kkk3]; 
	    // CapIdErrors
	    float calibcap =  -100.;
	    calibcap =  calibcapiderror[k1][kkk2][kkk3];

	    //                 HB:
	    if(k1==0) {
	      // ADC
	      h_ADCCalib_HB->Fill(adc,1.);
	      h_ADCCalib1_HB->Fill(adc,1.);
	      if(adc < calibrADCHBMin_ || adc > calibrADCHBMax_ ) h_mapADCCalib047_HB->Fill(double(k2plot),double(k3),1.);
	      h_mapADCCalib_HB->Fill(double(k2plot), double(k3), adc);    
	      // Ratio
	      h_RatioCalib_HB->Fill(ratio,1.);
	      if(ratio < calibrRatioHBMin_  || ratio > calibrRatioHBMax_) h_mapRatioCalib047_HB->Fill(double(k2plot),double(k3),1.);
	      h_mapRatioCalib_HB->Fill(double(k2plot), double(k3), ratio);    
	      // TSmax
	      if(calibtsmax > -0.5) {
		h_TSmaxCalib_HB->Fill(calibtsmax,1.);
		if(calibtsmax< calibrTSmaxHBMin_ || calibtsmax> calibrTSmaxHBMax_ ) h_mapTSmaxCalib047_HB->Fill(double(k2plot),double(k3),1.);
		h_mapTSmaxCalib_HB->Fill(double(k2plot), double(k3), calibtsmax); 
	      }   
	      // TSmean
	      if(calibtsmean > -0.5) {
		h_TSmeanCalib_HB->Fill(calibtsmean,1.);
		if(calibtsmean< calibrTSmeanHBMin_ || calibtsmean> calibrTSmeanHBMax_ ) h_mapTSmeanCalib047_HB->Fill(double(k2plot),double(k3),1.);
		h_mapTSmeanCalib_HB->Fill(double(k2plot), double(k3), calibtsmean); 
	      }   
	      // Width
	      if(calibwidth > -0.5) {
		h_WidthCalib_HB->Fill(calibwidth,1.);
		if(calibwidth< calibrWidthHBMin_ || calibwidth> calibrWidthHBMax_ ) h_mapWidthCalib047_HB->Fill(double(k2plot),double(k3),1.);
		h_mapWidthCalib_HB->Fill(double(k2plot), double(k3), calibwidth); 
	      }   
	      // CapIdErrors
	      if(calibcap > 0) h_mapCapCalib047_HB->Fill(double(k2plot),double(k3),1.);

	      if(verbosity == -233 && k3==51 && abs(k2plot)==5  && calibcap > 0) {
		cout << "******************************Initial indexes:  k2= " << k2 << "  k2plot= " << k2plot << " k3= " << k3 << " k1= " << k1 << endl;
		cout << "real indexes  k20= " << k20 << " k30= " << k30 << endl;
		cout << "calibration indexes  kk2= " << kk2 << " kk3= " << kk3 << endl;
		cout << "calibration indexes in massiv  kkk2= " << kkk2 << " kkk3= " << kkk3 << endl;
	      }
	      
	      //       
	      h_map_HB->Fill(double(k2plot), double(k3), 1.);    
	    }
	    //                 HE:
	    if(k1==1) {
	      // ADC
	      h_ADCCalib_HE->Fill(adc,1.);
	      h_ADCCalib1_HE->Fill(adc,1.);
	      if(adc < calibrADCHEMin_ || adc > calibrADCHEMax_ ) h_mapADCCalib047_HE->Fill(double(k2plot),double(k3),1.);
	      h_mapADCCalib_HE->Fill(double(k2plot), double(k3), adc);    
	      // Ratio
	      h_RatioCalib_HE->Fill(ratio,1.);
	      if(ratio < calibrRatioHEMin_ || ratio > calibrRatioHEMax_) h_mapRatioCalib047_HE->Fill(double(k2plot),double(k3),1.);
	      h_mapRatioCalib_HE->Fill(double(k2plot), double(k3), ratio);    
	      // TSmax
	      if(calibtsmax > -0.5) {
		h_TSmaxCalib_HE->Fill(calibtsmax,1.);
		if(calibtsmax< calibrTSmaxHEMin_  || calibtsmax> calibrTSmaxHEMax_ )h_mapTSmaxCalib047_HE->Fill(double(k2plot),double(k3),1.);
		h_mapTSmaxCalib_HE->Fill(double(k2plot), double(k3), calibtsmax); 
	      }   
	      // TSmean
	      if(calibtsmean > -0.5) {
		h_TSmeanCalib_HE->Fill(calibtsmean,1.);
		if(calibtsmean< calibrTSmeanHEMin_  || calibtsmean> calibrTSmeanHEMax_ )h_mapTSmeanCalib047_HE->Fill(double(k2plot),double(k3),1.);
		h_mapTSmeanCalib_HE->Fill(double(k2plot), double(k3), calibtsmean); 
	      }   
	      // Width
	      if(calibwidth > -0.5) {
		h_WidthCalib_HE->Fill(calibwidth,1.);
		if(calibwidth< calibrWidthHEMin_  || calibwidth> calibrWidthHEMax_ )h_mapWidthCalib047_HE->Fill(double(k2plot),double(k3),1.);
		h_mapWidthCalib_HE->Fill(double(k2plot), double(k3), calibwidth); 
	      }   
	      // CapIdErrors
	      if(calibcap > 0) h_mapCapCalib047_HE->Fill(double(k2plot),double(k3),1.);
	      //                
	      h_map_HE->Fill(double(k2plot), double(k3), 1.);    
	    }
	    //                 HO: 
	    if(k1==2) {
	      // ADC
	      h_ADCCalib_HO->Fill(adc,1.);
	      h_ADCCalib1_HO->Fill(adc,1.);
	      if(adc < calibrADCHOMin_ || adc > calibrADCHOMax_ ) h_mapADCCalib047_HO->Fill(double(k2plot),double(k3),1.);
	      h_mapADCCalib_HO->Fill(double(k2plot), double(k3), adc);    
	      // Ratio
	      h_RatioCalib_HO->Fill(ratio,1.);
	      if(ratio < calibrRatioHOMin_ || ratio > calibrRatioHOMax_) h_mapRatioCalib047_HO->Fill(double(k2plot), double(k3), 1.);
	      h_mapRatioCalib_HO->Fill(double(k2plot), double(k3), ratio);    
	      // TSmax
	      if(calibtsmax > -0.5) {
		h_TSmaxCalib_HO->Fill(calibtsmax,1.);
		if(calibtsmax< calibrTSmaxHOMin_  || calibtsmax> calibrTSmaxHOMax_ )h_mapTSmaxCalib047_HO->Fill(double(k2plot),double(k3),1.);
		h_mapTSmaxCalib_HO->Fill(double(k2plot), double(k3), calibtsmax);    
	      }
	      // TSmean
	      if(calibtsmean > -0.5) {
		h_TSmeanCalib_HO->Fill(calibtsmean,1.);
		if(calibtsmean< calibrTSmeanHOMin_  || calibtsmean> calibrTSmeanHOMax_ )h_mapTSmeanCalib047_HO->Fill(double(k2plot),double(k3),1.);
		h_mapTSmeanCalib_HO->Fill(double(k2plot), double(k3), calibtsmean);    
	      }
	      // Width
	      if(calibwidth > -0.5) {
		h_WidthCalib_HO->Fill(calibwidth,1.);
		if(calibwidth< calibrWidthHOMin_  || calibwidth> calibrWidthHOMax_ )h_mapWidthCalib047_HO->Fill(double(k2plot),double(k3),1.);
		h_mapWidthCalib_HO->Fill(double(k2plot), double(k3), calibwidth);    
	      }
	      // CapIdErrors
	      if(calibcap > 0) h_mapCapCalib047_HO->Fill(double(k2plot),double(k3),1.);
	      //                 
	      h_map_HO->Fill(double(k2plot), double(k3), 1.);    
	    }
	    //                 HF: 
	    if(k1==3) {
	      // ADC
	      h_ADCCalib_HF->Fill(adc,1.);
	      h_ADCCalib1_HF->Fill(adc,1.);
	      if(adc < calibrADCHFMin_ || adc > calibrADCHFMax_ ) h_mapADCCalib047_HF->Fill(double(k2plot),double(k3),1.);
	      h_mapADCCalib_HF->Fill(double(k2plot), double(k3), adc);    
	      // Ratio
	      h_RatioCalib_HF->Fill(ratio,1.);
	      if(ratio < calibrRatioHFMin_ || ratio > calibrRatioHFMax_ ) h_mapRatioCalib047_HF->Fill(double(k2plot), double(k3), 1.);
	      h_mapRatioCalib_HF->Fill(double(k2plot), double(k3), ratio);    
	      // TSmax
	      if(calibtsmax > -0.5) {
		h_TSmaxCalib_HF->Fill(calibtsmax,1.);
		if(calibtsmax< calibrTSmaxHFMin_ || calibtsmax> calibrTSmaxHFMax_ ) h_mapTSmaxCalib047_HF->Fill(double(k2plot),double(k3),1.);
		h_mapTSmaxCalib_HF->Fill(double(k2plot), double(k3), calibtsmax);  
	      }  
	      // TSmean
	      if(calibtsmean > -0.5) {
		h_TSmeanCalib_HF->Fill(calibtsmean,1.);
		if(calibtsmean< calibrTSmeanHFMin_ || calibtsmean> calibrTSmeanHFMax_ ) h_mapTSmeanCalib047_HF->Fill(double(k2plot),double(k3),1.);
		h_mapTSmeanCalib_HF->Fill(double(k2plot), double(k3), calibtsmean);  
	      }  
	      // Width
	      if(calibwidth > -0.5) {
		h_WidthCalib_HF->Fill(calibwidth,1.);
		if(calibwidth< calibrWidthHFMin_ || calibwidth> calibrWidthHFMax_ ) h_mapWidthCalib047_HF->Fill(double(k2plot),double(k3),1.);
		h_mapWidthCalib_HF->Fill(double(k2plot), double(k3), calibwidth);  
	      }  
	      // CapIdErrors
	      if(calibcap > 0) h_mapCapCalib047_HF->Fill(double(k2plot),double(k3),1.);
	      // 
	      h_map_HF->Fill(double(k2plot), double(k3), 1.);    
	    }
	    ////////// 
	  }// if(signal[k1][k2][k3]>0.) 
	  ////////// 
	} // k3 
      }  // k2
    }  // k1
    
    /////
    
  }//if(recordHistoes_&& studyCalibCellsHist_)

  /////////////////////////////////////////////////// 
  if(recordNtuples_ && nevent50 < maxNeventsInNtuple_) myTree->Fill();
  //  if(recordNtuples_ && nevent < maxNeventsInNtuple_) myTree->Fill();
  
  /////////////////////////////////////////////////// 
  if(++local_event%100==0)
    { 
      if (verbosity == -22) cout<<"run "<<Run<<" processing events "<<local_event<<" ok, "
				<<", lumi "<<lumi
				<<", numOfLaserEv "<<numOfLaserEv<<endl;          
    }                          
  }// bcn
  

//EndAnalyzer

}
// ------------ method called once each job just before starting event loop  -----------
void VeRawAnalyzer::beginJob()
{
  hOutputFile   = new TFile( fOutputFileName.c_str(), "RECREATE" ) ;
  ////////////////////////////////////////////////////////////////////////////////////////////////////////
  nnnnnn = 0;
  nevent = 0; 
  nevent50 = 0; 
  counter = 0;
  counterho = 0;
  nnnnnn1= 0;
  nnnnnn2= 0;
  nnnnnn3= 0;
  nnnnnn4= 0;
  nnnnnn5= 0;
  nnnnnn6= 0;
  nnnnnn7= 0;
  nnnnnn8= 0;


  //////////////////////////////////////////////////////////////////////////////////    book histoes
  if(recordHistoes_) {
    //  ha2 = new TH2F("ha2"," ", 82, -41., 41., 72, 0., 72.);
    
    h_errorGeneral = new TH1F("h_errorGeneral"," ", 5, 0., 5.);
    h_error1 = new TH1F("h_error1"," ", 5, 0., 5.);
    h_error2 = new TH1F("h_error2"," ", 5, 0., 5.);
    h_error3 = new TH1F("h_error3"," ", 5, 0., 5.);
    h_amplError = new TH1F("h_amplError"," ", 100, -2.,98.);
    h_amplFine = new TH1F("h_amplFine"," ", 100, -2.,98.);
    
    h_errorGeneral_HB = new TH1F("h_errorGeneral_HB"," ", 5, 0., 5.);
    h_error1_HB = new TH1F("h_error1_HB"," ", 5, 0., 5.);
    h_error2_HB = new TH1F("h_error2_HB"," ", 5, 0., 5.);
    h_error3_HB = new TH1F("h_error3_HB"," ", 5, 0., 5.);
    h_error4_HB = new TH1F("h_error4_HB"," ", 5, 0., 5.);
    h_error5_HB = new TH1F("h_error5_HB"," ", 5, 0., 5.);
    h_error6_HB = new TH1F("h_error6_HB"," ", 5, 0., 5.);
    h_error7_HB = new TH1F("h_error7_HB"," ", 5, 0., 5.);
    h_amplError_HB = new TH1F("h_amplError_HB"," ", 100, -2.,98.);
    h_amplFine_HB = new TH1F("h_amplFine_HB"," ", 100, -2.,98.);
    h_mapDepth1Error_HB = new TH2F("h_mapDepth1Error_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Error_HB = new TH2F("h_mapDepth2Error_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth3Error_HB = new TH2F("h_mapDepth3Error_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_fiber0_HB = new TH1F("h_fiber0_HB"," ", 10, 0., 10.);
    h_fiber1_HB = new TH1F("h_fiber1_HB"," ", 10, 0., 10.);
    h_fiber2_HB = new TH1F("h_fiber2_HB"," ", 40, 0., 40.);
    h_repetedcapid_HB = new TH1F("h_repetedcapid_HB"," ", 5, 0., 5.);
    
    h_errorGeneral_HE = new TH1F("h_errorGeneral_HE"," ", 5, 0., 5.);
    h_error1_HE = new TH1F("h_error1_HE"," ", 5, 0., 5.);
    h_error2_HE = new TH1F("h_error2_HE"," ", 5, 0., 5.);
    h_error3_HE = new TH1F("h_error3_HE"," ", 5, 0., 5.);
    h_error4_HE = new TH1F("h_error4_HE"," ", 5, 0., 5.);
    h_error5_HE = new TH1F("h_error5_HE"," ", 5, 0., 5.);
    h_error6_HE = new TH1F("h_error6_HE"," ", 5, 0., 5.);
    h_error7_HE = new TH1F("h_error7_HE"," ", 5, 0., 5.);
    h_amplError_HE = new TH1F("h_amplError_HE"," ", 100, -2.,98.);
    h_amplFine_HE = new TH1F("h_amplFine_HE"," ", 100, -2.,98.);
    h_mapDepth1Error_HE = new TH2F("h_mapDepth1Error_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Error_HE = new TH2F("h_mapDepth2Error_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth3Error_HE = new TH2F("h_mapDepth3Error_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_fiber0_HE = new TH1F("h_fiber0_HE"," ", 10, 0., 10.);
    h_fiber1_HE = new TH1F("h_fiber1_HE"," ", 10, 0., 10.);
    h_fiber2_HE = new TH1F("h_fiber2_HE"," ", 40, 0., 40.);
    h_repetedcapid_HE = new TH1F("h_repetedcapid_HE"," ", 5, 0., 5.);
    
    h_errorGeneral_HF = new TH1F("h_errorGeneral_HF"," ", 5, 0., 5.);
    h_error1_HF = new TH1F("h_error1_HF"," ", 5, 0., 5.);
    h_error2_HF = new TH1F("h_error2_HF"," ", 5, 0., 5.);
    h_error3_HF = new TH1F("h_error3_HF"," ", 5, 0., 5.);
    h_error4_HF = new TH1F("h_error4_HF"," ", 5, 0., 5.);
    h_error5_HF = new TH1F("h_error5_HF"," ", 5, 0., 5.);
    h_error6_HF = new TH1F("h_error6_HF"," ", 5, 0., 5.);
    h_error7_HF = new TH1F("h_error7_HF"," ", 5, 0., 5.);
    h_amplError_HF = new TH1F("h_amplError_HF"," ", 100, -2.,98.);
    h_amplFine_HF = new TH1F("h_amplFine_HF"," ", 100, -2.,98.);
    h_mapDepth1Error_HF = new TH2F("h_mapDepth1Error_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Error_HF = new TH2F("h_mapDepth2Error_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth3Error_HF = new TH2F("h_mapDepth3Error_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_fiber0_HF = new TH1F("h_fiber0_HF"," ", 10, 0., 10.);
    h_fiber1_HF = new TH1F("h_fiber1_HF"," ", 10, 0., 10.);
    h_fiber2_HF = new TH1F("h_fiber2_HF"," ", 40, 0., 40.);
    h_repetedcapid_HF = new TH1F("h_repetedcapid_HF"," ", 5, 0., 5.);
    
    h_errorGeneral_HO = new TH1F("h_errorGeneral_HO"," ", 5, 0., 5.);
    h_error1_HO = new TH1F("h_error1_HO"," ", 5, 0., 5.);
    h_error2_HO = new TH1F("h_error2_HO"," ", 5, 0., 5.);
    h_error3_HO = new TH1F("h_error3_HO"," ", 5, 0., 5.);
    h_error4_HO = new TH1F("h_error4_HO"," ", 5, 0., 5.);
    h_error5_HO = new TH1F("h_error5_HO"," ", 5, 0., 5.);
    h_error6_HO = new TH1F("h_error6_HO"," ", 5, 0., 5.);
    h_error7_HO = new TH1F("h_error7_HO"," ", 5, 0., 5.);
    h_amplError_HO = new TH1F("h_amplError_HO"," ", 100, -2.,98.);
    h_amplFine_HO = new TH1F("h_amplFine_HO"," ", 100, -2.,98.);
    h_mapDepth4Error_HO = new TH2F("h_mapDepth4Error_HO"," ", 82, -41., 41., 72, 0., 72.);
    h_fiber0_HO = new TH1F("h_fiber0_HO"," ", 10, 0., 10.);
    h_fiber1_HO = new TH1F("h_fiber1_HO"," ", 10, 0., 10.);
    h_fiber2_HO = new TH1F("h_fiber2_HO"," ", 40, 0., 40.);
    h_repetedcapid_HO = new TH1F("h_repetedcapid_HO"," ", 5, 0., 5.);
    
    /////////////////////////////////////////////////////////////////////////////////////////////////
    // fullAmplitude:
    h_ADCAmpl345Zoom_HB = new TH1F("h_ADCAmpl345Zoom_HB"," ", 100, 0.,400.);
    h_ADCAmpl345Zoom1_HB = new TH1F("h_ADCAmpl345Zoom1_HB"," ", 100, 0.,100.);
    h_ADCAmpl345_HB = new TH1F("h_ADCAmpl345_HB"," ", 100, 10.,3000.);

    h_ADCAmpl345_HBCapIdError = new TH1F("h_ADCAmpl345_HBCapIdError"," ", 100, 10.,3000.);
    h_ADCAmpl345_HBCapIdNoError = new TH1F("h_ADCAmpl345_HBCapIdNoError"," ", 100, 10.,3000.);
    h_ADCAmpl_HBCapIdError = new TH1F("h_ADCAmpl_HBCapIdError"," ", 100, 10.,3000.);
    h_ADCAmpl_HBCapIdNoError = new TH1F("h_ADCAmpl_HBCapIdNoError"," ", 100, 10.,3000.);


    h_ADCAmplZoom_HB = new TH1F("h_ADCAmplZoom_HB"," ", 100, 0.,400.);
    h_ADCAmplZoom1_HB = new TH1F("h_ADCAmplZoom1_HB"," ", 100, -20.,80.);
    h_ADCAmpl_HB = new TH1F("h_ADCAmpl_HB"," ", 100, 10.,3000.);
    h_mapDepth1ADCAmpl225_HB = new TH2F("h_mapDepth1ADCAmpl225_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2ADCAmpl225_HB = new TH2F("h_mapDepth2ADCAmpl225_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1ADCAmpl225Copy_HB = new TH2F("h_mapDepth1ADCAmpl225Copy_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2ADCAmpl225Copy_HB = new TH2F("h_mapDepth2ADCAmpl225Copy_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1ADCAmpl_HB = new TH2F("h_mapDepth1ADCAmpl_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2ADCAmpl_HB = new TH2F("h_mapDepth2ADCAmpl_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_TSmeanA_HB = new TH1F("h_TSmeanA_HB"," ", 100, -1.,11.);
    h_mapDepth1TSmeanA225_HB = new TH2F("h_mapDepth1TSmeanA225_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2TSmeanA225_HB = new TH2F("h_mapDepth2TSmeanA225_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1TSmeanA_HB = new TH2F("h_mapDepth1TSmeanA_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2TSmeanA_HB = new TH2F("h_mapDepth2TSmeanA_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_TSmaxA_HB = new TH1F("h_TSmaxA_HB"," ", 100, -1.,11.);
    h_mapDepth1TSmaxA225_HB = new TH2F("h_mapDepth1TSmaxA225_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2TSmaxA225_HB = new TH2F("h_mapDepth2TSmaxA225_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1TSmaxA_HB = new TH2F("h_mapDepth1TSmaxA_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2TSmaxA_HB = new TH2F("h_mapDepth2TSmaxA_HB"," ", 82, -41., 41., 72, 0., 72.);
    // RMS:
    h_Amplitude_HB = new TH1F("h_Amplitude_HB"," ", 100, 0.,5.);
    h_mapDepth1Amplitude225_HB=new TH2F("h_mapDepth1Amplitude225_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Amplitude225_HB=new TH2F("h_mapDepth2Amplitude225_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1Amplitude_HB = new TH2F("h_mapDepth1Amplitude_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Amplitude_HB = new TH2F("h_mapDepth2Amplitude_HB"," ", 82, -41., 41., 72, 0., 72.);
    // Ratio:
    h_Ampl_HB = new TH1F("h_Ampl_HB"," ", 100, 0.,1.1);
    h_mapDepth1Ampl047_HB = new TH2F("h_mapDepth1Ampl047_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Ampl047_HB = new TH2F("h_mapDepth2Ampl047_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1Ampl_HB = new TH2F("h_mapDepth1Ampl_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Ampl_HB = new TH2F("h_mapDepth2Ampl_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1AmplE34_HB = new TH2F("h_mapDepth1AmplE34_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2AmplE34_HB = new TH2F("h_mapDepth2AmplE34_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1_HB = new TH2F("h_mapDepth1_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2_HB = new TH2F("h_mapDepth2_HB"," ", 82, -41., 41., 72, 0., 72.);
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    // fullAmplitude:
//    h_ADCAmpl_HE = new TH1F("h_ADCAmpl_HE"," ", 100, -20.,100.);
    h_ADCAmplZoom1_HE = new TH1F("h_ADCAmplZoom1_HE"," ", 100, -20.,80.);
    h_ADCAmpl_HE = new TH1F("h_ADCAmpl_HE"," ", 100, 0.,3000.);
    h_mapDepth1ADCAmpl225_HE = new TH2F("h_mapDepth1ADCAmpl225_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2ADCAmpl225_HE = new TH2F("h_mapDepth2ADCAmpl225_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth3ADCAmpl225_HE = new TH2F("h_mapDepth3ADCAmpl225_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1ADCAmpl225Copy_HE = new TH2F("h_mapDepth1ADCAmpl225Copy_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2ADCAmpl225Copy_HE = new TH2F("h_mapDepth2ADCAmpl225Copy_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth3ADCAmpl225Copy_HE = new TH2F("h_mapDepth3ADCAmpl225Copy_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1ADCAmpl_HE = new TH2F("h_mapDepth1ADCAmpl_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2ADCAmpl_HE = new TH2F("h_mapDepth2ADCAmpl_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth3ADCAmpl_HE = new TH2F("h_mapDepth3ADCAmpl_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_TSmeanA_HE = new TH1F("h_TSmeanA_HE"," ", 100, -2.,8.);
    h_mapDepth1TSmeanA225_HE = new TH2F("h_mapDepth1TSmeanA225_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2TSmeanA225_HE = new TH2F("h_mapDepth2TSmeanA225_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth3TSmeanA225_HE = new TH2F("h_mapDepth3TSmeanA225_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1TSmeanA_HE = new TH2F("h_mapDepth1TSmeanA_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2TSmeanA_HE = new TH2F("h_mapDepth2TSmeanA_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth3TSmeanA_HE = new TH2F("h_mapDepth3TSmeanA_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_TSmaxA_HE = new TH1F("h_TSmaxA_HE"," ", 100, -1.,11.);
    h_mapDepth1TSmaxA225_HE = new TH2F("h_mapDepth1TSmaxA225_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2TSmaxA225_HE = new TH2F("h_mapDepth2TSmaxA225_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth3TSmaxA225_HE = new TH2F("h_mapDepth3TSmaxA225_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1TSmaxA_HE = new TH2F("h_mapDepth1TSmaxA_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2TSmaxA_HE = new TH2F("h_mapDepth2TSmaxA_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth3TSmaxA_HE = new TH2F("h_mapDepth3TSmaxA_HE"," ", 82, -41., 41., 72, 0., 72.);
    // RMS:
    h_Amplitude_HE = new TH1F("h_Amplitude_HE"," ", 100, 0.,5.5);
    h_mapDepth1Amplitude225_HE = new TH2F("h_mapDepth1Amplitude225_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Amplitude225_HE = new TH2F("h_mapDepth2Amplitude225_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth3Amplitude225_HE = new TH2F("h_mapDepth3Amplitude225_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1Amplitude_HE = new TH2F("h_mapDepth1Amplitude_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Amplitude_HE = new TH2F("h_mapDepth2Amplitude_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth3Amplitude_HE = new TH2F("h_mapDepth3Amplitude_HE"," ", 82, -41., 41., 72, 0., 72.);
    // Ratio:
    h_Ampl_HE = new TH1F("h_Ampl_HE"," ", 100, 0.,1.1);
    h_mapDepth1Ampl047_HE = new TH2F("h_mapDepth1Ampl047_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Ampl047_HE = new TH2F("h_mapDepth2Ampl047_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth3Ampl047_HE = new TH2F("h_mapDepth3Ampl047_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1Ampl_HE = new TH2F("h_mapDepth1Ampl_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Ampl_HE = new TH2F("h_mapDepth2Ampl_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth3Ampl_HE = new TH2F("h_mapDepth3Ampl_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1AmplE34_HE = new TH2F("h_mapDepth1AmplE34_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2AmplE34_HE = new TH2F("h_mapDepth2AmplE34_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth3AmplE34_HE = new TH2F("h_mapDepth3AmplE34_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1_HE = new TH2F("h_mapDepth1_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2_HE = new TH2F("h_mapDepth2_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth3_HE = new TH2F("h_mapDepth3_HE"," ", 82, -41., 41., 72, 0., 72.);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // fullAmplitude:
//    h_ADCAmpl_HF = new TH1F("h_ADCAmpl_HF"," ", 100, 0.,3000.);
    h_ADCAmplZoom1_HF = new TH1F("h_ADCAmplZoom1_HF"," ", 100, -20.,80.);
    h_ADCAmpl_HF = new TH1F("h_ADCAmpl_HF"," ", 100, -300.,3300.);
    h_mapDepth1ADCAmpl225_HF = new TH2F("h_mapDepth1ADCAmpl225_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2ADCAmpl225_HF = new TH2F("h_mapDepth2ADCAmpl225_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1ADCAmpl225Copy_HF = new TH2F("h_mapDepth1ADCAmpl225Copy_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2ADCAmpl225Copy_HF = new TH2F("h_mapDepth2ADCAmpl225Copy_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1ADCAmpl_HF = new TH2F("h_mapDepth1ADCAmpl_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2ADCAmpl_HF = new TH2F("h_mapDepth2ADCAmpl_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_TSmeanA_HF = new TH1F("h_TSmeanA_HF"," ", 100, -1.,11.);
    h_mapDepth1TSmeanA225_HF = new TH2F("h_mapDepth1TSmeanA225_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2TSmeanA225_HF = new TH2F("h_mapDepth2TSmeanA225_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1TSmeanA_HF = new TH2F("h_mapDepth1TSmeanA_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2TSmeanA_HF = new TH2F("h_mapDepth2TSmeanA_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_Amplitude_HF = new TH1F("h_Amplitude_HF"," ", 100, 0.,5.);
    h_TSmaxA_HF = new TH1F("h_TSmaxA_HF"," ", 100, -1.,11.);
    h_mapDepth1TSmaxA225_HF = new TH2F("h_mapDepth1TSmaxA225_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2TSmaxA225_HF = new TH2F("h_mapDepth2TSmaxA225_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1TSmaxA_HF = new TH2F("h_mapDepth1TSmaxA_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2TSmaxA_HF = new TH2F("h_mapDepth2TSmaxA_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_Amplitude_HF = new TH1F("h_Amplitude_HF"," ", 100, 0.,5.);
    h_mapDepth1Amplitude225_HF=new TH2F("h_mapDepth1Amplitude225_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Amplitude225_HF=new TH2F("h_mapDepth2Amplitude225_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1Amplitude_HF = new TH2F("h_mapDepth1Amplitude_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Amplitude_HF = new TH2F("h_mapDepth2Amplitude_HF"," ", 82, -41., 41., 72, 0., 72.);
    // Ratio:
    h_Ampl_HF = new TH1F("h_Ampl_HF"," ", 100, 0.,1.1);
    h_mapDepth1Ampl047_HF = new TH2F("h_mapDepth1Ampl047_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Ampl047_HF = new TH2F("h_mapDepth2Ampl047_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1Ampl_HF = new TH2F("h_mapDepth1Ampl_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Ampl_HF = new TH2F("h_mapDepth2Ampl_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1AmplE34_HF = new TH2F("h_mapDepth1AmplE34_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2AmplE34_HF = new TH2F("h_mapDepth2AmplE34_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1_HF = new TH2F("h_mapDepth1_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2_HF = new TH2F("h_mapDepth2_HF"," ", 82, -41., 41., 72, 0., 72.);
    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // fullAmplitude:
    h_ADCAmpl_HO = new TH1F("h_ADCAmpl_HO"," ", 100, 0.,3000.);
    h_ADCAmplZoom1_HO = new TH1F("h_ADCAmplZoom1_HO"," ", 100, -20.,280.);
    h_ADCAmpl_HO_copy = new TH1F("h_ADCAmpl_HO_copy"," ", 100, 0.,30000.);
    h_mapDepth4ADCAmpl225_HO = new TH2F("h_mapDepth4ADCAmpl225_HO"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth4ADCAmpl225Copy_HO = new TH2F("h_mapDepth4ADCAmpl225Copy_HO"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth4ADCAmpl_HO = new TH2F("h_mapDepth4ADCAmpl_HO"," ", 82, -41., 41., 72, 0., 72.);
    h_TSmeanA_HO = new TH1F("h_TSmeanA_HO"," ", 100, 0.,10.);
    h_mapDepth4TSmeanA225_HO = new TH2F("h_mapDepth4TSmeanA225_HO"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth4TSmeanA_HO = new TH2F("h_mapDepth4TSmeanA_HO"," ", 82, -41., 41., 72, 0., 72.);
    h_TSmaxA_HO = new TH1F("h_TSmaxA_HO"," ", 100, 0.,10.);
    h_mapDepth4TSmaxA225_HO = new TH2F("h_mapDepth4TSmaxA225_HO"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth4TSmaxA_HO = new TH2F("h_mapDepth4TSmaxA_HO"," ", 82, -41., 41., 72, 0., 72.);
    h_Amplitude_HO = new TH1F("h_Amplitude_HO"," ", 100, 0.,5.);
    h_mapDepth4Amplitude225_HO=new TH2F("h_mapDepth4Amplitude225_HO"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth4Amplitude_HO = new TH2F("h_mapDepth4Amplitude_HO"," ", 82, -41., 41., 72, 0., 72.);
    // Ratio:
    h_Ampl_HO = new TH1F("h_Ampl_HO"," ", 100, 0.,1.1);
    h_mapDepth4Ampl047_HO = new TH2F("h_mapDepth4Ampl047_HO"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth4Ampl_HO = new TH2F("h_mapDepth4Ampl_HO"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth4AmplE34_HO = new TH2F("h_mapDepth4AmplE34_HO"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth4_HO = new TH2F("h_mapDepth4_HO"," ", 82, -41., 41., 72, 0., 72.);

    //////////////////////////////////////////////////////////////////////////////////////
    int baP= 1000;
    int baR= 4000;
    float baR2=baR;
    h_bcnnbadchannels_depth1_HB = new TH1F("h_bcnnbadchannels_depth1_HB"," ",   baP, 1.,baR2);
    h_bcnnbadchannels_depth2_HB = new TH1F("h_bcnnbadchannels_depth2_HB"," ",   baP, 1.,baR2);
    h_bcnnbadchannels_depth1_HE = new TH1F("h_bcnnbadchannels_depth1_HE"," ",   baP, 1.,baR2);
    h_bcnnbadchannels_depth2_HE = new TH1F("h_bcnnbadchannels_depth2_HE"," ",   baP, 1.,baR2);
    h_bcnnbadchannels_depth3_HE = new TH1F("h_bcnnbadchannels_depth3_HE"," ",   baP, 1.,baR2);
    h_bcnnbadchannels_depth4_HO = new TH1F("h_bcnnbadchannels_depth4_HO"," ",   baP, 1.,baR2);
    h_bcnnbadchannels_depth1_HF = new TH1F("h_bcnnbadchannels_depth1_HF"," ",   baP, 1.,baR2);
    h_bcnnbadchannels_depth2_HF = new TH1F("h_bcnnbadchannels_depth2_HF"," ",   baP, 1.,baR2);
    h_bcnbadrate0_depth1_HB = new TH1F("h_bcnbadrate0_depth1_HB"," ",           baP, 1.,baR2);
    h_bcnbadrate0_depth2_HB = new TH1F("h_bcnbadrate0_depth2_HB"," ",           baP, 1.,baR2);
    h_bcnbadrate0_depth1_HE = new TH1F("h_bcnbadrate0_depth1_HE"," ",           baP, 1.,baR2);
    h_bcnbadrate0_depth2_HE = new TH1F("h_bcnbadrate0_depth2_HE"," ",           baP, 1.,baR2);
    h_bcnbadrate0_depth3_HE = new TH1F("h_bcnbadrate0_depth3_HE"," ",           baP, 1.,baR2);
    h_bcnbadrate0_depth4_HO = new TH1F("h_bcnbadrate0_depth4_HO"," ",           baP, 1.,baR2);
    h_bcnbadrate0_depth1_HF = new TH1F("h_bcnbadrate0_depth1_HF"," ",           baP, 1.,baR2);
    h_bcnbadrate0_depth2_HF = new TH1F("h_bcnbadrate0_depth2_HF"," ",           baP, 1.,baR2);


    int bac= howmanybinsonplots_;
    //  int bac= 15;
    float bac2=bac+1.;

    h_nbadchannels_depth1_HB = new TH1F("h_nbadchannels_depth1_HB"," ",   100, 1.,3001.);
    h_runnbadchannels_depth1_HB = new TH1F("h_runnbadchannels_depth1_HB"," ",   bac, 1.,bac2);
    h_runnbadchannelsC_depth1_HB = new TH1F("h_runnbadchannelsC_depth1_HB"," ",   bac, 1.,bac2);
    h_runbadrate_depth1_HB = new TH1F("h_runbadrate_depth1_HB"," ",             bac, 1.,bac2);
    h_runbadrateC_depth1_HB = new TH1F("h_runbadrateC_depth1_HB"," ",             bac, 1.,bac2);
    h_runbadrate0_depth1_HB = new TH1F("h_runbadrate0_depth1_HB"," ",           bac, 1.,bac2);

    h_nbadchannels_depth2_HB = new TH1F("h_nbadchannels_depth2_HB"," ",   100, 1.,501.);
    h_runnbadchannels_depth2_HB = new TH1F("h_runnbadchannels_depth2_HB"," ",   bac, 1.,bac2);
    h_runnbadchannelsC_depth2_HB = new TH1F("h_runnbadchannelsC_depth2_HB"," ",   bac, 1.,bac2);
    h_runbadrate_depth2_HB = new TH1F("h_runbadrate_depth2_HB"," ",             bac, 1.,bac2);
    h_runbadrateC_depth2_HB = new TH1F("h_runbadrateC_depth2_HB"," ",             bac, 1.,bac2);
    h_runbadrate0_depth2_HB = new TH1F("h_runbadrate0_depth2_HB"," ",           bac, 1.,bac2);

    h_nbadchannels_depth1_HE = new TH1F("h_nbadchannels_depth1_HE"," ",   100, 1.,3001.);
    h_runnbadchannels_depth1_HE = new TH1F("h_runnbadchannels_depth1_HE"," ",   bac, 1.,bac2);
    h_runnbadchannelsC_depth1_HE = new TH1F("h_runnbadchannelsC_depth1_HE"," ",   bac, 1.,bac2);
    h_runbadrate_depth1_HE = new TH1F("h_runbadrate_depth1_HE"," ",             bac, 1.,bac2);
    h_runbadrateC_depth1_HE = new TH1F("h_runbadrateC_depth1_HE"," ",             bac, 1.,bac2);
    h_runbadrate0_depth1_HE = new TH1F("h_runbadrate0_depth1_HE"," ",           bac, 1.,bac2);

    h_nbadchannels_depth2_HE = new TH1F("h_nbadchannels_depth2_HE"," ",   100, 1.,3001.);
    h_runnbadchannels_depth2_HE = new TH1F("h_runnbadchannels_depth2_HE"," ",   bac, 1.,bac2);
    h_runnbadchannelsC_depth2_HE = new TH1F("h_runnbadchannelsC_depth2_HE"," ",   bac, 1.,bac2);
    h_runbadrate_depth2_HE = new TH1F("h_runbadrate_depth2_HE"," ",             bac, 1.,bac2);
    h_runbadrateC_depth2_HE = new TH1F("h_runbadrateC_depth2_HE"," ",             bac, 1.,bac2);
    h_runbadrate0_depth2_HE = new TH1F("h_runbadrate0_depth2_HE"," ",           bac, 1.,bac2);

    h_nbadchannels_depth3_HE = new TH1F("h_nbadchannels_depth3_HE"," ",   100, 1.,501.);
    h_runnbadchannels_depth3_HE = new TH1F("h_runnbadchannels_depth3_HE"," ",   bac, 1.,bac2);
    h_runnbadchannelsC_depth3_HE = new TH1F("h_runnbadchannelsC_depth3_HE"," ",   bac, 1.,bac2);
    h_runbadrate_depth3_HE = new TH1F("h_runbadrate_depth3_HE"," ",             bac, 1.,bac2);
    h_runbadrateC_depth3_HE = new TH1F("h_runbadrateC_depth3_HE"," ",             bac, 1.,bac2);
    h_runbadrate0_depth3_HE = new TH1F("h_runbadrate0_depth3_HE"," ",           bac, 1.,bac2);

    h_nbadchannels_depth1_HF = new TH1F("h_nbadchannels_depth1_HF"," ",   100, 1.,3001.);
    h_runnbadchannels_depth1_HF = new TH1F("h_runnbadchannels_depth1_HF"," ",   bac, 1.,bac2);
    h_runnbadchannelsC_depth1_HF = new TH1F("h_runnbadchannelsC_depth1_HF"," ",   bac, 1.,bac2);
    h_runbadrate_depth1_HF = new TH1F("h_runbadrate_depth1_HF"," ",             bac, 1.,bac2);
    h_runbadrateC_depth1_HF = new TH1F("h_runbadrateC_depth1_HF"," ",             bac, 1.,bac2);
    h_runbadrate0_depth1_HF = new TH1F("h_runbadrate0_depth1_HF"," ",           bac, 1.,bac2);

    h_nbadchannels_depth2_HF = new TH1F("h_nbadchannels_depth2_HF"," ",   100, 1.,501.);
    h_runnbadchannels_depth2_HF = new TH1F("h_runnbadchannels_depth2_HF"," ",   bac, 1.,bac2);
    h_runnbadchannelsC_depth2_HF = new TH1F("h_runnbadchannelsC_depth2_HF"," ",   bac, 1.,bac2);
    h_runbadrate_depth2_HF = new TH1F("h_runbadrate_depth2_HF"," ",             bac, 1.,bac2);
    h_runbadrateC_depth2_HF = new TH1F("h_runbadrateC_depth2_HF"," ",             bac, 1.,bac2);
    h_runbadrate0_depth2_HF = new TH1F("h_runbadrate0_depth2_HF"," ",           bac, 1.,bac2);

    h_nbadchannels_depth4_HO = new TH1F("h_nbadchannels_depth4_HO"," ",   100, 1.,3001.);
    h_runnbadchannels_depth4_HO = new TH1F("h_runnbadchannels_depth4_HO"," ",   bac, 1.,bac2);
    h_runnbadchannelsC_depth4_HO = new TH1F("h_runnbadchannelsC_depth4_HO"," ",   bac, 1.,bac2);
    h_runbadrate_depth4_HO = new TH1F("h_runbadrate_depth4_HO"," ",             bac, 1.,bac2);
    h_runbadrateC_depth4_HO = new TH1F("h_runbadrateC_depth4_HO"," ",             bac, 1.,bac2);
    h_runbadrate0_depth4_HO = new TH1F("h_runbadrate0_depth4_HO"," ",           bac, 1.,bac2);


    ///////////////////////////////////////////////////////////////////////////////////////////////
    /*  
    float m7=2;
    float m8=3;
    float m9=5;
    h_GetRMSOverNormalizedSignal_HB = new TH1F("h_GetRMSOverNormalizedSignal_HB"," ",   100, 0.,m7);
    h_GetRMSOverNormalizedSignal_HE = new TH1F("h_GetRMSOverNormalizedSignal_HE"," ",   100, 0.,m7);
    h_GetRMSOverNormalizedSignal_HO = new TH1F("h_GetRMSOverNormalizedSignal_HO"," ",   100, 0.,m8);
    h_GetRMSOverNormalizedSignal_HF = new TH1F("h_GetRMSOverNormalizedSignal_HF"," ",   100, 0.,m9);
    h_GetRMSOverNormalizedSignal3_HB = new TH1F("h_GetRMSOverNormalizedSignal3_HB"," ", 100, 0.,m7);
    h_GetRMSOverNormalizedSignal3_HE = new TH1F("h_GetRMSOverNormalizedSignal3_HE"," ", 100, 0.,m7);
    h_GetRMSOverNormalizedSignal3_HO = new TH1F("h_GetRMSOverNormalizedSignal3_HO"," ", 100, 0.,m8);
    h_GetRMSOverNormalizedSignal3_HF = new TH1F("h_GetRMSOverNormalizedSignal3_HF"," ", 100, 0.,m9);
*/
    h_FullSignal3D_HB = new TH2F("h_FullSignal3D_HB"," ",   82, -41., 41., 72, 0., 72.);
    h_FullSignal3D0_HB = new TH2F("h_FullSignal3D0_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_FullSignal3D_HE = new TH2F("h_FullSignal3D_HE"," ",   82, -41., 41., 72, 0., 72.);
    h_FullSignal3D0_HE = new TH2F("h_FullSignal3D0_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_FullSignal3D_HO = new TH2F("h_FullSignal3D_HO"," ",   82, -41., 41., 72, 0., 72.);
    h_FullSignal3D0_HO = new TH2F("h_FullSignal3D0_HO"," ", 82, -41., 41., 72, 0., 72.);
    h_FullSignal3D_HF = new TH2F("h_FullSignal3D_HF"," ",   82, -41., 41., 72, 0., 72.);
    h_FullSignal3D0_HF = new TH2F("h_FullSignal3D0_HF"," ", 82, -41., 41., 72, 0., 72.);

    //////////////////////////////////////////////////////////////////////////////////////////////////
    h_ADCCalib_HB       = new TH1F("h_ADCCalib_HB"," ",      100, 10.,10000.);
    h_ADCCalib1_HB       = new TH1F("h_ADCCalib1_HB"," ",      100, 0.1,100.1);
    h_mapADCCalib047_HB = new TH2F("h_mapADCCalib047_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapADCCalib_HB    = new TH2F("h_mapADCCalib_HB"," ",    82, -41., 41., 72, 0., 72.);
    h_RatioCalib_HB       = new TH1F("h_RatioCalib_HB"," ",      100, 0.,1.);
    h_mapRatioCalib047_HB = new TH2F("h_mapRatioCalib047_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapRatioCalib_HB    = new TH2F("h_mapRatioCalib_HB"," ",    82, -41., 41., 72, 0., 72.);
    h_TSmaxCalib_HB       = new TH1F("h_TSmaxCalib_HB"," ",      100, 0.,10.);
    h_mapTSmaxCalib047_HB = new TH2F("h_mapTSmaxCalib047_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapTSmaxCalib_HB    = new TH2F("h_mapTSmaxCalib_HB"," ",    82, -41., 41., 72, 0., 72.);
    h_TSmeanCalib_HB       = new TH1F("h_TSmeanCalib_HB"," ",      100, 0.,10.);
    h_mapTSmeanCalib047_HB = new TH2F("h_mapTSmeanCalib047_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapTSmeanCalib_HB    = new TH2F("h_mapTSmeanCalib_HB"," ",    82, -41., 41., 72, 0., 72.);
    h_WidthCalib_HB       = new TH1F("h_WidthCalib_HB"," ",      100, 0.,5.);
    h_mapWidthCalib047_HB = new TH2F("h_mapWidthCalib047_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapCapCalib047_HB = new TH2F("h_mapCapCalib047_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapWidthCalib_HB    = new TH2F("h_mapWidthCalib_HB"," ",    82, -41., 41., 72, 0., 72.);
    h_map_HB              = new TH2F("h_map_HB"," ",              82, -41., 41., 72, 0., 72.);
    h_ADCCalib_HE       = new TH1F("h_ADCCalib_HE"," ",      100, 10.,10000.);
    h_ADCCalib1_HE       = new TH1F("h_ADCCalib1_HE"," ",      100, 0.1,100.1);
    h_mapADCCalib047_HE = new TH2F("h_mapADCCalib047_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapADCCalib_HE    = new TH2F("h_mapADCCalib_HE"," ",    82, -41., 41., 72, 0., 72.);
    h_RatioCalib_HE       = new TH1F("h_RatioCalib_HE"," ",      100, 0.,1.);
    h_mapRatioCalib047_HE = new TH2F("h_mapRatioCalib047_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapRatioCalib_HE    = new TH2F("h_mapRatioCalib_HE"," ",    82, -41., 41., 72, 0., 72.);
    h_TSmaxCalib_HE       = new TH1F("h_TSmaxCalib_HE"," ",      100, 0.,10.);
    h_mapTSmaxCalib047_HE = new TH2F("h_mapTSmaxCalib047_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapTSmaxCalib_HE    = new TH2F("h_mapTSmaxCalib_HE"," ",    82, -41., 41., 72, 0., 72.);
    h_TSmeanCalib_HE       = new TH1F("h_TSmeanCalib_HE"," ",      100, 0.,10.);
    h_mapTSmeanCalib047_HE = new TH2F("h_mapTSmeanCalib047_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapTSmeanCalib_HE    = new TH2F("h_mapTSmeanCalib_HE"," ",    82, -41., 41., 72, 0., 72.);
    h_WidthCalib_HE       = new TH1F("h_WidthCalib_HE"," ",      100, 0.,5.);
    h_mapWidthCalib047_HE = new TH2F("h_mapWidthCalib047_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapCapCalib047_HE = new TH2F("h_mapCapCalib047_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapWidthCalib_HE    = new TH2F("h_mapWidthCalib_HE"," ",    82, -41., 41., 72, 0., 72.);
    h_map_HE              = new TH2F("h_map_HE"," ",              82, -41., 41., 72, 0., 72.);
    h_ADCCalib_HO       = new TH1F("h_ADCCalib_HO"," ",      100, 10.,10000.);
    h_ADCCalib1_HO       = new TH1F("h_ADCCalib1_HO"," ",      100, 0.1,100.1);
    h_mapADCCalib047_HO = new TH2F("h_mapADCCalib047_HO"," ", 82, -41., 41., 72, 0., 72.);
    h_mapADCCalib_HO    = new TH2F("h_mapADCCalib_HO"," ",    82, -41., 41., 72, 0., 72.);
    h_RatioCalib_HO       = new TH1F("h_RatioCalib_HO"," ",      100, 0.,1.);
    h_mapRatioCalib047_HO = new TH2F("h_mapRatioCalib047_HO"," ", 82, -41., 41., 72, 0., 72.);
    h_mapRatioCalib_HO    = new TH2F("h_mapRatioCalib_HO"," ",    82, -41., 41., 72, 0., 72.);
    h_TSmaxCalib_HO       = new TH1F("h_TSmaxCalib_HO"," ",      100, 0.,10.);
    h_mapTSmaxCalib047_HO = new TH2F("h_mapTSmaxCalib047_HO"," ", 82, -41., 41., 72, 0., 72.);
    h_mapTSmaxCalib_HO    = new TH2F("h_mapTSmaxCalib_HO"," ",    82, -41., 41., 72, 0., 72.);
    h_TSmeanCalib_HO       = new TH1F("h_TSmeanCalib_HO"," ",      100, 0.,10.);
    h_mapTSmeanCalib047_HO = new TH2F("h_mapTSmeanCalib047_HO"," ", 82, -41., 41., 72, 0., 72.);
    h_mapTSmeanCalib_HO    = new TH2F("h_mapTSmeanCalib_HO"," ",    82, -41., 41., 72, 0., 72.);
    h_WidthCalib_HO       = new TH1F("h_WidthCalib_HO"," ",      100, 0.,5.);
    h_mapWidthCalib047_HO = new TH2F("h_mapWidthCalib047_HO"," ", 82, -41., 41., 72, 0., 72.);
    h_mapCapCalib047_HO = new TH2F("h_mapCapCalib047_HO"," ", 82, -41., 41., 72, 0., 72.);
    h_mapWidthCalib_HO    = new TH2F("h_mapWidthCalib_HO"," ",    82, -41., 41., 72, 0., 72.);
    h_map_HO              = new TH2F("h_map_HO"," ",              82, -41., 41., 72, 0., 72.);
    h_ADCCalib_HF       = new TH1F("h_ADCCalib_HF"," ",      100, 10.,2000.);
    h_ADCCalib1_HF       = new TH1F("h_ADCCalib1_HF"," ",      100, 0.1,100.1);
    h_mapADCCalib047_HF = new TH2F("h_mapADCCalib047_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapADCCalib_HF    = new TH2F("h_mapADCCalib_HF"," ",    82, -41., 41., 72, 0., 72.);
    h_RatioCalib_HF       = new TH1F("h_RatioCalib_HF"," ",      100, 0.,1.);
    h_mapRatioCalib047_HF = new TH2F("h_mapRatioCalib047_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapRatioCalib_HF    = new TH2F("h_mapRatioCalib_HF"," ",    82, -41., 41., 72, 0., 72.);
    h_TSmaxCalib_HF       = new TH1F("h_TSmaxCalib_HF"," ",      100, 0.,10.);
    h_mapTSmaxCalib047_HF = new TH2F("h_mapTSmaxCalib047_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapTSmaxCalib_HF    = new TH2F("h_mapTSmaxCalib_HF"," ",    82, -41., 41., 72, 0., 72.);
    h_TSmeanCalib_HF       = new TH1F("h_TSmeanCalib_HF"," ",      100, 0.,10.);
    h_mapTSmeanCalib047_HF = new TH2F("h_mapTSmeanCalib047_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapTSmeanCalib_HF    = new TH2F("h_mapTSmeanCalib_HF"," ",    82, -41., 41., 72, 0., 72.);
    h_WidthCalib_HF       = new TH1F("h_WidthCalib_HF"," ",      100, 0.,5.);
    h_mapWidthCalib047_HF = new TH2F("h_mapWidthCalib047_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapCapCalib047_HF = new TH2F("h_mapCapCalib047_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapWidthCalib_HF    = new TH2F("h_mapWidthCalib_HF"," ",    82, -41., 41., 72, 0., 72.);
    h_map_HF              = new TH2F("h_map_HF"," ",              82, -41., 41., 72, 0., 72.);


    h_nls_per_run          = new TH1F("h_nls_per_run"," ",      100, 0.,800.);
    h_nls_per_run10        = new TH1F("h_nls_per_run10"," ",      100, 0.,60.);
    h_nevents_per_LS       = new TH1F("h_nevents_per_LS"," ",      100, 0.,600.);
    h_nevents_per_LSzoom   = new TH1F("h_nevents_per_LSzoom"," ",      50, 0.,50.);
    h_nevents_per_eachLS   = new TH1F("h_nevents_per_eachLS"," ",   bac, 1.,bac2);
    h_nevents_per_eachRealLS   = new TH1F("h_nevents_per_eachRealLS"," ",   bac, 1.,bac2);
    h_lsnumber_per_eachLS   = new TH1F("h_lsnumber_per_eachLS"," ",   bac, 1.,bac2);

    // for estimator0:
    float pst1=30.;
    h_sumPedestalLS1   = new TH1F("h_sumPedestalLS1"," ",      100,  0.,pst1);
    h_2DsumPedestalLS1    = new TH2F("h_2DsumPedestalLS1"," ",    82, -41., 41., 72, 0., 72.);
    h_sumPedestalperLS1  = new TH1F("h_sumPedestalperLS1"," ",     bac, 1.,bac2);
    h_2D0sumPedestalLS1    = new TH2F("h_2D0sumPedestalLS1"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0PedestalperLS1  = new TH1F("h_sum0PedestalperLS1"," ",     bac, 1.,bac2);

    h_sumPedestalLS2   = new TH1F("h_sumPedestalLS2"," ",      100,  0.,pst1);
    h_2DsumPedestalLS2    = new TH2F("h_2DsumPedestalLS2"," ",    82, -41., 41., 72, 0., 72.);
    h_sumPedestalperLS2  = new TH1F("h_sumPedestalperLS2"," ",     bac, 1.,bac2);
    h_2D0sumPedestalLS2    = new TH2F("h_2D0sumPedestalLS2"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0PedestalperLS2  = new TH1F("h_sum0PedestalperLS2"," ",     bac, 1.,bac2);

    h_sumPedestalLS3   = new TH1F("h_sumPedestalLS3"," ",      100,  0.,pst1);
    h_2DsumPedestalLS3    = new TH2F("h_2DsumPedestalLS3"," ",    82, -41., 41., 72, 0., 72.);
    h_sumPedestalperLS3  = new TH1F("h_sumPedestalperLS3"," ",     bac, 1.,bac2);
    h_2D0sumPedestalLS3    = new TH2F("h_2D0sumPedestalLS3"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0PedestalperLS3  = new TH1F("h_sum0PedestalperLS3"," ",     bac, 1.,bac2);

    h_sumPedestalLS4   = new TH1F("h_sumPedestalLS4"," ",      100,  0.,pst1);
    h_2DsumPedestalLS4    = new TH2F("h_2DsumPedestalLS4"," ",    82, -41., 41., 72, 0., 72.);
    h_sumPedestalperLS4  = new TH1F("h_sumPedestalperLS4"," ",     bac, 1.,bac2);
    h_2D0sumPedestalLS4    = new TH2F("h_2D0sumPedestalLS4"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0PedestalperLS4  = new TH1F("h_sum0PedestalperLS4"," ",     bac, 1.,bac2);

    h_sumPedestalLS5   = new TH1F("h_sumPedestalLS5"," ",      100,  0.,pst1);
    h_2DsumPedestalLS5    = new TH2F("h_2DsumPedestalLS5"," ",    82, -41., 41., 72, 0., 72.);
    h_sumPedestalperLS5  = new TH1F("h_sumPedestalperLS5"," ",     bac, 1.,bac2);
    h_2D0sumPedestalLS5    = new TH2F("h_2D0sumPedestalLS5"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0PedestalperLS5  = new TH1F("h_sum0PedestalperLS5"," ",     bac, 1.,bac2);

    h_sumPedestalLS6   = new TH1F("h_sumPedestalLS6"," ",      100,  0.,pst1);
    h_2DsumPedestalLS6    = new TH2F("h_2DsumPedestalLS6"," ",    82, -41., 41., 72, 0., 72.);
    h_sumPedestalperLS6  = new TH1F("h_sumPedestalperLS6"," ",     bac, 1.,bac2);
    h_2D0sumPedestalLS6    = new TH2F("h_2D0sumPedestalLS6"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0PedestalperLS6  = new TH1F("h_sum0PedestalperLS6"," ",     bac, 1.,bac2);

    h_sumPedestalLS7   = new TH1F("h_sumPedestalLS7"," ",      100,  0.,pst1);
    h_2DsumPedestalLS7    = new TH2F("h_2DsumPedestalLS7"," ",    82, -41., 41., 72, 0., 72.);
    h_sumPedestalperLS7  = new TH1F("h_sumPedestalperLS7"," ",     bac, 1.,bac2);
    h_2D0sumPedestalLS7    = new TH2F("h_2D0sumPedestalLS7"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0PedestalperLS7  = new TH1F("h_sum0PedestalperLS7"," ",     bac, 1.,bac2);

    h_sumPedestalLS8   = new TH1F("h_sumPedestalLS8"," ",      100,  0.,pst1);
    h_2DsumPedestalLS8    = new TH2F("h_2DsumPedestalLS8"," ",    82, -41., 41., 72, 0., 72.);
    h_sumPedestalperLS8  = new TH1F("h_sumPedestalperLS8"," ",     bac, 1.,bac2);
    h_2D0sumPedestalLS8    = new TH2F("h_2D0sumPedestalLS8"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0PedestalperLS8  = new TH1F("h_sum0PedestalperLS8"," ",     bac, 1.,bac2);


    // for estimator1:
    h_sumADCAmplLS1copy1   = new TH1F("h_sumADCAmplLS1copy1"," ",      100,  0.,10000);
    h_sumADCAmplLS1copy2   = new TH1F("h_sumADCAmplLS1copy2"," ",      100,  0.,20000);
    h_sumADCAmplLS1copy3   = new TH1F("h_sumADCAmplLS1copy3"," ",      100,  0.,50000);
    h_sumADCAmplLS1copy4   = new TH1F("h_sumADCAmplLS1copy4"," ",      100,  0.,100000);
    h_sumADCAmplLS1copy5   = new TH1F("h_sumADCAmplLS1copy5"," ",      100,  0.,150000);
//    float est1=100.;
//    float est11=300.;
//    float est12=350.;
    h_sumADCAmplLS1   = new TH1F("h_sumADCAmplLS1"," ",      100,  0.,lsdep_estimator1_HBdepth1_);
    h_2DsumADCAmplLS1    = new TH2F("h_2DsumADCAmplLS1"," ",    82, -41., 41., 72, 0., 72.);
    h_2DsumADCAmplLS1_LSselected= new TH2F("h_2DsumADCAmplLS1_LSselected"," ",82,-41.,41.,72,0.,72.);
    h_sumADCAmplperLS1  = new TH1F("h_sumADCAmplperLS1"," ",     bac, 1.,bac2);
    h_sumCutADCAmplperLS1  = new TH1F("h_sumCutADCAmplperLS1"," ",     bac, 1.,bac2);
    h_2D0sumADCAmplLS1    = new TH2F("h_2D0sumADCAmplLS1"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0ADCAmplperLS1  = new TH1F("h_sum0ADCAmplperLS1"," ",     bac, 1.,bac2);

    h_sumADCAmplLS2   = new TH1F("h_sumADCAmplLS2"," ",      100,  0.,lsdep_estimator1_HBdepth2_);
    h_2DsumADCAmplLS2    = new TH2F("h_2DsumADCAmplLS2"," ",    82, -41., 41., 72, 0., 72.);
    h_2DsumADCAmplLS2_LSselected= new TH2F("h_2DsumADCAmplLS2_LSselected"," ",82,-41.,41.,72,0.,72.);
    h_sumADCAmplperLS2  = new TH1F("h_sumADCAmplperLS2"," ",     bac, 1.,bac2);
    h_sumCutADCAmplperLS2  = new TH1F("h_sumCutADCAmplperLS2"," ",     bac, 1.,bac2);
    h_2D0sumADCAmplLS2    = new TH2F("h_2D0sumADCAmplLS2"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0ADCAmplperLS2  = new TH1F("h_sum0ADCAmplperLS2"," ",     bac, 1.,bac2);

    h_sumADCAmplLS3   = new TH1F("h_sumADCAmplLS3"," ",      100,  0.,lsdep_estimator1_HEdepth1_);
    h_2DsumADCAmplLS3    = new TH2F("h_2DsumADCAmplLS3"," ",    82, -41., 41., 72, 0., 72.);
    h_2DsumADCAmplLS3_LSselected= new TH2F("h_2DsumADCAmplLS3_LSselected"," ",82,-41.,41.,72,0.,72.);
    h_sumADCAmplperLS3  = new TH1F("h_sumADCAmplperLS3"," ",     bac, 1.,bac2);
    h_sumCutADCAmplperLS3  = new TH1F("h_sumCutADCAmplperLS3"," ",     bac, 1.,bac2);
    h_2D0sumADCAmplLS3    = new TH2F("h_2D0sumADCAmplLS3"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0ADCAmplperLS3  = new TH1F("h_sum0ADCAmplperLS3"," ",     bac, 1.,bac2);

    h_sumADCAmplLS4   = new TH1F("h_sumADCAmplLS4"," ",      100,  0.,lsdep_estimator1_HEdepth2_);
    h_2DsumADCAmplLS4    = new TH2F("h_2DsumADCAmplLS4"," ",    82, -41., 41., 72, 0., 72.);
    h_2DsumADCAmplLS4_LSselected= new TH2F("h_2DsumADCAmplLS4_LSselected"," ",82,-41.,41.,72,0.,72.);
    h_sumADCAmplperLS4  = new TH1F("h_sumADCAmplperLS4"," ",     bac, 1.,bac2);
    h_sumCutADCAmplperLS4  = new TH1F("h_sumCutADCAmplperLS4"," ",     bac, 1.,bac2);
    h_2D0sumADCAmplLS4    = new TH2F("h_2D0sumADCAmplLS4"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0ADCAmplperLS4  = new TH1F("h_sum0ADCAmplperLS4"," ",     bac, 1.,bac2);

    h_sumADCAmplLS5   = new TH1F("h_sumADCAmplLS5"," ",      100,  0.,lsdep_estimator1_HEdepth3_);
    h_2DsumADCAmplLS5    = new TH2F("h_2DsumADCAmplLS5"," ",    82, -41., 41., 72, 0., 72.);
    h_2DsumADCAmplLS5_LSselected= new TH2F("h_2DsumADCAmplLS5_LSselected"," ",82,-41.,41.,72,0.,72.);
    h_sumADCAmplperLS5  = new TH1F("h_sumADCAmplperLS5"," ",     bac, 1.,bac2);
    h_sumCutADCAmplperLS5  = new TH1F("h_sumCutADCAmplperLS5"," ",     bac, 1.,bac2);
    h_2D0sumADCAmplLS5    = new TH2F("h_2D0sumADCAmplLS5"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0ADCAmplperLS5  = new TH1F("h_sum0ADCAmplperLS5"," ",     bac, 1.,bac2);

    h_sumADCAmplLS6   = new TH1F("h_sumADCAmplLS6"," ",      100,  0.,lsdep_estimator1_HFdepth1_);
    h_2DsumADCAmplLS6    = new TH2F("h_2DsumADCAmplLS6"," ",    82, -41., 41., 72, 0., 72.);
    h_2DsumADCAmplLS6_LSselected= new TH2F("h_2DsumADCAmplLS6_LSselected"," ",82,-41.,41.,72,0.,72.);
    h_sumADCAmplperLS6  = new TH1F("h_sumADCAmplperLS6"," ",     bac, 1.,bac2);
    h_sumCutADCAmplperLS6  = new TH1F("h_sumCutADCAmplperLS6"," ",     bac, 1.,bac2);
    h_2D0sumADCAmplLS6    = new TH2F("h_2D0sumADCAmplLS6"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0ADCAmplperLS6  = new TH1F("h_sum0ADCAmplperLS6"," ",     bac, 1.,bac2);

    h_sumADCAmplLS7   = new TH1F("h_sumADCAmplLS7"," ",      100,  0.,lsdep_estimator1_HFdepth2_);
    h_2DsumADCAmplLS7    = new TH2F("h_2DsumADCAmplLS7"," ",    82, -41., 41., 72, 0., 72.);
    h_2DsumADCAmplLS7_LSselected= new TH2F("h_2DsumADCAmplLS7_LSselected"," ",82,-41.,41.,72,0.,72.);
    h_sumADCAmplperLS7  = new TH1F("h_sumADCAmplperLS7"," ",     bac, 1.,bac2);
    h_sumCutADCAmplperLS7  = new TH1F("h_sumCutADCAmplperLS7"," ",     bac, 1.,bac2);
    h_2D0sumADCAmplLS7    = new TH2F("h_2D0sumADCAmplLS7"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0ADCAmplperLS7  = new TH1F("h_sum0ADCAmplperLS7"," ",     bac, 1.,bac2);

    h_sumADCAmplLS8   = new TH1F("h_sumADCAmplLS8"," ",      100,  0.,lsdep_estimator1_HOdepth4_);
    h_2DsumADCAmplLS8    = new TH2F("h_2DsumADCAmplLS8"," ",    82, -41., 41., 72, 0., 72.);
    h_2DsumADCAmplLS8_LSselected= new TH2F("h_2DsumADCAmplLS8_LSselected"," ",82,-41.,41.,72,0.,72.);
    h_sumADCAmplperLS8  = new TH1F("h_sumADCAmplperLS8"," ",     bac, 1.,bac2);
    h_sumCutADCAmplperLS8  = new TH1F("h_sumCutADCAmplperLS8"," ",     bac, 1.,bac2);
    h_2D0sumADCAmplLS8    = new TH2F("h_2D0sumADCAmplLS8"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0ADCAmplperLS8  = new TH1F("h_sum0ADCAmplperLS8"," ",     bac, 1.,bac2);

    // error-A for HB( depth1 only) 
    h_sumADCAmplperLS1_P1  = new TH1F("h_sumADCAmplperLS1_P1"," ",     bac, 1.,bac2);
    h_sum0ADCAmplperLS1_P1  = new TH1F("h_sum0ADCAmplperLS1_P1"," ",     bac, 1.,bac2);
    h_sumADCAmplperLS1_P2  = new TH1F("h_sumADCAmplperLS1_P2"," ",     bac, 1.,bac2);
    h_sum0ADCAmplperLS1_P2  = new TH1F("h_sum0ADCAmplperLS1_P2"," ",     bac, 1.,bac2);
    h_sumADCAmplperLS1_M1  = new TH1F("h_sumADCAmplperLS1_M1"," ",     bac, 1.,bac2);
    h_sum0ADCAmplperLS1_M1  = new TH1F("h_sum0ADCAmplperLS1_M1"," ",     bac, 1.,bac2);
    h_sumADCAmplperLS1_M2  = new TH1F("h_sumADCAmplperLS1_M2"," ",     bac, 1.,bac2);
    h_sum0ADCAmplperLS1_M2  = new TH1F("h_sum0ADCAmplperLS1_M2"," ",     bac, 1.,bac2);

    // error-A for HE( depth1 only)
    h_sumADCAmplperLS3_P1  = new TH1F("h_sumADCAmplperLS3_P1"," ",     bac, 1.,bac2);
    h_sum0ADCAmplperLS3_P1  = new TH1F("h_sum0ADCAmplperLS3_P1"," ",     bac, 1.,bac2);
    h_sumADCAmplperLS3_P2  = new TH1F("h_sumADCAmplperLS3_P2"," ",     bac, 1.,bac2);
    h_sum0ADCAmplperLS3_P2  = new TH1F("h_sum0ADCAmplperLS3_P2"," ",     bac, 1.,bac2);
    h_sumADCAmplperLS3_M1  = new TH1F("h_sumADCAmplperLS3_M1"," ",     bac, 1.,bac2);
    h_sum0ADCAmplperLS3_M1  = new TH1F("h_sum0ADCAmplperLS3_M1"," ",     bac, 1.,bac2);
    h_sumADCAmplperLS3_M2  = new TH1F("h_sumADCAmplperLS3_M2"," ",     bac, 1.,bac2);
    h_sum0ADCAmplperLS3_M2  = new TH1F("h_sum0ADCAmplperLS3_M2"," ",     bac, 1.,bac2);

    // error-A for HF( depth1 only)
    h_sumADCAmplperLS6_P1  = new TH1F("h_sumADCAmplperLS6_P1"," ",     bac, 1.,bac2);
    h_sum0ADCAmplperLS6_P1  = new TH1F("h_sum0ADCAmplperLS6_P1"," ",     bac, 1.,bac2);
    h_sumADCAmplperLS6_P2  = new TH1F("h_sumADCAmplperLS6_P2"," ",     bac, 1.,bac2);
    h_sum0ADCAmplperLS6_P2  = new TH1F("h_sum0ADCAmplperLS6_P2"," ",     bac, 1.,bac2);
    h_sumADCAmplperLS6_M1  = new TH1F("h_sumADCAmplperLS6_M1"," ",     bac, 1.,bac2);
    h_sum0ADCAmplperLS6_M1  = new TH1F("h_sum0ADCAmplperLS6_M1"," ",     bac, 1.,bac2);
    h_sumADCAmplperLS6_M2  = new TH1F("h_sumADCAmplperLS6_M2"," ",     bac, 1.,bac2);
    h_sum0ADCAmplperLS6_M2  = new TH1F("h_sum0ADCAmplperLS6_M2"," ",     bac, 1.,bac2);

    // error-A for HO( depth4 only)
    h_sumADCAmplperLS8_P1  = new TH1F("h_sumADCAmplperLS8_P1"," ",     bac, 1.,bac2);
    h_sum0ADCAmplperLS8_P1  = new TH1F("h_sum0ADCAmplperLS8_P1"," ",     bac, 1.,bac2);
    h_sumADCAmplperLS8_P2  = new TH1F("h_sumADCAmplperLS8_P2"," ",     bac, 1.,bac2);
    h_sum0ADCAmplperLS8_P2  = new TH1F("h_sum0ADCAmplperLS8_P2"," ",     bac, 1.,bac2);
    h_sumADCAmplperLS8_M1  = new TH1F("h_sumADCAmplperLS8_M1"," ",     bac, 1.,bac2);
    h_sum0ADCAmplperLS8_M1  = new TH1F("h_sum0ADCAmplperLS8_M1"," ",     bac, 1.,bac2);
    h_sumADCAmplperLS8_M2  = new TH1F("h_sumADCAmplperLS8_M2"," ",     bac, 1.,bac2);
    h_sum0ADCAmplperLS8_M2  = new TH1F("h_sum0ADCAmplperLS8_M2"," ",     bac, 1.,bac2);


    // for estimator2:
//    float est2 = 6.;
//    float est21= 4.;
//    float est22= 7.;
    h_sumTSmeanALS1   = new TH1F("h_sumTSmeanALS1"," ",      100,  0.,lsdep_estimator2_HBdepth1_);
    h_2DsumTSmeanALS1    = new TH2F("h_2DsumTSmeanALS1"," ",    82, -41., 41., 72, 0., 72.);
    h_sumTSmeanAperLS1  = new TH1F("h_sumTSmeanAperLS1"," ",     bac, 1.,bac2);
    h_sumTSmeanAperLS1_LSselected  = new TH1F("h_sumTSmeanAperLS1_LSselected"," ",     bac, 1.,bac2);
    h_sumCutTSmeanAperLS1  = new TH1F("h_sumCutTSmeanAperLS1"," ",     bac, 1.,bac2);
    h_2D0sumTSmeanALS1    = new TH2F("h_2D0sumTSmeanALS1"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0TSmeanAperLS1  = new TH1F("h_sum0TSmeanAperLS1"," ",     bac, 1.,bac2);

    h_sumTSmeanALS2   = new TH1F("h_sumTSmeanALS2"," ",      100,  0.,lsdep_estimator2_HBdepth2_);
    h_2DsumTSmeanALS2    = new TH2F("h_2DsumTSmeanALS2"," ",    82, -41., 41., 72, 0., 72.);
    h_sumTSmeanAperLS2  = new TH1F("h_sumTSmeanAperLS2"," ",     bac, 1.,bac2);
    h_sumCutTSmeanAperLS2  = new TH1F("h_sumCutTSmeanAperLS2"," ",     bac, 1.,bac2);
    h_2D0sumTSmeanALS2    = new TH2F("h_2D0sumTSmeanALS2"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0TSmeanAperLS2  = new TH1F("h_sum0TSmeanAperLS2"," ",     bac, 1.,bac2);

    h_sumTSmeanALS3   = new TH1F("h_sumTSmeanALS3"," ",      100,  0.,lsdep_estimator2_HEdepth1_);
    h_2DsumTSmeanALS3    = new TH2F("h_2DsumTSmeanALS3"," ",    82, -41., 41., 72, 0., 72.);
    h_sumTSmeanAperLS3  = new TH1F("h_sumTSmeanAperLS3"," ",     bac, 1.,bac2);
    h_sumCutTSmeanAperLS3  = new TH1F("h_sumCutTSmeanAperLS3"," ",     bac, 1.,bac2);
    h_2D0sumTSmeanALS3    = new TH2F("h_2D0sumTSmeanALS3"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0TSmeanAperLS3  = new TH1F("h_sum0TSmeanAperLS3"," ",     bac, 1.,bac2);

    h_sumTSmeanALS4   = new TH1F("h_sumTSmeanALS4"," ",      100,  0.,lsdep_estimator2_HEdepth2_);
    h_2DsumTSmeanALS4    = new TH2F("h_2DsumTSmeanALS4"," ",    82, -41., 41., 72, 0., 72.);
    h_sumTSmeanAperLS4  = new TH1F("h_sumTSmeanAperLS4"," ",     bac, 1.,bac2);
    h_sumCutTSmeanAperLS4  = new TH1F("h_sumCutTSmeanAperLS4"," ",     bac, 1.,bac2);
    h_2D0sumTSmeanALS4    = new TH2F("h_2D0sumTSmeanALS4"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0TSmeanAperLS4  = new TH1F("h_sum0TSmeanAperLS4"," ",     bac, 1.,bac2);

    h_sumTSmeanALS5   = new TH1F("h_sumTSmeanALS5"," ",      100,  0.,lsdep_estimator2_HEdepth3_);
    h_2DsumTSmeanALS5    = new TH2F("h_2DsumTSmeanALS5"," ",    82, -41., 41., 72, 0., 72.);
    h_sumTSmeanAperLS5  = new TH1F("h_sumTSmeanAperLS5"," ",     bac, 1.,bac2);
    h_sumCutTSmeanAperLS5  = new TH1F("h_sumCutTSmeanAperLS5"," ",     bac, 1.,bac2);
    h_2D0sumTSmeanALS5    = new TH2F("h_2D0sumTSmeanALS5"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0TSmeanAperLS5  = new TH1F("h_sum0TSmeanAperLS5"," ",     bac, 1.,bac2);

    h_sumTSmeanALS6   = new TH1F("h_sumTSmeanALS6"," ",      100,  0.,lsdep_estimator2_HFdepth1_);
    h_2DsumTSmeanALS6    = new TH2F("h_2DsumTSmeanALS6"," ",    82, -41., 41., 72, 0., 72.);
    h_sumTSmeanAperLS6  = new TH1F("h_sumTSmeanAperLS6"," ",     bac, 1.,bac2);
    h_sumCutTSmeanAperLS6  = new TH1F("h_sumCutTSmeanAperLS6"," ",     bac, 1.,bac2);
    h_2D0sumTSmeanALS6    = new TH2F("h_2D0sumTSmeanALS6"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0TSmeanAperLS6  = new TH1F("h_sum0TSmeanAperLS6"," ",     bac, 1.,bac2);

    h_sumTSmeanALS7   = new TH1F("h_sumTSmeanALS7"," ",      100,  0.,lsdep_estimator2_HFdepth2_);
    h_2DsumTSmeanALS7    = new TH2F("h_2DsumTSmeanALS7"," ",    82, -41., 41., 72, 0., 72.);
    h_sumTSmeanAperLS7  = new TH1F("h_sumTSmeanAperLS7"," ",     bac, 1.,bac2);
    h_sumCutTSmeanAperLS7  = new TH1F("h_sumCutTSmeanAperLS7"," ",     bac, 1.,bac2);
    h_2D0sumTSmeanALS7    = new TH2F("h_2D0sumTSmeanALS7"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0TSmeanAperLS7  = new TH1F("h_sum0TSmeanAperLS7"," ",     bac, 1.,bac2);

    h_sumTSmeanALS8   = new TH1F("h_sumTSmeanALS8"," ",      100,  0.,lsdep_estimator2_HOdepth4_);
    h_2DsumTSmeanALS8    = new TH2F("h_2DsumTSmeanALS8"," ",    82, -41., 41., 72, 0., 72.);
    h_sumTSmeanAperLS8  = new TH1F("h_sumTSmeanAperLS8"," ",     bac, 1.,bac2);
    h_sumCutTSmeanAperLS8  = new TH1F("h_sumCutTSmeanAperLS8"," ",     bac, 1.,bac2);
    h_2D0sumTSmeanALS8    = new TH2F("h_2D0sumTSmeanALS8"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0TSmeanAperLS8  = new TH1F("h_sum0TSmeanAperLS8"," ",     bac, 1.,bac2);

    // for estimator3:
    //  float est3 = 10.0;
    h_sumTSmaxALS1   = new TH1F("h_sumTSmaxALS1"," ",      100,  0.,lsdep_estimator3_HBdepth1_);
    h_2DsumTSmaxALS1    = new TH2F("h_2DsumTSmaxALS1"," ",    82, -41., 41., 72, 0., 72.);
    h_sumTSmaxAperLS1  = new TH1F("h_sumTSmaxAperLS1"," ",     bac, 1.,bac2);
    h_sumTSmaxAperLS1_LSselected  = new TH1F("h_sumTSmaxAperLS1_LSselected"," ",     bac, 1.,bac2);
    h_sumCutTSmaxAperLS1  = new TH1F("h_sumCutTSmaxAperLS1"," ",     bac, 1.,bac2);
    h_2D0sumTSmaxALS1    = new TH2F("h_2D0sumTSmaxALS1"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0TSmaxAperLS1  = new TH1F("h_sum0TSmaxAperLS1"," ",     bac, 1.,bac2);

    h_sumTSmaxALS2   = new TH1F("h_sumTSmaxALS2"," ",      100,  0.,lsdep_estimator3_HBdepth2_);
    h_2DsumTSmaxALS2    = new TH2F("h_2DsumTSmaxALS2"," ",    82, -41., 41., 72, 0., 72.);
    h_sumTSmaxAperLS2  = new TH1F("h_sumTSmaxAperLS2"," ",     bac, 1.,bac2);
    h_sumCutTSmaxAperLS2  = new TH1F("h_sumCutTSmaxAperLS2"," ",     bac, 1.,bac2);
    h_2D0sumTSmaxALS2    = new TH2F("h_2D0sumTSmaxALS2"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0TSmaxAperLS2  = new TH1F("h_sum0TSmaxAperLS2"," ",     bac, 1.,bac2);

    h_sumTSmaxALS3   = new TH1F("h_sumTSmaxALS3"," ",      100,  0.,lsdep_estimator3_HEdepth1_);
    h_2DsumTSmaxALS3    = new TH2F("h_2DsumTSmaxALS3"," ",    82, -41., 41., 72, 0., 72.);
    h_sumTSmaxAperLS3  = new TH1F("h_sumTSmaxAperLS3"," ",     bac, 1.,bac2);
    h_sumCutTSmaxAperLS3  = new TH1F("h_sumCutTSmaxAperLS3"," ",     bac, 1.,bac2);
    h_2D0sumTSmaxALS3    = new TH2F("h_2D0sumTSmaxALS3"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0TSmaxAperLS3  = new TH1F("h_sum0TSmaxAperLS3"," ",     bac, 1.,bac2);

    h_sumTSmaxALS4   = new TH1F("h_sumTSmaxALS4"," ",      100,  0.,lsdep_estimator3_HEdepth2_);
    h_2DsumTSmaxALS4    = new TH2F("h_2DsumTSmaxALS4"," ",    82, -41., 41., 72, 0., 72.);
    h_sumTSmaxAperLS4  = new TH1F("h_sumTSmaxAperLS4"," ",     bac, 1.,bac2);
    h_sumCutTSmaxAperLS4  = new TH1F("h_sumCutTSmaxAperLS4"," ",     bac, 1.,bac2);
    h_2D0sumTSmaxALS4    = new TH2F("h_2D0sumTSmaxALS4"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0TSmaxAperLS4  = new TH1F("h_sum0TSmaxAperLS4"," ",     bac, 1.,bac2);

    h_sumTSmaxALS5   = new TH1F("h_sumTSmaxALS5"," ",      100,  0.,lsdep_estimator3_HEdepth3_);
    h_2DsumTSmaxALS5    = new TH2F("h_2DsumTSmaxALS5"," ",    82, -41., 41., 72, 0., 72.);
    h_sumTSmaxAperLS5  = new TH1F("h_sumTSmaxAperLS5"," ",     bac, 1.,bac2);
    h_sumCutTSmaxAperLS5  = new TH1F("h_sumCutTSmaxAperLS5"," ",     bac, 1.,bac2);
    h_2D0sumTSmaxALS5    = new TH2F("h_2D0sumTSmaxALS5"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0TSmaxAperLS5  = new TH1F("h_sum0TSmaxAperLS5"," ",     bac, 1.,bac2);

    h_sumTSmaxALS6   = new TH1F("h_sumTSmaxALS6"," ",      100,  0.,lsdep_estimator3_HFdepth1_);
    h_2DsumTSmaxALS6    = new TH2F("h_2DsumTSmaxALS6"," ",    82, -41., 41., 72, 0., 72.);
    h_sumTSmaxAperLS6  = new TH1F("h_sumTSmaxAperLS6"," ",     bac, 1.,bac2);
    h_sumCutTSmaxAperLS6  = new TH1F("h_sumCutTSmaxAperLS6"," ",     bac, 1.,bac2);
    h_2D0sumTSmaxALS6    = new TH2F("h_2D0sumTSmaxALS6"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0TSmaxAperLS6  = new TH1F("h_sum0TSmaxAperLS6"," ",     bac, 1.,bac2);

    h_sumTSmaxALS7   = new TH1F("h_sumTSmaxALS7"," ",      100,  0.,lsdep_estimator3_HFdepth2_);
    h_2DsumTSmaxALS7    = new TH2F("h_2DsumTSmaxALS7"," ",    82, -41., 41., 72, 0., 72.);
    h_sumTSmaxAperLS7  = new TH1F("h_sumTSmaxAperLS7"," ",     bac, 1.,bac2);
    h_sumCutTSmaxAperLS7  = new TH1F("h_sumCutTSmaxAperLS7"," ",     bac, 1.,bac2);
    h_2D0sumTSmaxALS7    = new TH2F("h_2D0sumTSmaxALS7"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0TSmaxAperLS7  = new TH1F("h_sum0TSmaxAperLS7"," ",     bac, 1.,bac2);

    h_sumTSmaxALS8   = new TH1F("h_sumTSmaxALS8"," ",      100,  0.,lsdep_estimator3_HOdepth4_);
    h_2DsumTSmaxALS8    = new TH2F("h_2DsumTSmaxALS8"," ",    82, -41., 41., 72, 0., 72.);
    h_sumTSmaxAperLS8  = new TH1F("h_sumTSmaxAperLS8"," ",     bac, 1.,bac2);
    h_sumCutTSmaxAperLS8  = new TH1F("h_sumCutTSmaxAperLS8"," ",     bac, 1.,bac2);
    h_2D0sumTSmaxALS8    = new TH2F("h_2D0sumTSmaxALS8"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0TSmaxAperLS8  = new TH1F("h_sum0TSmaxAperLS8"," ",     bac, 1.,bac2);

    // for estimator4:
    //  float est4 = 3.4;
    //  float est41= 2.0;
    h_sumAmplitudeLS1   = new TH1F("h_sumAmplitudeLS1"," ",      100,  0.0,lsdep_estimator4_HBdepth1_);
    h_2DsumAmplitudeLS1    = new TH2F("h_2DsumAmplitudeLS1"," ",    82, -41., 41., 72, 0., 72.);
    h_sumAmplitudeperLS1  = new TH1F("h_sumAmplitudeperLS1"," ",     bac, 1.,bac2);
    h_sumAmplitudeperLS1_LSselected = new TH1F("h_sumAmplitudeperLS1_LSselected"," ", bac, 1.,bac2);
    h_sumCutAmplitudeperLS1  = new TH1F("h_sumCutAmplitudeperLS1"," ",     bac, 1.,bac2);
    h_2D0sumAmplitudeLS1    = new TH2F("h_2D0sumAmplitudeLS1"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0AmplitudeperLS1  = new TH1F("h_sum0AmplitudeperLS1"," ",     bac, 1.,bac2);

    h_sumAmplitudeLS2   = new TH1F("h_sumAmplitudeLS2"," ",      100,  0.0,lsdep_estimator4_HBdepth2_);
    h_2DsumAmplitudeLS2    = new TH2F("h_2DsumAmplitudeLS2"," ",    82, -41., 41., 72, 0., 72.);
    h_sumAmplitudeperLS2  = new TH1F("h_sumAmplitudeperLS2"," ",     bac, 1.,bac2);
    h_sumCutAmplitudeperLS2  = new TH1F("h_sumCutAmplitudeperLS2"," ",     bac, 1.,bac2);
    h_2D0sumAmplitudeLS2    = new TH2F("h_2D0sumAmplitudeLS2"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0AmplitudeperLS2  = new TH1F("h_sum0AmplitudeperLS2"," ",     bac, 1.,bac2);

    h_sumAmplitudeLS3   = new TH1F("h_sumAmplitudeLS3"," ",      100,  0.0,lsdep_estimator4_HEdepth1_);
    h_2DsumAmplitudeLS3    = new TH2F("h_2DsumAmplitudeLS3"," ",    82, -41., 41., 72, 0., 72.);
    h_sumAmplitudeperLS3  = new TH1F("h_sumAmplitudeperLS3"," ",     bac, 1.,bac2);
    h_sumCutAmplitudeperLS3  = new TH1F("h_sumCutAmplitudeperLS3"," ",     bac, 1.,bac2);
    h_2D0sumAmplitudeLS3    = new TH2F("h_2D0sumAmplitudeLS3"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0AmplitudeperLS3  = new TH1F("h_sum0AmplitudeperLS3"," ",     bac, 1.,bac2);

    h_sumAmplitudeLS4   = new TH1F("h_sumAmplitudeLS4"," ",      100,  0.0,lsdep_estimator4_HEdepth2_);
    h_2DsumAmplitudeLS4    = new TH2F("h_2DsumAmplitudeLS4"," ",    82, -41., 41., 72, 0., 72.);
    h_sumAmplitudeperLS4  = new TH1F("h_sumAmplitudeperLS4"," ",     bac, 1.,bac2);
    h_sumCutAmplitudeperLS4  = new TH1F("h_sumCutAmplitudeperLS4"," ",     bac, 1.,bac2);
    h_2D0sumAmplitudeLS4    = new TH2F("h_2D0sumAmplitudeLS4"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0AmplitudeperLS4  = new TH1F("h_sum0AmplitudeperLS4"," ",     bac, 1.,bac2);

    h_sumAmplitudeLS5   = new TH1F("h_sumAmplitudeLS5"," ",      100,  0.0,lsdep_estimator4_HEdepth3_);
    h_2DsumAmplitudeLS5    = new TH2F("h_2DsumAmplitudeLS5"," ",    82, -41., 41., 72, 0., 72.);
    h_sumAmplitudeperLS5  = new TH1F("h_sumAmplitudeperLS5"," ",     bac, 1.,bac2);
    h_sumCutAmplitudeperLS5  = new TH1F("h_sumCutAmplitudeperLS5"," ",     bac, 1.,bac2);
    h_2D0sumAmplitudeLS5    = new TH2F("h_2D0sumAmplitudeLS5"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0AmplitudeperLS5  = new TH1F("h_sum0AmplitudeperLS5"," ",     bac, 1.,bac2);

    h_sumAmplitudeLS6   = new TH1F("h_sumAmplitudeLS6"," ",      100,  0.,lsdep_estimator4_HFdepth1_);
    h_2DsumAmplitudeLS6    = new TH2F("h_2DsumAmplitudeLS6"," ",    82, -41., 41., 72, 0., 72.);
    h_sumAmplitudeperLS6  = new TH1F("h_sumAmplitudeperLS6"," ",     bac, 1.,bac2);
    h_sumCutAmplitudeperLS6  = new TH1F("h_sumCutAmplitudeperLS6"," ",     bac, 1.,bac2);
    h_2D0sumAmplitudeLS6    = new TH2F("h_2D0sumAmplitudeLS6"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0AmplitudeperLS6  = new TH1F("h_sum0AmplitudeperLS6"," ",     bac, 1.,bac2);

    h_sumAmplitudeLS7   = new TH1F("h_sumAmplitudeLS7"," ",      100,  0.,lsdep_estimator4_HFdepth2_);
    h_2DsumAmplitudeLS7    = new TH2F("h_2DsumAmplitudeLS7"," ",    82, -41., 41., 72, 0., 72.);
    h_sumAmplitudeperLS7  = new TH1F("h_sumAmplitudeperLS7"," ",     bac, 1.,bac2);
    h_sumCutAmplitudeperLS7  = new TH1F("h_sumCutAmplitudeperLS7"," ",     bac, 1.,bac2);
    h_2D0sumAmplitudeLS7    = new TH2F("h_2D0sumAmplitudeLS7"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0AmplitudeperLS7  = new TH1F("h_sum0AmplitudeperLS7"," ",     bac, 1.,bac2);

    h_sumAmplitudeLS8   = new TH1F("h_sumAmplitudeLS8"," ",      100,  0.,lsdep_estimator4_HOdepth4_);
    h_2DsumAmplitudeLS8    = new TH2F("h_2DsumAmplitudeLS8"," ",    82, -41., 41., 72, 0., 72.);
    h_sumAmplitudeperLS8  = new TH1F("h_sumAmplitudeperLS8"," ",     bac, 1.,bac2);
    h_sumCutAmplitudeperLS8  = new TH1F("h_sumCutAmplitudeperLS8"," ",     bac, 1.,bac2);
    h_2D0sumAmplitudeLS8    = new TH2F("h_2D0sumAmplitudeLS8"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0AmplitudeperLS8  = new TH1F("h_sum0AmplitudeperLS8"," ",     bac, 1.,bac2);





    // for estimator5:
    //  float est5 = 0.6;
    //  float est51= 1.0;
    //  float est52= 0.8;
    h_sumAmplLS1   = new TH1F("h_sumAmplLS1"," ",      100,  0.0,lsdep_estimator5_HBdepth1_);
    h_2DsumAmplLS1    = new TH2F("h_2DsumAmplLS1"," ",    82, -41., 41., 72, 0., 72.);
    h_sumAmplperLS1  = new TH1F("h_sumAmplperLS1"," ",     bac, 1.,bac2);
    h_sumAmplperLS1_LSselected  = new TH1F("h_sumAmplperLS1_LSselected"," ",     bac, 1.,bac2);
    h_sumCutAmplperLS1  = new TH1F("h_sumCutAmplperLS1"," ",     bac, 1.,bac2);
    h_2D0sumAmplLS1    = new TH2F("h_2D0sumAmplLS1"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0AmplperLS1  = new TH1F("h_sum0AmplperLS1"," ",     bac, 1.,bac2);

    h_sumAmplLS2   = new TH1F("h_sumAmplLS2"," ",      100,  0.0,lsdep_estimator5_HBdepth2_);
    h_2DsumAmplLS2    = new TH2F("h_2DsumAmplLS2"," ",    82, -41., 41., 72, 0., 72.);
    h_sumAmplperLS2  = new TH1F("h_sumAmplperLS2"," ",     bac, 1.,bac2);
    h_sumCutAmplperLS2  = new TH1F("h_sumCutAmplperLS2"," ",     bac, 1.,bac2);
    h_2D0sumAmplLS2    = new TH2F("h_2D0sumAmplLS2"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0AmplperLS2  = new TH1F("h_sum0AmplperLS2"," ",     bac, 1.,bac2);

    h_sumAmplLS3   = new TH1F("h_sumAmplLS3"," ",      100,  0.0,lsdep_estimator5_HEdepth1_);
    h_2DsumAmplLS3    = new TH2F("h_2DsumAmplLS3"," ",    82, -41., 41., 72, 0., 72.);
    h_sumAmplperLS3  = new TH1F("h_sumAmplperLS3"," ",     bac, 1.,bac2);
    h_sumCutAmplperLS3  = new TH1F("h_sumCutAmplperLS3"," ",     bac, 1.,bac2);
    h_2D0sumAmplLS3    = new TH2F("h_2D0sumAmplLS3"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0AmplperLS3  = new TH1F("h_sum0AmplperLS3"," ",     bac, 1.,bac2);

    h_sumAmplLS4   = new TH1F("h_sumAmplLS4"," ",      100,  0.0,lsdep_estimator5_HEdepth2_);
    h_2DsumAmplLS4    = new TH2F("h_2DsumAmplLS4"," ",    82, -41., 41., 72, 0., 72.);
    h_sumAmplperLS4  = new TH1F("h_sumAmplperLS4"," ",     bac, 1.,bac2);
    h_sumCutAmplperLS4  = new TH1F("h_sumCutAmplperLS4"," ",     bac, 1.,bac2);
    h_2D0sumAmplLS4    = new TH2F("h_2D0sumAmplLS4"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0AmplperLS4  = new TH1F("h_sum0AmplperLS4"," ",     bac, 1.,bac2);

    h_sumAmplLS5   = new TH1F("h_sumAmplLS5"," ",      100,  0.0,lsdep_estimator5_HEdepth3_);
    h_2DsumAmplLS5    = new TH2F("h_2DsumAmplLS5"," ",    82, -41., 41., 72, 0., 72.);
    h_sumAmplperLS5  = new TH1F("h_sumAmplperLS5"," ",     bac, 1.,bac2);
    h_sumCutAmplperLS5  = new TH1F("h_sumCutAmplperLS5"," ",     bac, 1.,bac2);
    h_2D0sumAmplLS5    = new TH2F("h_2D0sumAmplLS5"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0AmplperLS5  = new TH1F("h_sum0AmplperLS5"," ",     bac, 1.,bac2);

    h_sumAmplLS6   = new TH1F("h_sumAmplLS6"," ",      100,  0.0,lsdep_estimator5_HFdepth1_);
    h_2DsumAmplLS6    = new TH2F("h_2DsumAmplLS6"," ",    82, -41., 41., 72, 0., 72.);
    h_sumAmplperLS6  = new TH1F("h_sumAmplperLS6"," ",     bac, 1.,bac2);
    h_sumCutAmplperLS6  = new TH1F("h_sumCutAmplperLS6"," ",     bac, 1.,bac2);
    h_2D0sumAmplLS6    = new TH2F("h_2D0sumAmplLS6"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0AmplperLS6  = new TH1F("h_sum0AmplperLS6"," ",     bac, 1.,bac2);

    h_RatioOccupancy_HBP  = new TH1F("h_RatioOccupancy_HBP"," ",     bac, 1.,bac2);
    h_RatioOccupancy_HBM  = new TH1F("h_RatioOccupancy_HBM"," ",     bac, 1.,bac2);
    h_RatioOccupancy_HEP  = new TH1F("h_RatioOccupancy_HEP"," ",     bac, 1.,bac2);
    h_RatioOccupancy_HEM  = new TH1F("h_RatioOccupancy_HEM"," ",     bac, 1.,bac2);
    h_RatioOccupancy_HOP  = new TH1F("h_RatioOccupancy_HOP"," ",     bac, 1.,bac2);
    h_RatioOccupancy_HOM  = new TH1F("h_RatioOccupancy_HOM"," ",     bac, 1.,bac2);
    h_RatioOccupancy_HFP  = new TH1F("h_RatioOccupancy_HFP"," ",     bac, 1.,bac2);
    h_RatioOccupancy_HFM  = new TH1F("h_RatioOccupancy_HFM"," ",     bac, 1.,bac2);

    h_sumAmplLS7   = new TH1F("h_sumAmplLS7"," ",      100,  0.0,lsdep_estimator5_HFdepth2_);
    h_2DsumAmplLS7    = new TH2F("h_2DsumAmplLS7"," ",    82, -41., 41., 72, 0., 72.);
    h_sumAmplperLS7  = new TH1F("h_sumAmplperLS7"," ",     bac, 1.,bac2);
    h_sumCutAmplperLS7  = new TH1F("h_sumCutAmplperLS7"," ",     bac, 1.,bac2);
    h_2D0sumAmplLS7    = new TH2F("h_2D0sumAmplLS7"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0AmplperLS7  = new TH1F("h_sum0AmplperLS7"," ",     bac, 1.,bac2);

    h_sumAmplLS8   = new TH1F("h_sumAmplLS8"," ",      100,  0.0,lsdep_estimator5_HOdepth4_);
    h_2DsumAmplLS8    = new TH2F("h_2DsumAmplLS8"," ",    82, -41., 41., 72, 0., 72.);
    h_sumAmplperLS8  = new TH1F("h_sumAmplperLS8"," ",     bac, 1.,bac2);
    h_sumCutAmplperLS8  = new TH1F("h_sumCutAmplperLS8"," ",     bac, 1.,bac2);
    h_2D0sumAmplLS8    = new TH2F("h_2D0sumAmplLS8"," ",    82, -41., 41., 72, 0., 72.);
    h_sum0AmplperLS8  = new TH1F("h_sum0AmplperLS8"," ",     bac, 1.,bac2);

    // for estimator6:
    h_sumErrorBLS1   = new TH1F("h_sumErrorBLS1"," ",      10,  0.,10.);
    h_sumErrorBperLS1  = new TH1F("h_sumErrorBperLS1"," ",     bac, 1.,bac2);
    h_sum0ErrorBperLS1  = new TH1F("h_sum0ErrorBperLS1"," ",     bac, 1.,bac2);
    h_2D0sumErrorBLS1    = new TH2F("h_2D0sumErrorBLS1"," ",    82, -41., 41., 72, 0., 72.);
    h_2DsumErrorBLS1    = new TH2F("h_2DsumErrorBLS1"," ",    82, -41., 41., 72, 0., 72.);
    h_sumErrorBLS2   = new TH1F("h_sumErrorBLS2"," ",      10,  0.,10.);
    h_sumErrorBperLS2  = new TH1F("h_sumErrorBperLS2"," ",     bac, 1.,bac2);
    h_sum0ErrorBperLS2  = new TH1F("h_sum0ErrorBperLS2"," ",     bac, 1.,bac2);
    h_2D0sumErrorBLS2    = new TH2F("h_2D0sumErrorBLS2"," ",    82, -41., 41., 72, 0., 72.);
    h_2DsumErrorBLS2    = new TH2F("h_2DsumErrorBLS2"," ",    82, -41., 41., 72, 0., 72.);

    h_sumErrorBLS3   = new TH1F("h_sumErrorBLS3"," ",      10,  0.,10.);
    h_sumErrorBperLS3  = new TH1F("h_sumErrorBperLS3"," ",     bac, 1.,bac2);
    h_sum0ErrorBperLS3  = new TH1F("h_sum0ErrorBperLS3"," ",     bac, 1.,bac2);
    h_2D0sumErrorBLS3    = new TH2F("h_2D0sumErrorBLS3"," ",    82, -41., 41., 72, 0., 72.);
    h_2DsumErrorBLS3    = new TH2F("h_2DsumErrorBLS3"," ",    82, -41., 41., 72, 0., 72.);
    h_sumErrorBLS4   = new TH1F("h_sumErrorBLS4"," ",      10,  0.,10.);
    h_sumErrorBperLS4  = new TH1F("h_sumErrorBperLS4"," ",     bac, 1.,bac2);
    h_sum0ErrorBperLS4  = new TH1F("h_sum0ErrorBperLS4"," ",     bac, 1.,bac2);
    h_2D0sumErrorBLS4    = new TH2F("h_2D0sumErrorBLS4"," ",    82, -41., 41., 72, 0., 72.);
    h_2DsumErrorBLS4    = new TH2F("h_2DsumErrorBLS4"," ",    82, -41., 41., 72, 0., 72.);
    h_sumErrorBLS5   = new TH1F("h_sumErrorBLS5"," ",      10,  0.,10.);
    h_sumErrorBperLS5  = new TH1F("h_sumErrorBperLS5"," ",     bac, 1.,bac2);
    h_sum0ErrorBperLS5  = new TH1F("h_sum0ErrorBperLS5"," ",     bac, 1.,bac2);
    h_2D0sumErrorBLS5    = new TH2F("h_2D0sumErrorBLS5"," ",    82, -41., 41., 72, 0., 72.);
    h_2DsumErrorBLS5    = new TH2F("h_2DsumErrorBLS5"," ",    82, -41., 41., 72, 0., 72.);

    h_sumErrorBLS6   = new TH1F("h_sumErrorBLS6"," ",      10,  0.,10.);
    h_sumErrorBperLS6  = new TH1F("h_sumErrorBperLS6"," ",     bac, 1.,bac2);
    h_sum0ErrorBperLS6  = new TH1F("h_sum0ErrorBperLS6"," ",     bac, 1.,bac2);
    h_2D0sumErrorBLS6    = new TH2F("h_2D0sumErrorBLS6"," ",    82, -41., 41., 72, 0., 72.);
    h_2DsumErrorBLS6    = new TH2F("h_2DsumErrorBLS6"," ",    82, -41., 41., 72, 0., 72.);
    h_sumErrorBLS7   = new TH1F("h_sumErrorBLS7"," ",      10,  0.,10.);
    h_sumErrorBperLS7  = new TH1F("h_sumErrorBperLS7"," ",     bac, 1.,bac2);
    h_sum0ErrorBperLS7  = new TH1F("h_sum0ErrorBperLS7"," ",     bac, 1.,bac2);
    h_2D0sumErrorBLS7    = new TH2F("h_2D0sumErrorBLS7"," ",    82, -41., 41., 72, 0., 72.);
    h_2DsumErrorBLS7    = new TH2F("h_2DsumErrorBLS7"," ",    82, -41., 41., 72, 0., 72.);

    h_sumErrorBLS8   = new TH1F("h_sumErrorBLS8"," ",      10,  0.,10.);
    h_sumErrorBperLS8  = new TH1F("h_sumErrorBperLS8"," ",     bac, 1.,bac2);
    h_sum0ErrorBperLS8  = new TH1F("h_sum0ErrorBperLS8"," ",     bac, 1.,bac2);
    h_2D0sumErrorBLS8    = new TH2F("h_2D0sumErrorBLS8"," ",    82, -41., 41., 72, 0., 72.);
    h_2DsumErrorBLS8    = new TH2F("h_2DsumErrorBLS8"," ",    82, -41., 41., 72, 0., 72.);


    // for averSIGNALOCCUPANCY :
    h_averSIGNALoccupancy_HB  = new TH1F("h_averSIGNALoccupancy_HB"," ",     bac, 1.,bac2);
    h_averSIGNALoccupancy_HE  = new TH1F("h_averSIGNALoccupancy_HE"," ",     bac, 1.,bac2);
    h_averSIGNALoccupancy_HF  = new TH1F("h_averSIGNALoccupancy_HF"," ",     bac, 1.,bac2);
    h_averSIGNALoccupancy_HO  = new TH1F("h_averSIGNALoccupancy_HO"," ",     bac, 1.,bac2);
    
    // for averSIGNALsumamplitude :
    h_averSIGNALsumamplitude_HB  = new TH1F("h_averSIGNALsumamplitude_HB"," ",     bac, 1.,bac2);
    h_averSIGNALsumamplitude_HE  = new TH1F("h_averSIGNALsumamplitude_HE"," ",     bac, 1.,bac2);
    h_averSIGNALsumamplitude_HF  = new TH1F("h_averSIGNALsumamplitude_HF"," ",     bac, 1.,bac2);
    h_averSIGNALsumamplitude_HO  = new TH1F("h_averSIGNALsumamplitude_HO"," ",     bac, 1.,bac2);
    
    // for averNOSIGNALOCCUPANCY :
    h_averNOSIGNALoccupancy_HB  = new TH1F("h_averNOSIGNALoccupancy_HB"," ",     bac, 1.,bac2);
    h_averNOSIGNALoccupancy_HE  = new TH1F("h_averNOSIGNALoccupancy_HE"," ",     bac, 1.,bac2);
    h_averNOSIGNALoccupancy_HF  = new TH1F("h_averNOSIGNALoccupancy_HF"," ",     bac, 1.,bac2);
    h_averNOSIGNALoccupancy_HO  = new TH1F("h_averNOSIGNALoccupancy_HO"," ",     bac, 1.,bac2);
    
    // for averNOSIGNALsumamplitude :
    h_averNOSIGNALsumamplitude_HB  = new TH1F("h_averNOSIGNALsumamplitude_HB"," ",     bac, 1.,bac2);
    h_averNOSIGNALsumamplitude_HE  = new TH1F("h_averNOSIGNALsumamplitude_HE"," ",     bac, 1.,bac2);
    h_averNOSIGNALsumamplitude_HF  = new TH1F("h_averNOSIGNALsumamplitude_HF"," ",     bac, 1.,bac2);
    h_averNOSIGNALsumamplitude_HO  = new TH1F("h_averNOSIGNALsumamplitude_HO"," ",     bac, 1.,bac2);
    
    // for channel SUM over depthes Amplitudes for each sub-detector
    h_sumamplitudechannel_HB  = new TH1F("h_sumamplitudechannel_HB"," ",      100,  0., 2000.);
    h_sumamplitudechannel_HE  = new TH1F("h_sumamplitudechannel_HE"," ",      100,  0., 3000.);
    h_sumamplitudechannel_HF  = new TH1F("h_sumamplitudechannel_HF"," ",      100,  0., 7000.);
    h_sumamplitudechannel_HO  = new TH1F("h_sumamplitudechannel_HO"," ",      100,  0.,10000.);

    // for event Amplitudes for each sub-detector
    h_eventamplitude_HB  = new TH1F("h_eventamplitude_HB"," ",      100,  0.,  80000.);
    h_eventamplitude_HE  = new TH1F("h_eventamplitude_HE"," ",      100,  0., 100000.);
    h_eventamplitude_HF  = new TH1F("h_eventamplitude_HF"," ",      100,  0., 150000.);
    h_eventamplitude_HO  = new TH1F("h_eventamplitude_HO"," ",      100,  0., 250000.);

    // for event Occupancy for each sub-detector
    h_eventoccupancy_HB  = new TH1F("h_eventoccupancy_HB"," ",      100,  0.,  3000.);
    h_eventoccupancy_HE  = new TH1F("h_eventoccupancy_HE"," ",      100,  0.,  2000.);
    h_eventoccupancy_HF  = new TH1F("h_eventoccupancy_HF"," ",      100,  0.,  1000.);
    h_eventoccupancy_HO  = new TH1F("h_eventoccupancy_HO"," ",      100,  0.,  2500.);


    // for maxxSUMAmplitude 
    h_maxxSUMAmpl_HB = new TH1F("h_maxxSUMAmpl_HB"," ",     bac, 1.,bac2);
    h_maxxSUMAmpl_HE = new TH1F("h_maxxSUMAmpl_HE"," ",     bac, 1.,bac2);
    h_maxxSUMAmpl_HF = new TH1F("h_maxxSUMAmpl_HF"," ",     bac, 1.,bac2);
    h_maxxSUMAmpl_HO = new TH1F("h_maxxSUMAmpl_HO"," ",     bac, 1.,bac2);

    // for maxxOCCUP 
    h_maxxOCCUP_HB = new TH1F("h_maxxOCCUP_HB"," ",     bac, 1.,bac2);
    h_maxxOCCUP_HE = new TH1F("h_maxxOCCUP_HE"," ",     bac, 1.,bac2);
    h_maxxOCCUP_HF = new TH1F("h_maxxOCCUP_HF"," ",     bac, 1.,bac2);
    h_maxxOCCUP_HO = new TH1F("h_maxxOCCUP_HO"," ",     bac, 1.,bac2);

    // pedestals
    h_pedestal0_HB  = new TH1F("h_pedestal0_HB"," ", 100, 0.,10.);
    h_pedestal1_HB  = new TH1F("h_pedestal1_HB"," ", 100, 0.,10.);
    h_pedestal2_HB  = new TH1F("h_pedestal2_HB"," ", 100, 0.,10.);
    h_pedestal3_HB  = new TH1F("h_pedestal3_HB"," ", 100, 0.,10.);
    h_pedestalaver4_HB  = new TH1F("h_pedestalaver4_HB"," ", 100, 0.,10.);
    h_pedestalaver9_HB  = new TH1F("h_pedestalaver9_HB"," ", 100, 0.,10.);
    h_pedestalw0_HB  = new TH1F("h_pedestalw0_HB"," ", 100, 0.,2.5);
    h_pedestalw1_HB  = new TH1F("h_pedestalw1_HB"," ", 100, 0.,2.5);
    h_pedestalw2_HB  = new TH1F("h_pedestalw2_HB"," ", 100, 0.,2.5);
    h_pedestalw3_HB  = new TH1F("h_pedestalw3_HB"," ", 100, 0.,2.5);
    h_pedestalwaver4_HB  = new TH1F("h_pedestalwaver4_HB"," ", 100, 0.,2.5);
    h_pedestalwaver9_HB  = new TH1F("h_pedestalwaver9_HB"," ", 100, 0.,2.5);

    h_pedestal0_HE  = new TH1F("h_pedestal0_HE"," ", 100, 0.,10.);
    h_pedestal1_HE  = new TH1F("h_pedestal1_HE"," ", 100, 0.,10.);
    h_pedestal2_HE  = new TH1F("h_pedestal2_HE"," ", 100, 0.,10.);
    h_pedestal3_HE  = new TH1F("h_pedestal3_HE"," ", 100, 0.,10.);
    h_pedestalaver4_HE  = new TH1F("h_pedestalaver4_HE"," ", 100, 0.,10.);
    h_pedestalaver9_HE  = new TH1F("h_pedestalaver9_HE"," ", 100, 0.,10.);
    h_pedestalw0_HE  = new TH1F("h_pedestalw0_HE"," ", 100, 0.,2.5);
    h_pedestalw1_HE  = new TH1F("h_pedestalw1_HE"," ", 100, 0.,2.5);
    h_pedestalw2_HE  = new TH1F("h_pedestalw2_HE"," ", 100, 0.,2.5);
    h_pedestalw3_HE  = new TH1F("h_pedestalw3_HE"," ", 100, 0.,2.5);
    h_pedestalwaver4_HE  = new TH1F("h_pedestalwaver4_HE"," ", 100, 0.,2.5);
    h_pedestalwaver9_HE  = new TH1F("h_pedestalwaver9_HE"," ", 100, 0.,2.5);

    h_pedestal0_HF  = new TH1F("h_pedestal0_HF"," ", 100, 0.,20.);
    h_pedestal1_HF  = new TH1F("h_pedestal1_HF"," ", 100, 0.,20.);
    h_pedestal2_HF  = new TH1F("h_pedestal2_HF"," ", 100, 0.,20.);
    h_pedestal3_HF  = new TH1F("h_pedestal3_HF"," ", 100, 0.,20.);
    h_pedestalaver4_HF  = new TH1F("h_pedestalaver4_HF"," ", 100, 0.,20.);
    h_pedestalaver9_HF  = new TH1F("h_pedestalaver9_HF"," ", 100, 0.,20.);
    h_pedestalw0_HF  = new TH1F("h_pedestalw0_HF"," ", 100, 0.,2.5);
    h_pedestalw1_HF  = new TH1F("h_pedestalw1_HF"," ", 100, 0.,2.5);
    h_pedestalw2_HF  = new TH1F("h_pedestalw2_HF"," ", 100, 0.,2.5);
    h_pedestalw3_HF  = new TH1F("h_pedestalw3_HF"," ", 100, 0.,2.5);
    h_pedestalwaver4_HF  = new TH1F("h_pedestalwaver4_HF"," ", 100, 0.,2.5);
    h_pedestalwaver9_HF  = new TH1F("h_pedestalwaver9_HF"," ", 100, 0.,2.5);

    h_pedestal0_HO  = new TH1F("h_pedestal0_HO"," ", 100, 0.,20.);
    h_pedestal1_HO  = new TH1F("h_pedestal1_HO"," ", 100, 0.,20.);
    h_pedestal2_HO  = new TH1F("h_pedestal2_HO"," ", 100, 0.,20.);
    h_pedestal3_HO  = new TH1F("h_pedestal3_HO"," ", 100, 0.,20.);
    h_pedestalaver4_HO  = new TH1F("h_pedestalaver4_HO"," ", 100, 0.,20.);
    h_pedestalaver9_HO  = new TH1F("h_pedestalaver9_HO"," ", 100, 0.,20.);
    h_pedestalw0_HO  = new TH1F("h_pedestalw0_HO"," ", 100, 0.,2.5);
    h_pedestalw1_HO  = new TH1F("h_pedestalw1_HO"," ", 100, 0.,2.5);
    h_pedestalw2_HO  = new TH1F("h_pedestalw2_HO"," ", 100, 0.,2.5);
    h_pedestalw3_HO  = new TH1F("h_pedestalw3_HO"," ", 100, 0.,2.5);
    h_pedestalwaver4_HO  = new TH1F("h_pedestalwaver4_HO"," ", 100, 0.,2.5);
    h_pedestalwaver9_HO  = new TH1F("h_pedestalwaver9_HO"," ", 100, 0.,2.5);

    h_mapDepth1pedestalw_HB = new TH2F("h_mapDepth1pedestalw_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2pedestalw_HB = new TH2F("h_mapDepth2pedestalw_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1pedestalw_HE = new TH2F("h_mapDepth1pedestalw_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2pedestalw_HE = new TH2F("h_mapDepth2pedestalw_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth3pedestalw_HE = new TH2F("h_mapDepth3pedestalw_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1pedestalw_HF = new TH2F("h_mapDepth1pedestalw_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2pedestalw_HF = new TH2F("h_mapDepth2pedestalw_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth4pedestalw_HO = new TH2F("h_mapDepth4pedestalw_HO"," ", 82, -41., 41., 72, 0., 72.);

    h_mapDepth1pedestal_HB = new TH2F("h_mapDepth1pedestal_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2pedestal_HB = new TH2F("h_mapDepth2pedestal_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1pedestal_HE = new TH2F("h_mapDepth1pedestal_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2pedestal_HE = new TH2F("h_mapDepth2pedestal_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth3pedestal_HE = new TH2F("h_mapDepth3pedestal_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1pedestal_HF = new TH2F("h_mapDepth1pedestal_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2pedestal_HF = new TH2F("h_mapDepth2pedestal_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth4pedestal_HO = new TH2F("h_mapDepth4pedestal_HO"," ", 82, -41., 41., 72, 0., 72.);

    h_pedestal00_HB  = new TH1F("h_pedestal00_HB"," ", 100, 0.,10.);
    h_gain_HB  = new TH1F("h_gain_HB"," ", 100, 0.,1.);
    h_respcorr_HB  = new TH1F("h_respcorr_HB"," ", 100, 0.,2.5);
    h_timecorr_HB  = new TH1F("h_timecorr_HB"," ", 100, 0.,30.);
    h_lutcorr_HB  = new TH1F("h_lutcorr_HB"," ", 100, 0.,10.);
    h_difpedestal0_HB  = new TH1F("h_difpedestal0_HB"," ", 100, -3.,3.);
    h_difpedestal1_HB  = new TH1F("h_difpedestal1_HB"," ", 100, -3.,3.);
    h_difpedestal2_HB  = new TH1F("h_difpedestal2_HB"," ", 100, -3.,3.);
    h_difpedestal3_HB  = new TH1F("h_difpedestal3_HB"," ", 100, -3.,3.);

    h_pedestal00_HE  = new TH1F("h_pedestal00_HE"," ", 100, 0.,10.);
    h_gain_HE  = new TH1F("h_gain_HE"," ", 100, 0.,1.);
    h_respcorr_HE  = new TH1F("h_respcorr_HE"," ", 100, 0.,2.5);
    h_timecorr_HE  = new TH1F("h_timecorr_HE"," ", 100, 0.,30.);
    h_lutcorr_HE  = new TH1F("h_lutcorr_HE"," ", 100, 0.,10.);

    h_pedestal00_HF  = new TH1F("h_pedestal00_HF"," ", 100, 0.,10.);
    h_gain_HF  = new TH1F("h_gain_HF"," ", 100, 0.,1.);
    h_respcorr_HF  = new TH1F("h_respcorr_HF"," ", 100, 0.,2.5);
    h_timecorr_HF  = new TH1F("h_timecorr_HF"," ", 100, 0.,30.);
    h_lutcorr_HF  = new TH1F("h_lutcorr_HF"," ", 100, 0.,10.);

    h_pedestal00_HO  = new TH1F("h_pedestal00_HO"," ", 100, 0.,10.);
    h_gain_HO  = new TH1F("h_gain_HO"," ", 100, 0.,1.);
    h_respcorr_HO  = new TH1F("h_respcorr_HO"," ", 100, 0.,2.5);
    h_timecorr_HO  = new TH1F("h_timecorr_HO"," ", 100, 0.,30.);
    h_lutcorr_HO  = new TH1F("h_lutcorr_HO"," ", 100, 0.,10.);

    float est6 = 2500.;
    int ist6 = 30;
    int ist2 = 60;
    h2_pedvsampl_HB  = new TH2F("h2_pedvsampl_HB"," ",   ist2, 0., 7.0, ist2,0.,est6);
    h2_pedwvsampl_HB  = new TH2F("h2_pedwvsampl_HB"," ", ist2, 0., 2.5, ist2,0.,est6);
    h_pedvsampl_HB  = new TH1F("h_pedvsampl_HB"," ",     ist6, 0., 7.0);
    h_pedwvsampl_HB  = new TH1F("h_pedwvsampl_HB"," ",   ist6, 0., 2.5);
    h_pedvsampl0_HB  = new TH1F("h_pedvsampl0_HB"," ",   ist6, 0., 7.);
    h_pedwvsampl0_HB  = new TH1F("h_pedwvsampl0_HB"," ", ist6, 0., 2.5);
    h2_amplvsped_HB  = new TH2F("h2_amplvsped_HB"," ",   ist2, 0.,est6,ist2, 0., 7.0);
    h2_amplvspedw_HB  = new TH2F("h2_amplvspedw_HB"," ", ist2, 0.,est6,ist2, 0., 2.5);
    h_amplvsped_HB  = new TH1F("h_amplvsped_HB"," ",     ist6, 0.,est6);
    h_amplvspedw_HB  = new TH1F("h_amplvspedw_HB"," ",   ist6, 0.,est6);
    h_amplvsped0_HB  = new TH1F("h_amplvsped0_HB"," ",   ist6, 0.,est6);

    h2_pedvsampl_HE  = new TH2F("h2_pedvsampl_HE"," ",   ist2, 0., 7.0, ist2,0.,est6);
    h2_pedwvsampl_HE  = new TH2F("h2_pedwvsampl_HE"," ", ist2, 0., 2.5, ist2,0.,est6);
    h_pedvsampl_HE  = new TH1F("h_pedvsampl_HE"," ",     ist6, 0., 7.0);
    h_pedwvsampl_HE  = new TH1F("h_pedwvsampl_HE"," ",   ist6, 0., 2.5);
    h_pedvsampl0_HE  = new TH1F("h_pedvsampl0_HE"," ",   ist6, 0., 7.);
    h_pedwvsampl0_HE  = new TH1F("h_pedwvsampl0_HE"," ", ist6, 0., 2.5);

    h2_pedvsampl_HF  = new TH2F("h2_pedvsampl_HF"," ",   ist2, 0.,20.0, ist2,0.,est6);
    h2_pedwvsampl_HF  = new TH2F("h2_pedwvsampl_HF"," ", ist2, 0., 2.0, ist2,0.,est6);
    h_pedvsampl_HF  = new TH1F("h_pedvsampl_HF"," ",     ist6, 0.,20.0);
    h_pedwvsampl_HF  = new TH1F("h_pedwvsampl_HF"," ",   ist6, 0., 2.0);
    h_pedvsampl0_HF  = new TH1F("h_pedvsampl0_HF"," ",   ist6, 0.,20.);
    h_pedwvsampl0_HF  = new TH1F("h_pedwvsampl0_HF"," ", ist6, 0., 2.0);

    h2_pedvsampl_HO  = new TH2F("h2_pedvsampl_HO"," ",   ist2, 0.,20.0, ist2,0.,est6);
    h2_pedwvsampl_HO  = new TH2F("h2_pedwvsampl_HO"," ", ist2, 0., 2.5, ist2,0.,est6);
    h_pedvsampl_HO  = new TH1F("h_pedvsampl_HO"," ",     ist6, 0.,20.0);
    h_pedwvsampl_HO  = new TH1F("h_pedwvsampl_HO"," ",   ist6, 0., 2.5);
    h_pedvsampl0_HO  = new TH1F("h_pedvsampl0_HO"," ",   ist6, 0.,20.);
    h_pedwvsampl0_HO  = new TH1F("h_pedwvsampl0_HO"," ", ist6, 0., 2.5);

    h_mapDepth1Ped0_HB = new TH2F("h_mapDepth1Ped0_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1Ped1_HB = new TH2F("h_mapDepth1Ped1_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1Ped2_HB = new TH2F("h_mapDepth1Ped2_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1Ped3_HB = new TH2F("h_mapDepth1Ped3_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1Pedw0_HB = new TH2F("h_mapDepth1Pedw0_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1Pedw1_HB = new TH2F("h_mapDepth1Pedw1_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1Pedw2_HB = new TH2F("h_mapDepth1Pedw2_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1Pedw3_HB = new TH2F("h_mapDepth1Pedw3_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Ped0_HB = new TH2F("h_mapDepth2Ped0_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Ped1_HB = new TH2F("h_mapDepth2Ped1_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Ped2_HB = new TH2F("h_mapDepth2Ped2_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Ped3_HB = new TH2F("h_mapDepth2Ped3_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Pedw0_HB = new TH2F("h_mapDepth2Pedw0_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Pedw1_HB = new TH2F("h_mapDepth2Pedw1_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Pedw2_HB = new TH2F("h_mapDepth2Pedw2_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Pedw3_HB = new TH2F("h_mapDepth2Pedw3_HB"," ", 82, -41., 41., 72, 0., 72.);

    h_mapDepth1Ped0_HE = new TH2F("h_mapDepth1Ped0_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1Ped1_HE = new TH2F("h_mapDepth1Ped1_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1Ped2_HE = new TH2F("h_mapDepth1Ped2_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1Ped3_HE = new TH2F("h_mapDepth1Ped3_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1Pedw0_HE = new TH2F("h_mapDepth1Pedw0_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1Pedw1_HE = new TH2F("h_mapDepth1Pedw1_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1Pedw2_HE = new TH2F("h_mapDepth1Pedw2_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1Pedw3_HE = new TH2F("h_mapDepth1Pedw3_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Ped0_HE = new TH2F("h_mapDepth2Ped0_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Ped1_HE = new TH2F("h_mapDepth2Ped1_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Ped2_HE = new TH2F("h_mapDepth2Ped2_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Ped3_HE = new TH2F("h_mapDepth2Ped3_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Pedw0_HE = new TH2F("h_mapDepth2Pedw0_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Pedw1_HE = new TH2F("h_mapDepth2Pedw1_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Pedw2_HE = new TH2F("h_mapDepth2Pedw2_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Pedw3_HE = new TH2F("h_mapDepth2Pedw3_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth3Ped0_HE = new TH2F("h_mapDepth3Ped0_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth3Ped1_HE = new TH2F("h_mapDepth3Ped1_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth3Ped2_HE = new TH2F("h_mapDepth3Ped2_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth3Ped3_HE = new TH2F("h_mapDepth3Ped3_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth3Pedw0_HE = new TH2F("h_mapDepth3Pedw0_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth3Pedw1_HE = new TH2F("h_mapDepth3Pedw1_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth3Pedw2_HE = new TH2F("h_mapDepth3Pedw2_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth3Pedw3_HE = new TH2F("h_mapDepth3Pedw3_HE"," ", 82, -41., 41., 72, 0., 72.);

    h_mapDepth1Ped0_HF = new TH2F("h_mapDepth1Ped0_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1Ped1_HF = new TH2F("h_mapDepth1Ped1_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1Ped2_HF = new TH2F("h_mapDepth1Ped2_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1Ped3_HF = new TH2F("h_mapDepth1Ped3_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1Pedw0_HF = new TH2F("h_mapDepth1Pedw0_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1Pedw1_HF = new TH2F("h_mapDepth1Pedw1_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1Pedw2_HF = new TH2F("h_mapDepth1Pedw2_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1Pedw3_HF = new TH2F("h_mapDepth1Pedw3_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Ped0_HF = new TH2F("h_mapDepth2Ped0_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Ped1_HF = new TH2F("h_mapDepth2Ped1_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Ped2_HF = new TH2F("h_mapDepth2Ped2_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Ped3_HF = new TH2F("h_mapDepth2Ped3_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Pedw0_HF = new TH2F("h_mapDepth2Pedw0_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Pedw1_HF = new TH2F("h_mapDepth2Pedw1_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Pedw2_HF = new TH2F("h_mapDepth2Pedw2_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2Pedw3_HF = new TH2F("h_mapDepth2Pedw3_HF"," ", 82, -41., 41., 72, 0., 72.);

    h_mapDepth4Ped0_HO = new TH2F("h_mapDepth4Ped0_HO"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth4Ped1_HO = new TH2F("h_mapDepth4Ped1_HO"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth4Ped2_HO = new TH2F("h_mapDepth4Ped2_HO"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth4Ped3_HO = new TH2F("h_mapDepth4Ped3_HO"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth4Pedw0_HO = new TH2F("h_mapDepth4Pedw0_HO"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth4Pedw1_HO = new TH2F("h_mapDepth4Pedw1_HO"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth4Pedw2_HO = new TH2F("h_mapDepth4Pedw2_HO"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth4Pedw3_HO = new TH2F("h_mapDepth4Pedw3_HO"," ", 82, -41., 41., 72, 0., 72.);

    h_mapDepth1ADCAmpl12_HB = new TH2F("h_mapDepth1ADCAmpl12_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2ADCAmpl12_HB = new TH2F("h_mapDepth2ADCAmpl12_HB"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1ADCAmpl12_HE = new TH2F("h_mapDepth1ADCAmpl12_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2ADCAmpl12_HE = new TH2F("h_mapDepth2ADCAmpl12_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth3ADCAmpl12_HE = new TH2F("h_mapDepth3ADCAmpl12_HE"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth1ADCAmpl12_HF = new TH2F("h_mapDepth1ADCAmpl12_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth2ADCAmpl12_HF = new TH2F("h_mapDepth2ADCAmpl12_HF"," ", 82, -41., 41., 72, 0., 72.);
    h_mapDepth4ADCAmpl12_HO = new TH2F("h_mapDepth4ADCAmpl12_HO"," ", 82, -41., 41., 72, 0., 72.);

    h_mapGetRMSOverNormalizedSignal_HB=new TH2F("h_mapGetRMSOverNormalizedSignal_HB"," ",82,-41.,41.,72,0.,72.);
    h_mapGetRMSOverNormalizedSignal0_HB=new TH2F("h_mapGetRMSOverNormalizedSignal0_HB"," ",82,-41.,41.,72,0.,72.);
    h_mapGetRMSOverNormalizedSignal_HE=new TH2F("h_mapGetRMSOverNormalizedSignal_HE"," ",82,-41.,41.,72,0.,72.);
    h_mapGetRMSOverNormalizedSignal0_HE=new TH2F("h_mapGetRMSOverNormalizedSignal0_HE"," ",82,-41.,41.,72,0.,72.);
    h_mapGetRMSOverNormalizedSignal_HF=new TH2F("h_mapGetRMSOverNormalizedSignal_HF"," ",82,-41.,41.,72,0.,72.);
    h_mapGetRMSOverNormalizedSignal0_HF=new TH2F("h_mapGetRMSOverNormalizedSignal0_HF"," ",82,-41.,41.,72,0.,72.);
    h_mapGetRMSOverNormalizedSignal_HO=new TH2F("h_mapGetRMSOverNormalizedSignal_HO"," ",82,-41.,41.,72,0.,72.);
    h_mapGetRMSOverNormalizedSignal0_HO=new TH2F("h_mapGetRMSOverNormalizedSignal0_HO"," ",82,-41.,41.,72,0.,72.);

    h_shape_Ahigh_HB0 = new TH1F("h_shape_Ahigh_HB0"," ", 10, 0., 10.);
    h_shape0_Ahigh_HB0 = new TH1F("h_shape0_Ahigh_HB0"," ", 10, 0., 10.);
    h_shape_Alow_HB0 = new TH1F("h_shape_Alow_HB0"," ", 10, 0., 10.);
    h_shape0_Alow_HB0 = new TH1F("h_shape0_Alow_HB0"," ", 10, 0., 10.);
    h_shape_Ahigh_HB1 = new TH1F("h_shape_Ahigh_HB1"," ", 10, 0., 10.);
    h_shape0_Ahigh_HB1 = new TH1F("h_shape0_Ahigh_HB1"," ", 10, 0., 10.);
    h_shape_Alow_HB1 = new TH1F("h_shape_Alow_HB1"," ", 10, 0., 10.);
    h_shape0_Alow_HB1 = new TH1F("h_shape0_Alow_HB1"," ", 10, 0., 10.);
    h_shape_Ahigh_HB2 = new TH1F("h_shape_Ahigh_HB2"," ", 10, 0., 10.);
    h_shape0_Ahigh_HB2 = new TH1F("h_shape0_Ahigh_HB2"," ", 10, 0., 10.);
    h_shape_Alow_HB2 = new TH1F("h_shape_Alow_HB2"," ", 10, 0., 10.);
    h_shape0_Alow_HB2 = new TH1F("h_shape0_Alow_HB2"," ", 10, 0., 10.);
    h_shape_Ahigh_HB3 = new TH1F("h_shape_Ahigh_HB3"," ", 10, 0., 10.);
    h_shape0_Ahigh_HB3 = new TH1F("h_shape0_Ahigh_HB3"," ", 10, 0., 10.);
    h_shape_Alow_HB3 = new TH1F("h_shape_Alow_HB3"," ", 10, 0., 10.);
    h_shape0_Alow_HB3 = new TH1F("h_shape0_Alow_HB3"," ", 10, 0., 10.);

    h_shape_bad_channels_HB = new TH1F("h_shape_bad_channels_HB"," ", 10, 0., 10.);
    h_shape0_bad_channels_HB = new TH1F("h_shape0_bad_channels_HB"," ", 10, 0., 10.);
    h_shape_good_channels_HB = new TH1F("h_shape_good_channels_HB"," ", 10, 0., 10.);
    h_shape0_good_channels_HB = new TH1F("h_shape0_good_channels_HB"," ", 10, 0., 10.);
    h_shape_bad_channels_HE = new TH1F("h_shape_bad_channels_HE"," ", 10, 0., 10.);
    h_shape0_bad_channels_HE = new TH1F("h_shape0_bad_channels_HE"," ", 10, 0., 10.);
    h_shape_good_channels_HE = new TH1F("h_shape_good_channels_HE"," ", 10, 0., 10.);
    h_shape0_good_channels_HE = new TH1F("h_shape0_good_channels_HE"," ", 10, 0., 10.);
    h_shape_bad_channels_HF = new TH1F("h_shape_bad_channels_HF"," ", 10, 0., 10.);
    h_shape0_bad_channels_HF = new TH1F("h_shape0_bad_channels_HF"," ", 10, 0., 10.);
    h_shape_good_channels_HF = new TH1F("h_shape_good_channels_HF"," ", 10, 0., 10.);
    h_shape0_good_channels_HF = new TH1F("h_shape0_good_channels_HF"," ", 10, 0., 10.);
    h_shape_bad_channels_HO = new TH1F("h_shape_bad_channels_HO"," ", 10, 0., 10.);
    h_shape0_bad_channels_HO = new TH1F("h_shape0_bad_channels_HO"," ", 10, 0., 10.);
    h_shape_good_channels_HO = new TH1F("h_shape_good_channels_HO"," ", 10, 0., 10.);
    h_shape0_good_channels_HO = new TH1F("h_shape0_good_channels_HO"," ", 10, 0., 10.);

    //    if(flagcpuoptimization_== 0 ) {

    
				   int spl= 1000;
				   float spls=5000;
				   h_sumamplitude_depth1_HB = new TH1F("h_sumamplitude_depth1_HB"," ", spl, 0.,spls);
				   h_sumamplitude_depth2_HB = new TH1F("h_sumamplitude_depth2_HB"," ", spl, 0.,spls);
				   h_sumamplitude_depth1_HE = new TH1F("h_sumamplitude_depth1_HE"," ", spl, 0.,spls);
				   h_sumamplitude_depth2_HE = new TH1F("h_sumamplitude_depth2_HE"," ", spl, 0.,spls);
				   h_sumamplitude_depth3_HE = new TH1F("h_sumamplitude_depth3_HE"," ", spl, 0.,spls);
				   h_sumamplitude_depth1_HF = new TH1F("h_sumamplitude_depth1_HF"," ", spl, 0.,spls);
				   h_sumamplitude_depth2_HF = new TH1F("h_sumamplitude_depth2_HF"," ", spl, 0.,spls);
				   h_sumamplitude_depth4_HO = new TH1F("h_sumamplitude_depth4_HO"," ", spl, 0.,spls);
				   int spl0= 1000;
				   float spls0=10000;
				   h_sumamplitude_depth1_HB0 = new TH1F("h_sumamplitude_depth1_HB0"," ", spl0, 0.,spls0);
				   h_sumamplitude_depth2_HB0 = new TH1F("h_sumamplitude_depth2_HB0"," ", spl0, 0.,spls0);
				   h_sumamplitude_depth1_HE0 = new TH1F("h_sumamplitude_depth1_HE0"," ", spl0, 0.,spls0);
				   h_sumamplitude_depth2_HE0 = new TH1F("h_sumamplitude_depth2_HE0"," ", spl0, 0.,spls0);
				   h_sumamplitude_depth3_HE0 = new TH1F("h_sumamplitude_depth3_HE0"," ", spl0, 0.,spls0);
				   h_sumamplitude_depth1_HF0 = new TH1F("h_sumamplitude_depth1_HF0"," ", spl0, 0.,spls0);
				   h_sumamplitude_depth2_HF0 = new TH1F("h_sumamplitude_depth2_HF0"," ", spl0, 0.,spls0);
				   h_sumamplitude_depth4_HO0 = new TH1F("h_sumamplitude_depth4_HO0"," ", spl0, 0.,spls0);
				   int spl1= 1000;
				   float spls1=100000;
				   h_sumamplitude_depth1_HB1 = new TH1F("h_sumamplitude_depth1_HB1"," ", spl1, 0.,spls1);
				   h_sumamplitude_depth2_HB1 = new TH1F("h_sumamplitude_depth2_HB1"," ", spl1, 0.,spls1);
				   h_sumamplitude_depth1_HE1 = new TH1F("h_sumamplitude_depth1_HE1"," ", spl1, 0.,spls1);
				   h_sumamplitude_depth2_HE1 = new TH1F("h_sumamplitude_depth2_HE1"," ", spl1, 0.,spls1);
				   h_sumamplitude_depth3_HE1 = new TH1F("h_sumamplitude_depth3_HE1"," ", spl1, 0.,spls1);
				   h_sumamplitude_depth1_HF1 = new TH1F("h_sumamplitude_depth1_HF1"," ", spl1, 0.,spls1);
				   h_sumamplitude_depth2_HF1 = new TH1F("h_sumamplitude_depth2_HF1"," ", spl1, 0.,spls1);
				   h_sumamplitude_depth4_HO1 = new TH1F("h_sumamplitude_depth4_HO1"," ", spl1, 0.,spls1);

				   int eall= 1000;
				   float eall1=splashesUpperLimit_;
				   
				   h_ADC_HB = new TH1F("h_ADC_HB"," ", eall, 0.,eall1);  
				   h_ADC_HE = new TH1F("h_ADC_HE"," ", eall, 0.,eall1);  
				   h_ADC_HF = new TH1F("h_ADC_HF"," ", eall, 0.,eall1);  
				   h_ADC_HO = new TH1F("h_ADC_HO"," ", eall, 0.,eall1);  
				   h_ADC_HBdepth1 = new TH1F("h_ADC_HBdepth1"," ", eall, 0.,eall1);  
				   h_ADC_HBdepth2 = new TH1F("h_ADC_HBdepth2"," ", eall, 0.,eall1);  
				   h_ADC_HEdepth1 = new TH1F("h_ADC_HEdepth1"," ", eall, 0.,eall1);  
				   h_ADC_HEdepth2 = new TH1F("h_ADC_HEdepth2"," ", eall, 0.,eall1);  
				   h_ADC_HEdepth3 = new TH1F("h_ADC_HEdepth3"," ", eall, 0.,eall1);  
				   h_ADC_HFdepth1 = new TH1F("h_ADC_HFdepth1"," ", eall, 0.,eall1);  
				   h_ADC_HFdepth2 = new TH1F("h_ADC_HFdepth2"," ", eall, 0.,eall1);  
				   h_ADC_HOdepth4 = new TH1F("h_ADC_HOdepth4"," ", eall, 0.,eall1);  
				   
				   int each= 100;
				   float each1=splashesUpperLimit_;
				   h_ADC_HBdepth1_TS0 = new TH1F("h_ADC_HBdepth1_TS0"," ", each, 0.,each1);  
				   h_ADC_HBdepth1_TS1 = new TH1F("h_ADC_HBdepth1_TS1"," ", each, 0.,each1);  
				   h_ADC_HBdepth1_TS2 = new TH1F("h_ADC_HBdepth1_TS2"," ", each, 0.,each1);  
				   h_ADC_HBdepth1_TS3 = new TH1F("h_ADC_HBdepth1_TS3"," ", each, 0.,each1);  
				   h_ADC_HBdepth1_TS4 = new TH1F("h_ADC_HBdepth1_TS4"," ", each, 0.,each1);  
				   h_ADC_HBdepth1_TS5 = new TH1F("h_ADC_HBdepth1_TS5"," ", each, 0.,each1);  
				   h_ADC_HBdepth1_TS6 = new TH1F("h_ADC_HBdepth1_TS6"," ", each, 0.,each1);  
				   h_ADC_HBdepth1_TS7 = new TH1F("h_ADC_HBdepth1_TS7"," ", each, 0.,each1);  
				   h_ADC_HBdepth1_TS8 = new TH1F("h_ADC_HBdepth1_TS8"," ", each, 0.,each1); 
				   h_ADC_HBdepth1_TS9 = new TH1F("h_ADC_HBdepth1_TS9"," ", each, 0.,each1); 
				   h_ADC_HBdepth2_TS0 = new TH1F("h_ADC_HBdepth2_TS0"," ", each, 0.,each1);  
				   h_ADC_HBdepth2_TS1 = new TH1F("h_ADC_HBdepth2_TS1"," ", each, 0.,each1);  
				   h_ADC_HBdepth2_TS2 = new TH1F("h_ADC_HBdepth2_TS2"," ", each, 0.,each1);  
				   h_ADC_HBdepth2_TS3 = new TH1F("h_ADC_HBdepth2_TS3"," ", each, 0.,each1);  
				   h_ADC_HBdepth2_TS4 = new TH1F("h_ADC_HBdepth2_TS4"," ", each, 0.,each1);  
				   h_ADC_HBdepth2_TS5 = new TH1F("h_ADC_HBdepth2_TS5"," ", each, 0.,each1);  
				   h_ADC_HBdepth2_TS6 = new TH1F("h_ADC_HBdepth2_TS6"," ", each, 0.,each1);  
				   h_ADC_HBdepth2_TS7 = new TH1F("h_ADC_HBdepth2_TS7"," ", each, 0.,each1);  
				   h_ADC_HBdepth2_TS8 = new TH1F("h_ADC_HBdepth2_TS8"," ", each, 0.,each1); 
				   h_ADC_HBdepth2_TS9 = new TH1F("h_ADC_HBdepth2_TS9"," ", each, 0.,each1); 
				   h_ADC_HEdepth1_TS0 = new TH1F("h_ADC_HEdepth1_TS0"," ", each, 0.,each1);  
				   h_ADC_HEdepth1_TS1 = new TH1F("h_ADC_HEdepth1_TS1"," ", each, 0.,each1);  
				   h_ADC_HEdepth1_TS2 = new TH1F("h_ADC_HEdepth1_TS2"," ", each, 0.,each1);  
				   h_ADC_HEdepth1_TS3 = new TH1F("h_ADC_HEdepth1_TS3"," ", each, 0.,each1);  
				   h_ADC_HEdepth1_TS4 = new TH1F("h_ADC_HEdepth1_TS4"," ", each, 0.,each1);  
				   h_ADC_HEdepth1_TS5 = new TH1F("h_ADC_HEdepth1_TS5"," ", each, 0.,each1);  
				   h_ADC_HEdepth1_TS6 = new TH1F("h_ADC_HEdepth1_TS6"," ", each, 0.,each1);  
				   h_ADC_HEdepth1_TS7 = new TH1F("h_ADC_HEdepth1_TS7"," ", each, 0.,each1);  
				   h_ADC_HEdepth1_TS8 = new TH1F("h_ADC_HEdepth1_TS8"," ", each, 0.,each1); 
				   h_ADC_HEdepth1_TS9 = new TH1F("h_ADC_HEdepth1_TS9"," ", each, 0.,each1); 
				   h_ADC_HEdepth2_TS0 = new TH1F("h_ADC_HEdepth2_TS0"," ", each, 0.,each1);  
				   h_ADC_HEdepth2_TS1 = new TH1F("h_ADC_HEdepth2_TS1"," ", each, 0.,each1);  
				   h_ADC_HEdepth2_TS2 = new TH1F("h_ADC_HEdepth2_TS2"," ", each, 0.,each1);  
				   h_ADC_HEdepth2_TS3 = new TH1F("h_ADC_HEdepth2_TS3"," ", each, 0.,each1);  
				   h_ADC_HEdepth2_TS4 = new TH1F("h_ADC_HEdepth2_TS4"," ", each, 0.,each1);  
				   h_ADC_HEdepth2_TS5 = new TH1F("h_ADC_HEdepth2_TS5"," ", each, 0.,each1);  
				   h_ADC_HEdepth2_TS6 = new TH1F("h_ADC_HEdepth2_TS6"," ", each, 0.,each1);  
				   h_ADC_HEdepth2_TS7 = new TH1F("h_ADC_HEdepth2_TS7"," ", each, 0.,each1);  
				   h_ADC_HEdepth2_TS8 = new TH1F("h_ADC_HEdepth2_TS8"," ", each, 0.,each1); 
				   h_ADC_HEdepth2_TS9 = new TH1F("h_ADC_HEdepth2_TS9"," ", each, 0.,each1); 
				   h_ADC_HEdepth3_TS0 = new TH1F("h_ADC_HEdepth3_TS0"," ", each, 0.,each1);  
				   h_ADC_HEdepth3_TS1 = new TH1F("h_ADC_HEdepth3_TS1"," ", each, 0.,each1);  
				   h_ADC_HEdepth3_TS2 = new TH1F("h_ADC_HEdepth3_TS2"," ", each, 0.,each1);  
				   h_ADC_HEdepth3_TS3 = new TH1F("h_ADC_HEdepth3_TS3"," ", each, 0.,each1);  
				   h_ADC_HEdepth3_TS4 = new TH1F("h_ADC_HEdepth3_TS4"," ", each, 0.,each1);  
				   h_ADC_HEdepth3_TS5 = new TH1F("h_ADC_HEdepth3_TS5"," ", each, 0.,each1);  
				   h_ADC_HEdepth3_TS6 = new TH1F("h_ADC_HEdepth3_TS6"," ", each, 0.,each1);  
				   h_ADC_HEdepth3_TS7 = new TH1F("h_ADC_HEdepth3_TS7"," ", each, 0.,each1);  
				   h_ADC_HEdepth3_TS8 = new TH1F("h_ADC_HEdepth3_TS8"," ", each, 0.,each1); 
				   h_ADC_HEdepth3_TS9 = new TH1F("h_ADC_HEdepth3_TS9"," ", each, 0.,each1); 
				   h_ADC_HFdepth1_TS0 = new TH1F("h_ADC_HFdepth1_TS0"," ", each, 0.,each1);  
				   h_ADC_HFdepth1_TS1 = new TH1F("h_ADC_HFdepth1_TS1"," ", each, 0.,each1);  
				   h_ADC_HFdepth1_TS2 = new TH1F("h_ADC_HFdepth1_TS2"," ", each, 0.,each1);  
				   h_ADC_HFdepth1_TS3 = new TH1F("h_ADC_HFdepth1_TS3"," ", each, 0.,each1);  
				   h_ADC_HFdepth1_TS4 = new TH1F("h_ADC_HFdepth1_TS4"," ", each, 0.,each1);  
				   h_ADC_HFdepth1_TS5 = new TH1F("h_ADC_HFdepth1_TS5"," ", each, 0.,each1);  
				   h_ADC_HFdepth1_TS6 = new TH1F("h_ADC_HFdepth1_TS6"," ", each, 0.,each1);  
				   h_ADC_HFdepth1_TS7 = new TH1F("h_ADC_HFdepth1_TS7"," ", each, 0.,each1);  
				   h_ADC_HFdepth1_TS8 = new TH1F("h_ADC_HFdepth1_TS8"," ", each, 0.,each1); 
				   h_ADC_HFdepth1_TS9 = new TH1F("h_ADC_HFdepth1_TS9"," ", each, 0.,each1); 
				   h_ADC_HFdepth2_TS0 = new TH1F("h_ADC_HFdepth2_TS0"," ", each, 0.,each1);  
				   h_ADC_HFdepth2_TS1 = new TH1F("h_ADC_HFdepth2_TS1"," ", each, 0.,each1);  
				   h_ADC_HFdepth2_TS2 = new TH1F("h_ADC_HFdepth2_TS2"," ", each, 0.,each1);  
				   h_ADC_HFdepth2_TS3 = new TH1F("h_ADC_HFdepth2_TS3"," ", each, 0.,each1);  
				   h_ADC_HFdepth2_TS4 = new TH1F("h_ADC_HFdepth2_TS4"," ", each, 0.,each1);  
				   h_ADC_HFdepth2_TS5 = new TH1F("h_ADC_HFdepth2_TS5"," ", each, 0.,each1);  
				   h_ADC_HFdepth2_TS6 = new TH1F("h_ADC_HFdepth2_TS6"," ", each, 0.,each1);  
				   h_ADC_HFdepth2_TS7 = new TH1F("h_ADC_HFdepth2_TS7"," ", each, 0.,each1);  
				   h_ADC_HFdepth2_TS8 = new TH1F("h_ADC_HFdepth2_TS8"," ", each, 0.,each1); 
				   h_ADC_HFdepth2_TS9 = new TH1F("h_ADC_HFdepth2_TS9"," ", each, 0.,each1); 
				   h_ADC_HOdepth4_TS0 = new TH1F("h_ADC_HOdepth4_TS0"," ", each, 0.,each1);  
				   h_ADC_HOdepth4_TS1 = new TH1F("h_ADC_HOdepth4_TS1"," ", each, 0.,each1);  
				   h_ADC_HOdepth4_TS2 = new TH1F("h_ADC_HOdepth4_TS2"," ", each, 0.,each1);  
				   h_ADC_HOdepth4_TS3 = new TH1F("h_ADC_HOdepth4_TS3"," ", each, 0.,each1);  
				   h_ADC_HOdepth4_TS4 = new TH1F("h_ADC_HOdepth4_TS4"," ", each, 0.,each1);  
				   h_ADC_HOdepth4_TS5 = new TH1F("h_ADC_HOdepth4_TS5"," ", each, 0.,each1);  
				   h_ADC_HOdepth4_TS6 = new TH1F("h_ADC_HOdepth4_TS6"," ", each, 0.,each1);  
				   h_ADC_HOdepth4_TS7 = new TH1F("h_ADC_HOdepth4_TS7"," ", each, 0.,each1);  
				   h_ADC_HOdepth4_TS8 = new TH1F("h_ADC_HOdepth4_TS8"," ", each, 0.,each1); 
				   h_ADC_HOdepth4_TS9 = new TH1F("h_ADC_HOdepth4_TS9"," ", each, 0.,each1); 

				   //				   }
    
    h_Amplitude_forCapIdErrors_HB1 = new TH1F("h_Amplitude_forCapIdErrors_HB1"," ", 100, 0.,30000.);
    h_Amplitude_forCapIdErrors_HB2 = new TH1F("h_Amplitude_forCapIdErrors_HB2"," ", 100, 0.,30000.);
    h_Amplitude_forCapIdErrors_HE1 = new TH1F("h_Amplitude_forCapIdErrors_HE1"," ", 100, 0.,30000.);
    h_Amplitude_forCapIdErrors_HE2 = new TH1F("h_Amplitude_forCapIdErrors_HE2"," ", 100, 0.,30000.);
    h_Amplitude_forCapIdErrors_HE3 = new TH1F("h_Amplitude_forCapIdErrors_HE3"," ", 100, 0.,30000.);
    h_Amplitude_forCapIdErrors_HF1 = new TH1F("h_Amplitude_forCapIdErrors_HF1"," ", 100, 0.,30000.);
    h_Amplitude_forCapIdErrors_HF2 = new TH1F("h_Amplitude_forCapIdErrors_HF2"," ", 100, 0.,30000.);
    h_Amplitude_forCapIdErrors_HO4 = new TH1F("h_Amplitude_forCapIdErrors_HO4"," ", 100, 0.,30000.);

    h_Amplitude_notCapIdErrors_HB1 = new TH1F("h_Amplitude_notCapIdErrors_HB1"," ", 100, 0.,30000.);
    h_Amplitude_notCapIdErrors_HB2 = new TH1F("h_Amplitude_notCapIdErrors_HB2"," ", 100, 0.,30000.);
    h_Amplitude_notCapIdErrors_HE1 = new TH1F("h_Amplitude_notCapIdErrors_HE1"," ", 100, 0.,30000.);
    h_Amplitude_notCapIdErrors_HE2 = new TH1F("h_Amplitude_notCapIdErrors_HE2"," ", 100, 0.,30000.);
    h_Amplitude_notCapIdErrors_HE3 = new TH1F("h_Amplitude_notCapIdErrors_HE3"," ", 100, 0.,30000.);
    h_Amplitude_notCapIdErrors_HF1 = new TH1F("h_Amplitude_notCapIdErrors_HF1"," ", 100, 0.,30000.);
    h_Amplitude_notCapIdErrors_HF2 = new TH1F("h_Amplitude_notCapIdErrors_HF2"," ", 100, 0.,30000.);
    h_Amplitude_notCapIdErrors_HO4 = new TH1F("h_Amplitude_notCapIdErrors_HO4"," ", 100, 0.,30000.);


    h_2DAtaildepth1_HB  = new TH2F("h_2DAtaildepth1_HB"," ",    82, -41., 41., 72, 0., 72.);
    h_2D0Ataildepth1_HB  = new TH2F("h_2D0Ataildepth1_HB"," ",    82, -41., 41., 72, 0., 72.);
    h_2DAtaildepth2_HB  = new TH2F("h_2DAtaildepth2_HB"," ",    82, -41., 41., 72, 0., 72.);
    h_2D0Ataildepth2_HB  = new TH2F("h_2D0Ataildepth2_HB"," ",    82, -41., 41., 72, 0., 72.);





    ////////////////////////////////////////////////////////////////////////////////////
  }//if(recordHistoes_
  
    ///////////////////////////////////////////////////////            ntuples:
  if(recordNtuples_) {
    
    myTree = new TTree("Hcal","Hcal Tree");
    myTree->Branch("Nevent",  &Nevent, "Nevent/I");
    myTree->Branch("Run",  &Run, "Run/I");
    
    // Calibration box
    
    myTree->Branch("myCalEta", myCalEta, "myCalEta[5][40]/I");
    myTree->Branch("myCalPhi", myCalPhi, "myCalPhi[5][40]/I");
    
    myTree->Branch("hb_calibration0", hb_calibration0, "hb_calibration0[40][15]/F");
    myTree->Branch("hb_calibration1", hb_calibration1, "hb_calibration1[40][15]/F");
    
    myTree->Branch("he_calibration0", he_calibration0, "he_calibration0[40][15]/F");
    myTree->Branch("he_calibration1", he_calibration1, "he_calibration1[40][15]/F");
    
    myTree->Branch("hf_calibration0", hf_calibration0, "hf_calibration0[40][15]/F");
    myTree->Branch("hf_calibration1", hf_calibration1, "hf_calibration1[40][15]/F");
    
    myTree->Branch("ho_calibration0", ho_calibration0, "ho_calibration0[40][15]/F");
    myTree->Branch("ho_calibration1", ho_calibration1, "ho_calibration1[40][15]/F");
    
    // HBHE readouts
    
    myTree->Branch("hb15", hb15, "hb15[2][2][72][10]/F");
    myTree->Branch("hb16", hb16, "hb16[2][2][72][10]/F");
    myTree->Branch("he16", he16, "he16[2][1][72][10]/F");
    myTree->Branch("hb", hb, "hb[29][1][72][10]/F");
    myTree->Branch("he", he, "he[26][2][72][10]/F");

  }//if(recordNtuples_

  //////////////////////////////////////////////////////////////////

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------ method called for each event  ------------
void VeRawAnalyzer::fillDigiErrors(HBHEDigiCollection::const_iterator& digiItr)
{

    CaloSamples tool;  // TS
    
    if (verbosity == -22) std::cout << "**************   in loop over Digis   counter =     " << nnnnnn << std::endl;
   
    HcalDetId cell(digiItr->id()); 
    int mdepth = cell.depth();
    int iphi  = cell.iphi()-1;
    int ieta  = cell.ieta();
    if(ieta > 0) ieta -= 1;
    int sub   = cell.subdet(); // 1-HB, 2-HE (HFDigiCollection: 4-HF)
    if (verbosity == -22) std::cout << std::endl << nnnnnn << " DIGI ->  "  
				 << "sub, ieta, iphi, mdepth = "  
				 <<  sub << ", " << ieta << ", " << iphi << ", " << mdepth 
				 << std::endl;

    // !!!!!!
    int errorGeneral =  0;
    int error1 =  0;
    int error2 =  0;
    int error3 =  0;
    int error4 =  0;
    int error5 =  0;
    int error6 =  0;
    int error7 =  0;
    // !!!!!!
    bool anycapid   =  true;
    bool anyer      =  false;
    bool anydv      =  true;
    // for help:
    int firstcapid  = 0;
    int sumcapid    = 0;
    int lastcapid=0, capid=0;
    int ERRORfiber = -10;
    int ERRORfiberChan = -10;
    int ERRORfiberAndChan = -10;
    int repetedcapid = 0;


    if (verbosity == -22) std::cout <<" Size of Digi "<<(*digiItr).size()<<std::endl;

    ///////////////////////////////////////    
    for  (int ii=0;ii<(*digiItr).size();ii++) {
      capid = (*digiItr)[ii].capid(); // capId (0-3, sequential)
      bool er    = (*digiItr)[ii].er();   // error
      bool dv    = (*digiItr)[ii].dv();  // valid data
      int fiber    = (*digiItr)[ii].fiber();  // get the fiber number
      int fiberChan    = (*digiItr)[ii].fiberChan();  // get the fiber channel number
      int fiberAndChan    = (*digiItr)[ii].fiberAndChan();  // get the id channel
      if (verbosity == -22) std::cout <<" fiber =  "  <<  fiber << std::endl;
      if (verbosity == -22) std::cout <<" fiberChan =  "  <<  fiberChan << std::endl;
      if (verbosity == -22) std::cout <<" fiberAndChan =  "  <<  fiberAndChan << std::endl;
      
      if(ii!=0 && ((lastcapid+1)%4)!=capid) { 
	anycapid =  false;    
	if (verbosity == -22) std::cout <<" capid=false  digiItr =    "  <<  *digiItr << std::endl;
	ERRORfiber = fiber;
	ERRORfiberChan = fiberChan;
	ERRORfiberAndChan = fiberAndChan;
	if( capid != lastcapid ){} else{ repetedcapid =  1;}
      }
      lastcapid=capid;  
   
      if(ii == 0) firstcapid = capid;
      sumcapid += capid;
      
      if (verbosity == -22) std::cout <<" capid =    "  <<  capid << std::endl;
      if(er) { 
	anyer =  true;    
	if (verbosity == -22) std::cout <<" er=true   digiItr =   "  <<  *digiItr << std::endl;
	ERRORfiber = fiber;
	ERRORfiberChan = fiberChan;
	ERRORfiberAndChan = fiberAndChan;
      }
      if(!dv) { 
	anydv =  false;    
	if (verbosity == -22) std::cout <<" dv=false  digiItr =    "  <<  *digiItr << std::endl;
	ERRORfiber = fiber;
	ERRORfiberChan = fiberChan;
	ERRORfiberAndChan = fiberAndChan;
      }
      
    }// for

    if (verbosity == -22) std::cout <<" Digi is treated "<<std::endl;

    ///////////////////////////////////////    
    if( firstcapid==0 && !anycapid) errorGeneral = 1; 
    if( firstcapid==1 && !anycapid) errorGeneral = 2; 
    if( firstcapid==2 && !anycapid) errorGeneral = 3; 
    if( firstcapid==3 && !anycapid) errorGeneral = 4; 
    if( !anycapid )                     error1 = 1; 
    if( anyer )                         error2 = 1; 
    if( !anydv )                        error3 = 1; 
    
    if( !anycapid && anyer)                     error4 = 1; 
    if( !anycapid && !anydv)                    error5 = 1; 
    if( !anycapid && anyer && !anydv)           error6 = 1; 
    if( anyer && !anydv)                        error7 = 1; 
    ///////////////////////////////////////Energy
    // Energy:    

    double ampl = 0.;
    for (int ii=0; ii<10; ii++) {  
      double ampldefault = adc2fC[digiItr->sample(ii).adc()];
      ampl+=ampldefault;// fC
    }

    if (verbosity == -22) std::cout << std::endl << "*** E = " << ampl 
				 << "   ACD -> fC -> (gain ="
				 << std::endl;
    
    ///////////////////////////////////////Digis
    // Digis:
    // HB
    if ( sub == 1 ) {
      h_errorGeneral_HB->Fill(double(errorGeneral),1.);    
      h_error1_HB->Fill(double(error1),1.);    
      h_error2_HB->Fill(double(error2),1.);    
      h_error3_HB->Fill(double(error3),1.);    
      h_error4_HB->Fill(double(error4),1.);    
      h_error5_HB->Fill(double(error5),1.);    
      h_error6_HB->Fill(double(error6),1.);    
      h_error7_HB->Fill(double(error7),1.); 
      h_repetedcapid_HB->Fill(double(repetedcapid),1.); 
   
      if(error1 !=0 || error2 !=0 || error3 !=0 ) {
	if(studyRunDependenceHist_ && flagtodefinebadchannel_==0) ++badchannels[sub-1][mdepth-1][ieta+41][iphi];// 0-82 ; 0-71
	h_amplError_HB->Fill(ampl,1.);
	if(mdepth==1) h_mapDepth1Error_HB->Fill(double(ieta), double(iphi));    
	if(mdepth==2) h_mapDepth2Error_HB->Fill(double(ieta), double(iphi));    
	if(mdepth==3) h_mapDepth3Error_HB->Fill(double(ieta), double(iphi));    
	h_fiber0_HB->Fill(double(ERRORfiber), 1. );    
	h_fiber1_HB->Fill(double(ERRORfiberChan), 1. );    
	h_fiber2_HB->Fill(double(ERRORfiberAndChan), 1. );    
      }
      else { h_amplFine_HB->Fill(ampl,1.);}
    }
    // HE
    if ( sub == 2 ) {
      h_errorGeneral_HE->Fill(double(errorGeneral),1.);    
      h_error1_HE->Fill(double(error1),1.);    
      h_error2_HE->Fill(double(error2),1.);    
      h_error3_HE->Fill(double(error3),1.);    
      h_error4_HE->Fill(double(error4),1.);    
      h_error5_HE->Fill(double(error5),1.);    
      h_error6_HE->Fill(double(error6),1.);    
      h_error7_HE->Fill(double(error7),1.);    
      h_repetedcapid_HE->Fill(double(repetedcapid),1.); 

      if(error1 !=0 || error2 !=0 || error3 !=0 ) {
	if(studyRunDependenceHist_ && flagtodefinebadchannel_==0) ++badchannels[sub-1][mdepth-1][ieta+41][iphi];// 0-82 ; 0-71
	h_amplError_HE->Fill(ampl,1.);
	if(mdepth==1) h_mapDepth1Error_HE->Fill(double(ieta), double(iphi));    
	if(mdepth==2) h_mapDepth2Error_HE->Fill(double(ieta), double(iphi));    
	if(mdepth==3) h_mapDepth3Error_HE->Fill(double(ieta), double(iphi));    
	h_fiber0_HE->Fill(double(ERRORfiber), 1. );    
	h_fiber1_HE->Fill(double(ERRORfiberChan), 1. );    
	h_fiber2_HE->Fill(double(ERRORfiberAndChan), 1. );    
      }
      else { h_amplFine_HE->Fill(ampl,1.);}
    }
    //    ha2->Fill(double(ieta), double(iphi));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------ method called for each event  ------------
void VeRawAnalyzer::fillDigiErrorsHF(HFDigiCollection::const_iterator& digiItr)
{
    CaloSamples tool;  // TS
    if (verbosity == -22) std::cout << "**************   loop over HF Digis    " << std::endl;
    HcalDetId cell(digiItr->id()); 
    int mdepth = cell.depth();
    int iphi  = cell.iphi()-1;
    int ieta  = cell.ieta();
    if(ieta > 0) ieta -= 1;
    int sub   = cell.subdet(); // 1-HB, 2-HE (HFDigiCollection: 4-HF)
    if (verbosity == -22) std::cout << " HF DIGI ->  "  
				 << "sub, ieta, iphi, mdepth = "  
				 <<  sub << ", " << ieta << ", " << iphi << ", " << mdepth 
				 << std::endl;
    // !!!!!!
    int errorGeneral =  0;
    int error1 =  0;
    int error2 =  0;
    int error3 =  0;
    int error4 =  0;
    int error5 =  0;
    int error6 =  0;
    int error7 =  0;
    // !!!!!!
    bool anycapid   =  true;
    bool anyer      =  false;
    bool anydv      =  true;
    // for help:
    int firstcapid  = 0;
    int sumcapid    = 0;
    int lastcapid=0, capid=0;
    int ERRORfiber = -10;
    int ERRORfiberChan = -10;
    int ERRORfiberAndChan = -10;
    int repetedcapid = 0;
    if (verbosity == -22) std::cout <<" Size of HF Digi "<<(*digiItr).size()<<std::endl;
    ///////////////////////////////////////    
    for  (int ii=0;ii<(*digiItr).size();ii++) {
      capid = (*digiItr)[ii].capid(); // capId (0-3, sequential)
      bool er    = (*digiItr)[ii].er();   // error
      bool dv    = (*digiItr)[ii].dv();  // valid data
      int fiber    = (*digiItr)[ii].fiber();  // get the fiber number
      int fiberChan    = (*digiItr)[ii].fiberChan();  // get the fiber channel number
      int fiberAndChan    = (*digiItr)[ii].fiberAndChan();  // get the id channel
      if (verbosity == -22) std::cout <<" HF fiber =  "  <<  fiber << std::endl;
      if (verbosity == -22) std::cout <<" HF fiberChan =  "  <<  fiberChan << std::endl;
      if (verbosity == -22) std::cout <<" HF fiberAndChan =  "  <<  fiberAndChan << std::endl;
      
      if(ii!=0 && ((lastcapid+1)%4)!=capid) { 
	anycapid =  false;    
	if (verbosity == -22) std::cout <<" HF capid=false  digiItr =    "  <<  *digiItr << std::endl;
	ERRORfiber = fiber;
	ERRORfiberChan = fiberChan;
	ERRORfiberAndChan = fiberAndChan;
	if( capid != lastcapid ){} else{ repetedcapid =  1;}
      }
      lastcapid=capid;  
   
      if(ii == 0) firstcapid = capid;
      sumcapid += capid;
      
      if (verbosity == -22) std::cout <<" HF capid =    "  <<  capid << std::endl;
      if(er) { 
	anyer =  true;    
	if (verbosity == -22) std::cout <<" HF er=true   digiItr =   "  <<  *digiItr << std::endl;
	ERRORfiber = fiber;
	ERRORfiberChan = fiberChan;
	ERRORfiberAndChan = fiberAndChan;
      }
      if(!dv) { 
	anydv =  false;    
	if (verbosity == -22) std::cout <<" HF dv=false  digiItr =    "  <<  *digiItr << std::endl;
	ERRORfiber = fiber;
	ERRORfiberChan = fiberChan;
	ERRORfiberAndChan = fiberAndChan;
      }
      
    }// for

    if (verbosity == -22) std::cout <<" HF Digi is treated "<<std::endl;

    ///////////////////////////////////////    
    if( firstcapid==0 && !anycapid) errorGeneral = 1; 
    if( firstcapid==1 && !anycapid) errorGeneral = 2; 
    if( firstcapid==2 && !anycapid) errorGeneral = 3; 
    if( firstcapid==3 && !anycapid) errorGeneral = 4; 
    if( !anycapid )                     error1 = 1; 
    if( anyer )                         error2 = 1; 
    if( !anydv )                        error3 = 1; 
    
    if( !anycapid && anyer)                     error4 = 1; 
    if( !anycapid && !anydv)                    error5 = 1; 
    if( !anycapid && anyer && !anydv)           error6 = 1; 
    if( anyer && !anydv)                        error7 = 1; 
    ///////////////////////////////////////Energy
    // Energy:    


    double ampl = 0.;
    for (int ii=0; ii<10; ii++) {  
      double ampldefault = adc2fC[digiItr->sample(ii).adc()];
      ampl+=ampldefault;// fC
    }


    if (verbosity == -22) std::cout << std::endl << "*** E = " << ampl 
				 << "   ACD -> fC -> (gain ="
				 << std::endl;
    
    ///////////////////////////////////////Digis
    // Digis:
    // HF
    if ( sub == 4 ) {
      h_errorGeneral_HF->Fill(double(errorGeneral),1.);    
      h_error1_HF->Fill(double(error1),1.);    
      h_error2_HF->Fill(double(error2),1.);    
      h_error3_HF->Fill(double(error3),1.);    
      h_error4_HF->Fill(double(error4),1.);    
      h_error5_HF->Fill(double(error5),1.);    
      h_error6_HF->Fill(double(error6),1.);    
      h_error7_HF->Fill(double(error7),1.); 
      h_repetedcapid_HF->Fill(double(repetedcapid),1.); 
   
      if(error1 !=0 || error2 !=0 || error3 !=0 ) {
	if(studyRunDependenceHist_ && flagtodefinebadchannel_==0) ++badchannels[sub-1][mdepth-1][ieta+41][iphi];// 0-82 ; 0-71
	h_amplError_HF->Fill(ampl,1.);
	if(mdepth==1) h_mapDepth1Error_HF->Fill(double(ieta), double(iphi));    
	if(mdepth==2) h_mapDepth2Error_HF->Fill(double(ieta), double(iphi));    
	if(mdepth==3) h_mapDepth3Error_HF->Fill(double(ieta), double(iphi));    
	h_fiber0_HF->Fill(double(ERRORfiber), 1. );    
	h_fiber1_HF->Fill(double(ERRORfiberChan), 1. );    
	h_fiber2_HF->Fill(double(ERRORfiberAndChan), 1. );    
      }
      else { h_amplFine_HF->Fill(ampl,1.);}
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------ method called for each event  ------------
void VeRawAnalyzer::fillDigiErrorsHO(HODigiCollection::const_iterator& digiItr)
{

    CaloSamples tool;  // TS
    
    if (verbosity == -22) std::cout << "**************   loop over HO Digis    " << std::endl;
   
    HcalDetId cell(digiItr->id()); 
    int mdepth = cell.depth();
    int iphi  = cell.iphi()-1;
    int ieta  = cell.ieta();
    if(ieta > 0) ieta -= 1;
    int sub   = cell.subdet(); // 1-HB, 2-HE, 3-HO, 4-HF
    if (verbosity == -22) std::cout << " HO DIGI ->  "  
				 << "sub, ieta, iphi, mdepth = "  
				 <<  sub << ", " << ieta << ", " << iphi << ", " << mdepth 
				 << std::endl;

    // !!!!!!
    int errorGeneral =  0;
    int error1 =  0;
    int error2 =  0;
    int error3 =  0;
    int error4 =  0;
    int error5 =  0;
    int error6 =  0;
    int error7 =  0;
    // !!!!!!
    bool anycapid   =  true;
    bool anyer      =  false;
    bool anydv      =  true;
    // for help:
    int firstcapid  = 0;
    int sumcapid    = 0;
    int lastcapid=0, capid=0;
    int ERRORfiber = -10;
    int ERRORfiberChan = -10;
    int ERRORfiberAndChan = -10;
    int repetedcapid = 0;


    if (verbosity == -22) std::cout <<" Size of HO Digi "<<(*digiItr).size()<<std::endl;


    ///////////////////////////////////////    
    for  (int ii=0;ii<(*digiItr).size();ii++) {
      capid = (*digiItr)[ii].capid(); // capId (0-3, sequential)
      bool er    = (*digiItr)[ii].er();   // error
      bool dv    = (*digiItr)[ii].dv();  // valid data
      int fiber    = (*digiItr)[ii].fiber();  // get the fiber number
      int fiberChan    = (*digiItr)[ii].fiberChan();  // get the fiber channel number
      int fiberAndChan    = (*digiItr)[ii].fiberAndChan();  // get the id channel
      if (verbosity == -22) std::cout <<" HO fiber =  "  <<  fiber << std::endl;
      if (verbosity == -22) std::cout <<" HO fiberChan =  "  <<  fiberChan << std::endl;
      if (verbosity == -22) std::cout <<" HO fiberAndChan =  "  <<  fiberAndChan << std::endl;
      
      if(ii!=0 && ((lastcapid+1)%4)!=capid) { 
	anycapid =  false;    
	if (verbosity == -22) std::cout <<" HO capid=false  digiItr =    "  <<  *digiItr << std::endl;
	ERRORfiber = fiber;
	ERRORfiberChan = fiberChan;
	ERRORfiberAndChan = fiberAndChan;
	if( capid != lastcapid ){} else{ repetedcapid =  1;}
      }
      lastcapid=capid;  
   
      if(ii == 0) firstcapid = capid;
      sumcapid += capid;
      
      if (verbosity == -22) std::cout <<" HO capid =    "  <<  capid << std::endl;
      if(er) { 
	anyer =  true;    
	if (verbosity == -22) std::cout <<" HO er=true   digiItr =   "  <<  *digiItr << std::endl;
	ERRORfiber = fiber;
	ERRORfiberChan = fiberChan;
	ERRORfiberAndChan = fiberAndChan;
      }
      if(!dv) { 
	anydv =  false;    
	if (verbosity == -22) std::cout <<" HO dv=false  digiItr =    "  <<  *digiItr << std::endl;
	ERRORfiber = fiber;
	ERRORfiberChan = fiberChan;
	ERRORfiberAndChan = fiberAndChan;
      }
      
    }// for

    if (verbosity == -22) std::cout <<" HO Digi is treated "<<std::endl;

    ///////////////////////////////////////    
    if( firstcapid==0 && !anycapid) errorGeneral = 1; 
    if( firstcapid==1 && !anycapid) errorGeneral = 2; 
    if( firstcapid==2 && !anycapid) errorGeneral = 3; 
    if( firstcapid==3 && !anycapid) errorGeneral = 4; 
    if( !anycapid )                     error1 = 1; 
    if( anyer )                         error2 = 1; 
    if( !anydv )                        error3 = 1; 
    
    if( !anycapid && anyer)                     error4 = 1; 
    if( !anycapid && !anydv)                    error5 = 1; 
    if( !anycapid && anyer && !anydv)           error6 = 1; 
    if( anyer && !anydv)                        error7 = 1; 
    ///////////////////////////////////////Energy
    // Energy:    
     double ampl = 0.;
    for (int ii=0; ii<10; ii++) {  
      double ampldefault = adc2fC[digiItr->sample(ii).adc()];
      ampl+=ampldefault;// fC
    }
   if (verbosity == -22) std::cout << std::endl << "*** E = " << ampl 
				 << "   ACD -> fC -> (gain ="
				 << std::endl;
    
    ///////////////////////////////////////Digis
    // Digis:
    // HO
    if ( sub == 3 ) {
      h_errorGeneral_HO->Fill(double(errorGeneral),1.);    
      h_error1_HO->Fill(double(error1),1.);    
      h_error2_HO->Fill(double(error2),1.);    
      h_error3_HO->Fill(double(error3),1.);    
      h_error4_HO->Fill(double(error4),1.);    
      h_error5_HO->Fill(double(error5),1.);    
      h_error6_HO->Fill(double(error6),1.);    
      h_error7_HO->Fill(double(error7),1.); 
      h_repetedcapid_HO->Fill(double(repetedcapid),1.); 
   
      if(error1 !=0 || error2 !=0 || error3 !=0 ) {
	if(studyRunDependenceHist_ && flagtodefinebadchannel_==0) ++badchannels[sub-1][mdepth-1][ieta+41][iphi];// 0-82 ; 0-71
	h_amplError_HO->Fill(ampl,1.);
	if(mdepth==4) h_mapDepth4Error_HO->Fill(double(ieta), double(iphi));   
	// to be divided by h_mapDepth4_HO 

	if (verbosity == -12333 && (ieta == -9 || ieta == -8 || ieta == -10) && (iphi == 22 || iphi == 23 || iphi == 24  )) std::cout << " HO CapIdErrors->  "  << "sub, ieta, iphi, mdepth = " <<  sub << ", " << ieta << ", " << iphi << ", " << mdepth << std::endl;
	//	if (verbosity == -12333 && (iphi == 0 || iphi == 70 || iphi == 71  )) std::cout << " HO CapIdErrors->  "  << "sub, ieta, iphi, mdepth = " <<  sub << ", " << ieta << ", " << iphi << ", " << mdepth << std::endl;



	if(mdepth != 4) std::cout << " mdepth HO = " << mdepth  << std::endl;
	h_fiber0_HO->Fill(double(ERRORfiber), 1. );    
	h_fiber1_HO->Fill(double(ERRORfiberChan), 1. );    
	h_fiber2_HO->Fill(double(ERRORfiberAndChan), 1. );    
      }
      else { 
	h_amplFine_HO->Fill(ampl,1.);
	//		if (verbosity == -12333 && ieta == -9 && (iphi == 22 || iphi == 23 || iphi == 24  )) std::cout << " HO ALL->  "  << "sub, ieta, iphi, mdepth = " <<  sub << ", " << ieta << ", " << iphi << ", " << mdepth << std::endl;

      }
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VeRawAnalyzer::fillDigiAmplitude(HBHEDigiCollection::const_iterator& digiItr)
{
    CaloSamples tool;  // TS
    
      

    HcalDetId cell(digiItr->id()); 
    int mdepth = cell.depth();
//    int iphi0  = cell.iphi();// 1-72
    int iphi  = cell.iphi()-1;// 0-71
//    int ieta0  = cell.ieta();//-41 +41 !=0
    int ieta  = cell.ieta();
    if(ieta > 0) ieta -= 1;//-41 +41
    int sub   = cell.subdet(); // 1-HB, 2-HE (HFDigiCollection: 4-HF)
    if (verbosity == -22) std::cout << std::endl << " fillDigiAmplitude     DIGI ->  "  << "sub, ieta, iphi, mdepth = "  <<  sub << ", " << ieta << ", " << iphi << ", " << mdepth << std::endl;
    // !!!!!!
    if (verbosity == -22) std::cout <<" fillDigiAmplitude     Size of Digi "<<(*digiItr).size()<<std::endl;
    
    ///////////////////////////////////////Energy
    /*
    // Gains, pedestals 
      HcalGenericDetId hcalGenDetId(digiItr->id());
      const HcalPedestal* pedestal0 = conditions->getPedestal(hcalGenDetId);
      const HcalGain* gain = conditions->getGain(hcalGenDetId);
      const HcalGainWidth* gainWidth = conditions->getGainWidth(hcalGenDetId);
      const HcalPedestalWidth* pedWidth = conditions-> getPedestalWidth(hcalGenDetId);
      for (int i = 0; i < 4; i++) {
	if (verbosity == -81 && sub == 1) 
	  std::cout << "HB i = " <<i  
		    << "  gain = " <<gain->getValue(i)
		    << "  gainWidth = " <<gainWidth->getValue(i)
		    << "  pedestal0 = " <<pedestal0->getValue(i)
		    << "  pedWidth = " <<pedWidth->getWidth(i)
		    << std::endl;
      }
      */    

    // Energy: 
    const HcalPedestal* pedestal00 = conditions->getPedestal(cell);
    const HcalGain* gain = conditions->getGain(cell);
    const HcalGainWidth* gainWidth = conditions->getGainWidth(cell);
    const HcalRespCorr* respcorr = conditions->getHcalRespCorr(cell);
    const HcalTimeCorr* timecorr = conditions->getHcalTimeCorr(cell);
    const HcalLUTCorr* lutcorr = conditions->getHcalLUTCorr(cell);
    //
    //    HcalCalibrations calib = conditions->getHcalCalibrations(cell);


    const HcalQIECoder* channelCoder = conditions->getHcalCoder(cell);
    const HcalPedestalWidth* pedw = conditions->getPedestalWidth(cell);
    HcalCoderDb coder (*channelCoder, *shape);
    if( useADCfC_ )     coder.adc2fC(*digiItr,tool);

    if (verbosity == -22) std::cout << "coder done..." << std::endl;
    if (verbosity == -22) std::cout << "fillDigiAmplitude    coder done..." << std::endl;
    //    if (pedestal00 && gain && shape && channelCoder && respcorr && timecorr && lutcorr) {
    if (verbosity == -57 && sub == 1 ) std::cout << 
      " pedestal00-0 = " <<pedestal00->getValue(0) <<" pedestal00-1 = " <<pedestal00->getValue(1) <<
      " gain0 = " <<gain->getValue(0) <<" gain1 = " <<gain->getValue(1) <<
      " gainWidth0 = " <<gainWidth->getValue(0) <<" gainWidth1 = " <<gainWidth->getValue(1) <<
      " respcorr = " <<respcorr->getValue() <<" timecorr = " <<timecorr->getValue() << 
      " lutcorr = " <<lutcorr->getValue() << std::endl;
    //    }

    double pedestalaver9 = 0.;
    double pedestalaver4 = 0.;
    double pedestal0 = 0.;
    double pedestal1 = 0.;
    double pedestal2 = 0.;
    double pedestal3 = 0.;
    double pedestalwaver9 = 0.;
    double pedestalwaver4 = 0.;
    double pedestalw0 = 0.;
    double pedestalw1 = 0.;
    double pedestalw2 = 0.;
    double pedestalw3 = 0.;
    double difpedestal0 = 0.;
    double difpedestal1 = 0.;
    double difpedestal2 = 0.;
    double difpedestal3 = 0.;

    double amplitude = 0.;
    double absamplitude = 0.;
    double amplitude345 = 0.;
    double ampl = 0.;
    double timew = 0.;
    double timeww = 0.;
    double max_signal = -100.;
    int ts_with_max_signal = -100;
    int c0=0;
    int c1=0;
    int c2=0;
    int c3=0;
    int c4=0;
    double errorBtype = 0.;  

    //    int TSsize = 10;
    int TSsize = 10;
    //     if((*digiItr).size() !=  10) std::cout << "TSsize HBHE != 10 and = " <<(*digiItr).size()<< std::endl;
    if((*digiItr).size() !=  TSsize) errorBtype = 1.; 
    TSsize=digiItr->size();
//
    for (int ii=0; ii<TSsize; ii++) {  
      //  for (int ii=0; ii<digiItr->size(); ii++) {  
      double ampldefault = 0.;
      double ampldefault0 = 0.;
      double ampldefault1 = 0.;
      double ampldefault2 = 0.;
      ampldefault0 = adc2fC[digiItr->sample(ii).adc()];// massive
      if( useADCfC_ ) ampldefault1 = tool[ii];//adcfC
      ampldefault2  = (*digiItr)[ii].adc();//ADCcounts
      if( useADCmassive_ ) {ampldefault = ampldefault0;}
      if( useADCfC_ ) {ampldefault = ampldefault1;}
      if( useADCcounts_ ) {ampldefault = ampldefault2;}

      int capid = ((*digiItr)[ii]).capid();
      //      double pedestal = calib.pedestal(capid);
      double pedestalINI = pedestal00->getValue(capid);
      double pedestal = pedestal00->getValue(capid);
      double pedestalw= pedw->getSigma(capid,capid);

      if (verbosity == -81 && sub == 1) std::cout << "HB ii = " <<ii<< " massive = " <<ampldefault0<< " adcfC = " <<ampldefault1<< "  ADCcounts= " <<ampldefault2<< " pedestal = " << pedestal <<  " capid = " <<capid<< std::endl;

      // convert to ADC from fC
      //      ADC_ped+=channelCoder_->adc(*shape_,(float)calib.pedestal(capid),capid);
      //      ADC_width+=pedw->getSigma(capid,capid)*pow(1.*channelCoder->adc(*shape_,(float)calib.pedestal(capid),capid)/calib.pedestal(capid),2);
      
      if( usePedestalSubtraction_ ) ampldefault -=  pedestal; // pedestal subtraction
      //      ampldefault*= calib.respcorrgain(capid) ; // fC --> GeV      
      tool[ii] = ampldefault;

      pedestalaver9 +=  pedestal;    
      pedestalwaver9 +=  pedestalw*pedestalw;    
      //      if (verbosity == -5555 && sub==1 ) std::cout << "HB ii = " <<ii<< " massive = " <<ampldefault0<< " adcfC = " <<ampldefault1<< "  ADCcounts= " <<ampldefault2<< " pedestal = " << pedestal <<  " capid = " <<capid<< std::endl;


      if(capid == 0 && c0 == 0) {
	c0++;   c4++;
	pedestalaver4 +=  pedestal;    
	pedestal0 =  pedestal;  
	pedestalw0 =  pedestalw;  
	pedestalwaver4 +=  pedestalw*pedestalw;    
	difpedestal0 =  pedestal-pedestalINI;  
      }

      if(capid == 1 && c1 == 0) {
	c1++;   c4++;
	pedestalaver4 +=  pedestal;    
	pedestal1 =  pedestal;    
	pedestalw1 =  pedestalw;  
	pedestalwaver4 +=  pedestalw*pedestalw;    
	difpedestal1 =  pedestal-pedestalINI;  
      }
      if(capid == 2 && c2 == 0) {
	c2++;   c4++;
	pedestalaver4 +=  pedestal;    
	pedestal2 =  pedestal;    
	pedestalw2 =  pedestalw;  
	pedestalwaver4 +=  pedestalw*pedestalw;    
	difpedestal2 =  pedestal-pedestalINI;  
      }
      if(capid == 3 && c3 == 0) {
	c3++;   c4++;
	pedestalaver4 +=  pedestal;    
	pedestal3 =  pedestal;    
 	pedestalw3 =  pedestalw;  
	pedestalwaver4 +=  pedestalw*pedestalw;    
	difpedestal3 =  pedestal-pedestalINI;  
     }
      
      if(max_signal < ampldefault ) {
	max_signal = ampldefault;
	ts_with_max_signal = ii;
      }
      amplitude+=ampldefault;// 
      absamplitude+=abs(ampldefault);// 

      if(ii ==3 || ii==4 || ii==5) amplitude345+=ampldefault;  


      if(flagcpuoptimization_== 0 ) {
	/*
      // if(ii == 0 ) amplitude0+=ampldefault;// fC    
      // if(ii == 1 ) amplitude1+=ampldefault;// fC  
      //  
	if(sub == 1 )  h_ADC_HB->Fill(ampldefault, 1.);
	if(sub == 2 )  h_ADC_HE->Fill(ampldefault, 1.);
      //  
	if(sub == 1 && mdepth == 1 )  h_ADC_HBdepth1->Fill(ampldefault, 1.);
	if(sub == 1 && mdepth == 2 )  h_ADC_HBdepth2->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 1 )  h_ADC_HEdepth1->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 2 )  h_ADC_HEdepth2->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 3 )  h_ADC_HEdepth3->Fill(ampldefault, 1.);
      if(ii == 0 ) {
	if(sub == 1 && mdepth == 1 )  h_ADC_HBdepth1_TS0->Fill(ampldefault, 1.);
	if(sub == 1 && mdepth == 2 )  h_ADC_HBdepth2_TS0->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 1 )  h_ADC_HEdepth1_TS0->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 2 )  h_ADC_HEdepth2_TS0->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 3 )  h_ADC_HEdepth3_TS0->Fill(ampldefault, 1.);
      }
      if(ii == 1 ) {
	if(sub == 1 && mdepth == 1 )  h_ADC_HBdepth1_TS1->Fill(ampldefault, 1.);
	if(sub == 1 && mdepth == 2 )  h_ADC_HBdepth2_TS1->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 1 )  h_ADC_HEdepth1_TS1->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 2 )  h_ADC_HEdepth2_TS1->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 3 )  h_ADC_HEdepth3_TS1->Fill(ampldefault, 1.);
      }
      if(ii == 2 ) {
	if(sub == 1 && mdepth == 1 )  h_ADC_HBdepth1_TS2->Fill(ampldefault, 1.);
	if(sub == 1 && mdepth == 2 )  h_ADC_HBdepth2_TS2->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 1 )  h_ADC_HEdepth1_TS2->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 2 )  h_ADC_HEdepth2_TS2->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 3 )  h_ADC_HEdepth3_TS2->Fill(ampldefault, 1.);
      }
      if(ii == 3 ) {
	if(sub == 1 && mdepth == 1 )  h_ADC_HBdepth1_TS3->Fill(ampldefault, 1.);
	if(sub == 1 && mdepth == 2 )  h_ADC_HBdepth2_TS3->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 1 )  h_ADC_HEdepth1_TS3->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 2 )  h_ADC_HEdepth2_TS3->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 3 )  h_ADC_HEdepth3_TS3->Fill(ampldefault, 1.);
      }
      if(ii == 4 ) {
	if(sub == 1 && mdepth == 1 )  h_ADC_HBdepth1_TS4->Fill(ampldefault, 1.);
	if(sub == 1 && mdepth == 2 )  h_ADC_HBdepth2_TS4->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 1 )  h_ADC_HEdepth1_TS4->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 2 )  h_ADC_HEdepth2_TS4->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 3 )  h_ADC_HEdepth3_TS4->Fill(ampldefault, 1.);
      }
      if(ii == 5 ) {
	if(sub == 1 && mdepth == 1 )  h_ADC_HBdepth1_TS5->Fill(ampldefault, 1.);
	if(sub == 1 && mdepth == 2 )  h_ADC_HBdepth2_TS5->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 1 )  h_ADC_HEdepth1_TS5->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 2 )  h_ADC_HEdepth2_TS5->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 3 )  h_ADC_HEdepth3_TS5->Fill(ampldefault, 1.);
      }
      if(ii == 6 ) {
	if(sub == 1 && mdepth == 1 )  h_ADC_HBdepth1_TS6->Fill(ampldefault, 1.);
	if(sub == 1 && mdepth == 2 )  h_ADC_HBdepth2_TS6->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 1 )  h_ADC_HEdepth1_TS6->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 2 )  h_ADC_HEdepth2_TS6->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 3 )  h_ADC_HEdepth3_TS6->Fill(ampldefault, 1.);
      }
      if(ii == 7 ) {
	if(sub == 1 && mdepth == 1 )  h_ADC_HBdepth1_TS7->Fill(ampldefault, 1.);
	if(sub == 1 && mdepth == 2 )  h_ADC_HBdepth2_TS7->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 1 )  h_ADC_HEdepth1_TS7->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 2 )  h_ADC_HEdepth2_TS7->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 3 )  h_ADC_HEdepth3_TS7->Fill(ampldefault, 1.);
      }
      if(ii == 8 ) {
	if(sub == 1 && mdepth == 1 )  h_ADC_HBdepth1_TS8->Fill(ampldefault, 1.);
	if(sub == 1 && mdepth == 2 )  h_ADC_HBdepth2_TS8->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 1 )  h_ADC_HEdepth1_TS8->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 2 )  h_ADC_HEdepth2_TS8->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 3 )  h_ADC_HEdepth3_TS8->Fill(ampldefault, 1.);
      }
      if(ii == 9 ) {
	if(sub == 1 && mdepth == 1 )  h_ADC_HBdepth1_TS9->Fill(ampldefault, 1.);
	if(sub == 1 && mdepth == 2 )  h_ADC_HBdepth2_TS9->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 1 )  h_ADC_HEdepth1_TS9->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 2 )  h_ADC_HEdepth2_TS9->Fill(ampldefault, 1.);
	if(sub == 2 && mdepth == 3 )  h_ADC_HEdepth3_TS9->Fill(ampldefault, 1.);
      }
*/
  //////      
	}//flagcpuoptimization
      if (verbosity == -22) std::cout << "fillDigiAmplitude    amplitude = " << amplitude << std::endl;
      timew += (ii+1)*abs(ampldefault);
      timeww += (ii+1)*ampldefault;
    }//for 1
    amplitudechannel[sub-1][mdepth-1][ieta+41][iphi] += amplitude;// 0-82 ; 0-71  HBHE

    pedestalaver9 /= TSsize;
    pedestalaver4 /= c4;
    //pow(pedestalwaver9/TSsize,0.5);
    pedestalwaver9 = sqrt(pedestalwaver9/TSsize);
    //pow(pedestalwaver4/c4,0.5);
    pedestalwaver4 = sqrt(pedestalwaver4/c4);
    
    
    if (verbosity == -81 && sub == 1) std::cout << "HB pedestalaver9 = " <<pedestalaver9<< std::endl;
    //    if (verbosity == -22) std::cout << std::endl << "*** E = " << ampl << "   ACD -> fC -> (gain ="<< calib.respcorrgain(0) << ") GeV (ped.subtracted)" << std::endl;
    // ------------ to get signal in TS: -2 max +1  ------------
    
    if(ts_with_max_signal > -1 && ts_with_max_signal < 10) ampl = tool[ts_with_max_signal];
    if(ts_with_max_signal+2 > -1 && ts_with_max_signal+2 < 10) ampl += tool[ts_with_max_signal+2];
    if(ts_with_max_signal+1 > -1 && ts_with_max_signal+1 < 10) ampl += tool[ts_with_max_signal+1];
    if(ts_with_max_signal-1 > -1 && ts_with_max_signal-1 < 10) ampl += tool[ts_with_max_signal-1];

    //  if(ts_with_max_signal+1 > -1 && ts_with_max_signal+1 < 10) ampl += tool[ts_with_max_signal+1];
    //  if(ts_with_max_signal-1 > -1 && ts_with_max_signal-1 < 10) ampl += tool[ts_with_max_signal-1];
    //  if(ts_with_max_signal-2 > -1 && ts_with_max_signal-2 < 10) ampl += tool[ts_with_max_signal-2];

    double ratio = 0.;
    //    if(amplallTS != 0.) ratio = ampl/amplallTS;
    if(amplitude != 0. ) ratio = ampl/amplitude;
    if (verbosity == -22 && (ratio<0. || ratio>1.02)) {
      
      std::cout << " ratio = " <<ratio<< " ampl ="<<ampl<<" amplitude ="<<amplitude<< " ts_with_max_signal ="<<ts_with_max_signal<< " TSsize ="
                                      <<TSsize<< " max_signal ="<<max_signal<< std::endl;
      
      std::cout << " tool[ts_with_max_signal] = " << tool[ts_with_max_signal]  << "  tool[ts_with_max_signal+1]= " << tool[ts_with_max_signal+1]  
                << "  tool[ts_with_max_signal-1]= " << tool[ts_with_max_signal-1]  << "  tool[ts_with_max_signal-2]= " << tool[ts_with_max_signal-2]  
               << std::endl;
      
      std::cout << " tool[0] = " << tool[0]  << "  tool[1]= " << tool[1]  << "  tool[2]= " << tool[2]  << "  tool[3]= " << tool[3]  << "  tool[4]= " 
                << tool[4]  << "  tool[5]= " << tool[5]  << "  tool[6]= " << tool[6]  << "  tool[7]= " << tool[7]  << "  tool[8]= " << tool[8]  
                << "  tool[9]= " << tool[9]  << std::endl;
    }

    if (ratio<0. || ratio>1.02) ratio = 0.;
    
    if (verbosity == -22) std::cout << "* ratio = " <<ratio<< " ampl ="<<ampl<< std::endl;

    
    double aveamplitude = 0.;
    double aveamplitudew = 0.;
    if(absamplitude >0 && timew >0)  aveamplitude = timew/absamplitude;// average_TS +1
    if(amplitude >0 && timeww >0)  aveamplitudew = timeww/amplitude;// average_TS +1
    
    double rmsamp = 0.;
    // and CapIdErrors:
    int error = 0;
    bool anycapid   =  true;
    bool anyer      =  false;
    bool anydv      =  true;
    int lastcapid=0;
    int capid=0;
    if (verbosity == -222) std::cout << "* TSsize = " <<TSsize<< "* tool[0] = " <<tool[0]  << std::endl;
    for (int ii=0; ii<TSsize; ii++) {  
      double aaaaaa = (ii+1)-aveamplitudew;
      double aaaaaa2 = aaaaaa*aaaaaa;
      double ampldefault = tool[ii];
      rmsamp+=(aaaaaa2*ampldefault);// fC
      capid = ((*digiItr)[ii]).capid();
      bool er    = (*digiItr)[ii].er();   // error
      bool dv    = (*digiItr)[ii].dv();  // valid data
      if(ii!=0 && ((lastcapid+1)%4)!=capid) { anycapid =  false; }
      //    std::cout << " ii = " << ii  << " capid = " << capid  << " ((lastcapid+1)%4) = " << ((lastcapid+1)%4)  << std::endl;
      lastcapid=capid;  
      if(er) {anyer =  true; }
      if(!dv) {anydv =  false; }
    }//for 2
    if( !anycapid || anyer || !anydv )    error = 1; 

    double rmsamplitude = 0.;
    if( (amplitude >0 && rmsamp >0) || (amplitude <0 && rmsamp <0))  rmsamplitude = sqrt( rmsamp/amplitude );
    double aveamplitude1 = aveamplitude-1;// means iTS=0-9

//    for (int ii=0; ii<TSsize; ii++) {  
//      capid = ((*digiItr)[ii]).capid();
//      //    if( !anycapid) std::cout << "111: ii = " << ii  << " capid = " << capid  << std::endl;// main item here
//      if( anyer) std::cout << "222222: ii = " << ii  << " capid = " << capid  << std::endl;
//      if( !anydv) std::cout << "333333333: ii = " << ii  << " capid = " << capid  << std::endl;
//    }
    // CapIdErrors end  /////////////////////////////////////////////////////////
    
    // AZ 1.10.2015:
    if(error == 1 ) {
      if(sub == 1 && mdepth == 1 )  h_Amplitude_forCapIdErrors_HB1->Fill(amplitude, 1.);
      if(sub == 1 && mdepth == 2 )  h_Amplitude_forCapIdErrors_HB2->Fill(amplitude, 1.);
      if(sub == 2 && mdepth == 1 )  h_Amplitude_forCapIdErrors_HE1->Fill(amplitude, 1.);
      if(sub == 2 && mdepth == 2 )  h_Amplitude_forCapIdErrors_HE2->Fill(amplitude, 1.);
      if(sub == 2 && mdepth == 3 )  h_Amplitude_forCapIdErrors_HE3->Fill(amplitude, 1.);
    }
    if(error != 1 ) {
      if(sub == 1 && mdepth == 1 )  h_Amplitude_notCapIdErrors_HB1->Fill(amplitude, 1.);
      if(sub == 1 && mdepth == 2 )  h_Amplitude_notCapIdErrors_HB2->Fill(amplitude, 1.);
      if(sub == 2 && mdepth == 1 )  h_Amplitude_notCapIdErrors_HE1->Fill(amplitude, 1.);
      if(sub == 2 && mdepth == 2 )  h_Amplitude_notCapIdErrors_HE2->Fill(amplitude, 1.);
      if(sub == 2 && mdepth == 3 )  h_Amplitude_notCapIdErrors_HE3->Fill(amplitude, 1.);
    }
    
    
    for (int ii=0; ii<TSsize; ii++) {  
      //  for (int ii=0; ii<10; ii++) {  
      double ampldefault = tool[ii];
      ///
      if ( sub == 1 ) {
	if(amplitude >120 ) {
	  h_shape_Ahigh_HB0->Fill(float(ii),ampldefault);
	  h_shape0_Ahigh_HB0->Fill(float(ii),1.);
	}
	else {
	  h_shape_Alow_HB0->Fill(float(ii),ampldefault);
	  h_shape0_Alow_HB0->Fill(float(ii),1.);
	} //HB0
	///
	if(pedestal2 < pedestalHBMax_ || pedestal3 < pedestalHBMax_ || 
	   pedestal2 < pedestalHBMax_ || pedestal3 < pedestalHBMax_ ) {
	  h_shape_Ahigh_HB1->Fill(float(ii),ampldefault);
	  h_shape0_Ahigh_HB1->Fill(float(ii),1.);
	}
	else {
	  h_shape_Alow_HB1->Fill(float(ii),ampldefault);
	  h_shape0_Alow_HB1->Fill(float(ii),1.);
	} //HB1
	if(error == 0 ) {
	  h_shape_Ahigh_HB2->Fill(float(ii),ampldefault);
	  h_shape0_Ahigh_HB2->Fill(float(ii),1.);
	}
	else {
	  h_shape_Alow_HB2->Fill(float(ii),ampldefault);
	  h_shape0_Alow_HB2->Fill(float(ii),1.);
	} //HB2
	///
	if(pedestalw0 < pedestalwHBMax_ || pedestalw1 < pedestalwHBMax_
	   || pedestalw2 < pedestalwHBMax_ || pedestalw3 < pedestalwHBMax_ ) {
	  h_shape_Ahigh_HB3->Fill(float(ii),ampldefault);
	  h_shape0_Ahigh_HB3->Fill(float(ii),1.);
	}
	else {
	  h_shape_Alow_HB3->Fill(float(ii),ampldefault);
	  h_shape0_Alow_HB3->Fill(float(ii),1.);
	} //HB3
	
      }// sub   HB
      
    }//for 3 over TSs
    
    
    if ( sub == 1 ) {
      
      // bad_channels with C,A,W,P,pW,
      if( error == 1 ||
	  amplitude < ADCAmplHBMin_ || amplitude > ADCAmplHBMax_ ||
	  rmsamplitude < rmsHBMin_ || rmsamplitude > rmsHBMax_ ||
	  pedestal0 < pedestalHBMax_ || pedestal1 < pedestalHBMax_
	  || pedestal2 < pedestalHBMax_ || pedestal3 < pedestalHBMax_ ||
	  pedestalw0 < pedestalwHBMax_ || pedestalw1 < pedestalwHBMax_
	  || pedestalw2 < pedestalwHBMax_ || pedestalw3 < pedestalwHBMax_ 
	  
	  ) {
	for (int ii=0; ii<TSsize; ii++) {  
	  double ampldefault = tool[ii];
	  h_shape_bad_channels_HB->Fill(float(ii),ampldefault);
	  h_shape0_bad_channels_HB->Fill(float(ii),1.);
	}
      }
      // good_channels with C,A,W,P,pW 
      else {
	for (int ii=0; ii<TSsize; ii++) {  
	  double ampldefault = tool[ii];
	  h_shape_good_channels_HB->Fill(float(ii),ampldefault);
	  h_shape0_good_channels_HB->Fill(float(ii),1.);
	}
      }
    }// sub   HB
    if ( sub == 2 ) {
      // bad_channels with C,A,W,P,pW,
      if( error == 1 ||
	  amplitude < ADCAmplHEMin_ || amplitude > ADCAmplHEMax_ ||
	  rmsamplitude < rmsHEMin_ || rmsamplitude > rmsHEMax_ ||
	  pedestal0 < pedestalHEMax_ || pedestal1 < pedestalHEMax_
	  || pedestal2 < pedestalHEMax_ || pedestal3 < pedestalHEMax_ ||
	  pedestalw0 < pedestalwHEMax_ || pedestalw1 < pedestalwHEMax_
	  || pedestalw2 < pedestalwHEMax_ || pedestalw3 < pedestalwHEMax_ 
	  
	  ) {
	for (int ii=0; ii<TSsize; ii++) {  
	  double ampldefault = tool[ii];
	  h_shape_bad_channels_HE->Fill(float(ii),ampldefault);
	  h_shape0_bad_channels_HE->Fill(float(ii),1.);
	}
      }
      // good_channels with C,A,W,P,pW,
      else {
	for (int ii=0; ii<TSsize; ii++) {  
	  double ampldefault = tool[ii];
	  h_shape_good_channels_HE->Fill(float(ii),ampldefault);
	  h_shape0_good_channels_HE->Fill(float(ii),1.);
	}
      }
    }// sub   HE
    
    

    ///////////////////////////////////////Digis : over all digiHits

    sum0Estimator[sub-1][mdepth-1][ieta+41][iphi] += 1.;

    //      for Error B-type
    sumEstimator6[sub-1][mdepth-1][ieta+41][iphi] += errorBtype;


    //    sumEstimator0[sub-1][mdepth-1][ieta+41][iphi] += pedestalw0;//Sig_Pedestals
    sumEstimator0[sub-1][mdepth-1][ieta+41][iphi] += pedestal0;//Pedestals
    if (verbosity == -81 && sub == 1) std::cout << "HB pedestalaver = " <<pedestalaver9<< "sumEstimator0 = " <<sumEstimator0[sub-1][mdepth-1][ieta+41][iphi]<< std::endl;
    // HB
    if ( sub == 1 ) {

      if(studyPedestalCorrelations_) {
      //   //   //   //   //   //   //   //   //  HB       PedestalCorrelations :
//	double mypedestal  = pedestalaver9;
//	double mypedestalw = pedestalwaver9;
	double mypedestal  = pedestal0;
	double mypedestalw = pedestalw0;
	if(verbosity == -91 && pedestal0<0.2) std::cout << "111111 HB ped1 = " <<  pedestal1  << " ped2 = " <<  pedestal2  << " ped3 = " <<  pedestal3  << std::endl;
	if(verbosity == -92 && pedestal3<0.2) std::cout << "222 HB ped0 = " <<  pedestal0  << " ped1 = " <<  pedestal1  << " ped2 = " <<  pedestal2  << std::endl;
	h2_pedvsampl_HB->Fill(mypedestal,amplitude);
	h2_pedwvsampl_HB->Fill(mypedestalw,amplitude);
	h_pedvsampl_HB->Fill(mypedestal,amplitude);
	h_pedwvsampl_HB->Fill(mypedestalw,amplitude);
	h_pedvsampl0_HB->Fill(mypedestal,1.);
	h_pedwvsampl0_HB->Fill(mypedestalw,1.);
	
	h2_amplvsped_HB->Fill(amplitude,mypedestal);
	h2_amplvspedw_HB->Fill(amplitude,mypedestalw);
	h_amplvsped_HB->Fill(amplitude,mypedestal);
	h_amplvspedw_HB->Fill(amplitude,mypedestalw);
	h_amplvsped0_HB->Fill(amplitude,1.);
      }//
      
      //   //   //   //   //   //   //   //   //  HB       Pedestals:
      if(studyPedestalsHist_) {
	h_pedestal0_HB->Fill(pedestal0,1.);
	h_pedestal1_HB->Fill(pedestal1,1.);
	h_pedestal2_HB->Fill(pedestal2,1.);
	h_pedestal3_HB->Fill(pedestal3,1.);
	h_pedestalaver4_HB->Fill(pedestalaver4,1.);
	h_pedestalaver9_HB->Fill(pedestalaver9,1.);
	h_pedestalw0_HB->Fill(pedestalw0,1.);
	h_pedestalw1_HB->Fill(pedestalw1,1.);
	h_pedestalw2_HB->Fill(pedestalw2,1.);
	h_pedestalw3_HB->Fill(pedestalw3,1.);
	h_pedestalwaver4_HB->Fill(pedestalwaver4,1.);
	h_pedestalwaver9_HB->Fill(pedestalwaver9,1.);
	// for averaged values:
	if(mdepth==1) {
	  h_mapDepth1Ped0_HB->Fill(double(ieta), double(iphi), pedestal0);
	  h_mapDepth1Ped1_HB->Fill(double(ieta), double(iphi), pedestal1);
	  h_mapDepth1Ped2_HB->Fill(double(ieta), double(iphi), pedestal2);
	  h_mapDepth1Ped3_HB->Fill(double(ieta), double(iphi), pedestal3);
	  h_mapDepth1Pedw0_HB->Fill(double(ieta), double(iphi), pedestalw0);
	  h_mapDepth1Pedw1_HB->Fill(double(ieta), double(iphi), pedestalw1);
	  h_mapDepth1Pedw2_HB->Fill(double(ieta), double(iphi), pedestalw2);
	  h_mapDepth1Pedw3_HB->Fill(double(ieta), double(iphi), pedestalw3);
	}
	if(mdepth==2) {
	  h_mapDepth2Ped0_HB->Fill(double(ieta), double(iphi), pedestal0);
	  h_mapDepth2Ped1_HB->Fill(double(ieta), double(iphi), pedestal1);
	  h_mapDepth2Ped2_HB->Fill(double(ieta), double(iphi), pedestal2);
	  h_mapDepth2Ped3_HB->Fill(double(ieta), double(iphi), pedestal3);
	  h_mapDepth2Pedw0_HB->Fill(double(ieta), double(iphi), pedestalw0);
	  h_mapDepth2Pedw1_HB->Fill(double(ieta), double(iphi), pedestalw1);
	  h_mapDepth2Pedw2_HB->Fill(double(ieta), double(iphi), pedestalw2);
	  h_mapDepth2Pedw3_HB->Fill(double(ieta), double(iphi), pedestalw3);
	}

	if(pedestalw0 < pedestalwHBMax_ || pedestalw1 < pedestalwHBMax_
	   || pedestalw2 < pedestalwHBMax_ || pedestalw3 < pedestalwHBMax_) {
	  if(mdepth==1) h_mapDepth1pedestalw_HB->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==2) h_mapDepth2pedestalw_HB->Fill(double(ieta), double(iphi), 1.);
	}
	
	if(pedestal0 < pedestalHBMax_ || pedestal1 < pedestalHBMax_
	   || pedestal2 < pedestalHBMax_ || pedestal3 < pedestalHBMax_) {
	  if(mdepth==1) h_mapDepth1pedestal_HB->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==2) h_mapDepth2pedestal_HB->Fill(double(ieta), double(iphi), 1.);
	}
	
	for (int ii=0; ii<4; ii++) {
	  h_pedestal00_HB->Fill(pedestal00->getValue(ii),1.);
	  h_gain_HB->Fill(gain->getValue(ii),1.);
	}
	h_respcorr_HB->Fill(respcorr->getValue(),1.);
	h_timecorr_HB->Fill(timecorr->getValue(),1.);
	h_lutcorr_HB->Fill(lutcorr->getValue(),1.);
	h_difpedestal0_HB->Fill(difpedestal0,1.);
	h_difpedestal1_HB->Fill(difpedestal1,1.);
	h_difpedestal2_HB->Fill(difpedestal2,1.);
	h_difpedestal3_HB->Fill(difpedestal3,1.);
	
      }//
      
      //   //   //   //   //   //   //   //   //  HB       ADCAmpl:
      if(studyADCAmplHist_) {
	h_ADCAmpl345Zoom_HB->Fill(amplitude345,1.);
	h_ADCAmpl345Zoom1_HB->Fill(amplitude345,1.);
	h_ADCAmpl345_HB->Fill(amplitude345,1.);
	if(error == 0 ) {
	  h_ADCAmpl_HBCapIdNoError->Fill(amplitude,1.);
	  h_ADCAmpl345_HBCapIdNoError->Fill(amplitude345,1.);
	}
	if(error == 1 ) {
	  h_ADCAmpl_HBCapIdError->Fill(amplitude,1.);
	  h_ADCAmpl345_HBCapIdError->Fill(amplitude345,1.);
	}

	h_ADCAmplZoom_HB->Fill(amplitude,1.);
	h_ADCAmplZoom1_HB->Fill(amplitude,1.);
	h_ADCAmpl_HB->Fill(amplitude,1.);
	if(amplitude < ADCAmplHBMin_ || amplitude > ADCAmplHBMax_) {
	  if(studyRunDependenceHist_ && flagtodefinebadchannel_==5) ++badchannels[sub-1][mdepth-1][ieta+41][iphi];// 0-82 ; 0-71
	  if(mdepth==1) h_mapDepth1ADCAmpl225_HB->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==2) h_mapDepth2ADCAmpl225_HB->Fill(double(ieta), double(iphi), 1.);
	  if (verbosity == -56) std::cout << "***BAD HB channels from ADCAmpl: "  <<" ieta= " << ieta <<" iphi= " << iphi <<" amplitude= " << amplitude << std::endl;
	}// if

	//	if(amplitude >400.) averSIGNALoccupancy_HB += 1.;
	if(amplitude < 35.) {
	  if(mdepth==1) h_mapDepth1ADCAmpl225Copy_HB->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==2) h_mapDepth2ADCAmpl225Copy_HB->Fill(double(ieta), double(iphi), 1.);
	}// if
	
	// for averaged values:
	if(mdepth==1) h_mapDepth1ADCAmpl_HB->Fill(double(ieta), double(iphi), amplitude);
	if(mdepth==2) h_mapDepth2ADCAmpl_HB->Fill(double(ieta), double(iphi), amplitude);
	if(mdepth==1) h_mapDepth1ADCAmpl12_HB->Fill(double(ieta), double(iphi), ampl);
	if(mdepth==2) h_mapDepth2ADCAmpl12_HB->Fill(double(ieta), double(iphi), ampl);

	  if(amplitude > forallestimators_amplitude_bigger_) sumEstimator1[sub-1][mdepth-1][ieta+41][iphi] += amplitude;

	  	  if (verbosity == -5555 && sub==1 && mdepth==1&&ieta==15&&iphi==18) std::cout << "***ADCAmpl: "  <<" ieta= " << ieta <<" iphi= " << iphi <<" aveamplitude1= " << aveamplitude1 <<" amplitude= " << amplitude << "sumEstimator1[sub-1][mdepth-1][ieta+41][iphi]= "  << sumEstimator1[sub-1][mdepth-1][ieta+41][iphi]  << std::endl;	

      }//if(studyADCAmplHist_
      ///////////////////////////////

      //   //   //   //   //   //   //   //   //  HB       TSmean:
      if(studyTSmeanShapeHist_) {
	h_TSmeanA_HB->Fill(aveamplitude1,1.);
	if(aveamplitude1 < TSmeanHBMin_ || aveamplitude1 > TSmeanHBMax_ ) {
	  if(studyRunDependenceHist_ && flagtodefinebadchannel_==4) ++badchannels[sub-1][mdepth-1][ieta+41][iphi];// 0-82 ; 0-71
	  if(mdepth==1) h_mapDepth1TSmeanA225_HB->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==2) h_mapDepth2TSmeanA225_HB->Fill(double(ieta), double(iphi), 1.);
	  if (verbosity == -55) std::cout << "***BAD HB channels from TSmaxA: "  <<" ieta= " << ieta <<" iphi= " << iphi <<" aveamplitude1= " << aveamplitude1 << std::endl;
	}// if
	// for averaged values:
	if(mdepth==1) h_mapDepth1TSmeanA_HB->Fill(double(ieta), double(iphi), aveamplitude1);
	if(mdepth==2) h_mapDepth2TSmeanA_HB->Fill(double(ieta), double(iphi), aveamplitude1);

	if(amplitude > forallestimators_amplitude_bigger_) sumEstimator2[sub-1][mdepth-1][ieta+41][iphi] += aveamplitude1;
	
      }//if(studyTSmeanShapeHist_
      ///////////////////////////////

      //   //   //   //   //   //   //   //   //  HB       TSmax:
      if(studyTSmaxShapeHist_) {
	h_TSmaxA_HB->Fill(float(ts_with_max_signal),1.);
	if(ts_with_max_signal < TSpeakHBMin_ || ts_with_max_signal > TSpeakHBMax_ ) {
	  if(studyRunDependenceHist_ && flagtodefinebadchannel_==3) ++badchannels[sub-1][mdepth-1][ieta+41][iphi];// 0-82 ; 0-71
	  if(mdepth==1) h_mapDepth1TSmaxA225_HB->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==2) h_mapDepth2TSmaxA225_HB->Fill(double(ieta), double(iphi), 1.);
	  if (verbosity == -55) std::cout << "***BAD HB channels from TSmaxA: "  <<" ieta= " << ieta <<" iphi= " << iphi <<" ts_with_max_signal= " << ts_with_max_signal << std::endl;
	}// if
	// for averaged values:
	if(mdepth==1) h_mapDepth1TSmaxA_HB->Fill(double(ieta),double(iphi),float(ts_with_max_signal));
	if(mdepth==2) h_mapDepth2TSmaxA_HB->Fill(double(ieta),double(iphi),float(ts_with_max_signal));

	if(amplitude > forallestimators_amplitude_bigger_) sumEstimator3[sub-1][mdepth-1][ieta+41][iphi] += float(ts_with_max_signal);
      }//if(studyTSmaxShapeHist_
      ///////////////////////////////
      
      
      //   //   //   //   //   //   //   //   //  HB       RMS:
      if(studyRMSshapeHist_) {
	h_Amplitude_HB->Fill(rmsamplitude,1.);
	if(rmsamplitude < rmsHBMin_ || rmsamplitude > rmsHBMax_ ) {
	  if(studyRunDependenceHist_ && flagtodefinebadchannel_==2) ++badchannels[sub-1][mdepth-1][ieta+41][iphi];// 0-82 ; 0-71
	  if(mdepth==1) h_mapDepth1Amplitude225_HB->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==2) h_mapDepth2Amplitude225_HB->Fill(double(ieta), double(iphi), 1.);
	  if (verbosity == -54) std::cout << "***BAD HB channels from shape RMS:  "  <<" ieta= " << ieta <<" iphi= " << iphi << std::endl;
	}// if
	// for averaged values:
	if(mdepth==1) h_mapDepth1Amplitude_HB->Fill(double(ieta), double(iphi), rmsamplitude);    
	if(mdepth==2) h_mapDepth2Amplitude_HB->Fill(double(ieta), double(iphi), rmsamplitude);    

	if(amplitude > forallestimators_amplitude_bigger_) sumEstimator4[sub-1][mdepth-1][ieta+41][iphi] += rmsamplitude;
      }//if(studyRMSshapeHist_)
      ///////////////////////////////
      
      
      //   //   //   //   //   //   //   //   //  HB       Ratio:
      if(studyRatioShapeHist_) {
	h_Ampl_HB->Fill(ratio,1.);
	if(ratio < ratioHBMin_ || ratio > ratioHBMax_  ) {
	  if(studyRunDependenceHist_ && flagtodefinebadchannel_==1) ++badchannels[sub-1][mdepth-1][ieta+41][iphi];// 0-82 ; 0-71
	  if(mdepth==1) h_mapDepth1Ampl047_HB->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==2) h_mapDepth2Ampl047_HB->Fill(double(ieta), double(iphi), 1.);
	  // //
	  if (verbosity == -53) std::cout << "***BAD HB channels from shape Ratio:  " <<" ieta= " << ieta <<" iphi= " << iphi << std::endl;
	  if (verbosity == -51  ) std::cout << "***BAD HB channels from shape Ratio:  " <<" ieta= " << ieta <<" iphi= " << iphi <<" sub= " << sub <<" mdepth= " << mdepth <<" badchannels= " << badchannels[sub-1][mdepth-1][ieta+41][iphi] << std::endl;
	}//if(ratio
	// for averaged values:
	if(mdepth==1) h_mapDepth1Ampl_HB->Fill(double(ieta), double(iphi), ratio);    
	if(mdepth==2) h_mapDepth2Ampl_HB->Fill(double(ieta), double(iphi), ratio);    
	
	if(amplitude > forallestimators_amplitude_bigger_) sumEstimator5[sub-1][mdepth-1][ieta+41][iphi] += ratio;
      }//if(studyRatioShapeHist_)
      
      ///////////////////////////////
      
      //   //   //   //   //   //   //   //   //  HB      DiffAmplitude:
      if(studyDiffAmplHist_) {
	if(mdepth==1) h_mapDepth1AmplE34_HB->Fill(double(ieta), double(iphi), amplitude);    
	if(mdepth==2) h_mapDepth2AmplE34_HB->Fill(double(ieta), double(iphi), amplitude);    
      }// if(studyDiffAmplHist_)     
      
      
      ///////////////////////////////    for HB All 
      if(mdepth==1) h_mapDepth1_HB->Fill(double(ieta), double(iphi), 1.);    
      if(mdepth==2) h_mapDepth2_HB->Fill(double(ieta), double(iphi), 1.);    
      
      
    }//if ( sub == 1 )
    
    // HE
    if ( sub == 2 ) {

      //   //   //   //   //   //   //   //   //  HE       PedestalCorrelations :
      if(studyPedestalCorrelations_) {
//	double mypedestal  = pedestalaver9;
//	double mypedestalw = pedestalwaver9;
	double mypedestal  = pedestal0;
	double mypedestalw = pedestalw0;
	h2_pedvsampl_HE->Fill(mypedestal,amplitude);
	h2_pedwvsampl_HE->Fill(mypedestalw,amplitude);
	h_pedvsampl_HE->Fill(mypedestal,amplitude);
	h_pedwvsampl_HE->Fill(mypedestalw,amplitude);
	h_pedvsampl0_HE->Fill(mypedestal,1.);
	h_pedwvsampl0_HE->Fill(mypedestalw,1.);
      }//
      

      //   //   //   //   //   //   //   //   //  HE       Pedestals:
      if(studyPedestalsHist_) {
	h_pedestal0_HE->Fill(pedestal0,1.);
	h_pedestal1_HE->Fill(pedestal1,1.);
	h_pedestal2_HE->Fill(pedestal2,1.);
	h_pedestal3_HE->Fill(pedestal3,1.);
	h_pedestalaver4_HE->Fill(pedestalaver4,1.);
	h_pedestalaver9_HE->Fill(pedestalaver9,1.);
	h_pedestalw0_HE->Fill(pedestalw0,1.);
	h_pedestalw1_HE->Fill(pedestalw1,1.);
	h_pedestalw2_HE->Fill(pedestalw2,1.);
	h_pedestalw3_HE->Fill(pedestalw3,1.);
	h_pedestalwaver4_HE->Fill(pedestalwaver4,1.);
	h_pedestalwaver9_HE->Fill(pedestalwaver9,1.);
	// for averaged values:
	if(mdepth==1) {
	  h_mapDepth1Ped0_HE->Fill(double(ieta), double(iphi), pedestal0);
	  h_mapDepth1Ped1_HE->Fill(double(ieta), double(iphi), pedestal1);
	  h_mapDepth1Ped2_HE->Fill(double(ieta), double(iphi), pedestal2);
	  h_mapDepth1Ped3_HE->Fill(double(ieta), double(iphi), pedestal3);
	  h_mapDepth1Pedw0_HE->Fill(double(ieta), double(iphi), pedestalw0);
	  h_mapDepth1Pedw1_HE->Fill(double(ieta), double(iphi), pedestalw1);
	  h_mapDepth1Pedw2_HE->Fill(double(ieta), double(iphi), pedestalw2);
	  h_mapDepth1Pedw3_HE->Fill(double(ieta), double(iphi), pedestalw3);
	}
	if(mdepth==2) {
	  h_mapDepth2Ped0_HE->Fill(double(ieta), double(iphi), pedestal0);
	  h_mapDepth2Ped1_HE->Fill(double(ieta), double(iphi), pedestal1);
	  h_mapDepth2Ped2_HE->Fill(double(ieta), double(iphi), pedestal2);
	  h_mapDepth2Ped3_HE->Fill(double(ieta), double(iphi), pedestal3);
	  h_mapDepth2Pedw0_HE->Fill(double(ieta), double(iphi), pedestalw0);
	  h_mapDepth2Pedw1_HE->Fill(double(ieta), double(iphi), pedestalw1);
	  h_mapDepth2Pedw2_HE->Fill(double(ieta), double(iphi), pedestalw2);
	  h_mapDepth2Pedw3_HE->Fill(double(ieta), double(iphi), pedestalw3);
	}
	if(mdepth==3) {
	  h_mapDepth3Ped0_HE->Fill(double(ieta), double(iphi), pedestal0);
	  h_mapDepth3Ped1_HE->Fill(double(ieta), double(iphi), pedestal1);
	  h_mapDepth3Ped2_HE->Fill(double(ieta), double(iphi), pedestal2);
	  h_mapDepth3Ped3_HE->Fill(double(ieta), double(iphi), pedestal3);
	  h_mapDepth3Pedw0_HE->Fill(double(ieta), double(iphi), pedestalw0);
	  h_mapDepth3Pedw1_HE->Fill(double(ieta), double(iphi), pedestalw1);
	  h_mapDepth3Pedw2_HE->Fill(double(ieta), double(iphi), pedestalw2);
	  h_mapDepth3Pedw3_HE->Fill(double(ieta), double(iphi), pedestalw3);
	}

	if(pedestalw0 < pedestalwHEMax_ || pedestalw1 < pedestalwHEMax_
	   || pedestalw2 < pedestalwHEMax_ || pedestalw3 < pedestalwHEMax_) {
	  if(mdepth==1) h_mapDepth1pedestalw_HE->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==2) h_mapDepth2pedestalw_HE->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==3) h_mapDepth3pedestalw_HE->Fill(double(ieta), double(iphi), 1.);
	}

	if(pedestal0 < pedestalHEMax_ || pedestal1 < pedestalHEMax_
	   || pedestal2 < pedestalHEMax_ || pedestal3 < pedestalHEMax_) {
	  if(mdepth==1) h_mapDepth1pedestal_HE->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==2) h_mapDepth2pedestal_HE->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==3) h_mapDepth3pedestal_HE->Fill(double(ieta), double(iphi), 1.);
	}

	for (int ii=0; ii<4; ii++) {
	  h_pedestal00_HE->Fill(pedestal00->getValue(ii),1.);
	  h_gain_HE->Fill(gain->getValue(ii),1.);
	}
	h_respcorr_HE->Fill(respcorr->getValue(),1.);
	h_timecorr_HE->Fill(timecorr->getValue(),1.);
	h_lutcorr_HE->Fill(lutcorr->getValue(),1.);

      }//
      
      //   //   //   //   //   //   //   //   //  HE       ADCAmpl:
      if(studyADCAmplHist_) {
	h_ADCAmpl_HE->Fill(amplitude,1.);
	h_ADCAmplZoom1_HE->Fill(amplitude,1.);
	if(amplitude < ADCAmplHEMin_  || amplitude > ADCAmplHEMax_) {
	  if(studyRunDependenceHist_ && flagtodefinebadchannel_==5) ++badchannels[sub-1][mdepth-1][ieta+41][iphi];// 0-82 ; 0-71
	  if(mdepth==1) h_mapDepth1ADCAmpl225_HE->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==2) h_mapDepth2ADCAmpl225_HE->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==3) h_mapDepth3ADCAmpl225_HE->Fill(double(ieta), double(iphi), 1.);
	  if (verbosity == -56) std::cout << "***BAD HE channels from ADCAmpl: "  <<" ieta= " << ieta <<" iphi= " << iphi <<" amplitude= " << amplitude << std::endl;
	}// if
	//	if(amplitude > 700.) averSIGNALoccupancy_HE += 1.;

	if(amplitude < 40.) {
	  if(mdepth==1) h_mapDepth1ADCAmpl225Copy_HE->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==2) h_mapDepth2ADCAmpl225Copy_HE->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==3) h_mapDepth3ADCAmpl225Copy_HE->Fill(double(ieta), double(iphi), 1.);
	}// if
	

	// for averaged values:
	if(mdepth==1) h_mapDepth1ADCAmpl_HE->Fill(double(ieta), double(iphi), amplitude);
	if(mdepth==2) h_mapDepth2ADCAmpl_HE->Fill(double(ieta), double(iphi), amplitude);
	if(mdepth==3) h_mapDepth3ADCAmpl_HE->Fill(double(ieta), double(iphi), amplitude);
	if(mdepth==1) h_mapDepth1ADCAmpl12_HE->Fill(double(ieta), double(iphi), ampl);
	if(mdepth==2) h_mapDepth2ADCAmpl12_HE->Fill(double(ieta), double(iphi), ampl);
	if(mdepth==3) h_mapDepth3ADCAmpl12_HE->Fill(double(ieta), double(iphi), ampl);

	if(amplitude > forallestimators_amplitude_bigger_) sumEstimator1[sub-1][mdepth-1][ieta+41][iphi] += amplitude;
      }//if(studyADCAmplHist_
      ///////////////////////////////
      //   //   //   //   //   //   //   //   //  HE       TSmean:
      if(studyTSmeanShapeHist_) {
	h_TSmeanA_HE->Fill(aveamplitude1,1.);
	if(aveamplitude1 < TSmeanHEMin_ || aveamplitude1 > TSmeanHEMax_ ) {
	  if(studyRunDependenceHist_ && flagtodefinebadchannel_==4) ++badchannels[sub-1][mdepth-1][ieta+41][iphi];// 0-82 ; 0-71
	  if(mdepth==1) h_mapDepth1TSmeanA225_HE->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==2) h_mapDepth2TSmeanA225_HE->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==3) h_mapDepth3TSmeanA225_HE->Fill(double(ieta), double(iphi), 1.);
	  if (verbosity == -55) std::cout << "***BAD HE channels from TSmeanA: "  <<" ieta= " << ieta <<" iphi= " << iphi <<" aveamplitude1= " << aveamplitude1 << std::endl;
	}// if
	// for averaged values:
	if(mdepth==1) h_mapDepth1TSmeanA_HE->Fill(double(ieta), double(iphi), aveamplitude1);
	if(mdepth==2) h_mapDepth2TSmeanA_HE->Fill(double(ieta), double(iphi), aveamplitude1);
	if(mdepth==3) h_mapDepth3TSmeanA_HE->Fill(double(ieta), double(iphi), aveamplitude1);

	if(amplitude > forallestimators_amplitude_bigger_) sumEstimator2[sub-1][mdepth-1][ieta+41][iphi] += aveamplitude1;
      }//if(studyTSmeanShapeHist_) {
      ///////////////////////////////
      //   //   //   //   //   //   //   //   //  HE       TSmax:
      if(studyTSmaxShapeHist_) {
	h_TSmaxA_HE->Fill(float(ts_with_max_signal),1.);
	if(ts_with_max_signal < TSpeakHEMin_ || ts_with_max_signal > TSpeakHEMax_ ) {
	  if(studyRunDependenceHist_ && flagtodefinebadchannel_==3) ++badchannels[sub-1][mdepth-1][ieta+41][iphi];// 0-82 ; 0-71
	  if(mdepth==1) h_mapDepth1TSmaxA225_HE->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==2) h_mapDepth2TSmaxA225_HE->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==3) h_mapDepth3TSmaxA225_HE->Fill(double(ieta), double(iphi), 1.);
	  if (verbosity == -55) std::cout << "***BAD HE channels from TSmaxA: "  <<" ieta= " << ieta <<" iphi= " << iphi <<" ts_with_max_signal= " << ts_with_max_signal << std::endl;
	}// if
	// for averaged values:
	if(mdepth==1) h_mapDepth1TSmaxA_HE->Fill(double(ieta),double(iphi),float(ts_with_max_signal));
	if(mdepth==2) h_mapDepth2TSmaxA_HE->Fill(double(ieta),double(iphi),float(ts_with_max_signal));
	if(mdepth==3) h_mapDepth3TSmaxA_HE->Fill(double(ieta),double(iphi),float(ts_with_max_signal));

	if(amplitude > forallestimators_amplitude_bigger_) sumEstimator3[sub-1][mdepth-1][ieta+41][iphi] += float(ts_with_max_signal);
      }//if(studyTSmaxShapeHist_) {
      ///////////////////////////////
      
      
      //   //   //   //   //   //   //   //   //  HE       RMS:
      if(studyRMSshapeHist_) {
	h_Amplitude_HE->Fill(rmsamplitude,1.);
	if(rmsamplitude < rmsHEMin_ || rmsamplitude > rmsHEMax_ ) {
	  if(studyRunDependenceHist_ && flagtodefinebadchannel_==2) ++badchannels[sub-1][mdepth-1][ieta+41][iphi];// 0-82 ; 0-71
	  if(mdepth==1) h_mapDepth1Amplitude225_HE->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==2) h_mapDepth2Amplitude225_HE->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==3) h_mapDepth3Amplitude225_HE->Fill(double(ieta), double(iphi), 1.);
	  if (verbosity == -54) std::cout << "***BAD HE channels from shape RMS:  " <<" ieta= " << ieta <<" iphi= " << iphi << std::endl;
	} 
	// for averaged values:
	if(mdepth==1) h_mapDepth1Amplitude_HE->Fill(double(ieta), double(iphi), rmsamplitude);    
	if(mdepth==2) h_mapDepth2Amplitude_HE->Fill(double(ieta), double(iphi), rmsamplitude);    
	if(mdepth==3) h_mapDepth3Amplitude_HE->Fill(double(ieta), double(iphi), rmsamplitude);  

	if(amplitude > forallestimators_amplitude_bigger_) sumEstimator4[sub-1][mdepth-1][ieta+41][iphi] += rmsamplitude;
      }//if(studyRMSshapeHist_)
      ///////////////////////////////
      
      
      //   //   //   //   //   //   //   //   //  HE       Ratio:
      if(studyRatioShapeHist_) {
	h_Ampl_HE->Fill(ratio,1.);
	if(ratio < ratioHEMin_ || ratio > ratioHEMax_  ) {
	  if(studyRunDependenceHist_ && flagtodefinebadchannel_==1) ++badchannels[sub-1][mdepth-1][ieta+41][iphi];// 0-82 ; 0-71
	  if(mdepth==1) h_mapDepth1Ampl047_HE->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==2) h_mapDepth2Ampl047_HE->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==3) h_mapDepth3Ampl047_HE->Fill(double(ieta), double(iphi), 1.);
	  // //
	  if (verbosity == -53) std::cout << "***BAD HE channels from shape Ratio:  " <<" ieta= " << ieta <<" iphi= " << iphi << std::endl;
	  if (verbosity == -51  ) std::cout << "***BAD HE channels from shape Ratio:  " <<" ieta= " << ieta <<" iphi= " << iphi <<" sub= " << sub <<" mdepth= " << mdepth <<" badchannels= " << badchannels[sub-1][mdepth-1][ieta+41][iphi] << std::endl;
	}
	// for averaged values:

	if(mdepth==1) h_mapDepth1Ampl_HE->Fill(double(ieta), double(iphi), ratio);    
	if(mdepth==2) h_mapDepth2Ampl_HE->Fill(double(ieta), double(iphi), ratio);    
	if(mdepth==3) h_mapDepth3Ampl_HE->Fill(double(ieta), double(iphi), ratio);  

	if(amplitude > forallestimators_amplitude_bigger_) sumEstimator5[sub-1][mdepth-1][ieta+41][iphi] += ratio;
      }//if(studyRatioShapeHist_) 
      ///////////////////////////////
      
      //   //   //   //   //   //   //   //   //  HE       DiffAmplitude:
      if(studyDiffAmplHist_) {
	if(mdepth==1) h_mapDepth1AmplE34_HE->Fill(double(ieta), double(iphi), amplitude);    
	if(mdepth==2) h_mapDepth2AmplE34_HE->Fill(double(ieta), double(iphi), amplitude);    
	if(mdepth==3) h_mapDepth3AmplE34_HE->Fill(double(ieta), double(iphi), amplitude);  
      }// if(studyDiffAmplHist_)     
      
      ///////////////////////////////    for HE All 
      if(mdepth==1) h_mapDepth1_HE->Fill(double(ieta), double(iphi), 1.);    
      if(mdepth==2) h_mapDepth2_HE->Fill(double(ieta), double(iphi), 1.);    
      if(mdepth==3) h_mapDepth3_HE->Fill(double(ieta), double(iphi), 1.);    
      
      
    }//if ( sub == 2 )
    //    
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VeRawAnalyzer::fillDigiAmplitudeHF(HFDigiCollection::const_iterator& digiItr)
{
    CaloSamples tool;  // TS
    
    HcalDetId cell(digiItr->id()); 
    int mdepth = cell.depth();
//    int iphi0  = cell.iphi();// 1-72
    int iphi  = cell.iphi()-1;// 0-71
//    int ieta0  = cell.ieta();//-41 +41 !=0
    int ieta  = cell.ieta();
    if(ieta > 0) ieta -= 1;//-41 +41
    int sub   = cell.subdet(); // (HFDigiCollection: 4-HF)
    if (verbosity == -23) std::cout << " fillDigiAmplitude HF    DIGI ->  "  << "sub, ieta, iphi, mdepth = "  <<  sub << ", " << ieta << ", " << iphi << ", " << mdepth << std::endl;
    // !!!!!!
    if (verbosity == -23) std::cout <<" fillDigiAmplitude  HF   Size of Digi "<<(*digiItr).size()<<std::endl;

    ///////////////////////////////////////Energy
    const HcalPedestal* pedestal00 = conditions->getPedestal(cell);
    const HcalGain* gain = conditions->getGain(cell);
    //  const HcalGainWidth* gainWidth = conditions->getGainWidth(cell);
    const HcalRespCorr* respcorr = conditions->getHcalRespCorr(cell);
    const HcalTimeCorr* timecorr = conditions->getHcalTimeCorr(cell);
    const HcalLUTCorr* lutcorr = conditions->getHcalLUTCorr(cell);
    // Energy: 
    //    HcalCalibrations calib = conditions->getHcalCalibrations(cell);
    const HcalQIECoder* channelCoder = conditions->getHcalCoder(cell);
    const HcalPedestalWidth* pedw = conditions->getPedestalWidth(cell);

    HcalCoderDb coder (*channelCoder, *shape);
    if( useADCfC_ ) coder.adc2fC(*digiItr,tool);

    if (verbosity == -51) std::cout << "coder done..." << std::endl;
    if (verbosity == -51) std::cout << "fillDigiAmplitudeHF    coder done..." << std::endl;
    double pedestalaver9 = 0.;
    double pedestalaver4 = 0.;
    double pedestal0 = 0.;
    double pedestal1 = 0.;
    double pedestal2 = 0.;
    double pedestal3 = 0.;
    double pedestalwaver9 = 0.;
    double pedestalwaver4 = 0.;
    double pedestalw0 = 0.;
    double pedestalw1 = 0.;
    double pedestalw2 = 0.;
    double pedestalw3 = 0.;
    double amplitude = 0.;
    double absamplitude = 0.;
    double ampl = 0.;
    double timew = 0.;
    double timeww = 0.;
    double max_signal = -100.;
    int ts_with_max_signal = -100;
    int c0=0;
    int c1=0;
    int c2=0;
    int c3=0;
    int c4=0;
    double errorBtype = 0.;  

    //    int TSsize = 10;
    int TSsize = 4;
    //     if((*digiItr).size() !=  4) std::cout << "TSsize HF != 4 and = " <<(*digiItr).size()<< std::endl;
    if((*digiItr).size() !=  TSsize) errorBtype = 1.; 
    TSsize=digiItr->size();
    
    ////// 
    for (int ii=0; ii<TSsize; ii++) {  
      //  for (int ii=0; ii<digiItr->size(); ii++) {  
      double ampldefault = 0.;
      double ampldefault0 = 0.;
      double ampldefault1 = 0.;
      double ampldefault2 = 0.;
      ampldefault0 = adc2fC[digiItr->sample(ii).adc()];// massive
      if( useADCfC_ ) ampldefault1 = tool[ii];//adcfC
      ampldefault2  = (*digiItr)[ii].adc();//ADCcounts
      if( useADCmassive_ ) {ampldefault = ampldefault0;}
      if( useADCfC_ ) {ampldefault = ampldefault1;}
      if( useADCcounts_ ) {ampldefault = ampldefault2;}

      int capid = ((*digiItr)[ii]).capid();
      //      double pedestal = calib.pedestal(capid);
      double pedestal = pedestal00->getValue(capid);
      double pedestalw= pedw->getSigma(capid,capid);

      if (verbosity == -82 && sub == 4) std::cout << "HF ii = " <<ii<< " massive = " <<ampldefault0<< " adcfC = " <<ampldefault1<< "  ADCcounts= " <<ampldefault2<< " pedestal = " << pedestal <<  " capid = " <<capid<< std::endl;

      if( usePedestalSubtraction_ ) ampldefault -=  pedestal; // pedestal subtraction
      
      tool[ii] = ampldefault;

      pedestalaver9 +=  pedestal;    
      pedestalwaver9 +=  pedestalw*pedestalw;    

      if(capid == 0 && c0 == 0) {
	c0++;   c4++;
	pedestalaver4 +=  pedestal;    
	pedestal0 =  pedestal;  
	pedestalw0 =  pedestalw;  
	pedestalwaver4 +=  pedestalw*pedestalw;    
      }

      if(capid == 1 && c1 == 0) {
	c1++;   c4++;
	pedestalaver4 +=  pedestal;    
	pedestal1 =  pedestal;    
	pedestalw1 =  pedestalw;  
	pedestalwaver4 +=  pedestalw*pedestalw;    
      }
      if(capid == 2 && c2 == 0) {
	c2++;   c4++;
	pedestalaver4 +=  pedestal;    
	pedestal2 =  pedestal;    
	pedestalw2 =  pedestalw;  
	pedestalwaver4 +=  pedestalw*pedestalw;    
      }
      if(capid == 3 && c3 == 0) {
	c3++;   c4++;
	pedestalaver4 +=  pedestal;    
	pedestal3 =  pedestal;    
 	pedestalw3 =  pedestalw;  
	pedestalwaver4 +=  pedestalw*pedestalw;    
     }
      
      if(max_signal < ampldefault ) {
	max_signal = ampldefault;
	ts_with_max_signal = ii;
      }
      amplitude+=ampldefault;// 
      absamplitude+=abs(ampldefault);// 
      ///////////////////////////////////

      if(flagcpuoptimization_== 0 ) {
	
	if(sub == 4 )  h_ADC_HF->Fill(ampldefault, 1.);
      //  
	if(sub == 4 && mdepth == 1 )  h_ADC_HFdepth1->Fill(ampldefault, 1.);
	if(sub == 4 && mdepth == 2 )  h_ADC_HFdepth2->Fill(ampldefault, 1.);
      if(ii == 0 ) {
	if(sub == 4 && mdepth == 1 )  h_ADC_HFdepth1_TS0->Fill(ampldefault, 1.);
	if(sub == 4 && mdepth == 2 )  h_ADC_HFdepth2_TS0->Fill(ampldefault, 1.);
      }
      if(ii == 1 ) {
	if(sub == 4 && mdepth == 1 )  h_ADC_HFdepth1_TS1->Fill(ampldefault, 1.);
	if(sub == 4 && mdepth == 2 )  h_ADC_HFdepth2_TS1->Fill(ampldefault, 1.);
      }
      if(ii == 2 ) {
	if(sub == 4 && mdepth == 1 )  h_ADC_HFdepth1_TS2->Fill(ampldefault, 1.);
	if(sub == 4 && mdepth == 2 )  h_ADC_HFdepth2_TS2->Fill(ampldefault, 1.);
      }
      if(ii == 3 ) {
	if(sub == 4 && mdepth == 1 )  h_ADC_HFdepth1_TS3->Fill(ampldefault, 1.);
	if(sub == 4 && mdepth == 2 )  h_ADC_HFdepth2_TS3->Fill(ampldefault, 1.);
      }
      if(ii == 4 ) {
	if(sub == 4 && mdepth == 1 )  h_ADC_HFdepth1_TS4->Fill(ampldefault, 1.);
	if(sub == 4 && mdepth == 2 )  h_ADC_HFdepth2_TS4->Fill(ampldefault, 1.);
      }
      if(ii == 5 ) {
	if(sub == 4 && mdepth == 1 )  h_ADC_HFdepth1_TS5->Fill(ampldefault, 1.);
	if(sub == 4 && mdepth == 2 )  h_ADC_HFdepth2_TS5->Fill(ampldefault, 1.);
      }
      if(ii == 6 ) {
	if(sub == 4 && mdepth == 1 )  h_ADC_HFdepth1_TS6->Fill(ampldefault, 1.);
	if(sub == 4 && mdepth == 2 )  h_ADC_HFdepth2_TS6->Fill(ampldefault, 1.);
      }
      if(ii == 7 ) {
	if(sub == 4 && mdepth == 1 )  h_ADC_HFdepth1_TS7->Fill(ampldefault, 1.);
	if(sub == 4 && mdepth == 2 )  h_ADC_HFdepth2_TS7->Fill(ampldefault, 1.);
      }
      if(ii == 8 ) {
	if(sub == 4 && mdepth == 1 )  h_ADC_HFdepth1_TS8->Fill(ampldefault, 1.);
	if(sub == 4 && mdepth == 2 )  h_ADC_HFdepth2_TS8->Fill(ampldefault, 1.);
      }
      if(ii == 9 ) {
	if(sub == 4 && mdepth == 1 )  h_ADC_HFdepth1_TS9->Fill(ampldefault, 1.);
	if(sub == 4 && mdepth == 2 )  h_ADC_HFdepth2_TS9->Fill(ampldefault, 1.);
      }

  //////      
	}     
      if (verbosity == -51) std::cout << "fillDigiAmplitudeHF  amplitude = " << amplitude << std::endl;
      timew += (ii+1)*abs(ampldefault);
      timeww += (ii+1)*ampldefault;
    }//for 1
    amplitudechannel[sub-1][mdepth-1][ieta+41][iphi] += amplitude;// 0-82 ; 0-71 HF

    pedestalaver9 /= TSsize;
    pedestalaver4 /= c4;
    //pow(pedestalwaver9/TSsize,0.5);
    pedestalwaver9 = sqrt(pedestalwaver9/TSsize);
    //pow(pedestalwaver4/c4,0.5);
    pedestalwaver4 = sqrt(pedestalwaver4/c4);
    
    
//    if (verbosity == -23) std::cout << std::endl << "*** E = " << ampl << "   ACD -> fC -> (gain ="<< calib.respcorrgain(0) << ") GeV (ped.subtracted)" << std::endl;
    // ------------ to get signal in TS: -2 max +1  ------------


    if(ts_with_max_signal > -1 && ts_with_max_signal < 4) ampl = tool[ts_with_max_signal];
    if(ts_with_max_signal+2 > -1 && ts_with_max_signal+2 < 4) ampl += tool[ts_with_max_signal+2];
    if(ts_with_max_signal+1 > -1 && ts_with_max_signal+1 < 4) ampl += tool[ts_with_max_signal+1];
    if(ts_with_max_signal-1 > -1 && ts_with_max_signal-1 < 4) ampl += tool[ts_with_max_signal-1];

    //  if(ts_with_max_signal+1 > -1 && ts_with_max_signal+1 < 10) ampl += tool[ts_with_max_signal+1];
    //  if(ts_with_max_signal-1 > -1 && ts_with_max_signal-1 < 10) ampl += tool[ts_with_max_signal-1];
    //  if(ts_with_max_signal-2 > -1 && ts_with_max_signal-2 < 10) ampl += tool[ts_with_max_signal-2];

    double ratio = 0.;
    //    if(amplallTS != 0.) ratio = ampl/amplallTS;
    if(amplitude != 0. ) ratio = ampl/amplitude;
    if (verbosity == -51 && (ratio<0. || ratio>1.02)) {
      
      std::cout << "HF ratio = " <<ratio<< " ampl ="<<ampl<<" amplitude ="<<amplitude<< " ts_with_max_signal ="<<ts_with_max_signal
                                 << " TSsize ="<<TSsize<< " max_signal ="<<max_signal<< std::endl;
      
      std::cout << "HF tool[ts_with_max_signal] = " << tool[ts_with_max_signal]  << "  tool[ts_with_max_signal+1]= " << tool[ts_with_max_signal+1]  
                << "  tool[ts_with_max_signal-1]= " << tool[ts_with_max_signal-1]  << "  tool[ts_with_max_signal-2]= " << 
                  tool[ts_with_max_signal-2]  << std::endl;
      
      std::cout << "HF tool[0] = " << tool[0]  << "  tool[1]= " << tool[1]  << "  tool[2]= " << tool[2]  << "  tool[3]= " << tool[3]  
                << "  tool[4]= " << tool[4]  << "  tool[5]= " << tool[5]  << "  tool[6]= " << tool[6]  << "  tool[7]= " << tool[7]  << 
                   "  tool[8]= " << tool[8]  << "  tool[9]= " << tool[9]  << std::endl;
    }

    if (ratio<0. || ratio>1.02) ratio = 0.;
    
    if (verbosity == -51) std::cout << "*HF ratio = " <<ratio<< " ampl ="<<ampl<< std::endl;

    
    double aveamplitude = 0.;
    double aveamplitudew = 0.;
    if(absamplitude >0 && timew >0)  aveamplitude = timew/absamplitude;// average_TS +1
    if(amplitude >0 && timeww >0)  aveamplitudew = timeww/amplitude;// average_TS +1

    double rmsamp = 0.;
    // and CapIdErrors:
    int error = 0;
    bool anycapid   =  true;
    bool anyer      =  false;
    bool anydv      =  true;
    int lastcapid=0;
    int capid=0;
    for (int ii=0; ii<TSsize; ii++) {  
      double aaaaaa = (ii+1)-aveamplitudew;
      double aaaaaa2 = aaaaaa*aaaaaa;
      double ampldefault = tool[ii];
      rmsamp+=(aaaaaa2*ampldefault);// fC
      capid = ((*digiItr)[ii]).capid();
      bool er    = (*digiItr)[ii].er();   // error
      bool dv    = (*digiItr)[ii].dv();  // valid data
      if(ii!=0 && ((lastcapid+1)%4)!=capid) { anycapid =  false; }
      //    std::cout << " ii = " << ii  << " capid = " << capid  << " ((lastcapid+1)%4) = " << ((lastcapid+1)%4)  << std::endl;
      lastcapid=capid;  
      if(er) {anyer =  true; }
      if(!dv) {anydv =  false; }
    }//for 2

    if( !anycapid || anyer || !anydv )    error = 1; 
    double rmsamplitude = 0.;
    if( (amplitude >0 && rmsamp >0) || (amplitude <0 && rmsamp <0))  rmsamplitude = sqrt( rmsamp/amplitude );
    //	  if (verbosity == -79 && mdepth==2 && ieta==40 && iphi == 50 && lscounterM1 == 76) std::cout <<"******************************** rmsamplitude= " << rmsamplitude <<" rmsamp= " << rmsamp <<" amplitude= " << amplitude << std::endl;

    double aveamplitude1 = aveamplitude-1;// means iTS=0-9, so bad is iTS=0 and 9

//    for (int ii=0; ii<   TSsize; ii++) {  
//      capid = ((*digiItr)[ii]).capid();
//      //    if( !anycapid) std::cout << "HF 111: ii = " << ii  << " capid = " << capid  << std::endl;// main item here
//      if( anyer) std::cout << "HF 222222: ii = " << ii  << " capid = " << capid  << std::endl;
//      if( !anydv) std::cout << "HF 333333333: ii = " << ii  << " capid = " << capid  << std::endl;
//    }
    // CapIdErrors end  /////////////////////////////////////////////////////////
    // AZ 1.10.2015:
    if(error == 1 ) {
      if(sub == 4 && mdepth == 1 )  h_Amplitude_forCapIdErrors_HF1->Fill(amplitude, 1.);
      if(sub == 4 && mdepth == 2 )  h_Amplitude_forCapIdErrors_HF2->Fill(amplitude, 1.);
    }
    if(error != 1 ) {
      if(sub == 4 && mdepth == 1 )  h_Amplitude_notCapIdErrors_HF1->Fill(amplitude, 1.);
      if(sub == 4 && mdepth == 2 )  h_Amplitude_notCapIdErrors_HF2->Fill(amplitude, 1.);
    }
    
    if ( sub == 4 ) {
      // bad_channels with C,A,W,P,pW,
      if( error == 1 ||
	  amplitude < ADCAmplHFMin_ || amplitude > ADCAmplHFMax_ ||
	  rmsamplitude < rmsHFMin_ || rmsamplitude > rmsHFMax_ ||
	  pedestal0 < pedestalHFMax_ || pedestal1 < pedestalHFMax_
	  || pedestal2 < pedestalHFMax_ || pedestal3 < pedestalHFMax_ ||
	  pedestalw0 < pedestalwHFMax_ || pedestalw1 < pedestalwHFMax_
	  || pedestalw2 < pedestalwHFMax_ || pedestalw3 < pedestalwHFMax_ 
	  
	  ) {
	for (int ii=0; ii<TSsize; ii++) {  
	  double ampldefault = tool[ii];
	  h_shape_bad_channels_HF->Fill(float(ii),ampldefault);
	  h_shape0_bad_channels_HF->Fill(float(ii),1.);
	}
      }
      // good_channels with C,A,W,P,pW,
      else {
	for (int ii=0; ii<TSsize; ii++) {  
	  double ampldefault = tool[ii];
	  h_shape_good_channels_HF->Fill(float(ii),ampldefault);
	  h_shape0_good_channels_HF->Fill(float(ii),1.);
	}
      }
    }// sub   HF
    


    ///////////////////////////////////////Digis : over all digiHits

    sum0Estimator[sub-1][mdepth-1][ieta+41][iphi] += 1.;

    //      for Error B-type
    sumEstimator6[sub-1][mdepth-1][ieta+41][iphi] += errorBtype;
    

    //      sumEstimator0[sub-1][mdepth-1][ieta+41][iphi] += pedestalw0;//Sig_Pedestals	
    sumEstimator0[sub-1][mdepth-1][ieta+41][iphi] += pedestal0;//    Pedestals	
    // HF
    if ( sub == 4 ) {
      //   //   //   //   //   //   //   //   //  HF      PedestalCorrelations :
      if(studyPedestalCorrelations_) {
//	double mypedestal  = pedestalaver9;
//	double mypedestalw = pedestalwaver9;
	double mypedestal  = pedestal0;
	double mypedestalw = pedestalw0;
	h2_pedvsampl_HF->Fill(mypedestal,amplitude);
	h2_pedwvsampl_HF->Fill(mypedestalw,amplitude);
	h_pedvsampl_HF->Fill(mypedestal,amplitude);
	h_pedwvsampl_HF->Fill(mypedestalw,amplitude);
	h_pedvsampl0_HF->Fill(mypedestal,1.);
	h_pedwvsampl0_HF->Fill(mypedestalw,1.);
      }//

      //   //   //   //   //   //   //   //   //  HF       Pedestals:
      if(studyPedestalsHist_) {
	h_pedestal0_HF->Fill(pedestal0,1.);
	h_pedestal1_HF->Fill(pedestal1,1.);
	h_pedestal2_HF->Fill(pedestal2,1.);
	h_pedestal3_HF->Fill(pedestal3,1.);
	h_pedestalaver4_HF->Fill(pedestalaver4,1.);
	h_pedestalaver9_HF->Fill(pedestalaver9,1.);
	h_pedestalw0_HF->Fill(pedestalw0,1.);
	h_pedestalw1_HF->Fill(pedestalw1,1.);
	h_pedestalw2_HF->Fill(pedestalw2,1.);
	h_pedestalw3_HF->Fill(pedestalw3,1.);
	h_pedestalwaver4_HF->Fill(pedestalwaver4,1.);
	h_pedestalwaver9_HF->Fill(pedestalwaver9,1.);
	// for averaged values:
	if(mdepth==1) {
	  h_mapDepth1Ped0_HF->Fill(double(ieta), double(iphi), pedestal0);
	  h_mapDepth1Ped1_HF->Fill(double(ieta), double(iphi), pedestal1);
	  h_mapDepth1Ped2_HF->Fill(double(ieta), double(iphi), pedestal2);
	  h_mapDepth1Ped3_HF->Fill(double(ieta), double(iphi), pedestal3);
	  h_mapDepth1Pedw0_HF->Fill(double(ieta), double(iphi), pedestalw0);
	  h_mapDepth1Pedw1_HF->Fill(double(ieta), double(iphi), pedestalw1);
	  h_mapDepth1Pedw2_HF->Fill(double(ieta), double(iphi), pedestalw2);
	  h_mapDepth1Pedw3_HF->Fill(double(ieta), double(iphi), pedestalw3);
	}
	if(mdepth==2) {
	  h_mapDepth2Ped0_HF->Fill(double(ieta), double(iphi), pedestal0);
	  h_mapDepth2Ped1_HF->Fill(double(ieta), double(iphi), pedestal1);
	  h_mapDepth2Ped2_HF->Fill(double(ieta), double(iphi), pedestal2);
	  h_mapDepth2Ped3_HF->Fill(double(ieta), double(iphi), pedestal3);
	  h_mapDepth2Pedw0_HF->Fill(double(ieta), double(iphi), pedestalw0);
	  h_mapDepth2Pedw1_HF->Fill(double(ieta), double(iphi), pedestalw1);
	  h_mapDepth2Pedw2_HF->Fill(double(ieta), double(iphi), pedestalw2);
	  h_mapDepth2Pedw3_HF->Fill(double(ieta), double(iphi), pedestalw3);
	}

	if(pedestalw0 < pedestalwHFMax_ || pedestalw1 < pedestalwHFMax_
	   || pedestalw2 < pedestalwHFMax_ || pedestalw3 < pedestalwHFMax_) {
	  if(mdepth==1) h_mapDepth1pedestalw_HF->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==2) h_mapDepth2pedestalw_HF->Fill(double(ieta), double(iphi), 1.);
	}

	if(pedestal0 < pedestalHFMax_ || pedestal1 < pedestalHFMax_
	   || pedestal2 < pedestalHFMax_ || pedestal3 < pedestalHFMax_) {
	  if(mdepth==1) h_mapDepth1pedestal_HF->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==2) h_mapDepth2pedestal_HF->Fill(double(ieta), double(iphi), 1.);
	}

	for (int ii=0; ii<4; ii++) {
	  h_pedestal00_HF->Fill(pedestal00->getValue(ii),1.);
	  h_gain_HF->Fill(gain->getValue(ii),1.);
	}
	h_respcorr_HF->Fill(respcorr->getValue(),1.);
	h_timecorr_HF->Fill(timecorr->getValue(),1.);
	h_lutcorr_HF->Fill(lutcorr->getValue(),1.);

      }//
      
      //   //   //   //   //   //   //   //   //  HF       ADCAmpl:
      if(studyADCAmplHist_) {
	h_ADCAmpl_HF->Fill(amplitude,1.);
	h_ADCAmplZoom1_HF->Fill(amplitude,1.);
	if(amplitude < ADCAmplHFMin_  || amplitude > ADCAmplHFMax_) {
	  if(studyRunDependenceHist_ && flagtodefinebadchannel_==5) ++badchannels[sub-1][mdepth-1][ieta+41][iphi];// 0-82 ; 0-71
	  if(mdepth==1) h_mapDepth1ADCAmpl225_HF->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==2) h_mapDepth2ADCAmpl225_HF->Fill(double(ieta), double(iphi), 1.);
	  if (verbosity == -51) std::cout << "***BAD HF channels from ADCAmpl: "  <<" ieta= " << ieta <<" iphi= " << iphi <<" amplitude= " << amplitude << std::endl;
	}// if
	//	if(amplitude >1500.) averSIGNALoccupancy_HF += 1.;
	if(amplitude < 20.) {
	  if(mdepth==1) h_mapDepth1ADCAmpl225Copy_HF->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==2) h_mapDepth2ADCAmpl225Copy_HF->Fill(double(ieta), double(iphi), 1.);
	}// if
	
	// for averaged values:
	if(mdepth==1) h_mapDepth1ADCAmpl_HF->Fill(double(ieta), double(iphi), amplitude);
	if(mdepth==2) h_mapDepth2ADCAmpl_HF->Fill(double(ieta), double(iphi), amplitude);
	if(mdepth==1) h_mapDepth1ADCAmpl12_HF->Fill(double(ieta), double(iphi), ampl);
	if(mdepth==2) h_mapDepth2ADCAmpl12_HF->Fill(double(ieta), double(iphi), ampl);
	
	if(amplitude > forallestimators_amplitude_bigger_) sumEstimator1[sub-1][mdepth-1][ieta+41][iphi] += amplitude;
	//		  	  if (verbosity == -7777 && sub==4 && mdepth==1&&ieta==-35&&iphi==35) std::cout << "***ADCAmpl: "  <<" ieta= " << ieta <<" iphi= " << iphi <<" aveamplitude1= " << aveamplitude1 <<" amplitude= " << amplitude << "sumEstimator1[sub-1][mdepth-1][ieta+41][iphi]= "  << sumEstimator1[sub-1][mdepth-1][ieta+41][iphi]  << std::endl;	  	  
	
//		if (verbosity == -7777 && sub==4 && mdepth==1) std::cout << "HFdepth1 Amplitude: "  <<" ieta= " << ieta <<" iphi= " << iphi <<" aveamplitude1= " << aveamplitude1 <<" amplitude= " << amplitude << "sumEstimator1[sub-1][mdepth-1][ieta+41][iphi]= "  << sumEstimator1[sub-1][mdepth-1][ieta+41][iphi]  << std::endl;
	  
	  }//if(studyADCAmplHist_
      ///////////////////////////////
      
      //   //   //   //   //   //   //   //   //  HF       TSmean:
      if(studyTSmeanShapeHist_) {
	h_TSmeanA_HF->Fill(aveamplitude1,1.);
	if(aveamplitude1 < TSmeanHFMin_ || aveamplitude1 > TSmeanHFMax_ ) {
	  if(studyRunDependenceHist_ && flagtodefinebadchannel_==4) ++badchannels[sub-1][mdepth-1][ieta+41][iphi];// 0-82 ; 0-71
	  if(mdepth==1) h_mapDepth1TSmeanA225_HF->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==2) h_mapDepth2TSmeanA225_HF->Fill(double(ieta), double(iphi), 1.);
	  if (verbosity == -51) std::cout << "***BAD HF channels from TSmeanA: "  <<" ieta= " << ieta <<" iphi= " << iphi <<" aveamplitude1= " << aveamplitude1 << std::endl;
	}// if
	// for averaged values:
	if(mdepth==1) h_mapDepth1TSmeanA_HF->Fill(double(ieta), double(iphi), aveamplitude1);
	if(mdepth==2) h_mapDepth2TSmeanA_HF->Fill(double(ieta), double(iphi), aveamplitude1);

	if(amplitude > forallestimators_amplitude_bigger_) sumEstimator2[sub-1][mdepth-1][ieta+41][iphi] += aveamplitude1;
      }//if(studyTSmeanShapeHist_
      ///////////////////////////////
      //   //   //   //   //   //   //   //   //  HF       TSmax:
      if(studyTSmaxShapeHist_) {
	h_TSmaxA_HF->Fill(float(ts_with_max_signal),1.);
	if(ts_with_max_signal < TSpeakHFMin_ || ts_with_max_signal > TSpeakHFMax_ ) {
	  if(studyRunDependenceHist_ && flagtodefinebadchannel_==3) ++badchannels[sub-1][mdepth-1][ieta+41][iphi];// 0-82 ; 0-71
	  if(mdepth==1) h_mapDepth1TSmaxA225_HF->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==2) h_mapDepth2TSmaxA225_HF->Fill(double(ieta), double(iphi), 1.);
	  if (verbosity == -51) std::cout << "***BAD HF channels from TSmaxA: "  <<" ieta= " << ieta <<" iphi= " << iphi <<" ts_with_max_signal= " << ts_with_max_signal << std::endl;
	}// if
	// for averaged values:
	if(mdepth==1) h_mapDepth1TSmaxA_HF->Fill(double(ieta),double(iphi), float(ts_with_max_signal));
	if(mdepth==2) h_mapDepth2TSmaxA_HF->Fill(double(ieta),double(iphi), float(ts_with_max_signal));

	if(amplitude > forallestimators_amplitude_bigger_) sumEstimator3[sub-1][mdepth-1][ieta+41][iphi] += float(ts_with_max_signal);
      }//if(studyTSmaxShapeHist_
      ///////////////////////////////
      
      
      //   //   //   //   //   //   //   //   //  HF       RMS:
      if(studyRMSshapeHist_) {
	h_Amplitude_HF->Fill(rmsamplitude,1.);
	if(rmsamplitude < rmsHFMin_ || rmsamplitude > rmsHFMax_ ) {
	  if(studyRunDependenceHist_ && flagtodefinebadchannel_==2) ++badchannels[sub-1][mdepth-1][ieta+41][iphi];// 0-82 ; 0-71
	  if(mdepth==1) h_mapDepth1Amplitude225_HF->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==2) h_mapDepth2Amplitude225_HF->Fill(double(ieta), double(iphi), 1.);
	  if (verbosity == -51) std::cout << "***BAD HF channels from shape RMS:  "  <<" ieta= " << ieta <<" iphi= " << iphi << std::endl;
	}// if
	// for averaged values:

	if(mdepth==1) h_mapDepth1Amplitude_HF->Fill(double(ieta), double(iphi), rmsamplitude);    
	if(mdepth==2) h_mapDepth2Amplitude_HF->Fill(double(ieta), double(iphi), rmsamplitude);    

	if(amplitude > forallestimators_amplitude_bigger_) sumEstimator4[sub-1][mdepth-1][ieta+41][iphi] += rmsamplitude;

	if (verbosity == -79 &&  mdepth==2 &&  nevcounter0 !=0 && ieta == -1 && iphi == 50 && lscounterM1 == 76) std::cout << "************>>>>>   nevcounter0=  " << nevcounter0 <<" lscounterM1= " << lscounterM1 <<" nevcounter= " << nevcounter <<" sumEstimator4= " << (double)sumEstimator4[sub-1][mdepth-1][ieta+41][iphi] << std::endl;
      }//if(studyRMSshapeHist_)
      ///////////////////////////////
      
      
      //   //   //   //   //   //   //   //   //  HF       Ratio:
      if(studyRatioShapeHist_) {
	h_Ampl_HF->Fill(ratio,1.);
	if(ratio < ratioHFMin_ || ratio > ratioHFMax_  ) {
	  if(studyRunDependenceHist_ && flagtodefinebadchannel_==1) ++badchannels[sub-1][mdepth-1][ieta+41][iphi];// 0-82 ; 0-71
	  if(mdepth==1) h_mapDepth1Ampl047_HF->Fill(double(ieta), double(iphi), 1.);
	  if(mdepth==2) h_mapDepth2Ampl047_HF->Fill(double(ieta), double(iphi), 1.);
	  // //
	  if (verbosity == -51) std::cout << "***BAD HF channels from shape Ratio:  " <<" ieta= " << ieta <<" iphi= " << iphi << std::endl;
	  if (verbosity == -51  ) std::cout << "***BAD HF channels from shape Ratio:  " <<" ieta= " << ieta <<" iphi= " << iphi 
					    <<" sub= " << sub <<" mdepth= " << mdepth <<" badchannels= " << badchannels[sub-1][mdepth-1][ieta+41][iphi] << std::endl;
	}//if(ratio
	// for averaged values:
	if(mdepth==1) h_mapDepth1Ampl_HF->Fill(double(ieta), double(iphi), ratio);    
	if(mdepth==2) h_mapDepth2Ampl_HF->Fill(double(ieta), double(iphi), ratio);    

	if(amplitude > forallestimators_amplitude_bigger_) sumEstimator5[sub-1][mdepth-1][ieta+41][iphi] += ratio;
      }//if(studyRatioShapeHist_)
      
      ///////////////////////////////
      
      //   //   //   //   //   //   //   //   //  HF      DiffAmplitude:
      if(studyDiffAmplHist_) {
	if(mdepth==1) h_mapDepth1AmplE34_HF->Fill(double(ieta), double(iphi), amplitude);    
	if(mdepth==2) h_mapDepth2AmplE34_HF->Fill(double(ieta), double(iphi), amplitude);    
      }// if(studyDiffAmplHist_)     
      
      
      ///////////////////////////////    for HF All 
      if(mdepth==1) h_mapDepth1_HF->Fill(double(ieta), double(iphi), 1.);    
      if(mdepth==2) h_mapDepth2_HF->Fill(double(ieta), double(iphi), 1.);    
      
      
    }//if ( sub == 4 )
    
    //    
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VeRawAnalyzer::fillDigiAmplitudeHO(HODigiCollection::const_iterator& digiItr)
{
    CaloSamples tool;  // TS
    
    HcalDetId cell(digiItr->id()); 
    int mdepth = cell.depth();
//    int iphi0  = cell.iphi();// 1-72
    int iphi  = cell.iphi()-1;// 0-71
//    int ieta0  = cell.ieta();//-41 +41 !=0
    int ieta  = cell.ieta();
    if(ieta > 0) ieta -= 1;//-41 +41
    int sub   = cell.subdet(); // (HODigiCollection: 3-HO)
    if (verbosity == -24) std::cout << " fillDigiAmplitude HO    DIGI ->  "  << "sub, ieta, iphi, mdepth = "  <<  sub << ", " << ieta << ", " << iphi << ", " << mdepth << std::endl;
    // !!!!!!
    if (verbosity == -24) std::cout <<" fillDigiAmplitude  HO   Size of Digi "<<(*digiItr).size()<<std::endl;
    


    ///////////////////////////////////////Energy
    const HcalPedestal* pedestal00 = conditions->getPedestal(cell);
    const HcalGain* gain = conditions->getGain(cell);
    //  const HcalGainWidth* gainWidth = conditions->getGainWidth(cell);
    const HcalRespCorr* respcorr = conditions->getHcalRespCorr(cell);
    const HcalTimeCorr* timecorr = conditions->getHcalTimeCorr(cell);
    const HcalLUTCorr* lutcorr = conditions->getHcalLUTCorr(cell);
    // Energy: 
    //    HcalCalibrations calib = conditions->getHcalCalibrations(cell);
    const HcalQIECoder* channelCoder = conditions->getHcalCoder(cell);
    const HcalPedestalWidth* pedw = conditions->getPedestalWidth(cell);
    HcalCoderDb coder (*channelCoder, *shape);
    if( useADCfC_ )  coder.adc2fC(*digiItr,tool);

    if (verbosity == -22) std::cout << "coder done..." << std::endl;
    if (verbosity == -22) std::cout << "fillDigiAmplitudeH0    coder done..." << std::endl;
    double pedestalaver9 = 0.;
    double pedestalaver4 = 0.;
    double pedestal0 = 0.;
    double pedestal1 = 0.;
    double pedestal2 = 0.;
    double pedestal3 = 0.;
    double pedestalwaver9 = 0.;
    double pedestalwaver4 = 0.;
    double pedestalw0 = 0.;
    double pedestalw1 = 0.;
    double pedestalw2 = 0.;
    double pedestalw3 = 0.;
    double amplitude = 0.;
    double absamplitude = 0.;
    double ampl = 0.;
    double timew = 0.;
    double timeww = 0.;
    double max_signal = -100.;
    int ts_with_max_signal = -100;
    int c0=0;
    int c1=0;
    int c2=0;
    int c3=0;
    int c4=0;
    double errorBtype = 0.;  

    //    int TSsize = 10;
    int TSsize = 10;
    //     if((*digiItr).size() !=  10) std::cout << "TSsize HO != 10 and = " <<(*digiItr).size()<< std::endl;
    if((*digiItr).size() !=  TSsize) errorBtype = 1.; 
    TSsize=digiItr->size();

    //////  
    //  for (int ii=0; ii<digiItr->size(); ii++) {  
    for (int ii=0; ii<TSsize; ii++) {  
      double ampldefault = 0.;
      double ampldefault0 = 0.;
      double ampldefault1 = 0.;
      double ampldefault2 = 0.;
      ampldefault0 = adc2fC[digiItr->sample(ii).adc()];// massive
      if( useADCfC_ ) ampldefault1 = tool[ii];//adcfC
      ampldefault2  = (*digiItr)[ii].adc();//ADCcounts
      if( useADCmassive_ ) {ampldefault = ampldefault0;}
      if( useADCfC_ ) {ampldefault = ampldefault1;}
      if( useADCcounts_ ) {ampldefault = ampldefault2;}

      int capid = ((*digiItr)[ii]).capid();
      //      double pedestal = calib.pedestal(capid);
      double pedestal = pedestal00->getValue(capid);
      double pedestalw= pedw->getSigma(capid,capid);

      if (verbosity == -83 && sub == 3) std::cout << "HO ii = " <<ii<< " massive = " <<ampldefault0<< " adcfC = " <<ampldefault1<< "  ADCcounts= " <<ampldefault2<< " pedestal = " << pedestal <<  " capid = " <<capid<< std::endl;


      if( usePedestalSubtraction_ ) ampldefault -=  pedestal; // pedestal subtraction
      
      tool[ii] = ampldefault;

      pedestalaver9 +=  pedestal;    
      pedestalwaver9 +=  pedestalw*pedestalw;    

      if(capid == 0 && c0 == 0) {
	c0++;   c4++;
	pedestalaver4 +=  pedestal;    
	pedestal0 =  pedestal;  
	pedestalw0 =  pedestalw;  
	pedestalwaver4 +=  pedestalw*pedestalw;    
      }

      if(capid == 1 && c1 == 0) {
	c1++;   c4++;
	pedestalaver4 +=  pedestal;    
	pedestal1 =  pedestal;    
	pedestalw1 =  pedestalw;  
	pedestalwaver4 +=  pedestalw*pedestalw;    
      }
      if(capid == 2 && c2 == 0) {
	c2++;   c4++;
	pedestalaver4 +=  pedestal;    
	pedestal2 =  pedestal;    
	pedestalw2 =  pedestalw;  
	pedestalwaver4 +=  pedestalw*pedestalw;    
      }
      if(capid == 3 && c3 == 0) {
	c3++;   c4++;
	pedestalaver4 +=  pedestal;    
	pedestal3 =  pedestal;    
 	pedestalw3 =  pedestalw;  
	pedestalwaver4 +=  pedestalw*pedestalw;    
     }
      
      if(max_signal < ampldefault ) {
	max_signal = ampldefault;
	ts_with_max_signal = ii;
      }
      amplitude+=ampldefault;
      absamplitude+=abs(ampldefault);


      ///////////////////////////////////////////
      if(flagcpuoptimization_== 0 ) {
	/*
      if(sub == 3 )  h_ADC_HO->Fill(ampldefault, 1.);
      //  
      if(sub == 3 && mdepth == 4 )  h_ADC_HOdepth4->Fill(ampldefault, 1.);
      
      if(ii == 0 ) {
	if(sub == 3 && mdepth == 4 )  h_ADC_HOdepth4_TS0->Fill(ampldefault, 1.);
      }
      if(ii == 1 ) {
	if(sub == 3 && mdepth == 4 )  h_ADC_HOdepth4_TS1->Fill(ampldefault, 1.);
      }
      if(ii == 2 ) {
	if(sub == 3 && mdepth == 4 )  h_ADC_HOdepth4_TS2->Fill(ampldefault, 1.);
      }
      if(ii == 3 ) {
	if(sub == 3 && mdepth == 4 )  h_ADC_HOdepth4_TS3->Fill(ampldefault, 1.);
      }
      if(ii == 4 ) {
	if(sub == 3 && mdepth == 4 )  h_ADC_HOdepth4_TS4->Fill(ampldefault, 1.);
      }
      if(ii == 5 ) {
	if(sub == 3 && mdepth == 4 )  h_ADC_HOdepth4_TS5->Fill(ampldefault, 1.);
      }
      if(ii == 6 ) {
	if(sub == 3 && mdepth == 4 )  h_ADC_HOdepth4_TS6->Fill(ampldefault, 1.);
      }
      if(ii == 7 ) {
	if(sub == 3 && mdepth == 4 )  h_ADC_HOdepth4_TS7->Fill(ampldefault, 1.);
      }
      if(ii == 8 ) {
	if(sub == 3 && mdepth == 4 )  h_ADC_HOdepth4_TS8->Fill(ampldefault, 1.);
      }
      if(ii == 9 ) {
	if(sub == 3 && mdepth == 4 )  h_ADC_HOdepth4_TS9->Fill(ampldefault, 1.);
      }
*/
  //////      
      }     
      
      if (verbosity == -22) std::cout << "fillDigiAmplitudeHO  amplitude = " << amplitude << std::endl;
      timew += (ii+1)*abs(ampldefault);
      timeww += (ii+1)*ampldefault;
    }//for 1
    amplitudechannel[sub-1][mdepth-1][ieta+41][iphi] += amplitude;// 0-82 ; 0-71  HO

    pedestalaver9 /= TSsize;
    pedestalaver4 /= c4;
    //pow(pedestalwaver9/TSsize,0.5);
    pedestalwaver9 = sqrt(pedestalwaver9/TSsize);
    //pow(pedestalwaver4/c4,0.5);
    pedestalwaver4 = sqrt(pedestalwaver4/c4);
    
    
//    if (verbosity == -24) std::cout << std::endl << "*** E = " << ampl << "   ACD -> fC -> (gain ="<< calib.respcorrgain(0) << ") GeV (ped.subtracted)" << std::endl;
    // ------------ to get signal in TS: -2 max +1  ------------

    if(ts_with_max_signal > -1 && ts_with_max_signal < 10) ampl = tool[ts_with_max_signal];
    if(ts_with_max_signal+2 > -1 && ts_with_max_signal+2 < 10) ampl += tool[ts_with_max_signal+2];
    if(ts_with_max_signal+1 > -1 && ts_with_max_signal+1 < 10) ampl += tool[ts_with_max_signal+1];
    if(ts_with_max_signal-1 > -1 && ts_with_max_signal-1 < 10) ampl += tool[ts_with_max_signal-1];

    //  if(ts_with_max_signal+1 > -1 && ts_with_max_signal+1 < 10) ampl += tool[ts_with_max_signal+1];
    //  if(ts_with_max_signal-1 > -1 && ts_with_max_signal-1 < 10) ampl += tool[ts_with_max_signal-1];
    //  if(ts_with_max_signal-2 > -1 && ts_with_max_signal-2 < 10) ampl += tool[ts_with_max_signal-2];

    double ratio = 0.;
    //    if(amplallTS != 0.) ratio = ampl/amplallTS;
    if(amplitude != 0. ) ratio = ampl/amplitude;
//    if (verbosity == -244 && (ratio<0. || ratio>1.02 || ratio==0.)) {
    if (verbosity == -244 && (ratio<0. || ratio>1.02 )) {
      
      std::cout << "HO ratio = " <<ratio<< " ampl ="<<ampl<<" amplitude ="<<amplitude<< " ts_with_max_signal ="<<ts_with_max_signal
                 << " TSsize ="<<TSsize<< " max_signal ="<<max_signal<< std::endl;
      
      std::cout << "HO tool[ts_with_max_signal] = " << tool[ts_with_max_signal]  << "  tool[ts_with_max_signal+1]= " << tool[ts_with_max_signal+1]  
                << "  tool[ts_with_max_signal-1]= " << tool[ts_with_max_signal-1]  << "  tool[ts_with_max_signal-2]= " << tool[ts_with_max_signal-2]  
                << std::endl;
      
      std::cout << "HO tool[0] = " << tool[0]  << "  tool[1]= " << tool[1]  << "  tool[2]= " << tool[2]  << "  tool[3]= " << tool[3]  << "  tool[4]= " 
                << tool[4]  << "  tool[5]= " << tool[5]  << "  tool[6]= " << tool[6]  << "  tool[7]= " << tool[7]  << "  tool[8]= " << tool[8]  << "  tool[9]= " 
                << tool[9]  << std::endl;
    }
    if (ratio<0. || ratio>1.04) ratio = 0.;
    
    if (verbosity == -24) std::cout << "*HO ratio = " <<ratio<< " ampl ="<<ampl<< std::endl;

    
    double aveamplitude = 0.;
    double aveamplitudew = 0.;
    if(absamplitude >0 && timew >0)  aveamplitude = timew/absamplitude;// average_TS +1
    if(amplitude >0 && timeww >0)  aveamplitudew = timeww/amplitude;// average_TS +1

    double rmsamp = 0.;
    // and CapIdErrors:
    int error = 0;
    bool anycapid   =  true;
    bool anyer      =  false;
    bool anydv      =  true;
    int lastcapid=0;
    int capid=0;
    for (int ii=0; ii<TSsize; ii++) {  
      double aaaaaa = (ii+1)-aveamplitudew;
      double aaaaaa2 = aaaaaa*aaaaaa;
      double ampldefault = tool[ii];
      rmsamp+=(aaaaaa2*ampldefault);// fC
      capid = ((*digiItr)[ii]).capid();
      bool er    = (*digiItr)[ii].er();   // error
      bool dv    = (*digiItr)[ii].dv();  // valid data
      if(ii!=0 && ((lastcapid+1)%4)!=capid) { anycapid =  false; }
      //    std::cout << " ii = " << ii  << " capid = " << capid  << " ((lastcapid+1)%4) = " << ((lastcapid+1)%4)  << std::endl;
      lastcapid=capid;  
      if(er) {anyer =  true; }
      if(!dv) {anydv =  false; }
    }//for 2

    if( !anycapid || anyer || !anydv )    error = 1; 
    double rmsamplitude = 0.;
    if( (amplitude >0 && rmsamp >0) || (amplitude <0 && rmsamp <0))  rmsamplitude = sqrt( rmsamp/amplitude );
    //	  if (verbosity == -79 && mdepth==2 && ieta==40 && iphi == 50 && lscounterM1 == 76) std::cout <<"******************************** rmsamplitude= " << rmsamplitude <<" rmsamp= " << rmsamp <<" amplitude= " << amplitude << std::endl;

    double aveamplitude1 = aveamplitude-1;// means iTS=0-9, so bad is iTS=0 and 9

//    for (int ii=0; ii<TSsize; ii++) {  
//      capid = ((*digiItr)[ii]).capid();
//      //    if( !anycapid) std::cout << "HO 111: ii = " << ii  << " capid = " << capid  << std::endl;// main item here
//      if( anyer) std::cout << "HO 222222: ii = " << ii  << " capid = " << capid  << std::endl;
//      if( !anydv) std::cout << "HO 333333333: ii = " << ii  << " capid = " << capid  << std::endl;
//    }
    // CapIdErrors end  /////////////////////////////////////////////////////////
    if(error == 1 ) {
      if(sub == 3 && mdepth == 4 )  h_Amplitude_forCapIdErrors_HO4->Fill(amplitude, 1.);
    }
    if(error != 1 ) {
      if(sub == 3 && mdepth == 4 )  h_Amplitude_notCapIdErrors_HO4->Fill(amplitude, 1.);
    }
    
    if ( sub == 3 ) {
      // bad_channels with C,A,W,P,pW,
      if( error == 1 ||
	  amplitude < ADCAmplHOMin_ || amplitude > ADCAmplHOMax_ ||
	  rmsamplitude < rmsHOMin_ || rmsamplitude > rmsHOMax_ ||
	  pedestal0 < pedestalHOMax_ || pedestal1 < pedestalHOMax_
	  || pedestal2 < pedestalHOMax_ || pedestal3 < pedestalHOMax_ ||
	  pedestalw0 < pedestalwHOMax_ || pedestalw1 < pedestalwHOMax_
	  || pedestalw2 < pedestalwHOMax_ || pedestalw3 < pedestalwHOMax_ 
	  
	  ) {
	for (int ii=0; ii<TSsize; ii++) {  
	  double ampldefault = tool[ii];
	  h_shape_bad_channels_HO->Fill(float(ii),ampldefault);
	  h_shape0_bad_channels_HO->Fill(float(ii),1.);
	}
      }
      // good_channels with C,A,W,P,pW,
      else {
	for (int ii=0; ii<TSsize; ii++) {  
	  double ampldefault = tool[ii];
	  h_shape_good_channels_HO->Fill(float(ii),ampldefault);
	  h_shape0_good_channels_HO->Fill(float(ii),1.);
	}
      }
    }// sub   HO
    
    
    
    ///////////////////////////////////////Digis : over all digiHits

    sum0Estimator[sub-1][mdepth-1][ieta+41][iphi] += 1.;

    //      for Error B-type
    sumEstimator6[sub-1][mdepth-1][ieta+41][iphi] += errorBtype;


    //      sumEstimator0[sub-1][mdepth-1][ieta+41][iphi] += pedestalw0;//Sig_Pedestals	
    sumEstimator0[sub-1][mdepth-1][ieta+41][iphi] += pedestal0;//Pedestals	
    // HO
    if ( sub == 3 ) {
      if(studyPedestalCorrelations_) {
//	double mypedestal  = pedestalaver9;
//	double mypedestalw = pedestalwaver9;
	double mypedestal  = pedestal0;
	double mypedestalw = pedestalw0;
	h2_pedvsampl_HO->Fill(mypedestal,amplitude);
	h2_pedwvsampl_HO->Fill(mypedestalw,amplitude);
	h_pedvsampl_HO->Fill(mypedestal,amplitude);
	h_pedwvsampl_HO->Fill(mypedestalw,amplitude);
	h_pedvsampl0_HO->Fill(mypedestal,1.);
	h_pedwvsampl0_HO->Fill(mypedestalw,1.);
      }//
      //   //   //   //   //   //   //   //   //  HO       Pedestals:
      if(studyPedestalsHist_) {
	h_pedestal0_HO->Fill(pedestal0,1.);
	h_pedestal1_HO->Fill(pedestal1,1.);
	h_pedestal2_HO->Fill(pedestal2,1.);
	h_pedestal3_HO->Fill(pedestal3,1.);
	h_pedestalaver4_HO->Fill(pedestalaver4,1.);
	h_pedestalaver9_HO->Fill(pedestalaver9,1.);
	h_pedestalw0_HO->Fill(pedestalw0,1.);
	h_pedestalw1_HO->Fill(pedestalw1,1.);
	h_pedestalw2_HO->Fill(pedestalw2,1.);
	h_pedestalw3_HO->Fill(pedestalw3,1.);
	h_pedestalwaver4_HO->Fill(pedestalwaver4,1.);
	h_pedestalwaver9_HO->Fill(pedestalwaver9,1.);
	// for averaged values:
	if(mdepth==4) {
	  h_mapDepth4Ped0_HO->Fill(double(ieta), double(iphi), pedestal0);
	  h_mapDepth4Ped1_HO->Fill(double(ieta), double(iphi), pedestal1);
	  h_mapDepth4Ped2_HO->Fill(double(ieta), double(iphi), pedestal2);
	  h_mapDepth4Ped3_HO->Fill(double(ieta), double(iphi), pedestal3);
	  h_mapDepth4Pedw0_HO->Fill(double(ieta), double(iphi), pedestalw0);
	  h_mapDepth4Pedw1_HO->Fill(double(ieta), double(iphi), pedestalw1);
	  h_mapDepth4Pedw2_HO->Fill(double(ieta), double(iphi), pedestalw2);
	  h_mapDepth4Pedw3_HO->Fill(double(ieta), double(iphi), pedestalw3);
	}

	if(pedestalw0 < pedestalwHOMax_ || pedestalw1 < pedestalwHOMax_
	   || pedestalw2 < pedestalwHOMax_ || pedestalw3 < pedestalwHOMax_) {
	  if(mdepth==4) h_mapDepth4pedestalw_HO->Fill(double(ieta), double(iphi), 1.);
	}

	if(pedestal0 < pedestalHOMax_ || pedestal1 < pedestalHOMax_
	   || pedestal2 < pedestalHOMax_ || pedestal3 < pedestalHOMax_) {
	  if(mdepth==4) h_mapDepth4pedestal_HO->Fill(double(ieta), double(iphi), 1.);
	}

	for (int ii=0; ii<4; ii++) {
	  h_pedestal00_HO->Fill(pedestal00->getValue(ii),1.);
	  h_gain_HO->Fill(gain->getValue(ii),1.);
	}
	h_respcorr_HO->Fill(respcorr->getValue(),1.);
	h_timecorr_HO->Fill(timecorr->getValue(),1.);
	h_lutcorr_HO->Fill(lutcorr->getValue(),1.);

      }//
      
      //   //   //   //   //   //   //   //   //  HO       ADCAmpl:
      if(studyADCAmplHist_) {
	h_ADCAmpl_HO->Fill(amplitude,1.);
	h_ADCAmplZoom1_HO->Fill(amplitude,1.);
	h_ADCAmpl_HO_copy->Fill(amplitude,1.);
	if(amplitude < ADCAmplHOMin_  || amplitude > ADCAmplHOMax_) {
	  if(studyRunDependenceHist_ && flagtodefinebadchannel_==5) ++badchannels[sub-1][mdepth-1][ieta+41][iphi];// 0-82 ; 0-71
	  if(mdepth==4) h_mapDepth4ADCAmpl225_HO->Fill(double(ieta), double(iphi), 1.);
	  if (verbosity == -56) std::cout << "***BAD HO channels from ADCAmpl: "  <<" ieta= " << ieta <<" iphi= " << iphi <<" amplitude= " << amplitude << std::endl;
	}// if
	//	if(amplitude >2000.) averSIGNALoccupancy_HO += 1.;

	if(amplitude < 100.) {
	  if(mdepth==4) h_mapDepth4ADCAmpl225Copy_HO->Fill(double(ieta), double(iphi), 1.);
	}// if
	

	// for averaged values:
	if(mdepth==4) h_mapDepth4ADCAmpl_HO->Fill(double(ieta), double(iphi), amplitude);
	if(mdepth==4) h_mapDepth4ADCAmpl12_HO->Fill(double(ieta), double(iphi), ampl);

	if(amplitude > forallestimators_amplitude_bigger_) sumEstimator1[sub-1][mdepth-1][ieta+41][iphi] += amplitude;
	/*
	// to avoid high amplitude peaks of tiny bad channels
	if(amplitude > forallestimators_amplitude_bigger_ && amplitude  <18000) {
	  if(amplitude>990&&amplitude<1020) {
	  }
	  else {
	    sumEstimator1[sub-1][mdepth-1][ieta+41][iphi] += amplitude;
	  }
	}
*/	


      }//if(studyADCAmplHist_
      ///////////////////////////////

      //   //   //   //   //   //   //   //   //  HO       TSmean:
      if(studyTSmeanShapeHist_) {
	h_TSmeanA_HO->Fill(aveamplitude1,1.);
	if(aveamplitude1 < TSmeanHOMin_ || aveamplitude1 > TSmeanHOMax_ ) {
	  if(studyRunDependenceHist_ && flagtodefinebadchannel_==4) ++badchannels[sub-1][mdepth-1][ieta+41][iphi];// 0-82 ; 0-71
	  if(mdepth==4) h_mapDepth4TSmeanA225_HO->Fill(double(ieta), double(iphi), 1.);
	  if (verbosity == -55) std::cout << "***BAD HO channels from TSmeanA: "  <<" ieta= " << ieta <<" iphi= " << iphi <<" aveamplitude1= " << aveamplitude1 << std::endl;
	}// if
	// for averaged values:
	if(mdepth==4) h_mapDepth4TSmeanA_HO->Fill(double(ieta), double(iphi), aveamplitude1);

	if(amplitude > forallestimators_amplitude_bigger_) sumEstimator2[sub-1][mdepth-1][ieta+41][iphi] += aveamplitude1;
      }//if(studyTSmeanShapeHist_
      ///////////////////////////////
      //   //   //   //   //   //   //   //   //  HO       TSmax:
      if(studyTSmaxShapeHist_) {
	h_TSmaxA_HO->Fill(float(ts_with_max_signal),1.);
	if(ts_with_max_signal < TSpeakHOMin_ || ts_with_max_signal > TSpeakHOMax_ ) {
	  if(studyRunDependenceHist_ && flagtodefinebadchannel_==3) ++badchannels[sub-1][mdepth-1][ieta+41][iphi];// 0-82 ; 0-71
	  if(mdepth==4) h_mapDepth4TSmaxA225_HO->Fill(double(ieta), double(iphi), 1.);
	  if (verbosity == -55) std::cout << "***BAD HO channels from TSmaxA: "  <<" ieta= " << ieta <<" iphi= " << iphi <<" ts_with_max_signal= " << ts_with_max_signal << std::endl;
	}// if
	// for averaged values:
	if(mdepth==4) h_mapDepth4TSmaxA_HO->Fill(double(ieta),double(iphi),float(ts_with_max_signal));

	if(amplitude > forallestimators_amplitude_bigger_) sumEstimator3[sub-1][mdepth-1][ieta+41][iphi] += float(ts_with_max_signal);
      }//if(studyTSmaxShapeHist_
      ///////////////////////////////
      
      
      //   //   //   //   //   //   //   //   //  HO       RMS:
      if(studyRMSshapeHist_) {
	h_Amplitude_HO->Fill(rmsamplitude,1.);
	if(rmsamplitude < rmsHOMin_ || rmsamplitude > rmsHOMax_ ) {
	  if(studyRunDependenceHist_ && flagtodefinebadchannel_==2) ++badchannels[sub-1][mdepth-1][ieta+41][iphi];// 0-82 ; 0-71
	  if(mdepth==4) h_mapDepth4Amplitude225_HO->Fill(double(ieta), double(iphi), 1.);
	  if (verbosity == -54) std::cout << "***BAD HO channels from shape RMS:  "  <<" ieta= " << ieta <<" iphi= " << iphi << std::endl;
	}// if
	// for averaged values:
	if(mdepth==4) h_mapDepth4Amplitude_HO->Fill(double(ieta), double(iphi), rmsamplitude);    

	if(amplitude > forallestimators_amplitude_bigger_) sumEstimator4[sub-1][mdepth-1][ieta+41][iphi] += rmsamplitude;
      }//if(studyRMSshapeHist_)
      ///////////////////////////////
      
      
      //   //   //   //   //   //   //   //   //  HO       Ratio:
      if(studyRatioShapeHist_) {
	h_Ampl_HO->Fill(ratio,1.);
	if(ratio < ratioHOMin_ || ratio > ratioHOMax_  ) {
	  if(studyRunDependenceHist_ && flagtodefinebadchannel_==1) ++badchannels[sub-1][mdepth-1][ieta+41][iphi];// 0-82 ; 0-71
	  if(mdepth==4) h_mapDepth4Ampl047_HO->Fill(double(ieta), double(iphi), 1.);
	  // //
	  if (verbosity == -53) std::cout << "***BAD HO channels from shape Ratio:  " <<" ieta= " << ieta <<" iphi= " << iphi << std::endl;
	  if (verbosity == -51  ) std::cout << "***BAD HO channels from shape Ratio:  " <<" ieta= " << ieta <<" iphi= " << iphi <<" sub= " 
					    << sub <<" mdepth= " << mdepth <<" badchannels= " << badchannels[sub-1][mdepth-1][ieta+41][iphi] << std::endl;
	}//if(ratio
	// for averaged values:
	if(mdepth==4) h_mapDepth4Ampl_HO->Fill(double(ieta), double(iphi), ratio);    

	if(amplitude > forallestimators_amplitude_bigger_) sumEstimator5[sub-1][mdepth-1][ieta+41][iphi] += ratio;
      }//if(studyRatioShapeHist_)
      
      ///////////////////////////////
      
      //   //   //   //   //   //   //   //   //  HO      DiffAmplitude:
      if(studyDiffAmplHist_) {
	if(mdepth==4) h_mapDepth4AmplE34_HO->Fill(double(ieta), double(iphi), amplitude);    
      }// if(studyDiffAmplHist_)     
      
      
      ///////////////////////////////    for HO All 
      if(mdepth==4) h_mapDepth4_HO->Fill(double(ieta), double(iphi), 1.);    
      
      
    }//if ( sub == 3 )
    
    //    
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------ method called to for each event  ------------
int VeRawAnalyzer::getRBX(int& kdet, int& keta, int& kphi){
  
  // Find the correspondance between ieta, iphi and RBX
  
  int cal_RBX=0;
  // HBHE
  if(kdet==1 || kdet==2) {
    if(kphi==71) cal_RBX=0;
    else cal_RBX=(kphi+1)/4;
    cal_RBX=cal_RBX+18*(keta+1)/2;
  }
  // HF
  if(kdet == 4 ){
    cal_RBX = (int)(kphi/18)+1;
  }
  // HO
  if(kdet==3){
    if(keta==-2){
      if(kphi==71) cal_RBX=0;
      else cal_RBX=kphi/12+1;
    }
    if(keta==-1){
      if(kphi==71) cal_RBX=6;
      else cal_RBX=kphi/12+1+6;
    }
    if(keta==0){
      if(kphi==71) cal_RBX=12;
      else cal_RBX=kphi/6+1+12;
    }
    if(keta==1){
      if(kphi==71) cal_RBX=24;
      else cal_RBX=kphi/12+1+24;
    }
    if(keta==2){
      if(kphi==71) cal_RBX=30;
      else cal_RBX=kphi/12+1+30;
    }
  }
  return cal_RBX;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------ other methods -----------------------------
void VeRawAnalyzer::beginRun( const edm::Run& r, const edm::EventSetup& iSetup)
{

}


void VeRawAnalyzer::endRun( const edm::Run& r, const edm::EventSetup& iSetup)
{
  /////////////////////////////////////////////////////////////////
  
  
  if (verbosity == -7778 ) std::cout << "endRUN: ls0 = " <<ls0 << " lumi = " <<lumi << " run0 = " <<run0 << " nevcounter0 = " <<nevcounter0 << " nevcounter = " <<nevcounter << std::endl;
  /////////////////////////////// -------------------------------------------------------------------to take into account last LS in Run

  if(usecontinuousnumbering_) { lscounterM1 = lscounter - 1; }
  else {lscounterM1 = ls0; }

  if(ls0 != -1) h_nevents_per_eachRealLS->Fill( float(lscounterM1) ,float(nevcounter) );//
  h_nevents_per_LS->Fill(float(nevcounter) );
  h_nevents_per_LSzoom->Fill(float(nevcounter) );
  
  nevcounter0 = nevcounter;
  if( nevcounter0 != 0 ) {
    ///////  int sub= cell.subdet();  1-HB, 2-HE, 3-HO, 4-HF
    ////////////            k0(sub): =0 HB; =1 HE; =2 HO; =3 HF;
    ////////////         k1(depth-1): = 0 - 3 or depth: = 1 - 4;
    for(int k0 = 0; k0<4; k0++) {
      for(int k1 = 0; k1<4; k1++) {
	for(int k2 = 0; k2<82; k2++) {
	  for(int k3 = 0; k3<72; k3++) {
	    //	    if(badchannels[k0][k1][k2][k3] !=0) ++nbadchannels;
	    int ieta = k2-41;
	    
	    // ------------------------------------------------------------------------------------------------------------------------sumEstimator0
	    if(sumEstimator0[k0][k1][k2][k3] != 0. ) {
	      
	      // fill histoes:
	      double bbbc=0.;
	      if(flagestimatornormalization_ == 0 ) bbbc = sumEstimator0[k0][k1][k2][k3]/nevcounter0;
	      if(flagestimatornormalization_ == 1 ) bbbc = sumEstimator0[k0][k1][k2][k3]/sum0Estimator[k0][k1][k2][k3];
	      double bbb1=1.;
	      if(flagestimatornormalization_ == 2 ) {bbbc = sumEstimator0[k0][k1][k2][k3]; bbb1 = sum0Estimator[k0][k1][k2][k3];}
	      
	      // HB:
	      if(k0==0) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumPedestalLS1->Fill(bbbc/bbb1);
		  h_2DsumPedestalLS1->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumPedestalLS1->Fill(double(ieta), double(k3), bbb1);
		  h_sumPedestalperLS1->Fill( float(lscounterM1) ,bbbc);
		  h_sum0PedestalperLS1->Fill( float(lscounterM1) ,bbb1);
		}
		if(k1+1  ==2) {
		  h_sumPedestalLS2->Fill(bbbc/bbb1);
		  h_2DsumPedestalLS2->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumPedestalLS2->Fill(double(ieta), double(k3), bbb1);
		  h_sumPedestalperLS2->Fill( float(lscounterM1) ,bbbc);
		  h_sum0PedestalperLS2->Fill( float(lscounterM1) ,bbb1);
		}
	      }
	      // HE:
	      if(k0==1) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumPedestalLS3->Fill(bbbc/bbb1);
		  h_2DsumPedestalLS3->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumPedestalLS3->Fill(double(ieta), double(k3), bbb1);
		  h_sumPedestalperLS3->Fill( float(lscounterM1) ,bbbc);
		  h_sum0PedestalperLS3->Fill( float(lscounterM1) ,bbb1);
		}
		if(k1+1  ==2) {
		  h_sumPedestalLS4->Fill(bbbc/bbb1);
		  h_2DsumPedestalLS4->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumPedestalLS4->Fill(double(ieta), double(k3), bbb1);
		  h_sumPedestalperLS4->Fill( float(lscounterM1) ,bbbc);
		  h_sum0PedestalperLS4->Fill( float(lscounterM1) ,bbb1);
		}
		if(k1+1  ==3) {
		  h_sumPedestalLS5->Fill(bbbc/bbb1);
		  h_2DsumPedestalLS5->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumPedestalLS5->Fill(double(ieta), double(k3), bbb1);
		  h_sumPedestalperLS5->Fill( float(lscounterM1) ,bbbc);
		  h_sum0PedestalperLS5->Fill( float(lscounterM1) ,bbb1);
		}
	      }
	      // HF:
	      if(k0==3) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumPedestalLS6->Fill(bbbc/bbb1);
		  h_2DsumPedestalLS6->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumPedestalLS6->Fill(double(ieta), double(k3), bbb1);
		  h_sumPedestalperLS6->Fill( float(lscounterM1) ,bbbc);
		  h_sum0PedestalperLS6->Fill( float(lscounterM1) ,bbb1);
		}
		if(k1+1  ==2) {
		  h_sumPedestalLS7->Fill(bbbc/bbb1);
		  h_2DsumPedestalLS7->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumPedestalLS7->Fill(double(ieta), double(k3), bbb1);
		  h_sumPedestalperLS7->Fill( float(lscounterM1) ,bbbc);
		  h_sum0PedestalperLS7->Fill( float(lscounterM1) ,bbb1);
		}
	      }
	      // HO:
	      if(k0==2) {
		// HBdepth1
		if(k1+1  ==4) {
		  h_sumPedestalLS8->Fill(bbbc/bbb1);
		  h_2DsumPedestalLS8->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumPedestalLS8->Fill(double(ieta), double(k3), bbb1);
		  h_sumPedestalperLS8->Fill( float(lscounterM1) ,bbbc);
		  h_sum0PedestalperLS8->Fill( float(lscounterM1) ,bbb1);
		}
	      }
	    }//if(sumEstimator0[k0][k1][k2][k3] != 0.
	    
	    // ------------------------------------------------------------------------------------------------------------------------sumEstimator1
	    if(sumEstimator1[k0][k1][k2][k3] != 0. ) {
	      
	      // fill histoes:
	      double bbbc=0.;
	      if(flagestimatornormalization_ == 0 ) bbbc = sumEstimator1[k0][k1][k2][k3]/nevcounter0;
	      if(flagestimatornormalization_ == 1 ) bbbc = sumEstimator1[k0][k1][k2][k3]/sum0Estimator[k0][k1][k2][k3];
	      double bbb1=1.;
	      if(flagestimatornormalization_ == 2 ) {bbbc = sumEstimator1[k0][k1][k2][k3]; bbb1 = sum0Estimator[k0][k1][k2][k3];}
	      
	      // HB:
	      if(k0==0) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumADCAmplLS1copy1->Fill(bbbc/bbb1);
		  h_sumADCAmplLS1copy2->Fill(bbbc/bbb1);
		  h_sumADCAmplLS1copy3->Fill(bbbc/bbb1);
		  h_sumADCAmplLS1copy4->Fill(bbbc/bbb1);
		  h_sumADCAmplLS1copy5->Fill(bbbc/bbb1);
		  h_sumADCAmplLS1->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 > lsdep_estimator1_HBdepth1_ ) h_2DsumADCAmplLS1->Fill(double(ieta), double(k3), bbbc);
		  if(bbbc/bbb1 > 2.*lsdep_estimator1_HBdepth1_ ) h_2DsumADCAmplLS1_LSselected->Fill(double(ieta), double(k3), bbbc);
//		  if( ( lscounterM1 == 734 || lscounterM1 == 758 || lscounterM1 == 759 || lscounterM1 == 853 || lscounterM1 == 1436 ) ) h_2DsumADCAmplLS1_LSselected->Fill(double(ieta), double(k3), bbbc);
		  //		  if( ( lscounterM1 == 504 || lscounterM1 == 867 ) ) h_2DsumADCAmplLS1_LSselected->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumADCAmplLS1->Fill(double(ieta), double(k3), bbb1);
		  h_sumADCAmplperLS1->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 > lsdep_estimator1_HBdepth1_ ) h_sumCutADCAmplperLS1->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0ADCAmplperLS1->Fill( float(lscounterM1) ,bbb1);

		}
		if(k1+1  ==2) {
		  h_sumADCAmplLS2->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 > lsdep_estimator1_HBdepth2_  ) h_2DsumADCAmplLS2->Fill(double(ieta), double(k3), bbbc);
		  if(bbbc/bbb1 > 2.*lsdep_estimator1_HBdepth2_  ) h_2DsumADCAmplLS2_LSselected->Fill(double(ieta), double(k3), bbbc);
//		  if( ( lscounterM1 == 734 || lscounterM1 == 758 || lscounterM1 == 759 || lscounterM1 == 853 || lscounterM1 == 1436 ) ) h_2DsumADCAmplLS2_LSselected->Fill(double(ieta), double(k3), bbbc);
		  //		  if( ( lscounterM1 == 504 || lscounterM1 == 867 ) ) h_2DsumADCAmplLS2_LSselected->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumADCAmplLS2->Fill(double(ieta), double(k3), bbb1);
		  h_sumADCAmplperLS2->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 > lsdep_estimator1_HBdepth2_  ) h_sumCutADCAmplperLS2->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0ADCAmplperLS2->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	      // HE:
	      if(k0==1) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumADCAmplLS3->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 > lsdep_estimator1_HEdepth1_  ) h_2DsumADCAmplLS3->Fill(double(ieta), double(k3), bbbc);
		  if(bbbc/bbb1 > 2.*lsdep_estimator1_HEdepth1_  ) h_2DsumADCAmplLS3_LSselected->Fill(double(ieta), double(k3), bbbc);
		  //		    if( ( lscounterM1 == 146 || lscounterM1 == 515 ) ) h_2DsumADCAmplLS3_LSselected->Fill(double(ieta), double(k3), bbbc);
		  //		    if( ( lscounterM1 == 504 || lscounterM1 == 867 ) ) h_2DsumADCAmplLS3_LSselected->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumADCAmplLS3->Fill(double(ieta), double(k3), bbb1);
		  h_sumADCAmplperLS3->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 > lsdep_estimator1_HEdepth1_  ) h_sumCutADCAmplperLS3->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0ADCAmplperLS3->Fill( float(lscounterM1) ,bbb1);

		}
		if(k1+1  ==2) {
		  h_sumADCAmplLS4->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 > lsdep_estimator1_HEdepth2_  ) h_2DsumADCAmplLS4->Fill(double(ieta), double(k3), bbbc);
		  if(bbbc/bbb1 > 2.*lsdep_estimator1_HEdepth2_  ) h_2DsumADCAmplLS4_LSselected->Fill(double(ieta), double(k3), bbbc);
		  //		    if( ( lscounterM1 == 146 || lscounterM1 == 515 ) ) h_2DsumADCAmplLS4_LSselected->Fill(double(ieta), double(k3), bbbc);
		  //		    if( ( lscounterM1 == 504 || lscounterM1 == 867 ) ) h_2DsumADCAmplLS4_LSselected->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumADCAmplLS4->Fill(double(ieta), double(k3), bbb1);
		  h_sumADCAmplperLS4->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 > lsdep_estimator1_HEdepth2_  ) h_sumCutADCAmplperLS4->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0ADCAmplperLS4->Fill( float(lscounterM1) ,bbb1);

		}
		if(k1+1  ==3) {
		  h_sumADCAmplLS5->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 > lsdep_estimator1_HEdepth3_  ) h_2DsumADCAmplLS5->Fill(double(ieta), double(k3), bbbc);
		  if(bbbc/bbb1 > 2.*lsdep_estimator1_HEdepth3_  ) h_2DsumADCAmplLS5_LSselected->Fill(double(ieta), double(k3), bbbc);
		  //		    if( ( lscounterM1 == 146 || lscounterM1 == 515 ) ) h_2DsumADCAmplLS5_LSselected->Fill(double(ieta), double(k3), bbbc);
		  //		    if( ( lscounterM1 == 504 || lscounterM1 == 867 ) ) h_2DsumADCAmplLS5_LSselected->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumADCAmplLS5->Fill(double(ieta), double(k3), bbb1);
		  h_sumADCAmplperLS5->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 > lsdep_estimator1_HEdepth3_  ) h_sumCutADCAmplperLS5->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0ADCAmplperLS5->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	      // HF:
	      if(k0==3) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumADCAmplLS6->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 > lsdep_estimator1_HFdepth1_  ) h_2DsumADCAmplLS6->Fill(double(ieta), double(k3), bbbc);
		  if(bbbc/bbb1 > 2.*lsdep_estimator1_HFdepth1_  ) h_2DsumADCAmplLS6_LSselected->Fill(double(ieta), double(k3), bbbc);
		  //		    if( ( lscounterM1 == 1810 ) ) h_2DsumADCAmplLS6_LSselected->Fill(double(ieta), double(k3), bbbc);
		  //		    if( ( lscounterM1 == 504 || lscounterM1 == 867 ) ) h_2DsumADCAmplLS6_LSselected->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumADCAmplLS6->Fill(double(ieta), double(k3), bbb1);
		  h_sumADCAmplperLS6->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 > lsdep_estimator1_HFdepth1_  ) h_sumCutADCAmplperLS6->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0ADCAmplperLS6->Fill( float(lscounterM1) ,bbb1);

		}
		if(k1+1  ==2) {
		  h_sumADCAmplLS7->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 > lsdep_estimator1_HFdepth2_  ) h_2DsumADCAmplLS7->Fill(double(ieta), double(k3), bbbc);
		  if(bbbc/bbb1 > 2.*lsdep_estimator1_HFdepth2_  ) h_2DsumADCAmplLS7_LSselected->Fill(double(ieta), double(k3), bbbc);
		  //		    if( ( lscounterM1 == 1810 ) ) h_2DsumADCAmplLS7_LSselected->Fill(double(ieta), double(k3), bbbc);
		  //		    if( ( lscounterM1 == 504 || lscounterM1 == 867 ) ) h_2DsumADCAmplLS7_LSselected->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumADCAmplLS7->Fill(double(ieta), double(k3), bbb1);
		  h_sumADCAmplperLS7->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 > lsdep_estimator1_HFdepth2_  ) h_sumCutADCAmplperLS7->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0ADCAmplperLS7->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	      // HO:
	      if(k0==2) {
		// HBdepth1
		if(k1+1  ==4) {
		  h_sumADCAmplLS8->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 > lsdep_estimator1_HOdepth4_  ) h_2DsumADCAmplLS8->Fill(double(ieta), double(k3), bbbc);
		  if(bbbc/bbb1 > 2.*lsdep_estimator1_HOdepth4_  ) h_2DsumADCAmplLS8_LSselected->Fill(double(ieta), double(k3), bbbc);
		  //		  if( ( lscounterM1 == 14 ) ) h_2DsumADCAmplLS8_LSselected->Fill(double(ieta), double(k3), bbbc);
		  //		  if( ( lscounterM1 == 504 || lscounterM1 == 867 ) ) h_2DsumADCAmplLS8_LSselected->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumADCAmplLS8->Fill(double(ieta), double(k3), bbb1);
		  h_sumADCAmplperLS8->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 > lsdep_estimator1_HOdepth4_  ) h_sumCutADCAmplperLS8->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0ADCAmplperLS8->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	    }//if(sumEstimator1[k0][k1][k2][k3] != 0.


	    // ------------------------------------------------------------------------------------------------------------------------sumEstimator2 Tn
	    if(sumEstimator2[k0][k1][k2][k3] != 0. ) {

	      // fill histoes:
	      double bbbc=0.;
	      if(flagestimatornormalization_ == 0 ) bbbc = sumEstimator2[k0][k1][k2][k3]/nevcounter0;
	      if(flagestimatornormalization_ == 1 ) bbbc = sumEstimator2[k0][k1][k2][k3]/sum0Estimator[k0][k1][k2][k3];
	      double bbb1=1.;
	      if(flagestimatornormalization_ == 2 ) {bbbc = sumEstimator2[k0][k1][k2][k3]; bbb1 = sum0Estimator[k0][k1][k2][k3];}
	      
	      // HB:
	      if(k0==0) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumTSmeanALS1->Fill(bbbc/bbb1);
		  if (verbosity == -2345 ) std::cout << "sumEstimator2 Tn: bbbc/bbb1 = " <<bbbc/bbb1 << " lsdep_estimator2_HBdepth1_ = " <<lsdep_estimator2_HBdepth1_ << " bbbc = " <<bbbc << " bbb1 = " <<bbb1 << std::endl;
		  if(bbbc/bbb1 > lsdep_estimator2_HBdepth1_  )h_2DsumTSmeanALS1->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmeanALS1->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmeanAperLS1->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >  lsdep_estimator2_HBdepth1_  ) h_sumCutTSmeanAperLS1->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmeanAperLS1->Fill( float(lscounterM1) ,bbb1);
		  if(bbbc/bbb1 >  2.*lsdep_estimator2_HBdepth1_  ) h_sumTSmeanAperLS1_LSselected->Fill( float(lscounterM1) ,bbbc); 
//		  if( ( lscounterM1 == 734 || lscounterM1 == 758 || lscounterM1 == 759 || lscounterM1 == 853 || lscounterM1 == 1436 ) ) h_sumTSmeanAperLS1_LSselected->Fill( float(lscounterM1) ,bbbc); 
		  //		  if( ( lscounterM1 == 504 || lscounterM1 == 867 ) ) h_sumTSmeanAperLS1_LSselected->Fill( float(lscounterM1) ,bbbc); 

		}
		if(k1+1  ==2) {
		  h_sumTSmeanALS2->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 >  lsdep_estimator2_HBdepth2_  )h_2DsumTSmeanALS2->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmeanALS2->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmeanAperLS2->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >   lsdep_estimator2_HBdepth2_  ) h_sumCutTSmeanAperLS2->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmeanAperLS2->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	      // HE:
	      if(k0==1) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumTSmeanALS3->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 >  lsdep_estimator2_HEdepth1_  )h_2DsumTSmeanALS3->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmeanALS3->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmeanAperLS3->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >   lsdep_estimator2_HEdepth1_  ) h_sumCutTSmeanAperLS3->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmeanAperLS3->Fill( float(lscounterM1) ,bbb1);

		}
		if(k1+1  ==2) {
		  h_sumTSmeanALS4->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 >  lsdep_estimator2_HEdepth2_  )h_2DsumTSmeanALS4->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmeanALS4->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmeanAperLS4->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >   lsdep_estimator2_HEdepth2_  ) h_sumCutTSmeanAperLS4->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmeanAperLS4->Fill( float(lscounterM1) ,bbb1);

		}
		if(k1+1  ==3) {
		  h_sumTSmeanALS5->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 >  lsdep_estimator2_HEdepth3_  )h_2DsumTSmeanALS5->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmeanALS5->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmeanAperLS5->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >   lsdep_estimator2_HEdepth3_  ) h_sumCutTSmeanAperLS5->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmeanAperLS5->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	      // HF:
	      if(k0==3) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumTSmeanALS6->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 >  lsdep_estimator2_HFdepth1_  )h_2DsumTSmeanALS6->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmeanALS6->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmeanAperLS6->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >   lsdep_estimator2_HFdepth1_  ) h_sumCutTSmeanAperLS6->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmeanAperLS6->Fill( float(lscounterM1) ,bbb1);

		}
		if(k1+1  ==2) {
		  h_sumTSmeanALS7->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 >  lsdep_estimator2_HFdepth2_  )h_2DsumTSmeanALS7->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmeanALS7->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmeanAperLS7->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >   lsdep_estimator2_HFdepth2_  ) h_sumCutTSmeanAperLS7->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmeanAperLS7->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	      // HO:
	      if(k0==2) {
		// HBdepth1
		if(k1+1  ==4) {
		  h_sumTSmeanALS8->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 >  lsdep_estimator2_HOdepth4_  )h_2DsumTSmeanALS8->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmeanALS8->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmeanAperLS8->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >   lsdep_estimator2_HOdepth4_  ) h_sumCutTSmeanAperLS8->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmeanAperLS8->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	    }//if(sumEstimator2[k0][k1][k2][k3] != 0.


	    // ------------------------------------------------------------------------------------------------------------------------sumEstimator3 Tx
	    if(sumEstimator3[k0][k1][k2][k3] != 0. ) {

	      // fill histoes:
	      double bbbc=0.;
	      if(flagestimatornormalization_ == 0 ) bbbc = sumEstimator3[k0][k1][k2][k3]/nevcounter0;
	      if(flagestimatornormalization_ == 1 ) bbbc = sumEstimator3[k0][k1][k2][k3]/sum0Estimator[k0][k1][k2][k3];
	      double bbb1=1.;
	      if(flagestimatornormalization_ == 2 ) {bbbc = sumEstimator3[k0][k1][k2][k3]; bbb1 = sum0Estimator[k0][k1][k2][k3];}
	      
	      // HB:
	      if(k0==0) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumTSmaxALS1->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 > lsdep_estimator3_HBdepth1_  )h_2DsumTSmaxALS1->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmaxALS1->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmaxAperLS1->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >  lsdep_estimator3_HBdepth1_  ) h_sumCutTSmaxAperLS1->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmaxAperLS1->Fill( float(lscounterM1) ,bbb1);
		  if(bbbc/bbb1 >  2.*lsdep_estimator3_HBdepth1_  ) h_sumTSmaxAperLS1_LSselected->Fill( float(lscounterM1) ,bbbc); 
//		  if( ( lscounterM1 == 734 || lscounterM1 == 758 || lscounterM1 == 759 || lscounterM1 == 853 || lscounterM1 == 1436 ) ) h_sumTSmaxAperLS1_LSselected->Fill( float(lscounterM1) ,bbbc); 
		  //		  if( ( lscounterM1 == 504 || lscounterM1 == 867 ) ) h_sumTSmaxAperLS1_LSselected->Fill( float(lscounterM1) ,bbbc); 

		}
		if(k1+1  ==2) {
		  h_sumTSmaxALS2->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 >  lsdep_estimator3_HBdepth2_  )h_2DsumTSmaxALS2->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmaxALS2->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmaxAperLS2->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >   lsdep_estimator3_HBdepth2_  ) h_sumCutTSmaxAperLS2->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmaxAperLS2->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	      // HE:
	      if(k0==1) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumTSmaxALS3->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 >  lsdep_estimator3_HEdepth1_  )h_2DsumTSmaxALS3->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmaxALS3->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmaxAperLS3->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >   lsdep_estimator3_HEdepth1_  ) h_sumCutTSmaxAperLS3->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmaxAperLS3->Fill( float(lscounterM1) ,bbb1);

		}
		if(k1+1  ==2) {
		  h_sumTSmaxALS4->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 >  lsdep_estimator3_HEdepth2_  )h_2DsumTSmaxALS4->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmaxALS4->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmaxAperLS4->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >   lsdep_estimator3_HEdepth2_  ) h_sumCutTSmaxAperLS4->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmaxAperLS4->Fill( float(lscounterM1) ,bbb1);

		}
		if(k1+1  ==3) {
		  h_sumTSmaxALS5->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 >  lsdep_estimator3_HEdepth3_  )h_2DsumTSmaxALS5->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmaxALS5->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmaxAperLS5->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >   lsdep_estimator3_HEdepth3_  ) h_sumCutTSmaxAperLS5->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmaxAperLS5->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	      // HF:
	      if(k0==3) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumTSmaxALS6->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 >  lsdep_estimator3_HFdepth1_  )h_2DsumTSmaxALS6->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmaxALS6->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmaxAperLS6->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >   lsdep_estimator3_HFdepth1_  ) h_sumCutTSmaxAperLS6->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmaxAperLS6->Fill( float(lscounterM1) ,bbb1);

		}
		if(k1+1  ==2) {
		  h_sumTSmaxALS7->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 >  lsdep_estimator3_HFdepth2_  )h_2DsumTSmaxALS7->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmaxALS7->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmaxAperLS7->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >   lsdep_estimator3_HFdepth2_  ) h_sumCutTSmaxAperLS7->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmaxAperLS7->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	      // HO:
	      if(k0==2) {
		// HBdepth1
		if(k1+1  ==4) {
		  h_sumTSmaxALS8->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 >  lsdep_estimator3_HOdepth4_  )h_2DsumTSmaxALS8->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumTSmaxALS8->Fill(double(ieta), double(k3), bbb1);
		  h_sumTSmaxAperLS8->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >   lsdep_estimator3_HOdepth4_  ) h_sumCutTSmaxAperLS8->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0TSmaxAperLS8->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	    }//if(sumEstimator3[k0][k1][k2][k3] != 0.


	    // ------------------------------------------------------------------------------------------------------------------------sumEstimator4 W
	    if(sumEstimator4[k0][k1][k2][k3] != 0. ) {

	      // fill histoes:
	      double bbbc=0.;
	      if(flagestimatornormalization_ == 0 ) bbbc = sumEstimator4[k0][k1][k2][k3]/nevcounter0;
	      if(flagestimatornormalization_ == 1 ) bbbc = sumEstimator4[k0][k1][k2][k3]/sum0Estimator[k0][k1][k2][k3];
	      double bbb1=1.;
	      if(flagestimatornormalization_ == 2 ) {bbbc = sumEstimator4[k0][k1][k2][k3]; bbb1 = sum0Estimator[k0][k1][k2][k3];}
	      
	      // HB:
	      if(k0==0) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumAmplitudeLS1->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 > lsdep_estimator4_HBdepth1_  )h_2DsumAmplitudeLS1->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplitudeLS1->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplitudeperLS1->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >  lsdep_estimator4_HBdepth1_  ) h_sumCutAmplitudeperLS1->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplitudeperLS1->Fill( float(lscounterM1) ,bbb1);
		  if(bbbc/bbb1 >  2.*lsdep_estimator4_HBdepth1_  ) h_sumAmplitudeperLS1_LSselected->Fill( float(lscounterM1) ,bbbc); 
		  //		  if( ( lscounterM1 == 734 || lscounterM1 == 758 || lscounterM1 == 759 || lscounterM1 == 853 || lscounterM1 == 1436 ) ) h_sumAmplitudeperLS1_LSselected->Fill( float(lscounterM1) ,bbbc); 
				  //		  if( ( lscounterM1 == 504 || lscounterM1 == 867 ) ) h_sumAmplitudeperLS1_LSselected->Fill( float(lscounterM1) ,bbbc); 

		}
		if(k1+1  ==2) {
		  h_sumAmplitudeLS2->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 > lsdep_estimator4_HBdepth2_  )h_2DsumAmplitudeLS2->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplitudeLS2->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplitudeperLS2->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >  lsdep_estimator4_HBdepth2_  ) h_sumCutAmplitudeperLS2->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplitudeperLS2->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	      // HE:
	      if(k0==1) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumAmplitudeLS3->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 > lsdep_estimator4_HEdepth1_  )h_2DsumAmplitudeLS3->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplitudeLS3->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplitudeperLS3->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >  lsdep_estimator4_HEdepth1_  ) h_sumCutAmplitudeperLS3->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplitudeperLS3->Fill( float(lscounterM1) ,bbb1);

		}
		if(k1+1  ==2) {
		  h_sumAmplitudeLS4->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 > lsdep_estimator4_HEdepth2_  )h_2DsumAmplitudeLS4->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplitudeLS4->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplitudeperLS4->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >  lsdep_estimator4_HEdepth2_  ) h_sumCutAmplitudeperLS4->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplitudeperLS4->Fill( float(lscounterM1) ,bbb1);

		}
		if(k1+1  ==3) {
		  h_sumAmplitudeLS5->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 > lsdep_estimator4_HEdepth3_  )h_2DsumAmplitudeLS5->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplitudeLS5->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplitudeperLS5->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >  lsdep_estimator4_HEdepth3_  ) h_sumCutAmplitudeperLS5->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplitudeperLS5->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	      // HF:
	      if(k0==3) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumAmplitudeLS6->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 > lsdep_estimator4_HFdepth1_   )h_2DsumAmplitudeLS6->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplitudeLS6->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplitudeperLS6->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >  lsdep_estimator4_HFdepth1_   ) h_sumCutAmplitudeperLS6->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplitudeperLS6->Fill( float(lscounterM1) ,bbb1);

		}
		if(k1+1  ==2) {
		  h_sumAmplitudeLS7->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 > lsdep_estimator4_HFdepth2_   )h_2DsumAmplitudeLS7->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplitudeLS7->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplitudeperLS7->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >  lsdep_estimator4_HFdepth2_   ) h_sumCutAmplitudeperLS7->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplitudeperLS7->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	      // HO:
	      if(k0==2) {
		// HBdepth1
		if(k1+1  ==4) {
		  h_sumAmplitudeLS8->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 > lsdep_estimator4_HOdepth4_   )h_2DsumAmplitudeLS8->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplitudeLS8->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplitudeperLS8->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >  lsdep_estimator4_HOdepth4_   ) h_sumCutAmplitudeperLS8->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplitudeperLS8->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	    }//if(sumEstimator4[k0][k1][k2][k3] != 0.


	    // ------------------------------------------------------------------------------------------------------------------------sumEstimator5 R
	    if(sumEstimator5[k0][k1][k2][k3] != 0. ) {

	      // fill histoes:
	      double bbbc=0.;
	      if(flagestimatornormalization_ == 0 ) bbbc = sumEstimator5[k0][k1][k2][k3]/nevcounter0;
	      if(flagestimatornormalization_ == 1 ) bbbc = sumEstimator5[k0][k1][k2][k3]/sum0Estimator[k0][k1][k2][k3];
	      double bbb1=1.;
	      if(flagestimatornormalization_ == 2 ) {bbbc = sumEstimator5[k0][k1][k2][k3]; bbb1 = sum0Estimator[k0][k1][k2][k3];}
	      
	      // HB:
	      if(k0==0) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumAmplLS1->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 > lsdep_estimator5_HBdepth1_  )h_2DsumAmplLS1->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplLS1->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplperLS1->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >  lsdep_estimator5_HBdepth1_  ) h_sumCutAmplperLS1->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplperLS1->Fill( float(lscounterM1) ,bbb1);
		  if(bbbc/bbb1 >  2.*lsdep_estimator5_HBdepth1_  ) h_sumAmplperLS1_LSselected->Fill( float(lscounterM1) ,bbbc); 
		  //		  if( ( lscounterM1 == 734 || lscounterM1 == 758 || lscounterM1 == 759 || lscounterM1 == 853 || lscounterM1 == 1436 ) ) h_sumAmplperLS1_LSselected->Fill( float(lscounterM1) ,bbbc); 
		  //		  if( ( lscounterM1 == 504 || lscounterM1 == 867 ) ) h_sumAmplperLS1_LSselected->Fill( float(lscounterM1) ,bbbc); 

		}
		if(k1+1  ==2) {
		  h_sumAmplLS2->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 >  lsdep_estimator5_HBdepth2_  )h_2DsumAmplLS2->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplLS2->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplperLS2->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >   lsdep_estimator5_HBdepth2_  ) h_sumCutAmplperLS2->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplperLS2->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	      // HE:
	      if(k0==1) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumAmplLS3->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 >  lsdep_estimator5_HEdepth1_  )h_2DsumAmplLS3->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplLS3->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplperLS3->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >   lsdep_estimator5_HEdepth1_  ) h_sumCutAmplperLS3->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplperLS3->Fill( float(lscounterM1) ,bbb1);

		}
		if(k1+1  ==2) {
		  h_sumAmplLS4->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 >  lsdep_estimator5_HEdepth2_  )h_2DsumAmplLS4->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplLS4->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplperLS4->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >   lsdep_estimator5_HEdepth2_  ) h_sumCutAmplperLS4->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplperLS4->Fill( float(lscounterM1) ,bbb1);

		}
		if(k1+1  ==3) {
		  h_sumAmplLS5->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 >  lsdep_estimator5_HEdepth3_  )h_2DsumAmplLS5->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplLS5->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplperLS5->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >   lsdep_estimator5_HEdepth3_  ) h_sumCutAmplperLS5->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplperLS5->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	      // HF:
	      if(k0==3) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumAmplLS6->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 >  lsdep_estimator5_HFdepth1_  )h_2DsumAmplLS6->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplLS6->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplperLS6->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >   lsdep_estimator5_HFdepth1_  ) h_sumCutAmplperLS6->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplperLS6->Fill( float(lscounterM1) ,bbb1);

		}
		if(k1+1  ==2) {
		  h_sumAmplLS7->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 >  lsdep_estimator5_HFdepth2_  )h_2DsumAmplLS7->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplLS7->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplperLS7->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >   lsdep_estimator5_HFdepth2_  ) h_sumCutAmplperLS7->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplperLS7->Fill( float(lscounterM1) ,bbb1);

		}
	      }
	      // HO:
	      if(k0==2) {
		// HBdepth1
		if(k1+1  ==4) {
		  h_sumAmplLS8->Fill(bbbc/bbb1);
		  if(bbbc/bbb1 >  lsdep_estimator5_HOdepth4_  )h_2DsumAmplLS8->Fill(double(ieta),double(k3),bbbc);
		  h_2D0sumAmplLS8->Fill(double(ieta), double(k3), bbb1);
		  h_sumAmplperLS8->Fill( float(lscounterM1) ,bbbc);
		  if(bbbc/bbb1 >   lsdep_estimator5_HOdepth4_  ) h_sumCutAmplperLS8->Fill( float(lscounterM1) ,bbbc); 
		  h_sum0AmplperLS8->Fill( float(lscounterM1) ,bbb1);
		  
		}
	      }
	    }//if(sumEstimator5[k0][k1][k2][k3] != 0.
	    
	    
	    // ------------------------------------------------------------------------------------------------------------------------sumEstimator6 (Error-B)
	    if (verbosity == -81 ) std::cout << "sumEstimator6 = " <<sumEstimator6[k0][k1][k2][k3]<< std::endl;
	    if(sumEstimator6[k0][k1][k2][k3] != 0. ) {
	      
	      // fill histoes:
	      double bbbc=0.;
	      if(flagestimatornormalization_ == 0 ) bbbc = sumEstimator6[k0][k1][k2][k3]/nevcounter0;
	      if(flagestimatornormalization_ == 1 ) bbbc = sumEstimator6[k0][k1][k2][k3]/sum0Estimator[k0][k1][k2][k3];
	      double bbb1=1.;
	      if(flagestimatornormalization_ == 2 ) {bbbc = sumEstimator6[k0][k1][k2][k3]; bbb1 = sum0Estimator[k0][k1][k2][k3];}
	      
	      // HB:
	      if(k0==0) {
		// HBdepth1
		if(k1+1  ==1) {
		  h_sumErrorBLS1->Fill(bbbc/bbb1);
		  h_2DsumErrorBLS1->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumErrorBLS1->Fill(double(ieta), double(k3), bbb1);
		  h_sumErrorBperLS1->Fill( float(lscounterM1) ,bbbc);
		  h_sum0ErrorBperLS1->Fill( float(lscounterM1) ,bbb1);
		}
		if(k1+1  ==2) {
		  h_sumErrorBLS2->Fill(bbbc/bbb1);
		  h_2DsumErrorBLS2->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumErrorBLS2->Fill(double(ieta), double(k3), bbb1);
		  h_sumErrorBperLS2->Fill( float(lscounterM1) ,bbbc);
		  h_sum0ErrorBperLS2->Fill( float(lscounterM1) ,bbb1);
		}
	      }
	      // HE:
	      if(k0==1) {
		// HEdepth1
		if(k1+1  ==1) {
		  h_sumErrorBLS3->Fill(bbbc/bbb1);
		  h_2DsumErrorBLS3->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumErrorBLS3->Fill(double(ieta), double(k3), bbb1);
		  h_sumErrorBperLS3->Fill( float(lscounterM1) ,bbbc);
		  h_sum0ErrorBperLS3->Fill( float(lscounterM1) ,bbb1);
		}
		if(k1+1  ==2) {
		  h_sumErrorBLS4->Fill(bbbc/bbb1);
		  h_2DsumErrorBLS4->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumErrorBLS4->Fill(double(ieta), double(k3), bbb1);
		  h_sumErrorBperLS4->Fill( float(lscounterM1) ,bbbc);
		  h_sum0ErrorBperLS4->Fill( float(lscounterM1) ,bbb1);
		}
		if(k1+1  ==3) {
		  h_sumErrorBLS5->Fill(bbbc/bbb1);
		  h_2DsumErrorBLS5->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumErrorBLS5->Fill(double(ieta), double(k3), bbb1);
		  h_sumErrorBperLS5->Fill( float(lscounterM1) ,bbbc);
		  h_sum0ErrorBperLS5->Fill( float(lscounterM1) ,bbb1);
		}
	      }
	      // HF:
	      if(k0==3) {
		// HFdepth1
		if(k1+1  ==1) {
		  h_sumErrorBLS6->Fill(bbbc/bbb1);
		  h_2DsumErrorBLS6->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumErrorBLS6->Fill(double(ieta), double(k3), bbb1);
		  h_sumErrorBperLS6->Fill( float(lscounterM1) ,bbbc);
		  h_sum0ErrorBperLS6->Fill( float(lscounterM1) ,bbb1);
		}
		if(k1+1  ==2) {
		  h_sumErrorBLS7->Fill(bbbc/bbb1);
		  h_2DsumErrorBLS7->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumErrorBLS7->Fill(double(ieta), double(k3), bbb1);
		  h_sumErrorBperLS7->Fill( float(lscounterM1) ,bbbc);
		  h_sum0ErrorBperLS7->Fill( float(lscounterM1) ,bbb1);
		}
	      }
	      // HO:
	      if(k0==2) {
		// HOdepth4
		if(k1+1  ==4) {
		  h_sumErrorBLS8->Fill(bbbc/bbb1);
		  h_2DsumErrorBLS8->Fill(double(ieta), double(k3), bbbc);
		  h_2D0sumErrorBLS8->Fill(double(ieta), double(k3), bbb1);
		  h_sumErrorBperLS8->Fill( float(lscounterM1) ,bbbc);
		  h_sum0ErrorBperLS8->Fill( float(lscounterM1) ,bbb1);
		}
	      }
	      ///
	    }//if(sumEstimator6[k0][k1][k2][k3] != 0.
	    
	      ///
	      ///
	  }//for  
	}//for  
      }//for  
    }//for 



    //------------------------------------------------------   averSIGNAL
    averSIGNALoccupancy_HB /= float(nevcounter0);
    h_averSIGNALoccupancy_HB->Fill( float(lscounterM1) ,averSIGNALoccupancy_HB);
    averSIGNALoccupancy_HE /= float(nevcounter0);
    h_averSIGNALoccupancy_HE->Fill( float(lscounterM1) ,averSIGNALoccupancy_HE);
    averSIGNALoccupancy_HF /= float(nevcounter0);
    h_averSIGNALoccupancy_HF->Fill( float(lscounterM1) ,averSIGNALoccupancy_HF);
    averSIGNALoccupancy_HO /= float(nevcounter0);
    h_averSIGNALoccupancy_HO->Fill( float(lscounterM1) ,averSIGNALoccupancy_HO);

    averSIGNALoccupancy_HB = 0.;
    averSIGNALoccupancy_HE = 0.;
    averSIGNALoccupancy_HF = 0.;
    averSIGNALoccupancy_HO = 0.;
    
    //------------------------------------------------------
    averSIGNALsumamplitude_HB /= float(nevcounter0);
    h_averSIGNALsumamplitude_HB->Fill( float(lscounterM1) ,averSIGNALsumamplitude_HB);
    averSIGNALsumamplitude_HE /= float(nevcounter0);
    h_averSIGNALsumamplitude_HE->Fill( float(lscounterM1) ,averSIGNALsumamplitude_HE);
    averSIGNALsumamplitude_HF /= float(nevcounter0);
    h_averSIGNALsumamplitude_HF->Fill( float(lscounterM1) ,averSIGNALsumamplitude_HF);
    averSIGNALsumamplitude_HO /= float(nevcounter0);
    h_averSIGNALsumamplitude_HO->Fill( float(lscounterM1) ,averSIGNALsumamplitude_HO);

    averSIGNALsumamplitude_HB = 0.;
    averSIGNALsumamplitude_HE = 0.;
    averSIGNALsumamplitude_HF = 0.;
    averSIGNALsumamplitude_HO = 0.;
    
    //------------------------------------------------------   averNOSIGNAL
    averNOSIGNALoccupancy_HB /= float(nevcounter0);
    h_averNOSIGNALoccupancy_HB->Fill( float(lscounterM1) ,averNOSIGNALoccupancy_HB);
    averNOSIGNALoccupancy_HE /= float(nevcounter0);
    h_averNOSIGNALoccupancy_HE->Fill( float(lscounterM1) ,averNOSIGNALoccupancy_HE);
    averNOSIGNALoccupancy_HF /= float(nevcounter0);
    h_averNOSIGNALoccupancy_HF->Fill( float(lscounterM1) ,averNOSIGNALoccupancy_HF);
    averNOSIGNALoccupancy_HO /= float(nevcounter0);
    h_averNOSIGNALoccupancy_HO->Fill( float(lscounterM1) ,averNOSIGNALoccupancy_HO);

    averNOSIGNALoccupancy_HB = 0.;
    averNOSIGNALoccupancy_HE = 0.;
    averNOSIGNALoccupancy_HF = 0.;
    averNOSIGNALoccupancy_HO = 0.;
    
    //------------------------------------------------------
    averNOSIGNALsumamplitude_HB /= float(nevcounter0);
    h_averNOSIGNALsumamplitude_HB->Fill( float(lscounterM1) ,averNOSIGNALsumamplitude_HB);
    averNOSIGNALsumamplitude_HE /= float(nevcounter0);
    h_averNOSIGNALsumamplitude_HE->Fill( float(lscounterM1) ,averNOSIGNALsumamplitude_HE);
    averNOSIGNALsumamplitude_HF /= float(nevcounter0);
    h_averNOSIGNALsumamplitude_HF->Fill( float(lscounterM1) ,averNOSIGNALsumamplitude_HF);
    averNOSIGNALsumamplitude_HO /= float(nevcounter0);
    h_averNOSIGNALsumamplitude_HO->Fill( float(lscounterM1) ,averNOSIGNALsumamplitude_HO);

    averNOSIGNALsumamplitude_HB = 0.;
    averNOSIGNALsumamplitude_HE = 0.;
    averNOSIGNALsumamplitude_HF = 0.;
    averNOSIGNALsumamplitude_HO = 0.;
    
    //------------------------------------------------------
    if (verbosity == -9812 ) std::cout << "************* lscounterM1= " <<lscounterM1 << "   maxxSUM1 =  " <<maxxSUM1 << "   nevcounter0 =  " <<nevcounter0 << std::endl;
    if (verbosity == -9812 ) std::cout << "after dividing   maxxSUM1 =  " <<maxxSUM1 << std::endl;
    h_maxxSUMAmpl_HB->Fill( float(lscounterM1) ,maxxSUM1);
    h_maxxSUMAmpl_HE->Fill( float(lscounterM1) ,maxxSUM2);
    h_maxxSUMAmpl_HO->Fill( float(lscounterM1) ,maxxSUM3);
    h_maxxSUMAmpl_HF->Fill( float(lscounterM1) ,maxxSUM4);
    maxxSUM1 = 0.;
    maxxSUM2 = 0.;
    maxxSUM3 = 0.;
    maxxSUM4 = 0.;
      if (verbosity == -9814 ) std::cout << "*******  FINAL       ===loop   lscounterM1 =  " <<lscounterM1 <<"    nevcounter0 =  " <<nevcounter0 <<"    maxxSUM4 =  " <<maxxSUM4 << std::endl;

    //------------------------------------------------------
    h_maxxOCCUP_HB->Fill( float(lscounterM1) ,maxxOCCUP1);
    h_maxxOCCUP_HE->Fill( float(lscounterM1) ,maxxOCCUP2);
    h_maxxOCCUP_HO->Fill( float(lscounterM1) ,maxxOCCUP3);
    h_maxxOCCUP_HF->Fill( float(lscounterM1) ,maxxOCCUP4);
    maxxOCCUP1 = 0.;
    maxxOCCUP2 = 0.;
    maxxOCCUP3 = 0.;
    maxxOCCUP4 = 0.;
    
  }//if( nevcounter0 != 0 )    
  /////////////////////////////// -------------------------------------------------------------------

//    edm::ESHandle<HcalTopology> topo_;
//    iSetup.get<HcalRecNumberingRecord>().get(topo_);
//    topo = &*topo_;
/*
  if (MAPcreation>0) {
    std::cout << "===== Start writing Channel MAP =====" << std::endl;    
    MAPfile.open(MAPOutputFileName);
 
    edm::ESHandle<HcalTopology> topo_;
    iSetup.get<HcalRecNumberingRecord>().get(topo_);

    const HcalTopology* topo = &*topo_;
    
    HcalLogicalMapGenerator gen;
    HcalLogicalMap lmap=gen.createMap(topo); 
    
    HcalElectronicsMap emap=lmap.generateHcalElectronicsMap();
    std::string subdet =""; 
                     
 
//    MAPfile << "#ifndef LogEleMapdb_h" << std::endl;
    MAPfile << "#define LogEleMapdb_h" << std::endl;
    MAPfile << "#include <algorithm>" << std::endl;
    MAPfile << "#include <iostream>" << std::endl;
    MAPfile << "#include <vector>" << std::endl;
    MAPfile << "#include <string>" << std::endl;
    MAPfile << "#include <sstream>" << std::endl;

    MAPfile <<  std::endl;
    
    MAPfile << "struct Cell {" << std::endl;
    MAPfile << " std::string subdet;" << std::endl;    
    MAPfile << " int Eta;" << std::endl;             
    MAPfile << " int Phi;" << std::endl;           
    MAPfile << " int Depth;" << std::endl;           
    MAPfile << " std::string RBX;" << std::endl;
    MAPfile << " int RM;" << std::endl;             
    MAPfile << " int Pixel;" << std::endl;
    MAPfile << " int RMfiber;" << std::endl;
    MAPfile << " int FiberCh;" << std::endl;
    MAPfile << " int QIE;" << std::endl;
    MAPfile << " int ADC;" << std::endl;
    MAPfile << " int VMECardID;" << std::endl;
    MAPfile << " int dccID;" << std::endl;
    MAPfile << " int Spigot;" << std::endl;
    MAPfile << " int FiberIndex;" << std::endl;
    MAPfile << " int HtrSlot;" << std::endl;
    MAPfile << " int HtrTB;" << std::endl;
    MAPfile <<  std::endl;
    
    MAPfile << "// the function check, if \"par\" == \"val\" for this cell" << std::endl;
    MAPfile << " bool check(const std::string par, const int val) const " << std::endl;
    MAPfile << " {" << std::endl;
    MAPfile << "       if (par == \"Eta\")    return (val == Eta);" << std::endl;
    MAPfile << "  else if (par == \"Phi\")     return (val == Phi);" << std::endl;
    MAPfile << "  else if (par == \"Depth\")      return (val == Depth);" << std::endl;
    MAPfile << "  else if (par == \"RM\")     return (val == RM);" << std::endl;
    MAPfile << "  else if (par == \"Pixel\") return (val == Pixel);" << std::endl;
    MAPfile << "  else if (par == \"RMfiber\")    return (val == RMfiber);" << std::endl;
    MAPfile << "  else if (par == \"FiberCh\")    return (val == FiberCh);" << std::endl;
    MAPfile << "  else if (par == \"QIE\")     return (val == QIE);" << std::endl;
    MAPfile << "  else if (par == \"ADC\")     return (val == ADC);" << std::endl;
    MAPfile << "  else if (par == \"VMECardID\")     return (val == VMECardID);" << std::endl;
    MAPfile << "  else if (par == \"dccID\")     return (val == dccID);" << std::endl;
    MAPfile << "  else if (par == \"Spigot\")     return (val == Spigot);" << std::endl;
    MAPfile << "  else if (par == \"FiberIndex\")     return (val == FiberIndex);" << std::endl;
    MAPfile << "  else if (par == \"HtrSlot\")     return (val == HtrSlot);" << std::endl;
    MAPfile << "  else if (par == \"HtrTB\")     return (val == HtrTB);" << std::endl;
    MAPfile << "  else return false;" << std::endl;
    MAPfile << " }" << std::endl;
    MAPfile <<  std::endl;
    
    MAPfile << " bool check(const std::string par, const std::string val) const" << std::endl;
    MAPfile << " {" << std::endl;
    MAPfile << "       if (par == \"subdet\")    return (val == subdet);" << std::endl;
    MAPfile << "  else if (par == \"RBX\")    return (val == RBX);" << std::endl;
    MAPfile << "  else return false;" << std::endl;
    MAPfile << " }" << std::endl;

    MAPfile << "};" << std::endl;
    MAPfile <<  std::endl;  
   
    MAPfile << "const Cell AllCells[] = {" << std::endl; 
    MAPfile << "//{ SD, Eta, Phi, Depth,     RBX, RM, PIXEL, RMfiber, Fiber Ch., QIE, ADC, VMECrateId, dccid, spigot, fiberIndex, htrSlot, htrTopBottom }" << std::endl;     
    
//HB
        for (int eta= -16;eta<0;eta++) {
          for (int phi=1;phi<=72;phi++) {
	     for (int depth=1;depth<=2;depth++) {
                HcalDetId *detid=0;
                detid=new HcalDetId(HcalBarrel,eta,phi,depth); subdet="HB";
                std::cout<<" Detid = "<<detid<<" "<<eta<<" "<<phi<<" "<<depth<<std::endl;
	        HcalFrontEndId    lmap_entry=lmap.getHcalFrontEndId(*detid);
	        HcalElectronicsId emap_entry=emap.lookup(*detid);
                MAPfile << "  {\""<<subdet<<"\" , "<< detid->ieta()<<" , "<< detid->iphi()-1<<" ,    "<< detid->depth()<<" ," ;    
                MAPfile << "\""<<lmap_entry.rbx()<<"\" , "<<lmap_entry.rm()<<" ,   "<<lmap_entry.pixel()<<" ,      "<<lmap_entry.rmFiber()<<" ,        " ;
                MAPfile << lmap_entry.fiberChannel()<<" ,  "<<lmap_entry.qieCard()<<" ,  "<<lmap_entry.adc()<<" ,        ";    
                MAPfile << emap_entry.readoutVMECrateId()<<" ,    "<<emap_entry.dccid()<<" ,     "<<emap_entry.spigot()<<" ,         "<<emap_entry.fiberIndex()<<" ,      " ;
                MAPfile << emap_entry.htrSlot()<<" ,      "<<emap_entry.htrTopBottom() ;
                MAPfile << "}," << std::endl;		
		//delete detid;
             }  //Depth
          }  //Phi
       }  //Eta  
*/
/*  
        for (int eta= 1;eta<=16;eta++) {
          for (int phi=1;phi<=72;phi++) {
	     for (int depth=1;depth<=2;depth++) {
                HcalDetId *detid=0;
                detid=new HcalDetId(HcalBarrel,eta,phi,depth); subdet="HB";
	        HcalFrontEndId    lmap_entry=lmap.getHcalFrontEndId(*detid);
	        HcalElectronicsId emap_entry=emap.lookup(*detid);
                MAPfile << "  {\""<<subdet<<"\" , "<< detid->ieta()-1<<" , "<< detid->iphi()-1<<" ,    "<< detid->depth()<<" ," ;    
                MAPfile << "\""<<lmap_entry.rbx()<<"\" , "<<lmap_entry.rm()<<" ,   "<<lmap_entry.pixel()<<" ,      "<<lmap_entry.rmFiber()<<" ,        " ;
                MAPfile << lmap_entry.fiberChannel()<<" ,  "<<lmap_entry.qieCard()<<" ,  "<<lmap_entry.adc()<<" ,        ";    
                MAPfile << emap_entry.readoutVMECrateId()<<" ,    "<<emap_entry.dccid()<<" ,     "<<emap_entry.spigot()<<" ,         "<<emap_entry.fiberIndex()<<" ,      " ;
                MAPfile << emap_entry.htrSlot()<<" ,      "<<emap_entry.htrTopBottom() ;
                MAPfile << "}," << std::endl;		
		delete detid;
             }  //Depth
          }  //Phi
       }  //Eta 
*/
//  }

/*       
//HE     	      
      for (int eta= -20;eta<=-20;eta++) {
          for (int phi=72;phi<=72;phi++) {
	     for (int depth=1;depth<=2;depth++) {
                HcalDetId *detid=0;
                detid=new HcalDetId(HcalEndcap,eta,phi,depth); subdet="HE";
	        HcalFrontEndId    lmap_entry=lmap.getHcalFrontEndId(*detid);
	        HcalElectronicsId emap_entry=emap.lookup(*detid);
                MAPfile << "  {\""<<subdet<<"\" , "<< detid->ieta()<<" , "<< detid->iphi()-1<<" ,    "<< detid->depth()<<" ," ;    
                MAPfile << "\""<<lmap_entry.rbx()<<"\" , "<<lmap_entry.rm()<<" ,   "<<lmap_entry.pixel()<<" ,      "<<lmap_entry.rmFiber()<<" ,        " ;
                MAPfile << lmap_entry.fiberChannel()<<" ,  "<<lmap_entry.qieCard()<<" ,  "<<lmap_entry.adc()<<" ,        ";    
                MAPfile << emap_entry.readoutVMECrateId()<<" ,    "<<emap_entry.dccid()<<" ,     "<<emap_entry.spigot()<<" ,         "<<emap_entry.fiberIndex()<<" ,      " ;
                MAPfile << emap_entry.htrSlot()<<" ,      "<<emap_entry.htrTopBottom() ;
                MAPfile << "}," << std::endl;		
		delete detid;
             }  //Depth
          }  //Phi
       }  //Eta

      for (int eta= -19;eta<=-16;eta++) {
          for (int phi=72;phi<=72;phi++) {
	     for (int depth=1;depth<=3;depth++) {
                HcalDetId *detid=0;
                detid=new HcalDetId(HcalEndcap,eta,phi,depth); subdet="HE";
	        HcalFrontEndId    lmap_entry=lmap.getHcalFrontEndId(*detid);
	        HcalElectronicsId emap_entry=emap.lookup(*detid);
                MAPfile << "  {\""<<subdet<<"\" , "<< detid->ieta()<<" , "<< detid->iphi()-1<<" ,    "<< detid->depth()<<" ," ;    
                MAPfile << "\""<<lmap_entry.rbx()<<"\" , "<<lmap_entry.rm()<<" ,   "<<lmap_entry.pixel()<<" ,      "<<lmap_entry.rmFiber()<<" ,        " ;
                MAPfile << lmap_entry.fiberChannel()<<" ,  "<<lmap_entry.qieCard()<<" ,  "<<lmap_entry.adc()<<" ,        ";    
                MAPfile << emap_entry.readoutVMECrateId()<<" ,    "<<emap_entry.dccid()<<" ,     "<<emap_entry.spigot()<<" ,         "<<emap_entry.fiberIndex()<<" ,      " ;
                MAPfile << emap_entry.htrSlot()<<" ,      "<<emap_entry.htrTopBottom() ;
                MAPfile << "}," << std::endl;		
		delete detid;
             }  //Depth
          }  //Phi
       }  //Eta
      for (int eta= -29;eta<=-16;eta++) {
          for (int phi=1;phi<=71;phi++) {
	     for (int depth=1;depth<=3;depth++) {
                HcalDetId *detid=0;
                detid=new HcalDetId(HcalEndcap,eta,phi,depth); subdet="HE";
	        HcalFrontEndId    lmap_entry=lmap.getHcalFrontEndId(*detid);
	        HcalElectronicsId emap_entry=emap.lookup(*detid);
                MAPfile << "  {\""<<subdet<<"\" , "<< detid->ieta()<<" , "<< detid->iphi()-1 <<" ,    "<< detid->depth()<<" ," ;    
                MAPfile << "\""<<lmap_entry.rbx()<<"\" , "<<lmap_entry.rm()<<" ,   "<<lmap_entry.pixel()<<" ,      "<<lmap_entry.rmFiber()<<" ,        " ;
                MAPfile << lmap_entry.fiberChannel()<<" ,  "<<lmap_entry.qieCard()<<" ,  "<<lmap_entry.adc()<<" ,        ";    
                MAPfile << emap_entry.readoutVMECrateId()<<" ,    "<<emap_entry.dccid()<<" ,     "<<emap_entry.spigot()<<" ,         "<<emap_entry.fiberIndex()<<" ,      " ;
                MAPfile << emap_entry.htrSlot()<<" ,      "<<emap_entry.htrTopBottom() ;
                MAPfile << "}," << std::endl;		
		delete detid;
             }  //Depth
          }  //Phi
       }  //Eta
        for (int eta= 16;eta<=29;eta++) {
          for (int phi=1;phi<=72;phi++) {
	     for (int depth=1;depth<=3;depth++) {
                HcalDetId *detid=0;
                detid=new HcalDetId(HcalEndcap,eta,phi,depth); subdet="HE";
	        HcalFrontEndId    lmap_entry=lmap.getHcalFrontEndId(*detid);
	        HcalElectronicsId emap_entry=emap.lookup(*detid);
                MAPfile << "  {\""<<subdet<<"\" , "<< detid->ieta()<<" , "<< detid->iphi()-1 <<" ,    "<< detid->depth()<<" ," ;    
                MAPfile << "\""<<lmap_entry.rbx()<<"\" , "<<lmap_entry.rm()<<" ,   "<<lmap_entry.pixel()<<" ,      "<<lmap_entry.rmFiber()<<" ,        " ;
                MAPfile << lmap_entry.fiberChannel()<<" ,  "<<lmap_entry.qieCard()<<" ,  "<<lmap_entry.adc()<<" ,        ";    
                MAPfile << emap_entry.readoutVMECrateId()<<" ,    "<<emap_entry.dccid()<<" ,     "<<emap_entry.spigot()<<" ,         "<<emap_entry.fiberIndex()<<" ,      " ;
                MAPfile << emap_entry.htrSlot()<<" ,      "<<emap_entry.htrTopBottom() ;
                MAPfile << "}," << std::endl;		
		delete detid;
             }  //Depth
          }  //Phi
       }  //Eta
//HF
 
        for (int eta= -41;eta<=-29;eta++) {
          for (int phi=1;phi<=72;phi+=2) {
	     for (int depth=1;depth<=2;depth++) {
                HcalDetId *detid=0;
                detid=new HcalDetId(HcalForward,eta,phi,depth); subdet="HF";
	        HcalFrontEndId    lmap_entry=lmap.getHcalFrontEndId(*detid);
	        HcalElectronicsId emap_entry=emap.lookup(*detid);
                MAPfile << "  {\""<<subdet<<"\" , "<< detid->ieta()<<" , "<< detid->iphi()-1 <<" ,    "<< detid->depth()<<" ," ;    
                MAPfile << "\""<<lmap_entry.rbx()<<"\" , "<<lmap_entry.rm()<<" ,   "<<lmap_entry.pixel()<<" ,      "<<lmap_entry.rmFiber()<<" ,        " ;
                MAPfile << lmap_entry.fiberChannel()<<" ,  "<<lmap_entry.qieCard()<<" ,  "<<lmap_entry.adc()<<" ,        ";    
                MAPfile << emap_entry.readoutVMECrateId()<<" ,    "<<emap_entry.dccid()<<" ,     "<<emap_entry.spigot()<<" ,         "<<emap_entry.fiberIndex()<<" ,      " ;
                MAPfile << emap_entry.htrSlot()<<" ,      "<<emap_entry.htrTopBottom() ;
                MAPfile << "}," << std::endl;		
		delete detid;
             }  //Depth
          }  //Phi
       }  //Eta
        for (int eta= 29;eta<=41 ;eta++) {
          for (int phi=1;phi<=72;phi+=2) {
	     for (int depth=1;depth<=2;depth++) {
                HcalDetId *detid=0;
                detid=new HcalDetId(HcalForward,eta,phi,depth); subdet="HF";
	        HcalFrontEndId    lmap_entry=lmap.getHcalFrontEndId(*detid);
	        HcalElectronicsId emap_entry=emap.lookup(*detid);
                MAPfile << "  {\""<<subdet<<"\" , "<< detid->ieta()<<" , "<< detid->iphi()-1 <<" ,    "<< detid->depth()<<" ," ;    
                MAPfile << "\""<<lmap_entry.rbx()<<"\" , "<<lmap_entry.rm()<<" ,   "<<lmap_entry.pixel()<<" ,      "<<lmap_entry.rmFiber()<<" ,        " ;
                MAPfile << lmap_entry.fiberChannel()<<" ,  "<<lmap_entry.qieCard()<<" ,  "<<lmap_entry.adc()<<" ,        ";    
                MAPfile << emap_entry.readoutVMECrateId()<<" ,    "<<emap_entry.dccid()<<" ,     "<<emap_entry.spigot()<<" ,         "<<emap_entry.fiberIndex()<<" ,      " ;
                MAPfile << emap_entry.htrSlot()<<" ,      "<<emap_entry.htrTopBottom() ;
                MAPfile << "}," << std::endl;		
		delete detid;
             }  //Depth
          }  //Phi
       }  //Eta

//HO
        for (int eta= -15;eta<0;eta++) {
          for (int phi=1;phi<=72;phi++) {
	     for (int depth=4;depth<=4;depth++) {
                HcalDetId *detid=0;
                detid=new HcalDetId(HcalOuter,eta,phi,depth); subdet="HO";
	        HcalFrontEndId    lmap_entry=lmap.getHcalFrontEndId(*detid);
	        HcalElectronicsId emap_entry=emap.lookup(*detid);
                MAPfile << "  {\""<<subdet<<"\" , "<< detid->ieta()<<" , "<< detid->iphi()-1 <<" ,    "<< detid->depth()<<" ," ;    
                MAPfile << "\""<<lmap_entry.rbx()<<"\" , "<<lmap_entry.rm()<<" ,   "<<lmap_entry.pixel()<<" ,      "<<lmap_entry.rmFiber()<<" ,        " ;
                MAPfile << lmap_entry.fiberChannel()<<" ,  "<<lmap_entry.qieCard()<<" ,  "<<lmap_entry.adc()<<" ,        ";    
                MAPfile << emap_entry.readoutVMECrateId()<<" ,    "<<emap_entry.dccid()<<" ,     "<<emap_entry.spigot()<<" ,         "<<emap_entry.fiberIndex()<<" ,      " ;
                MAPfile << emap_entry.htrSlot()<<" ,      "<<emap_entry.htrTopBottom() ;
                MAPfile << "}," << std::endl;		
		delete detid;
             }  //Depth
          }  //Phi
       }  //Eta
        for (int eta= 1;eta<=15;eta++) {
          for (int phi=1;phi<=72;phi++) {
	     for (int depth=4;depth<=4;depth++) {
                HcalDetId *detid=0;
                detid=new HcalDetId(HcalOuter,eta,phi,depth); subdet="HO";
	        HcalFrontEndId    lmap_entry=lmap.getHcalFrontEndId(*detid);
	        HcalElectronicsId emap_entry=emap.lookup(*detid);
                MAPfile << "  {\""<<subdet<<"\" , "<< detid->ieta()-1<<" , "<< detid->iphi()-1 <<" ,    "<< detid->depth()<<" ," ;    
                MAPfile << "\""<<lmap_entry.rbx()<<"\" , "<<lmap_entry.rm()<<" ,   "<<lmap_entry.pixel()<<" ,      "<<lmap_entry.rmFiber()<<" ,        " ;
                MAPfile << lmap_entry.fiberChannel()<<" ,  "<<lmap_entry.qieCard()<<" ,  "<<lmap_entry.adc()<<" ,        ";    
                MAPfile << emap_entry.readoutVMECrateId()<<" ,    "<<emap_entry.dccid()<<" ,     "<<emap_entry.spigot()<<" ,         "<<emap_entry.fiberIndex()<<" ,      " ;
                MAPfile << emap_entry.htrSlot()<<" ,      "<<emap_entry.htrTopBottom() ;
                MAPfile << "}," << std::endl;		
		delete detid;
             }  //Depth
          }  //Phi
       }  //Eta

    MAPfile << "};" << std::endl;
    MAPfile <<  std::endl;
    
    MAPfile << "// macro for array length calculation" << std::endl; 
    MAPfile << "#define DIM(a) (sizeof(a)/sizeof(a[0]))" << std::endl; 
    MAPfile <<  std::endl;  
  
    MAPfile << "// class for cells array managing" << std::endl; 
    MAPfile << "class CellDB {" << std::endl; 
    MAPfile << "public:" << std::endl; 
    MAPfile << "  CellDB()" << std::endl; 
    MAPfile << "  : cells(AllCells,  AllCells + DIM(AllCells))" << std::endl; 
    MAPfile << "{}" << std::endl; 
    MAPfile <<  std::endl;  
  
    MAPfile << "// return i-th cell" << std::endl;
    MAPfile << "Cell operator [] (int i) const {return cells[i];}" << std::endl;
     
    MAPfile << "// number of cells in database" << std::endl; 
    MAPfile << "int size() const {return cells.size();}" << std::endl;
    MAPfile <<  std::endl; 
  
    MAPfile << "// select cells for which \"par\" == \"val\"" << std::endl; 
    MAPfile << "template<typename T>" << std::endl; 
    MAPfile << "CellDB find(const std::string par, const T val) const" << std::endl; 
    MAPfile << "{" << std::endl; 
    MAPfile << "  std::vector<Cell> s;" << std::endl; 
    MAPfile << "  for (size_t i = 0; i < cells.size(); ++i)" << std::endl; 
    MAPfile << "    if (cells[i].check(par, val))" << std::endl; 
    MAPfile << "    s.push_back(cells[i]);" << std::endl; 
    MAPfile << "  return CellDB(s);" << std::endl; 
    MAPfile << "} " << std::endl; 
    MAPfile <<  std::endl; 

    MAPfile << "private:" << std::endl; 
    MAPfile << " CellDB( const std::vector<Cell> s)" << std::endl; 
    MAPfile << " : cells(s)" << std::endl; 
    MAPfile << "{}" << std::endl; 
    MAPfile << "std::vector<Cell> cells;" << std::endl; 
    MAPfile << "};" << std::endl;

       
    MAPfile.close(); 
    MAPcreation = 0;
    std::cout << "===== Finish writing Channel MAP =====" << std::endl;  
  }
*/  
    std::cout<<" ==== Edn of run "<<std::endl;
}

void VeRawAnalyzer::endJob(){   

/*  if (MAPcreation>0) {
    std::cout << "===== Start writing Channel MAP =====" << std::endl;    
    MAPfile.open(MAPOutputFileName);
    
    HcalLogicalMapGenerator gen;
    HcalLogicalMap lmap=gen.createMap(topo); 
    
    HcalElectronicsMap emap=lmap.generateHcalElectronicsMap();
    std::string subdet =""; 

        for (int eta= -16;eta<0;eta++) {
          for (int phi=1;phi<=72;phi++) {
             for (int depth=1;depth<=2;depth++) {
                HcalDetId *detid=0;
                detid=new HcalDetId(HcalBarrel,eta,phi,depth); subdet="HB";
                if(detid == 0) continue;
                std::cout<<" Detid = "<<detid<<" "<<eta<<" "<<phi<<" "<<depth<<std::endl;
                HcalFrontEndId    lmap_entry=lmap.getHcalFrontEndId(*detid);
                HcalElectronicsId emap_entry=emap.lookup(*detid);
                MAPfile << "  {\""<<subdet<<"\" , "<< detid->ieta()<<" , "<< detid->iphi()-1<<" ,    "<< detid->depth()<<" ," ;    
                MAPfile << "\""<<lmap_entry.rbx()<<"\" , "<<lmap_entry.rm()<<" ,   "<<lmap_entry.pixel()<<" ,      "<<lmap_entry.rmFiber()<<" ,        " ;
                MAPfile << lmap_entry.fiberChannel()<<" ,  "<<lmap_entry.qieCard()<<" ,  "<<lmap_entry.adc()<<" ,        ";    
                MAPfile << emap_entry.readoutVMECrateId()<<" ,    "<<emap_entry.dccid()<<" ,     "<<emap_entry.spigot()<<" ,         "<<emap_entry.fiberIndex()<<" ,      " ;
                MAPfile << emap_entry.htrSlot()<<" ,      "<<emap_entry.htrTopBottom() ;
                MAPfile << "}," << std::endl;           
                //delete detid;
             }  //Depth
          }  //Phi
       }  //Eta  

  } 

*/

 
  cout  <<  " --------------------------------------- "  <<  endl;
  cout<<" for Run = "<<run0<<" with runcounter = "<< runcounter <<" #ev = "<<eventcounter<<endl;
  cout  << " #LS =  "  << lscounterrun << " #LS10 =  "  << lscounterrun10 << " Last LS =  "  << ls0 <<  endl;
  cout  <<  " --------------------------------------------- "  <<  endl;
  h_nls_per_run->Fill(float(lscounterrun) );
  h_nls_per_run10->Fill(float(lscounterrun10) );
  
  hOutputFile->SetCompressionLevel(2);
  
  hOutputFile->Write();   
  hOutputFile->cd();

  if(recordNtuples_) myTree->Write();

  ///////////////////////////////////////////////////////////////////////////////////////////////
  std::cout << "===== full number of events =  " << nevent << std::endl;
  std::cout << "===== possible max number of events in ntuple(each 50th recorded) =  " << nevent50 << std::endl;
  std::cout << "===== but limited by maxNeventsInNtuple  =  " << maxNeventsInNtuple_ << std::endl;
  std::cout << "===== full number of events*HBHEdigis =  " << nnnnnn << std::endl;
  std::cout << "===== full number of events*HFdigis =  " << counter << std::endl;
  std::cout << "===== full number of events*HOdigis =  " << counterho << std::endl;
  
  std::cout << "===== Start writing user histograms =====" << std::endl;
  //////////////////////////////////////////////////////////////////////   scaling of some histoes:
  /*
  double ww = 1.;
  if(nnnnnn1 != 0) ww = 1./nnnnnn1;
  h_nbadchannels_depth1_HB->Scale(ww);

  ww = 1.;
  if(nnnnnn2 != 0) ww = 1./nnnnnn2;
  h_nbadchannels_depth2_HB->Scale(ww);

  ww = 1.;
  if(nnnnnn3 != 0) ww = 1./nnnnnn3;
  h_nbadchannels_depth1_HE->Scale(ww);

  ww = 1.;
  if(nnnnnn4 != 0) ww = 1./nnnnnn4;
  h_nbadchannels_depth2_HE->Scale(ww);

  ww = 1.;
  if(nnnnnn5 != 0) ww = 1./nnnnnn5;
  h_nbadchannels_depth3_HE->Scale(ww);

  ww = 1.;
  if(nnnnnn6 != 0) ww = 1./nnnnnn6;
  h_nbadchannels_depth4_HO->Scale(ww);

  if(nnnnnn7 != 0) ww = 1./nnnnnn7;
  h_nbadchannels_depth1_HF->Scale(ww);

  ww = 1.;
  if(nnnnnn8 != 0) ww = 1./nnnnnn8;
  h_nbadchannels_depth2_HF->Scale(ww);
*/
  ///////////////////////////////////////////->Write();
  if(recordHistoes_) {
    h_errorGeneral->Write();
    h_error1->Write();
    h_error2->Write();
    h_error3->Write();
    h_amplError->Write();
    h_amplFine->Write();
    
    h_errorGeneral_HB->Write();
    h_error1_HB->Write();
    h_error2_HB->Write();
    h_error3_HB->Write();
    h_error4_HB->Write();
    h_error5_HB->Write();
    h_error6_HB->Write();
    h_error7_HB->Write();
    h_amplError_HB->Write();
    h_amplFine_HB->Write();
    h_mapDepth1Error_HB->Write();
    h_mapDepth2Error_HB->Write();
    h_mapDepth3Error_HB->Write();
    h_fiber0_HB->Write();
    h_fiber1_HB->Write();
    h_fiber2_HB->Write();
    h_repetedcapid_HB->Write();
    
    h_errorGeneral_HE->Write();
    h_error1_HE->Write();
    h_error2_HE->Write();
    h_error3_HE->Write();
    h_error4_HE->Write();
    h_error5_HE->Write();
    h_error6_HE->Write();
    h_error7_HE->Write();
    h_amplError_HE->Write();
    h_amplFine_HE->Write();
    h_mapDepth1Error_HE->Write();
    h_mapDepth2Error_HE->Write();
    h_mapDepth3Error_HE->Write();
    h_fiber0_HE->Write();
    h_fiber1_HE->Write();
    h_fiber2_HE->Write();
    h_repetedcapid_HE->Write();
    
    h_errorGeneral_HF->Write();
    h_error1_HF->Write();
    h_error2_HF->Write();
    h_error3_HF->Write();
    h_error4_HF->Write();
    h_error5_HF->Write();
    h_error6_HF->Write();
    h_error7_HF->Write();
    h_amplError_HF->Write();
    h_amplFine_HF->Write();
    h_mapDepth1Error_HF->Write();
    h_mapDepth2Error_HF->Write();
    h_mapDepth3Error_HF->Write();
    h_fiber0_HF->Write();
    h_fiber1_HF->Write();
    h_fiber2_HF->Write();
    h_repetedcapid_HF->Write();
    
    h_errorGeneral_HO->Write();
    h_error1_HO->Write();
    h_error2_HO->Write();
    h_error3_HO->Write();
    h_error4_HO->Write();
    h_error5_HO->Write();
    h_error6_HO->Write();
    h_error7_HO->Write();
    h_amplError_HO->Write();
    h_amplFine_HO->Write();
    h_mapDepth4Error_HO->Write();
    h_fiber0_HO->Write();
    h_fiber1_HO->Write();
    h_fiber2_HO->Write();
    h_repetedcapid_HO->Write();
    
    ///////////////////////
    h_ADCAmpl345Zoom_HB->Write();
    h_ADCAmpl345Zoom1_HB->Write();
    h_ADCAmpl345_HB->Write();

    h_ADCAmpl_HBCapIdNoError->Write();
    h_ADCAmpl345_HBCapIdNoError->Write();
    h_ADCAmpl_HBCapIdError->Write();
    h_ADCAmpl345_HBCapIdError->Write();
    
    h_ADCAmplZoom_HB->Write();
    h_ADCAmplZoom1_HB->Write();
    h_ADCAmpl_HB->Write();
    h_mapDepth1ADCAmpl225_HB->Write();
    h_mapDepth2ADCAmpl225_HB->Write();
    h_mapDepth1ADCAmpl_HB->Write();
    h_mapDepth2ADCAmpl_HB->Write();
    h_mapDepth1ADCAmpl12_HB->Write();
    h_mapDepth2ADCAmpl12_HB->Write();

    h_TSmeanA_HB->Write();
    h_mapDepth1TSmeanA225_HB->Write();
    h_mapDepth2TSmeanA225_HB->Write();
    h_mapDepth1TSmeanA_HB->Write();
    h_mapDepth2TSmeanA_HB->Write();

    h_TSmaxA_HB->Write();
    h_mapDepth1TSmaxA225_HB->Write();
    h_mapDepth2TSmaxA225_HB->Write();
    h_mapDepth1TSmaxA_HB->Write();
    h_mapDepth2TSmaxA_HB->Write();

    h_Amplitude_HB->Write();
    h_mapDepth1Amplitude225_HB->Write();
    h_mapDepth2Amplitude225_HB->Write();
    h_mapDepth1Amplitude_HB->Write();
    h_mapDepth2Amplitude_HB->Write();

    h_Ampl_HB->Write();
    h_mapDepth1Ampl047_HB->Write();
    h_mapDepth2Ampl047_HB->Write();
    h_mapDepth1Ampl_HB->Write();
    h_mapDepth2Ampl_HB->Write();
    h_mapDepth1AmplE34_HB->Write();
    h_mapDepth2AmplE34_HB->Write();
    h_mapDepth1_HB->Write();
    h_mapDepth2_HB->Write();

    h_mapDepth1ADCAmpl225Copy_HB->Write();
    h_mapDepth2ADCAmpl225Copy_HB->Write();
    h_mapDepth1ADCAmpl225Copy_HE->Write();
    h_mapDepth2ADCAmpl225Copy_HE->Write();
    h_mapDepth3ADCAmpl225Copy_HE->Write();
    h_mapDepth1ADCAmpl225Copy_HF->Write();
    h_mapDepth2ADCAmpl225Copy_HF->Write();
    h_mapDepth4ADCAmpl225Copy_HO->Write();
    ///////////////////////
    h_ADCAmpl_HF->Write();
    h_ADCAmplZoom1_HF->Write();
    h_mapDepth1ADCAmpl225_HF->Write();
    h_mapDepth2ADCAmpl225_HF->Write();
    h_mapDepth1ADCAmpl_HF->Write();
    h_mapDepth2ADCAmpl_HF->Write();
    h_mapDepth1ADCAmpl12_HF->Write();
    h_mapDepth2ADCAmpl12_HF->Write();

    h_TSmeanA_HF->Write();
    h_mapDepth1TSmeanA225_HF->Write();
    h_mapDepth2TSmeanA225_HF->Write();
    h_mapDepth1TSmeanA_HF->Write();
    h_mapDepth2TSmeanA_HF->Write();

    h_TSmaxA_HF->Write();
    h_mapDepth1TSmaxA225_HF->Write();
    h_mapDepth2TSmaxA225_HF->Write();
    h_mapDepth1TSmaxA_HF->Write();
    h_mapDepth2TSmaxA_HF->Write();

    h_Amplitude_HF->Write();
    h_mapDepth1Amplitude225_HF->Write();
    h_mapDepth2Amplitude225_HF->Write();
    h_mapDepth1Amplitude_HF->Write();
    h_mapDepth2Amplitude_HF->Write();

    h_Ampl_HF->Write();
    h_mapDepth1Ampl047_HF->Write();
    h_mapDepth2Ampl047_HF->Write();
    h_mapDepth1Ampl_HF->Write();
    h_mapDepth2Ampl_HF->Write();
    h_mapDepth1AmplE34_HF->Write();
    h_mapDepth2AmplE34_HF->Write();
    h_mapDepth1_HF->Write();
    h_mapDepth2_HF->Write();

    ///////////////////////
    h_ADCAmpl_HO->Write();
    h_ADCAmplZoom1_HO->Write();
    h_ADCAmpl_HO_copy->Write();
    h_mapDepth4ADCAmpl225_HO->Write();
    h_mapDepth4ADCAmpl_HO->Write();
    h_mapDepth4ADCAmpl12_HO->Write();

    h_TSmeanA_HO->Write();
    h_mapDepth4TSmeanA225_HO->Write();
    h_mapDepth4TSmeanA_HO->Write();

    h_TSmaxA_HO->Write();
    h_mapDepth4TSmaxA225_HO->Write();
    h_mapDepth4TSmaxA_HO->Write();

    h_Amplitude_HO->Write();
    h_mapDepth4Amplitude225_HO->Write();
    h_mapDepth4Amplitude_HO->Write();
    h_Ampl_HO->Write();
    h_mapDepth4Ampl047_HO->Write();
    h_mapDepth4Ampl_HO->Write();
    h_mapDepth4AmplE34_HO->Write();
    h_mapDepth4_HO->Write();

    //////////////////////////////////////////    
    h_ADCAmpl_HE->Write();
    h_ADCAmplZoom1_HE->Write();
    h_mapDepth1ADCAmpl225_HE->Write();
    h_mapDepth2ADCAmpl225_HE->Write();
    h_mapDepth3ADCAmpl225_HE->Write();
    h_mapDepth1ADCAmpl_HE->Write();
    h_mapDepth2ADCAmpl_HE->Write();
    h_mapDepth3ADCAmpl_HE->Write();
    h_mapDepth1ADCAmpl12_HE->Write();
    h_mapDepth2ADCAmpl12_HE->Write();
    h_mapDepth3ADCAmpl12_HE->Write();

    h_TSmeanA_HE->Write();
    h_mapDepth1TSmeanA225_HE->Write();
    h_mapDepth2TSmeanA225_HE->Write();
    h_mapDepth3TSmeanA225_HE->Write();
    h_mapDepth1TSmeanA_HE->Write();
    h_mapDepth2TSmeanA_HE->Write();
    h_mapDepth3TSmeanA_HE->Write();

    h_TSmaxA_HE->Write();
    h_mapDepth1TSmaxA225_HE->Write();
    h_mapDepth2TSmaxA225_HE->Write();
    h_mapDepth3TSmaxA225_HE->Write();
    h_mapDepth1TSmaxA_HE->Write();
    h_mapDepth2TSmaxA_HE->Write();
    h_mapDepth3TSmaxA_HE->Write();

    h_Amplitude_HE->Write();
    h_mapDepth1Amplitude225_HE->Write();
    h_mapDepth2Amplitude225_HE->Write();
    h_mapDepth3Amplitude225_HE->Write();
    h_mapDepth1Amplitude_HE->Write();
    h_mapDepth2Amplitude_HE->Write();
    h_mapDepth3Amplitude_HE->Write();

    h_Ampl_HE->Write();
    h_mapDepth1Ampl047_HE->Write();
    h_mapDepth2Ampl047_HE->Write();
    h_mapDepth3Ampl047_HE->Write();
    h_mapDepth1Ampl_HE->Write();
    h_mapDepth2Ampl_HE->Write();
    h_mapDepth3Ampl_HE->Write();
    h_mapDepth1AmplE34_HE->Write();
    h_mapDepth2AmplE34_HE->Write();
    h_mapDepth3AmplE34_HE->Write();
    h_mapDepth1_HE->Write();
    h_mapDepth2_HE->Write();
    h_mapDepth3_HE->Write();
    
    ///////////////////////
    /*  
    h_GetRMSOverNormalizedSignal_HB->Write();
    h_GetRMSOverNormalizedSignal_HE->Write();
    h_GetRMSOverNormalizedSignal_HO->Write();
    h_GetRMSOverNormalizedSignal_HF->Write();
    h_GetRMSOverNormalizedSignal3_HB->Write();
    h_GetRMSOverNormalizedSignal3_HE->Write();
    h_GetRMSOverNormalizedSignal3_HO->Write();
    h_GetRMSOverNormalizedSignal3_HF->Write();
*/

    h_FullSignal3D_HB->Write();
    h_FullSignal3D0_HB->Write();
    h_FullSignal3D_HE->Write();
    h_FullSignal3D0_HE->Write();
    h_FullSignal3D_HO->Write();
    h_FullSignal3D0_HO->Write();
    h_FullSignal3D_HF->Write();
    h_FullSignal3D0_HF->Write();

    h_nbadchannels_depth1_HB->Write();
    h_runnbadchannels_depth1_HB->Write();
    h_runnbadchannelsC_depth1_HB->Write();
    h_runbadrate_depth1_HB->Write();
    h_runbadrateC_depth1_HB->Write();
    h_runbadrate0_depth1_HB->Write();

    h_nbadchannels_depth2_HB->Write();
    h_runnbadchannels_depth2_HB->Write();
    h_runnbadchannelsC_depth2_HB->Write();
    h_runbadrate_depth2_HB->Write();
    h_runbadrateC_depth2_HB->Write();
    h_runbadrate0_depth2_HB->Write();

    h_nbadchannels_depth1_HE->Write();
    h_runnbadchannels_depth1_HE->Write();
    h_runnbadchannelsC_depth1_HE->Write();
    h_runbadrate_depth1_HE->Write();
    h_runbadrateC_depth1_HE->Write();
    h_runbadrate0_depth1_HE->Write();

    h_nbadchannels_depth2_HE->Write();
    h_runnbadchannels_depth2_HE->Write();
    h_runnbadchannelsC_depth2_HE->Write();
    h_runbadrate_depth2_HE->Write();
    h_runbadrateC_depth2_HE->Write();
    h_runbadrate0_depth2_HE->Write();

    h_nbadchannels_depth3_HE->Write();
    h_runnbadchannels_depth3_HE->Write();
    h_runnbadchannelsC_depth3_HE->Write();
    h_runbadrate_depth3_HE->Write();
    h_runbadrateC_depth3_HE->Write();
    h_runbadrate0_depth3_HE->Write();

    h_nbadchannels_depth1_HF->Write();
    h_runnbadchannels_depth1_HF->Write();
    h_runnbadchannelsC_depth1_HF->Write();
    h_runbadrate_depth1_HF->Write();
    h_runbadrateC_depth1_HF->Write();
    h_runbadrate0_depth1_HF->Write();

    h_nbadchannels_depth2_HF->Write();
    h_runnbadchannels_depth2_HF->Write();
    h_runnbadchannelsC_depth2_HF->Write();
    h_runbadrate_depth2_HF->Write();
    h_runbadrateC_depth2_HF->Write();
    h_runbadrate0_depth2_HF->Write();

    h_nbadchannels_depth4_HO->Write();
    h_runnbadchannels_depth4_HO->Write();
    h_runnbadchannelsC_depth4_HO->Write();
    h_runbadrate_depth4_HO->Write();
    h_runbadrateC_depth4_HO->Write();
    h_runbadrate0_depth4_HO->Write();


    ///////////////////////
    h_mapCapCalib047_HB->Write();
    h_mapCapCalib047_HE->Write();
    h_mapCapCalib047_HO->Write();
    h_mapCapCalib047_HF->Write();

    h_ADCCalib_HB->Write();
    h_ADCCalib1_HB->Write();
    h_mapADCCalib047_HB->Write();
    h_mapADCCalib_HB->Write();
    h_RatioCalib_HB->Write();
    h_mapRatioCalib047_HB->Write();
    h_mapRatioCalib_HB->Write();
    h_TSmaxCalib_HB->Write();
    h_mapTSmaxCalib047_HB->Write();
    h_mapTSmaxCalib_HB->Write();
    h_TSmeanCalib_HB->Write();
    h_mapTSmeanCalib047_HB->Write();
    h_mapTSmeanCalib_HB->Write();
    h_WidthCalib_HB->Write();
    h_mapWidthCalib047_HB->Write();
    h_mapWidthCalib_HB->Write();
    h_map_HB->Write();
    h_ADCCalib_HE->Write();
    h_ADCCalib1_HE->Write();
    h_mapADCCalib047_HE->Write();
    h_mapADCCalib_HE->Write();
    h_RatioCalib_HE->Write();
    h_mapRatioCalib047_HE->Write();
    h_mapRatioCalib_HE->Write();
    h_TSmaxCalib_HE->Write();
    h_mapTSmaxCalib047_HE->Write();
    h_mapTSmaxCalib_HE->Write();
    h_TSmeanCalib_HE->Write();
    h_mapTSmeanCalib047_HE->Write();
    h_mapTSmeanCalib_HE->Write();
    h_WidthCalib_HE->Write();
    h_mapWidthCalib047_HE->Write();
    h_mapWidthCalib_HE->Write();
    h_map_HE->Write();
    h_ADCCalib_HO->Write();
    h_ADCCalib1_HO->Write();
    h_mapADCCalib047_HO->Write();
    h_mapADCCalib_HO->Write();
    h_RatioCalib_HO->Write();
    h_mapRatioCalib047_HO->Write();
    h_mapRatioCalib_HO->Write();
    h_TSmaxCalib_HO->Write();
    h_mapTSmaxCalib047_HO->Write();
    h_mapTSmaxCalib_HO->Write();
    h_TSmeanCalib_HO->Write();
    h_mapTSmeanCalib047_HO->Write();
    h_mapTSmeanCalib_HO->Write();
    h_WidthCalib_HO->Write();
    h_mapWidthCalib047_HO->Write();
    h_mapWidthCalib_HO->Write();
    h_map_HO->Write();
    h_ADCCalib_HF->Write();
    h_ADCCalib1_HF->Write();
    h_mapADCCalib047_HF->Write();
    h_mapADCCalib_HF->Write();
    h_RatioCalib_HF->Write();
    h_mapRatioCalib047_HF->Write();
    h_mapRatioCalib_HF->Write();
    h_TSmaxCalib_HF->Write();
    h_mapTSmaxCalib047_HF->Write();
    h_mapTSmaxCalib_HF->Write();
    h_TSmeanCalib_HF->Write();
    h_mapTSmeanCalib047_HF->Write();
    h_mapTSmeanCalib_HF->Write();
    h_WidthCalib_HF->Write();
    h_mapWidthCalib047_HF->Write();
    h_mapWidthCalib_HF->Write();
    h_map_HF->Write();

    h_nls_per_run->Write();
    h_nls_per_run10->Write();
    h_nevents_per_LS->Write();
    h_nevents_per_LSzoom->Write();
    h_nevents_per_eachRealLS->Write();
    h_nevents_per_eachLS->Write();
    h_lsnumber_per_eachLS->Write();

    // for estimator0:
  h_sumPedestalLS1 ->Write()        ;
  h_2DsumPedestalLS1 ->Write()        ;
  h_sumPedestalperLS1 ->Write()        ;
  h_2D0sumPedestalLS1 ->Write()        ;
  h_sum0PedestalperLS1 ->Write()        ;

  h_sumPedestalLS2 ->Write()        ;
  h_2DsumPedestalLS2 ->Write()        ;
  h_sumPedestalperLS2 ->Write()        ;
  h_2D0sumPedestalLS2 ->Write()        ;
  h_sum0PedestalperLS2 ->Write()        ;

  h_sumPedestalLS3 ->Write()        ;
  h_2DsumPedestalLS3 ->Write()        ;
  h_sumPedestalperLS3 ->Write()        ;
  h_2D0sumPedestalLS3 ->Write()        ;
  h_sum0PedestalperLS3 ->Write()        ;

  h_sumPedestalLS4 ->Write()        ;
  h_2DsumPedestalLS4 ->Write()        ;
  h_sumPedestalperLS4 ->Write()        ;
  h_2D0sumPedestalLS4 ->Write()        ;
  h_sum0PedestalperLS4 ->Write()        ;

  h_sumPedestalLS5 ->Write()        ;
  h_2DsumPedestalLS5 ->Write()        ;
  h_sumPedestalperLS5 ->Write()        ;
  h_2D0sumPedestalLS5 ->Write()        ;
  h_sum0PedestalperLS5 ->Write()        ;

  h_sumPedestalLS6 ->Write()        ;
  h_2DsumPedestalLS6 ->Write()        ;
  h_sumPedestalperLS6 ->Write()        ;
  h_2D0sumPedestalLS6 ->Write()        ;
  h_sum0PedestalperLS6 ->Write()        ;

  h_sumPedestalLS7 ->Write()        ;
  h_2DsumPedestalLS7 ->Write()        ;
  h_sumPedestalperLS7 ->Write()        ;
  h_2D0sumPedestalLS7 ->Write()        ;
  h_sum0PedestalperLS7 ->Write()        ;

  h_sumPedestalLS8 ->Write()        ;
  h_2DsumPedestalLS8 ->Write()        ;
  h_sumPedestalperLS8 ->Write()        ;
  h_2D0sumPedestalLS8 ->Write()        ;
  h_sum0PedestalperLS8 ->Write()        ;

    // for estimator1:
    h_sumADCAmplLS1copy1->Write();
    h_sumADCAmplLS1copy2->Write();
    h_sumADCAmplLS1copy3->Write();
    h_sumADCAmplLS1copy4->Write();
    h_sumADCAmplLS1copy5->Write();
    h_sumADCAmplLS1->Write();
    h_2DsumADCAmplLS1->Write();
    h_2DsumADCAmplLS1_LSselected->Write();
    h_sumADCAmplperLS1->Write();
    h_sumCutADCAmplperLS1->Write();
    h_2D0sumADCAmplLS1->Write();
    h_sum0ADCAmplperLS1->Write();

    h_sumADCAmplLS2->Write();
    h_2DsumADCAmplLS2->Write();
    h_2DsumADCAmplLS2_LSselected->Write();
    h_sumADCAmplperLS2->Write();
    h_sumCutADCAmplperLS2->Write();
    h_2D0sumADCAmplLS2->Write();
    h_sum0ADCAmplperLS2->Write();
    
    h_sumADCAmplLS3->Write();
    h_2DsumADCAmplLS3->Write();
    h_2DsumADCAmplLS3_LSselected->Write();
    h_sumADCAmplperLS3->Write();
    h_sumCutADCAmplperLS3->Write();
    h_2D0sumADCAmplLS3->Write();
    h_sum0ADCAmplperLS3->Write();
    
    h_sumADCAmplLS4->Write();
    h_2DsumADCAmplLS4->Write();
    h_2DsumADCAmplLS4_LSselected->Write();
    h_sumADCAmplperLS4->Write();
    h_sumCutADCAmplperLS4->Write();
    h_2D0sumADCAmplLS4->Write();
    h_sum0ADCAmplperLS4->Write();
    
    h_sumADCAmplLS5->Write();
    h_2DsumADCAmplLS5->Write();
    h_2DsumADCAmplLS5_LSselected->Write();
    h_sumADCAmplperLS5->Write();
    h_sumCutADCAmplperLS5->Write();
    h_2D0sumADCAmplLS5->Write();
    h_sum0ADCAmplperLS5->Write();
    
    h_sumADCAmplLS6->Write();
    h_2DsumADCAmplLS6->Write();
    h_2DsumADCAmplLS6_LSselected->Write();
    h_sumADCAmplperLS6->Write();
    h_sumCutADCAmplperLS6->Write();
    h_2D0sumADCAmplLS6->Write();
    h_sum0ADCAmplperLS6->Write();


    h_sumADCAmplperLS1_P1 ->Write();
    h_sum0ADCAmplperLS1_P1 ->Write(); 
    h_sumADCAmplperLS1_P2  ->Write(); 
    h_sum0ADCAmplperLS1_P2 ->Write(); 
    h_sumADCAmplperLS1_M1  ->Write(); 
    h_sum0ADCAmplperLS1_M1  ->Write();
    h_sumADCAmplperLS1_M2   ->Write();
    h_sum0ADCAmplperLS1_M2  ->Write();

    h_sumADCAmplperLS3_P1 ->Write();
    h_sum0ADCAmplperLS3_P1 ->Write(); 
    h_sumADCAmplperLS3_P2  ->Write(); 
    h_sum0ADCAmplperLS3_P2 ->Write(); 
    h_sumADCAmplperLS3_M1  ->Write(); 
    h_sum0ADCAmplperLS3_M1  ->Write();
    h_sumADCAmplperLS3_M2   ->Write();
    h_sum0ADCAmplperLS3_M2  ->Write();

    h_sumADCAmplperLS6_P1 ->Write();
    h_sum0ADCAmplperLS6_P1 ->Write(); 
    h_sumADCAmplperLS6_P2  ->Write(); 
    h_sum0ADCAmplperLS6_P2 ->Write(); 
    h_sumADCAmplperLS6_M1  ->Write(); 
    h_sum0ADCAmplperLS6_M1  ->Write();
    h_sumADCAmplperLS6_M2   ->Write();
    h_sum0ADCAmplperLS6_M2  ->Write();

    h_sumADCAmplperLS8_P1 ->Write();
    h_sum0ADCAmplperLS8_P1 ->Write(); 
    h_sumADCAmplperLS8_P2  ->Write(); 
    h_sum0ADCAmplperLS8_P2 ->Write(); 
    h_sumADCAmplperLS8_M1  ->Write(); 
    h_sum0ADCAmplperLS8_M1  ->Write();
    h_sumADCAmplperLS8_M2   ->Write();
    h_sum0ADCAmplperLS8_M2  ->Write();


    
    h_sumADCAmplLS7->Write();
    h_2DsumADCAmplLS7->Write();
    h_2DsumADCAmplLS7_LSselected->Write();
    h_sumADCAmplperLS7->Write();
    h_sumCutADCAmplperLS7->Write();
    h_2D0sumADCAmplLS7->Write();
    h_sum0ADCAmplperLS7->Write();
    
    h_sumADCAmplLS8->Write();
    h_2DsumADCAmplLS8->Write();
    h_2DsumADCAmplLS8_LSselected->Write();
    h_sumADCAmplperLS8->Write();
    h_sumCutADCAmplperLS8->Write();
    h_2D0sumADCAmplLS8->Write();
    h_sum0ADCAmplperLS8->Write();
    
    // for estimator2:
    h_sumTSmeanALS1->Write();
    h_2DsumTSmeanALS1->Write();
    h_sumTSmeanAperLS1->Write();
    h_sumTSmeanAperLS1_LSselected->Write();
    h_sumCutTSmeanAperLS1->Write();
    h_2D0sumTSmeanALS1->Write();
    h_sum0TSmeanAperLS1->Write();

    h_sumTSmeanALS2->Write();
    h_2DsumTSmeanALS2->Write();
    h_sumTSmeanAperLS2->Write();
    h_sumCutTSmeanAperLS2->Write();
    h_2D0sumTSmeanALS2->Write();
    h_sum0TSmeanAperLS2->Write();
    
    h_sumTSmeanALS3->Write();
    h_2DsumTSmeanALS3->Write();
    h_sumTSmeanAperLS3->Write();
    h_sumCutTSmeanAperLS3->Write();
    h_2D0sumTSmeanALS3->Write();
    h_sum0TSmeanAperLS3->Write();
    
    h_sumTSmeanALS4->Write();
    h_2DsumTSmeanALS4->Write();
    h_sumTSmeanAperLS4->Write();
    h_sumCutTSmeanAperLS4->Write();
    h_2D0sumTSmeanALS4->Write();
    h_sum0TSmeanAperLS4->Write();
    
    h_sumTSmeanALS5->Write();
    h_2DsumTSmeanALS5->Write();
    h_sumTSmeanAperLS5->Write();
    h_sumCutTSmeanAperLS5->Write();
    h_2D0sumTSmeanALS5->Write();
    h_sum0TSmeanAperLS5->Write();
    
    h_sumTSmeanALS6->Write();
    h_2DsumTSmeanALS6->Write();
    h_sumTSmeanAperLS6->Write();
    h_sumCutTSmeanAperLS6->Write();
    h_2D0sumTSmeanALS6->Write();
    h_sum0TSmeanAperLS6->Write();
    
    h_sumTSmeanALS7->Write();
    h_2DsumTSmeanALS7->Write();
    h_sumTSmeanAperLS7->Write();
    h_sumCutTSmeanAperLS7->Write();
    h_2D0sumTSmeanALS7->Write();
    h_sum0TSmeanAperLS7->Write();
    
    h_sumTSmeanALS8->Write();
    h_2DsumTSmeanALS8->Write();
    h_sumTSmeanAperLS8->Write();
    h_sumCutTSmeanAperLS8->Write();
    h_2D0sumTSmeanALS8->Write();
    h_sum0TSmeanAperLS8->Write();
    
    // for estimator3:
    h_sumTSmaxALS1->Write();
    h_2DsumTSmaxALS1->Write();
    h_sumTSmaxAperLS1->Write();
    h_sumTSmaxAperLS1_LSselected->Write();
    h_sumCutTSmaxAperLS1->Write();
    h_2D0sumTSmaxALS1->Write();
    h_sum0TSmaxAperLS1->Write();

    h_sumTSmaxALS2->Write();
    h_2DsumTSmaxALS2->Write();
    h_sumTSmaxAperLS2->Write();
    h_sumCutTSmaxAperLS2->Write();
    h_2D0sumTSmaxALS2->Write();
    h_sum0TSmaxAperLS2->Write();
    
    h_sumTSmaxALS3->Write();
    h_2DsumTSmaxALS3->Write();
    h_sumTSmaxAperLS3->Write();
    h_sumCutTSmaxAperLS3->Write();
    h_2D0sumTSmaxALS3->Write();
    h_sum0TSmaxAperLS3->Write();
    
    h_sumTSmaxALS4->Write();
    h_2DsumTSmaxALS4->Write();
    h_sumTSmaxAperLS4->Write();
    h_sumCutTSmaxAperLS4->Write();
    h_2D0sumTSmaxALS4->Write();
    h_sum0TSmaxAperLS4->Write();
    
    h_sumTSmaxALS5->Write();
    h_2DsumTSmaxALS5->Write();
    h_sumTSmaxAperLS5->Write();
    h_sumCutTSmaxAperLS5->Write();
    h_2D0sumTSmaxALS5->Write();
    h_sum0TSmaxAperLS5->Write();
    
    h_sumTSmaxALS6->Write();
    h_2DsumTSmaxALS6->Write();
    h_sumTSmaxAperLS6->Write();
    h_sumCutTSmaxAperLS6->Write();
    h_2D0sumTSmaxALS6->Write();
    h_sum0TSmaxAperLS6->Write();
    
    h_sumTSmaxALS7->Write();
    h_2DsumTSmaxALS7->Write();
    h_sumTSmaxAperLS7->Write();
    h_sumCutTSmaxAperLS7->Write();
    h_2D0sumTSmaxALS7->Write();
    h_sum0TSmaxAperLS7->Write();
    
    h_sumTSmaxALS8->Write();
    h_2DsumTSmaxALS8->Write();
    h_sumTSmaxAperLS8->Write();
    h_sumCutTSmaxAperLS8->Write();
    h_2D0sumTSmaxALS8->Write();
    h_sum0TSmaxAperLS8->Write();
    
    // for estimator4:
    h_sumAmplitudeLS1->Write();
    h_2DsumAmplitudeLS1->Write();
    h_sumAmplitudeperLS1->Write();
    h_sumAmplitudeperLS1_LSselected->Write();
    h_sumCutAmplitudeperLS1->Write();
    h_2D0sumAmplitudeLS1->Write();
    h_sum0AmplitudeperLS1->Write();

    h_sumAmplitudeLS2->Write();
    h_2DsumAmplitudeLS2->Write();
    h_sumAmplitudeperLS2->Write();
    h_sumCutAmplitudeperLS2->Write();
    h_2D0sumAmplitudeLS2->Write();
    h_sum0AmplitudeperLS2->Write();
    
    h_sumAmplitudeLS3->Write();
    h_2DsumAmplitudeLS3->Write();
    h_sumAmplitudeperLS3->Write();
    h_sumCutAmplitudeperLS3->Write();
    h_2D0sumAmplitudeLS3->Write();
    h_sum0AmplitudeperLS3->Write();
    
    h_sumAmplitudeLS4->Write();
    h_2DsumAmplitudeLS4->Write();
    h_sumAmplitudeperLS4->Write();
    h_sumCutAmplitudeperLS4->Write();
    h_2D0sumAmplitudeLS4->Write();
    h_sum0AmplitudeperLS4->Write();
    
    h_sumAmplitudeLS5->Write();
    h_2DsumAmplitudeLS5->Write();
    h_sumAmplitudeperLS5->Write();
    h_sumCutAmplitudeperLS5->Write();
    h_2D0sumAmplitudeLS5->Write();
    h_sum0AmplitudeperLS5->Write();
    
    h_sumAmplitudeLS6->Write();
    h_2DsumAmplitudeLS6->Write();
    h_sumAmplitudeperLS6->Write();
    h_sumCutAmplitudeperLS6->Write();
    h_2D0sumAmplitudeLS6->Write();
    h_sum0AmplitudeperLS6->Write();
    
    h_sumAmplitudeLS7->Write();
    h_2DsumAmplitudeLS7->Write();
    h_sumAmplitudeperLS7->Write();
    h_sumCutAmplitudeperLS7->Write();
    h_2D0sumAmplitudeLS7->Write();
    h_sum0AmplitudeperLS7->Write();
    
    h_sumAmplitudeLS8->Write();
    h_2DsumAmplitudeLS8->Write();
    h_sumAmplitudeperLS8->Write();
    h_sumCutAmplitudeperLS8->Write();
    h_2D0sumAmplitudeLS8->Write();
    h_sum0AmplitudeperLS8->Write();
    

    // for estimator6:
    h_sumErrorBLS1->Write();
    h_sumErrorBperLS1->Write();
    h_sum0ErrorBperLS1->Write();
    h_2D0sumErrorBLS1->Write();
    h_2DsumErrorBLS1->Write();
    h_sumErrorBLS2->Write();
    h_sumErrorBperLS2->Write();
    h_sum0ErrorBperLS2->Write();
    h_2D0sumErrorBLS2->Write();
    h_2DsumErrorBLS2->Write();

    h_sumErrorBLS3->Write();
    h_sumErrorBperLS3->Write();
    h_sum0ErrorBperLS3->Write();
    h_2D0sumErrorBLS3->Write();
    h_2DsumErrorBLS3->Write();
    h_sumErrorBLS4->Write();
    h_sumErrorBperLS4->Write();
    h_sum0ErrorBperLS4->Write();
    h_2D0sumErrorBLS4->Write();
    h_2DsumErrorBLS4->Write();
    h_sumErrorBLS5->Write();
    h_sumErrorBperLS5->Write();
    h_sum0ErrorBperLS5->Write();
    h_2D0sumErrorBLS5->Write();
    h_2DsumErrorBLS5->Write();

    h_sumErrorBLS6->Write();
    h_sumErrorBperLS6->Write();
    h_sum0ErrorBperLS6->Write();
    h_2D0sumErrorBLS6->Write();
    h_2DsumErrorBLS6->Write();
    h_sumErrorBLS7->Write();
    h_sumErrorBperLS7->Write();
    h_sum0ErrorBperLS7->Write();
    h_2D0sumErrorBLS7->Write();
    h_2DsumErrorBLS7->Write();

    h_sumErrorBLS8->Write();
    h_sumErrorBperLS8->Write();
    h_sum0ErrorBperLS8->Write();
    h_2D0sumErrorBLS8->Write();
    h_2DsumErrorBLS8->Write();


    // for estimator5:
    h_sumAmplLS1->Write();
    h_2DsumAmplLS1->Write();
    h_sumAmplperLS1->Write();
    h_sumAmplperLS1_LSselected->Write();
    h_sumCutAmplperLS1->Write();
    h_2D0sumAmplLS1->Write();
    h_sum0AmplperLS1->Write();

    h_sumAmplLS2->Write();
    h_2DsumAmplLS2->Write();
    h_sumAmplperLS2->Write();
    h_sumCutAmplperLS2->Write();
    h_2D0sumAmplLS2->Write();
    h_sum0AmplperLS2->Write();
    
    h_sumAmplLS3->Write();
    h_2DsumAmplLS3->Write();
    h_sumAmplperLS3->Write();
    h_sumCutAmplperLS3->Write();
    h_2D0sumAmplLS3->Write();
    h_sum0AmplperLS3->Write();
    
    h_sumAmplLS4->Write();
    h_2DsumAmplLS4->Write();
    h_sumAmplperLS4->Write();
    h_sumCutAmplperLS4->Write();
    h_2D0sumAmplLS4->Write();
    h_sum0AmplperLS4->Write();
    
    h_sumAmplLS5->Write();
    h_2DsumAmplLS5->Write();
    h_sumAmplperLS5->Write();
    h_sumCutAmplperLS5->Write();
    h_2D0sumAmplLS5->Write();
    h_sum0AmplperLS5->Write();
    
    h_sumAmplLS6->Write();
    h_2DsumAmplLS6->Write();
    h_sumAmplperLS6->Write();
    h_sumCutAmplperLS6->Write();
    h_2D0sumAmplLS6->Write();
    h_sum0AmplperLS6->Write();

    h_RatioOccupancy_HBP->Write();
    h_RatioOccupancy_HBM->Write();
    h_RatioOccupancy_HEP->Write();
    h_RatioOccupancy_HEM->Write();
    h_RatioOccupancy_HOP->Write();
    h_RatioOccupancy_HOM->Write();
    h_RatioOccupancy_HFP->Write();
    h_RatioOccupancy_HFM->Write();

    
    h_sumAmplLS7->Write();
    h_2DsumAmplLS7->Write();
    h_sumAmplperLS7->Write();
    h_sumCutAmplperLS7->Write();
    h_2D0sumAmplLS7->Write();
    h_sum0AmplperLS7->Write();
    
    h_sumAmplLS8->Write();
    h_2DsumAmplLS8->Write();
    h_sumAmplperLS8->Write();
    h_sumCutAmplperLS8->Write();
    h_2D0sumAmplLS8->Write();
    h_sum0AmplperLS8->Write();

    h_pedestal0_HB->Write();
    h_pedestal1_HB->Write();
    h_pedestal2_HB->Write();
    h_pedestal3_HB->Write();
    h_pedestalaver4_HB->Write();
    h_pedestalaver9_HB->Write();
    h_pedestalw0_HB->Write();
    h_pedestalw1_HB->Write();
    h_pedestalw2_HB->Write();
    h_pedestalw3_HB->Write();
    h_pedestalwaver4_HB->Write();
    h_pedestalwaver9_HB->Write();
    
    h_pedestal0_HE->Write();
    h_pedestal1_HE->Write();
    h_pedestal2_HE->Write();
    h_pedestal3_HE->Write();
    h_pedestalaver4_HE->Write();
    h_pedestalaver9_HE->Write();
    h_pedestalw0_HE->Write();
    h_pedestalw1_HE->Write();
    h_pedestalw2_HE->Write();
    h_pedestalw3_HE->Write();
    h_pedestalwaver4_HE->Write();
    h_pedestalwaver9_HE->Write();
    
    h_pedestal0_HF->Write();
    h_pedestal1_HF->Write();
    h_pedestal2_HF->Write();
    h_pedestal3_HF->Write();
    h_pedestalaver4_HF->Write();
    h_pedestalaver9_HF->Write();
    h_pedestalw0_HF->Write();
    h_pedestalw1_HF->Write();
    h_pedestalw2_HF->Write();
    h_pedestalw3_HF->Write();
    h_pedestalwaver4_HF->Write();
    h_pedestalwaver9_HF->Write();
    
    h_pedestal0_HO->Write();
    h_pedestal1_HO->Write();
    h_pedestal2_HO->Write();
    h_pedestal3_HO->Write();
    h_pedestalaver4_HO->Write();
    h_pedestalaver9_HO->Write();
    h_pedestalw0_HO->Write();
    h_pedestalw1_HO->Write();
    h_pedestalw2_HO->Write();
    h_pedestalw3_HO->Write();
    h_pedestalwaver4_HO->Write();
    h_pedestalwaver9_HO->Write();
    
    h_mapDepth1pedestalw_HB->Write();
    h_mapDepth2pedestalw_HB->Write();
    h_mapDepth1pedestalw_HE->Write();
    h_mapDepth2pedestalw_HE->Write();
    h_mapDepth3pedestalw_HE->Write();
    h_mapDepth1pedestalw_HF->Write();
    h_mapDepth2pedestalw_HF->Write();
    h_mapDepth4pedestalw_HO->Write();
    
    h_mapDepth1pedestal_HB->Write();
    h_mapDepth2pedestal_HB->Write();
    h_mapDepth1pedestal_HE->Write();
    h_mapDepth2pedestal_HE->Write();
    h_mapDepth3pedestal_HE->Write();
    h_mapDepth1pedestal_HF->Write();
    h_mapDepth2pedestal_HF->Write();
    h_mapDepth4pedestal_HO->Write();
    
    h_pedestal00_HB->Write();
    h_gain_HB->Write();
    h_respcorr_HB->Write();
    h_timecorr_HB->Write();
    h_lutcorr_HB->Write();
    h_difpedestal0_HB->Write();
    h_difpedestal1_HB->Write();
    h_difpedestal2_HB->Write();
    h_difpedestal3_HB->Write();

    h_pedestal00_HE->Write();
    h_gain_HE->Write();
    h_respcorr_HE->Write();
    h_timecorr_HE->Write();
    h_lutcorr_HE->Write();
    
    h_pedestal00_HF->Write();
    h_gain_HF->Write();
    h_respcorr_HF->Write();
    h_timecorr_HF->Write();
    h_lutcorr_HF->Write();
    
    h_pedestal00_HO->Write();
    h_gain_HO->Write();
    h_respcorr_HO->Write();
    h_timecorr_HO->Write();
    h_lutcorr_HO->Write();
    
    h2_pedvsampl_HB->Write();
    h2_pedwvsampl_HB->Write();
    h_pedvsampl_HB->Write();
    h_pedwvsampl_HB->Write();
    h_pedvsampl0_HB->Write();
    h_pedwvsampl0_HB->Write();
    h2_amplvsped_HB->Write();
    h2_amplvspedw_HB->Write();
    h_amplvsped_HB->Write();
    h_amplvspedw_HB->Write();
    h_amplvsped0_HB->Write();

    h2_pedvsampl_HE->Write();
    h2_pedwvsampl_HE->Write();
    h_pedvsampl_HE->Write();
    h_pedwvsampl_HE->Write();
    h_pedvsampl0_HE->Write();
    h_pedwvsampl0_HE->Write();
    h2_pedvsampl_HF->Write();
    h2_pedwvsampl_HF->Write();
    h_pedvsampl_HF->Write();
    h_pedwvsampl_HF->Write();
    h_pedvsampl0_HF->Write();
    h_pedwvsampl0_HF->Write();
    h2_pedvsampl_HO->Write();
    h2_pedwvsampl_HO->Write();
    h_pedvsampl_HO->Write();
    h_pedwvsampl_HO->Write();
    h_pedvsampl0_HO->Write();
    h_pedwvsampl0_HO->Write();

    h_mapDepth1Ped0_HB->Write();
    h_mapDepth1Ped1_HB->Write();
    h_mapDepth1Ped2_HB->Write();
    h_mapDepth1Ped3_HB->Write();
    h_mapDepth1Pedw0_HB->Write();
    h_mapDepth1Pedw1_HB->Write();
    h_mapDepth1Pedw2_HB->Write();
    h_mapDepth1Pedw3_HB->Write();
    h_mapDepth2Ped0_HB->Write();
    h_mapDepth2Ped1_HB->Write();
    h_mapDepth2Ped2_HB->Write();
    h_mapDepth2Ped3_HB->Write();
    h_mapDepth2Pedw0_HB->Write();
    h_mapDepth2Pedw1_HB->Write();
    h_mapDepth2Pedw2_HB->Write();
    h_mapDepth2Pedw3_HB->Write();

    h_mapDepth1Ped0_HE->Write();
    h_mapDepth1Ped1_HE->Write();
    h_mapDepth1Ped2_HE->Write();
    h_mapDepth1Ped3_HE->Write();
    h_mapDepth1Pedw0_HE->Write();
    h_mapDepth1Pedw1_HE->Write();
    h_mapDepth1Pedw2_HE->Write();
    h_mapDepth1Pedw3_HE->Write();
    h_mapDepth2Ped0_HE->Write();
    h_mapDepth2Ped1_HE->Write();
    h_mapDepth2Ped2_HE->Write();
    h_mapDepth2Ped3_HE->Write();
    h_mapDepth2Pedw0_HE->Write();
    h_mapDepth2Pedw1_HE->Write();
    h_mapDepth2Pedw2_HE->Write();
    h_mapDepth2Pedw3_HE->Write();
    h_mapDepth3Ped0_HE->Write();
    h_mapDepth3Ped1_HE->Write();
    h_mapDepth3Ped2_HE->Write();
    h_mapDepth3Ped3_HE->Write();
    h_mapDepth3Pedw0_HE->Write();
    h_mapDepth3Pedw1_HE->Write();
    h_mapDepth3Pedw2_HE->Write();
    h_mapDepth3Pedw3_HE->Write();

    h_mapDepth1Ped0_HF->Write();
    h_mapDepth1Ped1_HF->Write();
    h_mapDepth1Ped2_HF->Write();
    h_mapDepth1Ped3_HF->Write();
    h_mapDepth1Pedw0_HF->Write();
    h_mapDepth1Pedw1_HF->Write();
    h_mapDepth1Pedw2_HF->Write();
    h_mapDepth1Pedw3_HF->Write();
    h_mapDepth2Ped0_HF->Write();
    h_mapDepth2Ped1_HF->Write();
    h_mapDepth2Ped2_HF->Write();
    h_mapDepth2Ped3_HF->Write();
    h_mapDepth2Pedw0_HF->Write();
    h_mapDepth2Pedw1_HF->Write();
    h_mapDepth2Pedw2_HF->Write();
    h_mapDepth2Pedw3_HF->Write();

    h_mapDepth4Ped0_HO->Write();
    h_mapDepth4Ped1_HO->Write();
    h_mapDepth4Ped2_HO->Write();
    h_mapDepth4Ped3_HO->Write();
    h_mapDepth4Pedw0_HO->Write();
    h_mapDepth4Pedw1_HO->Write();
    h_mapDepth4Pedw2_HO->Write();
    h_mapDepth4Pedw3_HO->Write();


    h_mapGetRMSOverNormalizedSignal_HB->Write();
    h_mapGetRMSOverNormalizedSignal0_HB->Write();
    h_mapGetRMSOverNormalizedSignal_HE->Write();
    h_mapGetRMSOverNormalizedSignal0_HE->Write();
    h_mapGetRMSOverNormalizedSignal_HF->Write();
    h_mapGetRMSOverNormalizedSignal0_HF->Write();
    h_mapGetRMSOverNormalizedSignal_HO->Write();
    h_mapGetRMSOverNormalizedSignal0_HO->Write();

    h_shape_Ahigh_HB0->Write();
    h_shape0_Ahigh_HB0->Write();
    h_shape_Alow_HB0->Write();
    h_shape0_Alow_HB0->Write();
    h_shape_Ahigh_HB1->Write();
    h_shape0_Ahigh_HB1->Write();
    h_shape_Alow_HB1->Write();
    h_shape0_Alow_HB1->Write();
    h_shape_Ahigh_HB2->Write();
    h_shape0_Ahigh_HB2->Write();
    h_shape_Alow_HB2->Write();
    h_shape0_Alow_HB2->Write();
    h_shape_Ahigh_HB3->Write();
    h_shape0_Ahigh_HB3->Write();
    h_shape_Alow_HB3->Write();
    h_shape0_Alow_HB3->Write();

    h_shape_bad_channels_HB->Write();
    h_shape0_bad_channels_HB->Write();
    h_shape_good_channels_HB->Write();
    h_shape0_good_channels_HB->Write();
    h_shape_bad_channels_HE->Write();
    h_shape0_bad_channels_HE->Write();
    h_shape_good_channels_HE->Write();
    h_shape0_good_channels_HE->Write();
    h_shape_bad_channels_HF->Write();
    h_shape0_bad_channels_HF->Write();
    h_shape_good_channels_HF->Write();
    h_shape0_good_channels_HF->Write();
    h_shape_bad_channels_HO->Write();
    h_shape0_bad_channels_HO->Write();
    h_shape_good_channels_HO->Write();
    h_shape0_good_channels_HO->Write();
    
    h_sumamplitude_depth1_HB->Write();
    h_sumamplitude_depth2_HB->Write();
    h_sumamplitude_depth1_HE->Write();
    h_sumamplitude_depth2_HE->Write();
    h_sumamplitude_depth3_HE->Write();
    h_sumamplitude_depth1_HF->Write();
    h_sumamplitude_depth2_HF->Write();
    h_sumamplitude_depth4_HO->Write();
    
    h_sumamplitude_depth1_HB0->Write();
    h_sumamplitude_depth2_HB0->Write();
    h_sumamplitude_depth1_HE0->Write();
    h_sumamplitude_depth2_HE0->Write();
    h_sumamplitude_depth3_HE0->Write();
    h_sumamplitude_depth1_HF0->Write();
    h_sumamplitude_depth2_HF0->Write();
    h_sumamplitude_depth4_HO0->Write();
    
    h_sumamplitude_depth1_HB1->Write();
    h_sumamplitude_depth2_HB1->Write();
    h_sumamplitude_depth1_HE1->Write();
    h_sumamplitude_depth2_HE1->Write();
    h_sumamplitude_depth3_HE1->Write();
    h_sumamplitude_depth1_HF1->Write();
    h_sumamplitude_depth2_HF1->Write();
    h_sumamplitude_depth4_HO1->Write();

    h_ADC_HB->Write();
    h_ADC_HE->Write();
    h_ADC_HF->Write();
    h_ADC_HO->Write();

    h_ADC_HBdepth1->Write();
    h_ADC_HBdepth2->Write();
    h_ADC_HEdepth1->Write();
    h_ADC_HEdepth2->Write();
    h_ADC_HEdepth3->Write();
    h_ADC_HFdepth1->Write();
    h_ADC_HFdepth2->Write();
    h_ADC_HOdepth4->Write();

    h_ADC_HBdepth1_TS0->Write();
    h_ADC_HBdepth1_TS1->Write();
    h_ADC_HBdepth1_TS2->Write();
    h_ADC_HBdepth1_TS3->Write();
    h_ADC_HBdepth1_TS4->Write();
    h_ADC_HBdepth1_TS5->Write();
    h_ADC_HBdepth1_TS6->Write();
    h_ADC_HBdepth1_TS7->Write();
    h_ADC_HBdepth1_TS8->Write();
    h_ADC_HBdepth1_TS9->Write();
    h_ADC_HBdepth2_TS0->Write();
    h_ADC_HBdepth2_TS1->Write();
    h_ADC_HBdepth2_TS2->Write();
    h_ADC_HBdepth2_TS3->Write();
    h_ADC_HBdepth2_TS4->Write();
    h_ADC_HBdepth2_TS5->Write();
    h_ADC_HBdepth2_TS6->Write();
    h_ADC_HBdepth2_TS7->Write();
    h_ADC_HBdepth2_TS8->Write();
    h_ADC_HBdepth2_TS9->Write();
    
    h_ADC_HEdepth1_TS0->Write();
    h_ADC_HEdepth1_TS1->Write();
    h_ADC_HEdepth1_TS2->Write();
    h_ADC_HEdepth1_TS3->Write();
    h_ADC_HEdepth1_TS4->Write();
    h_ADC_HEdepth1_TS5->Write();
    h_ADC_HEdepth1_TS6->Write();
    h_ADC_HEdepth1_TS7->Write();
    h_ADC_HEdepth1_TS8->Write();
    h_ADC_HEdepth1_TS9->Write();
    h_ADC_HEdepth2_TS0->Write();
    h_ADC_HEdepth2_TS1->Write();
    h_ADC_HEdepth2_TS2->Write();
    h_ADC_HEdepth2_TS3->Write();
    h_ADC_HEdepth2_TS4->Write();
    h_ADC_HEdepth2_TS5->Write();
    h_ADC_HEdepth2_TS6->Write();
    h_ADC_HEdepth2_TS7->Write();
    h_ADC_HEdepth2_TS8->Write();
    h_ADC_HEdepth2_TS9->Write();
    h_ADC_HEdepth3_TS0->Write();
    h_ADC_HEdepth3_TS1->Write();
    h_ADC_HEdepth3_TS2->Write();
    h_ADC_HEdepth3_TS3->Write();
    h_ADC_HEdepth3_TS4->Write();
    h_ADC_HEdepth3_TS5->Write();
    h_ADC_HEdepth3_TS6->Write();
    h_ADC_HEdepth3_TS7->Write();
    h_ADC_HEdepth3_TS8->Write();
    h_ADC_HEdepth3_TS9->Write();
    
    h_ADC_HFdepth1_TS0->Write();
    h_ADC_HFdepth1_TS1->Write();
    h_ADC_HFdepth1_TS2->Write();
    h_ADC_HFdepth1_TS3->Write();
    h_ADC_HFdepth1_TS4->Write();
    h_ADC_HFdepth1_TS5->Write();
    h_ADC_HFdepth1_TS6->Write();
    h_ADC_HFdepth1_TS7->Write();
    h_ADC_HFdepth1_TS8->Write();
    h_ADC_HFdepth1_TS9->Write();
    h_ADC_HFdepth2_TS0->Write();
    h_ADC_HFdepth2_TS1->Write();
    h_ADC_HFdepth2_TS2->Write();
    h_ADC_HFdepth2_TS3->Write();
    h_ADC_HFdepth2_TS4->Write();
    h_ADC_HFdepth2_TS5->Write();
    h_ADC_HFdepth2_TS6->Write();
    h_ADC_HFdepth2_TS7->Write();
    h_ADC_HFdepth2_TS8->Write();
    h_ADC_HFdepth2_TS9->Write();
    
    h_ADC_HOdepth4_TS0->Write();
    h_ADC_HOdepth4_TS1->Write();
    h_ADC_HOdepth4_TS2->Write();
    h_ADC_HOdepth4_TS3->Write();
    h_ADC_HOdepth4_TS4->Write();
    h_ADC_HOdepth4_TS5->Write();
    h_ADC_HOdepth4_TS6->Write();
    h_ADC_HOdepth4_TS7->Write();
    h_ADC_HOdepth4_TS8->Write();
    h_ADC_HOdepth4_TS9->Write();

    h_bcnnbadchannels_depth1_HB->Write();
    h_bcnnbadchannels_depth2_HB->Write();
    h_bcnnbadchannels_depth1_HE->Write();
    h_bcnnbadchannels_depth2_HE->Write();
    h_bcnnbadchannels_depth3_HE->Write();
    h_bcnnbadchannels_depth4_HO->Write();
    h_bcnnbadchannels_depth1_HF->Write();
    h_bcnnbadchannels_depth2_HF->Write();
    h_bcnbadrate0_depth1_HB->Write();
    h_bcnbadrate0_depth2_HB->Write();
    h_bcnbadrate0_depth1_HE->Write();
    h_bcnbadrate0_depth2_HE->Write();
    h_bcnbadrate0_depth3_HE->Write();
    h_bcnbadrate0_depth4_HO->Write();
    h_bcnbadrate0_depth1_HF->Write();
    h_bcnbadrate0_depth2_HF->Write();


    h_Amplitude_forCapIdErrors_HB1->Write();
    h_Amplitude_forCapIdErrors_HB2->Write();
    h_Amplitude_forCapIdErrors_HE1->Write();
    h_Amplitude_forCapIdErrors_HE2->Write();
    h_Amplitude_forCapIdErrors_HE3->Write();
    h_Amplitude_forCapIdErrors_HF1->Write();
    h_Amplitude_forCapIdErrors_HF2->Write();
    h_Amplitude_forCapIdErrors_HO4->Write();

    h_Amplitude_notCapIdErrors_HB1->Write();
    h_Amplitude_notCapIdErrors_HB2->Write();
    h_Amplitude_notCapIdErrors_HE1->Write();
    h_Amplitude_notCapIdErrors_HE2->Write();
    h_Amplitude_notCapIdErrors_HE3->Write();
    h_Amplitude_notCapIdErrors_HF1->Write();
    h_Amplitude_notCapIdErrors_HF2->Write();
    h_Amplitude_notCapIdErrors_HO4->Write();

    h_averSIGNALoccupancy_HB->Write();
    h_averSIGNALoccupancy_HE->Write();
    h_averSIGNALoccupancy_HF->Write();
    h_averSIGNALoccupancy_HO->Write();

    h_averSIGNALsumamplitude_HB->Write();
    h_averSIGNALsumamplitude_HE->Write();
    h_averSIGNALsumamplitude_HF->Write();
    h_averSIGNALsumamplitude_HO->Write();

    h_averNOSIGNALoccupancy_HB->Write();
    h_averNOSIGNALoccupancy_HE->Write();
    h_averNOSIGNALoccupancy_HF->Write();
    h_averNOSIGNALoccupancy_HO->Write();

    h_averNOSIGNALsumamplitude_HB->Write();
    h_averNOSIGNALsumamplitude_HE->Write();
    h_averNOSIGNALsumamplitude_HF->Write();
    h_averNOSIGNALsumamplitude_HO->Write();

    h_maxxSUMAmpl_HB->Write();
    h_maxxSUMAmpl_HE->Write();
    h_maxxSUMAmpl_HF->Write();
    h_maxxSUMAmpl_HO->Write();

    h_maxxOCCUP_HB->Write();
    h_maxxOCCUP_HE->Write();
    h_maxxOCCUP_HF->Write();
    h_maxxOCCUP_HO->Write();

    h_sumamplitudechannel_HB->Write();
    h_sumamplitudechannel_HE->Write();
    h_sumamplitudechannel_HF->Write();
    h_sumamplitudechannel_HO->Write();

    h_eventamplitude_HB->Write();
    h_eventamplitude_HE->Write();
    h_eventamplitude_HF->Write();
    h_eventamplitude_HO->Write();
    
    h_eventoccupancy_HB->Write();
    h_eventoccupancy_HE->Write();
    h_eventoccupancy_HF->Write();
    h_eventoccupancy_HO->Write();


    h_2DAtaildepth1_HB->Write();
    h_2D0Ataildepth1_HB->Write();
    h_2DAtaildepth2_HB->Write();
    h_2D0Ataildepth2_HB->Write();

    ///////////////////////
  }//if
  ///////////////////////
  hOutputFile->Close() ;
  std::cout << "===== Finish writing user histograms and ntuple =====" << std::endl;
  ///////////////////////
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  SERVICE FUNCTIONS --------------------------------------------------------

double VeRawAnalyzer::dR(double eta1, double phi1, double eta2, double phi2) { 
  double PI = 3.1415926535898;
  double deltaphi= phi1 - phi2;
  if( phi2 > phi1 ) { deltaphi= phi2 - phi1;}
  if(deltaphi > PI) { deltaphi = 2.*PI - deltaphi;}
  double deltaeta = eta2 - eta1;
  double tmp = sqrt(deltaeta* deltaeta + deltaphi*deltaphi);
  return tmp;
}

double VeRawAnalyzer::phi12(double phi1, double en1, double phi2, double en2) {
  // weighted mean value of phi1 and phi2
  
  double tmp;
  double PI = 3.1415926535898;
  double a1 = phi1; double a2  = phi2;

  if( a1 > 0.5*PI  && a2 < 0.) a2 += 2*PI;
  if( a2 > 0.5*PI  && a1 < 0.) a1 += 2*PI; 
  tmp = (a1 * en1 + a2 * en2)/(en1 + en2);
  if(tmp > PI) tmp -= 2.*PI; 
 
  return tmp;

}

double VeRawAnalyzer::dPhiWsign(double phi1, double phi2) {
  // clockwise      phi2 w.r.t phi1 means "+" phi distance
  // anti-clockwise phi2 w.r.t phi1 means "-" phi distance 

  double PI = 3.1415926535898;
  double a1 = phi1; double a2  = phi2;
  double tmp =  a2 - a1;
  if( a1*a2 < 0.) {
    if(a1 > 0.5 * PI)  tmp += 2.*PI;
    if(a2 > 0.5 * PI)  tmp -= 2.*PI;
  }
  return tmp;

}

void VeRawAnalyzer::fillMAP() {

    HcalLogicalMapGenerator gen;
    HcalLogicalMap lmap=gen.createMap(topo); 
  
    HcalElectronicsMap emap=lmap.generateHcalElectronicsMap();
    std::string subdet =""; 

    MAPfile << "#define LogEleMapdb_h" << std::endl;
    MAPfile << "#include <algorithm>" << std::endl;
    MAPfile << "#include <iostream>" << std::endl;
    MAPfile << "#include <vector>" << std::endl;
    MAPfile << "#include <string>" << std::endl;
    MAPfile << "#include <sstream>" << std::endl;
    MAPfile <<  std::endl;

    MAPfile << "struct Cell {" << std::endl;
    MAPfile << " std::string subdet;" << std::endl;    
    MAPfile << " int Eta;" << std::endl;             
    MAPfile << " int Phi;" << std::endl;           
    MAPfile << " int Depth;" << std::endl;           
    MAPfile << " std::string RBX;" << std::endl;
    MAPfile << " int RM;" << std::endl;             
    MAPfile << " int Pixel;" << std::endl;
    MAPfile << " int RMfiber;" << std::endl;
    MAPfile << " int FiberCh;" << std::endl;
    MAPfile << " int QIE;" << std::endl;
    MAPfile << " int ADC;" << std::endl;
    MAPfile << " int VMECardID;" << std::endl;
    MAPfile << " int dccID;" << std::endl;
    MAPfile << " int Spigot;" << std::endl;
    MAPfile << " int FiberIndex;" << std::endl;
    MAPfile << " int HtrSlot;" << std::endl;
    MAPfile << " int HtrTB;" << std::endl;
    MAPfile <<  std::endl;

    MAPfile << "// the function check, if \"par\" == \"val\" for this cell" << std::endl;
    MAPfile << " bool check(const std::string par, const int val) const " << std::endl;
    MAPfile << " {" << std::endl;
    MAPfile << "       if (par == \"Eta\")    return (val == Eta);" << std::endl;
    MAPfile << "  else if (par == \"Phi\")     return (val == Phi);" << std::endl;
    MAPfile << "  else if (par == \"Depth\")      return (val == Depth);" << std::endl;
    MAPfile << "  else if (par == \"RM\")     return (val == RM);" << std::endl;
    MAPfile << "  else if (par == \"Pixel\") return (val == Pixel);" << std::endl;
    MAPfile << "  else if (par == \"RMfiber\")    return (val == RMfiber);" << std::endl;
    MAPfile << "  else if (par == \"FiberCh\")    return (val == FiberCh);" << std::endl;
    MAPfile << "  else if (par == \"QIE\")     return (val == QIE);" << std::endl;
    MAPfile << "  else if (par == \"ADC\")     return (val == ADC);" << std::endl;
    MAPfile << "  else if (par == \"VMECardID\")     return (val == VMECardID);" << std::endl;
    MAPfile << "  else if (par == \"dccID\")     return (val == dccID);" << std::endl;
    MAPfile << "  else if (par == \"Spigot\")     return (val == Spigot);" << std::endl;
    MAPfile << "  else if (par == \"FiberIndex\")     return (val == FiberIndex);" << std::endl;
    MAPfile << "  else if (par == \"HtrSlot\")     return (val == HtrSlot);" << std::endl;
    MAPfile << "  else if (par == \"HtrTB\")     return (val == HtrTB);" << std::endl;
    MAPfile << "  else return false;" << std::endl;
    MAPfile << " }" << std::endl;
    MAPfile <<  std::endl;

    MAPfile << " bool check(const std::string par, const std::string val) const" << std::endl;
    MAPfile << " {" << std::endl;
    MAPfile << "       if (par == \"subdet\")    return (val == subdet);" << std::endl;
    MAPfile << "  else if (par == \"RBX\")    return (val == RBX);" << std::endl;
    MAPfile << "  else return false;" << std::endl;
    MAPfile << " }" << std::endl;

    MAPfile << "};" << std::endl;
    MAPfile <<  std::endl;  

    MAPfile << "const Cell AllCells[] = {" << std::endl; 
    MAPfile << "//{ SD, Eta, Phi, Depth,     RBX, RM, PIXEL, RMfiber, Fiber Ch., QIE, ADC, VMECrateId, dccid, spigot, fiberIndex, htrSlot, htrTopBottom }" << std::endl;     

// HB

        for (int eta= -16;eta<0;eta++) {
          for (int phi=1;phi<=72;phi++) {
             for (int depth=1;depth<=2;depth++) {
                HcalDetId *detid=0;
                detid=new HcalDetId(HcalBarrel,eta,phi,depth); subdet="HB";
                HcalFrontEndId    lmap_entry=lmap.getHcalFrontEndId(*detid);
                HcalElectronicsId emap_entry=emap.lookup(*detid);
                MAPfile << "  {\""<<subdet<<"\" , "<< detid->ieta()<<" , "<< detid->iphi()-1<<" ,    "<< detid->depth()<<" ," ;    
                MAPfile << "\""<<lmap_entry.rbx()<<"\" , "<<lmap_entry.rm()<<" ,   "<<lmap_entry.pixel()<<" ,      "<<lmap_entry.rmFiber()<<" ,        " ;
                MAPfile << lmap_entry.fiberChannel()<<" ,  "<<lmap_entry.qieCard()<<" ,  "<<lmap_entry.adc()<<" ,        ";    
                MAPfile << emap_entry.readoutVMECrateId()<<" ,    "<<emap_entry.dccid()<<" ,     "<<emap_entry.spigot()<<" ,         "<<emap_entry.fiberIndex()<<" ,      " ;
                MAPfile << emap_entry.htrSlot()<<" ,      "<<emap_entry.htrTopBottom() ;
                MAPfile << "}," << std::endl;           
                delete detid;
             }  //Depth
          }  //Phi
       }  //Eta    
        for (int eta= 1;eta<=16;eta++) {
          for (int phi=1;phi<=72;phi++) {
             for (int depth=1;depth<=2;depth++) {
                HcalDetId *detid=0;
                detid=new HcalDetId(HcalBarrel,eta,phi,depth); subdet="HB";
                HcalFrontEndId    lmap_entry=lmap.getHcalFrontEndId(*detid);
                HcalElectronicsId emap_entry=emap.lookup(*detid);
                MAPfile << "  {\""<<subdet<<"\" , "<< detid->ieta()-1<<" , "<< detid->iphi()-1<<" ,    "<< detid->depth()<<" ," ;    
                MAPfile << "\""<<lmap_entry.rbx()<<"\" , "<<lmap_entry.rm()<<" ,   "<<lmap_entry.pixel()<<" ,      "<<lmap_entry.rmFiber()<<" ,        " ;
                MAPfile << lmap_entry.fiberChannel()<<" ,  "<<lmap_entry.qieCard()<<" ,  "<<lmap_entry.adc()<<" ,        ";    
                MAPfile << emap_entry.readoutVMECrateId()<<" ,    "<<emap_entry.dccid()<<" ,     "<<emap_entry.spigot()<<" ,         "<<emap_entry.fiberIndex()<<" ,      " ;
                MAPfile << emap_entry.htrSlot()<<" ,      "<<emap_entry.htrTopBottom() ;
                MAPfile << "}," << std::endl;           
                delete detid;
             }  //Depth
          }  //Phi
       }  //Eta 

// HE
      for (int eta= -20;eta<=-20;eta++) {
          for (int phi=72;phi<=72;phi++) {
             for (int depth=1;depth<=2;depth++) {
                HcalDetId *detid=0;
                detid=new HcalDetId(HcalEndcap,eta,phi,depth); subdet="HE";
                HcalFrontEndId    lmap_entry=lmap.getHcalFrontEndId(*detid);
                HcalElectronicsId emap_entry=emap.lookup(*detid);
                MAPfile << "  {\""<<subdet<<"\" , "<< detid->ieta()<<" , "<< detid->iphi()-1<<" ,    "<< detid->depth()<<" ," ;    
                MAPfile << "\""<<lmap_entry.rbx()<<"\" , "<<lmap_entry.rm()<<" ,   "<<lmap_entry.pixel()<<" ,      "<<lmap_entry.rmFiber()<<" ,        " ;
                MAPfile << lmap_entry.fiberChannel()<<" ,  "<<lmap_entry.qieCard()<<" ,  "<<lmap_entry.adc()<<" ,        ";    
                MAPfile << emap_entry.readoutVMECrateId()<<" ,    "<<emap_entry.dccid()<<" ,     "<<emap_entry.spigot()<<" ,         "<<emap_entry.fiberIndex()<<" ,      " ;
                MAPfile << emap_entry.htrSlot()<<" ,      "<<emap_entry.htrTopBottom() ;
                MAPfile << "}," << std::endl;           
                delete detid;
             }  //Depth
          }  //Phi
       }  //Eta

      for (int eta= -19;eta<=-16;eta++) {
          for (int phi=72;phi<=72;phi++) {
             for (int depth=1;depth<=3;depth++) {
                HcalDetId *detid=0;
                detid=new HcalDetId(HcalEndcap,eta,phi,depth); subdet="HE";
                HcalFrontEndId    lmap_entry=lmap.getHcalFrontEndId(*detid);
                HcalElectronicsId emap_entry=emap.lookup(*detid);
                MAPfile << "  {\""<<subdet<<"\" , "<< detid->ieta()<<" , "<< detid->iphi()-1<<" ,    "<< detid->depth()<<" ," ;    
                MAPfile << "\""<<lmap_entry.rbx()<<"\" , "<<lmap_entry.rm()<<" ,   "<<lmap_entry.pixel()<<" ,      "<<lmap_entry.rmFiber()<<" ,        " ;
                MAPfile << lmap_entry.fiberChannel()<<" ,  "<<lmap_entry.qieCard()<<" ,  "<<lmap_entry.adc()<<" ,        ";    
                MAPfile << emap_entry.readoutVMECrateId()<<" ,    "<<emap_entry.dccid()<<" ,     "<<emap_entry.spigot()<<" ,         "<<emap_entry.fiberIndex()<<" ,      " ;
                MAPfile << emap_entry.htrSlot()<<" ,      "<<emap_entry.htrTopBottom() ;
                MAPfile << "}," << std::endl;           
                delete detid;
             }  //Depth
          }  //Phi
       }  //Eta
      for (int eta= -29;eta<=-16;eta++) {
          for (int phi=1;phi<=71;phi++) {
             for (int depth=1;depth<=3;depth++) {
                HcalDetId *detid=0;
                detid=new HcalDetId(HcalEndcap,eta,phi,depth); subdet="HE";
                HcalFrontEndId    lmap_entry=lmap.getHcalFrontEndId(*detid);
                HcalElectronicsId emap_entry=emap.lookup(*detid);
                MAPfile << "  {\""<<subdet<<"\" , "<< detid->ieta()<<" , "<< detid->iphi()-1 <<" ,    "<< detid->depth()<<" ," ;    
                MAPfile << "\""<<lmap_entry.rbx()<<"\" , "<<lmap_entry.rm()<<" ,   "<<lmap_entry.pixel()<<" ,      "<<lmap_entry.rmFiber()<<" ,        " ;
                MAPfile << lmap_entry.fiberChannel()<<" ,  "<<lmap_entry.qieCard()<<" ,  "<<lmap_entry.adc()<<" ,        ";    
                MAPfile << emap_entry.readoutVMECrateId()<<" ,    "<<emap_entry.dccid()<<" ,     "<<emap_entry.spigot()<<" ,         "<<emap_entry.fiberIndex()<<" ,      " ;
                MAPfile << emap_entry.htrSlot()<<" ,      "<<emap_entry.htrTopBottom() ;
                MAPfile << "}," << std::endl;           
                delete detid;
             }  //Depth
          }  //Phi
       }  //Eta
         for (int eta= 16;eta<=29;eta++) {
          for (int phi=1;phi<=72;phi++) {
             for (int depth=1;depth<=3;depth++) {
                HcalDetId *detid=0;
                detid=new HcalDetId(HcalEndcap,eta,phi,depth); subdet="HE";
                HcalFrontEndId    lmap_entry=lmap.getHcalFrontEndId(*detid);
                HcalElectronicsId emap_entry=emap.lookup(*detid);
                MAPfile << "  {\""<<subdet<<"\" , "<< detid->ieta()<<" , "<< detid->iphi()-1 <<" ,    "<< detid->depth()<<" ," ;    
                MAPfile << "\""<<lmap_entry.rbx()<<"\" , "<<lmap_entry.rm()<<" ,   "<<lmap_entry.pixel()<<" ,      "<<lmap_entry.rmFiber()<<" ,        " ;
                MAPfile << lmap_entry.fiberChannel()<<" ,  "<<lmap_entry.qieCard()<<" ,  "<<lmap_entry.adc()<<" ,        ";    
                MAPfile << emap_entry.readoutVMECrateId()<<" ,    "<<emap_entry.dccid()<<" ,     "<<emap_entry.spigot()<<" ,         "<<emap_entry.fiberIndex()<<" ,      " ;
                MAPfile << emap_entry.htrSlot()<<" ,      "<<emap_entry.htrTopBottom() ;
                MAPfile << "}," << std::endl;           
                delete detid;
             }  //Depth
          }  //Phi
       }  //Eta

// HF

        for (int eta= -41;eta<=-29;eta++) {
          for (int phi=1;phi<=72;phi+=2) {
             for (int depth=1;depth<=2;depth++) {
                HcalDetId *detid=0;
                detid=new HcalDetId(HcalForward,eta,phi,depth); subdet="HF";
                HcalFrontEndId    lmap_entry=lmap.getHcalFrontEndId(*detid);
                HcalElectronicsId emap_entry=emap.lookup(*detid);
                MAPfile << "  {\""<<subdet<<"\" , "<< detid->ieta()<<" , "<< detid->iphi()-1 <<" ,    "<< detid->depth()<<" ," ;    
                MAPfile << "\""<<lmap_entry.rbx()<<"\" , "<<lmap_entry.rm()<<" ,   "<<lmap_entry.pixel()<<" ,      "<<lmap_entry.rmFiber()<<" ,        " ;
                MAPfile << lmap_entry.fiberChannel()<<" ,  "<<lmap_entry.qieCard()<<" ,  "<<lmap_entry.adc()<<" ,        ";    
                MAPfile << emap_entry.readoutVMECrateId()<<" ,    "<<emap_entry.dccid()<<" ,     "<<emap_entry.spigot()<<" ,         "<<emap_entry.fiberIndex()<<" ,      " ;
                MAPfile << emap_entry.htrSlot()<<" ,      "<<emap_entry.htrTopBottom() ;
                MAPfile << "}," << std::endl;           
                delete detid;
             }  //Depth
          }  //Phi
       }  //Eta

        for (int eta= 29;eta<=41 ;eta++) {
          for (int phi=1;phi<=72;phi+=2) {
             for (int depth=1;depth<=2;depth++) {
                HcalDetId *detid=0;
                detid=new HcalDetId(HcalForward,eta,phi,depth); subdet="HF";
                HcalFrontEndId    lmap_entry=lmap.getHcalFrontEndId(*detid);
                HcalElectronicsId emap_entry=emap.lookup(*detid);
                MAPfile << "  {\""<<subdet<<"\" , "<< detid->ieta()<<" , "<< detid->iphi()-1 <<" ,    "<< detid->depth()<<" ," ;    
                MAPfile << "\""<<lmap_entry.rbx()<<"\" , "<<lmap_entry.rm()<<" ,   "<<lmap_entry.pixel()<<" ,      "<<lmap_entry.rmFiber()<<" ,        " ;
                MAPfile << lmap_entry.fiberChannel()<<" ,  "<<lmap_entry.qieCard()<<" ,  "<<lmap_entry.adc()<<" ,        ";    
                MAPfile << emap_entry.readoutVMECrateId()<<" ,    "<<emap_entry.dccid()<<" ,     "<<emap_entry.spigot()<<" ,         "<<emap_entry.fiberIndex()<<" ,      " ;
                MAPfile << emap_entry.htrSlot()<<" ,      "<<emap_entry.htrTopBottom() ;
                MAPfile << "}," << std::endl;           
                delete detid;
             }  //Depth
          }  //Phi
       }  //Eta

// HO

        for (int eta= -15;eta<0;eta++) {
          for (int phi=1;phi<=72;phi++) {
             for (int depth=4;depth<=4;depth++) {
                HcalDetId *detid=0;
                detid=new HcalDetId(HcalOuter,eta,phi,depth); subdet="HO";
                HcalFrontEndId    lmap_entry=lmap.getHcalFrontEndId(*detid);
                HcalElectronicsId emap_entry=emap.lookup(*detid);
                MAPfile << "  {\""<<subdet<<"\" , "<< detid->ieta()<<" , "<< detid->iphi()-1 <<" ,    "<< detid->depth()<<" ," ;    
                MAPfile << "\""<<lmap_entry.rbx()<<"\" , "<<lmap_entry.rm()<<" ,   "<<lmap_entry.pixel()<<" ,      "<<lmap_entry.rmFiber()<<" ,        " ;
                MAPfile << lmap_entry.fiberChannel()<<" ,  "<<lmap_entry.qieCard()<<" ,  "<<lmap_entry.adc()<<" ,        ";    
                MAPfile << emap_entry.readoutVMECrateId()<<" ,    "<<emap_entry.dccid()<<" ,     "<<emap_entry.spigot()<<" ,         "<<emap_entry.fiberIndex()<<" ,      " ;
                MAPfile << emap_entry.htrSlot()<<" ,      "<<emap_entry.htrTopBottom() ;
                MAPfile << "}," << std::endl;           
                delete detid;
             }  //Depth
          }  //Phi
       }  //Eta

        for (int eta= 1;eta<=15;eta++) {
          for (int phi=1;phi<=72;phi++) {
             for (int depth=4;depth<=4;depth++) {
                HcalDetId *detid=0;
                detid=new HcalDetId(HcalOuter,eta,phi,depth); subdet="HO";
                HcalFrontEndId    lmap_entry=lmap.getHcalFrontEndId(*detid);
                HcalElectronicsId emap_entry=emap.lookup(*detid);
                MAPfile << "  {\""<<subdet<<"\" , "<< detid->ieta()-1<<" , "<< detid->iphi()-1 <<" ,    "<< detid->depth()<<" ," ;    
                MAPfile << "\""<<lmap_entry.rbx()<<"\" , "<<lmap_entry.rm()<<" ,   "<<lmap_entry.pixel()<<" ,      "<<lmap_entry.rmFiber()<<" ,        " ;
                MAPfile << lmap_entry.fiberChannel()<<" ,  "<<lmap_entry.qieCard()<<" ,  "<<lmap_entry.adc()<<" ,        ";    
                MAPfile << emap_entry.readoutVMECrateId()<<" ,    "<<emap_entry.dccid()<<" ,     "<<emap_entry.spigot()<<" ,         "<<emap_entry.fiberIndex()<<" ,      " ;
                MAPfile << emap_entry.htrSlot()<<" ,      "<<emap_entry.htrTopBottom() ;
                MAPfile << "}," << std::endl;           
                delete detid;
             }  //Depth
          }  //Phi
       }  //Eta
    MAPfile << "};" << std::endl;
    MAPfile <<  std::endl;
    
    MAPfile << "// macro for array length calculation" << std::endl; 
    MAPfile << "#define DIM(a) (sizeof(a)/sizeof(a[0]))" << std::endl; 
    MAPfile <<  std::endl;  

    MAPfile << "// class for cells array managing" << std::endl; 
    MAPfile << "class CellDB {" << std::endl; 
    MAPfile << "public:" << std::endl; 
    MAPfile << "  CellDB()" << std::endl; 
    MAPfile << "  : cells(AllCells,  AllCells + DIM(AllCells))" << std::endl; 
    MAPfile << "{}" << std::endl; 
    MAPfile <<  std::endl;  
  
    MAPfile << "// return i-th cell" << std::endl;
    MAPfile << "Cell operator [] (int i) const {return cells[i];}" << std::endl;
     
    MAPfile << "// number of cells in database" << std::endl; 
    MAPfile << "int size() const {return cells.size();}" << std::endl;
    MAPfile <<  std::endl; 

    MAPfile << "// select cells for which \"par\" == \"val\"" << std::endl; 
    MAPfile << "template<typename T>" << std::endl; 
    MAPfile << "CellDB find(const std::string par, const T val) const" << std::endl; 
    MAPfile << "{" << std::endl; 
    MAPfile << "  std::vector<Cell> s;" << std::endl; 
    MAPfile << "  for (size_t i = 0; i < cells.size(); ++i)" << std::endl; 
    MAPfile << "    if (cells[i].check(par, val))" << std::endl; 
    MAPfile << "    s.push_back(cells[i]);" << std::endl; 
    MAPfile << "  return CellDB(s);" << std::endl; 
    MAPfile << "} " << std::endl; 
    MAPfile <<  std::endl; 

    MAPfile << "private:" << std::endl; 
    MAPfile << " CellDB( const std::vector<Cell> s)" << std::endl; 
    MAPfile << " : cells(s)" << std::endl; 
    MAPfile << "{}" << std::endl; 
    MAPfile << "std::vector<Cell> cells;" << std::endl; 
    MAPfile << "};" << std::endl;

       
    MAPfile.close(); 
    std::cout << "===== Finish writing Channel MAP =====" << std::endl;  

}

//define this as a plug-in
DEFINE_FWK_MODULE(VeRawAnalyzer);
