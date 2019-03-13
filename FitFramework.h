/**
 * @file	FitFramework.h
 * @date	May 9, 2011
 * @author	petric
 * @brief	A collection of different ROOT, RooFit, libComplexPDF and libRooComplesPDF related includes as well as important constants
 */

#ifndef FITFRAMEWORK_H_
#define FITFRAMEWORK_H_

// STL
#include <memory>
#include <array>
#include <vector>
#include <tuple>
#include <string>
#include <numeric>
using std::make_shared;
using std::shared_ptr;
// boost utilties
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/tokenizer.hpp>
// ROOT general
#include "TROOT.h"
#include "TMath.h"
#include "TRandom2.h"
#include "TUUID.h"
#include "TMatrixD.h"
#include "TDecompChol.h"
#include "TVector3.h"
// ROOT data
#include "TFile.h"
#include "TChain.h"
#include "TLeaf.h"
// ROOT plot
#include "TBox.h"
#include "TCanvas.h"
#include "TCut.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TGaxis.h"
#include "TGraphErrors.h"
#include "TLine.h"
#include "TLatex.h"
#include "TMathText.h"
#include "THStack.h"
#include "TMultiGraph.h"
#include "TGraph2D.h"
// RooFit
#include "RooAbsPdf.h"
#include "RooAddModel.h"
#include "RooAddPdf.h"
#include "RooArgusBG.h"
#include "RooBifurGauss.h"
#include "RooBreitWigner.h"
#include "RooCBShape.h"
#include "RooChebychev.h"
#include "RooConstVar.h"
#include "RooDstD0BG.h"
#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooEfficiency.h"
#include "RooEffProd.h"
#include "RooExponential.h"
#include "RooExtendPdf.h"
#include "RooFFTConvPdf.h"
#include "RooFitResult.h"
#include "RooGaussian.h"
#include "RooGenericPdf.h"
#include "RooGlobalFunc.h"
#include "RooHist.h"
#include "RooHistFunc.h"
#include "RooMCStudy.h"
#include "RooMsgService.h"
#include "RooNumIntConfig.h"
#include "RooNovosibirsk.h"
#include "RooRandom.h"
#include "RooPlot.h"
#include "RooPolynomial.h"
#include "RooPolyVar.h"
#include "RooProdPdf.h"
#include "RooRealVar.h"
#include "RooSimultaneous.h"
#include "RooStepFunction.h"
#include "RooTrace.h"
#include "RooUniform.h"
#include "RooWorkspace.h"
#include "RooNovosibirsk.h"
#include "RooNDKeysPdf.h"
// complex PDFs
#include "libComplexPDF/libComplexPDF.h"
#include "libRooComplexPDF/RooBifurStudentsTDistribution.h"
#include "libRooComplexPDF/RooBreitWignerTwoBodyPHSP.h"
#include "libRooComplexPDF/RooBreitWignerTwoBodyPHSPEvtGen.h"
#include "libRooComplexPDF/RooCategoryPdf.h"
#include "libRooComplexPDF/RooChebychevCut.h"
#include "libRooComplexPDF/RooCutPdf.h"
#include "libRooComplexPDF/RooFlatte.h"
#include "libRooComplexPDF/RooFlatteAndNonRes.h"
#include "libRooComplexPDF/RooHelicityAngles.h"
#include "libRooComplexPDF/RooKKInterference.h"
#include "libRooComplexPDF/RooKKInterferenceNorm.h"
#include "libRooComplexPDF/RooKKInterferenceNegative.h"
#include "libRooComplexPDF/RooKKInterferencePositive.h"
#include "libRooComplexPDF/RooKPiMass.h"
#include "libRooComplexPDF/RooLASS.h"
#include "libRooComplexPDF/RooMassHelicityAngles.h"
#include "libRooComplexPDF/RooMassHelicityAngles_TransformedBase.h"
#include "libRooComplexPDF/RooMassHelicityAnglesCPV.h"
#include "libRooComplexPDF/RooRelativisticSpinBreitWigner.h"
#include "libRooComplexPDF/RooStudentsTDistribution.h"
#include "libRooComplexPDF/RooThreeBodyPhaseSpaceProjection.h"
#include "libRooComplexPDF/RooTwoBodyThresholdFunction.h"
#include "libRooComplexPDF/RooVarianceGammaDistribution.h"
#include "libRooComplexPDF/RooDalitzCubicPolynomial.h"
#include "libRooComplexPDF/RooThresholdFunction.h"
#include "libRooComplexPDF/RooDalitzAnalysis.h"
#include "libRooComplexPDF/RooDalitzAnalysisCPV.h"
#include "libRooComplexPDF/RooDalitzDelaunay.h"
#include "libRooComplexPDF/RooDalitzPhaseSpaceAmplitude.h"
#include "libRooComplexPDF/RooDalitzGauss.h"
#include "libRooComplexPDF/RooDalitzRelativisticSpinBreitWigner.h"
#include "libRooComplexPDF/RooDalitzHist.h"
#include "libRooComplexPDF/RooDalitzXsd.h"
#include "libRooComplexPDF/RooDalitzNovosibirsk.h"

// CacheIntegrator
#include "CacheIntegrator/CacheIntegrator.h"
#include "CacheIntegrator/CachedIntegral.h"
#include "CacheIntegrator/CachedIntegralMC.h"
#include "CacheIntegrator/MassHelicityAnglesNormCachedIntegral.h"
#include "CacheIntegrator/MassHelicityAnglesRealCachedIntegral.h"
#include "CacheIntegrator/MassHelicityAnglesImagCachedIntegral.h"
#include "CacheIntegrator/MassHelicityAnglesNormCachedIntegral_phi_th1_th2.h"
#include "CacheIntegrator/MassHelicityAnglesRealCachedIntegral_phi_th1_th2.h"
#include "CacheIntegrator/MassHelicityAnglesImagCachedIntegral_phi_th1_th2.h"
#include "CacheIntegrator/MassHelicityAnglesNormCachedIntegral_m_th1_th2.h"
#include "CacheIntegrator/MassHelicityAnglesRealCachedIntegral_m_th1_th2.h"
#include "CacheIntegrator/MassHelicityAnglesImagCachedIntegral_m_th1_th2.h"
#include "CacheIntegrator/MassHelicityAnglesNormCachedIntegral_m_phi_th2.h"
#include "CacheIntegrator/MassHelicityAnglesRealCachedIntegral_m_phi_th2.h"
#include "CacheIntegrator/MassHelicityAnglesImagCachedIntegral_m_phi_th2.h"
#include "CacheIntegrator/MassHelicityAnglesNormCachedIntegral_m_phi_th1.h"
#include "CacheIntegrator/MassHelicityAnglesRealCachedIntegral_m_phi_th1.h"
#include "CacheIntegrator/MassHelicityAnglesImagCachedIntegral_m_phi_th1.h"
#include "CacheIntegrator/KKInterferenceNormCachedIntegral.h"
#include "CacheIntegrator/KKInterferenceRealCachedIntegral.h"
#include "CacheIntegrator/KKInterferenceImagCachedIntegral.h"
#include "CacheIntegrator/DalitzAnalysisNormCachedIntegral.h"
#include "CacheIntegrator/DalitzAnalysisRealCachedIntegral.h"
#include "CacheIntegrator/DalitzAnalysisImagCachedIntegral.h"
/*
#include "CacheIntegrator/DalitzAnalysisNormCachedIntegral_mass_AB.h"
#include "CacheIntegrator/DalitzAnalysisRealCachedIntegral_mass_AB.h"
#include "CacheIntegrator/DalitzAnalysisImagCachedIntegral_mass_AB.h"
#include "CacheIntegrator/DalitzAnalysisNormCachedIntegral_mass_BC.h"
#include "CacheIntegrator/DalitzAnalysisRealCachedIntegral_mass_BC.h"
#include "CacheIntegrator/DalitzAnalysisImagCachedIntegral_mass_BC.h"
 */

using namespace RooFit;

// Kaon mass
static const double m_K   = 0.493677;
// Pion mass
static const double m_Pi  = 0.139570;
// Kshort mass
static const double m_Ks  = 0.497648;
// Pi0 mass
static const double m_Pi0 = 0.134977;
// Phi mass
static const double m_Phi = 1.019460;
// K*(892) mass
static const double m_K892 = 0.89594;
// B0 mass
static const double m_B0  = 5.2795;

static const double mass_AB_min = (m_Pi0+m_Pi)*(m_Pi0+m_Pi);
static const double mass_AB_max = (m_B0-m_K)*(m_B0-m_K);

static const double mass_BC_min = (m_K+m_Pi)*(m_K+m_Pi);
static const double mass_BC_max = (m_B0-m_Pi0)*(m_B0-m_Pi0);

/**
 * @brief Sets some reasonable global ROOT Style settings
 */
inline void SetRootStyle() {
	gROOT->SetStyle("Plain");
	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);
	gStyle->SetLabelSize(0.04,"xy");
	gStyle->SetLabelOffset(0.006,"y");
	gStyle->SetTitleSize(0.04,"xy");
	gStyle->SetTitleOffset(1.0,"x");
	gStyle->SetTitleOffset(1.3,"y");
	gStyle->SetNdivisions(505,"x");

	gStyle->SetPadLeftMargin(0.12);
	gStyle->SetPadRightMargin(0.05);
	gStyle->SetPadBottomMargin(0.10);
	gStyle->SetPadTopMargin(0.05);

	gStyle->SetFillColor(0);
	gStyle->SetMarkerSize(0.8);
	gStyle->SetLineColor(kBlack);
	gStyle->SetLineWidth(1);

	gStyle->SetLegendBorderSize(0);
}

/**
 * @brief Sets some reasonable global ROOT Style settings with large axis labels
 */
inline void SetRootStyleLarge() {
	gROOT->SetStyle("Plain");
	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);
	gStyle->SetLabelSize(0.04,"xy");
	gStyle->SetLabelOffset(0.006,"y");
	gStyle->SetTitleSize(0.06,"xy");
	gStyle->SetTitleOffset(0.9,"x");
	gStyle->SetTitleOffset(1.15,"y");
	gStyle->SetNdivisions(505,"x");

	gStyle->SetPadLeftMargin(0.14);
	gStyle->SetPadRightMargin(0.05);
	gStyle->SetPadBottomMargin(0.15);
	gStyle->SetPadTopMargin(0.05);

	gStyle->SetFillColor(0);
	gStyle->SetMarkerSize(0.8);
	gStyle->SetLineColor(kBlack);
	gStyle->SetLineWidth(1);

	gStyle->SetLegendBorderSize(0);
}


inline void SetRootStyleLarge2D() {
	gROOT->SetStyle("Plain");
	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);
	gStyle->SetLabelSize(0.04,"xy");
	gStyle->SetLabelOffset(0.006,"y");
	gStyle->SetTitleSize(0.06,"xy");
	gStyle->SetTitleOffset(0.9,"x");
	gStyle->SetTitleOffset(1.15,"y");
	gStyle->SetNdivisions(505,"x");
    
	gStyle->SetPadLeftMargin(0.14);
	gStyle->SetPadRightMargin(0.12);
	gStyle->SetPadBottomMargin(0.15);
	gStyle->SetPadTopMargin(0.05);
    
	gStyle->SetFillColor(1);
	gStyle->SetMarkerSize(0.8);
	gStyle->SetLineColor(kBlack);
	gStyle->SetLineWidth(1);
    
	gStyle->SetLegendBorderSize(0);
    
}


#endif /* FITFRAMEWORK_H_ */
