// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    17 Feb 2012

#include <TFile.h>

#include "AliRsnOutManager.h"
#include "AliRsnOutGroup.h"

ClassImp(AliRsnOutManager)

//______________________________________________________________________________
AliRsnOutManager::AliRsnOutManager()
: TObject(), fFile(0), fGroups(0), fBla(0), fDza(0) //TODO "warning:   when initialized here [-Wreorder]"
{
  // Default constructor
  //  fFile = 0;
}
//______________________________________________________________________________
AliRsnOutManager::AliRsnOutManager(const AliRsnOutManager &copy)
: TObject(copy), fBla(copy.fBla), fDza(copy.fDza)
{
  // Copy constructor
}
//______________________________________________________________________________
AliRsnOutManager &AliRsnOutManager::operator=(const AliRsnOutManager &other)
{
  // Assignment operator
  if(this != &other) {
    fBla = other.fBla;
    fDza = other.fDza;
  }
  return *this;
}
//______________________________________________________________________________
AliRsnOutManager::~AliRsnOutManager()
{
  // Destructor
}
//______________________________________________________________________________
void AliRsnOutManager::SetFileName(const char *fname)
{
  if (fFile) delete fFile;
  fFile = TFile::Open(fname, "READ");
}
//______________________________________________________________________________
void AliRsnOutManager::MakeGroup(const char *lname)
{
  if (!fFile) {
    Info("MakeGroup", "first specify file name");
    return;
  }
  TList *list = dynamic_cast<TList *>(fFile->Get(lname));
  if (!list) {
    Warning("MakeGroup", "list '%s' doesn't exist in file '%s'", lname, fFile->GetName());
    return;
  }

  if (!fGroups) fGroups = new TList();
  AliRsnOutGroup *g = new AliRsnOutGroup(list);
  fGroups->Add(g);
}
//______________________________________________________________________________
void AliRsnOutManager::FindIntervals() const
{
  // TODO
}
