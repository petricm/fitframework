/**
 * @file	FF_Fit_gsim.cc
 * @date	Feb 8, 2013
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

void FitGSIMToyMC_4D(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt) {
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

	std::vector<RooRealVar*> variables_8D;
	variables_8D.push_back(&mbc);
	variables_8D.push_back(&deltae);
	variables_8D.push_back(&chil0_ma);
	variables_8D.push_back(&nb_prime);
	variables_8D.push_back(&mass_kpi);
	variables_8D.push_back(&hel_phi);
	variables_8D.push_back(&hel_th1);
	variables_8D.push_back(&hel_th2);
	RooArgSet varset_8D(mbc,deltae,chil0_ma,nb_prime,mass_kpi,hel_phi,hel_th1,hel_th2);

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
			pdf_bkg_continuum_4D, pdf_bkg_bbar_wrongphikkbar_4D,
			20000.0, category_name_flags_datatype);

	pdf_data_4D->GetVariable("signal_nevents")->setVal(1000); // 1000 for C
	pdf_data_4D->GetVariable("signal_nevents_asym")->setVal(0.0);
	pdf_data_4D->GetVariable("C_bkg_continuum_nevents")->setVal(7350*2+350*2);
	pdf_data_4D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents")->setVal(15.0);

	std::map<std::string, double> truth_map;
	truth_map["signal_nevents"] = 1000; // 1000 for C
	truth_map["signal_nevents_asym"] = 0.0;
	truth_map["C_bkg_continuum_nevents"] = 7350*2+350*2;
	truth_map["C_bkg_bbar_wrongphikkbar_nevents"] = 14.00;

	PrintFitStartBlock("INIT BOOTSTRAPPER: 4D Fit");
	std::map<std::string, std::string> parameter_cut;
	parameter_cut["signal_nevents"] = "channel == 0 && mbc < 5.289 && hel_th1 < 0.75 && ((mctype<1001 || mctype>1004) && runno==0 && (mcinfo==59 || mcinfo==61) && abs(chil0_mo)==511)";
	parameter_cut["C_bkg_continuum_nevents"] = "channel == 0 && mbc < 5.289 && hel_th1 < 0.75 && ((mctype>=1001 && mctype<=1004) || ((mctype<1001 || mctype>1004) && runno==0 && mcinfo!=59 && mcinfo!=61 && (mcinfo != 37 && mcinfo!=53 && mcinfo != 29)))";
	//parameter_cut["N_bkg_continuum_nevents"] = "channel == 1 && ((mctype>=1001 && mctype<=1004) || ((mctype<1001 || mctype>1004) && runno==0 && mcinfo!=59 && mcinfo!=61 && (mcinfo != 37 && mcinfo!=53 && mcinfo != 29)))";
	//parameter_cut["C_bkg_bbar_combinatoric_nevents"] = "channel == 0 && (mctype==1001 || mctype==1002 || ((mctype<1001 || mctype>1004) && runno==0 && mcinfo!=59 && mcinfo!=61 && (mcinfo != 37 && mcinfo!=53 && mcinfo != 29)))";
	//parameter_cut["N_bkg_bbar_combinatoric_nevents"] = "channel == 1 && (mctype==1001 || mctype==1002 || ((mctype<1001 || mctype>1004) && runno==0 && mcinfo!=59 && mcinfo!=61 && (mcinfo != 37 && mcinfo!=53 && mcinfo != 29)))";
	parameter_cut["C_bkg_bbar_wrongphikkbar_nevents"] = "channel == 0 && mbc < 5.289 && hel_th1 < 0.75 && ((mctype<1001 || mctype>1004) && runno==0 && mcinfo==29 && chil0_mc!=45)";
	//parameter_cut["N_bkg_bbar_wrongphikkbar_nevents"] = "channel == 1 && ((mctype<1001 || mctype>1004) && runno==0 && mcinfo==29 && chil0_mc!=45)";
	//parameter_cut["C_bkg_bbar_wrongphif0980_nevents"] = "channel == 0 && ((mctype<1001 || mctype>1004) && runno==0 && mcinfo==29 && chil0_mc==45)";
	//parameter_cut["N_bkg_bbar_wrongphif0980_nevents"] = "channel == 1 && ((mctype<1001 || mctype>1004) && runno==0 && mcinfo==29 && chil0_mc==45)";
	//parameter_cut["C_bkg_bbar_wrongmasshyp_nevents"] = "channel == 0 && ((mctype<1001 || mctype>1004) && runno==0 && (mcinfo==37 || mcinfo==53))";

	std::map<std::string, std::string> parameter_cut_CP;
	std::map<std::string, std::string> parameter_cut_CA;
	for(auto& p : parameter_cut) {
		parameter_cut_CP[p.first] = "datatype == 0 && " + p.second;
		parameter_cut_CA[p.first] = "datatype == 1 && " + p.second;
	}
	FF_Bootstrap bootstrapper_CP(chain.get(),parameter_cut_CP,varset,"weight","CP");
	FF_Bootstrap bootstrapper_CA(chain.get(),parameter_cut_CA,varset,"weight","CA");

	std::map<std::string, std::string> parameter_cut_CS;
	parameter_cut_CS["C_bkg_continuum_nevents_sideband"] = "datatype == 3 && channel == 0 && hel_th1 < 0.75 && ((mctype>=1001 && mctype<=1004) || ((mctype<1001 || mctype>1004) && runno==0 && mcinfo!=59 && mcinfo!=61 && (mcinfo != 37 && mcinfo!=53 && mcinfo != 29)))";
	FF_Bootstrap bootstrapper_CS(chain.get(),parameter_cut_CS,varset,"weight","CS");

	std::map<std::string, unsigned int> generator_truth_CP;
	generator_truth_CP["signal_nevents"] = 500;
	generator_truth_CP["C_bkg_continuum_nevents"] = 7350+350;
	//generator_truth_CP["C_bkg_bbar_combinatoric_nevents"] = 600;
	generator_truth_CP["C_bkg_bbar_wrongphikkbar_nevents"] = 7;
	std::map<std::string, unsigned int> generator_truth_CA;
	generator_truth_CA["signal_nevents"] = 500;
	generator_truth_CA["C_bkg_continuum_nevents"] = 7350+350;
	//generator_truth_CA["C_bkg_bbar_combinatoric_nevents"] = 600;
	generator_truth_CA["C_bkg_bbar_wrongphikkbar_nevents"] = 7;
	std::map<std::string, unsigned int> generator_truth_CS;
	generator_truth_CS["C_bkg_continuum_nevents_sideband"] = 15000;

	PrintFitStartBlock("GSIM-TOY-STUDY: 4D Fit");
	if(opt->GetNGsimToys() > 0) {
		RooMCStudy* mcstudy = new RooMCStudy(pdf_data_4D->GetSimPDF(),varset_4D);
		std::vector<RooFitResult*> fit_results;
		std::vector<RooDataSet*> data_sets;
		std::vector<RooFitResult*> fit_results_sideband;
		std::vector<RooDataSet*> data_sets_sideband;
		std::vector<std::string> parameters;
		parameters.push_back("signal_nevents");
		parameters.push_back("signal_nevents_asym");
		parameters.push_back("C_bkg_continuum_nevents");
		//parameters.push_back("C_bkg_bbar_wrongphikkbar_nevents");
		std::string outdir_files = opt->GetOutdir()+"/gsim_toystudy_"+mp::to_string(opt->GetNGsimToys())+(opt->GetMinos() ? "_minos" : "")+"/";
		mp::execute_command("mkdir -p "+outdir_files);
		for(int i = 1; i <= opt->GetNGsimToys(); ++i) {
			if(opt->GetSingleToy()) {
				if(i != (int)opt->GetSingleToy()) { // continue until we have the single toy we want
					continue;
				} else { // initialize random seed for this single toy
					RooRandom::randomGenerator()->SetSeed(opt->GetRandomSeed()+opt->GetSingleToy());
				}
			}
			std::stringstream ss;
			ss << "sample_" << i;
			std::cout << "INFO: Gsim Toy Sample: " << i << std::endl;

			RooDataSet *data_set_CP = bootstrapper_CP.bootstrap(generator_truth_CP,i);
			RooDataSet *data_set_CA = bootstrapper_CA.bootstrap(generator_truth_CA,i);
			RooDataSet *data_set_CS = bootstrapper_CS.bootstrap(generator_truth_CS,i);

			data_sets.push_back(new RooDataSet(("data_"+ss.str()).c_str(),("data_"+ss.str()).c_str(),RooArgSet(varset_4D,weight),Index(datatype),
					Import("CP",*data_set_CP),Import("CA",*data_set_CA), WeightVar(weight)));
			delete data_set_CP;
			delete data_set_CA;
			data_sets.back()->Print();
			data_sets_sideband.push_back(new RooDataSet(("data_sideband_"+ss.str()).c_str(),("data_sideband_"+ss.str()).c_str(),RooArgSet(varset_4D,weight),Index(datatype),
					Import("CS",*data_set_CS), WeightVar(weight)));
			delete data_set_CS;
			data_sets_sideband.back()->Print();

			// reset all parameters
			pdf_bkg_continuum_4D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_continuum_4D.dat",false);
			//pdf_signal_4D->ImportPDFValues(opt->GetOutdir()+"pdf_signal_4D.dat",false);
			pdf_data_4D->GetVariable("signal_nevents")->setVal(1000); // 1000 for C
			pdf_data_4D->GetVariable("signal_nevents_asym")->setVal(0.0);
			pdf_data_4D->GetVariable("C_bkg_continuum_nevents")->setVal(7350*2+350*2);
			pdf_data_4D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents")->setVal(14.0);

			// fit the continuum pdf to sideband data and fix the shape
			mbc.setMin(5.24);
			mbc.setMax(5.26);
			mbc.setRange("full_range",5.24,5.26);

			// release all parameters that shall be floated
			pdf_bkg_continuum_4D->GetVariable("C_chil0_ma_bkg_continuum_nonres_fraction")->setConstant(false);
			pdf_bkg_continuum_4D->GetVariable("chil0_ma_bkg_continuum_a")->setConstant(false);
			pdf_bkg_continuum_4D->GetVariable("chil0_ma_resolution")->setConstant(false);
			pdf_bkg_continuum_4D->GetVariable("deltae_bkg_continuum_a1")->setConstant(false);
			pdf_bkg_continuum_4D->GetVariable("mbc_bkg_continuum_c")->setConstant(false);
			pdf_bkg_continuum_4D->GetVariable("nb_prime_bkg_continuum_frac_1st")->setConstant(false);
			pdf_bkg_continuum_4D->GetVariable("nb_prime_bkg_continuum_width_1st_left")->setConstant(false);
			pdf_bkg_continuum_4D->GetVariable("nb_prime_bkg_continuum_width_1st_right")->setConstant(false);
			pdf_bkg_continuum_4D->GetVariable("nb_prime_bkg_continuum_width_2nd_left")->setConstant(false);
			pdf_bkg_continuum_4D->GetVariable("nb_prime_bkg_continuum_width_2nd_right")->setConstant(false);

			// set MINOS to false as we don't care about the MINOS errors of the shape parameters
			fit_results_sideband.push_back(pdf_bkg_continuum_4D->GetSimPDF().fitTo(*data_sets_sideband.back(), SumW2Error(false), NumCPU(opt->GetNumcpu()),
					Timer(true), Save(true), InitialHesse(false), Minos(false), Extended(false)));
			fit_results_sideband.back()->Print();

			// fit the fit region data with fixed pdf
			mbc.setMax(5.29);
			mbc.setMin(5.26);
			mbc.setRange("full_range",5.26,5.29);
			pdf_bkg_continuum_4D->SetVariablesConst();
			pdf_bkg_continuum_4D->GetVariable("mbc_bkg_continuum_c")->setConstant(false);

			fit_results.push_back(pdf_data_4D->GetSimPDF().fitTo(*data_sets.back(), SumW2Error(false), NumCPU(opt->GetNumcpu()),
					Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()), Extended(true),
					Constrain(RooArgSet(*pdf_data_4D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents")))));
			fit_results.back()->Print();
			mcstudy->addFitResult(*fit_results.back());

			std::ofstream fout((outdir_files+"GSIM_TOY_4D_fit_result_"+ss.str().c_str()+".dat").c_str());
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

			mbc.setMin(5.24);
			mbc.setMax(5.29);
			mbc.setRange("full_range",5.24,5.29);

			// if doing many samples, we might run in some memory limit of the machine we are working on...
			delete data_sets.back();
			data_sets.pop_back();
			delete data_sets_sideband.back();
			data_sets_sideband.pop_back();
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
			ss << opt->GetOutdir() << "gsim_toystudy_" << opt->GetNGsimToys() << (opt->GetMinos() ? "_minos" : "") << "." << opt->GetPlotFormat();
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
				ss << opt->GetOutdir() << "gsim_toystudy_" << opt->GetNGsimToys() << (opt->GetMinos() ? "_minos" : "") << "_" << p << "." << opt->GetPlotFormat();
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
		for(RooDataSet* s : data_sets_sideband) {
			delete s;
		}
		for(RooFitResult* r : fit_results_sideband) {
			delete r;
		}
		delete mcstudy;
	}

	if(opt->GetCreatePlots()) {
		PrintFitStartBlock("GENERATE: 4D Fit");
		RooDataSet *data_CP = bootstrapper_CP.bootstrap(generator_truth_CP,0);
		RooDataSet *data_CA = bootstrapper_CA.bootstrap(generator_truth_CA,0);
		RooDataSet *data_CS = bootstrapper_CS.bootstrap(generator_truth_CS,0);

		std::stringstream ss;
		ss << "sample_0";
		RooDataSet* data = new RooDataSet(("data_"+ss.str()).c_str(),("data_"+ss.str()).c_str(),varset,Index(datatype),
				Import("CP",*data_CP),Import("CA",*data_CA), WeightVar(weight));
		RooDataSet* data_sideband = new RooDataSet(("data_sideband_"+ss.str()).c_str(),("data_sideband_"+ss.str()).c_str(),varset,Index(datatype),
				Import("CS",*data_CS), WeightVar(weight));

		// reset all parameters
		pdf_bkg_continuum_4D->ImportPDFValues(opt->GetOutdir()+"pdf_bkg_continuum_4D.dat",false);
		//pdf_signal_4D->ImportPDFValues(opt->GetOutdir()+"pdf_signal_4D.dat",false);
		pdf_data_4D->GetVariable("signal_nevents")->setVal(1000); // 1000 for C
		pdf_data_4D->GetVariable("signal_nevents_asym")->setVal(0.0);
		pdf_data_4D->GetVariable("C_bkg_continuum_nevents")->setVal(7350*2+350*2);
		pdf_data_4D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents")->setVal(14.0);

		// fit the continuum pdf to sideband data and fix the shape
		mbc.setMin(5.24);
		mbc.setMax(5.26);
		mbc.setRange("full_range",5.24,5.26);

		// release all parameters that shall be floated
		pdf_bkg_continuum_4D->GetVariable("C_chil0_ma_bkg_continuum_nonres_fraction")->setConstant(false);
		pdf_bkg_continuum_4D->GetVariable("chil0_ma_bkg_continuum_a")->setConstant(false);
		pdf_bkg_continuum_4D->GetVariable("chil0_ma_resolution")->setConstant(false);
		pdf_bkg_continuum_4D->GetVariable("deltae_bkg_continuum_a1")->setConstant(false);
		pdf_bkg_continuum_4D->GetVariable("mbc_bkg_continuum_c")->setConstant(false);
		pdf_bkg_continuum_4D->GetVariable("nb_prime_bkg_continuum_frac_1st")->setConstant(false);
		pdf_bkg_continuum_4D->GetVariable("nb_prime_bkg_continuum_width_1st_left")->setConstant(false);
		pdf_bkg_continuum_4D->GetVariable("nb_prime_bkg_continuum_width_1st_right")->setConstant(false);
		pdf_bkg_continuum_4D->GetVariable("nb_prime_bkg_continuum_width_2nd_left")->setConstant(false);
		pdf_bkg_continuum_4D->GetVariable("nb_prime_bkg_continuum_width_2nd_right")->setConstant(false);

		PrintFitStartBlock("FITTING: 4D Fit - Sideband");
		RooFitResult *result_4D_sideband = pdf_bkg_continuum_4D->GetSimPDF().fitTo(*data_sideband, SumW2Error(false), NumCPU(opt->GetNumcpu()),
				Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()), Extended(false));
		pdf_bkg_continuum_4D->GetSimPDF().getParameters(data_sideband)->Print("v");
		result_4D_sideband->Print();

		for(RooRealVar *v : variables_4D) {
			std::string varname = v->GetName();
			FFUtil::VisualizeFitResult(pdf_bkg_continuum_4D,*data_sideband,*v,category_name_flags_datatype,false,
					opt->GetBins(),"full_range", opt->GetOutdir()+"GSIM_TOY_4D_sideband_"+varname+"_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
					opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
		}

		// fit the fit region data with fixed pdf
		mbc.setMax(5.29);
		mbc.setMin(5.26);
		mbc.setRange("full_range",5.26,5.29);
		pdf_bkg_continuum_4D->SetVariablesConst();
		pdf_bkg_continuum_4D->GetVariable("mbc_bkg_continuum_c")->setConstant(false);

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
					opt->GetBins(),"full_range",opt->GetOutdir()+"GSIM_TOY_4D_"+varname+"_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
					opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
			FFUtil::VisualizeFitResult(pdf_data_4D,*data,*v,category_name_flags_datatype,true,
					opt->GetBins(),("signal_box_"+varname).c_str(),opt->GetOutdir()+"GSIM_TOY_4D_"+varname+"_projection_signal_box."+opt->GetPlotFormat(),opt->GetPlotFormat(),
					opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
			if(varname != "mbc") {
				FFUtil::VisualizeFitResult(pdf_data_4D,*data,*v,category_name_flags_datatype,true,
						opt->GetBins(),"mbc_sideband", opt->GetOutdir()+"GSIM_TOY_4D_"+varname+"_projection_mbc_sideband."+opt->GetPlotFormat(),opt->GetPlotFormat(),
						opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
			}
		}

		delete result_toy;
		delete result_4D_sideband;
		delete data_CP;
		delete data_CA;
		delete data_CS;
		delete data;
		delete data_sideband;
	}
}

void FitGSIMToyMC_9D(shared_ptr<TChain> chain, shared_ptr<TChain> chain_reweight, shared_ptr<FFOpt> opt) {
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

	TCut cut_default_CP_and_CA = "(mbc < 5.289 && hel_th1 < 0.75 && (datatype==datatype::CP || datatype==datatype::CA))";
	TCut cut_default_CS = "(mbc < 5.289 && hel_th1 < 0.75 && (datatype==datatype::CS))";

	TCut cut_signal_nevents = "(channel == 0 && mbc < 5.289 && hel_th1 < 0.75 && ((mctype<1001 || mctype>1004) && runno==0 && (mcinfo==59 || mcinfo==61) && abs(chil0_mo)==511))";
	TCut cut_bkg_continuum_nevents = "(channel == 0 && mbc < 5.289 && hel_th1 < 0.75 && ((mctype>=1001 && mctype<=1004) || ((mctype<1001 || mctype>1004) && runno==0 && mcinfo!=59 && mcinfo!=61 && (mcinfo != 37 && mcinfo!=53 && mcinfo != 29))))";
	TCut cut_bkg_bbbar_wrongphikkbar_nevents = "(channel == 0 && mbc < 5.289 && hel_th1 < 0.75 && ((mctype<1001 || mctype>1004) && runno==0 && mcinfo==29 && chil0_mc!=45))";
	TCut cut_bkg_bbar_wrongphif0980_nevents = "(channel == 0 && ((mctype<1001 || mctype>1004) && runno==0 && mcinfo==29 && chil0_mc==45))";
	TCut cut_bkg_bbar_wrongmasshyp_nevents = "(channel == 0 && ((mctype<1001 || mctype>1004) && runno==0 && (mcinfo==37 || mcinfo==53)))";

	RooDataSet data_signal_phsp("data_signal_phsp","data_signal_phsp",varset,Import(*chain_reweight.get()),
			Cut(cut_default_CP_and_CA));
	data_signal_phsp.Print();
	RooDataSet data_bkg("data_bkg","data_bkg",RooArgSet(varset,weight),Import(*chain.get()),
			Cut(cut_default_CP_and_CA && cut_bkg_continuum_nevents),WeightVar(weight));
	data_bkg.Print();
	RooDataSet data_bkg_peak_kkbar("data_bkg_peak_kkbar","data_bkg_peak_kkbar",RooArgSet(varset,weight),Import(*chain.get()),
			Cut(cut_default_CP_and_CA && cut_bkg_bbbar_wrongphikkbar_nevents),WeightVar(weight));
	data_bkg_peak_kkbar.Print();
	RooDataSet data_bkg_peak_f0980("data_bkg_peak_f0980","data_bkg_peak_f0980",RooArgSet(varset,weight),Import(*chain.get()),
			Cut(cut_default_CP_and_CA && cut_bkg_bbar_wrongphif0980_nevents),WeightVar(weight));
	data_bkg_peak_f0980.Print();
	RooDataSet data_bkg_peak_masshyp("data_bkg_peak_masshyp","data_bkg_peak_masshyp",RooArgSet(varset,weight),Import(*chain.get()),
			Cut(cut_default_CP_and_CA && cut_bkg_bbar_wrongmasshyp_nevents),WeightVar(weight));
	data_bkg_peak_masshyp.Print();
	RooDataSet data_bkg_sideband("data_bkg_sideband","data_bkg_sideband",RooArgSet(varset,weight),Import(*chain.get()),
			Cut(cut_default_CS),WeightVar(weight));
	data_bkg_sideband.Print();
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
		CreateDataComponentPlots(data_bkg_sideband,*var,opt->GetBins(),"full_range_SB",opt->GetOutdir(),opt->GetPlotFormat(),"CP","flavor","_sideband");
		CreateDataComponentPlots(data_bkg_sideband,*var,opt->GetBins(),"full_range_SB",opt->GetOutdir(),opt->GetPlotFormat(),"CA","flavor","_sideband");
		CreateDataComponentPlots(data_bkg_sideband,*var,opt->GetBins(),"full_range_SB",opt->GetOutdir(),opt->GetPlotFormat(),"CS","datatype","_sideband");
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
	std::string path_to_reweight_integrals = "./CachedIntegrals_ACCEPTANCE" + FFUtil::GetAcceptanceString(UNIFORM) + "/";
	mass_helicity_angles_norm_cache_integrator->LoadIntegrals(path_to_reweight_integrals);
	mass_helicity_angles_real_cache_integrator->LoadIntegrals(path_to_reweight_integrals);
	mass_helicity_angles_imag_cache_integrator->LoadIntegrals(path_to_reweight_integrals);

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

	PrintFitStartBlock("CREATING TRUTH MAP");
	std::map<std::string, double> truth_map;
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

	PrintFitStartBlock("REWEIGHT SIGNAL");
	shared_ptr<FF_PDF> pdf_signal_5D_phsp = make_shared<FF_PDF_signal_5D_mass_helicity_angles_EvtGen_PHSP>(mass_kpi,
			hel_th1, hel_th2, hel_phi, flavor, category_name_flags_channel);

	shared_ptr<FF_PDF> pdf_signal_5D_angles_uniform = make_shared<FF_PDF_signal_5D_mass_helicity_angles_CPV>(mass_kpi,
			hel_th1, hel_th2, hel_phi, flavor, UNIFORM,
			mhi_pdg_constants, mhi_4D,
			mhi_3D_phi_th1_th2, mhi_3D_m_th1_th2, mhi_3D_m_phi_th2, mhi_3D_m_phi_th1,
			category_name_flags_channel);

	// set the signal pdf to the reweighted configuration
	pdf_signal_5D_angles_uniform->GetVariable("amp_00")->setVal(truth_map["amp_00"]);
	pdf_signal_5D_angles_uniform->GetVariable("amp_10")->setVal(truth_map["amp_10"]);
	pdf_signal_5D_angles_uniform->GetVariable("amp_1para")->setVal(truth_map["amp_1para"]);
	pdf_signal_5D_angles_uniform->GetVariable("amp_1perp")->setVal(truth_map["amp_1perp"]);
	pdf_signal_5D_angles_uniform->GetVariable("amp_20")->setVal(truth_map["amp_20"]);
	pdf_signal_5D_angles_uniform->GetVariable("amp_2para")->setVal(truth_map["amp_2para"]);
	pdf_signal_5D_angles_uniform->GetVariable("amp_2perp")->setVal(truth_map["amp_2perp"]);

	pdf_signal_5D_angles_uniform->GetVariable("phase_10")->setVal(truth_map["phase_10"]);
	pdf_signal_5D_angles_uniform->GetVariable("phase_1para")->setVal(truth_map["phase_1para"]);
	pdf_signal_5D_angles_uniform->GetVariable("phase_1perp")->setVal(truth_map["phase_1perp"]);
	pdf_signal_5D_angles_uniform->GetVariable("phase_20")->setVal(truth_map["phase_20"]);
	pdf_signal_5D_angles_uniform->GetVariable("phase_2para")->setVal(truth_map["phase_2para"]);
	pdf_signal_5D_angles_uniform->GetVariable("phase_2perp")->setVal(truth_map["phase_2perp"]);

	pdf_signal_5D_angles_uniform->GetVariable("amp_00_delta")->setVal(truth_map["amp_00_delta"]);
	pdf_signal_5D_angles_uniform->GetVariable("amp_10_delta")->setVal(truth_map["amp_10_delta"]);
	pdf_signal_5D_angles_uniform->GetVariable("amp_1para_delta")->setVal(truth_map["amp_1para_delta"]);
	pdf_signal_5D_angles_uniform->GetVariable("amp_1perp_delta")->setVal(truth_map["amp_1perp_delta"]);
	pdf_signal_5D_angles_uniform->GetVariable("amp_20_delta")->setVal(truth_map["amp_20_delta"]);
	pdf_signal_5D_angles_uniform->GetVariable("amp_2para_delta")->setVal(truth_map["amp_2para_delta"]);
	pdf_signal_5D_angles_uniform->GetVariable("amp_2perp_delta")->setVal(truth_map["amp_2perp_delta"]);

	pdf_signal_5D_angles_uniform->GetVariable("phase_10_delta")->setVal(truth_map["phase_10_delta"]);
	pdf_signal_5D_angles_uniform->GetVariable("phase_1para_delta")->setVal(truth_map["phase_1para_delta"]);
	pdf_signal_5D_angles_uniform->GetVariable("phase_1perp_delta")->setVal(truth_map["phase_1perp_delta"]);
	pdf_signal_5D_angles_uniform->GetVariable("phase_20_delta")->setVal(truth_map["phase_20_delta"]);
	pdf_signal_5D_angles_uniform->GetVariable("phase_2para_delta")->setVal(truth_map["phase_2para_delta"]);
	pdf_signal_5D_angles_uniform->GetVariable("phase_2perp_delta")->setVal(truth_map["phase_2perp_delta"]);

	// draw original distribution
	PrintFitStartBlock("Plotting: 5D ORIG");
	for(RooRealVar *v : variables_4D) {
		CreateDataComponentPlots(data_signal_phsp,*v,opt->GetBins(),"full_range",opt->GetOutdir(),opt->GetPlotFormat(),"CP","flavor","_original");
		CreateDataComponentPlots(data_signal_phsp,*v,opt->GetBins(),"full_range",opt->GetOutdir(),opt->GetPlotFormat(),"CA","flavor","_original");
		CreateDataComponentPlots(data_signal_phsp,*v,opt->GetBins(),"full_range",opt->GetOutdir(),opt->GetPlotFormat(),"C","channel","_original");
	}

	PrintFitStartBlock("Computing Weights",2);
	RooFormulaVar reweight_func(weight.GetName(), weight.GetName(), "@0/@1", RooArgList(pdf_signal_5D_angles_uniform->GetPDF("C"),pdf_signal_5D_phsp->GetPDF("C")));;
	RooRealVar* reweight = dynamic_cast<RooRealVar*>(data_signal_phsp.addColumn(reweight_func));
	RooDataSet data_signal_reweighted("data_signal_reweighted", "data_signal_reweighted", &data_signal_phsp, *data_signal_phsp.get(), 0, reweight->GetName());
	data_signal_reweighted.Print();

	// draw reweighted distribution
	PrintFitStartBlock("Plotting: 5D REWEIGHTED");
	for(RooRealVar *v : variables_4D) {
		CreateDataComponentPlots(data_signal_reweighted,*v,opt->GetBins(),"full_range",opt->GetOutdir(),opt->GetPlotFormat(),"CP","flavor","_reweighted");
		CreateDataComponentPlots(data_signal_reweighted,*v,opt->GetBins(),"full_range",opt->GetOutdir(),opt->GetPlotFormat(),"CA","flavor","_reweighted");
		CreateDataComponentPlots(data_signal_reweighted,*v,opt->GetBins(),"full_range",opt->GetOutdir(),opt->GetPlotFormat(),"C","channel","_reweighted");
	}

	RooDataSet data("data","data",RooArgSet(varset,weight),Import(data_signal_reweighted), WeightVar(reweight->GetName()));
	data.append(data_bkg);
	data.append(data_bkg_peak_kkbar);
	//data.append(data_bkg_peak_masshyp);
	//data.append(data_bkg_peak_f0980);
	data.Print();

	truth_map["signal_nevents"] = data_signal_reweighted.sumEntries();
	truth_map["C_bkg_continuum_nevents"] = data_bkg.sumEntries();
	truth_map["C_bkg_bbar_wrongphikkbar_nevents"] = data_bkg_peak_kkbar.sumEntries();

	for(RooRealVar *v : variables_4D) {
		CreateDataComponentPlots(data,*v,opt->GetBins(),"full_range",opt->GetOutdir(),opt->GetPlotFormat(),"CP","flavor","_combined");
		CreateDataComponentPlots(data,*v,opt->GetBins(),"full_range",opt->GetOutdir(),opt->GetPlotFormat(),"CA","flavor","_combined");
		CreateDataComponentPlots(data,*v,opt->GetBins(),"full_range",opt->GetOutdir(),opt->GetPlotFormat(),"C","channel","_combined");
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
			hel_phi, hel_th1, hel_th2, flavor, category_name_flags_channel, false);
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
	RooFitResult *result_mass_kpi = pdf_bkg_continuum_1D_mass_kpi->GetSimPDF().fitTo(data_bkg_sideband, SumW2Error(true), NumCPU(opt->GetNumcpu()),
			Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()), Extended(false));
	pdf_bkg_continuum_1D_mass_kpi->GetSimPDF().getParameters(data_bkg_sideband)->Print("v");
	WriteCorrelationMatrixFile(result_mass_kpi,opt->GetOutdir()+"pdf_data_1D_mass_kpi_correlation.dat");
	pdf_bkg_continuum_1D_mass_kpi->ExportPDFValues(opt->GetOutdir()+"pdf_data_1D_mass_kpi.dat",true);
	result_mass_kpi->Print();
	// set mass_kpi distribution and fix it
	for(RooRealVar *v : pdf_bkg_continuum_1D_mass_kpi->GetVariables()) {
		pdf_bkg_continuum_9D->SetVariable(v->GetName(),v->getVal(),v->getError(),true,false);
	}

	RooFitResult *result_9D_sideband = pdf_bkg_continuum_9D->GetSimPDF().fitTo(data_bkg_sideband, SumW2Error(true), NumCPU(opt->GetNumcpu()),
			Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()), Extended(false));
	pdf_bkg_continuum_9D->GetSimPDF().getParameters(data_bkg_sideband)->Print("v");

	if(opt->GetCreatePlots()) {
		for(RooRealVar *v : variables_8D) {
			std::string varname = v->GetName();

			PrintFitStartBlock("PLOT PROJECTION");
			FFUtil::VisualizeFitResult(pdf_bkg_continuum_9D,data_bkg_sideband,*v,category_name_flags_channel,false,
					opt->GetBins(),"full_range", opt->GetOutdir()+"GSIM_MC_9D_sideband_"+varname+"_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
					opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
		}
		WriteCorrelationMatrixFile(result_9D_sideband,opt->GetOutdir()+"pdf_data_9D_sideband_correlation.dat");
		pdf_bkg_continuum_9D->ExportPDFValues(opt->GetOutdir()+"pdf_data_9D_sideband.dat",true);

		// draw projections in 5 mass_kpi regions
		for(unsigned int i = 1; i <= 5; ++i) {
			mbc.setRange(("mass_kpi_bin_"+mp::to_string(i)).c_str(),5.24,5.26);
			FFUtil::VisualizeFitResult(pdf_bkg_continuum_9D,data_bkg_sideband,hel_th1,category_name_flags_channel,false,
					opt->GetBins(),"mass_kpi_bin_"+mp::to_string(i), opt->GetOutdir()+"GSIM_MC_9D_sideband_hel_th1_projection_in_mass_kpi_bin_"+mp::to_string(i)+"."+opt->GetPlotFormat(),opt->GetPlotFormat(),
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

	PrintFitStartBlock("TOY-STUDY: 9D Fit");
	std::vector<RooFitResult*> fit_results;
	std::string outdir_files = opt->GetOutdir()+"/toystudy_"+mp::to_string(opt->GetNGsimToys())+(opt->GetMinos() ? "_minos" : "")+"/";
	mp::execute_command("mkdir -p "+outdir_files);
	// start loop of fits
	for(int i = 1; i <= opt->GetNGsimToys(); ++i) {
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

		RooFitResult* result_noCPV = pdf_data_9D->GetSimPDF().fitTo(data, SumW2Error(true), NumCPU(opt->GetNumcpu()),
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

		fit_results.push_back(pdf_data_9D->GetSimPDF().fitTo(data, SumW2Error(true), NumCPU(opt->GetNumcpu()),
				Timer(true), Save(true), InitialHesse(false), Minos(false), Extended(true),
				Constrain(RooArgSet(*pdf_data_9D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents")))));
		fit_results.back()->Print();

		// check if any phase is outside the [-pi,pi] region and push it back towards truth value
		// this is only to get good pulls, the other solution is not wrong
		ShiftPhaseToInterval(pdf_data_9D, "phase_10", truth_map["phase_10"]);
		ShiftPhaseToInterval(pdf_data_9D, "phase_1para", truth_map["phase_1para"]);
		ShiftPhaseToInterval(pdf_data_9D, "phase_1perp", truth_map["phase_1perp"]);
		ShiftPhaseToInterval(pdf_data_9D, "phase_20", truth_map["phase_20"]);
		ShiftPhaseToInterval(pdf_data_9D, "phase_2para", truth_map["phase_2para"]);
		ShiftPhaseToInterval(pdf_data_9D, "phase_2perp", truth_map["phase_2perp"]);

		PrintFitResultLatex(pdf_data_9D,truth_map);
		//PrintPhysicsParametersLatex(PropagateErrors(pdf_data_9D,pdf_signal_9D,fit_results.back(),opt,varset_9D,false));

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
	RooFitResult* redo_result_9D_best = pdf_data_9D->GetSimPDF().fitTo(data, SumW2Error(true), NumCPU(opt->GetNumcpu()),
			Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()), Extended(true),
			Constrain(RooArgSet(*pdf_data_9D->GetVariable("C_bkg_bbar_wrongphikkbar_nevents"))));
	redo_result_9D_best->Print();
	redo_result_9D_best->correlationMatrix().Print();
	redo_result_9D_best->covarianceMatrix().Print();
	WriteCorrelationMatrixFile(redo_result_9D_best,opt->GetOutdir()+"pdf_data_9D_correlation.dat");
	WriteCovarianceMatrixFile(redo_result_9D_best,opt->GetOutdir()+"pdf_data_9D_covariance.dat");

	// check if any phase is outside the [-pi,pi] region and push it back towards truth value
	// this is only to get good pulls, the other solution is not wrong
	ShiftPhaseToInterval(pdf_data_9D, "phase_10", truth_map["phase_10"]);
	ShiftPhaseToInterval(pdf_data_9D, "phase_1para", truth_map["phase_1para"]);
	ShiftPhaseToInterval(pdf_data_9D, "phase_1perp", truth_map["phase_1perp"]);
	ShiftPhaseToInterval(pdf_data_9D, "phase_20", truth_map["phase_20"]);
	ShiftPhaseToInterval(pdf_data_9D, "phase_2para", truth_map["phase_2para"]);
	ShiftPhaseToInterval(pdf_data_9D, "phase_2perp", truth_map["phase_2perp"]);

	PrintFitResultLatex(pdf_data_9D,truth_map);
	PrintPhysicsParametersLatex(PropagateErrors(pdf_data_9D,pdf_signal_9D,redo_result_9D_best,opt,varset_9D,false));
	pdf_data_9D->ExportPDFValues(opt->GetOutdir()+"pdf_data_9D.dat",true);

	// store best fit result
	std::ofstream fout((outdir_files+"TOY_9D_best_fit_result.dat").c_str());
	if(fout.is_open()) {
		for(const std::string& p : parameters) {
			fout << p << ";";
			fout << truth_map[p] << ";";
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
					opt->GetBins(),"full_range", opt->GetOutdir()+"GSIM_MC_9D_"+varname+"_projection."+opt->GetPlotFormat(),opt->GetPlotFormat(),
					opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
			if(varname == "mbc" || varname == "deltae" || varname == "chil0_ma" || varname == "nb_prime") {
				FFUtil::VisualizeFitResult(pdf_data_9D,data,*v,category_name_flags_channel,true,
						opt->GetBins(),("signal_box_"+varname).c_str(),opt->GetOutdir()+"GSIM_MC_9D_"+varname+"_projection_signal_box."+opt->GetPlotFormat(),opt->GetPlotFormat(),
						opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
			} else {
				FFUtil::VisualizeFitResult(pdf_data_9D,data,*v,category_name_flags_channel,true,
						opt->GetBins(),"signal_box",opt->GetOutdir()+"GSIM_MC_9D_"+varname+"_projection_signal_box."+opt->GetPlotFormat(),opt->GetPlotFormat(),
						opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
			}
			if(varname != "mbc") {
				FFUtil::VisualizeFitResult(pdf_data_9D,data,*v,category_name_flags_channel,true,
						opt->GetBins(),"mbc_sideband", opt->GetOutdir()+"GSIM_MC_9D_"+varname+"_projection_mbc_sideband."+opt->GetPlotFormat(),opt->GetPlotFormat(),
						opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
			}
		}
	}
	redo_result_9D_best->Print();

	std::cout << "Signal-Yield/Signal-YieldError = " << pdf_data_9D->GetVariable("signal_nevents")->getVal()/pdf_data_9D->GetVariable("signal_nevents")->getError() << std::endl;

//	mass_helicity_angles_norm_cache_integrator->SaveIntegrals(path_to_integrals);
//	mass_helicity_angles_real_cache_integrator->SaveIntegrals(path_to_integrals);
//	mass_helicity_angles_imag_cache_integrator->SaveIntegrals(path_to_integrals);
//	mass_helicity_angles_norm_cache_integrator_phi_th1_th2->SaveIntegrals(path_to_integrals);
//	mass_helicity_angles_real_cache_integrator_phi_th1_th2->SaveIntegrals(path_to_integrals);
//	mass_helicity_angles_imag_cache_integrator_phi_th1_th2->SaveIntegrals(path_to_integrals);
//	mass_helicity_angles_norm_cache_integrator_m_th1_th2->SaveIntegrals(path_to_integrals);
//	mass_helicity_angles_real_cache_integrator_m_th1_th2->SaveIntegrals(path_to_integrals);
//	mass_helicity_angles_imag_cache_integrator_m_th1_th2->SaveIntegrals(path_to_integrals);
//	mass_helicity_angles_norm_cache_integrator_m_phi_th2->SaveIntegrals(path_to_integrals);
//	mass_helicity_angles_real_cache_integrator_m_phi_th2->SaveIntegrals(path_to_integrals);
//	mass_helicity_angles_imag_cache_integrator_m_phi_th2->SaveIntegrals(path_to_integrals);
//	mass_helicity_angles_norm_cache_integrator_m_phi_th1->SaveIntegrals(path_to_integrals);
//	mass_helicity_angles_real_cache_integrator_m_phi_th1->SaveIntegrals(path_to_integrals);
//	mass_helicity_angles_imag_cache_integrator_m_phi_th1->SaveIntegrals(path_to_integrals);

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
	delete result_mass_kpi;
}

}
