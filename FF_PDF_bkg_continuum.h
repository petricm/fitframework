/**
 * @file	FF_PDF_bkg_continuum.h
 * @date	Nov 15, 2011
 * @author	petric
 * @brief	brief description
 *
 * long description
 */

#ifndef FF_PDF_BKG_CONTINUUM_H_
#define FF_PDF_BKG_CONTINUUM_H_

#include "FF_PDF.h"

class FF_PDF_bkg_continuum : public FF_PDF {
public:
	virtual ~FF_PDF_bkg_continuum() { }
protected:
	FF_PDF_bkg_continuum(const std::string &label, const FFUtil::tuple_category_name_flags& cat) : FF_PDF(label) {
		m_category_name_flags = cat;
		m_pdf_color = kBlue;
		m_pdf_linestyle = kDashed;
		//m_pdf_legend_label = "q#bar{q} from u,d,s,c";
		m_pdf_legend_label = "Continuum";
	}
};

class FF_PDF_bkg_continuum_1D_mbc : public FF_PDF_bkg_continuum {
public:
	virtual ~FF_PDF_bkg_continuum_1D_mbc() { }
    
	FF_PDF_bkg_continuum_1D_mbc(RooRealVar& mbc, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_continuum("pdf_bkg_continuum_1D_mbc",cat) {
		// Argus with m0 = 5.289 for normal case and 5.29 for off-resonance data
		m_vars.push_back(new RooRealVar("mbc_bkg_continuum_m0","mbc bkg_continuum m0",5.2884465588412217,5.288,5.29));
		m_vars.push_back(new RooRealVar("mbc_bkg_continuum_c","mbc bkg_continuum c",-9.5910366132341082,-50.0,-1.0));
		m_vars.push_back(new RooRealVar("mbc_bkg_continuum_p","mbc bkg_continuum p",0.42483708277289478,0,1));
        
		m_pdfs.push_back(new RooArgusBG(("C_"+m_label+"_argus").c_str(),("C_"+m_label+"_argus").c_str(),mbc,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2)));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CC"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);
		m_main_pdfs["COFF"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_continuum_1D_deltae : public FF_PDF_bkg_continuum {
public:
	virtual ~FF_PDF_bkg_continuum_1D_deltae() { }
    
	FF_PDF_bkg_continuum_1D_deltae(RooRealVar& deltae, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_continuum("pdf_bkg_continuum_1D_deltae",cat) {
		m_vars.push_back(new RooRealVar("deltae_bkg_continuum_a1","deltae_bkg_continuum a1",-0.37301126047199373,-1.0,1.0));
		m_vars.push_back(new RooRealVar("deltae_bkg_continuum_a2","deltae_bkg_continuum a2",0.027750965191572607,-1.0,1.0));
        
		m_pdfs.push_back(new RooChebychev(("C_"+m_label).c_str(),("C_"+m_label).c_str(),deltae,RooArgList(*m_vars.at(0),*m_vars.at(1))));
        
        
		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CC"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);
		m_main_pdfs["COFF"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_continuum_2D_mbc_deltae : public FF_PDF_bkg_continuum {
public:
	virtual ~FF_PDF_bkg_continuum_2D_mbc_deltae() { }
    
	FF_PDF_bkg_continuum_2D_mbc_deltae(RooRealVar& mbc, RooRealVar& deltae, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_continuum("pdf_bkg_continuum_2D_mbc_deltae",cat) {
		// Argus with m0 = 5.289 for normal case and 5.29 for off-resonance data
		m_vars.push_back(new RooRealVar("mbc_bkg_continuum_m0","mbc bkg_continuum m0",5.289));
		m_vars.push_back(new RooRealVar("mbc_bkg_continuum_c","mbc bkg_continuum c",-20.0,-40.0,-1.0));
		m_vars.push_back(new RooRealVar("mbc_bkg_continuum_p","mbc bkg_continuum p",0.5));
		m_pdfs.push_back(new RooArgusBG(("CC_"+m_label+"_argus").c_str(),("CC_"+m_label+"_argus").c_str(),mbc,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2)));
        
		m_vars.push_back(new RooRealVar("deltae_bkg_continuum_a1","deltae_bkg_continuum a1",0.0,-1.0,1.0));
		m_pdfs.push_back(new RooChebychev(("CC_"+m_label+"_cheb").c_str(),("CC_"+m_label+"_cheb").c_str(),deltae,RooArgList(*m_vars.at(3))));
        
		m_pdfs.push_back(new RooProdPdf(("CC_"+m_label).c_str(),("CC_"+m_label).c_str(),RooArgList(*m_pdfs.at(0),*m_pdfs.at(1))));
        
		m_main_pdfs["CC"] = m_pdfs.back();
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_bkg_continuum_1D_chil0_ma : public FF_PDF_bkg_continuum {
public:
	virtual ~FF_PDF_bkg_continuum_1D_chil0_ma() { }
    
	FF_PDF_bkg_continuum_1D_chil0_ma(RooRealVar& chil0_ma, const FFUtil::tuple_category_name_flags& cat, bool is_data, RooRealVar* resolution) : FF_PDF_bkg_continuum("pdf_bkg_continuum_1D_chil0_ma",cat) {
		// Threshold function
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_continuum_a","chil0_ma bkg_continuum a",0.02,0.0,1.0));
		m_pdfs.push_back(new RooTwoBodyThresholdFunction(("C_"+m_label+"_nonres").c_str(),("C_"+m_label+"_nonres").c_str(),chil0_ma,*m_vars.at(0),m_K+m_K));
		m_pdfs.push_back(new RooTwoBodyThresholdFunction(("N_"+m_label+"_nonres").c_str(),("N_"+m_label+"_nonres").c_str(),chil0_ma,*m_vars.at(0),m_K+m_K));
        
		// Relativistic Spin Breit-Wigner or Two Body PSHP Breit-Wigner
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_continuum_mean","chil0_ma bkg_continuum mean",1.019455,1.019435,1.019475));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_continuum_width","chil0_ma bkg_continuum width",0.00426,0.00422,0.00430));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_continuum_r","chil0_ma bkg_continuum r",3.0));
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_continuum_J","chil0_ma bkg_continuum J",1.0));
		if(is_data) {
			m_pdfs.push_back(new RooRelativisticSpinBreitWigner(("C_"+m_label+"_BW").c_str(),("C_"+m_label+"_BW").c_str(),chil0_ma,*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),m_K,m_K));
			m_pdfs.push_back(new RooRelativisticSpinBreitWigner(("N_"+m_label+"_BW").c_str(),("N_"+m_label+"_BW").c_str(),chil0_ma,*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),m_K,m_K));
		} else {
			m_pdfs.push_back(new RooBreitWignerTwoBodyPHSP(("C_"+m_label+"_BW").c_str(),("C_"+m_label+"_BW").c_str(),chil0_ma,*m_vars.at(1),*m_vars.at(2),m_K,m_K));
			m_pdfs.push_back(new RooBreitWignerTwoBodyPHSP(("N_"+m_label+"_BW").c_str(),("N_"+m_label+"_BW").c_str(),chil0_ma,*m_vars.at(1),*m_vars.at(2),m_K,m_K));
		}
        
		// Gaussian resolution
		if(resolution != NULL) {
			m_pdfs.push_back(new RooGaussian(("C_"+m_label+"_resolution").c_str(),("C_"+m_label+"_resolution").c_str(),chil0_ma,RooConst(0.0),*resolution));
			m_pdfs.push_back(new RooGaussian(("N_"+m_label+"_resolution").c_str(),("N_"+m_label+"_resolution").c_str(),chil0_ma,RooConst(0.0),*resolution));
		} else {
			m_vars.push_back(new RooRealVar("chil0_ma_resolution","chil0_ma resolution",0.0008,0.0002,0.0014));
			m_pdfs.push_back(new RooGaussian(("C_"+m_label+"_resolution").c_str(),("C_"+m_label+"_resolution").c_str(),chil0_ma,RooConst(0.0),*m_vars.at(5)));
			m_pdfs.push_back(new RooGaussian(("N_"+m_label+"_resolution").c_str(),("N_"+m_label+"_resolution").c_str(),chil0_ma,RooConst(0.0),*m_vars.at(5)));
		}
        
		// Breit-Wigner folded with Gaussian resolution
		m_pdfs.push_back(new RooFFTConvPdf(("C_"+m_label+"_folding").c_str(),("C_"+m_label+"_folding").c_str(),chil0_ma,*m_pdfs.at(2),*m_pdfs.at(4)));
		m_pdfs.push_back(new RooFFTConvPdf(("N_"+m_label+"_folding").c_str(),("N_"+m_label+"_folding").c_str(),chil0_ma,*m_pdfs.at(3),*m_pdfs.at(5)));
		// some settings for the convolution
		dynamic_cast<RooFFTConvPdf*>(m_pdfs.at(6))->setBufferFraction(1.2);
		dynamic_cast<RooFFTConvPdf*>(m_pdfs.at(7))->setBufferFraction(1.2);
		chil0_ma.setBins(10000,"cache");
        
		// Add non-resonant threshold + resonsant part
		m_vars.push_back(new RooRealVar("C_chil0_ma_bkg_continuum_nonres_fraction","C_chil0_ma bkg_continuum non-resonant fraction",0.5,0.2,0.8));
		m_vars.push_back(new RooRealVar("N_chil0_ma_bkg_continuum_nonres_fraction","N_chil0_ma bkg_continuum non-resonant fraction",0.5,0.2,0.8));
		m_pdfs.push_back(new RooAddPdf(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),*m_pdfs.at(0),*m_pdfs.at(6),*m_vars.at(resolution == NULL ? 6 : 5)));
		m_pdfs.push_back(new RooAddPdf(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),*m_pdfs.at(1),*m_pdfs.at(7),*m_vars.at(resolution == NULL ? 7 : 6)));
        
		m_main_pdfs["C"] = m_pdfs.at(8);
		m_main_pdfs["CS"] = m_pdfs.at(8);
		m_main_pdfs["COFF"] = m_pdfs.at(8);
		m_main_pdfs["N"] = m_pdfs.at(9);
		m_main_pdfs["NS"] = m_pdfs.at(9);
		m_main_pdfs["NOFF"] = m_pdfs.at(9);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_continuum_1D_mass_kpi : public FF_PDF_bkg_continuum {
public:
	virtual ~FF_PDF_bkg_continuum_1D_mass_kpi() { }
    
	FF_PDF_bkg_continuum_1D_mass_kpi(RooRealVar& mass_kpi, const FFUtil::tuple_category_name_flags& cat, bool is_data) : FF_PDF_bkg_continuum("pdf_bkg_continuum_1D_mass_kpi",cat) {
		// Chebyshev
		m_vars.push_back(new RooRealVar("C_mass_kpi_bkg_continuum_a1","C_mass_kpi_bkg_continuum a1",0.0,-1.0,1.0));
		m_vars.push_back(new RooRealVar("C_mass_kpi_bkg_continuum_a2","C_mass_kpi_bkg_continuum a2",0.0,-1.0,1.0));
		m_vars.push_back(new RooRealVar("C_mass_kpi_bkg_continuum_a3","C_mass_kpi_bkg_continuum a3",0.0,-1.0,1.0));
		m_vars.push_back(new RooRealVar("C_mass_kpi_bkg_continuum_a4","C_mass_kpi_bkg_continuum a4",0.0,-1.0,1.0));
		m_vars.push_back(new RooRealVar("N_mass_kpi_bkg_continuum_a1","N_mass_kpi_bkg_continuum a1",0.0,-1.0,1.0));
		m_vars.push_back(new RooRealVar("N_mass_kpi_bkg_continuum_a2","N_mass_kpi_bkg_continuum a2",0.0,-1.0,1.0));
		m_vars.push_back(new RooRealVar("N_mass_kpi_bkg_continuum_a3","N_mass_kpi_bkg_continuum a3",0.0,-1.0,1.0));
		m_pdfs.push_back(new RooChebychev(("C_"+m_label+"_nonres").c_str(),("C_"+m_label+"_nonres").c_str(),mass_kpi,RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3))));
		m_pdfs.push_back(new RooChebychev(("N_"+m_label+"_nonres").c_str(),("N_"+m_label+"_nonres").c_str(),mass_kpi,RooArgList(*m_vars.at(4),*m_vars.at(5),*m_vars.at(6))));
        
		// Relativistic Spin Breit-Wigner or Two Body PSHP Breit-Wigner
		m_vars.push_back(new RooRealVar("mass_kpi_bkg_continuum_mean","mass_kpi bkg_continuum mean",0.89594,0.89572,0.89616));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_kpi_bkg_continuum_width","mass_kpi bkg_continuum width",0.0487,0.0479,0.0495));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_kpi_bkg_continuum_r","mass_kpi bkg_continuum r",3.0));
		m_vars.push_back(new RooRealVar("mass_kpi_bkg_continuum_J","mass_kpi bkg_continuum J",1.0));
		if(is_data) {
			m_pdfs.push_back(new RooRelativisticSpinBreitWigner(("C_"+m_label+"_BW").c_str(),("C_"+m_label+"_BW").c_str(),mass_kpi,*m_vars.at(7),*m_vars.at(8),*m_vars.at(9),*m_vars.at(10),m_K,m_Pi));
			m_pdfs.push_back(new RooRelativisticSpinBreitWigner(("N_"+m_label+"_BW").c_str(),("N_"+m_label+"_BW").c_str(),mass_kpi,*m_vars.at(7),*m_vars.at(8),*m_vars.at(9),*m_vars.at(10),m_Ks,m_Pi0));
		} else {
			m_pdfs.push_back(new RooBreitWignerTwoBodyPHSP(("C_"+m_label+"_BW").c_str(),("C_"+m_label+"_BW").c_str(),mass_kpi,*m_vars.at(7),*m_vars.at(8),m_K,m_Pi));
			m_pdfs.push_back(new RooBreitWignerTwoBodyPHSP(("N_"+m_label+"_BW").c_str(),("N_"+m_label+"_BW").c_str(),mass_kpi,*m_vars.at(7),*m_vars.at(8),m_Ks,m_Pi0));
		}
        
		// Add non-resonant threshold + resonsant part
		m_vars.push_back(new RooRealVar("C_mass_kpi_bkg_continuum_nonres_fraction","C_mass_kpi bkg_continuum non-resonant fraction",0.5,0.2,1.0));
		m_vars.push_back(new RooRealVar("N_mass_kpi_bkg_continuum_nonres_fraction","N_mass_kpi bkg_continuum non-resonant fraction",0.5,0.2,1.0));
		m_pdfs.push_back(new RooAddPdf(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),*m_pdfs.at(0),*m_pdfs.at(2),*m_vars.at(11)));
		m_pdfs.push_back(new RooAddPdf(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),*m_pdfs.at(1),*m_pdfs.at(3),*m_vars.at(12)));
        
		m_main_pdfs["C"] = m_pdfs.at(4);
		m_main_pdfs["CS"] = m_pdfs.at(4);
		m_main_pdfs["COFF"] = m_pdfs.at(4);
		m_main_pdfs["N"] = m_pdfs.at(5);
		m_main_pdfs["NS"] = m_pdfs.at(5);
		m_main_pdfs["NOFF"] = m_pdfs.at(5);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_continuum_1D_hel_phi : public FF_PDF_bkg_continuum {
public:
	virtual ~FF_PDF_bkg_continuum_1D_hel_phi() { }
    
	FF_PDF_bkg_continuum_1D_hel_phi(RooRealVar& hel_phi, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_continuum("pdf_bkg_continuum_1D_hel_phi",cat) {
		m_pdfs.push_back(new RooUniform(("C_"+m_label).c_str(),("C_"+m_label).c_str(),hel_phi));
		m_pdfs.push_back(new RooUniform(("N_"+m_label).c_str(),("N_"+m_label).c_str(),hel_phi));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);
		m_main_pdfs["COFF"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);
		m_main_pdfs["NS"] = m_pdfs.at(1);
		m_main_pdfs["NOFF"] = m_pdfs.at(1);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_continuum_1D_hel_th1 : public FF_PDF_bkg_continuum {
public:
	virtual ~FF_PDF_bkg_continuum_1D_hel_th1() { }
    
	FF_PDF_bkg_continuum_1D_hel_th1(RooRealVar& hel_th1, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_continuum("pdf_bkg_continuum_1D_hel_th1",cat) {
		m_vars.push_back(new RooRealVar("C_hel_th1_bkg_continuum_a1","C_hel_th1_bkg_continuum a1",0.0,-1.0,1.0));
		m_vars.push_back(new RooRealVar("C_hel_th1_bkg_continuum_a2","C_hel_th1_bkg_continuum a2",0.0,-1.0,1.0));
		m_vars.push_back(new RooRealVar("C_hel_th1_bkg_continuum_a3","C_hel_th1_bkg_continuum a3",0.0,-1.0,1.0));
		m_vars.push_back(new RooRealVar("C_hel_th1_bkg_continuum_a4","C_hel_th1_bkg_continuum a4",0.0,-1.0,1.0));
		m_vars.push_back(new RooRealVar("C_hel_th1_bkg_continuum_a5","C_hel_th1_bkg_continuum a5",0.0,-1.0,1.0));
		m_pdfs.push_back(new RooChebychevCut(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),hel_th1,RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4)),0.75));
        
		m_vars.push_back(new RooRealVar("N_hel_th1_bkg_continuum_a1","N_hel_th1_bkg_continuum a1",0.0,-1.0,1.0));
		m_vars.push_back(new RooRealVar("N_hel_th1_bkg_continuum_a2","N_hel_th1_bkg_continuum a2",0.0,-1.0,1.0));
		m_vars.push_back(new RooRealVar("N_hel_th1_bkg_continuum_a3","N_hel_th1_bkg_continuum a3",0.0,-1.0,1.0));
		m_vars.push_back(new RooRealVar("N_hel_th1_bkg_continuum_a4","N_hel_th1_bkg_continuum a4",0.0,-1.0,1.0));
		m_pdfs.push_back(new RooChebychevCut(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),hel_th1,RooArgList(*m_vars.at(5),*m_vars.at(6),*m_vars.at(7),*m_vars.at(8)),0.75));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);
		m_main_pdfs["COFF"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);
		m_main_pdfs["NS"] = m_pdfs.at(1);
		m_main_pdfs["NOFF"] = m_pdfs.at(1);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_continuum_1D_hel_th2 : public FF_PDF_bkg_continuum {
public:
	virtual ~FF_PDF_bkg_continuum_1D_hel_th2() { }
    
	FF_PDF_bkg_continuum_1D_hel_th2(RooRealVar& hel_th2, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_continuum("pdf_bkg_continuum_1D_hel_th2",cat) {
		m_vars.push_back(new RooRealVar("hel_th2_bkg_continuum_a1","hel_th2_bkg_continuum a1",0.0,-1.0,1.0));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("hel_th2_bkg_continuum_a2","hel_th2_bkg_continuum a2",0.0,-1.0,1.0));
        
		m_pdfs.push_back(new RooChebychev(("C_"+m_label).c_str(),("C_"+m_label).c_str(),hel_th2,RooArgList(*m_vars.at(0),*m_vars.at(1))));
		m_pdfs.push_back(new RooChebychev(("N_"+m_label).c_str(),("N_"+m_label).c_str(),hel_th2,RooArgList(*m_vars.at(0),*m_vars.at(1))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);
		m_main_pdfs["COFF"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);
		m_main_pdfs["NS"] = m_pdfs.at(1);
		m_main_pdfs["NOFF"] = m_pdfs.at(1);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_continuum_1D_nb_prime : public FF_PDF_bkg_continuum {
public:
	virtual ~FF_PDF_bkg_continuum_1D_nb_prime() { }
    
	FF_PDF_bkg_continuum_1D_nb_prime(RooRealVar& nb_prime, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_continuum("pdf_bkg_continuum_1D_nb_prime",cat) {

		m_vars.push_back(new RooRealVar("nb_prime_bkg_continuum_mean_1st","nb prime bkg continuum mean 1st",1.1028e+00,-5.0,5.0));//V0
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("nb_prime_bkg_continuum_mean_2nd","nb prime bkg continuum mean 2nd",-1.0392e+00,-5.0,5.0));//V0
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("nb_prime_bkg_continuum_mean_3rd","nb prime bkg continuum mean 3rd",2.8298e-01,-5.0,5.0));//V0
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("nb_prime_bkg_continuum_mean_4th","nb prime bkg continuum mean 4th",-3.1157e+00,-5.0,5.0));//V0
        m_vars.back()->setConstant(true);
        
		m_vars.push_back(new RooRealVar("nb_prime_bkg_continuum_width_1st","nb prime bkg continuum width 1st",1.9646e+00,0,5.0));//V0
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("nb_prime_bkg_continuum_width_2nd","nb prime bkg continuum width 2nd",1.2631e+00,0,5.0));//V0
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("nb_prime_bkg_continuum_width_3rd","nb prime bkg continuum width 3rd",1.2807e+00,0,5.0));//V5
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("nb_prime_bkg_continuum_width_4th","nb prime bkg continuum width 4th",1.7259e+00,0,5.0));//V0
        m_vars.back()->setConstant(true);
        
        m_vars.push_back(new RooRealVar("nb_prime_bkg_continuum_tail_1st","nb prime bkg continuum tail 1st",2.6226e-01,-5.0,5.0));//V0
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("nb_prime_bkg_continuum_tail_2nd","nb prime bkg continuum tail 2nd",1.9840e-01,-5.0,5.0));//V0
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("nb_prime_bkg_continuum_tail_3rd","nb prime bkg continuum tail 3rd",4.9625e-02,-5.0,5.0));//V0
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("nb_prime_bkg_continuum_tail_4th","nb prime bkg continuum tail 4th",0,-5.0,5.0));//V0
        m_vars.back()->setConstant(true);
        
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_1st").c_str(),("C_"+m_label+"_1st").c_str(),nb_prime,*m_vars.at(0),*m_vars.at(4),*m_vars.at(8)));
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_2nd").c_str(),("C_"+m_label+"_2nd").c_str(),nb_prime,*m_vars.at(1),*m_vars.at(5),*m_vars.at(9)));
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_3rd").c_str(),("C_"+m_label+"_3rd").c_str(),nb_prime,*m_vars.at(2),*m_vars.at(6),*m_vars.at(10)));
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_4th").c_str(),("C_"+m_label+"_4th").c_str(),nb_prime,*m_vars.at(3),*m_vars.at(7),*m_vars.at(11)));
        
		m_vars.push_back(new RooRealVar("nb_prime_bkg_continuum_frac_1st","nb prime bkg continuum fraction 1st",1.8897e-01,0,1));//V0
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("nb_prime_bkg_continuum_frac_2nd","nb prime bkg continuum fraction 2nd",4.0883e-01,0,1));//V0
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("nb_prime_bkg_continuum_frac_3rd","nb prime bkg continuum fraction 3rd",9.7465e-01,0,1));//V0
        m_vars.back()->setConstant(true);

        
		m_pdfs.push_back(new RooAddPdf((""+m_label+"").c_str(),(""+m_label+"").c_str(),RooArgList(*m_pdfs.at(0),*m_pdfs.at(1),*m_pdfs.at(2),*m_pdfs.at(3)),RooArgList(*m_vars.at(12),*m_vars.at(13),*m_vars.at(14)),kTRUE));
        
		m_main_pdfs["C"] = m_pdfs.at(4);
		m_main_pdfs["CS"] = m_pdfs.at(4);
		m_main_pdfs["COFF"] = m_pdfs.at(4);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_bkg_continuum_2D_dalitz: public FF_PDF_bkg_continuum {
public:
	virtual ~FF_PDF_bkg_continuum_2D_dalitz() { }
    
	FF_PDF_bkg_continuum_2D_dalitz(RooRealVar& mass_AB, RooRealVar& mass_BC,
                                   const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_continuum("FF_PDF_bkg_continuum_2D_dalitz", cat) {
        
        m_vars.push_back(new RooRealVar("mass_01_continuum_D0_mean_1","mass_01_continuum_D0_mean_1",3.4855,0,4));//V0
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_continuum_D0_sigma_1","mass_01_continuum_D0_sigma_1",3.0014e-02,0,5));//V1
		m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_continuum_D0_c1","mass_01_continuum_D0_c1",3.7485e-03));//V2
		m_vars.push_back(new RooRealVar("mass_01_continuum_D0_c2","mass_01_continuum_D0_c2",3.9753e-03));//V3
        m_vars.push_back(new RooRealVar("mass_01_continuum_D0_c3","mass_01_continuum_D0_c3",9.0391e-06));//V4
		m_vars.push_back(new RooRealVar("mass_01_continuum_D0_c4","mass_01_continuum_D0_c4",-1.4112e-05));//V5
        m_pdfs.push_back(new RooDalitzGauss("mass_01_continuum_D0_Gauss_1","mass_01_continuum_D0_Gauss_1",mass_AB,mass_BC,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5),23,m_Pi0,m_Pi,m_K,m_B0)); //P0
        
        m_vars.push_back(new RooRealVar("mass_01_continuum_D0_mean_2","mass_01_continuum_D0_mean_2",3.5890,0,4));//V6
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_continuum_D0_sigma_2","mass_01_continuum_D0_sigma_2",2.2063e-01,0,5));//V7
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzGauss("mass_01_continuum_D0_Gauss_2","mass_01_continuum_D0_Gauss_2",mass_AB,mass_BC,*m_vars.at(6),*m_vars.at(7),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5),23,m_Pi0,m_Pi,m_K,m_B0));//P1
        
        m_vars.push_back(new RooRealVar("mass_01_continuum_D0_frac","mass_01_continuum_D0_frac",9.2592e-01));//V8
        m_pdfs.push_back(new RooAddPdf("mass_01_continuum_D0","mass_01_continuum_D0",*m_pdfs.at(0),*m_pdfs.at(1),*m_vars.at(8))); //P2
        
        
        m_vars.push_back(new RooRealVar("mass_01_continuum_D0_miss_mean_1","mass_01_continuum_D0_miss_mean_1",2.3024,0,4));//V9
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_continuum_D0_miss_sigma_1","mass_01_continuum_D0_miss_sigma_1",5.5552e-01,0,5));//V10
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_continuum_D0_miss_c1","mass_01_continuum_D0_miss_c1",6.1593e-03));//V11
		m_vars.push_back(new RooRealVar("mass_01_continuum_D0_miss_c2","mass_01_continuum_D0_miss_c2",-6.6892e-05));//V12
        m_vars.push_back(new RooRealVar("mass_01_continuum_D0_miss_c3","mass_01_continuum_D0_miss_c3",5.0239e-05));//V13
		m_vars.push_back(new RooRealVar("mass_01_continuum_D0_miss_c4","mass_01_continuum_D0_miss_c4",-2.8413e-06));//V14
        m_pdfs.push_back(new RooDalitzGauss("mass_01_continuum_D0_miss_Gauss_1","mass_01_continuum_D0_miss_Gauss_1",mass_AB,mass_BC,*m_vars.at(9),*m_vars.at(10),*m_vars.at(11),*m_vars.at(12),*m_vars.at(13),*m_vars.at(14),23,m_Pi0,m_Pi,m_K,m_B0)); //P3
        
        m_vars.push_back(new RooRealVar("mass_01_continuum_D0_miss_mean_2","mass_01_continuum_D0_miss_mean_2",9.9095e-01,0,4));//V15
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_continuum_D0_miss_sigma_2","mass_01_continuum_D0_miss_sigma_2",1.0607,0,5));//V16
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzGauss("mass_01_continuum_D0_miss_Gauss_2","mass_01_continuum_D0_miss_Gauss_2",mass_AB,mass_BC,*m_vars.at(15),*m_vars.at(16),*m_vars.at(11),*m_vars.at(12),*m_vars.at(13),*m_vars.at(14),23,m_Pi0,m_Pi,m_K,m_B0));//P4
        m_vars.push_back(new RooRealVar("mass_01_continuum_D0_miss_frac","mass_01_continuum_D0_miss_frac",3.6610e-01,0,1));//V17
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooAddPdf("mass_01_continuum_D0_miss","mass_01_continuum_D0_miss",*m_pdfs.at(3),*m_pdfs.at(4),*m_vars.at(17)));//P5
        
        
        m_vars.push_back(new RooRealVar("mass_01_continuum_Dplus_mean_1","mass_01_continuum_Dplus_mean_1", 2.6680,0,4));//V18
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_continuum_Dplus_sigma_1","mass_01_continuum_Dplus_sigma_1",1.7364e-01,0,5));//V19
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_continuum_Dplus_c1","mass_01_continuum_Dplus_c1",3.7354e-03));//V20
		m_vars.push_back(new RooRealVar("mass_01_continuum_Dplus_c2","mass_01_continuum_Dplus_c2",7.8400e-04));//V21
        m_vars.push_back(new RooRealVar("mass_01_continuum_Dplus_c3","mass_01_continuum_Dplus_c3",6.4836e-05));//V22
		m_vars.push_back(new RooRealVar("mass_01_continuum_Dplus_c4","mass_01_continuum_Dplus_c4",-5.8829e-06));//V23
        m_pdfs.push_back(new RooDalitzGauss("mass_01_continuum_Dplus_Gauss_1","mass_01_continuum_Dplus_Gauss_1",mass_AB,mass_BC,*m_vars.at(18),*m_vars.at(19),*m_vars.at(20),*m_vars.at(21),*m_vars.at(22),*m_vars.at(23),23,m_Pi0,m_Pi,m_K,m_B0)); //P6
        
        m_vars.push_back(new RooRealVar("mass_01_continuum_Dplus_mean_2","mass_01_continuum_Dplus_mean_2",2.0999,0,4));//V24
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_continuum_Dplus_sigma_2","mass_01_continuum_Dplus_sigma_2",3.5907e-01,0,5));//V25
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzGauss("mass_01_continuum_Dplus_Gauss_2","mass_01_continuum_Dplus_Gauss_2",mass_AB,mass_BC,*m_vars.at(24),*m_vars.at(25),*m_vars.at(20),*m_vars.at(21),*m_vars.at(22),*m_vars.at(23),23,m_Pi0,m_Pi,m_K,m_B0));//P7
        
        m_vars.push_back(new RooRealVar("mass_01_continuum_Dplus_mean_3","mass_01_continuum_Dplus_mean_3",9.7765e-01,0,4));//V26
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_continuum_Dplus_sigma_3","mass_01_continuum_Dplus_sigma_3",6.9164e-01,0,5));//V27
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzGauss("mass_01_continuum_Dplus_Gauss_3","mass_01_continuum_Dplus_Gauss_3",mass_AB,mass_BC,*m_vars.at(26),*m_vars.at(27),*m_vars.at(20),*m_vars.at(21),*m_vars.at(22),*m_vars.at(23),23,m_Pi0,m_Pi,m_K,m_B0));//P8
        
        m_vars.push_back(new RooRealVar("mass_01_continuum_Dplus_frac_1","mass_01_continuum_Dplus_frac_1",1.0246e-01,0,1));//V28
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_continuum_Dplus_frac_2","mass_01_continuum_Dplus_frac_2",3.5889e-01,0,1));//V29
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooAddPdf("mass_01_continuum_Dplus","mass_01_continuum_Dplus",RooArgList(*m_pdfs.at(6),*m_pdfs.at(7),*m_pdfs.at(8)),RooArgList(*m_vars.at(28),*m_vars.at(29)),kTRUE));//P9
        

        m_vars.push_back(new RooRealVar("mass_01_continuum_K*+_mean_1","mass_01_continuum_K*+_mean_1", 8.9600000e-01,0,4));//V30
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_continuum_K*+_width_1","mass_01_continuum_K*+_width_1",5.0299999e-02,0,5));//V31
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_continuum_K*+_c1","mass_01_continuum_K*+_c1",-2.66177e-02));//V32
		m_vars.push_back(new RooRealVar("mass_01_continuum_K*+_c2","mass_01_continuum_K*+_c2",3.04243e-03));//V33
        m_vars.push_back(new RooRealVar("mass_01_continuum_K*+_c3","mass_01_continuum_K*+_c3",1.28580e-04));//V34
		m_vars.push_back(new RooRealVar("mass_01_continuum_K*+_c4","mass_01_continuum_K*+_c4",-7.85376e-06));//V35
        
        m_pdfs.push_back(new RooDalitzRelativisticSpinBreitWigner("mass_01_continuum_K*+_RBW_1","mass_01_continuum_K*+_RBW_1",mass_AB,mass_BC,*m_vars.at(30),*m_vars.at(31),*m_vars.at(32),*m_vars.at(33),*m_vars.at(34),*m_vars.at(35),5.3,1,23,m_Pi0,m_Pi,m_K,m_B0)); //P10
        
        
        m_vars.push_back(new RooRealVar("mass_01_continuum_rho770_mean_1","mass_01_continuum_rho770_mean_1", 1.0036,0,4));//V36
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_continuum_rho770_width_1","mass_01_continuum_rho770_width_1",2.2758e-01,0,5));//V37
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_continuum_rho770_c1","mass_01_continuum_rho770_c1", 6.5745e-03));//V38
		m_vars.push_back(new RooRealVar("mass_01_continuum_rho770_c2","mass_01_continuum_rho770_c2",2.7971e-04));//V39
        m_vars.push_back(new RooRealVar("mass_01_continuum_rho770_c3","mass_01_continuum_rho770_c3",6.9883e-05));//V40
		m_vars.push_back(new RooRealVar("mass_01_continuum_rho770_c4","mass_01_continuum_rho770_c4",-5.2535e-06));//V41
        
        m_pdfs.push_back(new RooDalitzRelativisticSpinBreitWigner("mass_01_continuum_rho770_RBW_1","mass_01_continuum_rho770_RBW_1",mass_AB,mass_BC,*m_vars.at(36),*m_vars.at(37),*m_vars.at(38),*m_vars.at(39),*m_vars.at(40),*m_vars.at(41),5.3,1,23,m_Pi0,m_Pi,m_K,m_B0)); //P11
        
        
        m_vars.push_back(new RooRealVar("mass_12_continuum_rho770P_mean_1","mass_12_continuum_rho770P_mean_1", 7.7549e-01,0,4));//V42
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_12_continuum_rho770P_width_1","mass_12_continuum_rho770P_width_1",1.93052e-01,0,5));//V43
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_12_continuum_rho770P_c1","mass_12_continuum_rho770P_c1", -3.17270e-02));//V44
		m_vars.push_back(new RooRealVar("mass_12_continuum_rho770P_c2","mass_12_continuum_rho770P_c2",1.32986e-03));//V45
        m_vars.push_back(new RooRealVar("mass_12_continuum_rho770P_c3","mass_12_continuum_rho770P_c3",2.65707e-05));//V46
		m_vars.push_back(new RooRealVar("mass_12_continuum_rho770P_c4","mass_12_continuum_rho770P_c4",-3.49456e-06));//V47
        
        m_pdfs.push_back(new RooDalitzRelativisticSpinBreitWigner("mass_12_continuum_rho770P_RBW_1","mass_12_continuum_rho770P_RBW_1",mass_AB,mass_BC,*m_vars.at(42),*m_vars.at(43),*m_vars.at(44),*m_vars.at(45),*m_vars.at(46),*m_vars.at(47),5.3,1,12,m_Pi0,m_Pi,m_K,m_B0)); //P12
        
        
        m_vars.push_back(new RooRealVar("mass_12_continuum_omega892_mean_1","mass_12_continuum_omega892_mean_1",2.1935e-01,0,4));//V48
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_12_continuum_omega892_sigma_1","mass_12_continuum_omega892_sigma_1",8.5261e-02,0,5));//V49
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_12_continuum_omega892_c1","mass_12_continuum_omega892_c1",-5.9059e-03));//V50
		m_vars.push_back(new RooRealVar("mass_12_continuum_omega892_c2","mass_12_continuum_omega892_c2",-3.9086e-03));//V51
        m_vars.push_back(new RooRealVar("mass_12_continuum_omega892_c3","mass_12_continuum_omega892_c3",-2.7722e-05));//V52
		m_vars.push_back(new RooRealVar("mass_12_continuum_omega892_c4","mass_12_continuum_omega892_c4",4.7212e-06));//V53
        m_pdfs.push_back(new RooDalitzGauss("mass_12_continuum_omega892_Gauss_1","mass_12_continuum_omega892_Gauss_1",mass_AB,mass_BC,*m_vars.at(48),*m_vars.at(49),*m_vars.at(50),*m_vars.at(51),*m_vars.at(52),*m_vars.at(53),12,m_Pi0,m_Pi,m_K,m_B0)); //P13
        
        m_vars.push_back(new RooRealVar("mass_12_continuum_omega892_mean_2","mass_12_continuum_omega892_mean_2",6.1025e-01,0,4));//V54
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_12_continuum_omega892_sigma_2","mass_12_continuum_omega892_sigma_2",2.8902e-01,0,5));//V55
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzGauss("mass_12_continuum_omega892_Gauss_2","mass_12_continuum_omega892_Gauss_2",mass_AB,mass_BC,*m_vars.at(54),*m_vars.at(55),*m_vars.at(50),*m_vars.at(51),*m_vars.at(52),*m_vars.at(53),12,m_Pi0,m_Pi,m_K,m_B0));//P14
        m_vars.push_back(new RooRealVar("mass_12_continuum_omega892_frac","mass_12_continuum_omega892_frac",9.3066e-01,0,1));//V56
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooAddPdf("mass_12_continuum_omega892","mass_12_continuum_omega892",*m_pdfs.at(13),*m_pdfs.at(14),*m_vars.at(56)));//P15
        
        
        m_vars.push_back(new RooRealVar("mass_02_continuum_KstarP_mean_1","mass_02_continuum_KstarP_mean_1", 8.9166000e-01,0,4));//V57
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_02_continuum_KstarP_width_1","mass_02_continuum_KstarP_width_1",7.9928e-02,0,5));//V58
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_02_continuum_KstarP_c1","mass_02_continuum_KstarP_c1", 3.0752e-02));//V59
		m_vars.push_back(new RooRealVar("mass_02_continuum_KstarP_c2","mass_02_continuum_KstarP_c2",-1.7201e-03));//V60
        m_vars.push_back(new RooRealVar("mass_02_continuum_KstarP_c3","mass_02_continuum_KstarP_c3",-1.7730e-04));//V61
		m_vars.push_back(new RooRealVar("mass_02_continuum_KstarP_c4","mass_02_continuum_KstarP_c4",-3.8135e-06));//V62
        
        m_pdfs.push_back(new RooDalitzRelativisticSpinBreitWigner("mass_02_continuum_KstarP_RBW_1","mass_02_continuum_KstarP_RBW_1",mass_AB,mass_BC,*m_vars.at(57),*m_vars.at(58),*m_vars.at(59),*m_vars.at(60),*m_vars.at(61),*m_vars.at(62),5.3,1,31,m_Pi0,m_Pi,m_K,m_B0)); //P16
        
        m_vars.push_back(new RooRealVar("mass_02_continuum_rho770_mean_1","mass_02_continuum_rho770_mean_1", 1.0104,0,4));//V63
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_02_continuum_rho770_width_1","mass_02_continuum_rho770_width_1",2.4314e-01,0,5));//V64
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_02_continuum_rho770_c1","mass_02_continuum_rho770_c1", -2.9854e-03));//V65
		m_vars.push_back(new RooRealVar("mass_02_continuum_rho770_c2","mass_02_continuum_rho770_c2",-3.3975e-03));//V66
        m_vars.push_back(new RooRealVar("mass_02_continuum_rho770_c3","mass_02_continuum_rho770_c3",-1.3773e-05));//V67
		m_vars.push_back(new RooRealVar("mass_02_continuum_rho770_c4","mass_02_continuum_rho770_c4",3.5031e-06));//V68
        
        m_pdfs.push_back(new RooDalitzRelativisticSpinBreitWigner("mass_02_continuum_rho770_RBW_1","mass_02_continuum_rho770_RBW_1",mass_AB,mass_BC,*m_vars.at(63),*m_vars.at(64),*m_vars.at(65),*m_vars.at(66),*m_vars.at(67),*m_vars.at(68),5.3,1,31,m_Pi0,m_Pi,m_K,m_B0)); //P17

        m_vars.push_back(new RooRealVar("continuum_coef_1","continuum_coef_1",1.1955e-02,0,1));//V69
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("continuum_coef_2","continuum_coef_2",1.9541e-02,0,1));//V70
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("continuum_coef_3","continuum_coef_3",1.4291e-08,0,1));//V71
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("continuum_coef_4","continuum_coef_4",1.0375e-02,0,1));//V72
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("continuum_coef_5","continuum_coef_5",7.0776e-03,0,1));//V73
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("continuum_coef_6","continuum_coef_6",5.6187e-02,0,1));//V74
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("continuum_coef_7","continuum_coef_7",7.1985e-03,0,1));//V75
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("continuum_coef_8","continuum_coef_8",9.3081e-03,0,1));//V76
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("continuum_coef_9","continuum_coef_9",7.5534e-03,0,1));//V77
        m_vars.back()->setConstant(true);
        

        TFile *file= new TFile("plots/continuum_background.root","READ");
        TH2F *continuum_pdf = dynamic_cast<TH2F*>(file->Get("data_continuum_00000000"));
        
        m_pdfs.push_back(new RooDalitzHist("continuum_nonresonant","continuum_nonresonant",mass_AB,mass_BC,continuum_pdf,m_B0,m_Pi0,m_Pi,m_K));//P18
        
        RooArgList pdfs(*m_pdfs.at(2),*m_pdfs.at(5),*m_pdfs.at(9),*m_pdfs.at(10),*m_pdfs.at(11),*m_pdfs.at(12),*m_pdfs.at(15),*m_pdfs.at(16),*m_pdfs.at(17));
        pdfs.add(*m_pdfs.at(18));
        RooArgList coefs(*m_vars.at(69),*m_vars.at(70),*m_vars.at(71),*m_vars.at(72),*m_vars.at(73),*m_vars.at(74),*m_vars.at(75),*m_vars.at(76),*m_vars.at(77));
        
        m_pdfs.push_back(new RooAddPdf("continuum_bkg","continuum_bkgce",pdfs,coefs,kTRUE));//P19
        
		m_main_pdfs["C"] = m_pdfs.at(19);
		m_main_pdfs["CC"] = m_pdfs.at(19);
		m_main_pdfs["CS"] = m_pdfs.at(19);
		m_main_pdfs["COFF"] = m_pdfs.at(19);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_bkg_continuum_2D_mass_kpi_hel_th1: public FF_PDF_bkg_continuum {
public:
	virtual ~FF_PDF_bkg_continuum_2D_mass_kpi_hel_th1() { }
    
	FF_PDF_bkg_continuum_2D_mass_kpi_hel_th1(RooRealVar& mass_kpi, RooRealVar& hel_th1,
                                             const FFUtil::tuple_category_name_flags& cat, bool is_data)
	: FF_PDF_bkg_continuum("pdf_bkg_continuum_2D_mass_kpi_hel_th1", cat) {
		// Chebyshev, pdf 0-1, var 0-6
		m_vars.push_back(new RooRealVar("C_mass_kpi_bkg_continuum_a1","C_mass_kpi_bkg_continuum a1",0.0,-1.0,1.0));
		m_vars.push_back(new RooRealVar("C_mass_kpi_bkg_continuum_a2","C_mass_kpi_bkg_continuum a2",0.0,-1.0,1.0));
		m_vars.push_back(new RooRealVar("C_mass_kpi_bkg_continuum_a3","C_mass_kpi_bkg_continuum a3",0.0,-1.0,1.0));
		m_vars.push_back(new RooRealVar("C_mass_kpi_bkg_continuum_a4","C_mass_kpi_bkg_continuum a4",0.0,-1.0,1.0));
		m_vars.push_back(new RooRealVar("N_mass_kpi_bkg_continuum_a1","N_mass_kpi_bkg_continuum a1",0.0,-1.0,1.0));
		m_vars.push_back(new RooRealVar("N_mass_kpi_bkg_continuum_a2","N_mass_kpi_bkg_continuum a2",0.0,-1.0,1.0));
		m_vars.push_back(new RooRealVar("N_mass_kpi_bkg_continuum_a3","N_mass_kpi_bkg_continuum a3",0.0,-1.0,1.0));
		m_pdfs.push_back(new RooChebychev(("C_"+m_label+"_mass_kpi_nonres").c_str(),("C_"+m_label+"_mass_kpi_nonres").c_str(),mass_kpi,RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3))));
		m_pdfs.push_back(new RooChebychev(("N_"+m_label+"_mass_kpi_nonres").c_str(),("N_"+m_label+"_mass_kpi_nonres").c_str(),mass_kpi,RooArgList(*m_vars.at(4),*m_vars.at(5),*m_vars.at(6))));
        
		// Relativistic Spin Breit-Wigner or Two Body PSHP Breit-Wigner, pdf 2-3, var 7-10
		m_vars.push_back(new RooRealVar("mass_kpi_bkg_continuum_mean","mass_kpi bkg_continuum mean",0.89594,0.89572,0.89616));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_kpi_bkg_continuum_width","mass_kpi bkg_continuum width",0.0487,0.0479,0.0495));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_kpi_bkg_continuum_r","mass_kpi bkg_continuum r",3.0));
		m_vars.push_back(new RooRealVar("mass_kpi_bkg_continuum_J","mass_kpi bkg_continuum J",1.0));
		if(is_data) {
			m_pdfs.push_back(new RooRelativisticSpinBreitWigner(("C_"+m_label+"_mass_kpi_BW").c_str(),("C_"+m_label+"_mass_kpi_BW").c_str(),mass_kpi,*m_vars.at(7),*m_vars.at(8),*m_vars.at(9),*m_vars.at(10),m_K,m_Pi));
			m_pdfs.push_back(new RooRelativisticSpinBreitWigner(("N_"+m_label+"_mass_kpi_BW").c_str(),("N_"+m_label+"_mass_kpi_BW").c_str(),mass_kpi,*m_vars.at(7),*m_vars.at(8),*m_vars.at(9),*m_vars.at(10),m_Ks,m_Pi0));
		} else {
			m_pdfs.push_back(new RooBreitWignerTwoBodyPHSP(("C_"+m_label+"_mass_kpi_BW").c_str(),("C_"+m_label+"_mass_kpi_BW").c_str(),mass_kpi,*m_vars.at(7),*m_vars.at(8),m_K,m_Pi));
			m_pdfs.push_back(new RooBreitWignerTwoBodyPHSP(("N_"+m_label+"_mass_kpi_BW").c_str(),("N_"+m_label+"_mass_kpi_BW").c_str(),mass_kpi,*m_vars.at(7),*m_vars.at(8),m_Ks,m_Pi0));
		}
        
		// Chebyshev for non-resonant part
		m_vars.push_back(new RooRealVar("C_hel_th1_bkg_continuum_mass_kpi_center_value","C_hel_th1_bkg_continuum_mass_kpi_center_value",-1.125)); // 11
        
		m_vars.push_back(new RooRealVar("C_hel_th1_bkg_continuum_a1_polyvar_mass_kpi_c0","C_hel_th1_bkg_continuum a1_mass_kpi polyvar_c0",0.25,0.1,0.4)); // 12
		m_vars.push_back(new RooRealVar("C_hel_th1_bkg_continuum_a1_polyvar_mass_kpi_c1","C_hel_th1_bkg_continuum a1_mass_kpi polyvar_c1",0.15,0.0,0.4)); // 13
		m_functions.push_back(new RooPolyVar("C_hel_th1_bkg_continuum_a1_polyvar_mass_kpi_offset","C_hel_th1_bkg_continuum_a1_polyvar_mass_kpi_offset",
                                             *m_vars.at(11),RooArgList(*m_vars.at(12),*m_vars.at(13)))); // F0
		m_functions.push_back(new RooPolyVar("C_hel_th1_bkg_continuum_a1_polyvar_mass_kpi","C_hel_th1_bkg_continuum_a1_polyvar_mass_kpi",
                                             mass_kpi,RooArgList(*m_functions.back(),*m_vars.at(13)))); // F1
        
		m_vars.push_back(new RooRealVar("C_hel_th1_bkg_continuum_a2_polyvar_mass_kpi_c0","C_hel_th1_bkg_continuum a2_mass_kpi polyvar_c0",0.3,0.2,0.4)); // 14
		m_vars.push_back(new RooRealVar("C_hel_th1_bkg_continuum_a2_polyvar_mass_kpi_c1","C_hel_th1_bkg_continuum a2_mass_kpi polyvar_c1",0.5,0.3,0.7)); // 15
		m_functions.push_back(new RooPolyVar("C_hel_th1_bkg_continuum_a2_polyvar_mass_kpi_offset","C_hel_th1_bkg_continuum_a2_polyvar_mass_kpi_offset",
                                             *m_vars.at(11),RooArgList(*m_vars.at(14),*m_vars.at(15)))); // F2
		m_functions.push_back(new RooPolyVar("C_hel_th1_bkg_continuum_a2_polyvar_mass_kpi","C_hel_th1_bkg_continuum_a2_polyvar_mass_kpi",
                                             mass_kpi,RooArgList(*m_functions.back(),*m_vars.at(15)))); // F3
        
		m_vars.push_back(new RooRealVar("C_hel_th1_bkg_continuum_a3_polyvar_mass_kpi_c0","C_hel_th1_bkg_continuum a3_mass_kpi polyvar_c0",0.1,0.0,0.2)); // 16
		m_vars.push_back(new RooRealVar("C_hel_th1_bkg_continuum_a3_polyvar_mass_kpi_c1","C_hel_th1_bkg_continuum a3_mass_kpi polyvar_c1",0.2,0.0,0.4)); // 17
		m_functions.push_back(new RooPolyVar("C_hel_th1_bkg_continuum_a3_polyvar_mass_kpi_offset","C_hel_th1_bkg_continuum_a3_polyvar_mass_kpi_offset",
                                             *m_vars.at(11),RooArgList(*m_vars.at(16),*m_vars.at(17)))); // F4
		m_functions.push_back(new RooPolyVar("C_hel_th1_bkg_continuum_a3_polyvar_mass_kpi","C_hel_th1_bkg_continuum_a3_polyvar_mass_kpi",
                                             mass_kpi,RooArgList(*m_functions.back(),*m_vars.at(17)))); // F5
        
		m_vars.push_back(new RooRealVar("C_hel_th1_bkg_continuum_a4_polyvar_mass_kpi_c0","C_hel_th1_bkg_continuum a4_mass_kpi polyvar_c0",0.0,-0.1,0.1)); // 18
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_hel_th1_bkg_continuum_a4_polyvar_mass_kpi_c1","C_hel_th1_bkg_continuum a4_mass_kpi polyvar_c1",0.0,-0.1,0.1)); // 19
		m_vars.back()->setConstant(true);
		m_functions.push_back(new RooPolyVar("C_hel_th1_bkg_continuum_a4_polyvar_mass_kpi_offset","C_hel_th1_bkg_continuum_a4_polyvar_mass_kpi_offset",
                                             *m_vars.at(11),RooArgList(*m_vars.at(18),*m_vars.at(19)))); // F6
		m_functions.push_back(new RooPolyVar("C_hel_th1_bkg_continuum_a4_polyvar_mass_kpi","C_hel_th1_bkg_continuum_a4_polyvar_mass_kpi",
                                             mass_kpi,RooArgList(*m_functions.back(),*m_vars.at(19)))); // F7
        
		m_vars.push_back(new RooRealVar("C_hel_th1_bkg_continuum_a5_polyvar_mass_kpi_c0","C_hel_th1_bkg_continuum a5_mass_kpi polyvar_c0",0.05,0.0,0.15)); // 20
		m_vars.push_back(new RooRealVar("C_hel_th1_bkg_continuum_a5_polyvar_mass_kpi_c1","C_hel_th1_bkg_continuum a5_mass_kpi polyvar_c1",0.05,0.0,0.15)); // 21
		m_functions.push_back(new RooPolyVar("C_hel_th1_bkg_continuum_a5_polyvar_mass_kpi_offset","C_hel_th1_bkg_continuum_a5_polyvar_mass_kpi_offset",
                                             *m_vars.at(11),RooArgList(*m_vars.at(20),*m_vars.at(21)))); // F8
		m_functions.push_back(new RooPolyVar("C_hel_th1_bkg_continuum_a5_polyvar_mass_kpi","C_hel_th1_bkg_continuum_a5_polyvar_mass_kpi",
                                             mass_kpi,RooArgList(*m_functions.back(),*m_vars.at(21)))); // F9
        
		m_vars.push_back(new RooRealVar("N_hel_th1_bkg_continuum_a1_polyvar_mass_kpi_c0","N_hel_th1_bkg_continuum a1_mass_kpi polyvar_c0",0.8,0.5,1.0)); // 22
        
		m_vars.push_back(new RooRealVar("N_hel_th1_bkg_continuum_a2_polyvar_mass_kpi_c0","N_hel_th1_bkg_continuum a2_mass_kpi polyvar_c0",0.3,0.2,0.4)); // 23
		m_vars.push_back(new RooRealVar("N_hel_th1_bkg_continuum_a2_polyvar_mass_kpi_c1","N_hel_th1_bkg_continuum a2_mass_kpi polyvar_c1",0.5,0.3,0.7)); // 24
		m_functions.push_back(new RooPolyVar("N_hel_th1_bkg_continuum_a2_polyvar_mass_kpi_offset","N_hel_th1_bkg_continuum_a2_polyvar_mass_kpi_offset",
                                             *m_vars.at(11),RooArgList(*m_vars.at(23),*m_vars.at(24)))); // F10
		m_functions.push_back(new RooPolyVar("N_hel_th1_bkg_continuum_a2_polyvar_mass_kpi","N_hel_th1_bkg_continuum_a2_polyvar_mass_kpi",
                                             mass_kpi,RooArgList(*m_functions.back(),*m_vars.at(24)))); // F11
        
		m_vars.push_back(new RooRealVar("N_hel_th1_bkg_continuum_a3_polyvar_mass_kpi_c0","N_hel_th1_bkg_continuum a3_mass_kpi polyvar_c0",0.1,0.0,0.2)); // 25
        
		m_vars.push_back(new RooRealVar("N_hel_th1_bkg_continuum_a4_polyvar_mass_kpi_c0","N_hel_th1_bkg_continuum a4_mass_kpi polyvar_c0",0.05,0.0,0.1)); // 26
        
		m_pdfs.push_back(new RooChebychevCut(("C_"+m_label+"_hel_th1_nonres").c_str(),("C_"+m_label+"_hel_th1_nonres").c_str(),hel_th1,RooArgList(*m_functions.at(1),*m_functions.at(3),*m_functions.at(5),*m_functions.at(7),*m_functions.at(9)),0.75));
		m_pdfs.push_back(new RooChebychevCut(("N_"+m_label+"_hel_th1_nonres").c_str(),("N_"+m_label+"_hel_th1_nonres").c_str(),hel_th1,RooArgList(*m_vars.at(22),*m_functions.at(11),*m_vars.at(25),*m_vars.at(26)),0.75));
        
		// Uniform for BW part
		m_pdfs.push_back(new RooCutPdf(("C_"+m_label+"_hel_th1_BW").c_str(),("C_"+m_label+"_hel_th1_BW").c_str(),hel_th1,0.75,true));
		m_pdfs.push_back(new RooCutPdf(("N_"+m_label+"_hel_th1_BW").c_str(),("N_"+m_label+"_hel_th1_BW").c_str(),hel_th1,0.75,true));
        
		// Prod PDF for BW part
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label+"_BW").c_str(),("C_"+m_label+"_BW").c_str(),
                                        *m_pdfs.at(2),*m_pdfs.at(6)));
		m_pdfs.push_back(new RooProdPdf(("N_"+m_label+"_BW").c_str(),("N_"+m_label+"_BW").c_str(),
                                        *m_pdfs.at(3),*m_pdfs.at(7)));
		// Conditional prod PDF for non-res part
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label+"_nonres").c_str(),("C_"+m_label+"_nonres").c_str(),
                                        *m_pdfs.at(0),Conditional(*m_pdfs.at(4),hel_th1)));
		m_pdfs.push_back(new RooProdPdf(("N_"+m_label+"_nonres").c_str(),("N_"+m_label+"_nonres").c_str(),
                                        *m_pdfs.at(1),Conditional(*m_pdfs.at(5),hel_th1)));
        
		// Both parts added
		m_vars.push_back(new RooRealVar("C_mass_kpi_bkg_continuum_nonres_fraction","C_mass_kpi_hel_th1 bkg_continuum non-resonant fraction",0.9,0.8,1.0)); // 27
		m_vars.push_back(new RooRealVar("N_mass_kpi_bkg_continuum_nonres_fraction","N_mass_kpi_hel_th1 bkg_continuum non-resonant fraction",0.9,0.8,1.0)); // 28
		m_pdfs.push_back(new RooAddPdf(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),*m_pdfs.at(10),*m_pdfs.at(8),*m_vars.at(27)));
		m_pdfs.push_back(new RooAddPdf(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),*m_pdfs.at(11),*m_pdfs.at(9),*m_vars.at(28)));
        
		m_main_pdfs["C"] = m_pdfs.at(12);
		m_main_pdfs["CS"] = m_pdfs.at(12);
		m_main_pdfs["COFF"] = m_pdfs.at(12);
		m_main_pdfs["N"] = m_pdfs.at(13);
		m_main_pdfs["NS"] = m_pdfs.at(13);
		m_main_pdfs["NOFF"] = m_pdfs.at(13);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_continuum_3D_deltae_chil0_ma_nb_prime: public FF_PDF_bkg_continuum {
public:
	virtual ~FF_PDF_bkg_continuum_3D_deltae_chil0_ma_nb_prime() { }
    
	FF_PDF_bkg_continuum_3D_deltae_chil0_ma_nb_prime(RooRealVar& deltae, RooRealVar& chil0_ma, RooRealVar& nb_prime,
                                                     const FFUtil::tuple_category_name_flags& cat, bool is_data, RooRealVar* resolution)
	: FF_PDF_bkg_continuum("pdf_bkg_continuum_3D_deltae_chil0_ma_nb_prime", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_chil0_ma>(chil0_ma,cat,is_data,resolution));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_nb_prime>(nb_prime,cat));
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
                                        RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
                                                   m_comb_pdfs.at(2)->GetPDF("C"))));
		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
                                        RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
                                                   m_comb_pdfs.at(2)->GetPDF("N"))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);
		m_main_pdfs["COFF"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);
		m_main_pdfs["NS"] = m_pdfs.at(1);
		m_main_pdfs["NOFF"] = m_pdfs.at(1);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_bkg_continuum_3D_mbc_deltae_nb_prime: public FF_PDF_bkg_continuum {
public:
	virtual ~FF_PDF_bkg_continuum_3D_mbc_deltae_nb_prime() { }
    
	FF_PDF_bkg_continuum_3D_mbc_deltae_nb_prime(RooRealVar& mbc, RooRealVar& deltae,RooRealVar& nb_prime,
                                                const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_continuum("pdf_bkg_continuum_3D_mbc_deltae_nb_prime", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_nb_prime>(nb_prime,cat));
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
                                        RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
                                                   m_comb_pdfs.at(2)->GetPDF("C"))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
        m_main_pdfs["CP"] = m_pdfs.at(0);
		m_main_pdfs["CA"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);
		m_main_pdfs["COFF"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
    
	FF_PDF_bkg_continuum_3D_mbc_deltae_nb_prime(shared_ptr<FF_PDF> mbc, shared_ptr<FF_PDF> deltae,
                                                shared_ptr<FF_PDF> nb_prime,
                                                const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_continuum("pdf_bkg_continuum_3D_mbc_deltae_nb_prime", cat) {
		m_comb_pdfs.push_back(mbc);
		m_comb_pdfs.push_back(deltae);
		m_comb_pdfs.push_back(nb_prime);
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
                                        RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
                                                   m_comb_pdfs.at(2)->GetPDF("C"))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
        m_main_pdfs["CP"] = m_pdfs.at(0);
		m_main_pdfs["CA"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);
		m_main_pdfs["COFF"] = m_pdfs.at(0);
        
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_bkg_continuum_4D_mbc_deltae_chil0_ma_nb_prime: public FF_PDF_bkg_continuum {
public:
	virtual ~FF_PDF_bkg_continuum_4D_mbc_deltae_chil0_ma_nb_prime() { }
    
	FF_PDF_bkg_continuum_4D_mbc_deltae_chil0_ma_nb_prime(RooRealVar& mbc, RooRealVar& deltae,
                                                         RooRealVar& chil0_ma, RooRealVar& nb_prime,
                                                         const FFUtil::tuple_category_name_flags& cat, bool is_data, RooRealVar* resolution)
	: FF_PDF_bkg_continuum("pdf_bkg_continuum_4D_mbc_deltae_chil0_ma_nb_prime", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_chil0_ma>(chil0_ma,cat,is_data,resolution));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_nb_prime>(nb_prime,cat));
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
                                        RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
                                                   m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"))));
		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
                                        RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
                                                   m_comb_pdfs.at(2)->GetPDF("N"),m_comb_pdfs.at(3)->GetPDF("N"))));
        
		m_pdfs.push_back(new RooProdPdf(("CS_"+m_label).c_str(),("CS_"+m_label).c_str(),
                                        RooArgList(m_comb_pdfs.at(0)->GetPDF("CS"),m_comb_pdfs.at(1)->GetPDF("CS"),
                                                   m_comb_pdfs.at(2)->GetPDF("CS"),m_comb_pdfs.at(3)->GetPDF("CS"))));
		m_pdfs.push_back(new RooProdPdf(("NS_"+m_label).c_str(),("NS_"+m_label).c_str(),
                                        RooArgList(m_comb_pdfs.at(0)->GetPDF("CS"),m_comb_pdfs.at(1)->GetPDF("NS"),
                                                   m_comb_pdfs.at(2)->GetPDF("NS"),m_comb_pdfs.at(3)->GetPDF("NS"))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(2);
		m_main_pdfs["COFF"] = m_pdfs.at(2);
		m_main_pdfs["N"] = m_pdfs.at(1);
		m_main_pdfs["NS"] = m_pdfs.at(3);
		m_main_pdfs["NOFF"] = m_pdfs.at(3);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
    
	FF_PDF_bkg_continuum_4D_mbc_deltae_chil0_ma_nb_prime(shared_ptr<FF_PDF> mbc, shared_ptr<FF_PDF> deltae,
                                                         shared_ptr<FF_PDF> chil0_ma, shared_ptr<FF_PDF> nb_prime,
                                                         const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_continuum("pdf_bkg_continuum_4D_mbc_deltae_chil0_ma_nb_prime", cat) {
		m_comb_pdfs.push_back(mbc);
		m_comb_pdfs.push_back(deltae);
		m_comb_pdfs.push_back(chil0_ma);
		m_comb_pdfs.push_back(nb_prime);
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
                                        RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
                                                   m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"))));
		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
                                        RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
                                                   m_comb_pdfs.at(2)->GetPDF("N"),m_comb_pdfs.at(3)->GetPDF("N"))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);
		m_main_pdfs["COFF"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);
		m_main_pdfs["NS"] = m_pdfs.at(1);
		m_main_pdfs["NOFF"] = m_pdfs.at(1);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_continuum_6D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th2: public FF_PDF_bkg_continuum {
public:
	virtual ~FF_PDF_bkg_continuum_6D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th2() { }
    
	FF_PDF_bkg_continuum_6D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th2(RooRealVar& deltae, RooRealVar& chil0_ma, RooRealVar& nb_prime,
                                                                              RooRealVar& mass_kpi, RooRealVar& hel_phi, RooRealVar& hel_th2,
                                                                              const FFUtil::tuple_category_name_flags& cat, bool is_data, RooRealVar* resolution)
	: FF_PDF_bkg_continuum("pdf_bkg_continuum_6D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th2", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_chil0_ma>(chil0_ma,cat,is_data,resolution));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_nb_prime>(nb_prime,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_mass_kpi>(mass_kpi,cat,is_data));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_hel_phi>(hel_phi,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_hel_th2>(hel_th2,cat));
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
                                        RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
                                                   m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),
                                                   m_comb_pdfs.at(4)->GetPDF("C"),m_comb_pdfs.at(5)->GetPDF("C"))));
		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
                                        RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
                                                   m_comb_pdfs.at(2)->GetPDF("N"),m_comb_pdfs.at(3)->GetPDF("N"),
                                                   m_comb_pdfs.at(4)->GetPDF("N"),m_comb_pdfs.at(5)->GetPDF("N"))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);
		m_main_pdfs["COFF"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);
		m_main_pdfs["NS"] = m_pdfs.at(1);
		m_main_pdfs["NOFF"] = m_pdfs.at(1);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_bkg_continuum_7D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th1_hel_th2: public FF_PDF_bkg_continuum {
public:
	virtual ~FF_PDF_bkg_continuum_7D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th1_hel_th2() { }
    
	FF_PDF_bkg_continuum_7D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th1_hel_th2(RooRealVar& deltae, RooRealVar& chil0_ma, RooRealVar& nb_prime,
                                                                                      RooRealVar& mass_kpi, RooRealVar& hel_phi, RooRealVar& hel_th1, RooRealVar& hel_th2,
                                                                                      const FFUtil::tuple_category_name_flags& cat, bool is_data, RooRealVar* resolution)
	: FF_PDF_bkg_continuum("pdf_bkg_continuum_7D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th1_hel_th2", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_chil0_ma>(chil0_ma,cat,is_data,resolution));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_nb_prime>(nb_prime,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_2D_mass_kpi_hel_th1>(mass_kpi,hel_th1,cat,is_data));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_hel_phi>(hel_phi,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_hel_th2>(hel_th2,cat));
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
                                        RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
                                                   m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),
                                                   m_comb_pdfs.at(4)->GetPDF("C"),m_comb_pdfs.at(5)->GetPDF("C")
                                                   )));
		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
                                        RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
                                                   m_comb_pdfs.at(2)->GetPDF("N"),m_comb_pdfs.at(3)->GetPDF("N"),
                                                   m_comb_pdfs.at(4)->GetPDF("N"),m_comb_pdfs.at(5)->GetPDF("N")
                                                   )));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);
		m_main_pdfs["COFF"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);
		m_main_pdfs["NS"] = m_pdfs.at(1);
		m_main_pdfs["NOFF"] = m_pdfs.at(1);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_continuum_4D_mass_kpi_hel_phi_hel_th1_hel_th2: public FF_PDF_bkg_continuum {
public:
	virtual ~FF_PDF_bkg_continuum_4D_mass_kpi_hel_phi_hel_th1_hel_th2() { }
    
	FF_PDF_bkg_continuum_4D_mass_kpi_hel_phi_hel_th1_hel_th2(RooRealVar& mass_kpi, RooRealVar& hel_phi,
                                                             RooRealVar& hel_th1, RooRealVar& hel_th2,
                                                             const FFUtil::tuple_category_name_flags& cat, bool is_data)
	: FF_PDF_bkg_continuum("pdf_bkg_continuum_4D_mass_kpi_hel_phi_hel_th1_hel_th2", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_2D_mass_kpi_hel_th1>(mass_kpi,hel_th1,cat,is_data));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_hel_phi>(hel_phi,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_hel_th2>(hel_th2,cat));
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
                                        RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
                                                   m_comb_pdfs.at(2)->GetPDF("C"))));
		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
                                        RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
                                                   m_comb_pdfs.at(2)->GetPDF("N"))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);
		m_main_pdfs["COFF"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);
		m_main_pdfs["NS"] = m_pdfs.at(1);
		m_main_pdfs["NOFF"] = m_pdfs.at(1);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
    
	FF_PDF_bkg_continuum_4D_mass_kpi_hel_phi_hel_th1_hel_th2(shared_ptr<FF_PDF> mass_kpi_hel_th1, shared_ptr<FF_PDF> hel_phi,
                                                             shared_ptr<FF_PDF> hel_th2,	const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_continuum("pdf_bkg_continuum_4D_mass_kpi_hel_phi_hel_th1_hel_th2", cat) {
		m_comb_pdfs.push_back(mass_kpi_hel_th1);
		m_comb_pdfs.push_back(hel_phi);
		m_comb_pdfs.push_back(hel_th2);
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
                                        RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
                                                   m_comb_pdfs.at(2)->GetPDF("C"))));
		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
                                        RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
                                                   m_comb_pdfs.at(2)->GetPDF("N"))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);
		m_main_pdfs["COFF"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);
		m_main_pdfs["NS"] = m_pdfs.at(1);
		m_main_pdfs["NOFF"] = m_pdfs.at(1);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_continuum_5D_mass_kpi_hel_phi_hel_th1_hel_th2_flavor: public FF_PDF_bkg_continuum {
public:
	virtual ~FF_PDF_bkg_continuum_5D_mass_kpi_hel_phi_hel_th1_hel_th2_flavor() { }
    
	FF_PDF_bkg_continuum_5D_mass_kpi_hel_phi_hel_th1_hel_th2_flavor(RooRealVar& mass_kpi, RooRealVar& hel_phi,
                                                                    RooRealVar& hel_th1, RooRealVar& hel_th2, RooCategory& flavor,
                                                                    const FFUtil::tuple_category_name_flags& cat, bool is_data)
	: FF_PDF_bkg_continuum("pdf_bkg_continuum_5D_mass_kpi_hel_phi_hel_th1_hel_th2_flavor", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_2D_mass_kpi_hel_th1>(mass_kpi,hel_th1,cat,is_data));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_hel_phi>(hel_phi,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_hel_th2>(hel_th2,cat));
        
		m_pdfs.push_back(new RooCategoryPdf(("C_"+m_label+"_flavor").c_str(),("C_"+m_label+"_flavor").c_str(),flavor));
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
                                        RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
                                                   m_comb_pdfs.at(2)->GetPDF("C"),*m_pdfs.back())));
		m_main_pdfs["C"] = m_pdfs.back();
		m_main_pdfs["CS"] = m_pdfs.back();
		m_main_pdfs["COFF"] = m_pdfs.back();
        
        //		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
        //				RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
        //						m_comb_pdfs.at(2)->GetPDF("N"))));
        //		m_main_pdfs["N"] = m_pdfs.back();
        //		m_main_pdfs["NS"] = m_pdfs.back();
        //		m_main_pdfs["NOFF"] = m_pdfs.back();
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_continuum_8D_mbc_deltae_chil0_ma_mass_kpi_hel_phi_hel_th1_hel_th2_nb_prime: public FF_PDF_bkg_continuum {
public:
	virtual ~FF_PDF_bkg_continuum_8D_mbc_deltae_chil0_ma_mass_kpi_hel_phi_hel_th1_hel_th2_nb_prime() { }
    
	FF_PDF_bkg_continuum_8D_mbc_deltae_chil0_ma_mass_kpi_hel_phi_hel_th1_hel_th2_nb_prime(RooRealVar& mbc, RooRealVar& deltae,
                                                                                          RooRealVar& chil0_ma, RooRealVar& mass_kpi, RooRealVar& hel_phi,
                                                                                          RooRealVar& hel_th1, RooRealVar& hel_th2, RooRealVar& nb_prime,
                                                                                          const FFUtil::tuple_category_name_flags& cat, bool is_data, RooRealVar* resolution)
	: FF_PDF_bkg_continuum("pdf_bkg_continuum_8D_mbc_deltae_chil0_ma_mass_kpi_hel_phi_hel_th1_hel_th2_nb_prime", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_chil0_ma>(chil0_ma,cat,is_data,resolution));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_nb_prime>(nb_prime,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_2D_mass_kpi_hel_th1>(mass_kpi,hel_th1,cat,is_data));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_hel_phi>(hel_phi,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_hel_th2>(hel_th2,cat));
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
                                        RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
                                                   m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),
                                                   m_comb_pdfs.at(4)->GetPDF("C"),m_comb_pdfs.at(5)->GetPDF("C"),
                                                   m_comb_pdfs.at(6)->GetPDF("C"))));
		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
                                        RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
                                                   m_comb_pdfs.at(2)->GetPDF("N"),m_comb_pdfs.at(3)->GetPDF("N"),
                                                   m_comb_pdfs.at(4)->GetPDF("N"),m_comb_pdfs.at(5)->GetPDF("N"),
                                                   m_comb_pdfs.at(6)->GetPDF("N"))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);
		m_main_pdfs["COFF"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);
		m_main_pdfs["NS"] = m_pdfs.at(1);
		m_main_pdfs["NOFF"] = m_pdfs.at(1);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
    
	FF_PDF_bkg_continuum_8D_mbc_deltae_chil0_ma_mass_kpi_hel_phi_hel_th1_hel_th2_nb_prime(shared_ptr<FF_PDF> mbc, shared_ptr<FF_PDF> deltae,
                                                                                          shared_ptr<FF_PDF> chil0_ma, shared_ptr<FF_PDF> nb_prime, shared_ptr<FF_PDF> mass_kpi_hel_th1, shared_ptr<FF_PDF> hel_phi,
                                                                                          shared_ptr<FF_PDF> hel_th2,	const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_continuum("pdf_bkg_continuum_8D_mbc_deltae_chil0_ma_mass_kpi_hel_phi_hel_th1_hel_th2_nb_prime", cat) {
		m_comb_pdfs.push_back(mbc);
		m_comb_pdfs.push_back(deltae);
		m_comb_pdfs.push_back(chil0_ma);
		m_comb_pdfs.push_back(nb_prime);
		m_comb_pdfs.push_back(mass_kpi_hel_th1);
		m_comb_pdfs.push_back(hel_phi);
		m_comb_pdfs.push_back(hel_th2);
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
                                        RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
                                                   m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),
                                                   m_comb_pdfs.at(4)->GetPDF("C"),m_comb_pdfs.at(5)->GetPDF("C"),
                                                   m_comb_pdfs.at(6)->GetPDF("C"))));
		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
                                        RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
                                                   m_comb_pdfs.at(2)->GetPDF("N"),m_comb_pdfs.at(3)->GetPDF("N"),
                                                   m_comb_pdfs.at(4)->GetPDF("N"),m_comb_pdfs.at(5)->GetPDF("N"),
                                                   m_comb_pdfs.at(6)->GetPDF("N"))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);
		m_main_pdfs["COFF"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);
		m_main_pdfs["NS"] = m_pdfs.at(1);
		m_main_pdfs["NOFF"] = m_pdfs.at(1);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_continuum_9D_mbc_deltae_chil0_ma_mass_kpi_hel_phi_hel_th1_hel_th2_nb_prime_flavor: public FF_PDF_bkg_continuum {
public:
	virtual ~FF_PDF_bkg_continuum_9D_mbc_deltae_chil0_ma_mass_kpi_hel_phi_hel_th1_hel_th2_nb_prime_flavor() { }
    
	FF_PDF_bkg_continuum_9D_mbc_deltae_chil0_ma_mass_kpi_hel_phi_hel_th1_hel_th2_nb_prime_flavor(RooRealVar& mbc, RooRealVar& deltae,
                                                                                                 RooRealVar& chil0_ma, RooRealVar& mass_kpi, RooRealVar& hel_phi,
                                                                                                 RooRealVar& hel_th1, RooRealVar& hel_th2, RooRealVar& nb_prime,
                                                                                                 RooCategory& flavor,
                                                                                                 const FFUtil::tuple_category_name_flags& cat, bool is_data, RooRealVar* resolution)
	: FF_PDF_bkg_continuum("pdf_bkg_continuum_9D_mbc_deltae_chil0_ma_mass_kpi_hel_phi_hel_th1_hel_th2_nb_prime_flavor", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_chil0_ma>(chil0_ma,cat,is_data,resolution));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_nb_prime>(nb_prime,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_2D_mass_kpi_hel_th1>(mass_kpi,hel_th1,cat,is_data));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_hel_phi>(hel_phi,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_hel_th2>(hel_th2,cat));
        
		m_pdfs.push_back(new RooCategoryPdf(("C_"+m_label+"_flavor").c_str(),("C_"+m_label+"_flavor").c_str(),flavor));
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
                                        RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
                                                   m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),
                                                   m_comb_pdfs.at(4)->GetPDF("C"),m_comb_pdfs.at(5)->GetPDF("C"),
                                                   m_comb_pdfs.at(6)->GetPDF("C"),*m_pdfs.back())));
		m_main_pdfs["C"] = m_pdfs.back();
		m_main_pdfs["CS"] = m_pdfs.back();
		m_main_pdfs["COFF"] = m_pdfs.back();
        
        //		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
        //				RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
        //						m_comb_pdfs.at(2)->GetPDF("N"),m_comb_pdfs.at(3)->GetPDF("N"),
        //						m_comb_pdfs.at(4)->GetPDF("N"),m_comb_pdfs.at(5)->GetPDF("N"),
        //						m_comb_pdfs.at(6)->GetPDF("N"))));
        //		m_main_pdfs["N"] = m_pdfs.back();
        //		m_main_pdfs["NS"] = m_pdfs.back();
        //		m_main_pdfs["NOFF"] = m_pdfs.back();
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_bkg_continuum_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor: public FF_PDF_bkg_continuum {
    public:
	virtual ~FF_PDF_bkg_continuum_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor() { }
    
	FF_PDF_bkg_continuum_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor(RooRealVar& mbc, RooRealVar& deltae, RooRealVar& nb_prime, RooRealVar& mass_AB, RooRealVar& mass_BC, RooCategory& flavor,
                                                                       const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_continuum("FF_PDF_bkg_continuum_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_1D_nb_prime>(nb_prime,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_continuum_2D_dalitz>(mass_AB,mass_BC,cat));
        
		m_pdfs.push_back(new RooCategoryPdf(("C_"+m_label+"_flavor").c_str(),("C_"+m_label+"_flavor").c_str(),flavor));
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
                                        RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
                                                   m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),
                                                   *m_pdfs.back())));
		m_main_pdfs["C"] = m_pdfs.back();
		m_main_pdfs["CS"] = m_pdfs.back();
		m_main_pdfs["COFF"] = m_pdfs.back();
        
        //		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
        //				RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
        //						m_comb_pdfs.at(2)->GetPDF("N"),m_comb_pdfs.at(3)->GetPDF("N"),
        //						m_comb_pdfs.at(4)->GetPDF("N"),m_comb_pdfs.at(5)->GetPDF("N"),
        //						m_comb_pdfs.at(6)->GetPDF("N"))));
        //		m_main_pdfs["N"] = m_pdfs.back();
        //		m_main_pdfs["NS"] = m_pdfs.back();
        //		m_main_pdfs["NOFF"] = m_pdfs.back();
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


#endif /* FF_DF_BKG_CONTINUUM_H_ */
