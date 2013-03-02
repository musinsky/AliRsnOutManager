// Jan Musinsky, Martin Vala
// 2013-03-02

{
  if (gROOT->GetClass("AliRsnOutManager")) {
    Printf("AliRsnOutManager libraries was already loaded");
    return;
  }

  gSystem->AddIncludePath("-Iinclude");
  gSystem->SetBuildDir("tmp", kTRUE);

  gSystem->Load("libRIO");
  gSystem->Load("libGpad");
  gSystem->Load("lib/libAliRootMini.so");
  gSystem->Load("lib/libAliRsnOutManager.so");

  gStyle->SetHistLineColor(kBlack);
  gStyle->SetGridColor(kGray);
}
