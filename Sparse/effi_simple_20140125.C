void effi_simple_20140125(Int_t rebin = 2)
{
  gROOT->LoadMacro("effi_simple.C");

  const char *suf[13] = {"00_DEFAULT","CHI2ITS100","CHI2TPC06","DCAXY5S",
    "DCAXY6S","DCAZ01","DCAZ05","DCAZ10","DCAZ15",
    "NCLSTTPC50","NCLSTTPC80","NCLSTTPC85","NCLSTTPC90"};
  TString path("/eos/saske.sk/alice/rsn/PHIKK/LHC12f1a/ESD/RSN_20140125/Merged/All/STD2010/");
  TString fname, fname_base = "root://eos.saske.sk/" + path;
  TString edir,  edir_base  = "EFFI_20140125_";
  TString efile, efile_base = "effi_PhiNsigma_qualityonly_STD2010_PRIMARY_";

  TString cutsStr="qualityonly,KTPCnsig10,KTPCnsig20,KTPCnsig30,KTPCnsig40";
  TObjArray *cuts = cutsStr.Tokenize(",");
  TString cutName;
  for (Int_t iCut = 0; iCut< cuts->GetEntries();iCut++) {
    cutName = ((TObjString *)cuts->At(iCut))->GetString();
    efile_base = TString::Format("effi_PhiNsigma_%s_STD2010_PRIMARY_",cutName.Data());
    // if (rebin == 0) efile_base = TString::Format("effi_ANDERS_PhiNsigma_%s_STD2010_PRIMARY_",cutName.Data());
    for (Int_t i = 0; i < 1; i++) {
      fname = fname_base + suf[i] + "/" + cutName + "/RsnOutput.root";
      Printf("Doing %s ...",fname.Data());
      edir  = edir_base + suf[i];
      efile = efile_base + suf[i];
      //    Printf("%s", fname.Data());
      //    Printf("%s", edir.Data());
      //    Printf("%s", efile.Data());

      effi_simple(201401, fname, rebin);

      gSystem->mkdir(edir.Data(), 1);
      gSystem->Exec(TString::Format("mv %s %s/%s", "out.effi",
                                    edir.Data(), efile.Data()).Data());
      gSystem->Exec(TString::Format("mv %s %s/%s_his.root", "out.root",
                                    edir.Data(), efile.Data()).Data());
    }
  }
}
