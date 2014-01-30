void effi_simple(Int_t dataset = 201401, TString fname = "", Int_t brebin = 2)
{
  TFile::SetCacheFileDir(gSystem->HomeDirectory());
  TFile *f = 0;
  if (fname.IsNull()) {
    if (dataset == 201310)
      f = TFile::Open("root://eos.saske.sk//eos/saske.sk/alice/rsn/PHIKK/LHC12f1a/ESD/RSN_20131015/Merged/All/STD2010/00_DEFAULT/qualityonly/RsnOutput.root", "CACHEREAD");
    if (dataset == 201401)
      f = TFile::Open("root://eos.saske.sk//eos/saske.sk/alice/rsn/PHIKK/LHC12f1a/ESD/RSN_20140125/Merged/All/STD2010/00_DEFAULT/qualityonly/RsnOutput.root", "CACHEREAD");
    if (dataset == 201304)
      f = TFile::Open("root://eos.saske.sk//eos/saske.sk/alice/rsn/PHIKK/LHC12f1a/ESD/RSN_20130411/All/STD2010/00_DEFAULT/qualityonly/RsnOutput.root", "CACHEREAD");
    if (dataset == 201301)
      f = TFile::Open("root://eos.saske.sk//eos/saske.sk/scratch/ALICE/RSN/RESULTS/Rsn_Phi/pp_2.76/2013-01-06/MC_LHC12i6_146805_ESD/00_DEFAULT/AnalysisResults.root", "CACHEREAD");
  }
  else
    f = TFile::Open(fname.Data());

  if (!f) return;
  THnSparse *sparse;
  TH1::AddDirectory(kFALSE);
  TH1 *hgen, *htrue, *hres;
  TString tmp(f->GetName());

  if (tmp.Contains("RsnOutput")) {   // new datasets
    f->GetObject("Generated", sparse);
    sparse->GetAxis(2)->SetRangeUser(-0.5, 0.5);
    hgen = sparse->Projection(1);
    delete sparse;
    f->GetObject("Trues", sparse);
    sparse->GetAxis(2)->SetRangeUser(-0.5, 0.5);
    htrue = sparse->Projection(1);
    delete sparse;
    Int_t rebinFac=2;

    hgen->Rebin(rebinFac);   // now 2013-04 and 2013-10 have same binning as 2013-01
    htrue->Rebin(rebinFac);
  }
  else {   // old dataset
    TList *list;
    f->GetObject("RsnHistMini_Phi_PhiNsigma_qualityonly_STD2010_PRIMARY", list);
    sparse = (THnSparse *)list->FindObject("RsnMini_phi_MCGenphi.PhiNsigma_qualityonly_STD2010_PRIMARY");
    hgen = sparse->Projection(1);
    delete sparse;
    sparse = (THnSparse *)list->FindObject("RsnMini_phi.PhiNsigma_qualityonly_STD2010_PRIMARY_Trues");
    htrue = sparse->Projection(1);
    delete sparse;
  }

  if (brebin == 0) {
    Double_t bbAnders[9] = {0.50, 0.80, 1.00, 1.50, 2.00, 2.50, 3.00, 4.00, 5.00}; // same as binning in Sparse.C
    TH1 *hgenR = (TH1 *)hgen->Rebin(8, "hgenR", bbAnders);
    TH1 *htrueR = (TH1 *)htrue->Rebin(8, "htrueR", bbAnders);
    hgen = hgenR;
    htrue = htrueR;
  }
  else {
    hgen->Rebin(brebin);   // now our working binning
    htrue->Rebin(brebin);
  }

  f->GetObject("Resolution", sparse);
  sparse->GetAxis(2)->SetRangeUser(-0.5, 0.5);
  Double_t min, max;
  Double_t resVal[100][2];
  for (Int_t i = 1; i < (hgen->GetNbinsX() + 1); i++) {
    min = hgen->GetBinLowEdge(i);
    max = hgen->GetBinLowEdge(i) + hgen->GetBinWidth(i);
    //    Printf("%7f, %7f", min, max);
    sparse->GetAxis(1)->SetRangeUser(min, max);
    hres = sparse->Projection(0);
    hres->Fit("gaus", "Q");

    TF1 *ff = hres->GetListOfFunctions()->FindObject("gaus");
    // add sigma
    if (!ff) resVal[i-1][0] = 0.0;
    else resVal[i-1][0] = ff->GetParameter(2);
    // add error in sigma
    if (!ff) resVal[i-1][1] = 0.0;
    else resVal[i-1][1] = ff->GetParError(2);
    Printf("%d, %f, %7f +- %7f", i-1, hgen->GetBinCenter(i), resVal[i-1][0], resVal[i-1][1]);
  }
  delete hres;
  delete sparse;
  delete f;

  TFile fout("out.root", "RECREATE");
  fout.cd();
  hgen->Write("hgen");
  htrue->Write("htrue");
  fout.Close();

  htrue->Divide(htrue, hgen, 1., 1., "B");
  htrue->SetTitle(Form("effi %d", dataset));
  htrue->SetMinimum(0.0);
  htrue->SetMaximum(0.8);
  // htrue->GetXaxis()->SetRangeUser(0.0, 5.0);
  if (gPad) {
    htrue->Draw("same");
  }
  else {
    htrue->Draw();
    gStyle->SetGridColor(kGray);
    gStyle->SetOptStat(0);
    gPad->SetGrid();
  }

  H2F(htrue, "out.effi", kTRUE, resVal);

}

void H2F(const TH1 *h, TString name, Bool_t info = kTRUE, Double_t extra[][2])
{
  if (info) Printf("Creating file %s", name.Data());
  ofstream myfile;
  myfile.open(name.Data());
  TString str_tmp;
  Double_t x, y, ex, ey;
  for (Int_t i = 1; i < (h->GetNbinsX() + 1); i++) {
    x  = h->GetBinCenter(i);
    y  = h->GetBinContent(i);
    ex = h->GetBinWidth(i)/2.0;
    ey = h->GetBinError(i);
    str_tmp = TString::Format("%f %f %f %f %f %f %f", x, y, NanCheck(ex), NanCheck(ey), extra[i-1][0], NanCheck(ex), extra[i-1][1]);
    if (info) Printf("%s", str_tmp.Data());
    myfile << str_tmp.Data() << endl;
  }
  myfile.close();
}

Double_t NanCheck(Double_t value, Double_t retvalue = 0.0)
{
  if (TMath::IsNaN(value)) return retvalue;
  return value;
}
