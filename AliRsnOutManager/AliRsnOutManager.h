// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    23 Mar 2012

#ifndef ALIRSNOUTMANAGER_H
#define ALIRSNOUTMANAGER_H

#include <TObject.h>

class TFile;

class AliRsnOutGroup;

class AliRsnOutManager : public TObject {

public:
  enum EAxisType { kInvMass = 0, kPt = 1, kEta = 2 };

  AliRsnOutManager();
  AliRsnOutManager(const AliRsnOutManager &copy);
  AliRsnOutManager &operator=(const AliRsnOutManager &other);
  virtual      ~AliRsnOutManager();

  void          MakeGroup(const char *fname, const char *lname);
  void          FindIntervals() const;

private:
  TList        *fGroups;            //->list of all groups

  ClassDef(AliRsnOutManager, 1) // AliRsnOutManager class
};

#endif
