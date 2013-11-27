{
  gROOT->LoadMacro("Sparse.C");
  rsn_data = 20131015;
  input_new = kTRUE;
  del_step = 0.20;
  del_step = 0.50;
  mixing = kTRUE;
  //  del_step = 0.50; // Viktor

  SetNameBordelNew(0,  0, 2010, kTRUE, "RsnOutput.root");
  AnalyzeSparse(kBlack);
  SetNameBordelNew(0, 10, 2010, kTRUE, "RsnOutput.root");
  AnalyzeSparse(kRed);
  SetNameBordelNew(0, 20, 2010, kTRUE, "RsnOutput.root");
  AnalyzeSparse(kGreen);
  SetNameBordelNew(0, 30, 2010, kTRUE, "RsnOutput.root");
  AnalyzeSparse(kBlue);
  SetNameBordelNew(0, 40, 2010, kTRUE, "RsnOutput.root");
  AnalyzeSparse(kMagenta);

  out_path=TString::Format("/eos/saske.sk/scratch/ALICE/RSN_OUT/2013-11-10/OUT/TPC%d/%d/",nsig,c);
  if (!nsig) out_path.ReplaceAll("TPC0","NOPID");

  // TPC 3Sigma
  SetNameBordel(id, nsig, 2010, 1, in_fname.Data());
  // NoPID
  //SetNameBordel(id, 0, 2010, 1, in_fname.Data());
  SetCombinations(c);

  if (mixing) out_path+="MIX";
  else out_path+="LS";
  AnalyzeSparse();
  if (!out_path.IsNull()) {
    //gSystem->Exec(TString::Format("mkdir -p %s",out_path.Data()).Data());
    //gSystem->Exec(TString::Format("mv *_PRIMARY_* %s/",out_path.Data()).Data());
    TString out = gSystem->GetFromPipe("find -name *_PRIMARY_*");
    out.ReplaceAll("./","");
    Printf("xrdcp -R -f %s root://eos.saske.sk/%s/",out.Data(), out_path.Data());
    gSystem->Exec(TString::Format("xrdcp -R -f %s root://eos.saske.sk/%s/%s/",out.Data(), out_path.Data(),out.Data()).Data());
    gSystem->Exec(TString::Format("rm -rf %s",out.Data()).Data());
  }


}
