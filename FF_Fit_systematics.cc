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
#include "libRooComplexPDF/RooMassHelicityAngles.h"

using namespace FFUtil;

namespace FFFits {

void FitSystematics_Efficiency(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt) {
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
	// some extra range for integrals
	mbc.setRange("full_LASS_range",5.26,5.29);
	deltae.setRange("full_LASS_range",-0.15,0.15);
	chil0_ma.setRange("full_LASS_range",0.985,1.05);
	nb_prime.setRange("full_LASS_range",-10.0,10.0);
	mass_kpi.setRange("full_LASS_range",m_K+m_Pi+0.001,1.6);
	hel_phi.setRange("full_LASS_range",-TMath::Pi(),TMath::Pi());
	hel_th1.setRange("full_LASS_range",-1.0,1.0);
	hel_th2.setRange("full_LASS_range",-1.0,1.0);

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
			hel_th2, nb_prime, flavor, category_name_flags_channel,false,(RooRealVar*)NULL);
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
			deltae, chil0_ma, nb_prime, category_name_flags_channel,false,pdf_bkg_continuum_9D->GetVariable("chil0_ma_resolution"));
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

	PrintFitStartBlock("REPEATING FIT WITH STARTING VALUES");
	std::vector<std::map<std::string,double>> physics_param_results;
	std::vector<std::array<double,4>> integrals_results;
	std::vector<RooFitResult*> fit_results;
	std::string outdir_files = opt->GetOutdir()+"/data_fit_"+(opt->GetMinos() ? "_minos" : "")+"/";
	mp::execute_command("mkdir -p "+outdir_files);
	// start loop of fits
	for(int i = 1; i <= 15; ++i) {
		std::stringstream ss;
		ss << "sample_" << i;
		PrintFitStartBlock("NOW FITTING SAMPLE "+ss.str());
		std::cout << "INFO: Data fit: " << i << std::endl;

		if(i > 1) {
			unsigned int to_modify = (i-2)/2;
			bool negative = (i-2)%2;

			std::cout << "INFO: Modify EffParam[" << to_modify << "]=" << EffParam[to_modify] << " by " << (negative ? "-" : "+") << EffParamErr[to_modify] << "\n";

			if(negative)
				EffParam[to_modify] = EffParam[to_modify] - EffParamErr[to_modify];
			else
				EffParam[to_modify] = EffParam[to_modify] + EffParamErr[to_modify];
		}

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
			fit_results.push_back(pdf_data_9D->GetSimPDF().fitTo(data, SumW2Error(false), NumCPU(opt->GetNumcpu()),
					Timer(true), Save(true), InitialHesse(false), Minos(minos_pars), Extended(true)));
		} else {
			fit_results.push_back(pdf_data_9D->GetSimPDF().fitTo(data, SumW2Error(false), NumCPU(opt->GetNumcpu()),
					Timer(true), Save(true), InitialHesse(false), Minos(false), Extended(true)));
		}
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
		//PrintPhysicsParametersLatex(PropagateErrors(pdf_data_9D,pdf_signal_9D,fit_results.back(),opt,varset_9D,false));
		physics_param_results.push_back(ComputePhysicsParameters(pdf_data_9D,pdf_signal_9D,varset_9D,true));
		std::cout << mp::map_to_string(physics_param_results.back()) << std::endl;
		//fit_results.back()->correlationMatrix().Print();
		//fit_results.back()->covarianceMatrix().Print();
		//WriteCorrelationMatrixFile(fit_results.back(),opt->GetOutdir()+"pdf_data_9D_correlation.dat");
		//WriteCovarianceMatrixFile(fit_results.back(),opt->GetOutdir()+"pdf_data_9D_covariance.dat");
		//pdf_data_9D->ExportPDFValues(opt->GetOutdir()+"pdf_data_9D.dat",true);

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

		PrintFitStartBlock("Compute normalization integrals",2);
		std::array<std::string, 2> ranges = {{"full_range", "full_LASS_range"}};
		std::array<double,4> integrals = {{0,0,0,0}};
		for(const std::string& r : ranges) {
			std::cout << "Currently using acceptance function: " << FFUtil::GetAcceptanceString(acceptance) << "\n";
			RooAbsReal* I_tot = pdf_signal_9D->GetPDF("C").createIntegral(varset_9D,r.c_str());

			pdf_data_9D->GetVariable("amp_00")->setVal(pt.get<double>("amp_00"));
			pdf_data_9D->GetVariable("amp_00_delta")->setVal(pt.get<double>("amp_00_delta"));
			pdf_data_9D->GetVariable("amp_10")->setVal(pt.get<double>("amp_10"));
			pdf_data_9D->GetVariable("amp_10_delta")->setVal(pt.get<double>("amp_10_delta"));
			pdf_data_9D->GetVariable("amp_1para")->setVal(pt.get<double>("amp_1para"));
			pdf_data_9D->GetVariable("amp_1para_delta")->setVal(pt.get<double>("amp_1para_delta"));
			pdf_data_9D->GetVariable("amp_1perp")->setVal(pt.get<double>("amp_1perp"));
			pdf_data_9D->GetVariable("amp_1perp_delta")->setVal(pt.get<double>("amp_1perp_delta"));
			pdf_data_9D->GetVariable("amp_20")->setVal(pt.get<double>("amp_20"));
			pdf_data_9D->GetVariable("amp_20_delta")->setVal(pt.get<double>("amp_20_delta"));
			pdf_data_9D->GetVariable("amp_2para")->setVal(pt.get<double>("amp_2para"));
			pdf_data_9D->GetVariable("amp_2para_delta")->setVal(pt.get<double>("amp_2para_delta"));
			pdf_data_9D->GetVariable("amp_2perp")->setVal(pt.get<double>("amp_2perp"));
			pdf_data_9D->GetVariable("amp_2perp_delta")->setVal(pt.get<double>("amp_2perp_delta"));
			double N_tot = I_tot->getVal();
			std::cout << "Integral over signal PDF in the " << r << " range: " << N_tot << "\n";
			if(r == "full_range")
				integrals[0] = N_tot;

			pdf_signal_9D->GetVariable("amp_00")->setVal(pt.get<double>("amp_00"));
			pdf_signal_9D->GetVariable("amp_00_delta")->setVal(pt.get<double>("amp_00_delta"));
			pdf_signal_9D->GetVariable("amp_10")->setVal(0.0);
			pdf_signal_9D->GetVariable("amp_10_delta")->setVal(0.0);
			pdf_signal_9D->GetVariable("amp_1perp")->setVal(0.0);
			pdf_signal_9D->GetVariable("amp_1perp_delta")->setVal(0.0);
			pdf_signal_9D->GetVariable("amp_1para")->setVal(0.0);
			pdf_signal_9D->GetVariable("amp_1para_delta")->setVal(0.0);
			pdf_signal_9D->GetVariable("amp_20")->setVal(0.0);
			pdf_signal_9D->GetVariable("amp_20_delta")->setVal(0.0);
			pdf_signal_9D->GetVariable("amp_2perp")->setVal(0.0);
			pdf_signal_9D->GetVariable("amp_2perp_delta")->setVal(0.0);
			pdf_signal_9D->GetVariable("amp_2para")->setVal(0.0);
			pdf_signal_9D->GetVariable("amp_2para_delta")->setVal(0.0);
			double N_LASS = I_tot->getVal();
			std::cout << "Integral over LASS PDF in the " << r << " range: " << N_LASS << "\n";
			if(r == "full_LASS_range")
				integrals[1] = N_LASS;

			pdf_signal_9D->GetVariable("amp_00")->setVal(0.0);
			pdf_signal_9D->GetVariable("amp_00_delta")->setVal(0.0);
			pdf_signal_9D->GetVariable("amp_10")->setVal(pt.get<double>("amp_10"));
			pdf_signal_9D->GetVariable("amp_10_delta")->setVal(pt.get<double>("amp_10_delta"));
			pdf_signal_9D->GetVariable("amp_1para")->setVal(pt.get<double>("amp_1para"));
			pdf_signal_9D->GetVariable("amp_1para_delta")->setVal(pt.get<double>("amp_1para_delta"));
			pdf_signal_9D->GetVariable("amp_1perp")->setVal(pt.get<double>("amp_1perp"));
			pdf_signal_9D->GetVariable("amp_1perp_delta")->setVal(pt.get<double>("amp_1perp_delta"));
			double N_K892 = I_tot->getVal();
			std::cout << "Integral over K892 PDF in the " << r << " range: " << N_K892 << "\n";
			if(r == "full_range")
				integrals[2] = N_K892;

			pdf_signal_9D->GetVariable("amp_10")->setVal(0.0);
			pdf_signal_9D->GetVariable("amp_10_delta")->setVal(0.0);
			pdf_signal_9D->GetVariable("amp_1perp")->setVal(0.0);
			pdf_signal_9D->GetVariable("amp_1perp_delta")->setVal(0.0);
			pdf_signal_9D->GetVariable("amp_1para")->setVal(0.0);
			pdf_signal_9D->GetVariable("amp_1para_delta")->setVal(0.0);
			pdf_signal_9D->GetVariable("amp_20")->setVal(pt.get<double>("amp_20"));
			pdf_signal_9D->GetVariable("amp_20_delta")->setVal(pt.get<double>("amp_20_delta"));
			pdf_signal_9D->GetVariable("amp_2para")->setVal(pt.get<double>("amp_2para"));
			pdf_signal_9D->GetVariable("amp_2para_delta")->setVal(pt.get<double>("amp_2para_delta"));
			pdf_signal_9D->GetVariable("amp_2perp")->setVal(pt.get<double>("amp_2perp"));
			pdf_signal_9D->GetVariable("amp_2perp_delta")->setVal(pt.get<double>("amp_2perp_delta"));
			double N_K21430 = I_tot->getVal();
			std::cout << "Integral over K2(2430) PDF in the " << r << " range: " << N_K21430 << "\n";
			if(r == "full_range")
				integrals[3] = N_K21430;

			delete I_tot;
		}

		integrals_results.push_back(integrals);

		mass_helicity_angles_norm_cache_integrator->ClearAllIntegrals();
		mass_helicity_angles_real_cache_integrator->ClearAllIntegrals();
		mass_helicity_angles_imag_cache_integrator->ClearAllIntegrals();
		mass_helicity_angles_norm_cache_integrator_phi_th1_th2->ClearAllIntegrals();
		mass_helicity_angles_real_cache_integrator_phi_th1_th2->ClearAllIntegrals();
		mass_helicity_angles_imag_cache_integrator_phi_th1_th2->ClearAllIntegrals();
		mass_helicity_angles_norm_cache_integrator_m_th1_th2->ClearAllIntegrals();
		mass_helicity_angles_real_cache_integrator_m_th1_th2->ClearAllIntegrals();
		mass_helicity_angles_imag_cache_integrator_m_th1_th2->ClearAllIntegrals();
		mass_helicity_angles_norm_cache_integrator_m_phi_th2->ClearAllIntegrals();
		mass_helicity_angles_real_cache_integrator_m_phi_th2->ClearAllIntegrals();
		mass_helicity_angles_imag_cache_integrator_m_phi_th2->ClearAllIntegrals();
		mass_helicity_angles_norm_cache_integrator_m_phi_th1->ClearAllIntegrals();
		mass_helicity_angles_real_cache_integrator_m_phi_th1->ClearAllIntegrals();
		mass_helicity_angles_imag_cache_integrator_m_phi_th1->ClearAllIntegrals();

		if(i > 1) {
			unsigned int to_modify = (i-2)/2;
			bool negative = (i-2)%2;

			if(negative)
				EffParam[to_modify] = EffParam[to_modify] + EffParamErr[to_modify];
			else
				EffParam[to_modify] = EffParam[to_modify] - EffParamErr[to_modify];

			std::cout << "INFO: Reset EffParam[" << to_modify << "] to " << EffParam[to_modify] << "\n";
		}
	}
	// end of fit

	PrintFitStartBlock("Checking deviations");
	std::map<std::string, double> delta_map;
	std::map<std::string, double> delta_map_relative;
	for(auto p : physics_param_results[0]) {
		std::cout << "INFO: Checking deviation for physics parameter " << p.first << " with nominal value " << p.second << "\n";
		double delta = 0;
		for(std::size_t i = 1; i < physics_param_results.size(); ++i) {
			double new_delta = std::fabs(physics_param_results[i][p.first] - p.second);
			if(new_delta > delta) {
				delta = new_delta;
				std::cout << "Larger deviation found for index " << i << " with value " << physics_param_results[i][p.first] << std::endl;
			}
		}
		delta_map[p.first] = delta;
		delta_map_relative[p.first] = delta/p.second;
	}

	std::cout << "INFO: Delta of physics parameters due to efficiency function errors (absolute values)\n";
	std::cout << mp::map_to_string(delta_map) << std::endl;
	std::cout << "INFO: Delta of physics parameters due to efficiency function errors (relative values)\n";
	std::cout << mp::map_to_string(delta_map_relative) << std::endl;

	std::map<std::string, double> delta_integral_map;
	std::map<std::string, double> delta_integral_map_relative;
	delta_integral_map["ALL"] = 0;
	delta_integral_map["LASS"] = 0;
	delta_integral_map["K892"] = 0;
	delta_integral_map["K21430"] = 0;
	std::array<std::string,4> integral_names = {{"ALL", "LASS", "K892", "K21430"}};

	for(unsigned int j = 0; j < 4; ++j){
		std::cout << "INFO: Checking deviation for integrals " << integral_names[j] << " with nominal value " << integrals_results[0][j] << "\n";
		double delta = 0;
		for(std::size_t i = 1; i < integrals_results.size(); ++i) {
			double new_delta = std::fabs(integrals_results[i][j] - integrals_results[0][j]);
			if(new_delta > delta) {
				delta = new_delta;
				std::cout << "Larger deviation found for index " << i << " with value " << integrals_results[i][j] << std::endl;
			}
		}
		delta_integral_map[integral_names[j]] = delta;
		delta_integral_map_relative[integral_names[j]] = delta/integrals_results[0][j];
	}

	std::cout << "INFO: Delta of integrals due to efficiency function errors (absolute values)\n";
	std::cout << mp::map_to_string(delta_integral_map) << std::endl;
	std::cout << "INFO: Delta of integrals due to efficiency function errors (relative values)\n";
	std::cout << mp::map_to_string(delta_integral_map_relative) << std::endl;

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
	delete result_mass_kpi;
}

void FitSystematics_Resolution(shared_ptr<FFOpt> opt) {
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
	RooRealVar mbc("mbc","m_{bc}",5.26,5.29,"GeV");
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
	// event weight
	RooRealVar weight("weight","weight",-999999,999999);
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
	varset.add(weight);
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
	variables_4D.push_back(&mbc);
	variables_4D.push_back(&deltae);
	variables_4D.push_back(&chil0_ma);
	variables_4D.push_back(&nb_prime);
	RooArgSet varset_4D(mbc,deltae,chil0_ma,nb_prime);

	std::vector<RooRealVar*> variables_4D_angles;
	variables_4D_angles.push_back(&mass_kpi);
	variables_4D_angles.push_back(&hel_phi);
	variables_4D_angles.push_back(&hel_th1);
	variables_4D_angles.push_back(&hel_th2);
	RooArgSet varset_5D_angles(mass_kpi,hel_phi,hel_th1,hel_th2,flavor);

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

	PrintFitStartBlock("SETUP: 9D Fit");

	shared_ptr<FF_PDF> pdf_bkg_continuum_9D = make_shared<FF_PDF_bkg_continuum_9D_mbc_deltae_chil0_ma_mass_kpi_hel_phi_hel_th1_hel_th2_nb_prime_flavor>(mbc,
			deltae, chil0_ma, mass_kpi, hel_phi, hel_th1,
			hel_th2, nb_prime, flavor, category_name_flags_channel, false, (RooRealVar*)NULL);
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
	shared_ptr<FF_PDF> pdf_signal_4D = 	make_shared<FF_PDF_signal_4D_mbc_deltae_chil0_ma_nb_prime>(mbc,
			deltae, chil0_ma, nb_prime, category_name_flags_channel,false,pdf_bkg_continuum_9D->GetVariable("chil0_ma_resolution"));
	pdf_signal_4D->ImportPDFValues(opt->GetOutdir()+"pdf_signal_4D.dat",true);
	pdf_signal_4D->SetVariablesConst();
	//pdf_signal_4D->GetVariable("C_deltae_signal_width_core")->setConstant(false);
	shared_ptr<FF_PDF> pdf_signal_5D_angles = make_shared<FF_PDF_signal_5D_mass_helicity_angles_CPV>(mass_kpi,
			hel_th1, hel_th2, hel_phi, flavor, PRODUCT_4D,
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
			20000.0, category_name_flags_channel);

	pdf_data_9D->GetVariable("signal_nevents")->setVal(1100); // 1000 for C
	pdf_data_9D->GetVariable("C_bkg_continuum_nevents")->setVal(7350*2+350*2);
	pdf_data_9D->GetVariable("C_bkg_bbar_wrongphif0980_nevents")->setVal(120.0);

	std::map<std::string, double> truth_map;
	truth_map["signal_nevents"] = 1100; // 1000 for C
	truth_map["C_bkg_continuum_nevents"] = 7350*2+350*2;
	truth_map["C_bkg_bbar_wrongphif0980_nevents"] = 120.0;
	truth_map["mbc_bkg_continuum_c"] = pdf_bkg_continuum_9D->GetVariable("mbc_bkg_continuum_c")->getVal();
	truth_map["amp_00"] = pt.get<double>("amp_00");
	truth_map["amp_10"] = pt.get<double>("amp_10");
	truth_map["amp_1para"] = pt.get<double>("amp_1para");
	truth_map["amp_1perp"] = pt.get<double>("amp_1perp");
	truth_map["amp_20"] = pt.get<double>("amp_20");
	truth_map["amp_2para"] = pt.get<double>("amp_2para");
	truth_map["amp_2perp"] = pt.get<double>("amp_2perp");
	truth_map["phase_00"] = 0.0;
	truth_map["phase_10"] = pt.get<double>("phase_10");
	truth_map["phase_1para"] = pt.get<double>("phase_1para");
	truth_map["phase_1perp"] = pt.get<double>("phase_1perp");
	truth_map["phase_20"] = pt.get<double>("phase_20");
	truth_map["phase_2para"] = pt.get<double>("phase_2para");
	truth_map["phase_2perp"] = pt.get<double>("phase_2perp");
	truth_map["amp_00_delta"] = pt.get<double>("amp_00_delta");
	truth_map["amp_10_delta"] = pt.get<double>("amp_10_delta");
	truth_map["amp_1para_delta"] = pt.get<double>("amp_1para_delta");
	truth_map["amp_1perp_delta"] = pt.get<double>("amp_1perp_delta");
	truth_map["amp_20_delta"] = pt.get<double>("amp_20_delta");
	truth_map["amp_2para_delta"] = pt.get<double>("amp_2para_delta");
	truth_map["amp_2perp_delta"] = pt.get<double>("amp_2perp_delta");
	truth_map["phase_00_delta"] = 0.0;
	truth_map["phase_10_delta"] = pt.get<double>("phase_10_delta");
	truth_map["phase_1para_delta"] = pt.get<double>("phase_1para_delta");
	truth_map["phase_1perp_delta"] = pt.get<double>("phase_1perp_delta");
	truth_map["phase_20_delta"] = pt.get<double>("phase_20_delta");
	truth_map["phase_2para_delta"] = pt.get<double>("phase_2para_delta");
	truth_map["phase_2perp_delta"] = pt.get<double>("phase_2perp_delta");

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

	RooDataSet* data_set_gen = NULL;
	std::vector<std::map<std::string, double> > physics_param_results;
	std::map<std::string,double> physics_parameters_expectation;

	PrintFitStartBlock("TOY-STUDY: 9D Fit");
	if(opt->GetNToys() > 0) {
		RooMCStudy* mcstudy = new RooMCStudy(pdf_data_9D->GetSimPDF(),varset_9D);
		std::vector<RooFitResult*> fit_results;
		std::vector<RooDataSet*> data_sets;
		std::string outdir_files = opt->GetOutdir()+"/toystudy_"+mp::to_string(opt->GetNToys())+(opt->GetMinos() ? "_minos" : "")+"/";
		mp::execute_command("mkdir -p "+outdir_files);
		for(int i = 1; i <= opt->GetNToys(); ++i) {
			if(opt->GetSingleToy()) {
				if(i != (int)opt->GetSingleToy()) { // continue until we have the single toy we want
					continue;
				} else { // initialize random seed for this single toy
					RooRandom::randomGenerator()->SetSeed(opt->GetRandomSeed()+opt->GetSingleToy());
				}
			}
			std::stringstream ss;
			ss << "sample_" << i;
			std::cout << "INFO: Toy Sample: " << i << std::endl;
			// resetting continuum shape and yields to default values
			pdf_bkg_continuum_9D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_continuum_8D.dat",false);
			pdf_bkg_continuum_9D->SetVariablesConst();
			pdf_bkg_continuum_9D->GetVariable("mbc_bkg_continuum_c")->setConstant(false);
			//pdf_signal_9D->ImportPDFValues(opt->GetOutdir()+"pdf_signal_4D.dat",false);
			pdf_data_9D->GetVariable("signal_nevents")->setVal(truth_map["signal_nevents"]); // 1000 for C
			pdf_data_9D->GetVariable("C_bkg_continuum_nevents")->setVal(truth_map["C_bkg_continuum_nevents"]);
			pdf_data_9D->GetVariable("C_bkg_bbar_wrongphif0980_nevents")->setVal(truth_map["C_bkg_bbar_wrongphif0980_nevents"]);

			// reset to generation values
			pdf_data_9D->GetVariable("amp_00")->setVal(truth_map["amp_00"]);
			pdf_data_9D->GetVariable("amp_10")->setVal(truth_map["amp_10"]);
			pdf_data_9D->GetVariable("amp_1para")->setVal(truth_map["amp_1para"]);
			pdf_data_9D->GetVariable("amp_1perp")->setVal(truth_map["amp_1perp"]);
			pdf_data_9D->GetVariable("amp_20")->setVal(truth_map["amp_20"]);
			pdf_data_9D->GetVariable("amp_2para")->setVal(truth_map["amp_2para"]);
			pdf_data_9D->GetVariable("amp_2perp")->setVal(truth_map["amp_2perp"]);

			pdf_data_9D->GetVariable("phase_10")->setVal(truth_map["phase_10"]);
			pdf_data_9D->GetVariable("phase_1para")->setVal(truth_map["phase_1para"]);
			pdf_data_9D->GetVariable("phase_1perp")->setVal(truth_map["phase_1perp"]);
			pdf_data_9D->GetVariable("phase_20")->setVal(truth_map["phase_20"]);
			pdf_data_9D->GetVariable("phase_2para")->setVal(truth_map["phase_2para"]);
			pdf_data_9D->GetVariable("phase_2perp")->setVal(truth_map["phase_2perp"]);

			pdf_data_9D->GetVariable("amp_00_delta")->setVal(truth_map["amp_00_delta"]);
			pdf_data_9D->GetVariable("amp_10_delta")->setVal(truth_map["amp_10_delta"]);
			pdf_data_9D->GetVariable("amp_1para_delta")->setVal(truth_map["amp_1para_delta"]);
			pdf_data_9D->GetVariable("amp_1perp_delta")->setVal(truth_map["amp_1perp_delta"]);
			pdf_data_9D->GetVariable("amp_20_delta")->setVal(truth_map["amp_20_delta"]);
			pdf_data_9D->GetVariable("amp_2para_delta")->setVal(truth_map["amp_2para_delta"]);
			pdf_data_9D->GetVariable("amp_2perp_delta")->setVal(truth_map["amp_2perp_delta"]);

			pdf_data_9D->GetVariable("phase_10_delta")->setVal(truth_map["phase_10_delta"]);
			pdf_data_9D->GetVariable("phase_1para_delta")->setVal(truth_map["phase_1para_delta"]);
			pdf_data_9D->GetVariable("phase_1perp_delta")->setVal(truth_map["phase_1perp_delta"]);
			pdf_data_9D->GetVariable("phase_20_delta")->setVal(truth_map["phase_20_delta"]);
			pdf_data_9D->GetVariable("phase_2para_delta")->setVal(truth_map["phase_2para_delta"]);
			pdf_data_9D->GetVariable("phase_2perp_delta")->setVal(truth_map["phase_2perp_delta"]);

			std::cout << "INFO: Expected physics parameters for truth values given" << std::endl;
			physics_parameters_expectation = ComputePhysicsParameters(pdf_data_9D, pdf_signal_9D, varset_9D);
			std::cout << mp::map_to_string(physics_parameters_expectation) << std::endl;

			std::cout << "INFO: Expected events for C: " << pdf_data_9D->GetPDF("C").expectedEvents(varset_9D) << std::endl;
			if(i == 1)
				data_set_gen = pdf_data_9D->GetPDF("C").generate(varset_9D,RooRandom::randomGenerator()->Poisson(pdf_data_9D->GetPDF("C").expectedEvents(varset_9D)));

			// do additional smearing with resolution
			if(i > 1) {
				const double resol_mass_kpi = 0.0038;
				const double resol_hel_phi = 0.0237;
				const double resol_hel_th1 = 0.00353;
				const double resol_hel_th2 = 0.00883;

				RooRealVar* var;
				const RooArgSet* set;
				Long64_t nentries = data_set_gen->numEntries();
				for(unsigned int i = 0; i < nentries; ++i) {
					set = data_set_gen->get(i);
					var = static_cast<RooRealVar*>(set->find("mass_kpi"));
					var->setVal(var->getVal() + RooRandom::randomGenerator()->Gaus(0,resol_mass_kpi));

					var = static_cast<RooRealVar*>(set->find("hel_phi"));
					var->setVal(var->getVal() + RooRandom::randomGenerator()->Gaus(0,resol_hel_phi));

					var = static_cast<RooRealVar*>(set->find("hel_th1"));
					var->setVal(var->getVal() + RooRandom::randomGenerator()->Gaus(0,resol_hel_th1));

					var = static_cast<RooRealVar*>(set->find("hel_th2"));
					var->setVal(var->getVal() + RooRandom::randomGenerator()->Gaus(0,resol_hel_th2));
				}
			}

			data_sets.push_back(new RooDataSet(("data"+ss.str()).c_str(),ss.str().c_str(),varset_9D,Index(channel),
					Import("C",*data_set_gen)));
			data_sets.back()->Print();

			fit_results.push_back(pdf_data_9D->GetSimPDF().fitTo(*data_sets.back(), SumW2Error(false), NumCPU(opt->GetNumcpu()),
					Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()), Extended(true)));
			fit_results.back()->Print();
			PrintFitResultLatex(pdf_data_9D,truth_map);
			//PrintPhysicsParametersLatex(PropagateErrors(pdf_data_9D,pdf_signal_9D,fit_results.back(),opt,varset_9D,false));
			auto physics_results = ComputePhysicsParameters(pdf_data_9D,pdf_signal_9D,varset_9D,false);
			std::cout << mp::map_to_string(physics_results) << std::endl;
			physics_param_results.push_back(physics_results);
			mcstudy->addFitResult(*fit_results.back());

			std::ofstream fout((outdir_files+"TOY_9D_fit_result_"+ss.str().c_str()+".dat").c_str());
			if(fout.is_open()) {
				for(const std::string& p : parameters) {
					fout << p << ";";
					fout << truth_map[p] << ";";
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

		for(RooDataSet* s : data_sets) {
			delete s;
		}
		for(RooFitResult* r : fit_results) {
			delete r;
		}
		delete mcstudy;
	}

	if(data_set_gen != NULL)
		delete data_set_gen;

	PrintFitStartBlock("Checking deviations");
	std::map<std::string, double> delta_map;
	std::map<std::string, double> delta_map_relative;
	for(auto p : physics_param_results[0]) {
		std::cout << "INFO: Checking deviation for physics parameter " << p.first << " with nominal value " << p.second << "\n";
		double delta = 0;
		for(std::size_t i = 1; i < physics_param_results.size(); ++i) {
			double new_delta = std::fabs(physics_param_results[i][p.first] - p.second);
			if(new_delta > delta) {
				delta = new_delta;
				std::cout << "Larger deviation found for index " << i << " with value " << physics_param_results[i][p.first] << std::endl;
			}
		}
		delta_map[p.first] = delta;
		delta_map_relative[p.first] = delta/p.second;
	}

	std::cout << "INFO: Delta of physics parameters due to resolution (absolute values)\n";
	std::cout << mp::map_to_string(delta_map) << std::endl;
	std::cout << "INFO: Delta of physics parameters due to resolution (relative values)\n";
	std::cout << mp::map_to_string(delta_map_relative) << std::endl;

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
}

void FitSystematics_PDF(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt) {
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
	// some extra range for integrals
	mbc.setRange("full_LASS_range",5.26,5.29);
	deltae.setRange("full_LASS_range",-0.15,0.15);
	chil0_ma.setRange("full_LASS_range",0.985,1.05);
	nb_prime.setRange("full_LASS_range",-10.0,10.0);
	mass_kpi.setRange("full_LASS_range",m_K+m_Pi+0.001,1.6);
	hel_phi.setRange("full_LASS_range",-TMath::Pi(),TMath::Pi());
	hel_th1.setRange("full_LASS_range",-1.0,1.0);
	hel_th2.setRange("full_LASS_range",-1.0,1.0);

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
			hel_th2, nb_prime, flavor, category_name_flags_channel,false,(RooRealVar*)NULL);
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
			deltae, chil0_ma, nb_prime, category_name_flags_channel,false,pdf_bkg_continuum_9D->GetVariable("chil0_ma_resolution"));
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

	std::map<std::string, std::pair<double,double> > fixed_parameters;
	for(auto var : pdf_data_9D->GetVariables()) {
		if(var->isConstant()){
			std::string varname = var->GetName();
			std::string::size_type n = varname.find("N_",0);
			if(n == 0) {
				std::cout << "INFO: Parameter " << var->GetName() << " is from neutral channel and skipped.\n";
			} else {
				if(varname == "a")
					var->setError(std::sqrt(0.09*0.09+0.06*0.06));
				if(varname == "b")
					var->setError(std::sqrt(0.36*0.36+0.67*0.67));
				if(varname == "mass_0")
					var->setError(std::sqrt(0.005*0.005+0.005*0.005));
				if(varname == "width_0")
					var->setError(std::sqrt(0.006*0.006+0.021*0.021));
				if(varname == "mass_1")
					var->setError(0.00022);
				if(varname == "width_1")
					var->setError(0.0006);
				if(varname == "r_1")
					var->setError(0.7);
				if(varname == "mass_2")
					var->setError(0.0013);
				if(varname == "width_2")
					var->setError(0.005);
				if(varname == "r_2")
					var->setError(1.3);
				if(varname == "chil0_ma_bkg_continuum_mean" || varname == "chil0_ma_signal_mean")
					var->setError(0.00005);
				if(varname == "chil0_ma_bkg_continuum_width" || varname == "chil0_ma_signal_width")
					var->setError(0.00004);
				if(varname == "chil0_ma_bkg_continuum_r" || varname == "chil0_ma_signal_r")
					var->setError(1);
				if(varname == "mass_kpi_bkg_bbar_wrongphif0980_mean" || varname == "mass_kpi_bkg_continuum_mean")
					var->setError(0.022);
				if(varname == "mass_kpi_bkg_bbar_wrongphif0980_width" || varname == "mass_kpi_bkg_continuum_width")
					var->setError(0.0006);
				if(varname == "mass_kpi_bkg_bbar_wrongphif0980_r" || varname == "mass_kpi_bkg_continuum_r")
					var->setError(1);
				if(varname == "chil0_ma_bkg_bbar_wrongphif0980_pole")
					var->setError(0.01);
				if(varname == "chil0_ma_bkg_bbar_wrongphif0980_g1")
					var->setError(0.018);
				if(varname == "chil0_ma_bkg_bbar_wrongphif0980_g2" || varname == "chil0_ma_bkg_bbar_wrongphif0980_g3")
					var->setError(0.031);

				fixed_parameters[var->GetName()] = std::make_pair((double)var->getVal(), (double)var->getError());
				std::cout << "INFO: Adding parameter " << var->GetName() << " with mean " << var->getVal() << " and error " << var->getError() << " to list!\n";
			}
		} else {
			std::cout << "INFO: Parameter " << var->GetName() << " is floated and skipped.\n";
		}
	}

	std::vector<std::pair<std::string, std::pair<double,double> > > parameters_to_vary;
	std::cout << "INFO: Parameters with zero error (do not vary):\n";
	for(auto p : fixed_parameters) {
		if(p.second.second == 0)
			std::cout << p.first << "  " << mp::pair_to_string(p.second) << "\n";
		else
			parameters_to_vary.push_back(std::make_pair(p.first,p.second));
	}

	std::cout << "INFO: Parameters to vary:\n";
	for(auto p : parameters_to_vary) {
		std::cout << p.first << "  " << mp::pair_to_string(p.second) << "\n";
	}

	PrintFitStartBlock("REPEATING FIT WITH STARTING VALUES");
	std::vector<std::map<std::string,double>> physics_param_results;
	std::vector<std::string> varied_parameter;
	std::vector<RooFitResult*> fit_results;
	std::string outdir_files = opt->GetOutdir()+"/data_fit_"+(opt->GetMinos() ? "_minos" : "")+"/";
	mp::execute_command("mkdir -p "+outdir_files);
	// start loop of fits
	for(int i = 1; i <= 1+(int)parameters_to_vary.size()*2; ++i) {
		std::stringstream ss;
		ss << "sample_" << i;
		PrintFitStartBlock("NOW FITTING SAMPLE "+ss.str());
		std::cout << "INFO: Data fit: " << i << std::endl;

		if(i == 1) {
			varied_parameter.push_back("none");
		}
		if(i > 1) {
			unsigned int to_modify = (i-2)/2;
			bool negative = (i-2)%2;

			std::cout << "INFO: Modify param[" << to_modify << "]=" << parameters_to_vary[to_modify].first << " by " << (negative ? "-" : "+") << parameters_to_vary[to_modify].second.second << "\n";
			if(negative)
				pdf_data_9D->GetVariable(parameters_to_vary[to_modify].first)->setVal(parameters_to_vary[to_modify].second.first - parameters_to_vary[to_modify].second.second);
			else
				pdf_data_9D->GetVariable(parameters_to_vary[to_modify].first)->setVal(parameters_to_vary[to_modify].second.first + parameters_to_vary[to_modify].second.second);
			std::cout << "INFO: Value of " << parameters_to_vary[to_modify].first << " is " << pdf_data_9D->GetVariable(parameters_to_vary[to_modify].first)->getVal() << "\n";
			varied_parameter.push_back(parameters_to_vary[to_modify].first + (negative ? "-" : "+"));
		}

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
			fit_results.push_back(pdf_data_9D->GetSimPDF().fitTo(data, SumW2Error(false), NumCPU(opt->GetNumcpu()),
					Timer(true), Save(true), InitialHesse(false), Minos(minos_pars), Extended(true)));
		} else {
			fit_results.push_back(pdf_data_9D->GetSimPDF().fitTo(data, SumW2Error(false), NumCPU(opt->GetNumcpu()),
					Timer(true), Save(true), InitialHesse(false), Minos(false), Extended(true)));
		}
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
		//PrintPhysicsParametersLatex(PropagateErrors(pdf_data_9D,pdf_signal_9D,fit_results.back(),opt,varset_9D,false));
		physics_param_results.push_back(ComputePhysicsParameters(pdf_data_9D,pdf_signal_9D,varset_9D,true));
		std::cout << mp::map_to_string(physics_param_results.back()) << std::endl;
		//fit_results.back()->correlationMatrix().Print();
		//fit_results.back()->covarianceMatrix().Print();
		//WriteCorrelationMatrixFile(fit_results.back(),opt->GetOutdir()+"pdf_data_9D_correlation.dat");
		//WriteCovarianceMatrixFile(fit_results.back(),opt->GetOutdir()+"pdf_data_9D_covariance.dat");
		//pdf_data_9D->ExportPDFValues(opt->GetOutdir()+"pdf_data_9D.dat",true);

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

		if(i > 1) {
			unsigned int to_modify = (i-2)/2;

			pdf_data_9D->GetVariable(parameters_to_vary[to_modify].first)->setVal(parameters_to_vary[to_modify].second.first);

			std::cout << "INFO: Reset param[" << to_modify << "] to " << parameters_to_vary[to_modify].second.first << "\n";
		}
	}
	// end of fits

	PrintFitStartBlock("Checking deviations");
	std::map<std::string, double> delta_map;
	std::map<std::string, double> delta_map_relative;
	for(auto p : physics_param_results[0]) {
		std::cout << "INFO: Checking deviation for physics parameter " << p.first << " with nominal value " << p.second << "\n";
		double delta = 0;
		for(std::size_t i = 1; i < physics_param_results.size(); ++i) {
			double new_delta = std::fabs(physics_param_results[i][p.first] - p.second);
			if(new_delta > delta) {
				delta = new_delta;
				std::cout << "Larger deviation found for index " << i << "(" << varied_parameter[i] << ") with value " << physics_param_results[i][p.first] << std::endl;
			}
		}
		delta_map[p.first] = delta;
		delta_map_relative[p.first] = delta/p.second;
	}

	std::cout << "INFO: Delta of physics parameters due to pdf model errors (absolute values)\n";
	std::cout << mp::map_to_string(delta_map) << std::endl;
	std::cout << "INFO: Delta of physics parameters due to pdf model errors (relative values)\n";
	std::cout << mp::map_to_string(delta_map_relative) << std::endl;

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
	delete result_mass_kpi;
}

void FitSystematics_SCF(shared_ptr<TChain> chain_scf, shared_ptr<FFOpt> opt) {
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
	RooRealVar mbc("mbc","m_{bc}",5.26,5.29,"GeV");
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
	// event weight
	RooRealVar weight("weight","weight",-999999,999999);
	// create data set (constructor only overload for max. 9 arguments, so we add some afterwards)
	RooArgSet varset(mbc,deltae,chil0_ma,mass_kpi,hel_phi,hel_th1,hel_th2,nb_prime);
	varset.add(channel);
	varset.add(datatype);
	varset.add(flavor);
//	varset.add(mcinfo);
//	varset.add(mctype);
//	varset.add(chil0_mo);
//	varset.add(chil0_mc);
//	varset.add(runno);
	varset.add(weight);
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
	variables_4D.push_back(&mbc);
	variables_4D.push_back(&deltae);
	variables_4D.push_back(&chil0_ma);
	variables_4D.push_back(&nb_prime);
	RooArgSet varset_4D(mbc,deltae,chil0_ma,nb_prime);

	std::vector<RooRealVar*> variables_4D_angles;
	variables_4D_angles.push_back(&mass_kpi);
	variables_4D_angles.push_back(&hel_phi);
	variables_4D_angles.push_back(&hel_th1);
	variables_4D_angles.push_back(&hel_th2);
	RooArgSet varset_5D_angles(mass_kpi,hel_phi,hel_th1,hel_th2,flavor);

	TCut cut_default_CP_and_CA = "(mbc < 5.289 && hel_th1 < 0.75 && (datatype==datatype::CP || datatype==datatype::CA))";
	RooDataSet data_scf("data_scf","data_scf",varset,Import(*chain_scf.get()),Cut(cut_default_CP_and_CA));
	data_scf.Print();

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
			hel_th2, nb_prime, flavor, category_name_flags_channel, false, (RooRealVar*)NULL);
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
	shared_ptr<FF_PDF> pdf_signal_4D = 	make_shared<FF_PDF_signal_4D_mbc_deltae_chil0_ma_nb_prime>(mbc,
			deltae, chil0_ma, nb_prime, category_name_flags_channel,false,pdf_bkg_continuum_9D->GetVariable("chil0_ma_resolution"));
	pdf_signal_4D->ImportPDFValues(opt->GetOutdir()+"pdf_signal_4D.dat",true);
	pdf_signal_4D->SetVariablesConst();
	//pdf_signal_4D->GetVariable("C_deltae_signal_width_core")->setConstant(false);
	shared_ptr<FF_PDF> pdf_signal_5D_angles = make_shared<FF_PDF_signal_5D_mass_helicity_angles_CPV>(mass_kpi,
			hel_th1, hel_th2, hel_phi, flavor, PRODUCT_4D,
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
			20000.0, category_name_flags_channel);

	pdf_data_9D->GetVariable("signal_nevents")->setVal(1100); // 1000 for C
	pdf_data_9D->GetVariable("C_bkg_continuum_nevents")->setVal(7350*2+350*2);
	pdf_data_9D->GetVariable("C_bkg_bbar_wrongphif0980_nevents")->setVal(120.0);

	std::map<std::string, double> truth_map;
	truth_map["signal_nevents"] = 1100; // 1000 for C
	truth_map["C_bkg_continuum_nevents"] = 7350*2+350*2;
	truth_map["C_bkg_bbar_wrongphif0980_nevents"] = 120.0;
	truth_map["mbc_bkg_continuum_c"] = pdf_bkg_continuum_9D->GetVariable("mbc_bkg_continuum_c")->getVal();
	truth_map["amp_00"] = pt.get<double>("amp_00");
	truth_map["amp_10"] = pt.get<double>("amp_10");
	truth_map["amp_1para"] = pt.get<double>("amp_1para");
	truth_map["amp_1perp"] = pt.get<double>("amp_1perp");
	truth_map["amp_20"] = pt.get<double>("amp_20");
	truth_map["amp_2para"] = pt.get<double>("amp_2para");
	truth_map["amp_2perp"] = pt.get<double>("amp_2perp");
	truth_map["phase_00"] = 0.0;
	truth_map["phase_10"] = pt.get<double>("phase_10");
	truth_map["phase_1para"] = pt.get<double>("phase_1para");
	truth_map["phase_1perp"] = pt.get<double>("phase_1perp");
	truth_map["phase_20"] = pt.get<double>("phase_20");
	truth_map["phase_2para"] = pt.get<double>("phase_2para");
	truth_map["phase_2perp"] = pt.get<double>("phase_2perp");
	truth_map["amp_00_delta"] = pt.get<double>("amp_00_delta");
	truth_map["amp_10_delta"] = pt.get<double>("amp_10_delta");
	truth_map["amp_1para_delta"] = pt.get<double>("amp_1para_delta");
	truth_map["amp_1perp_delta"] = pt.get<double>("amp_1perp_delta");
	truth_map["amp_20_delta"] = pt.get<double>("amp_20_delta");
	truth_map["amp_2para_delta"] = pt.get<double>("amp_2para_delta");
	truth_map["amp_2perp_delta"] = pt.get<double>("amp_2perp_delta");
	truth_map["phase_00_delta"] = 0.0;
	truth_map["phase_10_delta"] = pt.get<double>("phase_10_delta");
	truth_map["phase_1para_delta"] = pt.get<double>("phase_1para_delta");
	truth_map["phase_1perp_delta"] = pt.get<double>("phase_1perp_delta");
	truth_map["phase_20_delta"] = pt.get<double>("phase_20_delta");
	truth_map["phase_2para_delta"] = pt.get<double>("phase_2para_delta");
	truth_map["phase_2perp_delta"] = pt.get<double>("phase_2perp_delta");

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

	RooDataSet* data_set_gen = NULL;
	std::vector<std::map<std::string, double> > physics_param_results;
	std::map<std::string,double> physics_parameters_expectation;
	std::pair<int, int> event_from_to = std::make_pair(0,0);
	PrintFitStartBlock("TOY-STUDY: 9D Fit");
	if(opt->GetNToys() > 0) {
		RooMCStudy* mcstudy = new RooMCStudy(pdf_data_9D->GetSimPDF(),varset_9D);
		std::vector<RooFitResult*> fit_results;
		std::vector<RooDataSet*> data_sets;
		std::string outdir_files = opt->GetOutdir()+"/toystudy_"+mp::to_string(opt->GetNToys())+(opt->GetMinos() ? "_minos" : "")+"/";
		mp::execute_command("mkdir -p "+outdir_files);
		for(int i = 1; i <= opt->GetNToys(); ++i) {
			if(opt->GetSingleToy()) {
				if(i != (int)opt->GetSingleToy()) { // continue until we have the single toy we want
					continue;
				} else { // initialize random seed for this single toy
					RooRandom::randomGenerator()->SetSeed(opt->GetRandomSeed()+opt->GetSingleToy());
				}
			}
			std::stringstream ss;
			ss << "sample_" << i;
			std::cout << "INFO: Toy Sample: " << i << std::endl;
			// resetting continuum shape and yields to default values
			pdf_bkg_continuum_9D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_continuum_8D.dat",false);
			pdf_bkg_continuum_9D->SetVariablesConst();
			pdf_bkg_continuum_9D->GetVariable("mbc_bkg_continuum_c")->setConstant(false);
			//pdf_signal_9D->ImportPDFValues(opt->GetOutdir()+"pdf_signal_4D.dat",false);
			pdf_data_9D->GetVariable("signal_nevents")->setVal(truth_map["signal_nevents"]); // 1000 for C
			pdf_data_9D->GetVariable("C_bkg_continuum_nevents")->setVal(truth_map["C_bkg_continuum_nevents"]);
			pdf_data_9D->GetVariable("C_bkg_bbar_wrongphif0980_nevents")->setVal(truth_map["C_bkg_bbar_wrongphif0980_nevents"]);

			// reset to generation values
			pdf_data_9D->GetVariable("amp_00")->setVal(truth_map["amp_00"]);
			pdf_data_9D->GetVariable("amp_10")->setVal(truth_map["amp_10"]);
			pdf_data_9D->GetVariable("amp_1para")->setVal(truth_map["amp_1para"]);
			pdf_data_9D->GetVariable("amp_1perp")->setVal(truth_map["amp_1perp"]);
			pdf_data_9D->GetVariable("amp_20")->setVal(truth_map["amp_20"]);
			pdf_data_9D->GetVariable("amp_2para")->setVal(truth_map["amp_2para"]);
			pdf_data_9D->GetVariable("amp_2perp")->setVal(truth_map["amp_2perp"]);

			pdf_data_9D->GetVariable("phase_10")->setVal(truth_map["phase_10"]);
			pdf_data_9D->GetVariable("phase_1para")->setVal(truth_map["phase_1para"]);
			pdf_data_9D->GetVariable("phase_1perp")->setVal(truth_map["phase_1perp"]);
			pdf_data_9D->GetVariable("phase_20")->setVal(truth_map["phase_20"]);
			pdf_data_9D->GetVariable("phase_2para")->setVal(truth_map["phase_2para"]);
			pdf_data_9D->GetVariable("phase_2perp")->setVal(truth_map["phase_2perp"]);

			pdf_data_9D->GetVariable("amp_00_delta")->setVal(truth_map["amp_00_delta"]);
			pdf_data_9D->GetVariable("amp_10_delta")->setVal(truth_map["amp_10_delta"]);
			pdf_data_9D->GetVariable("amp_1para_delta")->setVal(truth_map["amp_1para_delta"]);
			pdf_data_9D->GetVariable("amp_1perp_delta")->setVal(truth_map["amp_1perp_delta"]);
			pdf_data_9D->GetVariable("amp_20_delta")->setVal(truth_map["amp_20_delta"]);
			pdf_data_9D->GetVariable("amp_2para_delta")->setVal(truth_map["amp_2para_delta"]);
			pdf_data_9D->GetVariable("amp_2perp_delta")->setVal(truth_map["amp_2perp_delta"]);

			pdf_data_9D->GetVariable("phase_10_delta")->setVal(truth_map["phase_10_delta"]);
			pdf_data_9D->GetVariable("phase_1para_delta")->setVal(truth_map["phase_1para_delta"]);
			pdf_data_9D->GetVariable("phase_1perp_delta")->setVal(truth_map["phase_1perp_delta"]);
			pdf_data_9D->GetVariable("phase_20_delta")->setVal(truth_map["phase_20_delta"]);
			pdf_data_9D->GetVariable("phase_2para_delta")->setVal(truth_map["phase_2para_delta"]);
			pdf_data_9D->GetVariable("phase_2perp_delta")->setVal(truth_map["phase_2perp_delta"]);

			std::cout << "INFO: Expected physics parameters for truth values given" << std::endl;
			physics_parameters_expectation = ComputePhysicsParameters(pdf_data_9D, pdf_signal_9D, varset_9D);
			std::cout << mp::map_to_string(physics_parameters_expectation) << std::endl;

			std::cout << "INFO: Expected events for C: " << pdf_data_9D->GetPDF("C").expectedEvents(varset_9D) << std::endl;
			if(i == 1) {
				data_set_gen = pdf_data_9D->GetPDF("C").generate(varset_9D,RooRandom::randomGenerator()->Poisson(pdf_data_9D->GetPDF("C").expectedEvents(varset_9D)));
				event_from_to.second = (int)(RooRandom::randomGenerator()->Poisson(50)+0.5);
			}
			data_sets.push_back(new RooDataSet(("data"+ss.str()).c_str(),ss.str().c_str(),varset_9D,Index(channel),
					Import("C",*data_set_gen)));
			data_sets.back()->Print();

			// add some additional SCF events to toy data set
			if(i > 1) {
				RooDataSet* data_tmp = dynamic_cast<RooDataSet*>(data_scf.reduce(EventRange(event_from_to.first,event_from_to.second)));
				std::cout << "DEBUG: SCF data set" << std::endl;
				data_tmp->Print();
				std::cout << "DEBUG: Before import SCF data set" << std::endl;
				data_sets.back()->Print();
				data_sets.back()->append(*data_tmp);
				std::cout << "DEBUG: After import SCF data set" << std::endl;
				data_sets.back()->Print();
				delete data_tmp;
				// set next subsample
				int event_shift = (int)(RooRandom::randomGenerator()->Poisson(50)+0.5);
				event_from_to.first = event_from_to.second+1;
				event_from_to.second = event_from_to.first + event_shift;
				// if file is
				if(event_from_to.second > data_scf.sumEntries()) {
					std::cout << "ERROR: More toys than events in SCF file requested...stopping after end of this loop" << std::endl;
					i = opt->GetNToys();
				}
			}

			fit_results.push_back(pdf_data_9D->GetSimPDF().fitTo(*data_sets.back(), SumW2Error(false), NumCPU(opt->GetNumcpu()),
					Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()), Extended(true)));
			fit_results.back()->Print();
			PrintFitResultLatex(pdf_data_9D,truth_map);
			//PrintPhysicsParametersLatex(PropagateErrors(pdf_data_9D,pdf_signal_9D,fit_results.back(),opt,varset_9D,false));
			auto physics_results = ComputePhysicsParameters(pdf_data_9D,pdf_signal_9D,varset_9D,false);
			std::cout << mp::map_to_string(physics_results) << std::endl;
			physics_param_results.push_back(physics_results);
			mcstudy->addFitResult(*fit_results.back());

			std::ofstream fout((outdir_files+"TOY_9D_fit_result_"+ss.str().c_str()+".dat").c_str());
			if(fout.is_open()) {
				for(const std::string& p : parameters) {
					fout << p << ";";
					fout << truth_map[p] << ";";
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

		for(RooDataSet* s : data_sets) {
			delete s;
		}
		for(RooFitResult* r : fit_results) {
			delete r;
		}
		delete mcstudy;
	}

	if(data_set_gen != NULL)
		delete data_set_gen;

	PrintFitStartBlock("Checking deviations");
	std::cout.precision(7);
	std::map<std::string, double> delta_map;
	std::map<std::string, double> delta_map_relative;
	std::map<std::string, std::pair<double, double> > delta_mean_map;
	std::map<std::string, double > delta_width_map;
	for(auto p : physics_param_results[0]) {
		std::cout << "INFO: Checking deviation for physics parameter " << p.first << " with nominal value " << p.second << "\n";
		double delta = 0;
		std::vector<double> delta_mean;
		for(std::size_t i = 1; i < physics_param_results.size(); ++i) {
			double new_delta = std::fabs(physics_param_results[i][p.first] - p.second);
			if(new_delta > delta) {
				delta = new_delta;
				std::cout << "Larger deviation found for index " << i << " with value " << physics_param_results[i][p.first] << std::endl;
			}
			delta_mean.push_back(physics_param_results[i][p.first] - p.second);
		}
		delta_map[p.first] = delta;
		delta_map_relative[p.first] = delta/p.second;
		double mean = mp::calc_mean(delta_mean);
		double width = mp::calc_sigma(delta_mean);
		std::cout << "Mean dev.: " << mean << " +- " << width/delta_mean.size() << "    Width: " << width << "\n";
		delta_mean_map[p.first] = std::make_pair(mean, width/delta_mean.size());
		delta_width_map[p.first] = width;
	}

	std::cout << "INFO: Delta max of physics parameters due to SCF (absolute values)\n";
	std::cout << mp::map_to_string(delta_map) << std::endl;
	std::cout << "INFO: Delta max of physics parameters due to SCF (relative values)\n";
	std::cout << mp::map_to_string(delta_map_relative) << std::endl;
	std::cout << "INFO: Residual mean of physics parameters due to SCF (absolute values)\n";
	for(auto &p : delta_mean_map) {
		std::cout << "Parameter " << p.first << " = " << p.second.first << " +- " << p.second.second << std::endl;
	}
	std::cout << "INFO: Residual width of physics parameters due to SCF (absolute values)\n";
	std::cout << mp::map_to_string(delta_width_map) << std::endl;

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
}

void FitSystematics_FixedCPV(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt) {
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
	// some extra range for integrals
	mbc.setRange("full_LASS_range",5.26,5.29);
	deltae.setRange("full_LASS_range",-0.15,0.15);
	chil0_ma.setRange("full_LASS_range",0.985,1.05);
	nb_prime.setRange("full_LASS_range",-10.0,10.0);
	mass_kpi.setRange("full_LASS_range",m_K+m_Pi+0.001,1.6);
	hel_phi.setRange("full_LASS_range",-TMath::Pi(),TMath::Pi());
	hel_th1.setRange("full_LASS_range",-1.0,1.0);
	hel_th2.setRange("full_LASS_range",-1.0,1.0);

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
			hel_th2, nb_prime, flavor, category_name_flags_channel,false,(RooRealVar*)NULL);
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
			deltae, chil0_ma, nb_prime, category_name_flags_channel,false,pdf_bkg_continuum_9D->GetVariable("chil0_ma_resolution"));
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

	std::vector<std::pair<std::string, std::pair<double,double> > > parameters_to_vary;
	parameters_to_vary.push_back(std::make_pair("phase_00_delta",std::make_pair(0.0,TMath::Pi()/4.0)));

	std::cout << "INFO: Parameters to vary:\n";
	for(auto p : parameters_to_vary) {
		std::cout << p.first << "  " << mp::pair_to_string(p.second) << "\n";
	}

	PrintFitStartBlock("REPEATING FIT WITH STARTING VALUES");
	std::vector<std::map<std::string,double>> physics_param_results;
	std::vector<std::string> varied_parameter;
	std::vector<RooFitResult*> fit_results;
	std::string outdir_files = opt->GetOutdir()+"/data_fit_"+(opt->GetMinos() ? "_minos" : "")+"/";
	mp::execute_command("mkdir -p "+outdir_files);
	// start loop of fits
	for(int i = 1; i <= 1+(int)parameters_to_vary.size()*2; ++i) {
		std::stringstream ss;
		ss << "sample_" << i;
		PrintFitStartBlock("NOW FITTING SAMPLE "+ss.str());
		std::cout << "INFO: Data fit: " << i << std::endl;

		if(i == 1) {
			varied_parameter.push_back("none");
		}
		if(i > 1) {
			unsigned int to_modify = (i-2)/2;
			bool negative = (i-2)%2;

			std::cout << "INFO: Modify param[" << to_modify << "]=" << parameters_to_vary[to_modify].first << " by " << (negative ? "-" : "+") << parameters_to_vary[to_modify].second.second << "\n";
			if(negative)
				pdf_data_9D->GetVariable(parameters_to_vary[to_modify].first)->setVal(parameters_to_vary[to_modify].second.first - parameters_to_vary[to_modify].second.second);
			else
				pdf_data_9D->GetVariable(parameters_to_vary[to_modify].first)->setVal(parameters_to_vary[to_modify].second.first + parameters_to_vary[to_modify].second.second);
			std::cout << "INFO: Value of " << parameters_to_vary[to_modify].first << " is " << pdf_data_9D->GetVariable(parameters_to_vary[to_modify].first)->getVal() << "\n";
			varied_parameter.push_back(parameters_to_vary[to_modify].first + (negative ? "-" : "+"));
		}

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
			fit_results.push_back(pdf_data_9D->GetSimPDF().fitTo(data, SumW2Error(false), NumCPU(opt->GetNumcpu()),
					Timer(true), Save(true), InitialHesse(false), Minos(minos_pars), Extended(true)));
		} else {
			fit_results.push_back(pdf_data_9D->GetSimPDF().fitTo(data, SumW2Error(false), NumCPU(opt->GetNumcpu()),
					Timer(true), Save(true), InitialHesse(false), Minos(false), Extended(true)));
		}
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
		//PrintPhysicsParametersLatex(PropagateErrors(pdf_data_9D,pdf_signal_9D,fit_results.back(),opt,varset_9D,false));
		physics_param_results.push_back(ComputePhysicsParameters(pdf_data_9D,pdf_signal_9D,varset_9D,true));
		std::cout << mp::map_to_string(physics_param_results.back()) << std::endl;
		//fit_results.back()->correlationMatrix().Print();
		//fit_results.back()->covarianceMatrix().Print();
		//WriteCorrelationMatrixFile(fit_results.back(),opt->GetOutdir()+"pdf_data_9D_correlation.dat");
		//WriteCovarianceMatrixFile(fit_results.back(),opt->GetOutdir()+"pdf_data_9D_covariance.dat");
		//pdf_data_9D->ExportPDFValues(opt->GetOutdir()+"pdf_data_9D.dat",true);

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

		if(i > 1) {
			unsigned int to_modify = (i-2)/2;

			pdf_data_9D->GetVariable(parameters_to_vary[to_modify].first)->setVal(parameters_to_vary[to_modify].second.first);

			std::cout << "INFO: Reset param[" << to_modify << "] to " << parameters_to_vary[to_modify].second.first << "\n";
		}
	}
	// end of fits

	PrintFitStartBlock("Checking deviations");
	std::map<std::string, double> delta_map;
	std::map<std::string, double> delta_map_relative;
	for(auto p : physics_param_results[0]) {
		std::cout << "INFO: Checking deviation for physics parameter " << p.first << " with nominal value " << p.second << "\n";
		double delta = 0;
		for(std::size_t i = 1; i < physics_param_results.size(); ++i) {
			double new_delta = std::fabs(physics_param_results[i][p.first] - p.second);
			if(new_delta > delta) {
				delta = new_delta;
				std::cout << "Larger deviation found for index " << i << "(" << varied_parameter[i] << ") with value " << physics_param_results[i][p.first] << std::endl;
			}
		}
		delta_map[p.first] = delta;
		delta_map_relative[p.first] = delta/p.second;
	}

	std::cout << "INFO: Delta of physics parameters due to fixend CPV parameter (absolute values)\n";
	std::cout << mp::map_to_string(delta_map) << std::endl;
	std::cout << "INFO: Delta of physics parameters due to fixend CPV parameter (relative values)\n";
	std::cout << mp::map_to_string(delta_map_relative) << std::endl;

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
	delete result_mass_kpi;
}

void FitSystematics_CoherentKK(shared_ptr<FFOpt> opt) {
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
	RooRealVar mbc("mbc","m_{bc}",5.26,5.29,"GeV");
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
	// event weight
	RooRealVar weight("weight","weight",-999999,999999);
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
	varset.add(weight);
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
	variables_4D.push_back(&mbc);
	variables_4D.push_back(&deltae);
	variables_4D.push_back(&chil0_ma);
	variables_4D.push_back(&nb_prime);
	RooArgSet varset_4D(mbc,deltae,chil0_ma,nb_prime);

	std::vector<RooRealVar*> variables_4D_angles;
	variables_4D_angles.push_back(&mass_kpi);
	variables_4D_angles.push_back(&hel_phi);
	variables_4D_angles.push_back(&hel_th1);
	variables_4D_angles.push_back(&hel_th2);
	RooArgSet varset_5D_angles(mass_kpi,hel_phi,hel_th1,hel_th2,flavor);

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

	PrintFitStartBlock("SETUP: 9D Fit");

	shared_ptr<FF_PDF> pdf_bkg_continuum_9D = make_shared<FF_PDF_bkg_continuum_9D_mbc_deltae_chil0_ma_mass_kpi_hel_phi_hel_th1_hel_th2_nb_prime_flavor>(mbc,
			deltae, chil0_ma, mass_kpi, hel_phi, hel_th1,
			hel_th2, nb_prime, flavor, category_name_flags_channel, false, (RooRealVar*)NULL);
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
	shared_ptr<FF_PDF> pdf_signal_4D = 	make_shared<FF_PDF_signal_4D_mbc_deltae_chil0_ma_nb_prime>(mbc,
			deltae, chil0_ma, nb_prime, category_name_flags_channel,false,pdf_bkg_continuum_9D->GetVariable("chil0_ma_resolution"));
	pdf_signal_4D->ImportPDFValues(opt->GetOutdir()+"pdf_signal_4D.dat",true);
	pdf_signal_4D->SetVariablesConst();
	//pdf_signal_4D->GetVariable("C_deltae_signal_width_core")->setConstant(false);
	shared_ptr<FF_PDF> pdf_signal_5D_angles = make_shared<FF_PDF_signal_5D_mass_helicity_angles_CPV>(mass_kpi,
			hel_th1, hel_th2, hel_phi, flavor, PRODUCT_4D,
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

	shared_ptr<FF_PDF> pdf_bkg_bbar_wrongphi_9D_coherent = make_shared<FF_PDF_bkg_bbar_wrongphif0980AndNonRes_9D>(mbc,
			deltae, chil0_ma, nb_prime, mass_kpi,
			hel_phi, hel_th1, hel_th2, flavor, category_name_flags_channel, true);
	pdf_bkg_bbar_wrongphi_9D_coherent->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongphif0980_8D.dat",true);
	pdf_bkg_bbar_wrongphi_9D_coherent->SetVariablesConst();
	pdf_bkg_bbar_wrongphi_9D_coherent->GetVariable("chil0_ma_bkg_bbar_wrongphinonres_amp")->setConstant(false);
	pdf_bkg_bbar_wrongphi_9D_coherent->GetVariable("chil0_ma_bkg_bbar_wrongphinonres_phase")->setConstant(false);

	shared_ptr<FF_PDF> pdf_data_9D = make_shared<FF_PDF_data_9D__CPV__KK_Flatte>(pdf_signal_9D,
			pdf_bkg_continuum_9D, pdf_bkg_bbar_wrongphi_9D,
			20000.0, category_name_flags_channel);

	pdf_data_9D->GetVariable("signal_nevents")->setVal(1100); // 1000 for C
	pdf_data_9D->GetVariable("C_bkg_continuum_nevents")->setVal(7350*2+350*2);
	pdf_data_9D->GetVariable("C_bkg_bbar_wrongphif0980_nevents")->setVal(120.0);

	shared_ptr<FF_PDF> pdf_data_9D_coherent = make_shared<FF_PDF_data_9D__CPV__KK_FlatteAndNonRes>(pdf_signal_9D,
			pdf_bkg_continuum_9D, pdf_bkg_bbar_wrongphi_9D_coherent,
			20000.0, category_name_flags_channel);

	pdf_data_9D_coherent->GetVariable("signal_nevents")->setVal(1100); // 1000 for C
	pdf_data_9D_coherent->GetVariable("C_bkg_continuum_nevents")->setVal(7350*2+350*2);
	pdf_data_9D_coherent->GetVariable("C_bkg_bbar_wrongphif0980_nevents")->setVal(120.0);
	pdf_data_9D_coherent->GetVariable("chil0_ma_bkg_bbar_wrongphinonres_amp")->setVal(6.6e-2);
	pdf_data_9D_coherent->GetVariable("chil0_ma_bkg_bbar_wrongphinonres_phase")->setVal(5.3);

	std::map<std::string, double> truth_map;
	truth_map["signal_nevents"] = 1100; // 1000 for C
	truth_map["C_bkg_continuum_nevents"] = 7350*2+350*2;
	truth_map["C_bkg_bbar_wrongphif0980_nevents"] = 120.0;
	truth_map["mbc_bkg_continuum_c"] = pdf_bkg_continuum_9D->GetVariable("mbc_bkg_continuum_c")->getVal();
	truth_map["amp_00"] = pt.get<double>("amp_00");
	truth_map["amp_10"] = pt.get<double>("amp_10");
	truth_map["amp_1para"] = pt.get<double>("amp_1para");
	truth_map["amp_1perp"] = pt.get<double>("amp_1perp");
	truth_map["amp_20"] = pt.get<double>("amp_20");
	truth_map["amp_2para"] = pt.get<double>("amp_2para");
	truth_map["amp_2perp"] = pt.get<double>("amp_2perp");
	truth_map["phase_00"] = 0.0;
	truth_map["phase_10"] = pt.get<double>("phase_10");
	truth_map["phase_1para"] = pt.get<double>("phase_1para");
	truth_map["phase_1perp"] = pt.get<double>("phase_1perp");
	truth_map["phase_20"] = pt.get<double>("phase_20");
	truth_map["phase_2para"] = pt.get<double>("phase_2para");
	truth_map["phase_2perp"] = pt.get<double>("phase_2perp");
	truth_map["amp_00_delta"] = pt.get<double>("amp_00_delta");
	truth_map["amp_10_delta"] = pt.get<double>("amp_10_delta");
	truth_map["amp_1para_delta"] = pt.get<double>("amp_1para_delta");
	truth_map["amp_1perp_delta"] = pt.get<double>("amp_1perp_delta");
	truth_map["amp_20_delta"] = pt.get<double>("amp_20_delta");
	truth_map["amp_2para_delta"] = pt.get<double>("amp_2para_delta");
	truth_map["amp_2perp_delta"] = pt.get<double>("amp_2perp_delta");
	truth_map["phase_00_delta"] = 0.0;
	truth_map["phase_10_delta"] = pt.get<double>("phase_10_delta");
	truth_map["phase_1para_delta"] = pt.get<double>("phase_1para_delta");
	truth_map["phase_1perp_delta"] = pt.get<double>("phase_1perp_delta");
	truth_map["phase_20_delta"] = pt.get<double>("phase_20_delta");
	truth_map["phase_2para_delta"] = pt.get<double>("phase_2para_delta");
	truth_map["phase_2perp_delta"] = pt.get<double>("phase_2perp_delta");

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

	RooDataSet* data_set_gen = NULL;
	std::vector<std::map<std::string, double> > physics_param_results;
	std::map<std::string,double> physics_parameters_expectation;

	PrintFitStartBlock("TOY-STUDY: 9D Fit");
	if(opt->GetNToys() > 0) {
		RooMCStudy* mcstudy = new RooMCStudy(pdf_data_9D->GetSimPDF(),varset_9D);
		std::vector<RooFitResult*> fit_results;
		std::vector<RooDataSet*> data_sets;
		std::string outdir_files = opt->GetOutdir()+"/toystudy_"+mp::to_string(opt->GetNToys())+(opt->GetMinos() ? "_minos" : "")+"/";
		mp::execute_command("mkdir -p "+outdir_files);
		for(int i = 1; i <= opt->GetNToys(); ++i) {
			if(opt->GetSingleToy()) {
				if(i != (int)opt->GetSingleToy()) { // continue until we have the single toy we want
					continue;
				} else { // initialize random seed for this single toy
					RooRandom::randomGenerator()->SetSeed(opt->GetRandomSeed()+opt->GetSingleToy());
				}
			}
			std::stringstream ss;
			ss << "sample_" << i;
			std::cout << "INFO: Toy Sample: " << i << std::endl;
			// resetting continuum shape and yields to default values
			pdf_bkg_continuum_9D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_continuum_8D.dat",false);
			pdf_bkg_continuum_9D->SetVariablesConst();
			pdf_bkg_continuum_9D->GetVariable("mbc_bkg_continuum_c")->setConstant(false);
			//pdf_signal_9D->ImportPDFValues(opt->GetOutdir()+"pdf_signal_4D.dat",false);
			pdf_data_9D->GetVariable("signal_nevents")->setVal(truth_map["signal_nevents"]); // 1000 for C
			pdf_data_9D->GetVariable("C_bkg_continuum_nevents")->setVal(truth_map["C_bkg_continuum_nevents"]);
			pdf_data_9D->GetVariable("C_bkg_bbar_wrongphif0980_nevents")->setVal(truth_map["C_bkg_bbar_wrongphif0980_nevents"]);

			pdf_data_9D_coherent->GetVariable("signal_nevents")->setVal(truth_map["signal_nevents"]); // 1000 for C
			pdf_data_9D_coherent->GetVariable("C_bkg_continuum_nevents")->setVal(truth_map["C_bkg_continuum_nevents"]);
			pdf_data_9D_coherent->GetVariable("C_bkg_bbar_wrongphif0980_nevents")->setVal(truth_map["C_bkg_bbar_wrongphif0980_nevents"]);

			// reset to generation values
			pdf_signal_9D->GetVariable("amp_00")->setVal(truth_map["amp_00"]);
			pdf_signal_9D->GetVariable("amp_10")->setVal(truth_map["amp_10"]);
			pdf_signal_9D->GetVariable("amp_1para")->setVal(truth_map["amp_1para"]);
			pdf_signal_9D->GetVariable("amp_1perp")->setVal(truth_map["amp_1perp"]);
			pdf_signal_9D->GetVariable("amp_20")->setVal(truth_map["amp_20"]);
			pdf_signal_9D->GetVariable("amp_2para")->setVal(truth_map["amp_2para"]);
			pdf_signal_9D->GetVariable("amp_2perp")->setVal(truth_map["amp_2perp"]);

			pdf_signal_9D->GetVariable("phase_10")->setVal(truth_map["phase_10"]);
			pdf_signal_9D->GetVariable("phase_1para")->setVal(truth_map["phase_1para"]);
			pdf_signal_9D->GetVariable("phase_1perp")->setVal(truth_map["phase_1perp"]);
			pdf_signal_9D->GetVariable("phase_20")->setVal(truth_map["phase_20"]);
			pdf_signal_9D->GetVariable("phase_2para")->setVal(truth_map["phase_2para"]);
			pdf_signal_9D->GetVariable("phase_2perp")->setVal(truth_map["phase_2perp"]);

			pdf_signal_9D->GetVariable("amp_00_delta")->setVal(truth_map["amp_00_delta"]);
			pdf_signal_9D->GetVariable("amp_10_delta")->setVal(truth_map["amp_10_delta"]);
			pdf_signal_9D->GetVariable("amp_1para_delta")->setVal(truth_map["amp_1para_delta"]);
			pdf_signal_9D->GetVariable("amp_1perp_delta")->setVal(truth_map["amp_1perp_delta"]);
			pdf_signal_9D->GetVariable("amp_20_delta")->setVal(truth_map["amp_20_delta"]);
			pdf_signal_9D->GetVariable("amp_2para_delta")->setVal(truth_map["amp_2para_delta"]);
			pdf_signal_9D->GetVariable("amp_2perp_delta")->setVal(truth_map["amp_2perp_delta"]);

			pdf_signal_9D->GetVariable("phase_10_delta")->setVal(truth_map["phase_10_delta"]);
			pdf_signal_9D->GetVariable("phase_1para_delta")->setVal(truth_map["phase_1para_delta"]);
			pdf_signal_9D->GetVariable("phase_1perp_delta")->setVal(truth_map["phase_1perp_delta"]);
			pdf_signal_9D->GetVariable("phase_20_delta")->setVal(truth_map["phase_20_delta"]);
			pdf_signal_9D->GetVariable("phase_2para_delta")->setVal(truth_map["phase_2para_delta"]);
			pdf_signal_9D->GetVariable("phase_2perp_delta")->setVal(truth_map["phase_2perp_delta"]);

			std::cout << "INFO: Expected physics parameters for truth values given" << std::endl;
			physics_parameters_expectation = ComputePhysicsParameters(pdf_data_9D, pdf_signal_9D, varset_9D);
			std::cout << mp::map_to_string(physics_parameters_expectation) << std::endl;

			std::cout << "INFO: Expected events for C: " << pdf_data_9D_coherent->GetPDF("C").expectedEvents(varset_9D) << std::endl;
			data_set_gen = pdf_data_9D_coherent->GetPDF("C").generate(varset_9D,RooRandom::randomGenerator()->Poisson(pdf_data_9D_coherent->GetPDF("C").expectedEvents(varset_9D)));

			data_sets.push_back(new RooDataSet(("data"+ss.str()).c_str(),ss.str().c_str(),varset_9D,Index(channel),
					Import("C",*data_set_gen)));
			data_sets.back()->Print();

			fit_results.push_back(pdf_data_9D_coherent->GetSimPDF().fitTo(*data_sets.back(), SumW2Error(false), NumCPU(opt->GetNumcpu()),
					Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()), Extended(true)));
			fit_results.back()->Print();
			PrintFitResultLatex(pdf_data_9D_coherent,truth_map);
			//PrintPhysicsParametersLatex(PropagateErrors(pdf_data_9D_coherent,pdf_signal_9D,fit_results.back(),opt,varset_9D,false));
			auto physics_results = ComputePhysicsParameters(pdf_data_9D_coherent,pdf_signal_9D,varset_9D,false);
			std::cout << mp::map_to_string(physics_results) << std::endl;
			physics_param_results.push_back(physics_results);
			mcstudy->addFitResult(*fit_results.back());

			PrintFitStartBlock("PAUSE BLOCK");

			fit_results.push_back(pdf_data_9D->GetSimPDF().fitTo(*data_sets.back(), SumW2Error(false), NumCPU(opt->GetNumcpu()),
					Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()), Extended(true)));
			fit_results.back()->Print();
			PrintFitResultLatex(pdf_data_9D,truth_map);
			//PrintPhysicsParametersLatex(PropagateErrors(pdf_data_9D,pdf_signal_9D,fit_results.back(),opt,varset_9D,false));
			physics_results = ComputePhysicsParameters(pdf_data_9D,pdf_signal_9D,varset_9D,false);
			std::cout << mp::map_to_string(physics_results) << std::endl;
			physics_param_results.push_back(physics_results);
			mcstudy->addFitResult(*fit_results.back());

			std::ofstream fout((outdir_files+"TOY_9D_fit_result_"+ss.str().c_str()+".dat").c_str());
			if(fout.is_open()) {
				for(const std::string& p : parameters) {
					fout << p << ";";
					fout << truth_map[p] << ";";
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

		for(RooDataSet* s : data_sets) {
			delete s;
		}
		for(RooFitResult* r : fit_results) {
			delete r;
		}
		delete mcstudy;
	}

	if(data_set_gen != NULL)
		delete data_set_gen;

	PrintFitStartBlock("Checking deviations");
	std::map<std::string, double> delta_map;
	std::map<std::string, double> delta_map_relative;
	for(auto p : physics_param_results[0]) {
		std::cout << "INFO: Checking deviation for physics parameter " << p.first << " with nominal value " << p.second << "\n";
		double delta = 0;
		for(std::size_t i = 1; i < physics_param_results.size(); ++i) {
			double new_delta = std::fabs(physics_param_results[i][p.first] - p.second);
			if(new_delta > delta) {
				delta = new_delta;
				std::cout << "Larger deviation found for index " << i << " with value " << physics_param_results[i][p.first] << std::endl;
			}
		}
		delta_map[p.first] = delta;
		delta_map_relative[p.first] = delta/p.second;
	}

	std::cout << "INFO: Delta of physics parameters due to coherent KK model (absolute values)\n";
	std::cout << mp::map_to_string(delta_map) << std::endl;
	std::cout << "INFO: Delta of physics parameters due to coherent KK model (relative values)\n";
	std::cout << mp::map_to_string(delta_map_relative) << std::endl;

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
}

}
