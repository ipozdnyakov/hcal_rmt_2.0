1. INTRODUCTION

	This project is intended to monitoring gains of hybrid 
	photodetectors (HPD) and photomultiplier  tubes  (PMT) 
	in CMS HCAL.

	Project consists of two parts - 
		1) PRODUCER. Filling histograms and TTrees 
		   with ADC responses on LEDs. 
		2) ANALYZER. Analyzing ADC responses on LEDs 
		   and monitoring gains drifts. 

	There are 8 subdetectors in  HCAL  and  the  following 
	numeration is used in the project:
		(0) HB depth 1, (1) depth 2
		(2) HE depth 1, (3) depth 2, (4) depth 3
		(5) HF depth 1, (6) depth 2
		(7) HO depth 4
	HINT: One  should  not  divide  results  by  depths in 
	presentations due to direct (independent of depth) way 
	from LED to PMT/HPT


2. ARCHITECTURE OF THE PROJECT

	PRODUCER is a CMSSW analyzer, which  takes  data  from 
	local  runs  (LED  runs)  of  CMS  detector  that  are 
	permanently stored in CMS database. Code should be run 
	from CMSSW_X_X_X/src, as PRODUCER initially taken from
		/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/ \
		HCALMONITORING/RDMScript/                    \
		CMSSW807patch2_STABLE/src/RecoHcal/	     \
 
	ANALYZER  is  a  set  of  "scripts"  for   CINT   ROOT 
	interpretator. 

2.1 INPUT DATA FOR PRODUCER

2.2 INPUT DATA FOR ANALYZER

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

PRODUCER:


ANALYZER:

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


4. USE OF THE PROJECT

4.1 USER MANUAL FOR PRODUCER


4.2 USER MANUAL FOR ANALYZER

One can use this project as follows:

	1) Make a list of run of interest by command like > ls -1 /afs/cern.ch/work/k/kodolova/public/RDMweb/histos/LED_* >> list_of_LED_files
	2) Remove all old runs (out of interest) from file list_of_LED_files - so, first run in this remaining list wiil be the reference run
	3) Run Drun_HCAL for first and last runs in the list - see how much cells need to be calibrated at the end
	4) Run Nrun_HCAL - see how number of cells wich is need to be calibrated changes with time
	5) Check all .gif files appeared in /output directory
	6) Use Drun_cell and Nrun_cell to plot gain of drift over time for particular cell

All scripts should be declared in root by executing > .x "script_name.C" or > .L "script_name.C"
directories /input and /output should be created



5. TASKS FOR FURTHER DEVELOPMENT

1) add Q > 50 linADC criteria for each cell in reference run
2) implement purity and stability for monitoring of overal run quality and distinguish drift from fluctuations
3) implement plot for time evolution of cells of interest within runs of interest
4) implement high_threshold and low_threshold to distinguish drift from faults
5) define places/sites for permanent storage of input LED files (output of VeRawAnalyzer) and output files with histos and plots of gain drifts
6) add list of bad runs in /input (make also list of good runs and information for them - date, N_of_events, processed or not, B field, ...)
7) make procedure of defining Magnet_on and Magnet_off LED runs clear in README.txt (run summary and RMT table)
------------------------------------
1) create simple, universal and lightweighted analyzer for local runs with only tree and few histos in output
2) create procedures and documentation for using analyzer, getting input and storing output files
3) create postprocessor for output of analyzer, which plot maps and evolution curves, gives permanent list of gains drifts
