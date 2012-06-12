// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    14 May 2012

#ifndef ALIRSNOUTMANAGER_H
#define ALIRSNOUTMANAGER_H

#include <TObject.h>

class TFile;
class THnSparse;

class AliRsnOutGroup;

class AliRsnOutManager : public TObject {

public:
  enum EAxisType { kInvMass = 0, kPt = 1, kEta = 2 };

  AliRsnOutManager();
  AliRsnOutManager(const AliRsnOutManager &copy);
  AliRsnOutManager &operator=(const AliRsnOutManager &other);
  virtual      ~AliRsnOutManager();

  TList        *Groups() const { return fGroups; }

  void          ScanFile(const char *fname);
  void          ScanList(TList *list);
  void          ScanSparse(THnSparse *sparse); // ParseSparse
  void          FindIntervals() const;

//  static const char *GetDelimiter() { return "."; }

private:
  TList        *fGroups;            //->list of all groups

  ClassDef(AliRsnOutManager, 1) // AliRsnOutManager class
};

#endif
