// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    12 Mar 2013

#include <TH1.h>

#include "AliROMGroup.h"

ClassImp(AliROMGroup)

//______________________________________________________________________________
AliROMGroup::AliROMGroup()
: TNamed(),
  fRsnName(),
  fCutName(),
  fPrefix()
{
  // Default constructor
}
//______________________________________________________________________________
AliROMGroup::AliROMGroup(const char *rsnname, const char *cutname, const char *prefix)
: TNamed(),
  fRsnName(rsnname),
  fCutName(cutname),
  fPrefix(prefix)
{
  // Normal constructor
  if (fPrefix.IsNull()) fName = Form("%s_%s", fRsnName.Data(), fCutName.Data());
  else                  fName = Form("%s_%s_%s", fPrefix.Data(), fRsnName.Data(), fCutName.Data());
}
//______________________________________________________________________________
AliROMGroup::~AliROMGroup()
{

}
//______________________________________________________________________________
void AliROMGroup::Print(Option_t * /*option*/) const
{

}
//______________________________________________________________________________
