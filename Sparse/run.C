void run(Int_t id = 0, Int_t id_to = 0, Int_t c = 0,
         TString in_fname = "AnalysisResults.root",
         const char *path_Sparse_macro = "/eos/saske.sk/scratch/ALICE/RSN_OUT/macros", TString out_path = "")
{
  gROOT->ProcessLine(TString::Format(".L %s/Sparse.C",path_Sparse_macro));

  eff_prefix="";
  // eff_prefix+="/eos/saske.sk/scratch/ALICE/RSN/RESULTS/Rsn_Phi/pp_2.76/2013-04-11/MC_LHC12f1a/";
//   eff_prefix+="EFFI_OK/effi_";
  eff_prefix += "/eos/saske.sk/scratch/ALICE/RSN_OUT/macros/";
//  eff_prefix+="EFFI_Pythia8/effi_";
  del_step = 0.2;
//  rsn_data = 20130411;
  mixing = kTRUE;
  mixing = kFALSE;
  Int_t nsig=0;

  binAnders = kTRUE;

  out_path=TString::Format("/eos/saske.sk/scratch/ALICE/RSN_OUT/2013-08-30/OUT/TPC%d/%d/",nsig,c);
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
