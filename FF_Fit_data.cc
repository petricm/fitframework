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
    
    void FitMC_C_and_N(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt) {
        
        if(chain->GetEntries() == 0) {
            std::cout << "No data for final fits given, skip fitting." << std::endl;
            return;
        }
        
        // ---------------------------------------------
        // ----- Define variables
        // ---------------------------------------------
        PrintFitStartBlock("STATUS: Define functions and variables...");
        RooRealVar mbc("mbcp","M'_{bc}",5.274, 5.285,"GeV/c^{2}");
        RooRealVar deltae("deltae","#Delta E",-0.08, 0.08,"GeV");
        //    RooRealVar mbcp("mbcp","m'_{bc}",5.25,5.29,"GeV");
        RooRealVar mass_BC("mass_01","M^{2}_{K#pi}",0,27,"GeV^{2}/c^{4}");//mass_BC_min+1.0e-4,mass_BC_max-1.0e-4,"GeV^{2}");
        RooRealVar mass_AB("mass_12","M^{2}_{#pi#pi^{0}}",0,27,"GeV^{2}/c^{4}");//mass_AB_min+1.0e-4,mass_AB_max-1.0e-4,"GeV^{2}");
        RooRealVar mass_CA("mass_02","M^{2}_{K#pi^0}",0,27,"GeV^{2}");
        RooRealVar nb_prime("nb_prime","C'_{NB}",1.0,10.0,"");
        RooRealVar mchash("mchash","mchash'",0,1e+9,"");
        RooCategory channel("channel","decay channel");
        channel.defineType("C",0);
        channel.defineType("N",1);
        RooCategory datatype("datatype","data sample type");
        datatype.defineType("CP",0);
        datatype.defineType("CA",1);
        datatype.defineType("N",2);
        datatype.defineType("CS",3);
        datatype.defineType("NS",4);
        datatype.defineType("COFF",5);
        datatype.defineType("NOFF",6);
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
        varset.add(datatype);
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
        varset.add(runno);
        
        //        RooDataSet data("data","data",varset,Import(*chain.get()),Cut("mbcp < 5.288 && (datatype==datatype::CP || datatype==datatype::CA) && dalitz==dalitz::IN"),WeightVar(weight));
        RooDataSet data("data","data",varset,Import(*chain.get()),Cut("mbcp < 5.288 && channel==channel::C && dalitz==dalitz::IN"),WeightVar(weight));
        // create helper for the simultanous fit
        std::vector<std::string> flags_datatype;
        flags_datatype.push_back("C");
        //        flags_datatype.push_back("CP");
        //        flags_datatype.push_back("CA");
        //	flags_datatype.push_back("N");
        //	flags_datatype.push_back("CS");
        //	flags_datatype.push_back("NS");
        //	flags_datatype.push_back("COFF");
        //	flags_datatype.push_back("NOFF");
        tuple_category_name_flags category_name_flags_datatype = std::make_tuple(&channel,"channel",flags_datatype);
        // create data subsamples
        RooDataSet* data_charged = dynamic_cast<RooDataSet*>(data.reduce("channel==channel::C"));
        data_charged->Print();
        // print data set
        data.Print();
        
        std::cout<<"!!!!!!"<<data.sumEntries("((mctype<1001 || mctype>1004) && runno==0 && (mcinfo==59 || mcinfo==61) && abs(chil0_mo)==511 && abs(chil1_mo)==511 && abs(chil2_mo)==511 )")<<std::endl;
        
        
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
        //	SetFitRanges(mbc,deltae,chil0_ma,mass_kpi,hel_phi,hel_th1,hel_th2,nb_prime);
        
        mbc.setRange("full_range",5.26, 5.288);
        deltae.setRange("full_range",-0.2, 0.2);
        nb_prime.setRange("full_range",-10.0, 10.0);
        
        mbc.setRange("full_range_SB1",5.26, 5.288);
        deltae.setRange("full_range_SB1",-0.08, 0.08);
        nb_prime.setRange("full_range_SB1",1, 10.0);
        
        mbc.setRange("full_range_SB2",5.274, 5.285);
        deltae.setRange("full_range_SB2",-0.2, 0.2);
        nb_prime.setRange("full_range_SB2",1, 10.0);
        
        mbc.setRange("full_range_SB3",5.274, 5.285);
        deltae.setRange("full_range_SB3",-0.08, 0.08);
        nb_prime.setRange("full_range_SB3",-10, 10.0);
        
        
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
            for(RooRealVar* var : variables_5D) {
                CreateDataComponentPlots(data,*var,opt->GetBins(),"full_range",opt->GetOutdir(),opt->GetPlotFormat(),"CP","datatype");
                CreateDataComponentPlots(data,*var,opt->GetBins(),"full_range",opt->GetOutdir(),opt->GetPlotFormat(),"CA","datatype");
            }
            mp::execute_command("mkdir -p "+opt->GetOutdir()+"DATA_MC && mv "+opt->GetOutdir()+"DATA_*."+opt->GetPlotFormat()+" "+opt->GetOutdir()+"DATA_MC");
        }
        // ---------------------------------------------
        // ----- Do fits
        // ---------------------------------------------
        // See RooFit tutorial macro #501 about HowTo do a simultaneous fit
        // See RooFit tutorial macro #603 about HowTo do a multi-core parallelized unbinned maximum likelihood fit
        std::cout << "STATUS: Do fits..." << std::endl;
        
        PrintFitStartBlock("FITTING: 3D Fit");
        
        shared_ptr<FF_PDF> pdf_bkg_continuum_3D = make_shared<FF_PDF_bkg_continuum_3D_mbc_deltae_nb_prime>(mbc, deltae, nb_prime, category_name_flags_datatype);
        pdf_bkg_continuum_3D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_continuum_3D.dat",true);
        pdf_bkg_continuum_3D->SetVariablesConst();
        
        shared_ptr<FF_PDF> pdf_signal_3D = make_shared<FF_PDF_signal_3D_mbc_deltae_nb_prime>(mbc, deltae, nb_prime, category_name_flags_datatype);
        pdf_signal_3D->ImportPDFValues(opt->GetOutdir()+"pdf_signal_3D.dat",true);
        pdf_signal_3D->SetVariablesConst();
        
        shared_ptr<FF_PDF> pdf_bkg_bbar_combinatoric_3D = make_shared<FF_PDF_bkg_bbar_combinatoric_3D_mbc_deltae_nb_prime>(mbc, deltae, nb_prime, category_name_flags_datatype);
        pdf_bkg_bbar_combinatoric_3D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_combinatoric_3D.dat",true);
        pdf_bkg_bbar_combinatoric_3D->SetVariablesConst();
        
        shared_ptr<FF_PDF> pdf_bkg_bbar_wrongmasshyp_3D = make_shared<FF_PDF_bkg_bbar_wrongmasshyp_3D_mbc_deltae_nb_prime>(mbc, deltae, nb_prime, category_name_flags_datatype);
        pdf_bkg_bbar_wrongmasshyp_3D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongmasshyp_3D.dat",true);
        pdf_bkg_bbar_wrongmasshyp_3D->SetVariablesConst();
        
        shared_ptr<FF_PDF> pdf_bkg_bbar_wrongpi0hyp_3D = make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_3D_mbc_deltae_nb_prime>(mbc, deltae, nb_prime, category_name_flags_datatype);
        pdf_bkg_bbar_wrongpi0hyp_3D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongpi0hyp_3D.dat",true);
        pdf_bkg_bbar_wrongpi0hyp_3D->SetVariablesConst();
        
        shared_ptr<FF_PDF> pdf_bkg_bbar_missingfsp_3D = make_shared<FF_PDF_bkg_bbar_missingfsp_3D_mbc_deltae_nb_prime>(mbc, deltae, nb_prime, category_name_flags_datatype);
        pdf_bkg_bbar_missingfsp_3D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_missingfsp_3D.dat",true);
        pdf_bkg_bbar_missingfsp_3D->SetVariablesConst();
        
        shared_ptr<FF_PDF> pdf_bkg_bbar_nonres_3D = make_shared<FF_PDF_bkg_bbar_nonres_3D_mbc_deltae_nb_prime>(mbc, deltae, nb_prime, category_name_flags_datatype);
        pdf_bkg_bbar_nonres_3D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_nonres_3D.dat",true);
        pdf_bkg_bbar_nonres_3D->SetVariablesConst();
        
        
        shared_ptr<FF_PDF> pdf_data_3D = make_shared<FF_PDF_data_3D_mbc_deltae_nb_prime__CPV>(pdf_signal_3D, pdf_bkg_continuum_3D, pdf_bkg_bbar_combinatoric_3D,pdf_bkg_bbar_wrongmasshyp_3D,pdf_bkg_bbar_wrongpi0hyp_3D, pdf_bkg_bbar_missingfsp_3D, pdf_bkg_bbar_nonres_3D, data.sumEntries(), category_name_flags_datatype);
        
        std::array<std::string, 7> cuts;
        typedef std::pair<const std::string, double> pair_string_double;
        std::map<std::string, double> truth_values;
        std::map<std::string, double> truth_values_func;
        
        // on MC we determine the truth and set better starting values
        cuts[0] = "((mctype<1001 || mctype>1004) && runno==0 && (mcinfo==59 || mcinfo==61) && abs(chil0_mo)==511 && abs(chil1_mo)==511 && abs(chil2_mo)==511 )";
        cuts[1] = "(mctype==1003 || mctype==1004)";
        cuts[2] = "(((mctype==1001 || mctype==1002) || ((mctype<1001 || mctype>1004) && runno==0 && mcinfo!=59 && mcinfo!=61)) && mchash==1)";
        cuts[3] = "(((mctype==1001 || mctype==1002) || ((mctype<1001 || mctype>1004) && runno==0 && mcinfo!=59 && mcinfo!=61)) && mchash!=1 && ((mcinfo & 0x8) ==0) && (((mcinfo & 0x4) !=0) || ((mcinfo & 0x2) !=0)) && mcinfo<64)";
        cuts[4] = "(((mctype==1001 || mctype==1002) || ((mctype<1001 || mctype>1004) && runno==0 && mcinfo!=59 && mcinfo!=61)) && mchash!=1 && mcinfo>64)";
        cuts[5] = "(((mctype==1001 || mctype==1002) || ((mctype<1001 || mctype>1004) && runno==0 && mcinfo!=59 && mcinfo!=61)) && mchash!=1 && !(((mcinfo & 0x8) ==0) && (((mcinfo & 0x4) !=0) || ((mcinfo & 0x2) !=0))) && mcinfo<64  && !(mchash==141183 ||mchash==2513873 ||mchash==2245944 ||mchash==2323258 ||mchash==2068777 ||mchash==1769521 ||mchash==1639018 ||mchash==3905351 ||mchash==2939915 ||mchash==1962594 ||mchash==3500850 ||mchash==1881338))";
        cuts[6] = "(((mctype==1001 || mctype==1002) || ((mctype<1001 || mctype>1004) && runno==0)) && (mcinfo==59 || mcinfo==61) && (mchash==141183 ||mchash==2513873 ||mchash==2245944 ||mchash==2323258 ||mchash==2068777 ||mchash==1769521 ||mchash==1639018 ||mchash==3905351 ||mchash==2939915 ||mchash==1962594 ||mchash==3500850 ||mchash==1881338))";
        
        
        
        for(unsigned int i = 0; i <= 6; i++) {
            RooDataSet* data_0 = dynamic_cast<RooDataSet*>(data.reduce(("channel==channel::C &&"+cuts[i]).c_str()));
            
            if(i==0) {
                pdf_data_3D->GetVariable("signal_nevents")->setVal(data_0->sumEntries());
                truth_values["signal_nevents"] = data_0->sumEntries();
                // truth_values_func["signal_nevents"] = data_0->sumEntries();
                RooDataSet* data_CP = dynamic_cast<RooDataSet*>(data_0->reduce("datatype==datatype::CP"));
                RooDataSet* data_CA = dynamic_cast<RooDataSet*>(data_0->reduce("datatype==datatype::CA"));
                truth_values["signal_nevents_asym"] = 0.0;
                //                truth_values_func["CP_signal_nevents"] = data_CP->sumEntries();
                //                truth_values_func["CA_signal_nevents"] = data_CA->sumEntries();
                delete data_CP;
                delete data_CA;
            } else if(i==1) {
                pdf_data_3D->GetVariable("C_bkg_continuum_nevents")->setVal(data_0->sumEntries());
                //                pdf_data_3D->GetVariable("C_bkg_continuum_nevents")->setConstant(true);
                truth_values["C_bkg_continuum_nevents"] = data_0->sumEntries();
                
            } else if(i==2) {
                pdf_data_3D->GetVariable("C_bkg_bbar_combinatoric_nevents")->setVal(data_0->sumEntries());
                //                pdf_data_3D->GetVariable("C_bkg_bbar_combinatoric_nevents")->setConstant(true);
                truth_values["C_bkg_bbar_combinatoric_nevents"] = data_0->sumEntries();
            } else if(i==3) {
                pdf_data_3D->GetVariable("C_bkg_bbar_wrongmasshyp_nevents")->setVal(data_0->sumEntries());
                pdf_data_3D->GetVariable("C_bkg_bbar_wrongmasshyp_nevents")->setConstant(true);
                truth_values["C_bkg_bbar_wrongmasshyp_nevents"] = data_0->sumEntries();
            } else if(i==4) {
                pdf_data_3D->GetVariable("C_bkg_bbar_wrongpi0hyp_nevents")->setVal(data_0->sumEntries());
                pdf_data_3D->GetVariable("C_bkg_bbar_wrongpi0hyp_nevents")->setConstant(true);
                truth_values["C_bkg_bbar_wrongpi0hyp_nevents"] = data_0->sumEntries();
            } else if(i==5) {
                pdf_data_3D->GetVariable("C_bkg_bbar_missingfsp_nevents")->setVal(data_0->sumEntries());
                pdf_data_3D->GetVariable("C_bkg_bbar_missingfsp_nevents")->setConstant(true);
                truth_values["C_bkg_bbar_missingfsp_nevents"] = data_0->sumEntries();
            } else if(i==6) {
                pdf_data_3D->GetVariable("C_bkg_bbar_nonres_nevents")->setVal(data_0->sumEntries());
                //                pdf_data_3D->GetVariable("C_bkg_bbar_nonres_nevents")->setConstant(true);
                truth_values["C_bkg_bbar_nonres_nevents"] = data_0->sumEntries();
            }
            
            delete data_0;
        }
        /*

            pdf_data_3D->GetVariable("signal_nevents")->setVal(5911.34);
            pdf_data_3D->GetVariable("C_bkg_continuum_nevents")->setVal(221155);
            pdf_data_3D->GetVariable("C_bkg_bbar_combinatoric_nevents")->setVal(15047.2);
            pdf_data_3D->GetVariable("C_bkg_bbar_wrongmasshyp_nevents")->setVal(1189.95);
            pdf_data_3D->GetVariable("C_bkg_bbar_wrongmasshyp_nevents")->setConstant(true);
            pdf_data_3D->GetVariable("C_bkg_bbar_wrongpi0hyp_nevents")->setVal(1210);
            pdf_data_3D->GetVariable("C_bkg_bbar_wrongpi0hyp_nevents")->setConstant(true);
            pdf_data_3D->GetVariable("C_bkg_bbar_missingfsp_nevents")->setVal(7758);
            pdf_data_3D->GetVariable("C_bkg_bbar_nonres_nevents")->setVal(1868);
            pdf_data_3D->GetVariable("C_bkg_bbar_nonres_nevents")->setConstant(true);
        /**/
        
        // deactivate optimization for combined fit
        // although we have a weighted dataset, we set SumW2Error(false) so that we get errors that correspond to the
        // statistics we would have if we have 'sumw2' events, and not errors that correspond to the number of entries
        //RooFitResult *result_3D = pdf_data_3D->GetSimPDF().fitTo(data, SumW2Error(true), NumCPU(opt->GetNumcpu()), Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()), Extended(true));
        //pdf_data_3D->GetSimPDF().getParameters(data)->Print("v");
        
        
        /*
        if(opt->GetCreatePlots()) {
            for(RooRealVar *v : variables_3D) {
                std::string varname = v->GetName();
                PrintFitStartBlock("PLOT PROJECTION");
                FFUtil::VisualizeFitResult(pdf_data_3D,data,*v,category_name_flags_datatype,false,
                                           opt->GetBins(),"full_range", opt->GetOutdir()+"MC_3D_"+varname+"_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                           opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                
                if (varname=="mbcp") {
                    FFUtil::VisualizeFitResult(pdf_data_3D,data,*v,category_name_flags_datatype,false,
                                               opt->GetBins(),"full_range_SB1", opt->GetOutdir()+"MC_3D_"+varname+"_projection_signalbox."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                               opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                }
                if (varname=="deltae") {
                    FFUtil::VisualizeFitResult(pdf_data_3D,data,*v,category_name_flags_datatype,false,
                                               opt->GetBins(),"full_range_SB2", opt->GetOutdir()+"MC_3D_"+varname+"_projection_signalbox."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                               opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                }
                if (varname=="nb_prime") {
                    FFUtil::VisualizeFitResult(pdf_data_3D,data,*v,category_name_flags_datatype,false,
                                               opt->GetBins(),"full_range_SB3", opt->GetOutdir()+"MC_3D_"+varname+"_projection_signalbox."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                               opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                }

            }
           // WriteCorrelationMatrixFile(result_3D,opt->GetOutdir()+"pdf_data_3D_correlation.dat");
            pdf_data_3D->ExportPDFValues(opt->GetOutdir()+"pdf_data_3D.dat",true);
        }
         */
        //result_3D->Print();
//        std::cout<<"NLL: "<<std::setprecision(16)<<result_3D->minNll()<<std::endl;
        std::cout << "Signal-Yield/Signal-YieldError = " << pdf_data_3D->GetVariable("signal_nevents")->getVal()/pdf_data_3D->GetVariable("signal_nevents")->getError() << std::endl;
        
        
        std::cout << "Truth values for this fit:" << std::endl;
        for(pair_string_double& p : truth_values) {
            std::cout << mp::pair_to_string(p) << std::endl;
        }
        /*
        std::cout << "Truth and calculated values for functions:" << std::endl;
        for(pair_string_double& p : truth_values_func) {
            std::cout << mp::pair_to_string(p) << "  ";
            std::cout << pdf_data_3D->GetFunction(p.first)->getVal() << std::endl;
        }
        // create a fit result file, that can be used for ToyMC studies
        /*
         TUUID uuid;
         std::cout << "UUID for this sample: " << uuid.AsString() << std::endl;
         std::ofstream fout((opt->GetOutdir()+"MC_3D_fit_result_"+uuid.AsString()+".dat").c_str());
         if(fout.is_open()) {
         for(pair_string_double& p : truth_values) {
         std::string name = p.first;
         double truth = p.second;
         RooRealVar* var = pdf_data_3D->GetVariable(name);
         fout << name << ";";
         fout << truth << ";";
         fout << var->getVal() << ";";
         fout << var->getError() << ";";
         fout << var->getErrorLo() << ";";
         fout << var->getErrorHi() << std::endl;
         }
         fout.close();
         } else {
         std::cerr << "ERROR: Could not write fit result to file" << std::endl;
         }
         */
        //delete result_3D;
        
        delete data_charged;
        
    }
    
    void FitMC_4D(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt) {
        
        if(chain->GetEntries() == 0) {
            std::cout << "No data for final fits given, skip fitting." << std::endl;
            return;
        }
        /*
         // ---------------------------------------------
         // ----- Define variables
         // ---------------------------------------------
         PrintFitStartBlock("STATUS: Define functions and variables...");
         RooRealVar mbc("mbcp","m'_{bc}",5.25, 5.288,"GeV/c^{2}");
         RooRealVar deltae("deltae","#Delta E",-0.2,0.2,"GeV");
         RooRealVar mass_BC("mass_01","M^{2}_{K#pi}",0,27,"GeV^{2}/c^{4}");//mass_BC_min+1.0e-4,mass_BC_max-1.0e-4,"GeV^{2}");
         RooRealVar mass_AB("mass_12","M^{2}_{#pi#pi^{0}}",0,27,"GeV^{2}/c^{4}");//mass_AB_min+1.0e-4,mass_AB_max-1.0e-4,"GeV^{2}");
         RooRealVar mass_CA("mass_02","M^{2}_{K#pi^0}",0,27,"GeV^{2}/c^{4}");
         RooRealVar nb_prime("nb_prime","NB'",-10.0,10.0,"");
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
         
         std::vector<RooRealVar*> variables_4D;
         variables_4D.push_back(&mbc);
         variables_4D.push_back(&deltae);
         variables_4D.push_back(&nb_prime);
         variables_4D.push_back(&flavor);
         
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
         mass_BC.setRange("full_range3",3.8, 27);
         // ---------------------------------------------
         // ----- Create data component plots
         // ---------------------------------------------
         if(opt->GetCreateComponentPlots()) {
         std::cout << "STATUS: Create data component plots..." << std::endl;
         for(RooRealVar* var : variables_3D) {
         CreateDataComponentPlots(data,*var,opt->GetBins(),"full_range",opt->GetOutdir(),opt->GetPlotFormat(),"CP","flavor");
         CreateDataComponentPlots(data,*var,opt->GetBins(),"full_range",opt->GetOutdir(),opt->GetPlotFormat(),"CA","flavor");
         //CreateDataComponentPlots(data_sideband,*var,opt->GetBins(),"full_range_SB",opt->GetOutdir(),opt->GetPlotFormat(),"CS","datatype");
         }
         mp::execute_command("mkdir -p "+opt->GetOutdir()+"DATA_MC && mv "+opt->GetOutdir()+"DATA_*."+opt->GetPlotFormat()+" "+opt->GetOutdir()+"DATA_MC");
         }
         // ---------------------------------------------
         // ----- Do fits
         // ---------------------------------------------
         // See RooFit tutorial macro #501 about HowTo do a simultaneous fit
         // See RooFit tutorial macro #603 about HowTo do a multi-core parallelized unbinned maximum likelihood fit
         std::cout << "STATUS: Do fits..." << std::endl;
         
         PrintFitStartBlock("FITTING: 3D Fit");
         
         shared_ptr<FF_PDF> pdf_bkg_continuum_4D = make_shared<FF_PDF_bkg_continuum_4D_mbc_deltae_chil0_ma_nb_prime>(mbc,
         deltae, chil0_ma, nb_prime, category_name_flags_datatype,false,(RooRealVar*)NULL);
         pdf_bkg_continuum_4D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_continuum_4D.dat",true);
         pdf_bkg_continuum_4D->GetVariable("N_chil0_ma_bkg_continuum_nonres_fraction")->setConstant(true);
         //pdf_bkg_continuum_4D->SetVariablesConst();
         shared_ptr<FF_PDF> pdf_signal_4D = 	make_shared<FF_PDF_signal_4D_mbc_deltae_chil0_ma_nb_prime>(mbc,
         deltae, chil0_ma, nb_prime, category_name_flags_datatype,false,pdf_bkg_continuum_4D->GetVariable("chil0_ma_resolution"));
         pdf_signal_4D->ImportPDFValues(opt->GetOutdir()+"pdf_signal_4D.dat",true);
         pdf_signal_4D->SetVariablesConst();
         //pdf_signal_4D->GetVariable("C_deltae_signal_width_core")->setConstant(false);
         shared_ptr<FF_PDF> pdf_bkg_bbar_wrongphikkbar_4D = make_shared<FF_PDF_bkg_bbar_wrongphikkbar_4D_mbc_deltae_chil0_ma_nb_prime>(mbc,
         deltae, chil0_ma, nb_prime, category_name_flags_datatype);
         pdf_bkg_bbar_wrongphikkbar_4D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongphikkbar_4D.dat",true);
         pdf_bkg_bbar_wrongphikkbar_4D->SetVariablesConst();
         
         shared_ptr<FF_PDF> pdf_data_4D = make_shared<FF_PDF_data_4D_mbc_deltae_chil0_ma_nb_prime__CPV>(pdf_signal_4D,
         pdf_bkg_continuum_4D, pdf_bkg_bbar_wrongphikkbar_4D,
         data.sumEntries(), category_name_flags_datatype);
         
         // fit the continuum pdf to sideband data and fix the shape
         mbc.setMin(5.24);
         mbc.setMax(5.26);
         mbc.setRange("full_range",5.24,5.26);
         
         RooFitResult *result_4D_sideband = pdf_bkg_continuum_4D->GetSimPDF().fitTo(data_sideband, SumW2Error(false), NumCPU(opt->GetNumcpu()),
         Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()), Extended(false));
         pdf_bkg_continuum_4D->GetSimPDF().getParameters(data_sideband)->Print("v");
         
         if(opt->GetCreatePlots()) {
         for(RooRealVar *v : variables_4D) {
         std::string varname = v->GetName();
         
         PrintFitStartBlock("PLOT PROJECTION");
         FFUtil::VisualizeFitResult(pdf_bkg_continuum_4D,data_sideband,*v,category_name_flags_datatype,false,
         opt->GetBins(),"full_range", opt->GetOutdir()+"MC_4D_sideband_"+varname+"_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         }
         WriteCorrelationMatrixFile(result_4D_sideband,opt->GetOutdir()+"pdf_data_4D_sideband_correlation.dat");
         pdf_bkg_continuum_4D->ExportPDFValues(opt->GetOutdir()+"pdf_data_4D_sideband.dat",true);
         }
         result_4D_sideband->Print();
         
         // fit the fit region data with fixed pdf
         mbc.setMax(5.29);
         mbc.setMin(5.26);
         mbc.setRange("full_range",5.26,5.29);
         pdf_bkg_continuum_4D->SetVariablesConst();
         pdf_bkg_continuum_4D->GetVariable("mbc_bkg_continuum_c")->setConstant(false);
         
         RooFitResult *result_4D = pdf_data_4D->GetSimPDF().fitTo(data, SumW2Error(false), NumCPU(opt->GetNumcpu()),
         Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()), Extended(true),
         Constrain(RooArgSet(*pdf_data_4D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents"))));
         pdf_data_4D->GetSimPDF().getParameters(data)->Print("v");
         
         if(opt->GetCreatePlots()) {
         for(RooRealVar *v : variables_4D) {
         std::string varname = v->GetName();
         
         PrintFitStartBlock("PLOT PROJECTION");
         FFUtil::VisualizeFitResult(pdf_data_4D,data,*v,category_name_flags_datatype,true,
         opt->GetBins(),"full_range", opt->GetOutdir()+"MC_4D_"+varname+"_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         if(varname != "mbc") {
         FFUtil::VisualizeFitResult(pdf_data_4D,data,*v,category_name_flags_datatype,true,
         opt->GetBins(),"mbc_sideband", opt->GetOutdir()+"MC_4D_"+varname+"_projection_mbc_sideband."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         }
         }
         WriteCorrelationMatrixFile(result_4D,opt->GetOutdir()+"pdf_data_4D_correlation.dat");
         pdf_data_4D->ExportPDFValues(opt->GetOutdir()+"pdf_data_4D.dat",true);
         }
         result_4D->Print();
         
         std::cout << "Signal-Yield/Signal-YieldError = " << pdf_data_4D->GetVariable("signal_nevents")->getVal()/pdf_data_4D->GetVariable("signal_nevents")->getError() << std::endl;
         delete result_4D;
         delete result_4D_sideband;*/
    }
    
    void FitMC_9D(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt) {
        
        if(chain->GetEntries() == 0) {
            std::cout << "No data for final fits given, skip fitting." << std::endl;
            return;
        }
        
        // ---------------------------------------------
        // ----- Define variables
        // ---------------------------------------------
        PrintFitStartBlock("STATUS: Define functions and variables...");
        PrintFitStartBlock("STATUS: Define functions and variables...");
        RooRealVar mbc("mbcp","M'_{bc}",5.26, 5.288,"GeV/c^{2}");
        RooRealVar deltae("deltae","#Delta E",-0.2,0.2,"GeV");
        RooRealVar mass_BC("mass_01","m^{2}_{K#pi}",0,27,"GeV^{2}/c^{4}");
        RooRealVar mass_AB("mass_12","m^{2}_{#pi#pi^{0}}",0,27,"GeV^{2}/c^{4}");
        RooRealVar mass_CA("mass_02","m^{2}_{K#pi^0}",0,27,"GeV^{2}/c^{4}");
        RooRealVar nb_prime("nb_prime","C_{NB}'",-10,10.0,"");
        RooCategory channel("channel","decay channel");
        channel.defineType("C",0);
        RooCategory datatype("datatype","data sample type");
        datatype.defineType("CP",0);
        datatype.defineType("CA",1);
        datatype.defineType("CS",3);
        datatype.defineType("COFF",5);
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
        RooRealVar mchash("mchash","mchash'",0,1e+9,"");
        RooRealVar runno("runno","Run Number",-999999,999999);
        RooCategory dalitz("dalitz","Inside Dalitz space");
        dalitz.defineType("IN",1);
        dalitz.defineType("OUT",-1);
        // event weight
        RooRealVar weight("weight","weight",-999999,999999);
        // create data set (constructor only overload for max. 9 arguments, so we add some afterwards)
        RooArgSet varset(mbc,deltae,nb_prime, mass_AB, mass_BC, mass_CA);
        varset.add(channel);
        varset.add(datatype);
        varset.add(flavor);
        varset.add(dalitz);
        varset.add(mcinfo);
        varset.add(mctype);
        varset.add(chil0_mo);
        varset.add(chil1_mo);
        varset.add(chil2_mo);
        varset.add(chil0_mc);
        varset.add(chil1_mc);
        varset.add(chil2_mc);
        varset.add(mchash);
        varset.add(runno);
        varset.add(weight);
        
        RooDataSet data("data","data",varset,Import(*chain.get()),Cut("mbcp < 5.288 && dalitz==dalitz::IN"),WeightVar(weight));
        RooDataSet data_sideband("data_sideband","data_sideband",varset,Import(*chain.get()),Cut("mbcp < 5.288 && (datatype==datatype::CS) && dalitz==dalitz::IN"),WeightVar(weight));
        // create helper for the simultanous fit
        std::vector<std::string> flags_channel;
        flags_channel.push_back("C");
        tuple_category_name_flags category_name_flags_channel = std::make_tuple(&channel,"channel",flags_channel);
        // print data set
        data.Print();
        data_sideband.Print();
        
        std::cout<<"!!!!!!"<<data.sumEntries("((mctype<1001 || mctype>1004) && runno==0 && (mcinfo==59 || mcinfo==61) && abs(chil0_mo)==511 && abs(chil1_mo)==511 && abs(chil2_mo)==511 )")<<std::endl;
        
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
//        SetFitRanges(mbc,deltae,chil0_ma,mass_kpi,hel_phi,hel_th1,hel_th2,nb_prime);
        
        mbc.setRange("full_range",5.26, 5.288);
        deltae.setRange("full_range",-0.2, 0.2);
        nb_prime.setRange("full_range",-10.0, 10.0);
        mass_AB.setRange("full_range",0, 27);
        mass_BC.setRange("full_range",0, 27);
        
        
        mbc.setRange("full_range_SB1",5.26, 5.288);
        deltae.setRange("full_range_SB1",-0.08, 0.08);
        nb_prime.setRange("full_range_SB1",1, 10.0);
        mass_AB.setRange("full_range_SB1",0, 27);
        mass_BC.setRange("full_range_SB1",0, 27);
        
        mbc.setRange("full_range_SB2",5.274, 5.285);
        deltae.setRange("full_range_SB2",-0.2, 0.2);
        nb_prime.setRange("full_range_SB2",1, 10.0);
        mass_AB.setRange("full_range_SB2",0, 27);
        mass_BC.setRange("full_range_SB2",0, 27);
        
        mbc.setRange("full_range_SB3",5.274, 5.285);
        deltae.setRange("full_range_SB3",-0.08, 0.08);
        nb_prime.setRange("full_range_SB3",-10, 10.0);
        mass_AB.setRange("full_range_SB3",0, 27);
        mass_BC.setRange("full_range_SB3",0, 27);
        
        
        /*
        mbc.setRange("range_SB_1",5.274, 5.285);
        deltae.setRange("range_SB_1",-0.2, -0.12);
        nb_prime.setRange("range_SB_1",1, 10.0);
        mass_AB.setRange("range_SB_1",0, 27);
        mass_BC.setRange("range_SB_1",0, 3.3);
        
        mbc.setRange("range_SB_2",5.274, 5.285);
        deltae.setRange("range_SB_2",-0.2, -0.12);
        nb_prime.setRange("range_SB_2",1, 10.0);
        mass_AB.setRange("range_SB_2",0, 27);
        mass_BC.setRange("range_SB_2",3.3, 3.8);
        
        mbc.setRange("range_SB_3",5.274, 5.285);
        deltae.setRange("range_SB_3",-0.2, -0.12);
        nb_prime.setRange("range_SB_3",1, 10.0);
        mass_AB.setRange("range_SB_3",0, 27);
        mass_BC.setRange("range_SB_3",3.8, 14);
        
        mbc.setRange("range_SB_4",5.274, 5.285);
        deltae.setRange("range_SB_4",-0.2, -0.12);
        nb_prime.setRange("range_SB_4",1, 10.0);
        mass_AB.setRange("range_SB_4",0, 27);
        mass_BC.setRange("range_SB_4",14, 27);
        
        
        mbc.setRange("range_SB_5",5.274, 5.285);
        deltae.setRange("range_SB_5",-0.2, -0.12);
        nb_prime.setRange("range_SB_5",1, 10.0);
        mass_BC.setRange("range_SB_5",0, 27);
        mass_AB.setRange("range_SB_5",0, 3.3);
        
        mbc.setRange("range_SB_6",5.274, 5.285);
        deltae.setRange("range_SB_6",-0.2, -0.12);
        nb_prime.setRange("range_SB_6",1, 10.0);
        mass_BC.setRange("range_SB_6",0, 27);
        mass_AB.setRange("range_SB_6",3.3, 3.8);
        
        mbc.setRange("range_SB_7",5.274, 5.285);
        deltae.setRange("range_SB_7",-0.2, -0.12);
        nb_prime.setRange("range_SB_7",1, 10.0);
        mass_BC.setRange("range_SB_7",0, 27);
        mass_AB.setRange("range_SB_7",3.8, 14);
        
        mbc.setRange("range_SB_8",5.274, 5.285);
        deltae.setRange("range_SB_8",-0.2, -0.12);
        nb_prime.setRange("range_SB_8",1, 10.0);
        mass_BC.setRange("range_SB_8",0, 27);
        mass_AB.setRange("range_SB_8",14, 27);
        
        */
        
        
        mbc.setRange("range_SB_1",5.274, 5.285);
        deltae.setRange("range_SB_1",-0.08, 0.08);
        nb_prime.setRange("range_SB_1",1, 10.0);
        mass_AB.setRange("range_SB_1",0, 27);
        mass_BC.setRange("range_SB_1",0, 3.3);
        
        mbc.setRange("range_SB_2",5.274, 5.285);
        deltae.setRange("range_SB_2",-0.08, 0.08);
        nb_prime.setRange("range_SB_2",1, 10.0);
        mass_AB.setRange("range_SB_2",0, 27);
        mass_BC.setRange("range_SB_2",3.3, 3.8);
        
        mbc.setRange("range_SB_3",5.274, 5.285);
        deltae.setRange("range_SB_3",-0.08, 0.08);
        nb_prime.setRange("range_SB_3",1, 10.0);
        mass_AB.setRange("range_SB_3",0, 27);
        mass_BC.setRange("range_SB_3",3.8, 14);
        
        mbc.setRange("range_SB_4",5.274, 5.285);
        deltae.setRange("range_SB_4",-0.08, 0.08);
        nb_prime.setRange("range_SB_4",1, 10.0);
        mass_AB.setRange("range_SB_4",0, 27);
        mass_BC.setRange("range_SB_4",14, 27);
        
        
        mbc.setRange("range_SB_5",5.274, 5.285);
        deltae.setRange("range_SB_5",-0.08, 0.08);
        nb_prime.setRange("range_SB_5",1, 10.0);
        mass_BC.setRange("range_SB_5",0, 27);
        mass_AB.setRange("range_SB_5",0, 3.3);
        
        mbc.setRange("range_SB_6",5.274, 5.285);
        deltae.setRange("range_SB_6",-0.08, 0.08);
        nb_prime.setRange("range_SB_6",1, 10.0);
        mass_BC.setRange("range_SB_6",0, 27);
        mass_AB.setRange("range_SB_6",3.3, 3.8);
        
        mbc.setRange("range_SB_7",5.274, 5.285);
        deltae.setRange("range_SB_7",-0.08, 0.08);
        nb_prime.setRange("range_SB_7",1, 10.0);
        mass_BC.setRange("range_SB_7",0, 27);
        mass_AB.setRange("range_SB_7",3.8, 14);
        
        mbc.setRange("range_SB_8",5.274, 5.285);
        deltae.setRange("range_SB_8",-0.08, 0.08);
        nb_prime.setRange("range_SB_8",1, 10.0);
        mass_BC.setRange("range_SB_8",0, 27);
        mass_AB.setRange("range_SB_8",14, 27);
        
        
        // ---------------------------------------------
        // ----- Create data component plots
        // ---------------------------------------------
        if(opt->GetCreateComponentPlots()) {
            std::cout << "STATUS: Create data component plots..." << std::endl;
            for(RooRealVar* var : variables_5D) {
                CreateDataComponentPlots(data,*var,opt->GetBins(),"full_range",opt->GetOutdir(),opt->GetPlotFormat(),"CP","datatype");
                CreateDataComponentPlots(data,*var,opt->GetBins(),"full_range",opt->GetOutdir(),opt->GetPlotFormat(),"CA","datatype");
                CreateDataComponentPlots(data_sideband,*var,opt->GetBins(),"full_range_SB",opt->GetOutdir(),opt->GetPlotFormat(),"CS","datatype");
            }
            mp::execute_command("mkdir -p "+opt->GetOutdir()+"DATA_MC && mv "+opt->GetOutdir()+"DATA_*."+opt->GetPlotFormat()+" "+opt->GetOutdir()+"DATA_MC");
        }
        // ---------------------------------------------
        // ----- Do fits
        // ---------------------------------------------
        // See RooFit tutorial macro #501 about HowTo do a simultaneous fit
        // See RooFit tutorial macro #603 about HowTo do a multi-core parallelized unbinned maximum likelihood fit
        std::cout << "STATUS: Do fits..." << std::endl;
        
        PrintFitStartBlock("SETUP: CacheIntegrator");
        int debug_level = 1;
        CacheIntegrator<DalitzAnalysisNormCachedIntegral>* dalitz_analysis_norm_cache_integrator = new CacheIntegrator<DalitzAnalysisNormCachedIntegral>(debug_level);
        CacheIntegrator<DalitzAnalysisRealCachedIntegral>* dalitz_analysis_real_cache_integrator = new CacheIntegrator<DalitzAnalysisRealCachedIntegral>(debug_level);
        CacheIntegrator<DalitzAnalysisImagCachedIntegral>* dalitz_analysis_imag_cache_integrator = new CacheIntegrator<DalitzAnalysisImagCachedIntegral>(debug_level);

        
        EFFICIENCY_FUNCTION acceptance = BIN_2D; // or FLAT
        std::string path_to_integrals = "./CachedIntegrals_ACCEPTANCE" + FFUtil::GetAcceptanceString(acceptance) + "/";
        dalitz_analysis_norm_cache_integrator->LoadIntegrals(path_to_integrals);
        dalitz_analysis_real_cache_integrator->LoadIntegrals(path_to_integrals);
        dalitz_analysis_imag_cache_integrator->LoadIntegrals(path_to_integrals);
        
        
        std::cout << "Reading fit configuration from " << opt->GetConfigFile() << std::endl;
        namespace bpt = boost::property_tree;
        bpt::ptree pt;
        bpt::info_parser::read_info(opt->GetConfigFile(),pt);
        
        
        DAI_PDG_constants dai_pdg_constants = {m_Pi0, m_Pi, m_K, m_B0};
        
        DAI_CacheIntegrators_2D dai_2D = {
            dalitz_analysis_norm_cache_integrator,
            dalitz_analysis_real_cache_integrator,
            dalitz_analysis_imag_cache_integrator
        };
        
        DAI_Dalitz_Model_parameters dai_DM_pars{
            pt.get<double>("r_1"),
            pt.get<double>("a_2"),
            pt.get<double>("r_2"),
            pt.get<double>("cutoff_2"),
            pt.get<double>("r_3"),
            pt.get<double>("r_4"),
            pt.get<double>("r_5"),
            pt.get<double>("r_6"),
            pt.get<double>("a_7"),
            pt.get<double>("r_7"),
            pt.get<double>("cutoff_7")
        };
        
        std::map<std::string, double> truth_map;
        truth_map["mass_1"] = pt.get<double>("mass_1");
        truth_map["mass_2"] = pt.get<double>("mass_2");
        truth_map["mass_3"] = pt.get<double>("mass_3");
        truth_map["mass_4"] = pt.get<double>("mass_4");
        truth_map["mass_5"] = pt.get<double>("mass_5");
        truth_map["mass_6"] = pt.get<double>("mass_6");
        truth_map["mass_7"] = pt.get<double>("mass_7");
        
        truth_map["width_1"] = pt.get<double>("width_1");
        truth_map["width_2"] = pt.get<double>("width_2");
        truth_map["width_3"] = pt.get<double>("width_3");
        truth_map["width_4"] = pt.get<double>("width_4");
        truth_map["width_5"] = pt.get<double>("width_5");
        truth_map["width_6"] = pt.get<double>("width_6");
        truth_map["width_7"] = pt.get<double>("width_7");
        
        truth_map["amp_1"] = pt.get<double>("amp_1");
        truth_map["amp_2"] = pt.get<double>("amp_2");
        truth_map["amp_3"] = pt.get<double>("amp_3");
        truth_map["amp_4"] = pt.get<double>("amp_4");
        truth_map["amp_5"] = pt.get<double>("amp_5");
        truth_map["amp_6"] = pt.get<double>("amp_6");
        truth_map["amp_7"] = pt.get<double>("amp_7");
        truth_map["amp_8"] = pt.get<double>("amp_8");
        
        truth_map["phase_1"] = pt.get<double>("phase_1");
        truth_map["phase_2"] = pt.get<double>("phase_2");
        truth_map["phase_3"] = pt.get<double>("phase_3");
        truth_map["phase_4"] = pt.get<double>("phase_4");
        truth_map["phase_5"] = pt.get<double>("phase_5");
        truth_map["phase_6"] = pt.get<double>("phase_6");
        truth_map["phase_7"] = pt.get<double>("phase_7");
        truth_map["phase_8"] = pt.get<double>("phase_8");
        
        truth_map["amp_1_CC"] = pt.get<double>("amp_1_CC");
        truth_map["amp_2_CC"] = pt.get<double>("amp_2_CC");
        truth_map["amp_3_CC"] = pt.get<double>("amp_3_CC");
        truth_map["amp_4_CC"] = pt.get<double>("amp_4_CC");
        truth_map["amp_5_CC"] = pt.get<double>("amp_5_CC");
        truth_map["amp_6_CC"] = pt.get<double>("amp_6_CC");
        truth_map["amp_7_CC"] = pt.get<double>("amp_7_CC");
        truth_map["amp_8_CC"] = pt.get<double>("amp_8_CC");
        
        truth_map["phase_1_CC"] = pt.get<double>("phase_1_CC");
        truth_map["phase_2_CC"] = pt.get<double>("phase_2_CC");
        truth_map["phase_3_CC"] = pt.get<double>("phase_3_CC");
        truth_map["phase_4_CC"] = pt.get<double>("phase_4_CC");
        truth_map["phase_5_CC"] = pt.get<double>("phase_5_CC");
        truth_map["phase_6_CC"] = pt.get<double>("phase_6_CC");
        truth_map["phase_7_CC"] = pt.get<double>("phase_7_CC");
        truth_map["phase_8_CC"] = pt.get<double>("phase_8_CC");
        
        PrintFitStartBlock("SETUP: 5D Fit");
        
        
        shared_ptr<FF_PDF> pdf_signal_3D_dalitz = make_shared<FF_PDF_signal_3D_dalitz_analysis_CPV>(mass_AB, mass_BC, flavor, acceptance, dai_pdg_constants, dai_DM_pars, dai_2D, category_name_flags_channel);
        
        pdf_signal_3D_dalitz->GetVariable("mass_1")->setVal(truth_map["mass_1"]);
        pdf_signal_3D_dalitz->GetVariable("mass_2")->setVal(truth_map["mass_2"]);
        pdf_signal_3D_dalitz->GetVariable("mass_3")->setVal(truth_map["mass_3"]);
        pdf_signal_3D_dalitz->GetVariable("mass_4")->setVal(truth_map["mass_4"]);
        pdf_signal_3D_dalitz->GetVariable("mass_5")->setVal(truth_map["mass_5"]);
        pdf_signal_3D_dalitz->GetVariable("mass_6")->setVal(truth_map["mass_6"]);
        pdf_signal_3D_dalitz->GetVariable("mass_7")->setVal(truth_map["mass_7"]);
        
        pdf_signal_3D_dalitz->GetVariable("width_1")->setVal(truth_map["width_1"]);
        pdf_signal_3D_dalitz->GetVariable("width_2")->setVal(truth_map["width_2"]);
        pdf_signal_3D_dalitz->GetVariable("width_3")->setVal(truth_map["width_3"]);
        pdf_signal_3D_dalitz->GetVariable("width_4")->setVal(truth_map["width_4"]);
        pdf_signal_3D_dalitz->GetVariable("width_5")->setVal(truth_map["width_5"]);
        pdf_signal_3D_dalitz->GetVariable("width_6")->setVal(truth_map["width_6"]);
        pdf_signal_3D_dalitz->GetVariable("width_7")->setVal(truth_map["width_7"]);
        
        pdf_signal_3D_dalitz->GetVariable("amp_1")->setVal(truth_map["amp_1"]);
        pdf_signal_3D_dalitz->GetVariable("amp_2")->setVal(truth_map["amp_2"]);
        pdf_signal_3D_dalitz->GetVariable("amp_3")->setVal(truth_map["amp_3"]);
        pdf_signal_3D_dalitz->GetVariable("amp_4")->setVal(truth_map["amp_4"]);
        pdf_signal_3D_dalitz->GetVariable("amp_5")->setVal(truth_map["amp_5"]);
        pdf_signal_3D_dalitz->GetVariable("amp_6")->setVal(truth_map["amp_6"]);
        pdf_signal_3D_dalitz->GetVariable("amp_7")->setVal(truth_map["amp_7"]);
        pdf_signal_3D_dalitz->GetVariable("amp_8")->setVal(truth_map["amp_8"]);
        
        pdf_signal_3D_dalitz->GetVariable("phase_1")->setVal(truth_map["phase_1"]);
        pdf_signal_3D_dalitz->GetVariable("phase_2")->setVal(truth_map["phase_2"]);
        pdf_signal_3D_dalitz->GetVariable("phase_3")->setVal(truth_map["phase_3"]);
        pdf_signal_3D_dalitz->GetVariable("phase_4")->setVal(truth_map["phase_4"]);
        pdf_signal_3D_dalitz->GetVariable("phase_5")->setVal(truth_map["phase_5"]);
        pdf_signal_3D_dalitz->GetVariable("phase_6")->setVal(truth_map["phase_6"]);
        pdf_signal_3D_dalitz->GetVariable("phase_7")->setVal(truth_map["phase_7"]);
        pdf_signal_3D_dalitz->GetVariable("phase_8")->setVal(truth_map["phase_8"]);
        
        pdf_signal_3D_dalitz->GetVariable("amp_1_CC")->setVal(truth_map["amp_1_CC"]);
        pdf_signal_3D_dalitz->GetVariable("amp_2_CC")->setVal(truth_map["amp_2_CC"]);
        pdf_signal_3D_dalitz->GetVariable("amp_3_CC")->setVal(truth_map["amp_3_CC"]);
        pdf_signal_3D_dalitz->GetVariable("amp_4_CC")->setVal(truth_map["amp_4_CC"]);
        pdf_signal_3D_dalitz->GetVariable("amp_5_CC")->setVal(truth_map["amp_5_CC"]);
        pdf_signal_3D_dalitz->GetVariable("amp_6_CC")->setVal(truth_map["amp_6_CC"]);
        pdf_signal_3D_dalitz->GetVariable("amp_7_CC")->setVal(truth_map["amp_7_CC"]);
        pdf_signal_3D_dalitz->GetVariable("amp_8_CC")->setVal(truth_map["amp_8_CC"]);
        
        pdf_signal_3D_dalitz->GetVariable("phase_1_CC")->setVal(truth_map["phase_1_CC"]);
        pdf_signal_3D_dalitz->GetVariable("phase_2_CC")->setVal(truth_map["phase_2_CC"]);
        pdf_signal_3D_dalitz->GetVariable("phase_3_CC")->setVal(truth_map["phase_3_CC"]);
        pdf_signal_3D_dalitz->GetVariable("phase_4_CC")->setVal(truth_map["phase_4_CC"]);
        pdf_signal_3D_dalitz->GetVariable("phase_5_CC")->setVal(truth_map["phase_5_CC"]);
        pdf_signal_3D_dalitz->GetVariable("phase_6_CC")->setVal(truth_map["phase_6_CC"]);
        pdf_signal_3D_dalitz->GetVariable("phase_7_CC")->setVal(truth_map["phase_7_CC"]);
        pdf_signal_3D_dalitz->GetVariable("phase_8_CC")->setVal(truth_map["phase_8_CC"]);
        
        
        pdf_signal_3D_dalitz->GetVariable("amp_1")->setConstant(true);
        pdf_signal_3D_dalitz->GetVariable("phase_1")->setConstant(true);
        pdf_signal_3D_dalitz->GetVariable("phase_1_CC")->setConstant(true);
        
        
        shared_ptr<FF_PDF> pdf_signal_3D = make_shared<FF_PDF_signal_3D_mbc_deltae_nb_prime>(mbc, deltae, nb_prime, category_name_flags_channel);
        pdf_signal_3D->ImportPDFValues(opt->GetOutdir()+"pdf_signal_3D.dat",true);
        pdf_signal_3D->SetVariablesConst();
        //DEFINE SIGNAL PDF
        shared_ptr<FF_PDF> pdf_signal_6D = make_shared<FF_PDF_signal_6D>(pdf_signal_3D,pdf_signal_3D_dalitz,category_name_flags_channel);
        
        //DEFINE CONTINUUM PDF
        shared_ptr<FF_PDF> pdf_bkg_continuum_6D = make_shared<FF_PDF_bkg_continuum_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor>(mbc, deltae, nb_prime, mass_AB, mass_BC, flavor, category_name_flags_channel);
        pdf_bkg_continuum_6D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_continuum_6D.dat",true);
        pdf_bkg_continuum_6D->SetVariablesConst();
        
        //DEFINE CONTINUUM PDF
        shared_ptr<FF_PDF> pdf_bkg_combinatoric_6D = make_shared<FF_PDF_bkg_bbar_combinatoric_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor>(mbc, deltae, nb_prime, mass_AB, mass_BC, flavor, category_name_flags_channel);
        pdf_bkg_combinatoric_6D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_combinatoric_5D.dat",true);
        pdf_bkg_combinatoric_6D->SetVariablesConst();
        
        shared_ptr<FF_PDF> pdf_bkg_wrongmasshyp_6D = make_shared<FF_PDF_bkg_bbar_wrongmasshyp_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor>(mbc, deltae, nb_prime, mass_AB, mass_BC, flavor, category_name_flags_channel);
        pdf_bkg_wrongmasshyp_6D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongmasshyp_5D.dat",true);
        pdf_bkg_wrongmasshyp_6D->SetVariablesConst();
        
        shared_ptr<FF_PDF> pdf_bkg_wrongpi0hyp_6D = make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor>(mbc, deltae, nb_prime, mass_AB, mass_BC, flavor, category_name_flags_channel);
        pdf_bkg_wrongpi0hyp_6D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongpi0hyp_5D.dat",true);
        pdf_bkg_wrongpi0hyp_6D->SetVariablesConst();
        
        shared_ptr<FF_PDF> pdf_bkg_missingfsp_6D = make_shared<FF_PDF_bkg_bbar_missingfsp_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor>(mbc, deltae, nb_prime, mass_AB, mass_BC, flavor, category_name_flags_channel);
        pdf_bkg_missingfsp_6D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_missingfsp_5D.dat",true);
        pdf_bkg_missingfsp_6D->SetVariablesConst();
        
        shared_ptr<FF_PDF> pdf_bkg_nonres_6D = make_shared<FF_PDF_bkg_bbar_nonres_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor>(mbc, deltae, nb_prime, mass_AB, mass_BC, flavor, category_name_flags_channel);
        pdf_bkg_nonres_6D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_nonres_5D.dat",true);
        pdf_bkg_nonres_6D->SetVariablesConst();
        

        shared_ptr<FF_PDF> pdf_data_6D = make_shared<FF_PDF_data_6D__CPV>(pdf_signal_6D, pdf_bkg_continuum_6D, pdf_bkg_combinatoric_6D, pdf_bkg_wrongmasshyp_6D, pdf_bkg_wrongpi0hyp_6D, pdf_bkg_missingfsp_6D, pdf_bkg_nonres_6D, data.sumEntries(), category_name_flags_channel);
        
        
        
        std::array<std::string, 7> cuts;
        typedef std::pair<const std::string, double> pair_string_double;
        std::map<std::string, double> truth_values;
        
        // on MC we determine the truth and set better starting values
        cuts[0] = "((mctype<1001 || mctype>1004) && runno==0 && (mcinfo==59 || mcinfo==61) && abs(chil0_mo)==511 && abs(chil1_mo)==511 && abs(chil2_mo)==511 )";
        cuts[1] = "(mctype==1003 || mctype==1004)";
        cuts[2] = "(((mctype==1001 || mctype==1002) || ((mctype<1001 || mctype>1004) && runno==0 && mcinfo!=59 && mcinfo!=61)) && mchash==1)";
        cuts[3] = "(((mctype==1001 || mctype==1002) || ((mctype<1001 || mctype>1004) && runno==0 && mcinfo!=59 && mcinfo!=61)) && mchash!=1 && ((mcinfo & 0x8) ==0) && (((mcinfo & 0x4) !=0) || ((mcinfo & 0x2) !=0)) && mcinfo<64)";
        cuts[4] = "(((mctype==1001 || mctype==1002) || ((mctype<1001 || mctype>1004) && runno==0 && mcinfo!=59 && mcinfo!=61)) && mchash!=1 && mcinfo>64)";
        cuts[5] = "(((mctype==1001 || mctype==1002) || ((mctype<1001 || mctype>1004) && runno==0 && mcinfo!=59 && mcinfo!=61)) && mchash!=1 && !(((mcinfo & 0x8) ==0) && (((mcinfo & 0x4) !=0) || ((mcinfo & 0x2) !=0))) && mcinfo<64  && !(mchash==141183 ||mchash==2513873 ||mchash==2245944 ||mchash==2323258 ||mchash==2068777 ||mchash==1769521 ||mchash==1639018 ||mchash==3905351 ||mchash==2939915 ||mchash==1962594 ||mchash==3500850 ||mchash==1881338))";
        cuts[6] = "(((mctype==1001 || mctype==1002) || ((mctype<1001 || mctype>1004) && runno==0)) && (mcinfo==59 || mcinfo==61) && (mchash==141183 ||mchash==2513873 ||mchash==2245944 ||mchash==2323258 ||mchash==2068777 ||mchash==1769521 ||mchash==1639018 ||mchash==3905351 ||mchash==2939915 ||mchash==1962594 ||mchash==3500850 ||mchash==1881338))";
        
        
        for(unsigned int i = 0; i <= 6; i++) {
            RooDataSet* data_0 = dynamic_cast<RooDataSet*>(data.reduce(("channel==channel::C &&"+cuts[i]).c_str()));
            
            if(i==0) {
                pdf_data_6D->GetVariable("signal_nevents")->setVal(data_0->sumEntries());
                truth_values["signal_nevents"] = data_0->sumEntries();
            } else if(i==1) {
                pdf_data_6D->GetVariable("C_bkg_continuum_nevents")->setVal(data_0->sumEntries());
                truth_values["C_bkg_continuum_nevents"] = data_0->sumEntries();
                
            } else if(i==2) {
                pdf_data_6D->GetVariable("C_bkg_bbar_combinatoric_nevents")->setVal(data_0->sumEntries());
                truth_values["C_bkg_bbar_combinatoric_nevents"] = data_0->sumEntries();
            } else if(i==3) {
                pdf_data_6D->GetVariable("C_bkg_bbar_wrongmasshyp_nevents")->setVal(data_0->sumEntries());
                pdf_data_6D->GetVariable("C_bkg_bbar_wrongmasshyp_nevents")->setConstant(true);
                truth_values["C_bkg_bbar_wrongmasshyp_nevents"] = data_0->sumEntries();
            } else if(i==4) {
                pdf_data_6D->GetVariable("C_bkg_bbar_wrongpi0hyp_nevents")->setVal(data_0->sumEntries());
                pdf_data_6D->GetVariable("C_bkg_bbar_wrongpi0hyp_nevents")->setConstant(true);
                truth_values["C_bkg_bbar_wrongpi0hyp_nevents"] = data_0->sumEntries();
            } else if(i==5) {
                pdf_data_6D->GetVariable("C_bkg_bbar_missingfsp_nevents")->setVal(data_0->sumEntries());
                truth_values["C_bkg_bbar_missingfsp_nevents"] = data_0->sumEntries();
            } else if(i==6) {
                pdf_data_6D->GetVariable("C_bkg_bbar_nonres_nevents")->setVal(data_0->sumEntries());
                truth_values["C_bkg_bbar_nonres_nevents"] = data_0->sumEntries();
            }
            
            delete data_0;
        }
        
        
        
        PrintFitStartBlock("FITTING: 6D Fit");

        //RooFitResult *result_6D = pdf_data_6D->GetSimPDF().fitTo(data, SumW2Error(true), NumCPU(opt->GetNumcpu()), Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()), Extended(true));
        pdf_data_6D->GetSimPDF().getParameters(data)->Print("v");
        
        for(RooRealVar *v : variables_5D) {
            
            std::string varname = v->GetName();
            /*
            if (varname == "mbcp" || varname == "deltae" || varname == "nb_prime") {
                FFUtil::VisualizeFitResult(pdf_data_6D,data,*v,category_name_flags_channel,true,
                                           opt->GetBins(),"full_range", opt->GetOutdir()+"MC_6D_"+varname+"_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                           opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
            }
            
            if (varname == "mbcp") {
                FFUtil::VisualizeFitResult(pdf_data_6D,data,*v,category_name_flags_channel,true,
                                           opt->GetBins(),"full_range_SB1", opt->GetOutdir()+"MC_6D_"+varname+"_projection_signal_box."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                           opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
            }
            if (varname == "deltae") {
                FFUtil::VisualizeFitResult(pdf_data_6D,data,*v,category_name_flags_channel,true,
                                           opt->GetBins(),"full_range_SB2", opt->GetOutdir()+"MC_6D_"+varname+"_projection_signal_box."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                           opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
            }
            if (varname == "nb_prime") {
                FFUtil::VisualizeFitResult(pdf_data_6D,data,*v,category_name_flags_channel,true,
                                           opt->GetBins(),"full_range_SB3", opt->GetOutdir()+"MC_6D_"+varname+"_projection_signal_box."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                           opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
            }
            
            
            if (varname == "mass_01") {
                FFUtil::VisualizeFitResult(pdf_data_6D,data,*v,category_name_flags_channel,true,
                                           opt->GetBins(),"range_SB_1", opt->GetOutdir()+"MC_6D_"+varname+"_projection_range1."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                           opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                FFUtil::VisualizeFitResult(pdf_data_6D,data,*v,category_name_flags_channel,true,
                                           opt->GetBins(),"range_SB_2", opt->GetOutdir()+"MC_6D_"+varname+"_projection_range2."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                           opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                
                RooAbsReal::defaultIntegratorConfig()->method1D().setLabel("RooAdaptiveGaussKronrodIntegrator1D");
                FFUtil::VisualizeFitResult(pdf_data_6D,data,*v,category_name_flags_channel,true,
                                           opt->GetBins(),"range_SB_3", opt->GetOutdir()+"MC_6D_"+varname+"_projection_range3."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                           opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                FFUtil::VisualizeFitResult(pdf_data_6D,data,*v,category_name_flags_channel,true,
                                           opt->GetBins(),"range_SB_4", opt->GetOutdir()+"MC_6D_"+varname+"_projection_range4."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                           opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                RooAbsReal::defaultIntegratorConfig()->method1D().setLabel("RooGaussKronrodIntegrator1D");
                
            }
     */
            
            if (varname == "mass_12") {
     
                FFUtil::VisualizeFitResult(pdf_data_6D,data,*v,category_name_flags_channel,true,
                                           opt->GetBins(),"range_SB_5", opt->GetOutdir()+"MC_6D_"+varname+"_projection_range1."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                           opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                FFUtil::VisualizeFitResult(pdf_data_6D,data,*v,category_name_flags_channel,true,
                                           opt->GetBins(),"range_SB_6", opt->GetOutdir()+"MC_6D_"+varname+"_projection_range2."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                           opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                /*
                RooAbsReal::defaultIntegratorConfig()->method1D().setLabel("RooAdaptiveGaussKronrodIntegrator1D");
                FFUtil::VisualizeFitResult(pdf_data_6D,data,*v,category_name_flags_channel,true,
                                           opt->GetBins(),"range_SB_7", opt->GetOutdir()+"MC_6D_"+varname+"_projection_range3."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                           opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                FFUtil::VisualizeFitResult(pdf_data_6D,data,*v,category_name_flags_channel,true,
                                           opt->GetBins(),"range_SB_8", opt->GetOutdir()+"MC_6D_"+varname+"_projection_range4."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                           opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                RooAbsReal::defaultIntegratorConfig()->method1D().setLabel("RooGaussKronrodIntegrator1D");
                 */
            }
            
            
            
            /*
            if (varname == "mass_01" || varname == "mass_12") {
                FFUtil::VisualizeFitResult(pdf_data_6D,data,*v,category_name_flags_channel,true,
                                           opt->GetBins(),"full_range1", opt->GetOutdir()+"MC_6D_"+varname+"_projection_range1."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                           opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                FFUtil::VisualizeFitResult(pdf_data_6D,data,*v,category_name_flags_channel,true,
                                           opt->GetBins(),"full_range2", opt->GetOutdir()+"MC_6D_"+varname+"_projection_range2."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                           opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
                FFUtil::VisualizeFitResult(pdf_data_6D,data,*v,category_name_flags_channel,true,
                                           opt->GetBins(),"full_range3", opt->GetOutdir()+"MC_6D_"+varname+"_projection_range3."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                           opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
            }*/
        }
        //result_6D->Print();
        
        std::cout << "Signal-Yield/Signal-YieldError = " << pdf_data_6D->GetVariable("signal_nevents")->getVal()/pdf_data_6D->GetVariable("signal_nevents")->getError() << std::endl;
        std::cout << "Truth values for this fit:" << std::endl;
        for(pair_string_double& p : truth_values) {
            std::cout << mp::pair_to_string(p) << std::endl;
        }

        delete dalitz_analysis_imag_cache_integrator;
        delete dalitz_analysis_norm_cache_integrator;
        delete dalitz_analysis_real_cache_integrator;
        
        //delete result_6D;
    }
    
    void FitData(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt) {
        /*
         if(chain->GetEntries() == 0) {
         std::cout << "No data for final fits given, skip fitting." << std::endl;
         return;
         }
         
         // ---------------------------------------------
         // ----- Define variables
         // ---------------------------------------------
         PrintFitStartBlock("STATUS: Define functions and variables...");
         RooRealVar mbc("mbc","m_{bc}",5.24,5.29,"GeV");
         RooRealVar deltae("deltae","#Delta E",-0.15,0.15,"GeV");
         RooRealVar chil0_ma("chil0_ma","m_{KK}",0.985,1.05,"GeV");
         RooRealVar mass_kpi("mass_kpi","m_{K#pi}",0.7,1.55,"GeV");
         RooRealVar hel_phi("hel_phi","#Phi",-TMath::Pi(),TMath::Pi(),"rad");
         RooRealVar hel_th1("hel_th1","cos #Theta_{1}",-1.0,1.0,"");
         RooRealVar hel_th2("hel_th2","cos #Theta_{2}",-1.0,1.0,"");
         RooRealVar nb_prime("nb_prime","C_{NB}'",-10.0,10.0,"");
         RooCategory channel("channel","decay channel");
         channel.defineType("C",0);
         channel.defineType("N",1);
         RooCategory datatype("datatype","data sample type");
         datatype.defineType("CP",0);
         datatype.defineType("CA",1);
         datatype.defineType("N",2);
         datatype.defineType("CS",3);
         datatype.defineType("NS",4);
         datatype.defineType("COFF",5);
         datatype.defineType("NOFF",6);
         RooCategory flavor("flavor","flavor type");
         flavor.defineType("CP",0);
         flavor.defineType("CA",1);
         // necessary for MC information checks
         RooRealVar mcinfo("mcinfo","MC information",0,256);
         RooRealVar mctype("mctype","MC type information",-1,1005);
         RooRealVar chil0_mo("chil0_mo","MC #phi mother",-999999,999999);
         RooRealVar chil0_mc("chil0_mc","MC information #phi",0,256);
         RooRealVar runno("runno","Run Number",-999999,999999);
         // create data set (constructor only overload for max. 9 arguments, so we add some afterwards)
         RooArgSet varset(mbc,deltae,chil0_ma,mass_kpi,hel_phi,hel_th1,hel_th2,nb_prime);
         varset.add(channel);
         varset.add(datatype);
         varset.add(flavor);
         varset.add(mcinfo);
         varset.add(mctype);
         varset.add(chil0_mo);
         varset.add(chil0_mc);
         varset.add(runno);
         
         TCut cut_default_CP_and_CA = "(mbc < 5.289 && hel_th1 < 0.75 && (datatype==datatype::CP || datatype==datatype::CA))";
         TCut cut_default_CS = "(mbc < 5.289 && hel_th1 < 0.75 && (datatype==datatype::CS))";
         
         RooDataSet data("data","data",varset,Import(*chain.get()),Cut(cut_default_CP_and_CA));
         RooDataSet data_sideband("data_sideband","data_sideband",varset,Import(*chain.get()),Cut(cut_default_CS));
         
         // create helper for the simultaneous fit
         std::vector<std::string> flags_channel;
         flags_channel.push_back("C");
         flags_channel.push_back("N");
         tuple_category_name_flags category_name_flags_channel = std::make_tuple(&channel,"channel",flags_channel);
         
         std::vector<RooRealVar*> variables_8D;
         variables_8D.push_back(&mbc);
         variables_8D.push_back(&deltae);
         variables_8D.push_back(&chil0_ma);
         variables_8D.push_back(&nb_prime);
         variables_8D.push_back(&mass_kpi);
         variables_8D.push_back(&hel_phi);
         variables_8D.push_back(&hel_th1);
         variables_8D.push_back(&hel_th2);
         RooArgSet varset_9D(mbc,deltae,chil0_ma,nb_prime,mass_kpi,hel_phi,hel_th1,hel_th2,flavor);
         
         std::vector<RooRealVar*> variables_4D;
         variables_4D.push_back(&mass_kpi);
         variables_4D.push_back(&hel_phi);
         variables_4D.push_back(&hel_th1);
         variables_4D.push_back(&hel_th2);
         RooArgSet varset_5D(mbc,deltae,chil0_ma,nb_prime,flavor);
         
         // ---------------------------------------------
         // ----- Define full data range
         // ---------------------------------------------
         SetFitRanges(mbc,deltae,chil0_ma,mass_kpi,hel_phi,hel_th1,hel_th2,nb_prime);
         // ---------------------------------------------
         // ----- Create data component plots
         // ---------------------------------------------
         for(RooRealVar* var : variables_8D) {
         CreateDataComponentPlots(data_sideband,*var,opt->GetBins(),"full_range_SB",opt->GetOutdir(),opt->GetPlotFormat(),"CP","flavor","_sideband");
         CreateDataComponentPlots(data_sideband,*var,opt->GetBins(),"full_range_SB",opt->GetOutdir(),opt->GetPlotFormat(),"CA","flavor","_sideband");
         CreateDataComponentPlots(data_sideband,*var,opt->GetBins(),"full_range_SB",opt->GetOutdir(),opt->GetPlotFormat(),"CS","datatype","_sideband");
         }
         // ---------------------------------------------
         // ----- Do fits
         // ---------------------------------------------
         // See RooFit tutorial macro #501 about HowTo do a simultaneous fit
         // See RooFit tutorial macro #603 about HowTo do a multi-core parallelized unbinned maximum likelihood fit
         std::cout << "STATUS: Do fits..." << std::endl;
         
         PrintFitStartBlock("SETUP: CacheIntegrator");
         int debug_level = 1;
         CacheIntegrator<MassHelicityAnglesNormCachedIntegral>* mass_helicity_angles_norm_cache_integrator = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral>(debug_level);
         CacheIntegrator<MassHelicityAnglesRealCachedIntegral>* mass_helicity_angles_real_cache_integrator = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral>(debug_level);
         CacheIntegrator<MassHelicityAnglesImagCachedIntegral>* mass_helicity_angles_imag_cache_integrator = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral>(debug_level);
         CacheIntegrator<MassHelicityAnglesNormCachedIntegral_phi_th1_th2>* mass_helicity_angles_norm_cache_integrator_phi_th1_th2 = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral_phi_th1_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesRealCachedIntegral_phi_th1_th2>* mass_helicity_angles_real_cache_integrator_phi_th1_th2 = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral_phi_th1_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesImagCachedIntegral_phi_th1_th2>* mass_helicity_angles_imag_cache_integrator_phi_th1_th2 = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral_phi_th1_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_th1_th2>* mass_helicity_angles_norm_cache_integrator_m_th1_th2 = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_th1_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_th1_th2>* mass_helicity_angles_real_cache_integrator_m_th1_th2 = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_th1_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_th1_th2>* mass_helicity_angles_imag_cache_integrator_m_th1_th2 = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_th1_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_phi_th2>* mass_helicity_angles_norm_cache_integrator_m_phi_th2 = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_phi_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_phi_th2>* mass_helicity_angles_real_cache_integrator_m_phi_th2 = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_phi_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_phi_th2>* mass_helicity_angles_imag_cache_integrator_m_phi_th2 = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_phi_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_phi_th1>* mass_helicity_angles_norm_cache_integrator_m_phi_th1 = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_phi_th1>(debug_level);
         CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_phi_th1>* mass_helicity_angles_real_cache_integrator_m_phi_th1 = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_phi_th1>(debug_level);
         CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_phi_th1>* mass_helicity_angles_imag_cache_integrator_m_phi_th1 = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_phi_th1>(debug_level);
         
         ACCEPTANCE_FUNCTION acceptance = PRODUCT_4D;
         std::string path_to_integrals = "./CachedIntegrals_ACCEPTANCE" + FFUtil::GetAcceptanceString(acceptance) + "/";
         mass_helicity_angles_norm_cache_integrator->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_real_cache_integrator->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_imag_cache_integrator->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_norm_cache_integrator_phi_th1_th2->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_real_cache_integrator_phi_th1_th2->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_imag_cache_integrator_phi_th1_th2->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_norm_cache_integrator_m_th1_th2->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_real_cache_integrator_m_th1_th2->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_imag_cache_integrator_m_th1_th2->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_norm_cache_integrator_m_phi_th2->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_real_cache_integrator_m_phi_th2->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_imag_cache_integrator_m_phi_th2->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_norm_cache_integrator_m_phi_th1->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_real_cache_integrator_m_phi_th1->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_imag_cache_integrator_m_phi_th1->LoadIntegrals(path_to_integrals);
         
         MHI_PDG_constants mhi_pdg_constants = {m_Pi, m_K, m_Phi, m_B0};
         MHI_CacheIntegrators_4D mhi_4D = {
         mass_helicity_angles_norm_cache_integrator,
         mass_helicity_angles_real_cache_integrator,
         mass_helicity_angles_imag_cache_integrator
         };
         MHI_CacheIntegrators_3D_phi_th1_th2 mhi_3D_phi_th1_th2 = {
         mass_helicity_angles_norm_cache_integrator_phi_th1_th2,
         mass_helicity_angles_real_cache_integrator_phi_th1_th2,
         mass_helicity_angles_imag_cache_integrator_phi_th1_th2
         };
         MHI_CacheIntegrators_3D_m_th1_th2 mhi_3D_m_th1_th2 = {
         mass_helicity_angles_norm_cache_integrator_m_th1_th2,
         mass_helicity_angles_real_cache_integrator_m_th1_th2,
         mass_helicity_angles_imag_cache_integrator_m_th1_th2
         };
         MHI_CacheIntegrators_3D_m_phi_th2 mhi_3D_m_phi_th2 = {
         mass_helicity_angles_norm_cache_integrator_m_phi_th2,
         mass_helicity_angles_real_cache_integrator_m_phi_th2,
         mass_helicity_angles_imag_cache_integrator_m_phi_th2
         };
         MHI_CacheIntegrators_3D_m_phi_th1 mhi_3D_m_phi_th1 = {
         mass_helicity_angles_norm_cache_integrator_m_phi_th1,
         mass_helicity_angles_real_cache_integrator_m_phi_th1,
         mass_helicity_angles_imag_cache_integrator_m_phi_th1
         };
         
         for(RooRealVar *v : variables_8D) {
         CreateDataComponentPlots(data,*v,opt->GetBins(),"full_range",opt->GetOutdir(),opt->GetPlotFormat(),"CP","flavor","");
         CreateDataComponentPlots(data,*v,opt->GetBins(),"full_range",opt->GetOutdir(),opt->GetPlotFormat(),"CA","flavor","");
         CreateDataComponentPlots(data,*v,opt->GetBins(),"full_range",opt->GetOutdir(),opt->GetPlotFormat(),"C","channel","");
         }
         
         PrintFitStartBlock("SETUP: 9D Fit");
         
         shared_ptr<FF_PDF> pdf_bkg_continuum_9D = make_shared<FF_PDF_bkg_continuum_9D_mbc_deltae_chil0_ma_mass_kpi_hel_phi_hel_th1_hel_th2_nb_prime_flavor>(mbc,
         deltae, chil0_ma, mass_kpi, hel_phi, hel_th1,
         hel_th2, nb_prime, flavor, category_name_flags_channel,true,(RooRealVar*)NULL);
         pdf_bkg_continuum_9D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_continuum_8D.dat",true);
         pdf_bkg_continuum_9D->GetVariable("N_chil0_ma_bkg_continuum_nonres_fraction")->setConstant(true);
         pdf_bkg_continuum_9D->GetVariable("N_hel_th1_bkg_continuum_a1_polyvar_mass_kpi_c0")->setConstant(true);
         pdf_bkg_continuum_9D->GetVariable("N_hel_th1_bkg_continuum_a2_polyvar_mass_kpi_c1")->setConstant(true);
         pdf_bkg_continuum_9D->GetVariable("N_hel_th1_bkg_continuum_a3_polyvar_mass_kpi_c0")->setConstant(true);
         pdf_bkg_continuum_9D->GetVariable("N_mass_kpi_bkg_continuum_a1")->setConstant(true);
         pdf_bkg_continuum_9D->GetVariable("N_mass_kpi_bkg_continuum_a2")->setConstant(true);
         pdf_bkg_continuum_9D->GetVariable("N_mass_kpi_bkg_continuum_a3")->setConstant(true);
         pdf_bkg_continuum_9D->GetVariable("N_mass_kpi_bkg_continuum_nonres_fraction")->setConstant(true);
         //pdf_bkg_continuum_9D->SetVariablesConst();
         shared_ptr<FF_PDF> pdf_signal_4D = make_shared<FF_PDF_signal_4D_mbc_deltae_chil0_ma_nb_prime>(mbc,
         deltae, chil0_ma, nb_prime, category_name_flags_channel,true,pdf_bkg_continuum_9D->GetVariable("chil0_ma_resolution"));
         pdf_signal_4D->ImportPDFValues(opt->GetOutdir()+"pdf_signal_4D.dat",true);
         // scale Delta E resolution
         double scale = 1.124;
         pdf_signal_4D->GetVariable("C_deltae_signal_width_core")->setVal(pdf_signal_4D->GetVariable("C_deltae_signal_width_core")->getVal()*scale);
         pdf_signal_4D->GetVariable("C_deltae_signal_width_1st_left")->setVal(pdf_signal_4D->GetVariable("C_deltae_signal_width_1st_left")->getVal()*scale);
         pdf_signal_4D->GetVariable("C_deltae_signal_width_1st_right")->setVal(pdf_signal_4D->GetVariable("C_deltae_signal_width_1st_right")->getVal()*scale);
         pdf_signal_4D->GetVariable("C_deltae_signal_width_2nd_left")->setVal(pdf_signal_4D->GetVariable("C_deltae_signal_width_2nd_left")->getVal()*scale);
         pdf_signal_4D->GetVariable("C_deltae_signal_width_2nd_right")->setVal(pdf_signal_4D->GetVariable("C_deltae_signal_width_2nd_right")->getVal()*scale);
         pdf_signal_4D->SetVariablesConst();
         //pdf_signal_4D->GetVariable("C_deltae_signal_width_core")->setConstant(false);
         shared_ptr<FF_PDF> pdf_signal_5D_angles = make_shared<FF_PDF_signal_5D_mass_helicity_angles_CPV>(mass_kpi,
         hel_th1, hel_th2, hel_phi, flavor, acceptance,
         mhi_pdg_constants, mhi_4D,
         mhi_3D_phi_th1_th2, mhi_3D_m_th1_th2, mhi_3D_m_phi_th2, mhi_3D_m_phi_th1,
         category_name_flags_channel);
         shared_ptr<FF_PDF> pdf_signal_9D = make_shared<FF_PDF_signal_9D>(pdf_signal_4D, pdf_signal_5D_angles,
         category_name_flags_channel);
         shared_ptr<FF_PDF> pdf_bkg_bbar_wrongphikkbar_9D = make_shared<FF_PDF_bkg_bbar_wrongphikkbar_9D>(mbc,
         deltae, chil0_ma, nb_prime, mass_kpi,
         hel_phi, hel_th1, hel_th2, flavor, category_name_flags_channel, true);
         pdf_bkg_bbar_wrongphikkbar_9D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongphikkbar_8D.dat",true);
         pdf_bkg_bbar_wrongphikkbar_9D->SetVariablesConst();
         
         shared_ptr<FF_PDF> pdf_data_9D = make_shared<FF_PDF_data_9D__CPV>(pdf_signal_9D,
         pdf_bkg_continuum_9D, pdf_bkg_bbar_wrongphikkbar_9D,
         data.sumEntries(), category_name_flags_channel);
         
         PrintFitStartBlock("FITTING: 9D Sideband Fit");
         
         // fit the continuum pdf to sideband data and fix the shape
         mbc.setMin(5.24);
         mbc.setMax(5.26);
         mbc.setRange("full_range",5.24,5.26);
         
         // fit the 1D mass_kpi distribution due to some instability
         shared_ptr<FF_PDF> pdf_bkg_continuum_1D_mass_kpi = make_shared<FF_PDF_bkg_continuum_1D_mass_kpi>(mass_kpi, category_name_flags_channel, false);
         pdf_bkg_continuum_1D_mass_kpi->GetVariable("N_mass_kpi_bkg_continuum_a1")->setConstant(true);
         pdf_bkg_continuum_1D_mass_kpi->GetVariable("N_mass_kpi_bkg_continuum_a2")->setConstant(true);
         pdf_bkg_continuum_1D_mass_kpi->GetVariable("N_mass_kpi_bkg_continuum_a3")->setConstant(true);
         pdf_bkg_continuum_1D_mass_kpi->GetVariable("N_mass_kpi_bkg_continuum_nonres_fraction")->setConstant(true);
         RooFitResult *result_mass_kpi = pdf_bkg_continuum_1D_mass_kpi->GetSimPDF().fitTo(data_sideband, SumW2Error(false), NumCPU(opt->GetNumcpu()),
         Timer(true), Save(true), InitialHesse(false), Minos(false), Extended(false));
         pdf_bkg_continuum_1D_mass_kpi->GetSimPDF().getParameters(data_sideband)->Print("v");
         WriteCorrelationMatrixFile(result_mass_kpi,opt->GetOutdir()+"pdf_data_1D_mass_kpi_correlation.dat");
         pdf_bkg_continuum_1D_mass_kpi->ExportPDFValues(opt->GetOutdir()+"pdf_data_1D_mass_kpi.dat",true);
         result_mass_kpi->Print();
         // set mass_kpi distribution and fix it
         for(RooRealVar *v : pdf_bkg_continuum_1D_mass_kpi->GetVariables()) {
         pdf_bkg_continuum_9D->SetVariable(v->GetName(),v->getVal(),v->getError(),true,false);
         }
         
         RooFitResult *result_9D_sideband = pdf_bkg_continuum_9D->GetSimPDF().fitTo(data_sideband, SumW2Error(false), NumCPU(opt->GetNumcpu()),
         Timer(true), Save(true), InitialHesse(false), Minos(false), Extended(false));
         pdf_bkg_continuum_9D->GetSimPDF().getParameters(data_sideband)->Print("v");
         
         if(opt->GetCreatePlots()) {
         for(RooRealVar *v : variables_8D) {
         std::string varname = v->GetName();
         
         PrintFitStartBlock("PLOT PROJECTION");
         FFUtil::VisualizeFitResult(pdf_bkg_continuum_9D,data_sideband,*v,category_name_flags_channel,false,
         opt->GetBins(),"full_range", opt->GetOutdir()+"DATA_9D_sideband_"+varname+"_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         }
         WriteCorrelationMatrixFile(result_9D_sideband,opt->GetOutdir()+"pdf_data_9D_sideband_correlation.dat");
         pdf_bkg_continuum_9D->ExportPDFValues(opt->GetOutdir()+"pdf_data_9D_sideband.dat",true);
         
         // draw projections in 5 mass_kpi regions
         for(unsigned int i = 1; i <= 5; ++i) {
         mbc.setRange(("mass_kpi_bin_"+mp::to_string(i)).c_str(),5.24,5.26);
         FFUtil::VisualizeFitResult(pdf_bkg_continuum_9D,data_sideband,hel_th1,category_name_flags_channel,false,
         opt->GetBins(),"mass_kpi_bin_"+mp::to_string(i), opt->GetOutdir()+"DATA_9D_sideband_hel_th1_projection_in_mass_kpi_bin_"+mp::to_string(i)+"."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         }
         WriteCorrelationMatrixFile(result_9D_sideband,opt->GetOutdir()+"pdf_data_9D_sideband_correlation.dat");
         pdf_bkg_continuum_9D->ExportPDFValues(opt->GetOutdir()+"pdf_data_9D_sideband.dat",true);
         }
         result_9D_sideband->Print();
         
         // fit the fit region data with fixed pdf
         mbc.setMax(5.29);
         mbc.setMin(5.26);
         mbc.setRange("full_range",5.26,5.29);
         pdf_bkg_continuum_9D->SetVariablesConst();
         pdf_bkg_continuum_9D->GetVariable("mbc_bkg_continuum_c")->setConstant(false);
         
         std::vector<std::string> parameters;
         parameters.push_back("signal_nevents");
         parameters.push_back("C_bkg_continuum_nevents");
         parameters.push_back("C_bkg_bbar_wrongphikkbar_nevents");
         parameters.push_back("mbc_bkg_continuum_c");
         parameters.push_back("amp_00");
         //parameters.push_back("amp_10");
         parameters.push_back("amp_1para");
         parameters.push_back("amp_1perp");
         parameters.push_back("amp_20");
         parameters.push_back("amp_2para");
         parameters.push_back("amp_2perp");
         parameters.push_back("phase_10");
         parameters.push_back("phase_1para");
         parameters.push_back("phase_1perp");
         parameters.push_back("phase_20");
         parameters.push_back("phase_2para");
         parameters.push_back("phase_2perp");
         parameters.push_back("amp_00_delta");
         parameters.push_back("amp_10_delta");
         parameters.push_back("amp_1para_delta");
         parameters.push_back("amp_1perp_delta");
         parameters.push_back("amp_20_delta");
         if(!opt->GetFixCPV2PerpAndParaComponent()) {
         parameters.push_back("amp_2para_delta");
         parameters.push_back("amp_2perp_delta");
         }
         parameters.push_back("phase_10_delta");
         parameters.push_back("phase_1para_delta");
         parameters.push_back("phase_1perp_delta");
         parameters.push_back("phase_20_delta");
         if(!opt->GetFixCPV2PerpAndParaComponent()) {
         parameters.push_back("phase_2para_delta");
         parameters.push_back("phase_2perp_delta");
         }
         
         PrintFitStartBlock("REPEATING 9D FITS");
         std::vector<RooFitResult*> fit_results;
         std::string outdir_files = opt->GetOutdir()+"/data_fit_"+mp::to_string(opt->GetNGsimToys())+(opt->GetMinos() ? "_minos" : "")+"/";
         mp::execute_command("mkdir -p "+outdir_files);
         // start loop of fits
         for(int i = 1; i <= 50; ++i) {
         std::stringstream ss;
         ss << "sample_" << i;
         std::cout << "INFO: Toy Sample: " << i << std::endl;
         
         // reset to fit start values
         pdf_data_9D->GetVariable("amp_00")->setVal(0.75);
         pdf_data_9D->GetVariable("amp_10")->setVal(0.707);
         pdf_data_9D->GetVariable("amp_1para")->setVal(0.5);
         pdf_data_9D->GetVariable("amp_1perp")->setVal(0.5);
         pdf_data_9D->GetVariable("amp_20")->setVal(0.707*0.8);
         pdf_data_9D->GetVariable("amp_2para")->setVal(0.5*0.8);
         pdf_data_9D->GetVariable("amp_2perp")->setVal(0.5*0.8);
         
         pdf_data_9D->GetVariable("phase_10")->setVal(RooRandom::randomGenerator()->Uniform(-TMath::Pi(),TMath::Pi()));
         pdf_data_9D->GetVariable("phase_1para")->setVal(RooRandom::randomGenerator()->Uniform(-TMath::Pi(),TMath::Pi()));
         pdf_data_9D->GetVariable("phase_1perp")->setVal(RooRandom::randomGenerator()->Uniform(-TMath::Pi(),TMath::Pi()));
         pdf_data_9D->GetVariable("phase_20")->setVal(RooRandom::randomGenerator()->Uniform(-TMath::Pi(),TMath::Pi()));
         pdf_data_9D->GetVariable("phase_2para")->setVal(RooRandom::randomGenerator()->Uniform(-TMath::Pi(),TMath::Pi()));
         pdf_data_9D->GetVariable("phase_2perp")->setVal(RooRandom::randomGenerator()->Uniform(-TMath::Pi(),TMath::Pi()));
         
         pdf_data_9D->GetVariable("amp_00_delta")->setVal(0.0);
         pdf_data_9D->GetVariable("amp_10_delta")->setVal(0.0);
         pdf_data_9D->GetVariable("amp_1para_delta")->setVal(0.0);
         pdf_data_9D->GetVariable("amp_1perp_delta")->setVal(0.0);
         pdf_data_9D->GetVariable("amp_20_delta")->setVal(0.0);
         pdf_data_9D->GetVariable("amp_2para_delta")->setVal(0.0);
         pdf_data_9D->GetVariable("amp_2perp_delta")->setVal(0.0);
         
         pdf_data_9D->GetVariable("phase_10_delta")->setVal(0.0);
         pdf_data_9D->GetVariable("phase_1para_delta")->setVal(0.0);
         pdf_data_9D->GetVariable("phase_1perp_delta")->setVal(TMath::Pi()/2);
         pdf_data_9D->GetVariable("phase_20_delta")->setVal(0.0);
         pdf_data_9D->GetVariable("phase_2para_delta")->setVal(0.0);
         pdf_data_9D->GetVariable("phase_2perp_delta")->setVal(TMath::Pi()/2);
         
         // fix to no CPV for first iteration
         pdf_data_9D->GetVariable("amp_00_delta")->setConstant(true);
         pdf_data_9D->GetVariable("amp_10_delta")->setConstant(true);
         pdf_data_9D->GetVariable("amp_1para_delta")->setConstant(true);
         pdf_data_9D->GetVariable("amp_1perp_delta")->setConstant(true);
         pdf_data_9D->GetVariable("amp_20_delta")->setConstant(true);
         pdf_data_9D->GetVariable("amp_2para_delta")->setConstant(true);
         pdf_data_9D->GetVariable("amp_2perp_delta")->setConstant(true);
         
         pdf_data_9D->GetVariable("phase_10_delta")->setConstant(true);
         pdf_data_9D->GetVariable("phase_1para_delta")->setConstant(true);
         pdf_data_9D->GetVariable("phase_1perp_delta")->setConstant(true);
         pdf_data_9D->GetVariable("phase_20_delta")->setConstant(true);
         pdf_data_9D->GetVariable("phase_2para_delta")->setConstant(true);
         pdf_data_9D->GetVariable("phase_2perp_delta")->setConstant(true);
         
         RooFitResult* result_noCPV = pdf_data_9D->GetSimPDF().fitTo(data, SumW2Error(false), NumCPU(opt->GetNumcpu()),
         Timer(true), Save(true), InitialHesse(false), Minos(false), Extended(true),
         Constrain(RooArgSet(*pdf_data_9D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents"))));
         result_noCPV->Print();
         delete result_noCPV;
         
         // check if any phase is outside the [-pi,pi] region and push it back towards truth value
         // this is only to get good pulls, the other solution is not wrong
         ShiftPhaseToInterval(pdf_data_9D, "phase_10", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phase_1para", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phase_1perp", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phase_20", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phase_2para", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phase_2perp", 0.0);
         
         // now allow for CPV
         pdf_data_9D->GetVariable("amp_00_delta")->setConstant(false);
         pdf_data_9D->GetVariable("amp_10_delta")->setConstant(false);
         pdf_data_9D->GetVariable("amp_1para_delta")->setConstant(false);
         pdf_data_9D->GetVariable("amp_1perp_delta")->setConstant(false);
         pdf_data_9D->GetVariable("amp_20_delta")->setConstant(false);
         if(!opt->GetFixCPV2PerpAndParaComponent()) {
         pdf_data_9D->GetVariable("amp_2para_delta")->setConstant(false);
         pdf_data_9D->GetVariable("amp_2perp_delta")->setConstant(false);
         }
         
         pdf_data_9D->GetVariable("phase_10_delta")->setConstant(false);
         pdf_data_9D->GetVariable("phase_1para_delta")->setConstant(false);
         pdf_data_9D->GetVariable("phase_1perp_delta")->setConstant(false);
         pdf_data_9D->GetVariable("phase_20_delta")->setConstant(false);
         if(!opt->GetFixCPV2PerpAndParaComponent()) {
         pdf_data_9D->GetVariable("phase_2para_delta")->setConstant(false);
         pdf_data_9D->GetVariable("phase_2perp_delta")->setConstant(false);
         }
         
         fit_results.push_back(pdf_data_9D->GetSimPDF().fitTo(data, SumW2Error(false), NumCPU(opt->GetNumcpu()),
         Timer(true), Save(true), InitialHesse(false), Minos(false), Extended(true),
         Constrain(RooArgSet(*pdf_data_9D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents")))));
         fit_results.back()->Print();
         
         // check if any phase is outside the [-pi,pi] region and push it back towards truth value
         // this is only to get good pulls, the other solution is not wrong
         ShiftPhaseToInterval(pdf_data_9D, "phase_10", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phase_1para", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phase_1perp", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phase_20", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phase_2para", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phase_2perp", 0.0);
         
         PrintFitResultLatex(pdf_data_9D);
         std::cout << mp::map_to_string(ComputePhysicsParameters(pdf_data_9D,pdf_signal_9D,varset_9D,false)) << "/n";
         //PrintPhysicsParametersLatex(PropagateErrors(pdf_data_9D,pdf_signal_9D,fit_results.back(),opt,varset_9D,false));
         
         std::ofstream fout((outdir_files+"DATA_9D_fit_result_"+ss.str().c_str()+".dat").c_str());
         if(fout.is_open()) {
         for(const std::string& p : parameters) {
         fout << p << ";";
         fout << 0.0 << ";";
         fout << pdf_data_9D->GetVariable(p)->getVal() << ";";
         fout << pdf_data_9D->GetVariable(p)->getError() << ";";
         fout << pdf_data_9D->GetVariable(p)->getErrorLo() << ";";
         fout << pdf_data_9D->GetVariable(p)->getErrorHi() << std::endl;
         }
         fout << "EDM;" << fit_results.back()->edm() << std::endl;
         fout << "NLL;" << fit_results.back()->minNll() << std::endl;
         fout.close();
         } else {
         std::cerr << "ERROR: Could not write fit result to file" << std::endl;
         }
         }
         // End loop of fits
         
         // search best fit result and set parameters
         RooFitResult* result_9D_best = fit_results.front();
         int result_number = 0;
         int best_result = 0;
         for(RooFitResult* r : fit_results) {
         ++result_number;
         if(r->minNll() < result_9D_best->minNll()) {
         std::cout << "INFO: Sample " << result_number << " has better NLL " << r->minNll() << std::endl;
         result_9D_best = r;
         best_result = result_number;
         } else {
         if(r->minNll() == result_9D_best->minNll()) {
         std::cout << "INFO: Sample " << result_number << " has equal NLL " << r->minNll() << std::endl;
         } else {
         std::cout << "INFO: Sample " << result_number << " has worse NLL " << r->minNll() << std::endl;
         }
         }
         }
         std::cout << "INFO: Sample " << best_result << " had best NLL " << result_9D_best->minNll() << std::endl;
         std::cout << "INFO: Resetting fit parameters now..." << std::endl;
         const RooArgList& floatParsFinal = result_9D_best->floatParsFinal();
         for(const std::string& p : parameters) {
         int index = floatParsFinal.index(p.c_str());
         RooRealVar& result = dynamic_cast<RooRealVar&>(floatParsFinal[index]);
         std::cout << "Setting " << p << " to value " << result.getVal() << std::endl;
         pdf_data_9D->GetVariable(p)->setVal(result.getVal());
         //pdf_data_9D->GetVariable(p)->setError(result.getError());
         }
         // redo fit with MINOS error estimation
         RooFitResult* redo_result_9D_best = pdf_data_9D->GetSimPDF().fitTo(data, SumW2Error(false), NumCPU(opt->GetNumcpu()),
         Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()), Extended(true),
         Constrain(RooArgSet(*pdf_data_9D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents"))));
         redo_result_9D_best->Print();
         redo_result_9D_best->correlationMatrix().Print();
         redo_result_9D_best->covarianceMatrix().Print();
         WriteCorrelationMatrixFile(redo_result_9D_best,opt->GetOutdir()+"pdf_data_9D_correlation.dat");
         WriteCovarianceMatrixFile(redo_result_9D_best,opt->GetOutdir()+"pdf_data_9D_covariance.dat");
         
         // check if any phase is outside the [-pi,pi] region and push it back towards truth value
         // this is only to get good pulls, the other solution is not wrong
         ShiftPhaseToInterval(pdf_data_9D, "phase_10", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phase_1para", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phase_1perp", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phase_20", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phase_2para", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phase_2perp", 0.0);
         
         PrintFitResultLatex(pdf_data_9D);
         PrintPhysicsParametersLatex(PropagateErrors(pdf_data_9D,pdf_signal_9D,redo_result_9D_best,opt,varset_9D,false));
         pdf_data_9D->ExportPDFValues(opt->GetOutdir()+"pdf_data_9D.dat",true);
         
         // store best fit result
         std::ofstream fout((outdir_files+"DATA_9D_best_fit_result.dat").c_str());
         if(fout.is_open()) {
         for(const std::string& p : parameters) {
         fout << p << ";";
         fout << 0.0 << ";";
         fout << pdf_data_9D->GetVariable(p)->getVal() << ";";
         fout << pdf_data_9D->GetVariable(p)->getError() << ";";
         fout << pdf_data_9D->GetVariable(p)->getErrorLo() << ";";
         fout << pdf_data_9D->GetVariable(p)->getErrorHi() << std::endl;
         }
         fout << "EDM;" << redo_result_9D_best->edm() << std::endl;
         fout << "NLL;" << redo_result_9D_best->minNll() << std::endl;
         fout.close();
         } else {
         std::cerr << "ERROR: Could not write fit result to file" << std::endl;
         }
         
         // plot best fit result
         if(opt->GetCreatePlots()) {
         for(RooRealVar *v : variables_8D) {
         std::string varname = v->GetName();
         
         PrintFitStartBlock("PLOT PROJECTION");
         FFUtil::VisualizeFitResult(pdf_data_9D,data,*v,category_name_flags_channel,true,
         opt->GetBins(),"full_range", opt->GetOutdir()+"DATA_9D_"+varname+"_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         if(varname == "mbc" || varname == "deltae" || varname == "chil0_ma" || varname == "nb_prime") {
         FFUtil::VisualizeFitResult(pdf_data_9D,data,*v,category_name_flags_channel,true,
         opt->GetBins(),("signal_box_"+varname).c_str(),opt->GetOutdir()+"DATA_9D_"+varname+"_projection_signal_box."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         } else {
         FFUtil::VisualizeFitResult(pdf_data_9D,data,*v,category_name_flags_channel,true,
         opt->GetBins(),"signal_box",opt->GetOutdir()+"DATA_9D_"+varname+"_projection_signal_box."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         }
         if(varname != "mbc") {
         FFUtil::VisualizeFitResult(pdf_data_9D,data,*v,category_name_flags_channel,true,
         opt->GetBins(),"mbc_sideband", opt->GetOutdir()+"DATA_9D_"+varname+"_projection_mbc_sideband."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         }
         }
         }
         redo_result_9D_best->Print();
         
         std::cout << "Signal-Yield/Signal-YieldError = " << pdf_data_9D->GetVariable("signal_nevents")->getVal()/pdf_data_9D->GetVariable("signal_nevents")->getError() << std::endl;
         
         mass_helicity_angles_norm_cache_integrator->SaveIntegrals(path_to_integrals);
         mass_helicity_angles_real_cache_integrator->SaveIntegrals(path_to_integrals);
         mass_helicity_angles_imag_cache_integrator->SaveIntegrals(path_to_integrals);
         mass_helicity_angles_norm_cache_integrator_phi_th1_th2->SaveIntegrals(path_to_integrals);
         mass_helicity_angles_real_cache_integrator_phi_th1_th2->SaveIntegrals(path_to_integrals);
         mass_helicity_angles_imag_cache_integrator_phi_th1_th2->SaveIntegrals(path_to_integrals);
         mass_helicity_angles_norm_cache_integrator_m_th1_th2->SaveIntegrals(path_to_integrals);
         mass_helicity_angles_real_cache_integrator_m_th1_th2->SaveIntegrals(path_to_integrals);
         mass_helicity_angles_imag_cache_integrator_m_th1_th2->SaveIntegrals(path_to_integrals);
         mass_helicity_angles_norm_cache_integrator_m_phi_th2->SaveIntegrals(path_to_integrals);
         mass_helicity_angles_real_cache_integrator_m_phi_th2->SaveIntegrals(path_to_integrals);
         mass_helicity_angles_imag_cache_integrator_m_phi_th2->SaveIntegrals(path_to_integrals);
         mass_helicity_angles_norm_cache_integrator_m_phi_th1->SaveIntegrals(path_to_integrals);
         mass_helicity_angles_real_cache_integrator_m_phi_th1->SaveIntegrals(path_to_integrals);
         mass_helicity_angles_imag_cache_integrator_m_phi_th1->SaveIntegrals(path_to_integrals);
         
         delete mass_helicity_angles_imag_cache_integrator;
         delete mass_helicity_angles_imag_cache_integrator_m_phi_th1;
         delete mass_helicity_angles_imag_cache_integrator_m_phi_th2;
         delete mass_helicity_angles_imag_cache_integrator_m_th1_th2;
         delete mass_helicity_angles_imag_cache_integrator_phi_th1_th2;
         delete mass_helicity_angles_norm_cache_integrator;
         delete mass_helicity_angles_norm_cache_integrator_m_phi_th1;
         delete mass_helicity_angles_norm_cache_integrator_m_phi_th2;
         delete mass_helicity_angles_norm_cache_integrator_m_th1_th2;
         delete mass_helicity_angles_norm_cache_integrator_phi_th1_th2;
         delete mass_helicity_angles_real_cache_integrator;
         delete mass_helicity_angles_real_cache_integrator_m_phi_th1;
         delete mass_helicity_angles_real_cache_integrator_m_phi_th2;
         delete mass_helicity_angles_real_cache_integrator_m_th1_th2;
         delete mass_helicity_angles_real_cache_integrator_phi_th1_th2;
         
         for(RooFitResult* r : fit_results) {
         delete r;
         }
         delete redo_result_9D_best;
         delete result_9D_sideband;
         delete result_mass_kpi;*/
    }
    
    void FitDataWithStartingValues(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt, int data_pdf_mode) {
        /*
         if(chain->GetEntries() == 0) {
         std::cout << "No data for final fits given, skip fitting." << std::endl;
         return;
         }
         
         // ---------------------------------------------
         // ----- Parse configuration
         // ---------------------------------------------
         std::cout << "Reading fit configuration from " << opt->GetConfigFile() << std::endl;
         namespace bpt = boost::property_tree;
         bpt::ptree pt;
         bpt::info_parser::read_info(opt->GetConfigFile(),pt);
         
         // ---------------------------------------------
         // ----- Define variables
         // ---------------------------------------------
         PrintFitStartBlock("STATUS: Define functions and variables...");
         RooRealVar mbc("mbc","m_{bc}",5.24,5.29,"GeV");
         RooRealVar deltae("deltae","#Delta E",-0.15,0.15,"GeV");
         RooRealVar chil0_ma("chil0_ma","m_{KK}",0.985,1.05,"GeV");
         RooRealVar mass_kpi("mass_kpi","m_{K#pi}",0.7,1.55,"GeV");
         RooRealVar hel_phi("hel_phi","#Phi",-TMath::Pi(),TMath::Pi(),"rad");
         RooRealVar hel_th1("hel_th1","cos #Theta_{1}",-1.0,1.0,"");
         RooRealVar hel_th2("hel_th2","cos #Theta_{2}",-1.0,1.0,"");
         RooRealVar nb_prime("nb_prime","C_{NB}'",-10.0,10.0,"");
         RooCategory channel("channel","decay channel");
         channel.defineType("C",0);
         channel.defineType("N",1);
         RooCategory datatype("datatype","data sample type");
         datatype.defineType("CP",0);
         datatype.defineType("CA",1);
         datatype.defineType("N",2);
         datatype.defineType("CS",3);
         datatype.defineType("NS",4);
         datatype.defineType("COFF",5);
         datatype.defineType("NOFF",6);
         RooCategory flavor("flavor","flavor type");
         flavor.defineType("CP",0);
         flavor.defineType("CA",1);
         // necessary for MC information checks
         RooRealVar mcinfo("mcinfo","MC information",0,256);
         RooRealVar mctype("mctype","MC type information",-1,1005);
         RooRealVar chil0_mo("chil0_mo","MC #phi mother",-999999,999999);
         RooRealVar chil0_mc("chil0_mc","MC information #phi",0,256);
         RooRealVar runno("runno","Run Number",-999999,999999);
         // create data set (constructor only overload for max. 9 arguments, so we add some afterwards)
         RooArgSet varset(mbc,deltae,chil0_ma,mass_kpi,hel_phi,hel_th1,hel_th2,nb_prime);
         varset.add(channel);
         varset.add(datatype);
         varset.add(flavor);
         varset.add(mcinfo);
         varset.add(mctype);
         varset.add(chil0_mo);
         varset.add(chil0_mc);
         varset.add(runno);
         
         TCut cut_default_CP_and_CA = "(mbc < 5.289 && hel_th1 < 0.75 && (datatype==datatype::CP || datatype==datatype::CA))";
         TCut cut_default_CS = "(mbc < 5.289 && hel_th1 < 0.75 && (datatype==datatype::CS))";
         
         RooDataSet data("data","data",varset,Import(*chain.get()),Cut(cut_default_CP_and_CA));
         RooDataSet data_sideband("data_sideband","data_sideband",varset,Import(*chain.get()),Cut(cut_default_CS));
         
         // create helper for the simultaneous fit
         std::vector<std::string> flags_channel;
         flags_channel.push_back("C");
         flags_channel.push_back("N");
         tuple_category_name_flags category_name_flags_channel = std::make_tuple(&channel,"channel",flags_channel);
         std::vector<std::string> flags_flavor;
         flags_flavor.push_back("CP");
         flags_flavor.push_back("CA");
         tuple_category_name_flags category_name_flags_flavor = std::make_tuple(&flavor,"flavor",flags_flavor);
         
         std::vector<RooRealVar*> variables_8D;
         variables_8D.push_back(&mbc);
         variables_8D.push_back(&deltae);
         variables_8D.push_back(&chil0_ma);
         variables_8D.push_back(&nb_prime);
         variables_8D.push_back(&mass_kpi);
         variables_8D.push_back(&hel_phi);
         variables_8D.push_back(&hel_th1);
         variables_8D.push_back(&hel_th2);
         RooArgSet varset_9D(mbc,deltae,chil0_ma,nb_prime,mass_kpi,hel_phi,hel_th1,hel_th2,flavor);
         
         std::vector<RooRealVar*> variables_4D;
         variables_4D.push_back(&mass_kpi);
         variables_4D.push_back(&hel_phi);
         variables_4D.push_back(&hel_th1);
         variables_4D.push_back(&hel_th2);
         RooArgSet varset_5D(mbc,deltae,chil0_ma,nb_prime,flavor);
         
         // ---------------------------------------------
         // ----- Define full data range
         // ---------------------------------------------
         SetFitRanges(mbc,deltae,chil0_ma,mass_kpi,hel_phi,hel_th1,hel_th2,nb_prime);
         // ---------------------------------------------
         // ----- Create data component plots
         // ---------------------------------------------
         for(RooRealVar* var : variables_8D) {
         CreateDataComponentPlots(data_sideband,*var,opt->GetBins(),"full_range_SB",opt->GetOutdir(),opt->GetPlotFormat(),"CP","flavor","_sideband");
         CreateDataComponentPlots(data_sideband,*var,opt->GetBins(),"full_range_SB",opt->GetOutdir(),opt->GetPlotFormat(),"CA","flavor","_sideband");
         CreateDataComponentPlots(data_sideband,*var,opt->GetBins(),"full_range_SB",opt->GetOutdir(),opt->GetPlotFormat(),"CS","datatype","_sideband");
         }
         // ---------------------------------------------
         // ----- Do fits
         // ---------------------------------------------
         // See RooFit tutorial macro #501 about HowTo do a simultaneous fit
         // See RooFit tutorial macro #603 about HowTo do a multi-core parallelized unbinned maximum likelihood fit
         std::cout << "STATUS: Do fits..." << std::endl;
         
         PrintFitStartBlock("SETUP: CacheIntegrator");
         int debug_level = 1;
         CacheIntegrator<MassHelicityAnglesNormCachedIntegral>* mass_helicity_angles_norm_cache_integrator = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral>(debug_level);
         CacheIntegrator<MassHelicityAnglesRealCachedIntegral>* mass_helicity_angles_real_cache_integrator = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral>(debug_level);
         CacheIntegrator<MassHelicityAnglesImagCachedIntegral>* mass_helicity_angles_imag_cache_integrator = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral>(debug_level);
         CacheIntegrator<MassHelicityAnglesNormCachedIntegral_phi_th1_th2>* mass_helicity_angles_norm_cache_integrator_phi_th1_th2 = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral_phi_th1_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesRealCachedIntegral_phi_th1_th2>* mass_helicity_angles_real_cache_integrator_phi_th1_th2 = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral_phi_th1_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesImagCachedIntegral_phi_th1_th2>* mass_helicity_angles_imag_cache_integrator_phi_th1_th2 = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral_phi_th1_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_th1_th2>* mass_helicity_angles_norm_cache_integrator_m_th1_th2 = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_th1_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_th1_th2>* mass_helicity_angles_real_cache_integrator_m_th1_th2 = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_th1_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_th1_th2>* mass_helicity_angles_imag_cache_integrator_m_th1_th2 = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_th1_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_phi_th2>* mass_helicity_angles_norm_cache_integrator_m_phi_th2 = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_phi_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_phi_th2>* mass_helicity_angles_real_cache_integrator_m_phi_th2 = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_phi_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_phi_th2>* mass_helicity_angles_imag_cache_integrator_m_phi_th2 = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_phi_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_phi_th1>* mass_helicity_angles_norm_cache_integrator_m_phi_th1 = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_phi_th1>(debug_level);
         CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_phi_th1>* mass_helicity_angles_real_cache_integrator_m_phi_th1 = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_phi_th1>(debug_level);
         CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_phi_th1>* mass_helicity_angles_imag_cache_integrator_m_phi_th1 = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_phi_th1>(debug_level);
         
         ACCEPTANCE_FUNCTION acceptance = PRODUCT_4D;
         std::string path_to_integrals = "./CachedIntegrals_ACCEPTANCE" + FFUtil::GetAcceptanceString(acceptance) + "/";
         mass_helicity_angles_norm_cache_integrator->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_real_cache_integrator->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_imag_cache_integrator->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_norm_cache_integrator_phi_th1_th2->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_real_cache_integrator_phi_th1_th2->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_imag_cache_integrator_phi_th1_th2->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_norm_cache_integrator_m_th1_th2->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_real_cache_integrator_m_th1_th2->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_imag_cache_integrator_m_th1_th2->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_norm_cache_integrator_m_phi_th2->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_real_cache_integrator_m_phi_th2->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_imag_cache_integrator_m_phi_th2->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_norm_cache_integrator_m_phi_th1->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_real_cache_integrator_m_phi_th1->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_imag_cache_integrator_m_phi_th1->LoadIntegrals(path_to_integrals);
         
         MHI_PDG_constants mhi_pdg_constants = {m_Pi, m_K, m_Phi, m_B0};
         MHI_CacheIntegrators_4D mhi_4D = {
         mass_helicity_angles_norm_cache_integrator,
         mass_helicity_angles_real_cache_integrator,
         mass_helicity_angles_imag_cache_integrator
         };
         MHI_CacheIntegrators_3D_phi_th1_th2 mhi_3D_phi_th1_th2 = {
         mass_helicity_angles_norm_cache_integrator_phi_th1_th2,
         mass_helicity_angles_real_cache_integrator_phi_th1_th2,
         mass_helicity_angles_imag_cache_integrator_phi_th1_th2
         };
         MHI_CacheIntegrators_3D_m_th1_th2 mhi_3D_m_th1_th2 = {
         mass_helicity_angles_norm_cache_integrator_m_th1_th2,
         mass_helicity_angles_real_cache_integrator_m_th1_th2,
         mass_helicity_angles_imag_cache_integrator_m_th1_th2
         };
         MHI_CacheIntegrators_3D_m_phi_th2 mhi_3D_m_phi_th2 = {
         mass_helicity_angles_norm_cache_integrator_m_phi_th2,
         mass_helicity_angles_real_cache_integrator_m_phi_th2,
         mass_helicity_angles_imag_cache_integrator_m_phi_th2
         };
         MHI_CacheIntegrators_3D_m_phi_th1 mhi_3D_m_phi_th1 = {
         mass_helicity_angles_norm_cache_integrator_m_phi_th1,
         mass_helicity_angles_real_cache_integrator_m_phi_th1,
         mass_helicity_angles_imag_cache_integrator_m_phi_th1
         };
         
         // KK interference cache integrator
         CacheIntegrator<KKInterferenceNormCachedIntegral>* KK_interference_norm_cache_integrator = NULL;
         CacheIntegrator<KKInterferenceRealCachedIntegral>* KK_interference_real_cache_integrator = NULL;
         CacheIntegrator<KKInterferenceImagCachedIntegral>* KK_interference_imag_cache_integrator = NULL;
         
         if(data_pdf_mode == 5) {
         debug_level = 1;
         KK_interference_norm_cache_integrator = new CacheIntegrator<KKInterferenceNormCachedIntegral>(debug_level);
         KK_interference_real_cache_integrator = new CacheIntegrator<KKInterferenceRealCachedIntegral>(debug_level);
         KK_interference_imag_cache_integrator = new CacheIntegrator<KKInterferenceImagCachedIntegral>(debug_level);
         
         KK_interference_norm_cache_integrator->LoadIntegrals(path_to_integrals);
         KK_interference_real_cache_integrator->LoadIntegrals(path_to_integrals);
         KK_interference_imag_cache_integrator->LoadIntegrals(path_to_integrals);
         }
         
         KKInterferenceCacheIntegrators_4D KK_4D = {
         KK_interference_norm_cache_integrator,
         KK_interference_real_cache_integrator,
         KK_interference_imag_cache_integrator
         };
         
         for(RooRealVar *v : variables_8D) {
         CreateDataComponentPlots(data,*v,opt->GetBins(),"full_range",opt->GetOutdir(),opt->GetPlotFormat(),"CP","flavor","");
         CreateDataComponentPlots(data,*v,opt->GetBins(),"full_range",opt->GetOutdir(),opt->GetPlotFormat(),"CA","flavor","");
         CreateDataComponentPlots(data,*v,opt->GetBins(),"full_range",opt->GetOutdir(),opt->GetPlotFormat(),"C","channel","");
         }
         
         PrintFitStartBlock("SETUP: 9D Fit");
         
         shared_ptr<FF_PDF> pdf_bkg_continuum_9D = make_shared<FF_PDF_bkg_continuum_9D_mbc_deltae_chil0_ma_mass_kpi_hel_phi_hel_th1_hel_th2_nb_prime_flavor>(mbc,
         deltae, chil0_ma, mass_kpi, hel_phi, hel_th1,
         hel_th2, nb_prime, flavor, category_name_flags_channel,true,(RooRealVar*)NULL);
         pdf_bkg_continuum_9D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_continuum_8D.dat",true);
         pdf_bkg_continuum_9D->GetVariable("N_chil0_ma_bkg_continuum_nonres_fraction")->setConstant(true);
         pdf_bkg_continuum_9D->GetVariable("N_hel_th1_bkg_continuum_a1_polyvar_mass_kpi_c0")->setConstant(true);
         pdf_bkg_continuum_9D->GetVariable("N_hel_th1_bkg_continuum_a2_polyvar_mass_kpi_c1")->setConstant(true);
         pdf_bkg_continuum_9D->GetVariable("N_hel_th1_bkg_continuum_a3_polyvar_mass_kpi_c0")->setConstant(true);
         pdf_bkg_continuum_9D->GetVariable("N_mass_kpi_bkg_continuum_a1")->setConstant(true);
         pdf_bkg_continuum_9D->GetVariable("N_mass_kpi_bkg_continuum_a2")->setConstant(true);
         pdf_bkg_continuum_9D->GetVariable("N_mass_kpi_bkg_continuum_a3")->setConstant(true);
         pdf_bkg_continuum_9D->GetVariable("N_mass_kpi_bkg_continuum_nonres_fraction")->setConstant(true);
         //pdf_bkg_continuum_9D->SetVariablesConst();
         shared_ptr<FF_PDF> pdf_signal_4D = make_shared<FF_PDF_signal_4D_mbc_deltae_chil0_ma_nb_prime>(mbc,
         deltae, chil0_ma, nb_prime, category_name_flags_channel,true,pdf_bkg_continuum_9D->GetVariable("chil0_ma_resolution"));
         pdf_signal_4D->ImportPDFValues(opt->GetOutdir()+"pdf_signal_4D.dat",true);
         // scale Delta E resolution
         double scale = 1.124;
         pdf_signal_4D->GetVariable("C_deltae_signal_width_core")->setVal(pdf_signal_4D->GetVariable("C_deltae_signal_width_core")->getVal()*scale);
         pdf_signal_4D->GetVariable("C_deltae_signal_width_1st_left")->setVal(pdf_signal_4D->GetVariable("C_deltae_signal_width_1st_left")->getVal()*scale);
         pdf_signal_4D->GetVariable("C_deltae_signal_width_1st_right")->setVal(pdf_signal_4D->GetVariable("C_deltae_signal_width_1st_right")->getVal()*scale);
         pdf_signal_4D->GetVariable("C_deltae_signal_width_2nd_left")->setVal(pdf_signal_4D->GetVariable("C_deltae_signal_width_2nd_left")->getVal()*scale);
         pdf_signal_4D->GetVariable("C_deltae_signal_width_2nd_right")->setVal(pdf_signal_4D->GetVariable("C_deltae_signal_width_2nd_right")->getVal()*scale);
         pdf_signal_4D->SetVariablesConst();
         //pdf_signal_4D->GetVariable("C_deltae_signal_width_core")->setConstant(false);
         shared_ptr<FF_PDF> pdf_signal_5D_angles;
         switch (data_pdf_mode) {
         case 0:
         case 1:
         case 2:
         case 3:
         case 5:
         pdf_signal_5D_angles= make_shared<FF_PDF_signal_5D_mass_helicity_angles_CPV>(mass_kpi,
         hel_th1, hel_th2, hel_phi, flavor, acceptance,
         mhi_pdg_constants, mhi_4D,
         mhi_3D_phi_th1_th2, mhi_3D_m_th1_th2, mhi_3D_m_phi_th2, mhi_3D_m_phi_th1,
         category_name_flags_channel);
         // NOTE: Correct signal K* mass and width for PDG2012 values and compare to nominal it results with old PDG values
         pdf_signal_5D_angles->GetVariable("mass_1")->setVal(0.89594);
         pdf_signal_5D_angles->GetVariable("width_1")->setVal(0.0487);
         break;
         case 6:
         pdf_signal_5D_angles= make_shared<FF_PDF_signal_5D_mass_helicity_angles_CPV_PhysBase>(mass_kpi,
         hel_th1, hel_th2, hel_phi, flavor, acceptance,
         mhi_pdg_constants, mhi_4D,
         mhi_3D_phi_th1_th2, mhi_3D_m_th1_th2, mhi_3D_m_phi_th2, mhi_3D_m_phi_th1,
         category_name_flags_channel);
         break;
         default:
         break;
         }
         shared_ptr<FF_PDF> pdf_signal_9D = make_shared<FF_PDF_signal_9D>(pdf_signal_4D, pdf_signal_5D_angles,
         category_name_flags_channel);
         shared_ptr<FF_PDF> pdf_bkg_bbar_wrongphi_9D;
         shared_ptr<FF_PDF> pdf_bkg_bbar_wrongphi_9D_pos;
         shared_ptr<FF_PDF> pdf_bkg_bbar_wrongphi_9D_neg;
         std::vector<RooRealVar*> amplitude_buffer;
         std::vector<RooRealVar*> amplitude_flatte_buffer;
         switch (data_pdf_mode) {
         case 0:
         pdf_bkg_bbar_wrongphi_9D = make_shared<FF_PDF_bkg_bbar_wrongphikkbar_9D>(mbc,
         deltae, chil0_ma, nb_prime, mass_kpi,
         hel_phi, hel_th1, hel_th2, flavor, category_name_flags_channel, true);
         pdf_bkg_bbar_wrongphi_9D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongphikkbar_8D.dat",true);
         pdf_bkg_bbar_wrongphi_9D->SetVariablesConst();
         break;
         case 1:
         pdf_bkg_bbar_wrongphi_9D = make_shared<FF_PDF_bkg_bbar_wrongphikkbar_9D>(mbc,
         deltae, chil0_ma, nb_prime, mass_kpi,
         hel_phi, hel_th1, hel_th2, flavor, category_name_flags_channel, true);
         pdf_bkg_bbar_wrongphi_9D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongphikkbar_8D.dat",true);
         pdf_bkg_bbar_wrongphi_9D->SetVariablesConst();
         break;
         case 2:
         case 6:
         pdf_bkg_bbar_wrongphi_9D = make_shared<FF_PDF_bkg_bbar_wrongphif0980_9D>(mbc,
         deltae, chil0_ma, nb_prime, mass_kpi,
         hel_phi, hel_th1, hel_th2, flavor, category_name_flags_channel, true);
         pdf_bkg_bbar_wrongphi_9D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongphif0980_8D.dat",true);
         pdf_bkg_bbar_wrongphi_9D->SetVariablesConst();
         break;
         case 3:
         pdf_bkg_bbar_wrongphi_9D = make_shared<FF_PDF_bkg_bbar_wrongphif0980AndNonRes_9D>(mbc,
         deltae, chil0_ma, nb_prime, mass_kpi,
         hel_phi, hel_th1, hel_th2, flavor, category_name_flags_channel, true);
         pdf_bkg_bbar_wrongphi_9D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongphif0980_8D.dat",true);
         pdf_bkg_bbar_wrongphi_9D->SetVariablesConst();
         pdf_bkg_bbar_wrongphi_9D->GetVariable("chil0_ma_bkg_bbar_wrongphinonres_amp")->setConstant(false);
         pdf_bkg_bbar_wrongphi_9D->GetVariable("chil0_ma_bkg_bbar_wrongphinonres_phase")->setConstant(false);
         break;
         case 5:
         pdf_bkg_bbar_wrongphi_9D = make_shared<FF_PDF_bkg_bbar_wrongphif0980_9D>(mbc,
         deltae, chil0_ma, nb_prime, mass_kpi,
         hel_phi, hel_th1, hel_th2, flavor, category_name_flags_channel, true);
         pdf_bkg_bbar_wrongphi_9D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongphif0980_8D.dat",true);
         pdf_bkg_bbar_wrongphi_9D->SetVariablesConst();
         
         amplitude_buffer.reserve(6);
         amplitude_buffer.push_back(pdf_signal_9D->GetVariable("amp_10"));
         amplitude_buffer.push_back(pdf_signal_9D->GetVariable("phase_10"));
         amplitude_buffer.push_back(pdf_signal_9D->GetVariable("amp_1para"));
         amplitude_buffer.push_back(pdf_signal_9D->GetVariable("phase_1para"));
         amplitude_buffer.push_back(pdf_signal_9D->GetVariable("amp_1perp"));
         amplitude_buffer.push_back(pdf_signal_9D->GetVariable("phase_1perp"));
         //			pdf_bkg_bbar_wrongphi_9D = make_shared<FF_PDF_bkg_bbar_wrongphif0980_with_interference_9D_norm>(mbc,
         //					deltae, chil0_ma, nb_prime, mass_kpi,
         //					hel_phi, hel_th1, hel_th2, flavor, category_name_flags_channel, true,
         //					amplitude_buffer, acceptance, KK_4D);
         //			pdf_bkg_bbar_wrongphi_9D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongphif0980_8D.dat",true);
         //			pdf_bkg_bbar_wrongphi_9D->SetVariablesConst();
         //			pdf_bkg_bbar_wrongphi_9D->GetVariable("amp_flatte")->setConstant(false);
         //			pdf_bkg_bbar_wrongphi_9D->GetVariable("phase_flatte")->setConstant(false);
         amplitude_flatte_buffer.reserve(2);
         amplitude_flatte_buffer.push_back(new RooRealVar("amp_flatte","amp_flatte",0.3,0.0,1.0));
         amplitude_flatte_buffer.push_back(new RooRealVar("phase_flatte","phase_flatte",0.0,-2*TMath::Pi(),2*TMath::Pi()));
         pdf_bkg_bbar_wrongphi_9D_pos = make_shared<FF_PDF_bkg_bbar_wrongphif0980_with_interference_9D_positive>(mbc,
         deltae, chil0_ma, nb_prime, mass_kpi,
         hel_phi, hel_th1, hel_th2, flavor, category_name_flags_channel, true,
         amplitude_buffer, amplitude_flatte_buffer, acceptance, KK_4D);
         pdf_bkg_bbar_wrongphi_9D_pos->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongphif0980_8D.dat",true);
         pdf_bkg_bbar_wrongphi_9D_pos->SetVariablesConst();
         pdf_bkg_bbar_wrongphi_9D_neg = make_shared<FF_PDF_bkg_bbar_wrongphif0980_with_interference_9D_negative>(mbc,
         deltae, chil0_ma, nb_prime, mass_kpi,
         hel_phi, hel_th1, hel_th2, flavor, category_name_flags_channel, true,
         amplitude_buffer, amplitude_flatte_buffer, acceptance, KK_4D);
         pdf_bkg_bbar_wrongphi_9D_neg->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongphif0980_8D.dat",true);
         pdf_bkg_bbar_wrongphi_9D_neg->SetVariablesConst();
         
         amplitude_flatte_buffer.at(0)->setConstant(true); // amp
         amplitude_flatte_buffer.at(1)->setConstant(false); // phase
         pdf_bkg_bbar_wrongphi_9D_neg->GetPDF("C").forceNumInt(false);
         pdf_bkg_bbar_wrongphi_9D_pos->GetPDF("C").forceNumInt(false);
         break;
         default:
         break;
         }
         
         shared_ptr<FF_PDF> pdf_data_9D;
         switch (data_pdf_mode) {
         case 0:
         pdf_data_9D = make_shared<FF_PDF_data_9D__CPV>(pdf_signal_9D,
         pdf_bkg_continuum_9D, pdf_bkg_bbar_wrongphi_9D,
         data.sumEntries(), category_name_flags_channel);
         break;
         case 1:
         pdf_data_9D = make_shared<FF_PDF_data_9D__CPV__KK_floated>(pdf_signal_9D,
         pdf_bkg_continuum_9D, pdf_bkg_bbar_wrongphi_9D,
         data.sumEntries(), category_name_flags_channel);
         break;
         case 2:
         case 6:
         pdf_data_9D = make_shared<FF_PDF_data_9D__CPV__KK_Flatte>(pdf_signal_9D,
         pdf_bkg_continuum_9D, pdf_bkg_bbar_wrongphi_9D,
         data.sumEntries(), category_name_flags_channel);
         break;
         case 3:
         pdf_data_9D = make_shared<FF_PDF_data_9D__CPV__KK_FlatteAndNonRes>(pdf_signal_9D,
         pdf_bkg_continuum_9D, pdf_bkg_bbar_wrongphi_9D,
         data.sumEntries(), category_name_flags_channel);
         break;
         case 5:
         pdf_data_9D = make_shared<FF_PDF_data_9D__CPV__KK_Flatte_with_interference_pos_neg>(pdf_signal_9D,
         pdf_bkg_continuum_9D, pdf_bkg_bbar_wrongphi_9D, pdf_bkg_bbar_wrongphi_9D_pos, pdf_bkg_bbar_wrongphi_9D_neg,
         data.sumEntries(), category_name_flags_channel);
         break;
         default:
         std::cout << "ERROR: Unknown data pdf mode for data fit!\n";
         return;
         break;
         }
         
         PrintFitStartBlock("FITTING: 9D Sideband Fit");
         
         // fit the continuum pdf to sideband data and fix the shape
         mbc.setMin(5.24);
         mbc.setMax(5.26);
         mbc.setRange("full_range",5.24,5.26);
         
         // fit the 1D mass_kpi distribution due to some instability
         shared_ptr<FF_PDF> pdf_bkg_continuum_1D_mass_kpi = make_shared<FF_PDF_bkg_continuum_1D_mass_kpi>(mass_kpi, category_name_flags_channel, false);
         pdf_bkg_continuum_1D_mass_kpi->GetVariable("N_mass_kpi_bkg_continuum_a1")->setConstant(true);
         pdf_bkg_continuum_1D_mass_kpi->GetVariable("N_mass_kpi_bkg_continuum_a2")->setConstant(true);
         pdf_bkg_continuum_1D_mass_kpi->GetVariable("N_mass_kpi_bkg_continuum_a3")->setConstant(true);
         pdf_bkg_continuum_1D_mass_kpi->GetVariable("N_mass_kpi_bkg_continuum_nonres_fraction")->setConstant(true);
         RooFitResult *result_mass_kpi = pdf_bkg_continuum_1D_mass_kpi->GetSimPDF().fitTo(data_sideband, SumW2Error(false), NumCPU(opt->GetNumcpu()),
         Timer(true), Save(true), InitialHesse(false), Minos(false), Extended(false));
         pdf_bkg_continuum_1D_mass_kpi->GetSimPDF().getParameters(data_sideband)->Print("v");
         WriteCorrelationMatrixFile(result_mass_kpi,opt->GetOutdir()+"pdf_data_1D_mass_kpi_correlation.dat");
         pdf_bkg_continuum_1D_mass_kpi->ExportPDFValues(opt->GetOutdir()+"pdf_data_1D_mass_kpi.dat",true);
         result_mass_kpi->Print("v");
         // set mass_kpi distribution and fix it
         for(RooRealVar *v : pdf_bkg_continuum_1D_mass_kpi->GetVariables()) {
         pdf_bkg_continuum_9D->SetVariable(v->GetName(),v->getVal(),v->getError(),true,false);
         }
         
         RooFitResult *result_9D_sideband = pdf_bkg_continuum_9D->GetSimPDF().fitTo(data_sideband, SumW2Error(false), NumCPU(opt->GetNumcpu()),
         Timer(true), Save(true), InitialHesse(false), Minos(false), Extended(false));
         pdf_bkg_continuum_9D->GetSimPDF().getParameters(data_sideband)->Print("v");
         
         if(opt->GetCreatePlots()) {
         for(RooRealVar *v : variables_8D) {
         std::string varname = v->GetName();
         
         PrintFitStartBlock("PLOT PROJECTION");
         FFUtil::VisualizeFitResult(pdf_bkg_continuum_9D,data_sideband,*v,category_name_flags_channel,false,
         opt->GetBins(),"full_range", opt->GetOutdir()+"DATA_9D_sideband_"+varname+"_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         }
         WriteCorrelationMatrixFile(result_9D_sideband,opt->GetOutdir()+"pdf_data_9D_sideband_correlation.dat");
         pdf_bkg_continuum_9D->ExportPDFValues(opt->GetOutdir()+"pdf_data_9D_sideband.dat",true);
         
         // draw projections in 5 mass_kpi regions
         for(unsigned int i = 1; i <= 5; ++i) {
         mbc.setRange(("mass_kpi_bin_"+mp::to_string(i)).c_str(),5.24,5.26);
         FFUtil::VisualizeFitResult(pdf_bkg_continuum_9D,data_sideband,hel_th1,category_name_flags_channel,false,
         opt->GetBins(),"mass_kpi_bin_"+mp::to_string(i), opt->GetOutdir()+"DATA_9D_sideband_hel_th1_projection_in_mass_kpi_bin_"+mp::to_string(i)+"."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         }
         WriteCorrelationMatrixFile(result_9D_sideband,opt->GetOutdir()+"pdf_data_9D_sideband_correlation.dat");
         pdf_bkg_continuum_9D->ExportPDFValues(opt->GetOutdir()+"pdf_data_9D_sideband.dat",true);
         }
         result_9D_sideband->Print("v");
         
         // fit the fit region data with fixed pdf
         mbc.setMax(5.29);
         mbc.setMin(5.26);
         mbc.setRange("full_range",5.26,5.29);
         pdf_bkg_continuum_9D->SetVariablesConst();
         pdf_bkg_continuum_9D->GetVariable("mbc_bkg_continuum_c")->setConstant(false);
         
         PrintFitStartBlock("CREATING TRUTH MAP");
         std::map<std::string, double> starting_values_map;
         starting_values_map["amp_00"] = pt.get<double>("amp_00");
         starting_values_map["amp_10"] = pt.get<double>("amp_10");
         starting_values_map["amp_1para"] = pt.get<double>("amp_1para");
         starting_values_map["amp_1perp"] = pt.get<double>("amp_1perp");
         starting_values_map["amp_20"] = pt.get<double>("amp_20");
         starting_values_map["amp_2para"] = pt.get<double>("amp_2para");
         starting_values_map["amp_2perp"] = pt.get<double>("amp_2perp");
         starting_values_map["phase_00"] = 0.0;
         starting_values_map["phase_10"] = pt.get<double>("phase_10");
         starting_values_map["phase_1para"] = pt.get<double>("phase_1para");
         starting_values_map["phase_1perp"] = pt.get<double>("phase_1perp");
         starting_values_map["phase_20"] = pt.get<double>("phase_20");
         starting_values_map["phase_2para"] = pt.get<double>("phase_2para");
         starting_values_map["phase_2perp"] = pt.get<double>("phase_2perp");
         starting_values_map["amp_00_delta"] = pt.get<double>("amp_00_delta");
         starting_values_map["amp_10_delta"] = pt.get<double>("amp_10_delta");
         starting_values_map["amp_1para_delta"] = pt.get<double>("amp_1para_delta");
         starting_values_map["amp_1perp_delta"] = pt.get<double>("amp_1perp_delta");
         starting_values_map["amp_20_delta"] = pt.get<double>("amp_20_delta");
         starting_values_map["amp_2para_delta"] = pt.get<double>("amp_2para_delta");
         starting_values_map["amp_2perp_delta"] = pt.get<double>("amp_2perp_delta");
         starting_values_map["phase_00_delta"] = 0.0;
         starting_values_map["phase_10_delta"] = pt.get<double>("phase_10_delta");
         starting_values_map["phase_1para_delta"] = pt.get<double>("phase_1para_delta");
         starting_values_map["phase_1perp_delta"] = pt.get<double>("phase_1perp_delta");
         starting_values_map["phase_20_delta"] = pt.get<double>("phase_20_delta");
         starting_values_map["phase_2para_delta"] = pt.get<double>("phase_2para_delta");
         starting_values_map["phase_2perp_delta"] = pt.get<double>("phase_2perp_delta");
         starting_values_map["signal_nevents"] = pt.get<double>("signal_nevents");
         starting_values_map["C_bkg_continuum_nevents"] = pt.get<double>("C_bkg_continuum_nevents");
         switch (data_pdf_mode) {
         case 0:
         case 1:
         starting_values_map["C_bkg_bbar_wrongphikkbar_nevents"] = pt.get<double>("C_bkg_bbar_wrongphikkbar_nevents");
         break;
         case 2:
         case 3:
         case 5:
         case 6:
         starting_values_map["C_bkg_bbar_wrongphif0980_nevents"] = pt.get<double>("C_bkg_bbar_wrongphif0980_nevents");
         break;
         default:
         break;
         }
         starting_values_map["mbc_bkg_continuum_c"] = pt.get<double>("mbc_bkg_continuum_c");
         
         std::vector<std::string> parameters;
         parameters.push_back("signal_nevents");
         parameters.push_back("C_bkg_continuum_nevents");
         switch (data_pdf_mode) {
         case 0:
         case 1:
         parameters.push_back("C_bkg_bbar_wrongphikkbar_nevents");
         break;
         case 2:
         case 3:
         case 5:
         case 6:
         parameters.push_back("C_bkg_bbar_wrongphif0980_nevents");
         break;
         default:
         break;
         }
         parameters.push_back("mbc_bkg_continuum_c");
         parameters.push_back("amp_00");
         //parameters.push_back("amp_10");
         parameters.push_back("amp_1para");
         parameters.push_back("amp_1perp");
         parameters.push_back("amp_20");
         parameters.push_back("amp_2para");
         parameters.push_back("amp_2perp");
         parameters.push_back("phase_10");
         parameters.push_back("phase_1para");
         parameters.push_back("phase_1perp");
         parameters.push_back("phase_20");
         parameters.push_back("phase_2para");
         parameters.push_back("phase_2perp");
         parameters.push_back("amp_00_delta");
         parameters.push_back("amp_10_delta");
         parameters.push_back("amp_1para_delta");
         parameters.push_back("amp_1perp_delta");
         parameters.push_back("amp_20_delta");
         if(!opt->GetFixCPV2PerpAndParaComponent()) {
         parameters.push_back("amp_2para_delta");
         parameters.push_back("amp_2perp_delta");
         }
         parameters.push_back("phase_10_delta");
         parameters.push_back("phase_1para_delta");
         parameters.push_back("phase_1perp_delta");
         parameters.push_back("phase_20_delta");
         if(!opt->GetFixCPV2PerpAndParaComponent()) {
         parameters.push_back("phase_2para_delta");
         parameters.push_back("phase_2perp_delta");
         }
         
         PrintFitStartBlock("REPEATING FIT WITH STARTING VALUES");
         std::vector<RooFitResult*> fit_results;
         std::string outdir_files = opt->GetOutdir()+"/data_fit_"+(opt->GetMinos() ? "_minos" : "")+"/";
         mp::execute_command("mkdir -p "+outdir_files);
         // start loop of fits
         for(int i = 1; i <= 1; ++i) {
         std::stringstream ss;
         ss << "sample_" << i;
         std::cout << "INFO: Data fit: " << i << std::endl;
         
         // reset to fit start values
         pdf_data_9D->GetVariable("amp_00")->setVal(starting_values_map["amp_00"]);
         pdf_data_9D->GetVariable("amp_10")->setVal(starting_values_map["amp_10"]);
         pdf_data_9D->GetVariable("amp_1para")->setVal(starting_values_map["amp_1para"]);
         pdf_data_9D->GetVariable("amp_1perp")->setVal(starting_values_map["amp_1perp"]);
         pdf_data_9D->GetVariable("amp_20")->setVal(starting_values_map["amp_20"]);
         pdf_data_9D->GetVariable("amp_2para")->setVal(starting_values_map["amp_2para"]);
         pdf_data_9D->GetVariable("amp_2perp")->setVal(starting_values_map["amp_2perp"]);
         
         pdf_data_9D->GetVariable("phase_10")->setVal(starting_values_map["phase_10"]);
         pdf_data_9D->GetVariable("phase_1para")->setVal(starting_values_map["phase_1para"]);
         pdf_data_9D->GetVariable("phase_1perp")->setVal(starting_values_map["phase_1perp"]);
         pdf_data_9D->GetVariable("phase_20")->setVal(starting_values_map["phase_20"]);
         pdf_data_9D->GetVariable("phase_2para")->setVal(starting_values_map["phase_2para"]);
         pdf_data_9D->GetVariable("phase_2perp")->setVal(starting_values_map["phase_2perp"]);
         
         pdf_data_9D->GetVariable("amp_00_delta")->setVal(starting_values_map["amp_00_delta"]);
         pdf_data_9D->GetVariable("amp_10_delta")->setVal(starting_values_map["amp_10_delta"]);
         pdf_data_9D->GetVariable("amp_1para_delta")->setVal(starting_values_map["amp_1para_delta"]);
         pdf_data_9D->GetVariable("amp_1perp_delta")->setVal(starting_values_map["amp_1perp_delta"]);
         pdf_data_9D->GetVariable("amp_20_delta")->setVal(starting_values_map["amp_20_delta"]);
         pdf_data_9D->GetVariable("amp_2para_delta")->setVal(starting_values_map["amp_2para_delta"]);
         pdf_data_9D->GetVariable("amp_2perp_delta")->setVal(starting_values_map["amp_2perp_delta"]);
         
         pdf_data_9D->GetVariable("phase_10_delta")->setVal(starting_values_map["phase_10_delta"]);
         pdf_data_9D->GetVariable("phase_1para_delta")->setVal(starting_values_map["phase_1para_delta"]);
         pdf_data_9D->GetVariable("phase_1perp_delta")->setVal(starting_values_map["phase_1perp_delta"]);
         pdf_data_9D->GetVariable("phase_20_delta")->setVal(starting_values_map["phase_20_delta"]);
         pdf_data_9D->GetVariable("phase_2para_delta")->setVal(starting_values_map["phase_2para_delta"]);
         pdf_data_9D->GetVariable("phase_2perp_delta")->setVal(starting_values_map["phase_2perp_delta"]);
         
         if(opt->GetFixCPV2PerpAndParaComponent()) {
         pdf_data_9D->GetVariable("amp_2para_delta")->setVal(0.0);
         pdf_data_9D->GetVariable("amp_2para_delta")->setConstant(true);
         pdf_data_9D->GetVariable("amp_2perp_delta")->setVal(0.0);
         pdf_data_9D->GetVariable("amp_2perp_delta")->setConstant(true);
         }
         
         if(opt->GetFixCPV2PerpAndParaComponent()) {
         pdf_data_9D->GetVariable("phase_2para_delta")->setVal(0.0);
         pdf_data_9D->GetVariable("phase_2para_delta")->setConstant(true);
         pdf_data_9D->GetVariable("phase_2perp_delta")->setVal(TMath::Pi()/2);
         pdf_data_9D->GetVariable("phase_2perp_delta")->setConstant(true);
         }
         
         if(opt->GetMinos()) {
         RooArgSet minos_pars(*pdf_data_9D->GetVariable("phase_2para"),
         *pdf_data_9D->GetVariable("phase_2perp"),
         *pdf_data_9D->GetVariable("amp_2para_delta"),
         *pdf_data_9D->GetVariable("amp_2perp_delta"),
         *pdf_data_9D->GetVariable("phase_2para_delta"),
         *pdf_data_9D->GetVariable("phase_2perp_delta"));
         pdf_data_9D->GetVariable("amp_2para_delta")->setMin(-0.95);
         switch (data_pdf_mode)	{
         case 0:
         fit_results.push_back(pdf_data_9D->GetSimPDF().fitTo(data, SumW2Error(false), NumCPU(opt->GetNumcpu()),
         Timer(true), Save(true), InitialHesse(false), Minos(minos_pars), Extended(true),
         Constrain(RooArgSet(*pdf_data_9D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents")))));
         break;
         case 1:
         case 2:
         case 3:
         case 6:
         fit_results.push_back(pdf_data_9D->GetSimPDF().fitTo(data, SumW2Error(false), NumCPU(opt->GetNumcpu()),
         Timer(true), Save(true), InitialHesse(false), Minos(minos_pars), Extended(true)));
         break;
         case 5:
         fit_results.push_back(pdf_data_9D->GetSimPDF().fitTo(data, SumW2Error(false), NumCPU(opt->GetNumcpu()),
         Timer(true), Save(true), InitialHesse(false), Minos(minos_pars), Extended(true)));
         break;
         default:
         return;
         break;
         }
         } else {
         switch (data_pdf_mode) {
         case 0:
         fit_results.push_back(pdf_data_9D->GetSimPDF().fitTo(data, SumW2Error(false), NumCPU(opt->GetNumcpu()),
         Timer(true), Save(true), InitialHesse(false), Minos(false), Extended(true),
         Constrain(RooArgSet(*pdf_data_9D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents")))));
         break;
         case 1:
         case 2:
         case 3:
         case 6:
         fit_results.push_back(pdf_data_9D->GetSimPDF().fitTo(data, SumW2Error(false), NumCPU(opt->GetNumcpu()),
         Timer(true), Save(true), InitialHesse(false), Minos(false), Extended(true)));
         break;
         case 5:
         {
         RooArgSet minos_pars(*amplitude_flatte_buffer.at(1));
         amplitude_flatte_buffer.at(1)->setVal(RooRandom::randomGenerator()->Uniform(-TMath::Pi(),TMath::Pi()));
         fit_results.push_back(pdf_data_9D->GetSimPDF().fitTo(data, SumW2Error(false), NumCPU(opt->GetNumcpu()),
         Timer(true), Save(true), InitialHesse(false), Minos(minos_pars), Extended(true)));
         break;
         }
         default:
         return;
         break;
         }
         }
         fit_results.back()->Print("v");
         
         // check if any phase is outside the [-pi,pi] region and push it back towards truth value
         // this is only to get good pulls, the other solution is not wrong
         switch (data_pdf_mode) {
         case 0:
         case 1:
         case 2:
         case 3:
         case 5:
         ShiftPhaseToInterval(pdf_data_9D, "phase_10", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phase_1para", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phase_1perp", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phase_20", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phase_2para", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phase_2perp", 0.0);
         break;
         case 6:
         ShiftPhaseToInterval(pdf_data_9D, "phi_01", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phi_1para", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phi_1perp", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phi_02", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phi_2para", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phi_2perp", 0.0);
         break;
         default:
         break;
         }
         
         PrintFitResultLatex(pdf_data_9D);
         PrintPhysicsParametersLatex(PropagateErrors(pdf_data_9D,pdf_signal_9D,fit_results.back(),opt,varset_9D,false));
         fit_results.back()->correlationMatrix().Print();
         fit_results.back()->covarianceMatrix().Print();
         WriteCorrelationMatrixFile(fit_results.back(),opt->GetOutdir()+"pdf_data_9D_correlation.dat");
         WriteCovarianceMatrixFile(fit_results.back(),opt->GetOutdir()+"pdf_data_9D_covariance.dat");
         pdf_data_9D->ExportPDFValues(opt->GetOutdir()+"pdf_data_9D.dat",true);
         
         // compute fit fraction for flatte with non-resonant coherent sum
         switch (data_pdf_mode) {
         case 3:
         {
         std::complex<double> amp_KK = std::polar((double)pdf_bkg_bbar_wrongphi_9D->GetVariable("chil0_ma_bkg_bbar_wrongphinonres_amp")->getVal(),
         (double)pdf_bkg_bbar_wrongphi_9D->GetVariable("chil0_ma_bkg_bbar_wrongphinonres_phase")->getVal());
         std::complex<double> amp_Flatte = std::polar((double)pdf_bkg_bbar_wrongphi_9D->GetVariable("chil0_ma_bkg_bbar_wrongphif0980_amp")->getVal(),
         (double)pdf_bkg_bbar_wrongphi_9D->GetVariable("chil0_ma_bkg_bbar_wrongphif0980_phase")->getVal());
         
         RooAbsReal* I_tot = pdf_bkg_bbar_wrongphi_9D->GetPDF("C").createIntegral(varset,"full_range");
         double N_tot = I_tot->getVal();
         
         pdf_bkg_bbar_wrongphi_9D->GetVariable("chil0_ma_bkg_bbar_wrongphif0980_amp")->setVal(0.0);
         double N_KK = I_tot->getVal();
         pdf_bkg_bbar_wrongphi_9D->GetVariable("chil0_ma_bkg_bbar_wrongphif0980_amp")->setVal(std::abs(amp_Flatte));
         pdf_bkg_bbar_wrongphi_9D->GetVariable("chil0_ma_bkg_bbar_wrongphinonres_amp")->setVal(0.0);
         double N_Flatte = I_tot->getVal();
         pdf_bkg_bbar_wrongphi_9D->GetVariable("chil0_ma_bkg_bbar_wrongphinonres_amp")->setVal(std::abs(amp_KK));
         
         std::cout << "INFO: Fraction of non-resonant KK contribution:  " << N_KK/N_tot << "\n";
         std::cout << "INFO: Fraction of resonant f0(980) contribution: " << N_Flatte/N_tot << "\n";
         
         delete I_tot;
         break;
         }
         default:
         break;
         }
         
         std::ofstream fout((outdir_files+"DATA_9D_fit_result_"+ss.str().c_str()+".dat").c_str());
         if(fout.is_open()) {
         for(const std::string& p : parameters) {
         fout << p << ";";
         fout << 0.0 << ";";
         fout << pdf_data_9D->GetVariable(p)->getVal() << ";";
         fout << pdf_data_9D->GetVariable(p)->getError() << ";";
         fout << pdf_data_9D->GetVariable(p)->getErrorLo() << ";";
         fout << pdf_data_9D->GetVariable(p)->getErrorHi() << std::endl;
         }
         fout << "EDM;" << fit_results.back()->edm() << std::endl;
         fout << "NLL;" << fit_results.back()->minNll() << std::endl;
         fout.close();
         } else {
         std::cerr << "ERROR: Could not write fit result to file" << std::endl;
         }
         }
         // end of fit
         
         // unfortunatly this takes ages...
         if(false && data_pdf_mode == 5) {
         for(RooRealVar *v : variables_8D) {
         std::string varname = v->GetName();
         if(varname == "hel_th2") {
         FFUtil::VisualizeFitResult(pdf_data_9D,data,*v,category_name_flags_channel,true,
         opt->GetBins(),"signal_box_K892",opt->GetOutdir()+"DATA_9D_"+varname+"_projection_signal_box_K892_Interference."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         }
         
         if(varname == "mbc" || varname == "chil0_ma") {
         FFUtil::VisualizeFitResult(pdf_data_9D,data,*v,category_name_flags_channel,true,
         opt->GetBins(),("signal_box_"+varname).c_str(),opt->GetOutdir()+"DATA_9D_"+varname+"_projection_signal_box_Interference."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         }
         }
         }
         
         // plot fit result
         if(opt->GetCreatePlots()) {
         for(RooRealVar *v : variables_8D) {
         std::string varname = v->GetName();
         
         PrintFitStartBlock("PLOT PROJECTION");
         FFUtil::VisualizeFitResult(pdf_data_9D,data,*v,category_name_flags_channel,true,
         opt->GetBins(),"full_range", opt->GetOutdir()+"DATA_9D_"+varname+"_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         if(varname == "mbc" || varname == "deltae" || varname == "chil0_ma" || varname == "nb_prime") {
         FFUtil::VisualizeFitResult(pdf_data_9D,data,*v,category_name_flags_channel,true,
         opt->GetBins(),("signal_box_"+varname).c_str(),opt->GetOutdir()+"DATA_9D_"+varname+"_projection_signal_box."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         if(opt->GetCreateProjWData()) {
         FFUtil::VisualizeFitResultProjWData(pdf_data_9D,data,*v,category_name_flags_channel,true,
         opt->GetBins(),("signal_box_"+varname).c_str(),opt->GetOutdir()+"DATA_9D_PROJWDATA_"+varname+"_projection_signal_box."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         category_name_flags_flavor,true,
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         }
         } else {
         FFUtil::VisualizeFitResult(pdf_data_9D,data,*v,category_name_flags_channel,true,
         opt->GetBins(),"signal_box",opt->GetOutdir()+"DATA_9D_"+varname+"_projection_signal_box."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         if(opt->GetCreateProjWData()) {
         FFUtil::VisualizeFitResultProjWData(pdf_data_9D,data,*v,category_name_flags_channel,true,
         opt->GetBins(),"signal_box",opt->GetOutdir()+"DATA_9D_PROJWDATA_"+varname+"_projection_signal_box."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         category_name_flags_flavor,true,
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         }
         }
         // angular distributions for each resonance
         if(varname == "hel_phi" || varname == "hel_th1" || varname == "hel_th2") {
         FFUtil::VisualizeFitResult(pdf_data_9D,data,*v,category_name_flags_channel,true,
         opt->GetBins(),"signal_box_LASS",opt->GetOutdir()+"DATA_9D_"+varname+"_projection_signal_box_LASS."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         FFUtil::VisualizeFitResult(pdf_data_9D,data,*v,category_name_flags_channel,true,
         opt->GetBins(),"signal_box_K892",opt->GetOutdir()+"DATA_9D_"+varname+"_projection_signal_box_K892."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         FFUtil::VisualizeFitResult(pdf_data_9D,data,*v,category_name_flags_channel,true,
         opt->GetBins(),"signal_box_K21430",opt->GetOutdir()+"DATA_9D_"+varname+"_projection_signal_box_K21430."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         }
         // f0(980) enriched regions
         if(varname == "mbc" || varname == "deltae") {
         FFUtil::VisualizeFitResult(pdf_data_9D,data,*v,category_name_flags_channel,true,
         opt->GetBins(),("signal_box_"+varname+"_f0980").c_str(),opt->GetOutdir()+"DATA_9D_"+varname+"_projection_signal_box_f0980."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         FFUtil::VisualizeFitResult(pdf_data_9D,data,*v,category_name_flags_channel,true,
         opt->GetBins(),("signal_box_"+varname+"_f0980_K892").c_str(),opt->GetOutdir()+"DATA_9D_"+varname+"_projection_signal_box_f0980_K892."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         FFUtil::VisualizeFitResult(pdf_data_9D,data,*v,category_name_flags_channel,true,
         opt->GetBins(),("signal_box_"+varname+"_f0980_highKPi").c_str(),opt->GetOutdir()+"DATA_9D_"+varname+"_projection_signal_box_f0980_highKPi."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         }
         if(varname == "chil0_ma") {
         FFUtil::VisualizeFitResult(pdf_data_9D,data,*v,category_name_flags_channel,true,
         opt->GetBins(),("signal_box_"+varname+"_f0980_K892").c_str(),opt->GetOutdir()+"DATA_9D_"+varname+"_projection_signal_box_f0980_K892."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         }
         if(varname == "hel_phi" || varname == "hel_th1" || varname == "hel_th2") {
         FFUtil::VisualizeFitResult(pdf_data_9D,data,*v,category_name_flags_channel,true,
         opt->GetBins(),"signal_box_f0980_K892",opt->GetOutdir()+"DATA_9D_"+varname+"_projection_signal_box_f0980_K892."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         }
         // nonres. K+K- enriched regions
         if(varname == "mbc" || varname == "deltae") {
         FFUtil::VisualizeFitResult(pdf_data_9D,data,*v,category_name_flags_channel,true,
         opt->GetBins(),("signal_box_"+varname+"_KK_K892").c_str(),opt->GetOutdir()+"DATA_9D_"+varname+"_projection_signal_box_KK_K892."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         }
         if(varname == "hel_phi" || varname == "hel_th1" || varname == "hel_th2") {
         FFUtil::VisualizeFitResult(pdf_data_9D,data,*v,category_name_flags_channel,true,
         opt->GetBins(),"signal_box_KK_K892",opt->GetOutdir()+"DATA_9D_"+varname+"_projection_signal_box_KK_K892."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         }
         // B->phiphi and B->phirho enriched regions
         if(varname == "mbc") {
         FFUtil::VisualizeFitResult(pdf_data_9D,data,*v,category_name_flags_channel,true,
         opt->GetBins(),("signal_box_"+varname+"_phi_phi").c_str(),opt->GetOutdir()+"DATA_9D_"+varname+"_projection_signal_box_phi_phi."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         FFUtil::VisualizeFitResult(pdf_data_9D,data,*v,category_name_flags_channel,true,
         opt->GetBins(),("signal_box_"+varname+"_phi_rho").c_str(),opt->GetOutdir()+"DATA_9D_"+varname+"_projection_signal_box_phi_rho."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         }
         if(varname != "mbc") {
         FFUtil::VisualizeFitResult(pdf_data_9D,data,*v,category_name_flags_channel,true,
         opt->GetBins(),"mbc_sideband", opt->GetOutdir()+"DATA_9D_"+varname+"_projection_mbc_sideband."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         }
         }
         }
         std::cout << "Signal-Yield/Signal-YieldError = " << pdf_data_9D->GetVariable("signal_nevents")->getVal()/pdf_data_9D->GetVariable("signal_nevents")->getError() << std::endl;
         
         mass_helicity_angles_norm_cache_integrator->SaveIntegrals(path_to_integrals);
         mass_helicity_angles_real_cache_integrator->SaveIntegrals(path_to_integrals);
         mass_helicity_angles_imag_cache_integrator->SaveIntegrals(path_to_integrals);
         mass_helicity_angles_norm_cache_integrator_phi_th1_th2->SaveIntegrals(path_to_integrals);
         mass_helicity_angles_real_cache_integrator_phi_th1_th2->SaveIntegrals(path_to_integrals);
         mass_helicity_angles_imag_cache_integrator_phi_th1_th2->SaveIntegrals(path_to_integrals);
         mass_helicity_angles_norm_cache_integrator_m_th1_th2->SaveIntegrals(path_to_integrals);
         mass_helicity_angles_real_cache_integrator_m_th1_th2->SaveIntegrals(path_to_integrals);
         mass_helicity_angles_imag_cache_integrator_m_th1_th2->SaveIntegrals(path_to_integrals);
         mass_helicity_angles_norm_cache_integrator_m_phi_th2->SaveIntegrals(path_to_integrals);
         mass_helicity_angles_real_cache_integrator_m_phi_th2->SaveIntegrals(path_to_integrals);
         mass_helicity_angles_imag_cache_integrator_m_phi_th2->SaveIntegrals(path_to_integrals);
         mass_helicity_angles_norm_cache_integrator_m_phi_th1->SaveIntegrals(path_to_integrals);
         mass_helicity_angles_real_cache_integrator_m_phi_th1->SaveIntegrals(path_to_integrals);
         mass_helicity_angles_imag_cache_integrator_m_phi_th1->SaveIntegrals(path_to_integrals);
         
         delete mass_helicity_angles_imag_cache_integrator;
         delete mass_helicity_angles_imag_cache_integrator_m_phi_th1;
         delete mass_helicity_angles_imag_cache_integrator_m_phi_th2;
         delete mass_helicity_angles_imag_cache_integrator_m_th1_th2;
         delete mass_helicity_angles_imag_cache_integrator_phi_th1_th2;
         delete mass_helicity_angles_norm_cache_integrator;
         delete mass_helicity_angles_norm_cache_integrator_m_phi_th1;
         delete mass_helicity_angles_norm_cache_integrator_m_phi_th2;
         delete mass_helicity_angles_norm_cache_integrator_m_th1_th2;
         delete mass_helicity_angles_norm_cache_integrator_phi_th1_th2;
         delete mass_helicity_angles_real_cache_integrator;
         delete mass_helicity_angles_real_cache_integrator_m_phi_th1;
         delete mass_helicity_angles_real_cache_integrator_m_phi_th2;
         delete mass_helicity_angles_real_cache_integrator_m_th1_th2;
         delete mass_helicity_angles_real_cache_integrator_phi_th1_th2;
         
         for(RooFitResult* r : fit_results) {
         delete r;
         }
         delete result_9D_sideband;
         delete result_mass_kpi;*/
    }
    
    void FitDataWithLikelihoodScan(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt) {
        /*
         if(chain->GetEntries() == 0) {
         std::cout << "No data for final fits given, skip fitting." << std::endl;
         return;
         }
         
         // ---------------------------------------------
         // ----- Parse configuration
         // ---------------------------------------------
         std::cout << "Reading fit configuration from " << opt->GetConfigFile() << std::endl;
         namespace bpt = boost::property_tree;
         bpt::ptree pt;
         bpt::info_parser::read_info(opt->GetConfigFile(),pt);
         
         // ---------------------------------------------
         // ----- Define variables
         // ---------------------------------------------
         PrintFitStartBlock("STATUS: Define functions and variables...");
         RooRealVar mbc("mbc","m_{bc}",5.24,5.29,"GeV");
         RooRealVar deltae("deltae","#Delta E",-0.15,0.15,"GeV");
         RooRealVar chil0_ma("chil0_ma","m_{KK}",0.985,1.05,"GeV");
         RooRealVar mass_kpi("mass_kpi","m_{K#pi}",0.7,1.55,"GeV");
         RooRealVar hel_phi("hel_phi","#Phi",-TMath::Pi(),TMath::Pi(),"rad");
         RooRealVar hel_th1("hel_th1","cos #Theta_{1}",-1.0,1.0,"");
         RooRealVar hel_th2("hel_th2","cos #Theta_{2}",-1.0,1.0,"");
         RooRealVar nb_prime("nb_prime","C_{NB}'",-10.0,10.0,"");
         RooCategory channel("channel","decay channel");
         channel.defineType("C",0);
         channel.defineType("N",1);
         RooCategory datatype("datatype","data sample type");
         datatype.defineType("CP",0);
         datatype.defineType("CA",1);
         datatype.defineType("N",2);
         datatype.defineType("CS",3);
         datatype.defineType("NS",4);
         datatype.defineType("COFF",5);
         datatype.defineType("NOFF",6);
         RooCategory flavor("flavor","flavor type");
         flavor.defineType("CP",0);
         flavor.defineType("CA",1);
         // necessary for MC information checks
         RooRealVar mcinfo("mcinfo","MC information",0,256);
         RooRealVar mctype("mctype","MC type information",-1,1005);
         RooRealVar chil0_mo("chil0_mo","MC #phi mother",-999999,999999);
         RooRealVar chil0_mc("chil0_mc","MC information #phi",0,256);
         RooRealVar runno("runno","Run Number",-999999,999999);
         // create data set (constructor only overload for max. 9 arguments, so we add some afterwards)
         RooArgSet varset(mbc,deltae,chil0_ma,mass_kpi,hel_phi,hel_th1,hel_th2,nb_prime);
         varset.add(channel);
         varset.add(datatype);
         varset.add(flavor);
         varset.add(mcinfo);
         varset.add(mctype);
         varset.add(chil0_mo);
         varset.add(chil0_mc);
         varset.add(runno);
         
         TCut cut_default_CP_and_CA = "(mbc < 5.289 && hel_th1 < 0.75 && (datatype==datatype::CP || datatype==datatype::CA))";
         TCut cut_default_CS = "(mbc < 5.289 && hel_th1 < 0.75 && (datatype==datatype::CS))";
         
         RooDataSet data("data","data",varset,Import(*chain.get()),Cut(cut_default_CP_and_CA));
         RooDataSet data_sideband("data_sideband","data_sideband",varset,Import(*chain.get()),Cut(cut_default_CS));
         
         // create helper for the simultaneous fit
         std::vector<std::string> flags_channel;
         flags_channel.push_back("C");
         flags_channel.push_back("N");
         tuple_category_name_flags category_name_flags_channel = std::make_tuple(&channel,"channel",flags_channel);
         
         std::vector<RooRealVar*> variables_8D;
         variables_8D.push_back(&mbc);
         variables_8D.push_back(&deltae);
         variables_8D.push_back(&chil0_ma);
         variables_8D.push_back(&nb_prime);
         variables_8D.push_back(&mass_kpi);
         variables_8D.push_back(&hel_phi);
         variables_8D.push_back(&hel_th1);
         variables_8D.push_back(&hel_th2);
         RooArgSet varset_9D(mbc,deltae,chil0_ma,nb_prime,mass_kpi,hel_phi,hel_th1,hel_th2,flavor);
         
         std::vector<RooRealVar*> variables_4D;
         variables_4D.push_back(&mass_kpi);
         variables_4D.push_back(&hel_phi);
         variables_4D.push_back(&hel_th1);
         variables_4D.push_back(&hel_th2);
         RooArgSet varset_5D(mbc,deltae,chil0_ma,nb_prime,flavor);
         
         // ---------------------------------------------
         // ----- Define full data range
         // ---------------------------------------------
         SetFitRanges(mbc,deltae,chil0_ma,mass_kpi,hel_phi,hel_th1,hel_th2,nb_prime);
         // ---------------------------------------------
         // ----- Do fits
         // ---------------------------------------------
         // See RooFit tutorial macro #501 about HowTo do a simultaneous fit
         // See RooFit tutorial macro #603 about HowTo do a multi-core parallelized unbinned maximum likelihood fit
         std::cout << "STATUS: Do fits..." << std::endl;
         
         PrintFitStartBlock("SETUP: CacheIntegrator");
         int debug_level = 1;
         CacheIntegrator<MassHelicityAnglesNormCachedIntegral>* mass_helicity_angles_norm_cache_integrator = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral>(debug_level);
         CacheIntegrator<MassHelicityAnglesRealCachedIntegral>* mass_helicity_angles_real_cache_integrator = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral>(debug_level);
         CacheIntegrator<MassHelicityAnglesImagCachedIntegral>* mass_helicity_angles_imag_cache_integrator = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral>(debug_level);
         CacheIntegrator<MassHelicityAnglesNormCachedIntegral_phi_th1_th2>* mass_helicity_angles_norm_cache_integrator_phi_th1_th2 = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral_phi_th1_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesRealCachedIntegral_phi_th1_th2>* mass_helicity_angles_real_cache_integrator_phi_th1_th2 = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral_phi_th1_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesImagCachedIntegral_phi_th1_th2>* mass_helicity_angles_imag_cache_integrator_phi_th1_th2 = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral_phi_th1_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_th1_th2>* mass_helicity_angles_norm_cache_integrator_m_th1_th2 = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_th1_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_th1_th2>* mass_helicity_angles_real_cache_integrator_m_th1_th2 = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_th1_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_th1_th2>* mass_helicity_angles_imag_cache_integrator_m_th1_th2 = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_th1_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_phi_th2>* mass_helicity_angles_norm_cache_integrator_m_phi_th2 = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_phi_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_phi_th2>* mass_helicity_angles_real_cache_integrator_m_phi_th2 = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_phi_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_phi_th2>* mass_helicity_angles_imag_cache_integrator_m_phi_th2 = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_phi_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_phi_th1>* mass_helicity_angles_norm_cache_integrator_m_phi_th1 = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_phi_th1>(debug_level);
         CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_phi_th1>* mass_helicity_angles_real_cache_integrator_m_phi_th1 = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_phi_th1>(debug_level);
         CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_phi_th1>* mass_helicity_angles_imag_cache_integrator_m_phi_th1 = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_phi_th1>(debug_level);
         
         ACCEPTANCE_FUNCTION acceptance = PRODUCT_4D;
         std::string path_to_integrals = "./CachedIntegrals_ACCEPTANCE" + FFUtil::GetAcceptanceString(acceptance) + "/";
         mass_helicity_angles_norm_cache_integrator->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_real_cache_integrator->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_imag_cache_integrator->LoadIntegrals(path_to_integrals);
         //	mass_helicity_angles_norm_cache_integrator_phi_th1_th2->LoadIntegrals(path_to_integrals);
         //	mass_helicity_angles_real_cache_integrator_phi_th1_th2->LoadIntegrals(path_to_integrals);
         //	mass_helicity_angles_imag_cache_integrator_phi_th1_th2->LoadIntegrals(path_to_integrals);
         //	mass_helicity_angles_norm_cache_integrator_m_th1_th2->LoadIntegrals(path_to_integrals);
         //	mass_helicity_angles_real_cache_integrator_m_th1_th2->LoadIntegrals(path_to_integrals);
         //	mass_helicity_angles_imag_cache_integrator_m_th1_th2->LoadIntegrals(path_to_integrals);
         //	mass_helicity_angles_norm_cache_integrator_m_phi_th2->LoadIntegrals(path_to_integrals);
         //	mass_helicity_angles_real_cache_integrator_m_phi_th2->LoadIntegrals(path_to_integrals);
         //	mass_helicity_angles_imag_cache_integrator_m_phi_th2->LoadIntegrals(path_to_integrals);
         //	mass_helicity_angles_norm_cache_integrator_m_phi_th1->LoadIntegrals(path_to_integrals);
         //	mass_helicity_angles_real_cache_integrator_m_phi_th1->LoadIntegrals(path_to_integrals);
         //	mass_helicity_angles_imag_cache_integrator_m_phi_th1->LoadIntegrals(path_to_integrals);
         
         MHI_PDG_constants mhi_pdg_constants = {m_Pi, m_K, m_Phi, m_B0};
         MHI_CacheIntegrators_4D mhi_4D = {
         mass_helicity_angles_norm_cache_integrator,
         mass_helicity_angles_real_cache_integrator,
         mass_helicity_angles_imag_cache_integrator
         };
         MHI_CacheIntegrators_3D_phi_th1_th2 mhi_3D_phi_th1_th2 = {
         mass_helicity_angles_norm_cache_integrator_phi_th1_th2,
         mass_helicity_angles_real_cache_integrator_phi_th1_th2,
         mass_helicity_angles_imag_cache_integrator_phi_th1_th2
         };
         MHI_CacheIntegrators_3D_m_th1_th2 mhi_3D_m_th1_th2 = {
         mass_helicity_angles_norm_cache_integrator_m_th1_th2,
         mass_helicity_angles_real_cache_integrator_m_th1_th2,
         mass_helicity_angles_imag_cache_integrator_m_th1_th2
         };
         MHI_CacheIntegrators_3D_m_phi_th2 mhi_3D_m_phi_th2 = {
         mass_helicity_angles_norm_cache_integrator_m_phi_th2,
         mass_helicity_angles_real_cache_integrator_m_phi_th2,
         mass_helicity_angles_imag_cache_integrator_m_phi_th2
         };
         MHI_CacheIntegrators_3D_m_phi_th1 mhi_3D_m_phi_th1 = {
         mass_helicity_angles_norm_cache_integrator_m_phi_th1,
         mass_helicity_angles_real_cache_integrator_m_phi_th1,
         mass_helicity_angles_imag_cache_integrator_m_phi_th1
         };
         
         PrintFitStartBlock("SETUP: 9D Fit");
         
         shared_ptr<FF_PDF> pdf_bkg_continuum_9D = make_shared<FF_PDF_bkg_continuum_9D_mbc_deltae_chil0_ma_mass_kpi_hel_phi_hel_th1_hel_th2_nb_prime_flavor>(mbc,
         deltae, chil0_ma, mass_kpi, hel_phi, hel_th1,
         hel_th2, nb_prime, flavor, category_name_flags_channel,true,(RooRealVar*)NULL);
         pdf_bkg_continuum_9D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_continuum_8D.dat",true);
         pdf_bkg_continuum_9D->GetVariable("N_chil0_ma_bkg_continuum_nonres_fraction")->setConstant(true);
         pdf_bkg_continuum_9D->GetVariable("N_hel_th1_bkg_continuum_a1_polyvar_mass_kpi_c0")->setConstant(true);
         pdf_bkg_continuum_9D->GetVariable("N_hel_th1_bkg_continuum_a2_polyvar_mass_kpi_c1")->setConstant(true);
         pdf_bkg_continuum_9D->GetVariable("N_hel_th1_bkg_continuum_a3_polyvar_mass_kpi_c0")->setConstant(true);
         pdf_bkg_continuum_9D->GetVariable("N_mass_kpi_bkg_continuum_a1")->setConstant(true);
         pdf_bkg_continuum_9D->GetVariable("N_mass_kpi_bkg_continuum_a2")->setConstant(true);
         pdf_bkg_continuum_9D->GetVariable("N_mass_kpi_bkg_continuum_a3")->setConstant(true);
         pdf_bkg_continuum_9D->GetVariable("N_mass_kpi_bkg_continuum_nonres_fraction")->setConstant(true);
         //pdf_bkg_continuum_9D->SetVariablesConst();
         shared_ptr<FF_PDF> pdf_signal_4D = make_shared<FF_PDF_signal_4D_mbc_deltae_chil0_ma_nb_prime>(mbc,
         deltae, chil0_ma, nb_prime, category_name_flags_channel,true,pdf_bkg_continuum_9D->GetVariable("chil0_ma_resolution"));
         pdf_signal_4D->ImportPDFValues(opt->GetOutdir()+"pdf_signal_4D.dat",true);
         // scale Delta E resolution
         double scale = 1.124;
         pdf_signal_4D->GetVariable("C_deltae_signal_width_core")->setVal(pdf_signal_4D->GetVariable("C_deltae_signal_width_core")->getVal()*scale);
         pdf_signal_4D->GetVariable("C_deltae_signal_width_1st_left")->setVal(pdf_signal_4D->GetVariable("C_deltae_signal_width_1st_left")->getVal()*scale);
         pdf_signal_4D->GetVariable("C_deltae_signal_width_1st_right")->setVal(pdf_signal_4D->GetVariable("C_deltae_signal_width_1st_right")->getVal()*scale);
         pdf_signal_4D->GetVariable("C_deltae_signal_width_2nd_left")->setVal(pdf_signal_4D->GetVariable("C_deltae_signal_width_2nd_left")->getVal()*scale);
         pdf_signal_4D->GetVariable("C_deltae_signal_width_2nd_right")->setVal(pdf_signal_4D->GetVariable("C_deltae_signal_width_2nd_right")->getVal()*scale);
         pdf_signal_4D->SetVariablesConst();
         //pdf_signal_4D->GetVariable("C_deltae_signal_width_core")->setConstant(false);
         shared_ptr<FF_PDF> pdf_signal_5D_angles = make_shared<FF_PDF_signal_5D_mass_helicity_angles_CPV>(mass_kpi,
         hel_th1, hel_th2, hel_phi, flavor, acceptance,
         mhi_pdg_constants, mhi_4D,
         mhi_3D_phi_th1_th2, mhi_3D_m_th1_th2, mhi_3D_m_phi_th2, mhi_3D_m_phi_th1,
         category_name_flags_channel);
         shared_ptr<FF_PDF> pdf_signal_9D = make_shared<FF_PDF_signal_9D>(pdf_signal_4D, pdf_signal_5D_angles,
         category_name_flags_channel);
         shared_ptr<FF_PDF> pdf_bkg_bbar_wrongphi_9D = make_shared<FF_PDF_bkg_bbar_wrongphif0980_9D>(mbc,
         deltae, chil0_ma, nb_prime, mass_kpi,
         hel_phi, hel_th1, hel_th2, flavor, category_name_flags_channel, true);
         pdf_bkg_bbar_wrongphi_9D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongphif0980_8D.dat",true);
         pdf_bkg_bbar_wrongphi_9D->SetVariablesConst();
         
         shared_ptr<FF_PDF> pdf_data_9D = make_shared<FF_PDF_data_9D__CPV__KK_Flatte>(pdf_signal_9D,
         pdf_bkg_continuum_9D, pdf_bkg_bbar_wrongphi_9D,
         data.sumEntries(), category_name_flags_channel);
         
         PrintFitStartBlock("FITTING: 9D Sideband Fit");
         
         // fit the continuum pdf to sideband data and fix the shape
         mbc.setMin(5.24);
         mbc.setMax(5.26);
         mbc.setRange("full_range",5.24,5.26);
         
         // fit the 1D mass_kpi distribution due to some instability
         shared_ptr<FF_PDF> pdf_bkg_continuum_1D_mass_kpi = make_shared<FF_PDF_bkg_continuum_1D_mass_kpi>(mass_kpi, category_name_flags_channel, false);
         pdf_bkg_continuum_1D_mass_kpi->GetVariable("N_mass_kpi_bkg_continuum_a1")->setConstant(true);
         pdf_bkg_continuum_1D_mass_kpi->GetVariable("N_mass_kpi_bkg_continuum_a2")->setConstant(true);
         pdf_bkg_continuum_1D_mass_kpi->GetVariable("N_mass_kpi_bkg_continuum_a3")->setConstant(true);
         pdf_bkg_continuum_1D_mass_kpi->GetVariable("N_mass_kpi_bkg_continuum_nonres_fraction")->setConstant(true);
         RooFitResult *result_mass_kpi = pdf_bkg_continuum_1D_mass_kpi->GetSimPDF().fitTo(data_sideband, SumW2Error(false), NumCPU(opt->GetNumcpu()),
         Timer(true), Save(true), InitialHesse(false), Minos(false), Extended(false));
         pdf_bkg_continuum_1D_mass_kpi->GetSimPDF().getParameters(data_sideband)->Print("v");
         result_mass_kpi->Print();
         // set mass_kpi distribution and fix it
         for(RooRealVar *v : pdf_bkg_continuum_1D_mass_kpi->GetVariables()) {
         pdf_bkg_continuum_9D->SetVariable(v->GetName(),v->getVal(),v->getError(),true,false);
         }
         
         RooFitResult *result_9D_sideband = pdf_bkg_continuum_9D->GetSimPDF().fitTo(data_sideband, SumW2Error(false), NumCPU(opt->GetNumcpu()),
         Timer(true), Save(true), InitialHesse(false), Minos(false), Extended(false));
         pdf_bkg_continuum_9D->GetSimPDF().getParameters(data_sideband)->Print("v");
         result_9D_sideband->Print();
         
         // fit the fit region data with fixed pdf
         mbc.setMax(5.29);
         mbc.setMin(5.26);
         mbc.setRange("full_range",5.26,5.29);
         pdf_bkg_continuum_9D->SetVariablesConst();
         pdf_bkg_continuum_9D->GetVariable("mbc_bkg_continuum_c")->setConstant(false);
         
         PrintFitStartBlock("CREATING TRUTH MAP");
         std::map<std::string, double> starting_values_map;
         starting_values_map["amp_00"] = pt.get<double>("amp_00");
         starting_values_map["amp_10"] = pt.get<double>("amp_10");
         starting_values_map["amp_1para"] = pt.get<double>("amp_1para");
         starting_values_map["amp_1perp"] = pt.get<double>("amp_1perp");
         starting_values_map["amp_20"] = pt.get<double>("amp_20");
         starting_values_map["amp_2para"] = pt.get<double>("amp_2para");
         starting_values_map["amp_2perp"] = pt.get<double>("amp_2perp");
         starting_values_map["phase_00"] = 0.0;
         starting_values_map["phase_10"] = pt.get<double>("phase_10");
         starting_values_map["phase_1para"] = pt.get<double>("phase_1para");
         starting_values_map["phase_1perp"] = pt.get<double>("phase_1perp");
         starting_values_map["phase_20"] = pt.get<double>("phase_20");
         starting_values_map["phase_2para"] = pt.get<double>("phase_2para");
         starting_values_map["phase_2perp"] = pt.get<double>("phase_2perp");
         starting_values_map["amp_00_delta"] = pt.get<double>("amp_00_delta");
         starting_values_map["amp_10_delta"] = pt.get<double>("amp_10_delta");
         starting_values_map["amp_1para_delta"] = pt.get<double>("amp_1para_delta");
         starting_values_map["amp_1perp_delta"] = pt.get<double>("amp_1perp_delta");
         starting_values_map["amp_20_delta"] = pt.get<double>("amp_20_delta");
         starting_values_map["amp_2para_delta"] = pt.get<double>("amp_2para_delta");
         starting_values_map["amp_2perp_delta"] = pt.get<double>("amp_2perp_delta");
         starting_values_map["phase_00_delta"] = 0.0;
         starting_values_map["phase_10_delta"] = pt.get<double>("phase_10_delta");
         starting_values_map["phase_1para_delta"] = pt.get<double>("phase_1para_delta");
         starting_values_map["phase_1perp_delta"] = pt.get<double>("phase_1perp_delta");
         starting_values_map["phase_20_delta"] = pt.get<double>("phase_20_delta");
         starting_values_map["phase_2para_delta"] = pt.get<double>("phase_2para_delta");
         starting_values_map["phase_2perp_delta"] = pt.get<double>("phase_2perp_delta");
         starting_values_map["signal_nevents"] = pt.get<double>("signal_nevents");
         starting_values_map["C_bkg_continuum_nevents"] = pt.get<double>("C_bkg_continuum_nevents");
         starting_values_map["C_bkg_bbar_wrongphif0980_nevents"] = pt.get<double>("C_bkg_bbar_wrongphif0980_nevents");
         starting_values_map["mbc_bkg_continuum_c"] = pt.get<double>("mbc_bkg_continuum_c");
         
         std::vector<std::string> parameters;
         parameters.push_back("signal_nevents");
         parameters.push_back("C_bkg_continuum_nevents");
         parameters.push_back("C_bkg_bbar_wrongphif0980_nevents");
         parameters.push_back("mbc_bkg_continuum_c");
         parameters.push_back("amp_00");
         //parameters.push_back("amp_10");
         parameters.push_back("amp_1para");
         parameters.push_back("amp_1perp");
         parameters.push_back("amp_20");
         parameters.push_back("amp_2para");
         parameters.push_back("amp_2perp");
         parameters.push_back("phase_10");
         parameters.push_back("phase_1para");
         parameters.push_back("phase_1perp");
         parameters.push_back("phase_20");
         parameters.push_back("phase_2para");
         parameters.push_back("phase_2perp");
         parameters.push_back("amp_00_delta");
         parameters.push_back("amp_10_delta");
         parameters.push_back("amp_1para_delta");
         parameters.push_back("amp_1perp_delta");
         parameters.push_back("amp_20_delta");
         if(!opt->GetFixCPV2PerpAndParaComponent()) {
         parameters.push_back("amp_2para_delta");
         parameters.push_back("amp_2perp_delta");
         }
         parameters.push_back("phase_10_delta");
         parameters.push_back("phase_1para_delta");
         parameters.push_back("phase_1perp_delta");
         parameters.push_back("phase_20_delta");
         if(!opt->GetFixCPV2PerpAndParaComponent()) {
         parameters.push_back("phase_2para_delta");
         parameters.push_back("phase_2perp_delta");
         }
         
         std::string profile_ll_parameter = opt->GetCreateProfileLLParameter();
         int steps = 75;
         std::vector<double> nominal_x; nominal_x.reserve(1);
         std::vector<double> nominal_y; nominal_y.reserve(1);
         std::vector<double> scan_x; scan_x.reserve(steps);
         std::vector<double> scan_y; scan_y.reserve(steps);
         
         std::map<std::string, std::pair<double,double> > profile_scan_limits;
         profile_scan_limits["amp_00"] = std::make_pair(0.0,1.0);
         //profile_scan_limits["amp_10"] = std::make_pair(0.0,1.0);
         profile_scan_limits["amp_1para"] = std::make_pair(0.0,1.0);
         profile_scan_limits["amp_1perp"] = std::make_pair(0.0,1.0);
         profile_scan_limits["amp_20"] = std::make_pair(0.0,1.0);
         profile_scan_limits["amp_2para"] = std::make_pair(0.0,1.0);
         profile_scan_limits["amp_2perp"] = std::make_pair(0.0,1.0);
         //profile_scan_limits["phase_00"] = std::make_pair(0.0,0.0);
         profile_scan_limits["phase_10"] = std::make_pair(-3.5,3.5);
         profile_scan_limits["phase_1para"] = std::make_pair(-3.5,3.5);
         profile_scan_limits["phase_1perp"] = std::make_pair(-3.5,3.5);
         profile_scan_limits["phase_20"] = std::make_pair(-3.5,3.5);
         profile_scan_limits["phase_2para"] = std::make_pair(-3.5,3.5);
         profile_scan_limits["phase_2perp"] = std::make_pair(-3.5,3.5);
         profile_scan_limits["amp_00_delta"] = std::make_pair(-0.5,0.5);
         profile_scan_limits["amp_10_delta"] = std::make_pair(-0.5,0.5);
         profile_scan_limits["amp_1para_delta"] = std::make_pair(-0.5,0.5);
         profile_scan_limits["amp_1perp_delta"] = std::make_pair(-0.5,0.5);
         profile_scan_limits["amp_20_delta"] = std::make_pair(-0.5,0.5);
         profile_scan_limits["amp_2para_delta"] = std::make_pair(-0.9,0.9);
         profile_scan_limits["amp_2perp_delta"] = std::make_pair(-0.9,0.9);
         //profile_scan_limits["phase_00_delta"] = std::make_pair(0.0,0.0);;
         profile_scan_limits["phase_10_delta"] = std::make_pair(-1.5,1.5);
         profile_scan_limits["phase_1para_delta"] = std::make_pair(-1.5,1.5);
         profile_scan_limits["phase_1perp_delta"] = std::make_pair(0.1,3.0);
         profile_scan_limits["phase_20_delta"] = std::make_pair(-1.5,1.5);
         profile_scan_limits["phase_2para_delta"] = std::make_pair(-1.5,1.5);
         profile_scan_limits["phase_2perp_delta"] = std::make_pair(0.1,3.0);
         profile_scan_limits["signal_nevents"] = std::make_pair(1000.0,1300.0);
         profile_scan_limits["C_bkg_bbar_wrongphif0980_nevents"] = std::make_pair(100.0,195.0);
         
         if(std::find(parameters.begin(),parameters.end(),profile_ll_parameter) == parameters.end()) {
         std::cout << "ERROR: Paramater for profile likelihood not in parameters!" << std::endl;
         steps = 0;
         }
         
         PrintFitStartBlock("REPEATING 9D FITS");
         std::vector<RooFitResult*> fit_results;
         std::string outdir_files = opt->GetOutdir()+"/data_fit_profile_ll_" + profile_ll_parameter +"/";
         mp::execute_command("mkdir -p "+outdir_files);
         // start loop of fits
         for(int i = 0; i < steps; ++i) {
         std::stringstream ss;
         ss << "sample_" << i;
         std::cout << "INFO: Data fit: " << i << std::endl;
         
         // reset to fit start values
         pdf_data_9D->GetVariable("amp_00")->setVal(starting_values_map["amp_00"]);
         pdf_data_9D->GetVariable("amp_10")->setVal(starting_values_map["amp_10"]);
         pdf_data_9D->GetVariable("amp_1para")->setVal(starting_values_map["amp_1para"]);
         pdf_data_9D->GetVariable("amp_1perp")->setVal(starting_values_map["amp_1perp"]);
         pdf_data_9D->GetVariable("amp_20")->setVal(starting_values_map["amp_20"]);
         pdf_data_9D->GetVariable("amp_2para")->setVal(starting_values_map["amp_2para"]);
         pdf_data_9D->GetVariable("amp_2perp")->setVal(starting_values_map["amp_2perp"]);
         
         pdf_data_9D->GetVariable("phase_10")->setVal(starting_values_map["phase_10"]);
         pdf_data_9D->GetVariable("phase_1para")->setVal(starting_values_map["phase_1para"]);
         pdf_data_9D->GetVariable("phase_1perp")->setVal(starting_values_map["phase_1perp"]);
         pdf_data_9D->GetVariable("phase_20")->setVal(starting_values_map["phase_20"]);
         pdf_data_9D->GetVariable("phase_2para")->setVal(starting_values_map["phase_2para"]);
         pdf_data_9D->GetVariable("phase_2perp")->setVal(starting_values_map["phase_2perp"]);
         
         pdf_data_9D->GetVariable("amp_00_delta")->setVal(starting_values_map["amp_00_delta"]);
         pdf_data_9D->GetVariable("amp_10_delta")->setVal(starting_values_map["amp_10_delta"]);
         pdf_data_9D->GetVariable("amp_1para_delta")->setVal(starting_values_map["amp_1para_delta"]);
         pdf_data_9D->GetVariable("amp_1perp_delta")->setVal(starting_values_map["amp_1perp_delta"]);
         pdf_data_9D->GetVariable("amp_20_delta")->setVal(starting_values_map["amp_20_delta"]);
         pdf_data_9D->GetVariable("amp_2para_delta")->setVal(starting_values_map["amp_2para_delta"]);
         pdf_data_9D->GetVariable("amp_2perp_delta")->setVal(starting_values_map["amp_2perp_delta"]);
         
         pdf_data_9D->GetVariable("phase_10_delta")->setVal(starting_values_map["phase_10_delta"]);
         pdf_data_9D->GetVariable("phase_1para_delta")->setVal(starting_values_map["phase_1para_delta"]);
         pdf_data_9D->GetVariable("phase_1perp_delta")->setVal(starting_values_map["phase_1perp_delta"]);
         pdf_data_9D->GetVariable("phase_20_delta")->setVal(starting_values_map["phase_20_delta"]);
         pdf_data_9D->GetVariable("phase_2para_delta")->setVal(starting_values_map["phase_2para_delta"]);
         pdf_data_9D->GetVariable("phase_2perp_delta")->setVal(starting_values_map["phase_2perp_delta"]);
         
         if(i > 0) {
         double min = profile_scan_limits[profile_ll_parameter].first;
         double max = profile_scan_limits[profile_ll_parameter].second;
         double stepsize = (max - min)/steps;
         pdf_data_9D->GetVariable(profile_ll_parameter)->setVal(min + i*stepsize);
         pdf_data_9D->GetVariable(profile_ll_parameter)->setConstant(true);
         std::cout << "INFO: Scanning " << profile_ll_parameter << " from " << min << " to " << max << "\n";
         std::cout << "INFO: Fixing parameter " << profile_ll_parameter << " to value " << pdf_data_9D->GetVariable(profile_ll_parameter)->getVal() << "\n";
         }
         
         if(opt->GetFixCPV2PerpAndParaComponent()) {
         pdf_data_9D->GetVariable("amp_2para_delta")->setVal(0.0);
         pdf_data_9D->GetVariable("amp_2para_delta")->setConstant(true);
         pdf_data_9D->GetVariable("amp_2perp_delta")->setVal(0.0);
         pdf_data_9D->GetVariable("amp_2perp_delta")->setConstant(true);
         }
         
         if(opt->GetFixCPV2PerpAndParaComponent()) {
         pdf_data_9D->GetVariable("phase_2para_delta")->setVal(0.0);
         pdf_data_9D->GetVariable("phase_2para_delta")->setConstant(true);
         pdf_data_9D->GetVariable("phase_2perp_delta")->setVal(TMath::Pi()/2);
         pdf_data_9D->GetVariable("phase_2perp_delta")->setConstant(true);
         }
         
         fit_results.push_back(pdf_data_9D->GetSimPDF().fitTo(data, SumW2Error(false), NumCPU(opt->GetNumcpu()),
         Timer(true), Save(true), InitialHesse(false), Minos(false), Extended(true)));
         fit_results.back()->Print();
         
         // check if any phase is outside the [-pi,pi] region and push it back towards truth value
         // this is only to get good pulls, the other solution is not wrong
         if(i == 0) {
         ShiftPhaseToInterval(pdf_data_9D, "phase_10", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phase_1para", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phase_1perp", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phase_20", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phase_2para", 0.0);
         ShiftPhaseToInterval(pdf_data_9D, "phase_2perp", 0.0);
         }
         
         PrintFitResultLatex(pdf_data_9D);
         
         std::ofstream fout((outdir_files+"DATA_9D_fit_result_"+ss.str().c_str()+".dat").c_str());
         if(fout.is_open()) {
         for(const std::string& p : parameters) {
         fout << p << ";";
         fout << 0.0 << ";";
         fout << pdf_data_9D->GetVariable(p)->getVal() << ";";
         fout << pdf_data_9D->GetVariable(p)->getError() << ";";
         fout << pdf_data_9D->GetVariable(p)->getErrorLo() << ";";
         fout << pdf_data_9D->GetVariable(p)->getErrorHi() << std::endl;
         }
         fout << "EDM;" << fit_results.back()->edm() << std::endl;
         fout << "NLL;" << fit_results.back()->minNll() << std::endl;
         fout.close();
         } else {
         std::cerr << "ERROR: Could not write fit result to file" << std::endl;
         }
         
         if(i == 0) {
         nominal_x.push_back(pdf_data_9D->GetVariable(profile_ll_parameter)->getVal());
         nominal_y.push_back(fit_results.back()->minNll());
         } else {
         scan_x.push_back(pdf_data_9D->GetVariable(profile_ll_parameter)->getVal());
         scan_y.push_back(fit_results.back()->minNll());
         }
         }
         // End loop of fits
         
         // Draw the profile LL plot for this parameter
         for(double& d: scan_y) {
         d = 2*(d - nominal_y.back());
         }
         nominal_y.back() = 0;
         
         TCanvas* canvas = new TCanvas("canvas","canvas",1280,1024);
         canvas->cd();
         
         TGraph* nominal_graph = new TGraph(1,&nominal_x[0],&nominal_y[0]);
         nominal_graph->SetMarkerStyle(8);
         nominal_graph->SetMarkerSize(3.0);
         nominal_graph->SetMarkerColor(kOrange);
         nominal_graph->SetLineColor(kOrange);
         nominal_graph->SetLineWidth(5);
         
         TGraph* scan_graph = new TGraph(scan_x.size(),&scan_x[0],&scan_y[0]);
         scan_graph->SetMarkerStyle(8);
         scan_graph->SetMarkerSize(3.0);
         scan_graph->SetMarkerColor(kRed);
         scan_graph->SetLineColor(kRed);
         scan_graph->SetLineWidth(5);
         
         TMultiGraph* multigraph = new TMultiGraph("profile_scan","profile_scan");
         // multigraph takes ownership of scan_graph and nominal_graph
         multigraph->Add(scan_graph);
         multigraph->Add(nominal_graph);
         multigraph->Draw("AP");
         
         multigraph->GetXaxis()->SetTitle(FFUtil::GetLabelToTLatex(profile_ll_parameter).c_str());
         multigraph->GetYaxis()->SetTitle("2#DeltalogL");
         multigraph->SetMinimum(0.0);
         
         canvas->Update();
         
         canvas->SaveAs((opt->GetOutdir()+"profile_LL_"+profile_ll_parameter+"."+opt->GetPlotFormat()).c_str());
         canvas->SaveAs((opt->GetOutdir()+"profile_LL_"+profile_ll_parameter+".root").c_str());
         
         canvas->Clear();
         
         delete multigraph;
         delete canvas;
         
         delete mass_helicity_angles_imag_cache_integrator;
         delete mass_helicity_angles_imag_cache_integrator_m_phi_th1;
         delete mass_helicity_angles_imag_cache_integrator_m_phi_th2;
         delete mass_helicity_angles_imag_cache_integrator_m_th1_th2;
         delete mass_helicity_angles_imag_cache_integrator_phi_th1_th2;
         delete mass_helicity_angles_norm_cache_integrator;
         delete mass_helicity_angles_norm_cache_integrator_m_phi_th1;
         delete mass_helicity_angles_norm_cache_integrator_m_phi_th2;
         delete mass_helicity_angles_norm_cache_integrator_m_th1_th2;
         delete mass_helicity_angles_norm_cache_integrator_phi_th1_th2;
         delete mass_helicity_angles_real_cache_integrator;
         delete mass_helicity_angles_real_cache_integrator_m_phi_th1;
         delete mass_helicity_angles_real_cache_integrator_m_phi_th2;
         delete mass_helicity_angles_real_cache_integrator_m_th1_th2;
         delete mass_helicity_angles_real_cache_integrator_phi_th1_th2;
         
         for(RooFitResult* r : fit_results) {
         delete r;
         }
         delete result_9D_sideband;
         delete result_mass_kpi;*/
    }
    
    void FitDataWithLikelihoodScan_CleanVersion(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt) {
        /*
         if(chain->GetEntries() == 0) {
         std::cout << "No data for final fits given, skip fitting." << std::endl;
         return;
         }
         
         // ---------------------------------------------
         // ----- Parse configuration
         // ---------------------------------------------
         std::cout << "Reading fit configuration from " << opt->GetConfigFile() << std::endl;
         namespace bpt = boost::property_tree;
         bpt::ptree pt;
         bpt::info_parser::read_info(opt->GetConfigFile(),pt);
         
         std::cout << "Reading profile LL configuration from " << opt->GetProfileLLConfigFile() << std::endl;
         bpt::ptree pt_profile_ll;
         bpt::info_parser::read_info(opt->GetProfileLLConfigFile(),pt_profile_ll);
         
         // ---------------------------------------------
         // ----- Define variables
         // ---------------------------------------------
         PrintFitStartBlock("STATUS: Define functions and variables...");
         RooRealVar mbc("mbc","m_{bc}",5.24,5.29,"GeV");
         RooRealVar deltae("deltae","#Delta E",-0.15,0.15,"GeV");
         RooRealVar chil0_ma("chil0_ma","m_{KK}",0.985,1.05,"GeV");
         RooRealVar mass_kpi("mass_kpi","m_{K#pi}",0.7,1.55,"GeV");
         RooRealVar hel_phi("hel_phi","#Phi",-TMath::Pi(),TMath::Pi(),"rad");
         RooRealVar hel_th1("hel_th1","cos #Theta_{1}",-1.0,1.0,"");
         RooRealVar hel_th2("hel_th2","cos #Theta_{2}",-1.0,1.0,"");
         RooRealVar nb_prime("nb_prime","C_{NB}'",-10.0,10.0,"");
         RooCategory channel("channel","decay channel");
         channel.defineType("C",0);
         channel.defineType("N",1);
         RooCategory datatype("datatype","data sample type");
         datatype.defineType("CP",0);
         datatype.defineType("CA",1);
         datatype.defineType("N",2);
         datatype.defineType("CS",3);
         datatype.defineType("NS",4);
         datatype.defineType("COFF",5);
         datatype.defineType("NOFF",6);
         RooCategory flavor("flavor","flavor type");
         flavor.defineType("CP",0);
         flavor.defineType("CA",1);
         // necessary for MC information checks
         RooRealVar mcinfo("mcinfo","MC information",0,256);
         RooRealVar mctype("mctype","MC type information",-1,1005);
         RooRealVar chil0_mo("chil0_mo","MC #phi mother",-999999,999999);
         RooRealVar chil0_mc("chil0_mc","MC information #phi",0,256);
         RooRealVar runno("runno","Run Number",-999999,999999);
         // create data set (constructor only overload for max. 9 arguments, so we add some afterwards)
         RooArgSet varset(mbc,deltae,chil0_ma,mass_kpi,hel_phi,hel_th1,hel_th2,nb_prime);
         varset.add(channel);
         varset.add(datatype);
         varset.add(flavor);
         varset.add(mcinfo);
         varset.add(mctype);
         varset.add(chil0_mo);
         varset.add(chil0_mc);
         varset.add(runno);
         
         TCut cut_default_CP_and_CA = "(mbc < 5.289 && hel_th1 < 0.75 && (datatype==datatype::CP || datatype==datatype::CA))";
         TCut cut_default_CS = "(mbc < 5.289 && hel_th1 < 0.75 && (datatype==datatype::CS))";
         
         RooDataSet data("data","data",varset,Import(*chain.get()),Cut(cut_default_CP_and_CA));
         RooDataSet data_sideband("data_sideband","data_sideband",varset,Import(*chain.get()),Cut(cut_default_CS));
         
         // create helper for the simultaneous fit
         std::vector<std::string> flags_channel;
         flags_channel.push_back("C");
         flags_channel.push_back("N");
         tuple_category_name_flags category_name_flags_channel = std::make_tuple(&channel,"channel",flags_channel);
         
         std::vector<RooRealVar*> variables_8D;
         variables_8D.push_back(&mbc);
         variables_8D.push_back(&deltae);
         variables_8D.push_back(&chil0_ma);
         variables_8D.push_back(&nb_prime);
         variables_8D.push_back(&mass_kpi);
         variables_8D.push_back(&hel_phi);
         variables_8D.push_back(&hel_th1);
         variables_8D.push_back(&hel_th2);
         RooArgSet varset_9D(mbc,deltae,chil0_ma,nb_prime,mass_kpi,hel_phi,hel_th1,hel_th2,flavor);
         
         std::vector<RooRealVar*> variables_4D;
         variables_4D.push_back(&mass_kpi);
         variables_4D.push_back(&hel_phi);
         variables_4D.push_back(&hel_th1);
         variables_4D.push_back(&hel_th2);
         RooArgSet varset_5D(mbc,deltae,chil0_ma,nb_prime,flavor);
         
         // ---------------------------------------------
         // ----- Define full data range
         // ---------------------------------------------
         SetFitRanges(mbc,deltae,chil0_ma,mass_kpi,hel_phi,hel_th1,hel_th2,nb_prime);
         // ---------------------------------------------
         // ----- Do fits
         // ---------------------------------------------
         // See RooFit tutorial macro #501 about HowTo do a simultaneous fit
         // See RooFit tutorial macro #603 about HowTo do a multi-core parallelized unbinned maximum likelihood fit
         std::cout << "STATUS: Do fits..." << std::endl;
         
         PrintFitStartBlock("SETUP: CacheIntegrator");
         int debug_level = 1;
         CacheIntegrator<MassHelicityAnglesNormCachedIntegral>* mass_helicity_angles_norm_cache_integrator = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral>(debug_level);
         CacheIntegrator<MassHelicityAnglesRealCachedIntegral>* mass_helicity_angles_real_cache_integrator = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral>(debug_level);
         CacheIntegrator<MassHelicityAnglesImagCachedIntegral>* mass_helicity_angles_imag_cache_integrator = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral>(debug_level);
         CacheIntegrator<MassHelicityAnglesNormCachedIntegral_phi_th1_th2>* mass_helicity_angles_norm_cache_integrator_phi_th1_th2 = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral_phi_th1_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesRealCachedIntegral_phi_th1_th2>* mass_helicity_angles_real_cache_integrator_phi_th1_th2 = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral_phi_th1_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesImagCachedIntegral_phi_th1_th2>* mass_helicity_angles_imag_cache_integrator_phi_th1_th2 = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral_phi_th1_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_th1_th2>* mass_helicity_angles_norm_cache_integrator_m_th1_th2 = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_th1_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_th1_th2>* mass_helicity_angles_real_cache_integrator_m_th1_th2 = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_th1_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_th1_th2>* mass_helicity_angles_imag_cache_integrator_m_th1_th2 = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_th1_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_phi_th2>* mass_helicity_angles_norm_cache_integrator_m_phi_th2 = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_phi_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_phi_th2>* mass_helicity_angles_real_cache_integrator_m_phi_th2 = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_phi_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_phi_th2>* mass_helicity_angles_imag_cache_integrator_m_phi_th2 = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_phi_th2>(debug_level);
         CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_phi_th1>* mass_helicity_angles_norm_cache_integrator_m_phi_th1 = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_phi_th1>(debug_level);
         CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_phi_th1>* mass_helicity_angles_real_cache_integrator_m_phi_th1 = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_phi_th1>(debug_level);
         CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_phi_th1>* mass_helicity_angles_imag_cache_integrator_m_phi_th1 = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_phi_th1>(debug_level);
         
         ACCEPTANCE_FUNCTION acceptance = PRODUCT_4D;
         std::string path_to_integrals = "./CachedIntegrals_ACCEPTANCE" + FFUtil::GetAcceptanceString(acceptance) + "/";
         mass_helicity_angles_norm_cache_integrator->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_real_cache_integrator->LoadIntegrals(path_to_integrals);
         mass_helicity_angles_imag_cache_integrator->LoadIntegrals(path_to_integrals);
         
         MHI_PDG_constants mhi_pdg_constants = {m_Pi, m_K, m_Phi, m_B0};
         MHI_CacheIntegrators_4D mhi_4D = {
         mass_helicity_angles_norm_cache_integrator,
         mass_helicity_angles_real_cache_integrator,
         mass_helicity_angles_imag_cache_integrator
         };
         MHI_CacheIntegrators_3D_phi_th1_th2 mhi_3D_phi_th1_th2 = {
         mass_helicity_angles_norm_cache_integrator_phi_th1_th2,
         mass_helicity_angles_real_cache_integrator_phi_th1_th2,
         mass_helicity_angles_imag_cache_integrator_phi_th1_th2
         };
         MHI_CacheIntegrators_3D_m_th1_th2 mhi_3D_m_th1_th2 = {
         mass_helicity_angles_norm_cache_integrator_m_th1_th2,
         mass_helicity_angles_real_cache_integrator_m_th1_th2,
         mass_helicity_angles_imag_cache_integrator_m_th1_th2
         };
         MHI_CacheIntegrators_3D_m_phi_th2 mhi_3D_m_phi_th2 = {
         mass_helicity_angles_norm_cache_integrator_m_phi_th2,
         mass_helicity_angles_real_cache_integrator_m_phi_th2,
         mass_helicity_angles_imag_cache_integrator_m_phi_th2
         };
         MHI_CacheIntegrators_3D_m_phi_th1 mhi_3D_m_phi_th1 = {
         mass_helicity_angles_norm_cache_integrator_m_phi_th1,
         mass_helicity_angles_real_cache_integrator_m_phi_th1,
         mass_helicity_angles_imag_cache_integrator_m_phi_th1
         };
         
         PrintFitStartBlock("SETUP: 9D Fit");
         
         shared_ptr<FF_PDF> pdf_bkg_continuum_9D = make_shared<FF_PDF_bkg_continuum_9D_mbc_deltae_chil0_ma_mass_kpi_hel_phi_hel_th1_hel_th2_nb_prime_flavor>(mbc,
         deltae, chil0_ma, mass_kpi, hel_phi, hel_th1,
         hel_th2, nb_prime, flavor, category_name_flags_channel,true,(RooRealVar*)NULL);
         pdf_bkg_continuum_9D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_continuum_8D.dat",true);
         pdf_bkg_continuum_9D->GetVariable("N_chil0_ma_bkg_continuum_nonres_fraction")->setConstant(true);
         pdf_bkg_continuum_9D->GetVariable("N_hel_th1_bkg_continuum_a1_polyvar_mass_kpi_c0")->setConstant(true);
         pdf_bkg_continuum_9D->GetVariable("N_hel_th1_bkg_continuum_a2_polyvar_mass_kpi_c1")->setConstant(true);
         pdf_bkg_continuum_9D->GetVariable("N_hel_th1_bkg_continuum_a3_polyvar_mass_kpi_c0")->setConstant(true);
         pdf_bkg_continuum_9D->GetVariable("N_mass_kpi_bkg_continuum_a1")->setConstant(true);
         pdf_bkg_continuum_9D->GetVariable("N_mass_kpi_bkg_continuum_a2")->setConstant(true);
         pdf_bkg_continuum_9D->GetVariable("N_mass_kpi_bkg_continuum_a3")->setConstant(true);
         pdf_bkg_continuum_9D->GetVariable("N_mass_kpi_bkg_continuum_nonres_fraction")->setConstant(true);
         //pdf_bkg_continuum_9D->SetVariablesConst();
         shared_ptr<FF_PDF> pdf_signal_4D = make_shared<FF_PDF_signal_4D_mbc_deltae_chil0_ma_nb_prime>(mbc,
         deltae, chil0_ma, nb_prime, category_name_flags_channel,true,pdf_bkg_continuum_9D->GetVariable("chil0_ma_resolution"));
         pdf_signal_4D->ImportPDFValues(opt->GetOutdir()+"pdf_signal_4D.dat",true);
         // scale Delta E resolution
         double scale = 1.124;
         pdf_signal_4D->GetVariable("C_deltae_signal_width_core")->setVal(pdf_signal_4D->GetVariable("C_deltae_signal_width_core")->getVal()*scale);
         pdf_signal_4D->GetVariable("C_deltae_signal_width_1st_left")->setVal(pdf_signal_4D->GetVariable("C_deltae_signal_width_1st_left")->getVal()*scale);
         pdf_signal_4D->GetVariable("C_deltae_signal_width_1st_right")->setVal(pdf_signal_4D->GetVariable("C_deltae_signal_width_1st_right")->getVal()*scale);
         pdf_signal_4D->GetVariable("C_deltae_signal_width_2nd_left")->setVal(pdf_signal_4D->GetVariable("C_deltae_signal_width_2nd_left")->getVal()*scale);
         pdf_signal_4D->GetVariable("C_deltae_signal_width_2nd_right")->setVal(pdf_signal_4D->GetVariable("C_deltae_signal_width_2nd_right")->getVal()*scale);
         pdf_signal_4D->SetVariablesConst();
         //pdf_signal_4D->GetVariable("C_deltae_signal_width_core")->setConstant(false);
         shared_ptr<FF_PDF> pdf_signal_5D_angles = make_shared<FF_PDF_signal_5D_mass_helicity_angles_CPV_PhysBase>(mass_kpi,
         hel_th1, hel_th2, hel_phi, flavor, acceptance,
         mhi_pdg_constants, mhi_4D,
         mhi_3D_phi_th1_th2, mhi_3D_m_th1_th2, mhi_3D_m_phi_th2, mhi_3D_m_phi_th1,
         category_name_flags_channel);
         shared_ptr<FF_PDF> pdf_signal_9D = make_shared<FF_PDF_signal_9D>(pdf_signal_4D, pdf_signal_5D_angles,
         category_name_flags_channel);
         shared_ptr<FF_PDF> pdf_bkg_bbar_wrongphi_9D = make_shared<FF_PDF_bkg_bbar_wrongphif0980_9D>(mbc,
         deltae, chil0_ma, nb_prime, mass_kpi,
         hel_phi, hel_th1, hel_th2, flavor, category_name_flags_channel, true);
         pdf_bkg_bbar_wrongphi_9D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongphif0980_8D.dat",true);
         pdf_bkg_bbar_wrongphi_9D->SetVariablesConst();
         
         shared_ptr<FF_PDF> pdf_data_9D = make_shared<FF_PDF_data_9D__CPV__KK_Flatte>(pdf_signal_9D,
         pdf_bkg_continuum_9D, pdf_bkg_bbar_wrongphi_9D,
         data.sumEntries(), category_name_flags_channel);
         
         PrintFitStartBlock("FITTING: 9D Sideband Fit");
         
         // fit the continuum pdf to sideband data and fix the shape
         mbc.setMin(5.24);
         mbc.setMax(5.26);
         mbc.setRange("full_range",5.24,5.26);
         
         // fit the 1D mass_kpi distribution due to some instability
         shared_ptr<FF_PDF> pdf_bkg_continuum_1D_mass_kpi = make_shared<FF_PDF_bkg_continuum_1D_mass_kpi>(mass_kpi, category_name_flags_channel, false);
         pdf_bkg_continuum_1D_mass_kpi->GetVariable("N_mass_kpi_bkg_continuum_a1")->setConstant(true);
         pdf_bkg_continuum_1D_mass_kpi->GetVariable("N_mass_kpi_bkg_continuum_a2")->setConstant(true);
         pdf_bkg_continuum_1D_mass_kpi->GetVariable("N_mass_kpi_bkg_continuum_a3")->setConstant(true);
         pdf_bkg_continuum_1D_mass_kpi->GetVariable("N_mass_kpi_bkg_continuum_nonres_fraction")->setConstant(true);
         RooFitResult *result_mass_kpi = pdf_bkg_continuum_1D_mass_kpi->GetSimPDF().fitTo(data_sideband, SumW2Error(false), NumCPU(opt->GetNumcpu()),
         Timer(true), Save(true), InitialHesse(false), Minos(false), Extended(false));
         pdf_bkg_continuum_1D_mass_kpi->GetSimPDF().getParameters(data_sideband)->Print("v");
         result_mass_kpi->Print();
         // set mass_kpi distribution and fix it
         for(RooRealVar *v : pdf_bkg_continuum_1D_mass_kpi->GetVariables()) {
         pdf_bkg_continuum_9D->SetVariable(v->GetName(),v->getVal(),v->getError(),true,false);
         }
         
         RooFitResult *result_9D_sideband = pdf_bkg_continuum_9D->GetSimPDF().fitTo(data_sideband, SumW2Error(false), NumCPU(opt->GetNumcpu()),
         Timer(true), Save(true), InitialHesse(false), Minos(false), Extended(false));
         pdf_bkg_continuum_9D->GetSimPDF().getParameters(data_sideband)->Print("v");
         result_9D_sideband->Print();
         
         // fit the fit region data with fixed pdf
         mbc.setMax(5.29);
         mbc.setMin(5.26);
         mbc.setRange("full_range",5.26,5.29);
         pdf_bkg_continuum_9D->SetVariablesConst();
         pdf_bkg_continuum_9D->GetVariable("mbc_bkg_continuum_c")->setConstant(false);
         
         PrintFitStartBlock("CREATING TRUTH MAP");
         std::map<std::string, double> starting_values_map;
         starting_values_map["amp_00"] = pt.get<double>("amp_00");
         starting_values_map["amp_10"] = pt.get<double>("amp_10");
         starting_values_map["amp_1para"] = pt.get<double>("amp_1para");
         starting_values_map["amp_1perp"] = pt.get<double>("amp_1perp");
         starting_values_map["amp_20"] = pt.get<double>("amp_20");
         starting_values_map["amp_2para"] = pt.get<double>("amp_2para");
         starting_values_map["amp_2perp"] = pt.get<double>("amp_2perp");
         starting_values_map["phase_00"] = 0.0;
         starting_values_map["phi_01"] = pt.get<double>("phi_01");
         starting_values_map["phi_1para"] = pt.get<double>("phi_1para");
         starting_values_map["phi_1perp"] = pt.get<double>("phi_1perp");
         starting_values_map["phi_02"] = pt.get<double>("phi_02");
         starting_values_map["phi_2para"] = pt.get<double>("phi_2para");
         starting_values_map["phi_2perp"] = pt.get<double>("phi_2perp");
         starting_values_map["amp_00_delta"] = pt.get<double>("amp_00_delta");
         starting_values_map["amp_10_delta"] = pt.get<double>("amp_10_delta");
         starting_values_map["amp_1para_delta"] = pt.get<double>("amp_1para_delta");
         starting_values_map["amp_1perp_delta"] = pt.get<double>("amp_1perp_delta");
         starting_values_map["amp_20_delta"] = pt.get<double>("amp_20_delta");
         starting_values_map["amp_2para_delta"] = pt.get<double>("amp_2para_delta");
         starting_values_map["amp_2perp_delta"] = pt.get<double>("amp_2perp_delta");
         starting_values_map["phase_00_delta"] = 0.0;
         starting_values_map["phi_01_delta"] = pt.get<double>("phi_01_delta");
         starting_values_map["phi_1para_delta"] = pt.get<double>("phi_1para_delta");
         starting_values_map["phi_1perp_delta"] = pt.get<double>("phi_1perp_delta");
         starting_values_map["phi_02_delta"] = pt.get<double>("phi_02_delta");
         starting_values_map["phi_2para_delta"] = pt.get<double>("phi_2para_delta");
         starting_values_map["phi_2perp_delta"] = pt.get<double>("phi_2perp_delta");
         starting_values_map["signal_nevents"] = pt.get<double>("signal_nevents");
         starting_values_map["C_bkg_continuum_nevents"] = pt.get<double>("C_bkg_continuum_nevents");
         starting_values_map["C_bkg_bbar_wrongphif0980_nevents"] = pt.get<double>("C_bkg_bbar_wrongphif0980_nevents");
         starting_values_map["mbc_bkg_continuum_c"] = pt.get<double>("mbc_bkg_continuum_c");
         
         std::vector<std::string> parameters;
         parameters.push_back("signal_nevents");
         parameters.push_back("C_bkg_continuum_nevents");
         parameters.push_back("C_bkg_bbar_wrongphif0980_nevents");
         parameters.push_back("mbc_bkg_continuum_c");
         parameters.push_back("amp_00");
         //parameters.push_back("amp_10");
         parameters.push_back("amp_1para");
         parameters.push_back("amp_1perp");
         parameters.push_back("amp_20");
         parameters.push_back("amp_2para");
         parameters.push_back("amp_2perp");
         parameters.push_back("phi_01");
         parameters.push_back("phi_1para");
         parameters.push_back("phi_1perp");
         parameters.push_back("phi_02");
         parameters.push_back("phi_2para");
         parameters.push_back("phi_2perp");
         parameters.push_back("amp_00_delta");
         parameters.push_back("amp_10_delta");
         parameters.push_back("amp_1para_delta");
         parameters.push_back("amp_1perp_delta");
         parameters.push_back("amp_20_delta");
         if(!opt->GetFixCPV2PerpAndParaComponent()) {
         parameters.push_back("amp_2para_delta");
         parameters.push_back("amp_2perp_delta");
         }
         parameters.push_back("phi_01_delta");
         parameters.push_back("phi_1para_delta");
         parameters.push_back("phi_1perp_delta");
         parameters.push_back("phi_02_delta");
         if(!opt->GetFixCPV2PerpAndParaComponent()) {
         parameters.push_back("phi_2para_delta");
         parameters.push_back("phi_2perp_delta");
         }
         
         std::vector<std::string> profile_var_names;
         std::map<std::string, int> var_to_index;
         std::vector<double> nominal_scan_var;
         double nominal_LL = 0;
         std::vector<std::vector<double>> profile_scan_var;
         std::vector<double> profile_LL;
         std::vector<unsigned int> profile_scan_var_pos;
         unsigned int total_scan_points = 1;
         
         unsigned int nvars = pt_profile_ll.get<unsigned int>("scan_vars");
         for(unsigned int i = 1; i <= nvars; ++i) {
         std::string s = "var" + mp::to_string(i);
         
         profile_var_names.push_back(pt_profile_ll.get<std::string>(s + ".name"));
         var_to_index[profile_var_names.back()] = i-1;
         nominal_scan_var.push_back(0.0);
         
         double varmin = pt_profile_ll.get<double>(s + ".min");
         double varmax = pt_profile_ll.get<double>(s + ".max");
         double stepsize = pt_profile_ll.get<double>(s + ".stepsize");
         
         std::vector<double> scan_points;
         for(double d = varmin; d <= varmax; d += stepsize) {
         scan_points.push_back(d);
         }
         profile_scan_var.push_back(scan_points);
         profile_scan_var_pos.push_back(0);
         total_scan_points *= scan_points.size();
         }
         
         std::cout << "Scanning variables: \n";
         std::cout << mp::vector_to_string(profile_var_names, true);
         for(std::size_t i = 0; i < profile_var_names.size(); ++i) {
         std::cout << "Scanning positions for variable " << i << "\n";
         std::cout << mp::vector_to_string(profile_scan_var[i]);
         }
         
         PrintFitStartBlock("REPEATING 9D FITS");
         std::vector<RooFitResult*> fit_results;
         std::string profile_ll_parameter = "";
         for(std::string s : profile_var_names) {
         profile_ll_parameter += "_" + s;
         }
         std::string outdir_files = opt->GetOutdir()+"/data_fit_profile_ll" + profile_ll_parameter +"/";
         mp::execute_command("mkdir -p "+outdir_files);
         // start loop of fits
         for(unsigned int i = 0; i <= total_scan_points; ++i) {
         std::stringstream ss;
         ss << "sample_" << i;
         std::cout << "----------------------------------------------------" << std::endl;
         std::cout << "INFO: Data fit: " << i << std::endl;
         
         // reset to fit start values
         pdf_data_9D->GetVariable("amp_00")->setVal(starting_values_map["amp_00"]);
         pdf_data_9D->GetVariable("amp_10")->setVal(starting_values_map["amp_10"]);
         pdf_data_9D->GetVariable("amp_1para")->setVal(starting_values_map["amp_1para"]);
         pdf_data_9D->GetVariable("amp_1perp")->setVal(starting_values_map["amp_1perp"]);
         pdf_data_9D->GetVariable("amp_20")->setVal(starting_values_map["amp_20"]);
         pdf_data_9D->GetVariable("amp_2para")->setVal(starting_values_map["amp_2para"]);
         pdf_data_9D->GetVariable("amp_2perp")->setVal(starting_values_map["amp_2perp"]);
         
         pdf_data_9D->GetVariable("phi_01")->setVal(starting_values_map["phi_01"]);
         pdf_data_9D->GetVariable("phi_1para")->setVal(starting_values_map["phi_1para"]);
         pdf_data_9D->GetVariable("phi_1perp")->setVal(starting_values_map["phi_1perp"]);
         pdf_data_9D->GetVariable("phi_02")->setVal(starting_values_map["phi_02"]);
         pdf_data_9D->GetVariable("phi_2para")->setVal(starting_values_map["phi_2para"]);
         pdf_data_9D->GetVariable("phi_2perp")->setVal(starting_values_map["phi_2perp"]);
         
         pdf_data_9D->GetVariable("amp_00_delta")->setVal(starting_values_map["amp_00_delta"]);
         pdf_data_9D->GetVariable("amp_10_delta")->setVal(starting_values_map["amp_10_delta"]);
         pdf_data_9D->GetVariable("amp_1para_delta")->setVal(starting_values_map["amp_1para_delta"]);
         pdf_data_9D->GetVariable("amp_1perp_delta")->setVal(starting_values_map["amp_1perp_delta"]);
         pdf_data_9D->GetVariable("amp_20_delta")->setVal(starting_values_map["amp_20_delta"]);
         pdf_data_9D->GetVariable("amp_2para_delta")->setVal(starting_values_map["amp_2para_delta"]);
         pdf_data_9D->GetVariable("amp_2perp_delta")->setVal(starting_values_map["amp_2perp_delta"]);
         
         pdf_data_9D->GetVariable("phi_01_delta")->setVal(starting_values_map["phi_01_delta"]);
         pdf_data_9D->GetVariable("phi_1para_delta")->setVal(starting_values_map["phi_1para_delta"]);
         pdf_data_9D->GetVariable("phi_1perp_delta")->setVal(starting_values_map["phi_1perp_delta"]);
         pdf_data_9D->GetVariable("phi_02_delta")->setVal(starting_values_map["phi_02_delta"]);
         pdf_data_9D->GetVariable("phi_2para_delta")->setVal(starting_values_map["phi_2para_delta"]);
         pdf_data_9D->GetVariable("phi_2perp_delta")->setVal(starting_values_map["phi_2perp_delta"]);
         
         if(i > 0) {
         for(std::size_t var_index = 0; var_index < profile_var_names.size(); ++var_index) {
         const std::string& s = profile_var_names[var_index];
         // 1D scan
         if(total_scan_points == profile_scan_var[var_index].size()) {
         pdf_data_9D->GetVariable(s)->setVal(profile_scan_var[var_index][i-1]);
         pdf_data_9D->GetVariable(s)->setConstant(true);
         std::cout << "INFO: Scanning " << s << " at " << pdf_data_9D->GetVariable(s)->getVal() << "\n";
         } else {
         pdf_data_9D->GetVariable(s)->setVal(profile_scan_var[var_index][profile_scan_var_pos[var_index]]);
         pdf_data_9D->GetVariable(s)->setConstant(true);
         std::cout << "INFO: Scanning " << s << " at " << pdf_data_9D->GetVariable(s)->getVal() << "\n";
         }
         }
         }
         
         if(opt->GetFixCPV2PerpAndParaComponent()) {
         pdf_data_9D->GetVariable("amp_2para_delta")->setVal(0.0);
         pdf_data_9D->GetVariable("amp_2para_delta")->setConstant(true);
         pdf_data_9D->GetVariable("amp_2perp_delta")->setVal(0.0);
         pdf_data_9D->GetVariable("amp_2perp_delta")->setConstant(true);
         }
         
         if(opt->GetFixCPV2PerpAndParaComponent()) {
         pdf_data_9D->GetVariable("phase_2para_delta")->setVal(0.0);
         pdf_data_9D->GetVariable("phase_2para_delta")->setConstant(true);
         pdf_data_9D->GetVariable("phase_2perp_delta")->setVal(TMath::Pi()/2);
         pdf_data_9D->GetVariable("phase_2perp_delta")->setConstant(true);
         }
         
         fit_results.push_back(pdf_data_9D->GetSimPDF().fitTo(data, SumW2Error(false), NumCPU(opt->GetNumcpu()),
         Timer(true), Save(true), InitialHesse(false), Minos(false), Extended(true)));
         fit_results.back()->Print();
         
         // check if any phase is outside the [-pi,pi] region and push it back towards truth value
         // this is only to get good pulls, the other solution is not wrong
         if(i == 0) {
         ShiftPhaseToInterval(pdf_data_9D, "phi_01", TMath::Pi());
         ShiftPhaseToInterval(pdf_data_9D, "phi_1para", TMath::Pi());
         ShiftPhaseToInterval(pdf_data_9D, "phi_1perp", TMath::Pi());
         ShiftPhaseToInterval(pdf_data_9D, "phi_02", TMath::Pi());
         ShiftPhaseToInterval(pdf_data_9D, "phi_2para", TMath::Pi());
         ShiftPhaseToInterval(pdf_data_9D, "phi_2perp", TMath::Pi());
         }
         
         //PrintFitResultLatex(pdf_data_9D);
         //auto physics_param = ComputePhysicsParameters(pdf_data_9D,pdf_signal_9D,varset_9D,false);
         
         //		std::ofstream fout((outdir_files+"DATA_9D_fit_result_"+ss.str().c_str()+".dat").c_str());
         //		if(fout.is_open()) {
         //			for(const std::string& p : parameters) {
         //				fout << p << ";";
         //				fout << 0.0 << ";";
         //				fout << pdf_data_9D->GetVariable(p)->getVal() << ";";
         //				fout << pdf_data_9D->GetVariable(p)->getError() << ";";
         //				fout << pdf_data_9D->GetVariable(p)->getErrorLo() << ";";
         //				fout << pdf_data_9D->GetVariable(p)->getErrorHi() << std::endl;
         //			}
         //			fout << "EDM;" << fit_results.back()->edm() << std::endl;
         //			fout << "NLL;" << fit_results.back()->minNll() << std::endl;
         //			fout.close();
         //		} else {
         //			std::cerr << "ERROR: Could not write fit result to file" << std::endl;
         //		}
         
         if(i == 0) {
         std::stringstream profile_ss;
         profile_ss.precision(12);
         for(std::size_t var_index = 0; var_index < profile_var_names.size(); ++var_index) {
         const std::string& s = profile_var_names[var_index];
         // 1D scan
         if(profile_var_names.size() == 1) {
         nominal_scan_var[var_index] = pdf_data_9D->GetVariable(s)->getVal();
         nominal_LL = fit_results.back()->minNll();
         
         profile_ss << "PROFILE_LL_FIT: " << s << ";NLL;MIGRAD;HESSE" << "\n";
         profile_ss << "PROFILE_LL_FIT: " << nominal_scan_var[var_index] << ";" << nominal_LL << "\n";
         } else {
         nominal_scan_var[var_index] = pdf_data_9D->GetVariable(s)->getVal();
         nominal_LL = fit_results.back()->minNll();
         if(var_index == 0) {
         profile_ss << "PROFILE_LL_FIT: ";
         for(std::string l : profile_var_names)
         profile_ss << l << ";";
         profile_ss << "NLL;MIGRAD;HESSE" << "\n";
         
         //profile_ss << "PROFILE_LL_FIT: ";
         }
         //profile_ss << nominal_scan_var[var_index] << ";";
         //if(var_index == profile_var_names.size() - 1)
         //	profile_ss << nominal_LL << "\n";
         }
         }
         std::cout << profile_ss.str();
         } else {
         std::stringstream profile_ss;
         profile_ss.precision(12);
         for(std::size_t var_index = 0; var_index < profile_var_names.size(); ++var_index) {
         const std::string& s = profile_var_names[var_index];
         // 1D scan
         if(profile_var_names.size() == 1) {
         profile_ss << "Scan position and parameter value: " << profile_scan_var[var_index][i-1] << " == " << pdf_data_9D->GetVariable(s)->getVal() << "\n";
         profile_LL.push_back(fit_results.back()->minNll());
         
         profile_ss << "PROFILE_LL_FIT: " << profile_scan_var[var_index][i-1] << ";" << profile_LL.back() << ";"
         << fit_results.back()->statusCodeHistory(0) << ";" << fit_results.back()->statusCodeHistory(1) << "\n";
         } else {
         profile_LL.push_back(fit_results.back()->minNll());
         
         if(var_index == 0) {
         profile_ss << "PROFILE_LL_FIT: ";
         }
         profile_ss << profile_scan_var[var_index][profile_scan_var_pos[var_index]] << ";";
         if(var_index == profile_var_names.size() - 1)
         profile_ss << profile_LL.back() << ";" << fit_results.back()->statusCodeHistory(0) << ";" << fit_results.back()->statusCodeHistory(1) << "\n";
         }
         }
         std::cout << profile_ss.str();
         
         for(int var_index = profile_var_names.size() - 1; var_index >= 0; --var_index) {
         // increase current var position
         if(profile_scan_var_pos[var_index] < profile_scan_var[var_index].size() - 1) {
         ++profile_scan_var_pos[var_index];
         break;
         } else {
         // if end is reached, reset to 0 and continue with next war to increase
         profile_scan_var_pos[var_index] = 0;
         continue;
         }
         }
         }
         }
         // End loop of fits
         
         delete mass_helicity_angles_imag_cache_integrator;
         delete mass_helicity_angles_imag_cache_integrator_m_phi_th1;
         delete mass_helicity_angles_imag_cache_integrator_m_phi_th2;
         delete mass_helicity_angles_imag_cache_integrator_m_th1_th2;
         delete mass_helicity_angles_imag_cache_integrator_phi_th1_th2;
         delete mass_helicity_angles_norm_cache_integrator;
         delete mass_helicity_angles_norm_cache_integrator_m_phi_th1;
         delete mass_helicity_angles_norm_cache_integrator_m_phi_th2;
         delete mass_helicity_angles_norm_cache_integrator_m_th1_th2;
         delete mass_helicity_angles_norm_cache_integrator_phi_th1_th2;
         delete mass_helicity_angles_real_cache_integrator;
         delete mass_helicity_angles_real_cache_integrator_m_phi_th1;
         delete mass_helicity_angles_real_cache_integrator_m_phi_th2;
         delete mass_helicity_angles_real_cache_integrator_m_th1_th2;
         delete mass_helicity_angles_real_cache_integrator_phi_th1_th2;
         
         for(RooFitResult* r : fit_results) {
         delete r;
         }
         delete result_9D_sideband;
         delete result_mass_kpi;*/
    }
    
    void FitDataSideband(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt) {
        /*
         if(chain->GetEntries() == 0) {
         std::cout << "No data for final fits given, skip fitting." << std::endl;
         return;
         }
         
         // ---------------------------------------------
         // ----- Define variables
         // ---------------------------------------------
         PrintFitStartBlock("STATUS: Define functions and variables...");
         RooRealVar mbc("mbc","m_{bc}",5.24,5.26,"GeV");
         RooRealVar deltae("deltae","#Delta E",-0.15,0.15,"GeV");
         RooRealVar chil0_ma("chil0_ma","m_{KK}",0.985,1.05,"GeV");
         RooRealVar mass_kpi("mass_kpi","m_{K#pi}",0.7,1.55,"GeV");
         RooRealVar hel_phi("hel_phi","#Phi",-TMath::Pi(),TMath::Pi(),"rad");
         RooRealVar hel_th1("hel_th1","cos #Theta_{1}",-1.0,1.0,"");
         RooRealVar hel_th2("hel_th2","cos #Theta_{2}",-1.0,1.0,"");
         RooRealVar nb_prime("nb_prime","C_{NB}'",-10.0,10.0,"");
         RooCategory flavor("flavor","flavor type");
         flavor.defineType("CP",0);
         flavor.defineType("CA",1);
         RooCategory datatype("datatype","data sample type");
         datatype.defineType("CP",0);
         datatype.defineType("CA",1);
         datatype.defineType("N",2);
         datatype.defineType("CS",3);
         datatype.defineType("NS",4);
         datatype.defineType("COFF",5);
         datatype.defineType("NOFF",6);
         // necessary for MC information checks
         RooRealVar mcinfo("mcinfo","MC information",0,256);
         RooRealVar mctype("mctype","MC type information",-1,1005);
         RooRealVar chil0_mo("chil0_mo","MC #phi mother",-999999,999999);
         RooRealVar chil0_mc("chil0_mc","MC information #phi",0,256);
         RooRealVar runno("runno","Run Number",-999999,999999);
         // event weight
         RooRealVar weight("weight","weight",-999999,999999);
         // create data set (constructor only overload for max. 9 arguments, so we add some afterwards)
         RooArgSet varset(mbc,deltae,chil0_ma,mass_kpi,hel_phi,hel_th1,hel_th2,nb_prime);
         varset.add(flavor);
         varset.add(datatype);
         varset.add(mcinfo);
         varset.add(mctype);
         varset.add(chil0_mo);
         varset.add(chil0_mc);
         varset.add(runno);
         varset.add(weight);
         RooDataSet data("data","data",varset,Import(*chain.get()),Cut("(datatype==datatype::CS || datatype==datatype::NS || datatype==datatype::COFF || datatype==datatype::NOFF) && hel_th1 < 0.75"),WeightVar(weight));
         // create helper for the simultanous fit
         std::vector<std::string> flags_datatype;
         flags_datatype.push_back("CP");
         flags_datatype.push_back("CA");
         flags_datatype.push_back("N");
         flags_datatype.push_back("CS");
         flags_datatype.push_back("NS");
         flags_datatype.push_back("COFF");
         flags_datatype.push_back("NOFF");
         tuple_category_name_flags category_name_flags_datatype = std::make_tuple(&datatype,"datatype",flags_datatype);
         std::vector<std::string> flags_flavor;
         flags_flavor.push_back("CP");
         flags_flavor.push_back("CA");
         tuple_category_name_flags category_name_flags_flavor = std::make_tuple(&flavor,"flavor",flags_flavor);
         // print data set
         data.Print();
         
         std::vector<RooRealVar*> variables_8D;
         variables_8D.push_back(&mbc);
         variables_8D.push_back(&deltae);
         variables_8D.push_back(&chil0_ma);
         variables_8D.push_back(&nb_prime);
         variables_8D.push_back(&mass_kpi);
         variables_8D.push_back(&hel_phi);
         variables_8D.push_back(&hel_th1);
         variables_8D.push_back(&hel_th2);
         
         // ---------------------------------------------
         // ----- Define full data range
         // ---------------------------------------------
         SetFitRanges(mbc,deltae,chil0_ma,mass_kpi,hel_phi,hel_th1,hel_th2,nb_prime);
         mbc.setRange("full_range",5.24,5.26);
         // ---------------------------------------------
         // ----- Create data component plots
         // ---------------------------------------------
         if(opt->GetCreateComponentPlots()) {
         std::cout << "STATUS: Create data component plots..." << std::endl;
         for(RooRealVar* var : variables_8D) {
         CreateDataComponentPlots(data,*var,opt->GetBins(),"full_range",opt->GetOutdir(),opt->GetPlotFormat(),"CS","datatype");
         CreateDataComponentPlots(data,*var,opt->GetBins(),"full_range",opt->GetOutdir(),opt->GetPlotFormat(),"NS","datatype");
         CreateDataComponentPlots(data,*var,opt->GetBins(),"full_range",opt->GetOutdir(),opt->GetPlotFormat(),"COFF","datatype");
         CreateDataComponentPlots(data,*var,opt->GetBins(),"full_range",opt->GetOutdir(),opt->GetPlotFormat(),"NOFF","datatype");
         }
         mp::execute_command("mkdir -p "+opt->GetOutdir()+"DATA_data_sideband && mv "+opt->GetOutdir()+"DATA_*."+opt->GetPlotFormat()+" "+opt->GetOutdir()+"DATA_data_sideband");
         }
         // ---------------------------------------------
         // ----- Do fits
         // ---------------------------------------------
         // See RooFit tutorial macro #501 about HowTo do a simultaneous fit
         // See RooFit tutorial macro #603 about HowTo do a multi-core parallelized unbinned maximum likelihood fit
         std::cout << "STATUS: Do fits..." << std::endl;
         
         PrintFitStartBlock("SETUP: 9D Fit");
         
         shared_ptr<FF_PDF> pdf_bkg_continuum_9D = make_shared<FF_PDF_bkg_continuum_9D_mbc_deltae_chil0_ma_mass_kpi_hel_phi_hel_th1_hel_th2_nb_prime_flavor>(mbc,
         deltae, chil0_ma, mass_kpi, hel_phi, hel_th1,
         hel_th2, nb_prime, flavor, category_name_flags_datatype,true,(RooRealVar*)NULL);
         pdf_bkg_continuum_9D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_continuum_8D.dat",true);
         
         PrintFitStartBlock("FITTING: 9D Fit");
         
         // fit the continuum pdf to sideband data and fix the shape
         mbc.setMin(5.24);
         mbc.setMax(5.26);
         mbc.setRange("full_range",5.24,5.26);
         
         // fit the 1D mass_kpi distribution due to some instability
         shared_ptr<FF_PDF> pdf_bkg_continuum_1D_mass_kpi = make_shared<FF_PDF_bkg_continuum_1D_mass_kpi>(mass_kpi, category_name_flags_datatype, false);
         RooFitResult *result_mass_kpi = pdf_bkg_continuum_1D_mass_kpi->GetSimPDF().fitTo(data, SumW2Error(false), NumCPU(opt->GetNumcpu()),
         Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()), Extended(false));
         pdf_bkg_continuum_1D_mass_kpi->GetSimPDF().getParameters(data)->Print("v");
         WriteCorrelationMatrixFile(result_mass_kpi,opt->GetOutdir()+"pdf_data_1D_mass_kpi_correlation.dat");
         pdf_bkg_continuum_1D_mass_kpi->ExportPDFValues(opt->GetOutdir()+"pdf_data_1D_mass_kpi.dat",true);
         result_mass_kpi->Print();
         // set mass_kpi distribution and fix it
         for(RooRealVar *v : pdf_bkg_continuum_1D_mass_kpi->GetVariables()) {
         pdf_bkg_continuum_9D->SetVariable(v->GetName(),v->getVal(),v->getError(),true,false);
         }
         
         RooFitResult *result_9D_sideband = pdf_bkg_continuum_9D->GetSimPDF().fitTo(data, SumW2Error(false), NumCPU(opt->GetNumcpu()),
         Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()), Extended(false));
         pdf_bkg_continuum_9D->GetSimPDF().getParameters(data)->Print("v");
         
         if(opt->GetCreatePlots()) {
         for(RooRealVar *v : variables_8D) {
         std::string varname = v->GetName();
         
         PrintFitStartBlock("PLOT PROJECTION");
         FFUtil::VisualizeFitResult(pdf_bkg_continuum_9D,data,*v,category_name_flags_datatype,false,
         opt->GetBins(),"full_range", opt->GetOutdir()+"DATA_9D_sideband_"+varname+"_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         FFUtil::VisualizeFitResultProjWData(pdf_bkg_continuum_9D,data,*v,category_name_flags_datatype,false,
         opt->GetBins(),"full_range", opt->GetOutdir()+"DATA_9D_sideband_"+varname+"_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         category_name_flags_flavor,true,
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         }
         WriteCorrelationMatrixFile(result_9D_sideband,opt->GetOutdir()+"pdf_data_9D_sideband_correlation.dat");
         pdf_bkg_continuum_9D->ExportPDFValues(opt->GetOutdir()+"pdf_data_9D_sideband.dat",true);
         
         // draw projections in 5 mass_kpi regions
         for(unsigned int i = 1; i <= 5; ++i) {
         mbc.setRange(("mass_kpi_bin_"+mp::to_string(i)).c_str(),5.24,5.26);
         FFUtil::VisualizeFitResult(pdf_bkg_continuum_9D,data,hel_th1,category_name_flags_datatype,false,
         opt->GetBins(),"mass_kpi_bin_"+mp::to_string(i), opt->GetOutdir()+"DATA_9D_sideband_hel_th1_projection_in_mass_kpi_bin_"+mp::to_string(i)+"."+opt->GetPlotFormat(),opt->GetPlotFormat(),
         opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
         }
         }
         result_9D_sideband->Print();*/
    }
    
}
