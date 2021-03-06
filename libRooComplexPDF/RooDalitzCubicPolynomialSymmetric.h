/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
 * This code was autogenerated by RooClassFactory                            *
 *****************************************************************************/

#ifndef ROODALITZCUBICPOLYNOMIALSYMMETRIC
#define ROODALITZCUBICPOLYNOMIALSYMMETRIC

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"

class RooDalitzCubicPolynomialSymmetric : public RooAbsPdf {
public:
	RooDalitzCubicPolynomialSymmetric() : m1(0), m2(0), m3(0), mM(0), m12_offset(0), m23_offset(0) {} ;
	RooDalitzCubicPolynomialSymmetric(const char *name, const char *title,
			RooAbsReal& _m12,
			RooAbsReal& _m23,
			RooAbsReal& _E_1,
			RooAbsReal& _E_2,
			RooAbsReal& _E_3,
			RooAbsReal& _E_xy,
			RooAbsReal& _E_xyn,
			double _m1,
			double _m2,
			double _m3,
			double _mM,
			double _m12_offset = 0.0,
			double _m23_offset = 0.0);
	RooDalitzCubicPolynomialSymmetric(const RooDalitzCubicPolynomialSymmetric& other, const char* name=0) ;
	virtual TObject* clone(const char* newname) const { return new RooDalitzCubicPolynomialSymmetric(*this,newname); }
	inline virtual ~RooDalitzCubicPolynomialSymmetric() { }

protected:

	RooRealProxy m12 ;
	RooRealProxy m23 ;
	RooRealProxy E_1 ;
	RooRealProxy E_2 ;
	RooRealProxy E_3 ;
	RooRealProxy E_xy ;
	RooRealProxy E_xyn ;
	const double m1;
	const double m2;
	const double m3;
	const double mM;
	const double m12_offset;
	const double m23_offset;

	Double_t evaluate() const ;

private:

	ClassDef(RooDalitzCubicPolynomialSymmetric,1) // Your description goes here...
};

#endif
