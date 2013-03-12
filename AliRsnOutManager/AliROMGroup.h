// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    12 Mar 2013

#ifndef ALIROMGROUP_H
#define ALIROMGROUP_H

#include <TNamed.h>

class TObjArray;

class AliROMGroup: public TNamed {

public:
  AliROMGroup();
  AliROMGroup(const char *rsnname, const char *cutname, const char *prefix = "");
  virtual ~AliROMGroup();

  virtual void  Print(Option_t *option = "") const;

private:
  TString       fRsnName;   // !
  TString       fCutName;   // !
  TString       fPrefix;    // !

  ClassDef(AliROMGroup, 1) // AliROMGroup class
};

#endif
