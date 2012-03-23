// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    23 Mar 2012

#include "AliRsnOutFragment.h"

ClassImp(AliRsnOutFragment)

//______________________________________________________________________________
AliRsnOutFragment::AliRsnOutFragment()
: TNamed(),
  fValueType(-1),
  fValueMin(0),
  fValueMax(0)
{
  // Default constructor
}
//______________________________________________________________________________
AliRsnOutFragment::AliRsnOutFragment(const AliRsnOutFragment &copy)
: TNamed(copy),
  fValueType(copy.fValueType),
  fValueMin(copy.fValueMin),
  fValueMax(copy.fValueMax)
{
  // Copy constructor
}
//______________________________________________________________________________
AliRsnOutFragment &AliRsnOutFragment::operator=(const AliRsnOutFragment &other)
{
  // Assignment operator
  if(this != &other) {
    fValueType = other.fValueType;
    fValueMin  = other.fValueMin;
    fValueMax  = other.fValueMax;
  }
  return *this;
}
//______________________________________________________________________________
AliRsnOutFragment::~AliRsnOutFragment()
{
  // Destructor
}
