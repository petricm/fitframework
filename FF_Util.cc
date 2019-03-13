/**
 * @file	FFUtil.cc
 * @date	May 9, 2011
 * @author	petric
 * @brief	brief description
 *
 * long description
 */

#include "FF_Util.h"
#include "FF_PDF.h"
#include "FF_PDF_signal.h"
// Utilities
#include "mp_utility.h"

namespace FFUtil {
    
    std::string GetLabelToTLatex(const std::string& label) {
        static bool map_initialized = false;
        static std::map<std::string, std::string> label_to_TLatex;
        if(!map_initialized) {
            label_to_TLatex["amp_00"] = "a_{00}";
            label_to_TLatex["amp_00_delta"] = "#Deltaa_{00}";
            label_to_TLatex["amp_10"] = "a_{10}";
            label_to_TLatex["amp_10_delta"] = "#Deltaa_{10}";
            label_to_TLatex["amp_1para"] = "a_{1#parallel}";
            label_to_TLatex["amp_1para_delta"] = "#Deltaa_{1#parallel}";
            label_to_TLatex["amp_1perp_delta"] = "#Deltaa_{1}#scale[0.6]{#perp}";
            label_to_TLatex["amp_1perp"] = "a_{1}#scale[0.6]{#perp}";
            label_to_TLatex["amp_20"] = "a_{20}";
            label_to_TLatex["amp_20_delta"] = "#Deltaa_{20}";
            label_to_TLatex["amp_2para"] = "a_{2#parallel}";
            label_to_TLatex["amp_2para_delta"] = "#Deltaa_{2#parallel}";
            label_to_TLatex["amp_2perp"] = "a_{2}#scale[0.6]{#perp}";
            label_to_TLatex["amp_2perp_delta"] = "#Deltaa_{2}#scale[0.6]{#perp}";
            
            label_to_TLatex["phase_00"] = "#varphi_{00}";
            label_to_TLatex["phase_00_delta"] = "#Delta#varphi_{00}";
            label_to_TLatex["phase_10"] = "#varphi_{10}";
            label_to_TLatex["phase_10_delta"] = "#Delta#varphi_{10}";
            label_to_TLatex["phase_1para"] = "#varphi_{1#parallel}";
            label_to_TLatex["phase_1para_delta"] = "#Delta#varphi_{1#parallel}";
            label_to_TLatex["phase_1perp"] = "#varphi_{1^{#scale[1.1]{#perp}}}";
            label_to_TLatex["phase_1perp_delta"] = "#Delta#varphi_{1^{#scale[1.1]{#perp}}}";
            label_to_TLatex["phase_20"] = "#varphi_{20}";
            label_to_TLatex["phase_20_delta"] = "#Delta#varphi_{20}";
            label_to_TLatex["phase_2para"] = "#varphi_{2#parallel}";
            label_to_TLatex["phase_2para_delta"] = "#Delta#varphi_{2#parallel}";
            label_to_TLatex["phase_2perp"] = "#varphi_{2^{#scale[1.1]{#perp}}}";
            label_to_TLatex["phase_2perp_delta"] = "#Delta#varphi_{2^{#scale[1.1]{#perp}}}";
            
            label_to_TLatex["signal_nevents"] = "N_{sig}";
            label_to_TLatex["C_bkg_continuum_nevents"] = "N_{bkg}";
            label_to_TLatex["C_bkg_bbar_wrongphikkbar_nevents"] = "N_{KK}";
            label_to_TLatex["C_bkg_bbar_wrongphif0980_nevents"] = "N_{KK}";
            label_to_TLatex["mbc_bkg_continuum_c"] = "c";
            
            label_to_TLatex["fL0"] = "f_{L0}";
            label_to_TLatex["fL1"] = "f_{L1}";
            label_to_TLatex["fP1"] = "f_{#perp1}";
            label_to_TLatex["fL2"] = "f_{L2}";
            label_to_TLatex["fP2"] = "f_{#perp2}";
            label_to_TLatex["A_CP0_0"] = "A_{CP0}^{0}";
            label_to_TLatex["A_CP1_0"] = "A_{CP1}^{0}";
            label_to_TLatex["A_CP1_P"] = "A_{CP1}^{#perp}";
            label_to_TLatex["A_CP2_0"] = "A_{CP2}^{0}";
            label_to_TLatex["A_CP2_P"] = "A_{CP2}^{#perp}";
            label_to_TLatex["phi_P1"] = "#phi_{#perp1}";
            label_to_TLatex["phi_P2"] = "#phi_{#perp2}";
            label_to_TLatex["phi_||1"] = "#phi_{#parallel1}";
            label_to_TLatex["phi_||2"] = "#phi_{#parallel2}";
            label_to_TLatex["Delta_phi_00"] = "#Delta#phi_{00}";
            label_to_TLatex["Delta_phi_P1"] = "#Delta#phi_{#perp1}";
            label_to_TLatex["Delta_phi_P2"] = "#Delta#phi_{#perp2}";
            label_to_TLatex["Delta_phi_||1"] = "#Delta#phi_{#parallel1}";
            label_to_TLatex["Delta_phi_||2"] = "#Delta#phi_{#parallel2}";
            label_to_TLatex["delta_00"] = "#delta_{00}";
            label_to_TLatex["delta_01"] = "#delta_{01}";
            label_to_TLatex["delta_02"] = "#delta_{02}";
            label_to_TLatex["Delta_delta_00"] = "#Delta#delta_{00}";
            label_to_TLatex["Delta_delta_01"] = "#Delta#delta_{01}";
            label_to_TLatex["Delta_delta_02"] = "#Delta#delta_{02}";
            label_to_TLatex["frac_K21430"] = "FF K_{2}*(1430)";
            label_to_TLatex["frac_K21430_asym"] = "A_{CP2}";
            label_to_TLatex["frac_K892"] = "FF K*(892)";
            label_to_TLatex["frac_K892_asym"] = "A_{CP1}";
            label_to_TLatex["frac_LASS"] = "FF K_{0}*(1430)";
            label_to_TLatex["frac_LASS_asym"] = "A_{CP0}";
            label_to_TLatex["frac_sum"] = "#sum FF";
            label_to_TLatex["signal_asym"] = "#DeltaN_{sig}";
            label_to_TLatex["signal_yield"] = "N_{sig}";
            label_to_TLatex["yield_K21430"] = "Yield K_{2}*(1430)";
            label_to_TLatex["yield_K892"] = "Yield K*(892)";
            label_to_TLatex["yield_LASS"] = "Yield K_{0}*(1430)";
            
            map_initialized = true;
        }
        
        if(label_to_TLatex.count(label)) {
            return label_to_TLatex[label];
        } else {
            std::cout << "ERROR: Label " << label << " not found in label_to_TLatex map!" << std::endl;
            return label;
        }
    }
    
    std::string GetLabelToLatex(const std::string& label) {
        static bool map_initialized = false;
        static std::map<std::string, std::string> label_to_latex;
        if(!map_initialized) {
            label_to_latex["amp_00"] = "a_{00}";
            label_to_latex["amp_00_delta"] = "\\Delta a_{00}";
            label_to_latex["amp_10"] = "a_{10}";
            label_to_latex["amp_10_delta"] = "\\Delta a_{10}";
            label_to_latex["amp_1para"] = "a_{1\\parallel}";
            label_to_latex["amp_1para_delta"] = "\\Delta a_{1\\parallel}";
            label_to_latex["amp_1perp"] = "a_{1\\perp}";
            label_to_latex["amp_1perp_delta"] = "\\Delta a_{1\\perp}";
            label_to_latex["amp_20"] = "a_{20}";
            label_to_latex["amp_20_delta"] = "\\Delta a_{20}";
            label_to_latex["amp_2para"] = "a_{2\\parallel}";
            label_to_latex["amp_2para_delta"] = "\\Delta a_{2\\parallel}";
            label_to_latex["amp_2perp"] = "a_{2\\perp}";
            label_to_latex["amp_2perp_delta"] = "\\Delta a_{2\\perp}";
            
            label_to_latex["phase_00"] = "\\varphi_{00}";
            label_to_latex["phase_00_delta"] = "\\Delta \\varphi_{00}";
            label_to_latex["phase_10"] = "\\varphi_{10}";
            label_to_latex["phase_10_delta"] = "\\Delta \\varphi_{10}";
            label_to_latex["phase_1para"] = "\\varphi_{1\\parallel}";
            label_to_latex["phase_1para_delta"] = "\\Delta \\varphi_{1\\parallel}";
            label_to_latex["phase_1perp"] = "\\varphi_{1\\perp}";
            label_to_latex["phase_1perp_delta"] = "\\Delta \\varphi_{1\\perp}";
            label_to_latex["phase_20"] = "\\varphi_{20}";
            label_to_latex["phase_20_delta"] = "\\Delta \\varphi_{20}";
            label_to_latex["phase_2para"] = "\\varphi_{2\\parallel}";
            label_to_latex["phase_2para_delta"] = "\\Delta \\varphi_{2\\parallel}";
            label_to_latex["phase_2perp"] = "\\varphi_{2\\perp}";
            label_to_latex["phase_2perp_delta"] = "\\Delta \\varphi_{2\\perp}";
            
            label_to_latex["signal_nevents"] = "N_{sig}";
            
            label_to_latex["fL0"] = "f_{L0}";
            label_to_latex["fL1"] = "f_{L1}";
            label_to_latex["fP1"] = "f_{\\perp 1}";
            label_to_latex["fL2"] = "f_{L2}";
            label_to_latex["fP2"] = "f_{\\perp 2}";
            label_to_latex["A_CP0_0"] = "A_{CP0}^{0}";
            label_to_latex["A_CP1_0"] = "A_{CP1}^{0}";
            label_to_latex["A_CP1_P"] = "A_{CP1}^{\\perp}";
            label_to_latex["A_CP2_0"] = "A_{CP2}^{0}";
            label_to_latex["A_CP2_P"] = "A_{CP2}^{\\perp}";
            label_to_latex["phi_P1"] = "\\phi_{\\perp 1}";
            label_to_latex["phi_P2"] = "\\phi_{\\perp 2}";
            label_to_latex["phi_||1"] = "\\phi_{\\parallel 1}";
            label_to_latex["phi_||2"] = "\\phi_{\\parallel 2}";
            label_to_latex["Delta_phi_00"] = "\\Delta\\phi_{00}";
            label_to_latex["Delta_phi_P1"] = "\\Delta\\phi_{\\perp 1}";
            label_to_latex["Delta_phi_P2"] = "\\Delta\\phi_{\\perp 2}";
            label_to_latex["Delta_phi_||1"] = "\\Delta\\phi_{\\parallel 1}";
            label_to_latex["Delta_phi_||2"] = "\\Delta\\phi_{\\parallel 2}";
            label_to_latex["delta_00"] = "\\delta_{00}";
            label_to_latex["delta_01"] = "\\delta_{01}";
            label_to_latex["delta_02"] = "\\delta_{02}";
            label_to_latex["Delta_delta_00"] = "\\Delta\\delta_{00}";
            label_to_latex["Delta_delta_01"] = "\\Delta\\delta_{01}";
            label_to_latex["Delta_delta_02"] = "\\Delta\\delta_{02}";
            label_to_latex["frac_K21430"] = "\\text{FF }K_{2}^{*}(1430)";
            label_to_latex["frac_K21430_asym"] = "A_{CP2}";
            label_to_latex["frac_K892"] = "\\text{FF }K^{*}(892)";
            label_to_latex["frac_K892_asym"] = "A_{CP1}";
            label_to_latex["frac_LASS"] = "\\text{FF }K_{0}^{*}(1430)";
            label_to_latex["frac_LASS_asym"] = "A_{CP0}";
            label_to_latex["frac_sum"] = "\\sum \\text{FF}";
            label_to_latex["signal_asym"] = "\\Delta N_{sig}";
            label_to_latex["signal_yield"] = "N_{sig}";
            label_to_latex["yield_K21430"] = "\\text{Yield }K_{2}^{*}(1430)";
            label_to_latex["yield_K892"] = "\\text{Yield }K^{*}(892)";
            label_to_latex["yield_LASS"] = "\\text{Yield }K_{0}^{*}(1430)";
            
        }
        
        if(label_to_latex.count(label)) {
            return label_to_latex[label];
        } else {
            std::cout << "ERROR: Label " << label << " not found in label_to_TLatex map!" << std::endl;
            return label;
        }
    }
    
    // Add to given plot "plot_on" of variable "variable" a stacked histogram of mctypes used in data "data"
    // Use nbins "opt_bins" for the histogram and draw data in CutRange "range"
    // return value is std::pair<TLegend*, std::pair<THStack*, std::vector<TH1*> > > and needs to be deleted by the caller after saving plot
    std::vector<TH1*> AddHistogram_MCType(RooPlot *plot_on, RooDataSet &data, RooRealVar &variable, int opt_bins, std::string range, std::string category_cut) {
        // The map needs to be edited by the user
        // std::map<COUNTER, std::tuple<COLOR, LEGEND-ENTRY, CUT-STRING> >;
        std::map<int , std::tuple<Color_t, std::string, std::string> > map_counter_color_legend_cut;
        map_counter_color_legend_cut[0] = std::make_tuple(kCyan, "mixed", "mctype==1001");      // mixed
        map_counter_color_legend_cut[1] = std::make_tuple(kMagenta, "charged", "mctype==1002"); // charged
        map_counter_color_legend_cut[2] = std::make_tuple(kOrange, "charm", "mctype==1003");    // charm
        map_counter_color_legend_cut[3] = std::make_tuple(kSpring-5, "uds", "mctype==1004");      // uds
        map_counter_color_legend_cut[4] = std::make_tuple(kViolet, "rare or signal", "mctype<1001 || mctype>1004");  // rare or signal
        // Below this line, no changes on this method need to be done by the user
        int hist_num = map_counter_color_legend_cut.size();
        int leg_entries = 0;
        std::vector<TH1*> hists;
        hists.assign(hist_num,NULL);
        THStack *plot_stack = new THStack("plot_stack","plot stack");
        // create histograms and fill to stack
        for(int i = 0; i < hist_num; ++i) {
            hists[i] = data.createHistogram(("plot_"+mp::to_string(i)).c_str(),variable,Binning(opt_bins),Cut((category_cut+std::get<2>(map_counter_color_legend_cut[i])).c_str()),CutRange(range.c_str()));
            if(hists[i]->GetEntries() > 0) {
                hists[i]->SetFillColor(std::get<0>(map_counter_color_legend_cut[i]));
                hists[i]->SetLineColor(std::get<0>(map_counter_color_legend_cut[i]));
                plot_stack->Add(hists[i]);
                ++leg_entries;
            }
        }
        // fill legend in reversed order, so that bottom histogram (which is drawn first) becomes last entry in the legend
        TLegend *leg = new TLegend(0.78,0.94-0.04*leg_entries,0.92,0.94,"","NDC");
        leg->SetBorderSize(0);
        leg->SetFillColor(0);
        leg->SetFillStyle(4000);
        leg->SetTextSize(0.035);
        std::cout << "--- Creating legend ---" << std::endl;
        for(int i = hist_num-1; i >= 0; --i) {
            if(hists[i]->GetEntries() > 0) {
                leg->AddEntry(hists[i],(std::get<1>(map_counter_color_legend_cut[i])).c_str(),"F");
                std::cout << "Filled " << hists[i]->GetEntries() << " " << std::get<1>(map_counter_color_legend_cut[i]) << " entries to the plot." << std::endl;
            }
        }
        std::cout << "------ [x] done -------" << std::endl;
        plot_on->addObject(plot_stack,"HISTsame");
        plot_on->drawBefore("data_hist",plot_stack->GetName());
        plot_on->addObject(leg,"");
        return hists;
    }
    
    
    // Add to given plot "plot_on" of variable "variable" a stacked histogram of background types in data "data"
    // Use nbins "opt_bins" for the histogram and draw data in CutRange "range"
    // return value is std::pair<TLegend*, std::pair<THStack*, std::vector<TH1*> > > and needs to be deleted by the caller after saving plot
    std::vector<TH1*> AddHistogram_MCInfo(RooPlot *plot_on, RooDataSet &data, RooRealVar &variable, int opt_bins, std::string range, std::string category_cut) {
        // The map needs to be edited by the user
        // std::map<COUNTER, std::tuple<COLOR, LEGEND-ENTRY, CUT-STRING> >;
        std::map<int , std::tuple<Color_t, std::string, std::string> > map_counter_color_legend_cut;
        map_counter_color_legend_cut[0] = std::make_tuple(kOrange, "q#bar{q} continuum", "mctype==1004 || mctype==1003"); // uds
        map_counter_color_legend_cut[1] = std::make_tuple(kAzure-5, "#Upsilon(4S) combinatoric", "(((mctype==1001 || mctype==1002) || ((mctype<1001 || mctype>1004) && runno==0 && mcinfo!=59 && mcinfo!=61)) && mchash==1)");
        map_counter_color_legend_cut[2] = std::make_tuple(kViolet, "B^{#pm}/B^{0} #rightarrow c/s bkg","(((mctype==1001 || mctype==1002) || ((mctype<1001 || mctype>1004) && runno==0 && mcinfo!=59 && mcinfo!=61)) && mchash!=1 && !(((mcinfo & 0x8) ==0) && (((mcinfo & 0x4) !=0) || ((mcinfo & 0x2) !=0))) && mcinfo<64 && !(mchash==141183 ||mchash==2513873 ||mchash==2245944 ||mchash==2323258 ||mchash==2068777 ||mchash==1769521 ||mchash==1639018 ||mchash==3905351 ||mchash==2939915 ||mchash==1962594 ||mchash==3500850 ||mchash==1881338))");
        map_counter_color_legend_cut[3] = std::make_tuple(kCyan, "Wrong K and/or #pi hypothesis","(((mctype==1001 || mctype==1002) || ((mctype<1001 || mctype>1004) && runno==0 && mcinfo!=59 && mcinfo!=61)) && mchash!=1 && ((mcinfo & 0x8) ==0) && (((mcinfo & 0x4) !=0) || ((mcinfo & 0x2) !=0)) && mcinfo<64 )");
        map_counter_color_legend_cut[4] = std::make_tuple(kMagenta-5,   "Wrong #pi^{0} hypothesis","(((mctype==1001 || mctype==1002) || ((mctype<1001 || mctype>1004) && runno==0 && mcinfo!=59 && mcinfo!=61)) && mchash!=1 && mcinfo>64)");
        map_counter_color_legend_cut[5] = std::make_tuple(kYellow, "B^{0} #rightarrow #bar{D}^{0}[K^{+} #pi^{-}]#pi^{0} and D^{-}[#pi^{0} #pi^{-}]K^{+}","(((mctype==1001 || mctype==1002) || ((mctype<1001 || mctype>1004) && runno==0)) && (mcinfo==59 || mcinfo==61) && (mchash==141183 ||mchash==2513873 ||mchash==2245944 ||mchash==2323258 ||mchash==2068777 ||mchash==1769521 ||mchash==1639018 ||mchash==3905351 ||mchash==2939915 ||mchash==1962594 ||mchash==3500850 ||mchash==1881338))");
        map_counter_color_legend_cut[6] = std::make_tuple(kSpring-5, "Signal","(mctype<1001 || mctype>1004) && runno==0 && (mcinfo==59 || mcinfo==61) && abs(chil0_mo)==511 && abs(chil1_mo)==511 && abs(chil2_mo)==511");
        // Below this line, no changes on this method need to be done by the user
        int hist_num = map_counter_color_legend_cut.size();
        int leg_entries = 0;
        std::vector<TH1*> hists;
        hists.assign(hist_num,NULL);
        THStack *plot_stack = new THStack("plot_stack","plot stack");
        // create histograms and fill to stack
        for(int i = 0; i < hist_num; ++i) {
            hists[i] = data.createHistogram(("plot_"+mp::to_string(i)).c_str(),variable,Binning(opt_bins),Cut((category_cut+std::get<2>(map_counter_color_legend_cut[i])).c_str()),CutRange(range.c_str()));
            if(hists[i]->GetEntries() > 0) {
                hists[i]->SetFillColor(std::get<0>(map_counter_color_legend_cut[i]));
                hists[i]->SetLineColor(std::get<0>(map_counter_color_legend_cut[i]));
                plot_stack->Add(hists[i]);
                ++leg_entries;
            }
        }
        // fill legend in reversed order, so that bottom histogram (which is drawn first) becomes last entry in the legend
        TLegend *leg = new TLegend(0.14,0.94-0.04*leg_entries,0.55,0.94,"","NDC");
        leg->SetBorderSize(0);
        leg->SetFillColor(0);
        leg->SetFillStyle(4000);
        leg->SetTextSize(0.035);
        std::cout << "--- Creating legend ---" << std::endl;
        for(int i = hist_num-1; i >= 0; --i) {
            if(hists[i]->GetEntries() > 0) {
                leg->AddEntry(hists[i],(std::get<1>(map_counter_color_legend_cut[i])).c_str(),"F");
                std::cout << "Filled " << hists[i]->GetEntries() << " " << std::get<1>(map_counter_color_legend_cut[i]) << " entries to the plot." << std::endl;
            }
        }
        std::cout << "------ [x] done -------" << std::endl;
        plot_on->addObject(plot_stack,"HISTsame");
        plot_on->drawBefore("data_hist",plot_stack->GetName());
        plot_on->addObject(leg,"");
        return hists;
    }
    
    // Add to given plot "plot_on" of variable "variable" a stacked histogram of background types in data "data"
    // Use nbins "opt_bins" for the histogram and draw data in CutRange "range". If "add_entries_to_legend" is
    // true, the number of entries are added to the histogram legend.
    // return value is std::pair<TLegend*, std::pair<THStack*, std::vector<TH1*> > > and needs to be deleted by the caller after saving plot
    std::vector<TH1*> AddHistogram_MCInfoType(RooPlot *plot_on, RooDataSet &data, RooRealVar &variable, int opt_bins, std::string range, std::string category_cut, bool add_entries_to_legend) {
        // The map needs to be edited by the user
        // std::map<COUNTER, std::tuple<COLOR, LEGEND-ENTRY, CUT-STRING> >;
        std::map<int , std::tuple<Color_t, std::string, std::string> > map_counter_color_legend_cut;
        map_counter_color_legend_cut[0] = std::make_tuple(kGreen, "q#bar{q} from u,d,s", "mctype==1004"); // uds
        map_counter_color_legend_cut[1] = std::make_tuple(kOrange, "q#bar{q} from c", "mctype==1003");     // charm
        map_counter_color_legend_cut[2] = std::make_tuple(kPink-3, "B^{#pm} #rightarrow c bkg", "mctype==1002"); // charged
        map_counter_color_legend_cut[3] = std::make_tuple(kViolet, "B^{0} #rightarrow c bkg", "mctype==1001");    // mixed
        map_counter_color_legend_cut[4] = std::make_tuple(kAzure-5, "B^{#pm}/B^{0} #rightarrow s bkg",    "(mctype<1001 || mctype>1004) && runno==0 && mcinfo!=59 && mcinfo!=61 && (mcinfo != 37 && mcinfo!=53 && mcinfo != 29)");
        map_counter_color_legend_cut[5] = std::make_tuple(kCyan, "Wrong K and/or #pi hypothesis","(mctype<1001 || mctype>1004) && runno==0 && (mcinfo==37 || mcinfo==53)");
        map_counter_color_legend_cut[6] = std::make_tuple(kMagenta,   "Wrong #phi bkg (f_{#lower[-0.4]{0}}(980)/a_{#lower[-0.4]{0}}(980))",             "(mctype<1001 || mctype>1004) && runno==0 && mcinfo==29 && chil0_mc==45");
        map_counter_color_legend_cut[7] = std::make_tuple(kMagenta-5, "Wrong #phi bkg (K^{+}K^{-})","(mctype<1001 || mctype>1004) && runno==0 && mcinfo==29 && chil0_mc!=45");
        map_counter_color_legend_cut[8] = std::make_tuple(kYellow, "B^{0} #rightarrow D_{s}^{-}K^{+} and D_{s}^{-} #rightarrow #phi#pi^{-}","(mctype<1001 || mctype>1004) && runno==0 && (mcinfo==59 || mcinfo==61) && abs(chil0_mo)!=511");
        map_counter_color_legend_cut[9] = std::make_tuple(kSpring-5, "Signal","(mctype<1001 || mctype>1004) && runno==0 && (mcinfo==59 || mcinfo==61) && abs(chil0_mo)==511");
        map_counter_color_legend_cut[10] = std::make_tuple(kGray, "Data","mctype==-1 && runno!=0");
        // Below this line, no changes on this method need to be done by the user
        int hist_num = map_counter_color_legend_cut.size();
        int leg_entries = 0;
        std::vector<TH1*> hists;
        hists.assign(hist_num,NULL);
        THStack *plot_stack = new THStack("plot_stack","plot stack");
        // create histograms and fill to stack
        for(int i = 0; i < hist_num; ++i) {
            hists[i] = data.createHistogram(("plot_"+mp::to_string(i)).c_str(),variable,Binning(opt_bins,variable.getMin(range.c_str()),variable.getMax(range.c_str())),Cut((category_cut+std::get<2>(map_counter_color_legend_cut[i])).c_str()),CutRange(range.c_str()));
            if(hists[i]->GetEntries() > 0) {
                hists[i]->SetFillColor(std::get<0>(map_counter_color_legend_cut[i]));
                hists[i]->SetLineColor(std::get<0>(map_counter_color_legend_cut[i]));
                plot_stack->Add(hists[i]);
                ++leg_entries;
            }
        }
        // fill legend in reversed order, so that bottom histogram (which is drawn first) becomes last entry in the legend
        TLegend *leg = new TLegend(0.14,0.94-0.04*leg_entries,0.45,0.94,"","NDC");
        leg->SetBorderSize(0);
        leg->SetFillColor(0);
        leg->SetFillStyle(4000);
        leg->SetTextSize(0.035);
        std::cout << "--- Creating legend ---" << std::endl;
        for(int i = hist_num-1; i >= 0; --i) {
            if(hists[i]->GetEntries() > 0) {
                std::string legentry = std::get<1>(map_counter_color_legend_cut[i]);
                if(add_entries_to_legend) {
                    if(data.isWeighted())
                        legentry += " (" + mp::to_string(hists[i]->GetSumOfWeights()) + ")";
                    else
                        legentry += " (" + mp::to_string(hists[i]->GetEntries()) + ")";
                }
                leg->AddEntry(hists[i],legentry.c_str(),"F");
                std::cout << "Filled " << hists[i]->GetEntries() << "(" << hists[i]->GetSumOfWeights() << ")" << std::get<1>(map_counter_color_legend_cut[i]) << " entries to the plot." << std::endl;
            }
        }
        std::cout << "------ [x] done -------" << std::endl;
        plot_on->addObject(plot_stack,"HISTsame");
        plot_on->drawBefore("data_hist",plot_stack->GetName());
        plot_on->addObject(leg,"");
        return hists;
    }
    
    // Create a plot of variable "variable" in data "data" in range "range" with "opt_bins" bins
    // in the folder "opt_outdir" and vizualizes the different components (e.g. signal, SCF, etc...)
    void CreateDataComponentPlots(RooDataSet &data, RooRealVar &variable, int opt_bins, std::string range, std::string opt_outdir,
                                  std::string opt_plot_format, std::string cat_label, std::string cat_name, std::string suffix) {
        RooPlot *plot = variable.frame(Bins(opt_bins), Name("plot"), Title("plot"), Range(range.c_str()));
        TCanvas *canvas = new TCanvas("c_canvas","c_canvas",1280,1024);
        
        // create plot of data and draw it
        data.plotOn(plot, Name("data_hist"), CutRange(range.c_str()), MarkerColor(kBlack),
                    Cut((cat_name+"=="+cat_name+"::"+cat_label).c_str()),
                    DataError(data.isWeighted() ? RooAbsData::SumW2 : RooAbsData::Poisson));
        
        // add additional histograms to the plot
        std::vector<TH1*> to_delete	=
        AddHistogram_MCInfoType(plot, data, variable, opt_bins, range.c_str(), (cat_name+"=="+cat_name+"::"+cat_label+" && ").c_str(), true);
        
        // write decay channel on top of plot
        std::string plotlabel;
        if(cat_label == "C") {
            plotlabel = "B^{0}/#bar{B^{0}} #rightarrow K^{#pm} #pi^{#mp} #pi^{0}";
        } else if (cat_label == "N") {
            plotlabel = "B^{0} #rightarrow #phi K_{s}^{0} #pi^{0}";
        }
        TLatex* text = new TLatex(0.81, 0.91, plotlabel.c_str());
        text->SetTextColor(kBlack);
        text->SetTextFont(42);
        text->SetTextSize(0.032);
        text->SetNDC(1);
        text->SetText(1.0 - 0.05 - 0.01 - text->GetXsize(), 0.91,plotlabel.c_str());
        plot->addObject(text,"");
        
        plot->Draw("");
        // fix y-label for phi angle
        std::string varname = variable.GetName();
        if(varname == "hel_phi") {
            std::stringstream ylabel;
            ylabel << "Events / ( #pi/" << opt_bins/2 << " rad )";
            plot->GetYaxis()->SetTitle(ylabel.str().c_str());
        }
        plot->SetMinimum(0.0);
        plot->SetMaximum(1.5*plot->GetMaximum());
        
        // save final plot
        std::string filename;
        if(cat_label != "")
            filename = opt_outdir + "DATA_" + cat_label + "_" + variable.GetName() + suffix + "."+opt_plot_format+"";
        else
            filename = opt_outdir + "DATA_" + variable.GetName() + suffix + "."+opt_plot_format+"";
        canvas->cd();
        canvas->SaveAs(filename.c_str());
        canvas->SaveAs((filename+".root").c_str());
        delete canvas;
        delete plot;
        
        // now recycle the data and histograms and make normalized distributions of each component
        canvas = new TCanvas("c_canvas","c_canvas",1280,1024);
        int linestyle = 2;
        for(TH1* h : to_delete) {
            h->SetFillStyle(0);
            h->SetLineStyle(linestyle);
            h->SetLineWidth(2);
            linestyle == 4 ? linestyle = 2 : ++linestyle;
        }
        std::vector<TH1*> norm_hist;
        for(TH1* h : to_delete) {
            if(h->GetSumOfWeights() != 0) {
                if(norm_hist.size() == 0) {
                    norm_hist.push_back(h->DrawNormalized("HIST"));
                } else {
                    norm_hist.push_back(h->DrawNormalized("HISTsame"));
                }
            }
        }
        double max = 0;
        for(TH1* h : norm_hist) {
            if(max < h->GetMaximum())
                max = h->GetMaximum();
        }
        if(!norm_hist.empty()) {
            norm_hist.front()->SetMinimum(0.0);
            norm_hist.front()->SetMaximum(1.5*max);
        }
        
        text = new TLatex(0.81,0.91,plotlabel.c_str());
        text->SetTextColor(kBlack);
        text->SetTextFont(42);
        text->SetTextSize(0.032);
        text->SetNDC(1);
        text->DrawLatex(1.0 - 0.05 - 0.01 - text->GetXsize(), 0.91,plotlabel.c_str());
        
        canvas->Update();
        if(cat_label != "")
            filename = opt_outdir + "DATA_NORM_" + cat_label + "_" + variable.GetName() + suffix + "."+opt_plot_format+"";
        else
            filename = opt_outdir + "DATA_NORM_" + variable.GetName() + suffix + "."+opt_plot_format+"";
        canvas->cd();
        canvas->SaveAs(filename.c_str());
        canvas->SaveAs((filename+".root").c_str());
        
        // clean up
        for(TH1* h : to_delete) {
            delete h;
        }
        for(TH1* h : norm_hist) {
            delete h;
        }
        delete text;
        delete canvas;
    }
    
    // writes a RooFitResult "result" to a file "filename" which can be plotted
    // via then Python script utils/plot_correlation_matrix.py
    void WriteCorrelationMatrixFile(RooFitResult *result, std::string filename) {
        if(result != NULL && &(result->correlationMatrix()) != NULL) {
            std::ofstream fout(filename.c_str());
            if(fout.is_open()) {
                result->printArgs(fout);
                fout << std::endl;
                for(int i = 0; i < result->correlationMatrix().GetNcols(); ++i) {
                    for(int j = 0; j < result->correlationMatrix().GetNcols(); ++j) {
                        fout << result->correlationMatrix()[i][j] << " ";
                    }
                    fout << std::endl;
                }
                fout.close();
                
                try {
                    mp::execute_command_throw("./utils/plot_correlation_matrix.py -i "+filename+" -o "+filename+".eps");
                } catch (std::runtime_error &e) {
                    std::cerr << "ERROR: Failed to create correlation matrix plot for " << filename << std::endl;
                }
                
            } else {
                std::cerr << "ERROR: WriteCorrelationMatrixFile(RooFitResult ...) Could not open filestream for " << filename << " to write correlation matrix!" << std::endl;
            }
        } else {
            std::cerr << "ERROR: WriteCorrelationMatrixFile(RooFitResult ...) RooFitResult or its correlation matrix was NULL!" << std::endl;
        }
    }
    
    // writes a RooFitResult "result" to a file "filename" which can be plotted
    // via then Python script utils/plot_correlation_matrix.py
    void WriteCovarianceMatrixFile(RooFitResult *result, std::string filename) {
        if(result != NULL && &(result->covarianceMatrix()) != NULL) {
            std::ofstream fout(filename.c_str());
            if(fout.is_open()) {
                result->printArgs(fout);
                fout << std::endl;
                for(int i = 0; i < result->covarianceMatrix().GetNcols(); ++i) {
                    for(int j = 0; j < result->covarianceMatrix().GetNcols(); ++j) {
                        fout << result->covarianceMatrix()[i][j] << " ";
                    }
                    fout << std::endl;
                }
                fout.close();
            } else {
                std::cerr << "ERROR: WriteCovarianceMatrixFile(RooFitResult ...) Could not open filestream for " << filename << " to write covariance matrix!" << std::endl;
            }
        } else {
            std::cerr << "ERROR: WriteCovarianceMatrixFile(RooFitResult ...) RooFitResult or its covariance matrix was NULL!" << std::endl;
        }
    }
    
    // creates a correlation matrix of the variables in RooArgList "arglist" from RooDataSet "data"
    // and stores results in file "filename" which can be plotted via then Python script utils/plot_correlation_matrix.py
    void WriteCorrelationMatrixFile(RooDataSet &data, RooArgList arglist, std::string filename) {
        // skip empty dataset
        if(!(data.sumEntries() > 0))
            return;
        
        TMatrixDSym &correlationMatrix = *data.correlationMatrix(arglist);
        std::ofstream fout(filename.c_str());
        if(fout.is_open()) {
            fout << "[constPars=(),floatPars=(";
            std::cout << "Variables in correlation matrix..." << std::endl;
            for(int i = 0; i < arglist.getSize(); ++i) {
                if(i > 0)
                    fout << ",";
                fout << arglist[i].GetName();
                std::cout << i << ": " << arglist[i].GetName() << std::endl;
            }
            fout << ")]" << std::endl;
            for(int i = 0; i < correlationMatrix.GetNcols(); ++i) {
                for(int j = 0; j < correlationMatrix.GetNcols(); ++j) {
                    fout << correlationMatrix[i][j] << " ";
                }
                fout << std::endl;
            }
            fout.close();
            correlationMatrix.Print();
            
            try {
                mp::execute_command_throw("./utils/plot_correlation_matrix.py -i "+filename+" -o "+filename+".eps");
            } catch (std::runtime_error &e) {
                std::cerr << "ERROR: Failed to create correlation matrix plot for " << filename << std::endl;
            }
        } else {
            std::cerr << "ERROR: WriteCorrelationMatrixFile(RooDataSet ...) Could not open filestream for " << filename << " to write correlation matrix!" << std::endl;
        }
    }
    
    // creates a CSV file "filename" which has a header line with the names of the variables in RooArgList "arglist"
    // and includes all values for these variables from RooDataSet "data"
    void WriteCorrelationStudyFile(RooDataSet &data, RooArgList arglist, std::string filename) {
        // skip empty dataset
        if(!(data.sumEntries() > 0))
            return;
        
        std::ofstream fout(filename.c_str());
        if(fout.is_open()) {
            for(int i = 0; i < data.numEntries(); ++i) {
                const RooArgSet *values = data.get(i);
                if(i == 0) {
                    for(int j = 0; j < arglist.getSize(); ++j) {
                        if(j > 0)
                            fout << ";";
                        fout << arglist[j].GetName();
                    }
                    fout << std::endl;
                }
                for(int j = 0; j < arglist.getSize(); ++j) {
                    if(j > 0)
                        fout << ";";
                    fout << values->getRealValue(arglist[j].GetName());
                }
                fout << std::endl;
            }
            fout.close();
            
            std::cout << "INFO: Run CAT with the command: " << "./utils/cat_run_correlation_analysis.py -i "+filename+" -o "+filename+".pdf --cleanup" << std::endl;
        } else {
            std::cerr << "ERROR: WriteCorrelationStudyFile(...) Could not open filestream for " << filename << " to write date for correlation study!" << std::endl;
        }
    }
    
    // print some message on std::cout to seperate each fit more clearly in the log file
    void PrintFitStartBlock(const std::string& label, unsigned int dashed_lines) {
        const unsigned int charsperline = 100;
        for(unsigned int i = 0; i < dashed_lines; ++i){
            for(unsigned int j = 0; j < charsperline; ++j)
                std::cout << '-';
            std::cout << std::endl;
        }
        
        if(charsperline >= label.size() +2) {
            unsigned int ndashes = charsperline - 2 - label.size();
            for(unsigned int i = 0; i < ndashes; ++i){
                if(i == ndashes/2) {
                    std::cout << " " << label << " ";
                }
                std::cout << '-';
            }
            std::cout << std::endl;
        } else {
            std::cout << "-- " << label << std::endl;
        }
        
        for(unsigned int i = 0; i < dashed_lines; ++i){
            for(unsigned int j = 0; j < charsperline; ++j)
                std::cout << '-';
            std::cout << std::endl;
        }
    }
    
    void VisualizeNLLCut(shared_ptr<FF_PDF> pdf, RooDataSet &data, RooRealVar &variable, const tuple_category_name_flags& flags,
                         const std::string& range, const std::string& filename, const std::string& opt_plot_format, double nll_quantile_signal) {
        for(unsigned int i = 0; i < std::get<2>(flags).size(); ++i) {
            std::string plotfilename = filename;
            plotfilename.replace(plotfilename.find("."+opt_plot_format+""),1+opt_plot_format.size(),"_"+std::get<2>(flags)[i]+"."+opt_plot_format+"");
            VisualizeNLLCut(pdf->GetSimPDF(),data,variable,
                            std::get<1>(flags),std::get<2>(flags)[i],
                            range,plotfilename, nll_quantile_signal);
        }
    }
    
    void VisualizeNLLCut(RooSimultaneous &pdf, RooDataSet &data, RooRealVar &variable,
                         std::string cat_name, std::string cat_label, std::string range, std::string filename, double nll_quantile_signal) {
        const unsigned int nprob = 9;
        double* prob = NULL;
        double* quant = NULL;
        if(nll_quantile_signal > 0.0 && nll_quantile_signal <= 1.0) {
            prob = new double[nprob];
            prob[0] = 0.5;
            prob[1] = 0.6;
            prob[2] = 0.7;
            prob[3] = 0.75;
            prob[4] = 0.8;
            prob[5] = 0.85;
            prob[6] = 0.9;
            prob[7] = 0.95;
            prob[8] = 1.0;
            quant = new double[nprob];
        } else {
            std::cerr << "ERROR: Requestes NLL quantile needs to be within (0,1.0]. Reset to Median." << std::endl;
            nll_quantile_signal = 0.5;
        }
        
        TCanvas *canvas_nll = new TCanvas("c_canvas_nll","c_canvas_nll",1280,1024);
        canvas_nll->Divide(1,1);
        canvas_nll->cd(1);
        
        std::cout << "INFO: Creating NLL projection for variable " << variable.GetName() << std::endl;
        RooAbsPdf* pdfProj = pdf.createProjection(variable);
        RooFormulaVar nllFunc("nll","-log(L)","-log(@0)",*pdfProj);
        
        RooDataSet* data_signal = dynamic_cast<RooDataSet*>(data.reduce(CutRange(range.c_str()), Cut((cat_name+"=="+cat_name+"::"+cat_label+" && ((mcinfo==59 || mcinfo==61) && abs(chil0_mo)==511)").c_str())));
        RooRealVar* nll_signal = dynamic_cast<RooRealVar*>(data_signal->addColumn(nllFunc));
        RooDataSet* data_bkg = dynamic_cast<RooDataSet*>(data.reduce(CutRange(range.c_str()), Cut((cat_name+"=="+cat_name+"::"+cat_label+" && !((mcinfo==59 || mcinfo==61) && abs(chil0_mo)==511)").c_str())));
        RooRealVar* nll_bkg = dynamic_cast<RooRealVar*>(data_bkg->addColumn(nllFunc));
        
        double low_s, high_s;
        data_signal->getRange(*nll_signal, low_s, high_s);
        double low_b, high_b;
        data_bkg->getRange(*nll_bkg, low_b, high_b);
        
        TH1* h_sig = data_signal->createHistogram("h_sig",*nll_signal,Binning(100,std::min(low_s,low_b),std::max(high_s,high_b)));
        TH1* h_bkg = data_bkg->createHistogram("h_bkg",*nll_bkg,Binning(100,std::min(low_s,low_b),std::max(high_s,high_b)));
        
        THStack *h_stack = new THStack("h_stack","h stack");
        h_sig->SetFillColor(kRed);
        h_sig->SetLineColor(kRed);
        h_stack->Add(h_sig);
        h_bkg->SetFillColor(kBlue);
        h_bkg->SetLineColor(kBlue);
        h_stack->Add(h_bkg);
        h_stack->Draw("HIST");
        std::string xtitle = "-log(L) for projection on ";
        xtitle += variable.GetTitle();
        h_stack->GetXaxis()->SetTitle(xtitle.c_str());
        h_stack->GetYaxis()->SetTitle("Entries");
        
        double max_entries = h_stack->GetMaximum();
        h_stack->SetMaximum(max_entries*1.3);
        
        std::vector<TLine*> quantile_lines;
        h_sig->GetQuantiles(nprob,quant,prob);
        for(unsigned int i = 0; i < nprob; ++i) {
            std::cout << "Quant_" << prob[i] << " at NLL < " << quant[i] << std::endl;
            quantile_lines.push_back(new TLine(quant[i],0.0,quant[i],max_entries*0.8));
            quantile_lines.back()->SetLineWidth(1);
            quantile_lines.back()->SetLineColor(kOrange);
            quantile_lines.back()->SetLineStyle(9);
            quantile_lines.back()->Draw("same");
        }
        double quant_cut;
        h_sig->GetQuantiles(1,&quant_cut,&nll_quantile_signal);
        quantile_lines.push_back(new TLine(quant_cut,0.0,quant_cut,max_entries*0.8));
        quantile_lines.back()->SetLineWidth(1);
        quantile_lines.back()->SetLineColor(kSpring-5);
        quantile_lines.back()->SetLineStyle(kDashed);
        quantile_lines.back()->Draw("same");
        
        TLegend *leg = new TLegend(0.14,0.94-0.04*4,0.45,0.94,"","NDC");
        leg->SetBorderSize(0);
        leg->SetFillColor(0);
        leg->SetFillStyle(4000);
        leg->SetTextSize(0.035);
        leg->AddEntry(h_sig,"Signal","F");
        leg->AddEntry(h_bkg,"Background","F");
        std::stringstream ss;
        ss << "NLL cut for q_{" << nll_quantile_signal << "} is " << quant_cut << std::endl;
        leg->AddEntry(quantile_lines.back(),ss.str().c_str(),"L");
        leg->AddEntry(quantile_lines.front(),"NLL cut for q_{x} | x #in (0.5,0.6,0.7,0.75,0.8,0.85,0.9,0.95,1.0)","L");
        leg->Draw("");
        
        canvas_nll->Update();
        canvas_nll->SaveAs(filename.c_str());
        canvas_nll->cd();
        canvas_nll->SaveAs((filename+".root").c_str());
        
        delete leg;
        for(TLine* l : quantile_lines) {
            delete l;
        }
        delete h_sig;
        delete h_bkg;
        delete h_stack;
        delete data_signal;
        delete data_bkg;
        delete pdfProj;
        delete canvas_nll;
        delete[] quant;
        delete[] prob;
    }
    
    
    void VisualizeLLRCut(shared_ptr<FF_PDF> pdf_component, shared_ptr<FF_PDF> pdf_tot, RooDataSet &data, RooRealVar &variable, const tuple_category_name_flags& flags,
                         const std::string& range, const std::string& filename, const std::string& opt_plot_format, double llr_quantile_signal) {
        for(unsigned int i = 0; i < std::get<2>(flags).size(); ++i) {
            std::string plotfilename = filename;
            plotfilename.replace(plotfilename.find("."+opt_plot_format+""),1+opt_plot_format.size(),"_"+std::get<2>(flags)[i]+"."+opt_plot_format+"");
            VisualizeLLRCut(pdf_component->GetSimPDF(),pdf_tot->GetSimPDF(),data,variable,
                            std::get<1>(flags),std::get<2>(flags)[i],
                            range,plotfilename, llr_quantile_signal);
        }
    }
    
    void VisualizeLLRCut(RooSimultaneous &pdf_component, RooSimultaneous &pdf_tot, RooDataSet &data, RooRealVar &variable,
                         std::string cat_name, std::string cat_label, std::string range, std::string filename, double llr_quantile_signal) {
        const unsigned int nprob = 9;
        double* prob = NULL;
        double* quant = NULL;
        if(llr_quantile_signal > 0.0 && llr_quantile_signal <= 1.0) {
            prob = new double[nprob];
            prob[0] = 0.5;
            prob[1] = 0.4;
            prob[2] = 0.3;
            prob[3] = 0.25;
            prob[4] = 0.2;
            prob[5] = 0.15;
            prob[6] = 0.1;
            prob[7] = 0.05;
            prob[8] = 0.0;
            quant = new double[nprob];
        } else {
            std::cerr << "ERROR: Requestes log(LR) quantile needs to be within (0,1.0]. Reset to Median." << std::endl;
            llr_quantile_signal = 0.5;
        }
        
        TCanvas *canvas_lr = new TCanvas("c_canvas_lr","c_canvas_lr",1280,1024);
        canvas_lr->cd(1);
        
        std::cout << "INFO: Creating log(LR) projection for variable " << variable.GetName() << std::endl;
        RooAbsPdf* pdfProj_component = pdf_component.createProjection(variable);
        RooAbsPdf* pdfProj_tot = pdf_tot.createProjection(variable);
        RooFormulaVar lratioFunc("lratioFunc","log(LR)","log(@0)-log(@1)",RooArgList(*pdfProj_component,*pdfProj_tot));
        
        RooDataSet* data_signal = dynamic_cast<RooDataSet*>(data.reduce(CutRange(range.c_str()), Cut((cat_name+"=="+cat_name+"::"+cat_label+" && ((mcinfo==59 || mcinfo==61) && abs(chil0_mo)==511)").c_str())));
        RooRealVar* lr_signal = dynamic_cast<RooRealVar*>(data_signal->addColumn(lratioFunc));
        RooDataSet* data_bkg = dynamic_cast<RooDataSet*>(data.reduce(CutRange(range.c_str()), Cut((cat_name+"=="+cat_name+"::"+cat_label+" && !((mcinfo==59 || mcinfo==61) && abs(chil0_mo)==511)").c_str())));
        RooRealVar* lr_bkg = dynamic_cast<RooRealVar*>(data_bkg->addColumn(lratioFunc));
        
        double low_s, high_s;
        data_signal->getRange(*lr_signal, low_s, high_s);
        double low_b, high_b;
        data_bkg->getRange(*lr_bkg, low_b, high_b);
        
        TH1* h_sig = data_signal->createHistogram("h_sig",*lr_signal,Binning(100,std::min(low_s,low_b),std::max(high_s,high_b)));
        TH1* h_bkg = data_bkg->createHistogram("h_bkg",*lr_bkg,Binning(100,std::min(low_s,low_b),std::max(high_s,high_b)));
        
        THStack *h_stack = new THStack("h_stack","h stack");
        h_sig->SetFillColor(kRed);
        h_sig->SetLineColor(kRed);
        h_stack->Add(h_sig);
        h_bkg->SetFillColor(kBlue);
        h_bkg->SetLineColor(kBlue);
        h_stack->Add(h_bkg);
        h_stack->Draw("HIST");
        std::string xtitle = "log(LR) for projection on ";
        xtitle += variable.GetTitle();
        h_stack->GetXaxis()->SetTitle(xtitle.c_str());
        h_stack->GetYaxis()->SetTitle("Entries");
        
        double max_entries = h_stack->GetMaximum();
        h_stack->SetMaximum(max_entries*1.3);
        
        std::vector<TLine*> quantile_lines;
        h_sig->GetQuantiles(nprob,quant,prob);
        for(unsigned int i = 0; i < nprob; ++i) {
            std::cout << "Quant_" << 1.0 - prob[i] << " at log(LR) > " << quant[i] << std::endl;
            quantile_lines.push_back(new TLine(quant[i],0.0,quant[i],max_entries*0.8));
            quantile_lines.back()->SetLineWidth(1);
            quantile_lines.back()->SetLineColor(kOrange);
            quantile_lines.back()->SetLineStyle(9);
            quantile_lines.back()->Draw("same");
        }
        double quant_cut;
        llr_quantile_signal = 1.0 - llr_quantile_signal;
        h_sig->GetQuantiles(1,&quant_cut,&llr_quantile_signal);
        quantile_lines.push_back(new TLine(quant_cut,0.0,quant_cut,max_entries*0.8));
        quantile_lines.back()->SetLineWidth(1);
        quantile_lines.back()->SetLineColor(kSpring-5);
        quantile_lines.back()->SetLineStyle(kDashed);
        quantile_lines.back()->Draw("same");
        
        TLegend *leg = new TLegend(0.14,0.94-0.04*4,0.45,0.94,"","NDC");
        leg->SetBorderSize(0);
        leg->SetFillColor(0);
        leg->SetFillStyle(4000);
        leg->SetTextSize(0.035);
        leg->AddEntry(h_sig,"Signal","F");
        leg->AddEntry(h_bkg,"Background","F");
        std::stringstream ss;
        ss << "log(LR) cut for q_{" << 1.0 - llr_quantile_signal << "} is " << quant_cut << std::endl;
        leg->AddEntry(quantile_lines.back(),ss.str().c_str(),"L");
        leg->AddEntry(quantile_lines.front(),"log(LR) cut for q_{x} | x #in (0.5,0.6,0.7,0.75,0.8,0.85,0.9,0.95,1.0)","L");
        leg->Draw("");
        
        canvas_lr->Update();
        canvas_lr->SaveAs(filename.c_str());
        canvas_lr->cd();
        canvas_lr->SaveAs((filename+".root").c_str());
        
        delete leg;
        for(TLine* l : quantile_lines) {
            delete l;
        }
        delete h_sig;
        delete h_bkg;
        delete h_stack;
        delete data_signal;
        delete data_bkg;
        delete pdfProj_component;
        delete pdfProj_tot;
        delete canvas_lr;
        delete[] quant;
        delete[] prob;
    }
    
    void VisualizeFitResultNLL(shared_ptr<FF_PDF> pdf, shared_ptr<FF_PDF> pdf_component, RooDataSet &data, RooRealVar &variable, const tuple_category_name_flags& flags,
                               bool add_all_components, int opt_bins, const std::string& range, const std::string& filename, const std::string& opt_plot_format, const std::vector<double>& nll_cut,
                               bool opt_pull, bool opt_logy, bool opt_mctype, bool opt_mcinfo, bool opt_numcpu) {
        if(nll_cut.size() != std::get<2>(flags).size()) {
            std::cerr << "ERROR: Given size of NLL cuts doesn't equal size of categories to plot! Error occured during creation of " << filename << std::endl;
            return;
        }
        for(unsigned int i = 0; i < std::get<2>(flags).size(); ++i) {
            std::vector< tuple_pdfname_color_style_label > styles;
            styles.push_back(pdf->GetPDFStyle(std::get<2>(flags)[i]));
            
            if(add_all_components) {
                // this adds all other components to the plot
                for(unsigned int j = 0; j < pdf->GetPDFStyles(std::get<2>(flags)[i]).size(); ++j){
                    styles.push_back(pdf->GetPDFStyles(std::get<2>(flags)[i])[j]);
                }
            }
            std::cout << "INFO: Adding components to plot: " << std::endl;
            for(const tuple_pdfname_color_style_label& tuple : styles) {
                std::cout << mp::tuple_to_string(tuple) << "\n";
            }
            
            std::string plotfilename = filename;
            plotfilename.replace(plotfilename.find("."+opt_plot_format+""),1+opt_plot_format.size(),"_"+std::get<2>(flags)[i]+"."+opt_plot_format+"");
            VisualizeFitResultNLL(pdf->GetSimPDF(),pdf_component->GetSimPDF(),data,variable,
                                  styles,*(std::get<0>(pdf->GetCategory())),std::get<1>(flags),std::get<2>(flags)[i],
                                  opt_bins,range,plotfilename, nll_cut.at(i),
                                  opt_pull,opt_logy,opt_mctype,opt_mcinfo,opt_numcpu);
        }
    }
    
    
    void VisualizeFitResultNLL(RooSimultaneous &pdf, RooSimultaneous &pdf_component, RooDataSet &data, RooRealVar &variable,
                               std::vector< tuple_pdfname_color_style_label > &pdf_style, RooCategory &category, std::string cat_name, std::string cat_label,
                               int opt_bins, std::string range, std::string filename, double nll_cut,
                               bool opt_pull, bool opt_logy, bool opt_mctype, bool opt_mcinfo, bool opt_numcpu) {
        RooPlot *plot = variable.frame(Bins(opt_bins), Range(range.c_str()), Name("plot"), Title("plot"));
        TCanvas *canvas;
        // split canvas in case of extra pull distribution
        if(opt_pull) {
            canvas = new TCanvas("c_canvas","c_canvas",1280,1280);
            canvas->Divide(1,2);
            canvas->GetPad(1)->SetPad(0.0,0.25,1,1);
            canvas->GetPad(1)->SetBottomMargin(0.05);
            canvas->GetPad(1)->SetRightMargin(0.05);
            canvas->GetPad(2)->SetPad(0.0,0.0,1,0.25);
            canvas->GetPad(2)->SetBottomMargin(0.32);
            canvas->GetPad(2)->SetRightMargin(0.05);
            canvas->cd(1);
        } else {
            canvas = new TCanvas("c_canvas","c_canvas",1280,1024);
        }
        
        // create projection for this variable, reduce data and prepare NLL cut
        RooAbsPdf* pdfProj_component = pdf_component.createProjection(variable);
        RooArgSet* dep_variables = pdfProj_component->getDependents(&data);
        std::cout << "INFO: Variables used for ProjWData: "; dep_variables->Print();
        RooFormulaVar nllFunc("nll","-log(L)","-log(@0)",*pdfProj_component);
        std::stringstream ss;
        ss << "nll<" << nll_cut;
        std::string nll_cut_string = ss.str();
        RooDataSet* data_nll = dynamic_cast<RooDataSet*>(data.reduce(CutRange(range.c_str())));
        data_nll->addColumn(nllFunc); // the returned RooAbsArg belongs to data_nll and is cleaned up there
        
        // TODO: In low statistics case, we could need the ToyMC trick for plotting this
        //	std::cout << "TOYMC" << std::endl;
        //	RooArgSet* all_vars = pdf.getDependents(&data);
        //	RooDataSet* mcprojData_cat = pdf.getPdf(cat_label.c_str())->generate(*all_vars,50000);
        //	RooDataSet* mcprojData = new RooDataSet("mcprojData","mcprojData",*all_vars,Index(category),Import(cat_label.c_str(),*mcprojData_cat));
        //	mcprojData->Print();
        //	mcprojData->addColumn(nllFunc);
        //	mcprojData->Print();
        //	RooDataSet* mcprojDataSel = dynamic_cast<RooDataSet*>(mcprojData->reduce(Cut(nll_cut_string.c_str())));
        //	mcprojDataSel->Print();
        //	std::cout << "TOYMC" << std::endl;
        
        // create reduced data set for drawing projections
        RooDataSet* data_reduced = dynamic_cast<RooDataSet*>(data_nll->reduce(Cut(nll_cut_string.c_str())));
        std::cout << "INFO: NLL cut for plot " << filename << " was: " << nll_cut_string << std::endl;
        // create plot of data and fits with NLL cut
        data_reduced->plotOn(plot, Name("data_hist"), CutRange(range.c_str()), Cut((cat_name+"=="+cat_name+"::"+cat_label+" && "+nll_cut_string).c_str()),
                             MarkerColor(kBlack), DataError(data_reduced->isWeighted() ? RooAbsData::SumW2 : RooAbsData::Poisson));
        // TODO: Make an option for binned dataset
        // RooAbsData* data_reduced_binned = data_reduced->binnedClone(); // FIXME
        // TODO: Use RooFitResult to visualize PDF uncertainties
        // this is the name of the main pdf, for which we want pulls, chi2, etc... (we need this label below)
        std::string main_pdf_name = "";
        for(unsigned int i = 0; i < pdf_style.size(); ++i) {
            std::string name = "component_"+mp::to_string(i);
            if(i == 0)
                main_pdf_name = name;
            std::string component = std::get<0>(pdf_style[i]);
            Color_t color = std::get<1>(pdf_style[i]);
            Style_t style = std::get<2>(pdf_style[i]);
            std::cout << "PLOTTING COMPONENT: " << component << std::endl; 	//FIXME
            pdf.plotOn(plot, Name(name.c_str()), Slice(category,cat_label.c_str()),
                       ProjWData(*dep_variables, *data_reduced), NumCPU(opt_numcpu), Components(component.c_str()), LineColor(color), LineStyle(style));
            //		pdf.plotOn(plot, Name(name.c_str()), Slice(category,cat_label.c_str()),
            //			ProjWData(*dep_variables, *data_reduced_binned), Components(component.c_str()), LineColor(color), LineStyle(style));  // FIXME
            // FIXME currently we cant plot other components, as their normalization is always wrong
            break;
        }
        //delete data_reduced_binned; // FIXME
        // print chi2 and parameter info
        // TODO: Check for proper chi2 and probabilty of unbinned fit... this is not yet correct...
        // TODO: Calc chi2 for pull distribution with its nbins bins and dgfs..
        std::cout << "INFO BLOCK FOR: " << variable.GetName() << std::endl;
        std::cout << "  chi^2 = " << plot->chiSquare(main_pdf_name.c_str(),"data_hist") << std::endl;
        std::cout << "  ndf = " << opt_bins-pdf.getParameters(data)->getSize() << std::endl;
        std::cout << "  chi^2/ndf = " << plot->chiSquare(main_pdf_name.c_str(),"data_hist",pdf.getParameters(data_reduced)->getSize()) << std::endl;
        std::cout << "  Prob = " << TMath::Prob(plot->chiSquare(main_pdf_name.c_str(),"data_hist"),opt_bins-pdf.getParameters(data_reduced)->getSize()) << std::endl;
        // set log scale if option selected
        canvas->cd(1)->SetLogy(opt_logy);
        // draw final plot (that's it if no extra options are given)
        plot->Draw("");
        // write decay channel on top of plot
        TLatex text;
        text.SetTextColor(kBlack);
        text.SetTextFont(42);
        if(opt_pull)
            text.SetTextSize(0.035);
        else
            text.SetTextSize(0.032);
        text.SetNDC(1);
        if(cat_label == "C") {
            text.DrawLatex(0.81, 0.91,"B^{0}/#bar{B^{0}} #rightarrow K^{#pm} #pi^{#mp} #pi^{0}");
        } else if (cat_label == "N") {
            text.DrawLatex(0.81, 0.91,"B^{0} #rightarrow #phi K_{s}^{0} #pi^{0}");
        } else {
            text.DrawLatex(0.81, 0.91,"Unknown");
        }
        
        // add additional histograms to the plot
        std::vector<TH1*> to_delete;
        if(opt_mctype && !opt_mcinfo)
            to_delete = AddHistogram_MCType(plot, *data_reduced, variable, opt_bins, range.c_str(), (cat_name+"=="+cat_name+"::"+cat_label+" && "+nll_cut_string+" && ").c_str());
        if(opt_mcinfo && !opt_mctype)
            to_delete = AddHistogram_MCInfo(plot, *data_reduced, variable, opt_bins, range.c_str(), (cat_name+"=="+cat_name+"::"+cat_label+" && "+nll_cut_string+" && ").c_str());
        if(opt_mcinfo && opt_mctype)
            to_delete = AddHistogram_MCInfoType(plot, *data_reduced, variable, opt_bins, range.c_str(), (cat_name+"=="+cat_name+"::"+cat_label+" && "+nll_cut_string+" && ").c_str());
        // add pull distribution plot
        TH1F *hist_pulls = (TH1F*)NULL;
        RooPlot* plot_pulls = (RooPlot*)NULL;
        if(opt_pull) {
            plot->SetXTitle("");
            canvas->cd(2);
            
            // TODO: Create a method that uses fit error instead of data error for the pull histogram
            // create pull distribution
            RooHist* pulls = plot->pullHist("data_hist",main_pdf_name.c_str()); // will be deleted from plot_pulls
            plot_pulls = variable.frame(Name("Pull_distribution"), Title("Pull distribution"), Range(range.c_str()));
            // create histogram with pull distribution (for nice overlay)
            hist_pulls = new TH1F("hist_pulls","hist pulls",opt_bins,variable.getMin(range.c_str()),variable.getMax(range.c_str()));
            double *pull_values = pulls->GetY();
            double xerr = (variable.getMax(range.c_str())-variable.getMin(range.c_str()))/(2*opt_bins);
            for(int i = 0; i < opt_bins; ++i) {
                hist_pulls->SetBinContent(i+1,pull_values[i]);
                pulls->SetPointEXlow(i,xerr);
                pulls->SetPointEXhigh(i,xerr);
            }
            plot_pulls->addPlotable(pulls,"PE1");
            
            // the pull distribution needs different sizes (ROOT's annoying relative size behavior)
            plot_pulls->GetYaxis()->SetTitle("Pull");
            plot_pulls->GetYaxis()->CenterTitle();
            plot_pulls->GetXaxis()->SetTitleSize(0.12);
            plot_pulls->GetYaxis()->SetTitleSize(0.12);
            plot_pulls->GetYaxis()->SetTitleOffset(0.44);
            plot_pulls->GetXaxis()->SetLabelSize(0.12);
            plot_pulls->GetYaxis()->SetLabelSize(0.12);
            plot_pulls->GetYaxis()->SetLabelOffset(0.012);
            plot_pulls->GetXaxis()->SetTickLength( plot_pulls->GetXaxis()->GetTickLength() * 3.0 );
            plot_pulls->GetYaxis()->SetNdivisions(505);
            
            // draw pull distribution with extra histogram
            plot_pulls->Draw("");
            hist_pulls->SetFillColor(kOrange);
            hist_pulls->SetLineColor(kOrange);
            hist_pulls->Draw("HISTsame");
            plot_pulls->Draw("same");
        }
        plot->SetMinimum(0.0);
        plot->SetMaximum(1.5*plot->GetMaximum());
        
        // save final plot
        canvas->cd();
        canvas->SaveAs(filename.c_str());
        canvas->SaveAs((filename+".root").c_str());
        // clean up
        for(TH1* h : to_delete) {
            delete h;
        }
        if(plot_pulls != NULL)
            delete plot_pulls;
        if(hist_pulls != NULL)
            delete hist_pulls;
        delete canvas;
        delete plot;
        delete data_reduced;
        delete data_nll;
        delete pdfProj_component;
        delete dep_variables;
        
        //FIXME
        //	delete mcprojData;
        //	delete mcprojDataSel;
    }
    
    void VisualizeFitResultLLR(shared_ptr<FF_PDF> pdf, shared_ptr<FF_PDF> pdf_component, RooDataSet &data, RooRealVar &variable, const tuple_category_name_flags& flags,
                               bool add_all_components, int opt_bins, const std::string& range, const std::string& filename,  const std::string& opt_plot_format, const std::vector<double>& llr_cut,
                               bool opt_pull, bool opt_logy, bool opt_mctype, bool opt_mcinfo, bool opt_numcpu) {
        if(llr_cut.size() != std::get<2>(flags).size()) {
            std::cerr << "ERROR: Given size of LLR cuts doesn't equal size of categories to plot! Error occured during creation of " << filename << std::endl;
            return;
        }
        for(unsigned int i = 0; i < std::get<2>(flags).size(); ++i) {
            std::vector< tuple_pdfname_color_style_label > styles;
            styles.push_back(pdf->GetPDFStyle(std::get<2>(flags)[i]));
            
            if(add_all_components) {
                // this adds all other components to the plot
                for(unsigned int j = 0; j < pdf->GetPDFStyles(std::get<2>(flags)[i]).size(); ++j){
                    styles.push_back(pdf->GetPDFStyles(std::get<2>(flags)[i])[j]);
                }
            }
            std::cout << "INFO: Adding components to plot: " << std::endl;
            for(const tuple_pdfname_color_style_label& tuple : styles) {
                std::cout << mp::tuple_to_string(tuple) << "\n";
            }
            
            std::string plotfilename = filename;
            plotfilename.replace(plotfilename.find("."+opt_plot_format+""),1+opt_plot_format.size(),"_"+std::get<2>(flags)[i]+"."+opt_plot_format+"");
            VisualizeFitResultLLR(pdf->GetSimPDF(),pdf_component->GetSimPDF(),data,variable,
                                  styles,*(std::get<0>(pdf->GetCategory())),std::get<1>(flags),std::get<2>(flags)[i],
                                  opt_bins,range,plotfilename, llr_cut.at(i),
                                  opt_pull,opt_logy,opt_mctype,opt_mcinfo,opt_numcpu);
        }
    }
    
    
    void VisualizeFitResultLLR(RooSimultaneous &pdf, RooSimultaneous &pdf_component, RooDataSet &data, RooRealVar &variable,
                               std::vector< tuple_pdfname_color_style_label > &pdf_style, RooCategory &category, std::string cat_name, std::string cat_label,
                               int opt_bins, std::string range, std::string filename, double llr_cut,
                               bool opt_pull, bool opt_logy, bool opt_mctype, bool opt_mcinfo, bool opt_numcpu) {
        RooPlot *plot = variable.frame(Bins(opt_bins), Range(range.c_str()), Name("plot"), Title("plot"));
        TCanvas *canvas;
        // split canvas in case of extra pull distribution
        if(opt_pull) {
            canvas = new TCanvas("c_canvas","c_canvas",1280,1280);
            canvas->Divide(1,2);
            canvas->GetPad(1)->SetPad(0.0,0.25,1,1);
            canvas->GetPad(1)->SetBottomMargin(0.05);
            canvas->GetPad(1)->SetRightMargin(0.05);
            canvas->GetPad(2)->SetPad(0.0,0.0,1,0.25);
            canvas->GetPad(2)->SetBottomMargin(0.32);
            canvas->GetPad(2)->SetRightMargin(0.05);
            canvas->cd(1);
        } else {
            canvas = new TCanvas("c_canvas","c_canvas",1280,1024);
        }
        
        // create projection for this variable, reduce data and prepare NLL cut
        RooAbsPdf* pdfProj_component = pdf_component.createProjection(variable);
        RooAbsPdf* pdfProj_tot = pdf.createProjection(variable);
        RooArgSet* dep_variables = pdfProj_component->getDependents(&data);
        std::cout << "INFO: Variables used for ProjWData: "; dep_variables->Print();
        RooFormulaVar lratioFunc("llr","log(LR)","log(@0)-log(@1)",RooArgList(*pdfProj_component,*pdfProj_tot));
        std::stringstream ss;
        ss << "llr>" << llr_cut;
        std::string llr_cut_string = ss.str();
        RooDataSet* data_llr = dynamic_cast<RooDataSet*>(data.reduce(CutRange(range.c_str())));
        data_llr->addColumn(lratioFunc); // the returned RooAbsArg belongs to data_nll and is cleaned up there
        
        // TODO: In low statistics case, we could need the ToyMC trick for plotting this
        //	std::cout << "TOYMC" << std::endl;
        //	RooArgSet* all_vars = pdf.getDependents(&data);
        //	RooDataSet* mcprojData_cat = pdf.getPdf(cat_label.c_str())->generate(*all_vars,50000);
        //	RooDataSet* mcprojData = new RooDataSet("mcprojData","mcprojData",*all_vars,Index(category),Import(cat_label.c_str(),*mcprojData_cat));
        //	mcprojData->Print();
        //	mcprojData->addColumn(nllFunc);
        //	mcprojData->Print();
        //	RooDataSet* mcprojDataSel = dynamic_cast<RooDataSet*>(mcprojData->reduce(Cut(nll_cut_string.c_str())));
        //	mcprojDataSel->Print();
        //	std::cout << "TOYMC" << std::endl;
        
        // create reduced data set for drawing projections
        RooDataSet* data_reduced = dynamic_cast<RooDataSet*>(data_llr->reduce(Cut(llr_cut_string.c_str())));
        std::cout << "INFO: NLL cut for plot " << filename << " was: " << llr_cut_string << std::endl;
        // create plot of data and fits with NLL cut
        data_reduced->plotOn(plot, Name("data_hist"), CutRange(range.c_str()), Cut((cat_name+"=="+cat_name+"::"+cat_label+" && "+llr_cut_string).c_str()),
                             MarkerColor(kBlack), DataError(data_reduced->isWeighted() ? RooAbsData::SumW2 : RooAbsData::Poisson));
        // TODO: Make an option for binned dataset
        //RooAbsData* data_reduced_binned = data_reduced->binnedClone(); // FIXME
        // TODO: Use RooFitResult to visualize PDF uncertainties
        // this is the name of the main pdf, for which we want pulls, chi2, etc... (we need this label below)
        std::string main_pdf_name = "";
        for(unsigned int i = 0; i < pdf_style.size(); ++i) {
            std::string name = "component_"+mp::to_string(i);
            if(i == 0)
                main_pdf_name = name;
            std::string component = std::get<0>(pdf_style[i]);
            Color_t color = std::get<1>(pdf_style[i]);
            Style_t style = std::get<2>(pdf_style[i]);
            std::cout << "PLOTTING COMPONENT: " << component << std::endl; 	//FIXME
            pdf.plotOn(plot, Name(name.c_str()), Slice(category,cat_label.c_str()),
                       ProjWData(*dep_variables, *data_reduced), NumCPU(opt_numcpu), Components(component.c_str()), LineColor(color), LineStyle(style));
            //		pdf.plotOn(plot, Name(name.c_str()), Slice(category,cat_label.c_str()),
            //			ProjWData(*dep_variables, *data_reduced_binned), Components(component.c_str()), LineColor(color), LineStyle(style)); // FIXME
            // FIXME currently we cant plot other components, as their normalization is always wrong
            break;
        }
        //delete data_reduced_binned; // FIXME
        // print chi2 and parameter info
        // TODO: Check for proper chi2 and probabilty of unbinned fit... this is not yet correct...
        // TODO: Calc chi2 for pull distribution with its nbins bins and dgfs..
        std::cout << "INFO BLOCK FOR: " << variable.GetName() << std::endl;
        std::cout << "  chi^2 = " << plot->chiSquare(main_pdf_name.c_str(),"data_hist") << std::endl;
        std::cout << "  ndf = " << opt_bins-pdf.getParameters(data)->getSize() << std::endl;
        std::cout << "  chi^2/ndf = " << plot->chiSquare(main_pdf_name.c_str(),"data_hist",pdf.getParameters(data_reduced)->getSize()) << std::endl;
        std::cout << "  Prob = " << TMath::Prob(plot->chiSquare(main_pdf_name.c_str(),"data_hist"),opt_bins-pdf.getParameters(data_reduced)->getSize()) << std::endl;
        // set log scale if option selected
        canvas->cd(1)->SetLogy(opt_logy);
        // draw final plot (that's it if no extra options are given)
        plot->Draw("");
        // write decay channel on top of plot
        TLatex text;
        text.SetTextColor(kBlack);
        text.SetTextFont(42);
        if(opt_pull)
            text.SetTextSize(0.035);
        else
            text.SetTextSize(0.032);
        text.SetNDC(1);
        if(cat_label == "C") {
            text.DrawLatex(0.81, 0.91,"B^{0}/#bar{B^{0}} #rightarrow K^{#pm} #pi^{#mp} #pi^{0}");
        } else if (cat_label == "N") {
            text.DrawLatex(0.81, 0.91,"B^{0} #rightarrow #phi K_{s}^{0} #pi^{0}");
        } else {
            text.DrawLatex(0.81, 0.91,"Unknown");
        }
        
        // add additional histograms to the plot
        std::vector<TH1*> to_delete;
        if(opt_mctype && !opt_mcinfo)
            to_delete = AddHistogram_MCType(plot, *data_reduced, variable, opt_bins, range.c_str(), (cat_name+"=="+cat_name+"::"+cat_label+" && "+llr_cut_string+" && ").c_str());
        if(opt_mcinfo && !opt_mctype)
            to_delete = AddHistogram_MCInfo(plot, *data_reduced, variable, opt_bins, range.c_str(), (cat_name+"=="+cat_name+"::"+cat_label+" && "+llr_cut_string+" && ").c_str());
        if(opt_mcinfo && opt_mctype)
            to_delete = AddHistogram_MCInfoType(plot, *data_reduced, variable, opt_bins, range.c_str(), (cat_name+"=="+cat_name+"::"+cat_label+" && "+llr_cut_string+" && ").c_str());
        // add pull distribution plot
        TH1F *hist_pulls = (TH1F*)NULL;
        RooPlot* plot_pulls = (RooPlot*)NULL;
        if(opt_pull) {
            plot->SetXTitle("");
            canvas->cd(2);
            
            // TODO: Create a method that uses fit error instead of data error for the pull histogram
            // create pull distribution
            RooHist* pulls = plot->pullHist("data_hist",main_pdf_name.c_str()); // will be deleted from plot_pulls
            plot_pulls = variable.frame(Name("Pull_distribution"), Title("Pull distribution"), Range(range.c_str()));
            // create histogram with pull distribution (for nice overlay)
            hist_pulls = new TH1F("hist_pulls","hist pulls",opt_bins,variable.getMin(range.c_str()),variable.getMax(range.c_str()));
            double *pull_values = pulls->GetY();
            double xerr = (variable.getMax(range.c_str())-variable.getMin(range.c_str()))/(2*opt_bins);
            for(int i = 0; i < opt_bins; ++i) {
                hist_pulls->SetBinContent(i+1,pull_values[i]);
                pulls->SetPointEXlow(i,xerr);
                pulls->SetPointEXhigh(i,xerr);
            }
            plot_pulls->addPlotable(pulls,"PE1");
            
            // the pull distribution needs different sizes (ROOT's annoying relative size behavior)
            plot_pulls->GetYaxis()->SetTitle("Pull");
            plot_pulls->GetYaxis()->CenterTitle();
            plot_pulls->GetXaxis()->SetTitleSize(0.12);
            plot_pulls->GetYaxis()->SetTitleSize(0.12);
            plot_pulls->GetYaxis()->SetTitleOffset(0.44);
            plot_pulls->GetXaxis()->SetLabelSize(0.12);
            plot_pulls->GetYaxis()->SetLabelSize(0.12);
            plot_pulls->GetYaxis()->SetLabelOffset(0.012);
            plot_pulls->GetXaxis()->SetTickLength( plot_pulls->GetXaxis()->GetTickLength() * 3.0 );
            plot_pulls->GetYaxis()->SetNdivisions(505);
            
            // draw pull distribution with extra histogram
            plot_pulls->Draw("");
            hist_pulls->SetFillColor(kOrange);
            hist_pulls->SetLineColor(kOrange);
            hist_pulls->Draw("HISTsame");
            plot_pulls->Draw("same");
        }
        plot->SetMinimum(0.0);
        plot->SetMaximum(1.5*plot->GetMaximum());
        
        // save final plot
        canvas->cd();
        canvas->SaveAs(filename.c_str());
        canvas->SaveAs((filename+".root").c_str());
        // clean up
        for(TH1* h : to_delete) {
            delete h;
        }
        if(plot_pulls != NULL)
            delete plot_pulls;
        if(hist_pulls != NULL)
            delete hist_pulls;
        delete canvas;
        delete plot;
        delete data_reduced;
        delete data_llr;
        delete pdfProj_component;
        delete dep_variables;
        
        //FIXME
        //	delete mcprojData;
        //	delete mcprojDataSel;
    }
    
    void VisualizeFitResult(shared_ptr<FF_PDF> pdf, RooDataSet &data, RooRealVar &variable, const tuple_category_name_flags& flags,
                            bool add_all_components, int opt_bins, const std::string& range, const std::string& filename, const std::string& opt_plot_format,
                            bool opt_pull, bool opt_logy, bool opt_mctype, bool opt_mcinfo, bool opt_use_fit_error_for_pull, bool opt_pdf_legend) {
        for(unsigned int i = 0; i < std::get<2>(flags).size(); ++i) {
            std::string cat_name = std::get<1>(flags);
            std::string cat_label = std::get<2>(flags)[i];
            std::cout << "INFO: Visualizing " << cat_label << " for " << variable.GetName() << " of " << pdf->GetSimPDFLabel() << std::endl;
            if(pdf->HasPDF(cat_label)) {
                std::vector< tuple_pdfname_color_style_label > styles;
                styles.push_back(pdf->GetPDFStyle(cat_label));
                
                if(add_all_components) {
                    // this adds all other components to the plot
                    std::vector<FFUtil::tuple_pdfname_color_style_label> all_pdf_styles = pdf->GetPDFStyles(cat_label,false); // set to true for adding ALL pdf components
                    for(unsigned int j = 0; j < all_pdf_styles.size(); ++j){
                        bool duplicate = false;
                        for(unsigned int k = 0; k < styles.size(); ++k) {
                            if(std::get<0>(styles[k]) == std::get<0>(all_pdf_styles[j]))
                                duplicate = true;
                        }
                        if(!duplicate)
                            styles.push_back(all_pdf_styles[j]);
                        else
                            std::cout << "INFO: Didn't add a duplicate off: " << std::get<0>(all_pdf_styles[j]) << std::endl;
                    }
                }
                std::cout << "INFO: Adding components to plot: " << std::endl;
                for(const tuple_pdfname_color_style_label& tuple : styles) {
                    std::cout << mp::tuple_to_string(tuple) << "\n";
                }
                
                std::string plotfilename = filename;
                plotfilename.replace(plotfilename.find("."+opt_plot_format+""),1+opt_plot_format.size(),"_"+cat_label+"."+opt_plot_format+"");
                
                std::string plotlabel = "";
                if(cat_label == "CP") {
                    plotlabel = "B^{0} #rightarrow K^{+} #pi^{-} #pi^{0}";
                } else if (cat_label == "CA") {
                    plotlabel = "#bar{B^{0}} #rightarrow K^{-} #pi^{+} #pi^{0}";
                } else if(cat_label == "N") {
                    plotlabel = "B^{0} #rightarrow #phi K_{s}^{0} #pi^{0}";
                } else if (cat_label == "C") {
                    plotlabel = "";//"B^{0}/#bar{B^{0}} #rightarrow K^{#pm} #pi^{#mp} #pi^{0}";
                } else if (cat_label == "CS" || cat_label == "COFF") {
                    plotlabel = "B^{0}/#bar{B^{0}} #rightarrow K^{#pm} #pi^{#mp} #pi^{0}";
                } else if (cat_label == "NS" || cat_label == "NOFF") {
                    plotlabel = "B^{0} #rightarrow #phi K_{s}^{0} #pi^{0}";
                }
                std::cout << "INFO: Using data " << cat_name+"=="+cat_name+"::"+cat_label << std::endl;
                RooDataSet* data_plot = dynamic_cast<RooDataSet*>(data.reduce(Cut((cat_name+"=="+cat_name+"::"+cat_label).c_str())));
                data_plot->Print();
                if(data_plot->sumEntries() > 0) {
                    VisualizeFitResult(pdf->GetPDF(cat_label),*data_plot,variable,styles,
                                       opt_bins,range,plotfilename,plotlabel,opt_pull,opt_logy,opt_mctype,opt_mcinfo,opt_use_fit_error_for_pull,opt_pdf_legend);
                } else {
                    std::cout << "INFO: Skipping plot as data category " << cat_label << " has zero entries" << std::endl;
                }
                delete data_plot;
            } else {
                std::cout << "INFO: Skipping plot there seems to be no PDF for category " << cat_label << std::endl;
            }
        }
    }
    
    void VisualizeFitResult(RooAbsPdf &pdf, RooDataSet &data, RooRealVar &variable,
                            const tuple_pdfname_color_style_label &pdf_style, int opt_bins, std::string range, std::string filename, std::string plotlabel,
                            bool opt_pull, bool opt_logy, bool opt_mctype, bool opt_mcinfo, bool opt_use_fit_error_for_pull, bool opt_pdf_legend) {
        std::vector< tuple_pdfname_color_style_label > styles;
        styles.push_back(pdf_style);
        VisualizeFitResult(pdf,data,variable,styles,opt_bins,range,filename,plotlabel,opt_pull,opt_logy,opt_mctype,opt_mcinfo,opt_use_fit_error_for_pull,opt_pdf_legend);
    }
    
    // Visualizes the results of a fitted PDF "pdf" on a data set "data" for variable "variable"
    // Draws all pdfs given in pdf_style vector with given color and style
    // Use nbins "opt_bins" and the CutRange/ProjectionRange "range" and store plot to file "filename"
    // Addition options are:
    //   opt_pull   --> Adds pull distribution
    //   opt_logy   --> Sets log-scale on y-axis
    //   opt_mctype --> Calls AddHistogram_MCType(..) to add MC type histograms
    //   opt_mcinfo --> Calls AddHistogram_MCInfo(..) to add MC info histograms
    void VisualizeFitResult(RooAbsPdf &pdf, RooDataSet &data, RooRealVar &variable,
                            std::vector< tuple_pdfname_color_style_label > &pdf_style, int opt_bins, std::string range, std::string filename, std::string plotlabel,
                            bool opt_pull, bool opt_logy, bool opt_mctype, bool opt_mcinfo, bool opt_use_fit_error_for_pull, bool opt_pdf_legend) {
        std::string varname = variable.GetName();
        // Note: This is a analysis specific fix: Use 40 bins due to hel_th1 cut
        if(varname == "hel_th1" || varname == "hel_th2" || varname == "nb_prime") {
            opt_bins = 60;
        }
        RooPlot *plot = variable.frame(Bins(opt_bins), Range(range.c_str()), Name("plot"), Title("plot"));
        TCanvas *canvas;
        // split canvas in case of extra pull distribution
        if(opt_pull) {
            canvas = new TCanvas("c_canvas","c_canvas",1280,1280);
            canvas->Divide(1,2);
            canvas->GetPad(1)->SetPad(0.0,0.25,1,1);
            canvas->GetPad(1)->SetBottomMargin(0.05);
            canvas->GetPad(1)->SetRightMargin(0.05);
            canvas->GetPad(2)->SetPad(0.0,0.0,1,0.25);
            canvas->GetPad(2)->SetBottomMargin(0.35);
            canvas->GetPad(2)->SetRightMargin(0.05);
            canvas->cd(1);
        } else {
            canvas = new TCanvas("c_canvas","c_canvas",1280,1024);
        }
        // create plot of data and fits
        data.plotOn(plot, Name("data_hist"), CutRange(range.c_str()), MarkerColor(kBlack),
                    DataError(data.isWeighted() ? RooAbsData::SumW2 : RooAbsData::Poisson));
        // create legend for fitted curves
        int leg_entries = pdf_style.size();
        TLegend *leg = new TLegend(0.69,0.88-0.04*leg_entries,0.94,0.88,"","NDC");
        leg->SetBorderSize(0);
        leg->SetFillColor(0);
        leg->SetFillStyle(4000);
        leg->SetTextSize(0.035);
        // TODO: Use RooFitResult to visualize PDF uncertainties
        // this is the name of the main pdf, for which we want pulls, chi2, etc... (we need this label below)
        std::string main_pdf_name = "";
        
        for(unsigned int i = 0; i < pdf_style.size(); ++i) {
            std::string name = "component_"+mp::to_string(i);
            if(i == 0)
                main_pdf_name = name;
            std::string component = std::get<0>(pdf_style[i]);
            Color_t color = std::get<1>(pdf_style[i]);
            Style_t style = std::get<2>(pdf_style[i]);
            std::string label = std::get<3>(pdf_style[i]);
            pdf.plotOn(plot, Name(name.c_str()), ProjectionRange(range.c_str()), Components(component.c_str()), LineColor(color), LineStyle(style));
            RooCurve* curve = plot->getCurve(name.c_str());
            leg->AddEntry(curve,label.c_str(),"L");
        }
        // print chi2 and parameter info
        std::cout << "INFO BLOCK FOR: " << variable.GetName() << std::endl;
        std::cout << "  chi^2 = " << plot->chiSquare(main_pdf_name.c_str(),"data_hist") * (opt_bins - pdf.getParameters(data)->getSize()) << std::endl;
        std::cout << "  param = " << pdf.getParameters(data)->getSize() << std::endl;
        std::cout << "  bins = " << opt_bins << std::endl;
        std::cout << "  ndf = " << opt_bins-pdf.getParameters(data)->getSize() << std::endl;
        std::cout << "  chi^2/ndf = " << plot->chiSquare(main_pdf_name.c_str(),"data_hist",pdf.getParameters(data)->getSize()) << std::endl;
        std::cout << "  Prob = " << TMath::Prob(plot->chiSquare(main_pdf_name.c_str(),"data_hist") * (opt_bins - pdf.getParameters(data)->getSize()),
                                                opt_bins-pdf.getParameters(data)->getSize()) << std::endl;
        
        
        // draw also the legend
        if(opt_pdf_legend)
            plot->addObject(leg,"");
        else
            delete leg;
        // write label into upper right of plot
        TLatex* text = new TLatex(0.81, 0.91, plotlabel.c_str());
        text->SetTextColor(kBlack);
        text->SetTextFont(42);
        if(opt_pull)
            text->SetTextSize(0.035);
        else
            text->SetTextSize(0.032);
        text->SetNDC(1);
        text->SetText(1.0 - 0.05 - 0.01 - text->GetXsize(), 0.91, plotlabel.c_str());
        plot->addObject(text,"");
        
        // add additional histograms to the plot
        std::vector<TH1*> to_delete;
        if(opt_mctype && !opt_mcinfo)
            to_delete = AddHistogram_MCType(plot, data, variable, opt_bins, range.c_str());
        if(opt_mcinfo && !opt_mctype)
            to_delete = AddHistogram_MCInfo(plot, data, variable, opt_bins, range.c_str());
        if(opt_mcinfo && opt_mctype)
            to_delete = AddHistogram_MCInfoType(plot, data, variable, opt_bins, range.c_str());
        
        // draw final plot (that's it if no extra options are given)
        plot->Print();
        plot->Draw("");
        // set log scale if option selected
		canvas->cd(1)->SetLogy(opt_logy);
        // Note: This is a analysis specific fix: fix y-label for phi angle
        if(varname == "hel_phi") {
            std::stringstream ylabel;
            ylabel << "Events / ( #pi/" << opt_bins/2 << " rad )";
            plot->GetYaxis()->SetTitle(ylabel.str().c_str());
        }
        plot->SetMinimum(0.0);
        plot->SetMaximum(1.618*plot->GetMaximum());
        
        // add pull distribution plot
        RooPlot* plot_pulls = (RooPlot*)NULL;
        if(opt_pull) {
            plot->SetXTitle("");
            canvas->cd(2);
            
            // create pull distribution
            RooHist* pulls = (RooHist*)NULL;
            if(opt_use_fit_error_for_pull) {
                pulls = plot->residHist("data_hist",main_pdf_name.c_str()); // will be deleted from plot_pulls
                RooCurve* curve = plot->getCurve(main_pdf_name.c_str());
                double* x_values = pulls->GetX();
                double* y_values = pulls->GetY();
                double xerr = (variable.getMax(range.c_str())-variable.getMin(range.c_str()))/(2*opt_bins);
                for(int i = 0; i < opt_bins; ++i) {
                    double fit_value = curve->average(x_values[i]-xerr,x_values[i]+xerr);
                    if(fit_value > 0) {
                        y_values[i] /= std::sqrt(fit_value);
                    } else {
                        y_values[i] = 0;
                    }
                }
            } else {
                pulls = plot->pullHist("data_hist",main_pdf_name.c_str()); // will be deleted from plot_pulls
            }
            plot_pulls = variable.frame(Name("Pull_distribution"), Title("Pull distribution"), Range(range.c_str()));
            // create histogram with pull distribution (for nice overlay)
            TH1F* hist_pulls = new TH1F("hist_pulls","hist pulls",opt_bins,variable.getMin(range.c_str()),variable.getMax(range.c_str()));
            double* pull_values = pulls->GetY();
            double xerr = (variable.getMax(range.c_str())-variable.getMin(range.c_str()))/(2*opt_bins);
            for(int i = 0; i < opt_bins; ++i) {
                hist_pulls->SetBinContent(i+1,pull_values[i]);
                pulls->SetPointEXlow(i,xerr);
                pulls->SetPointEXhigh(i,xerr);
            }
            if(!opt_use_fit_error_for_pull)
                plot_pulls->addPlotable(pulls,"PE1");
            
            // the pull distribution needs different sizes (ROOT's annoying relative size behavior)
            plot_pulls->GetYaxis()->SetTitle("Pull");
            plot_pulls->GetYaxis()->CenterTitle();
            //plot_pulls->GetXaxis()->SetTitleSize(0.12);
            //plot_pulls->GetYaxis()->SetTitleSize(0.12);
            plot_pulls->GetXaxis()->SetTitleSize(0.18);
            plot_pulls->GetYaxis()->SetTitleSize(0.18);
            //plot_pulls->GetYaxis()->SetTitleOffset(0.44);
            plot_pulls->GetYaxis()->SetTitleOffset(0.39);
            plot_pulls->GetXaxis()->SetTitleOffset(0.82);
            plot_pulls->GetXaxis()->SetLabelSize(0.12);
            plot_pulls->GetYaxis()->SetLabelSize(0.12);
            //plot_pulls->GetYaxis()->SetLabelOffset(0.012);
            plot_pulls->GetYaxis()->SetLabelOffset(0.006);
            plot_pulls->GetXaxis()->SetTickLength( plot_pulls->GetXaxis()->GetTickLength() * 3.0 );
            plot_pulls->GetYaxis()->SetNdivisions(505);
            
            // draw pull distribution with extra histogram
            if(opt_use_fit_error_for_pull) {
                hist_pulls->SetFillColor(kOrange);
                hist_pulls->SetLineColor(kBlack);
            } else {
                hist_pulls->SetFillColor(kOrange);
                hist_pulls->SetLineColor(kOrange);
            }
            plot_pulls->addTH1(hist_pulls,"HISTsame");
            plot_pulls->drawBefore(pulls->GetName(),hist_pulls->GetName());
            plot_pulls->Draw("");
            
            // set reasonable limits for the pull plots
            if(hist_pulls->GetMaximum() > 3.5 || hist_pulls->GetMinimum() < -3.5){
                plot_pulls->SetMinimum(-5.5);
                plot_pulls->SetMaximum(5.5);
            } else {
                plot_pulls->SetMinimum(-3.5);
                plot_pulls->SetMaximum(3.5);
            }
        }
        
        // save final plot
        canvas->cd();
        canvas->SaveAs(filename.c_str());
        canvas->Update();
        canvas->SaveAs((filename+".root").c_str());
        // clean up
        for(TH1* h : to_delete) {
            delete h;
        }
        if(plot_pulls != NULL)
            delete plot_pulls;
        delete canvas;
        delete plot;
    }
    
    void VisualizeFitResultProjWData(shared_ptr<FF_PDF> pdf, RooDataSet &data, RooRealVar &variable, const tuple_category_name_flags& flags,
                                     bool add_all_components, int opt_bins, const std::string& range, const std::string& filename, const std::string& opt_plot_format,
                                     const tuple_category_name_flags& flags_flavor, bool opt_plot_pdf,
                                     bool opt_pull, bool opt_logy, bool opt_mctype, bool opt_mcinfo,
                                     bool opt_use_fit_error_for_pull, bool opt_pdf_legend) {
        // switch on spheric harmonical caching
        ComplexPDF::SetHelicityAnglesCache(true);
        for(unsigned int i = 0; i < std::get<2>(flags).size(); ++i) {
            std::string cat_name = std::get<1>(flags);
            std::string cat_label = std::get<2>(flags)[i];
            std::cout << "INFO: Visualizing " << cat_label << " for " << variable.GetName() << " of " << pdf->GetSimPDFLabel() << std::endl;
            if(pdf->HasPDF(cat_label)) {
                std::vector< tuple_pdfname_color_style_label > styles;
                styles.push_back(pdf->GetPDFStyle(cat_label));
                
                if(add_all_components) {
                    // this adds all other components to the plot
                    std::vector<FFUtil::tuple_pdfname_color_style_label> all_pdf_styles = pdf->GetPDFStyles(cat_label,false); // set to true for adding ALL pdf components
                    for(unsigned int j = 0; j < all_pdf_styles.size(); ++j){
                        bool duplicate = false;
                        for(unsigned int k = 0; k < styles.size(); ++k) {
                            if(std::get<0>(styles[k]) == std::get<0>(all_pdf_styles[j]))
                                duplicate = true;
                        }
                        if(!duplicate)
                            styles.push_back(all_pdf_styles[j]);
                        else
                            std::cout << "INFO: Didn't add a duplicate off: " << std::get<0>(all_pdf_styles[j]) << std::endl;
                    }
                }
                std::cout << "INFO: Adding components to plot: " << std::endl;
                for(const tuple_pdfname_color_style_label& tuple : styles) {
                    std::cout << mp::tuple_to_string(tuple) << "\n";
                }
                
                std::cout << "INFO: Using data " << cat_name+"=="+cat_name+"::"+cat_label << std::endl;
                RooDataSet* data_plot = dynamic_cast<RooDataSet*>(data.reduce(Cut((cat_name+"=="+cat_name+"::"+cat_label).c_str())));
                data_plot->Print();
                if(data_plot->sumEntries() > 0) {
                    for(unsigned int j = 0; j < std::get<2>(flags_flavor).size(); ++j) {
                        std::string flavor_name = std::get<1>(flags_flavor);
                        std::string flavor_label = std::get<2>(flags_flavor)[j];
                        std::string flavor_cut = flavor_name+"=="+flavor_name+"::"+flavor_label;
                        
                        std::string plotfilename = filename;
                        plotfilename.replace(plotfilename.find("."+opt_plot_format+""),1+opt_plot_format.size(),"_"+cat_label+"_"+flavor_label+"."+opt_plot_format+"");
                        
                        std::string plotlabel = "";
                        if(flavor_label == "CP") {
                            plotlabel = "B^{0} #rightarrow #phi K^{+} #pi^{-}";
                        } else if (flavor_label == "CA") {
                            plotlabel = "#bar{B^{0}} #rightarrow #phi K^{-} #pi^{+}";
                        } else {
                            plotlabel = "Unknown";
                        }
                        VisualizeFitResultProjWData(pdf->GetPDF(cat_label),*data_plot,variable,styles,*(std::get<0>(flags_flavor)),flavor_cut,flavor_label,
                                                    opt_bins,range,plotfilename,plotlabel,opt_pull,opt_logy,opt_mctype,opt_mcinfo,
                                                    opt_use_fit_error_for_pull,opt_pdf_legend,opt_plot_pdf);
                    }
                } else {
                    std::cout << "INFO: Skipping plot as data category " << cat_label << " has zero entries" << std::endl;
                }
                delete data_plot;
            } else {
                std::cout << "INFO: Skipping plot there seems to be no PDF for category " << cat_label << std::endl;
            }
        }
        std::cout << "INFO: Spheric harmonic cache size is " << ComplexPDF::GetSphericalHarmonicCacheSize();
        std::cout << " and cached values have been accesses " << ComplexPDF::GetSphericalHarmonicCacheStats() << " times."<< std::endl;
        ComplexPDF::SetHelicityAnglesCache(false);
        ComplexPDF::ClearHelicityAnglesCache();
    }
    
    void VisualizeFitResultProjWData(RooAbsPdf &pdf, RooDataSet &data, RooRealVar &variable,
                                     const tuple_pdfname_color_style_label &pdf_style,
                                     RooCategory& flavor, const std::string& flavor_cut, const std::string& flavor_label,
                                     int opt_bins, std::string range, std::string filename, std::string plotlabel,
                                     bool opt_pull, bool opt_logy, bool opt_mctype, bool opt_mcinfo, bool opt_use_fit_error_for_pull,
                                     bool opt_pdf_legend, bool opt_plot_pdf) {
        std::vector< tuple_pdfname_color_style_label > styles;
        styles.push_back(pdf_style);
        VisualizeFitResultProjWData(pdf,data,variable,styles,flavor,flavor_cut,flavor_label,opt_bins,range,filename,plotlabel,
                                    opt_pull,opt_logy,opt_mctype,opt_mcinfo,opt_use_fit_error_for_pull,opt_pdf_legend,opt_plot_pdf);
    }
    
    void VisualizeFitResultProjWData(RooAbsPdf &pdf, RooDataSet &data, RooRealVar &variable,
                                     std::vector< tuple_pdfname_color_style_label > &pdf_style,
                                     RooCategory& flavor, const std::string& flavor_cut, const std::string& flavor_label,
                                     int opt_bins, std::string range, std::string filename, std::string plotlabel,
                                     bool opt_pull, bool opt_logy, bool opt_mctype, bool opt_mcinfo, bool opt_use_fit_error_for_pull,
                                     bool opt_pdf_legend, bool opt_plot_pdf) {
        std::string varname = variable.GetName();
        // Note: This is a analysis specific fix: Use 40 bins due to hel_th1 cut
        if(varname == "hel_th1" || varname == "hel_th2" || varname == "hel_phi") {
            opt_bins = 40;
        }
        RooPlot *plot = variable.frame(Bins(opt_bins), Range(range.c_str()), Name("plot"), Title("plot"));
        TCanvas *canvas;
        // split canvas in case of extra pull distribution
        if(opt_pull && opt_plot_pdf) {
            canvas = new TCanvas("c_canvas","c_canvas",1280,1280);
            canvas->Divide(1,2);
            canvas->GetPad(1)->SetPad(0.0,0.25,1,1);
            canvas->GetPad(1)->SetBottomMargin(0.05);
            canvas->GetPad(1)->SetRightMargin(0.05);
            canvas->GetPad(2)->SetPad(0.0,0.0,1,0.25);
            canvas->GetPad(2)->SetBottomMargin(0.32);
            canvas->GetPad(2)->SetRightMargin(0.05);
            canvas->cd(1);
        } else {
            canvas = new TCanvas("c_canvas","c_canvas",1280,1024);
        }
        // create plot of data and fits
        data.plotOn(plot, Name("data_hist"), CutRange(range.c_str()), MarkerColor(kBlack), Cut(flavor_cut.c_str()),
                    DataError(data.isWeighted() ? RooAbsData::SumW2 : RooAbsData::Poisson));
        // create legend for fitted curves
        int leg_entries = pdf_style.size();
        TLegend *leg = new TLegend(0.69,0.88-0.04*leg_entries,0.94,0.88,"","NDC");
        leg->SetBorderSize(0);
        leg->SetFillColor(0);
        leg->SetFillStyle(4000);
        leg->SetTextSize(0.035);
        // TODO: Use RooFitResult to visualize PDF uncertainties
        // this is the name of the main pdf, for which we want pulls, chi2, etc... (we need this label below)
        std::string main_pdf_name = "";
        if(opt_plot_pdf) {
            for(unsigned int i = 0; i < pdf_style.size(); ++i) {
                std::string name = "component_"+mp::to_string(i);
                if(i == 0)
                    main_pdf_name = name;
                std::string component = std::get<0>(pdf_style[i]);
                Color_t color = std::get<1>(pdf_style[i]);
                Style_t style = std::get<2>(pdf_style[i]);
                std::string label = std::get<3>(pdf_style[i]);
                pdf.plotOn(plot, Name(name.c_str()), ProjectionRange(range.c_str()), Components(component.c_str()), LineColor(color), LineStyle(style),
                           Slice(flavor,flavor_label.c_str()), ProjWData(data));
                RooCurve* curve = plot->getCurve(name.c_str());
                leg->AddEntry(curve,label.c_str(),"L");
            }
            // print chi2 and parameter info
            // TODO: Check for proper chi2 and probabilty of unbinned fit... this is not yet correct...
            // TODO: Calc chi2 for pull distribution with its nbins bins and dgfs..
            std::cout << "INFO BLOCK FOR: " << variable.GetName() << std::endl;
            std::cout << "  chi^2 = " << plot->chiSquare(main_pdf_name.c_str(),"data_hist") << std::endl;
            std::cout << "  ndf = " << opt_bins-pdf.getParameters(data)->getSize() << std::endl;
            std::cout << "  chi^2/ndf = " << plot->chiSquare(main_pdf_name.c_str(),"data_hist",pdf.getParameters(data)->getSize()) << std::endl;
            std::cout << "  Prob = " << TMath::Prob(plot->chiSquare(main_pdf_name.c_str(),"data_hist"),opt_bins-pdf.getParameters(data)->getSize()) << std::endl;
        }
        
        // draw also the legend
        if(opt_pdf_legend)
            plot->addObject(leg,"");
        else
            delete leg;
        // write label into upper right of plot
        TLatex* text = new TLatex(0.81, 0.91, plotlabel.c_str());
        text->SetTextColor(kBlack);
        text->SetTextFont(42);
        if(opt_pull && opt_plot_pdf)
            text->SetTextSize(0.035);
        else
            text->SetTextSize(0.032);
        text->SetNDC(1);
        text->SetText(1.0 - 0.05 - 0.01 - text->GetXsize(), 0.91, plotlabel.c_str());
        plot->addObject(text,"");
        
        // add additional histograms to the plot
        std::vector<TH1*> to_delete;
        if(opt_mctype && !opt_mcinfo)
            to_delete = AddHistogram_MCType(plot, data, variable, opt_bins, range.c_str(), flavor_cut + " && ");
        if(opt_mcinfo && !opt_mctype)
            to_delete = AddHistogram_MCInfo(plot, data, variable, opt_bins, range.c_str(), flavor_cut + " && ");
        if(opt_mcinfo && opt_mctype)
            to_delete = AddHistogram_MCInfoType(plot, data, variable, opt_bins, range.c_str(), flavor_cut + " && ");
        
        // draw final plot (that's it if no extra options are given)
        plot->Print();
        plot->Draw("");
        // set log scale if option selected
		canvas->cd(1)->SetLogy(opt_logy);
        // Note: This is a analysis specific fix: fix y-label for phi angle
        if(varname == "hel_phi") {
            std::stringstream ylabel;
            ylabel << "Events / ( #pi/" << opt_bins/2 << " rad )";
            plot->GetYaxis()->SetTitle(ylabel.str().c_str());
        }
        plot->SetMinimum(0.0);
        plot->SetMaximum(1.5*plot->GetMaximum());
        
        // add pull distribution plot
        RooPlot* plot_pulls = (RooPlot*)NULL;
        if(opt_pull && opt_plot_pdf) {
            plot->SetXTitle("");
            canvas->cd(2);
            
            // create pull distribution
            RooHist* pulls = (RooHist*)NULL;
            if(opt_use_fit_error_for_pull) {
                pulls = plot->residHist("data_hist",main_pdf_name.c_str()); // will be deleted from plot_pulls
                RooCurve* curve = plot->getCurve(main_pdf_name.c_str());
                double* x_values = pulls->GetX();
                double* y_values = pulls->GetY();
                double xerr = (variable.getMax(range.c_str())-variable.getMin(range.c_str()))/(2*opt_bins);
                for(int i = 0; i < opt_bins; ++i) {
                    double fit_value = curve->average(x_values[i]-xerr,x_values[i]+xerr);
                    if(fit_value > 0) {
                        y_values[i] /= std::sqrt(fit_value);
                    } else {
                        y_values[i] = 0;
                    }
                }
            } else {
                pulls = plot->pullHist("data_hist",main_pdf_name.c_str()); // will be deleted from plot_pulls
            }
            plot_pulls = variable.frame(Name("Pull_distribution"), Title("Pull distribution"), Range(range.c_str()));
            // create histogram with pull distribution (for nice overlay)
            TH1F* hist_pulls = new TH1F("hist_pulls","hist pulls",opt_bins,variable.getMin(range.c_str()),variable.getMax(range.c_str()));
            double* pull_values = pulls->GetY();
            double xerr = (variable.getMax(range.c_str())-variable.getMin(range.c_str()))/(2*opt_bins);
            for(int i = 0; i < opt_bins; ++i) {
                hist_pulls->SetBinContent(i+1,pull_values[i]);
                pulls->SetPointEXlow(i,xerr);
                pulls->SetPointEXhigh(i,xerr);
            }
            if(!opt_use_fit_error_for_pull)
                plot_pulls->addPlotable(pulls,"PE1");
            
            // the pull distribution needs different sizes (ROOT's annoying relative size behavior)
            plot_pulls->GetYaxis()->SetTitle("Pull");
            plot_pulls->GetYaxis()->CenterTitle();
            //plot_pulls->GetXaxis()->SetTitleSize(0.12);
            //plot_pulls->GetYaxis()->SetTitleSize(0.12);
            plot_pulls->GetXaxis()->SetTitleSize(0.18);
            plot_pulls->GetYaxis()->SetTitleSize(0.18);
            //plot_pulls->GetYaxis()->SetTitleOffset(0.44);
            plot_pulls->GetYaxis()->SetTitleOffset(0.39);
            plot_pulls->GetXaxis()->SetTitleOffset(0.82);
            plot_pulls->GetXaxis()->SetLabelSize(0.12);
            plot_pulls->GetYaxis()->SetLabelSize(0.12);
            //plot_pulls->GetYaxis()->SetLabelOffset(0.012);
            plot_pulls->GetYaxis()->SetLabelOffset(0.006);
            plot_pulls->GetXaxis()->SetTickLength( plot_pulls->GetXaxis()->GetTickLength() * 3.0 );
            plot_pulls->GetYaxis()->SetNdivisions(505);
            
            // draw pull distribution with extra histogram
            if(opt_use_fit_error_for_pull) {
                hist_pulls->SetFillColor(kOrange);
                hist_pulls->SetLineColor(kBlack);
            } else {
                hist_pulls->SetFillColor(kOrange);
                hist_pulls->SetLineColor(kOrange);
            }
            plot_pulls->addTH1(hist_pulls,"HISTsame");
            plot_pulls->drawBefore(pulls->GetName(),hist_pulls->GetName());
            plot_pulls->Draw("");
            
            // set reasonable limits for the pull plots
            if(hist_pulls->GetMaximum() > 3.5 || hist_pulls->GetMinimum() < -3.5){
                plot_pulls->SetMinimum(-5.5);
                plot_pulls->SetMaximum(5.5);
            } else {
                plot_pulls->SetMinimum(-3.5);
                plot_pulls->SetMaximum(3.5);
            }
        }
        
        // save final plot
        canvas->cd();
        canvas->SaveAs(filename.c_str());
        canvas->SaveAs((filename+".root").c_str());
        // clean up
        for(TH1* h : to_delete) {
            delete h;
        }
        if(plot_pulls != NULL)
            delete plot_pulls;
        delete canvas;
        delete plot;
    }
    
    void SetFitRanges(RooRealVar &mbc, RooRealVar &deltae, RooRealVar &chil0_ma, RooRealVar &mass_kpi,
                      RooRealVar &hel_phi, RooRealVar &hel_th1, RooRealVar &hel_th2, RooRealVar &nb_prime) {
        
        mbc.setRange("full_range",5.26,5.29);
        deltae.setRange("full_range",-0.15,0.15);
        chil0_ma.setRange("full_range",0.985,1.05);
        mass_kpi.setRange("full_range",0.7,1.55);
        hel_phi.setRange("full_range",-TMath::Pi(),TMath::Pi());
        hel_th1.setRange("full_range",-1.0,1.0);
        hel_th2.setRange("full_range",-1.0,1.0);
        nb_prime.setRange("full_range",-10,10);
        
        mbc.setRange("full_range_S",5.26,5.29);
        deltae.setRange("full_range_S",-0.15,0.15);
        chil0_ma.setRange("full_range_S",0.985,1.05);
        mass_kpi.setRange("full_range_S",0.7,1.55);
        hel_phi.setRange("full_range_S",-TMath::Pi(),TMath::Pi());
        hel_th1.setRange("full_range_S",-1.0,1.0);
        hel_th2.setRange("full_range_S",-1.0,1.0);
        nb_prime.setRange("full_range_S",-10,10);
        
        mbc.setRange("full_range_SB",5.24,5.26);
        deltae.setRange("full_range_SB",-0.15,0.15);
        chil0_ma.setRange("full_range_SB",0.985,1.05);
        mass_kpi.setRange("full_range_SB",0.7,1.55);
        hel_phi.setRange("full_range_SB",-TMath::Pi(),TMath::Pi());
        hel_th1.setRange("full_range_SB",-1.0,1.0);
        hel_th2.setRange("full_range_SB",-1.0,1.0);
        nb_prime.setRange("full_range_SB",-10,10);
        
        mbc.setRange("mbc_sideband",5.26,5.27);
        deltae.setRange("mbc_sideband",-0.15,0.15);
        chil0_ma.setRange("mbc_sideband",0.985,1.05);
        mass_kpi.setRange("mbc_sideband",0.7,1.55);
        hel_phi.setRange("mbc_sideband",-TMath::Pi(),TMath::Pi());
        hel_th1.setRange("mbc_sideband",-1.0,1.0);
        hel_th2.setRange("mbc_sideband",-1.0,1.0);
        nb_prime.setRange("mbc_sideband",-10,10);
        
        mbc.setRange("mbc_signal_box",5.27,5.29);
        deltae.setRange("mbc_signal_box",-0.15,0.15);
        chil0_ma.setRange("mbc_signal_box",0.985,1.05);
        mass_kpi.setRange("mbc_signal_box",0.7,1.55);
        hel_phi.setRange("mbc_signal_box",-TMath::Pi(),TMath::Pi());
        hel_th1.setRange("mbc_signal_box",-1.0,1.0);
        hel_th2.setRange("mbc_signal_box",-1.0,1.0);
        nb_prime.setRange("mbc_signal_box",-10,10);
        
        // signal box
        mbc.setRange("signal_box",5.27,5.29);
        deltae.setRange("signal_box",-0.04,0.04);
        chil0_ma.setRange("signal_box",1.01,1.03);
        mass_kpi.setRange("signal_box",0.7,1.55);
        hel_phi.setRange("signal_box",-TMath::Pi(),TMath::Pi());
        hel_th1.setRange("signal_box",-1.0,1.0);
        hel_th2.setRange("signal_box",-1.0,1.0);
        nb_prime.setRange("signal_box",-3,10);
        
        // three mass_kpi bins
        mass_kpi.setRange("mass_kpi_bin_1",0.7,0.87);
        hel_th1.setRange("mass_kpi_bin_1",-1.0,1.0);
        
        mass_kpi.setRange("mass_kpi_bin_2",0.87,1.04);
        hel_th1.setRange("mass_kpi_bin_2",-1.0,1.0);
        
        mass_kpi.setRange("mass_kpi_bin_3",1.04,1.21);
        hel_th1.setRange("mass_kpi_bin_3",-1.0,1.0);
        
        mass_kpi.setRange("mass_kpi_bin_4",1.21,1.38);
        hel_th1.setRange("mass_kpi_bin_4",-1.0,1.0);
        
        mass_kpi.setRange("mass_kpi_bin_5",1.38,1.55);
        hel_th1.setRange("mass_kpi_bin_5",-1.0,1.0);
        
        // signal box mbc
        mbc.setRange("signal_box_mbc",5.26,5.29);
        deltae.setRange("signal_box_mbc",-0.04,0.04);
        chil0_ma.setRange("signal_box_mbc",1.01,1.03);
        mass_kpi.setRange("signal_box_mbc",0.7,1.55);
        hel_phi.setRange("signal_box_mbc",-TMath::Pi(),TMath::Pi());
        hel_th1.setRange("signal_box_mbc",-1.0,1.0);
        hel_th2.setRange("signal_box_mbc",-1.0,1.0);
        nb_prime.setRange("signal_box_mbc",-3,10);
        
        // signal box deltae
        mbc.setRange("signal_box_deltae",5.27,5.29);
        deltae.setRange("signal_box_deltae",-0.15,0.15);
        chil0_ma.setRange("signal_box_deltae",1.01,1.03);
        mass_kpi.setRange("signal_box_deltae",0.7,1.55);
        hel_phi.setRange("signal_box_deltae",-TMath::Pi(),TMath::Pi());
        hel_th1.setRange("signal_box_deltae",-1.0,1.0);
        hel_th2.setRange("signal_box_deltae",-1.0,1.0);
        nb_prime.setRange("signal_box_deltae",-3,10);
        
        // signal box chil0_ma
        mbc.setRange("signal_box_chil0_ma",5.27,5.29);
        deltae.setRange("signal_box_chil0_ma",-0.04,0.04);
        chil0_ma.setRange("signal_box_chil0_ma",0.985,1.05);
        mass_kpi.setRange("signal_box_chil0_ma",0.7,1.55);
        hel_phi.setRange("signal_box_chil0_ma",-TMath::Pi(),TMath::Pi());
        hel_th1.setRange("signal_box_chil0_ma",-1.0,1.0);
        hel_th2.setRange("signal_box_chil0_ma",-1.0,1.0);
        nb_prime.setRange("signal_box_chil0_ma",-3,10);
        
        // signal box nb_prime
        mbc.setRange("signal_box_nb_prime",5.27,5.29);
        deltae.setRange("signal_box_nb_prime",-0.04,0.04);
        chil0_ma.setRange("signal_box_nb_prime",1.01,1.03);
        mass_kpi.setRange("signal_box_nb_prime",0.7,1.55);
        hel_phi.setRange("signal_box_nb_prime",-TMath::Pi(),TMath::Pi());
        hel_th1.setRange("signal_box_nb_prime",-1.0,1.0);
        hel_th2.setRange("signal_box_nb_prime",-1.0,1.0);
        nb_prime.setRange("signal_box_nb_prime",-10,10);
        
        // signal box LASS
        mbc.setRange("signal_box_LASS",5.27,5.29);
        deltae.setRange("signal_box_LASS",-0.04,0.04);
        chil0_ma.setRange("signal_box_LASS",1.01,1.03);
        mass_kpi.setRange("signal_box_LASS",1.05,1.25);
        hel_phi.setRange("signal_box_LASS",-TMath::Pi(),TMath::Pi());
        hel_th1.setRange("signal_box_LASS",-1.0,1.0);
        hel_th2.setRange("signal_box_LASS",-1.0,1.0);
        nb_prime.setRange("signal_box_LASS",-3,10);
        
        // signal box K892
        mbc.setRange("signal_box_K892",5.27,5.29);
        deltae.setRange("signal_box_K892",-0.04,0.04);
        chil0_ma.setRange("signal_box_K892",1.01,1.03);
        mass_kpi.setRange("signal_box_K892",0.8,1.0);
        hel_phi.setRange("signal_box_K892",-TMath::Pi(),TMath::Pi());
        hel_th1.setRange("signal_box_K892",-1.0,1.0);
        hel_th2.setRange("signal_box_K892",-1.0,1.0);
        nb_prime.setRange("signal_box_K892",-3,10);
        
        // signal box K21430
        mbc.setRange("signal_box_K21430",5.27,5.29);
        deltae.setRange("signal_box_K21430",-0.04,0.04);
        chil0_ma.setRange("signal_box_K21430",1.01,1.03);
        mass_kpi.setRange("signal_box_K21430",1.3,1.55);
        hel_phi.setRange("signal_box_K21430",-TMath::Pi(),TMath::Pi());
        hel_th1.setRange("signal_box_K21430",-1.0,1.0);
        hel_th2.setRange("signal_box_K21430",-1.0,1.0);
        nb_prime.setRange("signal_box_K21430",-3,10);
        
        // signal box f0(980) (general)
        mbc.setRange("signal_box_mbc_f0980",5.26,5.29);
        deltae.setRange("signal_box_mbc_f0980",-0.04,0.04);
        chil0_ma.setRange("signal_box_mbc_f0980",0.985,1.01);
        mass_kpi.setRange("signal_box_mbc_f0980",0.7,1.55);
        hel_phi.setRange("signal_box_mbc_f0980",-TMath::Pi(),TMath::Pi());
        hel_th1.setRange("signal_box_mbc_f0980",-1.0,1.0);
        hel_th2.setRange("signal_box_mbc_f0980",-1.0,1.0);
        nb_prime.setRange("signal_box_mbc_f0980",-3,10);
        
        // signal box f0(980) (general)
        mbc.setRange("signal_box_deltae_f0980",5.27,5.29);
        deltae.setRange("signal_box_deltae_f0980",-0.15,0.15);
        chil0_ma.setRange("signal_box_deltae_f0980",0.985,1.01);
        mass_kpi.setRange("signal_box_deltae_f0980",0.7,1.55);
        hel_phi.setRange("signal_box_deltae_f0980",-TMath::Pi(),TMath::Pi());
        hel_th1.setRange("signal_box_deltae_f0980",-1.0,1.0);
        hel_th2.setRange("signal_box_deltae_f0980",-1.0,1.0);
        nb_prime.setRange("signal_box_deltae_f0980",-3,10);
        
        // signal box f0(980) (high KPimass)
        mbc.setRange("signal_box_mbc_f0980_highKPi",5.26,5.29);
        deltae.setRange("signal_box_mbc_f0980_highKPi",-0.04,0.04);
        chil0_ma.setRange("signal_box_mbc_f0980_highKPi",0.985,1.01);
        mass_kpi.setRange("signal_box_mbc_f0980_highKPi",1.05,1.55);
        hel_phi.setRange("signal_box_mbc_f0980_highKPi",-TMath::Pi(),TMath::Pi());
        hel_th1.setRange("signal_box_mbc_f0980_highKPi",-1.0,1.0);
        hel_th2.setRange("signal_box_mbc_f0980_highKPi",-1.0,1.0);
        nb_prime.setRange("signal_box_mbc_f0980_highKPi",-3,10);
        
        // signal box f0(980) (high KPimass)
        mbc.setRange("signal_box_deltae_f0980_highKPi",5.27,5.29);
        deltae.setRange("signal_box_deltae_f0980_highKPi",-0.15,0.15);
        chil0_ma.setRange("signal_box_deltae_f0980_highKPi",0.985,1.01);
        mass_kpi.setRange("signal_box_deltae_f0980_highKPi",1.05,1.55);
        hel_phi.setRange("signal_box_deltae_f0980_highKPi",-TMath::Pi(),TMath::Pi());
        hel_th1.setRange("signal_box_deltae_f0980_highKPi",-1.0,1.0);
        hel_th2.setRange("signal_box_deltae_f0980_highKPi",-1.0,1.0);
        nb_prime.setRange("signal_box_deltae_f0980_highKPi",-3,10);
        
        // signal box f0(980)K*(892)
        mbc.setRange("signal_box_mbc_f0980_K892",5.26,5.29);
        deltae.setRange("signal_box_mbc_f0980_K892",-0.04,0.04);
        chil0_ma.setRange("signal_box_mbc_f0980_K892",0.985,1.005);
        mass_kpi.setRange("signal_box_mbc_f0980_K892",0.8,1.0);
        hel_phi.setRange("signal_box_mbc_f0980_K892",-TMath::Pi(),TMath::Pi());
        hel_th1.setRange("signal_box_mbc_f0980_K892",-1.0,1.0);
        hel_th2.setRange("signal_box_mbc_f0980_K892",-1.0,1.0);
        nb_prime.setRange("signal_box_mbc_f0980_K892",-3,10);
        
        // signal box f0(980)K*(892)
        mbc.setRange("signal_box_deltae_f0980_K892",5.27,5.29);
        deltae.setRange("signal_box_deltae_f0980_K892",-0.15,0.15);
        chil0_ma.setRange("signal_box_deltae_f0980_K892",0.985,1.005);
        mass_kpi.setRange("signal_box_deltae_f0980_K892",0.8,1.0);
        hel_phi.setRange("signal_box_deltae_f0980_K892",-TMath::Pi(),TMath::Pi());
        hel_th1.setRange("signal_box_deltae_f0980_K892",-1.0,1.0);
        hel_th2.setRange("signal_box_deltae_f0980_K892",-1.0,1.0);
        nb_prime.setRange("signal_box_deltae_f0980_K892",-3,10);
        
        // signal box f0(980)K*(892)
        mbc.setRange("signal_box_chil0_ma_f0980_K892",5.27,5.29);
        deltae.setRange("signal_box_chil0_ma_f0980_K892",-0.04,0.04);
        chil0_ma.setRange("signal_box_chil0_ma_f0980_K892",0.985,1.01);
        mass_kpi.setRange("signal_box_chil0_ma_f0980_K892",0.8,1.0);
        hel_phi.setRange("signal_box_chil0_ma_f0980_K892",-TMath::Pi(),TMath::Pi());
        hel_th1.setRange("signal_box_chil0_ma_f0980_K892",-1.0,1.0);
        hel_th2.setRange("signal_box_chil0_ma_f0980_K892",-1.0,1.0);
        nb_prime.setRange("signal_box_chil0_ma_f0980_K892",-3,10);
        
        // signal box f0(980)K*(892)
        mbc.setRange("signal_box_f0980_K892",5.27,5.29);
        deltae.setRange("signal_box_f0980_K892",-0.04,0.04);
        chil0_ma.setRange("signal_box_f0980_K892",0.985,1.01);
        mass_kpi.setRange("signal_box_f0980_K892",0.8,1.0);
        hel_phi.setRange("signal_box_f0980_K892",-TMath::Pi(),TMath::Pi());
        hel_th1.setRange("signal_box_f0980_K892",-1.0,1.0);
        hel_th2.setRange("signal_box_f0980_K892",-1.0,1.0);
        nb_prime.setRange("signal_box_f0980_K892",-3,10);
        
        // signal box K+K-K*(892)
        mbc.setRange("signal_box_mbc_KK_K892",5.26,5.29);
        deltae.setRange("signal_box_mbc_KK_K892",-0.04,0.04);
        chil0_ma.setRange("signal_box_mbc_KK_K892",1.035,1.05);
        mass_kpi.setRange("signal_box_mbc_KK_K892",0.8,1.0);
        hel_phi.setRange("signal_box_mbc_KK_K892",-TMath::Pi(),TMath::Pi());
        hel_th1.setRange("signal_box_mbc_KK_K892",-1.0,1.0);
        hel_th2.setRange("signal_box_mbc_KK_K892",-1.0,1.0);
        nb_prime.setRange("signal_box_mbc_KK_K892",-3,10);
        
        // signal box K+K-K*(892)
        mbc.setRange("signal_box_deltae_KK_K892",5.27,5.29);
        deltae.setRange("signal_box_deltae_KK_K892",-0.15,0.15);
        chil0_ma.setRange("signal_box_deltae_KK_K892",1.035,1.05);
        mass_kpi.setRange("signal_box_deltae_KK_K892",0.8,1.0);
        hel_phi.setRange("signal_box_deltae_KK_K892",-TMath::Pi(),TMath::Pi());
        hel_th1.setRange("signal_box_deltae_KK_K892",-1.0,1.0);
        hel_th2.setRange("signal_box_deltae_KK_K892",-1.0,1.0);
        nb_prime.setRange("signal_box_deltae_KK_K892",-3,10);
        
        // signal box K+K-K*(892)
        mbc.setRange("signal_box_KK_K892",5.27,5.29);
        deltae.setRange("signal_box_KK_K892",-0.04,0.04);
        chil0_ma.setRange("signal_box_KK_K892",1.035,1.05);
        mass_kpi.setRange("signal_box_KK_K892",0.8,1.0);
        hel_phi.setRange("signal_box_KK_K892",-TMath::Pi(),TMath::Pi());
        hel_th1.setRange("signal_box_KK_K892",-1.0,1.0);
        hel_th2.setRange("signal_box_KK_K892",-1.0,1.0);
        nb_prime.setRange("signal_box_KK_K892",-3,10);
        
        // signal box for B->phiphi
        mbc.setRange("signal_box_mbc_phi_phi",5.26,5.29);
        deltae.setRange("signal_box_mbc_phi_phi",-0.15,-0.04);
        chil0_ma.setRange("signal_box_mbc_phi_phi",1.01,1.03);
        mass_kpi.setRange("signal_box_mbc_phi_phi",0.7,1.55);
        hel_phi.setRange("signal_box_mbc_phi_phi",-TMath::Pi(),TMath::Pi());
        hel_th1.setRange("signal_box_mbc_phi_phi",-1.0,1.0);
        hel_th2.setRange("signal_box_mbc_phi_phi",-1.0,1.0);
        nb_prime.setRange("signal_box_mbc_phi_phi",-3,10);
        
        // signal box for B->phirho
        mbc.setRange("signal_box_mbc_phi_rho",5.26,5.29);
        deltae.setRange("signal_box_mbc_phi_rho",0.04,0.15);
        chil0_ma.setRange("signal_box_mbc_phi_rho",1.01,1.03);
        mass_kpi.setRange("signal_box_mbc_phi_rho",0.7,1.55);
        hel_phi.setRange("signal_box_mbc_phi_rho",-TMath::Pi(),TMath::Pi());
        hel_th1.setRange("signal_box_mbc_phi_rho",-1.0,1.0);
        hel_th2.setRange("signal_box_mbc_phi_rho",-1.0,1.0);
        nb_prime.setRange("signal_box_mbc_phi_rho",-3,10);
    }
    
    // returns binomial errors for two values, where accepted is subsample of all
    double binomialError(const double all, const double accepted) {
        if(all == 0) {
            return 0;
        } else if(accepted > all) {
            std::cerr << "ERROR: Accepted amount of events larger than all events" << std::endl;
            return 0;
        } else {
            return std::sqrt((accepted*(all - accepted))/(all*all*all));
        }
    }
    
    // Fits efficiency function "effFunc" to data set "data" in dimension of variable "var"
    // where the cut "cut" is 0 or 1, for rejected or accepted events.
    // Creats a plot in folder "opt_outdir" and uses "opt_numcpu" cores for fitting. For
    // visualization "opt_bins" bins are used.
    void calcAndFitEfficiency(RooDataSet &data, RooRealVar &var, RooAbsReal &effFunc,
                              RooCategory &cut, const std::string &opt_outdir, const std::string& opt_plot_format, bool opt_numcpu, int opt_bins, const std::string &flag, bool visualize_error) {
        std::string varname = var.GetName();
        
        // define efficiency pdf
        RooEfficiency effPdf(("effPdf_"+varname+flag).c_str(),("efficiency PDF for "+varname+flag).c_str(),effFunc,cut,"accept");
        
        // fit conditional efficiency pdf to data
        RooFitResult* eff_result = effPdf.fitTo(data,ConditionalObservables(var),NumCPU(opt_numcpu), Timer(true), Save(true));
        
        // plot distribution of all events and accepted fraction of events on frame
        RooPlot* left = var.frame(Bins(opt_bins),Name("left"), Title("left"));
        RooHist* all = data.plotOn(left,MarkerColor(kBlack))->getHist();
        RooHist* accepted = data.plotOn(left,MarkerColor(kBlue),Cut("cut==cut::accept"))->getHist();
        double *n_all = all->GetY();
        double *n_accepted = accepted->GetY();
        
        // plot accept/reject efficiency on data overlay fitted efficiency curve
        RooPlot* right = var.frame(Bins(opt_bins),Name("right"), Title("right"));
        RooHist* hist = data.plotOn(right,Name("data_hist"),Efficiency(cut))->getHist();
        double* y_low = hist->GetEYlow();
        double* y_high = hist->GetEYhigh();
        
        // recalculate binomial errors for right histogram (This bug should be fixed in ROOT 5.30.00 and newer)
        if(gROOT->GetVersionInt() < 53000) {
            for(int i = 0; i < opt_bins; ++i) {
                double binomial = binomialError(n_all[i],n_accepted[i]);
                y_low[i] = binomial;
                y_high[i] = binomial;
            }
        }
        
        // visualize error by adding VisualizeError(*eff_result) to arguments
        if(visualize_error) {
            effFunc.plotOn(right,VisualizeError(*eff_result));
        }
        effFunc.plotOn(right,Name("effPDF"),LineColor(kBlue),LineStyle(kDashed));
        
        std::cout << "INFO BLOCK FOR: " << var.GetName() << std::endl;
        std::cout << "  chi^2 = " << right->chiSquare("effPDF","data_hist") * (opt_bins - effFunc.getParameters(data)->getSize()) << std::endl;
        std::cout << "  param = " << effFunc.getParameters(data)->getSize() << std::endl;
        std::cout << "  bins = " << opt_bins << std::endl;
        std::cout << "  ndf = " << opt_bins - effFunc.getParameters(data)->getSize() << std::endl;
        std::cout << "  chi^2/ndf = " << right->chiSquare("effPDF","data_hist",effFunc.getParameters(data)->getSize()) << std::endl;
        std::cout << "  Prob = " << TMath::Prob(right->chiSquare("effPDF","data_hist") * (opt_bins - effFunc.getParameters(data)->getSize()),
                                                opt_bins-effFunc.getParameters(data)->getSize()) << std::endl;
        
        // draw all frames on a canvas
        TCanvas* canvas = new TCanvas("canvas","canvas",2*1280,1024) ;
        canvas->Divide(2,1);
        canvas->GetPad(1)->SetLeftMargin(0.15);
        canvas->GetPad(2)->SetLeftMargin(0.15);
        canvas->cd(1);
        left->GetYaxis()->SetTitleOffset(1.6);
        left->Draw("");
        canvas->cd(2);
        right->GetYaxis()->SetTitleOffset(1.4);
        right->SetMinimum(0.0);
        right->SetMaximum(0.5);
        right->Draw("");
        canvas->cd();
        canvas->SaveAs((opt_outdir+"efficiency_"+var.GetName()+flag+"."+opt_plot_format+"").c_str());
        canvas->SaveAs((opt_outdir+"efficiency_"+var.GetName()+flag+"."+opt_plot_format+".root").c_str());
        
        eff_result->Print();
        
        delete canvas;
        delete eff_result;
    }
    
    void calcAndFitEfficiency2D(RooDataSet &data, RooRealVar &var1, RooRealVar &var2, RooAbsReal &effFunc, RooCategory &cut, const std::string &opt_outdir, const std::string& opt_plot_format, bool opt_numcpu, int opt_bins, const std::string &flag, bool visualize_error) {
        std::string varname1 = var1.GetName();
        std::string varname2 = var2.GetName();
        
        // define efficiency pdf
        RooEfficiency effPdf(("effPdf_"+varname1+":"+varname2+flag).c_str(),("efficiency PDF for "+varname1+":"+varname2+flag).c_str(),effFunc,cut,"accept");
        
        // fit conditional efficiency pdf to data
        RooFitResult* eff_result = effPdf.fitTo(data,ConditionalObservables(RooArgSet(var1,var2)),NumCPU(opt_numcpu), Timer(true), Save(true));
        
        // plot distribution of all events and accepted fraction of events on frame
        //RooPlot* left = var.frame(Bins(opt_bins),Name("left"), Title("left"));
        //RooHist* all = data.plotOn(left,MarkerColor(kBlack))->getHist();
        //RooHist* accepted = data.plotOn(left,MarkerColor(kBlue),Cut("cut==cut::accept"))->getHist();
        //double *n_all = all->GetY();
        //double *n_accepted = accepted->GetY();
        
        
        // plot accept/reject efficiency on data overlay fitted efficiency curve
        RooPlot* left = var1.frame(Bins(opt_bins),Name("left"), Title("left"));
        RooHist* all = data.plotOn(left,Name("data_hist"),Efficiency(cut))->getHist();
        double* n_all = all->GetEYlow();
        double* n_accepted = all->GetEYhigh();
        
        
        // plot accept/reject efficiency on data overlay fitted efficiency curve
        RooPlot* right = var2.frame(Bins(opt_bins),Name("right"), Title("right"));
        RooHist* hist = data.plotOn(right,Name("data_hist"),Efficiency(cut))->getHist();
        double* y_low = hist->GetEYlow();
        double* y_high = hist->GetEYhigh();
        
        // visualize error by adding VisualizeError(*eff_result) to arguments
        if(visualize_error) {
            effFunc.plotOn(right,VisualizeError(*eff_result));
            effFunc.plotOn(left,VisualizeError(*eff_result));
        }
        
        effFunc.plotOn(left,Name("effPDF"),LineColor(kBlue),LineStyle(kDashed));
        effFunc.plotOn(right,Name("effPDF"),LineColor(kBlue),LineStyle(kDashed));

        
/*        std::cout << "INFO BLOCK FOR: " << var.GetName() << std::endl;
        std::cout << "  chi^2 = " << right->chiSquare("effPDF","data_hist") * (opt_bins - effFunc.getParameters(data)->getSize()) << std::endl;
        std::cout << "  param = " << effFunc.getParameters(data)->getSize() << std::endl;
        std::cout << "  bins = " << opt_bins << std::endl;
        std::cout << "  ndf = " << opt_bins - effFunc.getParameters(data)->getSize() << std::endl;
        std::cout << "  chi^2/ndf = " << right->chiSquare("effPDF","data_hist",effFunc.getParameters(data)->getSize()) << std::endl;
        std::cout << "  Prob = " << TMath::Prob(right->chiSquare("effPDF","data_hist") * (opt_bins - effFunc.getParameters(data)->getSize()),
                                                opt_bins-effFunc.getParameters(data)->getSize()) << std::endl;
        */
        // draw all frames on a canvas
        TCanvas* canvas = new TCanvas("canvas","canvas",2*1280,1024) ;
        canvas->Divide(2,1);
        canvas->GetPad(1)->SetLeftMargin(0.15);
        canvas->GetPad(2)->SetLeftMargin(0.15);
        canvas->cd(1);
        left->GetYaxis()->SetTitleOffset(1.6);
        left->Draw("");
        canvas->cd(2);
        right->GetYaxis()->SetTitleOffset(1.4);
        right->SetMinimum(0.0);
        right->SetMaximum(0.5);
        right->Draw("");
        canvas->cd();
        canvas->SaveAs((opt_outdir+"efficiency_"+varname1+":"+varname2+flag+"."+opt_plot_format+"").c_str());
        canvas->SaveAs((opt_outdir+"efficiency_"+varname1+":"+varname2+flag+"."+opt_plot_format+".root").c_str());
        
        eff_result->Print();
        
        delete canvas;
        delete eff_result;
    }
    
    
    
    /**
     * for i in *.log; do echo '\\begin{frame}\n\\frametitle{Fit result on MC}\n\\begin{tiny}\n\\begin{columns}\n\\column{0.5\\textwidth}' > tmp.latex; grep -A 51 "LATEX TABLE - START" $i | tail -n 48 | sed 's/^$/\\column{0.5\\textwidth}/g' >> tmp.latex; echo '\\end{columns}\n\\end{tiny}\n\\end{frame}' >> tmp.latex; mv tmp.latex fit_$i.tex; done
     *
     * @param pdf
     * @param truth
     */
    void PrintFitResultLatex(shared_ptr<FF_PDF> pdf, std::map<std::string, double> truth) {
        using namespace std;
        
        std::vector<std::string> parameters;
        parameters.push_back("signal_nevents");
        parameters.push_back("amp_00");
        parameters.push_back("amp_10");
        parameters.push_back("amp_1para");
        parameters.push_back("amp_1perp");
        parameters.push_back("amp_20");
        parameters.push_back("amp_2para");
        parameters.push_back("amp_2perp");
        parameters.push_back("phase_00");
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
        parameters.push_back("phase_00_delta");
        parameters.push_back("phase_10_delta");
        parameters.push_back("phase_1para_delta");
        parameters.push_back("phase_1perp_delta");
        parameters.push_back("phase_20_delta");
        parameters.push_back("phase_2para_delta");
        parameters.push_back("phase_2perp_delta");
        
        if(truth.size() > 0) {
            PrintFitStartBlock("LATEX TABLE - START");
            cout << "\\begin{table}[!ht]" << endl;
            cout << "\\centering" << endl;
            cout << "\\begin{tabular}{lrrr} \\toprule" << endl;
            cout << "Parameter     &  Fit Result  &   MC  & $\\sigma$ \\\\ \\hline" << endl;
            
            for(auto& pair : truth) {
                if(pair.first.find("delta") == pair.first.npos) {
                    if(std::find(parameters.begin(),parameters.end(),pair.first) != parameters.end()) {
                        if(pdf->GetVariable(pair.first)->isConstant() == false) {
                            cout << "$" << GetLabelToLatex(pair.first) << "$ & $ " << fixed << setprecision(3) << pdf->GetVariable(pair.first)->getVal() << "^{+";
                            cout << fixed << setprecision(3) << pdf->GetVariable(pair.first)->getErrorHi() << "}_{";
                            cout << fixed << setprecision(3) << pdf->GetVariable(pair.first)->getErrorLo() << "} $ & $ ";
                            cout << fixed << setprecision(3) << pair.second << " $ & $ ";
                            double sigma = pdf->GetVariable(pair.first)->getVal() - pair.second;
                            if(pdf->GetVariable(pair.first)->getVal() < pair.second)
                                sigma /= pdf->GetVariable(pair.first)->getErrorHi();
                            else
                                sigma /= -1*pdf->GetVariable(pair.first)->getErrorLo();
                            cout << fixed << setprecision(1) << sigma << "$\\\\" << endl;
                        }  else {
                            cout << "$" << GetLabelToLatex(pair.first) << "$ & $ " << fixed << setprecision(3) << pdf->GetVariable(pair.first)->getVal() << " $ & $ ";
                            cout << fixed << setprecision(3) << pair.second << " $ & fix \\\\" << endl;
                        }
                    }
                }
            }
            
            cout << "\\bottomrule \n \\end{tabular}" << endl;
            cout << "\\caption{Polarization parameters}" << endl;
            cout << "\\label{tab:fit_results_polarization}" << endl;
            cout << "\\end{table}" << endl;
            cout << endl;
            cout << "\\begin{table}[!ht]" << endl;
            cout << "\\centering" << endl;
            cout << "\\begin{tabular}{lrrr} \\toprule" << endl;
            cout << "Parameter     &  Fit Result  &   MC  & $\\sigma$ \\\\ \\hline" << endl;
            
            for(auto& pair : truth) {
                if(pair.first.find("delta") < pair.first.npos) {
                    if(std::find(parameters.begin(),parameters.end(),pair.first) != parameters.end()) {
                        if(pdf->GetVariable(pair.first)->isConstant() == false) {
                            cout << "$" << GetLabelToLatex(pair.first) << "$ & $ " << fixed << setprecision(3) << pdf->GetVariable(pair.first)->getVal() << "^{+";
                            cout << fixed << setprecision(3) << pdf->GetVariable(pair.first)->getErrorHi() << "}_{";
                            cout << fixed << setprecision(3) << pdf->GetVariable(pair.first)->getErrorLo() << "} $ & $ ";
                            cout << fixed << setprecision(3) << pair.second << " $ & $ ";
                            double sigma = pdf->GetVariable(pair.first)->getVal() - pair.second;
                            if(pdf->GetVariable(pair.first)->getVal() < pair.second)
                                sigma /= pdf->GetVariable(pair.first)->getErrorHi();
                            else
                                sigma /= -1*pdf->GetVariable(pair.first)->getErrorLo();
                            cout << fixed << setprecision(1) << sigma << "$\\\\" << endl;
                        } else {
                            cout << "$" << GetLabelToLatex(pair.first) << "$ & $ " << fixed << setprecision(3) << pdf->GetVariable(pair.first)->getVal() << " $ & $ ";
                            cout << fixed << setprecision(3) << pair.second << " $ & fix \\\\" << endl;
                        }
                    }
                }
            }
            
            cout << "\\bottomrule \n \\end{tabular}" << endl;
            cout << "\\caption{Direct CPV parameters}" << endl;
            cout << "\\label{tab:fit_results_CPV}" << endl;
            cout << "\\end{table}" << endl;
            PrintFitStartBlock("LATEX TABLE - END");
        }else {
            PrintFitStartBlock("LATEX TABLE - START");
            cout << "\\begin{table}[!ht]" << endl;
            cout << "\\centering" << endl;
            cout << "\\begin{tabular}{lr} \\toprule" << endl;
            cout << "Parameter     &  Fit Result  \\\\ \\hline" << endl;
            
            for(const std::string& param : parameters) {
                if(param.find("delta") == param.npos) {
                    if(pdf->GetVariable(param)->isConstant() == false) {
                        cout << "$" << GetLabelToLatex(param) << "$ & $ " << fixed << setprecision(3) << pdf->GetVariable(param)->getVal() << "^{+";
                        cout << fixed << setprecision(3) << pdf->GetVariable(param)->getErrorHi() << "}_{";
                        cout << fixed << setprecision(3) << pdf->GetVariable(param)->getErrorLo() << "} $ \\\\" << endl;
                    }  else {
                        cout << "$" << GetLabelToLatex(param) << "$ & $ " << fixed << setprecision(3) << pdf->GetVariable(param)->getVal() << " $ \\\\" << endl;
                    }
                }
            }
            
            cout << "\\bottomrule \n \\end{tabular}" << endl;
            cout << "\\caption{Polarization parameters}" << endl;
            cout << "\\label{tab:fit_results_polarization}" << endl;
            cout << "\\end{table}" << endl;
            cout << endl;
            cout << "\\begin{table}[!ht]" << endl;
            cout << "\\centering" << endl;
            cout << "\\begin{tabular}{lr} \\toprule" << endl;
            cout << "Parameter     &  Fit Result  \\\\ \\hline" << endl;
            
            for(const std::string& param : parameters) {
                if(param.find("delta") < param.npos) {
                    if(pdf->GetVariable(param)->isConstant() == false) {
                        cout << "$" << GetLabelToLatex(param) << "$ & $ " << fixed << setprecision(3) << pdf->GetVariable(param)->getVal() << "^{+";
                        cout << fixed << setprecision(3) << pdf->GetVariable(param)->getErrorHi() << "}_{";
                        cout << fixed << setprecision(3) << pdf->GetVariable(param)->getErrorLo() << "} $ \\\\" << endl;
                    } else {
                        cout << "$" << GetLabelToLatex(param) << "$ & $ " << fixed << setprecision(3) << pdf->GetVariable(param)->getVal() << " $ \\\\" << endl;
                    }
                }
            }
            
            cout << "\\bottomrule \n \\end{tabular}" << endl;
            cout << "\\caption{Direct CPV parameters}" << endl;
            cout << "\\label{tab:fit_results_CPV}" << endl;
            cout << "\\end{table}" << endl;
            PrintFitStartBlock("LATEX TABLE - END");
        }
    }
    
    void PrintFitResultLatex(shared_ptr<FF_PDF> pdf) {
        std::map<std::string, double> truth;
        PrintFitResultLatex(pdf, truth);
    }
    
    void ShiftPhaseToInterval(shared_ptr<FF_PDF> pdf, const std::string& varname, double truth) {
        RooRealVar* var = pdf->GetVariable(varname);
        while(var->getVal() - truth  > TMath::Pi())
            var->setVal(var->getVal() - 2*TMath::Pi());
        while(var->getVal() - truth  < -TMath::Pi())
            var->setVal(var->getVal() + 2*TMath::Pi());
    }
    
    // compute effective width from the signal DeltaE pdf
    std::map<std::string,double> ComputeEffectiveWidth(shared_ptr<FF_PDF> pdf) {
        double frac_core = pdf->GetVariable("C_deltae_signal_core_frac")->getVal();
        double width_core = pdf->GetVariable("C_deltae_signal_width_core")->getVal();
        double frac_1st = pdf->GetVariable("C_deltae_signal_1st_frac")->getVal();
        double width_1st = 0.5*(pdf->GetVariable("C_deltae_signal_width_1st_left")->getVal()+pdf->GetVariable("C_deltae_signal_width_1st_right")->getVal());
        double width_2nd = 0.5*(pdf->GetVariable("C_deltae_signal_width_2nd_left")->getVal()+pdf->GetVariable("C_deltae_signal_width_2nd_right")->getVal());
        
        double width_eff = frac_core*width_core + (1-frac_core)*(frac_1st*width_1st + (1-frac_1st)*width_2nd);
        std::map<std::string,double> result;
        result["width_eff"] = width_eff;
        return result;
    }
    
    // computes propagated errors using toys and cholesky transformation
    std::map<std::string, std::pair<double,double> > PropagateWidthError(shared_ptr<FF_PDF> pdf, RooFitResult* result, shared_ptr<FFOpt> opt, bool verbose) {
        // decompose the correlation matrix (correlation matrix would be fine if we rotate normal Gaussian
        // distributioned variables with mean 0 and width 1, in that case correlation and covariance matrix
        // are the same)
        TDecompChol chol(result->correlationMatrix());
        // decompose the covariance matrix
        //TDecompChol chol(result->covarianceMatrix());
        std::cout << "INFO: Decomposing Matrix..." << std::endl;
        if(!(chol.Decompose())) {
            std::cerr << "ERROR: Matrix decomposition failed!" << std::endl;
            std::exit(1);
        }
        const TMatrixD& U = chol.GetU();
        TMatrixD L(U);
        L.Transpose(U);
        std::cout << "...done" << std::endl;
        
        if(verbose) {
            std::cout << "DEBUG: Original matrix:" << std::endl;
            result->correlationMatrix().Print();
            //result->covarianceMatrix().Print();
            std::cout << "DEBUG: Decomposed matrix:" << std::endl;
            U.Print();
            L.Print();
        }
        
        // prepare the fit params and errors
        std::vector<std::string> param_names;
        param_names.reserve(result->floatParsFinal().getSize());
        for(int i = 0; i < result->floatParsFinal().getSize(); ++i) {
            param_names.push_back(result->floatParsFinal()[i].GetName());
        }
        if(verbose) {
            std::cout << "DEBUG: Parameter names: " << std::endl;
            std::cout << mp::vector_to_string(param_names,true);
        }
        std::map<std::string, double> fit_params;
        std::map<std::string, double> fit_errors;
        for(const std::string& par : param_names) {
            RooRealVar* variable = pdf->GetVariable(par);
            fit_params[par] = variable->getVal();
            double error_lo = -1*variable->getErrorLo();
            double error_hi = variable->getErrorHi();
            // make sure none of the errors is 0
            if(error_lo == 0.0) {
                error_lo = variable->getError();
                std::cerr << "ERROR: Zero size lower error for parameter " << par << ". Using symmetric errors instead. "<< std::endl;
            }
            if(error_hi == 0.0) {
                error_hi = variable->getError();
                std::cerr << "ERROR: Zero size higher error for parameter " << par << ". Using symmetric errors instead. "<< std::endl;
            }
            fit_errors[par] = (error_lo + error_hi) / 2.0;
            double asym = (error_lo - error_hi) / (error_lo + error_hi);
            if(std::fabs(asym) > 0.1)
                std::cout << "ERROR: Large asymmetry between errors for parameter " << par << std::endl;
        }
        
        // now do 1000 toys and compute the physics parameters for each toy
        std::map<std::string, std::vector<double> > toy_results;
        std::map<std::string, std::vector<double> > cor_random_numbers;
        for(unsigned int i_toy = 1; i_toy <= 10000; ++i_toy) {
            if(i_toy%1000 == 0) std::cout << "INFO: Status: " << std::setw(4) << i_toy << "/10000" << std::endl;
            // generate uncorrelated random numbers
            // order of the vector needs to correspond to the matrix order
            TVectorD uncorrelated_lo(param_names.size());
            for(unsigned int i = 0; i < param_names.size(); ++i) {
                uncorrelated_lo[i] = RooRandom::randomGenerator()->Gaus(0,1);
            }
            
            // correlate random numbers
            TVectorD correlated_lo = uncorrelated_lo;
            correlated_lo *= L;
            
            if(verbose && i_toy%1000 == 0) {
                std::cout << "DEBUG: Uncorrelated and correlated random numbers: " << std::endl;
                uncorrelated_lo.Print();
                correlated_lo.Print();
            }
            
            // set the pdf values with modified parameter set
            for(unsigned int i = 0; i < param_names.size(); ++i) {
                // scale correlated random number to error (needed if correlation matix is used)
                correlated_lo[i] = correlated_lo[i] * fit_errors[param_names[i]];
                // store correlated random numbers for later check
                cor_random_numbers[param_names[i]].push_back(correlated_lo[i]);
                // compute new value for variable
                RooRealVar* variable = pdf->GetVariable(param_names[i]);
                double new_value = fit_params[param_names[i]]+correlated_lo[i];
                // check if values of this set don't exceed limits
                if(variable->getMin() > new_value) {
                    std::cerr << "ERROR: Toy parameter " << param_names[i] << " = " << new_value << " exceeds lower limit " << variable->getMin() << " and was reset." << std::endl;
                    //new_value = variable->getMin();
                }
                if(variable->getMax() < new_value) {
                    std::cerr << "ERROR: Toy parameter " << param_names[i] << " = " << new_value << " exceeds upper limit " << variable->getMax() << " and was reset." << std::endl;
                    //new_value = variable->getMax();
                }
                
                // set value for this toy
                variable->setVal(new_value);
            }
            
            // compute the physics parameters with the modified set
            std::map<std::string, double> toy_result = ComputeEffectiveWidth(pdf);
            for(auto& p : toy_result) {
                toy_results[p.first].push_back(p.second);
            }
        }
        
        // reset the pdf values to fit parameter set
        for(unsigned int i = 0; i < param_names.size(); ++i) {
            pdf->GetVariable(param_names[i])->setVal(fit_params[param_names[i]]);
        }
        
        std::map<std::string, std::string> label_to_TLatex;
        label_to_TLatex["C_deltae_signal_core_frac"] = "f";
        label_to_TLatex["C_deltae_signal_width_core"] = "#sigma";
        label_to_TLatex["C_deltae_signal_1st_frac"] = "f_{1st}";
        label_to_TLatex["C_deltae_signal_width_1st_left"] = "#sigma_{1st}^{l}";
        label_to_TLatex["C_deltae_signal_width_1st_right"] = "#sigma_{1st}^{r}";
        label_to_TLatex["C_deltae_signal_width_2nd_left"] = "#sigma_{2nd}^{l}";
        label_to_TLatex["C_deltae_signal_width_2nd_right"] = "#sigma_{2nd}^{r}";
        
        // make control plots of all random numbers we created
        std::cout << "INFO: Now doing control plots for correlated random numbers..." << std::endl;
        for(unsigned int i = 0; i < param_names.size(); ++i) {
            double min = -fit_errors[param_names[i]]*5;
            double max = fit_errors[param_names[i]]*5;
            
            TCanvas* canvas = new TCanvas("canvas","canvas",1280,1024);
            canvas->cd();
            
            TH1F* hist = new TH1F("hist","hist",100,min,max);
            for(double d : cor_random_numbers[param_names[i]]) {
                hist->Fill(d);
            }
            
            hist->Draw("HIST");
            hist->GetYaxis()->SetTitle("Entries");
            if(label_to_TLatex.count(param_names[i]))
                hist->GetXaxis()->SetTitle(("Random deviation of parameter " + label_to_TLatex[param_names[i]]).c_str());
            else
                hist->GetXaxis()->SetTitle(("Random deviation of parameter " + param_names[i]).c_str());
            
            TF1* gauss = new TF1("gauss","[0]*(exp(-0.5*((x-[1])/[2])**2))",min,max);
            gauss->SetLineStyle(kDashed);
            gauss->SetLineColor(kBlue);
            gauss->SetParameter(0,hist->GetEntries());
            gauss->SetParameter(1,hist->GetMean());
            gauss->SetParameter(2,hist->GetRMS());
            hist->Fit(gauss,"0");
            gauss->Draw("same");
            
            TLatex text;
            text.SetTextColor(kBlack);
            text.SetTextFont(42);
            text.SetTextSize(0.045);
            text.SetNDC(1);
            std::stringstream ss;
            ss.precision(3);
            ss << "m = " << gauss->GetParameter(1) << " #pm " << gauss->GetParError(1);
            text.DrawLatex(0.18, 0.9, ss.str().c_str());
            ss.str(std::string());
            ss << "#sigma = " << gauss->GetParameter(2) << " #pm " << gauss->GetParError(2);
            text.DrawLatex(0.18, 0.85, ss.str().c_str());
            ss.str(std::string());
            ss << "Error of " << label_to_TLatex[param_names[i]] << " is " << fit_errors[param_names[i]];
            text.DrawLatex(0.18, 0.8, ss.str().c_str());
            
            canvas->cd();
            canvas->SaveAs((opt->GetOutdir()+"correlated_random_numbers_for_"+param_names[i]+".eps").c_str());
            canvas->SaveAs((opt->GetOutdir()+"correlated_random_numbers_for_"+param_names[i]+".root").c_str());
            
            delete gauss;
            delete hist;
            delete canvas;
        }
        
        std::map<std::string,std::string> physic_param_to_TLatex;
        physic_param_to_TLatex["width_eff"] = "#sigma_{#Delta E}";
        
        std::map<std::string, std::pair<double,double> > physics_params;
        
        // look at the resulting distribution for each physics parameter and
        // determine the errors by computing mean and width of the distribution
        std::cout << "INFO: Now determining propagated errors..." << std::endl;
        for(auto& p : toy_results) {
            
            double mean = std::accumulate(p.second.begin(),p.second.end(),0.0)/p.second.size();
            double sum = 0;
            for(double d : p.second) {
                sum += (d - mean)*(d - mean);
            }
            sum /= p.second.size() - 1;
            double width = std::sqrt(sum);
            
            double min = mean - width*5;
            double max = mean + width*5;
            
            TCanvas* canvas = new TCanvas("canvas","canvas",1280,1024);
            canvas->cd();
            
            TH1F* hist = new TH1F("hist","hist",100,min,max);
            for(double d : p.second) {
                hist->Fill(d);
            }
            
            hist->Draw("HIST");
            hist->GetYaxis()->SetTitle("Entries");
            if(physic_param_to_TLatex.count(p.first))
                hist->GetXaxis()->SetTitle(("Parameter " + physic_param_to_TLatex[p.first]).c_str());
            else
                hist->GetXaxis()->SetTitle(("Parameter " + p.first).c_str());
            
            TF1* gauss = new TF1("gauss","[0]*(exp(-0.5*((x-[1])/[2])**2))",min,max);
            gauss->SetLineStyle(kDashed);
            gauss->SetLineColor(kBlue);
            gauss->SetParameter(0,hist->GetEntries());
            gauss->SetParameter(1,hist->GetMean());
            gauss->SetParameter(2,hist->GetRMS());
            hist->Fit(gauss,"0");
            gauss->Draw("same");
            
            TLatex text;
            text.SetTextColor(kBlack);
            text.SetTextFont(42);
            text.SetTextSize(0.045);
            text.SetNDC(1);
            std::stringstream ss;
            ss.precision(3);
            ss << "m = " << gauss->GetParameter(1) << " #pm " << gauss->GetParError(1);
            text.DrawLatex(0.18, 0.9, ss.str().c_str());
            ss.str(std::string());
            ss << "#sigma = " << gauss->GetParameter(2) << " #pm " << gauss->GetParError(2);
            text.DrawLatex(0.18, 0.85, ss.str().c_str());
            
            physics_params[p.first] = std::make_pair(gauss->GetParameter(1),gauss->GetParameter(2));
            
            canvas->cd();
            std::string s_p = p.first;
            if(s_p.find("||") < s_p.npos)
                s_p.replace(s_p.find("||"),s_p.find("||")+2,"PA");
            canvas->SaveAs((opt->GetOutdir()+"physics_parameter_"+s_p+".eps").c_str());
            canvas->SaveAs((opt->GetOutdir()+"physics_parameter_"+s_p+".root").c_str());
            
            delete gauss;
            delete hist;
            delete canvas;
        }
        
        return physics_params;
    }
    
    double shift_pi_periodic_to_truth_value(double value, double truth = 0) {
        while(value - truth > TMath::Pi()/2.0)
            value -= TMath::Pi();
        while(value - truth < -TMath::Pi()/2.0)
            value += TMath::Pi();
        return value;
    }
    
    double shift_2pi_periodic_to_truth_value(double value, double truth = 0) {
        while(value - truth > TMath::Pi())
            value -= 2*TMath::Pi();
        while(value - truth < -TMath::Pi())
            value += 2*TMath::Pi();
        return value;
    }
    
    std::string get_polar_coordinate_string(const std::complex<double>& c) {
        std::stringstream ss;
        ss << std::fixed;
        ss.precision(6);
        ss << "(" << std::abs(c) << "," << std::arg(c) << ")";
        return ss.str();
    }
    
    // compute the physics parameters of interest according to their definitions
    std::map<std::string,double> ComputePhysicsParameters(shared_ptr<FF_PDF> pdf, shared_ptr<FF_PDF> signal_pdf, const RooArgSet& varset, bool verbose) {
        std::map<std::string,double> results;
        
        std::complex<double> A_00 = std::polar((double)pdf->GetFunction("CP_amp_00")->getVal(),(double)pdf->GetFunction("CP_phase_00")->getVal());
        std::complex<double> A_00_bar = std::polar((double)pdf->GetFunction("CA_amp_00")->getVal(),(double)pdf->GetFunction("CA_phase_00")->getVal());
        
        std::complex<double> A_10 = std::polar((double)pdf->GetFunction("CP_amp_10")->getVal(),(double)pdf->GetFunction("CP_phase_10")->getVal());
        std::complex<double> A_10_bar = std::polar((double)pdf->GetFunction("CA_amp_10")->getVal(),(double)pdf->GetFunction("CA_phase_10")->getVal());
        std::complex<double> A_1para = std::polar((double)pdf->GetFunction("CP_amp_1para")->getVal(),(double)pdf->GetFunction("CP_phase_1para")->getVal());
        std::complex<double> A_1para_bar = std::polar((double)pdf->GetFunction("CA_amp_1para")->getVal(),(double)pdf->GetFunction("CA_phase_1para")->getVal());
        std::complex<double> A_1perp = std::polar((double)pdf->GetFunction("CP_amp_1perp")->getVal(),(double)pdf->GetFunction("CP_phase_1perp")->getVal());
        std::complex<double> A_1perp_bar = std::polar((double)pdf->GetFunction("CA_amp_1perp")->getVal(),(double)pdf->GetFunction("CA_phase_1perp")->getVal());
        
        std::complex<double> A_20 = std::polar((double)pdf->GetFunction("CP_amp_20")->getVal(),(double)pdf->GetFunction("CP_phase_20")->getVal());
        std::complex<double> A_20_bar = std::polar((double)pdf->GetFunction("CA_amp_20")->getVal(),(double)pdf->GetFunction("CA_phase_20")->getVal());
        std::complex<double> A_2para = std::polar((double)pdf->GetFunction("CP_amp_2para")->getVal(),(double)pdf->GetFunction("CP_phase_2para")->getVal());
        std::complex<double> A_2para_bar = std::polar((double)pdf->GetFunction("CA_amp_2para")->getVal(),(double)pdf->GetFunction("CA_phase_2para")->getVal());
        std::complex<double> A_2perp = std::polar((double)pdf->GetFunction("CP_amp_2perp")->getVal(),(double)pdf->GetFunction("CP_phase_2perp")->getVal());
        std::complex<double> A_2perp_bar = std::polar((double)pdf->GetFunction("CA_amp_2perp")->getVal(),(double)pdf->GetFunction("CA_phase_2perp")->getVal());
        
        if(verbose) {
            std::cout << "A_00    / CP:    " << get_polar_coordinate_string(A_00) << "   (R,I): " << A_00 << "\n";
            std::cout << "A_00bar / CA:    " << get_polar_coordinate_string(A_00_bar) << "   (R,I): " << A_00_bar << "\n";
            
            std::cout << "A_10       / CP: " << get_polar_coordinate_string(A_10) << "   (R,I): " << A_10 << "\n";
            std::cout << "A_10bar    / CA: " << get_polar_coordinate_string(A_10_bar) << "   (R,I): " << A_10_bar << "\n";
            std::cout << "A_1para    / CP: " << get_polar_coordinate_string(A_1para) << "   (R,I): " << A_1para << "\n";
            std::cout << "A_1para_bar/ CA: " << get_polar_coordinate_string(A_1para_bar) << "   (R,I): " << A_1para_bar << "\n";
            std::cout << "A_1perp    / CP: " << get_polar_coordinate_string(A_1perp) << "   (R,I): " << A_1perp << "\n";
            std::cout << "A_1perp_bar/ CA: " << get_polar_coordinate_string(A_1perp_bar) << "   (R,I): " << A_1perp_bar << "\n";
            
            std::cout << "A_20       / CP: " << get_polar_coordinate_string(A_20) << "   (R,I): " << A_20 << "\n";
            std::cout << "A_20bar    / CA: " << get_polar_coordinate_string(A_20_bar) << "   (R,I): " << A_20_bar << "\n";
            std::cout << "A_2para    / CP: " << get_polar_coordinate_string(A_2para) << "   (R,I): " << A_2para << "\n";
            std::cout << "A_2para_bar/ CA: " << get_polar_coordinate_string(A_2para_bar) << "   (R,I): " << A_2para_bar << "\n";
            std::cout << "A_2perp    / CP: " << get_polar_coordinate_string(A_2perp) << "   (R,I): " << A_2perp << "\n";
            std::cout << "A_2perp_bar/ CA: " << get_polar_coordinate_string(A_2perp_bar) << "   (R,I): " << A_2perp_bar << "\n";
        }
        
        double sum_A0 = std::norm(A_00);
        double sum_A0_bar = std::norm(A_00_bar);
        
        double sum_A1 = std::norm(A_10) + std::norm(A_1perp) + std::norm(A_1para);
        double sum_A1_bar = std::norm(A_10_bar) + std::norm(A_1perp_bar) + std::norm(A_1para_bar);
        
        double sum_A2 = std::norm(A_20) + std::norm(A_2perp) + std::norm(A_2para);
        double sum_A2_bar = std::norm(A_20_bar) + std::norm(A_2perp_bar) + std::norm(A_2para_bar);
        
        // polarization fractions
        results["fL0"] = 0.5*(std::norm(A_00_bar)/sum_A0_bar + std::norm(A_00)/sum_A0);
        results["fL1"] = 0.5*(std::norm(A_10_bar)/sum_A1_bar + std::norm(A_10)/sum_A1);
        results["fL2"] = 0.5*(std::norm(A_20_bar)/sum_A2_bar + std::norm(A_20)/sum_A2);
        
        results["fP1"] = 0.5*(std::norm(A_1perp_bar)/sum_A1_bar + std::norm(A_1perp)/sum_A1);
        results["fP2"] = 0.5*(std::norm(A_2perp_bar)/sum_A2_bar + std::norm(A_2perp)/sum_A2);
        
        // phases
        results["phi_||1"] = 0.5*(std::arg(A_1para_bar/A_10_bar) + std::arg(A_1para/A_10));
        results["phi_||2"] = 0.5*(std::arg(A_2para_bar/A_20_bar) + std::arg(A_2para/A_20));
        
        results["phi_P1"] = 0.5*(std::arg(A_1perp_bar/A_10_bar) + shift_2pi_periodic_to_truth_value(std::arg(A_1perp/A_10) - (double)TMath::Pi()));
        results["phi_P2"] = 0.5*(std::arg(A_2perp_bar/A_20_bar) + shift_2pi_periodic_to_truth_value(std::arg(A_2perp/A_20) - (double)TMath::Pi()));
        
        results["delta_00"] = 0.5*(std::arg(A_00_bar/A_00_bar) + std::arg(A_00/A_00));
        results["delta_01"] = 0.5*(std::arg(A_00_bar/A_10_bar) + std::arg(A_00/A_10));
        results["delta_02"] = 0.5*(std::arg(A_00_bar/A_20_bar) + std::arg(A_00/A_20));
        
        // move the phases to the (0,2pi) interval for better comparision with BaBar
        results["phi_||1"] = shift_2pi_periodic_to_truth_value(results["phi_||1"],TMath::Pi());
        results["phi_||2"] = shift_2pi_periodic_to_truth_value(results["phi_||2"],TMath::Pi());
        results["phi_P1"] = shift_2pi_periodic_to_truth_value(results["phi_P1"],TMath::Pi());
        results["phi_P2"] = shift_2pi_periodic_to_truth_value(results["phi_P2"],TMath::Pi());
        results["delta_01"] = shift_2pi_periodic_to_truth_value(results["delta_01"],TMath::Pi());
        results["delta_02"] = shift_2pi_periodic_to_truth_value(results["delta_02"],TMath::Pi());
        
        // asymmetries
        results["A_CP0_0"] = (std::norm(A_00_bar)/sum_A0_bar - std::norm(A_00)/sum_A0)/(std::norm(A_00_bar)/sum_A0_bar + std::norm(A_00)/sum_A0);
        results["A_CP1_0"] = (std::norm(A_10_bar)/sum_A1_bar - std::norm(A_10)/sum_A1)/(std::norm(A_10_bar)/sum_A1_bar + std::norm(A_10)/sum_A1);
        results["A_CP2_0"] = (std::norm(A_20_bar)/sum_A2_bar - std::norm(A_20)/sum_A2)/(std::norm(A_20_bar)/sum_A2_bar + std::norm(A_20)/sum_A2);
        
        results["A_CP1_P"] = (std::norm(A_1perp_bar)/sum_A1_bar - std::norm(A_1perp)/sum_A1)/(std::norm(A_1perp_bar)/sum_A1_bar + std::norm(A_1perp)/sum_A1);
        results["A_CP2_P"] = (std::norm(A_2perp_bar)/sum_A2_bar - std::norm(A_2perp)/sum_A2)/(std::norm(A_2perp_bar)/sum_A2_bar + std::norm(A_2perp)/sum_A2);
        
        // Note: Delta phi/delta of pi between phases corresponds to 2pi difference between both phases, which corresponds to 0 phase difference
        // The Delta values are therefore pi periodic and shifted towards zero
        
        // phases-asymmetries
        results["Delta_phi_||1"] = shift_pi_periodic_to_truth_value(0.5*(std::arg(A_1para_bar/A_10_bar) - std::arg(A_1para/A_10)));
        results["Delta_phi_||2"] = shift_pi_periodic_to_truth_value(0.5*(std::arg(A_2para_bar/A_20_bar) - std::arg(A_2para/A_20)));
        
        results["Delta_phi_P1"] = shift_pi_periodic_to_truth_value(0.5*(std::arg(A_1perp_bar/A_10_bar) - shift_2pi_periodic_to_truth_value(std::arg(A_1perp/A_10) - (double)TMath::Pi())));
        results["Delta_phi_P2"] = shift_pi_periodic_to_truth_value(0.5*(std::arg(A_2perp_bar/A_20_bar) - shift_2pi_periodic_to_truth_value(std::arg(A_2perp/A_20) - (double)TMath::Pi())));
        
        results["Delta_delta_00"] = shift_pi_periodic_to_truth_value(0.5*(std::arg(A_00_bar/A_00_bar) - std::arg(A_00/A_00)));
        results["Delta_delta_01"] = shift_pi_periodic_to_truth_value(0.5*(std::arg(A_00_bar/A_10_bar) - std::arg(A_00/A_10)));
        results["Delta_delta_02"] = shift_pi_periodic_to_truth_value(0.5*(std::arg(A_00_bar/A_20_bar) - std::arg(A_00/A_20)));
        
        // CP-violation in mixing
        results["Delta_phi_00"] = shift_pi_periodic_to_truth_value(0.5*(std::arg(A_00/A_00_bar)));
        
        // yields
        results["signal_yield"] = (double)pdf->GetVariable("signal_nevents")->getVal();
        shared_ptr<FF_PDF_signal_9D> signal_ref(std::dynamic_pointer_cast<FF_PDF_signal_9D>(signal_pdf));
        if (!signal_ref) {
            std::cerr << "ERROR: Could not cast signal_pdf shared pointer to FF_PDF_signal_9D" << std::endl;
        }
        results["signal_asym"] = (double)signal_ref->getAsymmetry();
        
        // some values for branching fraction computation
        const double frac_KPI_to_charged = 0.6666666;
        const double frac_phi_to_KK     = 0.489; // +0.005 -0.005
        const double frac_K892_to_KPI   = frac_phi_to_KK*frac_KPI_to_charged*1.0;
        const double frac_LASS_to_KPI   = frac_phi_to_KK*frac_KPI_to_charged*1.0;   // for K01430: 0.93 +0.07 -0.1
        const double frac_K21430_to_KPI = frac_phi_to_KK*frac_KPI_to_charged*0.499; // +0.012 -0.012
        const double NBB = 771.581e6; // +10.566e6 -10.566e6
        
        // constraint model efficiency values
        //const double efficiency_LASS   = 0.287232;
        //const double efficiency_K892   = 0.256786;
        //const double efficiency_K21430 = 0.163106;
        
        // updated model efficiency values
        const double efficiency_LASS   = 0.287232;
        const double efficiency_K892   = 0.259482;
        const double efficiency_K21430 = 0.162910;
        
        RooAbsReal* I_tot = signal_pdf->GetPDF("C").createIntegral(varset,"full_range");
        double N_tot = I_tot->getVal();
        
        signal_pdf->GetVariable("amp_10")->setVal(0.0);
        signal_pdf->GetVariable("amp_10_delta")->setVal(0.0);
        signal_pdf->GetVariable("amp_1perp")->setVal(0.0);
        signal_pdf->GetVariable("amp_1perp_delta")->setVal(0.0);
        signal_pdf->GetVariable("amp_1para")->setVal(0.0);
        signal_pdf->GetVariable("amp_1para_delta")->setVal(0.0);
        signal_pdf->GetVariable("amp_20")->setVal(0.0);
        signal_pdf->GetVariable("amp_20_delta")->setVal(0.0);
        signal_pdf->GetVariable("amp_2perp")->setVal(0.0);
        signal_pdf->GetVariable("amp_2perp_delta")->setVal(0.0);
        signal_pdf->GetVariable("amp_2para")->setVal(0.0);
        signal_pdf->GetVariable("amp_2para_delta")->setVal(0.0);
        double N_LASS = I_tot->getVal();
        results["frac_LASS"] = N_LASS/N_tot;
        results["frac_LASS_asym"] = (double)signal_ref->getAsymmetry();
        results["yield_LASS"] = results["frac_LASS"]*results["signal_yield"];
        results["BR_LASS"] = results["yield_LASS"]/(NBB*frac_LASS_to_KPI*efficiency_LASS);
        
        signal_pdf->GetVariable("amp_00")->setVal(0.0);
        signal_pdf->GetVariable("amp_00_delta")->setVal(0.0);
        signal_pdf->GetVariable("amp_10")->setVal((std::abs(A_10) + std::abs(A_10_bar))/2.0);
        signal_pdf->GetVariable("amp_10_delta")->setVal((std::abs(A_10) - std::abs(A_10_bar))/(std::abs(A_10) + std::abs(A_10_bar)));
        signal_pdf->GetVariable("amp_1perp")->setVal((std::abs(A_1perp) + std::abs(A_1perp_bar))/2.0);
        signal_pdf->GetVariable("amp_1perp_delta")->setVal((std::abs(A_1perp) - std::abs(A_1perp_bar))/(std::abs(A_1perp) + std::abs(A_1perp_bar)));
        signal_pdf->GetVariable("amp_1para")->setVal((std::abs(A_1para) + std::abs(A_1para_bar))/2.0);
        signal_pdf->GetVariable("amp_1para_delta")->setVal((std::abs(A_1para) - std::abs(A_1para_bar))/(std::abs(A_1para) + std::abs(A_1para_bar)));
        double N_K892 = I_tot->getVal();
        results["frac_K892"] = N_K892/N_tot;
        results["frac_K892_asym"] = (double)signal_ref->getAsymmetry();;
        results["yield_K892"] = results["frac_K892"]*results["signal_yield"];
        results["BR_K892"] = results["yield_K892"]/(NBB*frac_K892_to_KPI*efficiency_K892);
        
        signal_pdf->GetVariable("amp_10")->setVal(0.0);
        signal_pdf->GetVariable("amp_10_delta")->setVal(0.0);
        signal_pdf->GetVariable("amp_1perp")->setVal(0.0);
        signal_pdf->GetVariable("amp_1perp_delta")->setVal(0.0);
        signal_pdf->GetVariable("amp_1para")->setVal(0.0);
        signal_pdf->GetVariable("amp_1para_delta")->setVal(0.0);
        signal_pdf->GetVariable("amp_20")->setVal((std::abs(A_20) + std::abs(A_20_bar))/2.0);
        signal_pdf->GetVariable("amp_20_delta")->setVal((std::abs(A_20) - std::abs(A_20_bar))/(std::abs(A_20) + std::abs(A_20_bar)));
        signal_pdf->GetVariable("amp_2perp")->setVal((std::abs(A_2perp) + std::abs(A_2perp_bar))/2.0);
        if((std::abs(A_2perp) + std::abs(A_2perp_bar)) > 0)
            signal_pdf->GetVariable("amp_2perp_delta")->setVal((std::abs(A_2perp) - std::abs(A_2perp_bar))/(std::abs(A_2perp) + std::abs(A_2perp_bar)));
        else
            signal_pdf->GetVariable("amp_2perp_delta")->setVal(0.0);
        signal_pdf->GetVariable("amp_2para")->setVal((std::abs(A_2para) + std::abs(A_2para_bar))/2.0);
        if((std::abs(A_2para) + std::abs(A_2para_bar)) > 0)
            signal_pdf->GetVariable("amp_2para_delta")->setVal((std::abs(A_2para) - std::abs(A_2para_bar))/(std::abs(A_2para) + std::abs(A_2para_bar)));
        else
            signal_pdf->GetVariable("amp_2para_delta")->setVal(0.0);
        double N_K21430 = I_tot->getVal();
        results["frac_K21430"] = N_K21430/N_tot;
        results["frac_K21430_asym"] = (double)signal_ref->getAsymmetry();;
        results["yield_K21430"] = results["frac_K21430"]*results["signal_yield"];
        results["BR_K21430"] = results["yield_K21430"]/(NBB*frac_K21430_to_KPI*efficiency_K21430);
        
        // sum of fit fractions
        results["frac_sum"] = results["frac_K892"] + results["frac_LASS"] + results["frac_K21430"];
        
        // reset all variables
        signal_pdf->GetVariable("amp_00")->setVal((std::abs(A_00) + std::abs(A_00_bar))/2.0);
        signal_pdf->GetVariable("amp_00_delta")->setVal((std::abs(A_00) - std::abs(A_00_bar))/(std::abs(A_00) + std::abs(A_00_bar)));
        signal_pdf->GetVariable("amp_10")->setVal((std::abs(A_10) + std::abs(A_10_bar))/2.0);
        signal_pdf->GetVariable("amp_10_delta")->setVal((std::abs(A_10) - std::abs(A_10_bar))/(std::abs(A_10) + std::abs(A_10_bar)));
        signal_pdf->GetVariable("amp_1perp")->setVal((std::abs(A_1perp) + std::abs(A_1perp_bar))/2.0);
        signal_pdf->GetVariable("amp_1perp_delta")->setVal((std::abs(A_1perp) - std::abs(A_1perp_bar))/(std::abs(A_1perp) + std::abs(A_1perp_bar)));
        signal_pdf->GetVariable("amp_1para")->setVal((std::abs(A_1para) + std::abs(A_1para_bar))/2.0);
        signal_pdf->GetVariable("amp_1para_delta")->setVal((std::abs(A_1para) - std::abs(A_1para_bar))/(std::abs(A_1para) + std::abs(A_1para_bar)));
        signal_pdf->GetVariable("amp_20")->setVal((std::abs(A_20) + std::abs(A_20_bar))/2.0);
        signal_pdf->GetVariable("amp_20_delta")->setVal((std::abs(A_20) - std::abs(A_20_bar))/(std::abs(A_20) + std::abs(A_20_bar)));
        signal_pdf->GetVariable("amp_2perp")->setVal((std::abs(A_2perp) + std::abs(A_2perp_bar))/2.0);
        signal_pdf->GetVariable("amp_2perp_delta")->setVal((std::abs(A_2perp) - std::abs(A_2perp_bar))/(std::abs(A_2perp) + std::abs(A_2perp_bar)));
        signal_pdf->GetVariable("amp_2para")->setVal((std::abs(A_2para) + std::abs(A_2para_bar))/2.0);
        signal_pdf->GetVariable("amp_2para_delta")->setVal((std::abs(A_2para) - std::abs(A_2para_bar))/(std::abs(A_2para) + std::abs(A_2para_bar)));
        
        // TODO: Fit fractions for K_0(1430)* only
        //double N_K01430_CA = I_tot_CA->getVal();
        //double N_K01430_CP = I_tot_CP->getVal();
        
        // computing A_T^0 and A_T^\parallel for B and Bbar
        // to correct for the sign flip of perpendicular component we multiply A_1perp with -1.0
        results["A_T1_0"] = std::imag(-1.0*A_1perp*std::conj(A_10))/sum_A1;
        results["A_T1_||"] = std::imag(-1.0*A_1perp*std::conj(A_1para))/sum_A1;
        
        results["Abar_T1_0"] = std::imag(A_1perp_bar*std::conj(A_10_bar))/sum_A1_bar;
        results["Abar_T1_||"] = std::imag(A_1perp_bar*std::conj(A_1para_bar))/sum_A1_bar;
        
        results["A_T1_0_asym"] = (results["A_T1_0"] - results["Abar_T1_0"])/(results["A_T1_0"] + results["Abar_T1_0"]);
        results["A_T1_||_asym"] = (results["A_T1_||"] - results["Abar_T1_||"])/(results["A_T1_||"] + results["Abar_T1_||"]);
        
        // computing Lambda and Sigma quantities
        // to correct for the sign flip of perpendicular component we multiply A_1perp with -1.0
        results["Lambda_0-0"] = results["fL1"];
        results["Lambda_para-para"] = 1 - (results["fL1"] + results["fP1"]);
        results["Lambda_perp-perp"] = results["fP1"];
        results["Lambda_para-0"] = std::real(A_1para*std::conj(A_10) + A_1para_bar*std::conj(A_10_bar));
        results["Lambda_perp-0"] = -std::imag(-1.0*A_1perp*std::conj(A_10) - A_1perp_bar*std::conj(A_10_bar));
        results["Lambda_perp-para"] = -std::imag(-1.0*A_1perp*std::conj(A_1para) - A_1perp_bar*std::conj(A_1para_bar));
        results["Sigma_0-0"] = (std::norm(A_10_bar)/sum_A1_bar - std::norm(A_10)/sum_A1);
        results["Sigma_para-para"] = (std::norm(A_1para_bar)/sum_A1_bar - std::norm(A_1para)/sum_A1);
        results["Sigma_perp-perp"] = (std::norm(A_1perp_bar)/sum_A1_bar - std::norm(-1.0*A_1perp)/sum_A1);
        results["Sigma_para-0"] = std::real(A_1para*std::conj(A_10) - A_1para_bar*std::conj(A_10_bar));
        results["Sigma_perp-0"] = -std::imag(-1.0*A_1perp*std::conj(A_10) + A_1perp_bar*std::conj(A_10_bar));
        results["Sigma_perp-para"] = -std::imag(-1.0*A_1perp*std::conj(A_1para) + A_1perp_bar*std::conj(A_1para_bar));
        
        delete I_tot;
        return results;
    }
    
    std::string GetErrorString(const std::pair<double,double>& errors, unsigned int digits = 3) {
        std::stringstream ss;
        ss << std::fixed;
        if(std::fabs(errors.first - errors.second) < 0.001)
            ss << " \\pm " << std::setprecision(digits) << errors.first;
        else
            ss << " ^{+" << std::setprecision(digits) << errors.second << "}_{-" << std::setprecision(digits) << errors.first << "}";
        return ss.str();
    }
    
    std::string GetValueString(double value, unsigned int digits = 3) {
        std::stringstream ss;
        ss << std::fixed;
        if(value < 0)
            ss << std::setprecision(digits) << value;
        else
            ss << "\\phantom{-}" << std::setprecision(digits) << value;
        return ss.str();
    }
    
    void PrintPhysicsParametersLatex(std::map<std::string, std::pair<double,std::pair<double,double> > > physics_params) {
        using namespace std;
        
        PrintFitStartBlock("LATEX TABLE - START");
        cout << "\\begin{table}[!ht]" << "\n";
        cout << "\\centering" << "\n";
        cout << "\\begin{tabular}{lccc} \\toprule" << "\n";
        cout << " & $\\HepProcess{\\Pphi (\\PK\\Ppi)^{*0}_{0}}$ & $\\HepProcess{\\Pphi\\PKsti}$ & $\\HepProcess{\\Pphi\\PKstii}$ \\\\" << "\n";
        cout << "Parameter & $J=0$ & $J=1$ & $J=2$ \\\\ \\midrule" << "\n";
        // signal yields
        cout << "$N_{sig}^{J}$";
        cout << " & $" << GetValueString(physics_params["yield_LASS"].first) << GetErrorString(physics_params["yield_LASS"].second) << "$";
        cout << " & $" << GetValueString(physics_params["yield_K892"].first) << GetErrorString(physics_params["yield_K892"].second) << "$";
        cout << " & $" << GetValueString(physics_params["yield_K21430"].first) << GetErrorString(physics_params["yield_K21430"].second) << "$";
        cout << "\\\\" << "\n";
        // branching fractions
        cout << "$\\mathcal{B}_J (10^{-6})$";
        std::pair<double,double> error_buffer = std::make_pair(1e6*physics_params["BR_LASS"].second.first,1e6*physics_params["BR_LASS"].second.second);
        cout << " & $" << GetValueString(1e6*physics_params["BR_LASS"].first) << GetErrorString(error_buffer) << "$";
        error_buffer = std::make_pair(1e6*physics_params["BR_K892"].second.first,1e6*physics_params["BR_K892"].second.second);
        cout << " & $" << GetValueString(1e6*physics_params["BR_K892"].first) << GetErrorString(error_buffer) << "$";
        error_buffer = std::make_pair(1e6*physics_params["BR_K21430"].second.first,1e6*physics_params["BR_K21430"].second.second);
        cout << " & $" << GetValueString(1e6*physics_params["BR_K21430"].first) << GetErrorString(error_buffer) << "$";
        cout << "\\\\" << "\n";
        cout << "\\bottomrule \n \\end{tabular}" << "\n";
        cout << "\\caption{Branching fractions}" << "\n";
        cout << "\\label{tab:branching_fractions}" << "\n";
        cout << "\\end{table}" << "\n";
        
        cout << "\n";
        cout << "\n";
        
        cout << "\\begin{table}[!ht]" << "\n";
        cout << "\\centering" << "\n";
        cout << "\\begin{tabular}{lccc} \\toprule" << "\n";
        cout << " & $\\HepProcess{\\Pphi (\\PK\\Ppi)^{*0}_{0}}$ & $\\HepProcess{\\Pphi\\PKsti}$ & $\\HepProcess{\\Pphi\\PKstii}$ \\\\" << "\n";
        cout << "Parameter & $J=0$ & $J=1$ & $J=2$ \\\\ \\midrule" << "\n";
        // fit fractions
        cout << "$FF_J$";
        cout << " & $" << GetValueString(physics_params["frac_LASS"].first) << GetErrorString(physics_params["frac_LASS"].second) << "$";
        cout << " & $" << GetValueString(physics_params["frac_K892"].first) << GetErrorString(physics_params["frac_K892"].second) << "$";
        cout << " & $" << GetValueString(physics_params["frac_K21430"].first) << GetErrorString(physics_params["frac_K21430"].second) << "$";
        cout << "\\\\" << "\n";
        // long polarization fraction
        cout << "$f_{LJ}$";
        cout << " & $1$";
        cout << " & $" << GetValueString(physics_params["fL1"].first) << GetErrorString(physics_params["fL1"].second) << "$";
        cout << " & $" << GetValueString(physics_params["fL2"].first) << GetErrorString(physics_params["fL2"].second) << "$";
        cout << "\\\\" << "\n";
        // perpendicular polarization fraction
        cout << "$f_{\\perp J}$";
        cout << " & ";
        cout << " & $" << GetValueString(physics_params["fP1"].first) << GetErrorString(physics_params["fP1"].second) << "$";
        cout << " & $" << GetValueString(physics_params["fP2"].first) << GetErrorString(physics_params["fP2"].second) << "$";
        cout << "\\\\" << "\n";
        // phi parallel
        cout << "$\\phi_{\\parallel J}$ (rad)";
        cout << " & ";
        cout << " & $" << GetValueString(physics_params["phi_||1"].first) << GetErrorString(physics_params["phi_||1"].second) << "$";
        cout << " & $" << GetValueString(physics_params["phi_||2"].first) << GetErrorString(physics_params["phi_||2"].second) << "$";
        cout << "\\\\" << "\n";
        // phi perpendicular
        cout << "$\\phi_{\\perp J}$ (rad)";
        cout << " & ";
        cout << " & $" << GetValueString(physics_params["phi_P1"].first) << GetErrorString(physics_params["phi_P1"].second) << "$";
        cout << " & $" << GetValueString(physics_params["phi_P2"].first) << GetErrorString(physics_params["phi_P2"].second) << "$";
        cout << "\\\\" << "\n";
        // delta
        cout << "$\\delta_{0J}$ (rad)";
        cout << " & $0$";
        cout << " & $" << GetValueString(physics_params["delta_01"].first) << GetErrorString(physics_params["delta_01"].second) << "$";
        cout << " & $" << GetValueString(physics_params["delta_02"].first) << GetErrorString(physics_params["delta_02"].second) << "$";
        cout << "\\\\" << "\n";
        // ACP
        cout << "$\\mathcal{A}_{CPJ}$";
        cout << " & $" << GetValueString(physics_params["frac_LASS_asym"].first) << GetErrorString(physics_params["frac_LASS_asym"].second) << "$";
        cout << " & $" << GetValueString(physics_params["frac_K892_asym"].first) << GetErrorString(physics_params["frac_K892_asym"].second) << "$";
        cout << " & $" << GetValueString(physics_params["frac_K21430_asym"].first) << GetErrorString(physics_params["frac_K21430_asym"].second) << "$";
        cout << "\\\\" << "\n";
        // ACP_0
        cout << "$\\mathcal{A}_{CPJ}^0$";
        cout << " & $0$";
        cout << " & $" << GetValueString(physics_params["A_CP1_0"].first) << GetErrorString(physics_params["A_CP1_0"].second) << "$";
        cout << " & $" << GetValueString(physics_params["A_CP2_0"].first) << GetErrorString(physics_params["A_CP2_0"].second) << "$";
        cout << "\\\\" << "\n";
        // ACP_P
        cout << "$\\mathcal{A}_{CPJ}^\\perp$";
        cout << " & ";
        cout << " & $" << GetValueString(physics_params["A_CP1_P"].first) << GetErrorString(physics_params["A_CP1_P"].second) << "$";
        cout << " & $" << GetValueString(physics_params["A_CP2_P"].first) << GetErrorString(physics_params["A_CP2_P"].second) << "$";
        cout << "\\\\" << "\n";
        // Delta phi parallel
        cout << "$\\Delta\\phi_{\\parallel J}$ (rad)";
        cout << " &";
        cout << " & $" << GetValueString(physics_params["Delta_phi_||1"].first) << GetErrorString(physics_params["Delta_phi_||1"].second) << "$";
        cout << " & $" << GetValueString(physics_params["Delta_phi_||2"].first) << GetErrorString(physics_params["Delta_phi_||2"].second) << "$";
        cout << "\\\\" << "\n";
        // Delta phi perpendicular
        cout << "$\\Delta\\phi_{\\perp J}$ (rad)";
        cout << " &";
        cout << " & $" << GetValueString(physics_params["Delta_phi_P1"].first) << GetErrorString(physics_params["Delta_phi_P1"].second) << "$";
        cout << " & $" << GetValueString(physics_params["Delta_phi_P2"].first) << GetErrorString(physics_params["Delta_phi_P2"].second) << "$";
        cout << "\\\\" << "\n";
        // Delta delta
        cout << "$\\Delta\\delta_{0J}$ (rad)";
        cout << " & $0$";
        cout << " & $" << GetValueString(physics_params["Delta_delta_01"].first) << GetErrorString(physics_params["Delta_delta_01"].second) << "$";
        cout << " & $" << GetValueString(physics_params["Delta_delta_02"].first) << GetErrorString(physics_params["Delta_delta_02"].second) << "$";
        cout << "\\\\" << "\n";
        // Delta phi 00
        cout << "$\\Delta\\phi_{00}$ (rad)";
        cout << " & $0$";
        cout << " & ";
        cout << " & ";
        cout << "\\\\" << "\n";
        cout << "\\bottomrule \n \\end{tabular}" << "\n";
        cout << "\\caption{Physics parameters}" << "\n";
        cout << "\\label{tab:fit_results_physics_parameters}" << "\n";
        cout << "\\end{table}" << "\n";
        
        cout << "\n";
        cout << "\n";
        
        cout << "\\begin{table}[!ht]" << "\n";
        cout << "\\centering" << "\n";
        cout << "\\begin{tabular}{lccc} \\toprule" << "\n";
        cout << " & $\\PBzero$ & $\\APBzero$ & $\\frac{\\PBzero - \\APBzero}{\\PBzero + \\APBzero}$ \\\\ \\midrule" << "\n";
        // triple product values 0
        cout << "$A_{T}^{0}$";
        cout << " & $" << GetValueString(physics_params["A_T1_0"].first) << GetErrorString(physics_params["A_T1_0"].second) << "$";
        cout << " & $" << GetValueString(physics_params["Abar_T1_0"].first) << GetErrorString(physics_params["Abar_T1_0"].second) << "$";
        cout << " & $" << GetValueString(physics_params["A_T1_0_asym"].first) << GetErrorString(physics_params["A_T1_0_asym"].second) << "$";
        cout << "\\\\" << "\n";
        // triple product values parallel
        cout << "$A_{T}^{\\parallel}$";
        cout << " & $" << GetValueString(physics_params["A_T1_||"].first) << GetErrorString(physics_params["A_T1_||"].second) << "$";
        cout << " & $" << GetValueString(physics_params["Abar_T1_||"].first) << GetErrorString(physics_params["Abar_T1_||"].second) << "$";
        cout << " & $" << GetValueString(physics_params["A_T1_||_asym"].first) << GetErrorString(physics_params["A_T1_||_asym"].second) << "$";
        cout << "\\\\" << "\n";
        cout << "\\bottomrule \n \\end{tabular}" << "\n";
        cout << "\\caption{Triple product correlations}" << "\n";
        cout << "\\label{tab:triple_product_correlations}" << "\n";
        cout << "\\end{table}" << "\n";
        
        cout << "\n";
        cout << "\n";
        
        cout << "\\begin{table}[!ht]" << "\n";
        cout << "\\centering" << "\n";
        cout << "\\begin{tabular}{ll} \\toprule" << "\n";
        // lambda and sigma quantities
        cout << "$\\Lambda_{00} = " << GetValueString(physics_params["Lambda_0-0"].first) << GetErrorString(physics_params["Lambda_0-0"].second) << "$";
        cout << " & $\\Sigma_{00} = " << GetValueString(physics_params["Sigma_0-0"].first) << GetErrorString(physics_params["Sigma_0-0"].second) << "$";
        cout << "\\\\" << "\n";
        cout << "$\\Lambda_{\\parallel\\parallel} = " << GetValueString(physics_params["Lambda_para-para"].first) << GetErrorString(physics_params["Lambda_para-para"].second) << "$";
        cout << " & $\\Sigma_{\\parallel\\parallel} = " << GetValueString(physics_params["Sigma_para-para"].first) << GetErrorString(physics_params["Sigma_para-para"].second) << "$";
        cout << "\\\\" << "\n";
        cout << "$\\Lambda_{\\perp\\perp} = " << GetValueString(physics_params["Lambda_perp-perp"].first) << GetErrorString(physics_params["Lambda_perp-perp"].second) << "$";
        cout << " & $\\Sigma_{\\perp\\perp} = " << GetValueString(physics_params["Sigma_perp-perp"].first) << GetErrorString(physics_params["Sigma_perp-perp"].second) << "$";
        cout << "\\\\" << "\n";
        cout << "$\\Lambda_{\\perp 0} = " << GetValueString(physics_params["Lambda_perp-0"].first) << GetErrorString(physics_params["Lambda_perp-0"].second) << "$";
        cout << " & $\\Sigma_{\\perp 0} = " << GetValueString(physics_params["Sigma_perp-0"].first) << GetErrorString(physics_params["Sigma_perp-0"].second) << "$";
        cout << "\\\\" << "\n";
        cout << "$\\Lambda_{\\perp\\parallel} = " << GetValueString(physics_params["Lambda_perp-para"].first) << GetErrorString(physics_params["Lambda_perp-para"].second) << "$";
        cout << " & $\\Sigma_{\\perp\\parallel} = " << GetValueString(physics_params["Sigma_perp-para"].first) << GetErrorString(physics_params["Sigma_perp-para"].second) << "$";
        cout << "\\\\" << "\n";
        cout << "$\\Lambda_{\\parallel 0} = " << GetValueString(physics_params["Lambda_para-0"].first) << GetErrorString(physics_params["Lambda_para-0"].second) << "$";
        cout << " & $\\Sigma_{\\parallel 0} = " << GetValueString(physics_params["Sigma_para-0"].first) << GetErrorString(physics_params["Sigma_para-0"].second) << "$";
        cout << "\\\\" << "\n";
        cout << "\\bottomrule \n \\end{tabular}" << "\n";
        cout << "\\caption{$\\Lambda$ and $\\Sigma$ values obtained from decay amplitudes}" << "\n";
        cout << "\\label{tab:lambda_and_sigma_values}" << "\n";
        cout << "\\end{table}" << "\n";
        cout << "\n";
        PrintFitStartBlock("LATEX TABLE - END");
    }
    
    // computes propagated errors using toys and cholesky transformation
    std::map<std::string, std::pair<double,std::pair<double,double> > > PropagateErrors(shared_ptr<FF_PDF> pdf, shared_ptr<FF_PDF> signal_pdf,
                                                                                        RooFitResult* result, shared_ptr<FFOpt> opt, const RooArgSet& varset, bool verbose) {
        // store nominal fit result
        std::map<std::string, double> nominal_toy_result = ComputePhysicsParameters(pdf,signal_pdf,varset, true);
        // decompose the correlation matrix (correlation matrix would be fine if we rotate normal Gaussian
        // distributioned variables with mean 0 and width 1, in that case correlation and covariance matrix
        // are the same)
        TDecompChol chol(result->correlationMatrix());
        // decompose the covariance matrix
        //TDecompChol chol(result->covarianceMatrix());
        std::cout << "INFO: Decomposing Matrix..." << std::endl;
        if(!(chol.Decompose())) {
            std::cerr << "ERROR: Matrix decomposition failed!" << std::endl;
            std::exit(1);
        }
        const TMatrixD& U = chol.GetU();
        TMatrixD L(U);
        L.Transpose(U);
        std::cout << "...done" << std::endl;
        
        if(verbose) {
            std::cout << "DEBUG: Original matrix:" << std::endl;
            result->correlationMatrix().Print();
            //result->covarianceMatrix().Print();
            std::cout << "DEBUG: Decomposed matrix:" << std::endl;
            U.Print();
            L.Print();
        }
        
        // prepare the fit params and errors
        std::vector<std::string> param_names;
        param_names.reserve(result->floatParsFinal().getSize());
        for(int i = 0; i < result->floatParsFinal().getSize(); ++i) {
            param_names.push_back(result->floatParsFinal()[i].GetName());
        }
        if(verbose) {
            std::cout << "DEBUG: Parameter names: " << std::endl;
            std::cout << mp::vector_to_string(param_names,true);
        }
        std::map<std::string, double> fit_params;
        std::map<std::string, double> fit_errors;
        std::map<std::string, double> fit_errors_lo;
        std::map<std::string, double> fit_errors_hi;
        for(const std::string& par : param_names) {
            RooRealVar* variable = pdf->GetVariable(par);
            fit_params[par] = variable->getVal();
            double error_lo = -1*variable->getErrorLo();
            double error_hi = variable->getErrorHi();
            // make sure none of the errors is 0
            if(error_lo == 0.0) {
                error_lo = variable->getError();
                std::cerr << "ERROR: Zero size lower error for parameter " << par << ". Using symmetric errors instead. "<< std::endl;
            }
            if(error_hi == 0.0) {
                error_hi = variable->getError();
                std::cerr << "ERROR: Zero size higher error for parameter " << par << ". Using symmetric errors instead. "<< std::endl;
            }
            fit_errors[par] = (error_lo + error_hi) / 2.0;
            fit_errors_lo[par] = error_lo;
            fit_errors_hi[par] = error_hi;
            double asym = (error_lo - error_hi) / (error_lo + error_hi);
            if(std::fabs(asym) > 0.1)
                std::cout << "ERROR: Large asymmetry between errors for parameter " << par << std::endl;
        }
        
        // now do 10000 toys and compute the physics parameters for each toy
        std::map<std::string, std::vector<double> > toy_results;
        std::map<std::string, std::vector<double> > cor_random_numbers;
        bool skip_toy = false;
        unsigned int skipped_toys = 0;
        const unsigned int n_toys = 25000;
        for(unsigned int i_toy = 1; i_toy <= n_toys; ++i_toy) {
            skip_toy = false;
            if(i_toy%5000 == 0) std::cout << "INFO: Status: " << std::setw(5) << i_toy << "/" << n_toys << std::endl;
            // generate uncorrelated random numbers
            // order of the vector needs to correspond to the matrix order
            TVectorD uncorrelated_lo(param_names.size());
            for(unsigned int i = 0; i < param_names.size(); ++i) {
                uncorrelated_lo[i] = RooRandom::randomGenerator()->Gaus(0,1);
            }
            
            // correlate random numbers
            TVectorD correlated_lo = uncorrelated_lo;
            correlated_lo *= L;
            
            if(verbose && i_toy%1000 == 0) {
                std::cout << "DEBUG: Uncorrelated and correlated random numbers: " << std::endl;
                uncorrelated_lo.Print();
                correlated_lo.Print();
            }
            
            // set the pdf values with modified parameter set
            for(unsigned int i = 0; i < param_names.size(); ++i) {
                // scale correlated random number to error (needed if correlation matix is used)
                if(opt->GetMinos())
                    correlated_lo[i] = correlated_lo[i] * (correlated_lo[i] > 0 ? fit_errors_hi[param_names[i]] : fit_errors_lo[param_names[i]]);
                else
                    correlated_lo[i] = correlated_lo[i] * fit_errors[param_names[i]];
                // store correlated random numbers for later check
                cor_random_numbers[param_names[i]].push_back(correlated_lo[i]);
                // compute new value for variable
                RooRealVar* variable = pdf->GetVariable(param_names[i]);
                double new_value = fit_params[param_names[i]]+correlated_lo[i];
                
                // set value for this toy
                if(variable->getMin() > new_value) {
                    if(verbose) std::cout << "INFO: Value " << new_value << " of parameter " << param_names[i] << " is below minimum --> skip toy" << std::endl;
                    skip_toy = true;
                    break;
                } else if(variable->getMax() < new_value) {
                    if(verbose) std::cout << "INFO: Value " << new_value << " of parameter " << param_names[i] << " is above maximum --> skip toy" << std::endl;
                    skip_toy = true;
                    break;
                } else {
                    variable->setVal(new_value);
                }
            }
            
            // if one of the fit parameters exceeds its range, we don't compute physics parameters as this will lead to many problems
            if(skip_toy) {
                ++skipped_toys;
                continue;
            }
            
            // compute the physics parameters with the modified set
            std::map<std::string, double> toy_result = ComputePhysicsParameters(pdf,signal_pdf,varset,false);
            for(auto& p : toy_result) {
                toy_results[p.first].push_back(p.second);
            }
        }
        std::cout << "INFO: " << skipped_toys << " toys have been skipped due to values that reach unphysical regions (beyond boundaries)" << std::endl;
        
        // reset the pdf values to fit parameter set
        for(unsigned int i = 0; i < param_names.size(); ++i) {
            pdf->GetVariable(param_names[i])->setVal(fit_params[param_names[i]]);
        }
        
        // make control plots of all random numbers we created
        std::cout << "INFO: Now doing control plots for correlated random numbers..." << std::endl;
        for(unsigned int i = 0; i < param_names.size(); ++i) {
            double min = -fit_errors[param_names[i]]*6;
            double max = fit_errors[param_names[i]]*6;
            
            TCanvas* canvas = new TCanvas("canvas","canvas",1280,1024);
            canvas->cd();
            
            std::cout << "INFO: Creating hist for " << param_names[i] << " in [" << min << "," << max << "]\n";
            TH1F* hist = new TH1F("hist","hist",100,min,max);
            for(double d : cor_random_numbers[param_names[i]]) {
                hist->Fill(d);
            }
            
            RooRealVar* var;
            if(param_names[i].find("phase") < param_names[i].npos) {
                var = new RooRealVar("variable",GetLabelToTLatex(param_names[i]).c_str(),min,max,"rad");
            } else {
                var = new RooRealVar("variable",GetLabelToTLatex(param_names[i]).c_str(),min,max);
            }
            RooDataHist data("data","data",*var,Import(*hist));
            
            RooPlot* plot = var->frame();
            data.plotOn(plot);
            
            RooRealVar mean("mean","mean",0.0) ;
            RooRealVar sigma_l("sigma_l","sigma_l",hist->GetRMS(),0.0,hist->GetRMS()*5);
            RooRealVar sigma_r("sigma_r","sigma_r",hist->GetRMS(),0.0,hist->GetRMS()*5);
            
            RooAbsPdf* pdf_l = new RooGaussian("gauss_l","gauss_l",*var,mean,sigma_l);
            RooAbsPdf* pdf_r = new RooGaussian("gauss_r","gauss_r",*var,mean,sigma_r);
            
            std::cout << "INFO: Fitting hist of " << param_names[i] << " in [" << min << "," << max << "]\n";
            if(opt->GetMinos()) {
                pdf_l->fitTo(data,Range(min,0.0));
                pdf_l->plotOn(plot, LineStyle(kDashed), LineColor(kBlue));
                pdf_r->fitTo(data,Range(0.0,max));
                pdf_r->plotOn(plot, LineStyle(kDashed), LineColor(kBlue));
            } else {
                pdf_l->fitTo(data,Range(min,max));
                pdf_l->plotOn(plot, LineStyle(kDashed), LineColor(kBlue));
            }
            plot->Draw("");
            plot->SetMinimum(0.0);
            plot->SetMaximum(plot->GetMaximum()*1.3);
            
            TLatex text;
            text.SetTextColor(kBlack);
            text.SetTextFont(42);
            text.SetTextSize(0.045);
            text.SetNDC(1);
            std::stringstream ss;
            ss.precision(3);
            ss << "m = 0.0";
            text.DrawLatex(0.18, 0.9, ss.str().c_str());
            if(opt->GetMinos()) {
                ss.str(std::string());
                ss << "#sigma_{lo} = " << sigma_l.getVal() << " #pm " << sigma_l.getError();
                text.DrawLatex(0.18, 0.85, ss.str().c_str());
                ss.str(std::string());
                ss << "Error_{lo} of " << GetLabelToTLatex(param_names[i]) << " is " << fit_errors_lo[param_names[i]];
                text.DrawLatex(0.18, 0.8, ss.str().c_str());
                ss.str(std::string());
                ss << "#sigma_{hi} = " << sigma_r.getVal() << " #pm " << sigma_r.getError();
                text.DrawLatex(0.18, 0.75, ss.str().c_str());
                ss.str(std::string());
                ss << "Error_{hi} of " << GetLabelToTLatex(param_names[i]) << " is " << fit_errors_hi[param_names[i]];
                text.DrawLatex(0.18, 0.7, ss.str().c_str());
            } else {
                ss.str(std::string());
                ss << "#sigma = " << sigma_l.getVal() << " #pm " << sigma_l.getError();
                text.DrawLatex(0.18, 0.85, ss.str().c_str());
                ss.str(std::string());
                ss << "Error of " << GetLabelToTLatex(param_names[i]) << " is " << fit_errors[param_names[i]];
                text.DrawLatex(0.18, 0.8, ss.str().c_str());
            }
            
            if(opt->GetCreatePlots()) {
                canvas->cd();
                canvas->SaveAs((opt->GetOutdir()+"correlated_random_numbers_for_"+param_names[i]+".eps").c_str());
                canvas->SaveAs((opt->GetOutdir()+"correlated_random_numbers_for_"+param_names[i]+".root").c_str());
            }
            
            delete var;
            delete pdf_l;
            delete pdf_r;
            delete plot;
            delete hist;
            delete canvas;
        }
        
        const double pi_half = TMath::Pi()/2.0;
        const double two_pi = 2*TMath::Pi();
        
        std::map<std::string,std::pair<double,double> > physic_param_boundary;
        physic_param_boundary["fL0"] = std::make_pair(0.0,1.0);
        physic_param_boundary["fL1"] = std::make_pair(0.0,1.0);
        physic_param_boundary["fP1"] = std::make_pair(0.0,1.0);
        physic_param_boundary["fL2"] = std::make_pair(0.0,1.0);
        physic_param_boundary["fP2"] = std::make_pair(0.0,1.0);
        physic_param_boundary["A_CP0_0"] = std::make_pair(-1.0,1.0);
        physic_param_boundary["A_CP1_0"] = std::make_pair(-1.0,1.0);
        physic_param_boundary["A_CP1_P"] = std::make_pair(-1.0,1.0);
        physic_param_boundary["A_CP2_0"] = std::make_pair(-1.0,1.0);
        physic_param_boundary["A_CP2_P"] = std::make_pair(-1.0,1.0);
        physic_param_boundary["phi_P1"] = std::make_pair(0.0,two_pi);
        physic_param_boundary["phi_P2"] = std::make_pair(2.5,6.0);
        physic_param_boundary["phi_||1"] = std::make_pair(0.0,two_pi);
        physic_param_boundary["phi_||2"] = std::make_pair(0.0,two_pi);
        physic_param_boundary["Delta_phi_00"] = std::make_pair(-pi_half,pi_half);
        physic_param_boundary["Delta_phi_P1"] = std::make_pair(-pi_half,pi_half);
        physic_param_boundary["Delta_phi_P2"] = std::make_pair((nominal_toy_result["Delta_phi_P2"] > 0) ? nominal_toy_result["Delta_phi_P2"]-pi_half : -pi_half, (nominal_toy_result["Delta_phi_P2"] < 0) ? nominal_toy_result["Delta_phi_P2"]+pi_half : pi_half);
        physic_param_boundary["Delta_phi_||1"] = std::make_pair(-pi_half,pi_half);
        physic_param_boundary["Delta_phi_||2"] = std::make_pair((nominal_toy_result["Delta_phi_||2"] > 0) ? nominal_toy_result["Delta_phi_||2"]-pi_half : -pi_half, (nominal_toy_result["Delta_phi_||2"] < 0) ? nominal_toy_result["Delta_phi_||2"]+pi_half : pi_half);
        physic_param_boundary["delta_00"] = std::make_pair(0.0,two_pi);
        physic_param_boundary["delta_01"] = std::make_pair(2.3,3.5);
        physic_param_boundary["delta_02"] = std::make_pair(2.8,4.2);
        physic_param_boundary["Delta_delta_00"] = std::make_pair(-pi_half,pi_half);
        physic_param_boundary["Delta_delta_01"] = std::make_pair(-pi_half,pi_half);
        physic_param_boundary["Delta_delta_02"] = std::make_pair(-pi_half,pi_half);
        physic_param_boundary["frac_K21430"] = std::make_pair(0.0,1.0);
        physic_param_boundary["frac_K21430_asym"] = std::make_pair(-1.0,1.0);
        physic_param_boundary["frac_K892"] = std::make_pair(0.0,1.0);
        physic_param_boundary["frac_K892_asym"] = std::make_pair(-1.0,1.0);
        physic_param_boundary["frac_LASS"] = std::make_pair(0.0,1.0);
        physic_param_boundary["frac_LASS_asym"] = std::make_pair(-1.0,1.0);
        physic_param_boundary["frac_sum"] = std::make_pair(0.0,4.0);
        physic_param_boundary["signal_asym"] = std::make_pair(-1.0,1.0);
        physic_param_boundary["signal_yield"] = std::make_pair(-2000.0,2000.0);
        physic_param_boundary["yield_K21430"] = std::make_pair(-2000.0,2000.0);
        physic_param_boundary["yield_K892"] = std::make_pair(-2000.0,2000.0);
        physic_param_boundary["yield_LASS"] = std::make_pair(-2000.0,2000.0);
        physic_param_boundary["BR_K21430"] = std::make_pair(0.0,1.0);
        physic_param_boundary["BR_K892"] = std::make_pair(0.0,1.0);
        physic_param_boundary["BR_LASS"] = std::make_pair(0.0,1.0);
        // boundaries for tripple product and other quantities
        physic_param_boundary["A_T1_0"] = std::make_pair(-1.0,1.0);
        physic_param_boundary["A_T1_||"] = std::make_pair(-1.0,1.0);
        physic_param_boundary["Abar_T1_0"] = std::make_pair(-1.0,1.0);
        physic_param_boundary["Abar_T1_||"] = std::make_pair(-1.0,1.0);
        physic_param_boundary["A_T1_0_asym"] = std::make_pair(-1.0,1.0);
        physic_param_boundary["A_T1_||_asym"] = std::make_pair(-1.0,1.0);
        physic_param_boundary["Lambda_0-0"] = std::make_pair(-1.0,1.0);
        physic_param_boundary["Lambda_para-para"] = std::make_pair(-1.0,1.0);
        physic_param_boundary["Lambda_perp-perp"] = std::make_pair(-1.0,1.0);
        physic_param_boundary["Lambda_para-0"] = std::make_pair(-1.0,1.0);
        physic_param_boundary["Lambda_perp-0"] = std::make_pair(-1.0,1.0);
        physic_param_boundary["Lambda_perp-para"] = std::make_pair(-1.0,1.0);
        physic_param_boundary["Sigma_0-0"] = std::make_pair(-1.0,1.0);
        physic_param_boundary["Sigma_para-para"] = std::make_pair(-1.0,1.0);
        physic_param_boundary["Sigma_perp-perp"] = std::make_pair(-1.0,1.0);
        physic_param_boundary["Sigma_para-0"] = std::make_pair(-1.0,1.0);
        physic_param_boundary["Sigma_perp-0"] = std::make_pair(-1.0,1.0);
        physic_param_boundary["Sigma_perp-para"] = std::make_pair(-1.0,1.0);
        
        std::map<std::string, std::pair<double,std::pair<double, double> > > physics_params;
        
        // look at the resulting distribution for each physics parameter and
        // determine the errors by computing mean and width of the distribution
        std::cout << "INFO: Now determining propagated errors..." << std::endl;
        for(auto &p : toy_results) {
            for(auto it = p.second.begin(); it != p.second.end(); ++it) {
                if(std::isnan(*it) || std::isinf(*it)) {
                    std::cout << "ERROR: NAN or infinite in value for parameter " << p.first << " was set to 0.0" << std::endl;
                    *it = 0.0;
                }
            }
            
            double mean = std::accumulate(p.second.begin(),p.second.end(),0.0)/p.second.size();
            double sum = 0;
            for(double d : p.second) {
                sum += (d - mean)*(d - mean);
            }
            sum /= p.second.size() - 1;
            double width = std::sqrt(sum);
            
            double min = mean - width*6;
            double max = mean + width*6;
            
            double hist_min = (physic_param_boundary[p.first].first > min ? physic_param_boundary[p.first].first : min);
            double hist_max = (physic_param_boundary[p.first].second < max ? physic_param_boundary[p.first].second : max);
            
            if(hist_min == hist_max) {
                std::cout << "INFO: Skipping parameter " << p.first << "\n";
                continue;
            }
            
            TCanvas* canvas = new TCanvas("canvas","canvas",1280,1024);
            canvas->cd();
            
            std::cout << "INFO: Creating hist for " << p.first << " in [" << hist_min << "," << hist_max << "]\n";
            TH1F* hist = new TH1F(("hist_"+p.first).c_str(),("hist_"+p.first).c_str(),100,hist_min,hist_max);
            for(double d: p.second) {
                hist->Fill(d);
            }
            
            RooRealVar* var;
            if(p.first.find("phi") < p.first.npos || p.first.find("delta") < p.first.npos) {
                var = new RooRealVar("variable",GetLabelToTLatex(p.first).c_str(),hist_min,hist_max,"rad");
            } else {
                var = new RooRealVar("variable",GetLabelToTLatex(p.first).c_str(),hist_min,hist_max);
            }
            RooDataHist data("data","data",*var,Import(*hist));
            
            RooPlot* plot = var->frame();
            data.plotOn(plot);
            
            RooRealVar central("central","central",nominal_toy_result[p.first]) ;
            RooRealVar sigma_l("sigma_l","sigma_l",hist->GetRMS(),0.0,hist->GetRMS()*5);
            RooRealVar sigma_r("sigma_r","sigma_r",hist->GetRMS(),0.0,hist->GetRMS()*5);
            
            RooAbsPdf* pdf_l = new RooGaussian("gauss_l","gauss_l",*var,central,sigma_l);
            RooAbsPdf* pdf_r = new RooGaussian("gauss_r","gauss_r",*var,central,sigma_r);
            
            double fit_min = (physic_param_boundary[p.first].first > min ? physic_param_boundary[p.first].first : min);
            double fit_max = (physic_param_boundary[p.first].second < max ? physic_param_boundary[p.first].second : max);
            std::cout << "INFO: Fitting hist of " << p.first << " in [" << fit_min << "," << fit_max << "]\n";
            pdf_l->fitTo(data,Range(fit_min,nominal_toy_result[p.first]));
            pdf_l->plotOn(plot, LineStyle(kDashed), LineColor(kBlue));
            pdf_r->fitTo(data,Range(nominal_toy_result[p.first],fit_max));
            pdf_r->plotOn(plot, LineStyle(kDashed), LineColor(kBlue));
            plot->Draw("");
            plot->SetMinimum(0.0);
            plot->SetMaximum(plot->GetMaximum()*1.3);
            
            TLatex text;
            text.SetTextColor(kBlack);
            text.SetTextFont(42);
            text.SetTextSize(0.045);
            text.SetNDC(1);
            std::stringstream ss;
            ss.precision(3);
            ss << "m = " << central.getVal();
            text.DrawLatex(0.18, 0.9, ss.str().c_str());
            ss.str(std::string());
            ss << "#sigma_{lo} = " << sigma_l.getVal() << " #pm " << sigma_l.getError();
            text.DrawLatex(0.18, 0.85, ss.str().c_str());
            ss.str(std::string());
            ss << "#sigma_{hi} = " << sigma_r.getVal() << " #pm " << sigma_r.getError();
            text.DrawLatex(0.18, 0.8, ss.str().c_str());
            
            physics_params[p.first] = std::make_pair(nominal_toy_result[p.first],std::make_pair(sigma_l.getVal(),sigma_r.getVal()));
            
            if(opt->GetCreatePlots()) {
                canvas->cd();
                std::string s_p = p.first;
                if(s_p.find("||") < s_p.npos)
                    s_p.replace(s_p.find("||"),s_p.find("||")+2,"PA");
                canvas->SaveAs((opt->GetOutdir()+"physics_parameter_"+s_p+".eps").c_str());
                canvas->SaveAs((opt->GetOutdir()+"physics_parameter_"+s_p+".root").c_str());
            }
            
            delete var;
            delete pdf_l;
            delete pdf_r;
            delete plot;
            delete hist;
            delete canvas;
        }
        
        return physics_params;
    }
    
    std::string GetAcceptanceString(ACCEPTANCE_FUNCTION value) {
        switch (value) {
            case UNIFORM:
                return "_uniform";
            case CUT_TH1:
                return "_cut_th1";
            case PRODUCT_4D:
                return "_4D_product";
            case HIST_4D:
                return "_4D_hist";
            default:
                return "_undefined";
        }
    }
    
    std::string GetAcceptanceString(EFFICIENCY_FUNCTION value) {
        switch (value) {
            case FLAT:
                return "_flat";
            case BIN_2D:
                return "_bin_2D";
            case FIT_2D:
                return "_fit_2D";
            default:
                return "_undefined";
        }
    }
    
    
    std::string FilenameHash(const std::vector<std::string> &filenames) {
        std::stringstream ss;
        for(const std::string& s : filenames) {
            ss << s;
        }
        std::hash<std::string> string_hash;
        std::size_t h = string_hash(ss.str());
        ss.str("");
        ss << h;
        return ss.str();
    }
    
    void CreateFitResultsComparisonPlot(shared_ptr<FFOpt> opt) {
        
        // struct to store one fit result from file
        struct s_fit_result {
            std::vector<std::string> i_varname;
            std::vector<double> i_truth;
            std::vector<double> i_value;
            std::vector<double> i_error;
            std::vector<double> i_error_lo;
            std::vector<double> i_error_hi;
            
            double i_NLL;
            double i_EDM;
        };
        
        std::vector<s_fit_result> fit_results_to_plot;
        
        // read in all fit result files
        for(const std::string& s : opt->GetFilenamesFitResultsToPlot() ) {
            s_fit_result f;
            
            // change from default splitting els with ','
            boost::escaped_list_separator<char> els('\\',';','\"');
            std::vector<std::string> tokens;
            std::string line;
            
            std::ifstream fit_result(s.c_str());
            if (!fit_result.is_open()) {
                std::cout << "ERROR: Could not open file " << s << std::endl;
            }
            while (std::getline(fit_result,line)) {
                // boost tokenizer to easily read in a CSV file (it splits the line at , and we can convert the strings to floats)
                boost::tokenizer< boost::escaped_list_separator<char> > tok(line, els);
                tokens.assign(tok.begin(),tok.end());
                
                if(tokens.size() == 6) {
                    f.i_varname.push_back(tokens[0]);
                    f.i_truth.push_back(std::atof(tokens[1].c_str()));
                    f.i_value.push_back(std::atof(tokens[2].c_str()));
                    f.i_error.push_back(std::atof(tokens[3].c_str()));
                    f.i_error_lo.push_back(std::atof(tokens[4].c_str()));
                    f.i_error_hi.push_back(std::atof(tokens[5].c_str()));
                } else if (tokens.size() == 2) {
                    f.i_EDM = std::atof(tokens[0].c_str());
                    f.i_NLL = std::atof(tokens[1].c_str());
                } else {
                    std::cout << "ERROR: Wrong expected token size!" << std::endl;
                }
            }
            fit_result.close();
            fit_results_to_plot.push_back(f);
        }
        
        std::vector<double> meta_pull;
        std::vector<std::vector<double> > meta_pull_per_file;
        
        // for each variable we create a graph
        std::size_t var_index = 0;
        std::size_t entries = fit_results_to_plot.size();
        meta_pull_per_file.resize(entries);
        for(const std::string& varname : fit_results_to_plot.front().i_varname) {
            std::cout << "INFO: Variable " << varname << std::endl;
            
            std::vector<double> x; x.resize(entries);
            std::vector<double> xlo; xlo.resize(entries);
            std::vector<double> xhi; xhi.resize(entries);
            std::vector<double> y; y.resize(entries);
            std::vector<double> ylo; ylo.resize(entries);
            std::vector<double> yhi; yhi.resize(entries);
            std::vector<double> truth; truth.resize(entries);
            
            std::size_t value_index = 0;
            for(const s_fit_result& fr : fit_results_to_plot) {
                if(varname == "phase_00" || varname == "phase_10" || varname == "phase_1para" || varname == "phase_1perp"
                   || varname == "phase_20" || varname == "phase_2para" || varname == "phase_2perp") {
                    x[value_index] = shift_2pi_periodic_to_truth_value(fr.i_value[var_index],fr.i_truth[var_index]);
                } else if(varname == "phase_00_delta" || varname == "phase_10_delta" || varname == "phase_1para_delta" || varname == "phase_1perp_delta"
                          || varname == "phase_20_delta" || varname == "phase_2para_delta" || varname == "phase_2perp_delta") {
                    x[value_index] = shift_pi_periodic_to_truth_value(fr.i_value[var_index],fr.i_truth[var_index]);
                } else {
                    x[value_index] = fr.i_value[var_index];
                }
                if(x[value_index] != fr.i_value[var_index]) {
                    std::cout << "DEBUG: Variable " << varname << " was shifted from " << fr.i_value[var_index] << " to " << x[value_index] << std::endl;
                }
                
                xlo[value_index] = std::fabs(fr.i_error_lo[var_index]);
                xhi[value_index] = fr.i_error_hi[var_index];
                // use symmetric errors explicitly
                //				xlo[value_index] = fr.i_error[var_index];
                //				xhi[value_index] = fr.i_error[var_index];
                y[value_index] = value_index*0.5;
                truth[value_index] = fr.i_truth[var_index];
                
                if(xlo[value_index] == 0) {
                    xlo[value_index] = fr.i_error[var_index];
                }
                if(xhi[value_index] == 0) {
                    xhi[value_index] = fr.i_error[var_index];
                }
                
                if(varname != "C_bkg_continuum_nevents" && varname != "C_bkg_bbar_wrongphikkbar_nevents" && varname != "mbc_bkg_continuum_c") {
                    double pull = x[value_index] - fr.i_truth[var_index];
                    if(pull >= 0)
                        pull /= (fr.i_error_lo[var_index] != 0 ? std::fabs(fr.i_error_lo[var_index]) : fr.i_error[var_index]);
                    else
                        pull /= (fr.i_error_hi[var_index] != 0 ? fr.i_error_hi[var_index] : fr.i_error[var_index]);
                    meta_pull.push_back(pull);
                    meta_pull_per_file[value_index].push_back(pull);
                    if(std::fabs(pull) > 5) {
                        std::cout << "DEBUG: pull: " << pull << std::endl;
                        std::cout << "DEBUG: value: " << x[value_index] <<  " - " << fr.i_error_lo[var_index] << " + " << fr.i_error_hi[var_index] << std::endl;
                        std::cout << "DEBUG: truth: " << fr.i_truth[var_index] << std::endl;
                    }
                }
                
                ++value_index;
            }
            --value_index;
            
            TCanvas* canvas = new TCanvas("canvas","canvas",1280,1024);
            canvas->cd();
            canvas->SetLeftMargin(0.05);
            canvas->SetBottomMargin(0.15);
            
            TGraphAsymmErrors* graph = new TGraphAsymmErrors(entries,&x[0],&y[0],&xlo[0],&xhi[0],&ylo[0],&yhi[0]);
            graph->SetName("graph");
            graph->SetMarkerStyle(8);
            graph->SetMarkerSize(5.0);
            graph->SetMarkerColor(kOrange);
            graph->SetLineColor(kOrange);
            graph->SetLineWidth(5);
            
            graph->Draw("APE1");
            
            graph->GetXaxis()->SetTitle(GetLabelToTLatex(varname).c_str());
            graph->GetYaxis()->SetLabelSize(0);
            graph->GetYaxis()->SetTickLength(0);
            graph->GetYaxis()->SetRangeUser(-0.2,value_index*0.5+0.2);
            
            double mean = mp::calc_mean(truth);
            
            TLine truth_line(mean,-0.18,mean,value_index*0.5+0.18);
            truth_line.SetLineWidth(5);
            truth_line.SetLineStyle(kDashed);
            truth_line.SetLineColor(kRed);
            truth_line.DrawLine(mean,-0.18,mean,value_index*0.5+0.18);
            
            gStyle->SetEndErrorSize(10);
            graph->Draw("PE1same");
            canvas->Update();
            
            canvas->cd();
            canvas->SaveAs((opt->GetOutdir()+opt->GetFitResultsPlotName()+varname+"."+opt->GetPlotFormat()).c_str());
            canvas->SaveAs((opt->GetOutdir()+opt->GetFitResultsPlotName()+varname+"."+opt->GetPlotFormat()+".root").c_str());
            
            canvas->Clear();
            
            delete graph;
            delete canvas;
            
            ++var_index;
        }
        
        // construct a meta pull variable from the fit results
        TCanvas* canvas = new TCanvas("canvas","canvas",1280,1024);
        canvas->cd();
        
        TH1F* meta_pull_hist = new TH1F("meta_pull_hist","meta_pull_hist",50,-5,5);
        for(double d : meta_pull) {
            meta_pull_hist->Fill(d);
        }
        meta_pull_hist->StatOverflows(true);
        
        std::cout << "meta_pull_hist entries: " << meta_pull_hist->GetEntries() << std::endl;
        std::cout << "meta_pull_hist underflow: " << meta_pull_hist->GetBinContent(0) << std::endl;
        std::cout << "meta_pull_hist overflow: " << meta_pull_hist->GetBinContent(51) << std::endl;
        
        meta_pull_hist->SetFillColor(kOrange);
        meta_pull_hist->Draw("HIST");
        meta_pull_hist->GetXaxis()->SetTitle("Deviation in #sigma");
        meta_pull_hist->GetYaxis()->SetTitle("Entries");
        
        TLatex text;
        text.SetTextColor(kBlack);
        text.SetTextFont(42);
        text.SetTextSize(0.045);
        text.SetNDC(1);
        std::stringstream ss;
        ss.precision(3);
        ss << "m = " << meta_pull_hist->GetMean() << " #pm " << meta_pull_hist->GetMeanError();
        text.DrawLatex(0.18, 0.9, ss.str().c_str());
        ss.str(std::string());
        ss << "#sigma = " << meta_pull_hist->GetRMS() << " #pm " << meta_pull_hist->GetRMSError();
        text.DrawLatex(0.18, 0.85, ss.str().c_str());
        
        canvas->cd();
        canvas->SaveAs((opt->GetOutdir()+opt->GetFitResultsPlotName()+"meta_pull_combined"+"."+opt->GetPlotFormat()).c_str());
        canvas->SaveAs((opt->GetOutdir()+opt->GetFitResultsPlotName()+"meta_pull_combined"+"."+opt->GetPlotFormat()+".root").c_str());
        canvas->Clear();
        
        THStack* meta_pull_stack = new THStack("meta_pull_stack","meta_pull_stack");
        std::vector<TH1F*> meta_pull_hists_;
        
        std::size_t stream_index = 0;
        short int color_array[4] = {kOrange, kRed, kSpring, kBlue};
        for(const auto& vec : meta_pull_per_file) {
            std::stringstream hs;
            hs << "meta_pull_hist_" << stream_index;
            
            meta_pull_hists_.push_back(new TH1F(hs.str().c_str(),hs.str().c_str(),50,-5,5));
            TH1F* meta_pull_hist_ = meta_pull_hists_.back();
            for(double d : vec) {
                meta_pull_hist_->Fill(d);
            }
            meta_pull_hist_->StatOverflows(true);
            
            meta_pull_hist_->SetFillColor(color_array[stream_index % 4]);
            meta_pull_hist_->SetLineWidth(2);
            meta_pull_stack->Add(meta_pull_hist_);
            
            ++stream_index;
        }
        
        meta_pull_stack->Draw("HIST");
        meta_pull_stack->GetXaxis()->SetTitle("Deviation in #sigma");
        meta_pull_stack->GetYaxis()->SetTitle("Entries");
        
        ss.str(std::string());
        ss << "m = " << meta_pull_hist->GetMean() << " #pm " << meta_pull_hist->GetMeanError();
        text.DrawLatex(0.6, 0.9, ss.str().c_str());
        ss.str(std::string());
        ss << "#sigma = " << meta_pull_hist->GetRMS() << " #pm " << meta_pull_hist->GetRMSError();
        text.DrawLatex(0.6, 0.85, ss.str().c_str());
        
        for(std::size_t i = 0; i < stream_index; ++i) {
            TLatex text_stream;
            text_stream.SetTextColor(color_array[i]);
            text_stream.SetTextFont(42);
            text_stream.SetTextSize(0.045);
            text_stream.SetNDC(1);
            ss.str(std::string());
            ss << "m = " << meta_pull_hists_[i]->GetMean() << " #pm " << meta_pull_hists_[i]->GetMeanError();
            text_stream.DrawLatex(0.18, 0.9 - 0.1*(i), ss.str().c_str());
            ss.str(std::string());
            ss << "#sigma = " << meta_pull_hists_[i]->GetRMS() << " #pm " << meta_pull_hists_[i]->GetRMSError();
            text_stream.DrawLatex(0.18, 0.85 - 0.1*(i), ss.str().c_str());
        }
        
        canvas->cd();
        canvas->SaveAs((opt->GetOutdir()+opt->GetFitResultsPlotName()+"meta_pull"+"."+opt->GetPlotFormat()).c_str());
        canvas->SaveAs((opt->GetOutdir()+opt->GetFitResultsPlotName()+"meta_pull"+"."+opt->GetPlotFormat()+".root").c_str());
        
        for(auto p : meta_pull_hists_)
            delete p;
        delete meta_pull_hist;
        delete canvas;
        
    }
    
}
