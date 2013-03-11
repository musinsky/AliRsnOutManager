// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    11 Mar 2013

#ifndef ALIROMFRAGMENT_H
#define ALIROMFRAGMENT_H

#include <TNamed.h>

class TObjArray;
class TH1;

class AliROMFragment: public TNamed {

public:
  enum EPairKind { kSingle, kMix, kTrueMC, kGenMC };
  enum EPairType { kUnlike = 0, kUnlikePM = 1, kUnlikeMP = 2, kLike = 3, kLikePP = 4, kLikeMM = 5 };

  AliROMFragment();
  virtual ~AliROMFragment();

  virtual void  Print(Option_t *option = "") const;

  void          AddHisto(TH1 *h, EPairKind kind, EPairType type);
  TH1          *GetHisto(EPairKind kind, EPairType type);
  static void   HistoOwner(Bool_t owner) { fgHistoOwner = owner; }
  static Bool_t IsHistoOwner() { return fgHistoOwner; }

private:
  enum { kMaxPairType = 6 };

  TObjArray    *fHistoSingle;   // !
  TObjArray    *fHistoMix;      // !
  TObjArray    *fHistoTrueMC;   // !
  TObjArray    *fHistoGenMC;    // !
  static Bool_t fgHistoOwner;   // !

  TObjArray    *CreateHistoArray(const char *name, Int_t max);
  TObjArray    *GetHistoArray(EPairKind kind, Bool_t create = kFALSE);

  ClassDef(AliROMFragment, 1) // AliROMFragment class
};

#endif
