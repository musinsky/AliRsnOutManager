{
  gROOT->LoadMacro("Sparse.C");
  rsn_data = 20140125;
  input_new = kTRUE;
  del_step = 0.20;
  //  binAnders = kTRUE;
  mixing = kTRUE;
  isVoig = kTRUE;
  //  isBinCounting = kTRUE;
  g2f_prefix="test/";
  Int_t polynom = 1;
  Int_t c = 0;
  SetCombinations(c, polynom);

  // switch effiTPC !!! before !!! SetNameBordelNew

  effiTPC = kTRUE;  // = each sigma has own effi


  effiTPC = kFALSE;
  SetNameBordelNew(0, 0, 2010, kTRUE, "RsnOutput.root");
  AnalyzeSparse(kBlack);
  //  SetNameBordelNew(0, 40, 2010, kTRUE, "RsnOutput.root");
  //  AnalyzeSparse(kRed);
  SetNameBordelNew(0, 30, 2010, kTRUE, "RsnOutput.root");
  AnalyzeSparse(kGreen);
  //  SetNameBordelNew(0, 20, 2010, kTRUE, "RsnOutput.root");
  //  AnalyzeSparse(kBlue);
  //  SetNameBordelNew(0, 10, 2010, kTRUE, "RsnOutput.root");
  //  AnalyzeSparse(kMagenta);
  return;

  // SetNameBordelNew(0, 20, 2010, kTRUE, "RsnOutput.root");
  // AnalyzeSparse(kGreen);
  // SetNameBordelNew(0, 30, 2010, kTRUE, "RsnOutput.root");
  // AnalyzeSparse(kBlue);
  // SetNameBordelNew(0, 40, 2010, kTRUE, "RsnOutput.root");
  // AnalyzeSparse(kMagenta);

  out_path=TString::Format("/eos/saske.sk/scratch/ALICE/RSN_OUT/2013-11-11/OUT/%d/",c);

  if (mixing) out_path += "MIX";
  else out_path += "LS";
  if (!out_path.IsNull()) {
    // gSystem->Exec(TString::Format("mkdir -p %s",out_path.Data()).Data());
    // gSystem->Exec(TString::Format("mv *_PRIMARY_* %s/",out_path.Data()).Data());
    // TString out = gSystem->GetFromPipe("find -name *_PRIMARY_*");
    // out.ReplaceAll("./","");
    // Printf("xrdcp -R -f %s root://eos.saske.sk/%s/",g2f_prefix.Data(), out_path.Data());
    gSystem->Exec(TString::Format("LD_LIBRARY_PATH= eos -b cp -r %s/%s root://eos.saske.sk/%s/",gSystem->WorkingDirectory(),g2f_prefix.Data(), out_path.Data()).Data());
    gSystem->Exec(TString::Format("rm -rf %s",g2f_prefix.Data()).Data());
  }
}
