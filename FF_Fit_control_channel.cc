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
#include "RooBinningCategory.h"

using namespace FFUtil;

namespace FFFits {

void FitControlChannel(shared_ptr<TChain> chain_data, shared_ptr<TChain> chain_gsim, shared_ptr<FFOpt> opt) {
	if(chain_data->GetEntries() == 0 || chain_gsim->GetEntries() == 0) {
		std::cout << "No data for control channel fits given, skip fitting." << std::endl;
		return;
	}

	// ---------------------------------------------
	// ----- Define variables
	// ---------------------------------------------
	PrintFitStartBlock("STATUS: Define functions and variables...");
	RooRealVar mbc("mbc","m_{bc}",5.26,5.29,"GeV");
	mbc.setBins(opt->GetBins());
	RooRealVar deltae("deltae","#Delta E",-0.1,0.1,"GeV");
	deltae.setBins(opt->GetBins());
	RooRealVar chil0_ma("chil0_ma","m_{KK}",3.085,3.11,"GeV");
	chil0_ma.setBins(opt->GetBins());
	RooRealVar mass_kpi("mass_kpi","m_{K#pi}",0.85,0.95,"GeV");
	mass_kpi.setBins(opt->GetBins());
	RooRealVar decay("decay","decay",0,2,"");
	RooRealVar mcinfo("mcinfo","mcinfo",0,256,"");
	// necessary for MC information checks
	// create data set (constructor only overload for max. 9 arguments, so we add some afterwards)
	RooArgSet varset(mbc,deltae,chil0_ma,mass_kpi,decay,mcinfo);


	RooDataSet data("data","data",varset,Import(*chain_data.get()),Cut("mbc < 5.289"));
	RooDataSet gsim("gsim","gsim",varset,Import(*chain_gsim.get()),Cut("mbc < 5.289"));
	RooDataSet gsim_sig("gsim_sig","gsim_sig",varset,Import(*chain_gsim.get()),Cut("mbc < 5.289 && (mcinfo==59 || mcinfo==61)"));

	data.Print();
	gsim.Print();
	gsim_sig.Print();

	decay.setBins(1,"CC");
	RooBinningCategory bins("channel","channel",decay,"CC");
	RooCategory* channel_data = (RooCategory*) data.addColumn(bins);
	RooCategory* channel_gsim = (RooCategory*) gsim.addColumn(bins);
	//channel_data->defineType("CC",0);
	//channel_gsim->defineType("CC",0);
	std::vector<std::string> flags;
	flags.push_back("CC");
	tuple_category_name_flags category_name_flags_data = std::make_tuple(channel_data,"channel",flags);
	tuple_category_name_flags category_name_flags_gsim = std::make_tuple(channel_gsim,"channel",flags);

	data.Print();
	gsim.Print();

	RooDataHist data_hist("data_hist","data_hist",RooArgSet(mbc,deltae),data);
	RooDataHist gsim_hist("gsim_hist","gsim_hist",RooArgSet(mbc,deltae),gsim);
	RooDataHist gsim_hist_sig("gsim_hist_sig","gsim_hist_sig",RooArgSet(mbc,deltae),gsim_sig);

	data_hist.Print();
	gsim_hist.Print();
	gsim_hist_sig.Print();

	mbc.setRange("full_range",5.26,5.29);
	deltae.setRange("full_range",-0.1,0.1);
	chil0_ma.setRange("full_range",3.085,3.11);
	mass_kpi.setRange("full_range",0.85,0.95);

	// ---------------------------------------------
	// ----- Do fits
	// ---------------------------------------------
	// See RooFit tutorial macro #501 about HowTo do a simultaneous fit
	// See RooFit tutorial macro #603 about HowTo do a multi-core parallelized unbinned maximum likelihood fit
	std::cout << "STATUS: Do fits..." << std::endl;

	std::map<std::string, std::pair<double,double> > resolution;

	PrintFitStartBlock("GSIM");
	bool do_gsim = true;
	if(do_gsim){
		PrintFitStartBlock("FITTING: mbc");
		shared_ptr<FF_PDF> pdf_cc_1D_mbc_sig = make_shared<FF_PDF_signal_1D_mbc>(mbc, category_name_flags_gsim);
		pdf_cc_1D_mbc_sig->GetVariable("C_mbc_signal_width_two")->setConstant(true);
		pdf_cc_1D_mbc_sig->GetVariable("C_mbc_signal_one_frac")->setVal(1.0);
		shared_ptr<FF_PDF> pdf_cc_1D_mbc_bkg = make_shared<FF_PDF_bkg_continuum_1D_mbc>(mbc, category_name_flags_gsim);
		pdf_cc_1D_mbc_bkg->GetVariable("mbc_bkg_continuum_c")->setMin(-200.0);
		shared_ptr<FF_PDF> pdf_cc_1D_mbc = make_shared<FF_PDF_data_1D_mbc>(pdf_cc_1D_mbc_sig, pdf_cc_1D_mbc_bkg, category_name_flags_gsim);
		RooFitResult *result_mbc = pdf_cc_1D_mbc->GetPDF("CC").fitTo(gsim_hist, NumCPU(opt->GetNumcpu()),
				Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()));
		pdf_cc_1D_mbc->GetPDF("CC").getParameters(gsim)->Print("v");
		std::vector<tuple_pdfname_color_style_label> styles;
		styles.push_back(pdf_cc_1D_mbc->GetPDFStyle("CC"));
		styles.push_back(pdf_cc_1D_mbc_sig->GetPDFStyle("CC"));
		styles.push_back(pdf_cc_1D_mbc_bkg->GetPDFStyle("CC"));
		FFUtil::VisualizeFitResult(pdf_cc_1D_mbc->GetPDF("CC"),gsim,mbc,styles,
				opt->GetBins(),"full_range", opt->GetOutdir()+"CC_GSIM_1D_mbc_fit."+opt->GetPlotFormat(),"B^{0}/#bar{B^{0}} #rightarrow J/#Psi K^{#pm} #pi^{#mp}",
				opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
		WriteCorrelationMatrixFile(result_mbc,opt->GetOutdir()+"pdf_cc_gsim_1D_mbc_correlation.dat");
		pdf_cc_1D_mbc->ExportPDFValues(opt->GetOutdir()+"pdf_cc_gsim_1D_mbc.dat",true);
		result_mbc->Print();
		delete result_mbc;

		PrintFitStartBlock("FITTING: deltae | mbc");
		shared_ptr<FF_PDF> pdf_cc_2D_mbc_deltae_sig =
				make_shared<FF_PDF_signal_2D_mbc_deltae>(mbc, deltae, pdf_cc_1D_mbc_sig, category_name_flags_gsim);
		//pdf_cc_2D_mbc_deltae_sig->GetVariable("C_deltae_signal_core_frac")->setVal(1.0);
		//pdf_cc_2D_mbc_deltae_sig->GetVariable("C_deltae_signal_core_frac")->setConstant(false);
		pdf_cc_2D_mbc_deltae_sig->GetVariable("C_deltae_signal_1st_frac")->setVal(1.0);
		pdf_cc_2D_mbc_deltae_sig->GetVariable("C_deltae_signal_1st_frac")->setConstant(false);
		pdf_cc_2D_mbc_deltae_sig->GetVariable("C_deltae_signal_width_1st_left")->setMax(0.1);
		pdf_cc_2D_mbc_deltae_sig->GetVariable("C_deltae_signal_width_1st_right")->setMax(0.1);
		pdf_cc_2D_mbc_deltae_sig->GetVariable("C_deltae_signal_width_1st_left")->setConstant(false);
		pdf_cc_2D_mbc_deltae_sig->GetVariable("C_deltae_signal_width_1st_right")->setConstant(false);
		pdf_cc_2D_mbc_deltae_sig->GetVariable("C_deltae_signal_width_2nd_left")->setMax(0.2);
		pdf_cc_2D_mbc_deltae_sig->GetVariable("C_deltae_signal_width_2nd_left")->setConstant(false);
		pdf_cc_2D_mbc_deltae_sig->GetVariable("C_deltae_signal_width_2nd_right")->setConstant(false);
		shared_ptr<FF_PDF> pdf_cc_2D_mbc_deltae_bkg =
				make_shared<FF_PDF_bkg_continuum_2D_mbc_deltae>(mbc, deltae, category_name_flags_gsim);
		pdf_cc_2D_mbc_deltae_bkg->GetVariable("mbc_bkg_continuum_c")->setMin(-200.0);
		pdf_cc_2D_mbc_deltae_bkg->GetVariable("mbc_bkg_continuum_c")->setVal(pdf_cc_1D_mbc->GetVariable("mbc_bkg_continuum_c")->getVal());
		pdf_cc_2D_mbc_deltae_bkg->GetVariable("mbc_bkg_continuum_c")->setConstant(true);
		shared_ptr<FF_PDF> pdf_cc_2D_mbc_deltae =
				make_shared<FF_PDF_data_2D_mbc_deltae>(pdf_cc_2D_mbc_deltae_sig, pdf_cc_2D_mbc_deltae_bkg, category_name_flags_gsim);
		pdf_cc_2D_mbc_deltae->GetVariable("CC_sig_frac")->setVal(pdf_cc_1D_mbc->GetVariable("CC_sig_frac")->getVal());
		pdf_cc_2D_mbc_deltae->GetVariable("CC_sig_frac")->setConstant(true);
		RooFitResult *result_mbc_deltae = pdf_cc_2D_mbc_deltae->GetPDF("CC").fitTo(gsim_hist, NumCPU(opt->GetNumcpu()),
				Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()));
		pdf_cc_2D_mbc_deltae->GetPDF("CC").getParameters(gsim)->Print("v");
		styles.clear();
		styles.push_back(pdf_cc_2D_mbc_deltae->GetPDFStyle("CC"));
		styles.push_back(pdf_cc_2D_mbc_deltae_sig->GetPDFStyle("CC"));
		styles.push_back(pdf_cc_2D_mbc_deltae_bkg->GetPDFStyle("CC"));
		FFUtil::VisualizeFitResult(pdf_cc_2D_mbc_deltae->GetPDF("CC"),gsim,mbc,styles,
				opt->GetBins(),"full_range", opt->GetOutdir()+"CC_GSIM_2D_mbc_deltae_fit_mbc_projection."+opt->GetPlotFormat(),"B^{0}/#bar{B^{0}} #rightarrow J/#Psi K^{#pm} #pi^{#mp}",
				opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
		FFUtil::VisualizeFitResult(pdf_cc_2D_mbc_deltae->GetPDF("CC"),gsim,deltae,styles,
				opt->GetBins(),"full_range", opt->GetOutdir()+"CC_GSIM_2D_mbc_deltae_fit_deltae_projection."+opt->GetPlotFormat(),"B^{0}/#bar{B^{0}} #rightarrow J/#Psi K^{#pm} #pi^{#mp}",
				opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
		WriteCorrelationMatrixFile(result_mbc_deltae,opt->GetOutdir()+"pdf_cc_gsim_2D_mbc_deltae_correlation.dat");
		pdf_cc_2D_mbc_deltae->ExportPDFValues(opt->GetOutdir()+"pdf_cc_gsim_2D_mbc_deltae.dat",true);
		result_mbc_deltae->Print();
		resolution["width_gsim"] = FFUtil::PropagateWidthError(pdf_cc_2D_mbc_deltae,result_mbc_deltae,opt)["width_eff"];
		delete result_mbc_deltae;

		TCanvas* canvas = new TCanvas("canvas","canvas",1280,1024);
		TH2F* plot_2D_pdf = mbc.createHistogram("plot_2D_pdf",deltae,"Entries");
		pdf_cc_2D_mbc_deltae_sig->GetPDF("CC").fillHistogram(plot_2D_pdf,RooArgList(mbc,deltae));
		plot_2D_pdf->Draw("COL");
		canvas->SaveAs((opt->GetOutdir()+"CC_GSIM_2D_mbc_deltae_fit_2D_COL."+opt->GetPlotFormat()).c_str());
		delete plot_2D_pdf;
		delete canvas;

		canvas = new TCanvas("canvas","canvas",1280,1024);
		plot_2D_pdf = mbc.createHistogram("plot_2D_pdf",deltae,"Entries");
		gsim_hist_sig.fillHistogram(plot_2D_pdf,RooArgList(mbc,deltae));
		plot_2D_pdf->Draw("COL");
		canvas->SaveAs((opt->GetOutdir()+"CC_GSIM_2D_mbc_deltae_fit_2D_COL_SIGNAL."+opt->GetPlotFormat()).c_str());
		delete plot_2D_pdf;
		delete canvas;
	}

	PrintFitStartBlock("DATA");
	bool do_data = true;
	if(do_data){
		PrintFitStartBlock("FITTING: mbc");
		shared_ptr<FF_PDF> pdf_cc_1D_mbc_sig = make_shared<FF_PDF_signal_1D_mbc>(mbc, category_name_flags_data);
		pdf_cc_1D_mbc_sig->GetVariable("C_mbc_signal_width_two")->setConstant(true);
		pdf_cc_1D_mbc_sig->GetVariable("C_mbc_signal_one_frac")->setVal(1.0);
		shared_ptr<FF_PDF> pdf_cc_1D_mbc_bkg = make_shared<FF_PDF_bkg_continuum_1D_mbc>(mbc, category_name_flags_data);
		pdf_cc_1D_mbc_bkg->GetVariable("mbc_bkg_continuum_c")->setMin(-200.0);
		shared_ptr<FF_PDF> pdf_cc_1D_mbc = make_shared<FF_PDF_data_1D_mbc>(pdf_cc_1D_mbc_sig, pdf_cc_1D_mbc_bkg, category_name_flags_data);
		RooFitResult *result_mbc = pdf_cc_1D_mbc->GetPDF("CC").fitTo(data_hist, NumCPU(opt->GetNumcpu()),
				Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()));
		pdf_cc_1D_mbc->GetPDF("CC").getParameters(data)->Print("v");
		std::vector<tuple_pdfname_color_style_label> styles;
		styles.push_back(pdf_cc_1D_mbc->GetPDFStyle("CC"));
		styles.push_back(pdf_cc_1D_mbc_sig->GetPDFStyle("CC"));
		styles.push_back(pdf_cc_1D_mbc_bkg->GetPDFStyle("CC"));
		FFUtil::VisualizeFitResult(pdf_cc_1D_mbc->GetPDF("CC"),data,mbc,styles,
				opt->GetBins(),"full_range", opt->GetOutdir()+"CC_1D_mbc_fit."+opt->GetPlotFormat(),"B^{0}/#bar{B^{0}} #rightarrow J/#Psi K^{#pm} #pi^{#mp}",
				opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
		WriteCorrelationMatrixFile(result_mbc,opt->GetOutdir()+"pdf_cc_1D_mbc_correlation.dat");
		pdf_cc_1D_mbc->ExportPDFValues(opt->GetOutdir()+"pdf_cc_1D_mbc.dat",true);
		result_mbc->Print();
		delete result_mbc;

		PrintFitStartBlock("FITTING: deltae | mbc");
		shared_ptr<FF_PDF> pdf_cc_2D_mbc_deltae_sig =
				make_shared<FF_PDF_signal_2D_mbc_deltae>(mbc, deltae, pdf_cc_1D_mbc_sig, category_name_flags_data);
		//pdf_cc_2D_mbc_deltae_sig->GetVariable("C_deltae_signal_core_frac")->setVal(1.0);
		//pdf_cc_2D_mbc_deltae_sig->GetVariable("C_deltae_signal_core_frac")->setConstant(false);
		pdf_cc_2D_mbc_deltae_sig->GetVariable("C_deltae_signal_1st_frac")->setVal(1.0);
		pdf_cc_2D_mbc_deltae_sig->GetVariable("C_deltae_signal_1st_frac")->setConstant(false);
		pdf_cc_2D_mbc_deltae_sig->GetVariable("C_deltae_signal_width_1st_left")->setMax(0.1);
		pdf_cc_2D_mbc_deltae_sig->GetVariable("C_deltae_signal_width_1st_right")->setMax(0.1);
		pdf_cc_2D_mbc_deltae_sig->GetVariable("C_deltae_signal_width_1st_left")->setConstant(false);
		pdf_cc_2D_mbc_deltae_sig->GetVariable("C_deltae_signal_width_1st_right")->setConstant(false);
		pdf_cc_2D_mbc_deltae_sig->GetVariable("C_deltae_signal_width_2nd_left")->setMax(0.2);
		pdf_cc_2D_mbc_deltae_sig->GetVariable("C_deltae_signal_width_2nd_left")->setConstant(false);
		pdf_cc_2D_mbc_deltae_sig->GetVariable("C_deltae_signal_width_2nd_right")->setConstant(false);
		shared_ptr<FF_PDF> pdf_cc_2D_mbc_deltae_bkg =
				make_shared<FF_PDF_bkg_continuum_2D_mbc_deltae>(mbc, deltae, category_name_flags_data);
		pdf_cc_2D_mbc_deltae_bkg->GetVariable("mbc_bkg_continuum_c")->setMin(-200.0);
		pdf_cc_2D_mbc_deltae_bkg->GetVariable("mbc_bkg_continuum_c")->setVal(pdf_cc_1D_mbc->GetVariable("mbc_bkg_continuum_c")->getVal());
		pdf_cc_2D_mbc_deltae_bkg->GetVariable("mbc_bkg_continuum_c")->setConstant(true);
		shared_ptr<FF_PDF> pdf_cc_2D_mbc_deltae =
				make_shared<FF_PDF_data_2D_mbc_deltae>(pdf_cc_2D_mbc_deltae_sig, pdf_cc_2D_mbc_deltae_bkg, category_name_flags_data);
		pdf_cc_2D_mbc_deltae->GetVariable("CC_sig_frac")->setVal(pdf_cc_1D_mbc->GetVariable("CC_sig_frac")->getVal());
		pdf_cc_2D_mbc_deltae->GetVariable("CC_sig_frac")->setConstant(true);
		RooFitResult *result_mbc_deltae = pdf_cc_2D_mbc_deltae->GetPDF("CC").fitTo(data_hist, NumCPU(opt->GetNumcpu()),
				Timer(true), Save(true), InitialHesse(false), Minos(opt->GetMinos()));
		pdf_cc_2D_mbc_deltae->GetPDF("CC").getParameters(data)->Print("v");
		styles.clear();
		styles.push_back(pdf_cc_2D_mbc_deltae->GetPDFStyle("CC"));
		styles.push_back(pdf_cc_2D_mbc_deltae_sig->GetPDFStyle("CC"));
		styles.push_back(pdf_cc_2D_mbc_deltae_bkg->GetPDFStyle("CC"));
		FFUtil::VisualizeFitResult(pdf_cc_2D_mbc_deltae->GetPDF("CC"),data,mbc,styles,
				opt->GetBins(),"full_range", opt->GetOutdir()+"CC_2D_mbc_deltae_fit_mbc_projection."+opt->GetPlotFormat(),"B^{0}/#bar{B^{0}} #rightarrow J/#Psi K^{#pm} #pi^{#mp}",
				opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
		FFUtil::VisualizeFitResult(pdf_cc_2D_mbc_deltae->GetPDF("CC"),data,deltae,styles,
				opt->GetBins(),"full_range", opt->GetOutdir()+"CC_2D_mbc_deltae_fit_deltae_projection."+opt->GetPlotFormat(),"B^{0}/#bar{B^{0}} #rightarrow J/#Psi K^{#pm} #pi^{#mp}",
				opt->GetPull(),opt->GetLogy(),opt->GetMctype(),opt->GetMcinfo(),opt->GetPullFiterror(),opt->GetPdfLegend());
		WriteCorrelationMatrixFile(result_mbc_deltae,opt->GetOutdir()+"pdf_cc_2D_mbc_deltae_correlation.dat");
		pdf_cc_2D_mbc_deltae->ExportPDFValues(opt->GetOutdir()+"pdf_cc_2D_mbc_deltae.dat",true);
		result_mbc_deltae->Print();
		resolution["width_data"] = FFUtil::PropagateWidthError(pdf_cc_2D_mbc_deltae,result_mbc_deltae,opt)["width_eff"];
		delete result_mbc_deltae;

		TCanvas* canvas = new TCanvas("canvas","canvas",1280,1024);
		TH2F* plot_2D_pdf = mbc.createHistogram("plot_2D_pdf",deltae,"Entries");
		pdf_cc_2D_mbc_deltae_sig->GetPDF("CC").fillHistogram(plot_2D_pdf,RooArgList(mbc,deltae));
		plot_2D_pdf->Draw("COL");
		canvas->SaveAs((opt->GetOutdir()+"CC_2D_mbc_deltae_fit_2D_COL."+opt->GetPlotFormat()).c_str());
		delete plot_2D_pdf;
		delete canvas;
	}

	std::cout << "Resolution GSIM: " << resolution["width_gsim"].first << " +- " << resolution["width_gsim"].second << std::endl;
	std::cout << "Resolution DATA: " << resolution["width_data"].first << " +- " << resolution["width_data"].second << std::endl;
	double scale = resolution["width_data"].first/resolution["width_gsim"].first;
	double error_scale = std::sqrt(std::pow(resolution["width_data"].second/resolution["width_data"].first,2) + std::pow(resolution["width_gsim"].second/resolution["width_gsim"].first,2));
	std::cout << "Scale factor for width on data: " << scale << " +- " << error_scale*scale << std::endl;
}

}
