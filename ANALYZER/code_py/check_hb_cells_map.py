import csv
import ROOT as root

def count_cells(hist2F):
    n = hist2F.GetNbinsX() #eta
    m = hist2F.GetNbinsY() #phi
    return [hist2F.GetBinContent(i+1,j+1) != 0 for i in range(n) for j in range(m)].count(True)

def count_cells_in_map(intuple):
    hist2F, depth, det_map = intuple[0],intuple[1],intuple[2]
    n = hist2F.GetNbinsX() #eta
    m = hist2F.GetNbinsY() #phi
    #cells = [(depth,i-40,j) for i in range(n) for j in range(m) if ((depth,i-40,j) in det_map and hist2F.GetBinContent(i+1,j+1) != 0)]
    #print cells
    return [(depth,i-40 if i > 40 else i-41,j+1) in det_map and hist2F.GetBinContent(i+1,j+1) != 0 for i in range(n) for j in range(m)].count(True)

#create a dictionary of {run_number:(date,n_of_events)}
run_dict = dict()
with open('../output/led_hb_runs', mode='r') as infile:
    reader = csv.reader(infile, delimiter='\t')
    run_dict = dict((int(row[0]),(row[1],row[2])) for row in reader)

#create a set of cells from mapping
with open('../map_hb', 'r') as infile:
	reader = csv.reader(infile, delimiter = '\t')
	map_hb = set((int(x[0]),int(x[1]),int(x[2])) for x in reader)

histname = [
    'h_mapDepth1ADCAmpl_HB',        #0
    'h_mapDepth2ADCAmpl_HB',        #1
]

depths = [0,1,2,3,4,5,6,0,1,2]

for run in run_dict:
    file = root.TFile("/afs/cern.ch/work/k/kodolova/public/RDMweb/histos/LED_" + str(run) + ".root", "READ")
    hist = map(file.Get,histname)
    c = map(count_cells_in_map, [(x[0], x[1], map_hb) for x in zip(hist, depths[:2])])
    print run,'\t',run_dict[run][0],'\t',run_dict[run][1],'\t', map(count_cells, hist), c
