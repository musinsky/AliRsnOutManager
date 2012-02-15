// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    15 Feb 2012

#ifndef ALIRSNOUTMANAGER_H
#define ALIRSNOUTMANAGER_H

#include <TObject.h>

class AliRsnOutManager : public TObject {

public:
  AliRsnOutManager();
  AliRsnOutManager(const AliRsnOutManager &copy);
  AliRsnOutManager &operator=(const AliRsnOutManager &other);
  virtual     ~AliRsnOutManager();

  Int_t        GetBla() const { return fBla; }
  Int_t        GetDza() const { return fDza; }

private:
  Int_t        fBla;
  Int_t        fDza;

  ClassDef(AliRsnOutManager, 1) // AliRsnOutManager class
};

#endif
