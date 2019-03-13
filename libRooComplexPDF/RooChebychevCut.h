/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitModels                                                     *
 *    File: $Id: RooChebychev.h,v 1.6 2007/05/11 09:13:07 verkerke Exp $
 * Authors:                                                                  *
 *   GR, Gerhard Raven,   UC San Diego, Gerhard.Raven@slac.stanford.edu
 *                                                                           *
 * Copyright (c) 2000-2005, Regents of the University of California          *
 *                          and Stanford University. All rights reserved.    *
 *                                                                           *
 * Redistribution and use in source and binary forms,                        *
 * with or without modification, are permitted according to the terms        *
 * listed in LICENSE (http://roofit.sourceforge.net/license.txt)             *
 *****************************************************************************/
#ifndef ROO_CHEBYCHEV_CUT
#define ROO_CHEBYCHEV_CUT

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooListProxy.h"

class RooRealVar;
class RooArgList ;

class RooChebychevCut : public RooAbsPdf {
public:

	RooChebychevCut();
	RooChebychevCut(const char *name, const char *title,
			RooAbsReal& _x, const RooArgList& _coefList, const double cut_value) ;

	RooChebychevCut(const RooChebychevCut& other, const char* name = 0);
	virtual TObject* clone(const char* newname) const { return new RooChebychevCut(*this, newname); }
	inline virtual ~RooChebychevCut() { }

	Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName=0) const ;
	Double_t analyticalIntegral(Int_t code, const char* rangeName=0) const ;

private:

	RooRealProxy _x;
	RooListProxy _coefList;

	double _cut_value;

	Double_t evaluate() const;

	ClassDef(RooChebychevCut,1) // Chebychev polynomial PDF with Cut
};

#endif
