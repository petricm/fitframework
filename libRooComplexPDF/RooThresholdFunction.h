/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
 * This code was autogenerated by RooClassFactory                            *
 *****************************************************************************/

#ifndef ROOTRESHOLDFUNCTION
#define ROOTRESHOLDFUNCTION

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"

class RooThresholdFunction : public RooAbsPdf {
public:
	RooThresholdFunction() : m12_offset(0), m23_offset(0), mM(0), m1(0), m2(0), m3(0) {} ;
	RooThresholdFunction(const char *name, const char *title,
                        RooAbsReal& _m12,
                        RooAbsReal& _m23,
                        RooAbsReal& _Ec1,
                        RooAbsReal& _Ec2,
                        RooAbsReal& _Ec3,
                        RooAbsReal& _Eth1,
                        RooAbsReal& _Eth2,
                        RooAbsReal& _Eth3,
                        RooAbsReal& _E_x,
                        RooAbsReal& _E_y,
                        RooAbsReal& _E_xx,
                        RooAbsReal& _E_xy,
                        RooAbsReal& _E_yy,
                        RooAbsReal& _E_xxx,
                        RooAbsReal& _E_xxy,
                        RooAbsReal& _E_xyy,
                        RooAbsReal& _E_yyy,
                        double _m12_offset,
                        double _m23_offset,
                        double _mM,
                        double _m1,
                        double _m2,
                        double _m3);
	RooThresholdFunction(const RooThresholdFunction& other, const char* name=0) ;
	virtual TObject* clone(const char* newname) const { return new RooThresholdFunction(*this,newname); }
	inline virtual ~RooThresholdFunction() { }

protected:

    RooRealProxy m12;
    RooRealProxy m23;
    RooRealProxy Ec1;
    RooRealProxy Ec2;
    RooRealProxy Ec3;
    RooRealProxy Eth1;
    RooRealProxy Eth2;
    RooRealProxy Eth3;
	RooRealProxy E_x;
	RooRealProxy E_y;
	RooRealProxy E_xx;
	RooRealProxy E_xy;
	RooRealProxy E_yy;
	RooRealProxy E_xxx;
	RooRealProxy E_xxy;
	RooRealProxy E_xyy;
	RooRealProxy E_yyy;
	const double m12_offset;
	const double m23_offset;
    double mM;
    double m1;
    double m2;
    double m3;

	Double_t evaluate() const;

private:

	ClassDef(RooThresholdFunction,1) // Your description goes here...
};

#endif
