/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
 * This code was autogenerated by RooClassFactory                            *
 *****************************************************************************/

#ifndef ROODALITZNOVOSIBIRSK
#define ROODALITZNOVOSIBIRSK

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"

class RooDalitzNovosibirsk : public RooAbsPdf {
public:
	RooDalitzNovosibirsk() :  comb(0), m1(0), m2(0), m3(0), mM(0) {} ;
	RooDalitzNovosibirsk(const char *name, const char *title,
			RooAbsReal& _m12,
			RooAbsReal& _m23,
			RooAbsReal& _mean,
			RooAbsReal& _sigma,
			RooAbsReal& _tail,
            RooAbsReal& _c1,
            RooAbsReal& _c2,
            RooAbsReal& _c3,
            RooAbsReal& _c4,
            int _comb,
			double _m1,
			double _m2,
			double _m3,
			double _mM);
	RooDalitzNovosibirsk(const RooDalitzNovosibirsk& other, const char* name=0) ;
	virtual TObject* clone(const char* newname) const { return new RooDalitzNovosibirsk(*this,newname); }
	inline virtual ~RooDalitzNovosibirsk() { }
    
protected:

	RooRealProxy m12;
	RooRealProxy m23;
	RooRealProxy mean;
	RooRealProxy sigma;
	RooRealProxy tail;
	RooRealProxy c1;
	RooRealProxy c2;
	RooRealProxy c3;
	RooRealProxy c4;
    const int comb;
	const double m1;
	const double m2;
	const double m3;
	const double mM;

	Double_t evaluate() const;

private:

	ClassDef(RooDalitzNovosibirsk,1) // Your description goes here...
};

#endif
