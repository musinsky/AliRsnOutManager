// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    23 Mar 2012

#include <TFile.h>

#include "AliRsnOutManager.h"
#include "AliRsnOutGroup.h"

ClassImp(AliRsnOutManager)

//______________________________________________________________________________
AliRsnOutManager::AliRsnOutManager()
: TObject(),
  fGroups()
{
  // Default constructor
}
//______________________________________________________________________________
AliRsnOutManager::AliRsnOutManager(const AliRsnOutManager &copy)
: TObject(copy),
  fGroups(copy.fGroups)
{
  // Copy constructor
}
//______________________________________________________________________________
AliRsnOutManager &AliRsnOutManager::operator=(const AliRsnOutManager &other)
{
  // Assignment operator
  if(this != &other) {
    fGroups = other.fGroups;
  }
  return *this;
}
//______________________________________________________________________________
AliRsnOutManager::~AliRsnOutManager()
{
  // Destructor
}
//______________________________________________________________________________
void AliRsnOutManager::MakeGroup(const char *fname, const char *lname)
{
  TFile *file = TFile::Open(fname, "READ");
  if (!file) return;

  TList *list = dynamic_cast<TList *>(file->Get(lname));
  if (!list) {
    Warning("MakeGroup", "list '%s' doesn't exist in file '%s'", lname, file->GetName());
    delete file;
    return;
  }

  if (!fGroups) fGroups = new TList();
  AliRsnOutGroup *g = new AliRsnOutGroup(list);
  g->SetFileName(file->GetName());
  fGroups->Add(g);

  delete file;
}
//______________________________________________________________________________
void AliRsnOutManager::FindIntervals() const
{
  // TODO
}
