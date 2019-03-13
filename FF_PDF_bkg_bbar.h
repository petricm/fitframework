/**
 * @file	FF_PDF_bkg_bbar.h
 * @date	Nov 18, 2011
 * @author	petric
 * @brief	brief description
 *
 * long description
 */

#ifndef FF_PDF_BKG_BBAR_H_
#define FF_PDF_BKG_BBAR_H_

#include "FF_PDF.h"

class FF_PDF_bkg_bbar : public FF_PDF {
public:
	virtual ~FF_PDF_bkg_bbar() { }
protected:
	FF_PDF_bkg_bbar(const std::string &label, const FFUtil::tuple_category_name_flags& cat) : FF_PDF(label) {
		m_category_name_flags = cat;
		m_pdf_color = kCyan-5;
		m_pdf_linestyle = kSolid;
		m_pdf_legend_label = "Total B^{#pm}/B^{0} bkg";
	}
};

class FF_PDF_bkg_bbar_combinatoric : public FF_PDF {
public:
	virtual ~FF_PDF_bkg_bbar_combinatoric() { }
protected:
	FF_PDF_bkg_bbar_combinatoric(const std::string &label, const FFUtil::tuple_category_name_flags& cat) : FF_PDF(label) {
		m_category_name_flags = cat;
		m_pdf_color = kSpring+5;
		m_pdf_linestyle = kDashed;
		m_pdf_legend_label = "Combinatoric";
	}
};

class FF_PDF_bkg_bbar_wrongmasshyp : public FF_PDF {
public:
	virtual ~FF_PDF_bkg_bbar_wrongmasshyp() { }
protected:
	FF_PDF_bkg_bbar_wrongmasshyp(const std::string &label, const FFUtil::tuple_category_name_flags& cat) : FF_PDF(label) {
		m_category_name_flags = cat;
		m_pdf_color = kTeal+10;
		m_pdf_linestyle = kDashDotted;
		m_pdf_legend_label = "Wrong K/#pi hyp.";
	}
};

class FF_PDF_bkg_bbar_wrongpi0hyp : public FF_PDF {
public:
	virtual ~FF_PDF_bkg_bbar_wrongpi0hyp() { }
protected:
	FF_PDF_bkg_bbar_wrongpi0hyp(const std::string &label, const FFUtil::tuple_category_name_flags& cat) : FF_PDF(label) {
		m_category_name_flags = cat;
		m_pdf_color = kViolet-5;
		m_pdf_linestyle = kDashDotted;
		m_pdf_legend_label = "Wrong #pi^{0} hyp.";
	}
};



class FF_PDF_bkg_bbar_missingfsp : public FF_PDF {
public:
	virtual ~FF_PDF_bkg_bbar_missingfsp() { }
protected:
	FF_PDF_bkg_bbar_missingfsp(const std::string &label, const FFUtil::tuple_category_name_flags& cat) : FF_PDF(label) {
		m_category_name_flags = cat;
		m_pdf_color = kViolet+4;
		m_pdf_linestyle = kDashDotted;
		m_pdf_legend_label = "B^{#pm}/B^{0} #rightarrow c/s";
	}
};


class FF_PDF_bkg_bbar_nonres : public FF_PDF {
public:
	virtual ~FF_PDF_bkg_bbar_nonres() { }
protected:
	FF_PDF_bkg_bbar_nonres(const std::string &label, const FFUtil::tuple_category_name_flags& cat) : FF_PDF(label) {
		m_category_name_flags = cat;
		m_pdf_color = kOrange;
		m_pdf_linestyle = kDashDotted;
		m_pdf_legend_label = "Non-interfering";
	}
};

class FF_PDF_bkg_bbar_wrongphikkbar : public FF_PDF {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphikkbar() { }
protected:
	FF_PDF_bkg_bbar_wrongphikkbar(const std::string &label, const FFUtil::tuple_category_name_flags& cat) : FF_PDF(label) {
		m_category_name_flags = cat;
		m_pdf_color = kViolet-5;
		m_pdf_linestyle = kDashDotted;
		m_pdf_legend_label = "K^{+}K^{-} non-res.";
	}
};

class FF_PDF_bkg_bbar_wrongphif0980 : public FF_PDF {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphif0980() { }
protected:
	FF_PDF_bkg_bbar_wrongphif0980(const std::string &label, const FFUtil::tuple_category_name_flags& cat) : FF_PDF(label) {
		m_category_name_flags = cat;
		m_pdf_color = kViolet;
		m_pdf_linestyle = kDashDotted;
		//m_pdf_legend_label = "K^{+}K^{-} f_{#lower[-0.2]{0}}/^{}a_{#lower[-0.2]{0}}(980)";
		m_pdf_legend_label = "K^{+}K^{-} S-wave";
	}
};


class FF_PDF_bkg_bbar_combinatoric_1D_mbc : public FF_PDF_bkg_bbar_combinatoric {
public:
	virtual ~FF_PDF_bkg_bbar_combinatoric_1D_mbc() { }

	FF_PDF_bkg_bbar_combinatoric_1D_mbc(RooRealVar& mbc, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_combinatoric("pdf_bkg_bbar_combinatoric_1D_mbc",cat) {
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_combinatoric_m0","C mbc bkg_bbar_combinatoric m0",5.2887630113158011,5.288,5.29));
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_combinatoric_c","C mbc bkg_bbar_combinatoric c",-28.891065010970667,-35.0,-10.0));
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_combinatoric_p","C mbc bkg_bbar_combinatoric p",0.5));
        
        m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_combinatoric_mean","C mbc bkg bbar combinatoric mean",5.2801796542275028,5.27,5.288));//V1
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_combinatoric_width","C mbc bkg bbar combinatoric width",0.004890876062416484,0.0,0.01));//V4
        m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_combinatoric_tail","C mbc bkg bbar combinatoric tail",0.18580388361915157));//V7
        
		m_pdfs.push_back(new RooArgusBG(("C_"+m_label+"ARG").c_str(),("C "+m_label+"ARG").c_str(),mbc,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2)));
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"N").c_str(),("C_"+m_label+"N").c_str(),mbc,*m_vars.at(3),*m_vars.at(4),*m_vars.at(5)));

        
        m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_combinatoric_frac_1st","C mbc bkg bbar combinatoric fraction 1st",0.91519695862254613,0.0,1.0));//V9
        m_vars.back()->setConstant(true);
		m_pdfs.push_back(new RooAddPdf((""+m_label+"").c_str(),(""+m_label+"").c_str(),RooArgList(*m_pdfs.at(0),*m_pdfs.at(1)),RooArgList(*m_vars.at(6))));
        
		m_main_pdfs["C"] = m_pdfs.at(2);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongmasshyp_1D_mbc : public FF_PDF_bkg_bbar_wrongmasshyp {
    public:
	virtual ~FF_PDF_bkg_bbar_wrongmasshyp_1D_mbc() { }
    
	FF_PDF_bkg_bbar_wrongmasshyp_1D_mbc(RooRealVar& mbc, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_wrongmasshyp("pdf_bkg_bbar_wrongmasshyp_1D_mbc",cat) {
        
        m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_wrongmasshyp_mean","C mbc bkg bbar wrongmasshyp mean",5.2805164789916326,5.25,5.288));//V0
        //m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_wrongmasshyp_mean_1st_off","C mbc bkg bbar wrongmasshyp mean 1st off",-0.00196541734982425,-5.0,5.0));//V1
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_wrongmasshyp_mean_2nd_off","C mbc bkg bbar wrongmasshyp mean 2nd off",-0.0001906111404705868,-5.0,5.0));//V2
        m_vars.back()->setConstant(true);
        
        m_functions.push_back(new RooFormulaVar("C_mbc_bkg_bbar_wrongmasshyp_mean_1st","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(1)))); //F0
        m_functions.push_back(new RooFormulaVar("C_mbc_bkg_bbar_wrongmasshyp_mean_2nd","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(2)))); //F1
        
        
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_wrongmasshyp_width","C mbc bkg bbar wrongmasshyp width",0.0039093505546941576,0,0.1));//V3
        //m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_wrongmasshyp_width_1st_off","C mbc bkg bbar wrongmasshyp width 1st off",0.99895473617189445,-5.0,5.0));//V4
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_wrongmasshyp_width_2nd_off","C mbc bkg bbar wrongmasshyp width 2nd off",0.88034498307901021,-5.0,5.0));//V5
        m_vars.back()->setConstant(true);
        
        m_functions.push_back(new RooFormulaVar("C_mbc_bkg_bbar_wrongmasshyp_width_1st","@0*@1",RooArgList(*m_vars.at(3),*m_vars.at(4)))); //F2
        m_functions.push_back(new RooFormulaVar("C_mbc_bkg_bbar_wrongmasshyp_width_2nd","@0*@1",RooArgList(*m_vars.at(3),*m_vars.at(5)))); //F3
        
        
        m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_wrongmasshyp_tail","C mbc bkg bbar wrongmasshyp tail",0.30280727072457797,0.0,0.5));//V6
		//m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_wrongmasshyp_tail_1st_off","C mbc bkg bbar wrongmasshyp tail 1st off",2.0375472891318678,-5.0,5.0));//V7
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_wrongmasshyp_tail_2nd_off","C mbc bkg bbar wrongmasshyp tail 2nd off",0.54777339924976243,-5.0,5.0));//V8
        m_vars.back()->setConstant(true);
        
        m_functions.push_back(new RooFormulaVar("C_mbc_bkg_bbar_wrongmasshyp_tail_1st","@0*@1",RooArgList(*m_vars.at(6),*m_vars.at(7)))); //F7
        m_functions.push_back(new RooFormulaVar("C_mbc_bkg_bbar_wrongmasshyp_tail_2nd","@0*@1",RooArgList(*m_vars.at(6),*m_vars.at(8)))); //F8
        
        
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_1st").c_str(),("C_"+m_label+"_1st").c_str(),mbc,*m_functions.at(0),*m_functions.at(2),*m_functions.at(4)));
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_2nd").c_str(),("C_"+m_label+"_2nd").c_str(),mbc,*m_functions.at(1),*m_functions.at(3),*m_functions.at(5)));
        
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_wrongmasshyp_frac_1st","C mbc bkg bbar wrongmasshyp fraction 1st",0.14289613632721065,0.0,1.0));//V9
        m_vars.back()->setConstant(true);
        
		m_pdfs.push_back(new RooAddPdf((""+m_label+"").c_str(),(""+m_label+"").c_str(),RooArgList(*m_pdfs.at(0),*m_pdfs.at(1)),RooArgList(*m_vars.at(9))));
        
		m_main_pdfs["C"] = m_pdfs.at(2);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_bkg_bbar_wrongpi0hyp_1D_mbc : public FF_PDF_bkg_bbar_wrongpi0hyp {
public:
	virtual ~FF_PDF_bkg_bbar_wrongpi0hyp_1D_mbc() { }
    
	FF_PDF_bkg_bbar_wrongpi0hyp_1D_mbc(RooRealVar& mbc, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_wrongpi0hyp("pdf_bkg_bbar_wrongpi0hyp_1D_mbc",cat) {
        
        
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_wrongpi0hyp_mean_1st","C mbc bkg bbar wrongpi0hyp mean 1st",5.2802190888551896,5.22,5.288));//V1
        m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_wrongpi0hyp_width_1st","C mbc bkg bbar wrongpi0hyp width 1st",0.0042631137221904591,0.0,5.0));//V4
        m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_wrongpi0hyp_tail_1st","C mbc bkg bbar wrongpi0hyp tail 1st",0.32981114834931358));//V7
        
        m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_wrongpi0hyp_a1","C mbc bkg_bbar_wrongpi0hyp a1",-0.021605877355949099,-1,1));
        m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_wrongpi0hyp_a2","C mbc bkg_bbar_wrongpi0hyp a2",-0.18706292975825267));
        
        
        
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_N1").c_str(),("C_"+m_label+" N1").c_str(),mbc,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2)));
		m_pdfs.push_back(new RooChebychev(("C_"+m_label+"_C1").c_str(),("C_"+m_label+" C1").c_str(),mbc,RooArgList(*m_vars.at(3),*m_vars.at(4))));
        
        m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_wrongpi0hyp_frac_1st","C mbc bkg bbar wrongpi0hyp fraction 1st",0.4593872016793254,0.0,1.0));//V9
		m_vars.back()->setConstant(true);
        
		m_pdfs.push_back(new RooAddPdf((""+m_label+"").c_str(),(""+m_label+"").c_str(),RooArgList(*m_pdfs.at(0),*m_pdfs.at(1)),RooArgList(*m_vars.at(5))));
        
		m_main_pdfs["C"] = m_pdfs.at(2);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_bkg_bbar_missingfsp_1D_mbc : public FF_PDF_bkg_bbar_missingfsp {
public:
	virtual ~FF_PDF_bkg_bbar_missingfsp_1D_mbc() { }
    
	FF_PDF_bkg_bbar_missingfsp_1D_mbc(RooRealVar& mbc, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_missingfsp("pdf_bkg_bbar_missingfsp_1D_mbc",cat) {
        
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_missingfsp_mean","C mbc bkg bbar missingfsp mean",5.2781330942805971,5.25,5.288));//V0
        //m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_missingfsp_mean_1st_off","C mbc bkg bbar missingfsp mean 1st off",0.0043758912439066933,-5.0,5.0));//V1
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_missingfsp_mean_2nd_off","C mbc bkg bbar missingfsp mean 2nd off",-0.0044952318209308118,-5.0,5.0));//V2
		m_vars.back()->setConstant(true);
        
        m_functions.push_back(new RooFormulaVar("C_mbc_bkg_bbar_missingfsp_mean_1st","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(1)))); //F0
        m_functions.push_back(new RooFormulaVar("C_mbc_bkg_bbar_missingfsp_mean_2nd","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(2)))); //F1
        
        
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_missingfsp_width","C mbc bkg bbar missingfsp width",0.00538214072947,0,0.1));//V3
//        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_missingfsp_width_1st_off","C mbc bkg bbar missingfsp width 1st off",1.0167004206813743,-5.0,5.0));//V4
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_missingfsp_width_2nd_off","C mbc bkg bbar missingfsp width 2nd off",1.0154263044376943,-5.0,5.0));//V5
		m_vars.back()->setConstant(true);
        
        m_functions.push_back(new RooFormulaVar("C_mbc_bkg_bbar_missingfsp_width_1st","@0*@1",RooArgList(*m_vars.at(3),*m_vars.at(4)))); //F2
        m_functions.push_back(new RooFormulaVar("C_mbc_bkg_bbar_missingfsp_width_2nd","@0*@1",RooArgList(*m_vars.at(3),*m_vars.at(5)))); //F3
        
        
        m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_missingfsp_tail","C mbc bkg bbar missingfsp tail",0.38951239931441,0.0,0.5));//V6
//		m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_missingfsp_tail_1st_off","C mbc bkg bbar missingfsp tail 1st off",1.7014034093701556,-5.0,5.0));//V7
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_missingfsp_tail_2nd_off","C mbc bkg bbar missingfsp tail 2nd off",0.41026863193844143,-5.0,5.0));//V8
		m_vars.back()->setConstant(true);
        
        m_functions.push_back(new RooFormulaVar("C_mbc_bkg_bbar_missingfsp_tail_1st","@0*@1",RooArgList(*m_vars.at(6),*m_vars.at(7)))); //F7
        m_functions.push_back(new RooFormulaVar("C_mbc_bkg_bbar_missingfsp_tail_2nd","@0*@1",RooArgList(*m_vars.at(6),*m_vars.at(8)))); //F8
        
        
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_1st").c_str(),("C_"+m_label+"_1st").c_str(),mbc,*m_functions.at(0),*m_functions.at(2),*m_functions.at(4)));
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_2nd").c_str(),("C_"+m_label+"_2nd").c_str(),mbc,*m_functions.at(1),*m_functions.at(3),*m_functions.at(5)));
        
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_missingfsp_frac_1st","C mbc bkg bbar missingfsp fraction 1st",0.68646129998530814,0.0,1.0));//V9
		m_vars.back()->setConstant(true);
        
		m_pdfs.push_back(new RooAddPdf((""+m_label+"").c_str(),(""+m_label+"").c_str(),RooArgList(*m_pdfs.at(0),*m_pdfs.at(1)),RooArgList(*m_vars.at(9))));
        
		m_main_pdfs["C"] = m_pdfs.at(2);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_nonres_1D_mbc : public FF_PDF_bkg_bbar_nonres {
public:
	virtual ~FF_PDF_bkg_bbar_nonres_1D_mbc() { }
    
	FF_PDF_bkg_bbar_nonres_1D_mbc(RooRealVar& mbc, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_nonres("pdf_bkg_bbar_nonres_1D_mbc",cat) {
        
        m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_nonres_mean_1st","C mbc bkg bbar nonres mean 1st",5.2789879651191729,5.26,5.29));
        //m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_nonres_width_1st","C mbc bkg bbar nonres width 1st",0.0022214685748740636,0.000,0.1));
        //m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_nonres_tail_1st","C mbc bkg bbar nonres tail 1st",0.097964651077363962,-1,1));
        m_vars.back()->setConstant(true);
        
        
        m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_nonres_mean_2st","C mbc bkg bbar nonres mean 2st",5.2815215244156137,5.26,5.29));
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_nonres_width_2st","C mbc bkg bbar nonres width 2st",0.0024133025849108814,0.000,0.1));
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_nonres_tail_2st","C mbc bkg bbar nonres tail 2st",0.28320362144767453,-1,1));
        m_vars.back()->setConstant(true);
        
        
        m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_nonres_mean_3st","C mbc bkg bbar nonres mean 3st",5.2849605380995426,5.26,5.29));
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_nonres_width_3st","C mbc bkg bbar nonres width 3st",0.0030363065524722124,0.000,0.1));
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_nonres_tail_3st","C mbc bkg bbar nonres tail 3st",0.86380189119727713,-1,1));
        m_vars.back()->setConstant(true);
        
		m_pdfs.push_back(new RooNovosibirsk((""+m_label+"_N1").c_str(),(""+m_label+"_N1").c_str(),mbc,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2)));
		m_pdfs.push_back(new RooNovosibirsk((""+m_label+"_N2").c_str(),(""+m_label+"_N2").c_str(),mbc,*m_vars.at(3),*m_vars.at(4),*m_vars.at(5)));
		m_pdfs.push_back(new RooNovosibirsk((""+m_label+"_N3").c_str(),(""+m_label+"_N3").c_str(),mbc,*m_vars.at(6),*m_vars.at(7),*m_vars.at(8)));
        
        m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_nonres_frac_1","C mbc bkg bbar nonres frac 1",0.6206951344197269,0.0,1.0));
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_nonres_frac_2","C mbc bkg bbar nonres frac 2",0.32914510899040234,0.0,1.0));
        m_vars.back()->setConstant(true);
        
		m_pdfs.push_back(new RooAddPdf((""+m_label+"").c_str(),(""+m_label+"").c_str(),RooArgList(*m_pdfs.at(0),*m_pdfs.at(1),*m_pdfs.at(2)),RooArgList(*m_vars.at(9),*m_vars.at(10))));
        
		m_main_pdfs["C"] = m_pdfs.at(3);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};



class FF_PDF_bkg_bbar_wrongphikkbar_1D_mbc : public FF_PDF_bkg_bbar_wrongphikkbar {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphikkbar_1D_mbc() { }

	FF_PDF_bkg_bbar_wrongphikkbar_1D_mbc(RooRealVar& mbc, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_wrongphikkbar("pdf_bkg_bbar_wrongphikkbar_1D_mbc",cat) {
		// Gaussian + Gaussian
		m_vars.push_back(new RooRealVar("mbc_bkg_bbar_wrongphikkbar_mean","mbc bkg_bbar_wrongphikkbar mean",5.28,5.27,5.29));
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_wrongphikkbar_width_one","C_mbc bkg_bbar_wrongphikkbar width one",0.0025,0.002,0.003));
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_wrongphikkbar_width_two","C_mbc bkg_bbar_wrongphikkbar width two",0.0035,0.003,0.02));
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_wrongphikkbar_one_frac","C_mbc bkg_bbar_wrongphikkbar one fraction",0.99,0.9,1.0));
		m_vars.back()->setConstant(true);
		m_pdfs.push_back(new RooGaussian(("C_"+m_label+"_G1").c_str(),("C_"+m_label+"_G1").c_str(),mbc,*m_vars.at(0),*m_vars.at(1)));
		m_pdfs.push_back(new RooGaussian(("C_"+m_label+"_G2").c_str(),("C_"+m_label+"_G2").c_str(),mbc,*m_vars.at(0),*m_vars.at(2)));
		m_pdfs.push_back(new RooAddPdf(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),*m_pdfs.at(0),*m_pdfs.at(1),*m_vars.at(3)));
		// Crystal Ball
		m_vars.push_back(new RooRealVar("N_mbc_bkg_bbar_wrongphikkbar_width","N_mbc bkg_bbar_wrongphikkbar width",0.0035,0.001,0.02));
		m_vars.push_back(new RooRealVar("N_mbc_bkg_bbar_wrongphikkbar_alpha","N_mbc bkg_bbar_wrongphikkbar alpha",2.0,0.0,10.0));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("N_mbc_bkg_bbar_wrongphikkbar_n","N_mbc bkg_bbar_wrongphikkbar n",5.0,1.0,10.0));
		m_vars.back()->setConstant(true);
		m_pdfs.push_back(new RooCBShape(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),mbc,*m_vars.at(0),*m_vars.at(4),*m_vars.at(5),*m_vars.at(6)));

		m_main_pdfs["C"] = m_pdfs.at(2);
		m_main_pdfs["N"] = m_pdfs.at(3);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongphif0980_1D_mbc : public FF_PDF_bkg_bbar_wrongphif0980 {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphif0980_1D_mbc() { }

	FF_PDF_bkg_bbar_wrongphif0980_1D_mbc(RooRealVar& mbc, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_wrongphif0980("pdf_bkg_bbar_wrongphif0980_1D_mbc",cat) {
		// Gaussian + Gaussian
		m_vars.push_back(new RooRealVar("mbc_bkg_bbar_wrongphif0980_mean","mbc bkg_bbar_wrongphif0980 mean",5.28,5.27,5.29));
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_wrongphif0980_width_one","C_mbc bkg_bbar_wrongphif0980 width one",0.0025,0.002,0.003));
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_wrongphif0980_width_two","C_mbc bkg_bbar_wrongphif0980 width two",0.0035,0.003,0.02));
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_wrongphif0980_one_frac","C_mbc bkg_bbar_wrongphif0980 one fraction",0.99,0.9,1.0));
		m_vars.back()->setConstant(true);
		m_pdfs.push_back(new RooGaussian(("C_"+m_label+"_G1").c_str(),("C_"+m_label+"_G1").c_str(),mbc,*m_vars.at(0),*m_vars.at(1)));
		m_pdfs.push_back(new RooGaussian(("C_"+m_label+"_G2").c_str(),("C_"+m_label+"_G2").c_str(),mbc,*m_vars.at(0),*m_vars.at(2)));
		m_pdfs.push_back(new RooAddPdf(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),*m_pdfs.at(0),*m_pdfs.at(1),*m_vars.at(3)));
		// Crystal Ball
		m_vars.push_back(new RooRealVar("N_mbc_bkg_bbar_wrongphif0980_width","N_mbc bkg_bbar_wrongphif0980 width",0.0035,0.001,0.02));
		m_vars.push_back(new RooRealVar("N_mbc_bkg_bbar_wrongphif0980_alpha","N_mbc bkg_bbar_wrongphif0980 alpha",2.0,0.0,10.0));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("N_mbc_bkg_bbar_wrongphif0980_n","N_mbc bkg_bbar_wrongphif0980 n",5.0,1.0,10.0));
		m_vars.back()->setConstant(true);
		m_pdfs.push_back(new RooCBShape(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),mbc,*m_vars.at(0),*m_vars.at(4),*m_vars.at(5),*m_vars.at(6)));

		m_main_pdfs["C"] = m_pdfs.at(2);
		m_main_pdfs["N"] = m_pdfs.at(3);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_1D_mbc : public FF_PDF_bkg_bbar {
public:
	virtual ~FF_PDF_bkg_bbar_1D_mbc() { }

	FF_PDF_bkg_bbar_1D_mbc(shared_ptr<FF_PDF> combinatoric, shared_ptr<FF_PDF> wrongmasshyp,shared_ptr<FF_PDF> wrongpi0hyp,
                           shared_ptr<FF_PDF> missingfsp, shared_ptr<FF_PDF> nonres,double nevents,
			const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar("pdf_bkg_bbar_1D_mbc",cat) {
		m_comb_pdfs.push_back(combinatoric);
		m_comb_pdfs.push_back(wrongmasshyp);
		m_comb_pdfs.push_back(wrongpi0hyp);
		m_comb_pdfs.push_back(missingfsp);
		m_comb_pdfs.push_back(nonres);
        
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_combinatoric_nevents","C_mbc_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_wrongmasshyp_nevents","C_mbc_bkg_bbar_wrongmasshyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_wrongpi0hyp_nevents","C_mbc_bkg_bbar_wrongpi0hyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_missingfsp_nevents","C_mbc_bkg_bbar_missingfsp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_nonres_nevents","C_mbc_bkg_bbar_nonres_nevents",nevents/7.0,0.0,nevents*1.2));
        
		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(
                                                                                                m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
                                                                                                m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),m_comb_pdfs.at(4)->GetPDF("C")),
                                       RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4)),false));

		m_main_pdfs["C"] = m_pdfs.at(0);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}

	FF_PDF_bkg_bbar_1D_mbc(RooRealVar& mbc, double nevents, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar("pdf_bkg_bbar_1D_mbc",cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_missingfsp_1D_mbc>(mbc,cat));
        m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_nonres_1D_mbc>(mbc,cat));

		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_combinatoric_nevents","C_mbc_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_wrongmasshyp_nevents","C_mbc_bkg_bbar_wrongmasshyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_wrongpi0hyp_nevents","C_mbc_bkg_bbar_wrongpi0hyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_missingfsp_nevents","C_mbc_bkg_bbar_missingfsp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_mbc_bkg_bbar_nonres_nevents","C_mbc_bkg_bbar_nonres_nevents",nevents/7.0,0.0,nevents*1.2));
        
		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(
                                                                                                m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
                                                                                                m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),m_comb_pdfs.at(4)->GetPDF("C")),
                                       RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4)),false));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CP"] = m_pdfs.at(0);
		m_main_pdfs["CA"] = m_pdfs.at(0);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};



class FF_PDF_bkg_bbar_combinatoric_1D_deltae : public FF_PDF_bkg_bbar_combinatoric {
public:
	virtual ~FF_PDF_bkg_bbar_combinatoric_1D_deltae() { }

	FF_PDF_bkg_bbar_combinatoric_1D_deltae(RooRealVar& deltae, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_combinatoric("pdf_bkg_bbar_combinatoric_1D_deltae",cat) {
        
		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_combinatoric_a1","C_deltae_bkg_bbar_combinatoric a1",-0.66387119327756183,-2.0,2.0));
		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_combinatoric_a2","C_deltae_bkg_bbar_combinatoric a2",0.12436915695469719,-1,1));
        //		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_combinatoric_a3","C_deltae_bkg_bbar_combinatoric a3",-0.023150173347434105));
        
		m_pdfs.push_back(new RooChebychev(("C_"+m_label).c_str(),("C_"+m_label).c_str(),deltae,RooArgList(*m_vars.at(0),*m_vars.at(1))));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongmasshyp_1D_deltae : public FF_PDF_bkg_bbar_wrongmasshyp {
public:
	virtual ~FF_PDF_bkg_bbar_wrongmasshyp_1D_deltae() { }

	FF_PDF_bkg_bbar_wrongmasshyp_1D_deltae(RooRealVar& deltae, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_wrongmasshyp("pdf_bkg_bbar_wrongmasshyp_1D_deltae",cat) {
		
        m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongmasshyp_mean_1st","C deltae bkg bbar wrongmasshyp mean 1st",0.051505059005518217,-0.2,0.2));//V0
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongmasshyp_width_1st","C deltae bkg bbar wrongmasshyp width 1st",0.026152431389137643,0,1.0));//V1
		m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongmasshyp_tail_1st","C deltae bkg bbar wrongmasshyp tail 1st",0.048814918888331116,-1,1));//V2
  		m_vars.back()->setConstant(true);
        
        m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongmasshyp_mean_2nd","C deltae bkg bbar wrongmasshyp mean 2nd",0.027453976032302346,-0.2,0.2));//V3
		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongmasshyp_width_2nd","C deltae bkg bbar wrongmasshyp width 2nd",0.095753911592261931,0,1.0));//V4
        m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongmasshyp_tail_2nd","C deltae bkg bbar wrongmasshyp tail 2nd",0.25871987309270761,-5,5));//V5
		m_vars.back()->setConstant(true);
        
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_1st").c_str(),("C_"+m_label+"_1st").c_str(),deltae,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2)));
  		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_2nd").c_str(),("C_"+m_label+" 2nd").c_str(),deltae,*m_vars.at(3),*m_vars.at(4),*m_vars.at(5)));
        
		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongmasshyp_frac_1st","C deltae bkg bbar wrongmasshyp fraction 1st",0.33826625864668058,0.0,1.0));//V6
		m_vars.back()->setConstant(true);
        
		m_pdfs.push_back(new RooAddPdf((""+m_label+"").c_str(),(""+m_label+"").c_str(),RooArgList(*m_pdfs.at(0),*m_pdfs.at(1)),RooArgList(*m_vars.at(6))));
        
		m_main_pdfs["C"] = m_pdfs.at(2);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_bkg_bbar_wrongpi0hyp_1D_deltae : public FF_PDF_bkg_bbar_wrongpi0hyp {
public:
	virtual ~FF_PDF_bkg_bbar_wrongpi0hyp_1D_deltae() { }
    
	FF_PDF_bkg_bbar_wrongpi0hyp_1D_deltae(RooRealVar& deltae, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_wrongpi0hyp("pdf_bkg_bbar_wrongpi0hyp_1D_deltae",cat) {
        
        m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongpi0hyp_mean","C deltae bkg bbar wrongpi0hyp mean",0.0215017560814732,-0.2,0.2));//V0
		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongpi0hyp_width","C deltae bkg bbar wrongpi0hyp width",0.074182808524071331,0,1.0));//V1
        m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongpi0hyp_tail","C deltae bkg bbar wrongpi0hyp tail",-0.31058093483854476,-1,1));//V2
		m_vars.back()->setConstant(true);
        
        m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongpi0hyp_a1","C deltae bkg bbar wrongpi0hyp a1",-1.0306763888170138,-2.0,2.0)); //V3
		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongpi0hyp_a2","C deltae bkg bbar wrongpi0hyp a2",0.1794334669708828)); //V4
        
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_1st").c_str(),("C_"+m_label+"_1st").c_str(),deltae,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2)));
		m_pdfs.push_back(new RooChebychev(("C_"+m_label).c_str(),("C_"+m_label).c_str(),deltae,RooArgList(*m_vars.at(3),*m_vars.at(4))));
        
        m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongpi0hyp_frac_1st","C deltae bkg bbar wrongpi0hyp fraction 1st",0.17494672463850941,0.0,1.0));//V5
		m_vars.back()->setConstant(true);
        
		m_pdfs.push_back(new RooAddPdf((""+m_label+"").c_str(),(""+m_label+"").c_str(),RooArgList(*m_pdfs.at(0),*m_pdfs.at(1)),RooArgList(*m_vars.at(5))));
        
		m_main_pdfs["C"] = m_pdfs.at(2);
        
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_bkg_bbar_missingfsp_1D_deltae : public FF_PDF_bkg_bbar_missingfsp {
public:
	virtual ~FF_PDF_bkg_bbar_missingfsp_1D_deltae() { }
    
	FF_PDF_bkg_bbar_missingfsp_1D_deltae(RooRealVar& deltae, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_missingfsp("pdf_bkg_bbar_missingfsp_1D_deltae",cat) {
        
		m_vars.push_back(new RooRealVar("C_deltae_bkg_missingfsp_mean","C deltae bkg missingfsp mean",-0.2045,-0.5,-0.1));//V0
        //m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_deltae_bkg_missingfsp_mean_1st_off","C deltae bkg missingfsp mean 1st off",0.056866216118135693,-1.0,1.0));//V1
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_deltae_bkg_missingfsp_mean_2nd_off","C deltae bkg missingfsp mean 2nd off",-0.0081841108660762885,-1.0,1.0));//V2
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_deltae_bkg_missingfsp_mean_3rd_off","C deltae bkg missingfsp mean 3rd off",0.026217855876494278,-1.0,1.0));//V3
		m_vars.back()->setConstant(true);
        
        m_functions.push_back(new RooFormulaVar("C_deltae_bkg_missingfsp_mean_1st","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(1)))); //F0
        m_functions.push_back(new RooFormulaVar("C_deltae_bkg_missingfsp_mean_2nd","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(2)))); //F1
        m_functions.push_back(new RooFormulaVar("C_deltae_bkg_missingfsp_mean_3rd","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(3)))); //F2
        
        
		m_vars.push_back(new RooRealVar("C_deltae_bkg_missingfsp_width","C deltae bkg missingfsp width",1.8572e-02,-5.0,5.0));//V4
        //m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("C_deltae_bkg_missingfsp_width_1st_off","C deltae bkg missingfsp width 1st off",0.96520751475706845,-50.0,50.0));//V5
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_deltae_bkg_missingfsp_width_2nd_off","C deltae bkg missingfsp width 2nd off",1.2441334112283116,-50.0,50.0));//V6
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_deltae_bkg_missingfsp_width_3rd_off","C deltae bkg missingfsp width 3rd off",1.0546674881339655,-50.0,50.0));//V7
		m_vars.back()->setConstant(true);
        
        m_functions.push_back(new RooFormulaVar("C_deltae_bkg_missingfsp_width_1st","@0*@1",RooArgList(*m_vars.at(4),*m_vars.at(5)))); //F4
        m_functions.push_back(new RooFormulaVar("C_deltae_bkg_missingfsp_width_2nd","@0*@1",RooArgList(*m_vars.at(4),*m_vars.at(6)))); //F5
        m_functions.push_back(new RooFormulaVar("C_deltae_bkg_missingfsp_width_3rd","@0*@1",RooArgList(*m_vars.at(4),*m_vars.at(7)))); //F6
        
        m_vars.push_back(new RooRealVar("C_deltae_bkg_missingfsp_tail","C deltae bkg missingfsp tail",6.82e-01,-1.0,5.0));//V8
		m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("C_deltae_bkg_missingfsp_tail_1st_off","C deltae bkg missingfsp tail 1st off",-0.63484439787252001,-50.0,50.0));//V9
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_deltae_bkg_missingfsp_tail_2nd_off","C deltae bkg missingfsp tail 2nd off",-1.1879486133008825,-50.0,50.0));//V10
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_deltae_bkg_missingfsp_tail_3rd_off","C deltae bkg missingfsp tail 3rd off",0.68196502162611949,-50.0,50.0));//V11
		m_vars.back()->setConstant(true);
        
        m_functions.push_back(new RooFormulaVar("C_deltae_bkg_missingfsp_tail_1st","@0*@1",RooArgList(*m_vars.at(8),*m_vars.at(9)))); //F8
        m_functions.push_back(new RooFormulaVar("C_deltae_bkg_missingfsp_tail_2nd","@0*@1",RooArgList(*m_vars.at(8),*m_vars.at(10)))); //F9
        m_functions.push_back(new RooFormulaVar("C_deltae_bkg_missingfsp_tail_3rd","@0*@1",RooArgList(*m_vars.at(8),*m_vars.at(11)))); //F10
        
        
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_1st").c_str(),("C_"+m_label+"_1st").c_str(),deltae,*m_functions.at(0),*m_functions.at(3),*m_functions.at(6)));
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_2nd").c_str(),("C_"+m_label+"_2nd").c_str(),deltae,*m_functions.at(1),*m_functions.at(4),*m_functions.at(7)));
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_3rd").c_str(),("C_"+m_label+"_3rd").c_str(),deltae,*m_functions.at(2),*m_functions.at(5),*m_functions.at(8)));
	    
		m_vars.push_back(new RooRealVar("C_deltae_bkg_missingfsp_frac_1st","C deltae bkg missingfsp fraction 1st",0.24020101509803538,0.0,1.0));//V12
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_deltae_bkg_missingfsp_frac_2nd","C deltae bkg missingfsp fraction 2nd",0.53788513323098142,0.0,1.0));//V13
        m_vars.back()->setConstant(true);
	    
		m_pdfs.push_back(new RooAddPdf((""+m_label+"").c_str(),(""+m_label+"").c_str(),RooArgList(*m_pdfs.at(0),*m_pdfs.at(1),*m_pdfs.at(2)),RooArgList(*m_vars.at(12),*m_vars.at(13))));
        
		m_main_pdfs["C"] = m_pdfs.at(3);
		m_main_pdfs["CP"] = m_pdfs.at(3);
		m_main_pdfs["CA"] = m_pdfs.at(3);
        
        
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_bkg_bbar_nonres_1D_deltae : public FF_PDF_bkg_bbar_nonres {
public:
	virtual ~FF_PDF_bkg_bbar_nonres_1D_deltae() { }
    
	FF_PDF_bkg_bbar_nonres_1D_deltae(RooRealVar& deltae, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_nonres("pdf_bkg_bbar_nonres_1D_deltae",cat) {
        
        m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_nonres_mean","C deltae signal mean",0,-0.1,0.1)); //V0 "
        //m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_nonres_mean 1st","C deltae signal mean 1st",-0.026842995150635138,-0.1,0.1)); //V1
        m_vars.back()->setConstant(true);
        m_functions.push_back(new RooFormulaVar("C_deltae_bkg_bbar_nonres_mean_off_1","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(1)))); //F0
		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_nonres_width_1st","C deltae signal width 1st",0.038341583569751252,0.000,0.2)); //V2
        m_vars.back()->setConstant(true);
        
        m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_nonres_mean_2nd","C deltae signal mean 2nd",0.0023505862911927522,-0.1,0.1)); //V3
        m_vars.back()->setConstant(true);
        m_functions.push_back(new RooFormulaVar("C_deltae_bkg_bbar_nonres_mean_off_2","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(3)))); //F1
		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_nonres_width_2nd","C deltae signal width 2nd",0.098993024342278579,0.000,0.2)); //V4
        m_vars.back()->setConstant(true);
        
		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_nonres_mean_3rd","C deltae signal mean  3rd",-0.091457307350592071,-0.1,0.1)); //V5
        m_vars.back()->setConstant(true);
        m_functions.push_back(new RooFormulaVar("C_deltae_bkg_bbar_nonres_mean_off_2","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(5)))); //F2
        
		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_nonres_width_3rd","C deltae signal width 3rd",0.029392818007661581,0.000,0.3)); //V6
        //m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_nonres_alpha","C deltae signal alpha",0.57168425540336498,0.0,2.0)); //V9
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_nonres_n","C deltae signal n",4.2653102822995361,0.0,6.0)); //V10
        m_vars.back()->setConstant(true);
        
        m_pdfs.push_back(new RooGaussian((""+m_label+"_G1").c_str(),(""+m_label+" G1").c_str(),deltae,*m_functions.at(0),*m_vars.at(2)));
        m_pdfs.push_back(new RooGaussian((""+m_label+"_G2").c_str(),(""+m_label+" G2").c_str(),deltae,*m_functions.at(1),*m_vars.at(4)));
		m_pdfs.push_back(new RooCBShape((""+m_label+"_CBS").c_str(),(""+m_label+" CBS").c_str(),deltae,*m_functions.at(2),*m_vars.at(6),*m_vars.at(7),*m_vars.at(8)));
        
        m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_nonres_frac_1","C deltae signal frac 1",0.13305714502219262,0.0,1.0)); //V11
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_nonres_frac_2","C deltae signal frac 2",0.030318571698451979,0.0,1.0)); //V12
        m_vars.back()->setConstant(true);
        
		m_pdfs.push_back(new RooAddPdf((""+m_label+"").c_str(),(""+m_label+"").c_str(),RooArgList(*m_pdfs.at(0),*m_pdfs.at(1),*m_pdfs.at(2)),RooArgList(*m_vars.at(9),*m_vars.at(10))));
        
		m_main_pdfs["C"] = m_pdfs.at(3);
		m_main_pdfs["CP"] = m_pdfs.at(3);
		m_main_pdfs["CA"] = m_pdfs.at(3);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};




class FF_PDF_bkg_bbar_wrongphikkbar_1D_deltae : public FF_PDF_bkg_bbar_wrongphikkbar {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphikkbar_1D_deltae() { }

	FF_PDF_bkg_bbar_wrongphikkbar_1D_deltae(RooRealVar& deltae, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_wrongphikkbar("pdf_bkg_bbar_wrongphikkbar_1D_deltae",cat) {
		// Gaussian + Bifur Gaussian + Bifur Gaussian
		m_vars.push_back(new RooRealVar("deltae_bkg_bbar_wrongphikkbar_mean","deltae_bkg_bbar_wrongphikkbar mean",0.0,-0.01,0.01));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongphikkbar_width_1st_left","C_deltae_bkg_bbar_wrongphikkbar width 1st_left",0.005,0.001,0.06));
		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongphikkbar_width_1st_right","C_deltae_bkg_bbar_wrongphikkbar width 1st_right",0.005,0.001,0.06));
		m_pdfs.push_back(new RooBifurGauss(("C_"+m_label+"_1st_gauss").c_str(),("C_"+m_label+"_1st_gauss").c_str(),deltae,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2)));

		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongphikkbar_width_2nd_left","C_deltae_bkg_bbar_wrongphikkbar width 2nd_left",0.06,0.05,0.1));
		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongphikkbar_width_2nd_right","C_deltae_bkg_bbar_wrongphikkbar width 2nd_right",0.06,0.05,0.1));
		m_pdfs.push_back(new RooBifurGauss(("C_"+m_label+"_2nd_gauss").c_str(),("C_"+m_label+"_2nd_gauss").c_str(),deltae,*m_vars.at(0),*m_vars.at(3),*m_vars.at(4)));

		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongphikkbar_width_core","C_deltae_bkg_bbar_wrongphikkbar width_core",0.008,0.001,0.1));
		m_pdfs.push_back(new RooGaussian(("C_"+m_label+"_core_gauss").c_str(),("C_"+m_label+"_core_gauss").c_str(),deltae,*m_vars.at(0),*m_vars.at(5)));

		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongphikkbar_1st_frac","C_deltae_bkg_bbar_wrongphikkbar 1st fraction",0.80,0.0,1.0));
		m_vars.back()->setConstant(true);
		m_pdfs.push_back(new RooAddPdf(("C_"+m_label+"_bi_gauss").c_str(),("C_"+m_label+"_bi_gauss").c_str(),*m_pdfs.at(0),*m_pdfs.at(1),*m_vars.at(6)));

		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongphikkbar_core_frac","C_deltae_bkg_bbar_wrongphikkbar core_frac",0.7,0.0,1.0));
		m_vars.back()->setConstant(true);
		m_pdfs.push_back(new RooAddPdf(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),*m_pdfs.at(2),*m_pdfs.at(3),*m_vars.at(7)));

		// Gaussian + Crystal-Ball
		m_vars.push_back(new RooRealVar("N_deltae_bkg_bbar_wrongphikkbar_width_gauss","N_deltae bkg_bbar_wrongphikkbar width gauss",0.005,0.001,0.06));
		m_pdfs.push_back(new RooGaussian(("N_"+m_label+"_gauss").c_str(),("N_"+m_label+"_gauss").c_str(),deltae,*m_vars.at(0),*m_vars.at(8)));
		m_vars.push_back(new RooRealVar("N_deltae_bkg_bbar_wrongphikkbar_width_crystal","N_deltae bkg_bbar_wrongphikkbar width crystal",0.01,0.001,0.03));
		m_vars.push_back(new RooRealVar("N_deltae_bkg_bbar_wrongphikkbar_alpha","N_deltae bkg_bbar_wrongphikkbar alpha",1.0,0.0,2.0));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("N_deltae_bkg_bbar_wrongphikkbar_n","N_deltae bkg_bbar_wrongphikkbar n",2.5,1.0,4.0));
		m_vars.back()->setConstant(true);
		m_pdfs.push_back(new RooCBShape(("N_"+m_label+"_crystal").c_str(),("N_"+m_label+"_crystal").c_str(),deltae,*m_vars.at(0),*m_vars.at(9),*m_vars.at(10),*m_vars.at(11)));
		m_vars.push_back(new RooRealVar("N_deltae_bkg_bbar_wrongphikkbar_gauss_frac","N_deltae bkg_bbar_wrongphikkbar gauss fraction",0.1,0.0,1.0));
		m_vars.back()->setConstant(true);
		m_pdfs.push_back(new RooAddPdf(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),*m_pdfs.at(5),*m_pdfs.at(6),*m_vars.at(12)));

		m_main_pdfs["C"] = m_pdfs.at(4);
		m_main_pdfs["N"] = m_pdfs.at(7);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongphif0980_1D_deltae : public FF_PDF_bkg_bbar_wrongphif0980 {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphif0980_1D_deltae() { }

	FF_PDF_bkg_bbar_wrongphif0980_1D_deltae(RooRealVar& deltae, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_wrongphif0980("pdf_bkg_bbar_wrongphif0980_1D_deltae",cat) {
		// Gaussian + Bifur Gaussian + Bifur Gaussian
		m_vars.push_back(new RooRealVar("deltae_bkg_bbar_wrongphif0980_mean","deltae bkg_bbar_wrongphif0980 mean",0.0,-0.01,0.01));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongphif0980_width_1st_left","C_deltae bkg_bbar_wrongphif0980 width 1st_left",0.005,0.001,0.06));
		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongphif0980_width_1st_right","C_deltae bkg_bbar_wrongphif0980 width 1st_right",0.005,0.001,0.06));
		m_pdfs.push_back(new RooBifurGauss(("C_"+m_label+"_1st_gauss").c_str(),("C_"+m_label+"_1st_gauss").c_str(),deltae,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2)));

		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongphif0980_width_2nd_left","C_deltae bkg_bbar_wrongphif0980 width 2nd_left",0.06,0.05,0.1));
		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongphif0980_width_2nd_right","C_deltae bkg_bbar_wrongphif0980 width 2nd_right",0.06,0.05,0.1));
		m_pdfs.push_back(new RooBifurGauss(("C_"+m_label+"_2nd_gauss").c_str(),("C_"+m_label+"_2nd_gauss").c_str(),deltae,*m_vars.at(0),*m_vars.at(3),*m_vars.at(4)));

		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongphif0980_width_core","C_deltae bkg_bbar_wrongphif0980 width_core",0.008,0.001,0.1));
		m_pdfs.push_back(new RooGaussian(("C_"+m_label+"_core_gauss").c_str(),("C_"+m_label+"_core_gauss").c_str(),deltae,*m_vars.at(0),*m_vars.at(5)));

		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongphif0980_1st_frac","C_deltae bkg_bbar_wrongphif0980 1st fraction",0.80,0.0,1.0));
		m_vars.back()->setConstant(true);
		m_pdfs.push_back(new RooAddPdf(("C_"+m_label+"_bi_gauss").c_str(),("C_"+m_label+"_bi_gauss").c_str(),*m_pdfs.at(0),*m_pdfs.at(1),*m_vars.at(6)));

		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongphif0980_core_frac","C_deltae bkg_bbar_wrongphif0980 core_frac",0.7,0.0,1.0));
		m_vars.back()->setConstant(true);
		m_pdfs.push_back(new RooAddPdf(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),*m_pdfs.at(2),*m_pdfs.at(3),*m_vars.at(7)));

		// Gaussian + Crystal-Ball
		m_vars.push_back(new RooRealVar("N_deltae_bkg_bbar_wrongphif0980_width_gauss","N_deltae bkg_bbar_wrongphif0980 width gauss",0.005,0.001,0.06));
		m_pdfs.push_back(new RooGaussian(("N_"+m_label+"_gauss").c_str(),("N_"+m_label+"_gauss").c_str(),deltae,*m_vars.at(0),*m_vars.at(8)));
		m_vars.push_back(new RooRealVar("N_deltae_bkg_bbar_wrongphif0980_width_crystal","N_deltae bkg_bbar_wrongphif0980 width crystal",0.01,0.001,0.03));
		m_vars.push_back(new RooRealVar("N_deltae_bkg_bbar_wrongphif0980_alpha","N_deltae bkg_bbar_wrongphif0980 alpha",1.0,0.0,2.0));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("N_deltae_bkg_bbar_wrongphif0980_n","N_deltae bkg_bbar_wrongphif0980 n",2.5,1.0,4.0));
		m_vars.back()->setConstant(true);
		m_pdfs.push_back(new RooCBShape(("N_"+m_label+"_crystal").c_str(),("N_"+m_label+"_crystal").c_str(),deltae,*m_vars.at(0),*m_vars.at(9),*m_vars.at(10),*m_vars.at(11)));
		m_vars.push_back(new RooRealVar("N_deltae_bkg_bbar_wrongphif0980_gauss_frac","N_deltae bkg_bbar_wrongphif0980 gauss fraction",0.1,0.0,1.0));
		m_vars.back()->setConstant(true);
		m_pdfs.push_back(new RooAddPdf(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),*m_pdfs.at(5),*m_pdfs.at(6),*m_vars.at(12)));

		m_main_pdfs["C"] = m_pdfs.at(4);
		m_main_pdfs["N"] = m_pdfs.at(7);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_1D_deltae : public FF_PDF_bkg_bbar {
public:
	virtual ~FF_PDF_bkg_bbar_1D_deltae() { }

	FF_PDF_bkg_bbar_1D_deltae(shared_ptr<FF_PDF> combinatoric, shared_ptr<FF_PDF> wrongmasshyp,
                              shared_ptr<FF_PDF> wrongpi0hyp, shared_ptr<FF_PDF> missingfsp, shared_ptr<FF_PDF> nonres,  double nevents,
                              const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar("pdf_bkg_bbar_1D_deltae",cat) {
		m_comb_pdfs.push_back(combinatoric);
		m_comb_pdfs.push_back(wrongmasshyp);
		m_comb_pdfs.push_back(wrongpi0hyp);
		m_comb_pdfs.push_back(missingfsp);
		m_comb_pdfs.push_back(nonres);
        
		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_combinatoric_nevents","C_deltae_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongmasshyp_nevents","C_deltae_bkg_bbar_wrongmasshyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongpi0hyp_nevents","C_deltae_bkg_bbar_wrongpi0hyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_missingfsp_nevents","C_deltae_bkg_bbar_missingfsp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_nonres_nevents","C_deltae_bkg_bbar_nonres_nevents",nevents/7.0,0.0,nevents*1.2));
        
		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(
                                                                                                m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
                                                                                                m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),m_comb_pdfs.at(4)->GetPDF("C")),
                                       RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4)),false));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CP"] = m_pdfs.at(0);
		m_main_pdfs["CA"] = m_pdfs.at(0);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}

	FF_PDF_bkg_bbar_1D_deltae(RooRealVar& deltae, double nevents, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar("pdf_bkg_bbar_1D_deltae",cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_missingfsp_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_nonres_1D_deltae>(deltae,cat));
        
		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_combinatoric_nevents","C_deltae_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongmasshyp_nevents","C_deltae_bkg_bbar_wrongmasshyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_wrongpi0hyp_nevents","C_deltae_bkg_bbar_wrongpi0hyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_missingfsp_nevents","C_deltae_bkg_bbar_missingfsp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_deltae_bkg_bbar_nonres_nevents","C_deltae_bkg_bbar_nonres_nevents",nevents/7.0,0.0,nevents*1.2));
        
		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(
                                                                                                m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
                                                                                                m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),m_comb_pdfs.at(4)->GetPDF("C")),
                                       RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4)),false));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CP"] = m_pdfs.at(0);
		m_main_pdfs["CA"] = m_pdfs.at(0);


		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};



class FF_PDF_bkg_bbar_combinatoric_1D_chil0_ma : public FF_PDF_bkg_bbar_combinatoric {
public:
	virtual ~FF_PDF_bkg_bbar_combinatoric_1D_chil0_ma() { }

	FF_PDF_bkg_bbar_combinatoric_1D_chil0_ma(RooRealVar& chil0_ma, const FFUtil::tuple_category_name_flags& cat, bool is_data, RooRealVar* resolution) : FF_PDF_bkg_bbar_combinatoric("pdf_bkg_bbar_combinatoric_1D_chil0_ma",cat) {
		// Threshold function
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_combinatoric_a","chil0_ma bkg_bbar_combinatoric a",0.02,0.0,1.0));
		m_pdfs.push_back(new RooTwoBodyThresholdFunction(("C_"+m_label+"_nonres").c_str(),("C_"+m_label+"_nonres").c_str(),chil0_ma,*m_vars.at(0),m_K+m_K));
		m_pdfs.push_back(new RooTwoBodyThresholdFunction(("N_"+m_label+"_nonres").c_str(),("N_"+m_label+"_nonres").c_str(),chil0_ma,*m_vars.at(0),m_K+m_K));

		// Relativistic Spin Breit-Wigner or Two Body PSHP Breit-Wigner
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_combinatoric_mean","chil0_ma bkg_bbar_combinatoric mean",1.019455,1.019435,1.019475));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_combinatoric_width","chil0_ma bkg_bbar_combinatoric width",0.00426,0.00422,0.00430));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_combinatoric_r","chil0_ma bkg_bbar_combinatoric r",3.0));
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_combinatoric_J","chil0_ma bkg_bbar_combinatoric J",1.0));
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
		m_vars.push_back(new RooRealVar("C_chil0_ma_bkg_bbar_combinatoric_nonres_fraction","C_chil0_ma bkg_bbar_combinatoric non-resonant fraction",0.5,0.2,0.8));
		m_vars.push_back(new RooRealVar("N_chil0_ma_bkg_bbar_combinatoric_nonres_fraction","N_chil0_ma bkg_bbar_combinatoric non-resonant fraction",0.5,0.2,0.8));
		m_pdfs.push_back(new RooAddPdf(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),*m_pdfs.at(0),*m_pdfs.at(6),*m_vars.at(resolution == NULL ? 6 : 5)));
		m_pdfs.push_back(new RooAddPdf(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),*m_pdfs.at(1),*m_pdfs.at(7),*m_vars.at(resolution == NULL ? 7 : 6)));

		m_main_pdfs["C"] = m_pdfs.at(8);
		m_main_pdfs["CS"] = m_pdfs.at(8);
		m_main_pdfs["N"] = m_pdfs.at(9);
		m_main_pdfs["NS"] = m_pdfs.at(9);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongmasshyp_1D_chil0_ma : public FF_PDF_bkg_bbar_wrongmasshyp {
public:
	virtual ~FF_PDF_bkg_bbar_wrongmasshyp_1D_chil0_ma() { }

	FF_PDF_bkg_bbar_wrongmasshyp_1D_chil0_ma(RooRealVar& chil0_ma, const FFUtil::tuple_category_name_flags& cat, bool is_data, RooRealVar* resolution) : FF_PDF_bkg_bbar_wrongmasshyp("pdf_bkg_bbar_wrongmasshyp_1D_chil0_ma",cat) {
		// Relativistic Spin Breit-Wigner or Two Body PSHP Breit-Wigner
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongmasshyp_mean","chil0_ma bkg_bbar_wrongmasshyp mean",1.019455,1.019435,1.019475));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongmasshyp_width","chil0_ma bkg_bbar_wrongmasshyp width",0.00426,0.00422,0.00430));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongmasshyp_r","chil0_ma bkg_bbar_wrongmasshyp r",3.0));
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongmasshyp_J","chil0_ma bkg_bbar_wrongmasshyp J",1.0));
		if(is_data) {
			m_pdfs.push_back(new RooRelativisticSpinBreitWigner(("C_"+m_label+"_BW").c_str(),("C_"+m_label+"_BW").c_str(),chil0_ma,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),m_K,m_K));
		} else {
			m_pdfs.push_back(new RooBreitWignerTwoBodyPHSP(("C_"+m_label+"_BW").c_str(),("C_"+m_label+"_BW").c_str(),chil0_ma,*m_vars.at(0),*m_vars.at(1),m_K,m_K));
		}

		// Gaussian resolution
		if(resolution != NULL) {
			m_pdfs.push_back(new RooGaussian(("C_"+m_label+"_resolution").c_str(),("C_"+m_label+"_resolution").c_str(),chil0_ma,RooConst(0.0),*resolution));
		} else {
			m_vars.push_back(new RooRealVar("chil0_ma_resolution","chil0_ma resolution",0.0008,0.0002,0.0014));
			m_pdfs.push_back(new RooGaussian(("C_"+m_label+"_resolution").c_str(),("C_"+m_label+"_resolution").c_str(),chil0_ma,RooConst(0.0),*m_vars.at(4)));
		}

		// Breit-Wigner folded with Gaussian resolution
		m_pdfs.push_back(new RooFFTConvPdf(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),chil0_ma,*m_pdfs.at(0),*m_pdfs.at(1)));
		// some settings for the convolution
		dynamic_cast<RooFFTConvPdf*>(m_pdfs.at(2))->setBufferFraction(1.2);
		chil0_ma.setBins(10000,"cache");

		m_main_pdfs["C"] = m_pdfs.at(2);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongphikkbar_1D_chil0_ma : public FF_PDF_bkg_bbar_wrongphikkbar {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphikkbar_1D_chil0_ma() { }

	FF_PDF_bkg_bbar_wrongphikkbar_1D_chil0_ma(RooRealVar& chil0_ma, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_wrongphikkbar("pdf_bkg_bbar_wrongphikkbar_1D_chil0_ma",cat) {
		// This takes only the most dominant B->KKK*(892) into account
		m_pdfs.push_back(new RooThreeBodyPhaseSpaceProjection(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),chil0_ma,m_K,m_K,m_K892,m_B0));
		m_pdfs.push_back(new RooThreeBodyPhaseSpaceProjection(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),chil0_ma,m_K,m_K,m_K892,m_B0));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongphif0980_1D_chil0_ma : public FF_PDF_bkg_bbar_wrongphif0980 {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphif0980_1D_chil0_ma() { }

	FF_PDF_bkg_bbar_wrongphif0980_1D_chil0_ma(RooRealVar& chil0_ma, const FFUtil::tuple_category_name_flags& cat, bool is_data) : FF_PDF_bkg_bbar_wrongphif0980("pdf_bkg_bbar_wrongphif0980_1D_chil0_ma",cat) {
		if(is_data) {
			// Flatte formalism (see BN 1129 for f0(980) parameters)
			m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongphif0980_pole","chil0_ma bkg_bbar_wrongphif0980 pole",0.965,0.955,0.975));
			m_vars.back()->setConstant(true);
			m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongphif0980_g1","chil0_ma bkg_bbar_wrongphif0980 g1",0.165));
			m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongphif0980_g2","chil0_ma bkg_bbar_wrongphif0980 g2",0.347/std::sqrt(2.0)));
			m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongphif0980_g3","chil0_ma bkg_bbar_wrongphif0980 g3",0.347/std::sqrt(2.0)));

			m_pdfs.push_back(new RooFlatte(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),
					chil0_ma,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),m_Pi,m_Pi,m_K,m_K,m_Ks,m_Ks,m_K,m_K));
			m_pdfs.push_back(new RooFlatte(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),
					chil0_ma,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),m_Pi,m_Pi,m_K,m_K,m_Ks,m_Ks,m_K,m_K));

			m_main_pdfs["C"] = m_pdfs.at(0);
			m_main_pdfs["N"] = m_pdfs.at(1);
		} else {
			// Two Body PSHP Breit-Wigner
			m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongphif0980_mean","chil0_ma bkg_bbar_wrongphif0980 mean",0.980,0.950,0.990));
			m_vars.back()->setConstant(true);
			m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongphif0980_width","chil0_ma bkg_bbar_wrongphif0980 width",0.02,0.01,0.1));

			m_pdfs.push_back(new RooBreitWignerTwoBodyPHSP(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),chil0_ma,*m_vars.at(0),*m_vars.at(1),m_K,m_K));
			m_pdfs.push_back(new RooBreitWignerTwoBodyPHSP(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),chil0_ma,*m_vars.at(0),*m_vars.at(1),m_K,m_K));

			m_main_pdfs["C"] = m_pdfs.at(0);
			m_main_pdfs["N"] = m_pdfs.at(1);
		}

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongphif0980AndNonRes_1D_chil0_ma : public FF_PDF_bkg_bbar_wrongphif0980 {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphif0980AndNonRes_1D_chil0_ma() { }

	FF_PDF_bkg_bbar_wrongphif0980AndNonRes_1D_chil0_ma(RooRealVar& chil0_ma, const FFUtil::tuple_category_name_flags& cat, bool is_data) : FF_PDF_bkg_bbar_wrongphif0980("pdf_bkg_bbar_wrongphif0980AndNonRes_1D_chil0_ma",cat) {
		if(is_data) {
			// Flatte formalism (see BN 1129 for f0(980) parameters)
			m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongphif0980_amp","chil0_ma bkg_bbar_wrongphif0980 amp",1.0));
			m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongphif0980_phase","chil0_ma bkg_bbar_wrongphif0980 phase",0.0));
			m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongphif0980_pole","chil0_ma bkg_bbar_wrongphif0980 pole",0.965,0.955,0.975));
			m_vars.back()->setConstant(true);
			m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongphif0980_g1","chil0_ma bkg_bbar_wrongphif0980 g1",0.165));
			m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongphif0980_g2","chil0_ma bkg_bbar_wrongphif0980 g2",0.347/std::sqrt(2.0)));
			m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongphif0980_g3","chil0_ma bkg_bbar_wrongphif0980 g3",0.347/std::sqrt(2.0)));
			m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongphinonres_amp","chil0_ma bkg_bbar_wrongphinonres amp",1.0,0.0,10.0));
			m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongphinonres_phase","chil0_ma bkg_bbar_wrongphinonres phase",0.0,0.0,2*TMath::Pi()));

			m_pdfs.push_back(new RooFlatteAndNonRes(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),
					chil0_ma,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5),*m_vars.at(6),*m_vars.at(7),
					m_Pi,m_Pi,m_K,m_K,m_Ks,m_Ks,m_K,m_K,m_K892,m_B0));
			m_pdfs.push_back(new RooFlatteAndNonRes(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),
					chil0_ma,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5),*m_vars.at(6),*m_vars.at(7),
					m_Pi,m_Pi,m_K,m_K,m_Ks,m_Ks,m_K,m_K,m_K892,m_B0));

			m_main_pdfs["C"] = m_pdfs.at(0);
			m_main_pdfs["N"] = m_pdfs.at(1);
		} else {
			// Two Body PSHP Breit-Wigner
			m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongphif0980_mean","chil0_ma bkg_bbar_wrongphif0980 mean",0.980,0.950,0.990));
			m_vars.back()->setConstant(true);
			m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongphif0980_width","chil0_ma bkg_bbar_wrongphif0980 width",0.02,0.01,0.1));

			m_pdfs.push_back(new RooBreitWignerTwoBodyPHSP(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),chil0_ma,*m_vars.at(0),*m_vars.at(1),m_K,m_K));
			m_pdfs.push_back(new RooBreitWignerTwoBodyPHSP(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),chil0_ma,*m_vars.at(0),*m_vars.at(1),m_K,m_K));

			m_main_pdfs["C"] = m_pdfs.at(0);
			m_main_pdfs["N"] = m_pdfs.at(1);
		}

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_1D_chil0_ma : public FF_PDF_bkg_bbar {
public:
	virtual ~FF_PDF_bkg_bbar_1D_chil0_ma() { }

	FF_PDF_bkg_bbar_1D_chil0_ma(shared_ptr<FF_PDF> combinatoric, shared_ptr<FF_PDF> wrongmasshyp,
			shared_ptr<FF_PDF> wrongphikkbar, shared_ptr<FF_PDF> wrongphif0980, double nevents,
			const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar("pdf_bkg_bbar_1D_chil0_ma",cat) {
		m_comb_pdfs.push_back(combinatoric);
		m_comb_pdfs.push_back(wrongmasshyp);
		m_comb_pdfs.push_back(wrongphikkbar);
		m_comb_pdfs.push_back(wrongphif0980);

		m_vars.push_back(new RooRealVar("C_chil0_ma_bkg_bbar_combinatoric_nevents","C_chil0_ma_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_chil0_ma_bkg_bbar_wrongmasshyp_nevents","C_chil0_ma_bkg_bbar_wrongmasshyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_chil0_ma_bkg_bbar_wrongphikkbar_nevents","C_chil0_ma_bkg_bbar_wrongphikkbar_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_chil0_ma_bkg_bbar_wrongphif0980_nevents","C_chil0_ma_bkg_bbar_wrongphif0980_nevents",nevents/7.0,0.0,nevents*1.2));

		m_vars.push_back(new RooRealVar("N_chil0_ma_bkg_bbar_combinatoric_nevents","N_chil0_ma_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_chil0_ma_bkg_bbar_wrongphikkbar_nevents","N_chil0_ma_bkg_bbar_wrongphikkbar_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_chil0_ma_bkg_bbar_wrongphif0980_nevents","N_chil0_ma_bkg_bbar_wrongphif0980_nevents",nevents/7.0,0.0,nevents*1.2));

		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
				m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C")),
				RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3)),false));
		m_pdfs.push_back(new RooAddPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(2)->GetPDF("N"),
				m_comb_pdfs.at(3)->GetPDF("N")),
				RooArgList(*m_vars.at(4),*m_vars.at(5),*m_vars.at(6)),false));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}

	FF_PDF_bkg_bbar_1D_chil0_ma(RooRealVar& chil0_ma, double nevents, const FFUtil::tuple_category_name_flags& cat, bool is_data, RooRealVar* resolution) : FF_PDF_bkg_bbar("pdf_bkg_bbar_1D_chil0_ma",cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_chil0_ma>(chil0_ma,cat,is_data,resolution));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_chil0_ma>(chil0_ma,cat,is_data,resolution));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_chil0_ma>(chil0_ma,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_chil0_ma>(chil0_ma,cat,is_data));

		m_vars.push_back(new RooRealVar("C_chil0_ma_bkg_bbar_combinatoric_nevents","C_chil0_ma_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_chil0_ma_bkg_bbar_wrongmasshyp_nevents","C_chil0_ma_bkg_bbar_wrongmasshyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_chil0_ma_bkg_bbar_wrongphikkbar_nevents","C_chil0_ma_bkg_bbar_wrongphikkbar_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_chil0_ma_bkg_bbar_wrongphif0980_nevents","C_chil0_ma_bkg_bbar_wrongphif0980_nevents",nevents/7.0,0.0,nevents*1.2));

		m_vars.push_back(new RooRealVar("N_chil0_ma_bkg_bbar_combinatoric_nevents","N_chil0_ma_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_chil0_ma_bkg_bbar_wrongphikkbar_nevents","N_chil0_ma_bkg_bbar_wrongphikkbar_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_chil0_ma_bkg_bbar_wrongphif0980_nevents","N_chil0_ma_bkg_bbar_wrongphif0980_nevents",nevents/7.0,0.0,nevents*1.2));

		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
				m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C")),
				RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3)),false));
		m_pdfs.push_back(new RooAddPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(2)->GetPDF("N"),
				m_comb_pdfs.at(3)->GetPDF("N")),
				RooArgList(*m_vars.at(4),*m_vars.at(5),*m_vars.at(6)),false));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};



class FF_PDF_bkg_bbar_combinatoric_1D_nb_prime : public FF_PDF_bkg_bbar_combinatoric {
public:
	virtual ~FF_PDF_bkg_bbar_combinatoric_1D_nb_prime() { }

	FF_PDF_bkg_bbar_combinatoric_1D_nb_prime(RooRealVar& nb_prime, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_combinatoric("pdf_bkg_bbar_combinatoric_1D_nb_prime",cat) {
        
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_combinatoric_mean","C nb_prime bkg bbar combinatoric mean",2.41110951135987,-10,10));//V0
        //m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_combinatoric_mean_1st_off","C nb_prime bkg bbar combinatoric mean 1st off",1.3271657746723875,-5.0,5.0));//V1
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_combinatoric_mean_2nd_off","C nb_prime bkg bbar combinatoric mean 2nd off",-2.0719548660127582,-5.0,5.0));//V2
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_combinatoric_mean_3rd_off","C nb_prime bkg bbar combinatoric mean 3rd off",-0.31335060320258457,-5.0,5.0));//V3
		m_vars.back()->setConstant(true);
        
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_combinatoric_mean_1st","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(1)))); //F0
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_combinatoric_mean_2nd","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(2)))); //F1
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_combinatoric_mean_3rd","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(3)))); //F2
        
        
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_combinatoric_width","C nb_prime bkg bbar combinatoric width",1.58551994661684,0,5.0));//V4
        //m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_combinatoric_width_1st_off","C nb_prime bkg bbar combinatoric width 1st off",0.89440640757916301,0.0,5.0));//V5
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_combinatoric_width_2nd_off","C nb_prime bkg bbar combinatoric width 2nd off",0.87663140581220245,0.0,5.0));//V6
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_combinatoric_width_3rd_off","C nb_prime bkg bbar combinatoric width 3rd off",0.33536987120264844,0.0,5.0));//V7
		m_vars.back()->setConstant(true);
        
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_combinatoric_width_1st","@0*@1",RooArgList(*m_vars.at(4),*m_vars.at(5)))); //F3
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_combinatoric_width_2nd","@0*@1",RooArgList(*m_vars.at(4),*m_vars.at(6)))); //F4
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_combinatoric_width_2nd","@0*@1",RooArgList(*m_vars.at(4),*m_vars.at(7)))); //F5
        
        
        m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_combinatoric_tail","C nb_prime bkg bbar combinatoric tail",0.25885358108502327,0.0,0.5));//V8
		//m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_combinatoric_tail_1st_off","C nb_prime bkg bbar combinatoric tail 1st off",1.5715714580611166,-5.0,5.0));//V9
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_combinatoric_tail_2nd_off","C nb_prime bkg bbar combinatoric tail 2nd off",0.83214999436313875,-5.0,5.0));//V10
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_combinatoric_tail_3rd_off","C nb_prime bkg bbar combinatoric tail 3rd off",-4.1801294357404171,-5.0,5.0));//V11
		m_vars.back()->setConstant(true);
        
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_combinatoric_tail_1st","@0*@1",RooArgList(*m_vars.at(8),*m_vars.at(9)))); //F6
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_combinatoric_tail_2nd","@0*@1",RooArgList(*m_vars.at(8),*m_vars.at(10)))); //F7
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_combinatoric_tail_2nd","@0*@1",RooArgList(*m_vars.at(8),*m_vars.at(11)))); //F8
        
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_1st").c_str(),("C_"+m_label+"_1st").c_str(),nb_prime,*m_functions.at(0),*m_functions.at(3),*m_functions.at(6)));
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_2nd").c_str(),("C_"+m_label+"_2nd").c_str(),nb_prime,*m_functions.at(1),*m_functions.at(4),*m_functions.at(7)));
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_3rd").c_str(),("C_"+m_label+"_3rd").c_str(),nb_prime,*m_functions.at(2),*m_functions.at(5),*m_functions.at(8)));
        
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_combinatoric_frac_1st","C nb_prime bkg bbar combinatoric fraction 1st",0.34520648498460726,0.0,1.0));//V12
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_combinatoric_frac_2nd","C nb_prime bkg bbar combinatoric fraction 2md",0.22835082764518527,0.0,1.0));//V13
		m_vars.back()->setConstant(true);
        
		m_pdfs.push_back(new RooAddPdf((""+m_label+"").c_str(),(""+m_label+"").c_str(),RooArgList(*m_pdfs.at(0),*m_pdfs.at(1),*m_pdfs.at(2)),RooArgList(*m_vars.at(12),*m_vars.at(13))));
        
		m_main_pdfs["C"] = m_pdfs.at(3);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongmasshyp_1D_nb_prime : public FF_PDF_bkg_bbar_wrongmasshyp {
public:
	virtual ~FF_PDF_bkg_bbar_wrongmasshyp_1D_nb_prime() { }

	FF_PDF_bkg_bbar_wrongmasshyp_1D_nb_prime(RooRealVar& nb_prime, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_wrongmasshyp("pdf_bkg_bbar_wrongmasshyp_1D_nb_prime",cat) {

        m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongmasshyp_mean","C nb_prime bkg bbar wrongmasshyp mean",2.94969232927159,-10,10));//V0
        //m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongmasshyp_mean_1st_off","C nb_prime bkg bbar wrongmasshyp mean 1st off",1.7387596661536993,-5.0,5.0));//V1
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongmasshyp_mean_2nd_off","C nb_prime bkg bbar wrongmasshyp mean 2nd off",-2.6396959545615144,-5.0,5.0));//V2
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongmasshyp_mean_3rd_off","C nb_prime bkg bbar wrongmasshyp mean 3rd off",-0.29171986338821565,-5.0,5.0));//V3
		m_vars.back()->setConstant(true);
        
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_wrongmasshyp_mean_1st","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(1)))); //F0
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_wrongmasshyp_mean_2nd","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(2)))); //F1
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_wrongmasshyp_mean_3rd","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(3)))); //F2
        
        
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongmasshyp_width","C nb_prime bkg bbar wrongmasshyp width",1.58551994661684,0,5.0));//V4
        //m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongmasshyp_width_1st_off","C nb_prime bkg bbar wrongmasshyp width 1st off",0.68201610171849025,0.0,5.0));//V5
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongmasshyp_width_2nd_off","C nb_prime bkg bbar wrongmasshyp width 2nd off",0.88597321112838667,0.0,5.0));//V6
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongmasshyp_width_3rd_off","C nb_prime bkg bbar wrongmasshyp width 3rd off",0.33536987120264844,0.0,5.0));//V7
		m_vars.back()->setConstant(true);
        
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_wrongmasshyp_width_1st","@0*@1",RooArgList(*m_vars.at(4),*m_vars.at(5)))); //F3
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_wrongmasshyp_width_2nd","@0*@1",RooArgList(*m_vars.at(4),*m_vars.at(6)))); //F4
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_wrongmasshyp_width_2nd","@0*@1",RooArgList(*m_vars.at(4),*m_vars.at(7)))); //F5
        
        
        m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongmasshyp_tail","C nb_prime bkg bbar wrongmasshyp tail",0.25885358108502327,0.0,0.5));//V8
		//m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongmasshyp_tail_1st_off","C nb_prime bkg bbar wrongmasshyp tail 1st off",1.7569024244424511,-5.0,5.0));//V9
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongmasshyp_tail_2nd_off","C nb_prime bkg bbar wrongmasshyp tail 2nd off",0.82321152767061978,-5.0,5.0));//V10
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongmasshyp_tail_3rd_off","C nb_prime bkg bbar wrongmasshyp tail 3rd off",-2.397127693021015,-5.0,5.0));//V11
		m_vars.back()->setConstant(true);
        
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_wrongmasshyp_tail_1st","@0*@1",RooArgList(*m_vars.at(8),*m_vars.at(9)))); //F6
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_wrongmasshyp_tail_2nd","@0*@1",RooArgList(*m_vars.at(8),*m_vars.at(10)))); //F7
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_wrongmasshyp_tail_2nd","@0*@1",RooArgList(*m_vars.at(8),*m_vars.at(11)))); //F8
        
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_1st").c_str(),("C_"+m_label+"_1st").c_str(),nb_prime,*m_functions.at(0),*m_functions.at(3),*m_functions.at(6)));
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_2nd").c_str(),("C_"+m_label+"_2nd").c_str(),nb_prime,*m_functions.at(1),*m_functions.at(4),*m_functions.at(7)));
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_3rd").c_str(),("C_"+m_label+"_3rd").c_str(),nb_prime,*m_functions.at(2),*m_functions.at(5),*m_functions.at(8)));
        
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongmasshyp_frac_1st","C nb_prime bkg bbar wrongmasshyp fraction 1st",0.30598645476376157,0.0,1.0));//V12
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongmasshyp_frac_2nd","C nb_prime bkg bbar wrongmasshyp fraction 2md",0.15555672443253671,0.0,1.0));//V13
		m_vars.back()->setConstant(true);
        
		m_pdfs.push_back(new RooAddPdf((""+m_label+"").c_str(),(""+m_label+"").c_str(),RooArgList(*m_pdfs.at(0),*m_pdfs.at(1),*m_pdfs.at(2)),RooArgList(*m_vars.at(12),*m_vars.at(13))));
        
		m_main_pdfs["C"] = m_pdfs.at(3);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongpi0hyp_1D_nb_prime : public FF_PDF_bkg_bbar_wrongpi0hyp {
public:
	virtual ~FF_PDF_bkg_bbar_wrongpi0hyp_1D_nb_prime() { }
    
	FF_PDF_bkg_bbar_wrongpi0hyp_1D_nb_prime(RooRealVar& nb_prime, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_wrongpi0hyp("pdf_bkg_bbar_wrongpi0hyp_1D_nb_prime",cat) {
        
        m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongpi0hyp_mean","C nb_prime bkg bbar wrongpi0hyp mean",2.94969232927159,-10,10));//V0
        //m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongpi0hyp_mean_1st_off","C nb_prime bkg bbar wrongpi0hyp mean 1st off",1.4290867617278025,-5.0,5.0));//V1
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongpi0hyp_mean_2nd_off","C nb_prime bkg bbar wrongpi0hyp mean 2nd off",-2.4694665078639777,-5.0,5.0));//V2
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongpi0hyp_mean_3rd_off","C nb_prime bkg bbar wrongpi0hyp mean 3rd off",-0.34048458317403263,-5.0,5.0));//V3
		m_vars.back()->setConstant(true);
        
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_wrongpi0hyp_mean_1st","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(1)))); //F0
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_wrongpi0hyp_mean_2nd","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(2)))); //F1
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_wrongpi0hyp_mean_3rd","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(3)))); //F2
        
        
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongpi0hyp_width","C nb_prime bkg bbar wrongpi0hyp width",1.58551994661684,0,5.0));//V4
        //m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongpi0hyp_width_1st_off","C nb_prime bkg bbar wrongpi0hyp width 1st off",0.71590844335079118,0.0,5.0));//V5
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongpi0hyp_width_2nd_off","C nb_prime bkg bbar wrongpi0hyp width 2nd off",0.8326746464786583,0.0,5.0));//V6
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongpi0hyp_width_3rd_off","C nb_prime bkg bbar wrongpi0hyp width 3rd off",0.33536987120264844,0.0,5.0));//V7
		m_vars.back()->setConstant(true);
        
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_wrongpi0hyp_width_1st","@0*@1",RooArgList(*m_vars.at(4),*m_vars.at(5)))); //F3
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_wrongpi0hyp_width_2nd","@0*@1",RooArgList(*m_vars.at(4),*m_vars.at(6)))); //F4
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_wrongpi0hyp_width_2nd","@0*@1",RooArgList(*m_vars.at(4),*m_vars.at(7)))); //F5
        
        
        m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongpi0hyp_tail","C nb_prime bkg bbar wrongpi0hyp tail",0.25885358108502327,0.0,0.5));//V8
		//m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongpi0hyp_tail_1st_off","C nb_prime bkg bbar wrongpi0hyp tail 1st off",1.6067408537448404,-5.0,5.0));//V9
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongpi0hyp_tail_2nd_off","C nb_prime bkg bbar wrongpi0hyp tail 2nd off",1.0467856855573201,-5.0,5.0));//V10
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongpi0hyp_tail_3rd_off","C nb_prime bkg bbar wrongpi0hyp tail 3rd off",-4.9836886929555808,-5.0,5.0));//V11
		m_vars.back()->setConstant(true);
        
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_wrongpi0hyp_tail_1st","@0*@1",RooArgList(*m_vars.at(8),*m_vars.at(9)))); //F6
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_wrongpi0hyp_tail_2nd","@0*@1",RooArgList(*m_vars.at(8),*m_vars.at(10)))); //F7
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_wrongpi0hyp_tail_2nd","@0*@1",RooArgList(*m_vars.at(8),*m_vars.at(11)))); //F8
        
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_1st").c_str(),("C_"+m_label+"_1st").c_str(),nb_prime,*m_functions.at(0),*m_functions.at(3),*m_functions.at(6)));
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_2nd").c_str(),("C_"+m_label+"_2nd").c_str(),nb_prime,*m_functions.at(1),*m_functions.at(4),*m_functions.at(7)));
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_3rd").c_str(),("C_"+m_label+"_3rd").c_str(),nb_prime,*m_functions.at(2),*m_functions.at(5),*m_functions.at(8)));
        
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongpi0hyp_frac_1st","C nb_prime bkg bbar wrongpi0hyp fraction 1st",0.25220379501660972,0.0,1.0));//V12
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongpi0hyp_frac_2nd","C nb_prime bkg bbar wrongpi0hyp fraction 2md",0.26039163131996645,0.0,1.0));//V13
		m_vars.back()->setConstant(true);
        
		m_pdfs.push_back(new RooAddPdf((""+m_label+"").c_str(),(""+m_label+"").c_str(),RooArgList(*m_pdfs.at(0),*m_pdfs.at(1),*m_pdfs.at(2)),RooArgList(*m_vars.at(12),*m_vars.at(13))));
        
		m_main_pdfs["C"] = m_pdfs.at(3);
		m_main_pdfs["CP"] = m_pdfs.at(3);
		m_main_pdfs["CA"] = m_pdfs.at(3);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_bkg_bbar_missingfsp_1D_nb_prime : public FF_PDF_bkg_bbar_missingfsp {
public:
	virtual ~FF_PDF_bkg_bbar_missingfsp_1D_nb_prime() { }
    
	FF_PDF_bkg_bbar_missingfsp_1D_nb_prime(RooRealVar& nb_prime, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_missingfsp("pdf_bkg_bbar_missingfsp_1D_nb_prime",cat) {
        
        m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_missingfsp_mean","C nb_prime bkg bbar missingfsp mean",2.94969232927159,-10,10));//V0
        //m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_missingfsp_mean_1st_off","C nb_prime bkg bbar missingfsp mean 1st off",1.9114933056711125,-5.0,5.0));//V1
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_missingfsp_mean_2nd_off","C nb_prime bkg bbar missingfsp mean 2nd off",-1.8774492362103068,-5.0,5.0));//V2
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_missingfsp_mean_3rd_off","C nb_prime bkg bbar missingfsp mean 3rd off",0.24775642863339442,-5.0,5.0));//V3
		m_vars.back()->setConstant(true);
        
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_missingfsp_mean_1st","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(1)))); //F0
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_missingfsp_mean_2nd","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(2)))); //F1
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_missingfsp_mean_3rd","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(3)))); //F2
        
        
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_missingfsp_width","C nb_prime bkg bbar missingfsp width",1.58551994661684,0,5.0));//V4
        //m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_missingfsp_width_1st_off","C nb_prime bkg bbar missingfsp width 1st off",0.66111446641454574,0.0,5.0));//V5
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_missingfsp_width_2nd_off","C nb_prime bkg bbar missingfsp width 2nd off",0.87207580074429358,0.0,5.0));//V6
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_missingfsp_width_3rd_off","C nb_prime bkg bbar missingfsp width 3rd off",0.33536987120264844,0.0,5.0));//V7
		m_vars.back()->setConstant(true);
        
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_missingfsp_width_1st","@0*@1",RooArgList(*m_vars.at(4),*m_vars.at(5)))); //F3
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_missingfsp_width_2nd","@0*@1",RooArgList(*m_vars.at(4),*m_vars.at(6)))); //F4
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_missingfsp_width_2nd","@0*@1",RooArgList(*m_vars.at(4),*m_vars.at(7)))); //F5
        
        
        m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_missingfsp_tail","C nb_prime bkg bbar missingfsp tail",0.25885358108502327,0.0,0.5));//V8
		//m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_missingfsp_tail_1st_off","C nb_prime bkg bbar missingfsp tail 1st off",1.9242546393805924,-5.0,5.0));//V9
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_missingfsp_tail_2nd_off","C nb_prime bkg bbar missingfsp tail 2nd off",0.9719235119417915,-5.0,5.0));//V10
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_missingfsp_tail_3rd_off","C nb_prime bkg bbar missingfsp tail 3rd off",-4.9836886929555808,-5.0,5.0));//V11
		m_vars.back()->setConstant(true);
        
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_missingfsp_tail_1st","@0*@1",RooArgList(*m_vars.at(8),*m_vars.at(9)))); //F6
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_missingfsp_tail_2nd","@0*@1",RooArgList(*m_vars.at(8),*m_vars.at(10)))); //F7
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_missingfsp_tail_2nd","@0*@1",RooArgList(*m_vars.at(8),*m_vars.at(11)))); //F8
        
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_1st").c_str(),("C_"+m_label+"_1st").c_str(),nb_prime,*m_functions.at(0),*m_functions.at(3),*m_functions.at(6)));
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_2nd").c_str(),("C_"+m_label+"_2nd").c_str(),nb_prime,*m_functions.at(1),*m_functions.at(4),*m_functions.at(7)));
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_3rd").c_str(),("C_"+m_label+"_3rd").c_str(),nb_prime,*m_functions.at(2),*m_functions.at(5),*m_functions.at(8)));
        
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_missingfsp_frac_1st","C nb_prime bkg bbar missingfsp fraction 1st",0.27646028388172106,0.0,1.0));//V12
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_missingfsp_frac_2nd","C nb_prime bkg bbar missingfsp fraction 2nd",0.21137884277368951,0.0,1.0));//V13
		m_vars.back()->setConstant(true);
        
        m_pdfs.push_back(new RooAddPdf((""+m_label+"").c_str(),(""+m_label+"").c_str(),RooArgList(*m_pdfs.at(0),*m_pdfs.at(1),*m_pdfs.at(2)),RooArgList(*m_vars.at(12),*m_vars.at(13))));
        
        
        m_main_pdfs["C"] = m_pdfs.at(3);
		m_main_pdfs["CP"] = m_pdfs.at(3);
		m_main_pdfs["CA"] = m_pdfs.at(3);
        
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_bkg_bbar_nonres_1D_nb_prime : public FF_PDF_bkg_bbar_nonres {
public:
	virtual ~FF_PDF_bkg_bbar_nonres_1D_nb_prime() { }
    
	FF_PDF_bkg_bbar_nonres_1D_nb_prime(RooRealVar& nb_prime, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_nonres("pdf_bkg_bbar_nonres_1D_nb_prime",cat) {
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_nonres_mean","C nb prime signal mean",2.61016,-1.0,5.0));//V0
		//m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_nonres_mean_1st_off","C nb prime signal mean 1st off",0.97482463205136582,-5.0,5.0));//V1
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_nonres_mean_2nd_off","C nb prime signal mean 2nd off",2.4377621066042243,-5.0,5.0));//V2
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_nonres_mean_3rd_off","C nb prime signal mean 3rd off",-0.92796060350496834,-5.0,5.0));//V3
		m_vars.back()->setConstant(true);
        
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_nonres_mean_1st","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(1)))); //F0
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_nonres_mean_2nd","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(2)))); //F1
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_nonres_mean_3rd","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(3)))); //F2
        
        
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_nonres_width","C nb prime signal width",1.82359,-5.0,5.0));//V4
		//m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_nonres_width_1st_off","C nb prime signal width 1st off",0.69458448667713579,-5.0,5.0));//V5
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_nonres_width_2nd_off","C nb prime signal width 2nd off",0.50547733132967565,-5.0,5.0));//V6
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_nonres_width_3rd_off","C nb prime signal width 3rd off",0.90053477097979151,-5.0,5.0));//V7
		m_vars.back()->setConstant(true);
        
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_nonres_width_1st","@0*@1",RooArgList(*m_vars.at(4),*m_vars.at(5)))); //F3
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_nonres_width_2nd","@0*@1",RooArgList(*m_vars.at(4),*m_vars.at(6)))); //F4
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_nonres_width_3rd","@0*@1",RooArgList(*m_vars.at(4),*m_vars.at(7)))); //F5
        
        
        m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_nonres_tail","C nb prime signal tail",3.53472e-01,-1.0,5.0));//V8
		//m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_nonres_tail_1st_off","C nb prime signal tail 1st off",0.79938266812523917,-5.0,5.0));//V9
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_nonres_tail_2nd_off","C nb prime signal tail 2nd off",1.3211572834500256,-5.0,5.0));//V10
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_nonres_tail_3rd_off","C nb prime signal tail 3rd off",0.45507687393452478,-5.0,5.0));//V11
		m_vars.back()->setConstant(true);
        
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_nonres_tail_1st","@0*@1",RooArgList(*m_vars.at(8),*m_vars.at(9)))); //F6
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_nonres_tail_2nd","@0*@1",RooArgList(*m_vars.at(8),*m_vars.at(10)))); //F7
        m_functions.push_back(new RooFormulaVar("C_nb_prime_bkg_bbar_nonres_tail_3rd","@0*@1",RooArgList(*m_vars.at(8),*m_vars.at(11)))); //F8
        
        
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_1st").c_str(),("C_"+m_label+"_1st").c_str(),nb_prime,*m_functions.at(0),*m_functions.at(3),*m_functions.at(6)));
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_2nd").c_str(),("C_"+m_label+"_2nd").c_str(),nb_prime,*m_functions.at(1),*m_functions.at(4),*m_functions.at(7)));
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_3rd").c_str(),("C_"+m_label+"_3rd").c_str(),nb_prime,*m_functions.at(2),*m_functions.at(5),*m_functions.at(8)));
        
        
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_nonres_frac_1st","C_nb_prime_bkg_bbar_nonres fraction 1st",0.39220170737461402,0.1,0.9));//V12
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_nonres_frac_2nd","C_nb_prime_bkg_bbar_nonres fraction 2nd",0.20195564753349463,0.1,0.9));//V13
		m_vars.back()->setConstant(true);
        
		m_pdfs.push_back(new RooAddPdf((""+m_label+"").c_str(),(""+m_label+"").c_str(),RooArgList(*m_pdfs.at(0),*m_pdfs.at(1),*m_pdfs.at(2)),RooArgList(*m_vars.at(12),*m_vars.at(13))));
        
		m_main_pdfs["C"] = m_pdfs.at(3);
		m_main_pdfs["CP"] = m_pdfs.at(3);
		m_main_pdfs["CA"] = m_pdfs.at(3);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};



class FF_PDF_bkg_bbar_wrongphikkbar_1D_nb_prime : public FF_PDF_bkg_bbar_wrongphikkbar {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphikkbar_1D_nb_prime() { }

	FF_PDF_bkg_bbar_wrongphikkbar_1D_nb_prime(RooRealVar& nb_prime, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_wrongphikkbar("pdf_bkg_bbar_wrongphikkbar_1D_nb_prime",cat) {
		m_vars.push_back(new RooRealVar("nb_prime_bkg_bbar_wrongphikkbar_mean_1st","nb_prime_bkg_bbar_wrongphikkbar mean 1st",2.7,-1.0,5.0));
		m_vars.back()->setConstant(false); //FIXME
		m_vars.push_back(new RooRealVar("nb_prime_bkg_bbar_wrongphikkbar_width_1st_left","nb_prime_bkg_bbar_wrongphikkbar width 1st left",3.2,0.5,4.0));
		m_vars.push_back(new RooRealVar("nb_prime_bkg_bbar_wrongphikkbar_width_1st_right","nb_prime_bkg_bbar_wrongphikkbar width 1st right",1.2,0.0,4.0));
		m_pdfs.push_back(new RooBifurGauss(("C_"+m_label+"_1st").c_str(),("C_"+m_label+"_1st").c_str(),nb_prime,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2)));
		m_pdfs.push_back(new RooBifurGauss(("N_"+m_label+"_1st").c_str(),("N_"+m_label+"_1st").c_str(),nb_prime,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2)));
		m_vars.push_back(new RooRealVar("nb_prime_bkg_bbar_wrongphikkbar_mean_2nd","nb_prime_bkg_bbar_wrongphikkbar mean 2nd",4.3,-1.0,5.0));
		m_vars.back()->setConstant(false); //FIXME
		m_vars.push_back(new RooRealVar("nb_prime_bkg_bbar_wrongphikkbar_width_2nd_left","nb_prime_bkg_bbar_wrongphikkbar width 2nd left",3.0,0.5,4.0));
		m_vars.back()->setConstant(false); //FIXME
		m_vars.push_back(new RooRealVar("nb_prime_bkg_bbar_wrongphikkbar_width_2nd_right","nb_prime_bkg_bbar_wrongphikkbar width 2nd right",0.5,0.0,3.0));
		m_pdfs.push_back(new RooBifurGauss(("C_"+m_label+"_2nd").c_str(),("C_"+m_label+"_2nd").c_str(),nb_prime,*m_vars.at(3),*m_vars.at(4),*m_vars.at(5)));
		m_pdfs.push_back(new RooBifurGauss(("N_"+m_label+"_2nd").c_str(),("N_"+m_label+"_2nd").c_str(),nb_prime,*m_vars.at(3),*m_vars.at(4),*m_vars.at(5)));

		m_vars.push_back(new RooRealVar("nb_prime_bkg_bbar_wrongphikkbar_frac_1st","nb_prime_bkg_bbar_wrongphikkbar fraction 1st",0.6,0.1,0.9));
		m_vars.back()->setConstant(false); //FIXME
		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),*m_pdfs.at(0),*m_pdfs.at(2),*m_vars.at(6)));
		m_pdfs.push_back(new RooAddPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),*m_pdfs.at(1),*m_pdfs.at(3),*m_vars.at(6)));

		m_main_pdfs["C"] = m_pdfs.at(4);
		m_main_pdfs["N"] = m_pdfs.at(5);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongphif0980_1D_nb_prime : public FF_PDF_bkg_bbar_wrongphif0980 {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphif0980_1D_nb_prime() { }

	FF_PDF_bkg_bbar_wrongphif0980_1D_nb_prime(RooRealVar& nb_prime, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_wrongphif0980("pdf_bkg_bbar_wrongphif0980_1D_nb_prime",cat) {
		m_vars.push_back(new RooRealVar("nb_prime_bkg_bbar_wrongphif0980_mean_1st","nb_prime_bkg_bbar_wrongphif0980 mean 1st",2.7,-1.0,5.0));
		m_vars.back()->setConstant(false); //FIXME
		m_vars.push_back(new RooRealVar("nb_prime_bkg_bbar_wrongphif0980_width_1st_left","nb_prime_bkg_bbar_wrongphif0980 width 1st left",3.2,0.5,4.0));
		m_vars.push_back(new RooRealVar("nb_prime_bkg_bbar_wrongphif0980_width_1st_right","nb_prime_bkg_bbar_wrongphif0980 width 1st right",1.2,0.0,4.0));
		m_pdfs.push_back(new RooBifurGauss(("C_"+m_label+"_1st").c_str(),("C_"+m_label+"_1st").c_str(),nb_prime,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2)));
		m_pdfs.push_back(new RooBifurGauss(("N_"+m_label+"_1st").c_str(),("N_"+m_label+"_1st").c_str(),nb_prime,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2)));
		m_vars.push_back(new RooRealVar("nb_prime_bkg_bbar_wrongphif0980_mean_2nd","nb_prime_bkg_bbar_wrongphif0980 mean 2nd",4.3,-1.0,5.0));
		m_vars.back()->setConstant(false); //FIXME
		m_vars.push_back(new RooRealVar("nb_prime_bkg_bbar_wrongphif0980_width_2nd_left","nb_prime_bkg_bbar_wrongphif0980 width 2nd left",3.0,0.5,4.0));
		m_vars.back()->setConstant(false); //FIXME
		m_vars.push_back(new RooRealVar("nb_prime_bkg_bbar_wrongphif0980_width_2nd_right","nb_prime_bkg_bbar_wrongphif0980 width 2nd right",0.5,0.0,3.0));
		m_pdfs.push_back(new RooBifurGauss(("C_"+m_label+"_2nd").c_str(),("C_"+m_label+"_2nd").c_str(),nb_prime,*m_vars.at(3),*m_vars.at(4),*m_vars.at(5)));
		m_pdfs.push_back(new RooBifurGauss(("N_"+m_label+"_2nd").c_str(),("N_"+m_label+"_2nd").c_str(),nb_prime,*m_vars.at(3),*m_vars.at(4),*m_vars.at(5)));

		m_vars.push_back(new RooRealVar("nb_prime_bkg_bbar_wrongphif0980_frac_1st","nb_prime_bkg_bbar_wrongphif0980 fraction 1st",0.6,0.1,0.9));
		m_vars.back()->setConstant(false); //FIXME
		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),*m_pdfs.at(0),*m_pdfs.at(2),*m_vars.at(6)));
		m_pdfs.push_back(new RooAddPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),*m_pdfs.at(1),*m_pdfs.at(3),*m_vars.at(6)));

		m_main_pdfs["C"] = m_pdfs.at(4);
		m_main_pdfs["N"] = m_pdfs.at(5);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_1D_nb_prime : public FF_PDF_bkg_bbar {
public:
	virtual ~FF_PDF_bkg_bbar_1D_nb_prime() { }

	FF_PDF_bkg_bbar_1D_nb_prime(shared_ptr<FF_PDF> combinatoric, shared_ptr<FF_PDF> wrongmasshyp,
                                shared_ptr<FF_PDF> wrongpi0hyp, shared_ptr<FF_PDF> missingfsp,shared_ptr<FF_PDF> nonres, double nevents,
                                const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar("pdf_bkg_bbar_1D_nb_prime",cat) {
		m_comb_pdfs.push_back(combinatoric);
		m_comb_pdfs.push_back(wrongmasshyp);
		m_comb_pdfs.push_back(wrongpi0hyp);
		m_comb_pdfs.push_back(missingfsp);
		m_comb_pdfs.push_back(nonres);

		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_combinatoric_nevents","C_nb_prime_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongmasshyp_nevents","C_nb_prime_bkg_bbar_wrongmasshyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongpi0hyp_nevents","C_nb_prime_bkg_bbar_wrongpi0hyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_missingfsp_nevents","C_nb_prime_bkg_bbar_missingfsp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_nonres_nevents","C_nb_prime_bkg_bbar_nonres_nevents",nevents/7.0,0.0,nevents*1.2));
        
		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(
                                                                                                m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
                                                                                                m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),m_comb_pdfs.at(4)->GetPDF("C")),
                                       RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4)),false));
        
		m_main_pdfs["C"] = m_pdfs.at(0);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}

	FF_PDF_bkg_bbar_1D_nb_prime(RooRealVar& nb_prime, double nevents, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar("pdf_bkg_bbar_1D_nb_prime",cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_nb_prime>(nb_prime,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_nb_prime>(nb_prime,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_1D_nb_prime>(nb_prime,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_missingfsp_1D_nb_prime>(nb_prime,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_nonres_1D_nb_prime>(nb_prime,cat));

		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_combinatoric_nevents","C_nb_prime_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongmasshyp_nevents","C_nb_prime_bkg_bbar_wrongmasshyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_wrongpi0hyp_nevents","C_nb_prime_bkg_bbar_wrongpi0hyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_missingfsp_nevents","C_nb_prime_bkg_bbar_missingfsp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_nb_prime_bkg_bbar_nonres_nevents","C_nb_prime_bkg_bbar_nonres_nevents",nevents/7.0,0.0,nevents*1.2));
        
		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(
                                                                                                m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
                                                                                                m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),m_comb_pdfs.at(4)->GetPDF("C")),
                                       RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(2),*m_vars.at(4)),false));
        
		m_main_pdfs["C"] = m_pdfs.at(0);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_combinatoric_3D_deltae_chil0_ma_nb_prime: public FF_PDF_bkg_bbar_combinatoric {
public:
	virtual ~FF_PDF_bkg_bbar_combinatoric_3D_deltae_chil0_ma_nb_prime() { }

	FF_PDF_bkg_bbar_combinatoric_3D_deltae_chil0_ma_nb_prime(RooRealVar& deltae,
			RooRealVar& chil0_ma, RooRealVar& nb_prime,
			const FFUtil::tuple_category_name_flags& cat, bool is_data, RooRealVar* resolution)
	: FF_PDF_bkg_bbar_combinatoric("pdf_bkg_bbar_combinatoric_3D_deltae_chil0_ma_nb_prime", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_chil0_ma>(chil0_ma,cat,is_data,resolution));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_nb_prime>(nb_prime,cat));

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
						m_comb_pdfs.at(2)->GetPDF("C"))));
		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
						m_comb_pdfs.at(2)->GetPDF("N"))));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);
		m_main_pdfs["NS"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_combinatoric_3D_mbc_deltae_nb_prime: public FF_PDF_bkg_bbar_combinatoric {
public:
	virtual ~FF_PDF_bkg_bbar_combinatoric_3D_mbc_deltae_nb_prime() { }
    
	FF_PDF_bkg_bbar_combinatoric_3D_mbc_deltae_nb_prime(RooRealVar& mbc, RooRealVar& deltae, RooRealVar& nb_prime, const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_combinatoric("pdf_bkg_bbar_combinatoric_3D_mbc_deltae_nb_prime", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_nb_prime>(nb_prime,cat));
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(), RooArgList(m_comb_pdfs.at(0)->GetPDF("C"), m_comb_pdfs.at(1)->GetPDF("C"), m_comb_pdfs.at(2)->GetPDF("C"))));

        
		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
    
	FF_PDF_bkg_bbar_combinatoric_3D_mbc_deltae_nb_prime(shared_ptr<FF_PDF> mbc, shared_ptr<FF_PDF> deltae, shared_ptr<FF_PDF> nb_prime, const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_combinatoric("pdf_bkg_bbar_combinatoric_3D_mbc_deltae_nb_prime", cat) {
		m_comb_pdfs.push_back(mbc);
		m_comb_pdfs.push_back(deltae);
		m_comb_pdfs.push_back(nb_prime);
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(), RooArgList(m_comb_pdfs.at(0)->GetPDF("C"), m_comb_pdfs.at(1)->GetPDF("C"), m_comb_pdfs.at(2)->GetPDF("C"))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};



class FF_PDF_bkg_bbar_combinatoric_4D_mbc_deltae_chil0_ma_nb_prime: public FF_PDF_bkg_bbar_combinatoric {
public:
	virtual ~FF_PDF_bkg_bbar_combinatoric_4D_mbc_deltae_chil0_ma_nb_prime() { }

	FF_PDF_bkg_bbar_combinatoric_4D_mbc_deltae_chil0_ma_nb_prime(RooRealVar& mbc, RooRealVar& deltae,
			RooRealVar& chil0_ma, RooRealVar& nb_prime,
			const FFUtil::tuple_category_name_flags& cat, bool is_data, RooRealVar* resolution)
	: FF_PDF_bkg_bbar_combinatoric("pdf_bkg_bbar_combinatoric_4D_mbc_deltae_chil0_ma_nb_prime", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_chil0_ma>(chil0_ma,cat,is_data,resolution));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_nb_prime>(nb_prime,cat));

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
						m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"))));
		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
						m_comb_pdfs.at(2)->GetPDF("N"),m_comb_pdfs.at(3)->GetPDF("N"))));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);
		m_main_pdfs["NS"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}

	FF_PDF_bkg_bbar_combinatoric_4D_mbc_deltae_chil0_ma_nb_prime(shared_ptr<FF_PDF> mbc, shared_ptr<FF_PDF> deltae,
			shared_ptr<FF_PDF> chil0_ma, shared_ptr<FF_PDF> nb_prime,
			const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_combinatoric("pdf_bkg_bbar_combinatoric_4D_mbc_deltae_chil0_ma_nb_prime", cat) {
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
		m_main_pdfs["N"] = m_pdfs.at(1);
		m_main_pdfs["NS"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongmasshyp_4D_mbc_deltae_chil0_ma_nb_prime: public FF_PDF_bkg_bbar_wrongmasshyp {
public:
	virtual ~FF_PDF_bkg_bbar_wrongmasshyp_4D_mbc_deltae_chil0_ma_nb_prime() { }

	FF_PDF_bkg_bbar_wrongmasshyp_4D_mbc_deltae_chil0_ma_nb_prime(RooRealVar& mbc, RooRealVar& deltae,
			RooRealVar& chil0_ma, RooRealVar& nb_prime,
			const FFUtil::tuple_category_name_flags& cat, bool is_data, RooRealVar* resolution)
	: FF_PDF_bkg_bbar_wrongmasshyp("pdf_bkg_bbar_wrongmasshyp_4D_mbc_deltae_chil0_ma_nb_prime", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_chil0_ma>(chil0_ma,cat,is_data,resolution));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_nb_prime>(nb_prime,cat));

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
						m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"))));

		m_main_pdfs["C"] = m_pdfs.at(0);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}

	FF_PDF_bkg_bbar_wrongmasshyp_4D_mbc_deltae_chil0_ma_nb_prime(shared_ptr<FF_PDF> mbc, shared_ptr<FF_PDF> deltae,
			shared_ptr<FF_PDF> chil0_ma, shared_ptr<FF_PDF> nb_prime,
			const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_wrongmasshyp("pdf_bkg_bbar_wrongmasshyp_4D_mbc_deltae_chil0_ma_nb_prime", cat) {
		m_comb_pdfs.push_back(mbc);
		m_comb_pdfs.push_back(deltae);
		m_comb_pdfs.push_back(chil0_ma);
		m_comb_pdfs.push_back(nb_prime);

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
						m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"))));

		m_main_pdfs["C"] = m_pdfs.at(0);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_bkg_bbar_wrongmasshyp_3D_mbc_deltae_nb_prime: public FF_PDF_bkg_bbar_wrongmasshyp {
public:
	virtual ~FF_PDF_bkg_bbar_wrongmasshyp_3D_mbc_deltae_nb_prime() { }
    
	FF_PDF_bkg_bbar_wrongmasshyp_3D_mbc_deltae_nb_prime(RooRealVar& mbc, RooRealVar& deltae, RooRealVar& nb_prime, const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_wrongmasshyp("pdf_bkg_bbar_wrongmasshyp_3D_mbc_deltae_nb_prime", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_nb_prime>(nb_prime,cat));
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(m_comb_pdfs.at(0)->GetPDF("C"), m_comb_pdfs.at(1)->GetPDF("C"), m_comb_pdfs.at(2)->GetPDF("C"))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
    
	FF_PDF_bkg_bbar_wrongmasshyp_3D_mbc_deltae_nb_prime(shared_ptr<FF_PDF> mbc, shared_ptr<FF_PDF> deltae, shared_ptr<FF_PDF> nb_prime, const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_wrongmasshyp("pdf_bkg_bbar_wrongmasshyp_3D_mbc_deltae_nb_prime", cat) {
		m_comb_pdfs.push_back(mbc);
		m_comb_pdfs.push_back(deltae);
		m_comb_pdfs.push_back(nb_prime);
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(), RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"), m_comb_pdfs.at(2)->GetPDF("C"))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_bkg_bbar_wrongpi0hyp_3D_mbc_deltae_nb_prime: public FF_PDF_bkg_bbar_wrongpi0hyp {
public:
	virtual ~FF_PDF_bkg_bbar_wrongpi0hyp_3D_mbc_deltae_nb_prime() { }
    
	FF_PDF_bkg_bbar_wrongpi0hyp_3D_mbc_deltae_nb_prime(RooRealVar& mbc, RooRealVar& deltae, RooRealVar& nb_prime, const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_wrongpi0hyp("pdf_bkg_bbar_wrongpi0hyp_3D_mbc_deltae_nb_prime", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_1D_nb_prime>(nb_prime,cat));
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(m_comb_pdfs.at(0)->GetPDF("C"), m_comb_pdfs.at(1)->GetPDF("C"), m_comb_pdfs.at(2)->GetPDF("C"))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
    
	FF_PDF_bkg_bbar_wrongpi0hyp_3D_mbc_deltae_nb_prime(shared_ptr<FF_PDF> mbc, shared_ptr<FF_PDF> deltae, shared_ptr<FF_PDF> nb_prime, const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_wrongpi0hyp("pdf_bkg_bbar_wrongpi0hyp_3D_mbc_deltae_nb_prime", cat) {
		m_comb_pdfs.push_back(mbc);
		m_comb_pdfs.push_back(deltae);
		m_comb_pdfs.push_back(nb_prime);
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(), RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"), m_comb_pdfs.at(2)->GetPDF("C"))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_bkg_bbar_missingfsp_3D_mbc_deltae_nb_prime: public FF_PDF_bkg_bbar_missingfsp {
public:
	virtual ~FF_PDF_bkg_bbar_missingfsp_3D_mbc_deltae_nb_prime() { }
    
	FF_PDF_bkg_bbar_missingfsp_3D_mbc_deltae_nb_prime(RooRealVar& mbc, RooRealVar& deltae, RooRealVar& nb_prime, const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_missingfsp("pdf_bkg_bbar_missingfsp_3D_mbc_deltae_nb_prime", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_missingfsp_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_missingfsp_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_missingfsp_1D_nb_prime>(nb_prime,cat));
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(m_comb_pdfs.at(0)->GetPDF("C"), m_comb_pdfs.at(1)->GetPDF("C"), m_comb_pdfs.at(2)->GetPDF("C"))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
    
	FF_PDF_bkg_bbar_missingfsp_3D_mbc_deltae_nb_prime(shared_ptr<FF_PDF> mbc, shared_ptr<FF_PDF> deltae, shared_ptr<FF_PDF> nb_prime, const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_missingfsp("pdf_bkg_bbar_missingfsp_3D_mbc_deltae_nb_prime", cat) {
		m_comb_pdfs.push_back(mbc);
		m_comb_pdfs.push_back(deltae);
		m_comb_pdfs.push_back(nb_prime);
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(), RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"), m_comb_pdfs.at(2)->GetPDF("C"))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_nonres_3D_mbc_deltae_nb_prime: public FF_PDF_bkg_bbar_nonres {
public:
	virtual ~FF_PDF_bkg_bbar_nonres_3D_mbc_deltae_nb_prime() { }
    
	FF_PDF_bkg_bbar_nonres_3D_mbc_deltae_nb_prime(RooRealVar& mbc, RooRealVar& deltae, RooRealVar& nb_prime, const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_nonres("pdf_bkg_bbar_nonres_3D_mbc_deltae_nb_prime", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_nonres_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_nonres_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_nonres_1D_nb_prime>(nb_prime,cat));
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(m_comb_pdfs.at(0)->GetPDF("C"), m_comb_pdfs.at(1)->GetPDF("C"), m_comb_pdfs.at(2)->GetPDF("C"))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
    
	FF_PDF_bkg_bbar_nonres_3D_mbc_deltae_nb_prime(shared_ptr<FF_PDF> mbc, shared_ptr<FF_PDF> deltae, shared_ptr<FF_PDF> nb_prime, const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_nonres("pdf_bkg_bbar_nonres_3D_mbc_deltae_nb_prime", cat) {
		m_comb_pdfs.push_back(mbc);
		m_comb_pdfs.push_back(deltae);
		m_comb_pdfs.push_back(nb_prime);
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(), RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"), m_comb_pdfs.at(2)->GetPDF("C"))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};



class FF_PDF_bkg_bbar_wrongphikkbar_4D_mbc_deltae_chil0_ma_nb_prime: public FF_PDF_bkg_bbar_wrongphikkbar {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphikkbar_4D_mbc_deltae_chil0_ma_nb_prime() { }

	FF_PDF_bkg_bbar_wrongphikkbar_4D_mbc_deltae_chil0_ma_nb_prime(RooRealVar& mbc, RooRealVar& deltae,
			RooRealVar& chil0_ma, RooRealVar& nb_prime,
			const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_wrongphikkbar("pdf_bkg_bbar_wrongphikkbar_4D_mbc_deltae_chil0_ma_nb_prime", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_chil0_ma>(chil0_ma,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_nb_prime>(nb_prime,cat));

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
						m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"))));
		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
						m_comb_pdfs.at(2)->GetPDF("N"),m_comb_pdfs.at(3)->GetPDF("N"))));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}

	FF_PDF_bkg_bbar_wrongphikkbar_4D_mbc_deltae_chil0_ma_nb_prime(shared_ptr<FF_PDF> mbc, shared_ptr<FF_PDF> deltae,
			shared_ptr<FF_PDF> chil0_ma, shared_ptr<FF_PDF> nb_prime,
			const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_wrongphikkbar("pdf_bkg_bbar_wrongphikkbar_4D_mbc_deltae_chil0_ma_nb_prime", cat) {
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
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_bkg_bbar_wrongphif0980_4D_mbc_deltae_chil0_ma_nb_prime: public FF_PDF_bkg_bbar_wrongphif0980 {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphif0980_4D_mbc_deltae_chil0_ma_nb_prime() { }

	FF_PDF_bkg_bbar_wrongphif0980_4D_mbc_deltae_chil0_ma_nb_prime(RooRealVar& mbc, RooRealVar& deltae,
			RooRealVar& chil0_ma, RooRealVar& nb_prime,
			const FFUtil::tuple_category_name_flags& cat, bool is_data)
	: FF_PDF_bkg_bbar_wrongphif0980("pdf_bkg_bbar_wrongphif0980_4D_mbc_deltae_chil0_ma_nb_prime", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_chil0_ma>(chil0_ma,cat,is_data));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_nb_prime>(nb_prime,cat));

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
						m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"))));
		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
						m_comb_pdfs.at(2)->GetPDF("N"),m_comb_pdfs.at(3)->GetPDF("N"))));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}

	FF_PDF_bkg_bbar_wrongphif0980_4D_mbc_deltae_chil0_ma_nb_prime(shared_ptr<FF_PDF> mbc, shared_ptr<FF_PDF> deltae,
			shared_ptr<FF_PDF> chil0_ma, shared_ptr<FF_PDF> nb_prime,
			const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_wrongphif0980("pdf_bkg_bbar_wrongphif0980_4D_mbc_deltae_chil0_ma_nb_prime", cat) {
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
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_4D_mbc_deltae_chil0_ma_nb_prime : public FF_PDF_bkg_bbar {
public:
	virtual ~FF_PDF_bkg_bbar_4D_mbc_deltae_chil0_ma_nb_prime() { }

	FF_PDF_bkg_bbar_4D_mbc_deltae_chil0_ma_nb_prime(shared_ptr<FF_PDF> combinatoric, shared_ptr<FF_PDF> wrongmasshyp,
			shared_ptr<FF_PDF> wrongphikkbar, shared_ptr<FF_PDF> wrongphif0980, double nevents,
			const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar("pdf_bkg_bbar_4D_mbc_deltae_chil0_ma_nb_prime",cat) {

		if(dynamic_cast<FF_PDF_bkg_bbar_combinatoric_4D_mbc_deltae_chil0_ma_nb_prime*>(combinatoric.get())) {
			m_comb_pdfs.push_back(combinatoric);
		} else {
			std::cerr << "ERROR: PDF for combinatoric component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_combinatoric_4D_mbc_deltae_chil0_ma_nb_prime" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_wrongmasshyp_4D_mbc_deltae_chil0_ma_nb_prime*>(wrongmasshyp.get())) {
			m_comb_pdfs.push_back(wrongmasshyp);
		} else {
			std::cerr << "ERROR: PDF for wrongmasshyp component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_wrongmasshyp_4D_mbc_deltae_chil0_ma_nb_prime" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_wrongphikkbar_4D_mbc_deltae_chil0_ma_nb_prime*>(wrongphikkbar.get())) {
			m_comb_pdfs.push_back(wrongphikkbar);
		} else {
			std::cerr << "ERROR: PDF for wrongphikkbar component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_wrongphikkbar_4D_mbc_deltae_chil0_ma_nb_prime" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_wrongphif0980_4D_mbc_deltae_chil0_ma_nb_prime*>(wrongphif0980.get())) {
			m_comb_pdfs.push_back(wrongphif0980);
		} else {
			std::cerr << "ERROR: PDF for wrongphif0980 component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_wrongphif0980_4D_mbc_deltae_chil0_ma_nb_prime" << std::endl;
			exit(EXIT_FAILURE);
		}

		m_vars.push_back(new RooRealVar("C_bkg_bbar_combinatoric_nevents","C_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongmasshyp_nevents","C_bkg_bbar_wrongmasshyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongphikkbar_nevents","C_bkg_bbar_wrongphikkbar_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongphif0980_nevents","C_bkg_bbar_wrongphif0980_nevents",nevents/7.0,0.0,nevents*1.2));

		m_vars.push_back(new RooRealVar("N_bkg_bbar_combinatoric_nevents","N_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_bkg_bbar_wrongphikkbar_nevents","N_bkg_bbar_wrongphikkbar_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_bkg_bbar_wrongphif0980_nevents","N_bkg_bbar_wrongphif0980_nevents",nevents/7.0,0.0,nevents*1.2));

		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
				m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C")),
				RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3)),false));
		m_pdfs.push_back(new RooAddPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(2)->GetPDF("N"),
				m_comb_pdfs.at(3)->GetPDF("N")),
				RooArgList(*m_vars.at(4),*m_vars.at(5),*m_vars.at(6)),false));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}

	FF_PDF_bkg_bbar_4D_mbc_deltae_chil0_ma_nb_prime(RooRealVar& mbc, RooRealVar& deltae, RooRealVar& chil0_ma, RooRealVar& nb_prime,
			double nevents, const FFUtil::tuple_category_name_flags& cat, bool is_data, RooRealVar* resolution)
	: FF_PDF_bkg_bbar("pdf_bkg_bbar_4D_mbc_deltae_chil0_ma_nb_prime",cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_4D_mbc_deltae_chil0_ma_nb_prime>(
				mbc,deltae,chil0_ma,nb_prime,cat,is_data,resolution));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_4D_mbc_deltae_chil0_ma_nb_prime>(
				mbc,deltae,chil0_ma,nb_prime,cat,is_data,resolution));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_4D_mbc_deltae_chil0_ma_nb_prime>(
				mbc,deltae,chil0_ma,nb_prime,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_4D_mbc_deltae_chil0_ma_nb_prime>(
				mbc,deltae,chil0_ma,nb_prime,cat,is_data));

		m_vars.push_back(new RooRealVar("C_bkg_bbar_combinatoric_nevents","C_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongmasshyp_nevents","C_bkg_bbar_wrongmasshyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongphikkbar_nevents","C_bkg_bbar_wrongphikkbar_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongphif0980_nevents","C_bkg_bbar_wrongphif0980_nevents",nevents/7.0,0.0,nevents*1.2));

		m_vars.push_back(new RooRealVar("N_bkg_bbar_combinatoric_nevents","N_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_bkg_bbar_wrongphikkbar_nevents","N_bkg_bbar_wrongphikkbar_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_bkg_bbar_wrongphif0980_nevents","N_bkg_bbar_wrongphif0980_nevents",nevents/7.0,0.0,nevents*1.2));

		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
				m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C")),
				RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3)),false));
		m_pdfs.push_back(new RooAddPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(2)->GetPDF("N"),
				m_comb_pdfs.at(3)->GetPDF("N")),
				RooArgList(*m_vars.at(4),*m_vars.at(5),*m_vars.at(6)),false));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_3D_mbc_deltae_nb_prime : public FF_PDF_bkg_bbar {
public:
	virtual ~FF_PDF_bkg_bbar_3D_mbc_deltae_nb_prime() { }
    
	FF_PDF_bkg_bbar_3D_mbc_deltae_nb_prime(shared_ptr<FF_PDF> combinatoric, shared_ptr<FF_PDF> wrongmasshyp,
                                           shared_ptr<FF_PDF> wrongpi0hyp, shared_ptr<FF_PDF> missingfsp, shared_ptr<FF_PDF> nonres, double nevents,
                                           const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar("pdf_bkg_bbar_3D_mbc_deltae_nb_prime",cat) {
        
		if(dynamic_cast<FF_PDF_bkg_bbar_combinatoric_3D_mbc_deltae_nb_prime*>(combinatoric.get())) {
			m_comb_pdfs.push_back(combinatoric);
		} else {
			std::cerr << "ERROR: PDF for combinatoric component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_combinatoric_3D_mbc_deltae_nb_prime" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_wrongmasshyp_3D_mbc_deltae_nb_prime*>(wrongmasshyp.get())) {
			m_comb_pdfs.push_back(wrongmasshyp);
		} else {
			std::cerr << "ERROR: PDF for wrongmasshyp component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_wrongmasshyp_3D_mbc_deltae_nb_prime" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_wrongpi0hyp_3D_mbc_deltae_nb_prime*>(wrongpi0hyp.get())) {
			m_comb_pdfs.push_back(wrongpi0hyp);
		} else {
			std::cerr << "ERROR: PDF for wrongpi0hyp component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_wrongpi0hyp_3D_mbc_deltae_nb_prime" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_missingfsp_3D_mbc_deltae_nb_prime*>(missingfsp.get())) {
			m_comb_pdfs.push_back(missingfsp);
		} else {
			std::cerr << "ERROR: PDF for missingfsp component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_missingfsp_3D_mbc_deltae_nb_prime" << std::endl;
			exit(EXIT_FAILURE);
		}
        if(dynamic_cast<FF_PDF_bkg_bbar_nonres_3D_mbc_deltae_nb_prime*>(nonres.get())) {
			m_comb_pdfs.push_back(nonres);
		} else {
			std::cerr << "ERROR: PDF for nonres component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_nonres_3D_mbc_deltae_nb_prime" << std::endl;
			exit(EXIT_FAILURE);
		}
        
		m_vars.push_back(new RooRealVar("C_bkg_bbar_combinatoric_nevents","C_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongmasshyp_nevents","C_bkg_bbar_wrongmasshyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongpi0hyp_nevents","C_bkg_bbar_wrongpi0hyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_missingfsp_nevents","C_bkg_bbar_missingfsp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_nonres_nevents","C_bkg_bbar_nonres_nevents",nevents/7.0,0.0,nevents*1.2));
    
        
		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),m_comb_pdfs.at(4)->GetPDF("C")), RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4)),false));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
    
	FF_PDF_bkg_bbar_3D_mbc_deltae_nb_prime(RooRealVar& mbc, RooRealVar& deltae, RooRealVar& nb_prime,
                                           double nevents, const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar("pdf_bkg_bbar_3D_mbc_deltae_nb_prime",cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_3D_mbc_deltae_nb_prime>(mbc,deltae,nb_prime,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_3D_mbc_deltae_nb_prime>(mbc,deltae,nb_prime,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_3D_mbc_deltae_nb_prime>(mbc,deltae,nb_prime,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_missingfsp_3D_mbc_deltae_nb_prime>(mbc,deltae,nb_prime,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_nonres_3D_mbc_deltae_nb_prime>(mbc,deltae,nb_prime,cat));
        
		m_vars.push_back(new RooRealVar("C_bkg_bbar_combinatoric_nevents","C_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongmasshyp_nevents","C_bkg_bbar_wrongmasshyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongpi0hyp_nevents","C_bkg_bbar_wrongpi0hyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_missingfsp_nevents","C_bkg_bbar_missingfsp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_nonres_nevents","C_bkg_bbar_nonres_nevents",nevents/7.0,0.0,nevents*1.2));
        
        
		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),m_comb_pdfs.at(4)->GetPDF("C")), RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4)),false));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};
//------ DALIT PDFs ------
class FF_PDF_bkg_bbar_combinatoric_2D_dalitz : public FF_PDF_bkg_bbar_combinatoric {
public:
	virtual ~FF_PDF_bkg_bbar_combinatoric_2D_dalitz() { }
    
	FF_PDF_bkg_bbar_combinatoric_2D_dalitz(RooRealVar& mass_AB, RooRealVar& mass_BC, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_combinatoric("pdf_bkg_bbar_combinatoric_2D_dalitz",cat) {
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_Xsd_mean_11","mass_01_bkg_bbar_combinatoric_Xsd_mean_11",8.9046e+01,0,200));//V0
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_Xsd_mean_12","mass_01_bkg_bbar_combinatoric_Xsd_mean_12",-8.4201e+00,-20,20));//V1
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_Xsd_sigma_11","mass_01_bkg_bbar_combinatoric_Xsd_sigma_11",-7.2975e-01,-20,20));//V2
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_Xsd_sigma_12","mass_01_bkg_bbar_combinatoric_Xsd_sigma_12",9.0119e-01,0,20));//V3
        m_vars.back()->setConstant(true);
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_Xsd_border_1","mass_01_bkg_bbar_combinatoric_Xsd_border_1",1.2075e+00,0,5));//V4
        m_vars.back()->setConstant(true);
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_Xsd_c1","mass_01_bkg_bbar_combinatoric_Xsd_c1",-6.7997e-04,-1,1));//V5
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_Xsd_c2","mass_01_bkg_bbar_combinatoric_Xsd_c2",1.4521e-04,-1,1));//V6
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_Xsd_c3","mass_01_bkg_bbar_combinatoric_Xsd_c3",1.0551e-05,-1,1));//V7
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_Xsd_c4","mass_01_bkg_bbar_combinatoric_Xsd_c4",-2.6045e-06,-1,1));//V8
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzXsd("mass_01_bkg_bbar_combinatoric_Xsd_1","mass_01_bkg_bbar_combinatoric_Xsd_1",mass_AB,mass_BC,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5),*m_vars.at(6),*m_vars.at(7),*m_vars.at(8),23,m_Pi0,m_Pi,m_K,m_B0)); //P0
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_Xsd_mean_21","mass_01_bkg_bbar_combinatoric_Xsd_mean_21",1.0201e+00,0,200));//V9
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_Xsd_mean_22","mass_01_bkg_bbar_combinatoric_Xsd_mean_22",7.3298e-04,0,4));//V10
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_Xsd_sigma_21","mass_01_bkg_bbar_combinatoric_Xsd_sigma_21",-1.8807e+00,-20,20));//V11
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_Xsd_sigma_22","mass_01_bkg_bbar_combinatoric_Xsd_sigma_22",8.1342e+00 ,0,20));//V12
        m_vars.back()->setConstant(true);

        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_Xsd_border_2","mass_01_bkg_bbar_combinatoric_Xsd_border_2",1.1332e+00,0,5));//V13
        m_vars.back()->setConstant(true);
        
        m_pdfs.push_back(new RooDalitzXsd("mass_01_bkg_bbar_combinatoric_Xsd_2","mass_01_bkg_bbar_combinatoric_Xsd_2",mass_AB,mass_BC,*m_vars.at(9),*m_vars.at(10),*m_vars.at(11),*m_vars.at(12),*m_vars.at(13),*m_vars.at(5),*m_vars.at(6),*m_vars.at(7),*m_vars.at(8),23,m_Pi0,m_Pi,m_K,m_B0));//P1
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_Xsd_frac","mass_01_bkg_bbar_combinatoric_Xsd_frac",9.5678e-01,0,1));//V14
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooAddPdf("mass_01_bkg_bbar_combinatoric_Xsd","mass_01_bkg_bbar_combinatoric_Xsd",*m_pdfs.at(0),*m_pdfs.at(1),*m_vars.at(14))); //P2
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_Ks0_mean_1","mass_01_bkg_bbar_combinatoric_Ks0_mean_1", 8.9528e-01,0,4));//V15
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_Ks0_width_1","mass_01_bkg_bbar_combinatoric_Ks0_width_1",5.3000e-02,0,5));//V16
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_Ks0_c1","mass_01_bkg_bbar_combinatoric_Ks0_c1",1.2337e-01,-1,1));//V17
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_Ks0_c2","mass_01_bkg_bbar_combinatoric_Ks0_c2",-1.8029e-03,-1,1));//V18
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_Ks0_c3","mass_01_bkg_bbar_combinatoric_Ks0_c3",-2.4582e-04,-1,1));//V19
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_Ks0_c4","mass_01_bkg_bbar_combinatoric_Ks0_c4",1.4218e-06,-1,1));//V20
        m_vars.back()->setConstant(true);
        
        m_pdfs.push_back(new RooDalitzRelativisticSpinBreitWigner("mass_01_bkg_bbar_combinatoric_Ks0_RBW_1","mass_01_bkg_bbar_combinatoric_Ks0_RBW_1",mass_AB,mass_BC,*m_vars.at(15),*m_vars.at(16),*m_vars.at(17),*m_vars.at(18),*m_vars.at(19),*m_vars.at(20),5.3,1,23,m_Pi0,m_Pi,m_K,m_B0)); //P3
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_Ks20_mean_1","mass_01_bkg_bbar_combinatoric_Ks20_mean_1", 1.4254e+00,0,4));//V21
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_Ks20_width_1","mass_01_bkg_bbar_combinatoric_Ks20_width_1",1.0873e-01,0,5));//V22
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_Ks20_c1","mass_01_bkg_bbar_combinatoric_Ks20_c1",-4.1039e-01,-1,1));//V23
		m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_Ks20_c2","mass_01_bkg_bbar_combinatoric_Ks20_c2",7.1481e-02,-1,1));//V24
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_Ks20_c3","mass_01_bkg_bbar_combinatoric_Ks20_c3",1.3048e-03,-1,1));//V25
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_Ks20_c4","mass_01_bkg_bbar_combinatoric_Ks20_c4",-1.3544e-04,-1,1));//V26
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzRelativisticSpinBreitWigner("mass_01_bkg_bbar_combinatoric_Ks20_RBW_1","mass_01_bkg_bbar_combinatoric_Ks20_RBW_1",mass_AB,mass_BC,*m_vars.at(21),*m_vars.at(22),*m_vars.at(23),*m_vars.at(24),*m_vars.at(25),*m_vars.at(26),5.3,1,23,m_Pi0,m_Pi,m_K,m_B0)); //P4
        
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_D0_mean_1","mass_01_bkg_bbar_combinatoric_D0_mean_1",3.4899e+00,0,4));//V27
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_D0_sigma_1","mass_01_bkg_bbar_combinatoric_D0_sigma_1",2.5205e-02,0,5));//V28
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_D0_c1","mass_01_bkg_bbar_combinatoric_D0_c1",9.1203e-03,-1,1));//V29
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_D0_c2","mass_01_bkg_bbar_combinatoric_D0_c2",2.6142e-03,-1,1));//V30
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_D0_c3","mass_01_bkg_bbar_combinatoric_D0_c3",-3.2763e-05,-1,1));//V31
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_D0_c4","mass_01_bkg_bbar_combinatoric_D0_c4",-7.6631e-06,-1,1));//V32
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzGauss("mass_01_bkg_bbar_combinatoric_D0_Gauss_1","mass_01_bkg_bbar_combinatoric_D0_Gauss_1",mass_AB,mass_BC,*m_vars.at(27),*m_vars.at(28),*m_vars.at(29),*m_vars.at(30),*m_vars.at(31),*m_vars.at(32),23,m_Pi0,m_Pi,m_K,m_B0)); //P5
        /// THIS PART FROM HETE TO
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_D0_mean_2","mass_01_bkg_bbar_combinatoric_D0_mean_2",3.4981e+00,0,4));//V33
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_D0_sigma_2","mass_01_bkg_bbar_combinatoric_D0_sigma_2",4.2329e-02,0,5));//V34
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzGauss("mass_01_bkg_bbar_combinatoric_D0_Gauss_2","mass_01_bkg_bbar_combinatoric_D0_Gauss_2",mass_AB,mass_BC,*m_vars.at(33),*m_vars.at(34),*m_vars.at(29),*m_vars.at(30),*m_vars.at(31),*m_vars.at(32),23,m_Pi0,m_Pi,m_K,m_B0));//P1
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_combinatoric_D0_frac","mass_01_bkg_bbar_combinatoric_D0_frac",6.9295e-01,0,1));//V35
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooAddPdf("mass_01_bkg_bbar_combinatoric_D0","mass_01_bkg_bbar_combinatoric_D0",*m_pdfs.at(5),*m_pdfs.at(6),*m_vars.at(35))); //P7
        /// HERE IS USELESS
        
        m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_combinatoric_rhoP_mean_1","mass_02_bkg_bbar_combinatoric_rhoP_mean_1", 1.4686e+00,0,4));//V36
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_combinatoric_rhoP_width_1","mass_02_bkg_bbar_combinatoric_rhoP_width_1",2.8098e+00,0,5));//V37
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_combinatoric_rhoP_c1","mass_02_bkg_bbar_combinatoric_rhoP_c1",2.9092e-01,-1,1));//V38
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_combinatoric_rhoP_c2","mass_02_bkg_bbar_combinatoric_rhoP_c2",2.9093e-02,-1,1));//V39
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_combinatoric_rhoP_c3","mass_02_bkg_bbar_combinatoric_rhoP_c3",-1.0440e-03,-1,1));//V40
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_combinatoric_rhoP_c4","mass_02_bkg_bbar_combinatoric_rhoP_c4",-6.5294e-05,-1,1));//V41
        m_vars.back()->setConstant(true);
        
        m_pdfs.push_back(new RooDalitzRelativisticSpinBreitWigner("mass_02_bkg_bbar_combinatoric_rhoP_RBW_1","mass_02_bkg_bbar_combinatoric_rhoP_RBW_1",mass_AB,mass_BC,*m_vars.at(36),*m_vars.at(37),*m_vars.at(38),*m_vars.at(39),*m_vars.at(40),*m_vars.at(41),5.3,1,31,m_Pi0,m_Pi,m_K,m_B0)); //P8
       
        TFile *file= new TFile("plots/bbbar_combinatoric_background.root","READ");
        TH2F *combinatoric_pdf = dynamic_cast<TH2F*>(file->Get("data_combinatoric__00000000"));
        
        m_pdfs.push_back(new RooDalitzHist("bkg_bbar_combinatoric_nonres","bkg_bbar_combinatoric_nonres",mass_AB,mass_BC,combinatoric_pdf,m_B0,m_Pi0,m_Pi,m_K));//P9
        
        
        m_vars.push_back(new RooRealVar("bkg_bbar_combinatoric_frac1","bkg_bbar_combinatoric_frac1",0.9,0,1));//V42
        m_vars.push_back(new RooRealVar("bkg_bbar_combinatoric_frac2","bkg_bbar_combinatoric_frac2",0.01,0,1));//V43
        m_vars.push_back(new RooRealVar("bkg_bbar_combinatoric_frac3","bkg_bbar_combinatoric_frac3",0.01,0,1));//V44
        m_vars.push_back(new RooRealVar("bkg_bbar_combinatoric_frac4","bkg_bbar_combinatoric_frac4",0.01,0,1));//V45
        m_vars.push_back(new RooRealVar("bkg_bbar_combinatoric_frac5","bkg_bbar_combinatoric_frac5",0.01,0,1));//V45
        
        m_pdfs.push_back(new RooAddPdf("bkg_bbar_combinatoric","bkg_bbar_combinatoric",RooArgList(*m_pdfs.at(5),*m_pdfs.at(2),*m_pdfs.at(3),*m_pdfs.at(4),*m_pdfs.at(8),*m_pdfs.at(9)),RooArgList(*m_vars.at(42),*m_vars.at(43),*m_vars.at(44),*m_vars.at(45),*m_vars.at(46)),true)); //P10
        
        //m_pdfs.push_back(new RooAddPdf("bkg_bbar_combinatoric","bkg_bbar_combinatoric",RooArgList(*m_pdfs.at(2),*m_pdfs.at(3)),RooArgList(*m_vars.at(42)),true)); //P9
        
        
		m_main_pdfs["C"] = m_pdfs.at(10);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_bkg_bbar_wrongmasshyp_2D_dalitz : public FF_PDF_bkg_bbar_wrongmasshyp {
public:
	virtual ~FF_PDF_bkg_bbar_wrongmasshyp_2D_dalitz() { }
    
	FF_PDF_bkg_bbar_wrongmasshyp_2D_dalitz(RooRealVar& mass_AB, RooRealVar& mass_BC, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_wrongmasshyp("pdf_bkg_bbar_wrongmasshyp_2D_dalitz",cat) {
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongmasshyp_rho770_mean_1","mass_01_bkg_bbar_wrongmasshyp_rho770_mean_1", 9.3753e-01,0,4));//V0
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongmasshyp_rho770_width_1","mass_01_bkg_bbar_wrongmasshyp_rho770_width_1",1.8788e-01,0,5));//V1
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongmasshyp_rho770_c1","mass_01_bkg_bbar_wrongmasshyp_rho770_c1",-4.3599e-01,-1,1));//V2
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongmasshyp_rho770_c2","mass_01_bkg_bbar_wrongmasshyp_rho770_c2",7.2788e-02,-1,1));//V3
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongmasshyp_rho770_c3","mass_01_bkg_bbar_wrongmasshyp_rho770_c3",1.8729e-03,-1,1));//V4
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongmasshyp_rho770_c4","mass_01_bkg_bbar_wrongmasshyp_rho770_c4",-1.2032e-04,-1,1));//V5
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzRelativisticSpinBreitWigner("mass_01_bkg_bbar_wrongmasshyp_rho770_RBW_1","mass_01_bkg_bbar_wrongmasshyp_rho770_RBW_1",mass_AB,mass_BC,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5),5.3,1,23,m_Pi0,m_Pi,m_K,m_B0)); //P0
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongmasshyp_KS_mean_1","mass_01_bkg_bbar_wrongmasshyp_KS_mean_1",5.6144e-01,0,4));//V6
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongmasshyp_KS_sigma_1","mass_01_bkg_bbar_wrongmasshyp_KS_sigma_1",4.2994e-02,0,5));//V7
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongmasshyp_KS_c1","mass_01_bkg_bbar_wrongmasshyp_KS_c1",-2.5627e-01,-1,1));//V8
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongmasshyp_KS_c2","mass_01_bkg_bbar_wrongmasshyp_KS_c2",3.2197e-02,-1,1));//V9
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongmasshyp_KS_c3","mass_01_bkg_bbar_wrongmasshyp_KS_c3",-1.5791e-03,-1,1));//V10
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongmasshyp_KS_c4","mass_01_bkg_bbar_wrongmasshyp_KS_c4",2.3382e-05,-1,1));//V11
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzGauss("mass_01_bkg_bbar_wrongmasshyp_KS_Gauss_1","mass_01_bkg_bbar_wrongmasshyp_KS_Gauss_1",mass_AB,mass_BC,*m_vars.at(6),*m_vars.at(7),*m_vars.at(8),*m_vars.at(9),*m_vars.at(10),*m_vars.at(11),23,m_Pi0,m_Pi,m_K,m_B0)); //P1
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongmasshyp_KS_mean_2","mass_01_bkg_bbar_wrongmasshyp_KS_mean_2",6.2165e-01,0,4));//V12
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongmasshyp_KS_sigma_2","mass_01_bkg_bbar_wrongmasshyp_KS_sigma_2",1.9948e-01,0,5));//V13
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzGauss("mass_01_bkg_bbar_wrongmasshyp_KS_Gauss_2","mass_01_bkg_bbar_wrongmasshyp_KS_Gauss_2",mass_AB,mass_BC,*m_vars.at(12),*m_vars.at(13),*m_vars.at(8),*m_vars.at(9),*m_vars.at(10),*m_vars.at(11),23,m_Pi0,m_Pi,m_K,m_B0));//P2
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongmasshyp_KS_frac","mass_01_bkg_bbar_wrongmasshyp_KS_frac",4.4010e-01,0,1));//V14
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooAddPdf("mass_01_bkg_bbar_wrongmasshyp_KS","mass_01_bkg_bbar_wrongmasshyp_KS",*m_pdfs.at(1),*m_pdfs.at(2),*m_vars.at(14))); //P3
        
        
        //START MASS12 RESONANCES
        
        m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_wrongmasshyp_rhoP_mean_1","mass_12_bkg_bbar_wrongmasshyp_rhoP_mean_1", 7.6137e-01,0,4));//V15
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_wrongmasshyp_rhoP_width_1","mass_12_bkg_bbar_wrongmasshyp_rhoP_width_1",1.3957e-01,0,5));//V16
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_wrongmasshyp_rhoP_c1","mass_12_bkg_bbar_wrongmasshyp_rhoP_c1",-6.8624e-01,-1,1));//V17
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_wrongmasshyp_rhoP_c2","mass_12_bkg_bbar_wrongmasshyp_rhoP_c2",5.9915e-02,-1,1));//V18
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_wrongmasshyp_rhoP_c3","mass_12_bkg_bbar_wrongmasshyp_rhoP_c3",1.0140e-03,-1,1));//V19
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_wrongmasshyp_rhoP_c4","mass_12_bkg_bbar_wrongmasshyp_rhoP_c4",-7.4904e-05,-1,1));//V20
        m_vars.back()->setConstant(true);
        
        m_pdfs.push_back(new RooDalitzRelativisticSpinBreitWigner("mass_12_bkg_bbar_wrongmasshyp_rhoP_RBW_1","mass_12_bkg_bbar_wrongmasshyp_rhoP_RBW_1",mass_AB,mass_BC,*m_vars.at(15),*m_vars.at(16),*m_vars.at(17),*m_vars.at(18),*m_vars.at(19),*m_vars.at(20),5.3,1,12,m_Pi0,m_Pi,m_K,m_B0)); //P4
        
        
        m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_wrongmasshyp_Dplus_mean_1","mass_12_bkg_bbar_wrongmasshyp_Dplus_mean_1", 3.4015e+00,0,4));//V21
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_wrongmasshyp_Dplus_width_1","mass_12_bkg_bbar_wrongmasshyp_Dplus_width_1",3.8189e-02,0,5));//V22
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_wrongmasshyp_Dplus_c1","mass_12_bkg_bbar_wrongmasshyp_Dplus_c1",-1.9893e-02,-1,1));//V23
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_wrongmasshyp_Dplus_c2","mass_12_bkg_bbar_wrongmasshyp_Dplus_c2",1.9657e-02,-1,1));//V24
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_wrongmasshyp_Dplus_c3","mass_12_bkg_bbar_wrongmasshyp_Dplus_c3",-6.6412e-05,-1,1));//V25
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_wrongmasshyp_Dplus_c4","mass_12_bkg_bbar_wrongmasshyp_Dplus_c4",-4.1208e-05,-1,1));//V26
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzGauss("mass_12_bkg_bbar_wrongmasshyp_Dplus_Gauss_1","mass_12_bkg_bbar_wrongmasshyp_Dplus_Gauss_1",mass_AB,mass_BC,*m_vars.at(21),*m_vars.at(22),*m_vars.at(23),*m_vars.at(24),*m_vars.at(25),*m_vars.at(26),12,m_Pi0,m_Pi,m_K,m_B0)); //P5
        
        m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_wrongmasshyp_Dplus_mean_2","mass_12_bkg_bbar_wrongmasshyp_Dplus_mean_2", 3.1849e+00,0,4));//V27
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_wrongmasshyp_Dplus_width_2","mass_12_bkg_bbar_wrongmasshyp_Dplus_width_2",7.4693e-01,0,5));//V28
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzGauss("mass_12_bkg_bbar_wrongmasshyp_Dplus_Gauss_2","mass_12_bkg_bbar_wrongmasshyp_Dplus_Gauss_2",mass_AB,mass_BC,*m_vars.at(27),*m_vars.at(28),*m_vars.at(23),*m_vars.at(24),*m_vars.at(25),*m_vars.at(26),12,m_Pi0,m_Pi,m_K,m_B0)); //P6
        m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_wrongmasshyp_Dplus_frac","mass_12_bkg_bbar_wrongmasshyp_Dplus_frac",9.0893e-01,0,1));//V29
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooAddPdf("mass_12_bkg_bbar_wrongmasshyp_Dplus","mass_12_bkg_bbar_wrongmasshyp_Dplus",*m_pdfs.at(5),*m_pdfs.at(6),*m_vars.at(29))); //P7
        
        //START MASS02 RESONANCES

        m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_wrongmasshyp_rhoP_mean_1","mass_02_bkg_bbar_wrongmasshyp_rhoP_mean_1", 9.4307e-01,0,4));//V30
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_wrongmasshyp_rhoP_width_1","mass_02_bkg_bbar_wrongmasshyp_rhoP_width_1",1.6995e-01,0,5));//V31
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_wrongmasshyp_rhoP_c1","mass_02_bkg_bbar_wrongmasshyp_rhoP_c1",4.8736e-02,-2,2));//V32
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_wrongmasshyp_rhoP_c2","mass_02_bkg_bbar_wrongmasshyp_rhoP_c2",5.9314e-01,-2,2));//V33
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_wrongmasshyp_rhoP_c3","mass_02_bkg_bbar_wrongmasshyp_rhoP_c3",-8.1681e-04,-2,2));//V34
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_wrongmasshyp_rhoP_c4","mass_02_bkg_bbar_wrongmasshyp_rhoP_c4",-1.0585e-03,-2,2));//V35
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzRelativisticSpinBreitWigner("mass_02_bkg_bbar_wrongmasshyp_rhoP_RBW_1","mass_02_bkg_bbar_wrongmasshyp_rhoP_RBW_1",mass_AB,mass_BC,*m_vars.at(30),*m_vars.at(31),*m_vars.at(32),*m_vars.at(33),*m_vars.at(34),*m_vars.at(35),5.3,1,31,m_Pi0,m_Pi,m_K,m_B0)); //P8
        
        TFile *file= new TFile("plots/bbbar_wrongmasshyp_background.root","READ");
        TH2F *wrongmasshyp_pdf = dynamic_cast<TH2F*>(file->Get("data_wrongmasshyp__00000001"));
        
        m_pdfs.push_back(new RooDalitzHist("bkg_bbar_wrongmasshyp_nonres","bkg_bbar_wrongmasshyp_nonres",mass_AB,mass_BC,wrongmasshyp_pdf,m_B0,m_Pi0,m_Pi,m_K));//P9
        
        
        m_vars.push_back(new RooRealVar("bkg_bbar_wrongmasshyp_frac1","bkg_bbar_wrongmasshyp_frac1",0.5,0,1));//V36
        m_vars.push_back(new RooRealVar("bkg_bbar_wrongmasshyp_frac2","bkg_bbar_wrongmasshyp_frac2",0.001,0,1));//V37
        m_vars.push_back(new RooRealVar("bkg_bbar_wrongmasshyp_frac3","bkg_bbar_wrongmasshyp_frac3",0.001,0,1));//V38
        m_vars.push_back(new RooRealVar("bkg_bbar_wrongmasshyp_frac4","bkg_bbar_wrongmasshyp_frac4",0.001,0,1));//V39
        m_vars.push_back(new RooRealVar("bkg_bbar_wrongmasshyp_frac5","bkg_bbar_wrongmasshyp_frac5",0.001,0,1));//V40
        
        m_pdfs.push_back(new RooAddPdf("bkg_bbar_wrongmasshyp","bkg_bbar_wrongmasshyp",RooArgList(*m_pdfs.at(7),*m_pdfs.at(0),*m_pdfs.at(3),*m_pdfs.at(4),*m_pdfs.at(9)),RooArgList(*m_vars.at(36),*m_vars.at(37),*m_vars.at(38),*m_vars.at(39)),true)); //P10
        
        
		m_main_pdfs["C"] = m_pdfs.at(10);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_bkg_bbar_wrongpi0hyp_2D_dalitz : public FF_PDF_bkg_bbar_wrongpi0hyp {
public:
	virtual ~FF_PDF_bkg_bbar_wrongpi0hyp_2D_dalitz() { }
    
	FF_PDF_bkg_bbar_wrongpi0hyp_2D_dalitz(RooRealVar& mass_AB, RooRealVar& mass_BC, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_wrongpi0hyp("pdf_bkg_bbar_wrongpi0hyp_2D_dalitz",cat) {
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongpi0hyp_Xsd_mean_11","mass_01_bkg_bbar_wrongpi0hyp_Xsd_mean_11",7.2683e+01,0,200));//V0
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongpi0hyp_Xsd_mean_12","mass_01_bkg_bbar_wrongpi0hyp_Xsd_mean_12",-1.5829e+01,-20,20));//V1
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongpi0hyp_Xsd_sigma_11","mass_01_bkg_bbar_wrongpi0hyp_Xsd_sigma_11",-6.5639e-01,-20,20));//V2
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongpi0hyp_Xsd_sigma_12","mass_01_bkg_bbar_wrongpi0hyp_Xsd_sigma_12",2.1823e-02,0,20));//V3
        m_vars.back()->setConstant(true);
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongpi0hyp_Xsd_border_1","mass_01_bkg_bbar_wrongpi0hyp_Xsd_border_1",1.2075e+00,0,5));//V4
        m_vars.back()->setConstant(true);
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongpi0hyp_Xsd_c1","mass_01_bkg_bbar_wrongpi0hyp_Xsd_c1",-6.7997e-04,-1,1));//V5
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongpi0hyp_Xsd_c2","mass_01_bkg_bbar_wrongpi0hyp_Xsd_c2",1.4521e-04,-1,1));//V6
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongpi0hyp_Xsd_c3","mass_01_bkg_bbar_wrongpi0hyp_Xsd_c3",1.0551e-05,-1,1));//V7
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongpi0hyp_Xsd_c4","mass_01_bkg_bbar_wrongpi0hyp_Xsd_c4",-2.6045e-06,-1,1));//V8
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzXsd("mass_01_bkg_bbar_wrongpi0hyp_Xsd_1","mass_01_bkg_bbar_wrongpi0hyp_Xsd_1",mass_AB,mass_BC,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5),*m_vars.at(6),*m_vars.at(7),*m_vars.at(8),23,m_Pi0,m_Pi,m_K,m_B0)); //P0
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongpi0hyp_Xsd_mean_21","mass_01_bkg_bbar_wrongpi0hyp_Xsd_mean_21",1.9877e+00,0,200));//V9
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongpi0hyp_Xsd_mean_22","mass_01_bkg_bbar_wrongpi0hyp_Xsd_mean_22",1.7335e+00,0,4));//V10
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongpi0hyp_Xsd_sigma_21","mass_01_bkg_bbar_wrongpi0hyp_Xsd_sigma_21",-9.0335e-01,-20,20));//V11
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongpi0hyp_Xsd_sigma_22","mass_01_bkg_bbar_wrongpi0hyp_Xsd_sigma_22",8.4836e-01 ,0,20));//V12
        m_vars.back()->setConstant(true);
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongpi0hyp_Xsd_border_2","mass_01_bkg_bbar_wrongpi0hyp_Xsd_border_2",1.1332e+00,0,5));//V13
        m_vars.back()->setConstant(true);
        
        m_pdfs.push_back(new RooDalitzXsd("mass_01_bkg_bbar_wrongpi0hyp_Xsd_2","mass_01_bkg_bbar_wrongpi0hyp_Xsd_2",mass_AB,mass_BC,*m_vars.at(9),*m_vars.at(10),*m_vars.at(11),*m_vars.at(12),*m_vars.at(13),*m_vars.at(5),*m_vars.at(6),*m_vars.at(7),*m_vars.at(8),23,m_Pi0,m_Pi,m_K,m_B0));//P1
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongpi0hyp_Xsd_frac","mass_01_bkg_bbar_wrongpi0hyp_Xsd_frac",8.7990e-01,0,1));//V14
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooAddPdf("mass_01_bkg_bbar_wrongpi0hyp_Xsd","mass_01_bkg_bbar_wrongpi0hyp_Xsd",*m_pdfs.at(0),*m_pdfs.at(1),*m_vars.at(14))); //P2
        
        
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongpi0hyp_Ks0_mean_1","mass_01_bkg_bbar_wrongpi0hyp_Ks0_mean_1", 8.9503e-01,0,4));//V15
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongpi0hyp_Ks0_width_1","mass_01_bkg_bbar_wrongpi0hyp_Ks0_width_1",5.0726e-02,0,5));//V16
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongpi0hyp_Ks0_c1","mass_01_bkg_bbar_wrongpi0hyp_Ks0_c1",1.1898e-01,-1,1));//V17
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongpi0hyp_Ks0_c2","mass_01_bkg_bbar_wrongpi0hyp_Ks0_c2",-2.4262e-03,-1,1));//V18
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongpi0hyp_Ks0_c3","mass_01_bkg_bbar_wrongpi0hyp_Ks0_c3",-2.1272e-04,-1,1));//V19
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongpi0hyp_Ks0_c4","mass_01_bkg_bbar_wrongpi0hyp_Ks0_c4",1.1639e-06,-1,1));//V20
        m_vars.back()->setConstant(true);
        
        m_pdfs.push_back(new RooDalitzRelativisticSpinBreitWigner("mass_01_bkg_bbar_wrongpi0hyp_Ks0_RBW_1","mass_01_bkg_bbar_wrongpi0hyp_Ks0_RBW_1",mass_AB,mass_BC,*m_vars.at(15),*m_vars.at(16),*m_vars.at(17),*m_vars.at(18),*m_vars.at(19),*m_vars.at(20),5.3,1,23,m_Pi0,m_Pi,m_K,m_B0)); //P3
        
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongpi0hyp_Ks20_mean_1","mass_01_bkg_bbar_wrongpi0hyp_Ks20_mean_1", 1.4323e+00,0,4));//V21
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongpi0hyp_Ks20_width_1","mass_01_bkg_bbar_wrongpi0hyp_Ks20_width_1",1.1512e-01,0,5));//V22
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongpi0hyp_Ks20_c1","mass_01_bkg_bbar_wrongpi0hyp_Ks20_c1",-5.0970e-01,-1,1));//V23
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongpi0hyp_Ks20_c2","mass_01_bkg_bbar_wrongpi0hyp_Ks20_c2",1.0414e-01,-1,1));//V24
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongpi0hyp_Ks20_c3","mass_01_bkg_bbar_wrongpi0hyp_Ks20_c3",1.7053e-03,-1,1));//V25
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_wrongpi0hyp_Ks20_c4","mass_01_bkg_bbar_wrongpi0hyp_Ks20_c4",-1.9485e-04,-1,1));//V26
        m_vars.back()->setConstant(true);
        
        m_pdfs.push_back(new RooDalitzRelativisticSpinBreitWigner("mass_01_bkg_bbar_wrongpi0hyp_Ks20_RBW_1","mass_01_bkg_bbar_wrongpi0hyp_Ks20_RBW_1",mass_AB,mass_BC,*m_vars.at(21),*m_vars.at(22),*m_vars.at(23),*m_vars.at(24),*m_vars.at(25),*m_vars.at(26),5.3,1,23,m_Pi0,m_Pi,m_K,m_B0)); //P4
        
        m_vars.push_back(new RooRealVar("mass_01_wrongpi0hyp_D0_mean_1","mass_01_wrongpi0hyp_D0_mean_1",3.4921e+00,0,4));//V27
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_wrongpi0hyp_D0_sigma_1","mass_01_wrongpi0hyp_D0_sigma_1",2.2062e-02,0,5));//V28
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_wrongpi0hyp_D0_c1","mass_01_wrongpi0hyp_D0_c1",7.6157e-04,-1,1));//V29
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_wrongpi0hyp_D0_c2","mass_01_wrongpi0hyp_D0_c2",-4.5354e-04,-1,1));//V30
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_wrongpi0hyp_D0_c3","mass_01_wrongpi0hyp_D0_c3",2.8342e-05,-1,1));//V31
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_wrongpi0hyp_D0_c4","mass_01_wrongpi0hyp_D0_c4",-1.1652e-06,-1,1));//V32
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzGauss("mass_01_wrongpi0hyp_D0_Gauss_1","mass_01_wrongpi0hyp_D0_Gauss_1",mass_AB,mass_BC,*m_vars.at(27),*m_vars.at(28),*m_vars.at(29),*m_vars.at(30),*m_vars.at(31),*m_vars.at(32),23,m_Pi0,m_Pi,m_K,m_B0)); //P5
        
        m_vars.push_back(new RooRealVar("mass_01_wrongpi0hyp_D0_mean_2","mass_01_wrongpi0hyp_D0_mean_2",3.5170e+00,0,4));//V33
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_wrongpi0hyp_D0_sigma_2","mass_01_wrongpi0hyp_D0_sigma_2",7.6425e-02,0,5));//V34
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzGauss("mass_01_wrongpi0hyp_D0_Gauss_2","mass_01_wrongpi0hyp_D0_Gauss_2",mass_AB,mass_BC,*m_vars.at(33),*m_vars.at(34),*m_vars.at(29),*m_vars.at(30),*m_vars.at(31),*m_vars.at(32),23,m_Pi0,m_Pi,m_K,m_B0));//P1
        
        m_vars.push_back(new RooRealVar("mass_01_wrongpi0hyp_D0_frac","mass_01_wrongpi0hyp_D0_frac",8.6046e-01,0,1));//V35
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooAddPdf("mass_01_wrongpi0hyp_D0","mass_01_wrongpi0hyp_D0",*m_pdfs.at(5),*m_pdfs.at(6),*m_vars.at(35))); //P7
        
        
        TFile *file= new TFile("plots/bbbar_wrongpi0hyp_background.root","READ");
        TH2F *wrongpi0hyp_pdf = dynamic_cast<TH2F*>(file->Get("data_wrongpi0hyp__00000002"));
        
        m_pdfs.push_back(new RooDalitzHist("bkg_bbar_wrongpi0hyp_nonres","bkg_bbar_wrongpi0hyp_nonres",mass_AB,mass_BC,wrongpi0hyp_pdf,m_B0,m_Pi0,m_Pi,m_K));//P8
        
        m_vars.push_back(new RooRealVar("bkg_bbar_wrongpi0hyp_frac1","bkg_bbar_wrongpi0hyp_frac1",0.5,0,1));//V36
        m_vars.push_back(new RooRealVar("bkg_bbar_wrongpi0hyp_frac2","bkg_bbar_wrongpi0hyp_frac2",0.5,0,1));//V37
        m_vars.push_back(new RooRealVar("bkg_bbar_wrongpi0hyp_frac3","bkg_bbar_wrongpi0hyp_frac3",0.5,0,1));//V38
        m_vars.push_back(new RooRealVar("bkg_bbar_wrongpi0hyp_frac4","bkg_bbar_wrongpi0hyp_frac4",0.5,0,1));//V39
        m_vars.push_back(new RooRealVar("bkg_bbar_wrongpi0hyp_frac5","bkg_bbar_wrongpi0hyp_frac5",0.5,0,1));//V40
        
        m_pdfs.push_back(new RooAddPdf("bkg_bbar_wrongpi0hyp","bkg_bbar_wrongpi0hyp",RooArgList(*m_pdfs.at(7),*m_pdfs.at(2),*m_pdfs.at(3),*m_pdfs.at(4),*m_pdfs.at(8)),RooArgList(*m_vars.at(36),*m_vars.at(37),*m_vars.at(38),*m_vars.at(39)),true)); //P9
        // m_pdfs.push_back(new RooAddPdf("bkg_bbar_combinatoric","bkg_bbar_combinatoric",RooArgList(*m_pdfs.at(4),*m_pdfs.at(7)),RooArgList(*m_vars.at(44)),true)); //P12
        
        
		m_main_pdfs["C"] = m_pdfs.at(9);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_bkg_bbar_missingfsp_2D_dalitz : public FF_PDF_bkg_bbar_missingfsp {
public:
	virtual ~FF_PDF_bkg_bbar_missingfsp_2D_dalitz() { }
    
	FF_PDF_bkg_bbar_missingfsp_2D_dalitz(RooRealVar& mass_AB, RooRealVar& mass_BC, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_missingfsp("pdf_bkg_bbar_missingfsp_2D_dalitz",cat) {

        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_Xsd_mean_11","mass_01_bkg_bbar_missingfsp_Xsd_mean_11",6.0656e+01,0,200));//V0
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_Xsd_mean_12","mass_01_bkg_bbar_missingfsp_Xsd_mean_12",-1.5555e+01,-20,20));//V1
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_Xsd_sigma_11","mass_01_bkg_bbar_missingfsp_Xsd_sigma_11",-8.3830e-01,-20,20));//V2
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_Xsd_sigma_12","mass_01_bkg_bbar_missingfsp_Xsd_sigma_12",2.1824e-02,0,20));//V3
        m_vars.back()->setConstant(true);
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_Xsd_border_1","mass_01_bkg_bbar_missingfsp_Xsd_border_1",1.2075e+00,0,5));//V4
        m_vars.back()->setConstant(true);
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_Xsd_c1","mass_01_bkg_bbar_missingfsp_Xsd_c1",3.0685e-02,-1,1));//V5
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_Xsd_c2","mass_01_bkg_bbar_missingfsp_Xsd_c2",-3.2303e-04,-1,1));//V6
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_Xsd_c3","mass_01_bkg_bbar_missingfsp_Xsd_c3",-6.0379e-05,-1,1));//V7
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_Xsd_c4","mass_01_bkg_bbar_missingfsp_Xsd_c4",-1.9119e-06,-1,1));//V8
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzXsd("mass_01_bkg_bbar_missingfsp_Xsd_1","mass_01_bkg_bbar_missingfsp_Xsd_1",mass_AB,mass_BC,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5),*m_vars.at(6),*m_vars.at(7),*m_vars.at(8),23,m_Pi0,m_Pi,m_K,m_B0)); //P0
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_Xsd_mean_21","mass_01_bkg_bbar_missingfsp_Xsd_mean_21",1.7718e+00,0,200));//V9
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_Xsd_mean_22","mass_01_bkg_bbar_missingfsp_Xsd_mean_22",7.1017e-01,0,4));//V10
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_Xsd_sigma_21","mass_01_bkg_bbar_missingfsp_Xsd_sigma_21",-1.2556e+00,-20,20));//V11
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_Xsd_sigma_22","mass_01_bkg_bbar_missingfsp_Xsd_sigma_22",2.4851e+00 ,0,20));//V12
        m_vars.back()->setConstant(true);
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_Xsd_border_2","mass_01_bkg_bbar_missingfsp_Xsd_border_2",1.1332e+00,0,5));//V13
        m_vars.back()->setConstant(true);
        
        m_pdfs.push_back(new RooDalitzXsd("mass_01_bkg_bbar_missingfsp_Xsd_2","mass_01_bkg_bbar_missingfsp_Xsd_2",mass_AB,mass_BC,*m_vars.at(9),*m_vars.at(10),*m_vars.at(11),*m_vars.at(12),*m_vars.at(13),*m_vars.at(5),*m_vars.at(6),*m_vars.at(7),*m_vars.at(8),23,m_Pi0,m_Pi,m_K,m_B0));//P1
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_Xsd_frac","mass_01_bkg_bbar_missingfsp_Xsd_frac",8.1871e-01,0,1));//V14
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooAddPdf("mass_01_bkg_bbar_missingfsp_Xsd","mass_01_bkg_bbar_missingfsp_Xsd",*m_pdfs.at(0),*m_pdfs.at(1),*m_vars.at(14))); //P2
        
        
        
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_Ks0_mean_1","mass_01_bkg_bbar_missingfsp_Ks0_mean_1", 8.9660e-01,0,4));//V15
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_Ks0_width_1","mass_01_bkg_bbar_missingfsp_Ks0_width_1",5.5393e-02,0,5));//V16
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_Ks0_c1","mass_01_bkg_bbar_missingfsp_Ks0_c1",-3.5109e-03,-1,1));//V17
		m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_Ks0_c2","mass_01_bkg_bbar_missingfsp_Ks0_c2",2.1114e-03,-1,1));//V18
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_Ks0_c3","mass_01_bkg_bbar_missingfsp_Ks0_c3",6.6602e-05,-1,1));//V19
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_Ks0_c4","mass_01_bkg_bbar_missingfsp_Ks0_c4",-7.4820e-06,-1,1));//V20
        m_vars.back()->setConstant(true);
        
        m_pdfs.push_back(new RooDalitzRelativisticSpinBreitWigner("mass_01_bkg_bbar_missingfsp_Ks0_RBW_1","mass_01_bkg_bbar_missingfsp_Ks0_RBW_1",mass_AB,mass_BC,*m_vars.at(15),*m_vars.at(16),*m_vars.at(17),*m_vars.at(18),*m_vars.at(19),*m_vars.at(20),5.3,1,23,m_Pi0,m_Pi,m_K,m_B0)); //P3
        

        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_Ks20_mean_1","mass_01_bkg_bbar_missingfsp_Ks20_mean_1", 1.4323e+00,0,4));//V21
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_Ks20_width_1","mass_01_bkg_bbar_missingfsp_Ks20_width_1",1.1512e-01,0,5));//V22
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_Ks20_c1","mass_01_bkg_bbar_missingfsp_Ks20_c1",-3.2061e-01,-1,1));//V23
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_Ks20_c2","mass_01_bkg_bbar_missingfsp_Ks20_c2",4.7125e-02,-1,1));//V24
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_Ks20_c3","mass_01_bkg_bbar_missingfsp_Ks20_c3",1.3346e-03,-1,1));//V25
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_Ks20_c4","mass_01_bkg_bbar_missingfsp_Ks20_c4",-1.0457e-04,-1,1));//V26
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzRelativisticSpinBreitWigner("mass_01_bkg_bbar_missingfsp_Ks20_RBW_1","mass_01_bkg_bbar_missingfsp_Ks20_RBW_1",mass_AB,mass_BC,*m_vars.at(21),*m_vars.at(22),*m_vars.at(23),*m_vars.at(24),*m_vars.at(25),*m_vars.at(26),5.3,1,23,m_Pi0,m_Pi,m_K,m_B0)); //P4
        
    
        m_vars.push_back(new RooRealVar("mass_01_missingfsp_D0_mean_1","mass_01_missingfsp_D0_mean_1",3.5053,0,4));//V27
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_missingfsp_D0_sigma_1","mass_01_missingfsp_D0_sigma_1",2.2273e-02,0,5));//V28
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_missingfsp_D0_c1","mass_01_missingfsp_D0_c1",4.0238e-04,-1,1));//V29
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_missingfsp_D0_c2","mass_01_missingfsp_D0_c2",6.5695e-04,-1,1));//V30
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_missingfsp_D0_c3","mass_01_missingfsp_D0_c3",1.1656e-05,-1,1));//V31
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_missingfsp_D0_c4","mass_01_missingfsp_D0_c4",-2.9386e-06,-1,1));//V32
        m_vars.back()->setConstant(true);
        
        m_pdfs.push_back(new RooDalitzGauss("mass_01_missingfsp_D0_Gauss_1","mass_01_missingfsp_D0_Gauss_1",mass_AB,mass_BC,*m_vars.at(27),*m_vars.at(28),*m_vars.at(29),*m_vars.at(30),*m_vars.at(31),*m_vars.at(32),23,m_Pi0,m_Pi,m_K,m_B0)); //P5
        
        m_vars.push_back(new RooRealVar("mass_01_missingfsp_D0_mean_2","mass_01_missingfsp_D0_mean_2",3.5477,0,4));//V33
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_missingfsp_D0_sigma_2","mass_01_missingfsp_D0_sigma_2",9.2575e-02,0,5));//V34
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_missingfsp_D0_tail_2","mass_01_missingfsp_D0_tail_2",7.4160e-02,-1,1));//V35
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzNovosibirsk("mass_01_missingfsp_D0_Gauss_2","mass_01_missingfsp_D0_Gauss_2",mass_AB,mass_BC,*m_vars.at(33),*m_vars.at(34),*m_vars.at(35),*m_vars.at(29),*m_vars.at(30),*m_vars.at(31),*m_vars.at(32),23,m_Pi0,m_Pi,m_K,m_B0));//P6
        
        m_vars.push_back(new RooRealVar("mass_01_missingfsp_D0_mean_3","mass_01_missingfsp_D0_mean_3",3.5300,0,4));//V36
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_missingfsp_D0_sigma_3","mass_01_missingfsp_D0_sigma_3",3.7862e-02,0,5));//V37
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_missingfsp_D0_tail_3","mass_01_missingfsp_D0_tail_3",-2.4189e-02,-1,1));//V38
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzNovosibirsk("mass_01_missingfsp_D0_Gauss_3","mass_01_missingfsp_D0_Gauss_3",mass_AB,mass_BC,*m_vars.at(36),*m_vars.at(37),*m_vars.at(38),*m_vars.at(29),*m_vars.at(30),*m_vars.at(31),*m_vars.at(32),23,m_Pi0,m_Pi,m_K,m_B0));//P7

        
        m_vars.push_back(new RooRealVar("mass_01_missingfsp_D0_mean_4","mass_01_missingfsp_D0_mean_4",3.4998,0,4));//V39
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_missingfsp_D0_sigma_4","mass_01_missingfsp_D0_sigma_4",1.4973e-02,0,5));//V40
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_missingfsp_D0_tail_4","mass_01_missingfsp_D0_tail_4",2.9552e-03 ,-1,1));//V41
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzNovosibirsk("mass_01_missingfsp_D0_Gauss_4","mass_01_missingfsp_D0_Gauss_3",mass_AB,mass_BC,*m_vars.at(39),*m_vars.at(40),*m_vars.at(41),*m_vars.at(29),*m_vars.at(30),*m_vars.at(31),*m_vars.at(32),23,m_Pi0,m_Pi,m_K,m_B0));//P8
        
        m_vars.push_back(new RooRealVar("mass_01_missingfsp_D0_frac","mass_01_missingfsp_D0_frac",6.4235e-01,0,1));//V42
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_missingfsp_D0_frac2","mass_01_missingfsp_D0_frac2",2.6335e-01,0,1));//V43
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_missingfsp_D0_frac3","mass_01_missingfsp_D0_frac3",4.5527e-01,0,1));//V44
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooAddPdf("mass_01_missingfsp_D0","mass_01_missingfsp_D0",RooArgList(*m_pdfs.at(5),*m_pdfs.at(6),*m_pdfs.at(7),*m_pdfs.at(8)),RooArgList(*m_vars.at(42),*m_vars.at(43),*m_vars.at(44)),true)); //P9
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_rho770_mean_1","mass_01_bkg_bbar_missingfsp_rho770_mean_1", 9.4528e-01,0,4));//V45
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_rho770_width_1","mass_01_bkg_bbar_missingfsp_rho770_width_1",1.9607e-01,0,5));//V46
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_rho770_c1","mass_01_bkg_bbar_missingfsp_rho770_c1",-7.5918e-01,-1,1));//V37
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_rho770_c2","mass_01_bkg_bbar_missingfsp_rho770_c2",2.0893e-01,-1,1));//V48
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_rho770_c3","mass_01_bkg_bbar_missingfsp_rho770_c3",1.0168e-04,-1,1));//V49
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_missingfsp_rho770_c4","mass_01_bkg_bbar_missingfsp_rho770_c4",-2.1613e-04,-1,1));//V50
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzRelativisticSpinBreitWigner("mass_01_bkg_bbar_missingfsp_rho770_RBW_1","mass_01_bkg_bbar_missingfsp_rho770_RBW_1",mass_AB,mass_BC,*m_vars.at(45),*m_vars.at(46),*m_vars.at(47),*m_vars.at(48),*m_vars.at(49),*m_vars.at(50),5.3,1,23,m_Pi0,m_Pi,m_K,m_B0)); //P10
        
        
        //START MASS12 RESONANCES
    
        m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_missingfsp_rhoP_mean_1","mass_12_bkg_bbar_missingfsp_rhoP_mean_1", 7.9760e-01,0,4));//V51
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_missingfsp_rhoP_width_1","mass_12_bkg_bbar_missingfsp_rhoP_width_1",1.5375e-01,0,5));//V52
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_missingfsp_rhoP_c1","mass_12_bkg_bbar_missingfsp_rhoP_c1",-4.7626e-01,-1,1));//V53
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_missingfsp_rhoP_c2","mass_12_bkg_bbar_missingfsp_rhoP_c2",5.2181e-02,-1,1));//V54
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_missingfsp_rhoP_c3","mass_12_bkg_bbar_missingfsp_rhoP_c3",6.3422e-04,-1,1));//V55
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_missingfsp_rhoP_c4","mass_12_bkg_bbar_missingfsp_rhoP_c4",-5.7493e-05,-1,1));//V56
        m_vars.back()->setConstant(true);
        
        m_pdfs.push_back(new RooDalitzRelativisticSpinBreitWigner("mass_12_bkg_bbar_missingfsp_rhoP_RBW_1","mass_12_bkg_bbar_missingfsp_rhoP_RBW_1",mass_AB,mass_BC,*m_vars.at(51),*m_vars.at(52),*m_vars.at(53),*m_vars.at(54),*m_vars.at(55),*m_vars.at(56),5.3,1,12,m_Pi0,m_Pi,m_K,m_B0)); //P11
        
        m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_missingfsp_Dplus_mean_1","mass_12_bkg_bbar_missingfsp_Dplus_mean_1", 3.7545e+00,0,4.5));//V57
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_missingfsp_Dplus_width_1","mass_12_bkg_bbar_missingfsp_Dplus_width_1",9.8274e-02,0,5));//V58
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_missingfsp_Dplus_c1","mass_12_bkg_bbar_missingfsp_Dplus_c1",-1.7119e-02,-1,1));//V59
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_missingfsp_Dplus_c2","mass_12_bkg_bbar_missingfsp_Dplus_c2",-1.4896e-03,-1,1));//V60
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_missingfsp_Dplus_c3","mass_12_bkg_bbar_missingfsp_Dplus_c3",1.6477e-05,-1,1));//V61
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_missingfsp_Dplus_c4","mass_12_bkg_bbar_missingfsp_Dplus_c4",1.2415e-06,-1,1));//V62
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzGauss("mass_12_bkg_bbar_missingfsp_Dplus_Gauss_1","mass_12_bkg_bbar_missingfsp_Dplus_Gauss_1",mass_AB,mass_BC,*m_vars.at(57),*m_vars.at(58),*m_vars.at(59),*m_vars.at(60),*m_vars.at(61),*m_vars.at(62),12,m_Pi0,m_Pi,m_K,m_B0)); //P12
        
        
        //RESONANCE MASS_02
        

        m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_missingfsp_rhoP_mean_1","mass_02_bkg_bbar_missingfsp_rhoP_mean_1", 9.5150e-01,0,4));//V63
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_missingfsp_rhoP_width_1","mass_02_bkg_bbar_missingfsp_rhoP_width_1",2.2168e-01,0,5));//V64
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_missingfsp_rhoP_c1","mass_02_bkg_bbar_missingfsp_rhoP_c1",-9.9954e-01,-1,1));//V65
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_missingfsp_rhoP_c2","mass_02_bkg_bbar_missingfsp_rhoP_c2",7.2000e-01,-1,1));//V66
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_missingfsp_rhoP_c3","mass_02_bkg_bbar_missingfsp_rhoP_c3",1.5153e-02,-1,1));//V67
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_missingfsp_rhoP_c4","mass_02_bkg_bbar_missingfsp_rhoP_c4",-5.9580e-04,-1,1));//V68
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzRelativisticSpinBreitWigner("mass_02_bkg_bbar_missingfsp_rhoP_RBW_1","mass_02_bkg_bbar_missingfsp_rhoP_RBW_1",mass_AB,mass_BC,*m_vars.at(63),*m_vars.at(64),*m_vars.at(65),*m_vars.at(66),*m_vars.at(67),*m_vars.at(68),5.3,1,31,m_Pi0,m_Pi,m_K,m_B0)); //P13
        
        m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_missingfsp_rhoP_mean_2","mass_01_missingfsp_rhoP_mean_2",1.2068e+00,0,4));//V69
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_missingfsp_rhoP_sigma_2","mass_01_missingfsp_rhoP_sigma_2",2.3663e-01,0,5));//V70
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_missingfsp_rhoP_tail_2","mass_02_missingfsp_rhoP_tail_2",0,-5,5));//V71
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzNovosibirsk("mass_02_bkg_bbar_missingfsp_rhoP_Gauss_2","mass_01_missingfsp_rhoP_Gauss_2",mass_AB,mass_BC,*m_vars.at(69),*m_vars.at(70),*m_vars.at(71),*m_vars.at(65),*m_vars.at(66),*m_vars.at(67),*m_vars.at(68),31,m_Pi0,m_Pi,m_K,m_B0));//P14
        
        m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_missingfsp_rhoP_mean_3","mass_01_missingfsp_rhoP_mean_3",2.8370e+00,0,4));//V72
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_missingfsp_rhoP_sigma_3","mass_01_missingfsp_rhoP_sigma_3",1.3140e+00,0,5));//V73
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_missingfsp_rhoP_tail_3","mass_02_missingfsp_rhoP_tail_3",0,-5,5));//V74
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzNovosibirsk("mass_02_bkg_bbar_missingfsp_rhoP_Gauss_3","mass_01_missingfsp_rhoP_Gauss_3",mass_AB,mass_BC,*m_vars.at(72),*m_vars.at(73),*m_vars.at(74),*m_vars.at(65),*m_vars.at(66),*m_vars.at(67),*m_vars.at(68),31,m_Pi0,m_Pi,m_K,m_B0));//P15
        
        
        m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_missingfsp_rhoP_frac","mass_02_missingfsp_rhoP_frac",7.1073e-01,0,1));//V75
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_missingfsp_rhoP_frac2","mass_02_missingfsp_rhoP_frac2",9.7277e-01,0,1));//V76
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooAddPdf("mass_02_bkg_bbar_missingfsp_rhoP","mass_02_missingfsp_rhoP",RooArgList(*m_pdfs.at(13),*m_pdfs.at(14),*m_pdfs.at(15)),RooArgList(*m_vars.at(75),*m_vars.at(76)),true)); //P16
        
        
        
        m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_missingfsp_KsP_mean_1","mass_02_bkg_bbar_missingfsp_KsP_mean_1", 9.1577e-01,0,4));//V77
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_missingfsp_KsP_width_1","mass_02_bkg_bbar_missingfsp_KsP_width_1",5.7900e-02,0,5));//V78
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_missingfsp_KsP_c1","mass_02_bkg_bbar_missingfsp_KsP_c1",8.5660e-02,-1,1));//V79
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_missingfsp_KsP_c2","mass_02_bkg_bbar_missingfsp_KsP_c2",3.4602e-03,-1,1));//V80
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_missingfsp_KsP_c3","mass_02_bkg_bbar_missingfsp_KsP_c3",-1.6514e-04,-1,1));//V81
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_02_bkg_bbar_missingfsp_KsP_c4","mass_02_bkg_bbar_missingfsp_KsP_c4",-1.1140e-05,-1,1));//V82
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzRelativisticSpinBreitWigner("mass_02_bkg_bbar_missingfsp_KsP_RBW_1","mass_02_bkg_bbar_missingfsp_KsP_RBW_1",mass_AB,mass_BC,*m_vars.at(77),*m_vars.at(78),*m_vars.at(79),*m_vars.at(80),*m_vars.at(81),*m_vars.at(82),5.3,1,31,m_Pi0,m_Pi,m_K,m_B0)); //P17
        
        TFile *file= new TFile("plots/bbbar_missingfsp_background.root","READ");
        TH2F *missingfsp_pdf = dynamic_cast<TH2F*>(file->Get("data_missingfsp__00000003"));
        
        m_pdfs.push_back(new RooDalitzHist("bkg_bbar_missingfsp_nonres","bkg_bbar_missingfsp_nonres",mass_AB,mass_BC,missingfsp_pdf,m_B0,m_Pi0,m_Pi,m_K));//P18
        
        
        m_vars.push_back(new RooRealVar("bkg_bbar_missingfsp_frac1","bkg_bbar_missingfsp_frac1",0.5,0,1));//V83
        m_vars.push_back(new RooRealVar("bkg_bbar_missingfsp_frac2","bkg_bbar_missingfsp_frac2",0.5,0,1));//V84
        m_vars.push_back(new RooRealVar("bkg_bbar_missingfsp_frac3","bkg_bbar_missingfsp_frac3",0.5,0,1));//V85
        m_vars.push_back(new RooRealVar("bkg_bbar_missingfsp_frac4","bkg_bbar_missingfsp_frac4",0.5,0,1));//V86
        m_vars.push_back(new RooRealVar("bkg_bbar_missingfsp_frac5","bkg_bbar_missingfsp_frac5",0.5,0,1));//V87
        m_vars.push_back(new RooRealVar("bkg_bbar_missingfsp_frac6","bkg_bbar_missingfsp_frac6",0.5,0,1));//V88
        m_vars.push_back(new RooRealVar("bkg_bbar_missingfsp_frac7","bkg_bbar_missingfsp_frac7",0.5,0,1));//V89
        m_vars.push_back(new RooRealVar("bkg_bbar_missingfsp_frac8","bkg_bbar_missingfsp_frac8",0.5,0,1));//V90
        m_vars.push_back(new RooRealVar("bkg_bbar_missingfsp_frac9","bkg_bbar_missingfsp_frac9",0.5,0,1));//V91
        
        RooArgList pdf_list_missingfsp(*m_pdfs.at(9),*m_pdfs.at(3),*m_pdfs.at(4),*m_pdfs.at(10),*m_pdfs.at(11),*m_pdfs.at(12));
        pdf_list_missingfsp.add(*m_pdfs.at(16));
        pdf_list_missingfsp.add(*m_pdfs.at(17));
        pdf_list_missingfsp.add(*m_pdfs.at(18));
        
        RooArgList pdf_coefs_missingfsp(*m_vars.at(83),*m_vars.at(84),*m_vars.at(85),*m_vars.at(86),*m_vars.at(87),*m_vars.at(88),*m_vars.at(89));
        pdf_coefs_missingfsp.add(*m_vars.at(90));
        
        m_pdfs.push_back(new RooAddPdf("bkg_bbar_missingfsp","bkg_bbar_missingfsp",pdf_list_missingfsp,pdf_coefs_missingfsp,true)); //P19
        
        
		m_main_pdfs["C"] = m_pdfs.at(19);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_nonres_2D_dalitz : public FF_PDF_bkg_bbar_nonres {
public:
	virtual ~FF_PDF_bkg_bbar_nonres_2D_dalitz() { }
    
	FF_PDF_bkg_bbar_nonres_2D_dalitz(RooRealVar& mass_AB, RooRealVar& mass_BC, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_nonres("pdf_bkg_bbar_nonres_2D_dalitz",cat) {
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_nonres_D0_mean_1","mass_01_bkg_bbar_nonres_D0_mean_1",3.4826e+00,3,4));//V0
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_nonres_D0_sigma_1","mass_01_bkg_bbar_nonres_D0_sigma_1",2.3167e-02,0,0.5));//V1
        m_vars.back()->setConstant(true);
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_nonres_D0_c1","mass_01_bkg_bbar_nonres_D0_c1",2.6720e-03,-1,1));//V2
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_nonres_D0_c2","mass_01_bkg_bbar_nonres_D0_c2",2.9087e-03,-1,1));//V3
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_nonres_D0_c3","mass_01_bkg_bbar_nonres_D0_c3",4.7150e-06,-1,1));//V4
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_nonres_D0_c4","mass_01_bkg_bbar_nonres_D0_c4",-8.7444e-06,-1,1));//V5
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzGauss("mass_01_bkg_bbar_nonres_D0_Gauss_1","mass_01_bkg_bbar_nonres_D0_Gauss_1",mass_AB,mass_BC,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5),23,m_Pi0,m_Pi,m_K,m_B0)); //P0
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_nonres_D0_mean_2","mass_01_bkg_bbar_nonres_D0_mean_2",3.4762e+00,0,4));//V6
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_nonres_D0_sigma_2","mass_01_bkg_bbar_nonres_D0_sigma_2",1.2692e-02,0,0.5));//V7
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_nonres_D0_tail_2","mass_01_bkg_bbar_nonres_D0_tail_2",4.5937e-04,0,5));//V8
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzNovosibirsk("mass_01_bkg_bbar_nonres_D0_Gauss_2","mass_01_bkg_bbar_nonres_D0_Gauss_2",mass_AB,mass_BC,*m_vars.at(6),*m_vars.at(7),*m_vars.at(8),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5),23,m_Pi0,m_Pi,m_K,m_B0));//P1
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_nonres_D0_mean_3","mass_01_bkg_bbar_nonres_D0_mean_3",3.4804e+00,0,4));//V9
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_nonres_D0_sigma_3","mass_01_bkg_bbar_nonres_D0_sigma_3",7.1243e-02,0,0.5));//V10
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_nonres_D0_tail_3","mass_01_bkg_bbar_nonres_D0_tail_3",8.2233e-02,0,5));//V11
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzNovosibirsk("mass_01_bkg_bbar_nonres_D0_Gauss_3","mass_01_bkg_bbar_nonres_D0_Gauss_3",mass_AB,mass_BC,*m_vars.at(9),*m_vars.at(10),*m_vars.at(11),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5),23,m_Pi0,m_Pi,m_K,m_B0));//P2
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_nonres_D0_mean_4","mass_01_bkg_bbar_nonres_D0_mean_4",3.4835e+00,0,4));//V12
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_nonres_D0_sigma_4","mass_01_bkg_bbar_nonres_D0_sigma_4",2.1690e-02,0,0.5));//V13
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_nonres_D0_tail_4","mass_01_bkg_bbar_nonres_D0_tail_4",1.0053e-07,0,5));//V14
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzNovosibirsk("mass_01_bkg_bbar_nonres_D0_Gauss_4","mass_01_bkg_bbar_nonres_D0_Gauss_4",mass_AB,mass_BC,*m_vars.at(12),*m_vars.at(13),*m_vars.at(14),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5),23,m_Pi0,m_Pi,m_K,m_B0));//P3
        
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_nonres_D0_frac","mass_01_bkg_bbar_nonres_D0_frac",5.3370e-01,0,1));//V15
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_nonres_D0_frac2","mass_01_bkg_bbar_nonres_D0_frac2",4.2049e-01,0,1));//V16
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_01_bkg_bbar_nonres_D0_frac3","mass_01_bkg_bbar_nonres_D0_frac3",4.1508e-01,0,1));//V17
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooAddPdf("mass_01_bkg_bbar_nonres_D0","mass_01_bkg_bbar_nonres_D0",RooArgList(*m_pdfs.at(0),*m_pdfs.at(1),*m_pdfs.at(2),*m_pdfs.at(3)),RooArgList(*m_vars.at(15),*m_vars.at(16),*m_vars.at(17)),true)); //P4
        
        m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_nonres_Dplus_mean_1","mass_12_bkg_bbar_nonres_Dplus_mean_1", 3.4883e+00,3,4));//V18
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_nonres_Dplus_width_1","mass_12_bkg_bbar_nonres_Dplus_width_1",2.6575e-02,0,5));//V19
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_nonres_Dplus_c1","mass_12_bkg_bbar_nonres_Dplus_c1",-3.9093e-04,-1,1));//V20
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_nonres_Dplus_c2","mass_12_bkg_bbar_nonres_Dplus_c2",-5.1172e-04,-1,1));//V21
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_nonres_Dplus_c3","mass_12_bkg_bbar_nonres_Dplus_c3",-3.6807e-05,-1,1));//V22
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_nonres_Dplus_c4","mass_12_bkg_bbar_nonres_Dplus_c4",-2.0438e-06,-1,1));//V23
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzGauss("mass_12_bkg_bbar_nonres_Dplus_Gauss_1","mass_12_bkg_bbar_nonres_Dplus_Gauss_1",mass_AB,mass_BC,*m_vars.at(18),*m_vars.at(19),*m_vars.at(20),*m_vars.at(21),*m_vars.at(22),*m_vars.at(23),12,m_Pi0,m_Pi,m_K,m_B0)); //P5
        m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_nonres_Dplus_mean_2","mass_12_bkg_bbar_nonres_Dplus_mean_2", 3.4838e+00,3,4));//V24
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_nonres_Dplus_width_2","mass_12_bkg_bbar_nonres_Dplus_width_2",8.1251e-02,0,5));//V25
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooDalitzGauss("mass_12_bkg_bbar_nonres_Dplus_Gauss_2","mass_12_bkg_bbar_nonres_Dplus_Gauss_2",mass_AB,mass_BC,*m_vars.at(24),*m_vars.at(25),*m_vars.at(20),*m_vars.at(21),*m_vars.at(22),*m_vars.at(23),12,m_Pi0,m_Pi,m_K,m_B0)); //P6
        m_vars.push_back(new RooRealVar("mass_12_bkg_bbar_nonres_Dplus_frac","mass_12_bkg_bbar_nonres_Dplus_frac",7.0721e-01,0,1));//V26
        m_vars.back()->setConstant(true);
        m_pdfs.push_back(new RooAddPdf("mass_12_bkg_bbar_nonres_Dplus","mass_12_bkg_bbar_nonres_Dplus",*m_pdfs.at(5),*m_pdfs.at(6),*m_vars.at(26))); //P7
        
        
        
        m_vars.push_back(new RooRealVar("bkg_bbar_nonres_frac1","bkg_bbar_nonres_frac1",0.5,0,1));//V27

    
        m_pdfs.push_back(new RooAddPdf("bkg_bbar_nonres","bkg_bbar_nonres",*m_pdfs.at(4),*m_pdfs.at(7),*m_vars.at(27))); //P8
        
        
		m_main_pdfs["C"] = m_pdfs.at(8);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_2D_dalitz : public FF_PDF_bkg_bbar {
public:
	virtual ~FF_PDF_bkg_bbar_2D_dalitz() { }
    
	FF_PDF_bkg_bbar_2D_dalitz(shared_ptr<FF_PDF> combinatoric, shared_ptr<FF_PDF> wrongmasshyp,
                              shared_ptr<FF_PDF> wrongpi0hyp, shared_ptr<FF_PDF> missingfsp, shared_ptr<FF_PDF> nonres, double nevents,
                              const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar("pdf_bkg_bbar_2D_dalitz",cat) {
        
		if(dynamic_cast<FF_PDF_bkg_bbar_combinatoric_2D_dalitz*>(combinatoric.get())) {
			m_comb_pdfs.push_back(combinatoric);
		} else {
			std::cerr << "ERROR: PDF for combinatoric component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_combinatoric_2D_dalitz" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_wrongmasshyp_2D_dalitz*>(wrongmasshyp.get())) {
			m_comb_pdfs.push_back(wrongmasshyp);
		} else {
			std::cerr << "ERROR: PDF for wrongmasshyp component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_wrongmasshyp_2D_dalitz" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_wrongpi0hyp_2D_dalitz*>(wrongpi0hyp.get())) {
			m_comb_pdfs.push_back(wrongpi0hyp);
		} else {
			std::cerr << "ERROR: PDF for wrongpi0hyp component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_wrongpi0hyp_2D_dalitz" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_missingfsp_2D_dalitz*>(missingfsp.get())) {
			m_comb_pdfs.push_back(missingfsp);
		} else {
			std::cerr << "ERROR: PDF for missingfsp component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_missingfsp_2D_dalitz" << std::endl;
			exit(EXIT_FAILURE);
		}
        if(dynamic_cast<FF_PDF_bkg_bbar_nonres_2D_dalitz*>(nonres.get())) {
			m_comb_pdfs.push_back(nonres);
		} else {
			std::cerr << "ERROR: PDF for nonres component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_nonres_2D_dalitz" << std::endl;
			exit(EXIT_FAILURE);
		}
        
		m_vars.push_back(new RooRealVar("C_bkg_bbar_combinatoric_nevents","C_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongmasshyp_nevents","C_bkg_bbar_wrongmasshyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongpi0hyp_nevents","C_bkg_bbar_wrongpi0hyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_missingfsp_nevents","C_bkg_bbar_missingfsp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_nonres_nevents","C_bkg_bbar_nonres_nevents",nevents/7.0,0.0,nevents*1.2));
        
        
		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),m_comb_pdfs.at(4)->GetPDF("C")), RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4)),false));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
    
	FF_PDF_bkg_bbar_2D_dalitz(RooRealVar& mass_AB, RooRealVar& mass_BC, double nevents, const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar("pdf_bkg_bbar_2D_dalitz",cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_2D_dalitz>(mass_AB, mass_BC,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_2D_dalitz>(mass_AB, mass_BC,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_2D_dalitz>(mass_AB, mass_BC,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_missingfsp_2D_dalitz>(mass_AB, mass_BC,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_nonres_2D_dalitz>(mass_AB, mass_BC,cat));
        
		m_vars.push_back(new RooRealVar("C_bkg_bbar_combinatoric_nevents","C_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongmasshyp_nevents","C_bkg_bbar_wrongmasshyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongpi0hyp_nevents","C_bkg_bbar_wrongpi0hyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_missingfsp_nevents","C_bkg_bbar_missingfsp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_nonres_nevents","C_bkg_bbar_nonres_nevents",nevents/7.0,0.0,nevents*1.2));
        
        
		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),m_comb_pdfs.at(4)->GetPDF("C")), RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4)),false));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

//------ END DALIT PDFs ------

//------ 5D PDFs

class FF_PDF_bkg_bbar_combinatoric_5D_mbc_deltae_nb_prime_dalitz: public FF_PDF_bkg_bbar_combinatoric {
public:
	virtual ~FF_PDF_bkg_bbar_combinatoric_5D_mbc_deltae_nb_prime_dalitz() { }
    
	FF_PDF_bkg_bbar_combinatoric_5D_mbc_deltae_nb_prime_dalitz(RooRealVar& mbc, RooRealVar& deltae, RooRealVar& nb_prime, RooRealVar& mass_AB, RooRealVar& mass_BC, const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_combinatoric("pdf_bkg_bbar_combinatoric_5D_mbc_deltae_nb_prime_dalitz", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_nb_prime>(nb_prime,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_2D_dalitz>(mass_AB, mass_BC,cat));
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(), RooArgList(m_comb_pdfs.at(0)->GetPDF("C"), m_comb_pdfs.at(1)->GetPDF("C"), m_comb_pdfs.at(2)->GetPDF("C"), m_comb_pdfs.at(3)->GetPDF("C"))));
        
        
		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
    
	FF_PDF_bkg_bbar_combinatoric_5D_mbc_deltae_nb_prime_dalitz(shared_ptr<FF_PDF> mbc, shared_ptr<FF_PDF> deltae, shared_ptr<FF_PDF> nb_prime, shared_ptr<FF_PDF> dalitz, const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_combinatoric("pdf_bkg_bbar_combinatoric_5D_mbc_deltae_nb_prime_dalitz", cat) {
		m_comb_pdfs.push_back(mbc);
		m_comb_pdfs.push_back(deltae);
		m_comb_pdfs.push_back(nb_prime);
		m_comb_pdfs.push_back(dalitz);
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(), RooArgList(m_comb_pdfs.at(0)->GetPDF("C"), m_comb_pdfs.at(1)->GetPDF("C"), m_comb_pdfs.at(2)->GetPDF("C"), m_comb_pdfs.at(3)->GetPDF("C"))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_bkg_bbar_wrongmasshyp_5D_mbc_deltae_nb_prime_dalitz: public FF_PDF_bkg_bbar_wrongmasshyp {
public:
	virtual ~FF_PDF_bkg_bbar_wrongmasshyp_5D_mbc_deltae_nb_prime_dalitz() { }
    
	FF_PDF_bkg_bbar_wrongmasshyp_5D_mbc_deltae_nb_prime_dalitz(RooRealVar& mbc, RooRealVar& deltae, RooRealVar& nb_prime, RooRealVar& mass_AB, RooRealVar& mass_BC, const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_wrongmasshyp("pdf_bkg_bbar_wrongmasshyp_5D_mbc_deltae_nb_prime_dalitz", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_nb_prime>(nb_prime,cat));
        m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_2D_dalitz>(mass_AB, mass_BC,cat));
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(), RooArgList(m_comb_pdfs.at(0)->GetPDF("C"), m_comb_pdfs.at(1)->GetPDF("C"), m_comb_pdfs.at(2)->GetPDF("C"), m_comb_pdfs.at(3)->GetPDF("C"))));
		m_main_pdfs["C"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
    
	FF_PDF_bkg_bbar_wrongmasshyp_5D_mbc_deltae_nb_prime_dalitz(shared_ptr<FF_PDF> mbc, shared_ptr<FF_PDF> deltae, shared_ptr<FF_PDF> nb_prime, shared_ptr<FF_PDF> dalitz, const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_wrongmasshyp("pdf_bkg_bbar_wrongmasshyp_5D_mbc_deltae_nb_prime_dalitz", cat) {
		m_comb_pdfs.push_back(mbc);
		m_comb_pdfs.push_back(deltae);
		m_comb_pdfs.push_back(nb_prime);
		m_comb_pdfs.push_back(dalitz);
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(), RooArgList(m_comb_pdfs.at(0)->GetPDF("C"), m_comb_pdfs.at(1)->GetPDF("C"), m_comb_pdfs.at(2)->GetPDF("C"), m_comb_pdfs.at(3)->GetPDF("C"))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_bkg_bbar_wrongpi0hyp_5D_mbc_deltae_nb_prime_dalitz: public FF_PDF_bkg_bbar_wrongpi0hyp {
public:
	virtual ~FF_PDF_bkg_bbar_wrongpi0hyp_5D_mbc_deltae_nb_prime_dalitz() { }
    
	FF_PDF_bkg_bbar_wrongpi0hyp_5D_mbc_deltae_nb_prime_dalitz(RooRealVar& mbc, RooRealVar& deltae, RooRealVar& nb_prime, RooRealVar& mass_AB, RooRealVar& mass_BC, const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_wrongpi0hyp("pdf_bkg_bbar_wrongpi0hyp_5D_mbc_deltae_nb_prime_dalitz", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_1D_nb_prime>(nb_prime,cat));
        m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_2D_dalitz>(mass_AB, mass_BC,cat));
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(), RooArgList(m_comb_pdfs.at(0)->GetPDF("C"), m_comb_pdfs.at(1)->GetPDF("C"), m_comb_pdfs.at(2)->GetPDF("C"), m_comb_pdfs.at(3)->GetPDF("C"))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
    
	FF_PDF_bkg_bbar_wrongpi0hyp_5D_mbc_deltae_nb_prime_dalitz(shared_ptr<FF_PDF> mbc, shared_ptr<FF_PDF> deltae, shared_ptr<FF_PDF> nb_prime, shared_ptr<FF_PDF> dalitz, const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_wrongpi0hyp("pdf_bkg_bbar_wrongpi0hyp_5D_mbc_deltae_nb_prime_dalitz", cat) {
		m_comb_pdfs.push_back(mbc);
		m_comb_pdfs.push_back(deltae);
		m_comb_pdfs.push_back(nb_prime);
		m_comb_pdfs.push_back(dalitz);
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(), RooArgList(m_comb_pdfs.at(0)->GetPDF("C"), m_comb_pdfs.at(1)->GetPDF("C"), m_comb_pdfs.at(2)->GetPDF("C"), m_comb_pdfs.at(3)->GetPDF("C"))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_bkg_bbar_missingfsp_5D_mbc_deltae_nb_prime_dalitz: public FF_PDF_bkg_bbar_missingfsp {
public:
	virtual ~FF_PDF_bkg_bbar_missingfsp_5D_mbc_deltae_nb_prime_dalitz() { }
    
	FF_PDF_bkg_bbar_missingfsp_5D_mbc_deltae_nb_prime_dalitz(RooRealVar& mbc, RooRealVar& deltae, RooRealVar& nb_prime, RooRealVar& mass_AB, RooRealVar& mass_BC, const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_missingfsp("pdf_bkg_bbar_missingfsp_5D_mbc_deltae_nb_prime_dalitz", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_missingfsp_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_missingfsp_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_missingfsp_1D_nb_prime>(nb_prime,cat));
        m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_missingfsp_2D_dalitz>(mass_AB, mass_BC,cat));
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(), RooArgList(m_comb_pdfs.at(0)->GetPDF("C"), m_comb_pdfs.at(1)->GetPDF("C"), m_comb_pdfs.at(2)->GetPDF("C"), m_comb_pdfs.at(3)->GetPDF("C"))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
    
	FF_PDF_bkg_bbar_missingfsp_5D_mbc_deltae_nb_prime_dalitz(shared_ptr<FF_PDF> mbc, shared_ptr<FF_PDF> deltae, shared_ptr<FF_PDF> nb_prime, shared_ptr<FF_PDF> dalitz, const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_missingfsp("pdf_bkg_bbar_missingfsp_5D_mbc_deltae_nb_prime_dalitz", cat) {
		m_comb_pdfs.push_back(mbc);
		m_comb_pdfs.push_back(deltae);
		m_comb_pdfs.push_back(nb_prime);
		m_comb_pdfs.push_back(dalitz);
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(), RooArgList(m_comb_pdfs.at(0)->GetPDF("C"), m_comb_pdfs.at(1)->GetPDF("C"), m_comb_pdfs.at(2)->GetPDF("C"), m_comb_pdfs.at(3)->GetPDF("C"))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_nonres_5D_mbc_deltae_nb_prime_dalitz: public FF_PDF_bkg_bbar_nonres {
public:
	virtual ~FF_PDF_bkg_bbar_nonres_5D_mbc_deltae_nb_prime_dalitz() { }
    
	FF_PDF_bkg_bbar_nonres_5D_mbc_deltae_nb_prime_dalitz(RooRealVar& mbc, RooRealVar& deltae, RooRealVar& nb_prime, RooRealVar& mass_AB, RooRealVar& mass_BC, const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_nonres("pdf_bkg_bbar_nonres_5D_mbc_deltae_nb_prime_dalitz", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_nonres_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_nonres_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_nonres_1D_nb_prime>(nb_prime,cat));
        m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_nonres_2D_dalitz>(mass_AB, mass_BC,cat));
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(), RooArgList(m_comb_pdfs.at(0)->GetPDF("C"), m_comb_pdfs.at(1)->GetPDF("C"), m_comb_pdfs.at(2)->GetPDF("C"), m_comb_pdfs.at(3)->GetPDF("C"))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
    
	FF_PDF_bkg_bbar_nonres_5D_mbc_deltae_nb_prime_dalitz(shared_ptr<FF_PDF> mbc, shared_ptr<FF_PDF> deltae, shared_ptr<FF_PDF> nb_prime, shared_ptr<FF_PDF> dalitz, const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_nonres("pdf_bkg_bbar_nonres_5D_mbc_deltae_nb_prime_dalitz", cat) {
		m_comb_pdfs.push_back(mbc);
		m_comb_pdfs.push_back(deltae);
		m_comb_pdfs.push_back(nb_prime);
		m_comb_pdfs.push_back(dalitz);
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(), RooArgList(m_comb_pdfs.at(0)->GetPDF("C"), m_comb_pdfs.at(1)->GetPDF("C"), m_comb_pdfs.at(2)->GetPDF("C"), m_comb_pdfs.at(3)->GetPDF("C"))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_bkg_bbar_5D_mbc_deltae_nb_prime_dalitz : public FF_PDF_bkg_bbar {
public:
	virtual ~FF_PDF_bkg_bbar_5D_mbc_deltae_nb_prime_dalitz() { }
    
	FF_PDF_bkg_bbar_5D_mbc_deltae_nb_prime_dalitz(shared_ptr<FF_PDF> combinatoric, shared_ptr<FF_PDF> wrongmasshyp,
                                           shared_ptr<FF_PDF> wrongpi0hyp, shared_ptr<FF_PDF> missingfsp, shared_ptr<FF_PDF> nonres, double nevents,
                                           const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar("pdf_bkg_bbar_5D_mbc_deltae_nb_prime_dalitz",cat) {
        
		if(dynamic_cast<FF_PDF_bkg_bbar_combinatoric_5D_mbc_deltae_nb_prime_dalitz*>(combinatoric.get())) {
			m_comb_pdfs.push_back(combinatoric);
		} else {
			std::cerr << "ERROR: PDF for combinatoric component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_combinatoric_3D_mbc_deltae_nb_prime" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_wrongmasshyp_5D_mbc_deltae_nb_prime_dalitz*>(wrongmasshyp.get())) {
			m_comb_pdfs.push_back(wrongmasshyp);
		} else {
			std::cerr << "ERROR: PDF for wrongmasshyp component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_wrongmasshyp_3D_mbc_deltae_nb_prime" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_wrongpi0hyp_5D_mbc_deltae_nb_prime_dalitz*>(wrongpi0hyp.get())) {
			m_comb_pdfs.push_back(wrongpi0hyp);
		} else {
			std::cerr << "ERROR: PDF for wrongpi0hyp component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_wrongpi0hyp_3D_mbc_deltae_nb_prime" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_missingfsp_5D_mbc_deltae_nb_prime_dalitz*>(missingfsp.get())) {
			m_comb_pdfs.push_back(missingfsp);
		} else {
			std::cerr << "ERROR: PDF for missingfsp component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_missingfsp_3D_mbc_deltae_nb_prime" << std::endl;
			exit(EXIT_FAILURE);
		}
        if(dynamic_cast<FF_PDF_bkg_bbar_nonres_5D_mbc_deltae_nb_prime_dalitz*>(nonres.get())) {
			m_comb_pdfs.push_back(nonres);
		} else {
			std::cerr << "ERROR: PDF for nonres component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_nonres_3D_mbc_deltae_nb_prime" << std::endl;
			exit(EXIT_FAILURE);
		}
        
		m_vars.push_back(new RooRealVar("C_bkg_bbar_combinatoric_nevents","C_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongmasshyp_nevents","C_bkg_bbar_wrongmasshyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongpi0hyp_nevents","C_bkg_bbar_wrongpi0hyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_missingfsp_nevents","C_bkg_bbar_missingfsp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_nonres_nevents","C_bkg_bbar_nonres_nevents",nevents/7.0,0.0,nevents*1.2));
        
        
		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),m_comb_pdfs.at(4)->GetPDF("C")), RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4)),false));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
    
	FF_PDF_bkg_bbar_5D_mbc_deltae_nb_prime_dalitz(RooRealVar& mbc, RooRealVar& deltae, RooRealVar& nb_prime, RooRealVar& mass_AB, RooRealVar& mass_BC,
                                           double nevents, const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar("pdf_bkg_bbar_3D_mbc_deltae_nb_prime",cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_5D_mbc_deltae_nb_prime_dalitz>(mbc,deltae,nb_prime,mass_AB,mass_BC,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_5D_mbc_deltae_nb_prime_dalitz>(mbc,deltae,nb_prime,mass_AB,mass_BC,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_5D_mbc_deltae_nb_prime_dalitz>(mbc,deltae,nb_prime,mass_AB,mass_BC,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_missingfsp_5D_mbc_deltae_nb_prime_dalitz>(mbc,deltae,nb_prime,mass_AB,mass_BC,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_nonres_5D_mbc_deltae_nb_prime_dalitz>(mbc,deltae,nb_prime,mass_AB,mass_BC,cat));
        
		m_vars.push_back(new RooRealVar("C_bkg_bbar_combinatoric_nevents","C_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongmasshyp_nevents","C_bkg_bbar_wrongmasshyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongpi0hyp_nevents","C_bkg_bbar_wrongpi0hyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_missingfsp_nevents","C_bkg_bbar_missingfsp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_nonres_nevents","C_bkg_bbar_nonres_nevents",nevents/7.0,0.0,nevents*1.2));
        
        
		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),m_comb_pdfs.at(4)->GetPDF("C")), RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4)),false));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


//---- END 5D pdfs

//------ 6D PDFs

class FF_PDF_bkg_bbar_combinatoric_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor: public FF_PDF_bkg_bbar_combinatoric {
public:
	virtual ~FF_PDF_bkg_bbar_combinatoric_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor() { }
    
	FF_PDF_bkg_bbar_combinatoric_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor(RooRealVar& mbc, RooRealVar& deltae, RooRealVar& nb_prime, RooRealVar& mass_AB, RooRealVar& mass_BC, RooCategory& flavor, const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_combinatoric("pdf_bkg_bbar_combinatoric_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_nb_prime>(nb_prime,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_2D_dalitz>(mass_AB, mass_BC,cat));
		
		m_pdfs.push_back(new RooCategoryPdf(("C_"+m_label+"_flavor").c_str(),("C_"+m_label+"_flavor").c_str(),flavor));
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(), RooArgList(m_comb_pdfs.at(0)->GetPDF("C"), m_comb_pdfs.at(1)->GetPDF("C"), m_comb_pdfs.at(2)->GetPDF("C"), m_comb_pdfs.at(3)->GetPDF("C"),*m_pdfs.back())));
        
		m_main_pdfs["C"] = m_pdfs.back();
		m_main_pdfs["CS"] = m_pdfs.back();
		m_main_pdfs["COFF"] = m_pdfs.back();
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
    
};


class FF_PDF_bkg_bbar_wrongmasshyp_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor: public FF_PDF_bkg_bbar_wrongmasshyp {
public:
	virtual ~FF_PDF_bkg_bbar_wrongmasshyp_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor() { }
    
	FF_PDF_bkg_bbar_wrongmasshyp_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor(RooRealVar& mbc, RooRealVar& deltae, RooRealVar& nb_prime, RooRealVar& mass_AB, RooRealVar& mass_BC, RooCategory& flavor, const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_wrongmasshyp("pdf_bkg_bbar_wrongmasshyp_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_nb_prime>(nb_prime,cat));
        m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_2D_dalitz>(mass_AB, mass_BC,cat));
        
		m_pdfs.push_back(new RooCategoryPdf(("C_"+m_label+"_flavor").c_str(),("C_"+m_label+"_flavor").c_str(),flavor));
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(), RooArgList(m_comb_pdfs.at(0)->GetPDF("C"), m_comb_pdfs.at(1)->GetPDF("C"), m_comb_pdfs.at(2)->GetPDF("C"), m_comb_pdfs.at(3)->GetPDF("C"),*m_pdfs.back())));
        
		m_main_pdfs["C"] = m_pdfs.back();
		m_main_pdfs["CS"] = m_pdfs.back();
		m_main_pdfs["COFF"] = m_pdfs.back();
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
    
};


class FF_PDF_bkg_bbar_wrongpi0hyp_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor: public FF_PDF_bkg_bbar_wrongpi0hyp {
public:
	virtual ~FF_PDF_bkg_bbar_wrongpi0hyp_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor() { }
    
	FF_PDF_bkg_bbar_wrongpi0hyp_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor(RooRealVar& mbc, RooRealVar& deltae, RooRealVar& nb_prime, RooRealVar& mass_AB, RooRealVar& mass_BC, RooCategory& flavor, const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_wrongpi0hyp("pdf_bkg_bbar_wrongpi0hyp_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_1D_nb_prime>(nb_prime,cat));
        m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongpi0hyp_2D_dalitz>(mass_AB, mass_BC,cat));
        
		m_pdfs.push_back(new RooCategoryPdf(("C_"+m_label+"_flavor").c_str(),("C_"+m_label+"_flavor").c_str(),flavor));
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(), RooArgList(m_comb_pdfs.at(0)->GetPDF("C"), m_comb_pdfs.at(1)->GetPDF("C"), m_comb_pdfs.at(2)->GetPDF("C"), m_comb_pdfs.at(3)->GetPDF("C"),*m_pdfs.back())));
        
		m_main_pdfs["C"] = m_pdfs.back();
		m_main_pdfs["CS"] = m_pdfs.back();
		m_main_pdfs["COFF"] = m_pdfs.back();
		
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
    
};


class FF_PDF_bkg_bbar_missingfsp_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor: public FF_PDF_bkg_bbar_missingfsp {
public:
	virtual ~FF_PDF_bkg_bbar_missingfsp_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor() { }
    
	FF_PDF_bkg_bbar_missingfsp_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor(RooRealVar& mbc, RooRealVar& deltae, RooRealVar& nb_prime, RooRealVar& mass_AB, RooRealVar& mass_BC, RooCategory& flavor, const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_missingfsp("pdf_bkg_bbar_missingfsp_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_missingfsp_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_missingfsp_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_missingfsp_1D_nb_prime>(nb_prime,cat));
        m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_missingfsp_2D_dalitz>(mass_AB, mass_BC,cat));
        
		m_pdfs.push_back(new RooCategoryPdf(("C_"+m_label+"_flavor").c_str(),("C_"+m_label+"_flavor").c_str(),flavor));
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(), RooArgList(m_comb_pdfs.at(0)->GetPDF("C"), m_comb_pdfs.at(1)->GetPDF("C"), m_comb_pdfs.at(2)->GetPDF("C"), m_comb_pdfs.at(3)->GetPDF("C"),*m_pdfs.back())));
        
		m_main_pdfs["C"] = m_pdfs.back();
		m_main_pdfs["CS"] = m_pdfs.back();
		m_main_pdfs["COFF"] = m_pdfs.back();
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
	
};

class FF_PDF_bkg_bbar_nonres_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor: public FF_PDF_bkg_bbar_nonres {
public:
	virtual ~FF_PDF_bkg_bbar_nonres_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor() { }
    
	FF_PDF_bkg_bbar_nonres_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor(RooRealVar& mbc, RooRealVar& deltae, RooRealVar& nb_prime, RooRealVar& mass_AB, RooRealVar& mass_BC, RooCategory& flavor, const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_nonres("pdf_bkg_bbar_nonres_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_nonres_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_nonres_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_nonres_1D_nb_prime>(nb_prime,cat));
        m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_nonres_2D_dalitz>(mass_AB, mass_BC,cat));
        
		m_pdfs.push_back(new RooCategoryPdf(("C_"+m_label+"_flavor").c_str(),("C_"+m_label+"_flavor").c_str(),flavor));
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(), RooArgList(m_comb_pdfs.at(0)->GetPDF("C"), m_comb_pdfs.at(1)->GetPDF("C"), m_comb_pdfs.at(2)->GetPDF("C"), m_comb_pdfs.at(3)->GetPDF("C"),*m_pdfs.back())));
        
		m_main_pdfs["C"] = m_pdfs.back();
		m_main_pdfs["CS"] = m_pdfs.back();
		m_main_pdfs["COFF"] = m_pdfs.back();
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
    
};

//---- END 6D pdfs


class FF_PDF_bkg_bbar_combinatoric_1D_mass_kpi : public FF_PDF_bkg_bbar_combinatoric {
public:
	virtual ~FF_PDF_bkg_bbar_combinatoric_1D_mass_kpi() { }

	FF_PDF_bkg_bbar_combinatoric_1D_mass_kpi(RooRealVar& mass_kpi, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_combinatoric("pdf_bkg_bbar_combinatoric_1D_mass_kpi",cat) {
		m_vars.push_back(new RooRealVar("C_mass_kpi_bkg_bbar_combinatoric_a0","mass_kpi bkg_bbar_combinatoric a0",0.8,0.4,1.0));
		m_vars.push_back(new RooRealVar("N_mass_kpi_bkg_bbar_combinatoric_a0","N_mass_kpi bkg_bbar_combinatoric a0",0.8,0.4,1.0));
		m_vars.push_back(new RooRealVar("N_mass_kpi_bkg_bbar_combinatoric_a1","N_mass_kpi bkg_bbar_combinatoric a1",0.0,-1.0,1.0));

		m_pdfs.push_back(new RooChebychev(("C_"+m_label).c_str(),("C_"+m_label).c_str(),mass_kpi,RooArgList(*m_vars.at(0))));
		m_pdfs.push_back(new RooChebychev(("N_"+m_label).c_str(),("N_"+m_label).c_str(),mass_kpi,RooArgList(*m_vars.at(1),*m_vars.at(2))));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);
		m_main_pdfs["NS"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongmasshyp_1D_mass_kpi : public FF_PDF_bkg_bbar_wrongmasshyp {
public:
	virtual ~FF_PDF_bkg_bbar_wrongmasshyp_1D_mass_kpi() { }

	FF_PDF_bkg_bbar_wrongmasshyp_1D_mass_kpi(RooRealVar& mass_kpi, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_wrongmasshyp("pdf_bkg_bbar_wrongmasshyp_1D_mass_kpi",cat) {
		// Gaussian + Chebyshev
		m_vars.push_back(new RooRealVar("mass_kpi_bkg_bbar_wrongmasshyp_mean","mass_kpi bkg_bbar_wrongmasshyp mean",0.8,0.7,0.9));
		m_vars.push_back(new RooRealVar("mass_kpi_bkg_bbar_wrongmasshyp_width_left","mass_kpi bkg_bbar_wrongmasshyp width left",0.01,0.001,0.05));
		m_vars.push_back(new RooRealVar("mass_kpi_bkg_bbar_wrongmasshyp_width_right","mass_kpi bkg_bbar_wrongmasshyp width right",0.001));
		m_pdfs.push_back(new RooBifurGauss(("C_"+m_label+"_gauss").c_str(),("C_"+m_label+"_gauss").c_str(),mass_kpi,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2)));

		m_vars.push_back(new RooRealVar("mass_kpi_bkg_bbar_combinatoric_a0","mass_kpi bkg_bbar_combinatoric a0",-0.5,-1.0,0.0));
		m_pdfs.push_back(new RooChebychev(("C_"+m_label+"_cheb").c_str(),("C_"+m_label+"_cheb").c_str(),mass_kpi,RooArgList(*m_vars.at(3))));

		m_vars.push_back(new RooRealVar("mass_kpi_bkg_bbar_combinatoric_cheb_frac","mass_kpi bkg_bbar_combinatoric cheb_frac",0.7,0.5,1.0));
		m_pdfs.push_back(new RooAddPdf(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),*m_pdfs.at(1),*m_pdfs.at(0),*m_vars.at(4)));

		m_main_pdfs["C"] = m_pdfs.at(2);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongphikkbar_1D_mass_kpi : public FF_PDF_bkg_bbar_wrongphikkbar {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphikkbar_1D_mass_kpi() { }

	FF_PDF_bkg_bbar_wrongphikkbar_1D_mass_kpi(RooRealVar& mass_kpi, const FFUtil::tuple_category_name_flags& cat, bool is_data) : FF_PDF_bkg_bbar_wrongphikkbar("pdf_bkg_bbar_wrongphikkbar_1D_mass_kpi",cat) {
		// Relativistic Spin Breit-Wigner or Two Body PSHP Breit-Wigner
		m_vars.push_back(new RooRealVar("mass_kpi_bkg_bbar_wrongphikkbar_mean","mass_kpi bkg_bbar_wrongphikkbar mean",0.89594,0.89572,0.89616));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_kpi_bkg_bbar_wrongphikkbar_width","mass_kpi bkg_bbar_wrongphikkbar width",0.0487,0.0479,0.0495));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_kpi_bkg_bbar_wrongphikkbar_r","mass_kpi bkg_bbar_wrongphikkbar r",3.0));
		m_vars.push_back(new RooRealVar("mass_kpi_bkg_bbar_wrongphikkbar_J","mass_kpi bkg_bbar_wrongphikkbar J",1.0));
		if(is_data) {
			m_pdfs.push_back(new RooRelativisticSpinBreitWigner(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),mass_kpi,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),m_K,m_Pi));
			m_pdfs.push_back(new RooRelativisticSpinBreitWigner(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),mass_kpi,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),m_Ks,m_Pi0));
		} else {
			m_pdfs.push_back(new RooBreitWignerTwoBodyPHSP(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),mass_kpi,*m_vars.at(0),*m_vars.at(1),m_K,m_Pi));
			m_pdfs.push_back(new RooBreitWignerTwoBodyPHSP(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),mass_kpi,*m_vars.at(0),*m_vars.at(1),m_Ks,m_Pi0));
		}

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongphif0980_1D_mass_kpi : public FF_PDF_bkg_bbar_wrongphif0980 {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphif0980_1D_mass_kpi() { }

	FF_PDF_bkg_bbar_wrongphif0980_1D_mass_kpi(RooRealVar& mass_kpi, const FFUtil::tuple_category_name_flags& cat, bool is_data) : FF_PDF_bkg_bbar_wrongphif0980("pdf_bkg_bbar_wrongphif0980_1D_mass_kpi",cat) {
		// Relativistic Spin Breit-Wigner or Two Body PSHP Breit-Wigner
		m_vars.push_back(new RooRealVar("mass_kpi_bkg_bbar_wrongphif0980_mean","mass_kpi bkg_bbar_wrongphif0980 mean",0.89594,0.89572,0.89616));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_kpi_bkg_bbar_wrongphif0980_width","mass_kpi bkg_bbar_wrongphif0980 width",0.0487,0.0479,0.0495));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mass_kpi_bkg_bbar_wrongphif0980_r","mass_kpi bkg_bbar_wrongphif0980 r",3.0));
		m_vars.push_back(new RooRealVar("mass_kpi_bkg_bbar_wrongphif0980_J","mass_kpi bkg_bbar_wrongphif0980 J",1.0));
		if(is_data) {
			m_pdfs.push_back(new RooRelativisticSpinBreitWigner(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),mass_kpi,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),m_K,m_Pi));
			m_pdfs.push_back(new RooRelativisticSpinBreitWigner(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),mass_kpi,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),m_Ks,m_Pi0));
		} else {
			m_pdfs.push_back(new RooBreitWignerTwoBodyPHSP(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),mass_kpi,*m_vars.at(0),*m_vars.at(1),m_K,m_Pi));
			m_pdfs.push_back(new RooBreitWignerTwoBodyPHSP(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),mass_kpi,*m_vars.at(0),*m_vars.at(1),m_Ks,m_Pi0));
		}

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_1D_mass_kpi : public FF_PDF_bkg_bbar {
public:
	virtual ~FF_PDF_bkg_bbar_1D_mass_kpi() { }

	FF_PDF_bkg_bbar_1D_mass_kpi(shared_ptr<FF_PDF> combinatoric, shared_ptr<FF_PDF> wrongmasshyp,
			shared_ptr<FF_PDF> wrongphikkbar, shared_ptr<FF_PDF> wrongphif0980, double nevents,
			const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar("pdf_bkg_bbar_1D_mass_kpi",cat) {
		m_comb_pdfs.push_back(combinatoric);
		m_comb_pdfs.push_back(wrongmasshyp);
		m_comb_pdfs.push_back(wrongphikkbar);
		m_comb_pdfs.push_back(wrongphif0980);

		m_vars.push_back(new RooRealVar("C_mass_kpi_bkg_bbar_combinatoric_nevents","C_mass_kpi_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_mass_kpi_bkg_bbar_wrongmasshyp_nevents","C_mass_kpi_bkg_bbar_wrongmasshyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_mass_kpi_bkg_bbar_wrongphikkbar_nevents","C_mass_kpi_bkg_bbar_wrongphikkbar_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_mass_kpi_bkg_bbar_wrongphif0980_nevents","C_mass_kpi_bkg_bbar_wrongphif0980_nevents",nevents/7.0,0.0,nevents*1.2));

		m_vars.push_back(new RooRealVar("N_mass_kpi_bkg_bbar_combinatoric_nevents","N_mass_kpi_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_mass_kpi_bkg_bbar_wrongphikkbar_nevents","N_mass_kpi_bkg_bbar_wrongphikkbar_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_mass_kpi_bkg_bbar_wrongphif0980_nevents","N_mass_kpi_bkg_bbar_wrongphif0980_nevents",nevents/7.0,0.0,nevents*1.2));

		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
				m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C")),
				RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3)),false));
		m_pdfs.push_back(new RooAddPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(2)->GetPDF("N"),
				m_comb_pdfs.at(3)->GetPDF("N")),
				RooArgList(*m_vars.at(4),*m_vars.at(5),*m_vars.at(6)),false));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}

	FF_PDF_bkg_bbar_1D_mass_kpi(RooRealVar& mass_kpi, double nevents, const FFUtil::tuple_category_name_flags& cat, bool is_data) : FF_PDF_bkg_bbar("pdf_bkg_bbar_1D_mass_kpi",cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_mass_kpi>(mass_kpi,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_mass_kpi>(mass_kpi,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_mass_kpi>(mass_kpi,cat,is_data));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_mass_kpi>(mass_kpi,cat,is_data));

		m_vars.push_back(new RooRealVar("C_mass_kpi_bkg_bbar_combinatoric_nevents","C_mass_kpi_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_mass_kpi_bkg_bbar_wrongmasshyp_nevents","C_mass_kpi_bkg_bbar_wrongmasshyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_mass_kpi_bkg_bbar_wrongphikkbar_nevents","C_mass_kpi_bkg_bbar_wrongphikkbar_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_mass_kpi_bkg_bbar_wrongphif0980_nevents","C_mass_kpi_bkg_bbar_wrongphif0980_nevents",nevents/7.0,0.0,nevents*1.2));

		m_vars.push_back(new RooRealVar("N_mass_kpi_bkg_bbar_combinatoric_nevents","N_mass_kpi_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_mass_kpi_bkg_bbar_wrongphikkbar_nevents","N_mass_kpi_bkg_bbar_wrongphikkbar_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_mass_kpi_bkg_bbar_wrongphif0980_nevents","N_mass_kpi_bkg_bbar_wrongphif0980_nevents",nevents/7.0,0.0,nevents*1.2));

		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
				m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C")),
				RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3)),false));
		m_pdfs.push_back(new RooAddPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(2)->GetPDF("N"),
				m_comb_pdfs.at(3)->GetPDF("N")),
				RooArgList(*m_vars.at(4),*m_vars.at(5),*m_vars.at(6)),false));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_combinatoric_1D_hel_phi : public FF_PDF_bkg_bbar_combinatoric {
public:
	virtual ~FF_PDF_bkg_bbar_combinatoric_1D_hel_phi() { }

	FF_PDF_bkg_bbar_combinatoric_1D_hel_phi(RooRealVar& hel_phi, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_combinatoric("pdf_bkg_bbar_combinatoric_1D_hel_phi",cat) {
		m_pdfs.push_back(new RooUniform(("C_"+m_label).c_str(),("C_"+m_label).c_str(),hel_phi));
		m_pdfs.push_back(new RooUniform(("N_"+m_label).c_str(),("N_"+m_label).c_str(),hel_phi));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);
		m_main_pdfs["NS"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongmasshyp_1D_hel_phi : public FF_PDF_bkg_bbar_wrongmasshyp {
public:
	virtual ~FF_PDF_bkg_bbar_wrongmasshyp_1D_hel_phi() { }

	FF_PDF_bkg_bbar_wrongmasshyp_1D_hel_phi(RooRealVar& hel_phi, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_wrongmasshyp("pdf_bkg_bbar_wrongmasshyp_1D_hel_phi",cat) {
		m_vars.push_back(new RooRealVar("hel_phi_bkg_bbar_wrongmasshyp_a","hel_phi_bkg_bbar_wrongmasshyp a",1.0,0.0,2.0));
		m_pdfs.push_back(new RooGenericPdf(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),"(cos(@0)*cos(@0)+@1)",RooArgList(hel_phi,*m_vars.at(0))));

		m_main_pdfs["C"] = m_pdfs.at(0);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongphikkbar_1D_hel_phi : public FF_PDF_bkg_bbar_wrongphikkbar {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphikkbar_1D_hel_phi() { }

	FF_PDF_bkg_bbar_wrongphikkbar_1D_hel_phi(RooRealVar& hel_phi, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_wrongphikkbar("pdf_bkg_bbar_wrongphikkbar_1D_hel_phi",cat) {
		m_pdfs.push_back(new RooUniform(("C_"+m_label).c_str(),("C_"+m_label).c_str(),hel_phi));
		m_pdfs.push_back(new RooUniform(("N_"+m_label).c_str(),("N_"+m_label).c_str(),hel_phi));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);
		m_main_pdfs["NS"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongphif0980_1D_hel_phi : public FF_PDF_bkg_bbar_wrongphif0980 {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphif0980_1D_hel_phi() { }

	FF_PDF_bkg_bbar_wrongphif0980_1D_hel_phi(RooRealVar& hel_phi, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_wrongphif0980("pdf_bkg_bbar_wrongphif0980_1D_hel_phi",cat) {
		m_pdfs.push_back(new RooUniform(("C_"+m_label).c_str(),("C_"+m_label).c_str(),hel_phi));
		m_pdfs.push_back(new RooUniform(("N_"+m_label).c_str(),("N_"+m_label).c_str(),hel_phi));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);
		m_main_pdfs["NS"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_1D_hel_phi : public FF_PDF_bkg_bbar {
public:
	virtual ~FF_PDF_bkg_bbar_1D_hel_phi() { }

	FF_PDF_bkg_bbar_1D_hel_phi(shared_ptr<FF_PDF> combinatoric, shared_ptr<FF_PDF> wrongmasshyp,
			shared_ptr<FF_PDF> wrongphikkbar, shared_ptr<FF_PDF> wrongphif0980, double nevents,
			const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar("pdf_bkg_bbar_1D_hel_phi",cat) {
		m_comb_pdfs.push_back(combinatoric);
		m_comb_pdfs.push_back(wrongmasshyp);
		m_comb_pdfs.push_back(wrongphikkbar);
		m_comb_pdfs.push_back(wrongphif0980);

		m_vars.push_back(new RooRealVar("C_hel_phi_bkg_bbar_combinatoric_nevents","C_hel_phi_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_hel_phi_bkg_bbar_wrongmasshyp_nevents","C_hel_phi_bkg_bbar_wrongmasshyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_hel_phi_bkg_bbar_wrongphikkbar_nevents","C_hel_phi_bkg_bbar_wrongphikkbar_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_hel_phi_bkg_bbar_wrongphif0980_nevents","C_hel_phi_bkg_bbar_wrongphif0980_nevents",nevents/7.0,0.0,nevents*1.2));

		m_vars.push_back(new RooRealVar("N_hel_phi_bkg_bbar_combinatoric_nevents","N_hel_phi_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_hel_phi_bkg_bbar_wrongphikkbar_nevents","N_hel_phi_bkg_bbar_wrongphikkbar_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_hel_phi_bkg_bbar_wrongphif0980_nevents","N_hel_phi_bkg_bbar_wrongphif0980_nevents",nevents/7.0,0.0,nevents*1.2));

		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
				m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C")),
				RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3)),false));
		m_pdfs.push_back(new RooAddPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(2)->GetPDF("N"),
				m_comb_pdfs.at(3)->GetPDF("N")),
				RooArgList(*m_vars.at(4),*m_vars.at(5),*m_vars.at(6)),false));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}

	FF_PDF_bkg_bbar_1D_hel_phi(RooRealVar& hel_phi, double nevents, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar("pdf_bkg_bbar_1D_hel_phi",cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_hel_phi>(hel_phi,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_hel_phi>(hel_phi,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_hel_phi>(hel_phi,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_hel_phi>(hel_phi,cat));

		m_vars.push_back(new RooRealVar("C_hel_phi_bkg_bbar_combinatoric_nevents","C_hel_phi_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_hel_phi_bkg_bbar_wrongmasshyp_nevents","C_hel_phi_bkg_bbar_wrongmasshyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_hel_phi_bkg_bbar_wrongphikkbar_nevents","C_hel_phi_bkg_bbar_wrongphikkbar_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_hel_phi_bkg_bbar_wrongphif0980_nevents","C_hel_phi_bkg_bbar_wrongphif0980_nevents",nevents/7.0,0.0,nevents*1.2));

		m_vars.push_back(new RooRealVar("N_hel_phi_bkg_bbar_combinatoric_nevents","N_hel_phi_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_hel_phi_bkg_bbar_wrongphikkbar_nevents","N_hel_phi_bkg_bbar_wrongphikkbar_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_hel_phi_bkg_bbar_wrongphif0980_nevents","N_hel_phi_bkg_bbar_wrongphif0980_nevents",nevents/7.0,0.0,nevents*1.2));

		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
				m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C")),
				RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3)),false));
		m_pdfs.push_back(new RooAddPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(2)->GetPDF("N"),
				m_comb_pdfs.at(3)->GetPDF("N")),
				RooArgList(*m_vars.at(4),*m_vars.at(5),*m_vars.at(6)),false));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_bkg_bbar_combinatoric_1D_hel_th1 : public FF_PDF_bkg_bbar_combinatoric {
public:
	virtual ~FF_PDF_bkg_bbar_combinatoric_1D_hel_th1() { }

	FF_PDF_bkg_bbar_combinatoric_1D_hel_th1(RooRealVar& hel_th1, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_combinatoric("pdf_bkg_bbar_combinatoric_1D_hel_th1",cat) {
		m_vars.push_back(new RooRealVar("C_hel_th1_bkg_bbar_combinatoric_a1","C_hel_th1_bkg_bbar_combinatoric a1",0.0,-1.0,1.0));
		m_vars.push_back(new RooRealVar("C_hel_th1_bkg_bbar_combinatoric_a2","C_hel_th1_bkg_bbar_combinatoric a2",0.6,0.0,1.0));
		m_vars.push_back(new RooRealVar("C_hel_th1_bkg_bbar_combinatoric_a3","C_hel_th1_bkg_bbar_combinatoric a3",0.0,-1.0,1.0));
		m_vars.push_back(new RooRealVar("C_hel_th1_bkg_bbar_combinatoric_a4","C_hel_th1_bkg_bbar_combinatoric a4",0.0,-1.0,1.0));
		m_vars.push_back(new RooRealVar("C_hel_th1_bkg_bbar_combinatoric_a5","C_hel_th1_bkg_bbar_combinatoric a5",0.0,-1.0,1.0));
		m_vars.push_back(new RooRealVar("N_hel_th1_bkg_bbar_combinatoric_a1","N_hel_th1_bkg_bbar_combinatoric a1",0.0,-1.0,1.0));
		m_vars.push_back(new RooRealVar("N_hel_th1_bkg_bbar_combinatoric_a2","N_hel_th1_bkg_bbar_combinatoric a2",0.6,0.0,1.0));
		m_vars.push_back(new RooRealVar("N_hel_th1_bkg_bbar_combinatoric_a3","N_hel_th1_bkg_bbar_combinatoric a3",0.0));
		m_vars.push_back(new RooRealVar("N_hel_th1_bkg_bbar_combinatoric_a4","N_hel_th1_bkg_bbar_combinatoric a4",0.0,-1.0,1.0));

		m_pdfs.push_back(new RooChebychevCut(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),hel_th1,RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4)),0.75));
		m_pdfs.push_back(new RooChebychevCut(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),hel_th1,RooArgList(*m_vars.at(5),*m_vars.at(6),*m_vars.at(7),*m_vars.at(8)),0.75));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);
		m_main_pdfs["NS"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongmasshyp_1D_hel_th1 : public FF_PDF_bkg_bbar_wrongmasshyp {
public:
	virtual ~FF_PDF_bkg_bbar_wrongmasshyp_1D_hel_th1() { }

	FF_PDF_bkg_bbar_wrongmasshyp_1D_hel_th1(RooRealVar& hel_th1, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_wrongmasshyp("pdf_bkg_bbar_wrongmasshyp_1D_hel_th1",cat) {
		m_vars.push_back(new RooRealVar("hel_th1_bkg_bbar_wrongmasshyp_a1","hel_th1_bkg_bbar_wrongmasshyp a1",-0.5,-1.0,0.0));
		m_pdfs.push_back(new RooChebychevCut(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),hel_th1,RooArgList(*m_vars.at(0)),0.75));

		m_main_pdfs["C"] = m_pdfs.at(0);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongphikkbar_1D_hel_th1 : public FF_PDF_bkg_bbar_wrongphikkbar {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphikkbar_1D_hel_th1() { }

	FF_PDF_bkg_bbar_wrongphikkbar_1D_hel_th1(RooRealVar& hel_th1, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_wrongphikkbar("pdf_bkg_bbar_wrongphikkbar_1D_hel_th1",cat) {
		m_pdfs.push_back(new RooCutPdf(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),hel_th1,0.75,true));
		m_pdfs.push_back(new RooCutPdf(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),hel_th1,0.75,true));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongphif0980_1D_hel_th1 : public FF_PDF_bkg_bbar_wrongphif0980 {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphif0980_1D_hel_th1() { }

	FF_PDF_bkg_bbar_wrongphif0980_1D_hel_th1(RooRealVar& hel_th1, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_wrongphif0980("pdf_bkg_bbar_wrongphif0980_1D_hel_th1",cat) {
		m_vars.push_back(new RooRealVar("hel_th1_bkg_bbar_wrongphif0980_a1","hel_th1_bkg_bbar_wrongphif0980 a1",-0.5,-1.0,0.0));
		m_vars.push_back(new RooRealVar("hel_th1_bkg_bbar_wrongphif0980_a2","hel_th1_bkg_bbar_wrongphif0980 a2",0.8,0.3,1.0));
		m_pdfs.push_back(new RooChebychevCut(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),hel_th1,RooArgList(*m_vars.at(0),*m_vars.at(1)),0.75));
		m_pdfs.push_back(new RooChebychevCut(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),hel_th1,RooArgList(*m_vars.at(0),*m_vars.at(1)),0.75));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_1D_hel_th1 : public FF_PDF_bkg_bbar {
public:
	virtual ~FF_PDF_bkg_bbar_1D_hel_th1() { }

	FF_PDF_bkg_bbar_1D_hel_th1(shared_ptr<FF_PDF> combinatoric, shared_ptr<FF_PDF> wrongmasshyp,
			shared_ptr<FF_PDF> wrongphikkbar, shared_ptr<FF_PDF> wrongphif0980, double nevents,
			const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar("pdf_bkg_bbar_1D_hel_th1",cat) {
		m_comb_pdfs.push_back(combinatoric);
		m_comb_pdfs.push_back(wrongmasshyp);
		m_comb_pdfs.push_back(wrongphikkbar);
		m_comb_pdfs.push_back(wrongphif0980);

		m_vars.push_back(new RooRealVar("C_hel_th1_bkg_bbar_combinatoric_nevents","C_hel_th1_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_hel_th1_bkg_bbar_wrongmasshyp_nevents","C_hel_th1_bkg_bbar_wrongmasshyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_hel_th1_bkg_bbar_wrongphikkbar_nevents","C_hel_th1_bkg_bbar_wrongphikkbar_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_hel_th1_bkg_bbar_wrongphif0980_nevents","C_hel_th1_bkg_bbar_wrongphif0980_nevents",nevents/7.0,0.0,nevents*1.2));

		m_vars.push_back(new RooRealVar("N_hel_th1_bkg_bbar_combinatoric_nevents","N_hel_th1_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_hel_th1_bkg_bbar_wrongphikkbar_nevents","N_hel_th1_bkg_bbar_wrongphikkbar_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_hel_th1_bkg_bbar_wrongphif0980_nevents","N_hel_th1_bkg_bbar_wrongphif0980_nevents",nevents/7.0,0.0,nevents*1.2));

		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
				m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C")),
				RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3)),false));
		m_pdfs.push_back(new RooAddPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(2)->GetPDF("N"),
				m_comb_pdfs.at(3)->GetPDF("N")),
				RooArgList(*m_vars.at(4),*m_vars.at(5),*m_vars.at(6)),false));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}

	FF_PDF_bkg_bbar_1D_hel_th1(RooRealVar& hel_th1, double nevents, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar("pdf_bkg_bbar_1D_hel_th1",cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_hel_th1>(hel_th1,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_hel_th1>(hel_th1,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_hel_th1>(hel_th1,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_hel_th1>(hel_th1,cat));

		m_vars.push_back(new RooRealVar("C_hel_th1_bkg_bbar_combinatoric_nevents","C_hel_th1_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_hel_th1_bkg_bbar_wrongmasshyp_nevents","C_hel_th1_bkg_bbar_wrongmasshyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_hel_th1_bkg_bbar_wrongphikkbar_nevents","C_hel_th1_bkg_bbar_wrongphikkbar_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_hel_th1_bkg_bbar_wrongphif0980_nevents","C_hel_th1_bkg_bbar_wrongphif0980_nevents",nevents/7.0,0.0,nevents*1.2));

		m_vars.push_back(new RooRealVar("N_hel_th1_bkg_bbar_combinatoric_nevents","N_hel_th1_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_hel_th1_bkg_bbar_wrongphikkbar_nevents","N_hel_th1_bkg_bbar_wrongphikkbar_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_hel_th1_bkg_bbar_wrongphif0980_nevents","N_hel_th1_bkg_bbar_wrongphif0980_nevents",nevents/7.0,0.0,nevents*1.2));

		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
				m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C")),
				RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3)),false));
		m_pdfs.push_back(new RooAddPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(2)->GetPDF("N"),
				m_comb_pdfs.at(3)->GetPDF("N")),
				RooArgList(*m_vars.at(4),*m_vars.at(5),*m_vars.at(6)),false));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_combinatoric_1D_hel_th2 : public FF_PDF_bkg_bbar_combinatoric {
public:
	virtual ~FF_PDF_bkg_bbar_combinatoric_1D_hel_th2() { }

	FF_PDF_bkg_bbar_combinatoric_1D_hel_th2(RooRealVar& hel_th2, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_combinatoric("pdf_bkg_bbar_combinatoric_1D_hel_th2",cat) {
		m_vars.push_back(new RooRealVar("hel_th2_bkg_bbar_combinatoric_a1","hel_th2_bkg_bbar_combinatoric a1",0.0));
		m_vars.push_back(new RooRealVar("hel_th2_bkg_bbar_combinatoric_a2","hel_th2_bkg_bbar_combinatoric a2",0.5,0.0,1.0));

		m_pdfs.push_back(new RooChebychev(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),hel_th2,RooArgList(*m_vars.at(0),*m_vars.at(1))));
		m_pdfs.push_back(new RooChebychev(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),hel_th2,RooArgList(*m_vars.at(0),*m_vars.at(1))));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);
		m_main_pdfs["NS"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongmasshyp_1D_hel_th2 : public FF_PDF_bkg_bbar_wrongmasshyp {
public:
	virtual ~FF_PDF_bkg_bbar_wrongmasshyp_1D_hel_th2() { }

	FF_PDF_bkg_bbar_wrongmasshyp_1D_hel_th2(RooRealVar& hel_th2, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_wrongmasshyp("pdf_bkg_bbar_wrongmasshyp_1D_hel_th2",cat) {
		m_vars.push_back(new RooRealVar("hel_th2_bkg_bbar_wrongmasshyp_a1","hel_th2_bkg_bbar_wrongmasshyp a1",0.0));
		m_vars.push_back(new RooRealVar("hel_th2_bkg_bbar_wrongmasshyp_a2","hel_th2_bkg_bbar_wrongmasshyp a2",0.5,0.0,1.0));

		m_pdfs.push_back(new RooChebychev(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),hel_th2,RooArgList(*m_vars.at(0),*m_vars.at(1))));

		m_main_pdfs["C"] = m_pdfs.at(0);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongphikkbar_1D_hel_th2 : public FF_PDF_bkg_bbar_wrongphikkbar {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphikkbar_1D_hel_th2() { }

	FF_PDF_bkg_bbar_wrongphikkbar_1D_hel_th2(RooRealVar& hel_th2, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_wrongphikkbar("pdf_bkg_bbar_wrongphikkbar_1D_hel_th2",cat) {
		m_pdfs.push_back(new RooUniform(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),hel_th2));
		m_pdfs.push_back(new RooUniform(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),hel_th2));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongphif0980_1D_hel_th2 : public FF_PDF_bkg_bbar_wrongphif0980 {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphif0980_1D_hel_th2() { }

	FF_PDF_bkg_bbar_wrongphif0980_1D_hel_th2(RooRealVar& hel_th2, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar_wrongphif0980("pdf_bkg_bbar_wrongphif0980_1D_hel_th2",cat) {
		m_pdfs.push_back(new RooUniform(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),hel_th2));
		m_pdfs.push_back(new RooUniform(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),hel_th2));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_1D_hel_th2 : public FF_PDF_bkg_bbar {
public:
	virtual ~FF_PDF_bkg_bbar_1D_hel_th2() { }

	FF_PDF_bkg_bbar_1D_hel_th2(shared_ptr<FF_PDF> combinatoric, shared_ptr<FF_PDF> wrongmasshyp,
			shared_ptr<FF_PDF> wrongphikkbar, shared_ptr<FF_PDF> wrongphif0980, double nevents,
			const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar("pdf_bkg_bbar_1D_hel_th2",cat) {
		m_comb_pdfs.push_back(combinatoric);
		m_comb_pdfs.push_back(wrongmasshyp);
		m_comb_pdfs.push_back(wrongphikkbar);
		m_comb_pdfs.push_back(wrongphif0980);

		m_vars.push_back(new RooRealVar("C_hel_th2_bkg_bbar_combinatoric_nevents","C_hel_th2_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_hel_th2_bkg_bbar_wrongmasshyp_nevents","C_hel_th2_bkg_bbar_wrongmasshyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_hel_th2_bkg_bbar_wrongphikkbar_nevents","C_hel_th2_bkg_bbar_wrongphikkbar_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_hel_th2_bkg_bbar_wrongphif0980_nevents","C_hel_th2_bkg_bbar_wrongphif0980_nevents",nevents/7.0,0.0,nevents*1.2));

		m_vars.push_back(new RooRealVar("N_hel_th2_bkg_bbar_combinatoric_nevents","N_hel_th2_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_hel_th2_bkg_bbar_wrongphikkbar_nevents","N_hel_th2_bkg_bbar_wrongphikkbar_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_hel_th2_bkg_bbar_wrongphif0980_nevents","N_hel_th2_bkg_bbar_wrongphif0980_nevents",nevents/7.0,0.0,nevents*1.2));

		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
				m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C")),
				RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3)),false));
		m_pdfs.push_back(new RooAddPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(2)->GetPDF("N"),
				m_comb_pdfs.at(3)->GetPDF("N")),
				RooArgList(*m_vars.at(4),*m_vars.at(5),*m_vars.at(6)),false));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}

	FF_PDF_bkg_bbar_1D_hel_th2(RooRealVar& hel_th2, double nevents, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar("pdf_bkg_bbar_1D_hel_th2",cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_hel_th2>(hel_th2,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_hel_th2>(hel_th2,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_hel_th2>(hel_th2,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_hel_th2>(hel_th2,cat));

		m_vars.push_back(new RooRealVar("C_hel_th2_bkg_bbar_combinatoric_nevents","C_hel_th2_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_hel_th2_bkg_bbar_wrongmasshyp_nevents","C_hel_th2_bkg_bbar_wrongmasshyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_hel_th2_bkg_bbar_wrongphikkbar_nevents","C_hel_th2_bkg_bbar_wrongphikkbar_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_hel_th2_bkg_bbar_wrongphif0980_nevents","C_hel_th2_bkg_bbar_wrongphif0980_nevents",nevents/7.0,0.0,nevents*1.2));

		m_vars.push_back(new RooRealVar("N_hel_th2_bkg_bbar_combinatoric_nevents","N_hel_th2_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_hel_th2_bkg_bbar_wrongphikkbar_nevents","N_hel_th2_bkg_bbar_wrongphikkbar_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_hel_th2_bkg_bbar_wrongphif0980_nevents","N_hel_th2_bkg_bbar_wrongphif0980_nevents",nevents/7.0,0.0,nevents*1.2));

		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
				m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C")),
				RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3)),false));
		m_pdfs.push_back(new RooAddPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(2)->GetPDF("N"),
				m_comb_pdfs.at(3)->GetPDF("N")),
				RooArgList(*m_vars.at(4),*m_vars.at(5),*m_vars.at(6)),false));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_combinatoric_6D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th2: public FF_PDF_bkg_bbar_combinatoric {
public:
	virtual ~FF_PDF_bkg_bbar_combinatoric_6D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th2() { }

	FF_PDF_bkg_bbar_combinatoric_6D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th2(RooRealVar& deltae,
			RooRealVar& chil0_ma, RooRealVar& nb_prime, RooRealVar& mass_kpi,
			RooRealVar& hel_phi, RooRealVar& hel_th2,
			const FFUtil::tuple_category_name_flags& cat, bool is_data, RooRealVar* resolution)
	: FF_PDF_bkg_bbar_combinatoric("pdf_bkg_bbar_combinatoric_6D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th2", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_chil0_ma>(chil0_ma,cat,is_data,resolution));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_nb_prime>(nb_prime,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_mass_kpi>(mass_kpi,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_hel_phi>(hel_phi,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_hel_th2>(hel_th2,cat));

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
		m_main_pdfs["N"] = m_pdfs.at(1);
		m_main_pdfs["NS"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_combinatoric_7D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th1_hel_th2: public FF_PDF_bkg_bbar_combinatoric {
public:
	virtual ~FF_PDF_bkg_bbar_combinatoric_7D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th1_hel_th2() { }

	FF_PDF_bkg_bbar_combinatoric_7D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th1_hel_th2(RooRealVar& deltae,
			RooRealVar& chil0_ma, RooRealVar& nb_prime, RooRealVar& mass_kpi,
			RooRealVar& hel_phi, RooRealVar& hel_th1, RooRealVar& hel_th2,
			const FFUtil::tuple_category_name_flags& cat, bool is_data, RooRealVar* resolution)
	: FF_PDF_bkg_bbar_combinatoric("pdf_bkg_bbar_combinatoric_7D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th1_hel_th2", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_chil0_ma>(chil0_ma,cat,is_data,resolution));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_nb_prime>(nb_prime,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_mass_kpi>(mass_kpi,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_hel_phi>(hel_phi,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_hel_th1>(hel_th1,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_hel_th2>(hel_th2,cat));

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
		m_main_pdfs["N"] = m_pdfs.at(1);
		m_main_pdfs["NS"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_combinatoric_8D: public FF_PDF_bkg_bbar_combinatoric {
public:
	virtual ~FF_PDF_bkg_bbar_combinatoric_8D() { }

	FF_PDF_bkg_bbar_combinatoric_8D(RooRealVar& mbc, RooRealVar& deltae,
			RooRealVar& chil0_ma, RooRealVar& nb_prime, RooRealVar& mass_kpi,
			RooRealVar& hel_phi, RooRealVar& hel_th1, RooRealVar& hel_th2,
			const FFUtil::tuple_category_name_flags& cat, bool is_data, RooRealVar* resolution)
	: FF_PDF_bkg_bbar_combinatoric("pdf_bkg_bbar_combinatoric_8D", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_chil0_ma>(chil0_ma,cat,is_data,resolution));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_nb_prime>(nb_prime,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_mass_kpi>(mass_kpi,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_hel_phi>(hel_phi,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_hel_th1>(hel_th1,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_1D_hel_th2>(hel_th2,cat));

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
						m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),
						m_comb_pdfs.at(4)->GetPDF("C"),m_comb_pdfs.at(5)->GetPDF("C"),
						m_comb_pdfs.at(6)->GetPDF("C"),m_comb_pdfs.at(7)->GetPDF("C"))));
		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
						m_comb_pdfs.at(2)->GetPDF("N"),m_comb_pdfs.at(3)->GetPDF("N"),
						m_comb_pdfs.at(4)->GetPDF("N"),m_comb_pdfs.at(5)->GetPDF("N"),
						m_comb_pdfs.at(6)->GetPDF("N"),m_comb_pdfs.at(7)->GetPDF("N"))));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);
		m_main_pdfs["NS"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}

	FF_PDF_bkg_bbar_combinatoric_8D(shared_ptr<FF_PDF> mbc, shared_ptr<FF_PDF> deltae,
			shared_ptr<FF_PDF> chil0_ma, shared_ptr<FF_PDF> nb_prime,
			shared_ptr<FF_PDF> mass_kpi, shared_ptr<FF_PDF> hel_phi,
			shared_ptr<FF_PDF> hel_th1, shared_ptr<FF_PDF> hel_th2,
			const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_combinatoric("pdf_bkg_bbar_combinatoric_8D", cat) {
		m_comb_pdfs.push_back(mbc);
		m_comb_pdfs.push_back(deltae);
		m_comb_pdfs.push_back(chil0_ma);
		m_comb_pdfs.push_back(nb_prime);
		m_comb_pdfs.push_back(mass_kpi);
		m_comb_pdfs.push_back(hel_phi);
		m_comb_pdfs.push_back(hel_th1);
		m_comb_pdfs.push_back(hel_th2);

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
						m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),
						m_comb_pdfs.at(4)->GetPDF("C"),m_comb_pdfs.at(5)->GetPDF("C"),
						m_comb_pdfs.at(6)->GetPDF("C"),m_comb_pdfs.at(7)->GetPDF("C"))));
		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
						m_comb_pdfs.at(2)->GetPDF("N"),m_comb_pdfs.at(3)->GetPDF("N"),
						m_comb_pdfs.at(4)->GetPDF("N"),m_comb_pdfs.at(5)->GetPDF("N"),
						m_comb_pdfs.at(6)->GetPDF("N"),m_comb_pdfs.at(7)->GetPDF("N"))));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["CS"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);
		m_main_pdfs["NS"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongmasshyp_8D: public FF_PDF_bkg_bbar_wrongmasshyp {
public:
	virtual ~FF_PDF_bkg_bbar_wrongmasshyp_8D() { }

	FF_PDF_bkg_bbar_wrongmasshyp_8D(RooRealVar& mbc, RooRealVar& deltae,
			RooRealVar& chil0_ma, RooRealVar& nb_prime, RooRealVar& mass_kpi,
			RooRealVar& hel_phi, RooRealVar& hel_th1, RooRealVar& hel_th2,
			const FFUtil::tuple_category_name_flags& cat, bool is_data, RooRealVar* resolution)
	: FF_PDF_bkg_bbar_wrongmasshyp("pdf_bkg_bbar_wrongmasshyp_8D", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_chil0_ma>(chil0_ma,cat,is_data,resolution));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_nb_prime>(nb_prime,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_mass_kpi>(mass_kpi,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_hel_phi>(hel_phi,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_hel_th1>(hel_th1,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_1D_hel_th2>(hel_th2,cat));

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
						m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),
						m_comb_pdfs.at(4)->GetPDF("C"),m_comb_pdfs.at(5)->GetPDF("C"),
						m_comb_pdfs.at(6)->GetPDF("C"),m_comb_pdfs.at(7)->GetPDF("C"))));

		m_main_pdfs["C"] = m_pdfs.at(0);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}

	FF_PDF_bkg_bbar_wrongmasshyp_8D(shared_ptr<FF_PDF> mbc, shared_ptr<FF_PDF> deltae,
			shared_ptr<FF_PDF> chil0_ma, shared_ptr<FF_PDF> nb_prime,
			shared_ptr<FF_PDF> mass_kpi, shared_ptr<FF_PDF> hel_phi,
			shared_ptr<FF_PDF> hel_th1, shared_ptr<FF_PDF> hel_th2,
			const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_wrongmasshyp("pdf_bkg_bbar_wrongmasshyp_8D", cat) {
		m_comb_pdfs.push_back(mbc);
		m_comb_pdfs.push_back(deltae);
		m_comb_pdfs.push_back(chil0_ma);
		m_comb_pdfs.push_back(nb_prime);
		m_comb_pdfs.push_back(mass_kpi);
		m_comb_pdfs.push_back(hel_phi);
		m_comb_pdfs.push_back(hel_th1);
		m_comb_pdfs.push_back(hel_th2);

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
						m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),
						m_comb_pdfs.at(4)->GetPDF("C"),m_comb_pdfs.at(5)->GetPDF("C"),
						m_comb_pdfs.at(6)->GetPDF("C"),m_comb_pdfs.at(7)->GetPDF("C"))));

		m_main_pdfs["C"] = m_pdfs.at(0);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongphikkbar_4D_mass_kpi_angles: public FF_PDF_bkg_bbar_wrongphikkbar {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphikkbar_4D_mass_kpi_angles() { }

	FF_PDF_bkg_bbar_wrongphikkbar_4D_mass_kpi_angles(RooRealVar& mass_kpi,
			RooRealVar& hel_phi, RooRealVar& hel_th1, RooRealVar& hel_th2,
			const FFUtil::tuple_category_name_flags& cat, bool is_data)
	: FF_PDF_bkg_bbar_wrongphikkbar("pdf_bkg_bbar_wrongphikkbar_4D_mass_kpi_angles", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_mass_kpi>(mass_kpi,cat,is_data));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_hel_phi>(hel_phi,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_hel_th1>(hel_th1,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_hel_th2>(hel_th2,cat));

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
						m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"))));
		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
						m_comb_pdfs.at(2)->GetPDF("N"),m_comb_pdfs.at(3)->GetPDF("N"))));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}

	FF_PDF_bkg_bbar_wrongphikkbar_4D_mass_kpi_angles(shared_ptr<FF_PDF> mass_kpi, shared_ptr<FF_PDF> hel_phi,
			shared_ptr<FF_PDF> hel_th1, shared_ptr<FF_PDF> hel_th2,
			const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_wrongphikkbar("pdf_bkg_bbar_wrongphikkbar_4D_mass_kpi_angles", cat) {
		m_comb_pdfs.push_back(mass_kpi);
		m_comb_pdfs.push_back(hel_phi);
		m_comb_pdfs.push_back(hel_th1);
		m_comb_pdfs.push_back(hel_th2);

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
						m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"))));
		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
						m_comb_pdfs.at(2)->GetPDF("N"),m_comb_pdfs.at(3)->GetPDF("N"))));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongphikkbar_5D_mass_kpi_angles_flavor: public FF_PDF_bkg_bbar_wrongphikkbar {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphikkbar_5D_mass_kpi_angles_flavor() { }

	FF_PDF_bkg_bbar_wrongphikkbar_5D_mass_kpi_angles_flavor(RooRealVar& mass_kpi,
			RooRealVar& hel_phi, RooRealVar& hel_th1, RooRealVar& hel_th2, RooCategory& flavor,
			const FFUtil::tuple_category_name_flags& cat, bool is_data)
	: FF_PDF_bkg_bbar_wrongphikkbar("pdf_bkg_bbar_wrongphikkbar_5D_mass_kpi_angles_flavor", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_mass_kpi>(mass_kpi,cat,is_data));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_hel_phi>(hel_phi,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_hel_th1>(hel_th1,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_hel_th2>(hel_th2,cat));

		m_pdfs.push_back(new RooCategoryPdf(("C_"+m_label+"_flavor").c_str(),("C_"+m_label+"_flavor").c_str(),flavor));

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
						m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),
						*m_pdfs.back())));
		m_main_pdfs["C"] = m_pdfs.back();

//		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
//				RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
//						m_comb_pdfs.at(2)->GetPDF("N"),m_comb_pdfs.at(3)->GetPDF("N"))));
//		m_main_pdfs["N"] = m_pdfs.back();

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongphikkbar_8D: public FF_PDF_bkg_bbar_wrongphikkbar {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphikkbar_8D() { }

	FF_PDF_bkg_bbar_wrongphikkbar_8D(RooRealVar& mbc, RooRealVar& deltae,
			RooRealVar& chil0_ma, RooRealVar& nb_prime, RooRealVar& mass_kpi,
			RooRealVar& hel_phi, RooRealVar& hel_th1, RooRealVar& hel_th2,
			const FFUtil::tuple_category_name_flags& cat, bool is_data)
	: FF_PDF_bkg_bbar_wrongphikkbar("pdf_bkg_bbar_wrongphikkbar_8D", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_chil0_ma>(chil0_ma,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_nb_prime>(nb_prime,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_mass_kpi>(mass_kpi,cat,is_data));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_hel_phi>(hel_phi,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_hel_th1>(hel_th1,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_hel_th2>(hel_th2,cat));

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
						m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),
						m_comb_pdfs.at(4)->GetPDF("C"),m_comb_pdfs.at(5)->GetPDF("C"),
						m_comb_pdfs.at(6)->GetPDF("C"),m_comb_pdfs.at(7)->GetPDF("C"))));
		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
						m_comb_pdfs.at(2)->GetPDF("N"),m_comb_pdfs.at(3)->GetPDF("N"),
						m_comb_pdfs.at(4)->GetPDF("N"),m_comb_pdfs.at(5)->GetPDF("N"),
						m_comb_pdfs.at(6)->GetPDF("N"),m_comb_pdfs.at(7)->GetPDF("N"))));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}

	FF_PDF_bkg_bbar_wrongphikkbar_8D(shared_ptr<FF_PDF> mbc, shared_ptr<FF_PDF> deltae,
			shared_ptr<FF_PDF> chil0_ma, shared_ptr<FF_PDF> nb_prime,
			shared_ptr<FF_PDF> mass_kpi, shared_ptr<FF_PDF> hel_phi,
			shared_ptr<FF_PDF> hel_th1, shared_ptr<FF_PDF> hel_th2,
			const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_wrongphikkbar("pdf_bkg_bbar_wrongphikkbar_8D", cat) {
		m_comb_pdfs.push_back(mbc);
		m_comb_pdfs.push_back(deltae);
		m_comb_pdfs.push_back(chil0_ma);
		m_comb_pdfs.push_back(nb_prime);
		m_comb_pdfs.push_back(mass_kpi);
		m_comb_pdfs.push_back(hel_phi);
		m_comb_pdfs.push_back(hel_th1);
		m_comb_pdfs.push_back(hel_th2);

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
						m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),
						m_comb_pdfs.at(4)->GetPDF("C"),m_comb_pdfs.at(5)->GetPDF("C"),
						m_comb_pdfs.at(6)->GetPDF("C"),m_comb_pdfs.at(7)->GetPDF("C"))));
		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
						m_comb_pdfs.at(2)->GetPDF("N"),m_comb_pdfs.at(3)->GetPDF("N"),
						m_comb_pdfs.at(4)->GetPDF("N"),m_comb_pdfs.at(5)->GetPDF("N"),
						m_comb_pdfs.at(6)->GetPDF("N"),m_comb_pdfs.at(7)->GetPDF("N"))));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongphikkbar_9D: public FF_PDF_bkg_bbar_wrongphikkbar {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphikkbar_9D() { }

	FF_PDF_bkg_bbar_wrongphikkbar_9D(RooRealVar& mbc, RooRealVar& deltae,
			RooRealVar& chil0_ma, RooRealVar& nb_prime, RooRealVar& mass_kpi,
			RooRealVar& hel_phi, RooRealVar& hel_th1, RooRealVar& hel_th2,
			RooCategory& flavor,
			const FFUtil::tuple_category_name_flags& cat, bool is_data)
	: FF_PDF_bkg_bbar_wrongphikkbar("pdf_bkg_bbar_wrongphikkbar_9D", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_chil0_ma>(chil0_ma,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_nb_prime>(nb_prime,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_mass_kpi>(mass_kpi,cat,is_data));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_hel_phi>(hel_phi,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_hel_th1>(hel_th1,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_1D_hel_th2>(hel_th2,cat));

		m_pdfs.push_back(new RooCategoryPdf(("C_"+m_label+"_flavor").c_str(),("C_"+m_label+"_flavor").c_str(),flavor));

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
						m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),
						m_comb_pdfs.at(4)->GetPDF("C"),m_comb_pdfs.at(5)->GetPDF("C"),
						m_comb_pdfs.at(6)->GetPDF("C"),m_comb_pdfs.at(7)->GetPDF("C"),
						*m_pdfs.back())));
		m_main_pdfs["C"] = m_pdfs.back();

//		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
//				RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
//						m_comb_pdfs.at(2)->GetPDF("N"),m_comb_pdfs.at(3)->GetPDF("N"),
//						m_comb_pdfs.at(4)->GetPDF("N"),m_comb_pdfs.at(5)->GetPDF("N"),
//						m_comb_pdfs.at(6)->GetPDF("N"),m_comb_pdfs.at(7)->GetPDF("N"))));
//		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongphif0980_8D: public FF_PDF_bkg_bbar_wrongphif0980 {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphif0980_8D() { }

	FF_PDF_bkg_bbar_wrongphif0980_8D(RooRealVar& mbc, RooRealVar& deltae,
			RooRealVar& chil0_ma, RooRealVar& nb_prime, RooRealVar& mass_kpi,
			RooRealVar& hel_phi, RooRealVar& hel_th1, RooRealVar& hel_th2,
			const FFUtil::tuple_category_name_flags& cat, bool is_data)
	: FF_PDF_bkg_bbar_wrongphif0980("pdf_bkg_bbar_wrongphif0980_8D", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_chil0_ma>(chil0_ma,cat,is_data));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_nb_prime>(nb_prime,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_mass_kpi>(mass_kpi,cat,is_data));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_hel_phi>(hel_phi,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_hel_th1>(hel_th1,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_hel_th2>(hel_th2,cat));

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
						m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),
						m_comb_pdfs.at(4)->GetPDF("C"),m_comb_pdfs.at(5)->GetPDF("C"),
						m_comb_pdfs.at(6)->GetPDF("C"),m_comb_pdfs.at(7)->GetPDF("C"))));
		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
						m_comb_pdfs.at(2)->GetPDF("N"),m_comb_pdfs.at(3)->GetPDF("N"),
						m_comb_pdfs.at(4)->GetPDF("N"),m_comb_pdfs.at(5)->GetPDF("N"),
						m_comb_pdfs.at(6)->GetPDF("N"),m_comb_pdfs.at(7)->GetPDF("N"))));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}

	FF_PDF_bkg_bbar_wrongphif0980_8D(shared_ptr<FF_PDF> mbc, shared_ptr<FF_PDF> deltae,
			shared_ptr<FF_PDF> chil0_ma, shared_ptr<FF_PDF> nb_prime,
			shared_ptr<FF_PDF> mass_kpi, shared_ptr<FF_PDF> hel_phi,
			shared_ptr<FF_PDF> hel_th1, shared_ptr<FF_PDF> hel_th2,
			const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_bkg_bbar_wrongphif0980("pdf_bkg_bbar_wrongphif0980_8D", cat) {
		m_comb_pdfs.push_back(mbc);
		m_comb_pdfs.push_back(deltae);
		m_comb_pdfs.push_back(chil0_ma);
		m_comb_pdfs.push_back(nb_prime);
		m_comb_pdfs.push_back(mass_kpi);
		m_comb_pdfs.push_back(hel_phi);
		m_comb_pdfs.push_back(hel_th1);
		m_comb_pdfs.push_back(hel_th2);

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
						m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),
						m_comb_pdfs.at(4)->GetPDF("C"),m_comb_pdfs.at(5)->GetPDF("C"),
						m_comb_pdfs.at(6)->GetPDF("C"),m_comb_pdfs.at(7)->GetPDF("C"))));
		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
						m_comb_pdfs.at(2)->GetPDF("N"),m_comb_pdfs.at(3)->GetPDF("N"),
						m_comb_pdfs.at(4)->GetPDF("N"),m_comb_pdfs.at(5)->GetPDF("N"),
						m_comb_pdfs.at(6)->GetPDF("N"),m_comb_pdfs.at(7)->GetPDF("N"))));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongphif0980_9D: public FF_PDF_bkg_bbar_wrongphif0980 {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphif0980_9D() { }

	FF_PDF_bkg_bbar_wrongphif0980_9D(RooRealVar& mbc, RooRealVar& deltae,
			RooRealVar& chil0_ma, RooRealVar& nb_prime, RooRealVar& mass_kpi,
			RooRealVar& hel_phi, RooRealVar& hel_th1, RooRealVar& hel_th2,
			RooCategory& flavor,
			const FFUtil::tuple_category_name_flags& cat, bool is_data)
	: FF_PDF_bkg_bbar_wrongphif0980("pdf_bkg_bbar_wrongphif0980_9D", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_chil0_ma>(chil0_ma,cat,is_data));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_nb_prime>(nb_prime,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_mass_kpi>(mass_kpi,cat,is_data));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_hel_phi>(hel_phi,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_hel_th1>(hel_th1,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_hel_th2>(hel_th2,cat));

		m_pdfs.push_back(new RooCategoryPdf(("C_"+m_label+"_flavor").c_str(),("C_"+m_label+"_flavor").c_str(),flavor));

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
						m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),
						m_comb_pdfs.at(4)->GetPDF("C"),m_comb_pdfs.at(5)->GetPDF("C"),
						m_comb_pdfs.at(6)->GetPDF("C"),m_comb_pdfs.at(7)->GetPDF("C"),
						*m_pdfs.back())));
		m_main_pdfs["C"] = m_pdfs.back();

		//		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
		//				RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
		//						m_comb_pdfs.at(2)->GetPDF("N"),m_comb_pdfs.at(3)->GetPDF("N"),
		//						m_comb_pdfs.at(4)->GetPDF("N"),m_comb_pdfs.at(5)->GetPDF("N"),
		//						m_comb_pdfs.at(6)->GetPDF("N"),m_comb_pdfs.at(7)->GetPDF("N"))));
		//		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongphif0980AndNonRes_9D: public FF_PDF_bkg_bbar_wrongphif0980 {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphif0980AndNonRes_9D() { }

	FF_PDF_bkg_bbar_wrongphif0980AndNonRes_9D(RooRealVar& mbc, RooRealVar& deltae,
			RooRealVar& chil0_ma, RooRealVar& nb_prime, RooRealVar& mass_kpi,
			RooRealVar& hel_phi, RooRealVar& hel_th1, RooRealVar& hel_th2,
			RooCategory& flavor,
			const FFUtil::tuple_category_name_flags& cat, bool is_data)
	: FF_PDF_bkg_bbar_wrongphif0980("pdf_bkg_bbar_wrongphif0980AndNonRes_9D", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980AndNonRes_1D_chil0_ma>(chil0_ma,cat,is_data));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_nb_prime>(nb_prime,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_mass_kpi>(mass_kpi,cat,is_data));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_hel_phi>(hel_phi,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_hel_th1>(hel_th1,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_hel_th2>(hel_th2,cat));

		m_pdfs.push_back(new RooCategoryPdf(("C_"+m_label+"_flavor").c_str(),("C_"+m_label+"_flavor").c_str(),flavor));

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
						m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),
						m_comb_pdfs.at(4)->GetPDF("C"),m_comb_pdfs.at(5)->GetPDF("C"),
						m_comb_pdfs.at(6)->GetPDF("C"),m_comb_pdfs.at(7)->GetPDF("C"),
						*m_pdfs.back())));
		m_main_pdfs["C"] = m_pdfs.back();

		//		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
		//				RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
		//						m_comb_pdfs.at(2)->GetPDF("N"),m_comb_pdfs.at(3)->GetPDF("N"),
		//						m_comb_pdfs.at(4)->GetPDF("N"),m_comb_pdfs.at(5)->GetPDF("N"),
		//						m_comb_pdfs.at(6)->GetPDF("N"),m_comb_pdfs.at(7)->GetPDF("N"))));
		//		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongphif0980_with_interference_4D_norm : public FF_PDF_bkg_bbar_wrongphif0980 {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphif0980_with_interference_4D_norm() { }

	FF_PDF_bkg_bbar_wrongphif0980_with_interference_4D_norm(RooRealVar& chil0_ma,
			RooRealVar& hel_phi, RooRealVar& hel_th1, RooRealVar& hel_th2,
			const FFUtil::tuple_category_name_flags& cat, const std::vector<RooRealVar*>& amps_phi,
			ACCEPTANCE_FUNCTION acceptance_function, KKInterferenceCacheIntegrators_4D KK_4D)
	: FF_PDF_bkg_bbar_wrongphif0980("pdf_bkg_bbar_wrongphif0980_with_interference_4D_norm",cat) {
		m_vars.push_back(new RooRealVar("amp_flatte","amp_flatte",0.2,0.0,1.0));
		m_vars.push_back(new RooRealVar("phase_flatte","phase_flatte",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		// Flatte formalism (see BN 1129 for f0(980) parameters)
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongphif0980_pole","chil0_ma bkg_bbar_wrongphif0980 pole",0.965,0.955,0.975));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongphif0980_g1","chil0_ma bkg_bbar_wrongphif0980 g1",0.165));
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongphif0980_g2","chil0_ma bkg_bbar_wrongphif0980 g2",0.347/std::sqrt(2.0)));
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongphif0980_g3","chil0_ma bkg_bbar_wrongphif0980 g3",0.347/std::sqrt(2.0)));

		// Relativistic Spin Breit-Wigner for phi
		m_vars.push_back(new RooRealVar("mass_phi","mass_phi",1.019455,1.019435,1.019475));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("width_phi","width_phi",0.00426,0.00422,0.00430));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_combinatoric_r","chil0_ma bkg_bbar_combinatoric r",3.0));
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_combinatoric_J","chil0_ma bkg_bbar_combinatoric J",1.0));

		if(amps_phi.size() == 0) {
			m_vars.push_back(new RooRealVar("amp_10","amp_10",0.707));
			m_vars.back()->setConstant(true);
			m_vars.push_back(new RooRealVar("phase_10","phase_10",0.0));
			m_vars.push_back(new RooRealVar("amp_1para","amp_1para",0.4,0.0,1.0));
			m_vars.push_back(new RooRealVar("phase_1para","phase_1para",0.0,-2*TMath::Pi(),2*TMath::Pi()));
			m_vars.push_back(new RooRealVar("amp_1perp","amp_1perp",0.6,0.0,1.0));
			m_vars.push_back(new RooRealVar("phase_1perp","phase_1perp",0.0,-2*TMath::Pi(),2*TMath::Pi()));

			m_pdfs.push_back(new RooKKInterferenceNorm(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),
					chil0_ma,hel_phi,hel_th1,hel_th2,
					*m_vars.at(0),*m_vars.at(1), // amp and phase of flatte
					*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5), // mass and g1-3 of flatte
					*m_vars.at(10),*m_vars.at(11), // amp and phase phi long
					*m_vars.at(12),*m_vars.at(13), // amp and phase phi para
					*m_vars.at(14),*m_vars.at(15), // amp and phase phi perp
					*m_vars.at(6),*m_vars.at(7),*m_vars.at(8),*m_vars.at(9), // mass, witdh, etc.. of phi
					acceptance_function,
					m_Pi,m_Pi,m_K,m_K,m_Ks,m_Ks,m_K,m_K,m_K892,m_B0,
					KK_4D));
			m_main_pdfs["C"] = m_pdfs.back();
			m_pdfs.push_back(new RooKKInterferenceNorm(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),
					chil0_ma,hel_phi,hel_th1,hel_th2,
					*m_vars.at(0),*m_vars.at(1), // amp and phase of flatte
					*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5), // mass and g1-3 of flatte
					*m_vars.at(10),*m_vars.at(11), // amp and phase phi long
					*m_vars.at(12),*m_vars.at(13), // amp and phase phi para
					*m_vars.at(14),*m_vars.at(15), // amp and phase phi perp
					*m_vars.at(6),*m_vars.at(7),*m_vars.at(8),*m_vars.at(9), // mass, witdh, etc.. of phi
					acceptance_function,
					m_Pi,m_Pi,m_K,m_K,m_Ks,m_Ks,m_K,m_K,m_K892,m_B0,
					KK_4D));
			m_main_pdfs["N"] = m_pdfs.back();
		} else {
			m_pdfs.push_back(new RooKKInterferenceNorm(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),
					chil0_ma,hel_phi,hel_th1,hel_th2,
					*m_vars.at(0),*m_vars.at(1), // amp and phase of flatte
					*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5), // mass and g1-3 of flatte
					*amps_phi.at(0),*amps_phi.at(1), // amp and phase phi long
					*amps_phi.at(2),*amps_phi.at(3), // amp and phase phi para
					*amps_phi.at(4),*amps_phi.at(5), // amp and phase phi perp
					*m_vars.at(6),*m_vars.at(7),*m_vars.at(8),*m_vars.at(9), // mass, witdh, etc.. of phi
					acceptance_function,
					m_Pi,m_Pi,m_K,m_K,m_Ks,m_Ks,m_K,m_K,m_K892,m_B0,
					KK_4D));
			m_main_pdfs["C"] = m_pdfs.back();
			m_pdfs.push_back(new RooKKInterferenceNorm(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),
					chil0_ma,hel_phi,hel_th1,hel_th2,
					*m_vars.at(0),*m_vars.at(1), // amp and phase of flatte
					*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5), // mass and g1-3 of flatte
					*amps_phi.at(0),*amps_phi.at(1), // amp and phase phi long
					*amps_phi.at(2),*amps_phi.at(3), // amp and phase phi para
					*amps_phi.at(4),*amps_phi.at(5), // amp and phase phi perp
					*m_vars.at(6),*m_vars.at(7),*m_vars.at(8),*m_vars.at(9), // mass, witdh, etc.. of phi
					acceptance_function,
					m_Pi,m_Pi,m_K,m_K,m_Ks,m_Ks,m_K,m_K,m_K892,m_B0,
					KK_4D));
			m_main_pdfs["N"] = m_pdfs.back();
		}

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongphif0980_with_interference_4D_pos : public FF_PDF_bkg_bbar_wrongphif0980 {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphif0980_with_interference_4D_pos() { }

	FF_PDF_bkg_bbar_wrongphif0980_with_interference_4D_pos(RooRealVar& chil0_ma,
			RooRealVar& hel_phi, RooRealVar& hel_th1, RooRealVar& hel_th2,
			const FFUtil::tuple_category_name_flags& cat, const std::vector<RooRealVar*>& amps_phi, const std::vector<RooRealVar*>& amps_flatte,
			ACCEPTANCE_FUNCTION acceptance_function, KKInterferenceCacheIntegrators_4D KK_4D)
	: FF_PDF_bkg_bbar_wrongphif0980("pdf_bkg_bbar_wrongphif0980_with_interference_4D_pos",cat) {
		m_vars.push_back(new RooRealVar("amp_flatte","amp_flatte",0.2,0.0,1.0));
		m_vars.push_back(new RooRealVar("phase_flatte","phase_flatte",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		// Flatte formalism (see BN 1129 for f0(980) parameters)
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongphif0980_pole","chil0_ma bkg_bbar_wrongphif0980 pole",0.965,0.955,0.975));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongphif0980_g1","chil0_ma bkg_bbar_wrongphif0980 g1",0.165));
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongphif0980_g2","chil0_ma bkg_bbar_wrongphif0980 g2",0.347/std::sqrt(2.0)));
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongphif0980_g3","chil0_ma bkg_bbar_wrongphif0980 g3",0.347/std::sqrt(2.0)));

		// Relativistic Spin Breit-Wigner for phi
		m_vars.push_back(new RooRealVar("mass_phi","mass_phi",1.019455,1.019435,1.019475));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("width_phi","width_phi",0.00426,0.00422,0.00430));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_combinatoric_r","chil0_ma bkg_bbar_combinatoric r",3.0));
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_combinatoric_J","chil0_ma bkg_bbar_combinatoric J",1.0));

		if(amps_phi.size() == 0) {
			m_vars.push_back(new RooRealVar("amp_10","amp_10",0.707));
			m_vars.back()->setConstant(true);
			m_vars.push_back(new RooRealVar("phase_10","phase_10",0.0));
			m_vars.push_back(new RooRealVar("amp_1para","amp_1para",0.4,0.0,1.0));
			m_vars.push_back(new RooRealVar("phase_1para","phase_1para",0.0,-2*TMath::Pi(),2*TMath::Pi()));
			m_vars.push_back(new RooRealVar("amp_1perp","amp_1perp",0.6,0.0,1.0));
			m_vars.push_back(new RooRealVar("phase_1perp","phase_1perp",0.0,-2*TMath::Pi(),2*TMath::Pi()));

			m_pdfs.push_back(new RooKKInterferencePositive(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),
					chil0_ma,hel_phi,hel_th1,hel_th2,
					*m_vars.at(0),*m_vars.at(1), // amp and phase of flatte
					*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5), // mass and g1-3 of flatte
					*m_vars.at(10),*m_vars.at(11), // amp and phase phi long
					*m_vars.at(12),*m_vars.at(13), // amp and phase phi para
					*m_vars.at(14),*m_vars.at(15), // amp and phase phi perp
					*m_vars.at(6),*m_vars.at(7),*m_vars.at(8),*m_vars.at(9), // mass, witdh, etc.. of phi
					acceptance_function,
					m_Pi,m_Pi,m_K,m_K,m_Ks,m_Ks,m_K,m_K,m_K892,m_B0,
					KK_4D));
			m_main_pdfs["C"] = m_pdfs.back();
			m_pdfs.push_back(new RooKKInterferencePositive(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),
					chil0_ma,hel_phi,hel_th1,hel_th2,
					*m_vars.at(0),*m_vars.at(1), // amp and phase of flatte
					*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5), // mass and g1-3 of flatte
					*m_vars.at(10),*m_vars.at(11), // amp and phase phi long
					*m_vars.at(12),*m_vars.at(13), // amp and phase phi para
					*m_vars.at(14),*m_vars.at(15), // amp and phase phi perp
					*m_vars.at(6),*m_vars.at(7),*m_vars.at(8),*m_vars.at(9), // mass, witdh, etc.. of phi
					acceptance_function,
					m_Pi,m_Pi,m_K,m_K,m_Ks,m_Ks,m_K,m_K,m_K892,m_B0,
					KK_4D));
			m_main_pdfs["N"] = m_pdfs.back();
		} else {
			m_pdfs.push_back(new RooKKInterferencePositive(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),
					chil0_ma,hel_phi,hel_th1,hel_th2,
					*amps_flatte.at(0),*amps_flatte.at(1), // amp and phase of flatte
					*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5), // mass and g1-3 of flatte
					*amps_phi.at(0),*amps_phi.at(1), // amp and phase phi long
					*amps_phi.at(2),*amps_phi.at(3), // amp and phase phi para
					*amps_phi.at(4),*amps_phi.at(5), // amp and phase phi perp
					*m_vars.at(6),*m_vars.at(7),*m_vars.at(8),*m_vars.at(9), // mass, witdh, etc.. of phi
					acceptance_function,
					m_Pi,m_Pi,m_K,m_K,m_Ks,m_Ks,m_K,m_K,m_K892,m_B0,
					KK_4D));
			m_main_pdfs["C"] = m_pdfs.back();
			m_pdfs.push_back(new RooKKInterferencePositive(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),
					chil0_ma,hel_phi,hel_th1,hel_th2,
					*amps_flatte.at(0),*amps_flatte.at(1), // amp and phase of flatte
					*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5), // mass and g1-3 of flatte
					*amps_phi.at(0),*amps_phi.at(1), // amp and phase phi long
					*amps_phi.at(2),*amps_phi.at(3), // amp and phase phi para
					*amps_phi.at(4),*amps_phi.at(5), // amp and phase phi perp
					*m_vars.at(6),*m_vars.at(7),*m_vars.at(8),*m_vars.at(9), // mass, witdh, etc.. of phi
					acceptance_function,
					m_Pi,m_Pi,m_K,m_K,m_Ks,m_Ks,m_K,m_K,m_K892,m_B0,
					KK_4D));
			m_main_pdfs["N"] = m_pdfs.back();
		}

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongphif0980_with_interference_4D_neg : public FF_PDF_bkg_bbar_wrongphif0980 {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphif0980_with_interference_4D_neg() { }

	FF_PDF_bkg_bbar_wrongphif0980_with_interference_4D_neg(RooRealVar& chil0_ma,
			RooRealVar& hel_phi, RooRealVar& hel_th1, RooRealVar& hel_th2,
			const FFUtil::tuple_category_name_flags& cat, const std::vector<RooRealVar*>& amps_phi, const std::vector<RooRealVar*>& amps_flatte,
			ACCEPTANCE_FUNCTION acceptance_function, KKInterferenceCacheIntegrators_4D KK_4D)
	: FF_PDF_bkg_bbar_wrongphif0980("pdf_bkg_bbar_wrongphif0980_with_interference_4D_neg",cat) {
		m_vars.push_back(new RooRealVar("amp_flatte","amp_flatte",0.2,0.0,1.0));
		m_vars.push_back(new RooRealVar("phase_flatte","phase_flatte",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		// Flatte formalism (see BN 1129 for f0(980) parameters)
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongphif0980_pole","chil0_ma bkg_bbar_wrongphif0980 pole",0.965,0.955,0.975));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongphif0980_g1","chil0_ma bkg_bbar_wrongphif0980 g1",0.165));
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongphif0980_g2","chil0_ma bkg_bbar_wrongphif0980 g2",0.347/std::sqrt(2.0)));
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_wrongphif0980_g3","chil0_ma bkg_bbar_wrongphif0980 g3",0.347/std::sqrt(2.0)));

		// Relativistic Spin Breit-Wigner for phi
		m_vars.push_back(new RooRealVar("mass_phi","mass_phi",1.019455,1.019435,1.019475));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("width_phi","width_phi",0.00426,0.00422,0.00430));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_combinatoric_r","chil0_ma bkg_bbar_combinatoric r",3.0));
		m_vars.push_back(new RooRealVar("chil0_ma_bkg_bbar_combinatoric_J","chil0_ma bkg_bbar_combinatoric J",1.0));

		if(amps_phi.size() == 0) {
			m_vars.push_back(new RooRealVar("amp_10","amp_10",0.707));
			m_vars.back()->setConstant(true);
			m_vars.push_back(new RooRealVar("phase_10","phase_10",0.0));
			m_vars.push_back(new RooRealVar("amp_1para","amp_1para",0.4,0.0,1.0));
			m_vars.push_back(new RooRealVar("phase_1para","phase_1para",0.0,-2*TMath::Pi(),2*TMath::Pi()));
			m_vars.push_back(new RooRealVar("amp_1perp","amp_1perp",0.6,0.0,1.0));
			m_vars.push_back(new RooRealVar("phase_1perp","phase_1perp",0.0,-2*TMath::Pi(),2*TMath::Pi()));

			m_pdfs.push_back(new RooKKInterferenceNegative(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),
					chil0_ma,hel_phi,hel_th1,hel_th2,
					*m_vars.at(0),*m_vars.at(1), // amp and phase of flatte
					*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5), // mass and g1-3 of flatte
					*m_vars.at(10),*m_vars.at(11), // amp and phase phi long
					*m_vars.at(12),*m_vars.at(13), // amp and phase phi para
					*m_vars.at(14),*m_vars.at(15), // amp and phase phi perp
					*m_vars.at(6),*m_vars.at(7),*m_vars.at(8),*m_vars.at(9), // mass, witdh, etc.. of phi
					acceptance_function,
					m_Pi,m_Pi,m_K,m_K,m_Ks,m_Ks,m_K,m_K,m_K892,m_B0,
					KK_4D));
			m_main_pdfs["C"] = m_pdfs.back();
			m_pdfs.push_back(new RooKKInterferenceNegative(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),
					chil0_ma,hel_phi,hel_th1,hel_th2,
					*m_vars.at(0),*m_vars.at(1), // amp and phase of flatte
					*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5), // mass and g1-3 of flatte
					*m_vars.at(10),*m_vars.at(11), // amp and phase phi long
					*m_vars.at(12),*m_vars.at(13), // amp and phase phi para
					*m_vars.at(14),*m_vars.at(15), // amp and phase phi perp
					*m_vars.at(6),*m_vars.at(7),*m_vars.at(8),*m_vars.at(9), // mass, witdh, etc.. of phi
					acceptance_function,
					m_Pi,m_Pi,m_K,m_K,m_Ks,m_Ks,m_K,m_K,m_K892,m_B0,
					KK_4D));
			m_main_pdfs["N"] = m_pdfs.back();
		} else {
			m_pdfs.push_back(new RooKKInterferenceNegative(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),
					chil0_ma,hel_phi,hel_th1,hel_th2,
					*amps_flatte.at(0),*amps_flatte.at(1), // amp and phase of flatte
					*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5), // mass and g1-3 of flatte
					*amps_phi.at(0),*amps_phi.at(1), // amp and phase phi long
					*amps_phi.at(2),*amps_phi.at(3), // amp and phase phi para
					*amps_phi.at(4),*amps_phi.at(5), // amp and phase phi perp
					*m_vars.at(6),*m_vars.at(7),*m_vars.at(8),*m_vars.at(9), // mass, witdh, etc.. of phi
					acceptance_function,
					m_Pi,m_Pi,m_K,m_K,m_Ks,m_Ks,m_K,m_K,m_K892,m_B0,
					KK_4D));
			m_main_pdfs["C"] = m_pdfs.back();
			m_pdfs.push_back(new RooKKInterferenceNegative(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),
					chil0_ma,hel_phi,hel_th1,hel_th2,
					*amps_flatte.at(0),*amps_flatte.at(1), // amp and phase of flatte
					*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5), // mass and g1-3 of flatte
					*amps_phi.at(0),*amps_phi.at(1), // amp and phase phi long
					*amps_phi.at(2),*amps_phi.at(3), // amp and phase phi para
					*amps_phi.at(4),*amps_phi.at(5), // amp and phase phi perp
					*m_vars.at(6),*m_vars.at(7),*m_vars.at(8),*m_vars.at(9), // mass, witdh, etc.. of phi
					acceptance_function,
					m_Pi,m_Pi,m_K,m_K,m_Ks,m_Ks,m_K,m_K,m_K892,m_B0,
					KK_4D));
			m_main_pdfs["N"] = m_pdfs.back();
		}

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongphif0980_with_interference_9D_norm: public FF_PDF_bkg_bbar_wrongphif0980 {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphif0980_with_interference_9D_norm() { }

	FF_PDF_bkg_bbar_wrongphif0980_with_interference_9D_norm(RooRealVar& mbc, RooRealVar& deltae,
			RooRealVar& chil0_ma, RooRealVar& nb_prime, RooRealVar& mass_kpi,
			RooRealVar& hel_phi, RooRealVar& hel_th1, RooRealVar& hel_th2,
			RooCategory& flavor,
			const FFUtil::tuple_category_name_flags& cat, bool is_data,
			const std::vector<RooRealVar*>& amps_phi, ACCEPTANCE_FUNCTION acceptance_function,
			KKInterferenceCacheIntegrators_4D KK_4D)
	: FF_PDF_bkg_bbar_wrongphif0980("pdf_bkg_bbar_wrongphif0980_with_interference_9D_norm", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_nb_prime>(nb_prime,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_mass_kpi>(mass_kpi,cat,is_data));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_with_interference_4D_norm>(chil0_ma, hel_phi, hel_th1, hel_th2,
				cat, amps_phi, acceptance_function, KK_4D));

		m_pdfs.push_back(new RooCategoryPdf(("C_"+m_label+"_flavor").c_str(),("C_"+m_label+"_flavor").c_str(),flavor));

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
						m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),
						m_comb_pdfs.at(4)->GetPDF("C"),*m_pdfs.back())));
		m_main_pdfs["C"] = m_pdfs.back();

		//		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
		//				RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
		//						m_comb_pdfs.at(2)->GetPDF("N"),m_comb_pdfs.at(3)->GetPDF("N"),
		//						m_comb_pdfs.at(4)->GetPDF("N"),m_comb_pdfs.at(5)->GetPDF("N"),
		//						m_comb_pdfs.at(6)->GetPDF("N"),m_comb_pdfs.at(7)->GetPDF("N"))));
		//		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongphif0980_with_interference_9D_positive: public FF_PDF_bkg_bbar_wrongphif0980 {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphif0980_with_interference_9D_positive() { }

	FF_PDF_bkg_bbar_wrongphif0980_with_interference_9D_positive(RooRealVar& mbc, RooRealVar& deltae,
			RooRealVar& chil0_ma, RooRealVar& nb_prime, RooRealVar& mass_kpi,
			RooRealVar& hel_phi, RooRealVar& hel_th1, RooRealVar& hel_th2,
			RooCategory& flavor,
			const FFUtil::tuple_category_name_flags& cat, bool is_data,
			const std::vector<RooRealVar*>& amps_phi, const std::vector<RooRealVar*>& amps_flatte, ACCEPTANCE_FUNCTION acceptance_function,
			KKInterferenceCacheIntegrators_4D KK_4D)
	: FF_PDF_bkg_bbar_wrongphif0980("pdf_bkg_bbar_wrongphif0980_with_interference_9D_positive", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_nb_prime>(nb_prime,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_mass_kpi>(mass_kpi,cat,is_data));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_with_interference_4D_pos>(chil0_ma, hel_phi, hel_th1, hel_th2,
				cat, amps_phi, amps_flatte, acceptance_function, KK_4D));

		m_pdfs.push_back(new RooCategoryPdf(("C_"+m_label+"_flavor").c_str(),("C_"+m_label+"_flavor").c_str(),flavor));

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
						m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),
						m_comb_pdfs.at(4)->GetPDF("C"),*m_pdfs.back())));
		m_main_pdfs["C"] = m_pdfs.back();

		//		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
		//				RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
		//						m_comb_pdfs.at(2)->GetPDF("N"),m_comb_pdfs.at(3)->GetPDF("N"),
		//						m_comb_pdfs.at(4)->GetPDF("N"),m_comb_pdfs.at(5)->GetPDF("N"),
		//						m_comb_pdfs.at(6)->GetPDF("N"),m_comb_pdfs.at(7)->GetPDF("N"))));
		//		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_wrongphif0980_with_interference_9D_negative: public FF_PDF_bkg_bbar_wrongphif0980 {
public:
	virtual ~FF_PDF_bkg_bbar_wrongphif0980_with_interference_9D_negative() { }

	FF_PDF_bkg_bbar_wrongphif0980_with_interference_9D_negative(RooRealVar& mbc, RooRealVar& deltae,
			RooRealVar& chil0_ma, RooRealVar& nb_prime, RooRealVar& mass_kpi,
			RooRealVar& hel_phi, RooRealVar& hel_th1, RooRealVar& hel_th2,
			RooCategory& flavor,
			const FFUtil::tuple_category_name_flags& cat, bool is_data,
			const std::vector<RooRealVar*>& amps_phi, const std::vector<RooRealVar*>& amps_flatte, ACCEPTANCE_FUNCTION acceptance_function,
			KKInterferenceCacheIntegrators_4D KK_4D)
	: FF_PDF_bkg_bbar_wrongphif0980("pdf_bkg_bbar_wrongphif0980_with_interference_9D_negative", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_mbc>(mbc,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_deltae>(deltae,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_nb_prime>(nb_prime,cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_1D_mass_kpi>(mass_kpi,cat,is_data));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_with_interference_4D_neg>(chil0_ma, hel_phi, hel_th1, hel_th2,
				cat, amps_phi, amps_flatte, acceptance_function, KK_4D));

		m_pdfs.push_back(new RooCategoryPdf(("C_"+m_label+"_flavor").c_str(),("C_"+m_label+"_flavor").c_str(),flavor));

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
						m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),
						m_comb_pdfs.at(4)->GetPDF("C"),*m_pdfs.back())));
		m_main_pdfs["C"] = m_pdfs.back();

		//		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
		//				RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
		//						m_comb_pdfs.at(2)->GetPDF("N"),m_comb_pdfs.at(3)->GetPDF("N"),
		//						m_comb_pdfs.at(4)->GetPDF("N"),m_comb_pdfs.at(5)->GetPDF("N"),
		//						m_comb_pdfs.at(6)->GetPDF("N"),m_comb_pdfs.at(7)->GetPDF("N"))));
		//		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_bkg_bbar_8D : public FF_PDF_bkg_bbar {
public:
	virtual ~FF_PDF_bkg_bbar_8D() { }

	FF_PDF_bkg_bbar_8D(shared_ptr<FF_PDF> combinatoric, shared_ptr<FF_PDF> wrongmasshyp,
			shared_ptr<FF_PDF> wrongphikkbar, shared_ptr<FF_PDF> wrongphif0980, double nevents,
			const FFUtil::tuple_category_name_flags& cat) : FF_PDF_bkg_bbar("pdf_bkg_bbar_8D",cat) {

		if(dynamic_cast<FF_PDF_bkg_bbar_combinatoric_8D*>(combinatoric.get())) {
			m_comb_pdfs.push_back(combinatoric);
		} else {
			std::cerr << "ERROR: PDF for combinatoric component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_combinatoric_8D" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_wrongmasshyp_8D*>(wrongmasshyp.get())) {
			m_comb_pdfs.push_back(wrongmasshyp);
		} else {
			std::cerr << "ERROR: PDF for wrongmasshyp component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_wrongmasshyp_8D" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_wrongphikkbar_8D*>(wrongphikkbar.get())) {
			m_comb_pdfs.push_back(wrongphikkbar);
		} else {
			std::cerr << "ERROR: PDF for wrongphikkbar component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_wrongphikkbar_8D" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_wrongphif0980_8D*>(wrongphif0980.get())) {
			m_comb_pdfs.push_back(wrongphif0980);
		} else {
			std::cerr << "ERROR: PDF for wrongphif0980 component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_wrongphif0980_8D" << std::endl;
			exit(EXIT_FAILURE);
		}

		m_vars.push_back(new RooRealVar("C_bkg_bbar_combinatoric_nevents","C_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongmasshyp_nevents","C_bkg_bbar_wrongmasshyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongphikkbar_nevents","C_bkg_bbar_wrongphikkbar_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongphif0980_nevents","C_bkg_bbar_wrongphif0980_nevents",nevents/7.0,0.0,nevents*1.2));

		m_vars.push_back(new RooRealVar("N_bkg_bbar_combinatoric_nevents","N_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_bkg_bbar_wrongphikkbar_nevents","N_bkg_bbar_wrongphikkbar_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_bkg_bbar_wrongphif0980_nevents","N_bkg_bbar_wrongphif0980_nevents",nevents/7.0,0.0,nevents*1.2));

		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
				m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C")),
				RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3)),false));
		m_pdfs.push_back(new RooAddPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(2)->GetPDF("N"),
				m_comb_pdfs.at(3)->GetPDF("N")),
				RooArgList(*m_vars.at(4),*m_vars.at(5),*m_vars.at(6)),false));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}

	FF_PDF_bkg_bbar_8D(RooRealVar& mbc, RooRealVar& deltae, RooRealVar& chil0_ma, RooRealVar& nb_prime,
			RooRealVar& mass_kpi, RooRealVar& hel_phi, RooRealVar& hel_th1, RooRealVar& hel_th2,
			double nevents, const FFUtil::tuple_category_name_flags& cat, bool is_data, RooRealVar* resolution)
	: FF_PDF_bkg_bbar("pdf_bkg_bbar_8D",cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_combinatoric_8D>(
				mbc,deltae,chil0_ma,nb_prime,
				mass_kpi,hel_phi,hel_th1,hel_th2,
				cat,is_data,resolution));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongmasshyp_8D>(
				mbc,deltae,chil0_ma,nb_prime,
				mass_kpi,hel_phi,hel_th1,hel_th2,
				cat,is_data,resolution));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphikkbar_8D>(
				mbc,deltae,chil0_ma,nb_prime,
				mass_kpi,hel_phi,hel_th1,hel_th2,
				cat,is_data));
		m_comb_pdfs.push_back(make_shared<FF_PDF_bkg_bbar_wrongphif0980_8D>(
				mbc,deltae,chil0_ma,nb_prime,
				mass_kpi,hel_phi,hel_th1,hel_th2,
				cat,is_data));

		m_vars.push_back(new RooRealVar("C_bkg_bbar_combinatoric_nevents","C_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongmasshyp_nevents","C_bkg_bbar_wrongmasshyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongphikkbar_nevents","C_bkg_bbar_wrongphikkbar_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongphif0980_nevents","C_bkg_bbar_wrongphif0980_nevents",nevents/7.0,0.0,nevents*1.2));

		m_vars.push_back(new RooRealVar("N_bkg_bbar_combinatoric_nevents","N_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_bkg_bbar_wrongphikkbar_nevents","N_bkg_bbar_wrongphikkbar_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_bkg_bbar_wrongphif0980_nevents","N_bkg_bbar_wrongphif0980_nevents",nevents/7.0,0.0,nevents*1.2));

		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
				m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C")),
				RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3)),false));
		m_pdfs.push_back(new RooAddPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(2)->GetPDF("N"),
				m_comb_pdfs.at(3)->GetPDF("N")),
				RooArgList(*m_vars.at(4),*m_vars.at(5),*m_vars.at(6)),false));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


#endif /* FF_PDF_BKG_BBAR_H_ */

