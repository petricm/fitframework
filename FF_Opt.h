/**
 * @file	FFOpt.h
 * @date	May 9, 2011
 * @author	petric
 * @brief	brief description
 *
 * long description
 */

#ifndef FF_OPT_H_
#define FF_OPT_H_

#include <string>
#include <vector>

class FFOpt {
public:
	FFOpt() : m_bins(0), m_numcpu(2), m_ntoys(-1), m_ngsimtoys(-1), m_minos(false), m_unblind(false),
			m_mctype(false), m_mcinfo(false), m_pdf_legend(false), m_logy(false), m_pull(false),
			m_pull_fiterror(false), m_testing(false), m_create_correlation_files(false), m_create_component_plots(false),
			m_create_llr_projection(false), m_create_plots(false), m_create_profile_ll(false), m_create_projwdata(false),
			m_fix_cpv_2perp_and_para_component(false), m_network_cut(0.0), m_random_seed(4357), m_single_toy(0) { };
	~FFOpt() { };

	/**
	 * @brief Parse the program options and set all members
	 * @param argc
	 * @param argv
	 * @return true if success, false if error occured
	 */
	bool ParseOptions(int argc, char *argv[]);

	/**
	 * @brief Prints current options
	 */
	void PrintOptions();

    std::string GetOutdir() const;
    std::string GetTreename() const;
    std::string GetTreenameFriend() const;
    std::vector<std::string> GetFilenamesMC() const;
    std::vector<std::string> GetFilenamesContinuumMC() const;
    std::vector<std::string> GetFilenamesSignalMC() const;
    std::vector<std::string> GetFilenamesBBbarMC() const;
    std::vector<std::string> GetFilenamesSelectedEvents() const;
    std::vector<std::string> GetFilenamesGeneratedEvents() const;
    std::vector<std::string> GetFilenamesData() const;
	std::vector<std::string> GetFilenamesGsim() const;
	std::vector<std::string> GetFilenamesSCF() const;
	std::vector<std::string> GetFilenamesGsimReweight() const;
	std::vector<std::string> GetFilenamesEvtGen() const;
	std::vector<std::string> GetFilenamesControlChannelData() const;
	std::vector<std::string> GetFilenamesControlChannelGsim() const;
	std::vector<std::string> GetFilenamesFitResultsToPlot() const;
    std::string GetPlotFormat() const;
    std::string GetConfigFile() const;
    std::string GetFitResultsPlotName() const;
    int GetBins() const;
    int GetNumcpu() const;
    int GetNToys() const;
    int GetNGsimToys() const;
    bool GetLogy() const;
    bool GetMcinfo() const;
    bool GetMctype() const;
    bool GetPdfLegend() const;
    bool GetMinos() const;
    bool GetSideband() const;
    bool GetPull() const;
    bool GetPullFiterror() const;
    bool GetTesting() const;
    bool GetUnblind() const;
    bool GetCreateCorrelationFiles() const;
    bool GetCreateComponentPlots() const;
    bool GetCreateLLRProjection() const;
    bool GetCreatePlots() const;
    bool GetCreateProfileLL() const;
    bool GetCreateProjWData() const;
    std::string GetCreateProfileLLParameter() const;
    std::string GetProfileLLConfigFile() const;
    bool GetFixCPV2PerpAndParaComponent() const;
    double GetNetworkCut() const;
    unsigned int GetRandomSeed() const;
    unsigned int GetSingleToy() const;

private:
	std::string m_outdir;
	std::string m_treename;
	std::string m_friend_treename;
	std::vector<std::string> m_filenames_MC;
	std::vector<std::string> m_filenames_signal_MC;
	std::vector<std::string> m_filenames_continuum;
	std::vector<std::string> m_filenames_bbbar_MC;
	std::vector<std::string> m_filenames_selected_events;
	std::vector<std::string> m_filenames_generated_events;
	std::vector<std::string> m_filenames_data;
	std::vector<std::string> m_filenames_gsim;
	std::vector<std::string> m_filenames_SCF;
	std::vector<std::string> m_filenames_gsim_reweight;
	std::vector<std::string> m_filenames_evtgen;
	std::vector<std::string> m_filenames_control_channel_data;
	std::vector<std::string> m_filenames_control_channel_gsim;
	std::vector<std::string> m_filenames_fit_results_to_plot;
	std::string m_plot_format;
	std::string m_config_file;
	std::string m_fit_results_plot_name_prefix;
	int m_bins;
	int m_numcpu;
	int m_ntoys;
	int m_ngsimtoys;
	bool m_minos;
	bool m_unblind;
	bool m_mctype;
	bool m_mcinfo;
	bool m_pdf_legend;
	bool m_logy;
	bool m_pull;
	bool m_pull_fiterror;
	bool m_testing;
	bool m_create_correlation_files;
	bool m_create_component_plots;
	bool m_create_llr_projection;
	bool m_create_plots;
	bool m_create_profile_ll;
	bool m_create_projwdata;
	std::string m_create_profile_ll_parameter;
	std::string m_profile_ll_config_file;
	bool m_fix_cpv_2perp_and_para_component;
	double m_network_cut;
	unsigned int m_random_seed;
	unsigned int m_single_toy;

};

#endif /* FF_OPT_H_ */
