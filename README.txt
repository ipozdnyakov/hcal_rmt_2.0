
LAST CHECK or UPDATE MONITORING: 21.02.2017

Project should be run from lxplus to have acces to all the  LED
files and other information.

1. INTRODUCTION

	This project is intended to monitoring gains of hybrid 
	photodetectors (HPD) and photomultiplier  tubes  (PMT) 
	in CMS HCAL.

	Project consists of two parts - ANALAYZER and PRODUCER
	(PRODUCER part is not completed yet)

      >>ANALYZER.  Analyzing  ADC  responses   on   LEDs   and
	monitoring gains drifts. It contains c++ (>>/code_cpp)
	as well as separate python scripts (>>/code_py)

      >>/code_cpp  -- a  set  of  C++  functions  called  from 
	the main(), monitor.exe is produced  by  >>./maker_gcc 
	and run by >>./monitor.exe

      >>/code_py   -- a set of python scripts  to  be  run  as
	>>python <script_name>.py

	------------------------------------------------------
	______________________________________________________

	There are 8 subdetectors in  HCAL  and  the  following 
	numeration is used in the project:

	(I)	(0) HB depth 1, (1) depth 2
	(II)	(2) HE depth 1, (3) depth 2, (4) depth 3
	(III)	(5) HF depth 1, (6) depth 2
	(IV)	(7) HO depth 4

	HINT: One  should  not  divide  results  by  depths in 
	presentations due to direct (independent of depth) way 
	from LED to PMT/HPD

	2017: the SiPM devices where added to  test  them  for
	the future CMS upgrade. They are  placed  in  some  HE
	sector and spans among depth 1 to depth 7.


2. INPUT FOR THE ANALYZER

      >>/afs/cern.ch/work/k/kodolova/public/RDMweb/histos/LED_*
	(LED  files ) produced  with VeRawAnalyzer.cc are used
	as input for the  monitoring. This folder is  regulary
	updated by Alexandr Zhokin and Olga Kodolova

      >>led_runs - contains a list of LED runs  ever  produced 
	as taken from the web-page - 
	    https://cms-conddb-dev.cern.ch/eosweb/hcal/HcalRemoteMonitoring/RMT/
	(currently about 994 LED runs) and their information

      >>LED_xxxxxx.root - examplea  of  LED  files  for  local
	and manual investigation. There are a lot of histos in
	these files, content of	some of them is determined  as
	follows:
	    TH2F  --  h_mapDepth[1/2/3/4]ADCAmpl_[HB/HE/HF/HO]		
	- amplitude = summ of  all  10  time  slices  (TS), it 
	depends on flags     useADCmassive/fC/Ccounts_     and
	usePedestalSubtraction_ -- at all 8 histos
	    Meaning of content of some other histos  might  be
	found among first 7 commits of the project.


3. CONTENT OF THE ANALYZER

	/plots	
	- directory for output .root and .gif files

	/output	
	- directory with list of input  files  and  arrays  of 
	input runs stored in text files

	----bad_runs
		- list of runs with abnormal high drifting, output  of 
		Nrun_HCAL()

	----bad_cells
		- list of cells for  reading  by  Ncell(),  output  of 
		Drun_HCAL()

	----gain_drifts
		- time evolution of  gains  from  the  bad_cells  list
		  output  of Ncell()

	Drun_cell()		
	- analyse gains for particular pair of runs  and  cell 
	(run_ref, run_ana,  subd,  ieta,  iphi)  outputs  gain 
	drift (ratio of gain in _ana run with reagrd  to  _ref 
	run)

	Nrun_cell()		
	- analyse gains for particular cell over all run (subd,
	ieta,  iphi)  outputs  a  list  of  gain  drifts   for 
	particular cell over all runs

	Drun_HCAL()		
	- analyse all cells of HCAL  for  particular  pair  of 
	runs (run_ref, run_ana, threshold)  where  threshold - 
	is an input for acceptable gain  drift  in  %  outputs 
	number of cells which need to be calibrated (drift  is 
	more  than  threshold)  for  each  run  also   outputs 
	corresponding histos in .root and .gif files

	Nrun_HCAL()		
	- analyse all cells of HCAL over all runs  (threshold)
	for each run outputs number of cells which need to  be 
	calibrated (drift is more than threshold) with  regard 
	to ref run  (first  run)  also  outputs  corresponding 
	histos in .root and .gif files

	Ncell()
	- read list of cells from  ./output/bad_cells  and  run 
	Nrun_cell() for each cell

	Choose_subdetector()
	- return name of histogram of interest by  the  number 
	of subdetector

4. USE OF THE ANALYZER

	One can use this project as follows:

	1) Make a list of run  of  interest  by  listing  like 
	   > ls -1 /afs/cern.ch/work/k/kodolova/public/RDMweb/
	   histos/LED_*

	2) Correct global array of runs  in  input.h  -  first   
	   run in this array will be the reference run

	3) Check outputs in Nrun_HCAL and Drun_HCAL
	   performing steps 3a),3b) and 3b) in code of main():
	3a) Run Nrun_HCAL - see how total number of cells wich 
	   are need to be calibrated changes with time
	3b) Run Drun_HCAL for first and last runs of  interest 
	   - see what cells need to be calibrated
	   list of bad cells appears in the ./output directory
	   plot all .gif files appeared  in  /plots  directory
	3c) Run Ncell() to get file  gain_drifts   in  /output
	   this is drift over time for  particular  cell  from
	   the list of bad cells

Notes:
	> ./maker_gcc 		- to compile main() = main.cpp
	> ./monitor.exe 	- to run
	Folders /input and /output should be created  manually

5. TASKS FOR FURTHER DEVELOPMENT
0) there are strong need to integrate with a lot of already done work in large RMT project
	- https://twiki.cern.ch/twiki/bin/viewauth/CMS/HcalRemoteMonitoring
1) add Q > 50 linADC criteria for each cell in reference run
2) implement purity and stability for monitoring of overal run quality and distinguish drift from fluctuations
3) make procedure of defining Magnet_on and Magnet_off LED runs clear in README.txt (run summary and RMT table)
1) create simple, universal and lightweighted analyzer for local runs with only tree and few histos in output
2) create procedures and documentation for using analyzer, getting input and storing output files



------------------------------------
------------------------------------
------------------------------------



4.2 USER MANUAL FOR PRODUCER

	--


	*OPTION*2) PRODUCER.  Filling  histograms  and  TTrees 
		   with ADC responses  on  LEDs.  -  currently
		   not developed// the same work is  performed
		   regularly  by  Alexandr  Zhokin  and   Olga
		   Kodolova.

	PRODUCER is a CMSSW analyzer, which  takes  data  from 
	local  runs  (LED  runs)  of  CMS  detector  that  are 
	permanently stored in CMS database. Code should be run 
	from CMSSW_X_X_X/src, as PRODUCER initially taken from
		/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/ \
		HCALMONITORING/RDMScript/                    \
		CMSSW807patch2_STABLE/src/RecoHcal/	     \


