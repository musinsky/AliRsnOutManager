// Jan Musinsky, Martin Vala
// 2012-06-26

{
  if (gROOT->GetClass("AliRsnOutManager")) {
    Printf("AliRsnOutManager libraries was already loaded");
    return;
  }

  gSystem->AddIncludePath("-Iinclude");
  gSystem->SetBuildDir("tmp", kTRUE);

  gSystem->Load("libRIO");
  gSystem->Load("lib/libAliRootMini.so");
  gSystem->Load("lib/libAliRsnOutManager.so");

  gStyle->SetHistLineColor(kBlack);
  gStyle->SetGridColor(kGray+2);
  gStyle->SetOptDate(21);
  gStyle->SetDateX(0.005);
  gStyle->SetDateY(0.005);
  gStyle->GetAttDate()->SetTextSize(0.015);
}
