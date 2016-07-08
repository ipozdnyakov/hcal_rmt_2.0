TString Choose_subdetector(int subd = 0){
TString hist_name;
        if (subd == 0){
                hist_name = "h_mapDepth1ADCAmpl_HB";
        }else if(subd == 1){
                hist_name = "h_mapDepth2ADCAmpl_HB";
        }else if(subd == 2){
                hist_name = "h_mapDepth1ADCAmpl_HE";
        }else if(subd == 3){
                hist_name = "h_mapDepth2ADCAmpl_HE";
        }else if(subd == 4){
                hist_name = "h_mapDepth3ADCAmpl_HE";
        }else if(subd == 5){
                hist_name = "h_mapDepth1ADCAmpl_HF";
        }else if(subd == 6){
                hist_name = "h_mapDepth2ADCAmpl_HF";
        }else if(subd == 7){
                hist_name = "h_mapDepth4ADCAmpl_HO";
        }
return hist_name;
}
