// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    23 Mar 2012

#ifndef ALIRSNOUTGROUP_H
#define ALIRSNOUTGROUP_H

#include "AliRsnOutManager.h"
#include <TNamed.h>


class THnBase;

class AliRsnOutGroup: public TNamed {

public:
  AliRsnOutGroup();
  AliRsnOutGroup(const char *name, const char *title);
  AliRsnOutGroup(const char *pre, const char *rsn, const char *cut);
  AliRsnOutGroup(const char *pre, const char *rsn, const char *cut, const char *pair);
  virtual     ~AliRsnOutGroup();

  TList       *Fragments() const { return fFragments; }

  void         SetDelimitedNames(const char *pre, const char *rsn, const char *cut);

  void         SetFileName(const char *fname) { fFileName = fname; }
  const char  *GetFileName() const { return fFileName.Data(); }
  Int_t        MakeFragments(const THnBase *sparse, Double_t min = 0.1);
  void         SetSignal();
  void         SetBackground();
  void         Addbackground();

private:
  TString      fPrefix;
  TString      fRsnName;
  TString      fCutName;
  TString      fPairName;
  TString      fFileName;
  TList       *fList;           //->list of fragments (asi skor ako groups)
  TList       *fFragments;
  //  AliRsnOutManager::EAxisType fFragmentType;


  ClassDef(AliRsnOutGroup, 1) // AliRsnOutGroup class
};

#endif
