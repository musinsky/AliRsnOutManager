// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    23 Mar 2012

#include <TList.h>
#include <TH1.h>
#include <THnBase.h>


#include "AliRsnOutGroup.h"
#include "AliRsnOutFragment.h"

#include "AliRsnOutAxis.h"

ClassImp(AliRsnOutGroup)

//______________________________________________________________________________
AliRsnOutGroup::AliRsnOutGroup()
: TNamed(),
  fFileName(0),
  fList(0),
  fFragments(0)
{
  // Default constructor
}

AliRsnOutGroup::AliRsnOutGroup(const char *name, const char *title)
: TNamed(name, title),
  fFileName(0),
  fList(0),
  fFragments(0)
{
  fFragments = 0;
  Printf("NOVA GRUPA");
  // Normal constructor

  // no a teraz vytvaram fragmenty (t.j. napr. 16 intrvalov);
  // ci bud PM, PP, MM skor to pod group ako pre kazdy group ....


}


void AliRsnOutGroup::SetDelimitedNames(const char *pre, const char *rsn, const char *cut)
{
  fPrefix  = pre;
  fRsnName = rsn;
  fCutName = cut;
}

//______________________________________________________________________________
AliRsnOutGroup::AliRsnOutGroup(const char *pre, const char *rsn, const char *cut,
                               const char *pair)
: TNamed(),
  fFileName(0),
  fList(0),
  fFragments(0)


///   Asi lepsie bez pair, pair bude urcovat frgmanet

// Rsn.Phi.Phi2010_onlyTPC_15.RecPM   jedna grupa
//          0.0<p<0.2                 jeden fragment
//          0.2<p<0.4                 druhy fragment
// Rsn.Phi.Phi2010_onlyTPC_15.GenPM   dtuha grupa
//          0.0<p<0.2                 jeden fragment
//          0.2<p<0.4                 druhy fragment



{
  // Normal constructor
  fPrefix = pre;
  fRsnName = rsn;
  fCutName = cut;
  fPairName = pair;
  SetName(Form("%s.%s.%s.%s", fPrefix.Data(), fRsnName.Data(), fCutName.Data(),
               fPairName.Data()));


  // nie "." ale const char *dot = !!!!
  //fFileName = "";
  fList = 0;
  //if (!l) return;
  //SetName(l->GetName());
  //fList = l;
}
//______________________________________________________________________________
AliRsnOutGroup::~AliRsnOutGroup()
{
  // Destructor
}


Int_t AliRsnOutGroup::MakeFragments(const THnBase *sparse, Double_t min)
{
  // alebo aj Make fragment
  // staticka premenna, ktora urcuje, ze intervaly su po pt
  Int_t atype = kPt;
  TH1D *his = sparse->Projection(atype);
  if (!fFragments) fFragments = new TList();
  //  AliRsnOutFragment *fragment = new AliRsnOutFragment(1, 2);
  //  fragment->SetGroup(this);
  //
  //  fFragments->Add(fragment);
  AliRsnOutFragment *fragment;

  Double_t step = 0.25;
  Double_t bin_step =  step/his->GetBinWidth(1);
  Int_t count = 0;
  Int_t fb, lb;
  // !!!!!!!!!!!!!!!!!!!!
  Double_t maxp = 3.4;
  for (Int_t i = 1; i <= his->GetNbinsX(); i += bin_step) {
    fb = i;
    lb = i + bin_step - 1;
    if (his->GetBinCenter(i) > maxp) {
      lb = his->GetNbinsX() - 1;
      break;
    }
    //  fragmnet z groupy nastavim jeho type
    fragment = new AliRsnOutFragment(atype);
    //fragment->AllocateRange(sparse, fb, lb);
    fragment->AxisRange(kPt, fb, lb);
    fragment->AxisProjection(kInvMass);
    fragment->SetGroup(this);
    fFragments->Add(fragment);
    //ponastavovat vsetky onfo: bin, widt, max, min, ...
    count++;
  }

  return count-1;

}
