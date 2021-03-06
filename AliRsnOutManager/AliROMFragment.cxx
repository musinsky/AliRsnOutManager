// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    12 Mar 2013

#include <TObjArray.h>
#include <TH1.h>
#include <TROOT.h>

#include "AliROMFragment.h"

Bool_t AliROMFragment::fgHistoOwner = kFALSE;

ClassImp(AliROMFragment)

//______________________________________________________________________________
AliROMFragment::AliROMFragment()
: TNamed(),
  fZoneMin(0.),
  fZoneMax(0.),
  fGroup(0),
  fHistoSingle(0),
  fHistoMix(0),
  fHistoTrueMC(0),
  fHistoGenMC(0)
{
  // Default constructor
}
//______________________________________________________________________________
AliROMFragment::~AliROMFragment()
{
  if (fHistoSingle) {
    if (fgHistoOwner) fHistoSingle->Delete();
    SafeDelete(fHistoSingle);
  }
  if (fHistoMix) {
    if (fgHistoOwner) fHistoMix->Delete();
    SafeDelete(fHistoMix);
  }
  if (fHistoTrueMC) {
    if (fgHistoOwner) fHistoTrueMC->Delete();
    SafeDelete(fHistoTrueMC);
  }
  if (fHistoGenMC) {
    if (fgHistoOwner) fHistoGenMC->Delete();
    SafeDelete(fHistoGenMC);
  }
}
//______________________________________________________________________________
Int_t AliROMFragment::Compare(const TObject *obj) const
{
  if (GetZoneMean() < (((AliROMFragment *)obj)->GetZoneMean())) return -1;
  else if (GetZoneMean() > (((AliROMFragment *)obj)->GetZoneMean())) return 1;
  else return 0;
}
//______________________________________________________________________________
void AliROMFragment::Print(Option_t *option) const
{
  if (fHistoSingle) fHistoSingle->Print(option);
  if (fHistoMix)    fHistoMix->Print(option);
  if (fHistoTrueMC) fHistoTrueMC->Print(option);
  if (fHistoGenMC)  fHistoGenMC->Print(option);
}
//______________________________________________________________________________
TObjArray *AliROMFragment::CreateHistoArray(const char *name, Int_t max)
{
  TObjArray *array = new TObjArray(max);
  array->SetName(name);
  gROOT->GetListOfCleanups()->Add(array);
  return array;
}
//______________________________________________________________________________
TObjArray *AliROMFragment::GetHistoArray(EPairKind kind, Bool_t create)
{
  switch (kind) {
    case kSingle:
      if (!fHistoSingle && create) fHistoSingle = CreateHistoArray("single", kMaxPairType);
      return fHistoSingle;

    case kMix:
      if (!fHistoMix && create) fHistoMix = CreateHistoArray("mix", kMaxPairType);
      return fHistoMix;

    case kTrueMC:
      if (!fHistoTrueMC && create) fHistoTrueMC = CreateHistoArray("trueMC", kMaxPairType);
      return fHistoTrueMC;

    case kGenMC:
      if (!fHistoGenMC && create) fHistoGenMC = CreateHistoArray("genMC", kMaxPairType);
      return fHistoGenMC;

    default:
      return 0;
  }
}
//______________________________________________________________________________
void AliROMFragment::AddHisto(TH1 *h, EPairKind kind, EPairType type)
{
  if (!h) return;
  TObjArray *harray = GetHistoArray(kind, kTRUE);
  if (!harray) return;

  TObject *prev = harray->At(type);
  if (prev) Warning("AddHisto", "histo %s replaced by %s", prev->GetName(), h->GetName());

  TString name;
  switch (type) {
    case kUnlike:   name = "unlike";   break;
    case kUnlikePM: name = "unlikePM"; break;
    case kUnlikeMP: name = "unlikeMP"; break;
    case kLike:     name = "like";     break;
    case kLikePP:   name = "likePP";   break;
    case kLikeMM:   name = "likeMM";   break;
    default:        name = "nan";      break;
  }

  h->SetName(Form("%s_%s", harray->GetName(), name.Data()));
  h->SetDirectory(0);
  h->SetBit(kMustCleanup);
  harray->AddAt(h, type);
}
//______________________________________________________________________________
TH1 *AliROMFragment::GetHisto(EPairKind kind, EPairType type)
{
  TObjArray *harray = GetHistoArray(kind, kFALSE);
  if (!harray) return 0;
  return (TH1 *)harray->At(type);
}
//______________________________________________________________________________
