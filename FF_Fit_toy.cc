/**
 * @file	FF_Fit_toy.cc
 * @date	Feb 8, 2013
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

void FitToyMC_4D(shared_ptr<FFOpt> opt) {
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
	varset.add(mcinfo);
	varset.add(mctype);
	varset.add(chil0_mo);
	varset.add(chil0_mc);
	varset.add(runno);
	varset.add(weight);
	std::vector<std::string> flags_datatype;
	flags_datatype.push_back("CP");
	flags_datatype.push_back("CA");
	flags_datatype.push_back("N");
	flags_datatype.push_back("CS");
	flags_datatype.push_back("NS");
	flags_datatype.push_back("COFF");
	flags_datatype.push_back("NOFF");
	tuple_category_name_flags category_name_flags_datatype = std::make_tuple(&datatype,"datatype",flags_datatype);

	std::vector<RooRealVar*> variables_4D;
	variables_4D.push_back(&mbc);
	variables_4D.push_back(&deltae);
	variables_4D.push_back(&chil0_ma);
	variables_4D.push_back(&nb_prime);
	RooArgSet varset_4D(mbc,deltae,chil0_ma,nb_prime);

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

	PrintFitStartBlock("SETUP: 4D Fit");

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
			pdf_bkg_continuum_4D,
			pdf_bkg_bbar_wrongphikkbar_4D, 20000.0, category_name_flags_datatype);

	pdf_data_4D->GetVariable("signal_nevents")->setVal(1000); // 1000 for C
	pdf_data_4D->GetVariable("signal_nevents_asym")->setVal(0.0);
	pdf_data_4D->GetVariable("C_bkg_continuum_nevents")->setVal(7350*2+350*2);
	pdf_data_4D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents")->setVal(15.0);

	std::map<std::string, double> truth_map;
	truth_map["signal_nevents"] = 1000; // 1000 for C
	truth_map["signal_nevents_asym"] = 0.0;
	truth_map["C_bkg_continuum_nevents"] = 7350*2+350*2;
	truth_map["C_bkg_bbar_wrongphikkbar_nevents"] = 15.0;
	truth_map["mbc_bkg_continuum_c"] = pdf_bkg_continuum_4D->GetVariable("mbc_bkg_continuum_c")->getVal();

	PrintFitStartBlock("TOY-STUDY: 4D Fit");
	if(opt->GetNToys() > 0) {
		RooMCStudy* mcstudy = new RooMCStudy(pdf_data_4D->GetSimPDF(),varset_4D);
		std::vector<RooFitResult*> fit_results;
		std::vector<RooDataSet*> data_sets;
		std::vector<std::string> parameters;
		parameters.push_back("signal_nevents");
		parameters.push_back("signal_nevents_asym");
		parameters.push_back("C_bkg_continuum_nevents");
		parameters.push_back("C_bkg_bbar_wrongphikkbar_nevents");
		parameters.push_back("mbc_bkg_continuum_c");
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
			pdf_bkg_continuum_4D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_continuum_4D.dat",false);
			pdf_bkg_continuum_4D->SetVariablesConst();
			pdf_bkg_continuum_4D->GetVariable("mbc_bkg_continuum_c")->setConstant(false);
			pdf_data_4D->GetVariable("signal_nevents")->setVal(truth_map["signal_nevents"]); // 1000 for C
			pdf_data_4D->GetVariable("signal_nevents_asym")->setVal(truth_map["signal_nevents_asym"]);
			pdf_data_4D->GetVariable("C_bkg_continuum_nevents")->setVal(truth_map["C_bkg_continuum_nevents"]);
			pdf_data_4D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents")->setVal(truth_map["C_bkg_bbar_wrongphikkbar_nevents"]);

			std::cout << "INFO: Expected events for CP: " << pdf_data_4D->GetPDF("CP").expectedEvents(varset_4D) << " and CA: " << pdf_data_4D->GetPDF("CA").expectedEvents(varset_4D) << std::endl;
			RooDataSet* data_sets_CP = pdf_data_4D->GetPDF("CP").generate(varset_4D,RooRandom::randomGenerator()->Poisson(pdf_data_4D->GetPDF("CP").expectedEvents(varset_4D)));
			RooDataSet* data_sets_CA = pdf_data_4D->GetPDF("CA").generate(varset_4D,RooRandom::randomGenerator()->Poisson(pdf_data_4D->GetPDF("CA").expectedEvents(varset_4D)));

			data_sets.push_back(new RooDataSet(("data"+ss.str()).c_str(),ss.str().c_str(),varset_4D,Index(datatype),
					Import("CP",*data_sets_CP),Import("CA",*data_sets_CA)));
			delete data_sets_CP;
			delete data_sets_CA;
			data_sets.back()->Print();

			fit_results.push_back(pdf_data_4D->GetSimPDF().fitTo(*data_sets.back(), SumW2Error(false), NumCPU(opt->GetNumcpu()),
					Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()), Extended(true),
					Constrain(RooArgSet(*pdf_data_4D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents")))));
			fit_results.back()->Print();
			mcstudy->addFitResult(*fit_results.back());

			std::ofstream fout((outdir_files+"TOY_4D_fit_result_"+ss.str().c_str()+".dat").c_str());
			if(fout.is_open()) {
				for(const std::string& p : parameters) {
					fout << p << ";";
					fout << truth_map[p] << ";";
					fout << pdf_data_4D->GetVariable(p)->getVal() << ";";
					fout << pdf_data_4D->GetVariable(p)->getError() << ";";
					fout << pdf_data_4D->GetVariable(p)->getErrorLo() << ";";
					fout << pdf_data_4D->GetVariable(p)->getErrorHi() << std::endl;
				}
				fout.close();
			} else {
				std::cerr << "ERROR: Could not write fit result to file" << std::endl;
			}
		}
		if(opt->GetCreatePlots()) {
			std::vector<RooPlot*> plots;
			TCanvas *canvas = new TCanvas("c_canvas","c_canvas",1280*parameters.size(),1024*3);
			canvas->Divide(parameters.size(),3);
			int c = 1;
			for(const std::string& p : parameters) {
				canvas->cd(c);
				plots.push_back(mcstudy->plotParam(*pdf_data_4D->GetVariable(p),Bins(50)));
				plots.back()->Draw("");
				canvas->cd(c+parameters.size());
				plots.push_back(mcstudy->plotError(*pdf_data_4D->GetVariable(p),Bins(50)));
				plots.back()->Draw("");
				canvas->cd(c+2*parameters.size());
				plots.push_back(mcstudy->plotPull(*pdf_data_4D->GetVariable(p),Bins(50),FitGauss(true)));
				plots.back()->Draw("");
				++c;
			}
			std::stringstream ss;
			ss << opt->GetOutdir() << "toystudy_" << opt->GetNToys() << (opt->GetMinos() ? "_minos" : "") << "." << opt->GetPlotFormat();
			canvas->SaveAs(ss.str().c_str());
			delete canvas;
			c = 0;
			for(const std::string& p : parameters) {
				canvas = new TCanvas("c_canvas","c_canvas",1280*3,1024);
				canvas->Divide(3,1);
				canvas->cd(1);
				plots[c]->Draw("");
				++c;
				canvas->cd(2);
				plots[c]->Draw("");
				++c;
				canvas->cd(3);
				plots[c]->Draw("");
				++c;
				ss.str("");
				ss << opt->GetOutdir() << "toystudy_" << opt->GetNToys() << (opt->GetMinos() ? "_minos" : "") << "_" << p << "." << opt->GetPlotFormat();
				canvas->SaveAs(ss.str().c_str());
				delete canvas;
			}
			for(RooPlot* p : plots) {
				delete p;
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

	if(opt->GetCreatePlots()) {
		PrintFitStartBlock("GENERATE: 4D Fit");
		// resetting continuum shape and yields to default values
		pdf_bkg_continuum_4D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_continuum_4D.dat",false);
		pdf_bkg_continuum_4D->SetVariablesConst();
		pdf_bkg_continuum_4D->GetVariable("mbc_bkg_continuum_c")->setConstant(false);
		pdf_data_4D->GetVariable("signal_nevents")->setVal(truth_map["signal_nevents"]); // 1000 for C
		pdf_data_4D->GetVariable("signal_nevents_asym")->setVal(truth_map["signal_nevents_asym"]);
		pdf_data_4D->GetVariable("C_bkg_continuum_nevents")->setVal(truth_map["C_bkg_continuum_nevents"]);
		pdf_data_4D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents")->setVal(truth_map["C_bkg_bbar_wrongphikkbar_nevents"]);

		std::cout << "INFO: Expected events for CP: " << pdf_data_4D->GetPDF("CP").expectedEvents(varset_4D) << " and CA: " << pdf_data_4D->GetPDF("CA").expectedEvents(varset_4D) << std::endl;
		RooDataSet* data_CP = pdf_data_4D->GetPDF("CP").generate(varset_4D,RooRandom::randomGenerator()->Poisson(pdf_data_4D->GetPDF("CP").expectedEvents(varset_4D)));
		RooDataSet* data_CA = pdf_data_4D->GetPDF("CA").generate(varset_4D,RooRandom::randomGenerator()->Poisson(pdf_data_4D->GetPDF("CA").expectedEvents(varset_4D)));

		RooDataSet* data = new RooDataSet("data","data",varset_4D,Index(datatype),
				Import("CP",*data_CP),Import("CA",*data_CA));

		PrintFitStartBlock("FITTING: 4D Fit");
		// although we have a weighted dataset, we set SumW2Error(false) so that we get errors that correspond to the
		// statistics we would have if we have 'sumw2' events, and not errors that correspond to the number of entries
		RooFitResult *result_toy = pdf_data_4D->GetSimPDF().fitTo(*data, SumW2Error(false), NumCPU(opt->GetNumcpu()),
				Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()), Extended(true),
				Constrain(RooArgSet(*pdf_data_4D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents"))));
		pdf_data_4D->GetSimPDF().getParameters(data)->Print("v");
		result_toy->Print();

		for(RooRealVar* v : variables_4D) {
			std::string varname = v->GetName();
			FFUtil::VisualizeFitResult(pdf_data_4D,*data,*v,category_name_flags_datatype,true,
					opt->GetBins(),"full_range",opt->GetOutdir()+"TOY_4D_"+varname+"_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
					opt->GetPull(),opt->GetLogy(),false,false,opt->GetPullFiterror(),opt->GetPdfLegend());
			FFUtil::VisualizeFitResult(pdf_data_4D,*data,*v,category_name_flags_datatype,true,
					opt->GetBins(),("signal_box_"+varname).c_str(),opt->GetOutdir()+"TOY_4D_"+varname+"_projection_signal_box."+opt->GetPlotFormat(),opt->GetPlotFormat(),
					opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
			if(varname != "mbc") {
				FFUtil::VisualizeFitResult(pdf_data_4D,*data,*v,category_name_flags_datatype,true,
						opt->GetBins(),"mbc_sideband", opt->GetOutdir()+"TOY_4D_"+varname+"_projection_mbc_sideband."+opt->GetPlotFormat(),opt->GetPlotFormat(),
						opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
			}
		}

		delete result_toy;
		delete data_CP;
		delete data_CA;
		delete data;
	}
}

void FitToyMC_5D_angles(shared_ptr<FFOpt> opt) {
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
	std::vector<std::string> flags_flavor;
	flags_flavor.push_back("CP");
	flags_flavor.push_back("CA");
	tuple_category_name_flags category_name_flags_flavor = std::make_tuple(&flavor,"flavor",flags_flavor);

	std::vector<RooRealVar*> variables_4D;
	variables_4D.push_back(&mass_kpi);
	variables_4D.push_back(&hel_phi);
	variables_4D.push_back(&hel_th1);
	variables_4D.push_back(&hel_th2);
	RooArgSet varset_5D(mass_kpi,hel_phi,hel_th1,hel_th2,flavor);

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

	PrintFitStartBlock("SETUP: 5D Fit");

	shared_ptr<FF_PDF> pdf_bkg_continuum_5D = make_shared<FF_PDF_bkg_continuum_5D_mass_kpi_hel_phi_hel_th1_hel_th2_flavor>(mass_kpi,
			hel_phi, hel_th1, hel_th2, flavor, category_name_flags_channel,false);
	pdf_bkg_continuum_5D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_continuum_8D.dat",true);
	//pdf_bkg_continuum_5D->SetVariablesConst();
	shared_ptr<FF_PDF> pdf_signal_5D = make_shared<FF_PDF_signal_5D_mass_helicity_angles_CPV>(mass_kpi,
				hel_th1, hel_th2, hel_phi, flavor, PRODUCT_4D,
				mhi_pdg_constants, mhi_4D,
				mhi_3D_phi_th1_th2, mhi_3D_m_th1_th2, mhi_3D_m_phi_th2, mhi_3D_m_phi_th1,
				category_name_flags_channel);
	//pdf_signal_5D->ImportPDFValues(opt->GetOutdir()+"pdf_signal_4D.dat",true);
	//pdf_signal_5D->SetVariablesConst();
	//pdf_signal_5D->GetVariable("C_deltae_signal_width_core")->setConstant(false);
	shared_ptr<FF_PDF> pdf_bkg_bbar_wrongphikkbar_5D = make_shared<FF_PDF_bkg_bbar_wrongphikkbar_5D_mass_kpi_angles_flavor>(mass_kpi,
			hel_phi, hel_th1, hel_th2, flavor, category_name_flags_channel, false);
	pdf_bkg_bbar_wrongphikkbar_5D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongphikkbar_8D.dat",true);
	pdf_bkg_bbar_wrongphikkbar_5D->SetVariablesConst();

	shared_ptr<FF_PDF> pdf_data_5D = make_shared<FF_PDF_data_5D_mass_kpi_angles__CPV>(pdf_signal_5D,
			pdf_bkg_continuum_5D,
			pdf_bkg_bbar_wrongphikkbar_5D, 30000.0, category_name_flags_channel);

	pdf_data_5D->GetVariable("signal_nevents")->setVal(1000); // 1000 for C
	pdf_data_5D->GetVariable("C_bkg_continuum_nevents")->setVal(7350*2+350*2);
	pdf_data_5D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents")->setVal(15.0);

	std::map<std::string, double> truth_map;
	truth_map["signal_nevents"] = 1000; // 1000 for C
	truth_map["C_bkg_continuum_nevents"] = 7350*2+350*2;
	truth_map["C_bkg_bbar_wrongphikkbar_nevents"] = 15.0;
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

	PrintFitStartBlock("TOY-STUDY: 5D Fit");
	if(opt->GetNToys() > 0) {
		RooMCStudy* mcstudy = new RooMCStudy(pdf_data_5D->GetSimPDF(),varset_5D);
		std::vector<RooFitResult*> fit_results;
		std::vector<RooDataSet*> data_sets;
		std::vector<std::string> parameters;
		parameters.push_back("signal_nevents");
		parameters.push_back("C_bkg_continuum_nevents");
		parameters.push_back("C_bkg_bbar_wrongphikkbar_nevents");
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
		parameters.push_back("amp_2para_delta");
		parameters.push_back("amp_2perp_delta");
		parameters.push_back("phase_10_delta");
		parameters.push_back("phase_1para_delta");
		parameters.push_back("phase_1perp_delta");
		parameters.push_back("phase_20_delta");
		parameters.push_back("phase_2para_delta");
		parameters.push_back("phase_2perp_delta");

		std::string outdir_files = opt->GetOutdir()+"/toystudy_MANG_"+mp::to_string(opt->GetNToys())+(opt->GetMinos() ? "_minos" : "")+"/";
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
			pdf_bkg_continuum_5D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_continuum_8D.dat",false);
			pdf_bkg_continuum_5D->SetVariablesConst();
			//pdf_signal_5D->ImportPDFValues(opt->GetOutdir()+"pdf_signal_4D.dat",false);
			pdf_data_5D->GetVariable("signal_nevents")->setVal(truth_map["signal_nevents"]); // 1000 for C
			pdf_data_5D->GetVariable("C_bkg_continuum_nevents")->setVal(truth_map["C_bkg_continuum_nevents"]);
			pdf_data_5D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents")->setVal(truth_map["C_bkg_bbar_wrongphikkbar_nevents"]);

			// reset to generation values
			pdf_data_5D->GetVariable("amp_00")->setVal(truth_map["amp_00"]);
			pdf_data_5D->GetVariable("amp_10")->setVal(truth_map["amp_10"]);
			pdf_data_5D->GetVariable("amp_1para")->setVal(truth_map["amp_1para"]);
			pdf_data_5D->GetVariable("amp_1perp")->setVal(truth_map["amp_1perp"]);
			pdf_data_5D->GetVariable("amp_20")->setVal(truth_map["amp_20"]);
			pdf_data_5D->GetVariable("amp_2para")->setVal(truth_map["amp_2para"]);
			pdf_data_5D->GetVariable("amp_2perp")->setVal(truth_map["amp_2perp"]);

			pdf_data_5D->GetVariable("phase_10")->setVal(truth_map["phase_10"]);
			pdf_data_5D->GetVariable("phase_1para")->setVal(truth_map["phase_1para"]);
			pdf_data_5D->GetVariable("phase_1perp")->setVal(truth_map["phase_1perp"]);
			pdf_data_5D->GetVariable("phase_20")->setVal(truth_map["phase_20"]);
			pdf_data_5D->GetVariable("phase_2para")->setVal(truth_map["phase_2para"]);
			pdf_data_5D->GetVariable("phase_2perp")->setVal(truth_map["phase_2perp"]);

			pdf_data_5D->GetVariable("amp_00_delta")->setVal(truth_map["amp_00_delta"]);
			pdf_data_5D->GetVariable("amp_10_delta")->setVal(truth_map["amp_10_delta"]);
			pdf_data_5D->GetVariable("amp_1para_delta")->setVal(truth_map["amp_1para_delta"]);
			pdf_data_5D->GetVariable("amp_1perp_delta")->setVal(truth_map["amp_1perp_delta"]);
			pdf_data_5D->GetVariable("amp_20_delta")->setVal(truth_map["amp_20_delta"]);
			pdf_data_5D->GetVariable("amp_2para_delta")->setVal(truth_map["amp_2para_delta"]);
			pdf_data_5D->GetVariable("amp_2perp_delta")->setVal(truth_map["amp_2perp_delta"]);

			pdf_data_5D->GetVariable("phase_10_delta")->setVal(truth_map["phase_10_delta"]);
			pdf_data_5D->GetVariable("phase_1para_delta")->setVal(truth_map["phase_1para_delta"]);
			pdf_data_5D->GetVariable("phase_1perp_delta")->setVal(truth_map["phase_1perp_delta"]);
			pdf_data_5D->GetVariable("phase_20_delta")->setVal(truth_map["phase_20_delta"]);
			pdf_data_5D->GetVariable("phase_2para_delta")->setVal(truth_map["phase_2para_delta"]);
			pdf_data_5D->GetVariable("phase_2perp_delta")->setVal(truth_map["phase_2perp_delta"]);

			std::cout << "INFO: Expected events for C: " << pdf_data_5D->GetPDF("C").expectedEvents(varset_5D) << std::endl;
			RooDataSet* data_set_gen = pdf_data_5D->GetPDF("C").generate(varset_5D,RooRandom::randomGenerator()->Poisson(pdf_data_5D->GetPDF("C").expectedEvents(varset_5D)));
			data_sets.push_back(new RooDataSet(("data"+ss.str()).c_str(),ss.str().c_str(),varset_5D,Index(channel),
					Import("C",*data_set_gen)));
			delete data_set_gen;
			data_sets.back()->Print();

			fit_results.push_back(pdf_data_5D->GetSimPDF().fitTo(*data_sets.back(), SumW2Error(false), NumCPU(opt->GetNumcpu()),
					Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()), Extended(true),
					Constrain(RooArgSet(*pdf_data_5D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents")))));
			fit_results.back()->Print();
			mcstudy->addFitResult(*fit_results.back());

			std::ofstream fout((outdir_files+"TOY_5D_MANG_fit_result_"+ss.str().c_str()+".dat").c_str());
			if(fout.is_open()) {
				for(const std::string& p : parameters) {
					fout << p << ";";
					fout << truth_map[p] << ";";
					fout << pdf_data_5D->GetVariable(p)->getVal() << ";";
					fout << pdf_data_5D->GetVariable(p)->getError() << ";";
					fout << pdf_data_5D->GetVariable(p)->getErrorLo() << ";";
					fout << pdf_data_5D->GetVariable(p)->getErrorHi() << std::endl;
				}
				fout.close();
			} else {
				std::cerr << "ERROR: Could not write fit result to file" << std::endl;
			}
		}
		if(opt->GetCreatePlots()) {
			std::vector<RooPlot*> plots;
			TCanvas *canvas = new TCanvas("c_canvas","c_canvas",1280*parameters.size(),1024*3);
			canvas->Divide(parameters.size(),3);
			int c = 1;
			for(const std::string& p : parameters) {
				canvas->cd(c);
				plots.push_back(mcstudy->plotParam(*pdf_data_5D->GetVariable(p),Bins(50)));
				plots.back()->Draw("");
				canvas->cd(c+parameters.size());
				plots.push_back(mcstudy->plotError(*pdf_data_5D->GetVariable(p),Bins(50)));
				plots.back()->Draw("");
				canvas->cd(c+2*parameters.size());
				plots.push_back(mcstudy->plotPull(*pdf_data_5D->GetVariable(p),Bins(50),FitGauss(true)));
				plots.back()->Draw("");
				++c;
			}
			std::stringstream ss;
			ss << opt->GetOutdir() << "toystudy_MANG_" << opt->GetNToys() << (opt->GetMinos() ? "_minos" : "") << "." << opt->GetPlotFormat();
			canvas->SaveAs(ss.str().c_str());
			delete canvas;
			c = 0;
			for(const std::string& p : parameters) {
				canvas = new TCanvas("c_canvas","c_canvas",1280*3,1024);
				canvas->Divide(3,1);
				canvas->cd(1);
				plots[c]->Draw("");
				++c;
				canvas->cd(2);
				plots[c]->Draw("");
				++c;
				canvas->cd(3);
				plots[c]->Draw("");
				++c;
				ss.str("");
				ss << opt->GetOutdir() << "toystudy_MANG_" << opt->GetNToys() << (opt->GetMinos() ? "_minos" : "") << "_" << p << "." << opt->GetPlotFormat();
				canvas->SaveAs(ss.str().c_str());
				delete canvas;
			}
			for(RooPlot* p : plots) {
				delete p;
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

	if(opt->GetCreatePlots()) {
		PrintFitStartBlock("GENERATE: 5D Fit");
		// resetting continuum shape and yields to default values
		pdf_bkg_continuum_5D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_continuum_8D.dat",false);
		pdf_bkg_continuum_5D->SetVariablesConst();
		//pdf_signal_5D->ImportPDFValues(opt->GetOutdir()+"pdf_signal_4D.dat",false);
		pdf_data_5D->GetVariable("signal_nevents")->setVal(truth_map["signal_nevents"]); // 1000 for C
		pdf_data_5D->GetVariable("C_bkg_continuum_nevents")->setVal(truth_map["C_bkg_continuum_nevents"]);
		pdf_data_5D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents")->setVal(truth_map["C_bkg_bbar_wrongphikkbar_nevents"]);

		// reset to generation values
		pdf_data_5D->GetVariable("amp_00")->setVal(truth_map["amp_00"]);
		pdf_data_5D->GetVariable("amp_10")->setVal(truth_map["amp_10"]);
		pdf_data_5D->GetVariable("amp_1para")->setVal(truth_map["amp_1para"]);
		pdf_data_5D->GetVariable("amp_1perp")->setVal(truth_map["amp_1perp"]);
		pdf_data_5D->GetVariable("amp_20")->setVal(truth_map["amp_20"]);
		pdf_data_5D->GetVariable("amp_2para")->setVal(truth_map["amp_2para"]);
		pdf_data_5D->GetVariable("amp_2perp")->setVal(truth_map["amp_2perp"]);

		pdf_data_5D->GetVariable("phase_10")->setVal(truth_map["phase_10"]);
		pdf_data_5D->GetVariable("phase_1para")->setVal(truth_map["phase_1para"]);
		pdf_data_5D->GetVariable("phase_1perp")->setVal(truth_map["phase_1perp"]);
		pdf_data_5D->GetVariable("phase_20")->setVal(truth_map["phase_20"]);
		pdf_data_5D->GetVariable("phase_2para")->setVal(truth_map["phase_2para"]);
		pdf_data_5D->GetVariable("phase_2perp")->setVal(truth_map["phase_2perp"]);

		pdf_data_5D->GetVariable("amp_00_delta")->setVal(truth_map["amp_00_delta"]);
		pdf_data_5D->GetVariable("amp_10_delta")->setVal(truth_map["amp_10_delta"]);
		pdf_data_5D->GetVariable("amp_1para_delta")->setVal(truth_map["amp_1para_delta"]);
		pdf_data_5D->GetVariable("amp_1perp_delta")->setVal(truth_map["amp_1perp_delta"]);
		pdf_data_5D->GetVariable("amp_20_delta")->setVal(truth_map["amp_20_delta"]);
		pdf_data_5D->GetVariable("amp_2para_delta")->setVal(truth_map["amp_2para_delta"]);
		pdf_data_5D->GetVariable("amp_2perp_delta")->setVal(truth_map["amp_2perp_delta"]);

		pdf_data_5D->GetVariable("phase_10_delta")->setVal(truth_map["phase_10_delta"]);
		pdf_data_5D->GetVariable("phase_1para_delta")->setVal(truth_map["phase_1para_delta"]);
		pdf_data_5D->GetVariable("phase_1perp_delta")->setVal(truth_map["phase_1perp_delta"]);
		pdf_data_5D->GetVariable("phase_20_delta")->setVal(truth_map["phase_20_delta"]);
		pdf_data_5D->GetVariable("phase_2para_delta")->setVal(truth_map["phase_2para_delta"]);
		pdf_data_5D->GetVariable("phase_2perp_delta")->setVal(truth_map["phase_2perp_delta"]);

		std::cout << "INFO: Expected events for C: " << pdf_data_5D->GetPDF("C").expectedEvents(varset_5D) << std::endl;
		RooDataSet* data_set_gen = pdf_data_5D->GetPDF("C").generate(varset_5D,RooRandom::randomGenerator()->Poisson(pdf_data_5D->GetPDF("C").expectedEvents(varset_5D)));
		RooDataSet* data = new RooDataSet("data","data",varset_5D,Index(channel),	Import("C",*data_set_gen));
		delete data_set_gen;
		data->Print();

		PrintFitStartBlock("FITTING: 5D Fit");
		// although we have a weighted dataset, we set SumW2Error(false) so that we get errors that correspond to the
		// statistics we would have if we have 'sumw2' events, and not errors that correspond to the number of entries
		RooFitResult *result_toy = pdf_data_5D->GetSimPDF().fitTo(*data, SumW2Error(false), NumCPU(opt->GetNumcpu()),
				Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()), Extended(true),
				Constrain(RooArgSet(*pdf_data_5D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents"))));
		pdf_data_5D->GetSimPDF().getParameters(data)->Print("v");
		result_toy->Print();
		WriteCorrelationMatrixFile(result_toy,opt->GetOutdir()+"TOY_5D_MANG_fit_parameter_correlation.dat");

		for(RooRealVar* v : variables_4D) {
			std::string varname = v->GetName();
			FFUtil::VisualizeFitResult(pdf_data_5D,*data,*v,category_name_flags_channel,true,
					opt->GetBins(),"full_range",opt->GetOutdir()+"TOY_5D_MANG_"+varname+"_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
					opt->GetPull(),opt->GetLogy(),false,false,opt->GetPullFiterror(),opt->GetPdfLegend());
			if(varname == "mass_kpi") {
				FFUtil::VisualizeFitResultProjWData(pdf_data_5D,*data,*v,category_name_flags_channel,true,
						opt->GetBins(),"full_range",opt->GetOutdir()+"TOY_5D_MANG_"+varname+"_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
						category_name_flags_flavor, false,
						opt->GetPull(),opt->GetLogy(),false,false,opt->GetPullFiterror(),opt->GetPdfLegend());
			}
		}

		delete result_toy;
		delete data;
	}

	if(!(opt->GetSingleToy())) {
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
	}

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

void FitToyMC_9D(shared_ptr<FFOpt> opt) {
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
	shared_ptr<FF_PDF> pdf_bkg_bbar_wrongphikkbar_9D = make_shared<FF_PDF_bkg_bbar_wrongphikkbar_9D>(mbc,
			deltae, chil0_ma, nb_prime, mass_kpi,
			hel_phi, hel_th1, hel_th2, flavor, category_name_flags_channel, false);
	pdf_bkg_bbar_wrongphikkbar_9D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongphikkbar_8D.dat",true);
	pdf_bkg_bbar_wrongphikkbar_9D->SetVariablesConst();

	shared_ptr<FF_PDF> pdf_data_9D = make_shared<FF_PDF_data_9D__CPV>(pdf_signal_9D,
			pdf_bkg_continuum_9D,
			pdf_bkg_bbar_wrongphikkbar_9D, 20000.0, category_name_flags_channel);

	pdf_data_9D->GetVariable("signal_nevents")->setVal(1000); // 1000 for C
	pdf_data_9D->GetVariable("C_bkg_continuum_nevents")->setVal(7350*2+350*2);
	pdf_data_9D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents")->setVal(15.0);

	std::map<std::string, double> truth_map;
	truth_map["signal_nevents"] = 1000; // 1000 for C
	truth_map["C_bkg_continuum_nevents"] = 7350*2+350*2;
	truth_map["C_bkg_bbar_wrongphikkbar_nevents"] = 15.0;
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
			pdf_data_9D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents")->setVal(truth_map["C_bkg_bbar_wrongphikkbar_nevents"]);

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
			std::map<std::string,double> physics_parameters_expectation = ComputePhysicsParameters(pdf_data_9D, pdf_signal_9D, varset);
			std::cout << mp::map_to_string(physics_parameters_expectation) << std::endl;

			std::cout << "INFO: Expected events for C: " << pdf_data_9D->GetPDF("C").expectedEvents(varset_9D) << std::endl;
			RooDataSet* data_set_gen = pdf_data_9D->GetPDF("C").generate(varset_9D,RooRandom::randomGenerator()->Poisson(pdf_data_9D->GetPDF("C").expectedEvents(varset_9D)));
			data_sets.push_back(new RooDataSet(("data"+ss.str()).c_str(),ss.str().c_str(),varset_9D,Index(channel),
					Import("C",*data_set_gen)));
			delete data_set_gen;
			data_sets.back()->Print();

			// reset to fit start values
			pdf_data_9D->GetVariable("amp_00")->setVal(0.75);
			pdf_data_9D->GetVariable("amp_10")->setVal(0.707);
			pdf_data_9D->GetVariable("amp_1para")->setVal(0.5);
			pdf_data_9D->GetVariable("amp_1perp")->setVal(0.5);
			pdf_data_9D->GetVariable("amp_20")->setVal(0.707*0.8);
			pdf_data_9D->GetVariable("amp_2para")->setVal(0.5*0.8);
			pdf_data_9D->GetVariable("amp_2perp")->setVal(0.5*0.8);

			pdf_data_9D->GetVariable("phase_10")->setVal(0.0);
			pdf_data_9D->GetVariable("phase_1para")->setVal(0.0);
			pdf_data_9D->GetVariable("phase_1perp")->setVal(0.0);
			pdf_data_9D->GetVariable("phase_20")->setVal(0.0);
			pdf_data_9D->GetVariable("phase_2para")->setVal(0.0);
			pdf_data_9D->GetVariable("phase_2perp")->setVal(0.0);

			pdf_data_9D->GetVariable("amp_00_delta")->setVal(0.0);
			pdf_data_9D->GetVariable("amp_10_delta")->setVal(0.0);
			pdf_data_9D->GetVariable("amp_1para_delta")->setVal(0.0);
			pdf_data_9D->GetVariable("amp_1perp_delta")->setVal(0.0);
			pdf_data_9D->GetVariable("amp_20_delta")->setVal(0.0);
			pdf_data_9D->GetVariable("amp_2para_delta")->setVal(0.0);
			pdf_data_9D->GetVariable("amp_2perp_delta")->setVal(0.0);

			pdf_data_9D->GetVariable("phase_10_delta")->setVal(0.0);
			pdf_data_9D->GetVariable("phase_1para_delta")->setVal(0.0);
			pdf_data_9D->GetVariable("phase_1perp_delta")->setVal(0.0);
			pdf_data_9D->GetVariable("phase_20_delta")->setVal(0.0);
			pdf_data_9D->GetVariable("phase_2para_delta")->setVal(0.0);
			pdf_data_9D->GetVariable("phase_2perp_delta")->setVal(0.0);

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

			RooFitResult* result_noCPV = pdf_data_9D->GetSimPDF().fitTo(*data_sets.back(), SumW2Error(false), NumCPU(opt->GetNumcpu()),
								Timer(true), Save(true), InitialHesse(false), Minos(false), Extended(true),
								Constrain(RooArgSet(*pdf_data_9D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents"))));
			result_noCPV->Print();
			delete result_noCPV;

			// check if any phase is outside the [-pi,pi] region and push it back towards truth value
			// this is only to get good pulls, the other solution is not wrong
			ShiftPhaseToInterval(pdf_data_9D, "phase_10", truth_map["phase_10"]);
			ShiftPhaseToInterval(pdf_data_9D, "phase_1para", truth_map["phase_1para"]);
			ShiftPhaseToInterval(pdf_data_9D, "phase_1perp", truth_map["phase_1perp"]);
			ShiftPhaseToInterval(pdf_data_9D, "phase_20", truth_map["phase_20"]);
			ShiftPhaseToInterval(pdf_data_9D, "phase_2para", truth_map["phase_2para"]);
			ShiftPhaseToInterval(pdf_data_9D, "phase_2perp", truth_map["phase_2perp"]);

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

			fit_results.push_back(pdf_data_9D->GetSimPDF().fitTo(*data_sets.back(), SumW2Error(false), NumCPU(opt->GetNumcpu()),
					Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()), Extended(true),
					Constrain(RooArgSet(*pdf_data_9D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents")))));
			fit_results.back()->Print();
			PrintFitResultLatex(pdf_data_9D,truth_map);
			PrintPhysicsParametersLatex(PropagateErrors(pdf_data_9D,pdf_signal_9D,fit_results.back(),opt,varset_9D,false));
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
		if(opt->GetCreatePlots() && opt->GetSingleToy() == 0) {
			std::vector<RooPlot*> plots;
			TCanvas *canvas = new TCanvas("c_canvas","c_canvas",1280*parameters.size(),1024*3);
			canvas->Divide(parameters.size(),3);
			int c = 1;
			for(const std::string& p : parameters) {
				canvas->cd(c);
				plots.push_back(mcstudy->plotParam(*pdf_data_9D->GetVariable(p),Bins(50)));
				plots.back()->Draw("");
				canvas->cd(c+parameters.size());
				plots.push_back(mcstudy->plotError(*pdf_data_9D->GetVariable(p),Bins(50)));
				plots.back()->Draw("");
				canvas->cd(c+2*parameters.size());
				plots.push_back(mcstudy->plotPull(*pdf_data_9D->GetVariable(p),Bins(50),FitGauss(true)));
				plots.back()->Draw("");
				++c;
			}
			std::stringstream ss;
			ss << opt->GetOutdir() << "toystudy_" << opt->GetNToys() << (opt->GetMinos() ? "_minos" : "") << "." << opt->GetPlotFormat();
			canvas->SaveAs(ss.str().c_str());
			delete canvas;
			c = 0;
			for(const std::string& p : parameters) {
				canvas = new TCanvas("c_canvas","c_canvas",1280*3,1024);
				canvas->Divide(3,1);
				canvas->cd(1);
				plots[c]->Draw("");
				++c;
				canvas->cd(2);
				plots[c]->Draw("");
				++c;
				canvas->cd(3);
				plots[c]->Draw("");
				++c;
				ss.str("");
				ss << opt->GetOutdir() << "toystudy_" << opt->GetNToys() << (opt->GetMinos() ? "_minos" : "") << "_" << p << "." << opt->GetPlotFormat();
				canvas->SaveAs(ss.str().c_str());
				delete canvas;
			}
			for(RooPlot* p : plots) {
				delete p;
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

	if(opt->GetCreatePlots()) {
		PrintFitStartBlock("GENERATE: 9D Fit");
		// resetting continuum shape and yields to default values
		pdf_bkg_continuum_9D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_continuum_8D.dat",false);
		pdf_bkg_continuum_9D->SetVariablesConst();
		pdf_bkg_continuum_9D->GetVariable("mbc_bkg_continuum_c")->setConstant(false);
		//pdf_signal_9D->ImportPDFValues(opt->GetOutdir()+"pdf_signal_4D.dat",false);
		pdf_data_9D->GetVariable("signal_nevents")->setVal(truth_map["signal_nevents"]); // 1000 for C
		pdf_data_9D->GetVariable("C_bkg_continuum_nevents")->setVal(truth_map["C_bkg_continuum_nevents"]);
		pdf_data_9D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents")->setVal(truth_map["C_bkg_bbar_wrongphikkbar_nevents"]);

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
		std::map<std::string,double> physics_parameters_expectation = ComputePhysicsParameters(pdf_data_9D, pdf_signal_9D, varset);
		std::cout << mp::map_to_string(physics_parameters_expectation) << std::endl;

		std::cout << "INFO: Expected events for C: " << pdf_data_9D->GetPDF("C").expectedEvents(varset_9D) << std::endl;
		RooDataSet* data_set_gen = pdf_data_9D->GetPDF("C").generate(varset_9D,RooRandom::randomGenerator()->Poisson(pdf_data_9D->GetPDF("C").expectedEvents(varset_9D)));
		RooDataSet* data = new RooDataSet("data","data",varset_9D,Index(channel),	Import("C",*data_set_gen));
		delete data_set_gen;
		data->Print();

		// reset to fit start values
		pdf_data_9D->GetVariable("amp_00")->setVal(0.75);
		pdf_data_9D->GetVariable("amp_10")->setVal(0.707);
		pdf_data_9D->GetVariable("amp_1para")->setVal(0.5);
		pdf_data_9D->GetVariable("amp_1perp")->setVal(0.5);
		pdf_data_9D->GetVariable("amp_20")->setVal(0.707*0.8);
		pdf_data_9D->GetVariable("amp_2para")->setVal(0.5*0.8);
		pdf_data_9D->GetVariable("amp_2perp")->setVal(0.5*0.8);


		pdf_data_9D->GetVariable("phase_10")->setVal(0.0);
		pdf_data_9D->GetVariable("phase_1para")->setVal(0.0);
		pdf_data_9D->GetVariable("phase_1perp")->setVal(0.0);
		pdf_data_9D->GetVariable("phase_20")->setVal(0.0);
		pdf_data_9D->GetVariable("phase_2para")->setVal(0.0);
		pdf_data_9D->GetVariable("phase_2perp")->setVal(0.0);

		pdf_data_9D->GetVariable("amp_00_delta")->setVal(0.0);
		pdf_data_9D->GetVariable("amp_10_delta")->setVal(0.0);
		pdf_data_9D->GetVariable("amp_1para_delta")->setVal(0.0);
		pdf_data_9D->GetVariable("amp_1perp_delta")->setVal(0.0);
		pdf_data_9D->GetVariable("amp_20_delta")->setVal(0.0);
		pdf_data_9D->GetVariable("amp_2para_delta")->setVal(0.0);
		pdf_data_9D->GetVariable("amp_2perp_delta")->setVal(0.0);

		pdf_data_9D->GetVariable("phase_10_delta")->setVal(0.0);
		pdf_data_9D->GetVariable("phase_1para_delta")->setVal(0.0);
		pdf_data_9D->GetVariable("phase_1perp_delta")->setVal(0.0);
		pdf_data_9D->GetVariable("phase_20_delta")->setVal(0.0);
		pdf_data_9D->GetVariable("phase_2para_delta")->setVal(0.0);
		pdf_data_9D->GetVariable("phase_2perp_delta")->setVal(0.0);

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

		RooFitResult* result_noCPV = pdf_data_9D->GetSimPDF().fitTo(*data, SumW2Error(false), NumCPU(opt->GetNumcpu()),
				Timer(true), Save(true), InitialHesse(false), Minos(false), Extended(true),
				Constrain(RooArgSet(*pdf_data_9D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents"))));
		result_noCPV->Print();
		delete result_noCPV;

		// check if any phase is outside the [-pi,pi] region and push it back towards truth value
		// this is only to get good pulls, the other solution is not wrong
		ShiftPhaseToInterval(pdf_data_9D, "phase_10", truth_map["phase_10"]);
		ShiftPhaseToInterval(pdf_data_9D, "phase_1para", truth_map["phase_1para"]);
		ShiftPhaseToInterval(pdf_data_9D, "phase_1perp", truth_map["phase_1perp"]);
		ShiftPhaseToInterval(pdf_data_9D, "phase_20", truth_map["phase_20"]);
		ShiftPhaseToInterval(pdf_data_9D, "phase_2para", truth_map["phase_2para"]);
		ShiftPhaseToInterval(pdf_data_9D, "phase_2perp", truth_map["phase_2perp"]);

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

		PrintFitStartBlock("FITTING: 9D Fit");
		// then continue with the 8D fit
		// although we have a weighted dataset, we set SumW2Error(false) so that we get errors that correspond to the
		// statistics we would have if we have 'sumw2' events, and not errors that correspond to the number of entries
		RooFitResult *result_toy = pdf_data_9D->GetSimPDF().fitTo(*data, SumW2Error(false), NumCPU(opt->GetNumcpu()),
				Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()), Extended(true),
				Constrain(RooArgSet(*pdf_data_9D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents"))));
		pdf_data_9D->GetSimPDF().getParameters(data)->Print("v");
		result_toy->Print();
		WriteCorrelationMatrixFile(result_toy,opt->GetOutdir()+"TOY_9D_fit_parameter_correlation.dat");
		PrintFitResultLatex(pdf_data_9D,truth_map);
		PrintPhysicsParametersLatex(PropagateErrors(pdf_data_9D,pdf_signal_9D,result_toy,opt,varset_9D,false));

		for(RooRealVar* v : variables_8D) {
			std::string varname = v->GetName();
			FFUtil::VisualizeFitResult(pdf_data_9D,*data,*v,category_name_flags_channel,true,
					opt->GetBins(),"full_range",opt->GetOutdir()+"TOY_9D_"+varname+"_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
					opt->GetPull(),opt->GetLogy(),false,false,opt->GetPullFiterror(),opt->GetPdfLegend());
			if(varname == "mbc" || varname == "deltae" || varname == "chil0_ma" || varname == "nb_prime") {
				FFUtil::VisualizeFitResult(pdf_data_9D,*data,*v,category_name_flags_channel,true,
						opt->GetBins(),("signal_box_"+varname).c_str(),opt->GetOutdir()+"TOY_9D_"+varname+"_projection_signal_box."+opt->GetPlotFormat(),opt->GetPlotFormat(),
						opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
			} else {
				FFUtil::VisualizeFitResult(pdf_data_9D,*data,*v,category_name_flags_channel,true,
						opt->GetBins(),"signal_box",opt->GetOutdir()+"TOY_9D_"+varname+"_projection_signal_box."+opt->GetPlotFormat(),opt->GetPlotFormat(),
						opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
			}
			if(varname != "mbc") {
				FFUtil::VisualizeFitResult(pdf_data_9D,*data,*v,category_name_flags_channel,true,
						opt->GetBins(),"mbc_sideband", opt->GetOutdir()+"TOY_9D_"+varname+"_projection_mbc_sideband."+opt->GetPlotFormat(),opt->GetPlotFormat(),
						opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
			}
		}

		if(opt->GetCreateProfileLL()) {
			for(const std::string& p : parameters) {
				if(p == "C_bkg_continuum_nevents" || p == "C_bkg_bbar_wrongphikkbar_nevents" || p == "mbc_bkg_continuum_c")
					continue;

				// fix amplitude parameters for the profile LL
				pdf_data_9D->GetVariable("amp_00")->setConstant(true);
				pdf_data_9D->GetVariable("amp_10")->setConstant(true);
				pdf_data_9D->GetVariable("amp_1para")->setConstant(true);
				pdf_data_9D->GetVariable("amp_1perp")->setConstant(true);
				pdf_data_9D->GetVariable("amp_20")->setConstant(true);
				pdf_data_9D->GetVariable("amp_2para")->setConstant(true);
				pdf_data_9D->GetVariable("amp_2perp")->setConstant(true);

				pdf_data_9D->GetVariable("amp_00_delta")->setConstant(true);
				pdf_data_9D->GetVariable("amp_10_delta")->setConstant(true);
				pdf_data_9D->GetVariable("amp_1para_delta")->setConstant(true);
				pdf_data_9D->GetVariable("amp_1perp_delta")->setConstant(true);
				pdf_data_9D->GetVariable("amp_20_delta")->setConstant(true);
				pdf_data_9D->GetVariable("amp_2para_delta")->setConstant(true);
				pdf_data_9D->GetVariable("amp_2perp_delta")->setConstant(true);

				PrintFitStartBlock("Profile LL for parameter " + p);
				double min, max;
				if(p.find("phase") < p.npos && p.find("delta") < p.npos) {
					min = -TMath::Pi()/2;
					max = TMath::Pi()/2;
				} else if (p.find("phase") < p.npos) {
					min = -4; //-2*TMath::Pi();
					max = 4; //2*TMath::Pi();
				} else if (p.find("amp") < p.npos && p.find("delta") < p.npos) {
					min = -0.3;
					max = 0.3;
					continue;
				} else if (p.find("amp") < p.npos) {
					min = 0.0;
					max = 1.0;
					continue;
				} else if (p == "signal_nevents") {
					min = 900;
					max = 1100;
					continue;
				} else {
					continue;
				}

				TCanvas *canvas = new TCanvas("c_canvas","c_canvas",1280,1024);

				RooPlot* frame = pdf_data_9D->GetVariable(p)->frame(Bins(20),
						Range(min,max),
						Title(("LL and profileLL of "+p).c_str()));

				RooAbsReal* nll = pdf_data_9D->GetSimPDF().createNLL(*data,NumCPU(opt->GetNumcpu()));
				nll->plotOn(frame,ShiftToZero(),LineColor(kBlue));

				// Plot the profile likelihood
				RooAbsReal* pll = nll->createProfile(RooArgSet(*(pdf_data_9D->GetVariable(p)))) ;
				pll->plotOn(frame,ShiftToZero(),LineColor(kRed),Precision(1e-1));

				// Adjust frame maximum for visual clarity
				frame->SetMinimum(0);
				frame->SetMaximum(5);

				frame->Draw();

				std::stringstream ss;
				ss << opt->GetOutdir() << "TOY_9D_NLL_SCAN_" << p << "." << opt->GetPlotFormat();
				canvas->SaveAs(ss.str().c_str());
				delete canvas;
				delete nll;
				delete pll;
				delete frame;
			}
		}
		delete result_toy;
		delete data;
	}

	if(!(opt->GetSingleToy())) {
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
	}

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

void FitToyMC_9D_random_start(shared_ptr<FFOpt> opt) {
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
	shared_ptr<FF_PDF> pdf_bkg_bbar_wrongphikkbar_9D = make_shared<FF_PDF_bkg_bbar_wrongphikkbar_9D>(mbc,
			deltae, chil0_ma, nb_prime, mass_kpi,
			hel_phi, hel_th1, hel_th2, flavor, category_name_flags_channel, false);
	pdf_bkg_bbar_wrongphikkbar_9D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_bbar_wrongphikkbar_8D.dat",true);
	pdf_bkg_bbar_wrongphikkbar_9D->SetVariablesConst();

	shared_ptr<FF_PDF> pdf_data_9D = make_shared<FF_PDF_data_9D__CPV>(pdf_signal_9D,
			pdf_bkg_continuum_9D,
			pdf_bkg_bbar_wrongphikkbar_9D, 20000.0, category_name_flags_channel);

	pdf_data_9D->GetVariable("signal_nevents")->setVal(1000); // 1000 for C
	pdf_data_9D->GetVariable("C_bkg_continuum_nevents")->setVal(7350*2+350*2);
	pdf_data_9D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents")->setVal(15.0);

	std::map<std::string, double> truth_map;
	truth_map["signal_nevents"] = 1000; // 1000 for C
	truth_map["C_bkg_continuum_nevents"] = 7350*2+350*2;
	truth_map["C_bkg_bbar_wrongphikkbar_nevents"] = 15.0;
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

	PrintFitStartBlock("TOY-STUDY: 9D Fit - Random-Start");
	std::string outdir_files = opt->GetOutdir()+"/random_start_results/";
	mp::execute_command("mkdir -p "+outdir_files);

	PrintFitStartBlock("GENERATE: 9D Fit");
	// resetting continuum shape and yields to default values
	pdf_bkg_continuum_9D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_continuum_8D.dat",false);
	pdf_bkg_continuum_9D->SetVariablesConst();
	pdf_bkg_continuum_9D->GetVariable("mbc_bkg_continuum_c")->setConstant(false);
	//pdf_signal_9D->ImportPDFValues(opt->GetOutdir()+"pdf_signal_4D.dat",false);
	pdf_data_9D->GetVariable("signal_nevents")->setVal(truth_map["signal_nevents"]); // 1000 for C
	pdf_data_9D->GetVariable("C_bkg_continuum_nevents")->setVal(truth_map["C_bkg_continuum_nevents"]);
	pdf_data_9D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents")->setVal(truth_map["C_bkg_bbar_wrongphikkbar_nevents"]);

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
	std::map<std::string,double> physics_parameters_expectation = ComputePhysicsParameters(pdf_data_9D, pdf_signal_9D, varset);
	std::cout << mp::map_to_string(physics_parameters_expectation) << std::endl;

	std::cout << "INFO: Expected events for C: " << pdf_data_9D->GetPDF("C").expectedEvents(varset_9D) << std::endl;
	RooDataSet* data_set_gen = pdf_data_9D->GetPDF("C").generate(varset_9D,RooRandom::randomGenerator()->Poisson(pdf_data_9D->GetPDF("C").expectedEvents(varset_9D)));
	RooDataSet* data = new RooDataSet("data","data",varset_9D,Index(channel),Import("C",*data_set_gen));
	delete data_set_gen;

	// scale Delta E resolution
	double scale = 1.124;
	pdf_data_9D->GetVariable("C_deltae_signal_width_core")->setVal(pdf_data_9D->GetVariable("C_deltae_signal_width_core")->getVal()*scale);
	pdf_data_9D->GetVariable("C_deltae_signal_width_1st_left")->setVal(pdf_data_9D->GetVariable("C_deltae_signal_width_1st_left")->getVal()*scale);
	pdf_data_9D->GetVariable("C_deltae_signal_width_1st_right")->setVal(pdf_data_9D->GetVariable("C_deltae_signal_width_1st_right")->getVal()*scale);
	pdf_data_9D->GetVariable("C_deltae_signal_width_2nd_left")->setVal(pdf_data_9D->GetVariable("C_deltae_signal_width_2nd_left")->getVal()*scale);
	pdf_data_9D->GetVariable("C_deltae_signal_width_2nd_right")->setVal(pdf_data_9D->GetVariable("C_deltae_signal_width_2nd_right")->getVal()*scale);

	// reset to fit start values
	pdf_data_9D->GetVariable("amp_00")->setVal(0.75);
	pdf_data_9D->GetVariable("amp_10")->setVal(0.707);
	pdf_data_9D->GetVariable("amp_1para")->setVal(0.5);
	pdf_data_9D->GetVariable("amp_1perp")->setVal(0.5);
	pdf_data_9D->GetVariable("amp_20")->setVal(0.707*0.8);
	pdf_data_9D->GetVariable("amp_2para")->setVal(0.5*0.8);
	pdf_data_9D->GetVariable("amp_2perp")->setVal(0.5*0.8);

	// seed after generator, to generate always the same data set but fit with different starting values
	RooRandom::randomGenerator()->SetSeed(opt->GetRandomSeed()+opt->GetSingleToy());
	pdf_data_9D->GetVariable("phase_10")->setVal(RooRandom::randomGenerator()->Uniform(-TMath::Pi(),TMath::Pi()));
	pdf_data_9D->GetVariable("phase_1para")->setVal(RooRandom::randomGenerator()->Uniform(-TMath::Pi(),TMath::Pi()));
	pdf_data_9D->GetVariable("phase_1perp")->setVal(RooRandom::randomGenerator()->Uniform(-TMath::Pi(),TMath::Pi()));
	pdf_data_9D->GetVariable("phase_20")->setVal(RooRandom::randomGenerator()->Uniform(-TMath::Pi(),TMath::Pi()));
	pdf_data_9D->GetVariable("phase_2para")->setVal(RooRandom::randomGenerator()->Uniform(-TMath::Pi(),TMath::Pi()));
	pdf_data_9D->GetVariable("phase_2perp")->setVal(RooRandom::randomGenerator()->Uniform(-TMath::Pi(),TMath::Pi()));

	std::cout << "-------------------------------------------------" << std::endl;
	std::cout << "Random Seed was: " << opt->GetRandomSeed()+opt->GetSingleToy() << std::endl;
	std::cout << "phase_10:    " << pdf_data_9D->GetVariable("phase_10")->getVal() << std::endl;
	std::cout << "phase_1para: " << pdf_data_9D->GetVariable("phase_1para")->getVal() << std::endl;
	std::cout << "phase_1perp: " << pdf_data_9D->GetVariable("phase_1perp")->getVal() << std::endl;
	std::cout << "phase_20:    " << pdf_data_9D->GetVariable("phase_20")->getVal() << std::endl;
	std::cout << "phase_2para: " << pdf_data_9D->GetVariable("phase_2para")->getVal() << std::endl;
	std::cout << "phase_2perp: " << pdf_data_9D->GetVariable("phase_2perp")->getVal() << std::endl;
	std::cout << "-------------------------------------------------" << std::endl;

	pdf_data_9D->GetVariable("amp_00_delta")->setVal(0.0);
	pdf_data_9D->GetVariable("amp_10_delta")->setVal(0.0);
	pdf_data_9D->GetVariable("amp_1para_delta")->setVal(0.0);
	pdf_data_9D->GetVariable("amp_1perp_delta")->setVal(0.0);
	pdf_data_9D->GetVariable("amp_20_delta")->setVal(0.0);
	pdf_data_9D->GetVariable("amp_2para_delta")->setVal(0.0);
	pdf_data_9D->GetVariable("amp_2perp_delta")->setVal(0.0);

	pdf_data_9D->GetVariable("phase_10_delta")->setVal(0.0);
	pdf_data_9D->GetVariable("phase_1para_delta")->setVal(0.0);
	pdf_data_9D->GetVariable("phase_1perp_delta")->setVal(0.0);
	pdf_data_9D->GetVariable("phase_20_delta")->setVal(0.0);
	pdf_data_9D->GetVariable("phase_2para_delta")->setVal(0.0);
	pdf_data_9D->GetVariable("phase_2perp_delta")->setVal(0.0);

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

	RooFitResult* result_noCPV = pdf_data_9D->GetSimPDF().fitTo(*data, SumW2Error(false), NumCPU(opt->GetNumcpu()),
			Timer(true), Save(true), InitialHesse(false), Minos(false), /*Minos(phase_vars),*/ Extended(true),
			Constrain(RooArgSet(*pdf_data_9D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents"))));
	result_noCPV->Print();

	if(opt->GetCreateProfileLL()) {
		for(const std::string& p : parameters) {
			if(!(p == "phase_10" || p == "phase_1para" || p == "phase_1perp" || p == "phase_20" || p == "phase_2para" || p == "phase_2perp"))
				continue;

			PrintFitStartBlock("Profile LL for parameter " + p);
			double min, max;
			if(p.find("phase") < p.npos && p.find("delta") < p.npos) {
				min = -TMath::Pi()/2;
				max = TMath::Pi()/2;
			} else if (p.find("phase") < p.npos) {
				min = -4; //-2*TMath::Pi();
				max = 4; //2*TMath::Pi();
			} else if (p.find("amp") < p.npos && p.find("delta") < p.npos) {
				min = -0.3;
				max = 0.3;
				continue;
			} else if (p.find("amp") < p.npos) {
				min = 0.0;
				max = 1.0;
				continue;
			} else if (p == "signal_nevents") {
				min = 900;
				max = 1100;
				continue;
			} else {
				continue;
			}

			TCanvas *canvas = new TCanvas("c_canvas","c_canvas",1280,1024);

			RooPlot* frame = pdf_data_9D->GetVariable(p)->frame(Bins(20),
					Range(min,max),
					Title(("LL and profileLL of "+p).c_str()));

			RooAbsReal* nll = pdf_data_9D->GetSimPDF().createNLL(*data,NumCPU(opt->GetNumcpu()));
			nll->plotOn(frame,ShiftToZero(),LineColor(kBlue));

			// Plot the profile likelihood
			RooAbsReal* pll = nll->createProfile(RooArgSet(*(pdf_data_9D->GetVariable(p)))) ;
			pll->plotOn(frame,ShiftToZero(),LineColor(kRed),Precision(1e-1));

			// Adjust frame maximum for visual clarity
			frame->SetMinimum(0);
			frame->SetMaximum(5);

			frame->Draw();

			std::stringstream ss;
			ss << opt->GetOutdir() << "TOY_9D_NLL_SCAN_NO_CPV_" << p << "." << opt->GetPlotFormat();
			canvas->SaveAs(ss.str().c_str());
			delete canvas;
			delete nll;
			delete pll;
			delete frame;
		}
	}
	delete result_noCPV;

	// check if any phase is outside the [-pi,pi] region and push it back towards truth value
	// this is only to get good pulls, the other solution is not wrong
	ShiftPhaseToInterval(pdf_data_9D, "phase_10", truth_map["phase_10"]);
	ShiftPhaseToInterval(pdf_data_9D, "phase_1para", truth_map["phase_1para"]);
	ShiftPhaseToInterval(pdf_data_9D, "phase_1perp", truth_map["phase_1perp"]);
	ShiftPhaseToInterval(pdf_data_9D, "phase_20", truth_map["phase_20"]);
	ShiftPhaseToInterval(pdf_data_9D, "phase_2para", truth_map["phase_2para"]);
	ShiftPhaseToInterval(pdf_data_9D, "phase_2perp", truth_map["phase_2perp"]);

	PrintFitResultLatex(pdf_data_9D,truth_map);

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

	PrintFitStartBlock("FITTING: 9D Fit");
	// then continue with the 8D fit
	// although we have a weighted dataset, we set SumW2Error(false) so that we get errors that correspond to the
	// statistics we would have if we have 'sumw2' events, and not errors that correspond to the number of entries
	RooFitResult *result_toy = pdf_data_9D->GetSimPDF().fitTo(*data, SumW2Error(false), NumCPU(opt->GetNumcpu()),
			Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()), Extended(true),
			Constrain(RooArgSet(*pdf_data_9D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents"))));
	pdf_data_9D->GetSimPDF().getParameters(data)->Print("v");
	WriteCorrelationMatrixFile(result_toy,outdir_files+"TOY_9D_fit_parameter_correlation_"+mp::to_string(opt->GetSingleToy())+".dat");
	result_toy->Print();

	// check if any phase is outside the [-pi,pi] region and push it back towards truth value
	// this is only to get good pulls, the other solution is not wrong
	ShiftPhaseToInterval(pdf_data_9D, "phase_10", truth_map["phase_10"]);
	ShiftPhaseToInterval(pdf_data_9D, "phase_1para", truth_map["phase_1para"]);
	ShiftPhaseToInterval(pdf_data_9D, "phase_1perp", truth_map["phase_1perp"]);
	ShiftPhaseToInterval(pdf_data_9D, "phase_20", truth_map["phase_20"]);
	ShiftPhaseToInterval(pdf_data_9D, "phase_2para", truth_map["phase_2para"]);
	ShiftPhaseToInterval(pdf_data_9D, "phase_2perp", truth_map["phase_2perp"]);

	PrintFitResultLatex(pdf_data_9D,truth_map);

	std::stringstream ss;
	ss << opt->GetSingleToy();
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
		fout << "EDM;" << result_toy->edm() << std::endl;
		fout << "NLL;" << result_toy->minNll() << std::endl;
		fout.close();
	} else {
		std::cerr << "ERROR: Could not write fit result to file" << std::endl;
	}

	PrintPhysicsParametersLatex(PropagateErrors(pdf_data_9D,pdf_signal_9D,result_toy,opt,varset_9D,false));

	delete data;
	delete result_toy;

	if(!(opt->GetSingleToy())) {
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
	}

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
