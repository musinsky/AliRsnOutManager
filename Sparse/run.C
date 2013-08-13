void run(Int_t id=0,Int_t id_to=0, Int_t c=0, TString in_fname="AnalysisResults.root", const char *path_Sparce_macro="", TString out_path = "") {

  gROOT->ProcessLine(TString::Format(".L %sSparse.C",path_Sparce_macro));

  eff_prefix="";
  eff_prefix+="/eos/saske.sk/scratch/ALICE/RSN/RESULTS/Rsn_Phi/pp_2.76/2013-04-11/MC_LHC12f1a/";
  eff_prefix+="EFFI_OK/effi_";
  del_step = 0.2;
  SetNameBordel(id, 30, 2010, 1, in_fname.Data());
  // SetNameBordel(id, 30, 2010, 1);
  SetCombinations(c);

  AnalyzeSparse();
  return ;
  if (!out_path.IsNull()) {
    TString out_fname = gSystem->GetFromPipe("ls -1 RsnHistMini_Phi_PhiNsigma* | head -n 1");
    Printf(out_fname.Data());
    in_fname.ReplaceAll("/AnalysisResults.root","");
    in_fname.ReplaceAll("AnalysisResults.root","");
    gSystem->Exec(TString::Format("mv %s %sR",out_fname.Data(), in_fname.Data()).Data());
    gSystem->Exec(TString::Format("mv %sF %sRF",out_fname.Data(), in_fname.Data()).Data());
    gSystem->Exec(TString::Format("mv %sFE %sRFE",out_fname.Data(), in_fname.Data()).Data());
    out_fname.ReplaceAll("RsnHistMini_Phi_PhiNsigma:","");
    out_fname.ReplaceAll("RsnHistMini_Phi_PhiNsigma_","");
    out_fname.ReplaceAll("_R","");
    
    
    out_path += out_fname;
    // out_path += "/";
    gSystem->Exec(TString::Format("mkdir -p %s", out_path.Data()).Data());
    gSystem->Exec(TString::Format("mv %sR %s/", in_fname.Data(), out_path.Data()).Data());
    gSystem->Exec(TString::Format("mv %sRF %s/", in_fname.Data(), out_path.Data()).Data());
    gSystem->Exec(TString::Format("mv %sRFE %s/", in_fname.Data(), out_path.Data()).Data());
  }

}
