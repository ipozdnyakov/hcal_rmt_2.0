#ifndef _INPUT_H
#define _INPUT_H
using namespace std;

void Ncell(TString);
vector<double> Nrun_cell(vector<vector<int> > cells, int run);

// Simp runs 2017

int runs_numbers[] = {
            287387, 287388, 287390, 287412, 287427, 287428, 287433, 287448, 287449, 287450,
            287453, 287454, 287457, 287458, 287461, 287472, 287499, 287508, 287509, 287516,
            287526, 287527, 287530, 287531, 287539, 287584, 287588, 287616, 287678, 287726,
            287768, 287773, 287774, 287775, 287800, 287851, 287860, 287872, 287874, 287878,
            287880, 287883, 287885, 287886, 287887, 287888, 287899, 287903, 287905, 287907,
            287953, 287982, 287983, 287987, 288023, 288029, 288030, 288083, 288084, 288087,
            288102, 288103, 288104, 288115, 288163, 288164, 288231, 288237, 288238, 288239,
            288264, 288346, 288347, 288348, 288390, 288391, 288392, 288395, 288396, 288398,
            288529, 288534, 288535, 288542, 288560, 288581, 288594, 288596, 288598, 288599,
            288630, 288631, 288633, 288648, 288668, 288673, 288675, 288676, 288732, 288770,
            288773, 288782, 288789, 288791, 288863, 288867, 288890, 288947, 288953, 288977,
            289009, 289086, 289088, 289093, 289104, 289214, 289215, 289297, 289299, 289399,
            289422, 289442, 289502, 289550, 289609, 289611, 289668, 289669, 289711, 289869,
            289870, 289889, 290010, 290034, 290036, 290042, 290105, 290132, 290134, 290140,
            290142, 290147, 290148, 290217, 290224, 290279, 290283, 290293, 290688, 290731,
            290745, 290770, 290771, 290773, 290775, 290776, 290778, 290781, 290789, 290791,
            290827, 290980, 291071, 291082, 291084, 291087, 291088, 291094, 291203, 291208,
            291211, 291298, 291653, 291654, 291656, 291657, 291660, 291661, 291663, 291664,
            291665, 291666, 291669, 291670, 291671, 291673, 291674, 291756, 291959, 292134,
            292140, 292395, 292398, 292400, 292403, 292412, 292453, 292454, 292517, 292802
};

TString runs[] = {
"287387", "287388", "287390", "287412", "287427", "287428", "287433", "287448", "287449", "287450", "287453", "287454", "287457", "287458", "287461", "287472", "287499", "287508", "287509", "287516", "287526", "287527", "287530", "287531", "287539", "287584", "287588", "287616", "287678", "287726", "287768", "287773", "287774", "287775", "287800", "287851", "287860", "287872", "287874", "287878", "287880", "287883", "287885", "287886", "287887", "287888", "287899", "287903", "287905", "287907", "287953", "287982", "287983", "287987", "288023", "288029", "288030", "288083", "288084", "288087", "288102", "288103", "288104", "288115", "288163", "288164", "288231", "288237", "288238", "288239", "288264", "288346", "288347", "288348", "288390", "288391", "288392", "288395", "288396", "288398", "288529", "288534", "288535", "288542", "288560", "288581", "288594", "288596", "288598", "288599", "288630", "288631", "288633", "288648", "288668", "288673", "288675", "288676", "288732", "288770", "288773", "288782", "288789", "288791", "288863", "288867", "288890", "288947", "288953", "288977", "289009", "289086", "289088", "289093", "289104", "289214", "289215", "289297", "289299", "289399", "289422", "289442", "289502", "289550", "289609", "289611", "289668", "289669", "289711", "289869", "289870", "289889", "290010", "290034", "290036", "290042", "290105", "290132", "290134", "290140", "290142", "290147", "290148", "290217", "290224", "290279", "290283", "290293", "290688", "290731", "290745", "290770", "290771", "290773", "290775", "290776", "290778", "290781", "290789", "290791", "290827", "290980", "291071", "291082", "291084", "291087", "291088", "291094", "291203", "291208", "291211", "291298", "291653", "291654", "291656", "291657", "291660", "291661", "291663", "291664", "291665", "291666", "291669", "291670", "291671", "291673", "291674", "291756", "291959", "292134", "292140", "292395", "292398", "292400", "292403", "292412", "292453", "292454", "292517", "292802"
};

TString dates[] = {
"2017-02-10", "2017-02-10", "2017-02-10", "2017-02-10", "2017-02-10", "2017-02-10", "2017-02-10", "2017-02-13", "2017-02-13", "2017-02-13", "2017-02-13", "2017-02-13", "2017-02-13", "2017-02-13", "2017-02-13", "2017-02-13", "2017-02-13", "2017-02-13", "2017-02-13", "2017-02-14", "2017-02-14", "2017-02-14", "2017-02-14", "2017-02-14", "2017-02-14", "2017-02-14", "2017-02-14", "2017-02-15", "2017-02-15", "2017-02-16", "2017-02-17", "2017-02-17", "2017-02-17", "2017-02-17", "2017-02-18", "2017-02-19", "2017-02-19", "2017-02-20", "2017-02-20", "2017-02-20", "2017-02-20", "2017-02-20", "2017-02-20", "2017-02-20", "2017-02-20", "2017-02-20", "2017-02-20", "2017-02-20", "2017-02-20", "2017-02-21", "2017-02-22", "2017-02-22", "2017-02-22", "2017-02-22", "2017-02-23", "2017-02-23", "2017-02-23", "2017-02-23", "2017-02-23", "2017-02-23", "2017-02-24", "2017-02-24", "2017-02-24", "2017-02-24", "2017-02-24", "2017-02-24", "2017-02-26", "2017-02-27", "2017-02-27", "2017-02-27", "2017-02-27", "2017-02-28", "2017-02-28", "2017-02-28", "2017-02-28", "2017-02-28", "2017-02-28", "2017-03-01", "2017-03-01", "2017-03-01", "2017-03-02", "2017-03-02", "2017-03-02", "2017-03-02", "2017-03-02", "2017-03-03", "2017-03-03", "2017-03-03", "2017-03-03", "2017-03-03", "2017-03-03", "2017-03-03", "2017-03-03", "2017-03-03", "2017-03-04", "2017-03-04", "2017-03-04", "2017-03-04", "2017-03-06", "2017-03-06", "2017-03-06", "2017-03-06", "2017-03-06", "2017-03-06", "2017-03-08", "2017-03-08", "2017-03-08", "2017-03-09", "2017-03-09", "2017-03-09", "2017-03-09", "2017-03-10", "2017-03-10", "2017-03-10", "2017-03-13", "2017-03-13", "2017-03-13", "2017-03-14", "2017-03-14", "2017-03-15", "2017-03-16", "2017-03-16", "2017-03-16", "2017-03-17", "2017-03-17", "2017-03-17", "2017-03-20", "2017-03-20", "2017-03-20", "2017-03-23", "2017-03-23", "2017-03-23", "2017-03-24", "2017-03-25", "2017-03-25", "2017-03-25", "2017-03-27", "2017-03-27", "2017-03-27", "2017-03-27", "2017-03-27", "2017-03-27", "2017-03-27", "2017-03-28", "2017-03-28", "2017-03-28", "2017-03-28", "2017-03-28", "2017-03-31", "2017-03-31", "2017-04-01", "2017-04-01", "2017-04-01", "2017-04-01", "2017-04-01", "2017-04-01", "2017-04-01", "2017-04-01", "2017-04-01", "2017-04-01", "2017-04-03", "2017-04-05", "2017-04-05", "2017-04-05", "2017-04-05", "2017-04-05", "2017-04-05", "2017-04-05", "2017-04-06", "2017-04-06", "2017-04-06", "2017-04-07", "2017-04-12", "2017-04-12", "2017-04-12", "2017-04-12", "2017-04-12", "2017-04-12", "2017-04-12", "2017-04-12", "2017-04-12", "2017-04-12", "2017-04-12", "2017-04-12", "2017-04-12", "2017-04-12", "2017-04-12", "2017-04-13", "2017-04-19", "2017-04-20", "2017-04-20", "2017-04-25", "2017-04-25", "2017-04-25", "2017-04-25", "2017-04-25", "2017-04-25", "2017-04-25", "2017-04-26", "2017-04-28"
};

Int_t nruns = sizeof(runs)/sizeof(runs[0]);

TString ref_runs[] = {"287387","287387","287387","287387","287387","287387","287387"};

//arrays of names and numbers of cells in subdetectors
TString subd_depth_name[] = {
	"h_mapDepth1ADCAmpl_HE",
	"h_mapDepth2ADCAmpl_HE",
	"h_mapDepth3ADCAmpl_HE",
	"h_mapDepth4ADCAmpl_HE",
	"h_mapDepth5ADCAmpl_HE",
	"h_mapDepth6ADCAmpl_HE",
	"h_mapDepth7ADCAmpl_HE"
};

Int_t nsubd = sizeof(ref_runs)/sizeof(ref_runs[0]);

#endif  /* _INPUT_H */
