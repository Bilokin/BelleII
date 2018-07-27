/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
  * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/

#ifndef DTBKG0
#define DTBKG0

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
 
class DTbkg0 : public RooAbsPdf {
public:
  DTbkg0() {} ; 
  DTbkg0(const char *name, const char *title,
	      RooAbsReal& _dt,
	      RooAbsReal& _tau,
	      RooAbsReal& _dw,
	      RooAbsReal& _w,
	      RooAbsCategory& _q);
  DTbkg0(const DTbkg0& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new DTbkg0(*this,newname); }
  inline virtual ~DTbkg0() { }

protected:

  RooRealProxy dt ;
  RooRealProxy tau ;
  RooRealProxy dw ;
  RooRealProxy w ;
  RooCategoryProxy q ;
  
  Double_t evaluate() const ;

private:

  ClassDef(DTbkg0,1) // Your description goes here...
};
 
#endif
