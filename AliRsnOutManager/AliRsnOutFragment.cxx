// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    12 June 2012

#include <TList.h>

#include "AliRsnOutFragment.h"
#include "AliRsnOutAxis.h"

ClassImp(AliRsnOutFragment)

//______________________________________________________________________________
AliRsnOutFragment::AliRsnOutFragment()
: TObject(),
  fGroup(0),
  fAxes(0),
  fHistograms(0)
{
  // Default constructor
}
//______________________________________________________________________________
AliRsnOutFragment::AliRsnOutFragment(Int_t ToDo)
: TObject(),
  fGroup(0),
  fAxes(0),
  fHistograms(0)
{
  // Normal constructor
  // ToDo
  fAxes = new TList();
  fHistograms = new TList();
}
//______________________________________________________________________________
AliRsnOutFragment::~AliRsnOutFragment()
{
  // Destructor
  // ToDo
  if (fAxes) fAxes->Delete();
  delete fAxes; fAxes = 0;
  if (fHistograms) fHistograms->Delete();
  delete fHistograms; fHistograms = 0;
}
//______________________________________________________________________________
const char *AliRsnOutFragment::GetName() const
{
  return "ToDo";
}
//______________________________________________________________________________
const char *AliRsnOutFragment::GetTitle() const
{
  return "ToDo";
}
//______________________________________________________________________________
void AliRsnOutFragment::Print(Option_t *option) const
{
  Printf("ToDo");
}
//______________________________________________________________________________
AliRsnOutAxis *AliRsnOutFragment::FindAxis(Int_t dim) const
{
  if (!fAxes) return 0;

  TIter next(fAxes);
  AliRsnOutAxis *axis;
  while ((axis = (AliRsnOutAxis *)next()))
    if (axis->GetDim() == dim) return axis;

  return 0;
}
//______________________________________________________________________________
void AliRsnOutFragment::DeleteAxis(Int_t dim)
{
  // ToDo
}
//______________________________________________________________________________
AliRsnOutAxis *AliRsnOutFragment::AxisRange(Int_t dim, Int_t bfirst, Int_t blast)
{
  AliRsnOutAxis *axis = FindAxis(dim);
  if (!axis) {
    axis = new AliRsnOutAxis(dim);
    axis->AllocateRange(bfirst, blast);
    fAxes->Add(axis);
  }
  else
    axis->AllocateRange(bfirst, blast);

  return axis;
}
//______________________________________________________________________________
AliRsnOutAxis *AliRsnOutFragment::AxisRangeUser(Int_t dim, Double_t first, Double_t last)
{
  AliRsnOutAxis *axis = FindAxis(dim);
  if (!axis) {
    axis = new AliRsnOutAxis(dim);
    axis->AllocateRangeUser(first, last);
    fAxes->Add(axis);
  }
  else
    axis->AllocateRangeUser(first, last);

  return axis;
}
//______________________________________________________________________________
AliRsnOutAxis *AliRsnOutFragment::AxisProjection(Int_t dim)
{
  AliRsnOutAxis *axis = FindAxis(dim);
  if (!axis) {
    axis = new AliRsnOutAxis(dim);
    axis->AllocateProjection();
    fAxes->Add(axis);
  }
  else
    axis->AllocateProjection();

  return axis;
}


//TH1D *AliRsnOutFragment::PullHisto(THnBase *base)
//{
//  if (!base) return 0;
//
//  // at first unset range off all axes
//  TIter next(base->GetListOfAxes());
//  TAxis *axis;
//  while ((axis = (TAxis *)next()))
//    axis->SetRange(0, 0);
//
// alebo len takto ?!
/*for (Int_t d = 0; d < ndim; ++d){
       TAxis *axis = GetAxis(dim[d]);
       hadRange[d] = axis->TestBit(TAxis::kAxisRange);
       axis->SetBit(TAxis::kAxisRange, kFALSE);
    }
 */
//  // and now ....
//  if (!fAxes->IsSorted()) fAxes->Sort();
//  // najprv musia ist range
//  // a az potom projection
//  TIter next2(fAxes);
//  AliRsnOutAxis *axis2;
//  while ((axis2 = (AliRsnOutAxis *)next2()))
////      axis2->DoAction(base);
//      Printf("%s", axis->GetName());
//
//    return 0;
//}
