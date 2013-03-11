// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    11 Mar 2013

#include <TFile.h>
#include <TKey.h>
#include <TROOT.h>
#include <TList.h>
#include <TClass.h>
#include <THnSparse.h>

#include "AliLog.h"
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
void AliRsnOutManager::ScanFile(const char *fname)
{
  TFile *file = TFile::Open(fname, "READ");
  if (!file) return;

  //  AliInfo(Form("Ali Info %s", file->GetName()));

  //  TObjLink* lnk = file->GetListOfKeys()->FirstLink();
  //  while (lnk) {
  //    Printf("======> %s", lnk->GetObject()->GetName());
  //    lnk = lnk->Next();
  //  }


  TIter next(file->GetListOfKeys());
  TKey *key;
  while ((key = (TKey *)next())) {
    if (!(TClass::GetClass(key->GetClassName()))->InheritsFrom(TList::Class())) continue;
    TList *list = (TList *)key->ReadObj(); // serious (I/O & memory) operation
    /////TList *list = (TList *)file->Get(key->GetName());
    ScanList(list);
    delete list; // list->IsOwner() = kTRUE
  }


  //  // pouzivat dynamic_cast,
  //  VERY IMPORTANT NOTE:
  //  In case the class of this object derives from TObject but not
  //  as a first inheritance, one must use dynamic_cast<>().

  delete file;
}
//_____________________________________________________________________________
void AliRsnOutManager::ScanList(TList *list)
{
  if (!list) {
    Error("ScanList", "pointer to TList object is null");
    return;
  }

  TIter next(list);
  TObject *obj;
  while ((obj = next()))
    if (obj->InheritsFrom(THnSparse::Class()))
      ScanSparse((THnSparse *)obj);
}
//______________________________________________________________________________
void AliRsnOutManager::ScanSparse(THnSparse *sparse)
{
  if (!sparse) {
    Error("ScanSparse", "pointer to THnSparse object is null");
    return;
  }

  const TString sname(sparse->GetName());
  const char *dot = ".";
  Ssiz_t prevDot = 0;
  Ssiz_t nextDot = 0;
  const Int_t maxWord = 4;
  TString parseName[maxWord]; // prefix, rsnName, cutName, pairName
  Int_t count = 0;

  while (nextDot != sname.Length()) {
    nextDot = sname.Index(dot, prevDot);
    if (nextDot < 0) nextDot = sname.Length(); // if last dot
    if (count < maxWord) parseName[count] = sname(prevDot, nextDot - prevDot);
    count++;
    prevDot = nextDot + strlen(dot); // + 1
  }

  if (count != maxWord) {
    Warning("ScanSparse", "name parser problem '%s'", sname.Data());
    return;
  }

  Printf("THnSparse name: %s", sname.Data());
  for (Int_t i = 0; i < maxWord; i++)
    Printf("%d: %s", i, parseName[i].Data());


  // checkgroup
  // ak existuje (t.j. prefix, rsnName, cutName, rovnake, tak len pridat)
  // nie tak vytvorit novu grupu
  //
  //  do grupy pridavame len pairName
  //

  if (!fGroups) fGroups = new TList();
  TString gname(Form("%s%s%s%s%s", parseName[0].Data(), dot, parseName[1].Data(),
                     dot, parseName[2].Data()));
  Printf("%s", gname.Data());
  AliRsnOutGroup *group = (AliRsnOutGroup *)fGroups->FindObject(gname.Data());

  if (!group) {
    group = new AliRsnOutGroup(gname.Data(), "");
    // ToDo prepinatko na aku os (1,2 alebo 3)
    group->MakeFragments(sparse);//->Projection(kPt));
    // a aby gorupa vedela ze je to projekcia pt
    // ale zas ne tu to je len na najdene intervalov

    //   g->SetFileName(file->GetName());
    fGroups->Add(group);
  }

  // a teraz jednotlive histogramy; (jeden sparse = jeden hoistogram (not unique))

  //    1) pouzijeme range !!!!
  //    2) zapneme histo, ak nie je tak najprv vytvorime




  //  g->SetFileName(file->GetName());


}
//______________________________________________________________________________
void AliRsnOutManager::FindIntervals() const
{
  // TODO
}
