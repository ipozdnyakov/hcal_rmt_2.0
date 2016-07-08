1. INTRODUCTION

This project is intended to monitoring gains of hybrid photodetectors (HPD) and photomultiplier tubes (PMT) in HCAL.

There are 8 subdetectors in HCAL and the following numeration is used:
(0) HB depth 1, (1) depth 2
(2) HE depth 1, (3) depth 2, (4) depth 3
(5) HF depth 1, (6) depth 2
(7) HO depth 4

This project is versionised in github.com/ipozdnyakov/hcal_rmt_2.0

2. INPUT DATA FOR MONITORING

Files /afs/cern.ch/work/k/kodolova/public/RDMweb/histos/LED_* (LED files)
produced with VeRawAnalyzer.cc are used as input for the monitoring.

There are a lot of histos in these files, content of some of them is determined as follows

TH2F	h_mapDepth1/2/3/4ADCAmpl_HB/HE/HF/HO			- amplitude = summ of all 10 time slices (TS)
								  (it depends of flags useADCmassive/fC/Ccounts_ and
								  usePedestalSubtraction_ ) (at all 8 histos)
TH2F	h_mapDepth1/2/3/4Ampl_HB/HE/HF/HO			- ratio = ampl/amplirude ratio of 4 slides (-1,0,+1,+2) around max TS 
								  to summ of all 10 (at all 8 histos)
TH2F	h_mapDepth1/2/3/4AmplE34_HB/HE/HF/HO			- amplitude = summ of all 10 TS
								  (it depends on flags useADCmassive/fC/Ccounts_ and
								  usePedestalSubtraction_ studyDiffAmplHist_) (at all 8 histos)
TH2F	h_mapDepth1/2/3/4Amplitude_HB/HE/HF/HO			- rmsamplitude = sqrt (rmsamp/amlitude) (at all 8 histos)
TH2F	h_mapDepth1/2/3Error_HB/HE/HF/HO			- histos with one entry for each error (at all 10 histos)
TH2F	h_mapDepth1/2/3/4TSmaxA_HB/HE/HF/HO			- TS with maximum signal (at all 8 histos)
TH2F	h_mapDepth1/2/3/4TSmeanA_HB/HE/HF/HO			- aveamplitude1 (at all 8 histos)

Currently histos h_mapDepth1/2/3/4ADCAmpl_HB/HE/HF/HO used for monitoring.



3. CONTENT OF THE PROJECT

/input			- directory with list of input files and arrays of input runs stored in text files

 list_of_LED_files 	- list of files (runs) of interest - one file for one run

/output			- directory for output .root and .gif files

/scripts		- directory contains scripts for monitoring


Drun_cell.C		- analyse gains for particular pair of runs and cell 			(run_ref, run_ana, subd, ieta, iphi)
			  outputs gain drift (ratio of gain in _ana run with reagrd to _ref run)

Nrun_cell.C		- analyse gains for particular cell over all run			(subd, ieta, iphi)
			  outputs a list of gain drifts for particular cell over all runs

Drun_HCAL.C		- analyse all cells of HCAL for particular pair of runs 		(run_ref, run_ana, threshold)
			  where threshold - is an input for acceptable gain drift in %
			  outputs number of cells which need to be calibrated (drift is more than threshold) for each run
			  also outputs corresponding histos in .root and .gif files

Nrun_HCAL.C		- analyse all cells of HCAL over all runs 				(threshold)
			  for each run outputs number of cells which need to be calibrated 
			  (drift is more than threshold) with regard to ref run (first run)
			  also outputs corresponding histos in .root and .gif files

Choose_subdetector.C	- return name of histogram of interest by the number of subdetector

all *.C filese are "script" files for ROOT CINT and should be run from lxplus with root in batch mode
(to have acces to /afs/cern.ch/work/k/kodolova/public/RDMweb/histos/)
> root -l -b

TASK.txt - list of current tasks for the project

4. USER MANUAL

One can use this project as follows:

	1) Make a list of run of interest by command like > ls -1 /afs/cern.ch/work/k/kodolova/public/RDMweb/histos/LED_* >> list_of_LED_files
	2) Remove all old runs (out of interest) from file list_of_LED_files - so, first run in this remaining list wiil be the reference run
	3) Run Drun_HCAL for first and last runs in the list - see how much cells need to be calibrated at the end
	4) Run Nrun_HCAL - see how number of cells wich is need to be calibrated changes with time
	5) Check all .gif files appeared in /output directory
	6) Use Drun_cell and Nrun_cell to plot gain of drift over time for particular cell

All scripts should be declared in root by executing > .x "script_name.C" or > .L "script_name.C"
directories /input and /output should be created
