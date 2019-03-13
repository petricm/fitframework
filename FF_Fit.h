/**
 * @file	FFFits.h
 * @date	May 9, 2011
 * @author	petric
 * @brief	brief description
 *
 * long description
 */

#ifndef FFFITS_H_
#define FFFITS_H_

#include "FitFramework.h"
#include "FF_Opt.h"
#include "FF_PDF.h"
#include "FF_PDF_data.h"
#include "FF_PDF_signal.h"
#include "FF_PDF_bkg_continuum.h"
#include "FF_PDF_bkg_bbar.h"

namespace FFFits {

void FitMC_4D(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt);

void FitMC_9D(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt);

void FitMC_C_and_N(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt);

void FitData(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt);

void FitDataWithStartingValues(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt, int data_pdf_mode = 0);

void FitDataWithLikelihoodScan(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt);

void FitDataWithLikelihoodScan_CleanVersion(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt);

void FitDataSideband(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt);

void FitContinuumBackground(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt);

void FitContinuumBackgroundCorrelations(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt);

void FitSignalMC(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt);

void FitSignalMCCorrelations(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt);

void FitBBbarMC(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt);

void FitEfficiencyResolution(shared_ptr<TChain> chain_sel, shared_ptr<TChain> chain_gen, shared_ptr<FFOpt> opt);
    
void FitEfficiency(shared_ptr<TChain> chain_sel, shared_ptr<TChain> chain_gen, shared_ptr<FFOpt> opt);

void FitToyMC_4D(shared_ptr<FFOpt> opt);

void FitToyMC_5D_angles(shared_ptr<FFOpt> opt);

void FitToyMC_9D(shared_ptr<FFOpt> opt);

void FitToyMC_9D_random_start(shared_ptr<FFOpt> opt);

void FitGSIMToyMC_4D(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt);

void FitGSIMToyMC_9D(shared_ptr<TChain> chain, shared_ptr<TChain> chain_reweight, shared_ptr<FFOpt> opt);

void FitEvtGenMC(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt);

void FitEvtGenMC_and_Reweight(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt);

void FitControlChannel(shared_ptr<TChain> chain_data, shared_ptr<TChain> chain_gsim, shared_ptr<FFOpt> opt);

void FitSystematics_Efficiency(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt);

void FitSystematics_Resolution(shared_ptr<FFOpt> opt);

void FitSystematics_PDF(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt);

void FitSystematics_SCF(shared_ptr<TChain> chain_scf, shared_ptr<FFOpt> opt);

void FitSystematics_FixedCPV(shared_ptr<TChain> chain, shared_ptr<FFOpt> opt);

void FitSystematics_CoherentKK(shared_ptr<FFOpt> opt);

}

#endif /* FFFITS_H_ */
