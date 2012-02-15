// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    15 Feb 2012

#include "AliRsnOutManager.h"

ClassImp(AliRsnOutManager)

//______________________________________________________________________________
AliRsnOutManager::AliRsnOutManager()
: TObject(), fBla(0), fDza(0)
{
  // Default constructor
}
//______________________________________________________________________________
AliRsnOutManager::AliRsnOutManager(const AliRsnOutManager &copy)
: TObject(copy), fBla(copy.fBla), fDza(copy.fDza)
{
  // Copy constructor
}
//______________________________________________________________________________
AliRsnOutManager &AliRsnOutManager::operator=(const AliRsnOutManager &other)
{
  // Assignment operator
  if(this != &other) {
    fBla = other.fBla;
    fDza = other.fDza;
  }
  return *this;
}
//______________________________________________________________________________
AliRsnOutManager::~AliRsnOutManager()
{
  // Destructor
}
