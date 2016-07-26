{
   //======================================================================
      printf("reco: gROOT Reset \n");
        gROOT->Reset();
        gROOT->SetStyle("Plain");
				gStyle->SetOptStat(0);   //  no statistics _or_
	//	        	  gStyle->SetOptStat(11111111);
	//gStyle->SetOptStat(1101);// mame mean and rms 
	//	gStyle->SetOptStat(0101);// name and entries
	//	gStyle->SetOptStat(1110000);// und over, integral !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//	   gStyle->SetOptStat(1100);// mean and rms only !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//				gStyle->SetOptStat(1110000);// und over, integral !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//	gStyle->SetOptStat(101110);                                          // entries, meam, rms, overflow !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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

//	TFile *hfile1= new TFile("test234457.root", "READ");
	//	TFile *hfile1= new TFile("test211006.root", "READ"); 
//	TFile *hfile1= new TFile("testNZS.root", "READ"); 
//	TFile *hfile1= new TFile("test22LED.root", "READ");
//	TFile *hfile1= new TFile("/afs/cern.ch/user/z/zhokin/public/html/RMT/lastMarch2014/cmt/Global_229713.root", "READ");
	//	TFile *hfile1= new TFile("test.root", "READ");

//
//	TFile *hfile1= new TFile("testRunDepCap32.root", "READ"); 
//	TFile *hfile1= new TFile("testRunDepWidth32.root", "READ"); 
//
//	TFile *hfile1= new TFile("Laser_238187.root", "READ");
//	TFile *hfile1= new TFile("Laser_238183.root", "READ");

	TFile *hfile1= new TFile("Global_ttt236770.root", "READ");

	//    getchar();
	//
          TPostScript psfile ("sumamplitudesubdet.ps", 111);
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
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_sumamplitude_depth1_HB");
//      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HB \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(2);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_sumamplitude_depth2_HB");
//      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HB \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(3);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_sumamplitude_depth1_HE");
//      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HE \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(4);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_sumamplitude_depth2_HE");
//      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HE \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(5);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_sumamplitude_depth3_HE");
//      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth3_HE \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(6);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_sumamplitude_depth1_HF");
//      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HF \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(7);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_sumamplitude_depth2_HF");
//      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HF \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(8);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_sumamplitude_depth4_HO");
//      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth4_HO \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      
      c1->Update();
      
      
    //=========================================================================================== 2
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,4);
      
      c1->cd(1);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_sumamplitude_depth1_HB0");
//      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HB \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(2);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_sumamplitude_depth2_HB0");
//      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HB \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(3);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_sumamplitude_depth1_HE0");
//      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HE \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(4);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_sumamplitude_depth2_HE0");
//      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HE \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(5);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_sumamplitude_depth3_HE0");
//      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth3_HE \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(6);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_sumamplitude_depth1_HF0");
//      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HF \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(7);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_sumamplitude_depth2_HF0");
//      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HF \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(8);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_sumamplitude_depth4_HO0");
//      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth4_HO \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      
      c1->Update();
      
      
    //=========================================================================================== 3
    //======================================================================
    //======================================================================
    //================
    //======================================================================
      c1->Clear();
      c1->Divide(2,4);
      
      c1->cd(1);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_sumamplitude_depth1_HB1");
//      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HB \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(2);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_sumamplitude_depth2_HB1");
//      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HB \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(3);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_sumamplitude_depth1_HE1");
//      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HE \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(4);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_sumamplitude_depth2_HE1");
//      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HE \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(5);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_sumamplitude_depth3_HE1");
//      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth3_HE \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(6);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_sumamplitude_depth1_HF1");
//      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth1_HF \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(7);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_sumamplitude_depth2_HF1");
//      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth2_HF \b");
      aaaaaa1->SetMarkerColor(2);
      aaaaaa1->SetLineColor(2);
      aaaaaa1->Draw("");
       
      c1->cd(8);
      TH1F *aaaaaa1= (TH1F*)hfile1->Get("h_sumamplitude_depth4_HO1");
//      gPad->SetLogy();
      aaaaaa1->SetMarkerStyle(20);
      aaaaaa1->SetMarkerSize(0.8);
      aaaaaa1->GetYaxis()->SetLabelSize(0.04);
      aaaaaa1->SetXTitle("ADC depth4_HO \b");
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
