/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
 * This code was autogenerated by RooClassFactory                            *
 *****************************************************************************/

#ifndef ROOMASSHELICITYANGLESCPV
#define ROOMASSHELICITYANGLESCPV

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
#include "RooMassHelicityAngles.h"

// parameters used for channel == 1 ("CA") of RooMassHelicityAnglesCPV
struct MHI_CC_parameters {
	RooAbsReal* _amp_00_CC;
	RooAbsReal* _phase_00_CC;
	RooAbsReal* _amp_10_CC;
	RooAbsReal* _phase_10_CC;
	RooAbsReal* _amp_1para_CC;
	RooAbsReal* _phase_1para_CC;
	RooAbsReal* _amp_1perp_CC;
	RooAbsReal* _phase_1perp_CC;
	RooAbsReal* _amp_20_CC;
	RooAbsReal* _phase_20_CC;
	RooAbsReal* _amp_2para_CC;
	RooAbsReal* _phase_2para_CC;
	RooAbsReal* _amp_2perp_CC;
	RooAbsReal* _phase_2perp_CC;
};

class RooMassHelicityAnglesCPV : public RooAbsPdf {
public:
	RooMassHelicityAnglesCPV() : 	m_Pi(0), m_K(0), m_Phi(0), m_B0(0), m_acceptance_function(UNIFORM),
			m_norm_cache_integrator(NULL), m_real_cache_integrator(NULL),
			m_imag_cache_integrator(NULL), m_norm_cache_integrator_phi_th1_th2(NULL),
			m_real_cache_integrator_phi_th1_th2(NULL), m_imag_cache_integrator_phi_th1_th2(NULL),
			m_norm_cache_integrator_m_th1_th2(NULL), m_real_cache_integrator_m_th1_th2(NULL),
			m_imag_cache_integrator_m_th1_th2(NULL), m_norm_cache_integrator_m_phi_th2(NULL),
			m_real_cache_integrator_m_phi_th2(NULL), m_imag_cache_integrator_m_phi_th2(NULL),
			m_norm_cache_integrator_m_phi_th1(NULL), m_real_cache_integrator_m_phi_th1(NULL),
			m_imag_cache_integrator_m_phi_th1(NULL) {
	};
	RooMassHelicityAnglesCPV(const char *name, const char *title,
			RooAbsReal& _m,
			RooAbsReal& _phi,
			RooAbsReal& _cos_th1,
			RooAbsReal& _cos_th2,
			RooAbsCategory& _channel,
			RooAbsReal& _mass_0,
			RooAbsReal& _width_0,
			RooAbsReal& _a,
			RooAbsReal& _b,
			RooAbsReal& _mass_1,
			RooAbsReal& _width_1,
			RooAbsReal& _r_1,
			RooAbsReal& _mass_2,
			RooAbsReal& _width_2,
			RooAbsReal& _r_2,
			RooAbsReal& _amp_00,
			RooAbsReal& _phase_00,
			RooAbsReal& _amp_10,
			RooAbsReal& _phase_10,
			RooAbsReal& _amp_1para,
			RooAbsReal& _phase_1para,
			RooAbsReal& _amp_1perp,
			RooAbsReal& _phase_1perp,
			RooAbsReal& _amp_20,
			RooAbsReal& _phase_20,
			RooAbsReal& _amp_2para,
			RooAbsReal& _phase_2para,
			RooAbsReal& _amp_2perp,
			RooAbsReal& _phase_2perp,
			ACCEPTANCE_FUNCTION _acceptance_function,
			MHI_CC_parameters _s_cc_parameters,
			MHI_PDG_constants _s_PDG,
			MHI_CacheIntegrators_4D _s_4D,
			MHI_CacheIntegrators_3D_phi_th1_th2 _s_3D_phi_th1_th2,
			MHI_CacheIntegrators_3D_m_th1_th2 _s_3D_m_th1_th2,
			MHI_CacheIntegrators_3D_m_phi_th2 _s_3D_m_phi_th2,
			MHI_CacheIntegrators_3D_m_phi_th1 _s_3D_m_phi_th1);
	RooMassHelicityAnglesCPV(const RooMassHelicityAnglesCPV& other, const char* name=0);
	virtual TObject* clone(const char* newname) const {
		return new RooMassHelicityAnglesCPV(*this,newname);
	}
	inline virtual ~RooMassHelicityAnglesCPV() { };

	virtual Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName=0) const;
	virtual Double_t analyticalIntegral(Int_t code, const char* rangeName=0) const;

	Double_t getAsymmetry(const char* rangeName=0) const;

protected:

	RooRealProxy m;
	RooRealProxy phi;
	RooRealProxy cos_th1;
	RooRealProxy cos_th2;
	RooCategoryProxy channel;
	RooRealProxy mass_0;
	RooRealProxy width_0;
	RooRealProxy a;
	RooRealProxy b;
	RooRealProxy mass_1;
	RooRealProxy width_1;
	RooRealProxy r_1;
	RooRealProxy mass_2;
	RooRealProxy width_2;
	RooRealProxy r_2;
	RooRealProxy amp_00;
	RooRealProxy phase_00;
	RooRealProxy amp_10;
	RooRealProxy phase_10;
	RooRealProxy amp_1para;
	RooRealProxy phase_1para;
	RooRealProxy amp_1perp;
	RooRealProxy phase_1perp;
	RooRealProxy amp_20;
	RooRealProxy phase_20;
	RooRealProxy amp_2para;
	RooRealProxy phase_2para;
	RooRealProxy amp_2perp;
	RooRealProxy phase_2perp;
	RooRealProxy amp_00_CC;
	RooRealProxy phase_00_CC;
	RooRealProxy amp_10_CC;
	RooRealProxy phase_10_CC;
	RooRealProxy amp_1para_CC;
	RooRealProxy phase_1para_CC;
	RooRealProxy amp_1perp_CC;
	RooRealProxy phase_1perp_CC;
	RooRealProxy amp_20_CC;
	RooRealProxy phase_20_CC;
	RooRealProxy amp_2para_CC;
	RooRealProxy phase_2para_CC;
	RooRealProxy amp_2perp_CC;
	RooRealProxy phase_2perp_CC;

	const double m_Pi;
	const double m_K;
	const double m_Phi;
	const double m_B0;

	const ACCEPTANCE_FUNCTION m_acceptance_function;

	//! in begin of comments prevents ROOT to try to serialize it...
	CacheIntegrator<MassHelicityAnglesNormCachedIntegral> *m_norm_cache_integrator; //! don't serialize
	CacheIntegrator<MassHelicityAnglesRealCachedIntegral> *m_real_cache_integrator; //! don't serialize
	CacheIntegrator<MassHelicityAnglesImagCachedIntegral> *m_imag_cache_integrator; //! don't serialize
	CacheIntegrator<MassHelicityAnglesNormCachedIntegral_phi_th1_th2> *m_norm_cache_integrator_phi_th1_th2; //! don't serialize
	CacheIntegrator<MassHelicityAnglesRealCachedIntegral_phi_th1_th2> *m_real_cache_integrator_phi_th1_th2; //! don't serialize
	CacheIntegrator<MassHelicityAnglesImagCachedIntegral_phi_th1_th2> *m_imag_cache_integrator_phi_th1_th2; //! don't serialize
	CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_th1_th2> *m_norm_cache_integrator_m_th1_th2; //! don't serialize
	CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_th1_th2> *m_real_cache_integrator_m_th1_th2; //! don't serialize
	CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_th1_th2> *m_imag_cache_integrator_m_th1_th2; //! don't serialize
	CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_phi_th2> *m_norm_cache_integrator_m_phi_th2; //! don't serialize
	CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_phi_th2> *m_real_cache_integrator_m_phi_th2; //! don't serialize
	CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_phi_th2> *m_imag_cache_integrator_m_phi_th2; //! don't serialize
	CacheIntegrator<MassHelicityAnglesNormCachedIntegral_m_phi_th1> *m_norm_cache_integrator_m_phi_th1; //! don't serialize
	CacheIntegrator<MassHelicityAnglesRealCachedIntegral_m_phi_th1> *m_real_cache_integrator_m_phi_th1; //! don't serialize
	CacheIntegrator<MassHelicityAnglesImagCachedIntegral_m_phi_th1> *m_imag_cache_integrator_m_phi_th1; //! don't serialize

	Double_t evaluate() const ;

	std::pair<Double_t, Double_t> analyticalIntegralAndAsymmetry(Int_t code, const char* rangeName=0) const;

private:

	ClassDef(RooMassHelicityAnglesCPV,1) // Your description goes here...
};

#endif
