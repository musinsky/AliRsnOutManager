// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    17 Feb 2012

#include <TList.h>

#include "AliRsnOutGroup.h"

ClassImp(AliRsnOutGroup)

//______________________________________________________________________________
AliRsnOutGroup::AliRsnOutGroup()
: TNamed(), fBla(0)
{
  // Default constructor
}
//______________________________________________________________________________
AliRsnOutGroup::AliRsnOutGroup(TList *l)
: TNamed(), fList(0)   // TNamed(l->GetName(), ""), fList(l)  // ??  if (!list)    ??
{
  // Normal constructor
  // FIXME
  // if (!list) ?!?!?;
  SetName(l->GetName());
  fList = l;
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
