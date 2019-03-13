/**
 * @file	FitFramework.cc
 * @date	May 9, 2011
 * @author	petric
 * @brief	brief description
 *
 * long description
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

// FitFramework
#include "FitFramework.h"
#include "FF_Opt.h"
#include "FF_Util.h"
#include "FF_Fit.h"
#include "FF_Test.h"
// Utilities
#include "mp_utility.h"

int main(int argc, char *argv[]) {
	// ---------------------------------------------
	// ----- Set global ROOT style
	// ---------------------------------------------
	//SetRootStyle();
	SetRootStyleLarge();
    
    
	// ---------------------------------------------
	// ----- Set global RooFit integrator
	// ---------------------------------------------
    RooAbsReal::defaultIntegratorConfig()->method1D().setLabel("RooGaussKronrodIntegrator1D");//RooMCIntegrator
//    RooAbsReal::defaultIntegratorConfig()->getConfigSection("RooAdaptiveGaussKronrodIntegrator1D").setRealValue("maxSeg",300) ;//100
//    RooAbsReal::defaultIntegratorConfig()->getConfigSection("RooAdaptiveGaussKronrodIntegrator1D").setCatIndex("method",6);
    // http://mathworld.wolfram.com/Gauss-KronrodQuadrature.html
    
//    RooAbsReal::defaultIntegratorConfig()->setEpsAbs(1e-3);
//    RooAbsReal::defaultIntegratorConfig()->setEpsRel(1e-3);
    
	// ---------------------------------------------
	// ----- Options
	// ---------------------------------------------
	std::cout << "STATUS: Parse options..." << std::endl;
	shared_ptr<FFOpt> opt = make_shared<FFOpt>();
	if(!(opt->ParseOptions(argc,argv))) {
		std::exit(EXIT_FAILURE);
	} else {
		opt->PrintOptions();
	}

	// initialize the default random number generators
	gRandom->SetSeed(opt->GetRandomSeed());
	RooRandom::randomGenerator()->SetSeed(opt->GetRandomSeed());

	mp::execute_command(mp::to_string("mkdir -p ")+opt->GetOutdir());

	if(opt->GetTesting()) {
		// ---------------------------------------------
		// ----- Testing PDFs
		// ---------------------------------------------
		mp::execute_command(mp::to_string("mkdir -p testing"));
		FFTest::TestingPHSPProjection();
		FFTest::TestingResonancesPDF();
//		FFTest::TestingKKModels();
//		FFTest::TestingAngularComputation();
//		FFTest::TestingSphericHarmonic();
//		FFTest::ScanResonanceAmplitudes();
//		FFTest::TestingHelicityAnglesPDF(0,0); // S -> SS
//		FFTest::TestingHelicityAnglesPDF(0,1); // S -> SV
//		FFTest::TestingHelicityAnglesPDF(0,2); // S -> ST
//		FFTest::TestingHelicityAnglesPDF(1,0); // S -> VS
//		FFTest::TestingHelicityAnglesPDF(1,1); // S -> VV
//		FFTest::TestingHelicityAnglesPDF(1,2); // S -> VT
//		FFTest::TestingHelicityAnglesPDF(1,3); // S -> VT3
//		FFTest::TestingHelicityAnglesPDF(1,4); // S -> VT4
//		FFTest::TestingMassHelicityAnglesPDF_CPV(true, UNIFORM, 0);
//		FFTest::TestingPhysicsParameterComputation(UNIFORM, opt->GetConfigFile());
//		FFTest::TestingMassHelicityAnglesPDF_CPV(true, CUT_TH1, 0);
//		FFTest::TestingPhysicsParameterComputation(CUT_TH1, opt->GetConfigFile());
//		FFTest::TestingMassHelicityAnglesPDF_CPV(true, PRODUCT_4D, 0);
//		FFTest::TestingPhysicsParameterComputation(PRODUCT_4D, opt->GetConfigFile());
//		FFTest::TestingMassHelicityAnglesPDF_CPV(true, HIST_4D, 0);
//		FFTest::TestingPhysicsParameterComputation(HIST_4D, opt->GetConfigFile());
	} else {
		// ---------------------------------------------
		// ----- Run the different fits
		// ---------------------------------------------

		if(opt->GetFilenamesSelectedEvents().size() > 0 && opt->GetFilenamesGeneratedEvents().size() > 0) {
			shared_ptr<TChain> chain_selected = make_shared<TChain>(opt->GetTreename().c_str());
			shared_ptr<TChain> chain_selected_friend = make_shared<TChain>(opt->GetTreenameFriend().c_str());
            
			for(unsigned int i = 0; i < opt->GetFilenamesSelectedEvents().size(); ++i) {
				chain_selected->Add(opt->GetFilenamesSelectedEvents()[i].c_str());
                chain_selected_friend->Add(opt->GetFilenamesSelectedEvents()[i].c_str());
			}
			shared_ptr<TChain> chain_generated = make_shared<TChain>(opt->GetTreename().c_str());
//			shared_ptr<TChain> chain_generated_friend = make_shared<TChain>(opt->GetTreenameFriend().c_str());
			for(unsigned int i = 0; i < opt->GetFilenamesGeneratedEvents().size(); ++i) {
				chain_generated->Add(opt->GetFilenamesGeneratedEvents()[i].c_str());
//				chain_generated_friend->Add(opt->GetFilenamesGeneratedEvents()[i].c_str());
			}
            
//            chain_generated->AddFriend(chain_generated_friend.get());
            chain_selected->AddFriend(chain_selected_friend.get());
            
//			FFFits::FitEfficiencyResolution(chain_selected, chain_generated, opt);
			FFFits::FitEfficiency(chain_selected, chain_generated, opt);
            
		}

		if(opt->GetFilenamesContinuumMC().size() > 0) {
			shared_ptr<TChain> chain_continuum = make_shared<TChain>(opt->GetTreename().c_str());
			shared_ptr<TChain> chain_continuum_friend = make_shared<TChain>(opt->GetTreenameFriend().c_str());
			for(unsigned int i = 0; i < opt->GetFilenamesContinuumMC().size(); ++i) {
				chain_continuum->Add(opt->GetFilenamesContinuumMC()[i].c_str());
				chain_continuum_friend->Add(opt->GetFilenamesContinuumMC()[i].c_str());
			}
			//chain_continuum->AddFriend(chain_continuum_friend.get());
			//FFFits::FitContinuumBackgroundCorrelations(chain_continuum, opt);
			//FFFits::FitContinuumBackground(chain_continuum, opt);
			// --- WORKAROUND CODE ---
			boost::filesystem::path workaround_filename = "/dev/shm/workaround-"+FFUtil::FilenameHash(opt->GetFilenamesContinuumMC())+".root";
			if(!boost::filesystem::exists(workaround_filename)) {
				std::cout << "INFO: Creating in-memory file: " << workaround_filename.c_str() << std::endl;
				shared_ptr<TFile> file = make_shared<TFile>(workaround_filename.c_str(),"RECREATE");
				TTree* tree_c = chain_continuum->CopyTree("","",chain_continuum->GetEntries());
				TTree* tree_cf = chain_continuum_friend->CopyTree("","",chain_continuum_friend->GetEntries());
				tree_c->Write();
				tree_cf->Write();
				file->Close();
			} else {
				std::cout << "INFO: Reusing in-memory file: " << workaround_filename.c_str() << std::endl;
			}
			shared_ptr<TChain> c_workaround = make_shared<TChain>(opt->GetTreename().c_str());
			shared_ptr<TChain> cf_workaround = make_shared<TChain>(opt->GetTreenameFriend().c_str());
			c_workaround->Add(workaround_filename.c_str());
			cf_workaround->Add(workaround_filename.c_str());
			c_workaround->AddFriend(cf_workaround.get());
			//FFFits::FitContinuumBackgroundCorrelations(c_workaround, opt);
			FFFits::FitContinuumBackground(c_workaround, opt);
			// --- WORKAROUND CODE ---
		}

		if(opt->GetFilenamesSignalMC().size() > 0) {
			shared_ptr<TChain> chain_signal_mc = make_shared<TChain>(opt->GetTreename().c_str());
			shared_ptr<TChain> chain_signal_mc_friend = make_shared<TChain>(opt->GetTreenameFriend().c_str());
			for(unsigned int i = 0; i < opt->GetFilenamesSignalMC().size(); ++i) {
				chain_signal_mc->Add(opt->GetFilenamesSignalMC()[i].c_str());
				chain_signal_mc_friend->Add(opt->GetFilenamesSignalMC()[i].c_str());
			}
			chain_signal_mc->AddFriend(chain_signal_mc_friend.get());
			//FFFits::FitSignalMCCorrelations(chain_signal_mc, opt);
			FFFits::FitSignalMC(chain_signal_mc, opt);
		}

		if(opt->GetFilenamesBBbarMC().size() > 0) {
			shared_ptr<TChain> chain_bbbar_mc = make_shared<TChain>(opt->GetTreename().c_str());
			shared_ptr<TChain> chain_bbbar_mc_friend = make_shared<TChain>(opt->GetTreenameFriend().c_str());
			for(unsigned int i = 0; i < opt->GetFilenamesBBbarMC().size(); ++i) {
				chain_bbbar_mc->Add(opt->GetFilenamesBBbarMC()[i].c_str());
				chain_bbbar_mc_friend->Add(opt->GetFilenamesBBbarMC()[i].c_str());
			}
			//chain_bbbar_mc->AddFriend(chain_bbbar_mc_friend.get());
			//FFFits::FitBBbarMC(chain_bbbar_mc, opt);
			// --- WORKAROUND CODE ---
			boost::filesystem::path workaround_filename = "/dev/shm/workaround-"+FFUtil::FilenameHash(opt->GetFilenamesBBbarMC())+".root";
			if(!boost::filesystem::exists(workaround_filename)) {
				std::cout << "INFO: Creating in-memory file: " << workaround_filename.c_str() << std::endl;
				shared_ptr<TFile> file = make_shared<TFile>(workaround_filename.c_str(),"RECREATE");
				TTree* tree_c = chain_bbbar_mc->CopyTree("","",chain_bbbar_mc->GetEntries());
				TTree* tree_cf = chain_bbbar_mc_friend->CopyTree("","",chain_bbbar_mc_friend->GetEntries());
				tree_c->Write();
				tree_cf->Write();
				file->Close();
			} else {
				std::cout << "INFO: Reusing in-memory file: " << workaround_filename.c_str() << std::endl;
			}
			shared_ptr<TChain> c_workaround = make_shared<TChain>(opt->GetTreename().c_str());
			shared_ptr<TChain> cf_workaround = make_shared<TChain>(opt->GetTreenameFriend().c_str());
			c_workaround->Add(workaround_filename.c_str());
			cf_workaround->Add(workaround_filename.c_str());
			c_workaround->AddFriend(cf_workaround.get());
			FFFits::FitBBbarMC(c_workaround, opt);
			// --- WORKAROUND CODE ---
		}

		if(opt->GetFilenamesMC().size() > 0) {
			shared_ptr<TChain> chain = make_shared<TChain>(opt->GetTreename().c_str());
			shared_ptr<TChain> chain_friend = make_shared<TChain>(opt->GetTreenameFriend().c_str());
			for(unsigned int i = 0; i < opt->GetFilenamesMC().size(); ++i) {
				chain->Add(opt->GetFilenamesMC()[i].c_str());
				chain_friend->Add(opt->GetFilenamesMC()[i].c_str());
			}
			//chain->AddFriend(chain_friend.get());
			//FFFits::FitMC(chain, opt);
			// --- WORKAROUND CODE ---
			boost::filesystem::path workaround_filename = "/dev/shm/workaround-"+FFUtil::FilenameHash(opt->GetFilenamesMC())+".root";
			if(!boost::filesystem::exists(workaround_filename)) {
				std::cout << "INFO: Creating in-memory file: " << workaround_filename.c_str() << std::endl;
				shared_ptr<TFile> file = make_shared<TFile>(workaround_filename.c_str(),"RECREATE");
				TTree* tree_c = chain->CopyTree("","",chain->GetEntries());
				TTree* tree_cf = chain_friend->CopyTree("","",chain_friend->GetEntries());
				tree_c->Write();
				tree_cf->Write();
				file->Close();
			} else {
				std::cout << "INFO: Reusing in-memory file: " << workaround_filename.c_str() << std::endl;
			}
			shared_ptr<TChain> c_workaround = make_shared<TChain>(opt->GetTreename().c_str());
			shared_ptr<TChain> cf_workaround = make_shared<TChain>(opt->GetTreenameFriend().c_str());
			c_workaround->Add(workaround_filename.c_str());
			cf_workaround->Add(workaround_filename.c_str());
			c_workaround->AddFriend(cf_workaround.get());
			//FFFits::FitMC_4D(c_workaround, opt);
			//FFFits::FitMC_9D(c_workaround, opt);
			FFFits::FitMC_C_and_N(c_workaround, opt);
			// --- WORKAROUND CODE ---
		}

		if(opt->GetFilenamesData().size() > 0) {
			shared_ptr<TChain> chain = make_shared<TChain>(opt->GetTreename().c_str());
			shared_ptr<TChain> chain_friend = make_shared<TChain>(opt->GetTreenameFriend().c_str());
			for(unsigned int i = 0; i < opt->GetFilenamesData().size(); ++i) {
				chain->Add(opt->GetFilenamesData()[i].c_str());
				chain_friend->Add(opt->GetFilenamesData()[i].c_str());
			}
			chain->AddFriend(chain_friend.get());
			//FFFits::FitDataSideband(chain, opt);
			if(opt->GetUnblind()) {
				if(opt->GetCreateProfileLL()) {
					//FFFits::FitDataWithLikelihoodScan(chain, opt);
					FFFits::FitDataWithLikelihoodScan_CleanVersion(chain, opt);
				} else {
					if(opt->GetConfigFile() == "")
						FFFits::FitData(chain, opt);
					else {
						//FFFits::FitDataWithStartingValues(chain, opt, 0); // default case
						//FFFits::FitDataWithStartingValues(chain, opt, 1); // floated KK
						FFFits::FitDataWithStartingValues(chain, opt, 2); // Flatte KK
						//FFFits::FitDataWithStartingValues(chain, opt, 3); // Flatte + non-res. KK
						//FFFits::FitDataWithStartingValues(chain, opt, 5); // KK S/P-wave interference term (FANCY BUT WORKING)
						//FFFits::FitDataWithStartingValues(chain, opt, 6); // Flatte KK + PhysPhaseBase
						//FFFits::FitSystematics_Efficiency(chain, opt);
						//FFFits::FitSystematics_PDF(chain, opt);
						//FFFits::FitSystematics_FixedCPV(chain, opt);
					}
				}
			}
		}

		if(opt->GetNToys() >= 0) {
			FFFits::FitToyMC_4D(opt);
			FFFits::FitToyMC_5D_angles(opt);
			FFFits::FitToyMC_9D(opt);
			FFFits::FitToyMC_9D_random_start(opt);
			//FFFits::FitSystematics_Resolution(opt);
			//FFFits::FitSystematics_CoherentKK(opt);

			if(opt->GetFilenamesSCF().size() > 0) {
				shared_ptr<TChain> chain = make_shared<TChain>(opt->GetTreename().c_str());
				shared_ptr<TChain> chain_friend = make_shared<TChain>(opt->GetTreenameFriend().c_str());
				for(unsigned int i = 0; i < opt->GetFilenamesSCF().size(); ++i) {
					chain->Add(opt->GetFilenamesSCF()[i].c_str());
					chain_friend->Add(opt->GetFilenamesSCF()[i].c_str());
				}
				chain->AddFriend(chain_friend.get());
				FFFits::FitSystematics_SCF(chain, opt);
			}
		}

		if(opt->GetNGsimToys() >= 0 && opt->GetFilenamesGsim().size() > 0) {
			shared_ptr<TChain> chain = make_shared<TChain>(opt->GetTreename().c_str());
			shared_ptr<TChain> chain_friend = make_shared<TChain>(opt->GetTreenameFriend().c_str());
			for(unsigned int i = 0; i < opt->GetFilenamesGsim().size(); ++i) {
				std::cout << "Adding file " << opt->GetFilenamesGsim()[i] << std::endl;
				chain->Add(opt->GetFilenamesGsim()[i].c_str());
				chain_friend->Add(opt->GetFilenamesGsim()[i].c_str());
			}
			//chain->AddFriend(chain_friend.get());
			//FFFits::FitGSIMToyMC_4D(chain, opt);

			// --- WORKAROUND CODE ---
			boost::filesystem::path workaround_filename = "/dev/shm/workaround-"+FFUtil::FilenameHash(opt->GetFilenamesGsim())+".root";
			if(!boost::filesystem::exists(workaround_filename)) {
				std::cout << "INFO: Creating in-memory file: " << workaround_filename.c_str() << std::endl;
				shared_ptr<TFile> file = make_shared<TFile>(workaround_filename.c_str(),"RECREATE");
				TTree* tree_c = chain->CopyTree("","",chain->GetEntries());
				TTree* tree_cf = chain_friend->CopyTree("","",chain_friend->GetEntries());
				tree_c->Write();
				tree_cf->Write();
				file->Close();
			} else {
				std::cout << "INFO: Reusing in-memory file: " << workaround_filename.c_str() << std::endl;
			}
			shared_ptr<TChain> c_workaround = make_shared<TChain>(opt->GetTreename().c_str());
			shared_ptr<TChain> cf_workaround = make_shared<TChain>(opt->GetTreenameFriend().c_str());
			c_workaround->Add(workaround_filename.c_str());
			cf_workaround->Add(workaround_filename.c_str());
			c_workaround->AddFriend(cf_workaround.get());
			// --- WORKAROUND CODE ---

			shared_ptr<TChain> chain_reweight = make_shared<TChain>(opt->GetTreename().c_str());
			shared_ptr<TChain> chain_reweight_friend = make_shared<TChain>(opt->GetTreenameFriend().c_str());
			for(unsigned int i = 0; i < opt->GetFilenamesGsimReweight().size(); ++i) {
				std::cout << "Adding file " << opt->GetFilenamesGsimReweight()[i] << std::endl;
				chain_reweight->Add(opt->GetFilenamesGsimReweight()[i].c_str());
				chain_reweight_friend->Add(opt->GetFilenamesGsimReweight()[i].c_str());
			}
			//chain_reweight->AddFriend(chain_reweight_friend.get());

			// --- WORKAROUND CODE ---
			boost::filesystem::path workaround_filename_reweight = "/dev/shm/workaround-"+FFUtil::FilenameHash(opt->GetFilenamesGsimReweight())+".root";
			if(!boost::filesystem::exists(workaround_filename_reweight)) {
				std::cout << "INFO: Creating in-memory file: " << workaround_filename_reweight.c_str() << std::endl;
				shared_ptr<TFile> file = make_shared<TFile>(workaround_filename_reweight.c_str(),"RECREATE");
				TTree* tree_c = chain_reweight->CopyTree("","",chain_reweight->GetEntries());
				TTree* tree_cf = chain_reweight_friend->CopyTree("","",chain_reweight_friend->GetEntries());
				tree_c->Write();
				tree_cf->Write();
				file->Close();
			} else {
				std::cout << "INFO: Reusing in-memory file: " << workaround_filename_reweight.c_str() << std::endl;
			}
			shared_ptr<TChain> cr_workaround = make_shared<TChain>(opt->GetTreename().c_str());
			shared_ptr<TChain> crf_workaround = make_shared<TChain>(opt->GetTreenameFriend().c_str());
			cr_workaround->Add(workaround_filename_reweight.c_str());
			crf_workaround->Add(workaround_filename_reweight.c_str());
			cr_workaround->AddFriend(crf_workaround.get());
			// --- WORKAROUND CODE ---

			FFFits::FitGSIMToyMC_9D(c_workaround, cr_workaround, opt);
		}

		if(opt->GetFilenamesEvtGen().size() > 0) {
			shared_ptr<TChain> chain = make_shared<TChain>(opt->GetTreename().c_str());
			//shared_ptr<TChain> chain_friend = make_shared<TChain>(opt->GetTreenameFriend().c_str());
			for(unsigned int i = 0; i < opt->GetFilenamesEvtGen().size(); ++i) {
				std::cout << "Adding file " << opt->GetFilenamesEvtGen()[i] << std::endl;
				chain->Add(opt->GetFilenamesEvtGen()[i].c_str());
				//chain_friend->Add(opt->GetFilenamesEvtGen()[i].c_str());
			}
			//chain->AddFriend(chain_friend.get());
			FFFits::FitEvtGenMC(chain, opt);
			//FFFits::FitEvtGenMC_and_Reweight(chain, opt);
		}

		if(opt->GetFilenamesControlChannelData().size() > 0 && opt->GetFilenamesControlChannelGsim().size() > 0) {
			shared_ptr<TChain> chain_data = make_shared<TChain>(opt->GetTreename().c_str());
			for(unsigned int i = 0; i < opt->GetFilenamesControlChannelData().size(); ++i) {
				std::cout << "Adding file " << opt->GetFilenamesControlChannelData()[i] << std::endl;
				chain_data->Add(opt->GetFilenamesControlChannelData()[i].c_str());
			}
			shared_ptr<TChain> chain_gsim = make_shared<TChain>(opt->GetTreename().c_str());
			for(unsigned int i = 0; i < opt->GetFilenamesControlChannelGsim().size(); ++i) {
				std::cout << "Adding file " << opt->GetFilenamesControlChannelGsim()[i] << std::endl;
				chain_gsim->Add(opt->GetFilenamesControlChannelGsim()[i].c_str());
			}
			FFFits::FitControlChannel(chain_data,chain_gsim,opt);
		}
	}

	if(opt->GetFilenamesFitResultsToPlot().size() > 0) {
		FFUtil::CreateFitResultsComparisonPlot(opt);
	}

	std::cout << "INFO: Printing gDirectory content..." << std::endl;
	gDirectory->Print();

	return EXIT_SUCCESS;
}
