void effi_simple_20131015()
{
  gROOT->LoadMacro("effi_simple.C");

  const char *suf[13] = {"00_DEFAULT","CHI2ITS100","CHI2TPC06","DCAXY5S",
                         "DCAXY6S","DCAZ01","DCAZ05","DCAZ10","DCAZ15",
                         "NCLSTTPC50","NCLSTTPC80","NCLSTTPC85","NCLSTTPC90"};
  TString path("/eos/saske.sk/alice/rsn/PHIKK/LHC12f1a/ESD/RSN_20131015/Merged/All/STD2010/");
  TString fname, fname_base = "root://eos.saske.sk/" + path;
  TString edir,  edir_base  = "EFFI_20131015_";
  TString efile, efile_base = "effi_PhiNsigma_qualityonly_STD2010_PRIMARY_";

  for (Int_t i = 0; i < 13; i++) {
    fname = fname_base + suf[i] + "/qualityonly/RsnOutput.root";
    edir  = edir_base + suf[i];
    efile = efile_base + suf[i];
    //    Printf("%s", fname.Data());
    //    Printf("%s", edir.Data());
    //    Printf("%s", efile.Data());

    effi_simple(201310, fname);

    gSystem->mkdir(edir.Data(), 1);
    gSystem->Exec(TString::Format("mv %s %s/%s", "out.effi",
                                  edir.Data(), efile.Data()).Data());
  }
}
