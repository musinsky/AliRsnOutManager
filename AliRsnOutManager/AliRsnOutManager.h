// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    17 Feb 2012

#ifndef ALIRSNOUTMANAGER_H
#define ALIRSNOUTMANAGER_H

#include <TObject.h>

class TFile;

class AliRsnOutGroup;

class AliRsnOutManager : public TObject {

public:
  enum EAxisType { kInvMass, kPt, kEta };

  AliRsnOutManager();
  AliRsnOutManager(const AliRsnOutManager &copy);
  AliRsnOutManager &operator=(const AliRsnOutManager &other);
  virtual      ~AliRsnOutManager();

  Int_t         GetBla() const { return fBla; }
  Int_t         GetDza() const { return fDza; }

  void          SetFileName(const char *fname);
  void          MakeGroup(const char *lname);
  void          FindIntervals() const;

private:
  Int_t         fBla;
  Int_t         fDza;
  TFile        *fFile;              //! sparse file
  TList        *fGroups;            //->list of groups

  ClassDef(AliRsnOutManager, 1) // AliRsnOutManager class
};

#endif
