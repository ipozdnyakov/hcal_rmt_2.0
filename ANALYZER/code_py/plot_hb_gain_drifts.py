import csv
import ROOT as root
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

#create a sorted list of runs [(date,run_number,n_of_events)]
run_list = []
with open('../output/led_hb_runs', mode='r') as infile:
    reader = csv.reader(infile, delimiter='\t')
    run_list = [(row[1],int(row[0]),int(row[2])) for row in reader]

#('2017-05-08', 293564, 2000)
run_list = sorted(run_list, key = lambda x: x[0])
run_list = run_list[run_list.index(('2017-05-08', 293564, 2000)):]
for x in run_list:
    print x

#create a set of cells
with open('../map_hb', 'r') as infile:
	reader = csv.reader(infile, delimiter = '\t')
	map_hb = set((int(x[0]),int(x[1]),int(x[2])) for x in reader)

histname = [
    'h_mapDepth1ADCAmpl_HB',        #0
    'h_mapDepth2ADCAmpl_HB',        #1
]

depths = [0,1,2,3,4,5,6,0,1,2]

#ref run
run = run_list[0]
file = root.TFile("/afs/cern.ch/work/k/kodolova/public/RDMweb/histos/LED_" + str(run[1]) + ".root", "READ")
hist = map(file.Get,histname)
ref_cells = dict(((depth,eta,phi),hist[depths.index(depth)].GetBinContent(eta+41 if eta>0 else eta+42, phi+1)/run[2]) for (depth,eta,phi) in map_hb)

results = dict((x,[]) for x in map_hb)

for run in sorted(run_list, key = lambda x: x[0]):
     file = root.TFile("/afs/cern.ch/work/k/kodolova/public/RDMweb/histos/LED_" + str(run[1]) + ".root", "READ")
     hist = map(file.Get,histname)
     cells = dict(((depth,eta,phi),hist[depths.index(depth)].GetBinContent(eta+41 if eta>0 else eta+42,phi+1)/run[2]) for (depth,eta,phi) in map_hb)
     results = dict((x,results[x] + [cells[x]/ref_cells[x] if ref_cells[x] != 0 else 0]) for x in map_hb)

fig = plt.figure()

for x in results:
    if any([ y != 0 for y in results[x]]):
        plt.plot(results[x],marker='o')

ax = fig.add_subplot(111)
fig.subplots_adjust(top=0.85)
ax.set_title('Ratio of Sum of 10 ADC Slices of each event in each run for each cell')
ax.set_ylabel('ratio to run 293564, 2017-05-08')


plt.xticks(range(0,len(run_list)+1)[0::1],[x[0] for x in run_list][0::1])
plt.xticks(rotation=90)
plt.ylim(0.9,1.1)
plt.subplots_adjust(bottom=0.2)
plt.savefig('../plots/results_hb.png')
