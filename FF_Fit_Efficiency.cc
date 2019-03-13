/**
 * @file	FFFits.cc
 * @date	May 9, 2011
 * @author	petric
 * @brief	brief description
 *
 * long description
 */

#include "FF_Fit.h"
#include "FF_Util.h"
// Utilities
#include "mp_utility.h"

using namespace FFUtil;

namespace FFFits {
    
    void FitEfficiencyResolution(shared_ptr<TChain> chain_sel, shared_ptr<TChain> chain_gen, shared_ptr<FFOpt> opt) {
        // map to hold all events that have been selected
        std::map<std::string,bool> accepted_exp_run_evt;
        std::map<std::string,std::array<double,3> > resol_exp_run_evt;
        typedef std::map<std::string,bool>::iterator map_iterator;
        
        // set branch address for generated events chain hash
        float expno;
        chain_sel->SetBranchAddress("expno",&expno);
        float runno;
        chain_sel->SetBranchAddress("runno",&runno);
        float evtno;
        chain_sel->SetBranchAddress("evtno",&evtno);
        float decay;
        chain_sel->SetBranchAddress("decay",&decay);
        float vf_deczm;
        chain_sel->SetBranchAddress("vf_deczm",&vf_deczm);
        
        // other variables we need
        float istrue;
        chain_sel->SetBranchAddress("istrue",&istrue);
        float CSNB_KPIPI0_5;
        chain_sel->SetBranchAddress("CSNB_KPIPI0_5",&CSNB_KPIPI0_5);
        
        // variables for resolution
        float f_mass_01;
        chain_sel->SetBranchAddress("mass_01", &f_mass_01);
        float f_mass_12;
        chain_sel->SetBranchAddress("mass_12", &f_mass_12);
        float f_mass_02;
        chain_sel->SetBranchAddress("mass_02", &f_mass_02);
        
        time_t rawtime;
        struct tm* timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        std::cout << "TIMER: Current local time and date: " << asctime(timeinfo);
        
        // loop over selected events, pick true ones (can be only one per event) and store their event_run_exp hash
        for(Long_t i = 0; i < chain_sel->GetEntries(); ++i) {
            chain_sel->GetEntry(i);
            
            if(istrue == 1.0) {
                std::string hash(mp::to_string((int)decay));
                hash += "_";
                hash += mp::to_string((int)expno);
                hash += "_";
                hash += mp::to_string((int)runno);
                hash += "_";
                hash += mp::to_string((int)evtno);
                hash += "_";
                hash += mp::to_string((int)(vf_deczm*100000));
                
                
                map_iterator it = accepted_exp_run_evt.find(hash);
                if(it == accepted_exp_run_evt.end()) {
                    // We need to make the network cut here by hand, as this variable is not on the generated tuple
                    // and we need to reject the events due to our later hel_th1_cut
                    
                    float sqrt_m12 = TMath::Sqrt(f_mass_12);
                    float sqrt_m23 = TMath::Sqrt(f_mass_01);
                    
                    if(CSNB_KPIPI0_5 > opt->GetNetworkCut() && ComplexPDF::InsideDalitzLimits(sqrt_m12,sqrt_m23,m_B0,m_Pi0,m_Pi,m_K)) {
                        accepted_exp_run_evt[hash] = true;
                        // store values for resolution
                        resol_exp_run_evt[hash] = {{ f_mass_01, f_mass_12, f_mass_02 }};
                    }
                } else {
                    std::cerr << "ERROR: Hash " << hash << " already in map. After BCS this should not happen, before it might be a curler!" << std::endl;
                }
            } else {
                continue;
            }
        }
        std::cout << "INFO: " << accepted_exp_run_evt.size() << " true events stored in map" << std::endl;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        std::cout << "TIMER: Current local time and date: " << asctime(timeinfo);
        
        // prepare output file with information whether event passed or not
        shared_ptr<TFile> file_out = make_shared<TFile>((opt->GetOutdir()+"flagged_events.root").c_str(),"RECREATE");
        shared_ptr<TTree> tree_out = make_shared<TTree>("flagged_events","flagged_events");
        int f_cut;
        tree_out->Branch("cut", &f_cut, "cut/I");
        float f_res_mass_01;
        tree_out->Branch("res_mass_01", &f_res_mass_01, "res_mass_01/F");
        float f_res_mass_12;
        tree_out->Branch("res_mass_12", &f_res_mass_12, "res_mass_12/F");
        float f_res_mass_02;
        tree_out->Branch("res_mass_02", &f_res_mass_02, "res_mass_02/F");
        
        // reset branch address for generated events chain hash
        chain_gen->SetBranchAddress("expno",&expno);
        chain_gen->SetBranchAddress("runno",&runno);
        chain_gen->SetBranchAddress("evtno",&evtno);
        chain_gen->SetBranchAddress("decay",&decay);
        chain_gen->SetBranchAddress("vf_deczm",&vf_deczm);
        
        chain_gen->SetBranchAddress("mass_01", &f_mass_01);
        chain_gen->SetBranchAddress("mass_12", &f_mass_12);
        chain_gen->SetBranchAddress("mass_02", &f_mass_02);
        
        int count_true = 0;
        // loop over all generated ones, check if this candidate was selected and flag it
        for(Long_t i = 0; i < chain_gen->GetEntries(); ++i) {
            chain_gen->GetEntry(i);
            
            f_res_mass_01 = -999;
            f_res_mass_12 = -999;
            f_res_mass_02 = -999;
            
            // this cut is to optimize a little bit the speed
            if(true) {
                std::string hash(mp::to_string((int)decay));
                hash += "_";
                hash += mp::to_string((int)expno);
                hash += "_";
                hash += mp::to_string((int)runno);
                hash += "_";
                hash += mp::to_string((int)evtno);
                hash += "_";
                hash += mp::to_string((int)(vf_deczm*100000));

                map_iterator it = accepted_exp_run_evt.find(hash);
                if(it != accepted_exp_run_evt.end() ) {
                    // event accepted
                    f_cut = 1;
                    (*it).second = false;
                    ++count_true;
                    // get reconstructed values for resolution
                    auto it_res = resol_exp_run_evt.find(hash);
                    // compute resolution
                    f_res_mass_01 = TMath::Sqrt(f_mass_01) - TMath::Sqrt((*it_res).second[0]);
                    f_res_mass_12 = TMath::Sqrt(f_mass_12) - TMath::Sqrt((*it_res).second[1]);
                    f_res_mass_02 = TMath::Sqrt(f_mass_02) - TMath::Sqrt((*it_res).second[2]);
                    
                } else {
                    // event rejected (by detector acceptance or selection)
                    f_cut = 0;
                }
            } else {
                // event rejected as it has by far to large mass_12 value
                f_cut = 0;
            }
            
            tree_out->Fill();
        }
        file_out->Write();
        
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        std::cout << "TIMER: Current local time and date: " << asctime(timeinfo);
        /*
        for(auto &i : accepted_exp_run_evt) {
            if(i.second == true)
                std::cout << "WARNING: Hash " << i.first << " still true" << std::endl;
        }*/
        std::cout << "INFO: " << count_true << " true events found in total sample" << std::endl;
        std::cout << "INFO: " << chain_gen->GetEntries() << " events in total" << std::endl;
        std::cout << "INFO: " << count_true/(double)chain_gen->GetEntries()*100.0<< " average efficiency" << std::endl;
        
        chain_gen->AddFriend(tree_out.get());
        
        std::cout << "STATUS: Define functions and variables..." << std::endl;
        RooRealVar res_mass_01("res_mass_01","Resolution m^{2}_{K#pi}",-1000,1000,"GeV^{2}/c^{4}");
        RooRealVar res_mass_12("res_mass_12","Resolution m^{2}_{#pi#pi^{0}}",-1000,1000,"GeV^{2}/c^{4}");
        RooRealVar res_mass_02("res_mass_02","Resolution m^{2}_{K#pi^{0}}",-1000,1000,"GeV^{2}/c^{4}");
        
        RooArgSet varset;
        varset.add(res_mass_01);
        varset.add(res_mass_12);
        varset.add(res_mass_02);
        RooDataSet data("data","data",chain_gen.get(),varset);
        
        std::vector<RooRealVar*> variables_resolution;
        variables_resolution.push_back(&res_mass_01);
        variables_resolution.push_back(&res_mass_12);
        variables_resolution.push_back(&res_mass_02);

        res_mass_01.setRange("resolution",-0.08,0.08);
        res_mass_12.setRange("resolution",-0.08,0.08);
        res_mass_02.setRange("resolution",-0.08,0.08);
        
        PrintFitStartBlock("Resolution Charged");
        RooDataSet* resdata_charged = dynamic_cast<RooDataSet*>(data.reduce(CutRange("resolution")));
        resdata_charged->Print();
        
         for(RooRealVar* v : variables_resolution) {
         RooRealVar mean1("mean1","mean1",2.55562e-03,-0.1,0.1);
         RooRealVar width1("width1","width1",1.01048e-02,0,10);
         RooGaussian res1("res1","res1",*v,mean1,width1);
         
         RooRealVar mean2("mean2","mean2",3.87601e-03,-0.1,0.1);
         RooRealVar width2("width2","width2",3.23313e-02,0,10);
         RooGaussian res2("res2","res2",*v,mean2,width2);
         
         RooRealVar mean3("mean3","mean3",5.88851e-04,-0.1,0.1);
         RooRealVar width3("width3","width3",4.49691e-03,0,10);
         RooGaussian res3("res3","res3",*v,mean3,width3);
         
         RooRealVar n12("n12","n12",8.26825e-01,0,1);
         RooRealVar n23("n23","n23",5.03481e-01,0,1);
         
         RooAddPdf res("res","res",RooArgList(res1,res2,res3),RooArgList(n12,n23),true);
         
         RooFitResult *result_res = res.fitTo(*resdata_charged, Range("resolution"), Save(true));
         
         TCanvas* c1 = new TCanvas("c1","c1",1280,1024);
         RooPlot* plot = v->frame(Bins(100), Range("resolution"));
         resdata_charged->plotOn(plot);
         res.plotOn(plot, LineColor(kBlue), LineStyle(kDashed));
         plot->Draw("");
         plot->SetMinimum(0.0);
         plot->SetMaximum(plot->GetMaximum()*1.3);
         
         TLatex text;
         text.SetTextColor(kBlack);
         text.SetTextFont(42);
         text.SetTextSize(0.045);
         text.SetNDC(1);
         std::stringstream ss;
         ss.precision(3);
         ss << "m = " << mean1.getVal()*n12.getVal() +(1-n12.getVal())*(mean2.getVal()*n23.getVal() +mean3.getVal()*(1-n23.getVal())) << " #pm " << mean1.getError()*n12.getVal() + (1-n12.getVal())*(mean2.getError()*n23.getVal()+ mean3.getError()*(1-n23.getVal())); //corect error calculation
         text.DrawLatex(0.18, 0.9, ss.str().c_str());
         ss.str(std::string());
         ss << "#sigma = " << width1.getVal()*n12.getVal() +(1-n12.getVal())*(width2.getVal()*n23.getVal() +width3.getVal()*(1-n23.getVal())) << " #pm " << width1.getError()*n12.getVal() + (1-n12.getVal())*(width2.getError()*n23.getVal() +width3.getError()*(1-n23.getVal())); //corect error calculation
         text.DrawLatex(0.18, 0.85, ss.str().c_str());
         
         c1->SaveAs((opt->GetOutdir()+"resolution_"+v->GetName()+"."+opt->GetPlotFormat()).c_str());
         
         std::cout << "INFO: Resolution of " << v->GetName() << " is " << width1.getVal()*n12.getVal() +(1-n12.getVal())*(width2.getVal()*n23.getVal() +width3.getVal()*(1-n23.getVal())) << " " << v->getUnit() << "\n";
         
         delete c1;
         delete result_res;
         delete plot;
         }
        
        
        delete resdata_charged;
        
        
        
    }
    void FitEfficiency(shared_ptr<TChain> chain_sel, shared_ptr<TChain> chain_gen, shared_ptr<FFOpt> opt) {
        
        //open the file from flagging
        shared_ptr<TChain> chain_flagged = make_shared<TChain>("flagged_events");
        chain_flagged->Add((opt->GetOutdir()+"flagged_events.root").c_str());
        //append the friend
        chain_gen->AddFriend(chain_flagged.get());
        
        std::cout<<"gen events: "<<chain_flagged->GetEntries()<<"\t"<<"friend events"<<chain_flagged->GetEntries()<<std::endl;
        
        
        // ---------------------------------------------
        // ----- Define variables
        // ---------------------------------------------
        std::cout << "STATUS: Define functions and variables..." << std::endl;
        
        RooRealVar mass_01("mass_01","M_{K#pi}^{2}",0,27,"GeV");
        RooRealVar mass_12("mass_12","M_{#pi#pi^{0}}^{2}",0,27,"GeV");
        RooRealVar idhep("idhep","idhep",-999,999,"");
        
        RooCategory cut("cut","cut");
        cut.defineType("accept",1);
        cut.defineType("reject",0);
        
        RooCategory channel("decay","decay channel");
        channel.defineType("C",0);
        channel.defineType("N",1);
        
        RooArgSet varset(mass_01,mass_12,channel);
        varset.add(cut);
        varset.add(idhep);
        
        mass_12.setRange("full_range",0, 27);
        mass_01.setRange("full_range",0, 27);
        
        std::vector<std::string> flags_channel;
        flags_channel.push_back("C");
        flags_channel.push_back("N");
        tuple_category_name_flags category_name_flags_channel = std::make_tuple(&channel,"decay",flags_channel);
        
        RooDataSet *data_charged = new RooDataSet("data","data",chain_gen.get(),varset);
        
        std::vector<RooRealVar*> variables;
        variables.push_back(&mass_12);
        variables.push_back(&mass_01);
        
        /*
         PrintFitStartBlock("Data Charged");
         
         RooDataHist* data_hist = data_charged->binnedClone();
         
         shared_ptr<FF_PDF> pdf_eff_2D = make_shared<FF_PDF_efficiency_2D_dalitz>(mass_12, mass_01, category_name_flags_channel);
         
         PrintFitStartBlock("Plotting: 2D");
         for(RooRealVar *v : variables) {
         std::string varname = v->GetName();
         FFUtil::VisualizeFitResult(pdf_eff_2D,*data_charged,*v,category_name_flags_channel,false,
         opt->GetBins(),"full_range", opt->GetOutdir()+"Efficiency_PDF_2D_"+varname+"_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         }*/
        
        /*
         TH1::AddDirectory(kFALSE);
         TFile* graph_file= new TFile((opt->GetOutdir()+"efficiency_2D_"+mass_12.GetName()+"_vs_"+mass_01.GetName()+".root").c_str(),"READ");
         TGraph2D *graph = dynamic_cast<TGraph2D*>(graph_file->Get("Graph2D"));
         graph_file->Close();
         
         RooDalitzDelaunay *pdf = new RooDalitzDelaunay("test","test",mass_12,mass_01,graph,m_B0,m_Pi0,m_Pi,m_K);
         
         TH1 *test_pdf = pdf->createHistogram("hh_data_eff_C",mass_12,Binning(100),YVar(mass_01,Binning(100)));
         
         
         TCanvas* canvas = new TCanvas("c_canvas","c_canvas",2*1280,2*1024);
         test_pdf->Draw("COLZ");
         canvas->SaveAs("test.pdf");
         
         delete canvas;
         */
        
        
        data_charged->Print();
        PrintFitStartBlock("Efficiency Charged - Binned");
        //        {
        const unsigned int nbin_mass_12 = 28;
        const unsigned int nbin_mass_01 = 28;
        const unsigned int n_flavour = 3;
        // index[mass_12][mass_01][hel_th1][hel_th2]
        std::array<std::array<std::array<unsigned int,n_flavour>,nbin_mass_01>,nbin_mass_12> hist_gen;
        std::array<std::array<std::array<unsigned int,n_flavour>,nbin_mass_01>,nbin_mass_12> hist_sel;
        std::array<std::array<std::array<double,n_flavour>,nbin_mass_01>,nbin_mass_12> hist_eff;
        std::array<std::array<std::array<double,n_flavour>,nbin_mass_01>,nbin_mass_12> hist_eff_err;
        // null arrays
        
        for(unsigned int i = 0; i < nbin_mass_12; ++i) {
            for(unsigned int j = 0; j < nbin_mass_01; ++j) {
                for(unsigned int k = 0; k < n_flavour; ++k) {
                    hist_gen[i][j][k] = 0;
                    hist_sel[i][j][k] = 0;
                    hist_eff[i][j][k] = 0.0;
                    hist_eff_err[i][j][k] = 0.0;
                }
            }
        }
        
        float min_mass_01 = 0;
        float max_mass_01 = 28;
        
        float min_mass_12 = 0;
        float max_mass_12 = 28;
        
        float step_mass_12 = (max_mass_12 - min_mass_12)/nbin_mass_12;
        float step_mass_01 = (max_mass_01 - min_mass_01)/nbin_mass_01;
        
        RooRealVar* var;
        RooCategory* cat;
        const RooArgSet* set;
        
        Long64_t nentries = data_charged->numEntries();
        
        for(unsigned int i = 0; i < nentries; ++i) {
            set = data_charged->get(i);
            var = static_cast<RooRealVar*>(set->find("mass_01"));
            double d_mass_01 = var->getVal();
            var = static_cast<RooRealVar*>(set->find("mass_12"));
            double d_mass_12 = var->getVal();
            var = static_cast<RooRealVar*>(set->find("idhep"));
            double flavour = var->getVal();
            cat = static_cast<RooCategory*>(set->find("cut"));
            int i_cut = cat->getIndex();
            
            unsigned int bin_mass_12 = 0;
            for(float c = min_mass_12+step_mass_12; c <= max_mass_12; c += step_mass_12) {
                if(d_mass_12 < c)
                    break;
                ++bin_mass_12;
            }
            unsigned int bin_mass_01 = 0;
            for(float c = min_mass_01+step_mass_01; c <= max_mass_01; c += step_mass_01) {
                if(d_mass_01 < c)
                    break;
                ++bin_mass_01;
            }
            
            ++hist_gen[bin_mass_12][bin_mass_01][2];
            if(i_cut == 1){
                ++hist_sel[bin_mass_12][bin_mass_01][2];
            }
            if (flavour>0) {
                ++hist_gen[bin_mass_12][bin_mass_01][0];
                if(i_cut == 1){
                    ++hist_sel[bin_mass_12][bin_mass_01][0];
                }
            }else{
                ++hist_gen[bin_mass_12][bin_mass_01][1];
                if(i_cut == 1){
                    ++hist_sel[bin_mass_12][bin_mass_01][1];
                }
            }
        }
        
        // compute efficiency histogram
        for(unsigned int k = 0; k < n_flavour; ++k) {
            for(unsigned int i = 0; i < nbin_mass_12; ++i) {
                for(unsigned int j = 0; j < nbin_mass_01; ++j) {
                    if(hist_sel[i][j][k] == 0 || hist_gen[i][j][k] == 0){
                        hist_eff[i][j][k] = 0;
                        hist_eff_err[i][j][k] = 0;
                    }
                    else{
                        hist_eff[i][j][k] = hist_sel[i][j][k]/(float)hist_gen[i][j][k];
                        double n_sel = (double)hist_sel[i][j][k];
                        double n_all = (double)hist_gen[i][j][k];
                        hist_eff_err[i][j][k] = std::sqrt((n_sel*(n_all-n_sel))/(n_all*n_all*n_all));
                    }
                    
                }
            }
        }
        
        std::array<std::array<float,nbin_mass_12>,nbin_mass_01> hist_eff_chi2;
        double chi2_eff = 0;
        int ndf_eff = 0;
        TH2F *eff_chi2 = new TH2F("eff_chi2","eff_chi2",nbin_mass_12,min_mass_12,max_mass_12,nbin_mass_01,min_mass_01,max_mass_01);
        
        TH1* pull_deviations = new TH1F("pull_devitions","pull_deviations",40,-5,5);
        
        for(unsigned int i = 0; i < nbin_mass_12; ++i) {
            for(unsigned int j = 0; j < nbin_mass_01; ++j) {
                if(hist_eff[i][j][0] == 0 || hist_eff[i][j][1] == 0){
                    hist_eff_chi2[i][j] == 0;
                }
                else{
                    hist_eff_chi2[i][j] = (hist_eff[i][j][0] - hist_eff[i][j][1]) / std::sqrt( hist_eff_err[i][j][0]*hist_eff_err[i][j][0] + hist_eff_err[i][j][1]*hist_eff_err[i][j][1]);
                    pull_deviations->Fill(hist_eff_chi2[i][j]);
                    hist_eff_chi2[i][j] = hist_eff_chi2[i][j]*hist_eff_chi2[i][j];
                    chi2_eff+=hist_eff_chi2[i][j];
                    ndf_eff++;
                    eff_chi2->SetBinContent(i,j,hist_eff_chi2[i][j]);
                }
                
            }
        }
        
        std::cout << "Chi2: " << chi2_eff << std::endl;
        std::cout << "ndf:  " << ndf_eff << std::endl;
        std::cout << "Chi2/ndf: " << chi2_eff/ndf_eff << std::endl;
        std::cout << "Prob: " << TMath::Prob(chi2_eff,ndf_eff) << std::endl;
        
                SetRootStyleLarge2D();
        
        TCanvas* canvas = new TCanvas("c_canvas","c_canvas",2*1280,2*1024);
        eff_chi2->Draw("COLZ");
        canvas->SaveAs((opt->GetOutdir()+"efficiency_chi2_2D_"+mass_12.GetName()+"_vs_"+mass_01.GetName()+"_C."+opt->GetPlotFormat()).c_str());
        
        canvas->Clear();
        canvas->cd(1);
        std::cout << "Mean: " << pull_deviations->GetMean() << "   " << pull_deviations->GetMeanError() << std::endl;
        std::cout << "RMS: " << pull_deviations->GetRMS() << "   " << pull_deviations->GetRMSError() << std::endl;
        pull_deviations->SetFillColor(kOrange);
        pull_deviations->Draw("HIST");
        pull_deviations->GetXaxis()->SetTitle("Deviation in #sigma");
        pull_deviations->GetYaxis()->SetTitle("Entries");
        
        TLatex text;
        text.SetTextColor(kBlack);
        text.SetTextFont(42);
        text.SetTextSize(0.045);
        text.SetNDC(1);
        std::stringstream ss;
        ss.precision(3);
        ss << "m_{#Delta#varepsilon} = " << pull_deviations->GetMean() << " #pm " << pull_deviations->GetMeanError();
        text.DrawLatex(0.15, 0.9, ss.str().c_str());
        std::stringstream ss2;
        ss2.precision(3);
        ss2 << "#sigma_{#Delta#varepsilon} = " << pull_deviations->GetRMS() << " #pm " << pull_deviations->GetRMSError();
        text.DrawLatex(0.15, 0.8, ss2.str().c_str());
        
        canvas->SaveAs((opt->GetOutdir()+"efficiency_2D_deviations_"+mass_12.GetName()+"_vs_"+mass_01.GetName()+"_pull."+opt->GetPlotFormat()).c_str());
        
        delete pull_deviations;
        
        canvas->Clear();
        
        
        //continuation of efficiency into kenmaticaly forbiden area for delauney triangulation
        
        for(unsigned int i = 0; i < nbin_mass_12; i++) {
            for(unsigned int j = 0; j < nbin_mass_01-1; j++) {
                if (hist_eff[i][j+1][2]==0 && hist_eff[i][j][2]>0) {
                    hist_eff[i][j+1][2] = hist_eff[i][j][2];
                    j=0;
                    i++;
                }
            }
        }
        
        for(unsigned int i = 0; i < nbin_mass_12; i++) {
            for(unsigned int j = 0; j < 5; j++) {
                if (hist_eff[i+1][j][2]==0 && hist_eff[i][j][2]>0) {
                    if (hist_eff[i][j][2]!=hist_eff[i-1][j][2]) {
                        hist_eff[i+1][j][2] = hist_eff[i][j][2];
                    }
                }
            }
        }
        
        //fill graph for triangulation
        TGraph2D *eff_graph = new 	TGraph2D();
        int count=0;
        for(unsigned int i = 0; i < nbin_mass_12; i++) {
            for(unsigned int j = 0; j < nbin_mass_01; j++) {
                if (hist_eff[i][j][2]>0) {
                    eff_graph->SetPoint(count, min_mass_12 + step_mass_12/2.0 + i*step_mass_12, min_mass_01 + step_mass_01/2.0 + j*step_mass_01, hist_eff[i][j][2]);
                    count++;
                }
            }
        }
        
        //extend edge for proper triangulation
        for(unsigned int j = 0; j < nbin_mass_01; j++) {
            int i = 0;
            if (hist_eff[i][j][2]>0) {
                eff_graph->SetPoint(count, 0.0 , min_mass_01 + step_mass_01/2.0 + j*step_mass_01, hist_eff[i][j][2]);
                count++;
            }
        }
        
        for(unsigned int i = 0; i < nbin_mass_12; i++) {
            int j = 0;
            if (hist_eff[i][j][2]>0) {
                eff_graph->SetPoint(count, min_mass_12 + step_mass_12/2.0 + i*step_mass_12 , 0.0, hist_eff[i][j][2]);
                count++;
            }
        }
        
        eff_graph->SetPoint(count, 0.0 , 0.0, hist_eff[0][0][2]);
        
        // consistency checks
        for(unsigned int i = 0; i < nbin_mass_12; ++i) {
            for(unsigned int j = 0; j < nbin_mass_01; ++j) {
                if(hist_gen[i][j][2] > nentries)
                    std::cerr << "ERROR: Found generated bin with more than nentries!\n";
                if(hist_sel[i][j][2] > nentries)
                    std::cerr << "ERROR: Found selected bin with more than nentries!\n";
                if(hist_eff[i][j][2] > 1.0)
                    std::cerr << "ERROR: Efficiency larger 1!\n";
            }
        }
        
        int nbin_mass_12_hist = 1000;
        int nbin_mass_01_hist = nbin_mass_12_hist;
        double step = 27.0/(double)nbin_mass_12_hist;
        
        std::ofstream fout(opt->GetOutdir()+"efficiency_histogram_2D.cc");
        if(fout.is_open()) {
            fout << "#include <array>\n";
            fout << "\n";
            fout << "unsigned int GetBin_mass_12(double d_mass_12) {\n";
            fout << "  return (unsigned int)((d_mass_12-"<<step/2.0<<")/"<<step<<");\n";
            fout << "}\n";
            fout << "\n";
            fout << "unsigned int GetBin_mass_01(double d_mass_01) {\n";
            fout << "  return (unsigned int)((d_mass_01-"<<step/2.0<<")/"<<step<<");\n";
            fout << "}\n";
            fout << "\n";
            fout << "// index[mass_12][mass_01]\n";
            fout << "std::array<std::array<double," << nbin_mass_12_hist << ">," << nbin_mass_01_hist << "> hist_eff = \n";
            fout << "{";
            for(unsigned int i = 0; i < nbin_mass_12_hist; i++) {
                fout << "{";
                for(unsigned int j = 0; j < nbin_mass_01_hist; j++) {
                    if(j != 0){
                        fout << ",";
                    }
                    fout << eff_graph->Interpolate(step/2.0 + i*step, step/2.0 + j*step);
                    if ((nbin_mass_12_hist*nbin_mass_12_hist - i*nbin_mass_12_hist-j)%1000 == 0) {
                                            std::cout <<nbin_mass_12_hist*nbin_mass_12_hist - i*nbin_mass_12_hist-j<<" ";
                    }
                }
                if (i==nbin_mass_12_hist-1) {
                    fout << "}\n";
                }else{
                    fout << "},\n";
                }
            }
            fout << "};";
            fout << "\n";
            /*
            fout << "\n";
            fout << "<std::array<std::array<float," << nbin_mass_12 << ">," << nbin_mass_01 << "> hist_eff_err = \n";
            fout << "{";
            for(unsigned int i = 0; i < nbin_mass_12; ++i) {
                fout << "{";
                for(unsigned int j = 0; j < nbin_mass_01; j++) {
                    if(j != 0){
                        fout << ",";
                    }
                    fout << hist_eff_err[i][j][2];
                }
                if (i==nbin_mass_12-1) {
                    fout << "}\n";
                }else{
                    fout << "},\n";
                }
            }
            fout << "};";
         */
        }
        fout << "\n";
        fout.close();
        //        }
        
        
        //test that everything is ok
        for(unsigned int i = 0; i < nbin_mass_12; i++) {
            for(unsigned int j = 0; j < nbin_mass_01; j++) {
                if (hist_eff[i][j][2]>0) {
                    if (std::abs((eff_graph->Interpolate(min_mass_12 + step_mass_12/2.0 + i*step_mass_12, min_mass_01 + step_mass_01/2.0 + j*step_mass_01) - hist_eff[i][j][2])/hist_eff[i][j][2]) * 100 > 1) {
                        std::cout << min_mass_12 + step_mass_12/2.0 + i*step_mass_12 <<"\t"<< min_mass_01 + step_mass_01/2.0 + j*step_mass_01 <<"\t"<< eff_graph->Interpolate(min_mass_12 + step_mass_12/2.0 + i*step_mass_12, min_mass_01 + step_mass_01/2.0 + j*step_mass_01)<<"\t"<<hist_eff[i][j][2]<<std::endl;
                    }
                }
            }
        }
        
        TH2F *eff_histo = new TH2F("eff_histo","eff_histo",1000,0,27,1000,0,27);
        double x=0;
        double y=0;
        for (int i=1; i<=1000; i++) {
            for (int j=1; j<=1000; j++) {
                x=27.0/1000.0*(i-1);
                y=27.0/1000.0*(j-1);
                if (ComplexPDF::InsideDalitzLimits(std::sqrt(x),std::sqrt(y),m_B0,m_Pi0,m_Pi,m_K)) {
                    eff_histo->SetBinContent(i, j, eff_graph->Interpolate(x,y));
                }
            }
        }

        
        std::cout<<"Integral of efficiency: "<<eff_histo->Integral("width")<<std::endl;
        /*
        TFile* rootFile = new TFile((opt->GetOutdir()+"efficiency_2D_"+mass_12.GetName()+"_vs_"+mass_01.GetName()+".root").c_str(), "RECREATE");
        rootFile->cd();
        eff_graph->Write();
        rootFile->Close();
        delete rootFile;
        */
        std::string varname_v1 = mass_12.GetName();
        std::string varname_v2 = mass_01.GetName();
        
        TH1* hh_data_all_C = data_charged->createHistogram("hh_data_all_C",mass_12,Binning(28),YVar(mass_01,Binning(28)));
        TH1* hh_data_eff_C = data_charged->createHistogram("hh_data_eff_C",mass_12,Binning(28),YVar(mass_01,Binning(28)),Cut("cut==cut::accept"));
        hh_data_eff_C->Divide(hh_data_all_C);
        hh_data_eff_C->SetMinimum(0);
//        hh_data_eff_C->SetMaximum(hh_data_eff_C->GetMaximum());
        hh_data_eff_C->GetXaxis()->SetTitle("m^{2}_{#pi#pi^{0}} (GeV^{2}/c^{4})");
        hh_data_eff_C->GetYaxis()->SetTitle("m^{2}_{K#pi^{0}} (GeV^{2}/c^{4})");
        hh_data_eff_C->GetXaxis()->CenterTitle();
        hh_data_eff_C->GetYaxis()->CenterTitle();
        hh_data_eff_C->GetZaxis()->SetTitle("");
        hh_data_eff_C->Draw("COLZ");

        canvas->SaveAs((opt->GetOutdir()+"efficiency_2D_"+mass_12.GetName()+"_vs_"+mass_01.GetName()+"_binned."+opt->GetPlotFormat()).c_str());
        

        eff_histo->SetMinimum(0);
//        eff_graph->SetMaximum(hh_data_eff_C->GetMaximum());
        eff_histo->GetXaxis()->SetTitle("m^{2}_{#pi#pi^{0}} (GeV^{2}/c^{4})");
        eff_histo->GetYaxis()->SetTitle("m^{2}_{K#pi^{0}} (GeV^{2}/c^{4})");
        eff_histo->GetZaxis()->SetTitle("");
        eff_histo->GetXaxis()->CenterTitle();
        eff_histo->GetYaxis()->CenterTitle();
        eff_histo->Draw("COLZ");

        canvas->SaveAs((opt->GetOutdir()+"efficiency_2D_"+mass_12.GetName()+"_vs_"+mass_01.GetName()+"_graph."+opt->GetPlotFormat()).c_str());
        
        
        SetRootStyleLarge();
        
        delete canvas;
        delete hh_data_all_C;
        delete hh_data_eff_C;
        delete eff_graph;
        delete data_charged;
        
    }
    
}
