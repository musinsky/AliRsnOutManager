{
  gROOT->LoadMacro("Sparse.C");
  rsn_data = 20131015;
  input_new = kTRUE;
  del_step = 0.20;
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
}
