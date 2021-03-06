/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
 * This code was autogenerated by RooClassFactory                            *
 *****************************************************************************/

#ifndef ROOLASS
#define ROOLASS

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"

class RooLASS : public RooAbsPdf {
public:
	RooLASS() : m1(0), m2(0) { };
	RooLASS(const char *name, const char *title,
			RooAbsReal& _m,
			RooAbsReal& _mass_0,
			RooAbsReal& _width_0,
			RooAbsReal& _a,
			RooAbsReal& _b,
			double _m1,
			double _m2);
	RooLASS(const RooLASS& other, const char* name=0);
	virtual TObject* clone(const char* newname) const {
		return new RooLASS(*this,newname);
	}
	inline virtual ~RooLASS() { };

	double CalculateMomentum(const double m) const;

protected:

	RooRealProxy m;
	RooRealProxy mass_0;
	RooRealProxy width_0;
	RooRealProxy a;
	RooRealProxy b;

	// store inv. mass of 1st decay product
	const double m1;
	// store inv. mass of 2nd decay prodcut
	const double m2;

	Double_t evaluate() const;

private:

	ClassDef(RooLASS,1) // Your description goes here...
};

#endif
