/**
 * @file	FFFits.cc
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
    
    void FitContinuumBackground(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt) {
        if(chain->GetEntries() == 0) {
            std::cout << "No data for continuum background fits given, skip fitting." << std::endl;
            return;
        }
        
        // ---------------------------------------------
        // ----- Define variables
        // ---------------------------------------------
        PrintFitStartBlock("STATUS: Define functions and variables...");
        RooRealVar mbc("mbcp","M'_{bc}",5.25, 5.288,"GeV/c^{2}");
        RooRealVar deltae("deltae","#Delta E",-0.2,0.2,"GeV");
        //    RooRealVar mbcp("mbcp","m'_{bc}",5.25,5.29,"GeV");
        RooRealVar mass_BC("mass_01","m^{2}_{K#pi}",0,27,"GeV^{2}/c^{4}");//mass_BC_min+1.0e-4,mass_BC_max-1.0e-4,"GeV^{2}");
        RooRealVar mass_AB("mass_12","m^{2}_{#pi#pi^{0}}",0,27,"GeV^{2}/c^{4}");//mass_AB_min+1.0e-4,mass_AB_max-1.0e-4,"GeV^{2}");
        RooRealVar mass_CA("mass_02","m^{2}_{K#pi^0}",0,27,"GeV^{2}");
        RooRealVar nb_prime("nb_prime","C'_{NB}",-10.0,10.0,"");
        RooRealVar mchash("mchash","mchash'",0,1e+9,"");
        RooCategory channel("channel","decay channel");
        channel.defineType("C",0);
        channel.defineType("N",1);
        RooCategory flavor("flavor","flavor type");
        flavor.defineType("CP",0);
        flavor.defineType("CA",1);
        // necessary for MC information checks
        RooRealVar mcinfo("mcinfo","MC information",0,256);
        RooRealVar mctype("mctype","MC type information",-1,1005);
        RooRealVar chil0_mo("chil0_mo","MC K mother",-999999,999999);
        RooRealVar chil1_mo("chil1_mo","MC #pi mother",-999999,999999);
        RooRealVar chil2_mo("chil2_mo","MC #pi^{0} mother",-999999,999999);
        RooRealVar chil0_mc("chil0_mc","MC information K",0,256);
        RooRealVar chil1_mc("chil1_mc","MC information #pi",0,256);
        RooRealVar chil2_mc("chil2_mc","MC information #pi^{0}",0,256);
        RooRealVar runno("runno","Run Number",-999999,999999);
        RooCategory dalitz("dalitz","Inside Dalitz space");
        dalitz.defineType("IN",1);
        dalitz.defineType("OUT",-1);
        // event weight
        RooRealVar weight("weight","weight",-999999,999999);
        // create data set (constructor only overload for max. 9 arguments, so we add some afterwards)
        RooArgSet varset(mbc,deltae, mass_AB, mass_BC, mass_CA, nb_prime,mchash);
        varset.add(channel);
        varset.add(mcinfo);
        varset.add(mctype);
        varset.add(chil0_mo);
        varset.add(chil1_mo);
        varset.add(chil2_mo);
        varset.add(chil0_mc);
        varset.add(chil1_mc);
        varset.add(chil2_mc);
        varset.add(dalitz);
        varset.add(weight);
        varset.add(flavor);
        
        RooDataSet data("data","data",varset,Import(*chain.get()),Cut("mbcp < 5.288 && dalitz==dalitz::IN"),WeightVar(weight));
        /*
         TH2F *data_hist = dynamic_cast<TH2F*>(data.createHistogram(mass_AB, mass_BC,300,300,"!((chil0_mo==chil1_mo && abs(chil0_mo)==113) || (chil0_mo==chil1_mo && abs(chil0_mo)==313) || (chil0_mo==chil1_mo && abs(chil0_mo)==411) || (chil0_mo==chil1_mo && abs(chil0_mo)==421) || (chil2_mo==chil1_mo && abs(chil2_mo)==213) || (chil2_mo==chil1_mo && abs(chil2_mo)==223) || (chil2_mo==chil0_mo && abs(chil2_mo)==213) || (chil2_mo==chil0_mo && abs(chil2_mo)==323))","data_hist"));
         
         //Save the PDF to file
         
         TFile *file = new TFile("plots/continuum_background.root", "RECREATE");
         file->cd();
         data_hist->Write();
         file->Close();
         //*/
        
        
        //define the dalitz phasepsace
        
        // create helper for the simultanous fit
        std::vector<std::string> flags;
        flags.push_back("C");
        tuple_category_name_flags category_name_flags = std::make_tuple(&channel,"channel",flags);
        // create data subsamples
        RooDataSet* data_charged = dynamic_cast<RooDataSet*>(data.reduce("channel==channel::C"));
        data_charged->Print();
        // print data set
        data.Print();
        
        std::vector<RooRealVar*> variables_5D;
        variables_5D.push_back(&mbc);
        variables_5D.push_back(&deltae);
        variables_5D.push_back(&nb_prime);
        variables_5D.push_back(&mass_AB);
        variables_5D.push_back(&mass_BC);
        
        std::vector<RooRealVar*> variables_3D;
        variables_3D.push_back(&mbc);
        variables_3D.push_back(&deltae);
        variables_3D.push_back(&nb_prime);
        
        std::vector<RooRealVar*> variables_2D;
        variables_2D.push_back(&mass_AB);
        variables_2D.push_back(&mass_BC);
        
        // ---------------------------------------------
        // ----- Define full data range
        // ---------------------------------------------
        //SetFitRanges(mbc,deltae,chil0_ma,mass_kpi,hel_phi,hel_th1,hel_th2,nb_prime);
        mbc.setRange("full_range",5.25, 5.288);
        deltae.setRange("full_range",-0.2, 0.2);
        nb_prime.setRange("full_range",-10.0, 10.0);
        
        mass_AB.setRange("full_range",0, 27);
        mass_BC.setRange("full_range",0, 27);
        
        mass_AB.setRange("full_range1",0, 27);
        mass_BC.setRange("full_range1",0, 3.3);
        mass_AB.setRange("full_range2",0, 27);
        mass_BC.setRange("full_range2",3.3, 3.8);
        mass_AB.setRange("full_range3",0, 27);
        mass_BC.setRange("full_range3",3.8, 27);
        
        mass_AB.setRange("full_range4",0, 2);
        mass_BC.setRange("full_range4",0, 27);
        mass_AB.setRange("full_range5",2, 12);
        mass_BC.setRange("full_range5",0, 27);
        mass_AB.setRange("full_range6",12, 27);
        mass_BC.setRange("full_range6",0, 27);

        
        // ---------------------------------------------
        // ----- Check correlation between input variables
        // ---------------------------------------------
        if(opt->GetCreateCorrelationFiles()) {
            std::cout << "STATUS: Write correlation files..." << std::endl;
            WriteCorrelationMatrixFile(*data_charged,RooArgList(mbc,deltae,nb_prime),opt->GetOutdir()+"correlation_matrix_of_input_variables_C.dat");
            WriteCorrelationStudyFile(*data_charged,RooArgList(mbc,deltae,nb_prime),opt->GetOutdir()+"fit_input_variables_C.dat");
        }
        // ---------------------------------------------
        // ----- Create data component plots
        // ---------------------------------------------
        if(opt->GetCreateComponentPlots()) {
            std::cout << "STATUS: Create data component plots..." << std::endl;
            for(RooRealVar* var : variables_3D) {
                CreateDataComponentPlots(data,*var,opt->GetBins(),"full_range",opt->GetOutdir(),opt->GetPlotFormat(),"C","channel");
            }
            mp::execute_command("mkdir -p "+opt->GetOutdir()+"DATA_bkg_continuum && mv "+opt->GetOutdir()+"DATA_*."+opt->GetPlotFormat()+" "+opt->GetOutdir()+"DATA_bkg_continuum");
        }
        // ---------------------------------------------
        // ----- Do fits
        // ---------------------------------------------
        // See RooFit tutorial macro #501 about HowTo do a simultaneous fit
        // See RooFit tutorial macro #603 about HowTo do a multi-core parallelized unbinned maximum likelihood fit
        std::cout << "STATUS: Do fits..." << std::endl;
        /*
        PrintFitStartBlock("FITTING: mbc");
        shared_ptr<FF_PDF> pdf_bkg_continuum_1D_mbc = make_shared<FF_PDF_bkg_continuum_1D_mbc>(mbc, category_name_flags);
        RooFitResult *result_mbc = pdf_bkg_continuum_1D_mbc->GetSimPDF().fitTo(data, NumCPU(opt->GetNumcpu()),
                                                                               SumW2Error(true), Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()));
        pdf_bkg_continuum_1D_mbc->GetSimPDF().getParameters(data)->Print("v");
        FFUtil::VisualizeFitResult(pdf_bkg_continuum_1D_mbc,data,mbc,category_name_flags,false,
                                   opt->GetBins(),"full_range", opt->GetOutdir()+"CONT_MC_1D_mbc_fit."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                   opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
        WriteCorrelationMatrixFile(result_mbc,opt->GetOutdir()+"pdf_bkg_continuum_1D_mbc_correlation.dat");
        pdf_bkg_continuum_1D_mbc->ExportPDFValues(opt->GetOutdir()+"pdf_bkg_continuum_1D_mbc.dat",true);
        result_mbc->Print();
        delete result_mbc;
        
        PrintFitStartBlock("FITTING: deltae");
        shared_ptr<FF_PDF> pdf_bkg_continuum_1D_deltae = make_shared<FF_PDF_bkg_continuum_1D_deltae>(deltae, category_name_flags);
        RooFitResult *result_deltae = pdf_bkg_continuum_1D_deltae->GetSimPDF().fitTo(data, NumCPU(opt->GetNumcpu()),
                                                                                     SumW2Error(true), Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()));
        pdf_bkg_continuum_1D_deltae->GetSimPDF().getParameters(data)->Print("v");
        FFUtil::VisualizeFitResult(pdf_bkg_continuum_1D_deltae,data,deltae,category_name_flags,false,
                                   opt->GetBins(),"full_range", opt->GetOutdir()+"CONT_MC_1D_deltae_fit."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                   opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
        WriteCorrelationMatrixFile(result_deltae,opt->GetOutdir()+"pdf_bkg_continuum_1D_deltae_correlation.dat");
        pdf_bkg_continuum_1D_deltae->ExportPDFValues(opt->GetOutdir()+"pdf_bkg_continuum_1D_deltae.dat",true);
        result_deltae->Print();
        delete result_deltae;
        
        
        PrintFitStartBlock("FITTING: nb_prime");
        shared_ptr<FF_PDF> pdf_bkg_continuum_1D_nb_prime = make_shared<FF_PDF_bkg_continuum_1D_nb_prime>(nb_prime, category_name_flags);
        RooFitResult *result_nb_prime = pdf_bkg_continuum_1D_nb_prime->GetSimPDF().fitTo(data, NumCPU(opt->GetNumcpu()),
                                                                                         SumW2Error(true), Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()));
        pdf_bkg_continuum_1D_nb_prime->GetSimPDF().getParameters(data)->Print("v");
        FFUtil::VisualizeFitResult(pdf_bkg_continuum_1D_nb_prime,data,nb_prime,category_name_flags,false,
                                   opt->GetBins(),"full_range", opt->GetOutdir()+"CONT_MC_1D_nb_prime_fit."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                   opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
        WriteCorrelationMatrixFile(result_nb_prime,opt->GetOutdir()+"pdf_bkg_continuum_1D_nb_prime_correlation.dat");
//        pdf_bkg_continuum_1D_nb_prime->ExportPDFValues(opt->GetOutdir()+"pdf_bkg_continuum_1D_nb_prime.dat",true);
        result_nb_prime->Print();
        delete result_nb_prime;
        
        PrintFitStartBlock("FITTING: 3D Fit");
        shared_ptr<FF_PDF> pdf_bkg_continuum_3D =
        make_shared<FF_PDF_bkg_continuum_3D_mbc_deltae_nb_prime>(pdf_bkg_continuum_1D_mbc,
                                                                 pdf_bkg_continuum_1D_deltae,
                                                                 pdf_bkg_continuum_1D_nb_prime, category_name_flags);
        RooFitResult *result_3D = pdf_bkg_continuum_3D->GetSimPDF().fitTo(data,	NumCPU(opt->GetNumcpu()),
                                                                          SumW2Error(true), Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()));
        pdf_bkg_continuum_3D->GetSimPDF().getParameters(data)->Print("v");
        for(RooRealVar *v : variables_3D) {
            std::string varname = v->GetName();
            FFUtil::VisualizeFitResult(pdf_bkg_continuum_3D,data,*v,category_name_flags,false,
                                       opt->GetBins(),"full_range", opt->GetOutdir()+"CONT_MC_3D_"+varname+"_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                       opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
        }
        WriteCorrelationMatrixFile(result_3D,opt->GetOutdir()+"pdf_bkg_continuum_3D_correlation.dat");
        pdf_bkg_continuum_3D->ExportPDFValues(opt->GetOutdir()+"pdf_bkg_continuum_3D.dat",true);
        result_3D->Print();
        delete result_3D;
        /*/
        std::cout<<"ZZA resonant "<<data_charged->sumEntries("((chil0_mo==chil1_mo && (abs(chil1_mo)==421 || abs(chil1_mo)==411 || abs(chil1_mo)==313 || abs(chil1_mo)==113)) || (chil2_mo==chil1_mo && (abs(chil1_mo)==213 || abs(chil1_mo)==223)) || (chil2_mo==chil0_mo && (abs(chil0_mo)==323 || abs(chil0_mo)==213)))")<<std::endl;
        std::cout<<"ZZA non-resonant "<<data_charged->sumEntries("!((chil0_mo==chil1_mo && (abs(chil1_mo)==421 || abs(chil1_mo)==411 || abs(chil1_mo)==313 || abs(chil1_mo)==113)) || (chil2_mo==chil1_mo && (abs(chil1_mo)==213 || abs(chil1_mo)==223)) || (chil2_mo==chil0_mo && (abs(chil0_mo)==323 || abs(chil0_mo)==213)))")<<std::endl;
        
        /*
        PrintFitStartBlock("FITTING: 2D Dalitz Fit");
        
        if (false) { //make background histogram pdf
            TH2F *data_hist = dynamic_cast<TH2F*>(data_charged->createHistogram(mass_AB, mass_BC,400,400,"!((chil0_mo==chil1_mo && (abs(chil1_mo)==421 || abs(chil1_mo)==411 || abs(chil1_mo)==313 || abs(chil1_mo)==113)) || (chil2_mo==chil1_mo && (abs(chil1_mo)==213 || abs(chil1_mo)==223)) || (chil2_mo==chil0_mo && (abs(chil0_mo)==323 || abs(chil0_mo)==213)))","continuum"));
            TFile *file = new TFile("plots/continuum_background.root", "RECREATE");
            file->cd();
            data_hist->Write();
            file->Close();
            delete file;
        }
        
        
        shared_ptr<FF_PDF> pdf_bkg_continuum_2D_dalitz = make_shared<FF_PDF_bkg_continuum_2D_dalitz>(mass_AB, mass_BC, category_name_flags);
        RooFitResult *result_2D_dalitz = pdf_bkg_continuum_2D_dalitz->GetSimPDF().fitTo(data, NumCPU(opt->GetNumcpu()),
                                                                                        SumW2Error(true), Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()));
        pdf_bkg_continuum_2D_dalitz->GetSimPDF().getParameters(data)->Print("v");
            RooAbsReal::defaultIntegratorConfig()->method1D().setLabel("RooAdaptiveGaussKronrodIntegrator1D");//RooMCIntegrator
        for(RooRealVar *v : variables_2D) {
            std::string varname = v->GetName();
            if (varname == "mass_01") {
            FFUtil::VisualizeFitResult(pdf_bkg_continuum_2D_dalitz,data,*v,category_name_flags,false,
                                       opt->GetBins(),"full_range1", opt->GetOutdir()+"CONT_DALITZ_2D_"+varname+"_projection_range1."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                       opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
            FFUtil::VisualizeFitResult(pdf_bkg_continuum_2D_dalitz,data,*v,category_name_flags,false,
                                       opt->GetBins(),"full_range2", opt->GetOutdir()+"CONT_DALITZ_2D_"+varname+"_projection_range2."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                       opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
            FFUtil::VisualizeFitResult(pdf_bkg_continuum_2D_dalitz,data,*v,category_name_flags,false,
                                       opt->GetBins(),"full_range3", opt->GetOutdir()+"CONT_DALITZ_2D_"+varname+"_projection_range3."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                       opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
            }
            
            if (varname == "mass_12") {
                FFUtil::VisualizeFitResult(pdf_bkg_continuum_2D_dalitz,data,*v,category_name_flags,false,
                                           opt->GetBins(),"full_range4", opt->GetOutdir()+"CONT_DALITZ_2D_"+varname+"_projection_range4."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                           opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                FFUtil::VisualizeFitResult(pdf_bkg_continuum_2D_dalitz,data,*v,category_name_flags,false,
                                           opt->GetBins(),"full_range5", opt->GetOutdir()+"CONT_DALITZ_2D_"+varname+"_projection_range5."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                           opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                FFUtil::VisualizeFitResult(pdf_bkg_continuum_2D_dalitz,data,*v,category_name_flags,false,
                                           opt->GetBins(),"full_range6", opt->GetOutdir()+"CONT_DALITZ_2D_"+varname+"_projection_range6."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                           opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
            }
            
        }
        WriteCorrelationMatrixFile(result_2D_dalitz,opt->GetOutdir()+"pdf_bkg_continuum_2D_dalitz_correlation.dat");
        pdf_bkg_continuum_2D_dalitz->ExportPDFValues(opt->GetOutdir()+"pdf_bkg_continuum_2D_dalitz.dat",true);
        result_2D_dalitz->Print();
        delete result_2D_dalitz;
        
        /*
        
        
        PrintFitStartBlock("FITTING: 6D Fit");
        
        shared_ptr<FF_PDF> pdf_bkg_continuum_6D = make_shared<FF_PDF_bkg_continuum_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor>(mbc,deltae,nb_prime,mass_AB,mass_BC,flavor,category_name_flags);
        RooFitResult *result_6D = pdf_bkg_continuum_6D->GetSimPDF().fitTo(data, NumCPU(opt->GetNumcpu()), Timer(true), Save(true), InitialHesse(false),
                                                                          Minos(opt->GetMinos()), SumW2Error(true));
        pdf_bkg_continuum_6D->GetSimPDF().getParameters(data)->Print("v");
        for(RooRealVar *v : variables_5D) {
            std::string varname = v->GetName();
            if (varname == "mbcp" || varname == "deltae" || varname == "nb_prime") {
                FFUtil::VisualizeFitResult(pdf_bkg_continuum_6D,data,*v,category_name_flags,false,
                                           opt->GetBins(),"full_range", opt->GetOutdir()+"CONT_MC_6D_"+varname+"_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                           opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
            }
            if (varname == "mass_01" || varname == "mass_12") {
                FFUtil::VisualizeFitResult(pdf_bkg_continuum_6D,data,*v,category_name_flags,false,
                                           opt->GetBins(),"full_range1", opt->GetOutdir()+"CONT_MC_6D_"+varname+"_projection_range1."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                           opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                FFUtil::VisualizeFitResult(pdf_bkg_continuum_6D,data,*v,category_name_flags,false,
                                           opt->GetBins(),"full_range2", opt->GetOutdir()+"CONT_MC_6D_"+varname+"_projection_range_2."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                           opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                FFUtil::VisualizeFitResult(pdf_bkg_continuum_6D,data,*v,category_name_flags,false,
                                           opt->GetBins(),"full_range3", opt->GetOutdir()+"CONT_MC_6D_"+varname+"_projection_range3."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                           opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                
            }
        }
        WriteCorrelationMatrixFile(result_6D,opt->GetOutdir()+"pdf_bkg_continuum_6D_correlation.dat");
        pdf_bkg_continuum_6D->ExportPDFValues(opt->GetOutdir()+"pdf_bkg_continuum_6D.dat",true);
        result_6D->Print();
        delete result_6D;
        
        
        /*
        
        const unsigned int nbins = 9;
        double bin_limits[nbins+1];
        
        //set the bins ranges gor the selected number of bins
        bin_limits[0]=(m_Pi0+m_Pi)*(m_Pi0+m_Pi);
        bin_limits[nbins]=(m_B0-m_K)*(m_B0-m_K);
        double binsize = (bin_limits[nbins]-bin_limits[0])/(double)nbins;
        for (unsigned int i=1; i<nbins; i++) {
            bin_limits[i] = bin_limits[0]+i*binsize;
        }
        
        for(unsigned int bin = 1; bin <= nbins; ++bin) {
            std::string binned_var_name = mass_AB.GetName();
            
            double min = bin_limits[bin-1];
            double max = bin_limits[bin];
            PrintFitStartBlock("Projecting: "+binned_var_name+" bin "+mp::to_string(bin)+" from ["+mp::to_string(min)+","+mp::to_string(max)+"] of 2D Dalitz PDF");
            
            // buffer range and reset full_range to make proper projections
            std::pair<Double_t,Double_t> range_buffer = std::make_pair(mass_AB.getMin("PHSP"),mass_AB.getMax("PHSP"));
            mass_AB.setRange("PHSP",min,max);
            
            
            std::string varname = mass_BC.GetName();
            FFUtil::VisualizeFitResult(pdf_bkg_continuum_6D,data,mass_BC,category_name_flags,false,
                                       opt->GetBins(),"PHSP", opt->GetOutdir()+"CONT_MC_6D_"+varname+"_projection_"+binned_var_name+"_bin_"+mp::to_string(bin)+"."+opt->GetPlotFormat(),
                                       opt->GetPlotFormat(),opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
            // reset full_range
            mass_AB.setRange("PHSP",range_buffer.first,range_buffer.second);
        }
        
        //set the bins ranges gor the selected number of bins
        bin_limits[0]=(m_K+m_Pi)*(m_K+m_Pi);
        bin_limits[nbins]=(m_B0-m_Pi0)*(m_B0-m_Pi0);
        binsize = (bin_limits[nbins]-bin_limits[0])/(double)nbins;
        for (unsigned int i=1; i<nbins; i++) {
            bin_limits[i] = bin_limits[0]+i*binsize;
        }
        
        for(unsigned int bin = 1; bin <= nbins; ++bin) {
            std::string binned_var_name = mass_BC.GetName();
            
            double min = bin_limits[bin-1];
            double max = bin_limits[bin];
            PrintFitStartBlock("Projecting: "+binned_var_name+" bin "+mp::to_string(bin)+" from ["+mp::to_string(min)+","+mp::to_string(max)+"] of 2D Dalitz PDF");
            
            // buffer range and reset full_range to make proper projections
            std::pair<Double_t,Double_t> range_buffer = std::make_pair(mass_BC.getMin("PHSP"),mass_BC.getMax("PHSP"));
            mass_BC.setRange("PHSP",min,max);
            
            
            std::string varname = mass_AB.GetName();
            FFUtil::VisualizeFitResult(pdf_bkg_continuum_6D,data,mass_AB,category_name_flags,false,
                                       opt->GetBins(),"PHSP", opt->GetOutdir()+"CONT_MC_6D_"+varname+"_projection_"+binned_var_name+"_bin_"+mp::to_string(bin)+"."+opt->GetPlotFormat(),
                                       opt->GetPlotFormat(),opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
            // reset full_range
            mass_BC.setRange("PHSP",range_buffer.first,range_buffer.second);
        }
        
        
        */
        
        //   delete data_test;
        delete data_charged;
    }
    
    void FitContinuumBackgroundCorrelations(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt) {
        if(chain->GetEntries() == 0) {
            std::cout << "No data for continuum background correlation fits given, skip fitting." << std::endl;
            return;
        }
        
        // ---------------------------------------------
        // ----- Define variables
        // ---------------------------------------------
        PrintFitStartBlock("STATUS: Define functions and variables...");
        RooRealVar mbc("mbcp","m'_{bc}",5.25, 5.288,"GeV");
        RooRealVar deltae("deltae","#Delta E",-0.2,0.2,"GeV");
        //    RooRealVar mbcp("mbcp","m'_{bc}",5.25,5.29,"GeV");
        RooRealVar mass_BC("mass_01","M^{2}_{K#pi}",0,27,"GeV^{2}/c^{4}");
        RooRealVar mass_AB("mass_12","M^{2}_{#pi#pi^{0}}",0,27,"GeV^{2}/c^{4}");
        RooRealVar mass_CA("mass_02","M^{2}_{K#pi^0}",0,27,"GeV^{2}");
        RooRealVar nb_prime("nb_prime","NB'",-10.0,10.0,"");
        RooCategory channel("channel","decay channel");
        channel.defineType("C",0);
        channel.defineType("N",1);
        RooCategory flavor("flavor","flavor type");
        flavor.defineType("CP",0);
        flavor.defineType("CA",1);
        RooCategory dalitz("dalitz","Inside Dalitz space");
        dalitz.defineType("IN",1);
        dalitz.defineType("OUT",-1);
        // event weight
        RooRealVar weight("weight","weight",-999999,999999);
        
        RooArgSet varset(mbc,deltae, mass_AB, mass_BC, mass_CA, nb_prime,channel);
        varset.add(flavor);
        varset.add(dalitz);
        varset.add(weight);
        
        TChain *data_chain = new TChain("h1");
        data_chain->Add("tuples/DATA_continuum.root");
        TChain *data_chain_friend = new TChain("friend");
        data_chain_friend->Add("tuples/DATA_continuum.root");
        data_chain->AddFriend(data_chain_friend);
        
        RooDataSet real_data("real_data","real_data",varset,Import(*data_chain),Cut("mbcp < 5.288 && dalitz==dalitz::IN"),WeightVar(weight));
        
        TChain *mc_chain = new TChain("h1");
        mc_chain->Add("/dev/shm/workaround-14817517140116359287.root");
        TChain *mc_chain_friend = new TChain("friend");
        mc_chain_friend->Add("/dev/shm/workaround-14817517140116359287.root");
        mc_chain->AddFriend(mc_chain_friend);
        
        RooDataSet mc_data("mc_data","mc_data",varset,Import(*mc_chain),Cut("mbcp < 5.288 && dalitz==dalitz::IN"),WeightVar(weight));
        
        
        TH2F *data_hist = dynamic_cast<TH2F*>(real_data.createHistogram(mass_AB, mass_BC,50,50,"","data_hist"));
        TH2F *mc_hist = dynamic_cast<TH2F*>(mc_data.createHistogram(mass_AB, mass_BC,50,50,"","mc_hist"));
        
        data_hist->Scale(1.0/data_hist->Integral(),"");
        mc_hist->Scale(1.0/mc_hist->Integral(),"");
        
        data_hist->Divide(mc_hist);
        /*
        
        TFile *file= new TFile("plots/continuum_background.root","READ");
        TH2F *continuum_pdf = dynamic_cast<TH2F*>(file->Get("data_data_hist_00000000"));
        
        TH2F *continuum_pdf_weighted = new TH2F("continuum_pdf_weighted","continuum_pdf_weighted",500,0,27,500,0,27);
        
        for (int i=1; i<500; i++) {
            for (int j = 1; j<500; j++) {
                double x = 27.0/500.0/2.0 + (i-1)*27.0/500.0;
                double y = 27.0/500.0/2.0 + (j-1)*27.0/500.0;
                double corr = mc_hist->GetBinContent(mc_hist->FindBin(x, y));
                if (corr<0.01) {
                    corr=1;
                }
                double value = continuum_pdf->GetBinContent(continuum_pdf->FindBin(x, y));
                
                continuum_pdf_weighted->Fill(x,y,corr*value);
            }
        }
        TFile *file2 = new TFile("plots/continuum_background_weighted.root", "RECREATE");
        file2->cd();
        continuum_pdf_weighted->Write();
        file2->Close();
        */
        SetRootStyleLarge2D();
        TCanvas* canvas = new TCanvas("c_canvas","c_canvas",2*1280,2*1024);
        SetRootStyleLarge2D();
        data_hist->SetMinimum(0);
        data_hist->SetMaximum(3);
        data_hist->GetXaxis()->SetTitle("m^{2}_{#pi#pi^{0}} (GeV^{2}/c^{4})");
        data_hist->GetYaxis()->SetTitle("m^{2}_{K#pi^{0}} (GeV^{2}/c^{4})");
        data_hist->GetXaxis()->CenterTitle();
        data_hist->GetYaxis()->CenterTitle();
        data_hist->GetZaxis()->SetTitle("");
        data_hist->Draw("COLZ");
        canvas->Print("test.pdf");
        SetRootStyleLarge();
        delete canvas;
        //Save the PDF to file
        /*
         TFile *file = new TFile("plots/continuum_background.root", "RECREATE");
         file->cd();
         data_hist->Write();
         file->Close();
         //*/
	}
}
