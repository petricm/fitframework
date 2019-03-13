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
#include "FF_Bootstrap.h"
// Utilities
#include "mp_utility.h"

using namespace FFUtil;

namespace FFFits {
    
    void FitEvtGenMC(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt) {
        
        // ---------------------------------------------
        // ----- Define variables
        // ---------------------------------------------
        
        std::cout << "Reading fit configuration from " << opt->GetConfigFile() << std::endl;
        namespace bpt = boost::property_tree;
        bpt::ptree pt;
        bpt::info_parser::read_info(opt->GetConfigFile(),pt);
        
        // ---------------------------------------------
        // ----- Define variables
        // ---------------------------------------------
        PrintFitStartBlock("STATUS: Define functions and variables...");
        RooRealVar mass_BC("mass_01","M^{2}_{K#pi}",0,27,"GeV^{2}");
        RooRealVar mass_AB("mass_12","M^{2}_{#pi#pi^{0}}",0,27,"GeV^{2}");
        RooRealVar mass_CA("mass_02","M^{2}_{K#pi^0}",0,27,"GeV^{2}");
        RooRealVar idhep("idhep","idhep'",-1000,1000,"");
        RooCategory channel("decay","decay channel");
        channel.defineType("C",0);
        channel.defineType("N",1);

        RooCategory flavor("flavour","flavor type");
        flavor.defineType("CP",511);
        flavor.defineType("CA",-511);
        
        mass_BC.setRange("full_range",0, 27);
        mass_CA.setRange("full_range",0, 27);
        mass_AB.setRange("full_range",0, 27);
        
        std::vector<std::string> flags_channel;
        flags_channel.push_back("C");
        flags_channel.push_back("N");
        tuple_category_name_flags category_name_flags_channel = std::make_tuple(&channel,"decay",flags_channel);
        std::vector<std::string> flags_flavor;
        flags_flavor.push_back("CP");
        flags_flavor.push_back("CA");
        tuple_category_name_flags category_name_flags_flavor = std::make_tuple(&flavor,"flavor",flags_flavor);
        // create data set (constructor only overload for max. 9 arguments, so we add some afterwards)
        RooArgSet varset(mass_AB,mass_BC,mass_CA,channel,flavor,idhep);
        
        std::vector<RooRealVar*> variables_2D;
        variables_2D.push_back(&mass_AB);
        variables_2D.push_back(&mass_BC);
        
        RooArgSet varset_2D(mass_AB,mass_BC);
        
        RooDataSet data("data","data",varset,Import(*chain.get()),Cut(""));
        
        
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
        
        
        shared_ptr<FF_PDF> pdf_signal_2D = make_shared<FF_PDF_signal_2D_dalitz_analysis>(mass_AB, mass_BC, acceptance, dai_pdg_constants, dai_DM_pars, dai_2D, category_name_flags_channel);
        
        // set the signal pdf
        pdf_signal_2D->GetVariable("mass_1")->setVal(truth_map["mass_1"]);
        pdf_signal_2D->GetVariable("mass_2")->setVal(truth_map["mass_2"]);
        pdf_signal_2D->GetVariable("mass_3")->setVal(truth_map["mass_3"]);
        pdf_signal_2D->GetVariable("mass_4")->setVal(truth_map["mass_4"]);
        pdf_signal_2D->GetVariable("mass_5")->setVal(truth_map["mass_5"]);
        pdf_signal_2D->GetVariable("mass_6")->setVal(truth_map["mass_6"]);
        pdf_signal_2D->GetVariable("mass_7")->setVal(truth_map["mass_7"]);
        
        pdf_signal_2D->GetVariable("width_1")->setVal(truth_map["width_1"]);
        pdf_signal_2D->GetVariable("width_2")->setVal(truth_map["width_2"]);
        pdf_signal_2D->GetVariable("width_3")->setVal(truth_map["width_3"]);
        pdf_signal_2D->GetVariable("width_4")->setVal(truth_map["width_4"]);
        pdf_signal_2D->GetVariable("width_5")->setVal(truth_map["width_5"]);
        pdf_signal_2D->GetVariable("width_6")->setVal(truth_map["width_6"]);
        pdf_signal_2D->GetVariable("width_7")->setVal(truth_map["width_7"]);
        
        pdf_signal_2D->GetVariable("amp_1")->setVal(truth_map["amp_1"]);
        pdf_signal_2D->GetVariable("amp_2")->setVal(truth_map["amp_2"]);
        pdf_signal_2D->GetVariable("amp_3")->setVal(truth_map["amp_3"]);
        pdf_signal_2D->GetVariable("amp_4")->setVal(truth_map["amp_4"]);
        pdf_signal_2D->GetVariable("amp_5")->setVal(truth_map["amp_5"]);
        pdf_signal_2D->GetVariable("amp_6")->setVal(truth_map["amp_6"]);
        pdf_signal_2D->GetVariable("amp_7")->setVal(truth_map["amp_7"]);
        pdf_signal_2D->GetVariable("amp_8")->setVal(truth_map["amp_8"]);
        
        pdf_signal_2D->GetVariable("phase_1")->setVal(truth_map["phase_1"]);
        pdf_signal_2D->GetVariable("phase_2")->setVal(truth_map["phase_2"]);
        pdf_signal_2D->GetVariable("phase_3")->setVal(truth_map["phase_3"]);
        pdf_signal_2D->GetVariable("phase_4")->setVal(truth_map["phase_4"]);
        pdf_signal_2D->GetVariable("phase_5")->setVal(truth_map["phase_5"]);
        pdf_signal_2D->GetVariable("phase_6")->setVal(truth_map["phase_6"]);
        pdf_signal_2D->GetVariable("phase_7")->setVal(truth_map["phase_7"]);
        pdf_signal_2D->GetVariable("phase_8")->setVal(truth_map["phase_8"]);
        
        
        // draw original distribution
        
        pdf_signal_2D->GetVariable("amp_1")->setConstant(true);
        pdf_signal_2D->GetVariable("phase_1")->setConstant(true);
        
        RooFitResult *result_evtgen = pdf_signal_2D->GetSimPDF().fitTo(data, SumW2Error(true), NumCPU(opt->GetNumcpu()), Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()));
        pdf_signal_2D->GetSimPDF().getParameters(data)->Print("v");
        result_evtgen->Print();
        delete result_evtgen;
        
        PrintFitStartBlock("Plotting: 2D");
        for(RooRealVar *v : variables_2D) {
            std::string varname = v->GetName();
            FFUtil::VisualizeFitResult(pdf_signal_2D,data,*v,category_name_flags_channel,false,
                                       opt->GetBins(),"full_range", opt->GetOutdir()+"EVTGEN_PDF_MC_2D_"+varname+"_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                       opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
        }

        
        const unsigned int nbins = 10;
        double bin_limits[nbins+1];
        
        //set the bins ranges gor the selected number of bins
        bin_limits[0]=(m_Pi0+m_Pi)*(m_Pi0+m_Pi);
        bin_limits[nbins]=(m_B0-m_K)*(m_B0-m_K);
        const double binsize = (bin_limits[nbins]-bin_limits[0])/(double)nbins;
        for (unsigned int i=1; i<nbins; i++) {
            bin_limits[i] = bin_limits[0]+i*binsize;
        }
        
        for(unsigned int bin = 1; bin <= nbins; ++bin) {
            std::string binned_var_name = mass_AB.GetName();

            double min = bin_limits[bin-1];
            double max = bin_limits[bin];
            PrintFitStartBlock("Projecting: "+binned_var_name+" bin "+mp::to_string(bin)+" from ["+mp::to_string(min)+","+mp::to_string(max)+"] of 2D Dalitz PDF");
            
            // buffer range and reset full_range to make proper projections
            std::pair<Double_t,Double_t> range_buffer = std::make_pair(mass_AB.getMin("full_range"),mass_AB.getMax("full_range"));
            mass_AB.setRange("full_range",min,max);
            
            
            std::string varname = mass_BC.GetName();
            FFUtil::VisualizeFitResult(pdf_signal_2D,data,mass_BC,category_name_flags_channel,false,
                                           opt->GetBins(),"full_range", opt->GetOutdir()+"EVTGEN_PDF_MC_2D_"+varname+"_projection_"+binned_var_name+"_bin_"+mp::to_string(bin)+"."+opt->GetPlotFormat(),
                                           opt->GetPlotFormat(),opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
            // reset full_range
            mass_AB.setRange("full_range",range_buffer.first,range_buffer.second);
        }
        
        dalitz_analysis_norm_cache_integrator->SaveIntegrals(path_to_integrals);
        dalitz_analysis_real_cache_integrator->SaveIntegrals(path_to_integrals);
        dalitz_analysis_imag_cache_integrator->SaveIntegrals(path_to_integrals);
        
        delete dalitz_analysis_imag_cache_integrator;
        delete dalitz_analysis_norm_cache_integrator;
        delete dalitz_analysis_real_cache_integrator;
        
        
        
    }
    
    void FitEvtGenMC_and_Reweight(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt) {
        
        std::cout << "Reading fit configuration from " << opt->GetConfigFile() << std::endl;
        namespace bpt = boost::property_tree;
        bpt::ptree pt;
        bpt::info_parser::read_info(opt->GetConfigFile(),pt);
        
        
        EFFICIENCY_FUNCTION acceptance = BIN_2D; // or BIN_2D
        std::string path_to_integrals = "./CachedIntegrals_ACCEPTANCE" + FFUtil::GetAcceptanceString(acceptance) + "/";
        
        std::vector<double> parameters;
        parameters.reserve(33);
        parameters.assign(32,0);
        // variable parameters (will be set in for loop below)
        parameters[0] = 0; // index of resonance
        parameters[1] = 0; // index of charged conjugate resonance
        // constant parameters for resonances
        parameters[2] = pt.get<double>("mass_1");
        parameters[3] = pt.get<double>("width_1");
        parameters[4] = pt.get<double>("r_1");
        parameters[5] = pt.get<double>("mass_2");
        parameters[6] = pt.get<double>("width_2");
        parameters[7] = pt.get<double>("a_2");
        parameters[8] = pt.get<double>("r_2");
        parameters[9] = pt.get<double>("cutoff_2");
        parameters[10] = pt.get<double>("mass_3");
        parameters[11] = pt.get<double>("width_3");
        parameters[12] = pt.get<double>("r_3");
        parameters[13] = pt.get<double>("mass_4");
        parameters[14] = pt.get<double>("width_4");
        parameters[15] = pt.get<double>("r_4");
        parameters[16] = pt.get<double>("mass_5");
        parameters[17] = pt.get<double>("width_5");
        parameters[18] = pt.get<double>("r_5");
        parameters[19] = pt.get<double>("mass_6");
        parameters[20] = pt.get<double>("width_6");
        parameters[21] = pt.get<double>("r_6");
        parameters[22] = pt.get<double>("mass_7");
        parameters[23] = pt.get<double>("width_7");
        parameters[24] = pt.get<double>("a_7");
        parameters[25] = pt.get<double>("r_7");
        parameters[26] = pt.get<double>("cutoff_7");
        // constant parameters
        parameters[27] = m_B0;
        parameters[28] = m_Pi0;
        parameters[29] = m_Pi;
        parameters[30] = m_K;
        // acceptance function
        parameters[31] = acceptance;
        
        
        std::vector<std::pair<double,double> > limits; limits.reserve(2);
		limits.push_back(std::make_pair(0,27));
		limits.push_back(std::make_pair(0,27));
        
        
        PrintFitStartBlock("SETUP: CacheIntegrator");
        int debug_level = 1;
        CacheIntegrator<DalitzAnalysisNormCachedIntegral>* dalitz_analysis_norm_cache_integrator = new CacheIntegrator<DalitzAnalysisNormCachedIntegral>(debug_level);
        CacheIntegrator<DalitzAnalysisRealCachedIntegral>* dalitz_analysis_real_cache_integrator = new CacheIntegrator<DalitzAnalysisRealCachedIntegral>(debug_level);
        CacheIntegrator<DalitzAnalysisImagCachedIntegral>* dalitz_analysis_imag_cache_integrator = new CacheIntegrator<DalitzAnalysisImagCachedIntegral>(debug_level);
        dalitz_analysis_norm_cache_integrator->LoadIntegrals(path_to_integrals);
        dalitz_analysis_real_cache_integrator->LoadIntegrals(path_to_integrals);
        dalitz_analysis_imag_cache_integrator->LoadIntegrals(path_to_integrals);
        
        const double J_values[8] = {1, 2, 3, 4, 5, 6, 7, 8};
        const int N_RES = 8;
        
         double amps[8]  = {pt.get<double>("amp_1"), pt.get<double>("amp_2"), pt.get<double>("amp_3"), pt.get<double>("amp_4"), pt.get<double>("amp_5"), pt.get<double>("amp_6"), pt.get<double>("amp_7"), pt.get<double>("amp_8")};
         double phase[8] = {pt.get<double>("phase_1"), pt.get<double>("phase_2"), pt.get<double>("phase_3"), pt.get<double>("phase_4"), pt.get<double>("phase_5"), pt.get<double>("phase_6"), pt.get<double>("phase_7"), pt.get<double>("phase_8")};
        
        double amps_CC[8]  = {pt.get<double>("amp_1_CC"), pt.get<double>("amp_2_CC"), pt.get<double>("amp_3_CC"), pt.get<double>("amp_4_CC"), pt.get<double>("amp_5_CC"), pt.get<double>("amp_6_CC"), pt.get<double>("amp_7_CC"), pt.get<double>("amp_8_CC")};
        double phase_CC[8] = {pt.get<double>("phase_1_CC"), pt.get<double>("phase_2_CC"), pt.get<double>("phase_3_CC"), pt.get<double>("phase_4_CC"), pt.get<double>("phase_5_CC"), pt.get<double>("phase_6_CC"), pt.get<double>("phase_7_CC"), pt.get<double>("phase_8_CC")};
        
        /*
        for(unsigned int i = 0; i < N_RES; i++) {
			for(unsigned int j = 0; j < N_RES; j++) {
                
				parameters[0] = J_values[i];
				parameters[1] = J_values[j];
                
				if(i==j) { // integral Abs(A_i A_i*)
                    dalitz_analysis_norm_cache_integrator->SetMode(1);
                    dalitz_analysis_norm_cache_integrator->GetIntegral(limits,parameters);
				} else if (i < j) { // integral Re(A_i A_j*)
                    dalitz_analysis_real_cache_integrator->SetMode(1);
                    dalitz_analysis_real_cache_integrator->GetIntegral(limits,parameters);
				} else if (i > j) { // integral Im(A_i A_j*)
                    dalitz_analysis_imag_cache_integrator->SetMode(1);
                    std::swap(parameters[0],parameters[1]);
                    dalitz_analysis_imag_cache_integrator->GetIntegral(limits,parameters);
                }
			}
		}
        
        dalitz_analysis_norm_cache_integrator->PrepareBatchJobs(path_to_integrals + "batch_job_norm/",1);
        dalitz_analysis_real_cache_integrator->PrepareBatchJobs(path_to_integrals + "batch_job_real/",1);
        dalitz_analysis_imag_cache_integrator->PrepareBatchJobs(path_to_integrals + "batch_job_imag/",1);
        */
        
        
        double M = 0;
		// calc each of the 64 terms (some need to be recalculated probably, some are done and fast)
		for(unsigned int i = 0; i < N_RES; ++i) {
			for(unsigned int j = 0; j < N_RES; ++j) {
				// setting missing paramters
				parameters[0] = J_values[i];
				parameters[1] = J_values[j];
				// switch between the three different types of integrals
				if(i==j) { // integral |A_i|^2
					M += amps[i]*amps[j]*dalitz_analysis_norm_cache_integrator->GetIntegral(limits,parameters);
				} else if (i < j) { // integral Re(A_i A_j*)
					M += 2*amps[i]*amps[j]*std::cos(phase[i]-phase[j])*dalitz_analysis_real_cache_integrator->GetIntegral(limits,parameters);
				} else if (i > j) { // integral Im(A_i A_j*)
					// flip i and j components, as always A_j needs to be cc
					std::swap(parameters[0],parameters[1]);
					M -= 2*amps[i]*amps[j]*std::sin(phase[j]-phase[i])*dalitz_analysis_imag_cache_integrator->GetIntegral(limits,parameters);
				}
			}
		}
        double Mbar = 0;
        for(unsigned int i = 0; i < N_RES; ++i) {
			for(unsigned int j = 0; j < N_RES; ++j) {
				// setting missing paramters
				parameters[0] = J_values[i];
				parameters[1] = J_values[j];
				// switch between the three different types of integrals
				if(i==j) { // integral |A_i|^2
					Mbar += amps_CC[i]*amps_CC[j]*dalitz_analysis_norm_cache_integrator->GetIntegral(limits,parameters);
				} else if (i < j) { // integral Re(A_i A_j*)
					Mbar += 2*amps_CC[i]*amps_CC[j]*std::cos(phase_CC[i]-phase_CC[j])*dalitz_analysis_real_cache_integrator->GetIntegral(limits,parameters);
				} else if (i > j) { // integral Im(A_i A_j*)
					// flip i and j components, as always A_j needs to be cc
					std::swap(parameters[0],parameters[1]);
					Mbar -= 2*amps_CC[i]*amps_CC[j]*std::sin(phase_CC[j]-phase_CC[i])*dalitz_analysis_imag_cache_integrator->GetIntegral(limits,parameters);
				}
			}
		}
        
        
        
        std::cout<<"integral |M^2| is "<<std::setprecision(16)<<M<<std::endl;
        std::cout<<"integral |Mbar^2| is "<<std::setprecision(16)<<Mbar<<std::endl;
        
        
        double B0 = M+Mbar;
        
        double sum = 0;
        

        
        
        for(unsigned int i = 0; i < N_RES; ++i) {
			for(unsigned int j = 0; j < N_RES; ++j) {
				// setting missing paramters
				parameters[0] = J_values[i];
				parameters[1] = J_values[j];
				// switch between the three different types of integrals
				if(i==j) { // integral |A_i|^2
                    std::cout<<i<<"\t"<<std::setprecision(16)<< (amps_CC[i]*amps_CC[j]+amps[i]*amps[j])*dalitz_analysis_norm_cache_integrator->GetIntegral(limits,parameters) / B0 * 100<<"\t";
                    std::cout<< (amps[j]*amps[j]-amps_CC[i]*amps_CC[i])/(amps[j]*amps[j]+amps_CC[i]*amps_CC[i])<<std::endl;
                    sum+=(amps_CC[i]*amps_CC[j]+amps[i]*amps[j])*dalitz_analysis_norm_cache_integrator->GetIntegral(limits,parameters) / B0;
				}
			}
		}
        
        std::cout <<"Total sum: "<<sum<<std::endl;
        
        /*
        
        const double amps_delta[8]  = {pt.get<double>("amp_1_delta"), pt.get<double>("amp_2_delta"), pt.get<double>("amp_3_delta"), pt.get<double>("amp_4_delta"), pt.get<double>("amp_5_delta"), pt.get<double>("amp_6_delta"), pt.get<double>("amp_7_delta"), pt.get<double>("amp_8_delta")};
        const double phase_delta[8] = {pt.get<double>("phase_1_delta"), pt.get<double>("phase_2_delta"), pt.get<double>("phase_3_delta"), pt.get<double>("phase_4_delta"), pt.get<double>("phase_5_delta"), pt.get<double>("phase_6_delta"), pt.get<double>("phase_7_delta"), pt.get<double>("phase_8_delta")};
        
        for (int i=0; i<8; i++) {
            std::cout<<i<<"\t"<<(amps[i]*amps[i]-(amps[i]+amps_delta[i])*(amps[i]+amps_delta[i]))/(amps[i]*amps[i]+(amps[i]+amps_delta[i])*(amps[i]+amps_delta[i]))<<std::endl;
            
            amps[i]+=amps_delta[i];
            phase[i]+=phase_delta[i];
        }
        
        M = 0;
		// calc each of the 64 terms (some need to be recalculated probably, some are done and fast)
		for(unsigned int i = 0; i < N_RES; ++i) {
			for(unsigned int j = 0; j < N_RES; ++j) {
				// setting missing paramters
				parameters[0] = J_values[i];
				parameters[1] = J_values[j];
				// switch between the three different types of integrals
				if(i==j) { // integral |A_i|^2
					M += amps[i]*amps[j]*dalitz_analysis_norm_cache_integrator->GetIntegral(limits,parameters);
				} else if (i < j) { // integral Re(A_i A_j*)
					M += 2*amps[i]*amps[j]*std::cos(phase[i]-phase[j])*dalitz_analysis_real_cache_integrator->GetIntegral(limits,parameters);
				} else if (i > j) { // integral Im(A_i A_j*)
					// flip i and j components, as always A_j needs to be cc
					std::swap(parameters[0],parameters[1]);
					M -= 2*amps[i]*amps[j]*std::sin(phase[j]-phase[i])*dalitz_analysis_imag_cache_integrator->GetIntegral(limits,parameters);
				}
			}
		}
        std::cout<<"integral anti |M^2| is "<<std::setprecision(16)<<M<<std::endl;
        
        double BBar = M;
        
        std::cout<<"A_CP is "<<std::setprecision(16)<<(B0-BBar)/(B0+BBar)<<std::endl;
        */
        delete dalitz_analysis_imag_cache_integrator;
        delete dalitz_analysis_norm_cache_integrator;
        delete dalitz_analysis_real_cache_integrator;
    }
    
}
