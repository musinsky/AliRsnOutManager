// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    23 Mar 2012

#ifndef ALIRSNOUTGROUP_H
#define ALIRSNOUTGROUP_H

#include <TNamed.h>

class AliRsnOutGroup: public TNamed {

public:
  AliRsnOutGroup();
  AliRsnOutGroup(TList *l);
  AliRsnOutGroup(const AliRsnOutGroup &copy);
  AliRsnOutGroup &operator=(const AliRsnOutGroup &other);
  virtual     ~AliRsnOutGroup();

  TList       *GetList() const { return fList; }

  void         SetFileName(const char *fname) { fFileName = fname; }
  const char  *GetFileName() const { return fFileName.Data(); }
  void         SetSignal();
  void         SetBackground();
  void         Addbackground();

private:
  TString      fFileName;
  TList       *fList;           //->list of groups

  ClassDef(AliRsnOutGroup, 1) // AliRsnOutGroup class
};

#endif
