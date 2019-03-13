/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
  * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/

#ifndef ROORELATIVISTICSPINBREITWIGNER
#define ROORELATIVISTICSPINBREITWIGNER

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"

class RooRelativisticSpinBreitWigner : public RooAbsPdf {
public:
	RooRelativisticSpinBreitWigner() : m1(0), m2(0) { };
	RooRelativisticSpinBreitWigner(const char *name, const char *title,
			RooAbsReal& _m,
			RooAbsReal& _mass_J,
			RooAbsReal& _width_J,
			RooAbsReal& _r,
			RooAbsReal& _J,
			double _m1,
			double _m2);
	RooRelativisticSpinBreitWigner(const RooRelativisticSpinBreitWigner& other, const char* name=0);
	virtual TObject* clone(const char* newname) const {
		return new RooRelativisticSpinBreitWigner(*this,newname);
	}
	inline virtual ~RooRelativisticSpinBreitWigner() { };

	double CalculateMomentum(const double m) const;

protected:

	RooRealProxy m;
	RooRealProxy mass_J;
	RooRealProxy width_J;
	RooRealProxy r;
	RooRealProxy J;

	// store inv. mass of 1st decay product
	const double m1;
	// store inv. mass of 2nd decay prodcut
	const double m2;

	Double_t evaluate() const;

private:

	ClassDef(RooRelativisticSpinBreitWigner,1) // Your description goes here...
};

#endif