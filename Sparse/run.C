void run(Int_t id=0,Int_t id_to=0, const char *path_Sparce_macro="") {

  gROOT->ProcessLine(TString::Format(".L %sSparse.C",path_Sparce_macro));

  eff_prefix="EFFI_OK/effi_";
  del_step = 0.1;
  SetNameBordel(id, 30, 2010, 1);
  SetCombinations(1);

  AnalyzeSparse();
}
