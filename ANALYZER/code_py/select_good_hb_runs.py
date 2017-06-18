import csv
import ROOT as root

#create a dictionary of {run_number:(date,n_of_events)}
run_dict = dict()
with open('../led_runs', mode='r') as infile:
    reader = csv.reader(infile, delimiter='\t')
    run_dict = dict((int(row[1]),(row[4],row[3])) for row in reader)

#There are no LED files for these two runs
run_dict.pop(260777, None)
run_dict.pop(285356, None)

histname = [
    'h_mapDepth1ADCAmpl_HB',        #0
    'h_mapDepth2ADCAmpl_HB',        #1
]

for run in run_dict:
    file = root.TFile("/afs/cern.ch/work/k/kodolova/public/RDMweb/histos/LED_" + str(run) + ".root", "READ")
    hist = map(file.Get,histname)
#all HB histograms
    if [type(h) == type(root.TH2F()) for h in hist].count(True) == 2:
#any entries
        if sum(map(root.TH2F.GetEntries, hist))/int(run_dict[run][1]) > 2592*0.9:
            print run,'\t',run_dict[run][0],'\t',run_dict[run][1] #,sum(map(root.TH2F.GetEntries, hist))/int(run_dict[run][1])
#END
