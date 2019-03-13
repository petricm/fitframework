/**
 * FF_Bootstrap.h
 *
 *  Created on: 23.03.2012
 *      Author: Michael Prim
 * Description: This class provides a utility for bootstrapping given data distributions. The input data
 *              needs to be provided from a TChain and the output data will be a RooDataSet, ready for
 *              being fitted.
 */

#ifndef FF_BOOTSTRAP_H_
#define FF_BOOTSTRAP_H_

// ROOT
#include "TChain.h"
#include "TTree.h"
#include "TLeaf.h"
// RooFit
#include "RooArgSet.h"
#include "RooRealVar.h"
#include "RooCategory.h"
#include "RooDataSet.h"
#include "RooRandom.h"
// std library
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <numeric>

class FF_Bootstrap {
public:
	~FF_Bootstrap() { }

	/**
	 * Constructor for FF_Bootstrap class, you need to provide a input data chain. The parameter_cut map has to be
	 * a map that contains as key a paramater e.g. "signal_yield" and a cut string to reduce the input chain to
	 * contain only such elements. (e.g. "is_signal>0").
	 *
	 * @param chain (see text)
	 * @param parameter_cut (see text)
	 * @param varset RooArgSet of all variables in the chain that you want to generate and that are needed by the cut
	 * @param weight_label Label of the variable that is used as weight
	 * @param label Label for this bootstrapper instance
	 */
	FF_Bootstrap(TChain* chain, const std::map<std::string, std::string>& parameter_cut,
			const RooArgSet& varset, const std::string& weight_label, const std::string& label)
	: m_varset(varset), m_weight_label(weight_label), m_label(label) {
		std::cout << "Entries for input chain " << chain->GetEntries() << std::endl;
		chain->SetBranchStatus("*",0);
		RooArgList varlist(m_varset);
		varlist.Print();
		float dummy_vars[varlist.getSize()];
		float chain_vars[varlist.getSize()];
		int dummy_cats[varlist.getSize()];
		int chain_cats[varlist.getSize()];
		TTree* dummy = chain->CopyTree("","",0);
		for(Int_t i = 0; i < varlist.getSize(); ++i) {
			//std::cout << "Creating branch for " << varlist[i].GetName() << std::endl;
			chain->SetBranchStatus(varlist[i].GetName(),1);
			std::string t = varlist[i].GetName();
			if(m_varset.getCatIndex(t.c_str(),-999) != -999) {
				dummy->Branch(varlist[i].GetName(),&dummy_cats[i],(t+"/I").c_str());
				chain->SetBranchAddress(varlist[i].GetName(),&chain_cats[i]);
			} else {
				dummy->Branch(varlist[i].GetName(),&dummy_vars[i],(t+"/F").c_str());
				chain->SetBranchAddress(varlist[i].GetName(),&chain_vars[i]);
			}
		}
		// reduce the input chain data to the variable limits and store result in the dummy tree
		for(Long64_t i = 0; i < chain->GetEntries(); ++i) {
			chain->GetEntry(i);
			bool fill_event = true;
			for(Int_t v = 0; v < varlist.getSize(); ++v) {
				std::string t = varlist[v].GetName();
				if(m_varset.getCatIndex(t.c_str(),-999) == -999) {
					if(chain_vars[v] < dynamic_cast<RooRealVar*>(&varlist[v])->getMin() ||
							chain_vars[v] > dynamic_cast<RooRealVar*>(&varlist[v])->getMax()) {
						fill_event = false;
						break;
					}
				}
				dummy_vars[v] = chain_vars[v];
				dummy_cats[v] = chain_cats[v];
			}
			if(fill_event)
				dummy->Fill();
			else
				continue;
		}
		// for each parameter, make an internal in memory copy with the given distribution
		typedef std::pair<const std::string, std::string> parameter_cut_pair;
		for(const parameter_cut_pair& p : parameter_cut) {
			TTree* tree = dummy->CopyTree(p.second.c_str());
			m_data_size[p.first] = tree->GetEntries();

			// resize internal storage
			for(int i = 0; i < varlist.getSize(); ++i) {
				std::string t = varlist[i].GetName();
				if(m_varset.getCatIndex(t.c_str(),-999) != -999) {
					m_data_categories[p.first][t].resize(m_data_size[p.first],0);
				} else {
					m_data_realvar[p.first][t].resize(m_data_size[p.first],0.0);
				}
			}
			// fill data from cutted tree to internal storage
			for(unsigned int e = 0; e < m_data_size[p.first]; ++e) {
				tree->GetEntry(e);

				for(int i = 0; i < varlist.getSize(); ++i) {
					std::string t = varlist[i].GetName();
					if(m_varset.getCatIndex(t.c_str(),-999) != -999) {
						m_data_categories[p.first][t].at(e) = (int)tree->GetBranch(t.c_str())->GetLeaf(t.c_str())->GetValue();
					} else {
						m_data_realvar[p.first][t].at(e) = (float)tree->GetBranch(t.c_str())->GetLeaf(t.c_str())->GetValue();
					}
				}
			}
			float sumw = std::accumulate(m_data_realvar[p.first][m_weight_label].begin(),m_data_realvar[p.first][m_weight_label].end(),0.0);
			m_data_sumw[p.first] = sumw;
			std::cout << "Entries for " << p.first << " in " << m_label << " are " << m_data_size[p.first] << " with SumW " << m_data_sumw[p.first] << std::endl;
			delete tree;
		}
		delete dummy;
	}

	/**
	 * Request bootstrapped sample from this FF_Bootstrap instance. The map specifies for each
	 * parameter a certain yield (e.g. "signal_yield" and 1000), which is used to select an
	 * according number of events, using Poisson statistics (i.e. random Poisson(1000)).
	 *
	 * @param parameter_nevents (see text)
	 * @param sample Identifier for this request (usally a increasing number 0-->NToys)
	 * @return RooDataSet with bootstrapped events (caller takes ownership)
	 */
	RooDataSet* bootstrap(std::map<std::string, unsigned int> parameter_nevents, int sample) {
		std::cout << "INFO: Bootstrapping sample " << sample << " in " << m_label << std::endl;

		RooArgList varlist(m_varset);
		varlist.Print();
		float dummy_vars[varlist.getSize()];
		int dummy_cats[varlist.getSize()];
		TTree* dummy = new TTree("dummy","dummy");
		for(Int_t i = 0; i < varlist.getSize(); ++i) {
			//std::cout << "Creating branch for " << varlist[i].GetName() << std::endl;
			std::string t = varlist[i].GetName();
			if(m_varset.getCatIndex(t.c_str(),-999) != -999) {
				dummy->Branch(varlist[i].GetName(),&dummy_cats[i],(t+"/I").c_str());
			} else {
				dummy->Branch(varlist[i].GetName(),&dummy_vars[i],(t+"/F").c_str());
			}
		}
		float sumw = 0.0;
		typedef std::pair<const std::string, unsigned int> pair_string_uint;
		for(const pair_string_uint& p : parameter_nevents) {
			if(m_data_size.count(p.first) == 0) {
				std::cout << p.first << " was not found in " << m_label << " bootstrapper" << std::endl;
				continue;
			}
			if(m_data_size[p.first] == 0) {
				std::cout << p.first << " has 0 entries in " << m_label << " bootstrapper" << std::endl;
				continue;
			}

			// get number of events for this experiment
			int nevents_sample = RooRandom::randomGenerator()->Poisson(p.second);
			// initalize counter for each event
			std::vector<unsigned int> events;
			events.resize(m_data_size[p.first], 0);
			// determine the events, that will be used for this sample
			for(int i = 0; i < nevents_sample; ++i) {
				++events[RooRandom::randomGenerator()->Integer(m_data_size[p.first])];
			}

			// add all events to the dummy tree
			float scale = m_data_sumw[p.first]/m_data_size[p.first];
			float sumw_this = 0.0;
			int events_added = 0;
			for(unsigned int e = 0; e < m_data_size[p.first]; ++e) {
				if(events[e] > 0) {
					for(Int_t i = 0; i < varlist.getSize(); ++i) {
						std::string t = varlist[i].GetName();
						if(m_varset.getCatIndex(t.c_str(),-999) != -999) {
							dummy_cats[i] = m_data_categories[p.first][t][e];
						} else if (t == m_weight_label) {
							dummy_vars[i] = (m_data_realvar[p.first][t][e]/scale)*events[e];
							sumw += dummy_vars[i];
							sumw_this += dummy_vars[i];
						} else {
							dummy_vars[i] = m_data_realvar[p.first][t][e];
						}
					}
					dummy->Fill();
					++events_added;
				} else {
					continue;
				}
			}
			std::cout << p.first << " has requested " << p.second << " events and " << nevents_sample << " of ";
			std::cout << m_data_size[p.first] << " were added with Entries(" << events_added << ") and SumW(" << sumw_this << ") in " << m_label << std::endl;

		}
		std::cout << "In total " << dummy->GetEntries() << " entries with SumW(" << sumw << ") are in sample" << std::endl;

		std::stringstream ss_data;
		ss_data << "data_" << m_label << "_" << sample;
		RooDataSet* data = new RooDataSet(ss_data.str().c_str(),ss_data.str().c_str(),dummy,m_varset,"mbc < 5.289","weight");
		delete dummy;
		data->Print();
		return data;
	}


private:
	RooArgSet m_varset;
	const std::string m_weight_label;
	const std::string m_label;

	std::map<std::string, unsigned int> m_data_size;
	std::map<std::string, float> m_data_sumw;
	std::map<std::string, std::map<std::string, std::vector<float> > > m_data_realvar;
	std::map<std::string, std::map<std::string, std::vector<int> > > m_data_categories;
};


#endif /* FF_BOOTSTRAP_H_ */
