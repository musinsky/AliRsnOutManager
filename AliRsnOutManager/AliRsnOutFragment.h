// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    12 June 2012

#ifndef ALIRSNOUTFRAGMENT_H
#define ALIRSNOUTFRAGMENT_H

#include <TObject.h>

class TH1D;
class TH2D;

class AliRsnOutGroup;
class AliRsnOutAxis;

class AliRsnOutFragment : public TObject {

public:
  AliRsnOutFragment();
  AliRsnOutFragment(Int_t ToDo);
  virtual ~AliRsnOutFragment();

  void              SetGroup(AliRsnOutGroup *group) { fGroup = group; }
  AliRsnOutGroup   *GetGroup() const { return fGroup; }
  TList            *GetListOfAxes() const { return fAxes; }
  TList            *GetListOfHistograms() const { return fHistograms; }

  virtual const char *GetName() const;
  virtual const char *GetTitle() const;
  virtual void        Print(Option_t *option = "") const;

  AliRsnOutAxis    *FindAxis(Int_t dim) const;
  void              DeleteAxis(Int_t dim);
  AliRsnOutAxis    *AxisRange(Int_t dim, Int_t bfirst, Int_t blast);
  AliRsnOutAxis    *AxisRangeUser(Int_t dim, Double_t first, Double_t last);
  AliRsnOutAxis    *AxisProjection(Int_t dim);
  //  TH1D             *Projection(Int_t type, Option_t *option = "") const;
  //  TH2D             *Projection(Int_t type1, Int_t type2,
  //                               Option_t *option = "") const;
  //  TH1D             *PullHisto(THnBase *base);

private:
  AliRsnOutGroup   *fGroup;         //! pointer to parent group
  TList            *fAxes;          //->list of axis
  TList            *fHistograms;    //->list of histograms

  ClassDef(AliRsnOutFragment, 1) // AliRsnOutFragment class
};

#endif
