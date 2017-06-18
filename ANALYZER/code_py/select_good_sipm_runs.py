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

for run in run_dict:
    file = root.TFile("/afs/cern.ch/work/k/kodolova/public/RDMweb/histos/LED_" + str(run) + ".root", "READ")
    hist = map(file.Get,histname)
#all HE histograms
    if [type(h) == type(root.TH2F()) for h in hist].count(True) == 10:
#any entries
        if sum(map(root.TH2F.GetEntries, hist)) != 0:
#any entries in SiPM HE 
            if sum(map(root.TH2F.GetEntries, hist[3:]))/int(run_dict[run][1]) > 188*0.9:
                print run,'\t',run_dict[run][0],'\t',run_dict[run][1] #,sum(map(root.TH2F.GetEntries, hist[3:]))/int(run_dict[run][1])
#END
