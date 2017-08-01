import csv
import ROOT as root

src = "/afs/cern.ch/work/k/kodolova/public/RDMweb/histos"

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
    return [(depth,i-40,j) in det_map and hist2F.GetBinContent(i+1,j+1) != 0 for i in range(n) for j in range(m)].count(True)

#create a dictionary of {run_number:(date,n_of_events)}
run_dict = dict()
with open('../output/led_sipm_he_runs', mode='r') as infile:
    reader = csv.reader(infile, delimiter='\t')
    run_dict = dict((int(row[0]),(row[1],row[2])) for row in reader)

#These runs contains only 68 SiPM cells of 188
run_dict.pop(288770,None)
run_dict.pop(288773,None)
run_dict.pop(288775,None)

#create a set of cells from mapping
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

for run in run_dict:
    file = root.TFile(src + "/LED_" + str(run) + ".root", "READ")
    hist = map(file.Get,histname)
    c = map(count_cells_in_map, [(x[0], x[1], map_hep17_sipm) for x in zip(hist[3:], depths[3:])])
    print run,'\t',run_dict[run][0],'\t',run_dict[run][1],'\t', map(count_cells, hist[3:]), c
