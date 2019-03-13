/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
 * This code was autogenerated by RooClassFactory                            *
 *****************************************************************************/

#ifndef ROOKPIMASS
#define ROOKPIMASS

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"

class RooKPiMass : public RooAbsPdf {
public:
	RooKPiMass() : m1(0), m2(0) { };
	RooKPiMass(const char *name, const char *title,
			RooAbsReal& _m,
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
			RooAbsReal& _amp_0,
			RooAbsReal& _phase_0,
			RooAbsReal& _amp_1,
			RooAbsReal& _phase_1,
			RooAbsReal& _amp_2,
			RooAbsReal& _phase_2,
			double _m1,
			double _m2);
	RooKPiMass(const RooKPiMass& other, const char* name=0);
	virtual TObject* clone(const char* newname) const {
		return new RooKPiMass(*this,newname);
	}
	inline virtual ~RooKPiMass() { };

	double CalculateMomentum(const double m) const;

protected:

	RooRealProxy m;
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
	RooRealProxy amp_0;
	RooRealProxy phase_0;
	RooRealProxy amp_1;
	RooRealProxy phase_1;
	RooRealProxy amp_2;
	RooRealProxy phase_2;

	// store inv. mass of 1st decay product
	const double m1;
	// store inv. mass of 2nd decay prodcut
	const double m2;

	Double_t evaluate() const;

private:

	ClassDef(RooKPiMass,1) // Your description goes here...
};

#endif
