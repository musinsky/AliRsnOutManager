void run(Int_t id = 0, Int_t id_to = 0, Int_t c = 0,
         TString in_fname = "AnalysisResults.root",
         const char *path_Sparse_macro = "/eos/saske.sk/alice/rsn/PHIKK/00_ANALYSIS/RSN_20131015/RsnOut_macros", TString out_path = "/eos/saske.sk/scratch/ALICE/RSN_OUT/test/OUT/")
{
  gROOT->ProcessLine(TString::Format(".L %s/Sparse.C",path_Sparse_macro).Data());

  rsn_data = 20131015;
  rsn_data = 20140125;
  //  rsn_data = 20130106;

  input_new = kTRUE;
  isVoig = kTRUE;
  isBinCounting = kTRUE;
  mixing = kTRUE;
  // use Own Eff
  // effiTPC = kTRUE;

  landscape = kTRUE;
  landscape = kFALSE;

  year=2010;
  //year=2011;

  polynom = 1;
  // polynom = 2;

  gROOT->ProcessLine(".x extra.C");

  SetCombinations(c,polynom);

  if (binAnders) 
    g2f_prefix=TString::Format("%d_anders_%d/",rsn_data,binAndersIdx).Data();
  else 
    g2f_prefix=TString::Format("%d_%.2f/",rsn_data,del_step).Data();
  

  SetNameBordelNew(id,  0, year, kTRUE, "RsnOutput.root");
  AnalyzeSparse(kBlack);
  // SetNameBordelNew(id, 10, year, kTRUE, "RsnOutput.root");
  // AnalyzeSparse(kRed);
  // SetNameBordelNew(id, 20, year, kTRUE, "RsnOutput.root");
  // AnalyzeSparse(kGreen);
  //SetNameBordelNew(id, 30, year, kTRUE, "RsnOutput.root");
  //AnalyzeSparse(kBlue);
  // SetNameBordelNew(id, 40, year, kTRUE, "RsnOutput.root");
  // AnalyzeSparse(kMagenta);
  
  gSystem->Exec(TString::Format("mv *.pdf %s/",g2f_prefix.Data()).Data());

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
