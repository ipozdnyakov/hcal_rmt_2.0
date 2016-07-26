#!/bin/tcsh
set YEARTAG="2012"
touch run2000_${YEARTAG}.txt
touch run5000_${YEARTAG}.txt
touch run10000_${YEARTAG}.txt
touch days2000_${YEARTAG}.txt
touch days5000_${YEARTAG}.txt
touch days10000_${YEARTAG}.txt


set diff=0
set MPHR=60
set HPD=24
set TARGET="2012-01-01 00:00:01"
set TARGET0=`date -u -d"$TARGET" +%s`

foreach i (`cat ${1}`)
set RUN=`echo ${i} | awk -F _ '{print $1}'`
set year=`echo ${i} | awk -F _ '{print $3}' | awk -F - '{print $1}'`
set NEV=`echo ${i} | awk -F _ '{print $5}'`
set CURRENT=`echo ${i} | awk -F _ '{print $3" "$4}'`
set CURRENT0=`date -u -d"$CURRENT" +%s`
@ diff = ( ${CURRENT0} - ${TARGET0} ) / ${MPHR}  
@ diff0 = ${diff} / ${MPHR}
@ diff1 = ${diff0} / ${HPD}
#/ ${MPHR} / ${HPD} / "2"
echo ${CURRENT} ${CURRENT0} ${TARGET0} ${diff} ${diff0} ${diff1}

if( ${NEV} >= "10000" ) then
#echo " " >> run10000_${YEARTAG}.txt 
echo \"${RUN}\" >> run10000_${YEARTAG}.txt
#echo " " >> run10000_${YEARTAG}.txt
echo ${diff1}"., " >> days10000_${YEARTAG}.txt
endif
if( ${NEV} >= "5000" && ${NEV} < 10000 ) then
#echo " " >> run5000_${YEARTAG}.txt
echo \"${RUN}\" >> run5000_${YEARTAG}.txt
#echo " " >> run5000_${YEARTAG}.txt
echo ${diff1}"., " >> days5000_${YEARTAG}.txt
endif
if( ${NEV} >= "2000" && ${NEV} < "5000" ) then
#echo " " >> run2000_${YEARTAG}.txt
echo \"${RUN}\" >> run2000_${YEARTAG}.txt
#echo " " >> run2000_${YEARTAG}.txt
echo ${diff1}"., " >> days2000_${YEARTAG}.txt
endif
end

tac run2000_${YEARTAG}.txt | tr '\n' ', ' | awk -F "," '{for(i=1;i<=NF; i=i+10 ) {for(j=i;j<=i+9;j++) {printf($j)}; printf("\n");}}' | sed s/\"\"/\"\,\"/g  > run2000_${YEARTAG}.txt1
tac run5000_${YEARTAG}.txt | tr '\n' ', ' | awk -F "," '{for(i=1;i<=NF; i=i+10 ) {for(j=i;j<=i+9;j++) {printf($j)}; printf("\n");}}' | sed s/\"\"/\"\,\"/g  > run5000_${YEARTAG}.txt1
tac run10000_${YEARTAG}.txt | tr '\n' ', ' | awk -F "," '{for(i=1;i<=NF; i=i+10 ) {for(j=i;j<=i+9;j++) {printf($j)}; printf("\n");}}' | sed s/\"\"/\"\,\"/g > run10000_${YEARTAG}.txt1

tac days2000_${YEARTAG}.txt | tr '\n' ', ' | sed 's/\, \,/\,/g' | awk -F "," '{for(i=1;i<=NF; i=i+10 ) {for(j=i;j<=i+9;j++) {printf($j",")}; printf("\n");}}' | sed s/,,,,//g > days2000_${YEARTAG}.txt1
tac days5000_${YEARTAG}.txt | tr '\n' ', ' | sed 's/\, \,/\,/g' | awk -F "," '{for(i=1;i<=NF; i=i+10 ) {for(j=i;j<=i+9;j++) {printf($j",")}; printf("\n");}}' | sed s/,,,,//g > days5000_${YEARTAG}.txt1
tac days10000_${YEARTAG}.txt | tr '\n' ', ' | sed 's/\, \,/\,/g' | awk -F "," '{for(i=1;i<=NF; i=i+10 ) {for(j=i;j<=i+9;j++) {printf($j",")}; printf("\n");}}' | sed s/,,,,//g > days10000_${YEARTAG}.txt1

