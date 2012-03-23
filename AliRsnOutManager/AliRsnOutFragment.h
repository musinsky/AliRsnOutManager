// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    23 Mar 2012

#ifndef ALIRSNOUTFRAGMENT_H
#define ALIRSNOUTFRAGMENT_H

#include <TNamed.h>

class AliRsnOutFragment: public TNamed {

public:
  AliRsnOutFragment();
  AliRsnOutFragment(const AliRsnOutFragment &copy);
  AliRsnOutFragment &operator=(const AliRsnOutFragment &other);
  virtual     ~AliRsnOutFragment();

  Int_t        GetValueType() const { return fValueType; }
  Double_t     GetValueMin() const { return fValueMin; }
  Double_t     GetValueMax() const { return fValueMax; }
  Double_t     GetValue() const { return (fValueMin + fValueMax)/2.0; }

private:
  Int_t        fValueType;   //
  Double_t     fValueMin;    //
  Double_t     fValueMax;    //

  ClassDef(AliRsnOutFragment, 1) // AliRsnOutFragment class
};

#endif
