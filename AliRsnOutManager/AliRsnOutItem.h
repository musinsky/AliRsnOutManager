// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    15 Feb 2012

#ifndef ALIRSNOUTITEM_H
#define ALIRSNOUTITEM_H

#include <TNamed.h>

class AliRsnOutItem: public TNamed {

public:
  AliRsnOutItem();
  AliRsnOutItem(const AliRsnOutItem &copy);
  AliRsnOutItem &operator=(const AliRsnOutItem &other);
  virtual     ~AliRsnOutItem();

private:
  Int_t        fBla;

  ClassDef(AliRsnOutItem, 1) // AliRsnOutItem class
};

#endif
