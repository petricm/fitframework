/**
 * @file	FFUtil.h
 * @date	May 9, 2011
 * @author	petric
 * @brief	brief description
 *
 * long description
 */

#ifndef FFUTIL_H_
#define FFUTIL_H_

#include "FitFramework.h"
#include "FF_Opt.h"

class FF_PDF;

namespace FFUtil {

typedef std::tuple<std::string, Color_t, Style_t, std::string> tuple_pdfname_color_style_label;

typedef std::tuple<RooCategory*, std::string, std::vector<std::string> > tuple_category_name_flags;

std::string GetLabelToTLatex(const std::string& label);

std::string GetLabelToLatex(const std::string& label);

std::vector<TH1*> AddHistogram_MCType(RooPlot *plot_on, RooDataSet &data, RooRealVar &variable, int opt_bins, std::string range, std::string category_cut = "");

std::vector<TH1*> AddHistogram_MCInfo(RooPlot *plot_on, RooDataSet &data, RooRealVar &variable, int opt_bins, std::string range, std::string category_cut = "");

std::vector<TH1*> AddHistogram_MCInfoType(RooPlot *plot_on, RooDataSet &data, RooRealVar &variable, int opt_bins, std::string range, std::string category_cut = "", bool add_entries_to_legend = false);

void CreateDataComponentPlots(RooDataSet &data, RooRealVar &variable, int opt_bins, std::string range, std::string opt_outdir, std::string opt_plot_format,
		std::string cat_label = "", std::string cat_name = "", std::string suffix = "");

void WriteCorrelationMatrixFile(RooFitResult *result, std::string filename);

void WriteCovarianceMatrixFile(RooFitResult *result, std::string filename);

void WriteCorrelationMatrixFile(RooDataSet &data, RooArgList arglist, std::string filename);

void WriteCorrelationStudyFile(RooDataSet &data, RooArgList arglist, std::string filename);

void PrintFitStartBlock(const std::string& label, unsigned int dashed_lines = 3);

void VisualizeNLLCut(shared_ptr<FF_PDF> pdf, RooDataSet &data, RooRealVar &variable, const tuple_category_name_flags& flags,
		const std::string& range, const std::string& filename, const std::string& opt_plot_format, double nll_quantile_signal);

void VisualizeNLLCut(RooSimultaneous &pdf, RooDataSet &data, RooRealVar &variable,
		std::string cat_name, std::string cat_label, std::string range, std::string filename, double nll_quantile_signal);

void VisualizeLLRCut(shared_ptr<FF_PDF> pdf_component, shared_ptr<FF_PDF> pdf_tot, RooDataSet &data, RooRealVar &variable, const tuple_category_name_flags& flags,
		const std::string& range, const std::string& filename, const std::string& opt_plot_format, double llr_quantile_signal);

void VisualizeLLRCut(RooSimultaneous &pdf_component, RooSimultaneous &pdf_tot, RooDataSet &data, RooRealVar &variable,
		std::string cat_name, std::string cat_label, std::string range, std::string filename, double llr_quantile_signal);

void VisualizeFitResultNLL(shared_ptr<FF_PDF> pdf, shared_ptr<FF_PDF> pdf_component, RooDataSet &data, RooRealVar &variable,
		const tuple_category_name_flags& flags,	bool add_all_components,
		int opt_bins, const std::string& range, const std::string& filename, const std::string& opt_plot_format, const std::vector<double>& nll_cut,
		bool opt_pull, bool opt_logy, bool opt_mctype, bool opt_mcinfo, bool opt_numcpu);

void VisualizeFitResultNLL(RooSimultaneous &pdf, RooSimultaneous &pdf_component, RooDataSet &data, RooRealVar &variable,
		std::vector< tuple_pdfname_color_style_label > &pdf_style, RooCategory &category, std::string cat_name, std::string cat_label,
		int opt_bins, std::string range, std::string filename, double nll_cut,
		bool opt_pull, bool opt_logy, bool opt_mctype, bool opt_mcinfo, bool opt_numcpu);

void VisualizeFitResultLLR(shared_ptr<FF_PDF> pdf, shared_ptr<FF_PDF> pdf_component, RooDataSet &data, RooRealVar &variable,
		const tuple_category_name_flags& flags,	bool add_all_components,
		int opt_bins, const std::string& range, const std::string& filename, const std::string& opt_plot_format, const std::vector<double>& llr_cut,
		bool opt_pull, bool opt_logy, bool opt_mctype, bool opt_mcinfo, bool opt_numcpu);

void VisualizeFitResultLLR(RooSimultaneous &pdf, RooSimultaneous &pdf_component, RooDataSet &data, RooRealVar &variable,
		std::vector< tuple_pdfname_color_style_label > &pdf_style, RooCategory &category, std::string cat_name, std::string cat_label,
		int opt_bins, std::string range, std::string filename, double llr_cut,
		bool opt_pull, bool opt_logy, bool opt_mctype, bool opt_mcinfo, bool opt_numcpu);

void VisualizeFitResult(shared_ptr<FF_PDF> pdf, RooDataSet &data, RooRealVar &variable,
		const tuple_category_name_flags& flags, bool add_all_components,
		int opt_bins, const std::string& range, const std::string& filename, const std::string& opt_plot_format,
		bool opt_pull=false, bool opt_logy=false, bool opt_mctype=false, bool opt_mcinfo=false, bool opt_use_fit_error_for_pull=false, bool opt_pdf_legend=false);

void VisualizeFitResultProjWData(shared_ptr<FF_PDF> pdf, RooDataSet &data, RooRealVar &variable,
		const tuple_category_name_flags& flags, bool add_all_components,
		int opt_bins, const std::string& range, const std::string& filename, const std::string& opt_plot_format,
		const tuple_category_name_flags& flags_flavor, bool opt_plot_pdf=false,
		bool opt_pull=false, bool opt_logy=false, bool opt_mctype=false,
		bool opt_mcinfo=false, bool opt_use_fit_error_for_pull=false,
		bool opt_pdf_legend=false);

void VisualizeFitResult(RooAbsPdf &pdf, RooDataSet &data, RooRealVar &variable,
		const tuple_pdfname_color_style_label &pdf_style, int opt_bins, std::string range, std::string filename, std::string plotlabel = "",
		bool opt_pull=false, bool opt_logy=false, bool opt_mctype=false, bool opt_mcinfo=false, bool opt_use_fit_error_for_pull=false, bool opt_pdf_legend=false);

void VisualizeFitResultProjWData(RooAbsPdf &pdf, RooDataSet &data, RooRealVar &variable,
		const tuple_pdfname_color_style_label &pdf_style,
		RooCategory& flavor, const std::string& flavor_cut, const std::string& flavor_label,
		int opt_bins, std::string range, std::string filename, std::string plotlabel = "",
		bool opt_pull=false, bool opt_logy=false, bool opt_mctype=false, bool opt_mcinfo=false,
		bool opt_use_fit_error_for_pull=false, bool opt_pdf_legend=false, bool opt_plot_pdf=false);

void VisualizeFitResult(RooAbsPdf &pdf, RooDataSet &data, RooRealVar &variable,
		std::vector< tuple_pdfname_color_style_label > &pdf_style, int opt_bins, std::string range, std::string filename, std::string plotlabel = "",
		bool opt_pull=false, bool opt_logy=false, bool opt_mctype=false, bool opt_mcinfo=false, bool opt_use_fit_error_for_pull=false, bool opt_pdf_legend=false);

void VisualizeFitResultProjWData(RooAbsPdf &pdf, RooDataSet &data, RooRealVar &variable,
		std::vector< tuple_pdfname_color_style_label > &pdf_style,
		RooCategory& flavor, const std::string& flavor_cut, const std::string& flavor_label,
		int opt_bins, std::string range, std::string filename, std::string plotlabel = "",
		bool opt_pull=false, bool opt_logy=false, bool opt_mctype=false, bool opt_mcinfo=false,
		bool opt_use_fit_error_for_pull=false, bool opt_pdf_legend=false, bool opt_plot_pdf=false);

void SetFitRanges(RooRealVar &mbc, RooRealVar &deltae, RooRealVar &chil0_ma, RooRealVar &mass_kpi,
		RooRealVar &hel_phi, RooRealVar &hel_th1, RooRealVar &hel_th2, RooRealVar &nb_prime);

double binomialError(const double all, const double accepted);

void calcAndFitEfficiency(RooDataSet &data, RooRealVar &var, RooAbsReal &effFunc,
		RooCategory &cut, const std::string &opt_outdir, const std::string& opt_plot_format, bool opt_numcpu, int opt_bins, const std::string &flag, bool visualize_error = false);
void calcAndFitEfficiency2D(RooDataSet &data, RooRealVar &var1, RooRealVar &var2, RooAbsReal &effFunc,
                              RooCategory &cut, const std::string &opt_outdir, const std::string& opt_plot_format, bool opt_numcpu, int opt_bins, const std::string &flag, bool visualize_error = false);
    

void PrintFitResultLatex(shared_ptr<FF_PDF> pdf, std::map<std::string, double> truth);

void PrintFitResultLatex(shared_ptr<FF_PDF> pdf);

void ShiftPhaseToInterval(shared_ptr<FF_PDF> pdf, const std::string& varname, double truth);

std::map<std::string,double> ComputePhysicsParameters(shared_ptr<FF_PDF> pdf, shared_ptr<FF_PDF> signal_pdf,
		const RooArgSet& varset, bool verbose = false);

void PrintPhysicsParametersLatex(std::map<std::string, std::pair<double,std::pair<double,double> > > physics_params);

std::map<std::string, std::pair<double,std::pair<double, double> > > PropagateErrors(shared_ptr<FF_PDF> pdf, shared_ptr<FF_PDF> signal_pdf, RooFitResult* result, shared_ptr<FFOpt> opt,
		const RooArgSet& varset, bool verbose = false);

std::map<std::string,double> ComputeEffectiveWidth(shared_ptr<FF_PDF> pdf);

std::map<std::string, std::pair<double,double> > PropagateWidthError(shared_ptr<FF_PDF> pdf, RooFitResult* result, shared_ptr<FFOpt> opt, bool verbose = false);

std::string GetAcceptanceString(ACCEPTANCE_FUNCTION value);

std::string GetAcceptanceString(EFFICIENCY_FUNCTION value);    
    
std::string FilenameHash(const std::vector<std::string> &filenames);

void CreateFitResultsComparisonPlot(shared_ptr<FFOpt> opt);

}

#endif /* FFUTIL_H_ */
