// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    17 Feb 2012

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

  void          SetSignal();
  void          SetBackground();
  void          Addbackground();

private:
  Int_t        fBla;
  TList       *fList;           //->list of groups

  ClassDef(AliRsnOutGroup, 1) // AliRsnOutGroup class
};

#endif
