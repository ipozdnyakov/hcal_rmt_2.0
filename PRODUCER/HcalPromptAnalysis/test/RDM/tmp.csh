#!/bin/csh

set CALIB=LASER


set WebDir='/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMweb'
set WebSite='https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/RMT'



cd /tmp/kodolova
ls > list
foreach j (`cat list`)
cd ${j}
set runnumber=${j}
set raw=3
echo '<td class="s'$raw'" align="center">NO</td>'>> index_draft.html
echo '<td class="s'$raw'" align="center">OK</td>'>> index_draft.html
echo '</tr>'>> index_draft.html

foreach i (`ls *.html`)
cat ${i} | sed s/LED/LASER/g > ${i}_t
mv ${i}_t ${i} 
end

ls *.png
if(${status} == "0") then

cp *.html $WebDir/${CALIB}_$runnumber 
cp *.png $WebDir/${CALIB}_$runnumber 

mv $WebDir/${CALIB}_$runnumber/index_draft.html $WebDir/${CALIB}_$runnumber/index_draft.html.orig
cp index_draft.html $WebDir/${CALIB}_$runnumber

cmsMkdir /store/group/dpg_hcal/comm_hcal/www/HcalRemoteMonitoring/RMT/${CALIB}_$runnumber
foreach i (`ls *.html`)
cat ${i} | sed 's#cms-cpt-software.web.cern.ch\/cms-cpt-software\/General\/Validation\/SVSuite#cms-conddb-dev.cern.ch\/eosweb\/hcal#g'> ${i}.n
cmsStage -f ${i}.n /store/group/dpg_hcal/comm_hcal/www/HcalRemoteMonitoring/RMT/${CALIB}_$runnumber
end
foreach k (`ls *.png`)
cmsStage -f ${k} /store/group/dpg_hcal/comm_hcal/www/HcalRemoteMonitoring/RMT/${CALIB}_$runnumber
end
endif
cd ..
end
cd /afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMScript/CMSSW807patch2_STABLE/src/RecoHcal/HcalPromptAnalysis/test/RDM

