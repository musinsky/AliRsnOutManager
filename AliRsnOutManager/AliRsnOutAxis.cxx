// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    30 Oct 2012

#include <THnBase.h>
#include <TAxis.h>

#include "AliRsnOutAxis.h"

ClassImp(AliRsnOutAxis)

//______________________________________________________________________________
AliRsnOutAxis::AliRsnOutAxis()
: TObject(),
  fDim(0),
  fRangeMin(0.0),
  fRangeMax(0.0),
  fRangeBinMin(0),
  fRangeBinMax(0),
  fRangeChecked(kFALSE)
{
  // Default constructor
}
//______________________________________________________________________________
AliRsnOutAxis::AliRsnOutAxis(Int_t dimension)
: TObject(),
  fDim(dimension),
  fRangeMin(0.0),
  fRangeMax(0.0),
  fRangeBinMin(0),
  fRangeBinMax(0),
  fRangeChecked(kFALSE)
{
  // Normal constructor
}
//______________________________________________________________________________
AliRsnOutAxis::~AliRsnOutAxis()
{
  // Destructor
}
//______________________________________________________________________________
const char *AliRsnOutAxis::GetName() const
{
  switch(fDim) {
    case kInvMass:
      return "InvMass";
    case kPt:
      return "Pt";
    case kEta:
      return "Eta";
    default:
      return "unknown";
  }
}
//______________________________________________________________________________
const char *AliRsnOutAxis::GetTitle() const
{
  return "ToDo";
}
//______________________________________________________________________________
void AliRsnOutAxis::Print(Option_t *option) const
{
  Printf("ToDo");
}
//______________________________________________________________________________
void AliRsnOutAxis::AllocateRange(Int_t bmin, Int_t bmax)
{
  if (TestBit(kRange))
    Info("AllocateRange", "axis '%s' re-allocate range", GetName());
  if (TestBit(kProjection))
    Info("AllocateRange", "axis '%s' is also as projection", GetName());
  if (bmin == bmax)
    Info("AllocateRange", "axis '%s': min bin = max bin = %d", GetName(), bmin);

  fRangeBinMin = bmin;
  fRangeBinMax = bmax;
  fRangeMin = fRangeMax = 0;
  SetBit(kRange);
  fRangeChecked = kFALSE;
}
//______________________________________________________________________________
void AliRsnOutAxis::AllocateRangeUser(Double_t min, Double_t max)
{
  if (TestBit(kRange))
    Info("AllocateRangeUser", "axis '%s' re-allocate range", GetName());
  if (TestBit(kProjection))
    Info("AllocateRangeUser", "axis '%s' is also as projection", GetName());
  if (TMath::AreEqualAbs(min, max, 1.0e-10))
    Info("AllocateRangeUser", "axis '%s': min = max = %10f", GetName(), min);

  fRangeMin = min;
  fRangeMax = max;
  fRangeBinMin = fRangeBinMax = -9999; // must be -9999, see SetRange()
  SetBit(kRange);
  fRangeChecked = kFALSE;
}
//______________________________________________________________________________
void AliRsnOutAxis::AllocateProjection()
{
  if (TestBit(kProjection))
    Info("AllocateProjection", "axis '%s' re-allocate projection", GetName());
  if (TestBit(kRange))
    Info("AllocateProjection", "axis '%s' is also as range", GetName());

  SetBit(kProjection);
}
//______________________________________________________________________________
void AliRsnOutAxis::SetRange(const THnBase *hnbase)
{
  if (!hnbase) return;
  if (!TestBit(kRange)) {
    Warning("SetRange", "axis %s: no allocated range", GetName());
    return;
  }
  TAxis *axis = hnbase->GetAxis(fDim);
  if (!axis) {
    Error("SetRange", "%s has no axis %s (%d)", hnbase->GetName(), GetName(), fDim);
    return;
  }

  if ((fRangeBinMin == -9999) && (fRangeBinMax == -9999))
    axis->SetRangeUser(fRangeMin, fRangeMax);
  else
    axis->SetRange(fRangeBinMin, fRangeBinMax);

  fRangeBinMin  = axis->GetFirst();
  fRangeBinMax  = axis->GetLast();
  fRangeMin     = axis->GetBinLowEdge(fRangeBinMin);
  fRangeMax     = axis->GetBinUpEdge(fRangeBinMax);
  fRangeChecked = kTRUE;
}
