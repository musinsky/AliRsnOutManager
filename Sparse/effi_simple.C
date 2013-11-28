void effi_simple(Int_t dataset = 201310, TString fname = "")
{
  TFile::SetCacheFileDir(gSystem->HomeDirectory());
  TFile *f = 0;
  if (fname.IsNull()) {
    if (dataset == 201310)
      f = TFile::Open("root://eos.saske.sk//eos/saske.sk/alice/rsn/PHIKK/LHC12f1a/ESD/RSN_20131015/Merged/All/STD2010/00_DEFAULT/qualityonly/RsnOutput.root", "CACHEREAD");
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
  TH1 *hgen, *htrue;
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
    hgen->Rebin(2);   // now 2013-04 and 2013-10 have same binning as 2013-01
    htrue->Rebin(2);
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
  delete f;

  hgen->Rebin();   // now our working binning
  htrue->Rebin();

  htrue->Divide(hgen);
  htrue->SetTitle(Form("effi %d", dataset));
  htrue->SetMinimum(0.0);
  htrue->SetMaximum(0.8);
  htrue->GetXaxis()->SetRangeUser(0.0, 5.0);
  if (gPad) {
    htrue->Draw("same");
  }
  else {
    htrue->Draw();
    gStyle->SetGridColor(kGray);
    gStyle->SetOptStat(0);
    gPad->SetGrid();
  }

  H2F(htrue, "out.effi", kFALSE);
}

void H2F(const TH1 *h, TString name, Bool_t info = kTRUE)
{
  if (info) Printf("Creating file %s", name.Data());
  ofstream myfile;
  myfile.open(name.Data());
  TString str_tmp;
  Double_t x, y, ex, ey;
  for (Int_t i = 1; i < h->GetNbinsX(); i++) {
    x  = h->GetBinCenter(i);
    y  = h->GetBinContent(i);
    ex = h->GetBinWidth(i)/2.0;
    ey = h->GetBinError(i);
    str_tmp = TString::Format("%f %f %f %f", x, y, NanCheck(ex), NanCheck(ey));
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
