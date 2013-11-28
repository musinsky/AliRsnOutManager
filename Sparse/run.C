void run(Int_t id = 0, Int_t id_to = 0, Int_t c = 0,
         TString in_fname = "AnalysisResults.root",
         const char *path_Sparse_macro = "/eos/saske.sk/scratch/ALICE/RSN_OUT/macros", TString out_path = "/eos/saske.sk/scratch/ALICE/RSN_OUT/2013-11-11/OUT/")
{
  gROOT->ProcessLine(TString::Format(".L %s/Sparse.C",path_Sparse_macro));

  eff_prefix="";
  // eff_prefix+="/eos/saske.sk/scratch/ALICE/RSN/RESULTS/Rsn_Phi/pp_2.76/2013-04-11/MC_LHC12f1a/";
//   eff_prefix+="EFFI_OK/effi_";
  eff_prefix += "/eos/saske.sk/scratch/ALICE/RSN_OUT/macros/";
//  eff_prefix+="EFFI_Pythia8/effi_";

  rsn_data = 20131015;
  input_new = kTRUE;
  del_step = 0.20;
  // del_step = 2.50;
  mixing = kTRUE;
  //  del_step = 0.50; // Viktor
  g2f_prefix="tmp/";

  SetCombinations(c);

  SetNameBordelNew(id,  0, 2010, kTRUE, "RsnOutput.root");
  AnalyzeSparse(kBlack);
  SetNameBordelNew(id, 10, 2010, kTRUE, "RsnOutput.root");
  AnalyzeSparse(kRed);
  SetNameBordelNew(id, 20, 2010, kTRUE, "RsnOutput.root");
  AnalyzeSparse(kGreen);
  SetNameBordelNew(id, 30, 2010, kTRUE, "RsnOutput.root");
  AnalyzeSparse(kBlue);
  SetNameBordelNew(id, 40, 2010, kTRUE, "RsnOutput.root");
  AnalyzeSparse(kMagenta);

  out_path=TString::Format("%s/%s/%d/",out_path.Data(),sufNameCurrent.Data(),c);

  if (mixing) out_path+="MIX";
  else out_path+="LS";
  if (!out_path.IsNull()) {
    //gSystem->Exec(TString::Format("mkdir -p %s",out_path.Data()).Data());
    //gSystem->Exec(TString::Format("mv *_PRIMARY_* %s/",out_path.Data()).Data());
    // TString out = gSystem->GetFromPipe("find -name *_PRIMARY_*");
    // out.ReplaceAll("./","");
    // Printf("xrdcp -R -f %s root://eos.saske.sk/%s/",g2f_prefix.Data(), out_path.Data());
    gSystem->Exec(TString::Format("LD_LIBRARY_PATH= eos -b cp -r %s/%s root://eos.saske.sk/%s/",gSystem->WorkingDirectory(),g2f_prefix.Data(), out_path.Data()).Data());
    gSystem->Exec(TString::Format("rm -rf %s",g2f_prefix.Data()).Data());
  }

}
