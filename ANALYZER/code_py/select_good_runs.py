import csv
import ROOT as root

def date_aligner(date):
    return '-'.join(date.replace('.','-').split('-')[::-1]) if '.' in date else date

def file_writer(name,ll):
    with open(name, 'wb') as csvfile:
        wrt = csv.writer(csvfile, delimiter='\t')
        for l in ll:
            wrt.writerow(l)

#create a dictionary of {run_number:(date,n_of_events)}
run_dict = dict()
with open('../led_runs', mode='r') as infile:
    reader = csv.reader(infile, delimiter='\t')
    run_dict = dict((int(row[1]),(date_aligner(row[4]),row[3])) for row in reader)

#There are no LED files for these two runs
run_dict.pop(260777, None)
run_dict.pop(285356, None)

histname = [
    'h_mapDepth1ADCAmpl_HB',        #0
    'h_mapDepth2ADCAmpl_HB',        #1
    'h_mapDepth1ADCAmpl_HE',        #2
    'h_mapDepth2ADCAmpl_HE',        #3
    'h_mapDepth3ADCAmpl_HE',        #4
    'h_mapDepth1ADCAmpl_HF',        #5
    'h_mapDepth2ADCAmpl_HF',        #6
    'h_mapDepth4ADCAmpl_HO',        #7
]

hb_runs = []
he_runs = []
hf_runs = []
ho_runs = []

for run in run_dict:
    file = root.TFile("/afs/cern.ch/work/k/kodolova/public/RDMweb/histos/LED_" + str(run) + ".root", "READ")
    hist = map(file.Get,histname)
#check there are all HB histograms and entries
    if [type(h) == type(root.TH2F()) for h in hist[0:2]].count(True) == 2:
        if sum(map(root.TH2F.GetEntries, hist[0:2]))/int(run_dict[run][1]) > 2592*0.99:
            hb_runs.append([run,run_dict[run][0],run_dict[run][1]])            
#check there are all HE histograms and entries
    if [type(h) == type(root.TH2F()) for h in hist[2:5]].count(True) == 3:
        if sum(map(root.TH2F.GetEntries, hist[2:5]))/int(run_dict[run][1]) > 2592*0.99:
            he_runs.append([run,run_dict[run][0],run_dict[run][1]])            
#check there are all HF histograms and entries
    if [type(h) == type(root.TH2F()) for h in hist[5:7]].count(True) == 2:
        if sum(map(root.TH2F.GetEntries, hist[5:7]))/int(run_dict[run][1]) > 1728*0.99:
            hf_runs.append([run,run_dict[run][0],run_dict[run][1]])            
#check there are all HO histograms and entries
    if type(hist[7]) == type(root.TH2F()):
        if root.TH2F.GetEntries(hist[7])/int(run_dict[run][1]) > 2160*0.99:
            ho_runs.append([run,run_dict[run][0],run_dict[run][1]])            
#            print run,'\t',run_dict[run][0],'\t',run_dict[run][1], root.TH2F.GetEntries(hist[7])/int(run_dict[run][1])
#END

file_writer('../output/led_hb_runs',sorted(hb_runs, key = lambda x: x[1]))
file_writer('../output/led_he_runs',sorted(he_runs, key = lambda x: x[1]))
file_writer('../output/led_hf_runs',sorted(hf_runs, key = lambda x: x[1]))
file_writer('../output/led_ho_runs',sorted(ho_runs, key = lambda x: x[1]))

hb = set(tuple(x) for x in hb_runs)
he = set(tuple(x) for x in he_runs)
hf = set(tuple(x) for x in hf_runs)
ho = set(tuple(x) for x in ho_runs)
common = list(set.intersection(hb,he,hf,ho))

file_writer('../output/led_all_runs',sorted(common, key = lambda x: x[1]))
