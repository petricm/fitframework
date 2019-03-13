/**
 * @file	FF_PDF_data.h
 * @date	Nov 18, 2011
 * @author	petric
 * @brief	brief description
 *
 * long description
 */

#ifndef FF_PDF_DATA_H_
#define FF_PDF_DATA_H_

#include "FF_PDF.h"
#include "FF_PDF_bkg_bbar.h"
#include "FF_PDF_bkg_continuum.h"
#include "FF_PDF_signal.h"

class FF_PDF_data : public FF_PDF {
public:
	virtual ~FF_PDF_data() { }
protected:
	FF_PDF_data(const std::string &label, const FFUtil::tuple_category_name_flags& cat) : FF_PDF(label) {
		m_category_name_flags = cat;
		m_pdf_color = kOrange+10;
		m_pdf_linestyle = kSolid;
		m_pdf_legend_label = "Fit Function";
	}
};

class FF_PDF_data_4D_mbc_deltae_chil0_ma_nb_prime : public FF_PDF_data {
public:
	virtual ~FF_PDF_data_4D_mbc_deltae_chil0_ma_nb_prime() { }

	FF_PDF_data_4D_mbc_deltae_chil0_ma_nb_prime(shared_ptr<FF_PDF> signal, shared_ptr<FF_PDF> continuum,
			shared_ptr<FF_PDF> combinatoric, shared_ptr<FF_PDF> wrongphikkbar, double nevents,
			const FFUtil::tuple_category_name_flags& cat) : FF_PDF_data("pdf_data_4D_mbc_deltae_chil0_ma_nb_prime",cat) {

		if(dynamic_cast<FF_PDF_signal_4D_mbc_deltae_chil0_ma_nb_prime*>(signal.get())) {
			m_comb_pdfs.push_back(signal);
		} else {
			std::cerr << "ERROR: PDF for signal component in " << m_label << " was not of type " << "FF_PDF_signal_4D_mbc_deltae_chil0_ma_nb_prime" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_continuum_4D_mbc_deltae_chil0_ma_nb_prime*>(continuum.get())) {
			m_comb_pdfs.push_back(continuum);
		} else {
			std::cerr << "ERROR: PDF for continuum component in " << m_label << " was not of type " << "FF_PDF_bkg_continuum_4D_mbc_deltae_chil0_ma_nb_prime" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_combinatoric_4D_mbc_deltae_chil0_ma_nb_prime*>(combinatoric.get())) {
			m_comb_pdfs.push_back(combinatoric);
		} else {
			std::cerr << "ERROR: PDF for combinatoric component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_combinatoric_4D_mbc_deltae_chil0_ma_nb_prime" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_wrongphikkbar_4D_mbc_deltae_chil0_ma_nb_prime*>(wrongphikkbar.get())) {
			m_comb_pdfs.push_back(wrongphikkbar);
		} else {
			std::cerr << "ERROR: PDF for wrongphikkbar component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_wrongphikkbar_4D_mbc_deltae_chil0_ma_nb_prime" << std::endl;
			exit(EXIT_FAILURE);
		}

		m_vars.push_back(new RooRealVar("signal_nevents","signal_nevents",1000.0,-150.0,5000.0));
		// TODO: Constrain this value and read from eff. fit result
		// TODO: Fit the 2/3 fractionof Iso-Spin
		// TODO: 18.0115 is number from eff. fit, as MC BR are wrong and we don't correct for m_kpi rel_eff difference in production, we use 13.88 instead
		m_vars.push_back(new RooRealVar("RelEffC","RelEffC",13.88));
		m_vars.push_back(new RooRealVar("RelEffN","RelEffN",1.0));

		m_functions.push_back(new RooFormulaVar("C_signal_nevents","C_signal_nevents","@0*@1/(@1+@2)",RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2))));
		m_vars.push_back(new RooRealVar("C_bkg_continuum_nevents","C_bkg_continuum_nevents",nevents/2.0,1000.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_combinatoric_nevents","C_bkg_bbar_combinatoric_nevents",nevents/8.0,-150.0,nevents*0.5));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongphikkbar_nevents","C_bkg_bbar_wrongphikkbar_nevents",15,5,25));
		m_pdfs.push_back(new RooGaussian("C_bkg_bbar_wrongphikkbar_constraint","C_bkg_bbar_wrongphikkbar_constraint",*m_vars.back(),RooConst(15.02),RooConst(1.42)));

		m_functions.push_back(new RooFormulaVar("N_signal_nevents","N_signal_nevents","@0*@1/(@1+@2)",RooArgList(*m_vars.at(0),*m_vars.at(2),*m_vars.at(1))));
		m_vars.push_back(new RooRealVar("N_bkg_continuum_nevents","N_bkg_continuum_nevents",nevents/2.0,100.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_bkg_bbar_combinatoric_nevents","N_bkg_bbar_combinatoric_nevents",nevents/8.0,-150.0,nevents*0.5));
		m_vars.push_back(new RooRealVar("N_bkg_bbar_wrongphikkbar_nevents","N_bkg_bbar_wrongphikkbar_nevents",0.83,0.1,1.5));
		m_pdfs.push_back(new RooGaussian("N_bkg_bbar_wrongphikkbar_constraint","N_bkg_bbar_wrongphikkbar_constraint",*m_vars.back(),RooConst(0.83),RooConst(0.08)));

		m_vars.push_back(new RooRealVar("signal_nevents_asym","signal_nevents_asym",0.0,-200.0,200.0));
		m_functions.push_back(new RooFormulaVar("CP_signal_nevents","CP_signal_nevents","(@0*@1/(@1+@2))/2 + @3",RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.back())));
		m_functions.push_back(new RooFormulaVar("CA_signal_nevents","CA_signal_nevents","(@0*@1/(@1+@2))/2 - @3",RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.back())));
		m_functions.push_back(new RooFormulaVar("CP_bkg_continuum_nevents","CP_bkg_continuum_nevents","@0/2",RooArgList(*m_vars.at(3))));
		m_functions.push_back(new RooFormulaVar("CA_bkg_continuum_nevents","CA_bkg_continuum_nevents","@0/2",RooArgList(*m_vars.at(3))));
		m_functions.push_back(new RooFormulaVar("CP_bkg_bbar_combinatoric_nevents","CP_bkg_bbar_combinatoric_nevents","@0/2",RooArgList(*m_vars.at(4))));
		m_functions.push_back(new RooFormulaVar("CA_bkg_bbar_combinatoric_nevents","CA_bkg_bbar_combinatoric_nevents","@0/2",RooArgList(*m_vars.at(4))));
		m_functions.push_back(new RooFormulaVar("CP_bkg_bbar_wrongphikkbar_nevents","CP_bkg_bbar_wrongphikkbar_nevents","@0/2",RooArgList(*m_vars.at(5))));
		m_functions.push_back(new RooFormulaVar("CA_bkg_bbar_wrongphikkbar_nevents","CA_bkg_bbar_wrongphikkbar_nevents","@0/2",RooArgList(*m_vars.at(5))));

		m_pdfs.push_back(new RooAddPdf(("C_"+m_label+"_noconstraint").c_str(),("C_"+m_label+"_noconstraint").c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
				m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C")),
				RooArgList(*m_functions.at(0),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5)),false));
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(*m_pdfs.back(),*m_pdfs.at(0))));
		m_main_pdfs["C"] = m_pdfs.at(3);

		m_pdfs.push_back(new RooAddPdf(("CP_"+m_label+"_noconstraint").c_str(),("CP_"+m_label+"_noconstraint").c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("CP"),m_comb_pdfs.at(1)->GetPDF("CP"),
				m_comb_pdfs.at(2)->GetPDF("CP"),m_comb_pdfs.at(3)->GetPDF("CP")),
				RooArgList(*m_functions.at(2),*m_functions.at(4),*m_functions.at(6),*m_functions.at(8)),false));
		m_pdfs.push_back(new RooProdPdf(("CP_"+m_label).c_str(),("CP_"+m_label).c_str(),
				RooArgList(*m_pdfs.back(),*m_pdfs.at(0))));
		m_main_pdfs["CP"] = m_pdfs.at(5);

		m_pdfs.push_back(new RooAddPdf(("CA_"+m_label+"_noconstraint").c_str(),("CA_"+m_label+"_noconstraint").c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("CA"),m_comb_pdfs.at(1)->GetPDF("CA"),
				m_comb_pdfs.at(2)->GetPDF("CA"),m_comb_pdfs.at(3)->GetPDF("CA")),
				RooArgList(*m_functions.at(3),*m_functions.at(5),*m_functions.at(7),*m_functions.at(9)),false));
		m_pdfs.push_back(new RooProdPdf(("CA_"+m_label).c_str(),("CA_"+m_label).c_str(),
				RooArgList(*m_pdfs.back(),*m_pdfs.at(0))));
		m_main_pdfs["CA"] = m_pdfs.at(7);

		m_pdfs.push_back(new RooAddPdf(("N_"+m_label+"_noconstraint").c_str(),("N_"+m_label+"_noconstraint").c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
				m_comb_pdfs.at(2)->GetPDF("N"),m_comb_pdfs.at(3)->GetPDF("N")),
				RooArgList(*m_functions.at(1),*m_vars.at(6),*m_vars.at(7),*m_vars.at(8)),false));
		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
				RooArgList(*m_pdfs.back(),*m_pdfs.at(1))));
		m_main_pdfs["N"] = m_pdfs.at(9);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
}  __attribute__ ((deprecated));

class FF_PDF_data_1D_mbc : public FF_PDF_data {
public:
	virtual ~FF_PDF_data_1D_mbc() { }

	FF_PDF_data_1D_mbc(shared_ptr<FF_PDF> signal, shared_ptr<FF_PDF> continuum,
			const FFUtil::tuple_category_name_flags& cat) : FF_PDF_data("FF_PDF_data_1D_mbc",cat) {

		if(dynamic_cast<FF_PDF_signal_1D_mbc*>(signal.get())) {
			m_comb_pdfs.push_back(signal);
		} else {
			std::cerr << "ERROR: PDF for signal component in " << m_label << " was not of type " << "FF_PDF_signal_1D_mbc" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_continuum_1D_mbc*>(continuum.get())) {
			m_comb_pdfs.push_back(continuum);
		} else {
			std::cerr << "ERROR: PDF for continuum component in " << m_label << " was not of type " << "FF_PDF_bkg_continuum_1D_mbc" << std::endl;
			exit(EXIT_FAILURE);
		}

		m_vars.push_back(new RooRealVar("CC_sig_frac","CC_sig_frac",0.95,0.7,1.0));

		m_pdfs.push_back(new RooAddPdf(("CC_"+m_label+"").c_str(),("CC_"+m_label+"").c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("CC"),m_comb_pdfs.at(1)->GetPDF("CC")),
				RooArgList(*m_vars.at(0))));
		m_main_pdfs["CC"] = m_pdfs.back();

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_data_2D_mbc_deltae : public FF_PDF_data {
public:
	virtual ~FF_PDF_data_2D_mbc_deltae() { }

	FF_PDF_data_2D_mbc_deltae(shared_ptr<FF_PDF> signal, shared_ptr<FF_PDF> continuum,
			const FFUtil::tuple_category_name_flags& cat) : FF_PDF_data("pdf_data_2D_mbc_deltae",cat) {

		if(dynamic_cast<FF_PDF_signal_2D_mbc_deltae*>(signal.get())) {
			m_comb_pdfs.push_back(signal);
		} else {
			std::cerr << "ERROR: PDF for signal component in " << m_label << " was not of type " << "FF_PDF_signal_2D_mbc_deltae" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_continuum_2D_mbc_deltae*>(continuum.get())) {
			m_comb_pdfs.push_back(continuum);
		} else {
			std::cerr << "ERROR: PDF for continuum component in " << m_label << " was not of type " << "FF_PDF_bkg_continuum_2D_mbc_deltae" << std::endl;
			exit(EXIT_FAILURE);
		}

		m_vars.push_back(new RooRealVar("CC_sig_frac","CC_sig_frac",0.95,0.0,1.0));

		m_pdfs.push_back(new RooAddPdf(("CC_"+m_label+"").c_str(),("CC_"+m_label+"").c_str(),
				RooArgList(m_comb_pdfs.at(0)->GetPDF("CC"),m_comb_pdfs.at(1)->GetPDF("CC")),
				RooArgList(*m_vars.at(0))));
		m_main_pdfs["CC"] = m_pdfs.back();

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_data_4D_mbc_deltae_chil0_ma_nb_prime__w_peak : public FF_PDF_data {
public:
	virtual ~FF_PDF_data_4D_mbc_deltae_chil0_ma_nb_prime__w_peak() { }

	FF_PDF_data_4D_mbc_deltae_chil0_ma_nb_prime__w_peak(shared_ptr<FF_PDF> signal, shared_ptr<FF_PDF> continuum,
			shared_ptr<FF_PDF> combinatoric, shared_ptr<FF_PDF> wrongphikkbar, shared_ptr<FF_PDF> wrongphif0980,
			shared_ptr<FF_PDF> wrongmasshyp, double nevents,
			const FFUtil::tuple_category_name_flags& cat) : FF_PDF_data("pdf_data_4D_mbc_deltae_chil0_ma_nb_prime__w_peak",cat) {

		if(dynamic_cast<FF_PDF_signal_4D_mbc_deltae_chil0_ma_nb_prime*>(signal.get())) {
			m_comb_pdfs.push_back(signal);
		} else {
			std::cerr << "ERROR: PDF for signal component in " << m_label << " was not of type " << "FF_PDF_signal_4D_mbc_deltae_chil0_ma_nb_prime" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_continuum_4D_mbc_deltae_chil0_ma_nb_prime*>(continuum.get())) {
			m_comb_pdfs.push_back(continuum);
		} else {
			std::cerr << "ERROR: PDF for continuum component in " << m_label << " was not of type " << "FF_PDF_bkg_continuum_4D_mbc_deltae_chil0_ma_nb_prime" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_combinatoric_4D_mbc_deltae_chil0_ma_nb_prime*>(combinatoric.get())) {
			m_comb_pdfs.push_back(combinatoric);
		} else {
			std::cerr << "ERROR: PDF for combinatoric component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_combinatoric_4D_mbc_deltae_chil0_ma_nb_prime" << std::endl;
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
		if(dynamic_cast<FF_PDF_bkg_bbar_wrongmasshyp_4D_mbc_deltae_chil0_ma_nb_prime*>(wrongmasshyp.get())) {
			m_comb_pdfs.push_back(wrongmasshyp);
		} else {
			std::cerr << "ERROR: PDF for wrongmasshyp component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_wrongmasshyp_4D_mbc_deltae_chil0_ma_nb_prime" << std::endl;
			exit(EXIT_FAILURE);
		}

		m_vars.push_back(new RooRealVar("signal_nevents","signal_nevents",1000.0,-150.0,5000.0));
		// TODO: Constrain this value and read from eff. fit result
		// TODO: Fit the 2/3 fractionof Iso-Spin
		// TODO: 18.0115 is number from eff. fit, as MC BR are wrong and we don't correct for m_kpi rel_eff difference in production, we use 13.88 instead
		m_vars.push_back(new RooRealVar("RelEffC","RelEffC",13.88));
		m_vars.push_back(new RooRealVar("RelEffN","RelEffN",1.0));

		m_functions.push_back(new RooFormulaVar("C_signal_nevents","C_signal_nevents","@0*@1/(@1+@2)",RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2))));
		m_vars.push_back(new RooRealVar("C_bkg_continuum_nevents","C_bkg_continuum_nevents",nevents/2.0,1000.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_combinatoric_nevents","C_bkg_bbar_combinatoric_nevents",nevents/8.0,-150.0,nevents*0.5));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongphikkbar_nevents","C_bkg_bbar_wrongphikkbar_nevents",15,5,25));
		m_pdfs.push_back(new RooGaussian("C_bkg_bbar_wrongphikkbar_constraint","C_bkg_bbar_wrongphikkbar_constraint",*m_vars.back(),RooConst(15.02),RooConst(1.42)));

		m_functions.push_back(new RooFormulaVar("N_signal_nevents","N_signal_nevents","@0*@1/(@1+@2)",RooArgList(*m_vars.at(0),*m_vars.at(2),*m_vars.at(1))));
		m_vars.push_back(new RooRealVar("N_bkg_continuum_nevents","N_bkg_continuum_nevents",nevents/2.0,100.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("N_bkg_bbar_combinatoric_nevents","N_bkg_bbar_combinatoric_nevents",nevents/8.0,-150.0,nevents*0.5));
		m_vars.push_back(new RooRealVar("N_bkg_bbar_wrongphikkbar_nevents","N_bkg_bbar_wrongphikkbar_nevents",0.83,0.1,1.5));
		m_pdfs.push_back(new RooGaussian("N_bkg_bbar_wrongphikkbar_constraint","N_bkg_bbar_wrongphikkbar_constraint",*m_vars.back(),RooConst(0.83),RooConst(0.08)));

		m_vars.push_back(new RooRealVar("signal_nevents_asym","signal_nevents_asym",0.0,-200.0,200.0));
		m_functions.push_back(new RooFormulaVar("CP_signal_nevents","CP_signal_nevents","(@0*@1/(@1+@2))/2 + @3",RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.back())));
		m_functions.push_back(new RooFormulaVar("CA_signal_nevents","CA_signal_nevents","(@0*@1/(@1+@2))/2 - @3",RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.back())));
		m_functions.push_back(new RooFormulaVar("CP_bkg_continuum_nevents","CP_bkg_continuum_nevents","@0/2",RooArgList(*m_vars.at(3))));
		m_functions.push_back(new RooFormulaVar("CA_bkg_continuum_nevents","CA_bkg_continuum_nevents","@0/2",RooArgList(*m_vars.at(3))));
		m_functions.push_back(new RooFormulaVar("CP_bkg_bbar_combinatoric_nevents","CP_bkg_bbar_combinatoric_nevents","@0/2",RooArgList(*m_vars.at(4))));
		m_functions.push_back(new RooFormulaVar("CA_bkg_bbar_combinatoric_nevents","CA_bkg_bbar_combinatoric_nevents","@0/2",RooArgList(*m_vars.at(4))));
		m_functions.push_back(new RooFormulaVar("CP_bkg_bbar_wrongphikkbar_nevents","CP_bkg_bbar_wrongphikkbar_nevents","@0/2",RooArgList(*m_vars.at(5))));
		m_functions.push_back(new RooFormulaVar("CA_bkg_bbar_wrongphikkbar_nevents","CA_bkg_bbar_wrongphikkbar_nevents","@0/2",RooArgList(*m_vars.at(5))));

		// non-constraint peaking components
		m_vars.push_back(new RooRealVar("bkg_bbar_wrongphif0980_nevents","bkg_bbar_wrongphif0980_nevents",0.0,-200.0,200));
		m_functions.push_back(new RooFormulaVar("C_bkg_bbar_wrongphif0980_nevents","C_bkg_bbar_wrongphif0980_nevents","@0*@1/(@1+@2)",RooArgList(*m_vars.back(),*m_vars.at(1),*m_vars.at(2))));
		m_functions.push_back(new RooFormulaVar("N_bkg_bbar_wrongphif0980_nevents","N_bkg_bbar_wrongphif0980_nevents","@0*@1/(@1+@2)",RooArgList(*m_vars.back(),*m_vars.at(2),*m_vars.at(1))));
		m_functions.push_back(new RooFormulaVar("CP_bkg_bbar_wrongphif0980_nevents","CP_bkg_bbar_wrongphif0980_nevents","(@0*@1/(@1+@2))/2",RooArgList(*m_vars.back(),*m_vars.at(1),*m_vars.at(2))));
		m_functions.push_back(new RooFormulaVar("CA_bkg_bbar_wrongphif0980_nevents","CA_bkg_bbar_wrongphif0980_nevents","(@0*@1/(@1+@2))/2",RooArgList(*m_vars.back(),*m_vars.at(1),*m_vars.at(2))));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongmasshyp_nevents","C_bkg_bbar_wrongmasshyp_nevents",0.0,-200.0,200.0));
		m_functions.push_back(new RooFormulaVar("CP_bkg_bbar_wrongmasshyp_nevents","CP_bkg_bbar_wrongmasshyp_nevents","@0/2",RooArgList(*m_vars.back())));
		m_functions.push_back(new RooFormulaVar("CA_bkg_bbar_wrongmasshyp_nevents","CA_bkg_bbar_wrongmasshyp_nevents","@0/2",RooArgList(*m_vars.back())));

		m_pdfs.push_back(new RooAddPdf(("C_"+m_label+"_noconstraint").c_str(),("C_"+m_label+"_noconstraint").c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
				m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),
				m_comb_pdfs.at(4)->GetPDF("C"),m_comb_pdfs.at(5)->GetPDF("C")),
				RooArgList(*m_functions.at(0),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5),*m_functions.at(10),*m_vars.at(11)),false));
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(*m_pdfs.back(),*m_pdfs.at(0))));
		m_main_pdfs["C"] = m_pdfs.at(3);

		m_pdfs.push_back(new RooAddPdf(("CP_"+m_label+"_noconstraint").c_str(),("CP_"+m_label+"_noconstraint").c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("CP"),m_comb_pdfs.at(1)->GetPDF("CP"),
				m_comb_pdfs.at(2)->GetPDF("CP"),m_comb_pdfs.at(3)->GetPDF("CP"),
				m_comb_pdfs.at(4)->GetPDF("CP"),m_comb_pdfs.at(5)->GetPDF("CP")),
				RooArgList(*m_functions.at(2),*m_functions.at(4),*m_functions.at(6),*m_functions.at(8),*m_functions.at(12),*m_functions.at(14)),false));
		m_pdfs.push_back(new RooProdPdf(("CP_"+m_label).c_str(),("CP_"+m_label).c_str(),
				RooArgList(*m_pdfs.back(),*m_pdfs.at(0))));
		m_main_pdfs["CP"] = m_pdfs.at(5);

		m_pdfs.push_back(new RooAddPdf(("CA_"+m_label+"_noconstraint").c_str(),("CA_"+m_label+"_noconstraint").c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("CA"),m_comb_pdfs.at(1)->GetPDF("CA"),
				m_comb_pdfs.at(2)->GetPDF("CA"),m_comb_pdfs.at(3)->GetPDF("CA"),
				m_comb_pdfs.at(4)->GetPDF("CA"),m_comb_pdfs.at(5)->GetPDF("CA")),
				RooArgList(*m_functions.at(3),*m_functions.at(5),*m_functions.at(7),*m_functions.at(9),*m_functions.at(13),*m_functions.at(15)),false));
		m_pdfs.push_back(new RooProdPdf(("CA_"+m_label).c_str(),("CA_"+m_label).c_str(),
				RooArgList(*m_pdfs.back(),*m_pdfs.at(0))));
		m_main_pdfs["CA"] = m_pdfs.at(7);

		m_pdfs.push_back(new RooAddPdf(("N_"+m_label+"_noconstraint").c_str(),("N_"+m_label+"_noconstraint").c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N"),
				m_comb_pdfs.at(2)->GetPDF("N"),m_comb_pdfs.at(3)->GetPDF("N"),
				m_comb_pdfs.at(4)->GetPDF("N")),
				RooArgList(*m_functions.at(1),*m_vars.at(6),*m_vars.at(7),*m_vars.at(8),*m_functions.at(11)),false));
		m_pdfs.push_back(new RooProdPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),
				RooArgList(*m_pdfs.back(),*m_pdfs.at(1))));
		m_main_pdfs["N"] = m_pdfs.at(9);

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
}  __attribute__ ((deprecated));

class FF_PDF_data_sideband_3D_deltae_chil0_ma_nb_prime : public FF_PDF_data {
public:
	virtual ~FF_PDF_data_sideband_3D_deltae_chil0_ma_nb_prime() { }

	FF_PDF_data_sideband_3D_deltae_chil0_ma_nb_prime(shared_ptr<FF_PDF> continuum,
			shared_ptr<FF_PDF> combinatoric,
			const FFUtil::tuple_category_name_flags& cat) : FF_PDF_data("pdf_data_sideband_3D_deltae_chil0_ma_nb_prime",cat) {

		if(dynamic_cast<FF_PDF_bkg_continuum_3D_deltae_chil0_ma_nb_prime*>(continuum.get())) {
			m_comb_pdfs.push_back(continuum);
		} else {
			std::cerr << "ERROR: PDF for continuum component in " << m_label << " was not of type " << "FF_PDF_bkg_continuum_3D_deltae_chil0_ma_nb_prime" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_combinatoric_3D_deltae_chil0_ma_nb_prime*>(combinatoric.get())) {
			m_comb_pdfs.push_back(combinatoric);
		} else {
			std::cerr << "ERROR: PDF for combinatoric component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_combinatoric_3D_deltae_chil0_ma_nb_prime" << std::endl;
			exit(EXIT_FAILURE);
		}

		m_vars.push_back(new RooRealVar("C_frac_qq","C_frac_qq",0.9,0.7,1.0));
		m_vars.push_back(new RooRealVar("N_frac_qq","N_frac_qq",0.9,0.7,1.0));

		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C")),
				RooArgList(*m_vars.at(0))));
		m_pdfs.push_back(new RooAddPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N")),
				RooArgList(*m_vars.at(1))));

		m_main_pdfs["CS"] = m_pdfs.at(0);
		m_main_pdfs["NS"] = m_pdfs.at(1);
		m_main_pdfs["COFF"] = &(m_comb_pdfs.at(0)->GetPDF("C"));
		m_main_pdfs["NOFF"] = &(m_comb_pdfs.at(0)->GetPDF("N"));

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_data_sideband_6D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th2 : public FF_PDF_data {
public:
	virtual ~FF_PDF_data_sideband_6D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th2() { }

	FF_PDF_data_sideband_6D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th2(shared_ptr<FF_PDF> continuum,
			shared_ptr<FF_PDF> combinatoric,
			const FFUtil::tuple_category_name_flags& cat) : FF_PDF_data("pdf_data_sideband_6D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th2",cat) {

		if(dynamic_cast<FF_PDF_bkg_continuum_6D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th2*>(continuum.get())) {
			m_comb_pdfs.push_back(continuum);
		} else {
			std::cerr << "ERROR: PDF for continuum component in " << m_label << " was not of type " << "FF_PDF_bkg_continuum_6D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th2" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_combinatoric_6D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th2*>(combinatoric.get())) {
			m_comb_pdfs.push_back(combinatoric);
		} else {
			std::cerr << "ERROR: PDF for combinatoric component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_combinatoric_6D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th2" << std::endl;
			exit(EXIT_FAILURE);
		}

		m_vars.push_back(new RooRealVar("C_frac_qq","C_frac_qq",0.9,0.7,1.0));
		m_vars.push_back(new RooRealVar("N_frac_qq","N_frac_qq",0.9,0.7,1.0));

		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C")),
				RooArgList(*m_vars.at(0))));
		m_pdfs.push_back(new RooAddPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N")),
				RooArgList(*m_vars.at(1))));

		m_main_pdfs["CS"] = m_pdfs.at(0);
		m_main_pdfs["NS"] = m_pdfs.at(1);
		m_main_pdfs["COFF"] = &(m_comb_pdfs.at(0)->GetPDF("C"));
		m_main_pdfs["NOFF"] = &(m_comb_pdfs.at(0)->GetPDF("N"));

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_data_sideband_7D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th1_hel_th2 : public FF_PDF_data {
public:
	virtual ~FF_PDF_data_sideband_7D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th1_hel_th2() { }

	FF_PDF_data_sideband_7D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th1_hel_th2(shared_ptr<FF_PDF> continuum,
			shared_ptr<FF_PDF> combinatoric,
			const FFUtil::tuple_category_name_flags& cat) : FF_PDF_data("pdf_data_sideband_7D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th1_hel_th2",cat) {

		if(dynamic_cast<FF_PDF_bkg_continuum_7D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th1_hel_th2*>(continuum.get())) {
			m_comb_pdfs.push_back(continuum);
		} else {
			std::cerr << "ERROR: PDF for continuum component in " << m_label << " was not of type " << "FF_PDF_bkg_continuum_7D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th1_hel_th2" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_combinatoric_7D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th1_hel_th2*>(combinatoric.get())) {
			m_comb_pdfs.push_back(combinatoric);
		} else {
			std::cerr << "ERROR: PDF for combinatoric component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_combinatoric_7D_deltae_chil0_ma_nb_prime_mass_kpi_hel_phi_hel_th1_hel_th2" << std::endl;
			exit(EXIT_FAILURE);
		}

		m_vars.push_back(new RooRealVar("C_frac_qq","C_frac_qq",0.9,0.7,1.0));
		m_vars.push_back(new RooRealVar("N_frac_qq","N_frac_qq",0.9,0.7,1.0));

		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C")),
				RooArgList(*m_vars.at(0))));
		m_pdfs.push_back(new RooAddPdf(("N_"+m_label).c_str(),("N_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("N"),m_comb_pdfs.at(1)->GetPDF("N")),
				RooArgList(*m_vars.at(1))));

		m_main_pdfs["CS"] = m_pdfs.at(0);
		m_main_pdfs["NS"] = m_pdfs.at(1);
		m_main_pdfs["COFF"] = &(m_comb_pdfs.at(0)->GetPDF("C"));
		m_main_pdfs["NOFF"] = &(m_comb_pdfs.at(0)->GetPDF("N"));

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_data_4D_mbc_deltae_chil0_ma_nb_prime__CPV : public FF_PDF_data {
public:
	virtual ~FF_PDF_data_4D_mbc_deltae_chil0_ma_nb_prime__CPV() { }
    
	FF_PDF_data_4D_mbc_deltae_chil0_ma_nb_prime__CPV(shared_ptr<FF_PDF> signal, shared_ptr<FF_PDF> continuum,
                                                     shared_ptr<FF_PDF> wrongphikkbar, double nevents,
                                                     const FFUtil::tuple_category_name_flags& cat) : FF_PDF_data("FF_PDF_data_4D_mbc_deltae_chil0_ma_nb_prime__CPV",cat) {
        
		if(dynamic_cast<FF_PDF_signal_4D_mbc_deltae_chil0_ma_nb_prime*>(signal.get())) {
			m_comb_pdfs.push_back(signal);
		} else {
			std::cerr << "ERROR: PDF for signal component in " << m_label << " was not of type " << "FF_PDF_signal_4D_mbc_deltae_chil0_ma_nb_prime" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_continuum_4D_mbc_deltae_chil0_ma_nb_prime*>(continuum.get())) {
			m_comb_pdfs.push_back(continuum);
		} else {
			std::cerr << "ERROR: PDF for continuum component in " << m_label << " was not of type " << "FF_PDF_bkg_continuum_4D_mbc_deltae_chil0_ma_nb_prime" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_wrongphikkbar_4D_mbc_deltae_chil0_ma_nb_prime*>(wrongphikkbar.get())) {
			m_comb_pdfs.push_back(wrongphikkbar);
		} else {
			std::cerr << "ERROR: PDF for wrongphikkbar component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_wrongphikkbar_4D_mbc_deltae_chil0_ma_nb_prime" << std::endl;
			exit(EXIT_FAILURE);
		}
        
		m_vars.push_back(new RooRealVar("signal_nevents","signal_nevents",1000.0,-150.0,5000.0));
		m_vars.push_back(new RooRealVar("signal_nevents_asym","signal_nevents_asym",0.0,-200.0,200.0));
		m_vars.push_back(new RooRealVar("C_bkg_continuum_nevents","C_bkg_continuum_nevents",nevents/2.0,1000.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongphikkbar_nevents","C_bkg_bbar_wrongphikkbar_nevents",15,5,25));
		m_pdfs.push_back(new RooGaussian("C_bkg_bbar_wrongphikkbar_constraint","C_bkg_bbar_wrongphikkbar_constraint",*m_vars.back(),RooConst(15.02),RooConst(1.42)));
        
		m_functions.push_back(new RooFormulaVar("CP_signal_nevents","CP_signal_nevents","@0/2 + @1",RooArgList(*m_vars.at(0),*m_vars.at(1))));
		m_functions.push_back(new RooFormulaVar("CA_signal_nevents","CA_signal_nevents","@0/2 - @1",RooArgList(*m_vars.at(0),*m_vars.at(1))));
		m_functions.push_back(new RooFormulaVar("CP_bkg_continuum_nevents","CP_bkg_continuum_nevents","@0/2",RooArgList(*m_vars.at(2))));
		m_functions.push_back(new RooFormulaVar("CA_bkg_continuum_nevents","CA_bkg_continuum_nevents","@0/2",RooArgList(*m_vars.at(2))));
		m_functions.push_back(new RooFormulaVar("CP_bkg_bbar_wrongphikkbar_nevents","CP_bkg_bbar_wrongphikkbar_nevents","@0/2",RooArgList(*m_vars.at(3))));
		m_functions.push_back(new RooFormulaVar("CA_bkg_bbar_wrongphikkbar_nevents","CA_bkg_bbar_wrongphikkbar_nevents","@0/2",RooArgList(*m_vars.at(3))));
        
		m_pdfs.push_back(new RooAddPdf(("CP_"+m_label+"_noconstraint").c_str(),("CP_"+m_label+"_noconstraint").c_str(),RooArgList(
                                                                                                                                  m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
                                                                                                                                  m_comb_pdfs.at(2)->GetPDF("C")),
                                       RooArgList(*m_functions.at(0),*m_functions.at(2),*m_functions.at(4)),false));
		m_pdfs.push_back(new RooProdPdf(("CP_"+m_label).c_str(),("CP_"+m_label).c_str(),
                                        RooArgList(*m_pdfs.back(),*m_pdfs.at(0))));
		m_main_pdfs["CP"] = m_pdfs.back();
        
		m_pdfs.push_back(new RooAddPdf(("CA_"+m_label+"_noconstraint").c_str(),("CA_"+m_label+"_noconstraint").c_str(),RooArgList(
                                                                                                                                  m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
                                                                                                                                  m_comb_pdfs.at(2)->GetPDF("C")),
                                       RooArgList(*m_functions.at(1),*m_functions.at(3),*m_functions.at(5)),false));
		m_pdfs.push_back(new RooProdPdf(("CA_"+m_label).c_str(),("CA_"+m_label).c_str(),
                                        RooArgList(*m_pdfs.back(),*m_pdfs.at(0))));
		m_main_pdfs["CA"] = m_pdfs.back();
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};



class FF_PDF_data_3D_mbc_deltae_nb_prime__CPV : public FF_PDF_data {
public:
	virtual ~FF_PDF_data_3D_mbc_deltae_nb_prime__CPV() { }

	FF_PDF_data_3D_mbc_deltae_nb_prime__CPV(shared_ptr<FF_PDF> signal, shared_ptr<FF_PDF> continuum, shared_ptr<FF_PDF> combinatoric,
			shared_ptr<FF_PDF> wrongmasshyp, shared_ptr<FF_PDF> wrongpi0hyp, shared_ptr<FF_PDF> missingfsp, shared_ptr<FF_PDF> nonres, double nevents,
			const FFUtil::tuple_category_name_flags& cat) : FF_PDF_data("FF_PDF_data_3D_mbc_deltae_nb_prime__CPV",cat) {

		if(dynamic_cast<FF_PDF_signal_3D_mbc_deltae_nb_prime*>(signal.get())) {
			m_comb_pdfs.push_back(signal);
		} else {
			std::cerr << "ERROR: PDF for signal component in " << m_label << " was not of type " << "FF_PDF_data_3D_mbc_deltae_nb_prime__CPV" << std::endl;
			exit(EXIT_FAILURE);
		}
        
        if(dynamic_cast<FF_PDF_bkg_continuum_3D_mbc_deltae_nb_prime*>(continuum.get())) {
			m_comb_pdfs.push_back(continuum);
		} else {
			std::cerr << "ERROR: PDF for continuum component in " << m_label << " was not of type " << "FF_PDF_data_3D_mbc_deltae_nb_prime__CPV" << std::endl;
			exit(EXIT_FAILURE);
		}
        
        if(dynamic_cast<FF_PDF_bkg_bbar_combinatoric_3D_mbc_deltae_nb_prime*>(combinatoric.get())) {
			m_comb_pdfs.push_back(combinatoric);
		} else {
			std::cerr << "ERROR: PDF for combinatoric component in " << m_label << " was not of type " << "FF_PDF_data_3D_mbc_deltae_nb_prime__CPV" << std::endl;
			exit(EXIT_FAILURE);
		}
        
        if(dynamic_cast<FF_PDF_bkg_bbar_wrongmasshyp_3D_mbc_deltae_nb_prime*>(wrongmasshyp.get())) {
			m_comb_pdfs.push_back(wrongmasshyp);
		} else {
			std::cerr << "ERROR: PDF for wrongmasshyp component in " << m_label << " was not of type " << "FF_PDF_data_3D_mbc_deltae_nb_prime__CPV" << std::endl;
			exit(EXIT_FAILURE);
		}
        
        if(dynamic_cast<FF_PDF_bkg_bbar_wrongpi0hyp_3D_mbc_deltae_nb_prime*>(wrongpi0hyp.get())) {
			m_comb_pdfs.push_back(wrongpi0hyp);
		} else {
			std::cerr << "ERROR: PDF for wrongpi0hyp component in " << m_label << " was not of type " << "FF_PDF_data_3D_mbc_deltae_nb_prime__CPV" << std::endl;
			exit(EXIT_FAILURE);
		}
        
        if(dynamic_cast<FF_PDF_bkg_bbar_missingfsp_3D_mbc_deltae_nb_prime*>(missingfsp.get())) {
			m_comb_pdfs.push_back(missingfsp);
		} else {
			std::cerr << "ERROR: PDF for missingfsp component in " << m_label << " was not of type " << "FF_PDF_data_3D_mbc_deltae_nb_prime__CPV" << std::endl;
			exit(EXIT_FAILURE);
		}
        
        if(dynamic_cast<FF_PDF_bkg_bbar_nonres_3D_mbc_deltae_nb_prime*>(nonres.get())) {
			m_comb_pdfs.push_back(nonres);
		} else {
			std::cerr << "ERROR: PDF for nonres component in " << m_label << " was not of type " << "FF_PDF_data_3D_mbc_deltae_nb_prime__CPV" << std::endl;
			exit(EXIT_FAILURE);
		}
    
		m_vars.push_back(new RooRealVar("signal_nevents","signal_nevents",10000.0,-150.0,5000000.0)); //V0
		m_vars.push_back(new RooRealVar("signal_nevents_asym","signal_nevents_asym",0.0,-200.0,200.0)); //V1
		m_vars.push_back(new RooRealVar("C_bkg_continuum_nevents","C_bkg_continuum_nevents",nevents/7.0,0,nevents*1.2));//V2
        m_vars.push_back(new RooRealVar("C_bkg_bbar_combinatoric_nevents","C_bkg_bbar_combinatoric_nevents",nevents/7.0,0,nevents*1.2));//V3
        m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongmasshyp_nevents","C_bkg_bbar_wrongmasshyp_nevents",nevents/7.0,0,nevents*1.2));//V4
        m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongpi0hyp_nevents","C_bkg_bbar_wrongpi0hyp_nevents",nevents/7.0,0,nevents*1.2));//V5
        m_vars.push_back(new RooRealVar("C_bkg_bbar_missingfsp_nevents","C_bkg_bbar_missingfsp_nevents",nevents/7.0,0,nevents*1.2));//V6
        m_vars.push_back(new RooRealVar("C_bkg_bbar_nonres_nevents","C_bkg_bbar_nonres_nevents",nevents/7.0,0,nevents*1.2));//V7
        /*
        m_vars.push_back(new RooRealVar("CA_bkg_continuum_nevents","CA_bkg_continuum_nevents",nevents/7.0,0,nevents*1.2));//V8
        m_vars.push_back(new RooRealVar("CA_bkg_bbar_combinatoric_nevents","CA_bkg_bbar_combinatoric_nevents",nevents/7.0,0,nevents*1.2));//V9
        m_vars.push_back(new RooRealVar("CA_bkg_bbar_wrongmasshyp_nevents","CA_bkg_bbar_wrongmasshyp_nevents",nevents/7.0,0,nevents*1.2));//V10
        m_vars.push_back(new RooRealVar("CA_bkg_bbar_wrongpi0hyp_nevents","CA_bkg_bbar_wrongpi0hyp_nevents",nevents/7.0,0,nevents*1.2));//V11
        m_vars.push_back(new RooRealVar("CA_bkg_bbar_missingfsp_nevents","CA_bkg_bbar_missingfsp_nevents",nevents/7.0,0,nevents*1.2));//V12
        m_vars.push_back(new RooRealVar("CA_bkg_bbar_nonres_nevents","CA_bkg_bbar_nonres_nevents",nevents/7.0,0,nevents*1.2));//V13
  
        //have in mind this
//		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongphikkbar_nevents","C_bkg_bbar_wrongphikkbar_nevents",15,5,25));
//		m_pdfs.push_back(new RooGaussian("C_bkg_bbar_wrongphikkbar_constraint","C_bkg_bbar_wrongphikkbar_constraint",*m_vars.back(),RooConst(15.02),RooConst(1.42)));

		m_functions.push_back(new RooFormulaVar("CP_signal_nevents","CP_signal_nevents","@0/2 + @1",RooArgList(*m_vars.at(0),*m_vars.at(1))));//f0
		m_functions.push_back(new RooFormulaVar("CA_signal_nevents","CA_signal_nevents","@0/2 - @1",RooArgList(*m_vars.at(0),*m_vars.at(1))));//f1
/*
		m_functions.push_back(new RooFormulaVar("CP_bkg_continuum_nevents","CP_bkg_continuum_nevents","@0/2",RooArgList(*m_vars.at(2))));
		m_functions.push_back(new RooFormulaVar("CA_bkg_continuum_nevents","CA_bkg_continuum_nevents","@0/2",RooArgList(*m_vars.at(2))));
		m_functions.push_back(new RooFormulaVar("CP_bkg_bbar_wrongphikkbar_nevents","CP_bkg_bbar_wrongphikkbar_nevents","@0/2",RooArgList(*m_vars.at(3))));
		m_functions.push_back(new RooFormulaVar("CA_bkg_bbar_wrongphikkbar_nevents","CA_bkg_bbar_wrongphikkbar_nevents","@0/2",RooArgList(*m_vars.at(3))));
*/

        m_pdfs.push_back(new RooAddPdf(("C_"+m_label+"_noconstraint").c_str(),("C_"+m_label+"_noconstraint").c_str(),RooArgList(
                m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"), m_comb_pdfs.at(2)->GetPDF("C"), m_comb_pdfs.at(3)->GetPDF("C"),
                m_comb_pdfs.at(4)->GetPDF("C"),m_comb_pdfs.at(5)->GetPDF("C"),m_comb_pdfs.at(6)->GetPDF("C")),
                RooArgList(*m_vars.at(0),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5),*m_vars.at(6),*m_vars.at(7)),false));
        //		m_pdfs.push_back(new RooProdPdf(("CP_"+m_label).c_str(),("CP_"+m_label).c_str(), this is constrained
        //				RooArgList(*m_pdfs.back(),*m_pdfs.at(0))));
		m_main_pdfs["C"] = m_pdfs.back();
        /*
        
        m_pdfs.push_back(new RooAddPdf(("CP_"+m_label+"_noconstraint").c_str(),("CP_"+m_label+"_noconstraint").c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"), m_comb_pdfs.at(2)->GetPDF("C"), m_comb_pdfs.at(3)->GetPDF("C"),
                m_comb_pdfs.at(4)->GetPDF("C"),m_comb_pdfs.at(5)->GetPDF("C"),m_comb_pdfs.at(6)->GetPDF("C")),
				RooArgList(*m_functions.at(0),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5),*m_vars.at(6),*m_vars.at(7)),false));
//		m_pdfs.push_back(new RooProdPdf(("CP_"+m_label).c_str(),("CP_"+m_label).c_str(), this is constrained
//				RooArgList(*m_pdfs.back(),*m_pdfs.at(0))));
		m_main_pdfs["CP"] = m_pdfs.back();

		m_pdfs.push_back(new RooAddPdf(("CA_"+m_label+"_noconstraint").c_str(),("CA_"+m_label+"_noconstraint").c_str(),RooArgList(
                m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"), m_comb_pdfs.at(2)->GetPDF("C"), m_comb_pdfs.at(3)->GetPDF("C"),
                m_comb_pdfs.at(4)->GetPDF("C"),m_comb_pdfs.at(5)->GetPDF("C"),m_comb_pdfs.at(6)->GetPDF("C")),
				RooArgList(*m_functions.at(1),*m_vars.at(8),*m_vars.at(9),*m_vars.at(10),*m_vars.at(11),*m_vars.at(12),*m_vars.at(13)),false));
//		m_pdfs.push_back(new RooProdPdf(("CA_"+m_label).c_str(),("CA_"+m_label).c_str(),
//				RooArgList(*m_pdfs.back(),*m_pdfs.at(0))));
		m_main_pdfs["CA"] = m_pdfs.back();
*/
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_data_5D_mass_kpi_angles__CPV : public FF_PDF_data {
public:
	virtual ~FF_PDF_data_5D_mass_kpi_angles__CPV() { }

	FF_PDF_data_5D_mass_kpi_angles__CPV(shared_ptr<FF_PDF> signal, shared_ptr<FF_PDF> continuum,
			shared_ptr<FF_PDF> wrongphikkbar, double nevents,
			const FFUtil::tuple_category_name_flags& cat) : FF_PDF_data("FF_PDF_data_5D_mass_kpi_angles__CPV",cat) {

		if(dynamic_cast<FF_PDF_signal_5D_mass_helicity_angles_CPV*>(signal.get())) {
			m_comb_pdfs.push_back(signal);
		} else {
			std::cerr << "ERROR: PDF for signal component in " << m_label << " was not of type " << "FF_PDF_signal_5D_mass_helicity_angles_CPV" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_continuum_5D_mass_kpi_hel_phi_hel_th1_hel_th2_flavor*>(continuum.get())) {
			m_comb_pdfs.push_back(continuum);
		} else {
			std::cerr << "ERROR: PDF for continuum component in " << m_label << " was not of type " << "FF_PDF_bkg_continuum_5D_mass_kpi_hel_phi_hel_th1_hel_th2_flavor" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_wrongphikkbar_5D_mass_kpi_angles_flavor*>(wrongphikkbar.get())) {
			m_comb_pdfs.push_back(wrongphikkbar);
		} else {
			std::cerr << "ERROR: PDF for wrongphikkbar component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_wrongphikkbar_5D_mass_kpi_angles_flavor" << std::endl;
			exit(EXIT_FAILURE);
		}

		m_vars.push_back(new RooRealVar("signal_nevents","signal_nevents",1000.0,-150.0,5000.0));
		m_vars.push_back(new RooRealVar("C_bkg_continuum_nevents","C_bkg_continuum_nevents",nevents,1000.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongphikkbar_nevents","C_bkg_bbar_wrongphikkbar_nevents",15,5,25));
		m_pdfs.push_back(new RooGaussian("C_bkg_bbar_wrongphikkbar_constraint","C_bkg_bbar_wrongphikkbar_constraint",*m_vars.back(),RooConst(15.02),RooConst(1.42)));

		m_pdfs.push_back(new RooAddPdf(("C_"+m_label+"_noconstraint").c_str(),("C_"+m_label+"_noconstraint").c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
				m_comb_pdfs.at(2)->GetPDF("C")),
				RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2)),false));
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(*m_pdfs.back(),*m_pdfs.at(0))));
		m_main_pdfs["C"] = m_pdfs.back();

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_data_6D__CPV : public FF_PDF_data {
public:
	virtual ~FF_PDF_data_6D__CPV() { }
    
	FF_PDF_data_6D__CPV(shared_ptr<FF_PDF> signal, shared_ptr<FF_PDF> continuum, shared_ptr<FF_PDF> combinatoric, shared_ptr<FF_PDF> wrongmasshyp, shared_ptr<FF_PDF> wrongpi0hyp, shared_ptr<FF_PDF> missingfsp, shared_ptr<FF_PDF> nonres, double nevents, const FFUtil::tuple_category_name_flags& cat) : FF_PDF_data("FF_PDF_data_6D__CPV",cat) {
        
		if(dynamic_cast<FF_PDF_signal_6D*>(signal.get())) {
			m_comb_pdfs.push_back(signal);
		} else {
			std::cerr << "ERROR: PDF for signal component in " << m_label << " was not of type " << "FF_PDF_signal_6D" << std::endl;
			exit(EXIT_FAILURE);
		}
        
        if(dynamic_cast<FF_PDF_bkg_continuum_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor*>(continuum.get())) {
			m_comb_pdfs.push_back(continuum);
		} else {
			std::cerr << "ERROR: PDF for continuum component in " << m_label << " was not of type " << "FF_PDF_bkg_continuum_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor" << std::endl;
			exit(EXIT_FAILURE);
		}
        
        if(dynamic_cast<FF_PDF_bkg_bbar_combinatoric_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor*>(combinatoric.get())) {
			m_comb_pdfs.push_back(combinatoric);
		} else {
			std::cerr << "ERROR: PDF for combinatoric component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_combinatoric_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor" << std::endl;
			exit(EXIT_FAILURE);
		}
        
        if(dynamic_cast<FF_PDF_bkg_bbar_wrongmasshyp_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor*>(wrongmasshyp.get())) {
			m_comb_pdfs.push_back(wrongmasshyp);
		} else {
			std::cerr << "ERROR: PDF for wrongmasshyp component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_wrongmasshyp_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor" << std::endl;
			exit(EXIT_FAILURE);
		}
        
        if(dynamic_cast<FF_PDF_bkg_bbar_wrongpi0hyp_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor*>(wrongpi0hyp.get())) {
			m_comb_pdfs.push_back(wrongpi0hyp);
		} else {
			std::cerr << "ERROR: PDF for wrongpi0hyp component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_wrongpi0hyp_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor" << std::endl;
			exit(EXIT_FAILURE);
		}
        
        if(dynamic_cast<FF_PDF_bkg_bbar_missingfsp_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor*>(missingfsp.get())) {
			m_comb_pdfs.push_back(missingfsp);
		} else {
			std::cerr << "ERROR: PDF for missingfsp component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_missingfsp_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor" << std::endl;
			exit(EXIT_FAILURE);
		}
        
        if(dynamic_cast<FF_PDF_bkg_bbar_nonres_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor*>(nonres.get())) {
			m_comb_pdfs.push_back(nonres);
		} else {
			std::cerr << "ERROR: PDF for nonres component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_nonres_6D_mbc_deltae_nb_prime_mass_AB_mass_BC_flavor" << std::endl;
			exit(EXIT_FAILURE);
		}
        
        
        
        
		m_vars.push_back(new RooRealVar("signal_nevents","signal_nevents",1000.0,0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_continuum_nevents","C_bkg_continuum_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_combinatoric_nevents","C_bkg_bbar_combinatoric_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongmasshyp_nevents","C_bkg_bbar_wrongmasshyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongpi0hyp_nevents","C_bkg_bbar_wrongpi0hyp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_missingfsp_nevents","C_bkg_bbar_missingfsp_nevents",nevents/7.0,0.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_nonres_nevents","C_bkg_bbar_nonres_nevents",nevents/7.0,0.0,nevents*1.2));
        
		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),m_comb_pdfs.at(4)->GetPDF("C"),m_comb_pdfs.at(5)->GetPDF("C"),m_comb_pdfs.at(6)->GetPDF("C")),RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_vars.at(4),*m_vars.at(5),*m_vars.at(6)),false));
		m_main_pdfs["C"] = m_pdfs.back();
        
		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_data_9D__CPV : public FF_PDF_data {
public:
	virtual ~FF_PDF_data_9D__CPV() { }

	FF_PDF_data_9D__CPV(shared_ptr<FF_PDF> signal, shared_ptr<FF_PDF> continuum,
			shared_ptr<FF_PDF> wrongphikkbar, double nevents,
			const FFUtil::tuple_category_name_flags& cat) : FF_PDF_data("FF_PDF_data_9D__CPV",cat) {

		if(dynamic_cast<FF_PDF_signal_9D*>(signal.get())) {
			m_comb_pdfs.push_back(signal);
		} else {
			std::cerr << "ERROR: PDF for signal component in " << m_label << " was not of type " << "FF_PDF_signal_9D" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_continuum_9D_mbc_deltae_chil0_ma_mass_kpi_hel_phi_hel_th1_hel_th2_nb_prime_flavor*>(continuum.get())) {
			m_comb_pdfs.push_back(continuum);
		} else {
			std::cerr << "ERROR: PDF for continuum component in " << m_label << " was not of type " << "FF_PDF_bkg_continuum_9D_mbc_deltae_chil0_ma_mass_kpi_hel_phi_hel_th1_hel_th2_nb_prime_flavor" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_wrongphikkbar_9D*>(wrongphikkbar.get())) {
			m_comb_pdfs.push_back(wrongphikkbar);
		} else {
			std::cerr << "ERROR: PDF for wrongphikkbar component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_wrongphikkbar_9D" << std::endl;
			exit(EXIT_FAILURE);
		}

		m_vars.push_back(new RooRealVar("signal_nevents","signal_nevents",1000.0,-150.0,5000.0));
		m_vars.push_back(new RooRealVar("C_bkg_continuum_nevents","C_bkg_continuum_nevents",nevents,1000.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongphikkbar_nevents","C_bkg_bbar_wrongphikkbar_nevents",15,5,25));
		m_pdfs.push_back(new RooGaussian("C_bkg_bbar_wrongphikkbar_constraint","C_bkg_bbar_wrongphikkbar_constraint",*m_vars.back(),RooConst(15.02),RooConst(1.42)));

		m_pdfs.push_back(new RooAddPdf(("C_"+m_label+"_noconstraint").c_str(),("C_"+m_label+"_noconstraint").c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
				m_comb_pdfs.at(2)->GetPDF("C")),
				RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2)),false));
		m_pdfs.push_back(new RooProdPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),
				RooArgList(*m_pdfs.back(),*m_pdfs.at(0))));
		m_main_pdfs["C"] = m_pdfs.back();

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


class FF_PDF_data_9D__CPV__KK_floated : public FF_PDF_data {
public:
	virtual ~FF_PDF_data_9D__CPV__KK_floated() { }

	FF_PDF_data_9D__CPV__KK_floated(shared_ptr<FF_PDF> signal, shared_ptr<FF_PDF> continuum,
			shared_ptr<FF_PDF> wrongphikkbar, double nevents,
			const FFUtil::tuple_category_name_flags& cat) : FF_PDF_data("FF_PDF_data_9D__CPV__KK_floated",cat) {

		if(dynamic_cast<FF_PDF_signal_9D*>(signal.get())) {
			m_comb_pdfs.push_back(signal);
		} else {
			std::cerr << "ERROR: PDF for signal component in " << m_label << " was not of type " << "FF_PDF_signal_9D" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_continuum_9D_mbc_deltae_chil0_ma_mass_kpi_hel_phi_hel_th1_hel_th2_nb_prime_flavor*>(continuum.get())) {
			m_comb_pdfs.push_back(continuum);
		} else {
			std::cerr << "ERROR: PDF for continuum component in " << m_label << " was not of type " << "FF_PDF_bkg_continuum_9D_mbc_deltae_chil0_ma_mass_kpi_hel_phi_hel_th1_hel_th2_nb_prime_flavor" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_wrongphikkbar_9D*>(wrongphikkbar.get())) {
			m_comb_pdfs.push_back(wrongphikkbar);
		} else {
			std::cerr << "ERROR: PDF for wrongphikkbar component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_wrongphikkbar_9D" << std::endl;
			exit(EXIT_FAILURE);
		}

		m_vars.push_back(new RooRealVar("signal_nevents","signal_nevents",1000.0,-150.0,5000.0));
		m_vars.push_back(new RooRealVar("C_bkg_continuum_nevents","C_bkg_continuum_nevents",nevents,1000.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongphikkbar_nevents","C_bkg_bbar_wrongphikkbar_nevents",50,-50,200));

		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
				m_comb_pdfs.at(2)->GetPDF("C")),
				RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2)),false));
		m_main_pdfs["C"] = m_pdfs.back();

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_data_9D__CPV__KK_Flatte : public FF_PDF_data {
public:
	virtual ~FF_PDF_data_9D__CPV__KK_Flatte() { }

	FF_PDF_data_9D__CPV__KK_Flatte(shared_ptr<FF_PDF> signal, shared_ptr<FF_PDF> continuum,
			shared_ptr<FF_PDF> wrongphif0980, double nevents,
			const FFUtil::tuple_category_name_flags& cat) : FF_PDF_data("FF_PDF_data_9D__CPV__KK_Flatte",cat) {

		if(dynamic_cast<FF_PDF_signal_9D*>(signal.get())) {
			m_comb_pdfs.push_back(signal);
		} else {
			std::cerr << "ERROR: PDF for signal component in " << m_label << " was not of type " << "FF_PDF_signal_9D" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_continuum_9D_mbc_deltae_chil0_ma_mass_kpi_hel_phi_hel_th1_hel_th2_nb_prime_flavor*>(continuum.get())) {
			m_comb_pdfs.push_back(continuum);
		} else {
			std::cerr << "ERROR: PDF for continuum component in " << m_label << " was not of type " << "FF_PDF_bkg_continuum_9D_mbc_deltae_chil0_ma_mass_kpi_hel_phi_hel_th1_hel_th2_nb_prime_flavor" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_wrongphif0980_9D*>(wrongphif0980.get())) {
			m_comb_pdfs.push_back(wrongphif0980);
		} else {
			std::cerr << "ERROR: PDF for wrongphif0980 component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_wrongphif0980_9D" << std::endl;
			exit(EXIT_FAILURE);
		}

		m_vars.push_back(new RooRealVar("signal_nevents","signal_nevents",1000.0,-150.0,5000.0));
		m_vars.push_back(new RooRealVar("C_bkg_continuum_nevents","C_bkg_continuum_nevents",nevents,1000.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongphif0980_nevents","C_bkg_bbar_wrongphif0980r_nevents",50,-50,200));

		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
				m_comb_pdfs.at(2)->GetPDF("C")),
				RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2)),false));
		m_main_pdfs["C"] = m_pdfs.back();

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_data_9D__CPV__KK_FlatteAndNonRes : public FF_PDF_data {
public:
	virtual ~FF_PDF_data_9D__CPV__KK_FlatteAndNonRes() { }

	FF_PDF_data_9D__CPV__KK_FlatteAndNonRes(shared_ptr<FF_PDF> signal, shared_ptr<FF_PDF> continuum,
			shared_ptr<FF_PDF> wrongphif0980, double nevents,
			const FFUtil::tuple_category_name_flags& cat) : FF_PDF_data("FF_PDF_data_9D__CPV__KK_FlatteAndNonRes",cat) {

		if(dynamic_cast<FF_PDF_signal_9D*>(signal.get())) {
			m_comb_pdfs.push_back(signal);
		} else {
			std::cerr << "ERROR: PDF for signal component in " << m_label << " was not of type " << "FF_PDF_signal_9D" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_continuum_9D_mbc_deltae_chil0_ma_mass_kpi_hel_phi_hel_th1_hel_th2_nb_prime_flavor*>(continuum.get())) {
			m_comb_pdfs.push_back(continuum);
		} else {
			std::cerr << "ERROR: PDF for continuum component in " << m_label << " was not of type " << "FF_PDF_bkg_continuum_9D_mbc_deltae_chil0_ma_mass_kpi_hel_phi_hel_th1_hel_th2_nb_prime_flavor" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_wrongphif0980AndNonRes_9D*>(wrongphif0980.get())) {
			m_comb_pdfs.push_back(wrongphif0980);
		} else {
			std::cerr << "ERROR: PDF for wrongphif0980 component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_wrongphif0980AndNonRes_9D" << std::endl;
			exit(EXIT_FAILURE);
		}

		m_vars.push_back(new RooRealVar("signal_nevents","signal_nevents",1000.0,-150.0,5000.0));
		m_vars.push_back(new RooRealVar("C_bkg_continuum_nevents","C_bkg_continuum_nevents",nevents,1000.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongphif0980_nevents","C_bkg_bbar_wrongphif0980_nevents",50,-50,200));

		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
				m_comb_pdfs.at(2)->GetPDF("C")),
				RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2)),false));
		m_main_pdfs["C"] = m_pdfs.back();

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};

class FF_PDF_data_9D__CPV__KK_Flatte_with_interference_pos_neg : public FF_PDF_data {
public:
	virtual ~FF_PDF_data_9D__CPV__KK_Flatte_with_interference_pos_neg() { }

	FF_PDF_data_9D__CPV__KK_Flatte_with_interference_pos_neg(shared_ptr<FF_PDF> signal, shared_ptr<FF_PDF> continuum,
			shared_ptr<FF_PDF> wrongphif0980, shared_ptr<FF_PDF> wrongphif0980_pos, shared_ptr<FF_PDF> wrongphif0980_neg, double nevents,
			const FFUtil::tuple_category_name_flags& cat) : FF_PDF_data("FF_PDF_data_9D__CPV__KK_Flatte_with_interference_pos_neg",cat) {

		if(dynamic_cast<FF_PDF_signal_9D*>(signal.get())) {
			m_comb_pdfs.push_back(signal);
		} else {
			std::cerr << "ERROR: PDF for signal component in " << m_label << " was not of type " << "FF_PDF_signal_9D" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_continuum_9D_mbc_deltae_chil0_ma_mass_kpi_hel_phi_hel_th1_hel_th2_nb_prime_flavor*>(continuum.get())) {
			m_comb_pdfs.push_back(continuum);
		} else {
			std::cerr << "ERROR: PDF for continuum component in " << m_label << " was not of type " << "FF_PDF_bkg_continuum_9D_mbc_deltae_chil0_ma_mass_kpi_hel_phi_hel_th1_hel_th2_nb_prime_flavor" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_wrongphif0980_9D*>(wrongphif0980.get())) {
			m_comb_pdfs.push_back(wrongphif0980);
		} else {
			std::cerr << "ERROR: PDF for wrongphif0980 component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_wrongphif0980_9D" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_wrongphif0980_with_interference_9D_positive*>(wrongphif0980_pos.get())) {
			m_comb_pdfs.push_back(wrongphif0980_pos);
		} else {
			std::cerr << "ERROR: PDF for wrongphif0980 component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_wrongphif0980_with_interference_9D_positive" << std::endl;
			exit(EXIT_FAILURE);
		}
		if(dynamic_cast<FF_PDF_bkg_bbar_wrongphif0980_with_interference_9D_negative*>(wrongphif0980_neg.get())) {
			m_comb_pdfs.push_back(wrongphif0980_neg);
		} else {
			std::cerr << "ERROR: PDF for wrongphif0980 component in " << m_label << " was not of type " << "FF_PDF_bkg_bbar_wrongphif0980_with_interference_9D_negative" << std::endl;
			exit(EXIT_FAILURE);
		}

		m_vars.push_back(new RooRealVar("signal_nevents","signal_nevents",1000.0,-150.0,5000.0));
		m_vars.push_back(new RooRealVar("C_bkg_continuum_nevents","C_bkg_continuum_nevents",nevents,1000.0,nevents*1.2));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongphif0980_nevents","C_bkg_bbar_wrongphif0980_nevents",50,-50,250));
		m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongphif0980_nevents_positive","C_bkg_bbar_wrongphif0980_nevents_positive",800.0));
		m_functions.push_back(new RooFormulaVar("C_bkg_bbar_wrongphif0980_nevents_negative","C_bkg_bbar_wrongphif0980_nevents_negative","-1 * @0",RooArgList(*m_vars.at(3))));
		//m_vars.push_back(new RooRealVar("C_bkg_bbar_wrongphif0980_nevents_negative","C_bkg_bbar_wrongphif0980_nevents_negative",-10.0));

		m_pdfs.push_back(new RooAddPdf(("C_"+m_label).c_str(),("C_"+m_label).c_str(),RooArgList(
				m_comb_pdfs.at(0)->GetPDF("C"),m_comb_pdfs.at(1)->GetPDF("C"),
				m_comb_pdfs.at(2)->GetPDF("C"),m_comb_pdfs.at(3)->GetPDF("C"),
				m_comb_pdfs.at(4)->GetPDF("C")),
				RooArgList(*m_vars.at(0),*m_vars.at(1),*m_vars.at(2),*m_vars.at(3),*m_functions.at(0)),false));
		m_main_pdfs["C"] = m_pdfs.back();

		m_sim_pdf = new RooSimultaneous(m_label.c_str(),m_label.c_str(),*std::get<0>(m_category_name_flags));
		for(auto& p : m_main_pdfs) {
			m_sim_pdf->addPdf(*p.second,p.first.c_str());
		}
	}
};


#endif /* FF_PDF_DATA_H_ */
