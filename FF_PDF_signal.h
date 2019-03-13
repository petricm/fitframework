/**
 * @file	FF_PDF_signal.h
 * @date	Jul 26, 2013
 * @author	petric
 * @brief	brief description
 *
 * long description
 */

#ifndef FF_PDF_SIGNAL_H_
#define FF_PDF_SIGNAL_H_

#include "FF_PDF.h"

class FF_PDF_signal : public FF_PDF {
public:
	virtual ~FF_PDF_signal() { }
protected:
	FF_PDF_signal(const std::string &label, const FFUtil::tuple_category_name_flags& cat) : FF_PDF(label) {
		m_category_name_flags = cat;
		m_pdf_color = kRed;
		m_pdf_linestyle = kDashed;
		m_pdf_legend_label = "Signal";
	}
};

class FF_PDF_signal_1D_mbc : public FF_PDF_signal {
public:
	virtual ~FF_PDF_signal_1D_mbc() { }

	FF_PDF_signal_1D_mbc(RooRealVar& mbc, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_signal("pdf_signal_1D_mbc",cat) {

        m_vars.push_back(new RooRealVar("mbc_signal_mean_1st","mbc signal mean 1st",5.2794718017500388,5.26,5.29));
        //m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mbc_signal_width_1st","mbc signal width 1st",0.0026353825890831962,0.000,0.1));
        //m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mbc_signal_tail_1st","mbc signal tail 1st",-0.046577032461130985,-1,1));
        m_vars.back()->setConstant(true);

        
        m_vars.push_back(new RooRealVar("mbc_signal_mean_2st","mbc signal mean 2st",5.2796286015544451,5.26,5.29));
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mbc_signal_width_2st","mbc signal width 2st",0.0026783092576333103,0.000,0.1));
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mbc_signal_tail_2st","mbc signal tail 2st",0.1162771480572129,-1,1));
        m_vars.back()->setConstant(true);

        
        m_vars.push_back(new RooRealVar("mbc_signal_mean_3st","mbc signal mean 3st",5.2792550594809784,5.26,5.29));
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mbc_signal_width_3st","mbc signal width 3st",0.0055537237958158299,0.000,0.1));
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("mbc_signal_tail_3st","mbc signal tail 3st",0.66745801916136283,-1,1));
        m_vars.back()->setConstant(true);
        
		m_pdfs.push_back(new RooNovosibirsk((""+m_label+"_N1").c_str(),(""+m_label+"_N1").c_str(),mbc,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2)));
		m_pdfs.push_back(new RooNovosibirsk((""+m_label+"_N2").c_str(),(""+m_label+"_N2").c_str(),mbc,*m_vars.at(3),*m_vars.at(4),*m_vars.at(5)));
		m_pdfs.push_back(new RooNovosibirsk((""+m_label+"_N3").c_str(),(""+m_label+"_N3").c_str(),mbc,*m_vars.at(6),*m_vars.at(7),*m_vars.at(8)));
        
        m_vars.push_back(new RooRealVar("mbc_signal_frac_1","mbc signal frac 1",0.52576255340783229,0.0,1.0));
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("mbc_signal_frac_2","mbc signal frac 2",0.46069545569568593,0.0,1.0));
        m_vars.back()->setConstant(true);
        
		m_pdfs.push_back(new RooAddPdf((""+m_label+"").c_str(),(""+m_label+"").c_str(),RooArgList(*m_pdfs.at(0),*m_pdfs.at(1),*m_pdfs.at(2)),RooArgList(*m_vars.at(9),*m_vars.at(10))));

		m_main_pdfs["C"] = m_pdfs.at(3);
		m_main_pdfs["CC"] = m_pdfs.at(3);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_signal_1D_deltae : public FF_PDF_signal {
public:
	virtual ~FF_PDF_signal_1D_deltae() { }

	FF_PDF_signal_1D_deltae(RooRealVar& deltae, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_signal("pdf_signal_1D_deltae",cat) {
        
        m_vars.push_back(new RooRealVar("deltae_signal_mean","deltae signal mean",0,-0.1,0.1)); //V0 "
        //m_vars.back()->setConstant(true);
        
        m_vars.push_back(new RooRealVar("deltae_signal_mean_1st","deltae signal mean 1st",-0.070022023360429397,-1,1));
        m_vars.back()->setConstant(true);
        m_functions.push_back(new RooFormulaVar("deltae_signal_mean_off_1","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(1)))); //F0
		m_vars.push_back(new RooRealVar("deltae_signal_width_1st","deltae signal width 1st",0.061419702916792884,0.000,5));
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("deltae_signal_tail_1st","deltae signal tail 1st",-0.24539234522788622,-1,1));
        m_vars.back()->setConstant(true);
        
        
        m_vars.push_back(new RooRealVar("deltae_signal_mean_2nd","deltae signal mean 2nd",-0.010450352174830368,-1,1));
        m_vars.back()->setConstant(true);
        m_functions.push_back(new RooFormulaVar("deltae_signal_mean_off_2","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(4)))); //F0
		m_vars.push_back(new RooRealVar("deltae_signal_width_2nd","deltae signal width 2nd",0.025503035113143002,0.000,5));
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("deltae_signal_tail_2nd","deltae signal tail 2nd",0.24919373515419108,-1,1));
        m_vars.back()->setConstant(true);
        
        
		m_vars.push_back(new RooRealVar("deltae_signal_mean_3rd","deltae signal mean  3rd",0.013974757186238612,-0.1,0.1)); //V5
        m_vars.back()->setConstant(true);
        m_functions.push_back(new RooFormulaVar("deltae_signal_mean_off_3","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(7)))); //F0
		m_vars.push_back(new RooRealVar("deltae_signal_width_3rd","deltae signal width 3rd",0.023790432590342608,0.000,0.3)); //V6
        //m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("deltae_signal_alpha","deltae signal alpha",1.9281234566208432,0.0,2.0)); //V9
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("deltae_signal_n","deltae signal n",0.14028085683372804,0.0,6.0)); //V10
        m_vars.back()->setConstant(true);
        
		m_pdfs.push_back(new RooNovosibirsk((""+m_label+"_N1").c_str(),(""+m_label+"_N1").c_str(),deltae,*m_functions.at(0),*m_vars.at(2),*m_vars.at(3)));
		m_pdfs.push_back(new RooNovosibirsk((""+m_label+"_N2").c_str(),(""+m_label+"_N2").c_str(),deltae,*m_functions.at(1),*m_vars.at(5),*m_vars.at(6)));
		m_pdfs.push_back(new RooCBShape((""+m_label+"_CBS").c_str(),(""+m_label+" CBS").c_str(),deltae,*m_functions.at(2),*m_vars.at(8),*m_vars.at(9),*m_vars.at(10)));
        
        m_vars.push_back(new RooRealVar("deltae_signal_frac_1","deltae signal frac 1",0.16833555780389831,0.0,1.0));
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("deltae_signal_frac_2","deltae signal frac 2",0.42882769902398465,0.0,1.0));
        m_vars.back()->setConstant(true);
        
		m_pdfs.push_back(new RooAddPdf((""+m_label+"").c_str(),(""+m_label+"").c_str(),RooArgList(*m_pdfs.at(0),*m_pdfs.at(1),*m_pdfs.at(2)),RooArgList(*m_vars.at(11),*m_vars.at(12))));

		m_main_pdfs["C"] = m_pdfs.at(3);
		m_main_pdfs["CC"] = m_pdfs.at(3);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_signal_2D_mbc_deltae: public FF_PDF_signal {
public:
	virtual ~FF_PDF_signal_2D_mbc_deltae() { }

	FF_PDF_signal_2D_mbc_deltae(RooRealVar& mbc, RooRealVar& deltae,
			shared_ptr<FF_PDF> mbc_pdf,
			const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_signal("pdf_signal_2D_mbc_deltae", cat) {
		// we reuse the 1D mbc fit
		m_comb_pdfs.push_back(mbc_pdf);
		// and fix all its parameters
		for(RooRealVar *v : m_comb_pdfs.back()->GetVariables()) {
			v->setConstant(true);
		}
        m_functions.push_back(new RooFormulaVar("mbc-mbc0","@0-5.27958",RooArgList(mbc))); //F0
        
        //Description of the mean
		m_vars.push_back(new RooRealVar("deltae_signal_mean_1st_polyvar_mbc_c0","deltae_signal_mean_1st_polyvar_mbc_c0",-1.22496e-04,-1,1)); //V0
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("deltae_signal_mean_1st_polyvar_mbc_c1","deltae_signal_mean_1st_polyvar_mbc_c1",-9.76732e-01,-10,10)); //V1
        m_vars.back()->setConstant(true);
		m_functions.push_back(new RooPolyVar("deltae_signal_mean_1st_polyvar_mbc","deltae_signal_mean_1st_polyvar_mbc",*m_functions.at(0),RooArgList(*m_vars.at(0),*m_vars.at(1)))); //F1
        
        m_vars.push_back(new RooRealVar("deltae_signal_mean_1st","deltae signal mean 1st",-0.070022023360429397,-1,1)); //V2
        m_vars.back()->setConstant(true);
        m_functions.push_back(new RooFormulaVar("deltae_signal_mean_off_1","@0+@1",RooArgList(*m_functions.at(1),*m_vars.at(2)))); //F2
		m_vars.push_back(new RooRealVar("deltae_signal_width_1st","deltae signal width 1st",0.061419702916792884,0.000,5)); //V3
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("deltae_signal_tail_1st","deltae signal tail 1st",-0.24539234522788622,-1,1));  //V4
        m_vars.back()->setConstant(true);
        
        m_vars.push_back(new RooRealVar("deltae_signal_mean_2nd","deltae signal mean 2nd",-0.010450352174830368,-1,1)); //V5
        m_vars.back()->setConstant(true);
        m_functions.push_back(new RooFormulaVar("deltae_signal_mean_off_2","@0+@1",RooArgList(*m_functions.at(1),*m_vars.at(5)))); //F3
		m_vars.push_back(new RooRealVar("deltae_signal_width_2nd","deltae signal width 2nd",0.025503035113143002,0.000,5)); //V6
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("deltae_signal_tail_2nd","deltae signal tail 2nd",0.24919373515419108,-1,1)); //V7
        m_vars.back()->setConstant(true);
        
		m_vars.push_back(new RooRealVar("deltae_signal_mean_3rd","deltae signal mean  3rd",0.013974757186238612,-0.1,0.1)); //V8
        m_vars.back()->setConstant(true);
        m_functions.push_back(new RooFormulaVar("deltae_signal_mean_off_3","@0+@1",RooArgList(*m_functions.at(1),*m_vars.at(8)))); //F4
		m_vars.push_back(new RooRealVar("deltae_signal_width_3rd","deltae signal width 3rd",0.023790432590342608,0.000,0.3)); //V9
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("deltae_signal_alpha","deltae signal alpha",1.9281234566208432,0.0,2.0)); //V10
        m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("deltae_signal_n","deltae signal n",0.14028085683372804,0.0,6.0)); //V11
        m_vars.back()->setConstant(true);
        
		m_pdfs.push_back(new RooNovosibirsk((""+m_label+"_N1").c_str(),(""+m_label+"_N1").c_str(),deltae,*m_functions.at(2),*m_vars.at(3),*m_vars.at(4)));
		m_pdfs.push_back(new RooNovosibirsk((""+m_label+"_N2").c_str(),(""+m_label+"_N2").c_str(),deltae,*m_functions.at(3),*m_vars.at(6),*m_vars.at(7)));
		m_pdfs.push_back(new RooCBShape((""+m_label+"_CBS").c_str(),(""+m_label+" CBS").c_str(),deltae,*m_functions.at(4),*m_vars.at(9),*m_vars.at(10),*m_vars.at(11)));
        
        m_vars.push_back(new RooRealVar("deltae_signal_frac_1","deltae signal frac 1",0.16833555780389831,0.0,1.0)); //V12
        m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("deltae_signal_frac_2","deltae signal frac 2",0.42882769902398465,0.0,1.0)); //V13
        m_vars.back()->setConstant(true);
        
		m_pdfs.push_back(new RooAddPdf((""+m_label+"").c_str(),(""+m_label+"").c_str(),RooArgList(*m_pdfs.at(0),*m_pdfs.at(1),*m_pdfs.at(2)),RooArgList(*m_vars.at(12),*m_vars.at(13))));

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),m_comb_pdfs.at(0)->GetPDF("C"),Conditional(*m_pdfs.at(3),deltae)));

		m_main_pdfs["C"] = m_pdfs.at(4);
		m_main_pdfs["N"] = m_pdfs.at(4);
		m_main_pdfs["CC"] = m_pdfs.at(4);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_signal_1D_chil0_ma : public FF_PDF_signal {
public:
	virtual ~FF_PDF_signal_1D_chil0_ma() { }

	FF_PDF_signal_1D_chil0_ma(RooRealVar& chil0_ma, const FFUtil::tuple_category_name_flags& cat, bool is_data, RooRealVar* resolution) : FF_PDF_signal("pdf_signal_1D_chil0_ma",cat) {
		// Relativistic Spin Breit-Wigner or Two Body PSHP Breit-Wigner
		m_vars.push_back(new RooRealVar("chil0_ma_signal_mean","chil0_ma signal mean",1.019455,1.019435,1.019475));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("chil0_ma_signal_width","chil0_ma signal width",0.00426,0.00422,0.00430));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("chil0_ma_signal_r","chil0_ma signal r",3.0));
		m_vars.push_back(new RooRealVar("chil0_ma_signal_J","chil0_ma signal J",1.0));
		if(is_data) {
			m_pdfs.push_back(new RooRelativisticSpinBreitWigner(("C_"+m_label+"_BW").c_str(),("C_"+m_label+"_BW").c_str(),chil0_ma,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),m_K,m_K));
			m_pdfs.push_back(new RooRelativisticSpinBreitWigner(("N_"+m_label+"_BW").c_str(),("N_"+m_label+"_BW").c_str(),chil0_ma,*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),m_K,m_K));
		} else {
			m_pdfs.push_back(new RooBreitWignerTwoBodyPHSP(("C_"+m_label+"_BW").c_str(),("C_"+m_label+"_BW").c_str(),chil0_ma,*m_vars.at(0),*m_vars.at(1),m_K,m_K));
			m_pdfs.push_back(new RooBreitWignerTwoBodyPHSP(("N_"+m_label+"_BW").c_str(),("N_"+m_label+"_BW").c_str(),chil0_ma,*m_vars.at(0),*m_vars.at(1),m_K,m_K));
		}

		// Gaussian resolution
		if(resolution != NULL) {
			m_pdfs.push_back(new RooGaussian(("C_"+m_label+"_resolution").c_str(),("C_"+m_label+"_resolution").c_str(),chil0_ma,RooConst(0.0),*resolution));
			m_pdfs.push_back(new RooGaussian(("N_"+m_label+"_resolution").c_str(),("N_"+m_label+"_resolution").c_str(),chil0_ma,RooConst(0.0),*resolution));
		} else {
			m_vars.push_back(new RooRealVar("chil0_ma_resolution","chil0_ma resolution",0.0008,0.0002,0.0014));
			m_pdfs.push_back(new RooGaussian(("C_"+m_label+"_resolution").c_str(),("C_"+m_label+"_resolution").c_str(),chil0_ma,RooConst(0.0),*m_vars.at(4)));
			m_pdfs.push_back(new RooGaussian(("N_"+m_label+"_resolution").c_str(),("N_"+m_label+"_resolution").c_str(),chil0_ma,RooConst(0.0),*m_vars.at(4)));
		}

		// Breit-Wigner folded with Gaussian resolution
		m_pdfs.push_back(new RooFFTConvPdf(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),chil0_ma,*m_pdfs.at(0),*m_pdfs.at(2)));
		m_pdfs.push_back(new RooFFTConvPdf(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),chil0_ma,*m_pdfs.at(1),*m_pdfs.at(3)));
		// some settings for the convolution
		dynamic_cast<RooFFTConvPdf*>(m_pdfs.at(4))->setBufferFraction(1.2);
		dynamic_cast<RooFFTConvPdf*>(m_pdfs.at(5))->setBufferFraction(1.2);
		chil0_ma.setBins(10000,"cache");

		m_main_pdfs["C"] = m_pdfs.at(4);
		m_main_pdfs["N"] = m_pdfs.at(5);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_signal_1D_nb_prime : public FF_PDF_signal {
public:
	virtual ~FF_PDF_signal_1D_nb_prime() { }

	FF_PDF_signal_1D_nb_prime(RooRealVar& nb_prime, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_signal("pdf_signal_1D_nb_prime",cat) {
		m_vars.push_back(new RooRealVar("nb_prime_signal_mean","nb prime signal mean",2.61016,-1.0,5.0));//V0
		//m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("nb_prime_signal_mean_1st_off","nb prime signal mean 1st off",0.31252745972366114,-5.0,5.0));//V1
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("nb_prime_signal_mean_2nd_off","nb prime signal mean 2nd off",1.9813801475873092,-5.0,5.0));//V2
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("nb_prime_signal_mean_3rd_off","nb prime signal mean 3rd off",-1.6140588068691031,-5.0,5.0));//V3
		m_vars.back()->setConstant(true);
        
        m_functions.push_back(new RooFormulaVar("nb_prime_signal_mean_1st","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(1)))); //F0
        m_functions.push_back(new RooFormulaVar("nb_prime_signal_mean_2nd","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(2)))); //F1
        m_functions.push_back(new RooFormulaVar("nb_prime_signal_mean_3rd","@0+@1",RooArgList(*m_vars.at(0),*m_vars.at(3)))); //F2
        
        
		m_vars.push_back(new RooRealVar("nb_prime_signal_width","nb prime signal width",1.82359,-5.0,5.0));//V4
		//m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("nb_prime_signal_width_1st_off","nb prime signal width 1st off",0.83156226988438409,-5.0,5.0));//V5
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("nb_prime_signal_width_2nd_off","nb prime signal width 2nd off",0.6376062582526254,-5.0,5.0));//V6
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("nb_prime_signal_width_3rd_off","nb prime signal width 3rd off",0.8505040303602982,-5.0,5.0));//V7
		m_vars.back()->setConstant(true);
        
        m_functions.push_back(new RooFormulaVar("nb_prime_signal_width_1st","@0*@1",RooArgList(*m_vars.at(4),*m_vars.at(5)))); //F3
        m_functions.push_back(new RooFormulaVar("nb_prime_signal_width_2nd","@0*@1",RooArgList(*m_vars.at(4),*m_vars.at(6)))); //F4
        m_functions.push_back(new RooFormulaVar("nb_prime_signal_width_3rd","@0*@1",RooArgList(*m_vars.at(4),*m_vars.at(7)))); //F5
        
        
        m_vars.push_back(new RooRealVar("nb_prime_signal_tail","nb prime signal tail",0.35347001561612412,-1.0,5.0));//V8
		//m_vars.back()->setConstant(true);
        m_vars.push_back(new RooRealVar("nb_prime_signal_tail_1st_off","nb prime signal tail 1st off",0.8580679530939781,-5.0,5.0));//V9
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("nb_prime_signal_tail_2nd_off","nb prime signal tail 2nd off",1.3494723179033654,-5.0,5.0));//V10
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("nb_prime_signal_tail_3rd_off","nb prime signal tail 3rd off",0.25758403733031443,-5.0,5.0));//V11
		m_vars.back()->setConstant(true);
        
        m_functions.push_back(new RooFormulaVar("nb_prime_signal_tail_1st","@0*@1",RooArgList(*m_vars.at(8),*m_vars.at(9)))); //F6
        m_functions.push_back(new RooFormulaVar("nb_prime_signal_tail_2nd","@0*@1",RooArgList(*m_vars.at(8),*m_vars.at(10)))); //F7
        m_functions.push_back(new RooFormulaVar("nb_prime_signal_tail_3rd","@0*@1",RooArgList(*m_vars.at(8),*m_vars.at(11)))); //F8
        
        
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_1st").c_str(),("C_"+m_label+"_1st").c_str(),nb_prime,*m_functions.at(0),*m_functions.at(3),*m_functions.at(6)));
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_2nd").c_str(),("C_"+m_label+"_2nd").c_str(),nb_prime,*m_functions.at(1),*m_functions.at(4),*m_functions.at(7)));
		m_pdfs.push_back(new RooNovosibirsk(("C_"+m_label+"_3rd").c_str(),("C_"+m_label+"_3rd").c_str(),nb_prime,*m_functions.at(2),*m_functions.at(5),*m_functions.at(8)));
        
        
		m_vars.push_back(new RooRealVar("nb_prime_signal_frac_1st","nb_prime_signal fraction 1st",0.47201675411174693,0.1,0.9));//V12
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("nb_prime_signal_frac_2nd","nb_prime_signal fraction 2nd",0.28381426266752979,0.1,0.9));//V13
		m_vars.back()->setConstant(true);
        
		m_pdfs.push_back(new RooAddPdf((""+m_label+"").c_str(),(""+m_label+"").c_str(),RooArgList(*m_pdfs.at(0),*m_pdfs.at(1),*m_pdfs.at(2)),RooArgList(*m_vars.at(12),*m_vars.at(13))));
        
		m_main_pdfs["C"] = m_pdfs.at(3);
		m_main_pdfs["CC"] = m_pdfs.at(3);
        m_main_pdfs["CP"] = m_pdfs.at(3);
		m_main_pdfs["CA"] = m_pdfs.at(3);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_signal_3D_mbc_deltae_nb_prime: public FF_PDF_signal {
public:
	virtual ~FF_PDF_signal_3D_mbc_deltae_nb_prime() { }
    
	FF_PDF_signal_3D_mbc_deltae_nb_prime(RooRealVar& mbc, RooRealVar& deltae,RooRealVar& nb_prime,
                                         const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_signal("pdf_signal_3D_mbc_deltae_nb_prime", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_signal_2D_mbc_deltae>(mbc,deltae,
                                                                       make_shared<FF_PDF_signal_1D_mbc>(mbc,cat),cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_signal_1D_nb_prime>(nb_prime,cat));
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
                                        RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
        m_main_pdfs["CP"] = m_pdfs.at(0);
		m_main_pdfs["CA"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
    
	FF_PDF_signal_3D_mbc_deltae_nb_prime(shared_ptr<FF_PDF> mbc_deltae, shared_ptr<FF_PDF> nb_prime,
                                         const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_signal("pdf_signal_3D_mbc_deltae_nb_prime", cat) {
		m_comb_pdfs.push_back(mbc_deltae);
		m_comb_pdfs.push_back(nb_prime);
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
                                        RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"))));
        
		m_main_pdfs["C"] = m_pdfs.at(0);
        m_main_pdfs["CP"] = m_pdfs.at(0);
		m_main_pdfs["CA"] = m_pdfs.at(0);
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
    
};


class FF_PDF_signal_4D_mbc_deltae_chil0_ma_nb_prime: public FF_PDF_signal {
public:
	virtual ~FF_PDF_signal_4D_mbc_deltae_chil0_ma_nb_prime() { }

	FF_PDF_signal_4D_mbc_deltae_chil0_ma_nb_prime(RooRealVar& mbc, RooRealVar& deltae,
			RooRealVar& chil0_ma, RooRealVar& nb_prime,
			const FFUtil::tuple_category_name_flags& cat, bool is_data, RooRealVar* resolution)
	: FF_PDF_signal("pdf_signal_4D_mbc_deltae_chil0_ma_nb_prime", cat) {
		m_comb_pdfs.push_back(make_shared<FF_PDF_signal_2D_mbc_deltae>(mbc,deltae,
				make_shared<FF_PDF_signal_1D_mbc>(mbc,cat),cat));
		m_comb_pdfs.push_back(make_shared<FF_PDF_signal_1D_chil0_ma>(chil0_ma,cat,is_data,resolution));
		m_comb_pdfs.push_back(make_shared<FF_PDF_signal_1D_nb_prime>(nb_prime,cat));

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
						m_comb_pdfs.at(2)->GetPDF("C"))));
		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
						m_comb_pdfs.at(2)->GetPDF("N"))));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}

	FF_PDF_signal_4D_mbc_deltae_chil0_ma_nb_prime(shared_ptr<FF_PDF> mbc_deltae,
			shared_ptr<FF_PDF> chil0_ma, shared_ptr<FF_PDF> nb_prime,
			const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_signal("pdf_signal_4D_mbc_deltae_chil0_ma_nb_prime", cat) {
		m_comb_pdfs.push_back(mbc_deltae);
		m_comb_pdfs.push_back(chil0_ma);
		m_comb_pdfs.push_back(nb_prime);

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
						m_comb_pdfs.at(2)->GetPDF("C"))));
		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
						m_comb_pdfs.at(2)->GetPDF("N"))));

		m_main_pdfs["C"] = m_pdfs.at(0);
		m_main_pdfs["N"] = m_pdfs.at(1);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}

};

class FF_PDF_signal_4D_mass_helicity_angles : public FF_PDF_signal {
public:
	virtual ~FF_PDF_signal_4D_mass_helicity_angles() { }

	FF_PDF_signal_4D_mass_helicity_angles(RooRealVar& mass_kpi, RooRealVar& hel_th1,
			RooRealVar& hel_th2, RooRealVar& hel_phi,
			ACCEPTANCE_FUNCTION _acceptance_function,
			MHI_PDG_constants _s_PDG,
			MHI_CacheIntegrators_4D _s_4D,
			MHI_CacheIntegrators_3D_phi_th1_th2 _s_3D_phi_th1_th2,
			MHI_CacheIntegrators_3D_m_th1_th2 _s_3D_m_th1_th2,
			MHI_CacheIntegrators_3D_m_phi_th2 _s_3D_m_phi_th2,
			MHI_CacheIntegrators_3D_m_phi_th1 _s_3D_m_phi_th1,
			const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_signal("FF_PDF_signal_4D_mass_helicity_angles",cat) {

		// the BW amplitude values (they are all fixed)
		// TODO: Need to be varied by uncertainties (see BaBar paper)
		m_vars.push_back(new RooRealVar("mass_0","mass_0",1.435));
		m_vars.push_back(new RooRealVar("width_0","width_0",0.279));
		m_vars.push_back(new RooRealVar("a","a",1.95));
		m_vars.push_back(new RooRealVar("b","b",1.76));
		m_vars.push_back(new RooRealVar("mass_1","mass_1",0.896));
		m_vars.push_back(new RooRealVar("width_1","width_1",0.0503));
		m_vars.push_back(new RooRealVar("r_1","r_1",3.4));
		m_vars.push_back(new RooRealVar("mass_2","mass_2",1.432));
		m_vars.push_back(new RooRealVar("width_2","width_2",0.109));
		m_vars.push_back(new RooRealVar("r_2","r_2",2.7));

		// the mean variables (helicity amplitudes and phases) (Start Index 10)
		m_vars.push_back(new RooRealVar("amp_00","amp_00",0.75,0.0,2.0));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("phase_00","phase_00",0.0));

		m_vars.push_back(new RooRealVar("amp_1m","amp_1m",0.4,0.0,2.0));
		m_vars.push_back(new RooRealVar("phase_1m","phase_1m",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_10","amp_10",0.707,0.0,2.0));
		m_vars.push_back(new RooRealVar("phase_10","phase_10",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_1p","amp_1p",0.6,0.0,2.0));
		m_vars.push_back(new RooRealVar("phase_1p","phase_1p",0.0,-2*TMath::Pi(),2*TMath::Pi()));

		m_vars.push_back(new RooRealVar("amp_2m","amp_2m",0.22*0.9,0.0,2.0));
		m_vars.push_back(new RooRealVar("phase_2m","phase_2m",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_20","amp_20",0.95*0.9,0.0,2.0));
		m_vars.push_back(new RooRealVar("phase_20","phase_20",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_2p","amp_2p",0.22*0.9,0.0,2.0));
		m_vars.push_back(new RooRealVar("phase_2p","phase_2p",0.0,-2*TMath::Pi(),2*TMath::Pi()));

		m_pdfs.push_back(new RooMassHelicityAngles(("C_"+m_label).c_str(),("C_"+m_label).c_str(),mass_kpi,hel_phi,hel_th1,hel_th2,
				/*mass_0,width_0,a,b,mass_1,width_1,r_1,mass_2,width_2,r_2,*/
				*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5),*m_vars.at(6),*m_vars.at(7),*m_vars.at(8),*m_vars.at(9),
				/*amp_00,phase_00,*/
				*m_vars.at(10),*m_vars.at(11),
				/*amp_1m,phase_1m,amp_10,phase_10,amp_1p,phase_1p,*/
				*m_vars.at(12),*m_vars.at(13),*m_vars.at(14),*m_vars.at(15),*m_vars.at(16),*m_vars.at(17),
				/*amp_2m,phase_2m,amp_20,phase_20,amp_2p,phase_2p,*/
				*m_vars.at(18),*m_vars.at(19),*m_vars.at(20),*m_vars.at(21),*m_vars.at(22),*m_vars.at(23),
				_acceptance_function,
				_s_PDG,
				_s_4D,
				_s_3D_phi_th1_th2,
				_s_3D_m_th1_th2,
				_s_3D_m_phi_th2,
				_s_3D_m_phi_th1));
		m_main_pdfs["C"] = m_pdfs.back();

		m_pdfs.push_back(new RooMassHelicityAngles(("N_"+m_label).c_str(),("N_"+m_label).c_str(),mass_kpi,hel_phi,hel_th1,hel_th2,
				/*mass_0,width_0,a,b,mass_1,width_1,r_1,mass_2,width_2,r_2,*/
				*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5),*m_vars.at(6),*m_vars.at(7),*m_vars.at(8),*m_vars.at(9),
				/*amp_00,phase_00,*/
				*m_vars.at(10),*m_vars.at(11),
				/*amp_1m,phase_1m,amp_10,phase_10,amp_1p,phase_1p,*/
				*m_vars.at(12),*m_vars.at(13),*m_vars.at(14),*m_vars.at(15),*m_vars.at(16),*m_vars.at(17),
				/*amp_2m,phase_2m,amp_20,phase_20,amp_2p,phase_2p,*/
				*m_vars.at(18),*m_vars.at(19),*m_vars.at(20),*m_vars.at(21),*m_vars.at(22),*m_vars.at(23),
				_acceptance_function,
				_s_PDG,
				_s_4D,
				_s_3D_phi_th1_th2,
				_s_3D_m_th1_th2,
				_s_3D_m_phi_th2,
				_s_3D_m_phi_th1));
		m_main_pdfs["N"] = m_pdfs.back();

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_signal_4D_mass_helicity_angles_TransformedBase : public FF_PDF_signal {
public:
	virtual ~FF_PDF_signal_4D_mass_helicity_angles_TransformedBase() { }

	FF_PDF_signal_4D_mass_helicity_angles_TransformedBase(RooRealVar& mass_kpi, RooRealVar& hel_th1,
			RooRealVar& hel_th2, RooRealVar& hel_phi,
			ACCEPTANCE_FUNCTION _acceptance_function,
			MHI_PDG_constants _s_PDG,
			MHI_CacheIntegrators_4D _s_4D,
			MHI_CacheIntegrators_3D_phi_th1_th2 _s_3D_phi_th1_th2,
			MHI_CacheIntegrators_3D_m_th1_th2 _s_3D_m_th1_th2,
			MHI_CacheIntegrators_3D_m_phi_th2 _s_3D_m_phi_th2,
			MHI_CacheIntegrators_3D_m_phi_th1 _s_3D_m_phi_th1,
			const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_signal("FF_PDF_signal_4D_mass_helicity_angles_TransformedBase",cat) {

		// the BW amplitude values (they are all fixed)
		// TODO: Need to be varied by uncertainties (see BaBar paper)
		m_vars.push_back(new RooRealVar("mass_0","mass_0",1.435));
		m_vars.push_back(new RooRealVar("width_0","width_0",0.279));
		m_vars.push_back(new RooRealVar("a","a",1.95));
		m_vars.push_back(new RooRealVar("b","b",1.76));
		m_vars.push_back(new RooRealVar("mass_1","mass_1",0.896));
		m_vars.push_back(new RooRealVar("width_1","width_1",0.0503));
		m_vars.push_back(new RooRealVar("r_1","r_1",3.4));
		m_vars.push_back(new RooRealVar("mass_2","mass_2",1.432));
		m_vars.push_back(new RooRealVar("width_2","width_2",0.109));
		m_vars.push_back(new RooRealVar("r_2","r_2",2.7));

		// the mean variables (helicity amplitudes and phases) (Start Index 10)
		m_vars.push_back(new RooRealVar("amp_00","amp_00",0.75,0.0,2.0));
		m_vars.push_back(new RooRealVar("phase_00","phase_00",0.0));

		m_vars.push_back(new RooRealVar("amp_10","amp_10",0.707,0.0,2.0));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("phase_10","phase_10",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_1para","amp_1para",0.4,0.0,2.0));
		m_vars.push_back(new RooRealVar("phase_1para","phase_1para",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_1perp","amp_1perp",0.6,0.0,2.0));
		m_vars.push_back(new RooRealVar("phase_1perp","phase_1perp",0.0,-2*TMath::Pi(),2*TMath::Pi()));

		m_vars.push_back(new RooRealVar("amp_20","amp_20",0.95*0.9,0.0,2.0));
		m_vars.push_back(new RooRealVar("phase_20","phase_20",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_2para","amp_2para",0.22*0.9,0.0,2.0));
		m_vars.push_back(new RooRealVar("phase_2para","phase_2para",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_2perp","amp_2perp",0.22*0.9,0.0,2.0));
		m_vars.push_back(new RooRealVar("phase_2perp","phase_2perp",0.0,-2*TMath::Pi(),2*TMath::Pi()));

		m_pdfs.push_back(new RooMassHelicityAngles_TransformedBase(("C_"+m_label).c_str(),("C_"+m_label).c_str(),mass_kpi,hel_phi,hel_th1,hel_th2,
				/*mass_0,width_0,a,b,mass_1,width_1,r_1,mass_2,width_2,r_2,*/
				*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5),*m_vars.at(6),*m_vars.at(7),*m_vars.at(8),*m_vars.at(9),
				/*amp_00,phase_00,*/
				*m_vars.at(10),*m_vars.at(11),
				/*amp_10,phase_10,amp_1para,phase_1para,amp_1perp,phase_1perp,*/
				*m_vars.at(12),*m_vars.at(13),*m_vars.at(14),*m_vars.at(15),*m_vars.at(16),*m_vars.at(17),
				/*amp_20,phase_20,amp_2para,phase_2para,amp_2perp,phase_2perp,*/
				*m_vars.at(18),*m_vars.at(19),*m_vars.at(20),*m_vars.at(21),*m_vars.at(22),*m_vars.at(23),
				_acceptance_function,
				_s_PDG,
				_s_4D,
				_s_3D_phi_th1_th2,
				_s_3D_m_th1_th2,
				_s_3D_m_phi_th2,
				_s_3D_m_phi_th1));
		m_main_pdfs["C"] = m_pdfs.back();

		m_pdfs.push_back(new RooMassHelicityAngles_TransformedBase(("N_"+m_label).c_str(),("N_"+m_label).c_str(),mass_kpi,hel_phi,hel_th1,hel_th2,
				/*mass_0,width_0,a,b,mass_1,width_1,r_1,mass_2,width_2,r_2,*/
				*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5),*m_vars.at(6),*m_vars.at(7),*m_vars.at(8),*m_vars.at(9),
				/*amp_00,phase_00,*/
				*m_vars.at(10),*m_vars.at(11),
				/*amp_10,phase_10,amp_1para,phase_1para,amp_1perp,phase_1perp,*/
				*m_vars.at(12),*m_vars.at(13),*m_vars.at(14),*m_vars.at(15),*m_vars.at(16),*m_vars.at(17),
				/*amp_20,phase_20,amp_2para,phase_2para,amp_2perp,phase_2perp,*/
				*m_vars.at(18),*m_vars.at(19),*m_vars.at(20),*m_vars.at(21),*m_vars.at(22),*m_vars.at(23),
				_acceptance_function,
				_s_PDG,
				_s_4D,
				_s_3D_phi_th1_th2,
				_s_3D_m_th1_th2,
				_s_3D_m_phi_th2,
				_s_3D_m_phi_th1));
		m_main_pdfs["N"] = m_pdfs.back();

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};




class FF_PDF_signal_5D_mass_helicity_angles_CPV : public FF_PDF_signal {
public:
	virtual ~FF_PDF_signal_5D_mass_helicity_angles_CPV() { }

	FF_PDF_signal_5D_mass_helicity_angles_CPV(RooRealVar& mass_kpi, RooRealVar& hel_th1,
			RooRealVar& hel_th2, RooRealVar& hel_phi, RooCategory& flavor,
			ACCEPTANCE_FUNCTION _acceptance_function,
			MHI_PDG_constants _s_PDG,
			MHI_CacheIntegrators_4D _s_4D,
			MHI_CacheIntegrators_3D_phi_th1_th2 _s_3D_phi_th1_th2,
			MHI_CacheIntegrators_3D_m_th1_th2 _s_3D_m_th1_th2,
			MHI_CacheIntegrators_3D_m_phi_th2 _s_3D_m_phi_th2,
			MHI_CacheIntegrators_3D_m_phi_th1 _s_3D_m_phi_th1,
			const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_signal("FF_PDF_signal_5D_mass_helicity_angles_CPV",cat) {

		// the BW amplitude values (they are all fixed)
		// TODO: Need to be varied by uncertainties (see BaBar paper)
		m_vars.push_back(new RooRealVar("mass_0","mass_0",1.435));
		m_vars.push_back(new RooRealVar("width_0","width_0",0.279));
		m_vars.push_back(new RooRealVar("a","a",1.95));
		m_vars.push_back(new RooRealVar("b","b",1.76));
		m_vars.push_back(new RooRealVar("mass_1","mass_1",0.896));
		m_vars.push_back(new RooRealVar("width_1","width_1",0.0503));
		m_vars.push_back(new RooRealVar("r_1","r_1",3.4));
		m_vars.push_back(new RooRealVar("mass_2","mass_2",1.432));
		m_vars.push_back(new RooRealVar("width_2","width_2",0.109));
		m_vars.push_back(new RooRealVar("r_2","r_2",2.7));

		// the mean variables (helicity amplitudes and phases) (Start Index 10)
		m_vars.push_back(new RooRealVar("amp_00","amp_00",0.75,0.0,1.0));
		m_vars.push_back(new RooRealVar("phase_00","phase_00",0.0));

		m_vars.push_back(new RooRealVar("amp_10","amp_10",0.707,0.0,1.0));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("phase_10","phase_10",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_1para","amp_1para",0.4,0.0,1.0));
		m_vars.push_back(new RooRealVar("phase_1para","phase_1para",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_1perp","amp_1perp",0.6,0.0,1.0));
		m_vars.push_back(new RooRealVar("phase_1perp","phase_1perp",0.0,-2*TMath::Pi(),2*TMath::Pi()));

		m_vars.push_back(new RooRealVar("amp_20","amp_20",0.95*0.9,0.0,1.0));
		m_vars.push_back(new RooRealVar("phase_20","phase_20",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_2para","amp_2para",0.22*0.9,0.0,1.0));
		m_vars.push_back(new RooRealVar("phase_2para","phase_2para",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_2perp","amp_2perp",0.22*0.9,0.0,1.0));
		m_vars.push_back(new RooRealVar("phase_2perp","phase_2perp",0.0,-2*TMath::Pi(),2*TMath::Pi()));

		// the diff variables (helicity amplitudes and phases) (Start Index 24)
		m_vars.push_back(new RooRealVar("amp_00_delta","amp_00_delta",0.0,-0.5,0.5));
		m_vars.push_back(new RooRealVar("phase_00_delta","phase_00_delta",0.0));

		m_vars.push_back(new RooRealVar("amp_10_delta","amp_10_delta",0.0,-0.5,0.5));
		m_vars.push_back(new RooRealVar("phase_10_delta","phase_10_delta",0.0,-TMath::Pi()/2,TMath::Pi()/2));
		m_vars.push_back(new RooRealVar("amp_1para_delta","amp_1para_delta",0.0,-0.5,0.5));
		m_vars.push_back(new RooRealVar("phase_1para_delta","phase_1para_delta",0.0,-TMath::Pi()/2,TMath::Pi()/2));
		m_vars.push_back(new RooRealVar("amp_1perp_delta","amp_1perp_delta",0.0,-0.5,0.5));
		m_vars.push_back(new RooRealVar("phase_1perp_delta","phase_1perp_delta",TMath::Pi()/2,0.0,TMath::Pi()));

		m_vars.push_back(new RooRealVar("amp_20_delta","amp_20_delta",0.0,-0.5,0.5));
		m_vars.push_back(new RooRealVar("phase_20_delta","phase_20_delta",0.0,-TMath::Pi()/2,TMath::Pi()/2));
		m_vars.push_back(new RooRealVar("amp_2para_delta","amp_2para_delta",0.0,-0.9,0.9));
		m_vars.push_back(new RooRealVar("phase_2para_delta","phase_2para_delta",0.0,-TMath::Pi()/2,TMath::Pi()/2));
		m_vars.push_back(new RooRealVar("amp_2perp_delta","amp_2perp_delta",0.0,-0.9,0.9));
		m_vars.push_back(new RooRealVar("phase_2perp_delta","phase_2perp_delta",TMath::Pi()/2,0.0,TMath::Pi()));

		// function index 0 - 3
		m_functions.push_back(new RooFormulaVar("CP_amp_00","CP_amp_00","@0 * (1 + @1)",RooArgList(*m_vars.at(10),*m_vars.at(24))));
		m_functions.push_back(new RooFormulaVar("CA_amp_00","CA_amp_00","@0 * (1 - @1)",RooArgList(*m_vars.at(10),*m_vars.at(24))));
		m_functions.push_back(new RooFormulaVar("CP_phase_00","CP_phase_00","@0 + @1",RooArgList(*m_vars.at(11),*m_vars.at(25))));
		m_functions.push_back(new RooFormulaVar("CA_phase_00","CA_phase_00","@0 - @1",RooArgList(*m_vars.at(11),*m_vars.at(25))));

		// function index 4 - 15
		m_functions.push_back(new RooFormulaVar("CP_amp_10","CP_amp_10","@0 * (1 + @1)",RooArgList(*m_vars.at(12),*m_vars.at(26))));
		m_functions.push_back(new RooFormulaVar("CA_amp_10","CA_amp_10","@0 * (1 - @1)",RooArgList(*m_vars.at(12),*m_vars.at(26))));
		m_functions.push_back(new RooFormulaVar("CP_phase_10","CP_phase_10","@0 + @1",RooArgList(*m_vars.at(13),*m_vars.at(27))));
		m_functions.push_back(new RooFormulaVar("CA_phase_10","CA_phase_10","@0 - @1",RooArgList(*m_vars.at(13),*m_vars.at(27))));
		m_functions.push_back(new RooFormulaVar("CP_amp_1para","CP_amp_1para","@0 * (1 + @1)",RooArgList(*m_vars.at(14),*m_vars.at(28))));
		m_functions.push_back(new RooFormulaVar("CA_amp_1para","CA_amp_1para","@0 * (1 - @1)",RooArgList(*m_vars.at(14),*m_vars.at(28))));
		m_functions.push_back(new RooFormulaVar("CP_phase_1para","CP_phase_1para","@0 + @1",RooArgList(*m_vars.at(15),*m_vars.at(29))));
		m_functions.push_back(new RooFormulaVar("CA_phase_1para","CA_phase_1para","@0 - @1",RooArgList(*m_vars.at(15),*m_vars.at(29))));
		m_functions.push_back(new RooFormulaVar("CP_amp_1perp","CP_amp_1perp","@0 * (1 + @1)",RooArgList(*m_vars.at(16),*m_vars.at(30))));
		m_functions.push_back(new RooFormulaVar("CA_amp_1perp","CA_amp_1perp","@0 * (1 - @1)",RooArgList(*m_vars.at(16),*m_vars.at(30))));
		m_functions.push_back(new RooFormulaVar("CP_phase_1perp","CP_phase_1perp","@0 + @1",RooArgList(*m_vars.at(17),*m_vars.at(31))));
		m_functions.push_back(new RooFormulaVar("CA_phase_1perp","CA_phase_1perp","@0 - @1",RooArgList(*m_vars.at(17),*m_vars.at(31))));

		// function index 16 - 27
		m_functions.push_back(new RooFormulaVar("CP_amp_20","CP_amp_20","@0 * (1 + @1)",RooArgList(*m_vars.at(18),*m_vars.at(32))));
		m_functions.push_back(new RooFormulaVar("CA_amp_20","CA_amp_20","@0 * (1 - @1)",RooArgList(*m_vars.at(18),*m_vars.at(32))));
		m_functions.push_back(new RooFormulaVar("CP_phase_20","CP_phase_20","@0 + @1",RooArgList(*m_vars.at(19),*m_vars.at(33))));
		m_functions.push_back(new RooFormulaVar("CA_phase_20","CA_phase_20","@0 - @1",RooArgList(*m_vars.at(19),*m_vars.at(33))));
		m_functions.push_back(new RooFormulaVar("CP_amp_2para","CP_amp_2para","@0 * (1 + @1)",RooArgList(*m_vars.at(20),*m_vars.at(34))));
		m_functions.push_back(new RooFormulaVar("CA_amp_2para","CA_amp_2para","@0 * (1 - @1)",RooArgList(*m_vars.at(20),*m_vars.at(34))));
		m_functions.push_back(new RooFormulaVar("CP_phase_2para","CP_phase_2para","@0 + @1",RooArgList(*m_vars.at(21),*m_vars.at(35))));
		m_functions.push_back(new RooFormulaVar("CA_phase_2para","CA_phase_2para","@0 - @1",RooArgList(*m_vars.at(21),*m_vars.at(35))));
		m_functions.push_back(new RooFormulaVar("CP_amp_2perp","CP_amp_2perp","@0 * (1 + @1)",RooArgList(*m_vars.at(22),*m_vars.at(36))));
		m_functions.push_back(new RooFormulaVar("CA_amp_2perp","CA_amp_2perp","@0 * (1 - @1)",RooArgList(*m_vars.at(22),*m_vars.at(36))));
		m_functions.push_back(new RooFormulaVar("CP_phase_2perp","CP_phase_2perp","@0 + @1",RooArgList(*m_vars.at(23),*m_vars.at(37))));
		m_functions.push_back(new RooFormulaVar("CA_phase_2perp","CA_phase_2perp","@0 - @1",RooArgList(*m_vars.at(23),*m_vars.at(37))));

		MHI_CC_parameters cc_paramters_CA = {
				m_functions.at(1),
				m_functions.at(3),
				m_functions.at(5),
				m_functions.at(7),
				m_functions.at(9),
				m_functions.at(11),
				m_functions.at(13),
				m_functions.at(15),
				m_functions.at(17),
				m_functions.at(19),
				m_functions.at(21),
				m_functions.at(23),
				m_functions.at(25),
				m_functions.at(27)
		};

		m_pdfs.push_back(new RooMassHelicityAnglesCPV(("C_"+m_label).c_str(),("C_"+m_label).c_str(),mass_kpi,hel_phi,hel_th1,hel_th2,flavor,
				/*mass_0,width_0,a,b,mass_1,width_1,r_1,mass_2,width_2,r_2,*/
				*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5),*m_vars.at(6),*m_vars.at(7),*m_vars.at(8),*m_vars.at(9),
				/*amp_00,phase_00,*/
				*m_functions.at(0),*m_functions.at(2),
				/*amp_10,phase_10,amp_1para,phase_1para,amp_1perp,phase_1perp*/
				*m_functions.at(4),*m_functions.at(6),*m_functions.at(8),*m_functions.at(10),*m_functions.at(12),*m_functions.at(14),
				/*amp_20,phase_20,amp_2para,phase_2para,amp_2perp,phase_2perp*/
				*m_functions.at(16),*m_functions.at(18),*m_functions.at(20),*m_functions.at(22),*m_functions.at(24),*m_functions.at(26),
				_acceptance_function,
				cc_paramters_CA,
				_s_PDG,
				_s_4D,
				_s_3D_phi_th1_th2,
				_s_3D_m_th1_th2,
				_s_3D_m_phi_th2,
				_s_3D_m_phi_th1));
		m_main_pdfs["C"] = m_pdfs.back();

		m_pdfs.push_back(new RooMassHelicityAnglesCPV(("N_"+m_label).c_str(),("N_"+m_label).c_str(),mass_kpi,hel_phi,hel_th1,hel_th2,flavor,
				/*mass_0,width_0,a,b,mass_1,width_1,r_1,mass_2,width_2,r_2,*/
				*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5),*m_vars.at(6),*m_vars.at(7),*m_vars.at(8),*m_vars.at(9),
				/*amp_00,phase_00,*/
				*m_functions.at(0),*m_functions.at(2),
				/*amp_10,phase_10,amp_1para,phase_1para,amp_1perp,phase_1perp*/
				*m_functions.at(4),*m_functions.at(6),*m_functions.at(8),*m_functions.at(10),*m_functions.at(12),*m_functions.at(14),
				/*amp_20,phase_20,amp_2para,phase_2para,amp_2perp,phase_2perp*/
				*m_functions.at(16),*m_functions.at(18),*m_functions.at(20),*m_functions.at(22),*m_functions.at(24),*m_functions.at(26),
				_acceptance_function,
				cc_paramters_CA,
				_s_PDG,
				_s_4D,
				_s_3D_phi_th1_th2,
				_s_3D_m_th1_th2,
				_s_3D_m_phi_th2,
				_s_3D_m_phi_th1));
		m_main_pdfs["N"] = m_pdfs.back();

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}

	double getAsymmetry() {
		RooMassHelicityAnglesCPV* pdf = dynamic_cast<RooMassHelicityAnglesCPV*>(m_main_pdfs["C"]);
		return pdf->getAsymmetry();
	}
};

class FF_PDF_signal_5D_mass_helicity_angles_CPV_PhysBase : public FF_PDF_signal {
public:
	virtual ~FF_PDF_signal_5D_mass_helicity_angles_CPV_PhysBase() { }

	FF_PDF_signal_5D_mass_helicity_angles_CPV_PhysBase(RooRealVar& mass_kpi, RooRealVar& hel_th1,
			RooRealVar& hel_th2, RooRealVar& hel_phi, RooCategory& flavor,
			ACCEPTANCE_FUNCTION _acceptance_function,
			MHI_PDG_constants _s_PDG,
			MHI_CacheIntegrators_4D _s_4D,
			MHI_CacheIntegrators_3D_phi_th1_th2 _s_3D_phi_th1_th2,
			MHI_CacheIntegrators_3D_m_th1_th2 _s_3D_m_th1_th2,
			MHI_CacheIntegrators_3D_m_phi_th2 _s_3D_m_phi_th2,
			MHI_CacheIntegrators_3D_m_phi_th1 _s_3D_m_phi_th1,
			const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_signal("FF_PDF_signal_5D_mass_helicity_angles_CPV_PhysBase",cat) {

		// the BW amplitude values (they are all fixed)
		// TODO: Need to be varied by uncertainties (see BaBar paper)
		m_vars.push_back(new RooRealVar("mass_0","mass_0",1.435));
		m_vars.push_back(new RooRealVar("width_0","width_0",0.279));
		m_vars.push_back(new RooRealVar("a","a",1.95));
		m_vars.push_back(new RooRealVar("b","b",1.76));
		m_vars.push_back(new RooRealVar("mass_1","mass_1",0.896));
		m_vars.push_back(new RooRealVar("width_1","width_1",0.0503));
		m_vars.push_back(new RooRealVar("r_1","r_1",3.4));
		m_vars.push_back(new RooRealVar("mass_2","mass_2",1.432));
		m_vars.push_back(new RooRealVar("width_2","width_2",0.109));
		m_vars.push_back(new RooRealVar("r_2","r_2",2.7));

		// the mean variables (helicity amplitudes and phases) (Start Index 10)
		m_vars.push_back(new RooRealVar("amp_00","amp_00",0.75,0.0,1.0));
		m_vars.push_back(new RooRealVar("phase_00","phase_00",0.0));

		m_vars.push_back(new RooRealVar("amp_10","amp_10",0.707,0.0,1.0));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("phi_01","phi_01",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_1para","amp_1para",0.4,0.0,1.0));
		m_vars.push_back(new RooRealVar("phi_1para","phi_1para",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_1perp","amp_1perp",0.6,0.0,1.0));
		m_vars.push_back(new RooRealVar("phi_1perp","phi_1perp",0.0,-2*TMath::Pi(),2*TMath::Pi()));

		m_vars.push_back(new RooRealVar("amp_20","amp_20",0.95*0.9,0.0,1.0));
		m_vars.push_back(new RooRealVar("phi_02","phi_02",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_2para","amp_2para",0.22*0.9,0.0,1.0));
		m_vars.push_back(new RooRealVar("phi_2para","phi_2para",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_2perp","amp_2perp",0.22*0.9,0.0,1.0));
		m_vars.push_back(new RooRealVar("phi_2perp","phi_2perp",0.0,-2*TMath::Pi(),2*TMath::Pi()));

		// the diff variables (helicity amplitudes and phases) (Start Index 24)
		m_vars.push_back(new RooRealVar("amp_00_delta","amp_00_delta",0.0,-0.5,0.5));
		m_vars.push_back(new RooRealVar("phase_00_delta","phase_00_delta",0.0));

		m_vars.push_back(new RooRealVar("amp_10_delta","amp_10_delta",0.0,-0.5,0.5));
		m_vars.push_back(new RooRealVar("phi_01_delta","phi_01_delta",0.0,-TMath::Pi()/2,TMath::Pi()/2));
		m_vars.push_back(new RooRealVar("amp_1para_delta","amp_1para_delta",0.0,-0.5,0.5));
		m_vars.push_back(new RooRealVar("phi_1para_delta","phi_1para_delta",0.0,-TMath::Pi()/2,TMath::Pi()/2));
		m_vars.push_back(new RooRealVar("amp_1perp_delta","amp_1perp_delta",0.0,-0.5,0.5));
		m_vars.push_back(new RooRealVar("phi_1perp_delta","phi_1perp_delta",0.0,-TMath::Pi()/2,TMath::Pi()/2));

		m_vars.push_back(new RooRealVar("amp_20_delta","amp_20_delta",0.0,-0.5,0.5));
		m_vars.push_back(new RooRealVar("phi_02_delta","phi_02_delta",0.0,-TMath::Pi()/2,TMath::Pi()/2));
		m_vars.push_back(new RooRealVar("amp_2para_delta","amp_2para_delta",0.0,-0.9,0.9));
		m_vars.push_back(new RooRealVar("phi_2para_delta","phi_2para_delta",0.0,-TMath::Pi()/2,TMath::Pi()/2));
		m_vars.push_back(new RooRealVar("amp_2perp_delta","amp_2perp_delta",0.0,-0.9,0.9));
		m_vars.push_back(new RooRealVar("phi_2perp_delta","phi_2perp_delta",0.0,-TMath::Pi()/2,TMath::Pi()/2));

		// compute the transversity phase values from the physics phase parameters
		// function index 0 - 5 (phases)
		m_functions.push_back(new RooFormulaVar("phase_10","phase_10","-1 * @0",RooArgList(*m_vars.at(13))));
		m_functions.push_back(new RooFormulaVar("phase_1para","phase_1para","@0 - @1",RooArgList(*m_vars.at(15),*m_vars.at(13))));
		m_functions.push_back(new RooFormulaVar("phase_1perp","phase_1perp","@0 - @1 + @2",RooArgList(*m_vars.at(17),*m_vars.at(13),RooConst(TMath::Pi()/2))));
		m_functions.push_back(new RooFormulaVar("phase_20","phase_20","-1 * @0",RooArgList(*m_vars.at(19))));
		m_functions.push_back(new RooFormulaVar("phase_2para","phase_2para","@0 - @1",RooArgList(*m_vars.at(21),*m_vars.at(19))));
		m_functions.push_back(new RooFormulaVar("phase_2perp","phase_2perp","@0 - @1 + @2",RooArgList(*m_vars.at(23),*m_vars.at(19),RooConst(TMath::Pi()/2))));

		// function index 6 - 11 (phases delta)
		m_functions.push_back(new RooFormulaVar("phase_10_delta","phase_10_delta","@0",RooArgList(*m_vars.at(27))));
		m_functions.push_back(new RooFormulaVar("phase_1para_delta","phase_1para_delta","-1 * @0 + @1",RooArgList(*m_vars.at(29),*m_vars.at(27))));
		m_functions.push_back(new RooFormulaVar("phase_1perp_delta","phase_1perp_delta","-1 * @0 + @1 + @2",RooArgList(*m_vars.at(31),*m_vars.at(27),RooConst(TMath::Pi()/2))));
		m_functions.push_back(new RooFormulaVar("phase_20_delta","phase_20_delta","@0",RooArgList(*m_vars.at(33))));
		m_functions.push_back(new RooFormulaVar("phase_2para_delta","phase_2para_delta","-1 * @0 + @1",RooArgList(*m_vars.at(35),*m_vars.at(33))));
		m_functions.push_back(new RooFormulaVar("phase_2perp_delta","phase_2perp_delta","-1 * @0 + @1 + @2",RooArgList(*m_vars.at(37),*m_vars.at(33),RooConst(TMath::Pi()/2))));

		// compute the CP/CA values from the transversity base values
		// function index 12 - 15
		m_functions.push_back(new RooFormulaVar("CP_amp_00","CP_amp_00","@0 * (1 + @1)",RooArgList(*m_vars.at(10),*m_vars.at(24))));
		m_functions.push_back(new RooFormulaVar("CA_amp_00","CA_amp_00","@0 * (1 - @1)",RooArgList(*m_vars.at(10),*m_vars.at(24))));
		m_functions.push_back(new RooFormulaVar("CP_phase_00","CP_phase_00","@0 + @1",RooArgList(*m_vars.at(11),*m_vars.at(25))));
		m_functions.push_back(new RooFormulaVar("CA_phase_00","CA_phase_00","@0 - @1",RooArgList(*m_vars.at(11),*m_vars.at(25))));

		// function index 16 - 27
		m_functions.push_back(new RooFormulaVar("CP_amp_10","CP_amp_10","@0 * (1 + @1)",RooArgList(*m_vars.at(12),*m_vars.at(26))));
		m_functions.push_back(new RooFormulaVar("CA_amp_10","CA_amp_10","@0 * (1 - @1)",RooArgList(*m_vars.at(12),*m_vars.at(26))));
		m_functions.push_back(new RooFormulaVar("CP_phase_10","CP_phase_10","@0 + @1",RooArgList(*m_functions.at(0),*m_functions.at(6))));
		m_functions.push_back(new RooFormulaVar("CA_phase_10","CA_phase_10","@0 - @1",RooArgList(*m_functions.at(0),*m_functions.at(6))));
		m_functions.push_back(new RooFormulaVar("CP_amp_1para","CP_amp_1para","@0 * (1 + @1)",RooArgList(*m_vars.at(14),*m_vars.at(28))));
		m_functions.push_back(new RooFormulaVar("CA_amp_1para","CA_amp_1para","@0 * (1 - @1)",RooArgList(*m_vars.at(14),*m_vars.at(28))));
		m_functions.push_back(new RooFormulaVar("CP_phase_1para","CP_phase_1para","@0 + @1",RooArgList(*m_functions.at(1),*m_functions.at(7))));
		m_functions.push_back(new RooFormulaVar("CA_phase_1para","CA_phase_1para","@0 - @1",RooArgList(*m_functions.at(1),*m_functions.at(7))));
		m_functions.push_back(new RooFormulaVar("CP_amp_1perp","CP_amp_1perp","@0 * (1 + @1)",RooArgList(*m_vars.at(16),*m_vars.at(30))));
		m_functions.push_back(new RooFormulaVar("CA_amp_1perp","CA_amp_1perp","@0 * (1 - @1)",RooArgList(*m_vars.at(16),*m_vars.at(30))));
		m_functions.push_back(new RooFormulaVar("CP_phase_1perp","CP_phase_1perp","@0 + @1",RooArgList(*m_functions.at(2),*m_functions.at(8))));
		m_functions.push_back(new RooFormulaVar("CA_phase_1perp","CA_phase_1perp","@0 - @1",RooArgList(*m_functions.at(2),*m_functions.at(8))));

		// function index 28 - 39
		m_functions.push_back(new RooFormulaVar("CP_amp_20","CP_amp_20","@0 * (1 + @1)",RooArgList(*m_vars.at(18),*m_vars.at(32))));
		m_functions.push_back(new RooFormulaVar("CA_amp_20","CA_amp_20","@0 * (1 - @1)",RooArgList(*m_vars.at(18),*m_vars.at(32))));
		m_functions.push_back(new RooFormulaVar("CP_phase_20","CP_phase_20","@0 + @1",RooArgList(*m_functions.at(3),*m_functions.at(9))));
		m_functions.push_back(new RooFormulaVar("CA_phase_20","CA_phase_20","@0 - @1",RooArgList(*m_functions.at(3),*m_functions.at(9))));
		m_functions.push_back(new RooFormulaVar("CP_amp_2para","CP_amp_2para","@0 * (1 + @1)",RooArgList(*m_vars.at(20),*m_vars.at(34))));
		m_functions.push_back(new RooFormulaVar("CA_amp_2para","CA_amp_2para","@0 * (1 - @1)",RooArgList(*m_vars.at(20),*m_vars.at(34))));
		m_functions.push_back(new RooFormulaVar("CP_phase_2para","CP_phase_2para","@0 + @1",RooArgList(*m_functions.at(4),*m_functions.at(10))));
		m_functions.push_back(new RooFormulaVar("CA_phase_2para","CA_phase_2para","@0 - @1",RooArgList(*m_functions.at(4),*m_functions.at(10))));
		m_functions.push_back(new RooFormulaVar("CP_amp_2perp","CP_amp_2perp","@0 * (1 + @1)",RooArgList(*m_vars.at(22),*m_vars.at(36))));
		m_functions.push_back(new RooFormulaVar("CA_amp_2perp","CA_amp_2perp","@0 * (1 - @1)",RooArgList(*m_vars.at(22),*m_vars.at(36))));
		m_functions.push_back(new RooFormulaVar("CP_phase_2perp","CP_phase_2perp","@0 + @1",RooArgList(*m_functions.at(5),*m_functions.at(11))));
		m_functions.push_back(new RooFormulaVar("CA_phase_2perp","CA_phase_2perp","@0 - @1",RooArgList(*m_functions.at(5),*m_functions.at(11))));

		MHI_CC_parameters cc_paramters_CA = {
				m_functions.at(13),
				m_functions.at(15),
				m_functions.at(17),
				m_functions.at(19),
				m_functions.at(21),
				m_functions.at(23),
				m_functions.at(25),
				m_functions.at(27),
				m_functions.at(29),
				m_functions.at(31),
				m_functions.at(33),
				m_functions.at(35),
				m_functions.at(37),
				m_functions.at(39)
		};

		m_pdfs.push_back(new RooMassHelicityAnglesCPV(("C_"+m_label).c_str(),("C_"+m_label).c_str(),mass_kpi,hel_phi,hel_th1,hel_th2,flavor,
				/*mass_0,width_0,a,b,mass_1,width_1,r_1,mass_2,width_2,r_2,*/
				*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5),*m_vars.at(6),*m_vars.at(7),*m_vars.at(8),*m_vars.at(9),
				/*amp_00,phase_00,*/
				*m_functions.at(12),*m_functions.at(14),
				/*amp_10,phase_10,amp_1para,phase_1para,amp_1perp,phase_1perp*/
				*m_functions.at(16),*m_functions.at(18),*m_functions.at(20),*m_functions.at(22),*m_functions.at(24),*m_functions.at(26),
				/*amp_20,phase_20,amp_2para,phase_2para,amp_2perp,phase_2perp*/
				*m_functions.at(28),*m_functions.at(30),*m_functions.at(32),*m_functions.at(34),*m_functions.at(36),*m_functions.at(38),
				_acceptance_function,
				cc_paramters_CA,
				_s_PDG,
				_s_4D,
				_s_3D_phi_th1_th2,
				_s_3D_m_th1_th2,
				_s_3D_m_phi_th2,
				_s_3D_m_phi_th1));
		m_main_pdfs["C"] = m_pdfs.back();

		m_pdfs.push_back(new RooMassHelicityAnglesCPV(("N_"+m_label).c_str(),("N_"+m_label).c_str(),mass_kpi,hel_phi,hel_th1,hel_th2,flavor,
				/*mass_0,width_0,a,b,mass_1,width_1,r_1,mass_2,width_2,r_2,*/
				*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5),*m_vars.at(6),*m_vars.at(7),*m_vars.at(8),*m_vars.at(9),
				/*amp_00,phase_00,*/
				*m_functions.at(12),*m_functions.at(14),
				/*amp_10,phase_10,amp_1para,phase_1para,amp_1perp,phase_1perp*/
				*m_functions.at(16),*m_functions.at(18),*m_functions.at(20),*m_functions.at(22),*m_functions.at(24),*m_functions.at(26),
				/*amp_20,phase_20,amp_2para,phase_2para,amp_2perp,phase_2perp*/
				*m_functions.at(28),*m_functions.at(30),*m_functions.at(32),*m_functions.at(34),*m_functions.at(36),*m_functions.at(38),
				_acceptance_function,
				cc_paramters_CA,
				_s_PDG,
				_s_4D,
				_s_3D_phi_th1_th2,
				_s_3D_m_th1_th2,
				_s_3D_m_phi_th2,
				_s_3D_m_phi_th1));
		m_main_pdfs["N"] = m_pdfs.back();

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}

	double getAsymmetry() {
		RooMassHelicityAnglesCPV* pdf = dynamic_cast<RooMassHelicityAnglesCPV*>(m_main_pdfs["C"]);
		return pdf->getAsymmetry();
	}
};


class FF_PDF_signal_2D_dalitz_analysis : public FF_PDF_signal {
public:
	virtual ~FF_PDF_signal_2D_dalitz_analysis() { }
    
	FF_PDF_signal_2D_dalitz_analysis(RooRealVar& mass_AB, RooRealVar& mass_BC,
                                     EFFICIENCY_FUNCTION _efficiency_function,
                                     DAI_PDG_constants _s_PDG,
                                     DAI_Dalitz_Model_parameters _s_DM_pars,
                                     DAI_CacheIntegrators_2D _s_2D,
                                     const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_signal("FF_PDF_signal_2D_dalitz_analysis",cat) {
        
		//The particles masses and widths in the model
		// TODO: Need to be varied by uncertainties (see BaBar paper)
        
        //m_vars 0-13 masses and widths
		m_vars.push_back(new RooRealVar("mass_1","mass_1",8.9166000e-01)); //K*+
		m_vars.push_back(new RooRealVar("width_1","width_1",5.0800012e-02)); //K*+
        m_vars.push_back(new RooRealVar("mass_2","mass_2",1.412)); //K*(1430)+ (S-wave)
		m_vars.push_back(new RooRealVar("width_2","width_2",0.294)); //K*(1430)+ (S-wave)
        m_vars.push_back(new RooRealVar("mass_3","mass_3",7.7549000e-01)); //rho(770)-
		m_vars.push_back(new RooRealVar("width_3","width_3",1.4940001e-01)); //rho(770)-
        m_vars.push_back(new RooRealVar("mass_4","mass_4",1.4650000)); //rho(2S)-
		m_vars.push_back(new RooRealVar("width_4","width_4",4.0000012e-01)); //rho(2S)-
        m_vars.push_back(new RooRealVar("mass_5","mass_5",1.7200000)); //rho(3S)-
		m_vars.push_back(new RooRealVar("width_5","width_5",2.4999998e-01)); //rho(3S)-
        m_vars.push_back(new RooRealVar("mass_6","mass_6",8.9600000e-01)); //K*0
		m_vars.push_back(new RooRealVar("width_6","width_6",5.0299999e-02)); //K*0
        m_vars.push_back(new RooRealVar("mass_7","mass_7",1.412)); //K*(1430)0 (S-wave)
		m_vars.push_back(new RooRealVar("width_7","width_7",0.294)); //K*(1430)0 (S-wave)
        
		// m_vars 14 - 29 amplitudes and phases
		m_vars.push_back(new RooRealVar("amp_1","amp_1",0.4,0.0,50.0));
		m_vars.push_back(new RooRealVar("phase_1","phase_1",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_2","amp_2",0.4,0.0,50.0));
		m_vars.push_back(new RooRealVar("phase_2","phase_2",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_3","amp_3",0.4,0.0,50.0));
		m_vars.push_back(new RooRealVar("phase_3","phase_3",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_4","amp_4",0.4,0.0,50.0));
		m_vars.push_back(new RooRealVar("phase_4","phase_4",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_5","amp_5",0.4,0.0,50.0));
		m_vars.push_back(new RooRealVar("phase_5","phase_5",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_6","amp_6",0.4,0.0,50.0));
		m_vars.push_back(new RooRealVar("phase_6","phase_6",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_7","amp_7",0.4,0.0,50.0));
		m_vars.push_back(new RooRealVar("phase_7","phase_7",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_8","amp_8",0.4,0.0,50.0));
		m_vars.push_back(new RooRealVar("phase_8","phase_8",0.0,-2*TMath::Pi(),2*TMath::Pi()));
        
        
		m_pdfs.push_back(new RooDalitzAnalysis(("C_"+m_label).c_str(),("C_"+m_label).c_str(),mass_AB,mass_BC,*m_vars.at(0), *m_vars.at(1),
                                               *m_vars.at(2), *m_vars.at(3), *m_vars.at(4), *m_vars.at(5), *m_vars.at(6), *m_vars.at(7),
                                               *m_vars.at(8), *m_vars.at(9), *m_vars.at(10), *m_vars.at(11), *m_vars.at(12), *m_vars.at(13),
                                               *m_vars.at(14), *m_vars.at(15), *m_vars.at(16), *m_vars.at(17), *m_vars.at(18), *m_vars.at(19),
                                               *m_vars.at(20), *m_vars.at(21), *m_vars.at(22), *m_vars.at(23), *m_vars.at(24), *m_vars.at(25),
                                               *m_vars.at(26), *m_vars.at(27), *m_vars.at(28), *m_vars.at(29),
                                               _efficiency_function,
                                               _s_PDG,
                                               _s_DM_pars,
                                               _s_2D));
		m_main_pdfs["C"] = m_pdfs.back();
        
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};






class FF_PDF_signal_3D_dalitz_analysis_CPV : public FF_PDF_signal {
public:
	virtual ~FF_PDF_signal_3D_dalitz_analysis_CPV() { }
    
	FF_PDF_signal_3D_dalitz_analysis_CPV(RooRealVar& mass_AB, RooRealVar& mass_BC, RooCategory& flavor,
                                         EFFICIENCY_FUNCTION _efficiency_function,
                                         DAI_PDG_constants _s_PDG,
                                         DAI_Dalitz_Model_parameters _s_DM_pars,
                                         DAI_CacheIntegrators_2D _s_2D,
                                         const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_signal("FF_PDF_signal_3D_dalitz_analysis_CPV",cat) {
        
		// the BW amplitude values (they are all fixed)
		// TODO: Need to be varied by uncertainties (see BaBar paper)
        
        //m_vars 0-13 masses and widths
		m_vars.push_back(new RooRealVar("mass_1","mass_1",8.9166000e-01)); //K*+
		m_vars.push_back(new RooRealVar("width_1","width_1",5.0800012e-02)); //K*+
        m_vars.push_back(new RooRealVar("mass_2","mass_2",1.412)); //K*(1430)+ (S-wave)
		m_vars.push_back(new RooRealVar("width_2","width_2",0.294)); //K*(1430)+ (S-wave)
        m_vars.push_back(new RooRealVar("mass_3","mass_3",7.7549000e-01)); //rho(770)-
		m_vars.push_back(new RooRealVar("width_3","width_3",1.4940001e-01)); //rho(770)-
        m_vars.push_back(new RooRealVar("mass_4","mass_4",1.4650000)); //rho(2S)-
		m_vars.push_back(new RooRealVar("width_4","width_4",4.0000012e-01)); //rho(2S)-
        m_vars.push_back(new RooRealVar("mass_5","mass_5",1.7200000)); //rho(3S)-
		m_vars.push_back(new RooRealVar("width_5","width_5",2.4999998e-01)); //rho(3S)-
        m_vars.push_back(new RooRealVar("mass_6","mass_6",8.9600000e-01)); //K*0
		m_vars.push_back(new RooRealVar("width_6","width_6",5.0299999e-02)); //K*0
        m_vars.push_back(new RooRealVar("mass_7","mass_7",1.412)); //K*(1430)0 (S-wave)
		m_vars.push_back(new RooRealVar("width_7","width_7",0.294)); //K*(1430)0 (S-wave)
        
        // m_vars 14 - 29 amplitudes and phases
		m_vars.push_back(new RooRealVar("amp_1","amp_1",0.4,0.0,50.0));
		m_vars.push_back(new RooRealVar("phase_1","phase_1",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_2","amp_2",0.4,0.0,50.0));
		m_vars.push_back(new RooRealVar("phase_2","phase_2",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_3","amp_3",0.4,0.0,50.0));
		m_vars.push_back(new RooRealVar("phase_3","phase_3",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_4","amp_4",0.4,0.0,50.0));
		m_vars.push_back(new RooRealVar("phase_4","phase_4",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_5","amp_5",0.4,0.0,50.0));
		m_vars.push_back(new RooRealVar("phase_5","phase_5",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_6","amp_6",0.4,0.0,50.0));
		m_vars.push_back(new RooRealVar("phase_6","phase_6",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_7","amp_7",0.4,0.0,50.0));
		m_vars.push_back(new RooRealVar("phase_7","phase_7",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_8","amp_8",0.4,0.0,50.0));
		m_vars.push_back(new RooRealVar("phase_8","phase_8",0.0,-2*TMath::Pi(),2*TMath::Pi()));
        
        // m_vars 30 - 44 amplitudes and phases
        m_vars.push_back(new RooRealVar("amp_1_CC","amp_1_CC",0.4,0.0,50.0));
		m_vars.push_back(new RooRealVar("phase_1_CC","phase_1_CC",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_2_CC","amp_2_CC",0.4,0.0,50.0));
		m_vars.push_back(new RooRealVar("phase_2_CC","phase_2_CC",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_3_CC","amp_3_CC",0.4,0.0,50.0));
		m_vars.push_back(new RooRealVar("phase_3_CC","phase_3_CC",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_4_CC","amp_4_CC",0.4,0.0,50.0));
		m_vars.push_back(new RooRealVar("phase_4_CC","phase_4_CC",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_5_CC","amp_5_CC",0.4,0.0,50.0));
		m_vars.push_back(new RooRealVar("phase_5_CC","phase_5_CC",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_6_CC","amp_6_CC",0.4,0.0,50.0));
		m_vars.push_back(new RooRealVar("phase_6_CC","phase_6_CC",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_7_CC","amp_7_CC",0.4,0.0,50.0));
		m_vars.push_back(new RooRealVar("phase_7_CC","phase_7_CC",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.push_back(new RooRealVar("amp_8_CC","amp_8_CC",0.4,0.0,50.0));
		m_vars.push_back(new RooRealVar("phase_8_CC","phase_8_CC",0.0,-2*TMath::Pi(),2*TMath::Pi()));
        
        
		DAI_CC_parameters cc_paramters_CA = {
            m_vars.at(30),
            m_vars.at(31),
            m_vars.at(32),
            m_vars.at(33),
            m_vars.at(34),
            m_vars.at(35),
            m_vars.at(36),
            m_vars.at(37),
            m_vars.at(38),
            m_vars.at(39),
            m_vars.at(40),
            m_vars.at(41),
            m_vars.at(42),
            m_vars.at(43),
            m_vars.at(44),
            m_vars.at(45)
		};
        
		m_pdfs.push_back(new RooDalitzAnalysisCPV(("C_"+m_label).c_str(),("C_"+m_label).c_str(),mass_AB,mass_BC,flavor, *m_vars.at(0), *m_vars.at(1),
                                                      *m_vars.at(2), *m_vars.at(3), *m_vars.at(4), *m_vars.at(5), *m_vars.at(6), *m_vars.at(7),
                                                      *m_vars.at(8), *m_vars.at(9), *m_vars.at(10), *m_vars.at(11), *m_vars.at(12), *m_vars.at(13),
                                                      *m_vars.at(14), *m_vars.at(15), *m_vars.at(16), *m_vars.at(17), *m_vars.at(18), *m_vars.at(19),
                                                      *m_vars.at(20), *m_vars.at(21), *m_vars.at(22), *m_vars.at(23), *m_vars.at(24), *m_vars.at(25),
                                                      *m_vars.at(26), *m_vars.at(27), *m_vars.at(28), *m_vars.at(29),
                                                      cc_paramters_CA,
                                                      _efficiency_function,
                                                      _s_PDG,
                                                      _s_DM_pars,
                                                      _s_2D));
		m_main_pdfs["C"] = m_pdfs.back();
        
		m_pdfs.push_back(new RooDalitzAnalysisCPV(("N_"+m_label).c_str(),("N_"+m_label).c_str(),mass_AB,mass_BC,flavor, *m_vars.at(0), *m_vars.at(1),
                                                      *m_vars.at(2), *m_vars.at(3), *m_vars.at(4), *m_vars.at(5), *m_vars.at(6), *m_vars.at(7),
                                                      *m_vars.at(8), *m_vars.at(9), *m_vars.at(10), *m_vars.at(11), *m_vars.at(12), *m_vars.at(13),
                                                      *m_vars.at(14), *m_vars.at(15), *m_vars.at(16), *m_vars.at(17), *m_vars.at(18), *m_vars.at(19),
                                                      *m_vars.at(20), *m_vars.at(21), *m_vars.at(22), *m_vars.at(23), *m_vars.at(24), *m_vars.at(25),
                                                      *m_vars.at(26), *m_vars.at(27), *m_vars.at(28), *m_vars.at(29),
                                                      cc_paramters_CA,
                                                      _efficiency_function,
                                                      _s_PDG,
                                                      _s_DM_pars,
                                                      _s_2D));
		m_main_pdfs["N"] = m_pdfs.back();
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
    
	double getAsymmetry() {
		RooDalitzAnalysisCPV* pdf = dynamic_cast<RooDalitzAnalysisCPV*>(m_main_pdfs["C"]);
		return pdf->getAsymmetry();
	}
};

class FF_PDF_signal_6D: public FF_PDF_signal {
public:
	virtual ~FF_PDF_signal_6D() { }
    
	FF_PDF_signal_6D(shared_ptr<FF_PDF> normal_3D, shared_ptr<FF_PDF> dalitz_3D,
                     const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_signal("FF_PDF_signal_6D", cat) {
		m_comb_pdfs.push_back(normal_3D);
		m_comb_pdfs.push_back(dalitz_3D);
        
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
                                        RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"))));
		m_main_pdfs["C"] = m_pdfs.back();
        //		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
        //				RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"))));
        //		m_main_pdfs["N"] = m_pdfs.back();
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
    double getAsymmetry() {
		RooDalitzAnalysisCPV& pdf = dynamic_cast<RooDalitzAnalysisCPV&>(m_comb_pdfs.at(1)->GetPDF("C"));
		return pdf.getAsymmetry();
	}
};






class FF_PDF_signal_8D: public FF_PDF_signal {
public:
	virtual ~FF_PDF_signal_8D() { }

	FF_PDF_signal_8D(shared_ptr<FF_PDF> normal_4D, shared_ptr<FF_PDF> angles_4D,
			const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_signal("FF_PDF_signal_8D", cat) {
		m_comb_pdfs.push_back(normal_4D);
		m_comb_pdfs.push_back(angles_4D);

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"))));
		m_main_pdfs["C"] = m_pdfs.back();
//		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
//				RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"))));
//		m_main_pdfs["N"] = m_pdfs.back();

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_signal_9D: public FF_PDF_signal {
public:
	virtual ~FF_PDF_signal_9D() { }

	FF_PDF_signal_9D(shared_ptr<FF_PDF> normal_4D, shared_ptr<FF_PDF> angles_5D,
		const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_signal("FF_PDF_signal_9D", cat) {
		m_comb_pdfs.push_back(normal_4D);
		m_comb_pdfs.push_back(angles_5D);

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"))));
		m_main_pdfs["C"] = m_pdfs.back();
//		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
//				RooArgList(m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"))));
//		m_main_pdfs["N"] = m_pdfs.back();

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}

	double getAsymmetry() {
		RooMassHelicityAnglesCPV& pdf = dynamic_cast<RooMassHelicityAnglesCPV&>(m_comb_pdfs.at(1)->GetPDF("C"));
		return pdf.getAsymmetry();
	}
};


class FF_PDF_signal_4D_mass_helicity_angles_EvtGen : public FF_PDF_signal {
public:
	virtual ~FF_PDF_signal_4D_mass_helicity_angles_EvtGen() { }

	FF_PDF_signal_4D_mass_helicity_angles_EvtGen(RooRealVar& mass_kpi, RooRealVar& hel_th1,
			RooRealVar& hel_th2, RooRealVar& hel_phi,
			const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_signal("FF_PDF_signal_4D_mass_helicity_angles_EvtGen",cat) {

		// values are taken from BELLE evt.pdl file, not from PDG !!!
		// vars 0 - 5
		m_vars.push_back(new RooRealVar("mass_0","mass_0",1.412));
		m_vars.push_back(new RooRealVar("width_0","width_0",0.294));  // Delta lower cutoff is 0.6
		m_vars.push_back(new RooRealVar("mass_1","mass_1",0.896));
		m_vars.push_back(new RooRealVar("width_1","width_1",0.0503));  // Delta lower cutoff is 0.258
		m_vars.push_back(new RooRealVar("mass_2","mass_2",1.432));
		m_vars.push_back(new RooRealVar("width_2","width_2",0.109));  // Delta lower cutoff is 0.4

		// pdf 0 - 4
		m_pdfs.push_back(new RooBreitWignerTwoBodyPHSPEvtGen(("C_"+m_label+"_K0_1430_mass").c_str(),("C_"+m_label+"_K0_1430_mass").c_str(),
				mass_kpi,*m_vars.at(0),*m_vars.at(1),0.6,m_K,m_Pi));
		m_pdfs.push_back(new RooThreeBodyPhaseSpaceProjection(("C_"+m_label+"_PHSP_mass").c_str(),("C_"+m_label+"_PHSP_mass").c_str(),
				mass_kpi,m_K,m_Pi,m_Phi,m_B0));
		m_pdfs.push_back(new RooBreitWignerTwoBodyPHSPEvtGen(("C_"+m_label+"_K1_892_mass").c_str(),("C_"+m_label+"_K1_892_mass").c_str(),
				mass_kpi,*m_vars.at(2),*m_vars.at(3),0.258,m_K,m_Pi));
		m_pdfs.push_back(new RooBreitWignerTwoBodyPHSPEvtGen(("C_"+m_label+"_K2_1430_mass").c_str(),("C_"+m_label+"_K2_1430_mass").c_str(),
				mass_kpi,*m_vars.at(4),*m_vars.at(5),0.4,m_K,m_Pi));

		// vars 6 - 7
		m_vars.push_back(new RooRealVar("amp_00","amp_00",1.0,0.0,2.0));
		m_vars.push_back(new RooRealVar("phase_00","phase_00",0.0));
		m_vars.back()->setConstant(true);

		// pdfs 5 - 8
		m_pdfs.push_back(new RooHelicityAngles(("C_"+m_label+"_K0_1430_angles").c_str(),("C_"+m_label+"_K0_1430_angles").c_str(),
				hel_phi,hel_th1,hel_th2,RooConst(0.0),RooConst(1.0),
				*m_vars.at(6),*m_vars.at(7),*m_vars.at(6),*m_vars.at(7),*m_vars.at(6),*m_vars.at(7)));

		m_pdfs.push_back(new RooUniform(("C_"+m_label+"_PHSP_angles_phi").c_str(),("C_"+m_label+"_PHSP_angles_phi").c_str(),hel_phi));
		m_pdfs.push_back(new RooUniform(("C_"+m_label+"_PHSP_angles_th1").c_str(),("C_"+m_label+"_PHSP_angles_th1").c_str(),hel_th1));
		m_pdfs.push_back(new RooUniform(("C_"+m_label+"_PHSP_angles_th2").c_str(),("C_"+m_label+"_PHSP_angles_th2").c_str(),hel_th2));
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label+"_PHSP_angles").c_str(),("C_"+m_label+"_PHSP_angles").c_str(),
				RooArgList(*m_pdfs.at(5),*m_pdfs.at(6),*m_pdfs.at(7))));

		// vars 8 - 13
		m_vars.push_back(new RooRealVar("amp_1m","amp_1m",1.0,0.0,2.0));
		m_vars.push_back(new RooRealVar("phase_1m","phase_1m",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("amp_10","amp_10",1.0,0.0,2.0));
		m_vars.push_back(new RooRealVar("phase_10","phase_10",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("amp_1p","amp_1p",1.0,0.0,2.0));
		m_vars.push_back(new RooRealVar("phase_1p","phase_1p",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.back()->setConstant(true);

		// pdf 9
		m_pdfs.push_back(new RooHelicityAngles(("C_"+m_label+"_K1_892_angles").c_str(),("C_"+m_label+"_K1_892_angles").c_str(),
				hel_phi,hel_th1,hel_th2,RooConst(1.0),RooConst(1.0),
				*m_vars.at(8),*m_vars.at(9),*m_vars.at(10),*m_vars.at(11),*m_vars.at(12),*m_vars.at(13)));

		// vars 14 - 19
		m_vars.push_back(new RooRealVar("amp_2m","amp_2m",1.0,0.0,2.0));
		m_vars.push_back(new RooRealVar("phase_2m","phase_2m",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("amp_20","amp_20",1.0,0.0,2.0));
		m_vars.push_back(new RooRealVar("phase_20","phase_20",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.back()->setConstant(true);
		m_vars.push_back(new RooRealVar("amp_2p","amp_2p",1.0,0.0,2.0));
		m_vars.push_back(new RooRealVar("phase_2p","phase_2p",0.0,-2*TMath::Pi(),2*TMath::Pi()));
		m_vars.back()->setConstant(true);

		// pdf 10
		m_pdfs.push_back(new RooHelicityAngles(("C_"+m_label+"_K2_1430_angles").c_str(),("C_"+m_label+"_K2_1430_angles").c_str(),
				hel_phi,hel_th1,hel_th2,RooConst(2.0),RooConst(1.0),
				*m_vars.at(14),*m_vars.at(15),*m_vars.at(16),*m_vars.at(17),*m_vars.at(18),*m_vars.at(19)));

		// pdfs 11 - 14
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label+"_K0_1430").c_str(),("C_"+m_label+"_K0_1430").c_str(),
				RooArgList(*m_pdfs.at(/*11*/0),*m_pdfs.at(4))));
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label+"_K1_892").c_str(),("C_"+m_label+"_K1_892").c_str(),
				RooArgList(*m_pdfs.at(/*12*/2),*m_pdfs.at(9))));
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label+"_K2_1430").c_str(),("C_"+m_label+"_K2_1430").c_str(),
				RooArgList(*m_pdfs.at(/*13*/3),*m_pdfs.at(10))));
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label+"_PHSP").c_str(),("C_"+m_label+"_PHSP").c_str(),
				RooArgList(*m_pdfs.at(1),*m_pdfs.at(8))));

		// vars 20 - 23
		m_vars.push_back(new RooRealVar("yield_K0_1430","yield_K0_1430",18470,0,110000));
		m_vars.push_back(new RooRealVar("yield_K1_892","yield_K1_892",31330,0,110000));
		m_vars.push_back(new RooRealVar("yield_K2_1430","yield_K2_1430",12050,0,110000));
		m_vars.push_back(new RooRealVar("yield_PHSP","yield_PHSP",38150,0,110000));

		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(*m_pdfs.at(11),*m_pdfs.at(12),*m_pdfs.at(13),*m_pdfs.at(14)),
				RooArgList(*m_vars.at(20),*m_vars.at(21),*m_vars.at(22),*m_vars.at(23))
				));
		m_main_pdfs["C"] = m_pdfs.back();
		m_main_pdfs["N"] = m_pdfs.back();

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_signal_4D_mass_helicity_angles_EvtGen_PHSP : public FF_PDF_signal {
public:
	virtual ~FF_PDF_signal_4D_mass_helicity_angles_EvtGen_PHSP() { }

	FF_PDF_signal_4D_mass_helicity_angles_EvtGen_PHSP(RooRealVar& mass_kpi, RooRealVar& hel_th1,
			RooRealVar& hel_th2, RooRealVar& hel_phi,
			const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_signal("FF_PDF_signal_4D_mass_helicity_angles_EvtGen_PHSP",cat) {

		m_pdfs.push_back(new RooThreeBodyPhaseSpaceProjection(("C_"+m_label+"_PHSP_mass").c_str(),("C_"+m_label+"_PHSP_mass").c_str(),
				mass_kpi,m_K,m_Pi,m_Phi,m_B0));
		m_pdfs.push_back(new RooThreeBodyPhaseSpaceProjection(("N_"+m_label+"_PHSP_mass").c_str(),("N_"+m_label+"_PHSP_mass").c_str(),
				mass_kpi,m_Ks,m_Pi0,m_Phi,m_B0));

		m_pdfs.push_back(new RooUniform((""+m_label+"_PHSP_angles_phi").c_str(),(""+m_label+"_PHSP_angles_phi").c_str(),hel_phi));
		m_pdfs.push_back(new RooUniform((""+m_label+"_PHSP_angles_th1").c_str(),(""+m_label+"_PHSP_angles_th1").c_str(),hel_th1));
		m_pdfs.push_back(new RooUniform((""+m_label+"_PHSP_angles_th2").c_str(),(""+m_label+"_PHSP_angles_th2").c_str(),hel_th2));
		m_pdfs.push_back(new RooProdPdf((""+m_label+"_PHSP_angles").c_str(),(""+m_label+"_PHSP_angles").c_str(),
				RooArgList(*m_pdfs.at(2),*m_pdfs.at(3),*m_pdfs.at(4))));

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),
				RooArgList(*m_pdfs.at(0),*m_pdfs.at(5))));
		m_main_pdfs["C"] = m_pdfs.back();
		m_pdfs.push_back(new RooProdPdf(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),
				RooArgList(*m_pdfs.at(1),*m_pdfs.at(5))));
		m_main_pdfs["N"] = m_pdfs.back();

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_signal_5D_mass_helicity_angles_EvtGen_PHSP : public FF_PDF_signal {
public:
	virtual ~FF_PDF_signal_5D_mass_helicity_angles_EvtGen_PHSP() { }

	FF_PDF_signal_5D_mass_helicity_angles_EvtGen_PHSP(RooRealVar& mass_kpi, RooRealVar& hel_th1,
			RooRealVar& hel_th2, RooRealVar& hel_phi, RooCategory& flavor,
			const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_signal("FF_PDF_signal_5D_mass_helicity_angles_EvtGen_PHSP",cat) {

		m_pdfs.push_back(new RooThreeBodyPhaseSpaceProjection(("C_"+m_label+"_PHSP_mass").c_str(),("C_"+m_label+"_PHSP_mass").c_str(),
				mass_kpi,m_K,m_Pi,m_Phi,m_B0));
		m_pdfs.push_back(new RooThreeBodyPhaseSpaceProjection(("N_"+m_label+"_PHSP_mass").c_str(),("N_"+m_label+"_PHSP_mass").c_str(),
				mass_kpi,m_Ks,m_Pi0,m_Phi,m_B0));

		m_pdfs.push_back(new RooUniform((""+m_label+"_PHSP_angles_phi").c_str(),(""+m_label+"_PHSP_angles_phi").c_str(),hel_phi));
		m_pdfs.push_back(new RooUniform((""+m_label+"_PHSP_angles_th1").c_str(),(""+m_label+"_PHSP_angles_th1").c_str(),hel_th1));
		m_pdfs.push_back(new RooUniform((""+m_label+"_PHSP_angles_th2").c_str(),(""+m_label+"_PHSP_angles_th2").c_str(),hel_th2));
		m_pdfs.push_back(new RooProdPdf((""+m_label+"_PHSP_angles").c_str(),(""+m_label+"_PHSP_angles").c_str(),
				RooArgList(*m_pdfs.at(2),*m_pdfs.at(3),*m_pdfs.at(4))));

		m_pdfs.push_back(new RooCategoryPdf(("C_"+m_label+"_flavor").c_str(),("C_"+m_label+"_flavor").c_str(),flavor));

		m_pdfs.push_back(new RooProdPdf(("C_"+m_label+"").c_str(),("C_"+m_label+"").c_str(),
				RooArgList(*m_pdfs.at(0),*m_pdfs.at(5),*m_pdfs.at(6))));
		m_main_pdfs["C"] = m_pdfs.back();
		m_pdfs.push_back(new RooProdPdf(("N_"+m_label+"").c_str(),("N_"+m_label+"").c_str(),
				RooArgList(*m_pdfs.at(1),*m_pdfs.at(5))));
		m_main_pdfs["N"] = m_pdfs.back();

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};



class FF_PDF_efficiency_2D_dalitz : public FF_PDF_signal {
public:
	virtual ~FF_PDF_efficiency_2D_dalitz() { }
    
	FF_PDF_efficiency_2D_dalitz(RooRealVar& mass_AB, RooRealVar& mass_BC, const FFUtil::tuple_category_name_flags& cat)
	: FF_PDF_signal("FF_PDF_efficiency_2D_dalitz",cat) {
        
        TH1::AddDirectory(kFALSE);
        
        TFile* graph_file= new TFile("plots/efficiency_2D_mass_12_vs_mass_01.root","READ");
        TGraph2D *graph = dynamic_cast<TGraph2D*>(graph_file->Get("Graph2D"));
        graph_file->Close();
        
        m_pdfs.push_back(new RooDalitzDelaunay("eff_interpolated","eff_interpolated",mass_AB,mass_BC,graph,m_B0,m_Pi0,m_Pi,m_K));
        
		m_main_pdfs["C"] = m_pdfs.back();
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};






#endif /* FF_PDF_SIGNAL_H_ */
