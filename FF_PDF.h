/**
 * @file	FF_PDF.h
 * @date	Nov 15, 2011
 * @author	petric
 * @brief	brief description
 *
 * long description
 */

#ifndef FF_PDF_H_
#define FF_PDF_H_

#include "FitFramework.h"
#include "FF_Util.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/map.hpp>

typedef std::map<std::string, std::pair<double, std::pair<double, std::pair<bool, std::pair<double,double> > > > > FF_PDF_IO_FORMAT;

class FF_PDF {
public:
	virtual ~FF_PDF() {
		for(RooRealVar* v: m_vars) {
			delete v;
		}
		for(RooAbsPdf* p: m_pdfs) {
			delete p;
		}
		for(RooAbsReal* f: m_functions) {
			delete f;
		}
		delete m_sim_pdf;
	}

	RooSimultaneous& GetSimPDF() const {
		return *m_sim_pdf;
	}

	std::string GetSimPDFLabel() const {
		return m_label;
	}

	bool HasPDF(const std::string& flag) const {
		std::map<std::string, RooAbsPdf*>::const_iterator it = m_main_pdfs.find(flag);
		if(it != m_main_pdfs.end()) {
			return true;
		} else {
			return false;
		}
	}

	RooAbsPdf& GetPDF(const std::string& flag) const {
		std::map<std::string, RooAbsPdf*>::const_iterator it = m_main_pdfs.find(flag);
		if(it != m_main_pdfs.end()) {
			return *(*it).second;
		} else {
			std::cerr << "ERROR: Category flag " << flag << " not found in FF_PDF in GetPDF(..)" << m_label << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	FFUtil::tuple_pdfname_color_style_label GetPDFStyle(const std::string& flag) const {
		std::map<std::string, RooAbsPdf*>::const_iterator it = m_main_pdfs.find(flag);
		if(it != m_main_pdfs.end()) {
			return std::make_tuple((*it).second->GetName(),m_pdf_color,m_pdf_linestyle,m_pdf_legend_label);
		} else {
			std::cerr << "ERROR: Category flag " << flag << " not found in FF_PDF in GetPDFStyle(..)" << m_label << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	FFUtil::tuple_category_name_flags& GetCategory() {
		return m_category_name_flags;
	}

	void ExportPDF(const std::string& filename, bool verbose = false) const {
		std::cout << "Exporting PDF: " << m_label << " to " << filename << std::endl;
		shared_ptr<TFile> f = make_shared<TFile>(filename.c_str(),"RECREATE");
		shared_ptr<RooWorkspace> w = make_shared<RooWorkspace>(("w_"+m_label).c_str(),("w_"+m_label).c_str());
		w->import(*m_sim_pdf);
		if(verbose) {
			std::cout << "Content of RooWorkspace: " << "w_"+m_label << std::endl;
			w->Print();
			std::cout << "Content ENDE" << std::endl;
		}
		w->Write();
		f->Save();
	}

	void ExportPDFValues(const std::string& filename, bool verbose = false) const {
		std::cout << "Exporting PDF: " << m_label << " to " << filename << std::endl;
		// check if we would overwrite some existing file
		std::ifstream ifs(filename.c_str());
		bool file_exists = false;
		if(ifs.is_open()) {
			file_exists = true;
			ifs.close();
		}
		if(file_exists) {
			std::string filename_backup = filename + ".backup";
			std::string cmd = "mv " + filename + " " + filename_backup;
			if(system(cmd.c_str())) {
				std::cerr << "ERROR: Couldn't properly execute " << cmd << std::endl;
			}
			std::cout << "WARNING: File " << filename << " already exists, moved to backup file " << filename_backup << std::endl;
		}
		// get PDF values and export them
		std::ofstream fout(filename.c_str());
		if(fout.is_open()) {
			FF_PDF_IO_FORMAT variables;
			unsigned int counter = 1;
			for(RooRealVar* v : GetVariables()) {
				variables[v->GetName()] = std::make_pair(v->getVal(), std::make_pair(v->getError(), std::make_pair(v->hasAsymError(), std::make_pair(v->getAsymErrorLo(),v->getAsymErrorHi()))));
				if(verbose) {
					std::cout << "  Export: " << std::setw(3) << counter << ") " << v->GetName() << " = " << v->getVal() << " +- " << v->getError();
					std::cout << "  (-" << v->getAsymErrorLo() << ", +" << v->getAsymErrorHi() << ")" << std::endl;
					if(std::fabs(v->getError()/v->getVal()) > 0.5) {
						std::cout << "WARNING: Relative uncertainty on " << v->GetName() << " is larger than 50%" << std::endl;
					}
					++counter;
				}
			}
			boost::archive::text_oarchive oa(fout);
			oa << variables;
			fout.close();
		} else {
			std::cerr << "ERROR: ExportPDFValues(...) Could not open filestream for " << filename << "!" << std::endl;
		}
	}

	bool ImportPDFValues(const std::string& filename, bool verbose = false) {
		std::cout << "Importing PDF: " << m_label << " from " << filename << std::endl;
		// import PDF values
		std::ifstream fin(filename.c_str());
		if(fin.is_open()) {
			FF_PDF_IO_FORMAT variables;
			boost::archive::text_iarchive ia(fin);
			ia >> variables;
			fin.close();

			unsigned int counter = 1;
			for(RooRealVar* v : GetVariables()) {
				if(verbose) {
					std::cout << "  Import: " << std::setw(3) << counter << ") " << v->GetName() << " was " << v->getVal() << " +- " << v->getError();
					++counter;
				}
				if(variables.count(v->GetName())) {
					v->setVal(variables[v->GetName()].first);
					v->setError(variables[v->GetName()].second.first);
					if(variables[v->GetName()].second.second.first)
						v->setAsymError(variables[v->GetName()].second.second.second.first,variables[v->GetName()].second.second.second.second);
					if(verbose) {
						std::cout << " and is reset to " << v->getVal() << " +- " << v->getError();
						std::cout << "  (-" << v->getAsymErrorLo() << ", +" << v->getAsymErrorHi() << ")" << std::endl;
					}
				} else {
					std::cout << std::endl;
					std::cerr << "ERROR: ImportPDFValues(...) Variable " << v->GetName() << " not found in imported file " << filename << std::endl;
				}
			}
		} else {
			std::cerr << "ERROR: ImportPDFValues(...) Could not open filestream for " << filename << "!" << std::endl;
			return false;
		}
		return true;
	}

	RooRealVar* GetVariable(const std::string &varname) {
		std::vector<RooRealVar*> variables = GetVariables();
		for(RooRealVar* v : variables) {
			std::string s = v->GetName();
			if(s == varname) {
				return v;
			}
		}
		std::cerr << "ERROR: Variable " << varname << " not found in PDF " << m_label << std::endl;
		exit(EXIT_FAILURE);
	}

	RooAbsReal* GetFunction(const std::string &funcname) {
		std::vector<RooAbsReal*> functions = GetFunctions();
		for(RooAbsReal* f : functions) {
			std::string s = f->GetName();
			if(s == funcname) {
				return f;
			}
		}
		std::cerr << "ERROR: Function " << funcname << " not found in PDF " << m_label << std::endl;
		exit(EXIT_FAILURE);
	}

	void SetVariable(const std::string &varname, double value, double error, bool constant, bool var_must_exist = true) {
		std::vector<RooRealVar*> variables = GetVariables();
		for(RooRealVar* v : variables) {
			std::string s = v->GetName();
			if(s == varname) {
				v->setVal(value);
				v->setError(error);
				v->setConstant(constant);
				return;
			}
		}

		if(var_must_exist) {
			std::cerr << "ERROR: Variable " << varname << " not found in PDF " << m_label << std::endl;
			exit(EXIT_FAILURE);
		} else {
			std::cerr << "WARNING: Variable " << varname << " not found in PDF " << m_label << std::endl;
		}
	}

	void SetVariablesConst() const {
		std::vector<RooRealVar*> variables = GetVariables();
		for(RooRealVar* v : variables) {
			v->setConstant(true);
		}
	}

	std::vector<RooRealVar*> GetVariables() const {
		std::vector<RooRealVar*> variables;
		for(RooRealVar *v : m_vars) {
			variables.push_back(v);
		}
		for(unsigned int i = 0; i < m_comb_pdfs.size(); ++i) {
			m_comb_pdfs[i]->GetVariables(variables);
		}
		return variables;
	}

	std::vector<RooAbsReal*> GetFunctions() const {
		std::vector<RooAbsReal*> functions;
		for(RooAbsReal *f : m_functions) {
			functions.push_back(f);
		}
		for(unsigned int i = 0; i < m_comb_pdfs.size(); ++i) {
			m_comb_pdfs[i]->GetFunctions(functions);
		}
		return functions;
	}

	bool HasFreeParameter() const {
		std::vector<RooRealVar*> variables;
		for(RooRealVar *v : m_vars) {
			variables.push_back(v);
		}
		for(unsigned int i = 0; i < m_comb_pdfs.size(); ++i) {
			m_comb_pdfs[i]->GetVariables(variables);
		}
		for(RooRealVar *v : variables) {
			if(v->getAttribute("Constant") == false)
				return true;
		}
		return false;
	}

	std::vector<FFUtil::tuple_pdfname_color_style_label> GetPDFStyles(const std::string& flag, bool debug = false) {
		std::vector<FFUtil::tuple_pdfname_color_style_label> pdf_styles;
		// this is debug code, which adds even the different components of this pdf
		if(debug) {
			// some predefined colors and styles for components
			int colors[4] = {kOrange, kSpring+5, kMagenta, kViolet};
			int linestyles[3] = {kDashed, kDotted, kDashDotted};
			unsigned int color_count = 0;
			for(unsigned i = 0; i < m_pdfs.size(); ++i) {
				if(m_pdfs[i] == m_main_pdfs[flag]) {
					continue;
				} else {
					std::string pdf_flag(m_pdfs[i]->GetName());
					if(pdf_flag.substr(0,flag.size()) == flag) {
						std::stringstream ss;
						ss << color_count+1 << ". component";
						pdf_styles.push_back(std::make_tuple(pdf_flag,colors[color_count%4],linestyles[(color_count/4)%3],ss.str()));
						++color_count;
					}
				}
			}
		}
		// get main-components from comb_pdfs
		for(unsigned int i = 0; i < m_comb_pdfs.size(); ++i) {
			if(m_comb_pdfs[i]->HasPDF(flag))
				pdf_styles.push_back(m_comb_pdfs[i]->GetPDFStyle(flag));
			else {
				std::cout << "INFO: No component with flag " << flag << " was found in " << m_comb_pdfs[i]->GetSimPDFLabel() << std::endl;
			}
		}
		return pdf_styles;
	}

protected:
	FF_PDF(const std::string &label) : m_label(label), m_sim_pdf(NULL), m_pdf_color(kBlack), m_pdf_linestyle(kSolid) {

	}

	void GetVariables(std::vector<RooRealVar*>& variables) {
		for(RooRealVar *v : m_vars) {
			variables.push_back(v);
		}
		for(unsigned int i = 0; i < m_comb_pdfs.size(); ++i) {
			m_comb_pdfs[i]->GetVariables(variables);
		}
	}

	void GetFunctions(std::vector<RooAbsReal*>& functions) {
		for(RooAbsReal *f : m_functions) {
			functions.push_back(f);
		}
		for(unsigned int i = 0; i < m_comb_pdfs.size(); ++i) {
			m_comb_pdfs[i]->GetFunctions(functions);
		}
	}

	// label for related filenames
	std::string m_label;
	// the final simultaneous pdf
	RooSimultaneous* m_sim_pdf;
	// the tuple with the different category information
	FFUtil::tuple_category_name_flags m_category_name_flags;
	// different drawing options
	int m_pdf_color;
	int m_pdf_linestyle;
	std::string m_pdf_legend_label;
	// a map with the main pdfs for each category
	typedef std::pair<const std::string, RooAbsPdf*> main_pdfs_pair;
	std::map<std::string, RooAbsPdf*> m_main_pdfs;
	// the pdfs the sim pdf is made of
	std::vector<RooAbsPdf*> m_pdfs;
	// the variables of the pdfs
	std::vector<RooRealVar*> m_vars;
	// polyvars and other functions
	std::vector<RooAbsReal*> m_functions;
	// a vector of other FF_PDFs, used for combinations of pdfs
	std::vector<shared_ptr<FF_PDF> > m_comb_pdfs;
};

#endif /* FF_PDF_H_ */
