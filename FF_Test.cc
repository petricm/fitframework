/**
 * @file	FFTest.cc
 * @date	Aug 25, 2011
 * @author	petric
 * @brief	brief description
 *
 * long description
 */

#include "FF_Test.h"
#include "FF_PDF.h"
#include "FF_PDF_bkg_continuum.h"
#include "FF_PDF_bkg_bbar.h"
#include "FF_PDF_signal.h"
#include "FF_PDF_data.h"

/*
// valgrind callgrind header and MACRO to profile only part of code
#include <valgrind/callgrind.h>

// additional use --instr-atstart=no option at startup
CALLGRIND_START_INSTRUMENTATION;
do_something();
CALLGRIND_STOP_INSTRUMENTATION;
CALLGRIND_DUMP_STATS;
*/

using namespace FFUtil;

namespace FFTest {

void PrintTimestamp() {
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	std::cout << "TIMESTAMP: Current local time and date: " << asctime(timeinfo);;
}

void TestingResonancesPDF() {
	std::cout << "INFO: Initialize PDF" << std::endl;

	RooRealVar m("m","m_{K#pi}",0.7,1.55,"GeV");
	RooRealVar mass_J0("mass_J0","mass_J0",1.435);
	RooRealVar width_J0("width_J0","width_J0",0.279);
	RooRealVar r0("r0","r0",0.0);
	RooRealVar J0("J0","J0",0.0);
	RooRelativisticSpinBreitWigner relBW0("relBW0","relBW0",m,mass_J0,width_J0,r0,J0,m_K,m_Pi);
	RooRealVar mass_J1("mass_J1","mass_J1",0.896);
	RooRealVar width_J1("width_J1","width_J1",0.0503);
	RooRealVar r1("r1","r1",3.4);
	RooRealVar J1("J1","J1",1.0);
	RooRelativisticSpinBreitWigner relBW1("relBW1","relBW1",m,mass_J1,width_J1,r1,J1,m_K,m_Pi);
	RooRealVar mass_J2("mass_J2","mass_J2",1.432);
	RooRealVar width_J2("width_J2","width_J2",0.109);
	RooRealVar r2("r2","r2",2.7);
	RooRealVar J2("J2","J2",2.0);
	RooRelativisticSpinBreitWigner relBW2("relBW2","relBW2",m,mass_J2,width_J2,r2,J2,m_K,m_Pi);

	RooRealVar a("a","a",1.95);
	RooRealVar b("b","b",1.76);
	RooLASS lass("LASS","LASS",m,mass_J0,width_J0,a,b,m_K,m_Pi);

	RooBreitWignerTwoBodyPHSP BW0("BW0","BW0",m,mass_J0,width_J0,m_K,m_Pi);
	RooBreitWignerTwoBodyPHSP BW1("BW1","BW1",m,mass_J1,width_J1,m_K,m_Pi);
	RooBreitWignerTwoBodyPHSP BW2("BW2","BW2",m,mass_J2,width_J2,m_K,m_Pi);

	RooThreeBodyPhaseSpaceProjection ThreeBody("ThreeBody","ThreeBody",m,m_K,m_Pi,m_Phi,m_B0);

	std::cout << "INFO: Generate ToyMC" << std::endl;
	RooDataSet *data_test0 = relBW0.generate(m,10000);
	RooDataSet *data_test1 = relBW1.generate(m,10000);
	RooDataSet *data_test2 = relBW2.generate(m,10000);
	RooDataSet *data_test3 = lass.generate(m,10000);

	std::cout << "INFO: Draw PDF" << std::endl;
	RooPlot *frame = m.frame(Bins(50));
	RooPlot *frame_PD = m.frame(Bins(50));
	RooPlot *frame_S = m.frame(Bins(50));
	//data_test0->plotOn(frame);
	//data_test1->plotOn(frame);
	//data_test2->plotOn(frame);
	//data_test3->plotOn(frame);
	//relBW0.plotOn(frame, Name("relBW0_curve"), LineColor(kBlue), LineStyle(kDashed), Normalization(1.0,RooAbsPdf::NumEvent));
	ThreeBody.plotOn(frame_S, Name("ThreeBody_curve_S"), LineColor(kOrange), LineStyle(kDashed), Normalization(1.0,RooAbsPdf::NumEvent));
	relBW0.plotOn(frame_S, Name("relBW0_curve_S"), LineColor(kViolet), LineStyle(kDashed), Normalization(1.0,RooAbsPdf::NumEvent));
	relBW1.plotOn(frame, Name("relBW1_curve"), LineColor(kRed), LineStyle(kDashed), Normalization(1.0,RooAbsPdf::NumEvent));
	relBW1.plotOn(frame_PD, Name("relBW1_curve_PD"), LineColor(kRed), LineStyle(kDashed), Normalization(1.0,RooAbsPdf::NumEvent));
	relBW2.plotOn(frame, Name("relBW2_curve"), LineColor(kSpring), LineStyle(kDashed), Normalization(1.0,RooAbsPdf::NumEvent));
	relBW2.plotOn(frame_PD, Name("relBW2_curve_PD"), LineColor(kSpring), LineStyle(kDashed), Normalization(1.0,RooAbsPdf::NumEvent));
	//BW0.plotOn(frame, Name("BW0_curve"), LineColor(kCyan), LineStyle(kDashDotted));
	//BW1.plotOn(frame, Name("BW1_curve"), LineColor(kOrange), LineStyle(kDashDotted));
	//BW2.plotOn(frame, Name("BW2_curve"), LineColor(kGreen+3), LineStyle(kDashDotted));
	lass.plotOn(frame, Name("LASS_curve"), LineColor(kBlue), LineStyle(kDashDotted), Normalization(1.0,RooAbsPdf::NumEvent));
	lass.plotOn(frame_S, Name("LASS_curve_S"), LineColor(kBlue), LineStyle(kDashDotted), Normalization(1.0,RooAbsPdf::NumEvent));

	TCanvas* c = new TCanvas("BW-LASS","BW-LASS",1024,768);
	frame->Draw();
	frame->GetYaxis()->SetTitle("Intensity #||{R}^{2}");
	frame->SetMaximum(0.25);

	int leg_entries = 3;
	TLegend *leg = new TLegend(0.4,0.94-0.05*leg_entries,0.94,0.94,"","NDC");
	leg->SetBorderSize(0);
	leg->SetFillColor(0);
	leg->SetFillStyle(4000);
	leg->AddEntry("relBW1_curve","relativ. BW for K^{*}_{}(892)^{0}","L");
	leg->AddEntry("relBW2_curve","relativ. BW for K^{*}_{2}(1430)^{0}","L");
	//leg->AddEntry("relBW0_curve","relativ. BW for K_{0}^{*}(1430)^{0}","L");
	leg->AddEntry("LASS_curve","LASS parametrization","L");
	leg->Draw("same");
	c->SaveAs("testing/BreitWigners_SPD.eps");
	c->Clear();
	leg->Clear();

	frame_S->Draw();
	frame_S->GetYaxis()->SetTitle("Intensity #||{R}^{2}");
	frame_S->SetMaximum(0.25);
	leg->AddEntry("relBW0_curve_S","relativ. BW for K_{0}^{*}(1430)^{0}","L");
	//leg->AddEntry("ThreeBody_curve_S","Three body phase space","L");
	leg->AddEntry("ThreeBody_curve_S","B#rightarrow#phiK#pi phase space","L");
	leg->AddEntry("LASS_curve_S","LASS parametrization","L");
	leg->Draw("same");
	c->SaveAs("testing/BreitWigners_S.eps");
	c->Clear();
	leg->Clear();

	frame_PD->Draw();
	frame_PD->GetYaxis()->SetTitle("Intensity #||{R}^{2}");
	frame_PD->SetMaximum(0.25);
	leg->AddEntry("relBW1_curve_PD","relativ. BW for K_{}^{*}(892)^{0}","L");
	leg->AddEntry("relBW2_curve_PD","relativ. BW for K_{2}^{*}(1430)^{0}","L");
	leg->AddEntry("","","");
	leg->Draw("same");
	c->SaveAs("testing/BreitWigners_PD.eps");
	c->Clear();

	std::cout << "INFO: Inv KPi Mass PDF" << std::endl;
	// yields from BaBar measurement
	const double N_phiKPi_0 = 172;
	const double N_phiK892 = 535;
	const double N_phiK1430_2 = 167;
	RooRealVar amp_0("amp_0","amp_0",N_phiKPi_0/(N_phiKPi_0+N_phiK892+N_phiK1430_2));
	RooRealVar phase_0("phase_0","phase_0",0.0);
	RooRealVar amp_1("amp_1","amp_1",N_phiK892/(N_phiKPi_0+N_phiK892+N_phiK1430_2));
	RooRealVar phase_1("phase_1","phase_1",0.0);
	RooRealVar amp_2("amp_2","amp_2",N_phiK1430_2/(N_phiKPi_0+N_phiK892+N_phiK1430_2));
	RooRealVar phase_2("phase_2","phase_2",0.0);

	RooKPiMass invmass("invmass","invmass",m,mass_J0,width_J0,a,b,mass_J1,width_J1,r1,mass_J2,width_J2,r2,
			amp_0,phase_0,
			amp_1,phase_1,
			amp_2,phase_2,
			m_K,m_Pi);
	RooDataSet *data_all = invmass.generate(m,1000);
	RooPlot *frame2 = m.frame(Bins(50));
	data_all->plotOn(frame2);
	invmass.plotOn(frame2, LineColor(kBlue), LineStyle(kDashed));

	RooAbsReal* I_tot = invmass.createIntegral(RooArgSet(m));
	double N_tot = I_tot->getVal();
	amp_1.setVal(0.0);
	amp_2.setVal(0.0);
	invmass.plotOn(frame2, LineColor(kRed), LineStyle(kDashed), Normalization(I_tot->getVal()/N_tot));
	amp_0.setVal(0.0);
	amp_1.setVal(N_phiK892/(N_phiKPi_0+N_phiK892+N_phiK1430_2));
	invmass.plotOn(frame2, LineColor(kOrange), LineStyle(kDashed), Normalization(I_tot->getVal()/N_tot));
	amp_1.setVal(0.0);
	amp_2.setVal(N_phiK1430_2/(N_phiKPi_0+N_phiK892+N_phiK1430_2));
	invmass.plotOn(frame2, LineColor(kSpring), LineStyle(kDashed), Normalization(I_tot->getVal()/N_tot));

	frame2->Draw();
	c->SaveAs("testing/InvKPiMass.eps");
	c->Clear();

	delete c;
	delete data_test0;
	delete data_test1;
	delete data_test2;
	delete data_test3;
	delete frame;
	delete frame_S;
	delete frame_PD;
	delete data_all;
	delete frame2;
}

void TestingKKModels() {
	std::cout << "INFO: Initialize PDF" << std::endl;

	RooRealVar m("m","m_{KK}",0.985,1.05,"GeV");
	RooRealVar hel_th1("hel_th1","cos #theta_{1}",-1.0,1.0,"");
	m.setRange("proj_range",0.985,1.05);
	hel_th1.setRange("proj_range",-1.0,1.0);
	m.setRange("full_range",0.985,m_B0-m_K892);
	RooRealVar amp_flatte("amp_flatte","amp_flatte",1.0);
	RooRealVar phase_flatte("phase_flatte","phase_flatte",0.0);
	RooRealVar f0_mass_R("chil0_ma_bkg_bbar_wrongphif0980_pole","chil0_ma bkg_bbar_wrongphif0980 pole",0.965,0.955,0.975);
	f0_mass_R.setConstant(true);
	RooRealVar f0_g_1("chil0_ma_bkg_bbar_wrongphif0980_g1","chil0_ma bkg_bbar_wrongphif0980 g1",0.165);
	RooRealVar f0_g_2("chil0_ma_bkg_bbar_wrongphif0980_g2","chil0_ma bkg_bbar_wrongphif0980 g2",0.347/std::sqrt(2.0));
	RooRealVar f0_g_3("chil0_ma_bkg_bbar_wrongphif0980_g3","chil0_ma bkg_bbar_wrongphif0980 g3",0.347/std::sqrt(2.0));
	RooRealVar amp_nonres("amp_nonres","amp_nonres",0.067,0.0,10.0);
	RooRealVar phase_nonres("phase_nonres","phase_nonres",5.305,0.0,2*TMath::Pi());

	RooRealVar a1("hel_th1_bkg_bbar_wrongphif0980_a1","hel_th1_bkg_bbar_wrongphif0980 a1",-0.5,-1.0,0.0);
	RooRealVar a2("hel_th1_bkg_bbar_wrongphif0980_a2","hel_th1_bkg_bbar_wrongphif0980 a2",0.8,0.3,1.0);
	RooChebychevCut hel_th1_dist("hel_th1_dist","hel_th1_dist",hel_th1,RooArgList(a1,a2),0.75);

	RooFlatte f0_flatte_pdf_only_KK("f0_flatte_pdf_only_KK","f0_flatte_pdf_only_KK",m,f0_mass_R,f0_g_1,f0_g_2,f0_g_3,m_Pi,m_Pi,m_K,m_K,m_Ks,m_Ks,m_K,m_K);
	RooThreeBodyPhaseSpaceProjection three_body_pdf_only_KK("three_body_pdf_only_KK","three_body_pdf_only_KK",m,m_K,m_K,m_K892,m_B0);
	RooFlatteAndNonRes KK_model_only_KK("KK_model_only_KK","KK_model_only_KK",m,amp_flatte,phase_flatte,f0_mass_R,f0_g_1,f0_g_2,f0_g_3,amp_nonres,phase_nonres,
			m_Pi,m_Pi,m_K,m_K,m_Ks,m_Ks,m_K,m_K,m_K892,m_B0);

	RooProdPdf f0_flatte_pdf("f0_flatte_pdf","f0_flatte_pdf",RooArgList(f0_flatte_pdf_only_KK,hel_th1_dist));
	RooProdPdf three_body_pdf("three_body_pdf","three_body_pdf",RooArgList(three_body_pdf_only_KK,hel_th1_dist));
	RooProdPdf KK_model("KK_model","KK_model",RooArgList(KK_model_only_KK,hel_th1_dist));

	std::cout << "INFO: Generate ToyMC" << std::endl;
	RooDataSet *data_combined = KK_model.generate(RooArgSet(m,hel_th1),5000);

	RooFitResult* fit_result = KK_model.fitTo(*data_combined, Save(true));
	fit_result->Print();
	double phase_buffer = phase_nonres.getVal();
	std::stringstream ss;
	ss.precision(3);
	ss << phase_nonres.getVal() << "#pm" << phase_nonres.getError();

	TCanvas* c = new TCanvas("KK","KK",1024,768);
	c->cd();
	std::cout << "INFO: Draw PDF" << std::endl;
	RooPlot *frame = m.frame(Bins(50));
	data_combined->plotOn(frame, Name("data_plot"));
	f0_flatte_pdf.plotOn(frame, Name("f0_flatte_pdf_curve"), LineColor(kOrange), LineStyle(kSolid));
	three_body_pdf.plotOn(frame, Name("three_body_pdf_curve"), LineColor(kRed), LineStyle(kSolid));
	KK_model.plotOn(frame, Name("KK_model"), LineColor(kBlue), LineStyle(kSolid), LineStyle(kSolid));
	phase_nonres.setVal(0.0);
	KK_model.plotOn(frame, Name("KK_model_shift_0"), LineColor(kSpring), LineStyle(kDashed));
	phase_nonres.setVal(TMath::Pi()/2.0);
	KK_model.plotOn(frame, Name("KK_model_shift_pi_2"), LineColor(kViolet), LineStyle(kDashed));
	phase_nonres.setVal(TMath::Pi());
	KK_model.plotOn(frame, Name("KK_model_shift_pi"), LineColor(kCyan), LineStyle(kDashed));
	phase_nonres.setVal(3*TMath::Pi()/2.0);
	KK_model.plotOn(frame, Name("KK_model_shift_3pi_2"), LineColor(kMagenta), LineStyle(kDashed));
	phase_nonres.setVal(phase_buffer);
	frame->Draw();
	frame->SetMaximum(frame->GetMaximum()*2.0);

	int leg_entries = 8;
	TLegend *leg = new TLegend(0.3,0.94-0.05*leg_entries,0.93,0.94,"","NDC");
	leg->SetBorderSize(0);
	leg->SetFillColor(0);
	leg->SetFillStyle(4000);
	leg->AddEntry("data_plot","Data (generated as coherent #Sigma)","PE");
	leg->AddEntry("f0_flatte_pdf_curve","B #rightarrow f0(980)K*(892) Flatte shape","L");
	leg->AddEntry("three_body_pdf_curve","B #rightarrow KKK*(892) 3-body non-res. shape","L");
	leg->AddEntry("KK_model",("|Flatte + non-res.|^{2} shape fit (#delta = "+ss.str()+")").c_str(),"L");
	leg->AddEntry("KK_model_shift_0","|Flatte + non-res.|^{2} shape (#delta = 0)","L");
	leg->AddEntry("KK_model_shift_pi_2","|Flatte + non-res.|^{2} shape (#delta = #pi/2)","L");
	leg->AddEntry("KK_model_shift_pi","|Flatte + non-res.|^{2} shape (#delta = #pi)","L");
	leg->AddEntry("KK_model_shift_3pi_2","|Flatte + non-res.|^{2} shape (#delta = 3#pi/2)","L");
	leg->Draw("same");

	c->SaveAs("testing/KK_models_mass_KK.eps");
	c->SaveAs("testing/KK_models_mass_KK.root");
	c->Clear();
	leg->Clear();

	RooPlot *frame_ang = hel_th1.frame(Bins(50));
	data_combined->plotOn(frame_ang, Name("data_plot"));
	f0_flatte_pdf.plotOn(frame_ang, Name("f0_flatte_pdf_curve"), LineColor(kOrange), LineStyle(kSolid));
	three_body_pdf.plotOn(frame_ang, Name("three_body_pdf_curve"), LineColor(kRed), LineStyle(kSolid));
	KK_model.plotOn(frame_ang, Name("KK_model"), LineColor(kBlue), LineStyle(kSolid), LineStyle(kSolid));
	phase_nonres.setVal(0.0);
	KK_model.plotOn(frame_ang, Name("KK_model_shift_0"), LineColor(kSpring), LineStyle(kDashed));
	phase_nonres.setVal(TMath::Pi()/2.0);
	KK_model.plotOn(frame_ang, Name("KK_model_shift_pi_2"), LineColor(kViolet), LineStyle(kDashed));
	phase_nonres.setVal(TMath::Pi());
	KK_model.plotOn(frame_ang, Name("KK_model_shift_pi"), LineColor(kCyan), LineStyle(kDashed));
	phase_nonres.setVal(3*TMath::Pi()/2.0);
	KK_model.plotOn(frame_ang, Name("KK_model_shift_3pi_2"), LineColor(kMagenta), LineStyle(kDashed));
	phase_nonres.setVal(phase_buffer);
	frame_ang->Draw();

	c->SaveAs("testing/KK_models_hel_th1.eps");
	c->SaveAs("testing/KK_models_hel_th1.root");
	c->Clear();

	double buffer = amp_nonres.getVal();
	RooAbsReal* I_tot = KK_model.createIntegral(RooArgSet(m),"proj_range");
	double N_tot = I_tot->getVal();
	amp_flatte.setVal(0.0);
	double N_nonres = I_tot->getVal();
	amp_flatte.setVal(1.0);
	amp_nonres.setVal(0.0);
	double N_flatte = I_tot->getVal();
	amp_nonres.setVal(buffer);
	delete I_tot;

	std::cout << "*** Fit fraction in projection range\n";
	std::cout << "Fit fraction of Flatte:   " << N_flatte/N_tot << "\n";
	std::cout << "Fit fraction of non-res.: " << N_nonres/N_tot << "\n";

	I_tot = KK_model.createIntegral(RooArgSet(m),"full_range");
	N_tot = I_tot->getVal();
	amp_flatte.setVal(0.0);
	N_nonres = I_tot->getVal();
	amp_flatte.setVal(1.0);
	amp_nonres.setVal(0.0);
	N_flatte = I_tot->getVal();
	amp_nonres.setVal(buffer);

	std::cout << "*** Fit fraction in full phase space range\n";
	std::cout << "Fit fraction of Flatte:   " << N_flatte/N_tot << "\n";
	std::cout << "Fit fraction of non-res.: " << N_nonres/N_tot << "\n";

	delete c;
	delete data_combined;
	delete frame;
	delete fit_result;
}

double rad_to_deg(double rad) {
	return rad/(M_PI/180);
}

void TestingAngularComputation() {
	/*
	 * code from reconstruction module
	 *
	// boost to (Kpi)^0-System
	p_k.boost(-p_kpi.boostVector());
	// calc theta_1 between K and (Kpi)^0
	set_helicity_angles(1,(float)std::cos(p_kpi.vect().angle(p_k.vect())));

	// boost to Phi-System
	p_phi_kp.boost(-p_phi.boostVector());
	// calc theta_2 between K+ and Phi
	set_helicity_angles(2,(float)std::cos(p_phi.vect().angle(p_phi_kp.vect())));

	// intersecting line between both planes
	Hep3Vector cut = p_phi.vect();
	// calc phi between both planes
	set_helicity_angles(0,(float)std::atan2(
			cut.mag()*p_k.vect().dot(cut.cross(p_phi_kp.vect())),
			(p_k.vect().cross(cut)).dot(cut.cross(p_phi_kp.vect()))
	));
	*/

	TVector3 p_phi(1,0,0);
	TVector3 p_k(0,-1,0);
	TVector3 p_phi_kp(0,-1,1);

	std::cout << "p_phi:    ";
	p_phi.Print();
	std::cout << "p_k:      ";
	p_k.Print();
	std::cout << "p_phi_kp: ";
	p_phi_kp.Print();
	std::cout << "\n";

	double cos_th1 = std::cos(-p_phi.Angle(p_k));
	double cos_th2 = std::cos(p_phi.Angle(p_phi_kp));
	double phi = M_PI-std::atan2(p_phi.Mag()*(p_k.Dot(p_phi.Cross(p_phi_kp))), (p_k.Cross(p_phi)).Dot(p_phi.Cross(p_phi_kp)));

	std::cout << "cos(Theta_1): " << cos_th1 << "  " << rad_to_deg(std::acos(cos_th1)) << "°\n";
	std::cout << "cos(Theta_2): " << cos_th2 << "  " << rad_to_deg(std::acos(cos_th2)) << "°\n";
	std::cout << "         Phi: " << phi << "  " << rad_to_deg(phi) << "°\n";

	std::cout << "\n";
	std::cout << "1/4 * PI = " << M_PI/4.0 << "\n";
	std::cout << "1/2 * PI = " << M_PI/2.0 << "\n";
	std::cout << "3/4 * PI = " << M_PI*(3.0/4.0) << "\n";
	std::cout << "      PI = " << M_PI << "\n";

	std::cout << "\n";
	std::cout << "cos(1/4 * PI) = " << std::cos(M_PI/4.0) << "\n";
	std::cout << "cos(1/2 * PI) = " << std::cos(M_PI/2.0) << "\n";
	std::cout << "cos(3/4 * PI) = " << std::cos(M_PI*(3.0/4.0)) << "\n";
	std::cout << "cos(      PI) = " << std::cos(M_PI) << "\n";
}

void TestingSphericHarmonic() {
	std::vector<double> phi_ = {{-3,-2,-1,0,1,2,3}};
	std::vector<double> th1_ = {{0,1,2,3}};
	std::vector<double> th2_ = {{0,1,2,3}};

	double pi = M_PI;

	std::array<unsigned int, 3> J1_ = {{0,1,2}};
	unsigned int J2 = 0;
	std::array<int, 3> lambda_ = {{-1,0,1}};

	for(auto J1 : J1_) {
		std::cout << "**** Checking Spin " << J1 << "\n";
		for(auto lambda : lambda_) {
			std::cout << "** Checking Lambda " << lambda << "\n";

			double phi = 1;
			double th1 = 1;
			double th2 = 1;

			std::cout << "* new definition" << "\n";
			std::complex<double> Y_J1 = ComplexPDF::SphericalHarmonic(J1,lambda,th1,(pi-phi));
			std::complex<double> Y_J2 = ComplexPDF::SphericalHarmonic(J2,-lambda,-th2,0);
			std::cout << "Y_J1: " << Y_J1 << "\n";
			std::cout << "Y_J2: " << Y_J2 << "\n";
			std::cout << "Prod: " << Y_J1*Y_J2 << "\n";
			std::cout << "* old definition" << "\n";
			std::complex<double> Y_J1_ = ComplexPDF::SphericalHarmonic(J1,-lambda,pi-th1,-phi);
			std::complex<double> Y_J2_ = ComplexPDF::SphericalHarmonic(J2,lambda,th2,0);
			std::cout << "Y_J1: " << Y_J1_ << "\n";
			std::cout << "Y_J2: " << Y_J2_ << "\n";
			std::cout << "Prod: " << Y_J1_*Y_J2_ << "\n";
		}
	}
}

void TestingHelicityAnglesPDF(int j1, int j2) {
	int M[3] = {0,1,1};
	int Z[3] = {1,0,1};
	int P[3] = {0,1,1};

	for(int p = 0; p < 3; ++p) {
		std::cout << "INFO: Initialize PDF" << std::endl;
		RooRealVar hel_phi("hel_phi","#Phi",-TMath::Pi(),TMath::Pi(),"rad");
		RooRealVar hel_cos_th1("hel_cos_th1","cos #Theta_{1}",-1.0,1.0,"");
		RooRealVar hel_cos_th2("hel_cos_th2","cos #Theta_{2}",-1.0,1.0,"");
		RooRealVar J1("J1","J1",j1);
		RooRealVar J2("J2","J2",j2);
		RooRealVar amp_m("amp_m","amp_m",M[p]);
		RooRealVar phase_m("phase_m","phase_m",0.0);
		RooRealVar amp_0("amp_0","amp_0",Z[p]);
		RooRealVar phase_0("phase_0","phase_0",0.0);
		RooRealVar amp_p("amp_p","amp_p",P[p]);
		RooRealVar phase_p("phase_p","phase_p",0.0);
		RooHelicityAngles angles("angles","angles",hel_phi,hel_cos_th1,hel_cos_th2,J1,J2,amp_m,phase_m,amp_0,phase_0,amp_p,phase_p);

		std::cout << "INFO: Generate ToyMC" << std::endl;
		RooDataSet *data_test0 = angles.generate(RooArgSet(hel_phi,hel_cos_th1,hel_cos_th2),10000);

		std::cout << "INFO: Draw Angular PDF" << std::endl;
		RooPlot *frame_hel_phi = hel_phi.frame(Bins(50));
		data_test0->plotOn(frame_hel_phi);
		angles.plotOn(frame_hel_phi, LineColor(kBlue), LineStyle(kDashed));

		RooPlot *frame_hel_cos_th1 = hel_cos_th1.frame(Bins(50));
		data_test0->plotOn(frame_hel_cos_th1);
		angles.plotOn(frame_hel_cos_th1, LineColor(kBlue), LineStyle(kDashed));

		RooPlot *frame_hel_cos_th2 = hel_cos_th2.frame(Bins(50));
		data_test0->plotOn(frame_hel_cos_th2);
		angles.plotOn(frame_hel_cos_th2, LineColor(kBlue), LineStyle(kDashed));

		std::string filename = "testing/HelicityAngles_" + mp::to_string(j1) + "_" + mp::to_string(j2) + "___" + mp::to_string(M[p]) + mp::to_string(Z[p]) + mp::to_string(P[p]);
		TCanvas* c = new TCanvas(filename.c_str(),filename.c_str(),3*1024,768);
		c->Divide(3,1);
		c->cd(1);
		frame_hel_phi->Draw();
		frame_hel_phi->GetYaxis()->SetTitle("Events / ( #pi/25 rad )");
		c->cd(2);
		frame_hel_cos_th1->Draw();
		c->cd(3);
		frame_hel_cos_th2->Draw();
		filename += ".eps";
		c->SaveAs(filename.c_str());
		c->Clear();

		delete c;
		delete data_test0;
		delete frame_hel_cos_th1;
		delete frame_hel_cos_th2;
		delete frame_hel_phi;

		// for spin 0, only long. polarization makes sense, skip other
		if(std::min(j1,j2) == 0) {
			break;
		}
	}
}

void ScanResonanceAmplitudes() {
	static const double m_min = m_K + m_Pi;
	static const double m_max = 1.65;

	static const double mass_J0 = 1.435;
	static const double width_J0 = 0.279;
	static const double r0 = 0.0;

	static const double mass_J1 = 0.896;
	static const double width_J1 = 0.0503;
	static const double r1 = 3.4;

	static const double mass_J2 = 1.432;
	static const double width_J2 = 0.109;
	static const double r2 = 2.7;

	static const double a = 1.95;
	static const double b = 1.76;

	std::ofstream fout("testing/scan_complex_amplitudes.dat");
	if(fout.is_open()) {
		for(double m = m_min+0.001; m <= m_max; m += 0.001) {
			double q = ComplexPDF::CalculateMomentum(m, m_K, m_Pi);
			double q_0 = ComplexPDF::CalculateMomentum(mass_J0, m_K, m_Pi);
			double q_1 = ComplexPDF::CalculateMomentum(mass_J1, m_K, m_Pi);
			double q_2 = ComplexPDF::CalculateMomentum(mass_J2, m_K, m_Pi);

			std::complex<double> lass  = ComplexPDF::LASS(m,mass_J0,width_J0,a,b,q,q_0);
			std::complex<double> rbw_0 = ComplexPDF::RelativisticSpinBreitWigner(m,mass_J0,width_J0,r0,0,q,q_0);
			std::complex<double> rbw_1 = ComplexPDF::RelativisticSpinBreitWigner(m,mass_J1,width_J1,r1,1,q,q_1);
			std::complex<double> rbw_2 = ComplexPDF::RelativisticSpinBreitWigner(m,mass_J2,width_J2,r2,2,q,q_2);

			// weights for each amplitude
			std::complex<double> w_0 = std::polar(std::sqrt(1.0/(1.0+2.0+1.5)),0.0);
			std::complex<double> w_1 = std::polar(std::sqrt(2.0/(1.0+2.0+1.5)),0.0);
			std::complex<double> w_2 = std::polar(std::sqrt(1.5/(1.0+2.0+1.5)),0.0);
			// calc combined amplitude
			std::complex<double> amplitude = w_0 * lass + w_1 * rbw_1 + w_2 * rbw_2;

			fout << m << ";";
			fout << std::norm(rbw_0) << ";";
			fout << std::arg(rbw_0) << ";";
			fout << std::norm(rbw_1) << ";";
			fout << std::arg(rbw_1) << ";";
			fout << std::norm(rbw_2) << ";";
			fout << std::arg(rbw_2) << ";";
			fout << std::norm(lass) << ";";
			fout << std::arg(lass) << ";";
			fout << std::norm(amplitude) << ";";
			fout << std::arg(amplitude) << std::endl;
		}

		fout.close();
	} else {
		std::cerr << "ERROR: ScanAmplitudes(...) Could not open filestream" << std::endl;
	}

	std::ofstream dout("testing/scan_dalitz_plane.dat");
	if(dout.is_open()) {
		double m12_min = (m_Pi + m_K)*(m_Pi + m_K);
		double m12_max = (m_B0 - m_Phi)*(m_B0 - m_Phi);
		for(double m = m12_min; m <= m12_max; m += 0.001) {
			double m23_max_2 = 0;
			double m23_min_2 = 0;
			ComplexPDF::DalitzLimits(std::sqrt(m),m_B0,m_Pi,m_K,m_Phi,m23_max_2,m23_min_2);
			double F = ComplexPDF::PhaseSpaceFactor(std::sqrt(m),m_B0,m_Pi,m_K,m_Phi);

			dout << m << ";";
			dout << F << ";";
			dout << m23_max_2 << ";";
			dout << m23_min_2 << std::endl;
		}

		dout.close();
	} else {
		std::cerr << "ERROR: ScanAmplitudes(...) Could not open filestream" << std::endl;
	}
}

void TestingPHSPProjection() {
	std::cout << "INFO: Initialize PDF" << std::endl;
	RooRealVar m12("m12","m_{K#pi}",0.60,4.5);
	RooThreeBodyPhaseSpaceProjection proj_pdf("proj_pdf","proj_pdf",m12,m_K,m_Pi,m_Phi,m_B0);
	RooRealVar m12_lim("m12_lim","m_{K#pi}",0.7,1.55);
	RooThreeBodyPhaseSpaceProjection proj_pdf_lim("proj_pdf_lim","proj_pdf_lim",m12_lim,m_K,m_Pi,m_Phi,m_B0);

	RooRealVar m12_KK("m12_KK","m_{KK}",0.95,4.5);
	RooThreeBodyPhaseSpaceProjection proj_pdf_KK("proj_pdf_KK","proj_pdf",m12_KK,m_K,m_K,0.892,m_B0);
	RooRealVar m12_lim_KK("m12_lim_KK","m_{KK}",0.98,1.05);
	RooThreeBodyPhaseSpaceProjection proj_pdf_lim_KK("proj_pdf_lim_KK","proj_pdf_lim",m12_lim_KK,m_K,m_K,0.892,m_B0);

	std::cout << "INFO: Generate ToyMC" << std::endl;
	RooDataSet *data = proj_pdf.generate(RooArgSet(m12),10000);
	RooDataSet *data_lim = proj_pdf_lim.generate(RooArgSet(m12_lim),10000);
	RooDataSet *data_KK = proj_pdf_KK.generate(RooArgSet(m12_KK),10000);
	RooDataSet *data_lim_KK = proj_pdf_lim_KK.generate(RooArgSet(m12_lim_KK),10000);

	std::cout << "INFO: Draw PHSP Projection PDF" << std::endl;
	RooPlot *frame_m12 = m12.frame(Bins(50));
	data->plotOn(frame_m12);
	proj_pdf.plotOn(frame_m12, LineColor(kBlue), LineStyle(kDashed));

	RooPlot *frame_m12_lim = m12_lim.frame(Bins(50));
	data_lim->plotOn(frame_m12_lim);
	proj_pdf_lim.plotOn(frame_m12_lim, LineColor(kBlue), LineStyle(kDashed));

	RooPlot *frame_m12_KK = m12_KK.frame(Bins(50));
	data_KK->plotOn(frame_m12_KK);
	proj_pdf_KK.plotOn(frame_m12_KK, LineColor(kBlue), LineStyle(kDashed));

	RooPlot *frame_m12_lim_KK = m12_lim_KK.frame(Bins(50));
	data_lim_KK->plotOn(frame_m12_lim_KK);
	proj_pdf_lim_KK.plotOn(frame_m12_lim_KK, LineColor(kBlue), LineStyle(kDashed));

	std::string filename = "testing/PHSP_Projection";
	TCanvas* c = new TCanvas(filename.c_str(),filename.c_str(),2*1024,2*768);
	c->Divide(2,2);
	c->cd(1);
	frame_m12->Draw();
	c->cd(2);
	frame_m12_lim->Draw();
	c->cd(3);
	frame_m12_KK->Draw();
	c->cd(4);
	frame_m12_lim_KK->Draw();
	filename += ".eps";
	c->SaveAs(filename.c_str());
	c->Clear();

	delete data;
	delete data_lim;
	delete data_KK;
	delete data_lim_KK;
	delete frame_m12;
	delete frame_m12_KK;
	delete frame_m12_lim;
	delete frame_m12_lim_KK;
}

void TestingMassHelicityAnglesPDF_CPV(bool draw_projections, ACCEPTANCE_FUNCTION acceptance, int mode = 0) {
	RooRealVar m("m","m_{K#pi}",0.7,1.55,"GeV");
	RooRealVar hel_phi("hel_phi","#Phi",-TMath::Pi(),TMath::Pi(),"rad");
	RooRealVar hel_cos_th1("hel_cos_th1","cos #Theta_{1}",-1.0,1.0,"");
	RooRealVar hel_cos_th2("hel_cos_th2","cos #Theta_{2}",-1.0,1.0,"");

	m.setRange("full_range",0.7,1.55);
	hel_phi.setRange("full_range",-TMath::Pi(),TMath::Pi());
	hel_cos_th1.setRange("full_range",-1.0,1.0);
	hel_cos_th2.setRange("full_range",-1.0,1.0);

	RooCategory channel("channel","channel");
	channel.defineType("CP");
	channel.defineType("CA");

	channel.setRange("CP","CP");
	channel.setRange("CA","CA");

	RooRealVar mass_0("mass_0","mass_0",1.435);
	RooRealVar width_0("width_0","width_0",0.279);
	RooRealVar a("a","a",1.95);
	RooRealVar b("b","b",1.76);
	RooRealVar mass_1("mass_1","mass_1",0.896);
	RooRealVar width_1("width_1","width_1",0.0503);
	RooRealVar r_1("r_1","r_1",3.4);
	RooRealVar mass_2("mass_2","mass_2",1.432);
	RooRealVar width_2("width_2","width_2",0.109);
	RooRealVar r_2("r_2","r_2",2.7);

	// from the yields measured by BaBar we can estimate the fraction
	// of Spin S states: S==0: ~0.2, S==1: ~0.6, S==2: ~0.2
	RooRealVar amp_00("amp_00","amp_00",0.75,0.0,2.0);
	RooRealVar phase_00("phase_00","phase_00",0.0);

	RooRealVar amp_10("amp_10","amp_10",0.707);
	RooRealVar phase_10("phase_10","phase_10",-TMath::Pi(),-2*TMath::Pi(),2*TMath::Pi());
	RooRealVar amp_1para("amp_1para","amp_1para",0.5,0.0,2.0);
	RooRealVar phase_1para("phase_1para","phase_1para",2.4,-2*TMath::Pi(),2*TMath::Pi());
	RooRealVar amp_1perp("amp_1perp","amp_1perp",0.5,0.0,2.0);
	RooRealVar phase_1perp("phase_1perp","phase_1perp",2.4,-2*TMath::Pi(),2*TMath::Pi());

	RooRealVar amp_20("amp_20","amp_20",0.95*0.9,0.0,2.0);
	RooRealVar phase_20("phase_20","phase_20",-TMath::Pi(),-2*TMath::Pi(),2*TMath::Pi());
	RooRealVar amp_2para("amp_2para","amp_2para",0.3*0.9,0.0,2.0);
	RooRealVar phase_2para("phase_2para","phase_2para",4.0,-2*TMath::Pi(),2*TMath::Pi());
	RooRealVar amp_2perp("amp_2perp","amp_2perp",0.1*0.9,0.0,2.0);
	RooRealVar phase_2perp("phase_2perp","phase_2perp",0.0,-2*TMath::Pi(),2*TMath::Pi());

	RooRealVar amp_00_delta("amp_00_delta","amp_00_delta",0.0,-0.5,0.5);
	RooRealVar phase_00_delta("phase_00_delta","phase_00_delta",0.0);

	RooRealVar amp_10_delta("amp_10_delta","amp_10_delta",0.0,-0.5,0.5);
	RooRealVar phase_10_delta("phase_10_delta","phase_10_delta",0.0,-TMath::Pi()/2,TMath::Pi()/2);
	RooRealVar amp_1para_delta("amp_1para_delta","amp_1para_delta",0.0,-0.5,0.5);
	RooRealVar phase_1para_delta("phase_1para_delta","phase_1para_delta",0.0,-TMath::Pi()/2,TMath::Pi()/2);
	RooRealVar amp_1perp_delta("amp_1perp_delta","amp_1perp_delta",0.0,-0.5,0.5);
	RooRealVar phase_1perp_delta("phase_1perp_delta","phase_1perp_delta",0.0,-TMath::Pi()/2,TMath::Pi()/2);

	RooRealVar amp_20_delta("amp_20_delta","amp_20_delta",0.0,-0.5,0.5);
	RooRealVar phase_20_delta("phase_20_delta","phase_20_delta",0.0,-TMath::Pi()/2,TMath::Pi()/2);
	RooRealVar amp_2para_delta("amp_2para_delta","amp_2para_delta",0.0,-0.5,0.5);
	RooRealVar phase_2para_delta("phase_2para_delta","phase_2para_delta",0.0,-TMath::Pi()/2,TMath::Pi()/2);
	RooRealVar amp_2perp_delta("amp_2perp_delta","amp_2perp_delta",0.0,-0.5,0.5);
	RooRealVar phase_2perp_delta("phase_2perp_delta","phase_2perp_delta",0.0,-TMath::Pi()/2,TMath::Pi()/2);

	RooFormulaVar CA_amp_00("CA_amp_00","CA_amp_00","@0 * (1 - @1)",RooArgList(amp_00,amp_00_delta));
	RooFormulaVar CA_phase_00("CA_phase_00","CA_phase_00","@0 - @1",RooArgList(phase_00,phase_00_delta));
	RooFormulaVar CA_amp_10("CA_amp_10","CA_amp_10","@0 * (1 - @1)",RooArgList(amp_10,amp_10_delta));
	RooFormulaVar CA_phase_10("CA_phase_10","CA_phase_10","@0 - @1",RooArgList(phase_10,phase_10_delta));
	RooFormulaVar CA_amp_1para("CA_amp_1para","CA_amp_1para","@0 * (1 - @1)",RooArgList(amp_1para,amp_1para_delta));
	RooFormulaVar CA_phase_1para("CA_phase_1para","CA_phase_1para","@0 - @1",RooArgList(phase_1para,phase_1para_delta));
	RooFormulaVar CA_amp_1perp("CA_amp_1perp","CA_amp_1perp","@0 * (1 - @1)",RooArgList(amp_1perp,amp_1perp_delta));
	RooFormulaVar CA_phase_1perp("CA_phase_1perp","CA_phase_1perp","@0 - @1",RooArgList(phase_1perp,phase_1perp_delta));
	RooFormulaVar CA_amp_20("CA_amp_20","CA_amp_20","@0 * (1 - @1)",RooArgList(amp_20,amp_20_delta));
	RooFormulaVar CA_phase_20("CA_phase_20","CA_phase_20","@0 - @1",RooArgList(phase_20,phase_20_delta));
	RooFormulaVar CA_amp_2para("CA_amp_2para","CA_amp_2para","@0 * (1 - @1)",RooArgList(amp_2para,amp_2para_delta));
	RooFormulaVar CA_phase_2para("CA_phase_2para","CA_phase_2para","@0 - @1",RooArgList(phase_2para,phase_2para_delta));
	RooFormulaVar CA_amp_2perp("CA_amp_2perp","CA_amp_2perp","@0 * (1 - @1)",RooArgList(amp_2perp,amp_2perp_delta));
	RooFormulaVar CA_phase_2perp("CA_phase_2perp","CA_phase_2perp","@0 - @1",RooArgList(phase_2perp,phase_2perp_delta));

	RooFormulaVar CP_amp_00("CP_amp_00","CP_amp_00","@0 * (1 + @1)",RooArgList(amp_00,amp_00_delta));
	RooFormulaVar CP_phase_00("CP_phase_00","CP_phase_00","@0 + @1",RooArgList(phase_00,phase_00_delta));
	RooFormulaVar CP_amp_10("CP_amp_10","CP_amp_10","@0 * (1 + @1)",RooArgList(amp_10,amp_10_delta));
	RooFormulaVar CP_phase_10("CP_phase_10","CP_phase_10","@0 + @1",RooArgList(phase_10,phase_10_delta));
	RooFormulaVar CP_amp_1para("CP_amp_1para","CP_amp_1para","@0 * (1 + @1)",RooArgList(amp_1para,amp_1para_delta));
	RooFormulaVar CP_phase_1para("CP_phase_1para","CP_phase_1para","@0 + @1",RooArgList(phase_1para,phase_1para_delta));
	RooFormulaVar CP_amp_1perp("CP_amp_1perp","CP_amp_1perp","@0 * (1 + @1)",RooArgList(amp_1perp,amp_1perp_delta));
	RooFormulaVar CP_phase_1perp("CP_phase_1perp","CP_phase_1perp","@0 + @1",RooArgList(phase_1perp,phase_1perp_delta));
	RooFormulaVar CP_amp_20("CP_amp_20","CP_amp_20","@0 * (1 + @1)",RooArgList(amp_20,amp_20_delta));
	RooFormulaVar CP_phase_20("CP_phase_20","CP_phase_20","@0 + @1",RooArgList(phase_20,phase_20_delta));
	RooFormulaVar CP_amp_2para("CP_amp_2para","CP_amp_2para","@0 * (1 + @1)",RooArgList(amp_2para,amp_2para_delta));
	RooFormulaVar CP_phase_2para("CP_phase_2para","CP_phase_2para","@0 + @1",RooArgList(phase_2para,phase_2para_delta));
	RooFormulaVar CP_amp_2perp("CP_amp_2perp","CP_amp_2perp","@0 * (1 + @1)",RooArgList(amp_2perp,amp_2perp_delta));
	RooFormulaVar CP_phase_2perp("CP_phase_2perp","CP_phase_2perp","@0 + @1",RooArgList(phase_2perp,phase_2perp_delta));

	// TODO add delete or move to main method and clean up there...
	int debug_level = 1;
	CacheIntegrator<MassHelicityAnglesNormCachedIntegral>* mass_helicity_angles_norm_cache_integrator = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral>(debug_level, 0);
	CacheIntegrator<MassHelicityAnglesRealCachedIntegral>* mass_helicity_angles_real_cache_integrator = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral>(debug_level, 0);
	CacheIntegrator<MassHelicityAnglesImagCachedIntegral>* mass_helicity_angles_imag_cache_integrator = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral>(debug_level, 0);
	CacheIntegrator<MassHelicityAnglesNormCachedIntegral_phi_th1_th2>* mass_helicity_angles_norm_cache_integrator_phi_th1_th2 = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral_phi_th1_th2>(debug_level, mode);
	CacheIntegrator<MassHelicityAnglesRealCachedIntegral_phi_th1_th2>* mass_helicity_angles_real_cache_integrator_phi_th1_th2 = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral_phi_th1_th2>(debug_level, mode);
	CacheIntegrator<MassHelicityAnglesImagCachedIntegral_phi_th1_th2>* mass_helicity_angles_imag_cache_integrator_phi_th1_th2 = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral_phi_th1_th2>(debug_level, mode);
	CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_th1_th2>* mass_helicity_angles_norm_cache_integrator_m_th1_th2 = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_th1_th2>(debug_level, mode);
	CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_th1_th2>* mass_helicity_angles_real_cache_integrator_m_th1_th2 = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_th1_th2>(debug_level, mode);
	CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_th1_th2>* mass_helicity_angles_imag_cache_integrator_m_th1_th2 = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_th1_th2>(debug_level, mode);
	CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_phi_th2>* mass_helicity_angles_norm_cache_integrator_m_phi_th2 = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_phi_th2>(debug_level, mode);
	CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_phi_th2>* mass_helicity_angles_real_cache_integrator_m_phi_th2 = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_phi_th2>(debug_level, mode);
	CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_phi_th2>* mass_helicity_angles_imag_cache_integrator_m_phi_th2 = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_phi_th2>(debug_level, mode);
	CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_phi_th1>* mass_helicity_angles_norm_cache_integrator_m_phi_th1 = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_phi_th1>(debug_level, mode);
	CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_phi_th1>* mass_helicity_angles_real_cache_integrator_m_phi_th1 = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_phi_th1>(debug_level, mode);
	CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_phi_th1>* mass_helicity_angles_imag_cache_integrator_m_phi_th1 = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_phi_th1>(debug_level, mode);

	std::cout << mass_helicity_angles_norm_cache_integrator->GetMode() << std::endl;
	std::cout << mass_helicity_angles_real_cache_integrator->GetMode() << std::endl;
	std::cout << mass_helicity_angles_imag_cache_integrator->GetMode() << std::endl;
	std::cout << mass_helicity_angles_norm_cache_integrator_phi_th1_th2->GetMode() << std::endl;
	std::cout << mass_helicity_angles_real_cache_integrator_phi_th1_th2->GetMode() << std::endl;
	std::cout << mass_helicity_angles_imag_cache_integrator_phi_th1_th2->GetMode() << std::endl;
	std::cout << mass_helicity_angles_norm_cache_integrator_m_th1_th2->GetMode() << std::endl;
	std::cout << mass_helicity_angles_real_cache_integrator_m_th1_th2->GetMode() << std::endl;
	std::cout << mass_helicity_angles_imag_cache_integrator_m_th1_th2->GetMode() << std::endl;
	std::cout << mass_helicity_angles_norm_cache_integrator_m_phi_th2->GetMode() << std::endl;
	std::cout << mass_helicity_angles_real_cache_integrator_m_phi_th2->GetMode() << std::endl;
	std::cout << mass_helicity_angles_imag_cache_integrator_m_phi_th2->GetMode() << std::endl;
	std::cout << mass_helicity_angles_norm_cache_integrator_m_phi_th1->GetMode() << std::endl;
	std::cout << mass_helicity_angles_real_cache_integrator_m_phi_th1->GetMode() << std::endl;
	std::cout << mass_helicity_angles_imag_cache_integrator_m_phi_th1->GetMode() << std::endl;

	std::string path_to_integrals = "./CachedIntegrals_ACCEPTANCE" + FFUtil::GetAcceptanceString(acceptance) + "/";
	mp::execute_command("mkdir -p " + path_to_integrals);
	mass_helicity_angles_norm_cache_integrator->LoadIntegrals(path_to_integrals);
	mass_helicity_angles_real_cache_integrator->LoadIntegrals(path_to_integrals);
	mass_helicity_angles_imag_cache_integrator->LoadIntegrals(path_to_integrals);
	mass_helicity_angles_norm_cache_integrator_phi_th1_th2->LoadIntegrals(path_to_integrals);
	mass_helicity_angles_real_cache_integrator_phi_th1_th2->LoadIntegrals(path_to_integrals);
	mass_helicity_angles_imag_cache_integrator_phi_th1_th2->LoadIntegrals(path_to_integrals);
	mass_helicity_angles_norm_cache_integrator_m_th1_th2->LoadIntegrals(path_to_integrals);
	mass_helicity_angles_real_cache_integrator_m_th1_th2->LoadIntegrals(path_to_integrals);
	mass_helicity_angles_imag_cache_integrator_m_th1_th2->LoadIntegrals(path_to_integrals);
	mass_helicity_angles_norm_cache_integrator_m_phi_th2->LoadIntegrals(path_to_integrals);
	mass_helicity_angles_real_cache_integrator_m_phi_th2->LoadIntegrals(path_to_integrals);
	mass_helicity_angles_imag_cache_integrator_m_phi_th2->LoadIntegrals(path_to_integrals);
	mass_helicity_angles_norm_cache_integrator_m_phi_th1->LoadIntegrals(path_to_integrals);
	mass_helicity_angles_real_cache_integrator_m_phi_th1->LoadIntegrals(path_to_integrals);
	mass_helicity_angles_imag_cache_integrator_m_phi_th1->LoadIntegrals(path_to_integrals);

	MHI_CC_parameters mhi_cc_paramters = {
			&CA_amp_00,
			&CA_phase_00,
			&CA_amp_10,
			&CA_phase_10,
			&CA_amp_1para,
			&CA_phase_1para,
			&CA_amp_1perp,
			&CA_phase_1perp,
			&CA_amp_20,
			&CA_phase_20,
			&CA_amp_2para,
			&CA_phase_2para,
			&CA_amp_2perp,
			&CA_phase_2perp
	};
	MHI_PDG_constants mhi_pdg_constants = {m_Pi, m_K, m_Phi, m_B0};
	MHI_CacheIntegrators_4D mhi_4D = {
			mass_helicity_angles_norm_cache_integrator,
			mass_helicity_angles_real_cache_integrator,
			mass_helicity_angles_imag_cache_integrator
	};
	MHI_CacheIntegrators_3D_phi_th1_th2 mhi_3D_phi_th1_th2 = {
			mass_helicity_angles_norm_cache_integrator_phi_th1_th2,
			mass_helicity_angles_real_cache_integrator_phi_th1_th2,
			mass_helicity_angles_imag_cache_integrator_phi_th1_th2
	};
	MHI_CacheIntegrators_3D_m_th1_th2 mhi_3D_m_th1_th2 = {
			mass_helicity_angles_norm_cache_integrator_m_th1_th2,
			mass_helicity_angles_real_cache_integrator_m_th1_th2,
			mass_helicity_angles_imag_cache_integrator_m_th1_th2
	};
	MHI_CacheIntegrators_3D_m_phi_th2 mhi_3D_m_phi_th2 = {
			mass_helicity_angles_norm_cache_integrator_m_phi_th2,
			mass_helicity_angles_real_cache_integrator_m_phi_th2,
			mass_helicity_angles_imag_cache_integrator_m_phi_th2
	};
	MHI_CacheIntegrators_3D_m_phi_th1 mhi_3D_m_phi_th1 = {
			mass_helicity_angles_norm_cache_integrator_m_phi_th1,
			mass_helicity_angles_real_cache_integrator_m_phi_th1,
			mass_helicity_angles_imag_cache_integrator_m_phi_th1
	};

//	if(draw_projections) {
//		PrintFitStartBlock("Generate ToyMC");
//		RooDataSet *data = pdf.generate(RooArgSet(m,hel_phi,hel_cos_th1,hel_cos_th2),10000);
//
//		PrintFitStartBlock("Plotting projection on m",1);
//		RooPlot *frame_m = m.frame(Bins(50));
//		data->plotOn(frame_m);
//		pdf.plotOn(frame_m, LineColor(kBlue), LineStyle(kDashed));
//
//		PrintFitStartBlock("Plotting projection on phi",1);
//		RooPlot *frame_hel_phi = hel_phi.frame(Bins(50));
//		data->plotOn(frame_hel_phi);
//		pdf.plotOn(frame_hel_phi, LineColor(kBlue), LineStyle(kDashed));
//
//		PrintFitStartBlock("Plotting projection on cos_th1",1);
//		RooPlot *frame_hel_cos_th1 = hel_cos_th1.frame(Bins(50));
//		data->plotOn(frame_hel_cos_th1);
//		pdf.plotOn(frame_hel_cos_th1, LineColor(kBlue), LineStyle(kDashed));
//
//		PrintFitStartBlock("Plotting projection on cos_th2",1);
//		RooPlot *frame_hel_cos_th2 = hel_cos_th2.frame(Bins(50));
//		data->plotOn(frame_hel_cos_th2);
//		pdf.plotOn(frame_hel_cos_th2, LineColor(kBlue), LineStyle(kDashed));
//
//		PrintFitStartBlock("Plotting projections for subcomponents",1);
//		RooAbsReal* I_tot = pdf.createIntegral(RooArgSet(m,hel_phi,hel_cos_th1,hel_cos_th2));
//		double N_tot = I_tot->getVal();
//		std::cout << "INFO: I_tot: " << N_tot << std::endl;
//
//		amp_00.setVal(0.75);
//		amp_1para.setVal(0.0);
//		amp_10.setVal(0.0);
//		amp_1perp.setVal(0.0);
//		amp_2para.setVal(0.0);
//		amp_20.setVal(0.0);
//		amp_2perp.setVal(0.0);
//		double N_0 = I_tot->getVal();
//		double norm_0_scale = N_0/N_tot;
//		std::cout << "INFO: I_0: " << N_0 << "  " << norm_0_scale << std::endl;
//		pdf.plotOn(frame_m, LineColor(kRed), LineStyle(kDotted), Normalization(norm_0_scale,RooAbsReal::Relative));
//		pdf.plotOn(frame_hel_phi, LineColor(kRed), LineStyle(kDotted), Normalization(norm_0_scale,RooAbsReal::Relative));
//		pdf.plotOn(frame_hel_cos_th1, LineColor(kRed), LineStyle(kDotted), Normalization(norm_0_scale,RooAbsReal::Relative));
//		pdf.plotOn(frame_hel_cos_th2, LineColor(kRed), LineStyle(kDotted), Normalization(norm_0_scale,RooAbsReal::Relative));
//		amp_00.setVal(0.0);
//		amp_1para.setVal(0.5);
//		amp_10.setVal(0.707);
//		amp_1perp.setVal(0.5);
//		amp_2para.setVal(0.0);
//		amp_20.setVal(0.0);
//		amp_2perp.setVal(0.0);
//		double N_1 = I_tot->getVal();
//		double norm_1_scale = N_1/N_tot;
//		std::cout << "INFO: I_1: " << N_1 << "  " << norm_1_scale << std::endl;
//		pdf.plotOn(frame_m, LineColor(kOrange), LineStyle(kDashed), Normalization(norm_1_scale,RooAbsReal::Relative));
//		pdf.plotOn(frame_hel_phi, LineColor(kOrange), LineStyle(kDashed), Normalization(norm_1_scale,RooAbsReal::Relative));
//		pdf.plotOn(frame_hel_cos_th1, LineColor(kOrange), LineStyle(kDashed), Normalization(norm_1_scale,RooAbsReal::Relative));
//		pdf.plotOn(frame_hel_cos_th2, LineColor(kOrange), LineStyle(kDashed), Normalization(norm_1_scale,RooAbsReal::Relative));
//		amp_00.setVal(0.0);
//		amp_1para.setVal(0.0);
//		amp_10.setVal(0.0);
//		amp_1perp.setVal(0.0);
//		amp_2para.setVal(0.3*0.9);
//		amp_20.setVal(0.95*0.9);
//		amp_2perp.setVal(0.1*0.9);
//		double N_2 = I_tot->getVal();
//		double norm_2_scale = N_2/N_tot;
//		std::cout << "INFO: I_2: " << N_2 << "  " << norm_2_scale << std::endl;
//		pdf.plotOn(frame_m, LineColor(kSpring), LineStyle(kDashDotted), Normalization(norm_2_scale,RooAbsReal::Relative));
//		pdf.plotOn(frame_hel_phi, LineColor(kSpring), LineStyle(kDashDotted), Normalization(norm_2_scale,RooAbsReal::Relative));
//		pdf.plotOn(frame_hel_cos_th1, LineColor(kSpring), LineStyle(kDashDotted), Normalization(norm_2_scale,RooAbsReal::Relative));
//		pdf.plotOn(frame_hel_cos_th2, LineColor(kSpring), LineStyle(kDashDotted), Normalization(norm_2_scale,RooAbsReal::Relative));
//		amp_00.setVal(0.75);
//		amp_1para.setVal(0.5);
//		amp_10.setVal(0.707);
//		amp_1perp.setVal(0.5);
//		amp_2para.setVal(0.3*0.9);
//		amp_20.setVal(0.95*0.9);
//		amp_2perp.setVal(0.1*0.9);
//		std::cout << "INFO: I_sum: " << N_0+N_1+N_2 << "  " << norm_0_scale+norm_1_scale+norm_2_scale << std::endl;
//
//		std::string filename = "testing/MassHelicityAngles_CPV" + FFUtil::GetAcceptanceString(acceptance);
//		TCanvas* c = new TCanvas(filename.c_str(),filename.c_str(),2*1024,2*768);
//		c->Divide(2,2);
//		c->cd(1);
//		frame_m->Draw();
//		c->cd(2);
//		frame_hel_phi->Draw();
//		c->cd(3);
//		frame_hel_cos_th1->Draw();
//		c->cd(4);
//		frame_hel_cos_th2->Draw();
//		filename += ".eps";
//		c->SaveAs(filename.c_str());
//		c->Clear();
//		delete c;
//		delete frame_m;
//		delete frame_hel_cos_th1;
//		delete frame_hel_cos_th2;
//		delete frame_hel_phi;
//
//		PrintTimestamp();
//
//		m.setRange("K892",0.8,1.0);
//		m.setRange("LASS",1.1,1.3);
//		m.setRange("K1430",1.3,1.55);
//
//		std::vector<std::string> ranges;
//		ranges.push_back("K892");
//		ranges.push_back("LASS");
//		ranges.push_back("K1430");
//
//		for(unsigned int i = 0; i < ranges.size(); ++i) {
//			PrintFitStartBlock("Plotting projection for range "+ranges[i], 2);
//			RooPlot *frame_hel_phi_range = hel_phi.frame(Bins(50));
//			data->plotOn(frame_hel_phi_range, CutRange(ranges[i].c_str()));
//			pdf.plotOn(frame_hel_phi_range, LineColor(kBlue), LineStyle(kDashed), ProjectionRange(ranges[i].c_str()));
//
//			RooPlot *frame_hel_cos_th1_range = hel_cos_th1.frame(Bins(50));
//			data->plotOn(frame_hel_cos_th1_range, CutRange(ranges[i].c_str()));
//			pdf.plotOn(frame_hel_cos_th1_range, LineColor(kBlue), LineStyle(kDashed), ProjectionRange(ranges[i].c_str()));
//
//			RooPlot *frame_hel_cos_th2_range = hel_cos_th2.frame(Bins(50));
//			data->plotOn(frame_hel_cos_th2_range, CutRange(ranges[i].c_str()));
//			pdf.plotOn(frame_hel_cos_th2_range, LineColor(kBlue), LineStyle(kDashed), ProjectionRange(ranges[i].c_str()));
//
//			PrintFitStartBlock("Plotting projections for subcomponents", 1);
//			//RooAbsPdf* pdf_proj = pdf.createProjection(RooArgSet(hel_phi));
//			RooAbsReal* I_tot = pdf.createIntegral(RooArgSet(m,hel_phi,hel_cos_th1,hel_cos_th2),ranges[i].c_str());
//			double N_tot = I_tot->getVal();
//			std::cout << "INFO: I_tot: " << N_tot << std::endl;
//
//			amp_00.setVal(0.75);
//			amp_1para.setVal(0.0);
//			amp_10.setVal(0.0);
//			amp_1perp.setVal(0.0);
//			amp_2para.setVal(0.0);
//			amp_20.setVal(0.0);
//			amp_2perp.setVal(0.0);
//			double N_0 = I_tot->getVal();
//			double norm_0_scale = N_0/N_tot;
//			std::cout << "INFO: I_0: " << N_0 << "  " << norm_0_scale << std::endl;
//			pdf.plotOn(frame_hel_phi_range, LineColor(kRed), LineStyle(kSolid), ProjectionRange(ranges[i].c_str()));
//			pdf.plotOn(frame_hel_cos_th1_range, LineColor(kRed), LineStyle(kSolid), ProjectionRange(ranges[i].c_str()));
//			pdf.plotOn(frame_hel_cos_th2_range, LineColor(kRed), LineStyle(kSolid), ProjectionRange(ranges[i].c_str()));
//			pdf.plotOn(frame_hel_phi_range, LineColor(kRed), LineStyle(kDotted), Normalization(norm_0_scale,RooAbsReal::Relative), ProjectionRange(ranges[i].c_str()));
//			pdf.plotOn(frame_hel_cos_th1_range, LineColor(kRed), LineStyle(kDotted), Normalization(norm_0_scale,RooAbsReal::Relative), ProjectionRange(ranges[i].c_str()));
//			pdf.plotOn(frame_hel_cos_th2_range, LineColor(kRed), LineStyle(kDotted), Normalization(norm_0_scale,RooAbsReal::Relative), ProjectionRange(ranges[i].c_str()));
//			amp_00.setVal(0.0);
//			amp_1para.setVal(0.5);
//			amp_10.setVal(0.707);
//			amp_1perp.setVal(0.5);
//			amp_2para.setVal(0.0);
//			amp_20.setVal(0.0);
//			amp_2perp.setVal(0.0);
//			double N_1 = I_tot->getVal();
//			double norm_1_scale = N_1/N_tot;
//			std::cout << "INFO: I_1: " << N_1 << "  " << norm_1_scale << std::endl;
//			pdf.plotOn(frame_hel_phi_range, LineColor(kOrange), LineStyle(kSolid), ProjectionRange(ranges[i].c_str()));
//			pdf.plotOn(frame_hel_cos_th1_range, LineColor(kOrange), LineStyle(kSolid), ProjectionRange(ranges[i].c_str()));
//			pdf.plotOn(frame_hel_cos_th2_range, LineColor(kOrange), LineStyle(kSolid), ProjectionRange(ranges[i].c_str()));
//			pdf.plotOn(frame_hel_phi_range, LineColor(kOrange), LineStyle(kDashed), Normalization(norm_1_scale,RooAbsReal::Relative), ProjectionRange(ranges[i].c_str()));
//			pdf.plotOn(frame_hel_cos_th1_range, LineColor(kOrange), LineStyle(kDashed), Normalization(norm_1_scale,RooAbsReal::Relative), ProjectionRange(ranges[i].c_str()));
//			pdf.plotOn(frame_hel_cos_th2_range, LineColor(kOrange), LineStyle(kDashed), Normalization(norm_1_scale,RooAbsReal::Relative), ProjectionRange(ranges[i].c_str()));
//			amp_00.setVal(0.0);
//			amp_1para.setVal(0.0);
//			amp_10.setVal(0.0);
//			amp_1perp.setVal(0.0);
//			amp_2para.setVal(0.3*0.9);
//			amp_20.setVal(0.95*0.9);
//			amp_2perp.setVal(0.1*0.9);
//			double N_2 = I_tot->getVal();
//			double norm_2_scale = N_2/N_tot;
//			std::cout << "INFO: I_2: " << N_2 << "  " << norm_2_scale << std::endl;
//			pdf.plotOn(frame_hel_phi_range, LineColor(kSpring), LineStyle(kSolid), ProjectionRange(ranges[i].c_str()));
//			pdf.plotOn(frame_hel_cos_th1_range, LineColor(kSpring), LineStyle(kSolid), ProjectionRange(ranges[i].c_str()));
//			pdf.plotOn(frame_hel_cos_th2_range, LineColor(kSpring), LineStyle(kSolid), ProjectionRange(ranges[i].c_str()));
//			pdf.plotOn(frame_hel_phi_range, LineColor(kSpring), LineStyle(kDashDotted), Normalization(norm_2_scale,RooAbsReal::Relative), ProjectionRange(ranges[i].c_str()));
//			pdf.plotOn(frame_hel_cos_th1_range, LineColor(kSpring), LineStyle(kDashDotted), Normalization(norm_2_scale,RooAbsReal::Relative), ProjectionRange(ranges[i].c_str()));
//			pdf.plotOn(frame_hel_cos_th2_range, LineColor(kSpring), LineStyle(kDashDotted), Normalization(norm_2_scale,RooAbsReal::Relative), ProjectionRange(ranges[i].c_str()));
//			amp_00.setVal(0.75);
//			amp_1para.setVal(0.5);
//			amp_10.setVal(0.707);
//			amp_1perp.setVal(0.5);
//			amp_2para.setVal(0.3*0.9);
//			amp_20.setVal(0.95*0.9);
//			amp_2perp.setVal(0.1*0.9);
//			std::cout << "INFO: I_sum: " << N_0+N_1+N_2 << "  " << norm_0_scale+norm_1_scale+norm_2_scale << std::endl;
//
//			std::string filename = "testing/MassHelicityAngles_CPV_"+ranges[i] + FFUtil::GetAcceptanceString(acceptance);
//			TCanvas* d = new TCanvas(filename.c_str(),filename.c_str(),3*1024,768);
//			d->Divide(3,1);
//			d->cd(1);
//			frame_hel_phi_range->Draw();
//			d->cd(2);
//			frame_hel_cos_th1_range->Draw();
//			d->cd(3);
//			frame_hel_cos_th2_range->Draw();
//			filename += ".eps";
//			d->SaveAs(filename.c_str());
//			d->Clear();
//			delete d;
//			delete frame_hel_cos_th1_range;
//			delete frame_hel_cos_th2_range;
//			delete frame_hel_phi_range;
//		}
//		delete data;
//	}

	PrintFitStartBlock("Comparing Normalizations and Asymmetry");

	long int num_cores = 1;
//	long int num_cores = sysconf( _SC_NPROCESSORS_ONLN );
//	if(num_cores > 4)
//		num_cores = 4;
//	std::cout << "Going to fit with NumCPU: " << num_cores << std::endl;

	// setting test asymmetry
	amp_10_delta.setVal(0.3);
	amp_1para_delta.setVal(0.3);
	amp_1perp_delta.setVal(0.3);

	RooArgSet varset(m,hel_phi,hel_cos_th1,hel_cos_th2,channel);

	PrintTimestamp();
	PrintFitStartBlock("Signal");

	RooMassHelicityAnglesCPV signal_pdf("signal_pdf","signal_pdf",m,hel_phi,hel_cos_th1,hel_cos_th2,channel,
				mass_0,width_0,a,b,mass_1,width_1,r_1,mass_2,width_2,r_2,
				CP_amp_00,CP_phase_00,
				CP_amp_10,CP_phase_10,CP_amp_1para,CP_phase_1para,CP_amp_1perp,CP_phase_1perp,
				CP_amp_20,CP_phase_20,CP_amp_2para,CP_phase_2para,CP_amp_2perp,CP_phase_2perp,
				acceptance,
				mhi_cc_paramters,
				mhi_pdg_constants,
				mhi_4D,
				mhi_3D_phi_th1_th2,
				mhi_3D_m_th1_th2,
				mhi_3D_m_phi_th2,
				mhi_3D_m_phi_th1);
	RooRealVar signal_yield("signal_yield","signal_yield",1000,-100,2000);
	RooExtendPdf signal_pdf_extended("signal_pdf_extended","signal_pdf_extended",signal_pdf,signal_yield);
	double expected = signal_pdf_extended.expectedEvents(varset);
	std::cout << "Expected events in signal pdf: " << expected << std::endl;
	double asym = signal_pdf.getAsymmetry();
	std::cout << "Expected asymmetry in signal pdf: " << asym << std::endl;
	double expected_CP = expected / (1 + (1 - asym)/(1 + asym));
	double expected_CA = expected - expected_CP;
	std::cout << "Expected events in signal pdf for CP: " << expected_CP << std::endl;
	std::cout << "Expected events in signal pdf for CA: " << expected_CA << std::endl;

	PrintTimestamp();
	RooDataSet *_data_signal = signal_pdf_extended.generate(varset,RooRandom::randomGenerator()->Poisson(signal_pdf_extended.expectedEvents(varset)));
	RooDataSet& data_signal = *_data_signal;
	data_signal.Print();

	//ComplexPDF::SetSphericalHarmonicCache(true);
	ComplexPDF::SetHelicityAnglesCache(true);

	PrintTimestamp();
	if(draw_projections) {
		RooPlot *frame_m_0 = m.frame(Bins(50));
		data_signal.plotOn(frame_m_0);
		signal_pdf_extended.plotOn(frame_m_0, LineColor(kOrange), LineStyle(kSolid));
		RooPlot *frame_m_1 = m.frame(Bins(50));
		data_signal.plotOn(frame_m_1, Cut("channel==channel::CP"));
		signal_pdf_extended.plotOn(frame_m_1, Slice(channel,"CP"), LineColor(kBlue), LineStyle(kSolid), ProjWData(data_signal), NumCPU(num_cores));
		RooPlot *frame_m_2 = m.frame(Bins(50));
		data_signal.plotOn(frame_m_2, Cut("channel==channel::CA"));
		signal_pdf_extended.plotOn(frame_m_2, Slice(channel,"CA"), LineColor(kRed), LineStyle(kSolid), ProjWData(data_signal), NumCPU(num_cores));
		std::string filename = "testing/MassHelicityAngles_CPV_data_signal" + FFUtil::GetAcceptanceString(acceptance);
		TCanvas* c = new TCanvas(filename.c_str(),filename.c_str(),3*1280,1024);
		c->Divide(3,1);
		TLatex text;
		text.SetTextColor(kBlack);
		text.SetTextFont(42);
		text.SetNDC(1);
		c->cd(1);
		frame_m_0->Draw();
		text.SetTitle("B^{0}/#bar{B^{0}} #rightarrow #phi K^{#pm} #pi^{#mp}");
		text.DrawLatex(1.0 - 0.05 - 0.01 - text.GetXsize(), 0.89,"B^{0}/#bar{B^{0}} #rightarrow #phi K^{#pm} #pi^{#mp}");
		c->cd(2);
		frame_m_1->Draw();
		text.SetTitle("B^{0} #rightarrow #phi K^{+} #pi^{-}");
		text.DrawLatex(1.0 - 0.05 - 0.01 - text.GetXsize(), 0.89,"B^{0} #rightarrow #phi K^{+} #pi^{-}");
		c->cd(3);
		frame_m_2->Draw();
		text.SetTitle("#bar{B^{0}} #rightarrow #phi K^{-} #pi^{+}");
		text.DrawLatex(1.0 - 0.05 - 0.01 - text.GetXsize(), 0.89,"#bar{B^{0}} #rightarrow #phi K^{-} #pi^{+}");
		double max = std::max((double)frame_m_1->GetMaximum(),(double)frame_m_2->GetMaximum());
		frame_m_1->SetMaximum(max);
		frame_m_2->SetMaximum(max);
		filename += "_pdf_projection.eps";
		c->SaveAs(filename.c_str());
		c->Clear();
		delete c;
		delete frame_m_0;
		delete frame_m_1;
		delete frame_m_2;
	}

//	std::cout << "Cache size: " << ComplexPDF::GetSphericalHarmonicCacheSize() << std::endl;
//	std::cout << "Cache stats: " << ComplexPDF::GetSphericalHarmonicCacheStats() << std::endl;
	std::cout << "Cache size: " << ComplexPDF::GetHelicityAnglesCacheSize() << std::endl;
	std::cout << "Cache stats: " << ComplexPDF::GetHelicityAnglesCacheStats() << std::endl;

	PrintTimestamp();
	PrintFitStartBlock("Background");

	RooRealVar bkg_slope("bkg_slope","bkg_slope",0.3,-1.0,1.0);
	bkg_slope.setConstant(true);
	RooChebychev m_bkg("m_bkg","m_bkg",m,RooArgList(bkg_slope));
	RooChebychev hel_phi_bkg("hel_phi_bkg","hel_phi_bkg",hel_phi,RooArgList(bkg_slope));
	RooChebychev hel_th1_bkg("hel_th1_bkg","hel_th1_bkg",hel_cos_th1,RooArgList(bkg_slope));
	RooChebychev hel_th2_bkg("hel_th2_bkg","hel_th2_bkg",hel_cos_th2,RooArgList(bkg_slope));
	RooCategoryPdf channel_bkg("channel_bkg","channel_bkg",channel);
	RooProdPdf bkg_pdf("bkg_pdf","bkg_pdf",RooArgList(m_bkg,hel_phi_bkg,hel_th1_bkg,hel_th2_bkg,channel_bkg));
	RooRealVar bkg_yield("bkg_yield","bkg_yield",1000,-100,2000);
	RooExtendPdf bkg_pdf_extended("bkg_pdf_extended","bkg_pdf_extended",bkg_pdf,bkg_yield);
	std::cout << "Expected events in bkg pdf: " << bkg_pdf_extended.expectedEvents(varset) << std::endl;

	RooDataSet *_data_bkg = bkg_pdf_extended.generate(varset,RooRandom::randomGenerator()->Poisson(bkg_pdf_extended.expectedEvents(varset)));
	RooDataSet& data_bkg = *_data_bkg;
	data_bkg.Print();

	PrintTimestamp();
	if(draw_projections) {
		RooPlot *frame_m_0 = m.frame(Bins(50));
		data_bkg.plotOn(frame_m_0);
		bkg_pdf_extended.plotOn(frame_m_0, LineColor(kOrange), LineStyle(kSolid));
		RooPlot *frame_m_1 = m.frame(Bins(50));
		data_bkg.plotOn(frame_m_1, Cut("channel==channel::CP"));
		bkg_pdf_extended.plotOn(frame_m_1,  Slice(channel,"CP"), LineColor(kBlue), LineStyle(kSolid));
		RooPlot *frame_m_2 = m.frame(Bins(50));
		data_bkg.plotOn(frame_m_2, Cut("channel==channel::CA"));
		bkg_pdf_extended.plotOn(frame_m_2,  Slice(channel,"CA"), LineColor(kRed), LineStyle(kSolid));
		std::string filename = "testing/MassHelicityAngles_CPV_data_bkg" + FFUtil::GetAcceptanceString(acceptance);
		TCanvas* c = new TCanvas(filename.c_str(),filename.c_str(),3*1280,1024);
		c->Divide(3,1);
		TLatex text;
		text.SetTextColor(kBlack);
		text.SetTextFont(42);
		text.SetNDC(1);
		c->cd(1);
		frame_m_0->Draw();
		text.SetTitle("B^{0}/#bar{B^{0}} #rightarrow #phi K^{#pm} #pi^{#mp}");
		text.DrawLatex(1.0 - 0.05 - 0.01 - text.GetXsize(), 0.89,"B^{0}/#bar{B^{0}} #rightarrow #phi K^{#pm} #pi^{#mp}");
		c->cd(2);
		frame_m_1->Draw();
		text.SetTitle("B^{0} #rightarrow #phi K^{+} #pi^{-}");
		text.DrawLatex(1.0 - 0.05 - 0.01 - text.GetXsize(), 0.89,"B^{0} #rightarrow #phi K^{+} #pi^{-}");
		c->cd(3);
		frame_m_2->Draw();
		text.SetTitle("#bar{B^{0}} #rightarrow #phi K^{-} #pi^{+}");
		text.DrawLatex(1.0 - 0.05 - 0.01 - text.GetXsize(), 0.89,"#bar{B^{0}} #rightarrow #phi K^{-} #pi^{+}");
		double max = std::max((double)frame_m_1->GetMaximum(),(double)frame_m_2->GetMaximum());
		frame_m_1->SetMaximum(max);
		frame_m_2->SetMaximum(max);
		filename += "_pdf_projection.eps";
		c->SaveAs(filename.c_str());
		c->Clear();
		delete c;
		delete frame_m_0;
		delete frame_m_1;
		delete frame_m_2;
	}

	PrintTimestamp();
	PrintFitStartBlock("Signal+Background");

	RooAddPdf model("model","model",RooArgList(signal_pdf,bkg_pdf),RooArgList(signal_yield,bkg_yield));
	std::cout << "Expected events in model: " << model.expectedEvents(varset) << std::endl;

//	ComplexPDF::SetSphericalHarmonicCache(false);
//	ComplexPDF::ClearSphericalHarmonicCache();
//	std::cout << "Cache size: " << ComplexPDF::GetSphericalHarmonicCacheSize() << std::endl;
//	std::cout << "Cache stats: " << ComplexPDF::GetSphericalHarmonicCacheStats() << std::endl;
	ComplexPDF::SetHelicityAnglesCache(false);
	ComplexPDF::ClearHelicityAnglesCache();
	std::cout << "Cache size: " << ComplexPDF::GetHelicityAnglesCacheSize() << std::endl;
	std::cout << "Cache stats: " << ComplexPDF::GetHelicityAnglesCacheStats() << std::endl;

	RooDataSet *_data_model = model.generate(varset,RooRandom::randomGenerator()->Poisson(model.expectedEvents(varset)));
	RooDataSet& data_model = *_data_model;
	data_model.Print();

//	ComplexPDF::SetSphericalHarmonicCache(true);
	ComplexPDF::SetHelicityAnglesCache(true);

	PrintTimestamp();
	RooFitResult* fit_result_model = model.fitTo(data_model, Save(true), Extended(true));
	WriteCorrelationMatrixFile(fit_result_model,"testing/MassHelicityAngles_CPV_data_model_correlation.dat");

//	std::cout << "Cache size: " << ComplexPDF::GetSphericalHarmonicCacheSize() << std::endl;
//	std::cout << "Cache stats: " << ComplexPDF::GetSphericalHarmonicCacheStats() << std::endl;
	std::cout << "Cache size: " << ComplexPDF::GetHelicityAnglesCacheSize() << std::endl;
	std::cout << "Cache stats: " << ComplexPDF::GetHelicityAnglesCacheStats() << std::endl;

	PrintTimestamp();
	if(draw_projections) {
		RooPlot *frame_m_0 = m.frame(Bins(50));
		data_model.plotOn(frame_m_0);
		model.plotOn(frame_m_0, LineColor(kOrange), LineStyle(kSolid));
		model.plotOn(frame_m_0, LineColor(kOrange), LineStyle(kDashed), Components("signal_pdf"));
		model.plotOn(frame_m_0, LineColor(kOrange), LineStyle(kDashDotted), Components("bkg_pdf"));
		RooPlot *frame_m_1 = m.frame(Bins(50));
		data_model.plotOn(frame_m_1, Cut("channel==channel::CP"));
		model.plotOn(frame_m_1, Slice(channel,"CP"), LineColor(kBlue), LineStyle(kSolid), ProjWData(data_model), NumCPU(num_cores));
		model.plotOn(frame_m_1, Slice(channel,"CP"), LineColor(kBlue), LineStyle(kDashed), ProjWData(data_model), NumCPU(num_cores), Components("signal_pdf"));
		model.plotOn(frame_m_1, Slice(channel,"CP"), LineColor(kBlue), LineStyle(kDashDotted), Components("bkg_pdf"));
		RooPlot *frame_m_2 = m.frame(Bins(50));
		data_model.plotOn(frame_m_2, Cut("channel==channel::CA"));
		model.plotOn(frame_m_2, Slice(channel,"CA"), LineColor(kRed), LineStyle(kSolid), ProjWData(data_model), NumCPU(num_cores));
		model.plotOn(frame_m_2, Slice(channel,"CA"), LineColor(kRed), LineStyle(kDashed), ProjWData(data_model), NumCPU(num_cores), Components("signal_pdf"));
		model.plotOn(frame_m_2, Slice(channel,"CA"), LineColor(kRed), LineStyle(kDashDotted), Components("bkg_pdf"));
		std::string filename = "testing/MassHelicityAngles_CPV_data_model" + FFUtil::GetAcceptanceString(acceptance);
		TCanvas* c = new TCanvas(filename.c_str(),filename.c_str(),3*1280,1024);
		c->Divide(3,1);
		TLatex text;
		text.SetTextColor(kBlack);
		text.SetTextFont(42);
		text.SetNDC(1);
		c->cd(1);
		frame_m_0->Draw();
		text.SetTitle("B^{0}/#bar{B^{0}} #rightarrow #phi K^{#pm} #pi^{#mp}");
		text.DrawLatex(1.0 - 0.05 - 0.01 - text.GetXsize(), 0.89,"B^{0}/#bar{B^{0}} #rightarrow #phi K^{#pm} #pi^{#mp}");
		c->cd(2);
		frame_m_1->Draw();
		text.SetTitle("B^{0} #rightarrow #phi K^{+} #pi^{-}");
		text.DrawLatex(1.0 - 0.05 - 0.01 - text.GetXsize(), 0.89,"B^{0} #rightarrow #phi K^{+} #pi^{-}");
		c->cd(3);
		frame_m_2->Draw();
		text.SetTitle("#bar{B^{0}} #rightarrow #phi K^{-} #pi^{+}");
		text.DrawLatex(1.0 - 0.05 - 0.01 - text.GetXsize(), 0.89,"#bar{B^{0}} #rightarrow #phi K^{-} #pi^{+}");
		double max = std::max((double)frame_m_1->GetMaximum(),(double)frame_m_2->GetMaximum());
		frame_m_1->SetMaximum(max);
		frame_m_2->SetMaximum(max);
		filename += "_pdf_projection.eps";
		c->SaveAs(filename.c_str());
		c->Clear();
		delete c;
		delete frame_m_0;
		delete frame_m_1;
		delete frame_m_2;
	}

//	std::cout << "Cache size: " << ComplexPDF::GetSphericalHarmonicCacheSize() << std::endl;
//	std::cout << "Cache stats: " << ComplexPDF::GetSphericalHarmonicCacheStats() << std::endl;
	std::cout << "Cache size: " << ComplexPDF::GetHelicityAnglesCacheSize() << std::endl;
	std::cout << "Cache stats: " << ComplexPDF::GetHelicityAnglesCacheStats() << std::endl;

	delete _data_signal;
	delete _data_bkg;
	delete _data_model;

	PrintTimestamp();

	if(mode == 0) {
		mass_helicity_angles_norm_cache_integrator->SaveIntegrals(path_to_integrals);
		mass_helicity_angles_real_cache_integrator->SaveIntegrals(path_to_integrals);
		mass_helicity_angles_imag_cache_integrator->SaveIntegrals(path_to_integrals);
		mass_helicity_angles_norm_cache_integrator_phi_th1_th2->SaveIntegrals(path_to_integrals);
		mass_helicity_angles_real_cache_integrator_phi_th1_th2->SaveIntegrals(path_to_integrals);
		mass_helicity_angles_imag_cache_integrator_phi_th1_th2->SaveIntegrals(path_to_integrals);
		mass_helicity_angles_norm_cache_integrator_m_th1_th2->SaveIntegrals(path_to_integrals);
		mass_helicity_angles_real_cache_integrator_m_th1_th2->SaveIntegrals(path_to_integrals);
		mass_helicity_angles_imag_cache_integrator_m_th1_th2->SaveIntegrals(path_to_integrals);
		mass_helicity_angles_norm_cache_integrator_m_phi_th2->SaveIntegrals(path_to_integrals);
		mass_helicity_angles_real_cache_integrator_m_phi_th2->SaveIntegrals(path_to_integrals);
		mass_helicity_angles_imag_cache_integrator_m_phi_th2->SaveIntegrals(path_to_integrals);
		mass_helicity_angles_norm_cache_integrator_m_phi_th1->SaveIntegrals(path_to_integrals);
		mass_helicity_angles_real_cache_integrator_m_phi_th1->SaveIntegrals(path_to_integrals);
		mass_helicity_angles_imag_cache_integrator_m_phi_th1->SaveIntegrals(path_to_integrals);
	} else {
		std::string batch_job_path = path_to_integrals + "/batch_jobs/";
		mass_helicity_angles_norm_cache_integrator->PrepareBatchJobs(batch_job_path,49);
		mass_helicity_angles_real_cache_integrator->PrepareBatchJobs(batch_job_path,49);
		mass_helicity_angles_imag_cache_integrator->PrepareBatchJobs(batch_job_path,49);
		mass_helicity_angles_norm_cache_integrator_phi_th1_th2->PrepareBatchJobs(batch_job_path,49);
		mass_helicity_angles_real_cache_integrator_phi_th1_th2->PrepareBatchJobs(batch_job_path,49);
		mass_helicity_angles_imag_cache_integrator_phi_th1_th2->PrepareBatchJobs(batch_job_path,49);
		mass_helicity_angles_norm_cache_integrator_m_th1_th2->PrepareBatchJobs(batch_job_path,49);
		mass_helicity_angles_real_cache_integrator_m_th1_th2->PrepareBatchJobs(batch_job_path,49);
		mass_helicity_angles_imag_cache_integrator_m_th1_th2->PrepareBatchJobs(batch_job_path,49);
		mass_helicity_angles_norm_cache_integrator_m_phi_th2->PrepareBatchJobs(batch_job_path,49);
		mass_helicity_angles_real_cache_integrator_m_phi_th2->PrepareBatchJobs(batch_job_path,49);
		mass_helicity_angles_imag_cache_integrator_m_phi_th2->PrepareBatchJobs(batch_job_path,49);
		mass_helicity_angles_norm_cache_integrator_m_phi_th1->PrepareBatchJobs(batch_job_path,49);
		mass_helicity_angles_real_cache_integrator_m_phi_th1->PrepareBatchJobs(batch_job_path,49);
		mass_helicity_angles_imag_cache_integrator_m_phi_th1->PrepareBatchJobs(batch_job_path,49);
	}

	PrintTimestamp();
}

void TestingPhysicsParameterComputation(ACCEPTANCE_FUNCTION acceptance, const std::string& config_file) {
	RooRealVar mbc("mbc","m_{bc}",5.26,5.29,"GeV");
	RooRealVar deltae("deltae","#Delta E",-0.15,0.15,"GeV");
	RooRealVar chil0_ma("chil0_ma","m_{KK}",0.985,1.05,"GeV");
	RooRealVar mass_kpi("mass_kpi","m_{K#pi}",0.7,1.55,"GeV");
	RooRealVar hel_phi("hel_phi","#Phi",-TMath::Pi(),TMath::Pi(),"rad");
	RooRealVar hel_th1("hel_th1","cos #Theta_{1}",-1.0,1.0,"");
	RooRealVar hel_th2("hel_th2","cos #Theta_{2}",-1.0,1.0,"");
	RooRealVar nb_prime("nb_prime","C_{NB}'",-10.0,10.0,"");

	mbc.setRange("full_range",5.26,5.29);
	deltae.setRange("full_range",-0.15,0.15);
	chil0_ma.setRange("full_range",0.985,1.05);
	nb_prime.setRange("full_range",-10.0,10.0);
	mass_kpi.setRange("full_range",0.7,1.55);
	hel_phi.setRange("full_range",-TMath::Pi(),TMath::Pi());
	hel_th1.setRange("full_range",-1.0,1.0);
	hel_th2.setRange("full_range",-1.0,1.0);

	mbc.setRange("full_phsp_range",5.26,5.29);
	deltae.setRange("full_phsp_range",-0.15,0.15);
	chil0_ma.setRange("full_phsp_range",0.985,1.05);
	nb_prime.setRange("full_phsp_range",-10.0,10.0);
	mass_kpi.setRange("full_phsp_range",m_K+m_Pi+0.001,m_B0-m_Phi);
	hel_phi.setRange("full_phsp_range",-TMath::Pi(),TMath::Pi());
	hel_th1.setRange("full_phsp_range",-1.0,1.0);
	hel_th2.setRange("full_phsp_range",-1.0,1.0);

	mbc.setRange("full_LASS_range",5.26,5.29);
	deltae.setRange("full_LASS_range",-0.15,0.15);
	chil0_ma.setRange("full_LASS_range",0.985,1.05);
	nb_prime.setRange("full_LASS_range",-10.0,10.0);
	mass_kpi.setRange("full_LASS_range",m_K+m_Pi+0.001,1.6);
	hel_phi.setRange("full_LASS_range",-TMath::Pi(),TMath::Pi());
	hel_th1.setRange("full_LASS_range",-1.0,1.0);
	hel_th2.setRange("full_LASS_range",-1.0,1.0);

	RooCategory flavor("flavor","flavor");
	flavor.defineType("CP");
	flavor.defineType("CA");

	flavor.setRange("CP","CP");
	flavor.setRange("CA","CA");

	RooCategory channel("channel","decay channel");
	channel.defineType("C",0);

	// TODO add delete or move to main method and clean up there...
	int debug_level = 1;
	CacheIntegrator<MassHelicityAnglesNormCachedIntegral>* mass_helicity_angles_norm_cache_integrator = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral>(debug_level, 0);
	CacheIntegrator<MassHelicityAnglesRealCachedIntegral>* mass_helicity_angles_real_cache_integrator = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral>(debug_level, 0);
	CacheIntegrator<MassHelicityAnglesImagCachedIntegral>* mass_helicity_angles_imag_cache_integrator = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral>(debug_level, 0);
	CacheIntegrator<MassHelicityAnglesNormCachedIntegral_phi_th1_th2>* mass_helicity_angles_norm_cache_integrator_phi_th1_th2 = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral_phi_th1_th2>(debug_level, 0);
	CacheIntegrator<MassHelicityAnglesRealCachedIntegral_phi_th1_th2>* mass_helicity_angles_real_cache_integrator_phi_th1_th2 = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral_phi_th1_th2>(debug_level, 0);
	CacheIntegrator<MassHelicityAnglesImagCachedIntegral_phi_th1_th2>* mass_helicity_angles_imag_cache_integrator_phi_th1_th2 = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral_phi_th1_th2>(debug_level, 0);
	CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_th1_th2>* mass_helicity_angles_norm_cache_integrator_m_th1_th2 = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_th1_th2>(debug_level, 0);
	CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_th1_th2>* mass_helicity_angles_real_cache_integrator_m_th1_th2 = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_th1_th2>(debug_level, 0);
	CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_th1_th2>* mass_helicity_angles_imag_cache_integrator_m_th1_th2 = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_th1_th2>(debug_level, 0);
	CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_phi_th2>* mass_helicity_angles_norm_cache_integrator_m_phi_th2 = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_phi_th2>(debug_level, 0);
	CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_phi_th2>* mass_helicity_angles_real_cache_integrator_m_phi_th2 = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_phi_th2>(debug_level, 0);
	CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_phi_th2>* mass_helicity_angles_imag_cache_integrator_m_phi_th2 = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_phi_th2>(debug_level, 0);
	CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_phi_th1>* mass_helicity_angles_norm_cache_integrator_m_phi_th1 = new CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_phi_th1>(debug_level, 0);
	CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_phi_th1>* mass_helicity_angles_real_cache_integrator_m_phi_th1 = new CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_phi_th1>(debug_level, 0);
	CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_phi_th1>* mass_helicity_angles_imag_cache_integrator_m_phi_th1 = new CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_phi_th1>(debug_level, 0);

	std::string path_to_integrals = "./CachedIntegrals_ACCEPTANCE" + FFUtil::GetAcceptanceString(acceptance) + "/";
	mp::execute_command("mkdir -p " + path_to_integrals);
	mass_helicity_angles_norm_cache_integrator->LoadIntegrals(path_to_integrals);
	mass_helicity_angles_real_cache_integrator->LoadIntegrals(path_to_integrals);
	mass_helicity_angles_imag_cache_integrator->LoadIntegrals(path_to_integrals);

	MHI_PDG_constants mhi_pdg_constants = {m_Pi, m_K, m_Phi, m_B0};
	MHI_CacheIntegrators_4D mhi_4D = {
			mass_helicity_angles_norm_cache_integrator,
			mass_helicity_angles_real_cache_integrator,
			mass_helicity_angles_imag_cache_integrator
	};
	MHI_CacheIntegrators_3D_phi_th1_th2 mhi_3D_phi_th1_th2 = {
			mass_helicity_angles_norm_cache_integrator_phi_th1_th2,
			mass_helicity_angles_real_cache_integrator_phi_th1_th2,
			mass_helicity_angles_imag_cache_integrator_phi_th1_th2
	};
	MHI_CacheIntegrators_3D_m_th1_th2 mhi_3D_m_th1_th2 = {
			mass_helicity_angles_norm_cache_integrator_m_th1_th2,
			mass_helicity_angles_real_cache_integrator_m_th1_th2,
			mass_helicity_angles_imag_cache_integrator_m_th1_th2
	};
	MHI_CacheIntegrators_3D_m_phi_th2 mhi_3D_m_phi_th2 = {
			mass_helicity_angles_norm_cache_integrator_m_phi_th2,
			mass_helicity_angles_real_cache_integrator_m_phi_th2,
			mass_helicity_angles_imag_cache_integrator_m_phi_th2
	};
	MHI_CacheIntegrators_3D_m_phi_th1 mhi_3D_m_phi_th1 = {
			mass_helicity_angles_norm_cache_integrator_m_phi_th1,
			mass_helicity_angles_real_cache_integrator_m_phi_th1,
			mass_helicity_angles_imag_cache_integrator_m_phi_th1
	};

	RooArgSet varset(mbc,deltae,chil0_ma,nb_prime,mass_kpi,hel_phi,hel_th1,hel_th2,flavor);

	PrintTimestamp();
	PrintFitStartBlock("Setup PDF",2);

	std::vector<std::string> flags_channel;
	flags_channel.push_back("C");
	tuple_category_name_flags category_name_flags_channel = std::make_tuple(&channel,"channel",flags_channel);

	shared_ptr<FF_PDF> pdf_bkg_continuum_9D = make_shared<FF_PDF_bkg_continuum_9D_mbc_deltae_chil0_ma_mass_kpi_hel_phi_hel_th1_hel_th2_nb_prime_flavor>(mbc,
			deltae, chil0_ma, mass_kpi, hel_phi, hel_th1,
			hel_th2, nb_prime, flavor, category_name_flags_channel, false, (RooRealVar*)NULL);
	pdf_bkg_continuum_9D->SetVariablesConst();
	shared_ptr<FF_PDF> pdf_signal_4D = 	make_shared<FF_PDF_signal_4D_mbc_deltae_chil0_ma_nb_prime>(mbc,
			deltae, chil0_ma, nb_prime, category_name_flags_channel,false,pdf_bkg_continuum_9D->GetVariable("chil0_ma_resolution"));
	pdf_signal_4D->SetVariablesConst();
	shared_ptr<FF_PDF> pdf_signal_5D_angles = make_shared<FF_PDF_signal_5D_mass_helicity_angles_CPV>(mass_kpi,
			hel_th1, hel_th2, hel_phi, flavor, acceptance,
			mhi_pdg_constants, mhi_4D,
			mhi_3D_phi_th1_th2, mhi_3D_m_th1_th2, mhi_3D_m_phi_th2, mhi_3D_m_phi_th1,
			category_name_flags_channel);
	shared_ptr<FF_PDF> pdf_signal_9D = make_shared<FF_PDF_signal_9D>(pdf_signal_4D, pdf_signal_5D_angles,
			category_name_flags_channel);
	shared_ptr<FF_PDF> pdf_bkg_bbar_wrongphikkbar_9D = make_shared<FF_PDF_bkg_bbar_wrongphif0980_9D>(mbc,
			deltae, chil0_ma, nb_prime, mass_kpi,
			hel_phi, hel_th1, hel_th2, flavor, category_name_flags_channel, true);
	pdf_bkg_bbar_wrongphikkbar_9D->SetVariablesConst();

	shared_ptr<FF_PDF> pdf_data_9D = make_shared<FF_PDF_data_9D__CPV__KK_Flatte>(pdf_signal_9D,
			pdf_bkg_continuum_9D,
			pdf_bkg_bbar_wrongphikkbar_9D, 20000.0, category_name_flags_channel);

	std::cout << "Reading configuration from " << config_file << std::endl;
	namespace bpt = boost::property_tree;
	bpt::ptree pt;
	bpt::info_parser::read_info(config_file,pt);

	pdf_data_9D->GetVariable("signal_nevents")->setVal(pt.get<double>("signal_nevents"));
	pdf_data_9D->GetVariable("C_bkg_continuum_nevents")->setMin(0.0);
	pdf_data_9D->GetVariable("C_bkg_continuum_nevents")->setVal(pt.get<double>("C_bkg_continuum_nevents"));
	pdf_data_9D->GetVariable("C_bkg_bbar_wrongphif0980_nevents")->setMin(0.0);
	pdf_data_9D->GetVariable("C_bkg_bbar_wrongphif0980_nevents")->setVal(pt.get<double>("C_bkg_bbar_wrongphif0980_nevents"));

	// reset to simple values (amps 1.0, phases 0.0)
	pdf_data_9D->GetVariable("amp_00")->setVal(pt.get<double>("amp_00"));
	pdf_data_9D->GetVariable("amp_10")->setVal(pt.get<double>("amp_10"));
	pdf_data_9D->GetVariable("amp_1para")->setVal(pt.get<double>("amp_1para"));
	pdf_data_9D->GetVariable("amp_1perp")->setVal(pt.get<double>("amp_1perp"));
	pdf_data_9D->GetVariable("amp_20")->setVal(pt.get<double>("amp_20"));
	pdf_data_9D->GetVariable("amp_2para")->setVal(pt.get<double>("amp_2para"));
	pdf_data_9D->GetVariable("amp_2perp")->setVal(pt.get<double>("amp_2perp"));

	pdf_data_9D->GetVariable("phase_00")->setVal(pt.get<double>("phase_00"));
	pdf_data_9D->GetVariable("phase_10")->setVal(pt.get<double>("phase_10"));
	pdf_data_9D->GetVariable("phase_1para")->setVal(pt.get<double>("phase_1para"));
	pdf_data_9D->GetVariable("phase_1perp")->setVal(pt.get<double>("phase_1perp"));
	pdf_data_9D->GetVariable("phase_20")->setVal(pt.get<double>("phase_20"));
	pdf_data_9D->GetVariable("phase_2para")->setVal(pt.get<double>("phase_2para"));
	pdf_data_9D->GetVariable("phase_2perp")->setVal(pt.get<double>("phase_2perp"));

	pdf_data_9D->GetVariable("amp_00_delta")->setVal(pt.get<double>("amp_00_delta"));
	pdf_data_9D->GetVariable("amp_10_delta")->setVal(pt.get<double>("amp_10_delta"));
	pdf_data_9D->GetVariable("amp_1para_delta")->setVal(pt.get<double>("amp_1para_delta"));
	pdf_data_9D->GetVariable("amp_1perp_delta")->setVal(pt.get<double>("amp_1perp_delta"));
	pdf_data_9D->GetVariable("amp_20_delta")->setVal(pt.get<double>("amp_20_delta"));
	pdf_data_9D->GetVariable("amp_2para_delta")->setVal(pt.get<double>("amp_2para_delta"));
	pdf_data_9D->GetVariable("amp_2perp_delta")->setVal(pt.get<double>("amp_2perp_delta"));

	pdf_data_9D->GetVariable("phase_00_delta")->setVal(pt.get<double>("phase_00_delta"));
	pdf_data_9D->GetVariable("phase_10_delta")->setVal(pt.get<double>("phase_10_delta"));
	pdf_data_9D->GetVariable("phase_1para_delta")->setVal(pt.get<double>("phase_1para_delta"));
	pdf_data_9D->GetVariable("phase_1perp_delta")->setVal(pt.get<double>("phase_1perp_delta"));
	pdf_data_9D->GetVariable("phase_20_delta")->setVal(pt.get<double>("phase_20_delta"));
	pdf_data_9D->GetVariable("phase_2para_delta")->setVal(pt.get<double>("phase_2para_delta"));
	pdf_data_9D->GetVariable("phase_2perp_delta")->setVal(pt.get<double>("phase_2perp_delta"));

	PrintFitStartBlock("Compute expected events",2);

	double expected = pdf_data_9D->GetPDF("C").expectedEvents(varset);
	std::cout << "Expected events in signal pdf: " << expected << std::endl;
	double asym = std::dynamic_pointer_cast<FF_PDF_signal_9D>(pdf_signal_9D)->getAsymmetry();
	std::cout << "Expected asymmetry in signal pdf: " << asym << std::endl;
	double expected_CP = expected / (1 + (1 - asym)/(1 + asym));
	double expected_CA = expected - expected_CP;
	std::cout << "Expected events in signal pdf for CP: " << expected_CP << std::endl;
	std::cout << "Expected events in signal pdf for CA: " << expected_CA << std::endl;

	PrintFitStartBlock("Compute physics parameters",2);

	std::map<std::string,double> results = ComputePhysicsParameters(pdf_data_9D, pdf_signal_9D, varset, true);

	std::cout << mp::map_to_string(results) << std::endl;

	PrintFitStartBlock("Compute normalization integrals",2);
	std::array<std::string, 3> ranges = {{"full_range", "full_phsp_range", "full_LASS_range"}};
	for(const std::string& r : ranges) {
		std::cout << "Currently using acceptance function: " << FFUtil::GetAcceptanceString(acceptance) << "\n";
		RooAbsReal* I_tot = pdf_signal_9D->GetPDF("C").createIntegral(varset,r.c_str());

		pdf_data_9D->GetVariable("amp_00")->setVal(pt.get<double>("amp_00"));
		pdf_data_9D->GetVariable("amp_00_delta")->setVal(pt.get<double>("amp_00_delta"));
		pdf_data_9D->GetVariable("amp_10")->setVal(pt.get<double>("amp_10"));
		pdf_data_9D->GetVariable("amp_10_delta")->setVal(pt.get<double>("amp_10_delta"));
		pdf_data_9D->GetVariable("amp_1para")->setVal(pt.get<double>("amp_1para"));
		pdf_data_9D->GetVariable("amp_1para_delta")->setVal(pt.get<double>("amp_1para_delta"));
		pdf_data_9D->GetVariable("amp_1perp")->setVal(pt.get<double>("amp_1perp"));
		pdf_data_9D->GetVariable("amp_1perp_delta")->setVal(pt.get<double>("amp_1perp_delta"));
		pdf_data_9D->GetVariable("amp_20")->setVal(pt.get<double>("amp_20"));
		pdf_data_9D->GetVariable("amp_20_delta")->setVal(pt.get<double>("amp_20_delta"));
		pdf_data_9D->GetVariable("amp_2para")->setVal(pt.get<double>("amp_2para"));
		pdf_data_9D->GetVariable("amp_2para_delta")->setVal(pt.get<double>("amp_2para_delta"));
		pdf_data_9D->GetVariable("amp_2perp")->setVal(pt.get<double>("amp_2perp"));
		pdf_data_9D->GetVariable("amp_2perp_delta")->setVal(pt.get<double>("amp_2perp_delta"));
		double N_tot = I_tot->getVal();
		std::cout << "Integral over signal PDF in the " << r << " range: " << N_tot << "\n";

		pdf_signal_9D->GetVariable("amp_00")->setVal(pt.get<double>("amp_00"));
		pdf_signal_9D->GetVariable("amp_00_delta")->setVal(pt.get<double>("amp_00_delta"));
		pdf_signal_9D->GetVariable("amp_10")->setVal(0.0);
		pdf_signal_9D->GetVariable("amp_10_delta")->setVal(0.0);
		pdf_signal_9D->GetVariable("amp_1perp")->setVal(0.0);
		pdf_signal_9D->GetVariable("amp_1perp_delta")->setVal(0.0);
		pdf_signal_9D->GetVariable("amp_1para")->setVal(0.0);
		pdf_signal_9D->GetVariable("amp_1para_delta")->setVal(0.0);
		pdf_signal_9D->GetVariable("amp_20")->setVal(0.0);
		pdf_signal_9D->GetVariable("amp_20_delta")->setVal(0.0);
		pdf_signal_9D->GetVariable("amp_2perp")->setVal(0.0);
		pdf_signal_9D->GetVariable("amp_2perp_delta")->setVal(0.0);
		pdf_signal_9D->GetVariable("amp_2para")->setVal(0.0);
		pdf_signal_9D->GetVariable("amp_2para_delta")->setVal(0.0);
		double N_LASS = I_tot->getVal();
		std::cout << "Integral over LASS PDF in the " << r << " range: " << N_LASS << "\n";

		pdf_signal_9D->GetVariable("amp_00")->setVal(0.0);
		pdf_signal_9D->GetVariable("amp_00_delta")->setVal(0.0);
		pdf_signal_9D->GetVariable("amp_10")->setVal(pt.get<double>("amp_10"));
		pdf_signal_9D->GetVariable("amp_10_delta")->setVal(pt.get<double>("amp_10_delta"));
		pdf_signal_9D->GetVariable("amp_1para")->setVal(pt.get<double>("amp_1para"));
		pdf_signal_9D->GetVariable("amp_1para_delta")->setVal(pt.get<double>("amp_1para_delta"));
		pdf_signal_9D->GetVariable("amp_1perp")->setVal(pt.get<double>("amp_1perp"));
		pdf_signal_9D->GetVariable("amp_1perp_delta")->setVal(pt.get<double>("amp_1perp_delta"));
		double N_K892 = I_tot->getVal();
		std::cout << "Integral over K892 PDF in the " << r << " range: " << N_K892 << "\n";

		pdf_signal_9D->GetVariable("amp_10")->setVal(0.0);
		pdf_signal_9D->GetVariable("amp_10_delta")->setVal(0.0);
		pdf_signal_9D->GetVariable("amp_1perp")->setVal(0.0);
		pdf_signal_9D->GetVariable("amp_1perp_delta")->setVal(0.0);
		pdf_signal_9D->GetVariable("amp_1para")->setVal(0.0);
		pdf_signal_9D->GetVariable("amp_1para_delta")->setVal(0.0);
		pdf_signal_9D->GetVariable("amp_20")->setVal(pt.get<double>("amp_20"));
		pdf_signal_9D->GetVariable("amp_20_delta")->setVal(pt.get<double>("amp_20_delta"));
		pdf_signal_9D->GetVariable("amp_2para")->setVal(pt.get<double>("amp_2para"));
		pdf_signal_9D->GetVariable("amp_2para_delta")->setVal(pt.get<double>("amp_2para_delta"));
		pdf_signal_9D->GetVariable("amp_2perp")->setVal(pt.get<double>("amp_2perp"));
		pdf_signal_9D->GetVariable("amp_2perp_delta")->setVal(pt.get<double>("amp_2perp_delta"));
		double N_K21430 = I_tot->getVal();
		std::cout << "Integral over K2(2430) PDF in the " << r << " range: " << N_K21430 << "\n";

		delete I_tot;
	}

	mass_helicity_angles_norm_cache_integrator->SaveIntegrals(path_to_integrals);
	mass_helicity_angles_real_cache_integrator->SaveIntegrals(path_to_integrals);
	mass_helicity_angles_imag_cache_integrator->SaveIntegrals(path_to_integrals);

	PrintTimestamp();
}

}
