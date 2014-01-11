void run(Int_t id = 0, Int_t id_to = 0, Int_t c = 0,
         TString in_fname = "AnalysisResults.root",
         const char *path_Sparse_macro = "/eos/saske.sk/alice/rsn/PHIKK/00_ANALYSIS/RSN_20131015/RsnOut_macros", TString out_path = "/eos/saske.sk/scratch/ALICE/RSN_OUT/test/OUT/")
{
  gROOT->ProcessLine(TString::Format(".L %s/Sparse.C",path_Sparse_macro).Data());

  rsn_data = 20131015;
  //  rsn_data = 20130106;
  Int_t year=2010;
  // year=2011;

  input_new = kTRUE;
  del_step = 0.20;
  // del_step = 2.50;
  mixing = kTRUE;
  // del_step = 0.50; // Viktor
  isVoig = kTRUE;
  isBinCounting = kTRUE;

  // use Own Eff
  // effiTPC = kTRUE;

  g2f_prefix="tmp/";

  Int_t polynom = 1;
  polynom = 2;

  SetCombinations(c,polynom);

  // SetNameBordelNew(id,  0, year, kTRUE, "RsnOutput.root");
  // // eff_prefix = path_Sparse_macro;
  // // eff_prefix += "/EFFI_20131015_";
  // // eff_prefix+=TString::Format("%s/effi_",sufNameCurrent.Data()).Data();

  // AnalyzeSparse(kBlack);
  SetNameBordelNew(id, 10, year, kTRUE, "RsnOutput.root");
  AnalyzeSparse(kRed);
  SetNameBordelNew(id, 20, year, kTRUE, "RsnOutput.root");
  AnalyzeSparse(kGreen);
  SetNameBordelNew(id, 30, year, kTRUE, "RsnOutput.root");
  AnalyzeSparse(kBlue);
  SetNameBordelNew(id, 40, year, kTRUE, "RsnOutput.root");
  AnalyzeSparse(kMagenta);
  
  return ;

  if (!out_path.IsNull()) {
    out_path=TString::Format("%s/%s/%d/",out_path.Data(),sufNameCurrent.Data(),c);
    
    if (mixing) out_path+="MIX";
    else out_path+="LS";
    //gSystem->Exec(TString::Format("mkdir -p %s",out_path.Data()).Data());
    //gSystem->Exec(TString::Format("mv *_PRIMARY_* %s/",out_path.Data()).Data());
    // TString out = gSystem->GetFromPipe("find -name *_PRIMARY_*");
    // out.ReplaceAll("./","");
    // Printf("xrdcp -R -f %s root://eos.saske.sk/%s/",g2f_prefix.Data(), out_path.Data());
    gSystem->Exec(TString::Format("LD_LIBRARY_PATH= eos -b cp -r %s/%s root://eos.saske.sk/%s/",gSystem->WorkingDirectory(),g2f_prefix.Data(), out_path.Data()).Data());
    gSystem->Exec(TString::Format("rm -rf %s",g2f_prefix.Data()).Data());
  }

}
