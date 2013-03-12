// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    12 Mar 2013

#ifndef ALIROMGRAPHERRORS_H
#define ALIROMGRAPHERRORS_H

#include <TGraphErrors.h>

class TMarker;

class AliROMGraphErrors: public TGraphErrors {

public:
  AliROMGraphErrors();
  AliROMGraphErrors(Int_t n);
  AliROMGraphErrors(Int_t n, const Float_t *x, const Float_t *y, const Float_t *ex = 0, const Float_t *ey = 0);
  AliROMGraphErrors(Int_t n, const Double_t *x, const Double_t *y, const Double_t *ex = 0, const Double_t *ey = 0);
  AliROMGraphErrors(const char *filename, const char *format = "%lg %lg %lg %lg", Option_t *option = "");
  virtual ~AliROMGraphErrors();

  virtual Int_t DistancetoPrimitive(Int_t px, Int_t py);
  virtual void  Draw(Option_t *chopt = "");
  virtual char *GetObjectInfo(Int_t px, Int_t py) const;

  void          SetShowHisto(Option_t *option = ""); // *MENU*
  void          ShowHisto(Option_t *option = "") const;
  void          FlashPoint(Bool_t flash);

protected:

private:
  Int_t         fFlashPoint;        // !
  TMarker      *fFlashMarker;       // !

  ClassDef(AliROMGraphErrors, 1) // AliROMGraphErrors class
};

#endif
