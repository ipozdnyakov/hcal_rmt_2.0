import csv

with open('./output/cells_of_SiMP_HE', 'rb') as csvfile1:
	file1 = csv.reader(csvfile1, delimiter = '\t')
	SiMP = set((int(x[0]),int(x[1]),int(x[2])) for x in file1)

with open('./output/cells_of_SiMP_HE_from_mapping', 'rb') as csvfile2:
	file2 = csv.reader(csvfile2, delimiter = '\t')
	SiMP_mapping = set((int(x[0]),int(x[1]),int(x[2])) for x in file2)

print 'len SiMP: ', len(SiMP)
print 'len SiMP_map: ', len(SiMP)
print 'len intersection: ', len(SiMP.intersection(SiMP_mapping))


with open('./output/gain_drift_cells_of_SiMP_HE_from_mapping', 'rb') as csvfile3:
	file3 = csv.reader(csvfile3, delimiter = '\t')
#	SiMP_mapping = dict()
	x = file3.next()
	x = file3.next()
	print 'ref to zeros: ', x.count('ref_to_zero')
