
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
    
    void FitBBbarMC(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt) {
        if(chain->GetEntries() == 0) {
            std::cout << "No data for BBbar fits given, skip fitting." << std::endl;
            return;
        }
        
        const unsigned int COMBINATORIC = 0;
        const unsigned int WRONGMASSHYP = 1;
        const unsigned int WRONGPI0HYP = 2;
        const unsigned int MISSINGFSP = 3;
        const unsigned int NONRES = 4;
        const unsigned int COMBINED = 5;
        
        unsigned int number_of_entries[COMBINED+1];
        
        typedef std::tuple<unsigned int, std::string, std::string> tuple_count_cut_label;
        std::array<tuple_count_cut_label, 6> bbar_bkg_types;
        bbar_bkg_types[0] = std::make_tuple(0,"combinatoric_"," && (((mctype==1001 || mctype==1002) || ((mctype<1001 || mctype>1004) && runno==0 && mcinfo!=59 && mcinfo!=61)) && mchash==1)");
        bbar_bkg_types[1] = std::make_tuple(1,"wrongmasshyp_"," && (((mctype==1001 || mctype==1002) || ((mctype<1001 || mctype>1004) && runno==0 && mcinfo!=59 && mcinfo!=61)) && mchash!=1 && ((mcinfo & 0x8) ==0) && (((mcinfo & 0x4) !=0) || ((mcinfo & 0x2) !=0)) && mcinfo<64)");
        bbar_bkg_types[2] = std::make_tuple(2,"wrongpi0hyp_"," && (((mctype==1001 || mctype==1002) || ((mctype<1001 || mctype>1004) && runno==0 && mcinfo!=59 && mcinfo!=61)) && mchash!=1 && mcinfo>64)");
        bbar_bkg_types[3] = std::make_tuple(3,"missingfsp_"," && (((mctype==1001 || mctype==1002) || ((mctype<1001 || mctype>1004) && runno==0 && mcinfo!=59 && mcinfo!=61)) && mchash!=1 && !(((mcinfo & 0x8) ==0) && (((mcinfo & 0x4) !=0) || ((mcinfo & 0x2) !=0))) && mcinfo<64  && !(mchash==141183 ||mchash==2513873 ||mchash==2245944 ||mchash==2323258 ||mchash==2068777 ||mchash==1769521 ||mchash==1639018 ||mchash==3905351 ||mchash==2939915 ||mchash==1962594 ||mchash==3500850 ||mchash==1881338))");
        bbar_bkg_types[4] = std::make_tuple(4,"nonres_"," && (((mctype==1001 || mctype==1002) || ((mctype<1001 || mctype>1004) && runno==0)) && (mcinfo==59 || mcinfo==61) && (mchash==141183 ||mchash==2513873 ||mchash==2245944 ||mchash==2323258 ||mchash==2068777 ||mchash==1769521 ||mchash==1639018 ||mchash==3905351 ||mchash==2939915 ||mchash==1962594 ||mchash==3500850 ||mchash==1881338))");
        bbar_bkg_types[5] = std::make_tuple(5,"","");
        
        
        for(const tuple_count_cut_label& bbar_bkg_type : bbar_bkg_types) {
            std::string bbar_type_label = std::get<1>(bbar_bkg_type);
            std::string bbar_type_cut = std::get<2>(bbar_bkg_type);
            bool is_combined_bbar_bkg = std::get<0>(bbar_bkg_type) == COMBINED ? true : false;
            // ---------------------------------------------
            // ----- Define variables
            // ---------------------------------------------
            PrintFitStartBlock("STATUS: Define functions and variables...");
            RooRealVar mbc("mbcp","M'_{bc}",5.25, 5.288,"GeV/c^{2}");
            RooRealVar deltae("deltae","#Delta E",-0.2,0.2,"GeV");
            //    RooRealVar mbcp("mbcp","m'_{bc}",5.25,5.29,"GeV");
            RooRealVar mass_BC("mass_01","m^{2}_{K#pi}",0,27,"GeV^{2}/c^{4}");
            RooRealVar mass_CA("mass_02","m^{2}_{K#pi^{0}}",0,27,"GeV^{2}/c^{4}");
            RooRealVar mass_AB("mass_12","m^{2}_{#pi#pi^{0}}",0,27,"GeV^{2}/c^{4}");
            RooRealVar nb_prime("nb_prime","C'_{NB}",-10.0,10.0,"");
            RooRealVar mchash("mchash","mchash'",0,1e+9,"");
            RooCategory channel("channel","decay channel");
            channel.defineType("C",0);
            channel.defineType("N",1);
            RooCategory flavor("flavor","flavor type");
            flavor.defineType("CP",0);
            flavor.defineType("CA",1);
            RooCategory dalitz("dalitz","Inside Dalitz space");
            dalitz.defineType("IN",1);
            dalitz.defineType("OUT",-1);
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
            varset.add(runno);
            varset.add(weight);
            varset.add(flavor);
            varset.add(dalitz);
            RooDataSet data("data","data",varset,Import(*chain.get()),Cut(("mbcp < 5.288 "+bbar_type_cut).c_str()),WeightVar(weight));
            
            number_of_entries[std::get<0>(bbar_bkg_type)]=(unsigned int)data.sumEntries();
            
            // create helper for the simultanous fit
            std::vector<std::string> flags;
            flags.push_back("C");
            tuple_category_name_flags category_name_flags = std::make_tuple(&channel,"channel",flags);
            // create data subsamples
            RooDataSet* data_charged = dynamic_cast<RooDataSet*>(data.reduce("channel==channel::C && dalitz==dalitz::IN"));
            data_charged->Print();
            number_of_entries[std::get<0>(bbar_bkg_type)]=(unsigned int)data_charged->sumEntries();
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
            
/*            mass_BC.setRange("full_range1",0, 27);
            mass_AB.setRange("full_range1",0, 3.2);
            mass_BC.setRange("full_range2",0, 27);
            mass_AB.setRange("full_range2",3.2, 3.6);
            mass_BC.setRange("full_range3",0, 27);
            mass_AB.setRange("full_range3",3.6, 27);*/
            
            mass_AB.setRange("full_range1",0, 27);
            mass_BC.setRange("full_range1",0, 3.3);
            mass_AB.setRange("full_range2",0, 27);
            mass_BC.setRange("full_range2",3.3, 3.7);
            mass_AB.setRange("full_range3",0, 27);
            mass_BC.setRange("full_range3",4.2, 27);
            
            mass_AB.setRange("full_range4",0, 2);
            mass_BC.setRange("full_range4",0, 27);
            mass_AB.setRange("full_range5",2, 12);
            mass_BC.setRange("full_range5",0, 27);
            mass_AB.setRange("full_range6",12, 27);
            mass_BC.setRange("full_range6",0, 27);
            
            mbc.setRange("mbc_sideband",5.25,5.27);
            deltae.setRange("mbc_sideband",-0.2,0.2);
            nb_prime.setRange("mbc_sideband",-10,10);
            mbc.setRange("mbc_signal_box",5.27,5.288);
            deltae.setRange("mbc_signal_box",-0.2,0.2);
            nb_prime.setRange("mbc_signal_box",-10,10);
            
            // ---------------------------------------------
            // ----- Check correlation between input variables
            // ---------------------------------------------
            if(opt->GetCreateCorrelationFiles()) {
                std::cout << "STATUS: Write correlation files..." << std::endl;
                WriteCorrelationMatrixFile(*data_charged,RooArgList(mbc,deltae,nb_prime),opt->GetOutdir()+"correlation_matrix_of_input_variables_"+bbar_type_label+"C.dat");
                WriteCorrelationStudyFile(*data_charged,RooArgList(mbc,deltae,nb_prime),opt->GetOutdir()+"fit_input_variables_"+bbar_type_label+"C.dat");
            }
            // ---------------------------------------------
            // ----- Create data component plots
            // ---------------------------------------------
            if(opt->GetCreateComponentPlots() && is_combined_bbar_bkg) {
                std::cout << "STATUS: Create data component plots..." << std::endl;
                for(RooRealVar* var : variables_3D) {
                    CreateDataComponentPlots(data,*var,opt->GetBins(),"full_range",opt->GetOutdir(),opt->GetPlotFormat(),"C","channel");
                }
                mp::execute_command("mkdir -p "+opt->GetOutdir()+"DATA_bkg_bbar && mv "+opt->GetOutdir()+"DATA_*."+opt->GetPlotFormat()+" "+opt->GetOutdir()+"DATA_bkg_bbar");
            }
            // ---------------------------------------------
            // ----- Do fits
            // ---------------------------------------------
            // See RooFit tutorial macro #501 about HowTo do a simultaneous fit
            // See RooFit tutorial macro #603 about HowTo do a multi-core parallelized unbinned maximum likelihood fit
            std::cout << "STATUS: Do fits..." << std::endl;
            /*
             PrintFitStartBlock("FITTING: mbc for "+bbar_type_label);
             shared_ptr<FF_PDF> pdf_bkg_bbar_1D_mbc;
             switch(std::get<0>(bbar_bkg_type)) {
             case COMBINATORIC:
             pdf_bkg_bbar_1D_mbc = make_shared<FF_PDF_bkg_bbar_combinatoric_1D_mbc>(mbc, category_name_flags);
             break;
             case WRONGMASSHYP:
             pdf_bkg_bbar_1D_mbc = make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_mbc>(mbc, category_name_flags);
             break;
             case WRONGPI0HYP:
             pdf_bkg_bbar_1D_mbc = make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_1D_mbc>(mbc, category_name_flags);
             break;
             case MISSINGFSP:
             pdf_bkg_bbar_1D_mbc = make_shared<FF_PDF_bkg_bbar_missingfsp_1D_mbc>(mbc, category_name_flags);
             break;
             case NONRES:
             pdf_bkg_bbar_1D_mbc = make_shared<FF_PDF_bkg_bbar_nonres_1D_mbc>(mbc, category_name_flags);
             break;
             case COMBINED:
             std::array<shared_ptr<FF_PDF>, 5> pdf_buffer_mbc;
             pdf_buffer_mbc[COMBINATORIC] = make_shared<FF_PDF_bkg_bbar_combinatoric_1D_mbc>(mbc, category_name_flags);
             pdf_buffer_mbc[WRONGMASSHYP] = make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_mbc>(mbc, category_name_flags);
             pdf_buffer_mbc[WRONGPI0HYP] = make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_1D_mbc>(mbc, category_name_flags);
             pdf_buffer_mbc[MISSINGFSP] = make_shared<FF_PDF_bkg_bbar_missingfsp_1D_mbc>(mbc, category_name_flags);
             pdf_buffer_mbc[NONRES] = make_shared<FF_PDF_bkg_bbar_nonres_1D_mbc>(mbc, category_name_flags);
             for(unsigned int i = 0; i < 5; ++i) {
             pdf_buffer_mbc[i]->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_"+std::get<1>(bbar_bkg_types[i])+"1D_mbc.dat",true);
             pdf_buffer_mbc[i]->SetVariablesConst();
             }
             pdf_bkg_bbar_1D_mbc = make_shared<FF_PDF_bkg_bbar_1D_mbc>(pdf_buffer_mbc[COMBINATORIC],pdf_buffer_mbc[WRONGMASSHYP],
             pdf_buffer_mbc[WRONGPI0HYP],pdf_buffer_mbc[MISSINGFSP],pdf_buffer_mbc[NONRES],
             data.sumEntries(), category_name_flags);
             break;
             }
             
             if(!is_combined_bbar_bkg) {
             RooFitResult *result_mbc = pdf_bkg_bbar_1D_mbc->GetSimPDF().fitTo(data, SumW2Error(pdf_bkg_bbar_1D_mbc->HasFreeParameter()), NumCPU(opt->GetNumcpu()),
             Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()), Extended(is_combined_bbar_bkg));
             pdf_bkg_bbar_1D_mbc->GetSimPDF().getParameters(data)->Print("v");
             FFUtil::VisualizeFitResult(pdf_bkg_bbar_1D_mbc,data,mbc,category_name_flags,is_combined_bbar_bkg,
             opt->GetBins(),"full_range", opt->GetOutdir()+"BB_MC_"+bbar_type_label+"1D_mbc_fit."+opt->GetPlotFormat(),opt->GetPlotFormat(),
             opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
             WriteCorrelationMatrixFile(result_mbc,opt->GetOutdir()+"pdf_bkg_bbar_"+bbar_type_label+"1D_mbcp_correlation.dat");
             pdf_bkg_bbar_1D_mbc->ExportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_"+bbar_type_label+"1D_mbcp.dat",true);
             result_mbc->Print();
             delete result_mbc;
             }
             
             // -----------------------------------------------------------------------------------------------------------------------
             
             PrintFitStartBlock("FITTING: deltae for "+bbar_type_label);
             shared_ptr<FF_PDF> pdf_bkg_bbar_1D_deltae;
             switch(std::get<0>(bbar_bkg_type)) {
             case COMBINATORIC:
             pdf_bkg_bbar_1D_deltae = make_shared<FF_PDF_bkg_bbar_combinatoric_1D_deltae>(deltae, category_name_flags);
             break;
             case WRONGMASSHYP:
             pdf_bkg_bbar_1D_deltae = make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_deltae>(deltae, category_name_flags);
             break;
             case WRONGPI0HYP:
             pdf_bkg_bbar_1D_deltae = make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_1D_deltae>(deltae, category_name_flags);
             break;
             case MISSINGFSP:
             pdf_bkg_bbar_1D_deltae = make_shared<FF_PDF_bkg_bbar_missingfsp_1D_deltae>(deltae, category_name_flags);
             break;
             case NONRES:
             pdf_bkg_bbar_1D_deltae = make_shared<FF_PDF_bkg_bbar_nonres_1D_deltae>(deltae, category_name_flags);
             break;
             case COMBINED:
             std::array<shared_ptr<FF_PDF>, 5> pdf_buffer_deltae;
             pdf_buffer_deltae[COMBINATORIC] = make_shared<FF_PDF_bkg_bbar_combinatoric_1D_deltae>(deltae, category_name_flags);
             pdf_buffer_deltae[WRONGMASSHYP] = make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_deltae>(deltae, category_name_flags);
             pdf_buffer_deltae[WRONGPI0HYP] = make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_1D_deltae>(deltae, category_name_flags);
             pdf_buffer_deltae[MISSINGFSP] = make_shared<FF_PDF_bkg_bbar_missingfsp_1D_deltae>(deltae, category_name_flags);
             pdf_buffer_deltae[NONRES] = make_shared<FF_PDF_bkg_bbar_nonres_1D_deltae>(deltae, category_name_flags);
             for(unsigned int i = 0; i < COMBINED; ++i) {
             pdf_buffer_deltae[i]->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_"+std::get<1>(bbar_bkg_types[i])+"1D_deltae.dat",true);
             pdf_buffer_deltae[i]->SetVariablesConst();
             }
             pdf_bkg_bbar_1D_deltae = make_shared<FF_PDF_bkg_bbar_1D_deltae>(pdf_buffer_deltae[COMBINATORIC],pdf_buffer_deltae[WRONGMASSHYP],
             pdf_buffer_deltae[MISSINGFSP],pdf_buffer_deltae[MISSINGFSP],pdf_buffer_deltae[NONRES],
             data.sumEntries(), category_name_flags);
             break;
             }
             
             if(!is_combined_bbar_bkg) {
             RooFitResult *result_deltae = pdf_bkg_bbar_1D_deltae->GetSimPDF().fitTo(data, SumW2Error(pdf_bkg_bbar_1D_deltae->HasFreeParameter()), NumCPU(opt->GetNumcpu()),
             Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()), Extended(is_combined_bbar_bkg));
             pdf_bkg_bbar_1D_deltae->GetSimPDF().getParameters(data)->Print("v");
             FFUtil::VisualizeFitResult(pdf_bkg_bbar_1D_deltae,data,deltae,category_name_flags,is_combined_bbar_bkg,
             opt->GetBins(),"full_range", opt->GetOutdir()+"BB_MC_"+bbar_type_label+"1D_deltae_fit."+opt->GetPlotFormat(),opt->GetPlotFormat(),
             opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
             WriteCorrelationMatrixFile(result_deltae,opt->GetOutdir()+"pdf_bkg_bbar_"+bbar_type_label+"1D_deltae_correlation.dat");
             pdf_bkg_bbar_1D_deltae->ExportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_"+bbar_type_label+"1D_deltae.dat",true);
             result_deltae->Print();
             delete result_deltae;
             }
             // -----------------------------------------------------------------------------------------------------------------------
             
             PrintFitStartBlock("FITTING: nb_prime for "+bbar_type_label);
             shared_ptr<FF_PDF> pdf_bkg_bbar_1D_nb_prime;
             switch(std::get<0>(bbar_bkg_type)) {
             case COMBINATORIC:
             pdf_bkg_bbar_1D_nb_prime = make_shared<FF_PDF_bkg_bbar_combinatoric_1D_nb_prime>(nb_prime, category_name_flags);
             break;
             case WRONGMASSHYP:
             pdf_bkg_bbar_1D_nb_prime = make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_nb_prime>(nb_prime, category_name_flags);
             break;
             case WRONGPI0HYP:
             pdf_bkg_bbar_1D_nb_prime = make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_1D_nb_prime>(nb_prime, category_name_flags);
             break;
             case MISSINGFSP:
             pdf_bkg_bbar_1D_nb_prime = make_shared<FF_PDF_bkg_bbar_missingfsp_1D_nb_prime>(nb_prime, category_name_flags);
             break;
             case NONRES:
             pdf_bkg_bbar_1D_nb_prime = make_shared<FF_PDF_bkg_bbar_nonres_1D_nb_prime>(nb_prime, category_name_flags);
             break;
             case COMBINED:
             std::array<shared_ptr<FF_PDF>, 5> pdf_buffer_nb_prime;
             pdf_buffer_nb_prime[COMBINATORIC] = make_shared<FF_PDF_bkg_bbar_combinatoric_1D_nb_prime>(boost::ref(nb_prime), category_name_flags);
             pdf_buffer_nb_prime[WRONGMASSHYP] = make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_nb_prime>(boost::ref(nb_prime), category_name_flags);
             pdf_buffer_nb_prime[WRONGPI0HYP] = make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_1D_nb_prime>(boost::ref(nb_prime), category_name_flags);
             pdf_buffer_nb_prime[MISSINGFSP] = make_shared<FF_PDF_bkg_bbar_missingfsp_1D_nb_prime>(boost::ref(nb_prime), category_name_flags);
             pdf_buffer_nb_prime[NONRES] = make_shared<FF_PDF_bkg_bbar_nonres_1D_nb_prime>(boost::ref(nb_prime), category_name_flags);
             for(unsigned int i = 0; i < COMBINED; ++i) {
             pdf_buffer_nb_prime[i]->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_"+std::get<1>(bbar_bkg_types[i])+"1D_nb_prime.dat",true);
             pdf_buffer_nb_prime[i]->SetVariablesConst();
             }
             pdf_bkg_bbar_1D_nb_prime = make_shared<FF_PDF_bkg_bbar_1D_nb_prime>(pdf_buffer_nb_prime[COMBINATORIC],
             pdf_buffer_nb_prime[WRONGMASSHYP],pdf_buffer_nb_prime[WRONGPI0HYP],pdf_buffer_nb_prime[MISSINGFSP],pdf_buffer_nb_prime[NONRES],
             data.sumEntries(), category_name_flags);
             break;
             }
             
             if(!is_combined_bbar_bkg) {
             RooFitResult *result_nb_prime = pdf_bkg_bbar_1D_nb_prime->GetSimPDF().fitTo(data, SumW2Error(pdf_bkg_bbar_1D_nb_prime->HasFreeParameter()), NumCPU(opt->GetNumcpu()),
             Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()), Extended(is_combined_bbar_bkg));
             pdf_bkg_bbar_1D_nb_prime->GetSimPDF().getParameters(data)->Print("v");
             FFUtil::VisualizeFitResult(pdf_bkg_bbar_1D_nb_prime,data,nb_prime,category_name_flags,is_combined_bbar_bkg,
             opt->GetBins(),"full_range", opt->GetOutdir()+"BB_MC_"+bbar_type_label+"1D_nb_prime_fit."+opt->GetPlotFormat(),opt->GetPlotFormat(),
             opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
             WriteCorrelationMatrixFile(result_nb_prime,opt->GetOutdir()+"pdf_bkg_bbar_"+bbar_type_label+"1D_nb_prime_correlation.dat");
             pdf_bkg_bbar_1D_nb_prime->ExportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_"+bbar_type_label+"1D_nb_prime.dat",true);
             result_nb_prime->Print();
             delete result_nb_prime;
             
             }
             // -----------------------------------------------------------------------------------------------------------------------
             /*
             if(is_combined_bbar_bkg) {
             PrintFitStartBlock("FITTING: 3D Fit");
             
             std::array<shared_ptr<FF_PDF>, 3> pdf_buffer_3D_combinatoric;
             for(unsigned int i = 0; i < variables_3D.size(); ++i) {
             if(i == 0) pdf_buffer_3D_combinatoric[i] = make_shared<FF_PDF_bkg_bbar_combinatoric_1D_mbc>(mbc, category_name_flags);
             if(i == 1) pdf_buffer_3D_combinatoric[i] = make_shared<FF_PDF_bkg_bbar_combinatoric_1D_deltae>(deltae, category_name_flags);
             if(i == 2) pdf_buffer_3D_combinatoric[i] = make_shared<FF_PDF_bkg_bbar_combinatoric_1D_nb_prime>(nb_prime, category_name_flags);
             std::string varname = variables_3D[i]->GetName();
             pdf_buffer_3D_combinatoric[i]->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_combinatoric_1D_"+varname+".dat",true);
             pdf_buffer_3D_combinatoric[i]->SetVariablesConst();
             }
             shared_ptr<FF_PDF> pdf_bkg_bbar_combinatoric_3D = make_shared<FF_PDF_bkg_bbar_combinatoric_3D_mbc_deltae_nb_prime>(pdf_buffer_3D_combinatoric[0], pdf_buffer_3D_combinatoric[1], pdf_buffer_3D_combinatoric[2], category_name_flags);
             pdf_bkg_bbar_combinatoric_3D->SetVariablesConst();
             pdf_bkg_bbar_combinatoric_3D->ExportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_combinatoric_3D.dat",true);
             
             std::array<shared_ptr<FF_PDF>, 3> pdf_buffer_3D_wrongmasshyp;
             for(unsigned int i = 0; i < variables_3D.size(); ++i) {
             if(i == 0) pdf_buffer_3D_wrongmasshyp[i] = make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_mbc>(mbc, category_name_flags);
             if(i == 1) pdf_buffer_3D_wrongmasshyp[i] = make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_deltae>(deltae, category_name_flags);
             if(i == 2) pdf_buffer_3D_wrongmasshyp[i] = make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_nb_prime>(nb_prime, category_name_flags);
             std::string varname = variables_3D[i]->GetName();
             pdf_buffer_3D_wrongmasshyp[i]->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongmasshyp_1D_"+varname+".dat",true);
             pdf_buffer_3D_wrongmasshyp[i]->SetVariablesConst();
             }
             shared_ptr<FF_PDF> pdf_bkg_bbar_wrongmasshyp_3D = make_shared<FF_PDF_bkg_bbar_wrongmasshyp_3D_mbc_deltae_nb_prime>(pdf_buffer_3D_wrongmasshyp[0], pdf_buffer_3D_wrongmasshyp[1], pdf_buffer_3D_wrongmasshyp[2], category_name_flags);
             pdf_bkg_bbar_wrongmasshyp_3D->SetVariablesConst();
             pdf_bkg_bbar_wrongmasshyp_3D->ExportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongmasshyp_3D.dat",true);
             
             std::array<shared_ptr<FF_PDF>, 3> pdf_buffer_3D_wrongpi0hyp;
             for(unsigned int i = 0; i < variables_3D.size(); ++i) {
             if(i == 0) pdf_buffer_3D_wrongpi0hyp[i] = make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_1D_mbc>(mbc, category_name_flags);
             if(i == 1) pdf_buffer_3D_wrongpi0hyp[i] = make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_1D_deltae>(deltae, category_name_flags);
             if(i == 2) pdf_buffer_3D_wrongpi0hyp[i] = make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_1D_nb_prime>(nb_prime, category_name_flags);
             std::string varname = variables_3D[i]->GetName();
             pdf_buffer_3D_wrongpi0hyp[i]->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongpi0hyp_1D_"+varname+".dat",true);
             pdf_buffer_3D_wrongpi0hyp[i]->SetVariablesConst();
             }
             shared_ptr<FF_PDF> pdf_bkg_bbar_wrongpi0hyp_3D = make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_3D_mbc_deltae_nb_prime>(pdf_buffer_3D_wrongpi0hyp[0], pdf_buffer_3D_wrongpi0hyp[1], pdf_buffer_3D_wrongpi0hyp[2], category_name_flags);
             pdf_bkg_bbar_wrongpi0hyp_3D->SetVariablesConst();
             pdf_bkg_bbar_wrongpi0hyp_3D->ExportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongpi0hyp_3D.dat",true);
             
             std::array<shared_ptr<FF_PDF>, 3> pdf_buffer_3D_missingfsp;
             for(unsigned int i = 0; i < variables_3D.size(); ++i) {
             if(i == 0) pdf_buffer_3D_missingfsp[i] = make_shared<FF_PDF_bkg_bbar_missingfsp_1D_mbc>(mbc, category_name_flags);
             if(i == 1) pdf_buffer_3D_missingfsp[i] = make_shared<FF_PDF_bkg_bbar_missingfsp_1D_deltae>(deltae, category_name_flags);
             if(i == 2) pdf_buffer_3D_missingfsp[i] = make_shared<FF_PDF_bkg_bbar_missingfsp_1D_nb_prime>(nb_prime, category_name_flags);
             std::string varname = variables_3D[i]->GetName();
             pdf_buffer_3D_missingfsp[i]->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_missingfsp_1D_"+varname+".dat",true);
             pdf_buffer_3D_missingfsp[i]->SetVariablesConst();
             }
             shared_ptr<FF_PDF> pdf_bkg_bbar_missingfsp_3D = make_shared<FF_PDF_bkg_bbar_missingfsp_3D_mbc_deltae_nb_prime>(pdf_buffer_3D_missingfsp[0], pdf_buffer_3D_missingfsp[1], pdf_buffer_3D_missingfsp[2], category_name_flags);
             pdf_bkg_bbar_missingfsp_3D->SetVariablesConst();
             pdf_bkg_bbar_missingfsp_3D->ExportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_missingfsp_3D.dat",true);
             
             std::array<shared_ptr<FF_PDF>, 3> pdf_buffer_3D_nonres;
             for(unsigned int i = 0; i < variables_3D.size(); ++i) {
             if(i == 0) pdf_buffer_3D_nonres[i] = make_shared<FF_PDF_bkg_bbar_nonres_1D_mbc>(mbc, category_name_flags);
             if(i == 1) pdf_buffer_3D_nonres[i] = make_shared<FF_PDF_bkg_bbar_nonres_1D_deltae>(deltae, category_name_flags);
             if(i == 2) pdf_buffer_3D_nonres[i] = make_shared<FF_PDF_bkg_bbar_nonres_1D_nb_prime>(nb_prime, category_name_flags);
             std::string varname = variables_3D[i]->GetName();
             pdf_buffer_3D_nonres[i]->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_nonres_1D_"+varname+".dat",true);
             pdf_buffer_3D_nonres[i]->SetVariablesConst();
             }
             shared_ptr<FF_PDF> pdf_bkg_bbar_nonres_3D = make_shared<FF_PDF_bkg_bbar_nonres_3D_mbc_deltae_nb_prime>(pdf_buffer_3D_nonres[0], pdf_buffer_3D_nonres[1], pdf_buffer_3D_nonres[2], category_name_flags);
             pdf_bkg_bbar_nonres_3D->SetVariablesConst();
             pdf_bkg_bbar_nonres_3D->ExportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_nonres_3D.dat",true);
             
             
             shared_ptr<FF_PDF> pdf_bkg_bbar_3D = make_shared<FF_PDF_bkg_bbar_3D_mbc_deltae_nb_prime>(pdf_bkg_bbar_combinatoric_3D,
             pdf_bkg_bbar_wrongmasshyp_3D, pdf_bkg_bbar_wrongpi0hyp_3D,
             pdf_bkg_bbar_missingfsp_3D, pdf_bkg_bbar_nonres_3D, data.sumEntries(), category_name_flags);
             
             
             pdf_bkg_bbar_3D->GetVariable("C_bkg_bbar_combinatoric_nevents")->setVal(number_of_entries[0]);
             //                pdf_bkg_bbar_3D->GetVariable("C_bkg_bbar_combinatoric_nevents")->setConstant(true);
             pdf_bkg_bbar_3D->GetVariable("C_bkg_bbar_wrongmasshyp_nevents")->setVal(number_of_entries[1]);
             pdf_bkg_bbar_3D->GetVariable("C_bkg_bbar_wrongmasshyp_nevents")->setConstant(true);
             pdf_bkg_bbar_3D->GetVariable("C_bkg_bbar_wrongpi0hyp_nevents")->setVal(number_of_entries[2]);
             pdf_bkg_bbar_3D->GetVariable("C_bkg_bbar_wrongpi0hyp_nevents")->setConstant(true);
             pdf_bkg_bbar_3D->GetVariable("C_bkg_bbar_missingfsp_nevents")->setVal(number_of_entries[3]);
             //                pdf_bkg_bbar_3D->GetVariable("C_bkg_bbar_missingfsp_nevents")->setConstant(true);
             pdf_bkg_bbar_3D->GetVariable("C_bkg_bbar_nonres_nevents")->setVal(number_of_entries[4]);
             //                pdf_bkg_bbar_3D->GetVariable("C_bkg_bbar_nonres_nevents")->setConstant(true);
             
             RooFitResult *result_3D = pdf_bkg_bbar_3D->GetSimPDF().fitTo(data, SumW2Error(true), NumCPU(opt->GetNumcpu()),
             Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()), Extended(is_combined_bbar_bkg));
             pdf_bkg_bbar_3D->GetSimPDF().getParameters(data)->Print("v");
             for(RooRealVar *v : variables_3D) {
             std::string varname = v->GetName();
             FFUtil::VisualizeFitResult(pdf_bkg_bbar_3D,data,*v,category_name_flags,true,
             opt->GetBins(),"full_range", opt->GetOutdir()+"BB_MC_3D_"+varname+"_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
             opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
             if(varname != "mbc") {
             FFUtil::VisualizeFitResult(pdf_bkg_bbar_3D,data,*v,category_name_flags,true,
             opt->GetBins(),"mbc_sideband", opt->GetOutdir()+"BB_MC_3D_"+varname+"_projection_mbc_sideband."+opt->GetPlotFormat(),opt->GetPlotFormat(),
             opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
             FFUtil::VisualizeFitResult(pdf_bkg_bbar_3D,data,*v,category_name_flags,true,
             opt->GetBins(),"mbc_signal_box", opt->GetOutdir()+"BB_MC_3D_"+varname+"_projection_mbc_signal_box."+opt->GetPlotFormat(),opt->GetPlotFormat(),
             opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
             }
             }
             WriteCorrelationMatrixFile(result_3D,opt->GetOutdir()+"pdf_bkg_bbar_3D_correlation.dat");
             pdf_bkg_bbar_3D->ExportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_3D.dat",true);
             result_3D->Print();
             delete result_3D;
             }*/
            
            PrintFitStartBlock("FITTING: 2D Dalitz "+bbar_type_label);
            
            if (false) { //Put true if you want to create background histograms
                if (std::get<0>(bbar_bkg_type)==COMBINATORIC) {
                    TH2F *data_hist = dynamic_cast<TH2F*>(data_charged->createHistogram(mass_AB, mass_BC,200,200,"!((chil0_mo==chil1_mo && (abs(chil1_mo)==30343 || abs(chil1_mo)==313 || abs(chil1_mo)==315 || (abs(chil1_mo)==421) && abs(mass_01-3.5)<0.1)) || (chil0_mo==chil2_mo && abs(chil2_mo)==213))",(bbar_type_label).c_str()));
                    TFile *file = new TFile("plots/bbbar_combinatoric_background.root", "RECREATE");
                    file->cd();
                    data_hist->Write();
                    file->Close();
                    delete file;
                }
                
                if (std::get<0>(bbar_bkg_type)==WRONGMASSHYP) {
                    TH2F *data_hist = dynamic_cast<TH2F*>(data_charged->createHistogram(mass_AB, mass_BC,200,200,"!((chil0_mo==chil1_mo && (abs(chil1_mo)==113 || abs(chil1_mo)==310 )) || (chil1_mo==chil2_mo && (abs(chil2_mo)==213 || abs(chil2_mo)==411)))",(bbar_type_label).c_str()));
                    TFile *file = new TFile("plots/bbbar_wrongmasshyp_background.root", "RECREATE");
                    file->cd();
                    data_hist->Write();
                    file->Close();
                    delete file;
                }
                
                if (std::get<0>(bbar_bkg_type)==WRONGPI0HYP) {
                    TH2F *data_hist = dynamic_cast<TH2F*>(data_charged->createHistogram(mass_AB, mass_BC,200,200,"!((chil0_mo==chil1_mo && (abs(chil1_mo)==30343 || abs(chil1_mo)==313 || abs(chil1_mo)==315 || abs(chil1_mo)==421)))",(bbar_type_label).c_str()));
                    TFile *file = new TFile("plots/bbbar_wrongpi0hyp_background.root", "RECREATE");
                    file->cd();
                    data_hist->Write();
                    file->Close();
                    delete file;
                }
                
                if (std::get<0>(bbar_bkg_type)==MISSINGFSP) {
                    TH2F *data_hist = dynamic_cast<TH2F*>(data_charged->createHistogram(mass_AB, mass_BC,200,200,"!((chil0_mo==chil1_mo && (abs(chil1_mo)==313 || abs(chil1_mo)==315 || abs(chil1_mo)==421 || abs(chil1_mo)==113)) || (chil0_mo==chil2_mo && (abs(chil2_mo)==213 || abs(chil2_mo)==323)) || (chil1_mo==chil2_mo && (abs(chil1_mo)==213 || abs(chil1_mo)==421)))",(bbar_type_label).c_str()));
                    TFile *file = new TFile("plots/bbbar_missingfsp_background.root", "RECREATE");
                    file->cd();
                    data_hist->Write();
                    file->Close();
                    delete file;
                }
            }
            
            if (true) { //Put true if you want to create background histograms
                if (std::get<0>(bbar_bkg_type)==COMBINATORIC) {
                    std::cout << "ZZA resonant " << data_charged->sumEntries("((chil0_mo==chil1_mo && (abs(chil1_mo)==30343 || abs(chil1_mo)==313 || abs(chil1_mo)==315 || (abs(chil1_mo)==421) && abs(mass_01-3.5)<0.1)) || (chil0_mo==chil2_mo && abs(chil2_mo)==213))")<<std::endl;
                    std::cout << "ZZA non-resonant " << data_charged->sumEntries("!((chil0_mo==chil1_mo && (abs(chil1_mo)==30343 || abs(chil1_mo)==313 || abs(chil1_mo)==315 || (abs(chil1_mo)==421) && abs(mass_01-3.5)<0.1)) || (chil0_mo==chil2_mo && abs(chil2_mo)==213))")<<std::endl;
                }
                
                if (std::get<0>(bbar_bkg_type)==WRONGMASSHYP) {
                    std::cout << "ZZA resonant " << data_charged->sumEntries("((chil0_mo==chil1_mo && (abs(chil1_mo)==113 || abs(chil1_mo)==310 )) || (chil1_mo==chil2_mo && (abs(chil2_mo)==213 || abs(chil2_mo)==411)))")<<std::endl;
                    std::cout << "ZZA non-resonant " << data_charged->sumEntries("!((chil0_mo==chil1_mo && (abs(chil1_mo)==113 || abs(chil1_mo)==310 )) || (chil1_mo==chil2_mo && (abs(chil2_mo)==213 || abs(chil2_mo)==411)))")<<std::endl;
                }
                
                if (std::get<0>(bbar_bkg_type)==WRONGPI0HYP) {
                    std::cout << "ZZA resonant " << data_charged->sumEntries("((chil0_mo==chil1_mo && (abs(chil1_mo)==30343 || abs(chil1_mo)==313 || abs(chil1_mo)==315 || abs(chil1_mo)==421)))")<<std::endl;
                    std::cout << "ZZA non-resonant " << data_charged->sumEntries("!((chil0_mo==chil1_mo && (abs(chil1_mo)==30343 || abs(chil1_mo)==313 || abs(chil1_mo)==315 || abs(chil1_mo)==421)))")<<std::endl;
                }
                
                if (std::get<0>(bbar_bkg_type)==MISSINGFSP) {
                    std::cout << "ZZA resonant " << data_charged->sumEntries("((chil0_mo==chil1_mo && (abs(chil1_mo)==313 || abs(chil1_mo)==315 || abs(chil1_mo)==421 || abs(chil1_mo)==113)) || (chil0_mo==chil2_mo && (abs(chil2_mo)==213 || abs(chil2_mo)==323)) || (chil1_mo==chil2_mo && (abs(chil1_mo)==213 || abs(chil1_mo)==421)))")<<std::endl;
                    std::cout << "ZZA non-resonant " << data_charged->sumEntries("!((chil0_mo==chil1_mo && (abs(chil1_mo)==313 || abs(chil1_mo)==315 || abs(chil1_mo)==421 || abs(chil1_mo)==113)) || (chil0_mo==chil2_mo && (abs(chil2_mo)==213 || abs(chil2_mo)==323)) || (chil1_mo==chil2_mo && (abs(chil1_mo)==213 || abs(chil1_mo)==421)))")<<std::endl;
                }
            }
            
            

 
            shared_ptr<FF_PDF> pdf_bkg_bbar_2D_dalitz;
            switch(std::get<0>(bbar_bkg_type)) {
             
                case COMBINATORIC:
                    pdf_bkg_bbar_2D_dalitz = make_shared<FF_PDF_bkg_bbar_combinatoric_2D_dalitz>(mass_AB, mass_BC, category_name_flags);
                    pdf_bkg_bbar_2D_dalitz->GetVariable("bkg_bbar_combinatoric_frac1")->setVal(data_charged->sumEntries("chil0_mo==chil1_mo && abs(chil1_mo)==421 && abs(mass_01-3.5)<0.1")/number_of_entries[0]);
                    pdf_bkg_bbar_2D_dalitz->GetVariable("bkg_bbar_combinatoric_frac1")->setConstant(true);
                    break;
                case WRONGMASSHYP:
                    pdf_bkg_bbar_2D_dalitz = make_shared<FF_PDF_bkg_bbar_wrongmasshyp_2D_dalitz>(mass_AB, mass_BC, category_name_flags);
                    pdf_bkg_bbar_2D_dalitz->GetVariable("bkg_bbar_wrongmasshyp_frac1")->setVal(data_charged->sumEntries("chil2_mo==chil1_mo && abs(chil1_mo)==411 && mass_12>3.3 && mass_12<3.5 ")/number_of_entries[1]*0.6);
                    pdf_bkg_bbar_2D_dalitz->GetVariable("bkg_bbar_wrongmasshyp_frac1")->setConstant(true);
                    break;
                case WRONGPI0HYP:
                    pdf_bkg_bbar_2D_dalitz = make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_2D_dalitz>(mass_AB, mass_BC, category_name_flags);
                    pdf_bkg_bbar_2D_dalitz->GetVariable("bkg_bbar_wrongpi0hyp_frac1")->setVal(data_charged->sumEntries("chil0_mo==chil1_mo && abs(chil1_mo)==421")/number_of_entries[2]);
                    pdf_bkg_bbar_2D_dalitz->GetVariable("bkg_bbar_wrongpi0hyp_frac1")->setConstant(true);
                    break;
                case MISSINGFSP:
                    pdf_bkg_bbar_2D_dalitz = make_shared<FF_PDF_bkg_bbar_missingfsp_2D_dalitz>(mass_AB, mass_BC, category_name_flags);
                    pdf_bkg_bbar_2D_dalitz->GetVariable("bkg_bbar_missingfsp_frac1")->setVal(data_charged->sumEntries("chil0_mo==chil1_mo && abs(chil1_mo)==421 && mass_01>3.3 && mass_01<3.7")/number_of_entries[3]);
//                    pdf_bkg_bbar_2D_dalitz->GetVariable("bkg_bbar_missingfsp_frac1")->setConstant(true);
                    break;
                case NONRES:
                    pdf_bkg_bbar_2D_dalitz = make_shared<FF_PDF_bkg_bbar_nonres_2D_dalitz>(mass_AB, mass_BC, category_name_flags);
                    pdf_bkg_bbar_2D_dalitz->GetVariable("bkg_bbar_nonres_frac1")->setVal(data_charged->sumEntries("chil0_mo==chil1_mo && abs(chil1_mo)==421")/number_of_entries[4]);
                    pdf_bkg_bbar_2D_dalitz->GetVariable("bkg_bbar_nonres_frac1")->setConstant(true);
                    break;
             
                case COMBINED:
                    std::array<shared_ptr<FF_PDF>, 5> pdf_buffer_2D_dalitz;
                    
                    pdf_buffer_2D_dalitz[COMBINATORIC] = make_shared<FF_PDF_bkg_bbar_combinatoric_2D_dalitz>(mass_AB, mass_BC, category_name_flags);
                    pdf_buffer_2D_dalitz[WRONGMASSHYP] = make_shared<FF_PDF_bkg_bbar_wrongmasshyp_2D_dalitz>(mass_AB, mass_BC, category_name_flags);
                    pdf_buffer_2D_dalitz[WRONGPI0HYP] = make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_2D_dalitz>(mass_AB, mass_BC, category_name_flags);
                    pdf_buffer_2D_dalitz[MISSINGFSP] = make_shared<FF_PDF_bkg_bbar_missingfsp_2D_dalitz>(mass_AB, mass_BC, category_name_flags);
                    pdf_buffer_2D_dalitz[NONRES] = make_shared<FF_PDF_bkg_bbar_nonres_2D_dalitz>(mass_AB, mass_BC, category_name_flags);
                    for(unsigned int i = 0; i < COMBINED; ++i) {
                        pdf_buffer_2D_dalitz[i]->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_"+std::get<1>(bbar_bkg_types[i])+"2D_dalitz.dat",true);
                        pdf_buffer_2D_dalitz[i]->SetVariablesConst();
                    }
                    pdf_bkg_bbar_2D_dalitz = make_shared<FF_PDF_bkg_bbar_2D_dalitz>(pdf_buffer_2D_dalitz[COMBINATORIC], pdf_buffer_2D_dalitz[WRONGMASSHYP],pdf_buffer_2D_dalitz[WRONGPI0HYP],pdf_buffer_2D_dalitz[MISSINGFSP],pdf_buffer_2D_dalitz[NONRES],data.sumEntries(), category_name_flags);
                    
                    pdf_bkg_bbar_2D_dalitz->GetVariable("C_bkg_bbar_combinatoric_nevents")->setVal(number_of_entries[0]);
                    //                pdf_bkg_bbar_3D->GetVariable("C_bkg_bbar_combinatoric_nevents")->setConstant(true);
                    pdf_bkg_bbar_2D_dalitz->GetVariable("C_bkg_bbar_wrongmasshyp_nevents")->setVal(number_of_entries[1]);
                    pdf_bkg_bbar_2D_dalitz->GetVariable("C_bkg_bbar_wrongmasshyp_nevents")->setConstant(true);
                    pdf_bkg_bbar_2D_dalitz->GetVariable("C_bkg_bbar_wrongpi0hyp_nevents")->setVal(number_of_entries[2]);
                    pdf_bkg_bbar_2D_dalitz->GetVariable("C_bkg_bbar_wrongpi0hyp_nevents")->setConstant(true);
                    pdf_bkg_bbar_2D_dalitz->GetVariable("C_bkg_bbar_missingfsp_nevents")->setVal(number_of_entries[3]);
                    //                pdf_bkg_bbar_3D->GetVariable("C_bkg_bbar_missingfsp_nevents")->setConstant(true);
                    pdf_bkg_bbar_2D_dalitz->GetVariable("C_bkg_bbar_nonres_nevents")->setVal(number_of_entries[4]);
                    //                pdf_bkg_bbar_3D->GetVariable("C_bkg_bbar_nonres_nevents")->setConstant(true);

                    
                    break;
            }
            
            if(std::get<0>(bbar_bkg_type)==MISSINGFSP) {
                RooFitResult *result_2D_dalitz = pdf_bkg_bbar_2D_dalitz->GetSimPDF().fitTo(*data_charged, SumW2Error(pdf_bkg_bbar_2D_dalitz->HasFreeParameter()), NumCPU(opt->GetNumcpu()),
                                                                                          Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()), Extended(is_combined_bbar_bkg));
                pdf_bkg_bbar_2D_dalitz->GetSimPDF().getParameters(*data_charged)->Print("v");
                for(RooRealVar *v : variables_2D) {
                    std::string varname = v->GetName();
                   // if (varname == "mass_01") {
                    FFUtil::VisualizeFitResult(pdf_bkg_bbar_2D_dalitz,*data_charged,*v,category_name_flags,is_combined_bbar_bkg,
                                               opt->GetBins(),"full_range1", opt->GetOutdir()+"BB_MC_"+bbar_type_label+"2D_dalitz_fit_"+varname+"_projection_region1."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                               opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                   /*FFUtil::VisualizeFitResult(pdf_bkg_bbar_2D_dalitz,*data_charged,*v,category_name_flags,is_combined_bbar_bkg,
                                               opt->GetBins(),"full_range2", opt->GetOutdir()+"BB_MC_"+bbar_type_label+"2D_dalitz_fit_"+varname+"_projection_region2."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                               opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                    FFUtil::VisualizeFitResult(pdf_bkg_bbar_2D_dalitz,*data_charged,*v,category_name_flags,is_combined_bbar_bkg,
                                               opt->GetBins(),"full_range3", opt->GetOutdir()+"BB_MC_"+bbar_type_label+"2D_dalitz_fit_"+varname+"_projection_region3."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                               opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                    //}
                    /*if (varname == "mass_12") {
                        FFUtil::VisualizeFitResult(pdf_bkg_bbar_2D_dalitz,*data_charged,*v,category_name_flags,is_combined_bbar_bkg,
                                                   opt->GetBins(),"full_range", opt->GetOutdir()+"BB_MC_"+bbar_type_label+"2D_dalitz_fit_"+varname+"_projection_region4."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                                   opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                     /*   FFUtil::VisualizeFitResult(pdf_bkg_bbar_2D_dalitz,*data_charged,*v,category_name_flags,is_combined_bbar_bkg,
                                                   opt->GetBins(),"full_range5", opt->GetOutdir()+"BB_MC_"+bbar_type_label+"2D_dalitz_fit_"+varname+"_projection_region5."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                                   opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                        FFUtil::VisualizeFitResult(pdf_bkg_bbar_2D_dalitz,*data_charged,*v,category_name_flags,is_combined_bbar_bkg,
                                                   opt->GetBins(),"full_range6", opt->GetOutdir()+"BB_MC_"+bbar_type_label+"2D_dalitz_fit_"+varname+"_projection_region6."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                                   opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                    }*/
                    
                }
                WriteCorrelationMatrixFile(result_2D_dalitz,opt->GetOutdir()+"pdf_bkg_bbar_"+bbar_type_label+"2D_dalitz_correlation.dat");
                pdf_bkg_bbar_2D_dalitz->ExportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_"+bbar_type_label+"2D_dalitz.dat",true);
                result_2D_dalitz->Print();
                delete result_2D_dalitz;
                
            }
            
            /*
            if(is_combined_bbar_bkg) {
                PrintFitStartBlock("FITTING: 5D Fit");
                
                std::array<shared_ptr<FF_PDF>, 4> pdf_buffer_5D_combinatoric;
                for(unsigned int i = 0; i < variables_5D.size()-1; ++i) {
                    if(i == 0) pdf_buffer_5D_combinatoric[i] = make_shared<FF_PDF_bkg_bbar_combinatoric_1D_mbc>(mbc, category_name_flags);
                    if(i == 1) pdf_buffer_5D_combinatoric[i] = make_shared<FF_PDF_bkg_bbar_combinatoric_1D_deltae>(deltae, category_name_flags);
                    if(i == 2) pdf_buffer_5D_combinatoric[i] = make_shared<FF_PDF_bkg_bbar_combinatoric_1D_nb_prime>(nb_prime, category_name_flags);
                    if(i == 3) pdf_buffer_5D_combinatoric[i] = make_shared<FF_PDF_bkg_bbar_combinatoric_2D_dalitz>(mass_AB, mass_BC, category_name_flags);
                    if (i!=3) {
                        std::string varname = variables_5D[i]->GetName();
                        pdf_buffer_5D_combinatoric[i]->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_combinatoric_1D_"+varname+".dat",true);
                    }else{
                        pdf_buffer_5D_combinatoric[i]->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_combinatoric_2D_dalitz.dat",true);
                    }
                    
                    pdf_buffer_5D_combinatoric[i]->SetVariablesConst();
                }
                shared_ptr<FF_PDF> pdf_bkg_bbar_combinatoric_5D = make_shared<FF_PDF_bkg_bbar_combinatoric_5D_mbc_deltae_nb_prime_dalitz>(pdf_buffer_5D_combinatoric[0], pdf_buffer_5D_combinatoric[1], pdf_buffer_5D_combinatoric[2], pdf_buffer_5D_combinatoric[3], category_name_flags);
                pdf_bkg_bbar_combinatoric_5D->SetVariablesConst();
                pdf_bkg_bbar_combinatoric_5D->ExportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_combinatoric_5D.dat",true);
                
                std::array<shared_ptr<FF_PDF>, 4> pdf_buffer_5D_wrongmasshyp;
                for(unsigned int i = 0; i < variables_5D.size() -1; ++i) {
                    if(i == 0) pdf_buffer_5D_wrongmasshyp[i] = make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_mbc>(mbc, category_name_flags);
                    if(i == 1) pdf_buffer_5D_wrongmasshyp[i] = make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_deltae>(deltae, category_name_flags);
                    if(i == 2) pdf_buffer_5D_wrongmasshyp[i] = make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_nb_prime>(nb_prime, category_name_flags);
                    if(i == 3) pdf_buffer_5D_wrongmasshyp[i] = make_shared<FF_PDF_bkg_bbar_wrongmasshyp_2D_dalitz>(mass_AB, mass_BC, category_name_flags);
                    if (i!=3) {
                        std::string varname = variables_5D[i]->GetName();
                        pdf_buffer_5D_wrongmasshyp[i]->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongmasshyp_1D_"+varname+".dat",true);
                    }else{
                        pdf_buffer_5D_wrongmasshyp[i]->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongmasshyp_2D_dalitz.dat",true);
                    }
                    pdf_buffer_5D_wrongmasshyp[i]->SetVariablesConst();
                }
                shared_ptr<FF_PDF> pdf_bkg_bbar_wrongmasshyp_5D = make_shared<FF_PDF_bkg_bbar_wrongmasshyp_5D_mbc_deltae_nb_prime_dalitz>(pdf_buffer_5D_wrongmasshyp[0], pdf_buffer_5D_wrongmasshyp[1], pdf_buffer_5D_wrongmasshyp[2], pdf_buffer_5D_wrongmasshyp[3], category_name_flags);
                pdf_bkg_bbar_wrongmasshyp_5D->SetVariablesConst();
                pdf_bkg_bbar_wrongmasshyp_5D->ExportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongmasshyp_5D.dat",true);
                
                std::array<shared_ptr<FF_PDF>, 4> pdf_buffer_5D_wrongpi0hyp;
                for(unsigned int i = 0; i < variables_5D.size() -1 ; ++i) {
                    if(i == 0) pdf_buffer_5D_wrongpi0hyp[i] = make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_1D_mbc>(mbc, category_name_flags);
                    if(i == 1) pdf_buffer_5D_wrongpi0hyp[i] = make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_1D_deltae>(deltae, category_name_flags);
                    if(i == 2) pdf_buffer_5D_wrongpi0hyp[i] = make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_1D_nb_prime>(nb_prime, category_name_flags);
                    if(i == 3) pdf_buffer_5D_wrongpi0hyp[i] = make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_2D_dalitz>(mass_AB, mass_BC, category_name_flags);
                    if (i!=3) {
                        std::string varname = variables_5D[i]->GetName();
                        pdf_buffer_5D_wrongpi0hyp[i]->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongpi0hyp_1D_"+varname+".dat",true);
                    }else{
                        pdf_buffer_5D_wrongpi0hyp[i]->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongpi0hyp_2D_dalitz.dat",true);
                    }
                    pdf_buffer_5D_wrongpi0hyp[i]->SetVariablesConst();
                }
                shared_ptr<FF_PDF> pdf_bkg_bbar_wrongpi0hyp_5D = make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_5D_mbc_deltae_nb_prime_dalitz>(pdf_buffer_5D_wrongpi0hyp[0], pdf_buffer_5D_wrongpi0hyp[1], pdf_buffer_5D_wrongpi0hyp[2],pdf_buffer_5D_wrongpi0hyp[3], category_name_flags);
                pdf_bkg_bbar_wrongpi0hyp_5D->SetVariablesConst();
                pdf_bkg_bbar_wrongpi0hyp_5D->ExportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongpi0hyp_5D.dat",true);
                
                std::array<shared_ptr<FF_PDF>, 4> pdf_buffer_5D_missingfsp;
                for(unsigned int i = 0; i < variables_5D.size()-1; ++i) {
                    if(i == 0) pdf_buffer_5D_missingfsp[i] = make_shared<FF_PDF_bkg_bbar_missingfsp_1D_mbc>(mbc, category_name_flags);
                    if(i == 1) pdf_buffer_5D_missingfsp[i] = make_shared<FF_PDF_bkg_bbar_missingfsp_1D_deltae>(deltae, category_name_flags);
                    if(i == 2) pdf_buffer_5D_missingfsp[i] = make_shared<FF_PDF_bkg_bbar_missingfsp_1D_nb_prime>(nb_prime, category_name_flags);
                    if(i == 3) pdf_buffer_5D_missingfsp[i] = make_shared<FF_PDF_bkg_bbar_missingfsp_2D_dalitz>(mass_AB, mass_BC, category_name_flags);
                    if (i!=3) {
                        std::string varname = variables_5D[i]->GetName();
                        pdf_buffer_5D_missingfsp[i]->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_missingfsp_1D_"+varname+".dat",true);
                    }else{
                        pdf_buffer_5D_missingfsp[i]->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_missingfsp_2D_dalitz.dat",true);
                    }
                    pdf_buffer_5D_missingfsp[i]->SetVariablesConst();
                }
                shared_ptr<FF_PDF> pdf_bkg_bbar_missingfsp_5D = make_shared<FF_PDF_bkg_bbar_missingfsp_5D_mbc_deltae_nb_prime_dalitz>(pdf_buffer_5D_missingfsp[0], pdf_buffer_5D_missingfsp[1], pdf_buffer_5D_missingfsp[2], pdf_buffer_5D_missingfsp[3], category_name_flags);
                pdf_bkg_bbar_missingfsp_5D->SetVariablesConst();
                pdf_bkg_bbar_missingfsp_5D->ExportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_missingfsp_5D.dat",true);
                
                std::array<shared_ptr<FF_PDF>, 4> pdf_buffer_5D_nonres;
                for(unsigned int i = 0; i < variables_5D.size() -1; ++i) {
                    if(i == 0) pdf_buffer_5D_nonres[i] = make_shared<FF_PDF_bkg_bbar_nonres_1D_mbc>(mbc, category_name_flags);
                    if(i == 1) pdf_buffer_5D_nonres[i] = make_shared<FF_PDF_bkg_bbar_nonres_1D_deltae>(deltae, category_name_flags);
                    if(i == 2) pdf_buffer_5D_nonres[i] = make_shared<FF_PDF_bkg_bbar_nonres_1D_nb_prime>(nb_prime, category_name_flags);
                    if(i == 3) pdf_buffer_5D_nonres[i] = make_shared<FF_PDF_bkg_bbar_nonres_2D_dalitz>(mass_AB, mass_BC, category_name_flags);
                    if (i!=3) {
                        std::string varname = variables_5D[i]->GetName();
                        pdf_buffer_5D_nonres[i]->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_nonres_1D_"+varname+".dat",true);
                    }else{
                        pdf_buffer_5D_nonres[i]->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_nonres_2D_dalitz.dat",true);
                    }
                    pdf_buffer_5D_nonres[i]->SetVariablesConst();
                }
                shared_ptr<FF_PDF> pdf_bkg_bbar_nonres_5D = make_shared<FF_PDF_bkg_bbar_nonres_5D_mbc_deltae_nb_prime_dalitz>(pdf_buffer_5D_nonres[0], pdf_buffer_5D_nonres[1], pdf_buffer_5D_nonres[2], pdf_buffer_5D_nonres[3], category_name_flags);
                pdf_bkg_bbar_nonres_5D->SetVariablesConst();
                pdf_bkg_bbar_nonres_5D->ExportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_nonres_5D.dat",true);
                
                
                shared_ptr<FF_PDF> pdf_bkg_bbar_5D = make_shared<FF_PDF_bkg_bbar_5D_mbc_deltae_nb_prime_dalitz>(pdf_bkg_bbar_combinatoric_5D,
                                                                                                         pdf_bkg_bbar_wrongmasshyp_5D, pdf_bkg_bbar_wrongpi0hyp_5D,
                                                                                                         pdf_bkg_bbar_missingfsp_5D, pdf_bkg_bbar_nonres_5D, data_charged->sumEntries(), category_name_flags);
                
                
                pdf_bkg_bbar_5D->GetVariable("C_bkg_bbar_combinatoric_nevents")->setVal(number_of_entries[0]);
                //                pdf_bkg_bbar_5D->GetVariable("C_bkg_bbar_combinatoric_nevents")->setConstant(true);
                pdf_bkg_bbar_5D->GetVariable("C_bkg_bbar_wrongmasshyp_nevents")->setVal(number_of_entries[1]);
                pdf_bkg_bbar_5D->GetVariable("C_bkg_bbar_wrongmasshyp_nevents")->setConstant(true);
                pdf_bkg_bbar_5D->GetVariable("C_bkg_bbar_wrongpi0hyp_nevents")->setVal(number_of_entries[2]);
                pdf_bkg_bbar_5D->GetVariable("C_bkg_bbar_wrongpi0hyp_nevents")->setConstant(true);
                pdf_bkg_bbar_5D->GetVariable("C_bkg_bbar_missingfsp_nevents")->setVal(number_of_entries[3]);
                //                pdf_bkg_bbar_5D->GetVariable("C_bkg_bbar_missingfsp_nevents")->setConstant(true);
                pdf_bkg_bbar_5D->GetVariable("C_bkg_bbar_nonres_nevents")->setVal(number_of_entries[4]);
                //                pdf_bkg_bbar_5D->GetVariable("C_bkg_bbar_nonres_nevents")->setConstant(true);
                
                RooFitResult *result_5D = pdf_bkg_bbar_5D->GetSimPDF().fitTo(data, SumW2Error(true), NumCPU(opt->GetNumcpu()),
                                                                             Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()), Extended(is_combined_bbar_bkg));
                pdf_bkg_bbar_5D->GetSimPDF().getParameters(data)->Print("v");
                for(RooRealVar *v : variables_5D) {
                    std::string varname = v->GetName();
                    if (varname == "mbcp" || varname == "deltae" || varname == "nb_prime") {
                        FFUtil::VisualizeFitResult(pdf_bkg_bbar_5D,*data_charged,*v,category_name_flags,true,
                                                   opt->GetBins(),"full_range", opt->GetOutdir()+"BB_MC_5D_"+varname+"_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                                   opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                    }
                    if (varname == "mass_01" || varname == "mass_12") {
                        FFUtil::VisualizeFitResult(pdf_bkg_bbar_5D,*data_charged,*v,category_name_flags,true,
                                                   opt->GetBins(),"full_range1", opt->GetOutdir()+"BB_MC_5D_"+varname+"_projection_range1."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                                   opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                        FFUtil::VisualizeFitResult(pdf_bkg_bbar_5D,*data_charged,*v,category_name_flags,true,
                                                   opt->GetBins(),"full_range2", opt->GetOutdir()+"BB_MC_5D_"+varname+"_projection_range2."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                                   opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                        FFUtil::VisualizeFitResult(pdf_bkg_bbar_5D,*data_charged,*v,category_name_flags,true,
                                                   opt->GetBins(),"full_range3", opt->GetOutdir()+"BB_MC_5D_"+varname+"_projection_range3."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                                   opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                    }
                    
                    
                    
                    /*if(varname != "mbc") {
                        FFUtil::VisualizeFitResult(pdf_bkg_bbar_5D,data,*v,category_name_flags,true,
                                                   opt->GetBins(),"mbc_sideband", opt->GetOutdir()+"BB_MC_5D_"+varname+"_projection_mbc_sideband."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                                   opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                        FFUtil::VisualizeFitResult(pdf_bkg_bbar_5D,data,*v,category_name_flags,true,
                                                   opt->GetBins(),"mbc_signal_box", opt->GetOutdir()+"BB_MC_5D_"+varname+"_projection_mbc_signal_box."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                                   opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                    }*//*
                }
                WriteCorrelationMatrixFile(result_5D,opt->GetOutdir()+"pdf_bkg_bbar_5D_correlation.dat");
                pdf_bkg_bbar_5D->ExportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_5D.dat",true);
                result_5D->Print();
                delete result_5D;
            }*/
            
            
            
            delete data_charged;
        }
        for (int i =0; i<6; i++) {
            std::cout <<std::get<1>(bbar_bkg_types[i]) <<": \t"<< number_of_entries[i]<<std::endl;
        }
    }
    
}
