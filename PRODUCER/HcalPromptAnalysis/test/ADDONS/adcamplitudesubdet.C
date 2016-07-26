{
   //======================================================================
      printf("reco: gROOT Reset \n");
        gROOT->Reset();
        gROOT->SetStyle("Plain");
	//			gStyle->SetOptStat(0);   //  no statistics _or_
	//	        	  gStyle->SetOptStat(11111111);
	//gStyle->SetOptStat(1101);// mame mean and rms 
	//	gStyle->SetOptStat(0101);// name and entries
	//	gStyle->SetOptStat(1110000);// und over, integral !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//	   gStyle->SetOptStat(1100);// mean and rms only !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//				gStyle->SetOptStat(1110000);// und over, integral !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		gStyle->SetOptStat(101110);                                          // entries, meam, rms, overflow !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//
	//gStyle->SetOptFit(00010);// constant, mean and sigma only !!
		//	gStyle->SetOptFit(00001);// hi2/nu, constant, mean and sigma only !!
		gStyle->SetOptFit(0010);// constant, mean and sigma only !!
	//	gStyle->SetOptFit(00011);// constant, mean and sigma only !!
	// gStyle->SetOptFit(1101);
		//	   gStyle->SetOptFit(1011);
	//
	//gStyle->SetStatX(0.98);
	//gStyle->SetStatY(0.99);
	//gStyle->SetStatW(0.30);
	//gStyle->SetStatH(0.25);
	gStyle->SetStatX(0.91);
	gStyle->SetStatY(0.75);
	gStyle->SetStatW(0.20);
	gStyle->SetStatH(0.10);
	// gStyle->SetStatH(0.35);
        //

	//        Float_t LeftOffset = 0.12;
        //Float_t TopOffset = 0.12;
        Float_t LeftOffset = 0.12;
        Float_t TopOffset = 0.22;

        gStyle->SetLineWidth(1);
        gStyle->SetErrorX(0);

//---=[ Titles,Labels ]=-----------
        gStyle->SetOptTitle(0);             // title on/off
        //      gStyle->SetTitleColor(0);           // title color
        gStyle->SetTitleColor(1);           // title color
        //      gStyle->SetTitleX(0.35);            // title x-position
        gStyle->SetTitleX(0.15);            // title x-position
        gStyle->SetTitleH(0.15);             // title height
        //      gStyle->SetTitleW(0.53);            // title width
        gStyle->SetTitleW(0.60);            // title width
        gStyle->SetTitleFont(42);           // title font
        gStyle->SetTitleFontSize(0.07);     // title font size

	gStyle->SetPalette(1);
//---=[ Histogram style ]=----------
        //        gStyle->SetHistFillColor(38);
	//	        gStyle->SetFrameFillColor(41);// jelto-kori4nev-svetl
	//	gStyle->SetFrameFillColor(5);// jeltyi
	//	gStyle->SetFrameFillColor(17);// seryi
	//	gStyle->SetFrameFillColor(18);// svetlo seryi
	//	gStyle->SetFrameFillColor(20);// svetlo kori4nev
	//        gStyle->SetFrameFillColor(33);// sine-seryi
	//	gStyle->SetFrameFillColor(40);// fiolet-seryi
	//	gStyle->SetFrameFillColor(23);// sv.kor

//---=[ Pad style ]=----------------
        gStyle->SetPadTopMargin(TopOffset);
        gStyle->SetPadBottomMargin(LeftOffset);
        gStyle->SetPadRightMargin(TopOffset);
        gStyle->SetPadLeftMargin(LeftOffset);
//---=[ SetCanvasDef           ]=----------------
    //======================================================================
//
// Connect the input file and get the 2-d histogram in memory
    //======================================================================
//  TBrowser *b = new TBrowser

	TFile *hfile1= new TFile("Global_ttt236770.root", "READ");

	//    getchar();
	//
          TPostScript psfile ("adcamplitudesubdet.ps", 111);
//

   c1 = new TCanvas("c1","Hcal4test",200,10,700,900);


    //=========================================================================================== 1 
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,4);
      
      c1->cd(1);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HBdepth1");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HB\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(2);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HBdepth2");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HB \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(3);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth1");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HE \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(4);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth2");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HE \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(5);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth3");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth3_HE \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(6);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HFdepth1");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HF \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(7);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HFdepth2");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HF \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(8);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HOdepth4");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth4_HO \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      
      c1->Update();
      
      
    //===========================================================================================2
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,2);
      
      c1->cd(1);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HB");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC HB \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(2);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HE");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC HE \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(3);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HF");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC HF \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(4);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HO");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC HO \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      
      c1->Update();
      
      
      
    //===========================================================================================3
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,4);
      
      c1->cd(1);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HBdepth1_TS0");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HB TS0\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(2);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HBdepth2_TS0");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HB TS0\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(3);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth1_TS0");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HE TS0\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(4);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth2_TS0");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HE TS0\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(5);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth3_TS0");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth3_HE TS0\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(6);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HFdepth1_TS0");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HF TS0\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(7);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HFdepth2_TS0");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HF TS0\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(8);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HOdepth4_TS0");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth4_HO TS0\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      
      c1->Update();
      
      
    //=========================================================================================== 4
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,4);
      
      c1->cd(1);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HBdepth1_TS1");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HB TS1\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(2);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HBdepth2_TS1");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HB TS1\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(3);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth1_TS1");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HE TS1\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(4);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth2_TS1");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HE TS1\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(5);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth3_TS1");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth3_HE TS1\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(6);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HFdepth1_TS1");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HF TS1\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(7);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HFdepth2_TS1");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HF TS1\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(8);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HOdepth4_TS1");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth4_HO TS1\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      
      c1->Update();
      
      
    //=========================================================================================== 5
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,4);
      
      c1->cd(1);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HBdepth1_TS2");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HB TS2\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(2);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HBdepth2_TS2");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HB TS2\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(3);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth1_TS2");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HE TS2\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(4);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth2_TS2");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HE TS2\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(5);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth3_TS2");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth3_HE TS2\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(6);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HFdepth1_TS2");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HF TS2\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(7);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HFdepth2_TS2");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HF TS2\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(8);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HOdepth4_TS2");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth4_HO TS2\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      
      c1->Update();
      
      
    //=========================================================================================== 6
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,4);
      
      c1->cd(1);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HBdepth1_TS3");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HB TS3\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(2);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HBdepth2_TS3");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HB TS3\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(3);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth1_TS3");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HE TS3\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(4);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth2_TS3");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HE TS3\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(5);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth3_TS3");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth3_HE TS3\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(6);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HFdepth1_TS3");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HF TS3\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(7);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HFdepth2_TS3");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HF TS3\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(8);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HOdepth4_TS3");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth4_HO TS3\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      
      c1->Update();
      
      
    //=========================================================================================== 7
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,4);
      
      c1->cd(1);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HBdepth1_TS4");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HB TS4\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(2);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HBdepth2_TS4");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HB TS4\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(3);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth1_TS4");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HE TS4\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(4);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth2_TS4");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HE TS4\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(5);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth3_TS4");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth3_HE TS4\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(6);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HFdepth1_TS4");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HF TS4\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(7);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HFdepth2_TS4");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HF TS4\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(8);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HOdepth4_TS4");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth4_HO TS4\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      
      c1->Update();
      
      
    //=========================================================================================== 8
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,4);
      
      c1->cd(1);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HBdepth1_TS5");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HB TS5\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(2);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HBdepth2_TS5");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HB TS5\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(3);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth1_TS5");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HE TS5\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(4);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth2_TS5");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HE TS5\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(5);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth3_TS5");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth3_HE TS5\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(6);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HFdepth1_TS5");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HF TS5\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(7);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HFdepth2_TS5");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HF TS5\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(8);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HOdepth4_TS5");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth4_HO TS5\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      
      c1->Update();
      
      
    //=========================================================================================== 9
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,4);
      
      c1->cd(1);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HBdepth1_TS6");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HB TS6\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(2);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HBdepth2_TS6");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HB TS6\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(3);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth1_TS6");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HE TS6\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(4);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth2_TS6");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HE TS6\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(5);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth3_TS6");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth3_HE TS6\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(6);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HFdepth1_TS6");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HF TS6\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(7);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HFdepth2_TS6");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HF TS6\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(8);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HOdepth4_TS6");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth4_HO TS6\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      
      c1->Update();
      
      
    //=========================================================================================== 10
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,4);
      
      c1->cd(1);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HBdepth1_TS7");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HB TS7\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(2);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HBdepth2_TS7");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HB TS7\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(3);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth1_TS7");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HE TS7\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(4);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth2_TS7");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HE TS7\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(5);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth3_TS7");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth3_HE TS7\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(6);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HFdepth1_TS7");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HF TS7\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(7);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HFdepth2_TS7");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HF TS7\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(8);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HOdepth4_TS7");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth4_HO TS7\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      
      c1->Update();
      
      
    //=========================================================================================== 11
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,4);
      
      c1->cd(1);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HBdepth1_TS8");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HB TS8\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(2);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HBdepth2_TS8");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HB TS8\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(3);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth1_TS8");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HE TS8\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(4);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth2_TS8");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HE TS8\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(5);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth3_TS8");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth3_HE TS8\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(6);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HFdepth1_TS8");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HF TS8\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(7);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HFdepth2_TS8");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HF TS8\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(8);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HOdepth4_TS8");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth4_HO TS8\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      
      c1->Update();
      
      
    //=========================================================================================== 12
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,4);
      
      c1->cd(1);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HBdepth1_TS9");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HB TS9\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(2);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HBdepth2_TS9");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HB TS9\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(3);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth1_TS9");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HE TS9\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(4);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth2_TS9");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HE TS9\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(5);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HEdepth3_TS9");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth3_HE TS9\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(6);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HFdepth1_TS9");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HF TS9\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(7);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HFdepth2_TS9");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HF TS9\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(8);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_ADC_HOdepth4_TS9");
      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth4_HO TS9\b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      
      c1->Update();
      
      
    //======================================================================
    //============================================================================================ end
    //======================================================================
    //======================================================================
    // close and delete all possible things:

    psfile->Close();

//    hfile0->Close();
//    hfile0->Delete();
    hfile1->Close();
    hfile1->Delete();

        //  Exit Root
        gSystem->Exit(0);
    //======================================================================

}
