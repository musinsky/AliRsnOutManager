// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    15 Feb 2012

#include "AliRsnOutGroup.h"

ClassImp(AliRsnOutGroup)

//______________________________________________________________________________
AliRsnOutGroup::AliRsnOutGroup()
: TNamed(), fBla(0)
{
  // Default constructor
}
//______________________________________________________________________________
AliRsnOutGroup::AliRsnOutGroup(const AliRsnOutGroup &copy)
: TNamed(copy), fBla(copy.fBla)
{
  // Copy constructor
}
//______________________________________________________________________________
AliRsnOutGroup &AliRsnOutGroup::operator=(const AliRsnOutGroup &other)
{
  // Assignment operator
  if(this != &other) {
    fBla = other.fBla;
  }
  return *this;
}
//______________________________________________________________________________
AliRsnOutGroup::~AliRsnOutGroup()
{
  // Destructor
}
