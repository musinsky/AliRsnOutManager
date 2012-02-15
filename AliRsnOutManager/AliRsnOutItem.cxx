// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    15 Feb 2012

#include "AliRsnOutItem.h"

ClassImp(AliRsnOutItem)

//______________________________________________________________________________
AliRsnOutItem::AliRsnOutItem()
: TNamed(), fBla(0)
{
  // Default constructor
}
//______________________________________________________________________________
AliRsnOutItem::AliRsnOutItem(const AliRsnOutItem &copy)
: TNamed(copy), fBla(copy.fBla)
{
  // Copy constructor
}
//______________________________________________________________________________
AliRsnOutItem &AliRsnOutItem::operator=(const AliRsnOutItem &other)
{
  // Assignment operator
  if(this != &other) {
    fBla = other.fBla;
  }
  return *this;
}
//______________________________________________________________________________
AliRsnOutItem::~AliRsnOutItem()
{
  // Destructor
}
