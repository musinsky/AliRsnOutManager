// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    12 June 2012

#ifndef ALIRSNOUTAXIS_H
#define ALIRSNOUTAXIS_H

#include <TObject.h>

class THnBase;

enum EAxisDim { kInvMass = 0, kPt = 1, kEta = 2 };

class AliRsnOutAxis: public TObject {

public:
  AliRsnOutAxis();
  AliRsnOutAxis(Int_t dimension);
  virtual ~AliRsnOutAxis();

  Bool_t        IsRange() const { return TestBit(kRange); }
  Bool_t        IsProjection() const { return TestBit(kProjection); }
  Int_t         GetDim() const { return fDim; }
  Double_t      GetRangeMin() const { return fRangeMin; }
  Double_t      GetRangeMax() const { return fRangeMax; }
  Double_t      GetRangeMean() const { return (fRangeMin + fRangeMax)/2.0; }
  Bool_t        RangeChecked() const { return fRangeChecked; }

  virtual const char *GetName() const;
  virtual const char *GetTitle() const;
  virtual void        Print(Option_t *option = "") const;

  void          AllocateRange(Int_t bmin, Int_t bmax);
  void          AllocateRangeUser(Double_t min, Double_t max);
  void          AllocateProjection();
  void          SetRange(const THnBase *hnbase);

private:
  enum          { kRange = BIT(14), kProjection = BIT(15) };
  Int_t         fDim;           //
  Double_t      fRangeMin;      //
  Double_t      fRangeMax;      //
  Int_t         fRangeBinMin;   //
  Int_t         fRangeBinMax;   //
  Bool_t        fRangeChecked;  //

  ClassDef(AliRsnOutAxis, 1) // AliRsnOutAxis class
};

#endif
