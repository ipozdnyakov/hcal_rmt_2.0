import csv
import ROOT as root
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

#create a sorted list of runs [(date,run_number,n_of_events)]
run_list = []
with open('../output/led_sipm_he_runs', mode='r') as infile:
    reader = csv.reader(infile, delimiter='\t')
#excluded runs contains only 68 SiPM cells of 188
    run_list = [(row[1],int(row[0]),int(row[2])) for row in reader if int(row[0]) not in (288770,288773,288775)]

#('2017-04-24 ', 292350, 2000)
#('2017-05-02 ', 292990, 2000)
#('2017-05-04 ', 293300, 2000)
run_list = sorted(run_list, key = lambda x: x[0])
run_list = run_list[run_list.index(('2017-05-04 ', 293300, 2000)):]

#create a set of cells
with open('../map_hep17_sipm', 'r') as infile:
	reader = csv.reader(infile, delimiter = '\t')
	map_hep17_sipm = set((int(x[0]),int(x[1]),int(x[2])) for x in reader)

histname = [
    'h_mapDepth1ADCAmpl_HE',        #0
    'h_mapDepth2ADCAmpl_HE',        #1
    'h_mapDepth3ADCAmpl_HE',        #2
    'h_mapDepth4ADCAmpl_HE',        #3
    'h_mapDepth5ADCAmpl_HE',        #4
    'h_mapDepth6ADCAmpl_HE',        #5
    'h_mapDepth7ADCAmpl_HE',        #6
    'h_mapDepth1ADCAmplSiPM_HE',    #7
    'h_mapDepth2ADCAmplSiPM_HE',    #8
    'h_mapDepth3ADCAmplSiPM_HE'     #9
]

depths = [0,1,2,3,4,5,6,0,1,2]

#ref run
run = run_list[0]
file = root.TFile("/afs/cern.ch/work/k/kodolova/public/RDMweb/histos/LED_" + str(run[1]) + ".root", "READ")
hist = map(file.Get,histname[3:])
ref_cells = dict(((depth,eta,phi),hist[depths[3:].index(depth)].GetBinContent(eta+41,phi+1)/run[2]) for (depth,eta,phi) in map_hep17_sipm)

results = dict((x,[]) for x in map_hep17_sipm)

for run in sorted(run_list, key = lambda x: x[0]):
     file = root.TFile("/afs/cern.ch/work/k/kodolova/public/RDMweb/histos/LED_" + str(run[1]) + ".root", "READ")
     hist = map(file.Get,histname[3:])
     cells = dict(((depth,eta,phi),hist[depths[3:].index(depth)].GetBinContent(eta+41,phi+1)/run[2]) for (depth,eta,phi) in map_hep17_sipm)
     results = dict((x,results[x]+[cells[x]/ref_cells[x]] if ref_cells[x] != 0 else 0) for x in map_hep17_sipm)

for x in results:
#    print x, results[x]
    plt.plot(results[x])
plt.xticks(range(1,len(run_list)+1)[1::10],[x[0] for x in run_list][1::10])
plt.xticks(rotation=90)
plt.yscale('log')
plt.ylim(0.000001,100000)
plt.subplots_adjust(bottom=0.2)
plt.savefig('results.png')
