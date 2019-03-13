/**
 * @file	FFOpt.cc
 * @date	May 9, 2011
 * @author	petric
 * @brief	brief description
 *
 * long description
 */

#include <iostream>

#include "FF_Opt.h"
// boost library
#include <boost/program_options.hpp>
namespace po = boost::program_options;
// Utilities
#include "mp_utility.h"

int FFOpt::GetBins() const {
    return m_bins;
}

std::vector<std::string> FFOpt::GetFilenamesMC() const {
    return m_filenames_MC;
}

std::vector<std::string> FFOpt::GetFilenamesData() const {
    return m_filenames_data;
}

std::vector<std::string> FFOpt::GetFilenamesSignalMC() const {
    return m_filenames_signal_MC;
}

std::vector<std::string> FFOpt::GetFilenamesContinuumMC() const {
    return m_filenames_continuum;
}

std::vector<std::string> FFOpt::GetFilenamesBBbarMC() const {
    return m_filenames_bbbar_MC;
}

std::vector<std::string> FFOpt::GetFilenamesSelectedEvents() const {
    return m_filenames_selected_events;
}

std::vector<std::string> FFOpt::GetFilenamesGeneratedEvents() const {
    return m_filenames_generated_events;
}

std::vector<std::string> FFOpt::GetFilenamesGsim() const {
    return m_filenames_gsim;
}

std::vector<std::string> FFOpt::GetFilenamesSCF() const {
    return m_filenames_SCF;
}

std::vector<std::string> FFOpt::GetFilenamesGsimReweight() const {
	return m_filenames_gsim_reweight;
}

std::vector<std::string> FFOpt::GetFilenamesEvtGen() const {
    return m_filenames_evtgen;
}

std::vector<std::string> FFOpt::GetFilenamesControlChannelData() const {
    return m_filenames_control_channel_data;
}

std::vector<std::string> FFOpt::GetFilenamesControlChannelGsim() const {
    return m_filenames_control_channel_gsim;
}

std::vector<std::string> FFOpt::GetFilenamesFitResultsToPlot() const {
	return m_filenames_fit_results_to_plot;
}

std::string FFOpt::GetFitResultsPlotName() const {
	return m_fit_results_plot_name_prefix;
}

std::string FFOpt::GetPlotFormat() const {
	return m_plot_format;
}

std::string FFOpt::GetConfigFile() const {
	return m_config_file;
}

bool FFOpt::GetLogy() const {
    return m_logy;
}

bool FFOpt::GetMcinfo() const {
    return m_mcinfo;
}

bool FFOpt::GetMctype() const {
    return m_mctype;
}

bool FFOpt::GetPdfLegend() const {
    return m_pdf_legend;
}

bool FFOpt::GetMinos() const {
    return m_minos;
}

int FFOpt::GetNumcpu() const {
    return m_numcpu;
}

std::string FFOpt::GetOutdir() const {
    return m_outdir;
}

bool FFOpt::GetPull() const {
    return m_pull;
}

bool FFOpt::GetPullFiterror() const {
    return m_pull_fiterror;
}

bool FFOpt::GetTesting() const {
    return m_testing;
}

bool FFOpt::GetUnblind() const {
    return m_unblind;
}

std::string FFOpt::GetTreename() const {
    return m_treename;
}

std::string FFOpt::GetTreenameFriend() const {
    return m_friend_treename;
}

bool FFOpt::GetCreateCorrelationFiles() const {
	return m_create_correlation_files;
}

bool FFOpt::GetCreateComponentPlots() const {
	return m_create_component_plots;
}

bool FFOpt::GetCreateLLRProjection() const {
	return m_create_llr_projection;
}

bool FFOpt::GetCreatePlots() const {
	return m_create_plots;
}

bool FFOpt::GetCreateProfileLL() const {
	return m_create_profile_ll;
}

bool FFOpt::GetCreateProjWData() const {
	return m_create_projwdata;
}

std::string FFOpt::GetCreateProfileLLParameter() const {
	return m_create_profile_ll_parameter;
}

std::string FFOpt::GetProfileLLConfigFile() const {
	return m_profile_ll_config_file;
}

bool FFOpt::GetFixCPV2PerpAndParaComponent() const{
	return m_fix_cpv_2perp_and_para_component;
}

double FFOpt::GetNetworkCut() const {
	return m_network_cut;
}

int FFOpt::GetNToys() const {
	return m_ntoys;
}

int FFOpt::GetNGsimToys() const {
	return m_ngsimtoys;
}

unsigned int FFOpt::GetRandomSeed() const {
	return m_random_seed;
}

unsigned int FFOpt::GetSingleToy() const {
	return m_single_toy;
}

bool FFOpt::ParseOptions(int argc, char *argv[]) {
	po::options_description opt_data("Options for data handling");
	opt_data.add_options()
		("outdir,o", po::value<std::string>(&m_outdir)->default_value("plots/"), "Output files path")
		("treename", po::value<std::string>(&m_treename)->default_value("h1"), "Name of TTree for the TChain")
		("treename_friend", po::value<std::string>(&m_friend_treename)->default_value("friend"), "Name of friend TTree for the TChain")
		("f_mc", po::value<std::vector<std::string> >(&m_filenames_MC)->multitoken(), "Input filename(s) or regular expression for TChain used as MC sample")
		("f_data", po::value<std::vector<std::string> >(&m_filenames_data)->multitoken(), "Input filename(s) or regular expression for TChain used as data sample")
		("f_continuum,c", po::value<std::vector<std::string> >(&m_filenames_continuum)->multitoken(), "Input filename(s) or regular expression for TChain used as continuum MC sample")
		("f_signal_mc,s", po::value<std::vector<std::string> >(&m_filenames_signal_MC)->multitoken(), "Input filename(s) or regular expression for TChain used as signal MC sample")
		("f_bbbar_mc,b", po::value<std::vector<std::string> >(&m_filenames_bbbar_MC)->multitoken(), "Input filename(s) or regular expression for TChain used as BBbar background MC sample")
		("f_selected_events", po::value<std::vector<std::string> >(&m_filenames_selected_events)->multitoken(), "Input filename(s) or regular expression for TChain used as selected events sample")
		("f_generated_events", po::value<std::vector<std::string> >(&m_filenames_generated_events)->multitoken(), "Input filename(s) or regular expression for TChain used as generated events sample")
		("f_gsim", po::value<std::vector<std::string> >(&m_filenames_gsim)->multitoken(), "Input filename(s) or regular expression for TChain used as GSIM (bootstrapping) input")
		("f_gsim_reweight", po::value<std::vector<std::string> >(&m_filenames_gsim_reweight)->multitoken(), "Input filename(s) or regular expression for TChain used as GSIM input to be reweighted with signal config")
		("f_scf", po::value<std::vector<std::string> >(&m_filenames_SCF)->multitoken(), "Input filename(s) or regular expression for TChain used as SCF input")
		("f_evtgen", po::value<std::vector<std::string> >(&m_filenames_evtgen)->multitoken(), "Input filename(s) or regular expression for TChain used as fit on EvtGen input")
		("f_cc_data", po::value<std::vector<std::string> >(&m_filenames_control_channel_data)->multitoken(), "Input filename(s) or regular expression for TChain used as control channel data sample")
		("f_cc_gsim", po::value<std::vector<std::string> >(&m_filenames_control_channel_gsim)->multitoken(), "Input filename(s) or regular expression for TChain used as contorl channel GSIM sample")
		;

	po::options_description opt_general("General options");
	opt_general.add_options()
		("help,h", "Produce help message)")
		("numcpu", po::value<int>(&m_numcpu)->default_value(2), "Number of CPUs used during minimization")
		("minos", po::value<bool>(&m_minos)->default_value(false), "Do MINOS error estimation")
		("unblind", po::value<bool>(&m_unblind)->default_value(false), "Flag whether to unblind the signal region in data")
		("fix_cpv_2perp_and_para_component", po::value<bool>(&m_fix_cpv_2perp_and_para_component)->default_value(false), "Fix the a_2 CPV parameters for parallel and perp component to zero")
		("create_correlation_files", po::value<bool>(&m_create_correlation_files)->default_value(false), "Write correlation study files")
		("testing", po::value<bool>(&m_testing)->default_value(false), "Run some testing parts, e.g. scan PDFs or generate ToyMC. Many other options are ignored if testing is true.")
		("toymc", po::value<int>(&m_ntoys)->default_value(-1), "Run Toy MC fits and do a toy study. Other options (numcpu, minos, etc..) are taken into account!")
		("gsimtoymc", po::value<int>(&m_ngsimtoys)->default_value(-1), "Run Gsim Toy MC fits and do a toy study. Other options (numcpu, minos, etc..) are taken into account!")
		("network_cut", po::value<double>(&m_network_cut)->default_value(0.0), "Network cut applied to data during all fits!")
		("random_seed", po::value<unsigned int>(&m_random_seed)->default_value(4357), "Random seed, which is usually used in combination with the single toy flag and the toy number")
		("single_toy", po::value<unsigned int>(&m_single_toy)->default_value(0), "Run just a single toy with the given number used for additional seeding")
		("config_file", po::value<std::string>(&m_config_file)->default_value(""), "Configuration file (e.g. 8D toy settings)")
		("config_profile_ll_file", po::value<std::string>(&m_profile_ll_config_file)->default_value(""), "Configuration file for profile LL scan")
		;

	po::options_description opt_plot("Options for visualization control");
	opt_plot.add_options()
		("bins", po::value<int>(&m_bins)->default_value(50), "Number of bins in each histogram")
		("mctype", po::value<bool>(&m_mctype)->default_value(false), "Add MC Type histogram to fit result")
		("mcinfo", po::value<bool>(&m_mcinfo)->default_value(false), "Add MC Info histogram to fit result")
		("pdflegend", po::value<bool>(&m_pdf_legend)->default_value(false), "Add a legend for fitted pdfs")
		("logy", po::value<bool>(&m_logy)->default_value(false), "Create plots with log-scale on y-axis")
		("pull", po::value<bool>(&m_pull)->default_value(true), "Adds a histogram with the pulls below the fit histogram")
		("fiterror_pull", po::value<bool>(&m_pull_fiterror)->default_value(false), "Uses fiterror instead of dataerror for pull normalization")
		("create_llr_projection", po::value<bool>(&m_create_llr_projection)->default_value(false), "Create LLR projection plots")
		("create_component_plots", po::value<bool>(&m_create_component_plots)->default_value(false), "Create data component plots")
		("create_plots", po::value<bool>(&m_create_plots)->default_value(true), "Create plots in general (e.g. turn of for Toy Study)")
		("create_profile_ll", po::value<bool>(&m_create_profile_ll)->default_value(false), "Create profile likelihood plots after fit")
		("create_profile_ll_parameter", po::value<std::string>(&m_create_profile_ll_parameter)->default_value(""), "Parameter to create profile LL for")
		("create_projwdata", po::value<bool>(&m_create_projwdata)->default_value(false), "Create time cosuming ProjWData plots")
		("plot_format,p", po::value<std::string>(&m_plot_format)->default_value("pdf"), "Plot output format (eps,pdf,png,C,root,...)")
		;

	po::options_description opt_fit_results("Options for fit result plots");
	opt_fit_results.add_options()
		("f_fit_results", po::value<std::vector<std::string> >(&m_filenames_fit_results_to_plot)->multitoken(), "Input filename(s) for fit results plot to be created")
		("f_fit_results_plot_name_prefix", po::value<std::string>(&m_fit_results_plot_name_prefix)->default_value("GSIM_fit_results_variable_"), "Prefix for fit result plots")
		;

	po::options_description opt_all("Allowed options");
	opt_all.add(opt_general).add(opt_data).add(opt_plot).add(opt_fit_results);

	// create variables map and run parser
	bool parser_succeeded = true;
	po::variables_map vm;
	try {
		//po::store(po::parse_command_line(argc, argv, opt_all), vm);
		po::store(po::command_line_parser(argc, argv).options(opt_all).run(), vm);
		po::notify(vm);
	} catch (po::error& e) {
		std::cerr << "ERROR in command_line_parser or notify: " << e.what() << std::endl;
		parser_succeeded = false;
	}

	if (vm.count("help")) {
		std::cout << opt_all << std::endl;
		parser_succeeded = false;
	}

	if(m_numcpu > sysconf( _SC_NPROCESSORS_ONLN )) {
		std::cout << "WARNING: NumCPU set is larger than available CPUs, value was reset to " << sysconf( _SC_NPROCESSORS_ONLN ) << std::endl;
		m_numcpu = sysconf( _SC_NPROCESSORS_ONLN );
	}

	if(mp::to_string(m_outdir[m_outdir.size()-1]).compare(mp::to_string("/")) != 0){
		std::cout << "WARNING: Missing slash at the end of outdir option, an / was appended" << std::endl;
		m_outdir += "/";
	}

	return parser_succeeded;
}

void FFOpt::PrintOptions() {
	std::cout << "Outdir for results: " << m_outdir << "\n";
	std::cout << "Number of bins used per histogram:    " << m_bins << "\n";
	std::cout << "Number of CPUs used for minimization: " << m_numcpu << "\n";
	std::cout << "Network Cut Value (for eff. fits):    " << mp::to_string(m_network_cut) << "\n";
	std::cout << "MINOS errors will be calculated:     " << mp::bool_string(m_minos) << "\n";
	std::cout << "Signal region will be unblinded:     " << mp::bool_string(m_unblind) << "\n";
	std::cout << "MC Type will be added to plots:      " << mp::bool_string(m_mctype) << "\n";
	std::cout << "MC Info will be added to plots:      " << mp::bool_string(m_mcinfo) << "\n";
	std::cout << "PDF Legend will be added to plots:   " << mp::bool_string(m_pdf_legend) << "\n";
	std::cout << "Using log scale on y-axis:           " << mp::bool_string(m_logy) << "\n";
	std::cout << "Create pull histogram below plots:   " << mp::bool_string(m_pull) << "\n";
	std::cout << "Using fiterror instead of dataerror: " << mp::bool_string(m_pull_fiterror) << "\n";
	std::cout << "Testing PDF Mode:                    " << mp::bool_string(m_testing) << "\n";
	std::cout << "Random seed:                         " << m_random_seed << "\n";
	std::cout << "Single toy:                          " << m_single_toy << "\n";
	std::cout << "#toys:                               " << m_ntoys << "\n";
	std::cout << "#gsim toys:                          " << m_ngsimtoys << "\n";
	std::cout << "Configuration file:                  " << m_config_file << "\n";
	std::cout << "Plot format:                         " << m_plot_format << "\n";
	std::cout << "Create files for correlation study:  " << mp::bool_string(m_create_correlation_files) << "\n";
	std::cout << "Create data component plots:         " << mp::bool_string(m_create_component_plots) << "\n";
	std::cout << "Create LLR projection plots:         " << mp::bool_string(m_create_llr_projection) << "\n";
	std::cout << "Create plots in general:             " << mp::bool_string(m_create_plots) << "\n";
	std::cout << "Create profile LL plots:             " << mp::bool_string(m_create_profile_ll) << "\n";
	std::cout << "Profile LL plots for variable:       " << m_create_profile_ll_parameter << "\n";
	std::cout << "Fix A_2 CPV perp and para component: " << mp::bool_string(m_fix_cpv_2perp_and_para_component) << "\n";
	std::cout << "Tree " << m_treename << " and friend " << m_friend_treename << " will be chained using the following files..." << "\n";
	std::cout << "...for data:" << "\n";
	std::cout << mp::vector_to_string(m_filenames_data, true) << "\n";
	std::cout << "...for continuum MC:" << "\n";
	std::cout << mp::vector_to_string(m_filenames_continuum, true) << "\n";
	std::cout << "...for signal MC:" << "\n";
	std::cout << mp::vector_to_string(m_filenames_signal_MC, true) << "\n";
	std::cout << "...for BBbar MC:" << "\n";
	std::cout << mp::vector_to_string(m_filenames_bbbar_MC, true) << "\n";
	std::cout << "...for MC:" << "\n";
	std::cout << mp::vector_to_string(m_filenames_MC, true) << "\n";
	std::cout << "...for generated events:" << "\n";
	std::cout << mp::vector_to_string(m_filenames_generated_events, true) << "\n";
	std::cout << "...for selected events:" << "\n";
	std::cout << mp::vector_to_string(m_filenames_selected_events, true) << "\n";
	std::cout << "...for GSIM events:" << "\n";
	std::cout << mp::vector_to_string(m_filenames_gsim, true) << "\n";
	std::cout << "...for GSIM events to reweight:" << "\n";
	std::cout << mp::vector_to_string(m_filenames_gsim_reweight, true) << "\n";
	std::cout << "...for SCF:" << "\n";
	std::cout << mp::vector_to_string(m_filenames_SCF, true) << "\n";
	std::cout << "...for EvtGen events:" << "\n";
	std::cout << mp::vector_to_string(m_filenames_evtgen, true) << "\n";
	std::cout << "...for control channel data events:" << "\n";
	std::cout << mp::vector_to_string(m_filenames_control_channel_data, true) << "\n";
	std::cout << "...for control channel GSIM events:" << "\n";
	std::cout << mp::vector_to_string(m_filenames_control_channel_gsim, true) << "\n";
}
