/**
 * @file	FFFits.cc
 * @date	July 8, 203
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

void FitSignalMC(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt) {
    
	if(chain->GetEntries() == 0) {
		std::cout << "No data for signal MC fits given, skip fitting." << std::endl;
		return;
	}

	// ---------------------------------------------
	// ----- Define variables
	// ---------------------------------------------
    PrintFitStartBlock("STATUS: Define functions and variables...");
    RooRealVar mbc("mbcp","M'_{bc}",5.25, 5.288,"GeV/c^{2}");
    RooRealVar deltae("deltae","#Delta E",-0.2,0.2,"GeV");
    RooRealVar nb_prime("nb_prime","C'_{NB}",-10.0,10.0,"");

    RooRealVar mass_BC("mass_01","m^{2}_{K#pi}",0,27,"GeV^{2}/c^{4}");
    RooRealVar mass_AB("mass_12","m^{2}_{#pi#pi^{0}}",0,27,"GeV^{2}/c^{4}");
    RooRealVar mass_CA("mass_02","m^{2}_{K#pi^0}",0,27,"GeV^{2}/c^{4}");

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
    // event weight
    RooRealVar weight("weight","weight",-999999,999999);
    // create data set (constructor only overload for max. 9 arguments, so we add some afterwards)
    RooArgSet varset(mbc, deltae, nb_prime, mass_AB, mass_BC, mass_CA);
    varset.add(channel);
    varset.add(mcinfo);
    varset.add(mctype);
    varset.add(mchash);
    varset.add(flavor);
    varset.add(weight);
    RooDataSet data("data","data",varset,Import(*chain.get()),Cut("mbcp < 5.288"),WeightVar(weight));

	// create helper for the simultanous fit
	std::vector<std::string> flags;
	flags.push_back("C");
	flags.push_back("N");
	tuple_category_name_flags category_name_flags = std::make_tuple(&channel,"channel",flags);
	// create data subsamples
	RooDataSet* data_charged = dynamic_cast<RooDataSet*>(data.reduce("channel==channel::C"));
	data_charged->Print();
    // print data set
	data.Print();

    std::vector<std::string> flags_flavor;
    flags_flavor.push_back("CP");
    flags_flavor.push_back("CA");
    tuple_category_name_flags category_name_flags_flavor = std::make_tuple(&flavor,"flavor",flags_flavor);
    
	
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
//	hel_th1.setMax(1.0);
//	SetFitRanges(mbc,deltae,chil0_ma,mass_kpi,hel_phi,hel_th1,hel_th2,nb_prime);
    
    mass_AB.setRange("full_range1",0, 27);
    mass_BC.setRange("full_range1",0, 4);
    
    mass_AB.setRange("full_range2",0, 27);
    mass_BC.setRange("full_range2",4, 27);
    
    mass_AB.setRange("full_range3",0, 4);
    mass_BC.setRange("full_range3",0, 27);

    mass_AB.setRange("full_range4",4, 27);
    mass_BC.setRange("full_range4",0, 27);
    
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
			CreateDataComponentPlots(data,*var,opt->GetBins(),"full_range",opt->GetOutdir(),opt->GetPlotFormat(),"C","channel");
		}
		mp::execute_command("mkdir -p "+opt->GetOutdir()+"DATA_signal && mv "+opt->GetOutdir()+"DATA_*."+opt->GetPlotFormat()+" "+opt->GetOutdir()+"DATA_signal");
	}
	// ---------------------------------------------
	// ----- Do fits
	// ---------------------------------------------
	// See RooFit tutorial macro #501 about HowTo do a simultaneous fit
	// See RooFit tutorial macro #603 about HowTo do a multi-core parallelized unbinned maximum likelihood fit
	std::cout << "STATUS: Do fits..." << std::endl;
/*
	PrintFitStartBlock("FITTING: mbc");
	shared_ptr<FF_PDF> pdf_signal_1D_mbc = make_shared<FF_PDF_signal_1D_mbc>(mbc, category_name_flags);
	RooFitResult *result_mbc = pdf_signal_1D_mbc->GetSimPDF().fitTo(data, SumW2Error(true), NumCPU(opt->GetNumcpu()), Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()));
	pdf_signal_1D_mbc->GetSimPDF().getParameters(data)->Print("v");
	FFUtil::VisualizeFitResult(pdf_signal_1D_mbc,data,mbc,category_name_flags,false,
			opt->GetBins(),"full_range", opt->GetOutdir()+"SIG_MC_1D_mbc_fit."+opt->GetPlotFormat(),opt->GetPlotFormat(),
			opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
	WriteCorrelationMatrixFile(result_mbc,opt->GetOutdir()+"pdf_signal_1D_mbc_correlation.dat");
	pdf_signal_1D_mbc->ExportPDFValues(opt->GetOutdir()+"pdf_signal_1D_mbc.dat",true);
	result_mbc->Print();
	delete result_mbc;

	PrintFitStartBlock("FITTING: deltae");
	shared_ptr<FF_PDF> pdf_signal_1D_deltae = make_shared<FF_PDF_signal_1D_deltae>(deltae, category_name_flags);
	RooFitResult *result_deltae = pdf_signal_1D_deltae->GetSimPDF().fitTo(data, SumW2Error(true), NumCPU(opt->GetNumcpu()), Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()));
	pdf_signal_1D_deltae->GetSimPDF().getParameters(data)->Print("v");
	FFUtil::VisualizeFitResult(pdf_signal_1D_deltae,data,deltae,category_name_flags,false,
			opt->GetBins(),"full_range", opt->GetOutdir()+"SIG_MC_1D_deltae_fit."+opt->GetPlotFormat(),opt->GetPlotFormat(),
			opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
	WriteCorrelationMatrixFile(result_deltae,opt->GetOutdir()+"pdf_signal_1D_deltae_correlation.dat");
	pdf_signal_1D_deltae->ExportPDFValues(opt->GetOutdir()+"pdf_signal_1D_deltae.dat",true);
	result_deltae->Print();
	delete result_deltae;


	PrintFitStartBlock("FITTING: nb_prime");
	shared_ptr<FF_PDF> pdf_signal_1D_nb_prime = make_shared<FF_PDF_signal_1D_nb_prime>(nb_prime, category_name_flags);
	RooFitResult *result_nb_prime = pdf_signal_1D_nb_prime->GetSimPDF().fitTo(data, SumW2Error(true), NumCPU(opt->GetNumcpu()), Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()));
	pdf_signal_1D_nb_prime->GetSimPDF().getParameters(data)->Print("v");
	FFUtil::VisualizeFitResult(pdf_signal_1D_nb_prime,data,nb_prime,category_name_flags,true,
			opt->GetBins(),"full_range", opt->GetOutdir()+"SIG_MC_1D_nb_prime_fit."+opt->GetPlotFormat(),opt->GetPlotFormat(),
			opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
	WriteCorrelationMatrixFile(result_nb_prime,opt->GetOutdir()+"pdf_signal_1D_nb_prime_correlation.dat");
	pdf_signal_1D_nb_prime->ExportPDFValues(opt->GetOutdir()+"pdf_signal_1D_nb_prime.dat",true);
	result_nb_prime->Print();
	delete result_nb_prime;

	PrintFitStartBlock("FITTING: deltae | mbc");
	shared_ptr<FF_PDF> pdf_signal_2D_mbc_deltae =
			make_shared<FF_PDF_signal_2D_mbc_deltae>(mbc, deltae,
					pdf_signal_1D_mbc, category_name_flags);
	//pdf_signal_2D_mbc_deltae->ImportPDFValues(opt->GetOutdir()+"pdf_signal_2D_mbc_deltae.dat",true);
	// pdf_signal_2D_mbc_deltae->SetVariablesConst();
	RooFitResult *result_mbc_deltae = pdf_signal_2D_mbc_deltae->GetSimPDF().fitTo(data, NumCPU(opt->GetNumcpu()),
			SumW2Error(true), Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()));
	pdf_signal_2D_mbc_deltae->GetSimPDF().getParameters(data)->Print("v");
	FFUtil::VisualizeFitResult(pdf_signal_2D_mbc_deltae,data,mbc,category_name_flags,false,
			opt->GetBins(),"full_range", opt->GetOutdir()+"SIG_MC_2D_mbc_deltae_fit_mbc_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
			opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
	FFUtil::VisualizeFitResult(pdf_signal_2D_mbc_deltae,data,deltae,category_name_flags,false,
			opt->GetBins(),"full_range", opt->GetOutdir()+"SIG_MC_2D_mbc_deltae_fit_deltae_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
			opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
	WriteCorrelationMatrixFile(result_mbc_deltae,opt->GetOutdir()+"pdf_signal_2D_mbc_deltae_correlation.dat");
	pdf_signal_2D_mbc_deltae->ExportPDFValues(opt->GetOutdir()+"pdf_signal_2D_mbc_deltae.dat",true);
	result_mbc_deltae->Print();
//	std::map<std::string, std::pair<double,double> > resolution = FFUtil::PropagateWidthError(pdf_signal_2D_mbc_deltae,result_mbc_deltae,opt);
//	std::cout << "Resolution SignalMC: " << resolution["width_eff"].first << " +- " << resolution["width_eff"].second;
	delete result_mbc_deltae;
/*
	PrintFitStartBlock("FITTING: 3D Fit");
	shared_ptr<FF_PDF> pdf_signal_3D =
			make_shared<FF_PDF_signal_3D_mbc_deltae_nb_prime>(pdf_signal_2D_mbc_deltae, pdf_signal_1D_nb_prime, category_name_flags);
    
    pdf_signal_3D->SetVariablesConst();
    
	RooFitResult *result_3D = pdf_signal_3D->GetSimPDF().fitTo(data, SumW2Error(true),
			NumCPU(opt->GetNumcpu()), Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()));
	pdf_signal_3D->GetSimPDF().getParameters(data)->Print("v");
	for(RooRealVar *v : variables_3D) {
		std::string varname = v->GetName();
		FFUtil::VisualizeFitResult(pdf_signal_3D,data,*v,category_name_flags,false,
				opt->GetBins(),"full_range", opt->GetOutdir()+"SIG_MC_3D_"+varname+"_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
				opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
	}
	WriteCorrelationMatrixFile(result_3D,opt->GetOutdir()+"pdf_signal_3D_correlation.dat");
	pdf_signal_3D->ExportPDFValues(opt->GetOutdir()+"pdf_signal_3D.dat",true);
	result_3D->Print();
    delete result_3D;
    /**/
    
    PrintFitStartBlock("FITTING: 3D Dalitz Fit");
    
    std::cout << "Reading fit configuration from " << opt->GetConfigFile() << std::endl;
    namespace bpt = boost::property_tree;
    bpt::ptree pt;
    bpt::info_parser::read_info(opt->GetConfigFile(),pt);
    
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
    
    shared_ptr<FF_PDF> pdf_signal_3D_dalitz = make_shared<FF_PDF_signal_3D_dalitz_analysis_CPV>(mass_AB, mass_BC, flavor, acceptance, dai_pdg_constants, dai_DM_pars, dai_2D, category_name_flags);
    
    
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
    
    
    pdf_signal_3D_dalitz->GetVariable("amp_3")->setConstant(true);
    pdf_signal_3D_dalitz->GetVariable("phase_3")->setConstant(true);
    pdf_signal_3D_dalitz->GetVariable("phase_3_CC")->setConstant(true);    

    RooFitResult *result_3D_dalitz = pdf_signal_3D_dalitz->GetSimPDF().fitTo(data, SumW2Error(false), NumCPU(opt->GetNumcpu()), Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()));
	pdf_signal_3D_dalitz->GetSimPDF().getParameters(data)->Print("v");
    result_3D_dalitz->Print();
    std::cout<<"NLL: "<<result_3D_dalitz->minNll()<<std::endl;
    
    /*
	for(RooRealVar *v : variables_2D) {
		std::string varname = v->GetName();
        if (varname == "mass_01") {
		FFUtil::VisualizeFitResult(pdf_signal_3D_dalitz,data,*v,category_name_flags,false,
                                   opt->GetBins(),"full_range1", opt->GetOutdir()+"SIG_MC_3D_DALITZ_"+varname+"_projection_range1."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                   opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
        FFUtil::VisualizeFitResult(pdf_signal_3D_dalitz,data,*v,category_name_flags,false,
                                   opt->GetBins(),"full_range2", opt->GetOutdir()+"SIG_MC_3D_DALITZ_"+varname+"_projection_range2."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                   opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
        }
        if (varname == "mass_12") {
            FFUtil::VisualizeFitResult(pdf_signal_3D_dalitz,data,*v,category_name_flags,false,
                                       opt->GetBins(),"full_range3", opt->GetOutdir()+"SIG_MC_3D_DALITZ_"+varname+"_projection_range1."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                       opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
            FFUtil::VisualizeFitResult(pdf_signal_3D_dalitz,data,*v,category_name_flags,false,
                                       opt->GetBins(),"full_range4", opt->GetOutdir()+"SIG_MC_3D_DALITZ_"+varname+"_projection_range2."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                       opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
        }
	}
    
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
        std::pair<Double_t,Double_t> range_buffer = std::make_pair(mass_AB.getMin("full_range"),mass_AB.getMax("full_range"));
        mass_AB.setRange("full_range",min,max);
        
        
        std::string varname = mass_BC.GetName();
        FFUtil::VisualizeFitResult(pdf_signal_3D_dalitz,data,mass_BC,category_name_flags,false,
                                   opt->GetBins(),"full_range", opt->GetOutdir()+"SIG_MC_3D_DALITZ_"+varname+"_projection_"+binned_var_name+"_bin_"+mp::to_string(bin)+"."+opt->GetPlotFormat(),
                                   opt->GetPlotFormat(),opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
        // reset full_range
        mass_AB.setRange("full_range",range_buffer.first,range_buffer.second);
    }
    
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
        std::pair<Double_t,Double_t> range_buffer = std::make_pair(mass_BC.getMin("full_range"),mass_BC.getMax("full_range"));
        mass_BC.setRange("full_range",min,max);
        
        
        std::string varname = mass_AB.GetName();
        FFUtil::VisualizeFitResult(pdf_signal_3D_dalitz,data,mass_AB,category_name_flags,false,
                                   opt->GetBins(),"full_range", opt->GetOutdir()+"SIG_MC_3D_DALITZ_"+varname+"_projection_"+binned_var_name+"_bin_"+mp::to_string(bin)+"."+opt->GetPlotFormat(),
                                   opt->GetPlotFormat(),opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
        // reset full_range
        mass_BC.setRange("full_range",range_buffer.first,range_buffer.second);
    }
    
    
    
    
    
	WriteCorrelationMatrixFile(result_3D_dalitz,opt->GetOutdir()+"pdf_signal_3D_dalitz_correlation.dat");
	pdf_signal_3D_dalitz->ExportPDFValues(opt->GetOutdir()+"pdf_signal_3D_dalitz.dat",true);
	result_3D_dalitz->Print();
    delete result_3D_dalitz;
    /*
    PrintFitStartBlock("FITTING: 6D Fit");
    shared_ptr<FF_PDF> pdf_signal_6D = make_shared<FF_PDF_signal_6D>(pdf_signal_3D,pdf_signal_3D_dalitz,category_name_flags);
	RooFitResult *result_6D = pdf_signal_6D->GetSimPDF().fitTo(data, SumW2Error(true),
                                                               NumCPU(opt->GetNumcpu()), Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()));
	pdf_signal_6D->GetSimPDF().getParameters(data)->Print("v");
	for(RooRealVar *v : variables_5D) {
		std::string varname = v->GetName();
		FFUtil::VisualizeFitResult(pdf_signal_6D,data,*v,category_name_flags,false,
                                   opt->GetBins(),"full_range", opt->GetOutdir()+"SIG_MC_6D_"+varname+"_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
                                   opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
	}
	WriteCorrelationMatrixFile(result_6D,opt->GetOutdir()+"pdf_signal_6D_correlation.dat");
	pdf_signal_6D->ExportPDFValues(opt->GetOutdir()+"pdf_signal_6D.dat",true);
	result_6D->Print();
    
    
    delete result_6D;
     */
	delete data_charged;
    
    
    
    
}

void FitSignalMCCorrelations(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt) {
	if(chain->GetEntries() == 0) {
		std::cout << "No data for signal correlation fits given, skip fitting." << std::endl;
		return;
	}

	// ---------------------------------------------
	// ----- Define variables
	// ---------------------------------------------
    PrintFitStartBlock("STATUS: Define functions and variables...");
    RooRealVar mbc("mbcp","M'_{bc}",5.25, 5.288,"GeV/c^{2}");
    RooRealVar deltae("deltae","#Delta E",-0.2,0.2,"GeV");
    //    RooRealVar mbcp("mbcp","m'_{bc}",5.25,5.29,"GeV");
    RooRealVar hel_th0("hel_th0","cos #Theta_{0}",-1.0,1.0,"");
    RooRealVar hel_th1("hel_th1","cos #Theta_{1}",-1.0,1.0,"");
    RooRealVar hel_th2("hel_th2","cos #Theta_{2}",-1.0,1.0,"");
    RooRealVar mass_01("mass_01","m_{K#pi}^{2}",0,27,"GeV");
    RooRealVar mass_02("mass_02","m_{K#pi^{0}}^{2}",0,27,"GeV");
    RooRealVar mass_12("mass_12","m_{#pi#pi^{0}}^{2}",0,27,"GeV");
    RooRealVar nb_prime("nb_prime","C'_{NB}",-10.0,10.0,"");
    RooRealVar mchash("mchash","mchash'",0,1e+9,"");
    RooCategory channel("channel","decay channel");
    channel.defineType("C",0);
    channel.defineType("N",1);
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
    RooArgSet varset(mbc,deltae, mass_01, mass_02, mass_12, nb_prime,mchash);
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
    RooDataSet data("data","data",varset,Import(*chain.get()),Cut("mbcp < 5.288"),WeightVar(weight));
    
    // create helper for the simultanous fit
	std::vector<std::string> flags;
	flags.push_back("C");
	flags.push_back("N");
	tuple_category_name_flags category_name_flags = std::make_tuple(&channel,"channel",flags);
	// create data subsamples
	RooDataSet* data_charged = dynamic_cast<RooDataSet*>(data.reduce("channel==channel::C"));
	data_charged->Print();
	RooDataSet* data_neutral = dynamic_cast<RooDataSet*>(data.reduce("channel==channel::N"));
	data_neutral->Print();
	// print data set
	data.Print();
    
    std::vector<RooRealVar*> variables_5D;
    variables_5D.push_back(&mbc);
    variables_5D.push_back(&deltae);
    variables_5D.push_back(&nb_prime);
    variables_5D.push_back(&mass_01);
    variables_5D.push_back(&mass_02);
    variables_5D.push_back(&mass_12);
    
    std::vector<RooRealVar*> variables_3D;
    variables_3D.push_back(&mbc);
    variables_3D.push_back(&deltae);
    variables_3D.push_back(&nb_prime);
    
    // ---------------------------------------------
    // ----- Define full data range
    // ---------------------------------------------
    //SetFitRanges(mbc,deltae,chil0_ma,mass_kpi,hel_phi,hel_th1,hel_th2,nb_prime);
    mbc.setRange("full_range",5.25, 5.288);
    deltae.setRange("full_range",-0.2, 0.2);
    nb_prime.setRange("full_range",-10.0, 10.0);
    
    float mbcp_mean = 5.27958;
    std::cout<<"Mean mbc': "<<mbcp_mean<<std::endl;
    std::cout<<"Sigma mbc': "<<data.sigma(mbc)<<std::endl;
    std::cout<<"Suggested range: ["<<mbcp_mean-2*data.sigma(mbc)<<", "<<mbcp_mean+2*data.sigma(mbc)<<"]"<<std::endl;
    
	// ---------------------------------------------
	// ----- Do fits
	// ---------------------------------------------
	std::cout << "STATUS: Do fits..." << std::endl;

	PrintFitStartBlock("FITTING: mbc");
	shared_ptr<FF_PDF> pdf_signal_1D_mbc = make_shared<FF_PDF_signal_1D_mbc>(mbc, category_name_flags);
	RooFitResult *result_mbc = pdf_signal_1D_mbc->GetSimPDF().fitTo(data, SumW2Error(true), NumCPU(opt->GetNumcpu()), Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()));
	pdf_signal_1D_mbc->GetSimPDF().getParameters(data)->Print("v");
	FFUtil::VisualizeFitResult(pdf_signal_1D_mbc,data,mbc,category_name_flags,false,
			opt->GetBins(),"full_range", opt->GetOutdir()+"SIG_MC_COND_PDF_1D_mbc_fit."+opt->GetPlotFormat(),opt->GetPlotFormat(),
			opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
	//WriteCorrelationMatrixFile(result_mbc,opt->GetOutdir()+"pdf_signal_1D_mbc_correlation.dat");
	//pdf_signal_1D_mbc->ExportPDFValues(opt->GetOutdir()+"pdf_signal_1D_mbc.dat",true);
	result_mbc->Print();

	PrintFitStartBlock("FITTING: deltae");
	shared_ptr<FF_PDF> pdf_signal_1D_deltae = make_shared<FF_PDF_signal_1D_deltae>(deltae, category_name_flags);
	// for this test we float also the mean value
	pdf_signal_1D_deltae->GetVariable("deltae_signal_mean")->setConstant(false);
	RooFitResult *result_deltae = pdf_signal_1D_deltae->GetSimPDF().fitTo(data, SumW2Error(true), NumCPU(opt->GetNumcpu()), Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()));
	pdf_signal_1D_deltae->GetSimPDF().getParameters(data)->Print("v");
	FFUtil::VisualizeFitResult(pdf_signal_1D_deltae,data,deltae,category_name_flags,false,
			opt->GetBins(),"full_range", opt->GetOutdir()+"SIG_MC_COND_PDF_1D_deltae_fit."+opt->GetPlotFormat(),opt->GetPlotFormat(),
			opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
	//WriteCorrelationMatrixFile(result_deltae,opt->GetOutdir()+"pdf_signal_1D_deltae_correlation.dat");
	//pdf_signal_1D_deltae->ExportPDFValues(opt->GetOutdir()+"pdf_signal_1D_deltae.dat",true);
	result_deltae->Print();
	// number of bins we fit one variable in
	const unsigned int nbins = 25;
    double bin_limits[nbins+1];
	// variable we are going to bin
	RooRealVar* binned_var = &mbc;
	// list of all parameters to fit (model which is fitted is selected in loop)
	std::vector<std::string> parameters;
	parameters.push_back("deltae_signal_mean");

    //set the bins ranges gor the selected number of bins
    bin_limits[0]=5.27308; //binned_var->getMin();
    bin_limits[nbins]=5.28608; //binned_var->getMax();
    const double binsize = (bin_limits[nbins]-bin_limits[0])/(double)nbins;
    for (unsigned int i=1; i<nbins; i++) {
        bin_limits[i] = bin_limits[0]+i*binsize;
    }
    
	// storage for the fitted parameter results
	std::map<std::string, std::pair<std::vector<double>,std::vector<double> > > fit_results;
	for(const std::string& s : parameters) {
		std::vector<double> v;
		fit_results[s] = make_pair(v, v);
	}
	// storage for the x values of fitted parameters (i.e. the bin positions)
	std::vector<double> fit_x;
	std::vector<double> fit_xerr;
	// loop over all bins of variable
	for(unsigned int bin = 1; bin <= nbins; ++bin) {
		std::string binned_var_name = binned_var->GetName();
		// compute bin limits and bin centers
//		double bin_limits[7] = {binned_var->getMin(),5.277,5.279,5.28,5.281,5.283,binned_var->getMax()};
//		double binsize = bin_limits[bin] - bin_limits[bin-1];
		double min = bin_limits[bin-1];
		double max = bin_limits[bin];
		fit_x.push_back((max+min)/2.0);
		fit_xerr.push_back(binsize/2.0);
		PrintFitStartBlock("FITTING: "+binned_var_name+" bin "+mp::to_string(bin)+" from ["+mp::to_string(min)+","+mp::to_string(max)+"]");

		// reduce the data to the current bin
		std::string cut = "&& ("+binned_var_name+" >= "+mp::to_string(min)+" && "+binned_var->GetName()+" < "+mp::to_string(max)+")";
		RooDataSet* data_charged_bin = dynamic_cast<RooDataSet*>(data.reduce(("channel==channel::C "+cut).c_str()));
		data_charged_bin->Print();
		RooDataSet* data_neutral_bin = dynamic_cast<RooDataSet*>(data.reduce(("channel==channel::N "+cut).c_str()));
		data_neutral_bin->Print();
		RooDataSet data_bin("data_bin","data_bin",varset,Index(channel),Import("C",*data_charged_bin),Import("N",*data_neutral_bin));

		// buffer range and reset full_range to make proper projections
		std::pair<Double_t,Double_t> range_buffer = std::make_pair(binned_var->getMin("full_range"),binned_var->getMax("full_range"));
		binned_var->setRange("full_range",min,max);

		// select model to fit at each bin
		shared_ptr<FF_PDF> pdf_signal_1D_deltae = make_shared<FF_PDF_signal_1D_deltae>(deltae, category_name_flags);
		pdf_signal_1D_deltae->GetVariable("deltae_signal_mean")->setConstant(false);
		RooFitResult *result_deltae = pdf_signal_1D_deltae->GetSimPDF().fitTo(data_bin, NumCPU(opt->GetNumcpu()), SumW2Error(true), Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()));
		pdf_signal_1D_deltae->GetSimPDF().getParameters(data)->Print("v");
		FFUtil::VisualizeFitResult(pdf_signal_1D_deltae,data_bin,deltae,category_name_flags,true,
				opt->GetBins(),"full_range", opt->GetOutdir()+"SIG_MC_COND_PDF_1D_deltae_fit_"+binned_var_name+"_bin_"+mp::to_string(bin)+"."+opt->GetPlotFormat(),opt->GetPlotFormat(),
				opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
		//WriteCorrelationMatrixFile(result_deltae,opt->GetOutdir()+"pdf_signal_1D_deltae_correlation_"+binned_var_name+"_bin_"+mp::to_string(bin)+".dat");
		//pdf_signal_1D_deltae->ExportPDF(opt->GetOutdir()+"pdf_bkg_continuum_1D_deltae_"+binned_var_name+"_bin_"+mp::to_string(bin)+".root",true);
		result_deltae->Print();

		// reset full_range
		binned_var->setRange("full_range",range_buffer.first,range_buffer.second);

		// store the fit results from each bin
		for(RooRealVar *v : pdf_signal_1D_deltae->GetVariables()) {
			std::string varname = v->GetName();
			fit_results[varname].first.push_back(v->getVal());
			fit_results[varname].second.push_back(v->getError());
		}

		// cleanup
		delete result_deltae;
		delete data_charged_bin;
		delete data_neutral_bin;
	}

	// Visualize results, for each parameter we create one graph
	std::vector<TGraphErrors*> err_graphs;
    std::vector<TH1F*> err_histos;
    std::vector<TF1*> fit_funs;
    std::vector<TLine*> null_lines;
	std::vector<TLine*> global_fit_lines;
	std::vector<std::pair<TLine*,TLine*> > global_fiterr_lines;
    SetRootStyleLarge();
    gStyle->SetPadLeftMargin(0.2);
	TCanvas* canvas = new TCanvas("c_canvas","c_canvas",1*1280,1*1280);
	unsigned int i = 1;
	for(const std::string& s : parameters) {
		// will be deleted by TGraphErrors destructor
		double *x = new double[nbins];
		double *xerr = new double[nbins];
		double *y = new double[nbins];
		double *yerr = new double[nbins];
		double limit = 0;
        err_histos.push_back(new TH1F(s.c_str(),s.c_str(),nbins,bin_limits[0]-mbcp_mean,bin_limits[nbins]-mbcp_mean));
		for(unsigned int j = 0; j < nbins; ++j) {
			x[j] = fit_x[j];
			xerr[j] = fit_xerr[j];
			y[j] = fit_results[s].first[j];
			yerr[j] = fit_results[s].second[j];

			limit = std::max(std::fabs(y[j])+std::fabs(yerr[j]),limit);
            err_histos.back()->SetBinContent(j+1,y[j]);
            err_histos.back()->SetBinError(j+1,yerr[j]);
		}
		limit *= 1.1;
		gStyle->SetOptTitle(0);
//        canvas->cd(1)->SetLeftMargin(4);
		err_graphs.push_back(new TGraphErrors(nbins,x,y,xerr,yerr));
		err_histos.back()->SetNameTitle(("fit_parameter_plot_"+mp::to_string(i)).c_str(),s.c_str());
		err_histos.back()->SetMarkerStyle(8);
		err_histos.back()->SetMarkerSize(3);
		err_histos.back()->SetMarkerColor(kBlue);
		err_histos.back()->Draw("AP");
		err_histos.back()->GetYaxis()->SetTitle("#Delta E mean (GeV)");
		err_histos.back()->GetYaxis()->SetTitleOffset(1.4);
        err_histos.back()->SetMinimum(-limit);
		err_histos.back()->SetMaximum(limit);
		err_histos.back()->GetXaxis()->SetTitle("M'{}_{bc} - #bar{M}'{}_{bc} (GeV/c^{2})");
		err_histos.back()->GetXaxis()->SetLimits(bin_limits[0]-mbcp_mean,bin_limits[nbins]-mbcp_mean);
        
        fit_funs.push_back(new TF1(("F_"+s).c_str(),"pol1",bin_limits[0]-mbcp_mean,bin_limits[nbins]-mbcp_mean));
        err_histos.back()->Fit(("F_"+s).c_str(),"F");
        std::cout<<"Chi2: "<<fit_funs.back()->GetChisquare()/(double)(nbins-2)<<std::endl;
        
        /*
		double fit_val = 0.0;
		double fiterr_val = 0.0;
		if(dynamic_cast<RooRealVar*>(result_deltae->floatParsFinal().find(s.c_str())) != NULL) {
			fit_val = dynamic_cast<RooRealVar*>(result_deltae->floatParsFinal().find(s.c_str()))->getVal();
			fiterr_val = dynamic_cast<RooRealVar*>(result_deltae->floatParsFinal().find(s.c_str()))->getError();
		}
		global_fit_lines.push_back(new TLine(binned_var->getMin()-mbcp_mean,fit_val,binned_var->getMax()-mbcp_mean,fit_val));
		global_fit_lines.back()->SetLineWidth(1);
		global_fit_lines.back()->SetLineColor(kSpring-5);
		global_fit_lines.back()->SetLineStyle(kDashed);
		global_fit_lines.back()->Draw("same");
		global_fiterr_lines.push_back(std::make_pair(
				new TLine(binned_var->getMin()-mbcp_mean,fit_val+fiterr_val,binned_var->getMax()-mbcp_mean,fit_val+fiterr_val),
				new TLine(binned_var->getMin()-mbcp_mean,fit_val-fiterr_val,binned_var->getMax()-mbcp_mean,fit_val-fiterr_val))
		);
		global_fiterr_lines.back().first->SetLineWidth(1);
		global_fiterr_lines.back().first->SetLineColor(kSpring-5);
		global_fiterr_lines.back().first->SetLineStyle(kDashDotted);
		global_fiterr_lines.back().first->Draw("same");
		global_fiterr_lines.back().second->SetLineWidth(1);
		global_fiterr_lines.back().second->SetLineColor(kSpring-5);
		global_fiterr_lines.back().second->SetLineStyle(kDashDotted);
		global_fiterr_lines.back().second->Draw("same");*/
		//canvas->Update();
		++i;
	}

	// draw a horizontal line to split C and N mode parameters

	canvas->SaveAs((opt->GetOutdir()+"SIG_MC_COND_PDF_1D_fit_parameters_of_deltae_in_bins_of_"+binned_var->GetName()+"."+opt->GetPlotFormat()).c_str());
	gStyle->SetOptTitle(0);
	for(TGraphErrors* e : err_graphs) {
		delete e;
	}
	err_graphs.clear();
	for(TLine* l : null_lines) {
		delete l;
	}
	null_lines.clear();
	for(TLine*l : global_fit_lines) {
		delete l;
	}
	global_fit_lines.clear();
	typedef std::pair<TLine*,TLine*> linepair;
	for(linepair p : global_fiterr_lines) {
		delete p.first;
		delete p.second;
	}
	global_fiterr_lines.clear();
	delete canvas;
	delete data_charged;
	delete data_neutral;
	delete result_mbc;;
	delete result_deltae;
    
    SetRootStyleLarge();
    
	PrintFitStartBlock("FITTING: deltae | mbc");
	shared_ptr<FF_PDF> pdf_signal_2D_mbc_deltae =
			make_shared<FF_PDF_signal_2D_mbc_deltae>(mbc, deltae,
					pdf_signal_1D_mbc, category_name_flags);
	RooFitResult *result_mbc_deltae = pdf_signal_2D_mbc_deltae->GetSimPDF().fitTo(data, NumCPU(opt->GetNumcpu()),
			SumW2Error(true), Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()));
	pdf_signal_2D_mbc_deltae->GetSimPDF().getParameters(data)->Print("v");
	FFUtil::VisualizeFitResult(pdf_signal_2D_mbc_deltae,data,mbc,category_name_flags,false,
			opt->GetBins(),"full_range", opt->GetOutdir()+"SIG_MC_COND_PDF_2D_mbc_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
			opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
	FFUtil::VisualizeFitResult(pdf_signal_2D_mbc_deltae,data,deltae,category_name_flags,false,
			opt->GetBins(),"full_range", opt->GetOutdir()+"SIG_MC_COND_PDF_2D_deltae_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
			opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
	//WriteCorrelationMatrixFile(result_mbc_deltae,opt->GetOutdir()+"pdf_signal_2D_mbc_deltae_correlation.dat");
	//pdf_signal_2D_mbc_deltae->ExportPDFValues(opt->GetOutdir()+"pdf_signal_2D_mbc_deltae.dat",true);
	result_mbc_deltae->Print();
	delete result_mbc_deltae;

	// draw projection of conditional PDF in bins of one variable
	for(unsigned int bin = 1; bin <= nbins; ++bin) {
		std::string binned_var_name = binned_var->GetName();
		// compute bin limits and bin centers
		// compute bin limits and bin centers
		//double bin_limits[7] = {binned_var->getMin(),5.277,5.279,5.28,5.281,5.283,binned_var->getMax()};
		double min = bin_limits[bin-1];
		double max = bin_limits[bin];
		PrintFitStartBlock("Projecting: "+binned_var_name+" bin "+mp::to_string(bin)+" from ["+mp::to_string(min)+","+mp::to_string(max)+"] of 2D conditional PDF");

		// buffer range and reset full_range to make proper projections
		std::pair<Double_t,Double_t> range_buffer = std::make_pair(binned_var->getMin("full_range"),binned_var->getMax("full_range"));
		binned_var->setRange("full_range",min,max);

		// create projection of 2D PDF in each bin
		FFUtil::VisualizeFitResult(pdf_signal_2D_mbc_deltae,data,deltae,category_name_flags,false,
				opt->GetBins(),"full_range", opt->GetOutdir()+"SIG_MC_COND_PDF_2D_deltae_projection_"+binned_var_name+"_bin_"+mp::to_string(bin)+"."+opt->GetPlotFormat(),opt->GetPlotFormat(),
				opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());

		// reset full_range
		binned_var->setRange("full_range",range_buffer.first,range_buffer.second);
	}

	// visualize 2D data and fit result
	std::vector<std::string> plot_types;
	plot_types.push_back("LEGO2");
	plot_types.push_back("SURF");
	plot_types.push_back("COL");
	for(const std::string& s : plot_types) {
		canvas = new TCanvas("c_canvas","c_canvas",3*1280,1*1024);
		canvas->Divide(3,1);
		canvas->cd(1);
		TH2F* plot_2D_data_C = mbc.createHistogram("plot_2D_data_C",deltae,"Entries");
		data.fillHistogram(plot_2D_data_C,RooArgList(mbc,deltae),"channel==channel::C","full_range");
		plot_2D_data_C->Draw(s.c_str());
		canvas->cd(2);
		TH2F* plot_2D_data_smooth_C = mbc.createHistogram("plot_2D_data_smooth_C",deltae,"Entries");
		data.fillHistogram(plot_2D_data_smooth_C,RooArgList(mbc,deltae),"channel==channel::C","full_range");
		plot_2D_data_smooth_C->Smooth(1,"k5a");
		plot_2D_data_smooth_C->Draw(s.c_str());
		canvas->cd(3);
		TH2F* plot_2D_pdf_C = mbc.createHistogram("plot_2D_pdf_C",deltae,"Entries");
		pdf_signal_2D_mbc_deltae->GetPDF("C").fillHistogram(plot_2D_pdf_C,RooArgList(mbc,deltae));
		plot_2D_pdf_C->Draw(s.c_str());
		canvas->SaveAs((opt->GetOutdir()+"SIG_MC_COND_PDF_2D_"+s+"_data_pdf."+opt->GetPlotFormat()).c_str());
		delete plot_2D_data_C;
		delete plot_2D_data_smooth_C;
		delete plot_2D_pdf_C;
		delete canvas;
	}
 
}

}
