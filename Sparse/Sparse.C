Bool_t effiTPC = kFALSE;
Bool_t binAnders = kFALSE;
Bool_t useCF = kFALSE;
Bool_t mixing  = kFALSE;
Bool_t noSigma = kFALSE;
Double_t sigma = 1.0;
Double_t norm[2]  = {1.045, 1.085};
Double_t del_step = 0.25;
Bool_t mc = kFALSE;
Bool_t isTPC = kTRUE;
Bool_t isVoig = kFALSE; // !!!!!!!!!!!!
Double_t fmin = 0.995;
Double_t fmax = 1.185;
Double_t fipm = 3.0;
TString mv_colon = ":";
// mv_colon = "_";
Int_t combi = 0;

TString fname,lname,s1name,s3name_p,s3name_m,smix,smixpp,smixmm, graph_name,
  graphee_name;
TMultiGraph *m_gr     = new TMultiGraph();
TMultiGraph *m_gr_mass= new TMultiGraph();
TMultiGraph *m_gr_fix = new TMultiGraph();

const char *suf[12] = {"00_DEFAULT", "CHI2ITS036", "CHI2ITS100", "CHI2TPC04",
                       "CHI2TPC06", "DCAXY035", "DCAXY140", "DCAZ01", "DCAZ20",
                       "NCLSTTPC50", "NCLSTTPC70", "NCLSTTPC80"};
const char *what   = "";
Int_t ilist = 0;

TH1D *hg, *h1, *h3_p, *h3_m, *ht;
TMultiGraph *m_gr = new TMultiGraph();

void SetCombinations(Int_t c = 0)
{
  if (c == 0) { // default
    effiTPC = kFALSE; // common or own TPC effi, correct only for 2013_01
    mixing  = kFALSE;
    norm[0] = 1.045;  // where is norm signal and background
    norm[1] = 1.085;
    fmin    = 0.995;  // where is fit
    fmax    = 1.185;
    fipm    = 3.0;    // where is integral (+- 'fipm' sigma)
    combi   = c;
  }
  if (c == 1) {
    effiTPC = kFALSE;
    mixing  = kFALSE;
    norm[0] = norm[0] - 0.01;
    norm[1] = norm[1] + 0.01;
    fmin    = fmin - 0.01;
    fmax    = fmax + 0.01;
    fipm    = 5.0;
    combi   = c;
  }
  if (c == 2) {
    effiTPC = kFALSE;
    mixing  = kFALSE;
    norm[0] = norm[0] - 0.01;
    norm[1] = norm[1] + 0.05;
    fmin    = fmin - 0.02;
    fmax    = fmax + 0.02;
    fipm    = 4.0;
    combi   = c;
  }
}

void SetNameBordel(Int_t fsuf, Int_t qc, Int_t std10or11, Bool_t info=kFALSE, const char *my_fname="AnalysisResults.root")
{
  //  fname = Form("~/ALICE_RSN/pp_2.76/2013-01-06/DATA_LHC11a_ESD/%s/%s", suf[fsuf], my_fname);
  fname = Form("root://eos.saske.sk//eos/saske.sk/users/m/mvala/ALICE/RSN/RESULTS/Rsn_Phi/pp_2.76/2013-04-11/DATA/%s/%s", suf[fsuf], my_fname);

  const char *tmp_qc = "";
  if      (qc == 0)  { tmp_qc = "qualityonly"; ilist = 0; sigma = 0.0;
    noSigma = kTRUE; }
  else if (qc == 15) { tmp_qc = "KTPCnsig15";  ilist = 2; sigma = 1.5;
    noSigma = kFALSE; }
  else if (qc == 20) { tmp_qc = "KTPCnsig20";  ilist = 3; sigma = 2.0;
    noSigma = kFALSE; }
  else if (qc == 25) { tmp_qc = "KTPCnsig25";  ilist = 4; sigma = 2.5;
    noSigma = kFALSE; }
  else if (qc == 30) { tmp_qc = "KTPCnsig30";  ilist = 5; sigma = 3.0;
    noSigma = kFALSE; }
  else {
    Printf("Bad TPC cut !!!!!!!!!!!");
    return;
  }
  const char *tmp_10or11 = "";
  if      (std10or11 == 2010) tmp_10or11 = "STD2010_PRIMARY";
  else if (std10or11 == 2011) tmp_10or11 = "STD2011_PRIMARY_NCLSTTPC";
  else {
    Printf("Bad 2010 or 2011 cut !!!!!!!!!!!");
    return;
  }

  if (fsuf == 0) { // bo Mato
    lname = Form("RsnHistMini_Phi_PhiNsigma%s%s_%s",
                 mv_colon.Data(),tmp_qc, tmp_10or11);
    what  = Form("PhiNsigma_%s_%s",
                 tmp_qc, tmp_10or11);
    graph_name = Form("%s_%s", what, suf[fsuf]);
  }
  else {
    lname = Form("RsnHistMini_Phi_PhiNsigma%s%s_%s_%s",
                 mv_colon.Data(),tmp_qc, tmp_10or11, suf[fsuf]);
    what  = Form("PhiNsigma_%s_%s_%s",
                 tmp_qc, tmp_10or11, suf[fsuf]);
    graph_name = what;
  }

  graphee_name =
    Form("PhiNsigma_qualityonly_%s_%s", tmp_10or11, suf[fsuf]);

  s1name     = Form("RsnMini_phi.%s_Unlike",   what);
  s3name_p   = Form("RsnMini_phi.%s_LikePP",   what);
  s3name_m   = Form("RsnMini_phi.%s_LikeMM",   what);
  smix       = Form("RsnMini_phi.%s_Mixing",   what);
  smixpp     = Form("RsnMini_phi.%s_MixingPP", what);
  smixmm     = Form("RsnMini_phi.%s_MixingMM", what);

  if (!info) return;
  Printf("file name: %s", fname.Data());
  Printf("list name: %s", lname.Data());
  Printf("\n1 - %s\n2 - %s\n3 - %s\n4 - %s\n5 - %s\n6 - %s\n",
         s1name.Data(), s3name_p.Data(), s3name_m.Data(),
         smix.Data(), smixpp.Data(), smixmm.Data());
}

Double_t fun_s(double *m, double *par) const
{
  Double_t val = 0.0;
  double x     = m[0];
  val = TMath::BreitWigner(x, par[1], par[2]);
  return par[0]*val + par[3] + x*par[4] + x*x*par[5] + x*x*x*par[6];
}

Double_t fun_s2(double *m, double *par) const
{
  Double_t val = 0.0;
  double x     = m[0];
  val = TMath::Voigt(x - par[1], par[3], par[2]);
  return par[0]*val + par[4] + x*par[5] + x*x*par[6] + x*x*x*par[7];
}

Double_t Levy(const Double_t *pt, const Double_t *par) const
{
  Double_t lMass  = par[3];
  Double_t ldNdy  = par[0];
  Double_t l2pi   = 2*TMath::Pi();
  Double_t lTemp = par[1];
  Double_t lPower = par[2];

  Double_t lBigCoef = ((lPower-1)*(lPower-2)) / (l2pi*lPower*lTemp*(lPower*lTemp+lMass*(lPower-2)));
  Double_t lInPower = 1 + (TMath::Sqrt(pt[0]*pt[0]+lMass*lMass)-lMass) / (lPower*lTemp);

  return pt[0]*(ldNdy * pt[0] * lBigCoef * TMath::Power(lInPower,(-1.0)*lPower));
}
TF1 *fl = new TF1("fl", Levy, 0., 4., 4);
fl->SetParameters(13732, 0.2, 6.56, 1.02);
fl->SetLineStyle(2);
fl->SetLineWidth(2);

Double_t LevyTsallis(const Double_t *pt, const Double_t *par) const
{
  Double_t ldNdy  = par[0];
  Double_t lTemp  = par[1];
  Double_t lPower = par[2];
  Double_t lMass  = par[3];

  Double_t lBigCoef = ((lPower-1.0)*(lPower-2.0)) /
    (lPower*lTemp*(lPower*lTemp + lMass*(lPower-2.0)));
  Double_t lInPower = 1.0 + (TMath::Sqrt(pt[0]*pt[0]+lMass*lMass) - lMass) /
    (lPower*lTemp);
  lInPower = TMath::Power(lInPower, (-1.0)*lPower);

  return lBigCoef * ldNdy * pt[0] * lInPower;
}
TF1 *flt = new TF1("flt", LevyTsallis, 0., 5., 4);
flt->SetParameters(13732, 0.2, 6.56, 1.02);
//flt->FixParameter(3, 1.019445);
flt->SetLineStyle(2);
flt->SetLineWidth(2);
flt->SetLineColor(kMagenta+1);


void AnalyzeSparse(Color_t lcolor = -1)
{
  gStyle->SetGridColor(kGray);

  TH1D *hg, *h1, *h3_p, *h3_m, *ht;
  TString gtitle;
  if (mc) gtitle = Form("Monte Carlo, %s", graph_name.Data());
  else    gtitle = Form("Real Data, %s", graph_name.Data());
  Double_t grx[999], gry[999], gry2[999], gry3[999], gry4[999],
    gry_eff[999], gry_fix[999], grxE[999],  gryE[999], gry_fixE[999];
  Double_t gr_mass[999], gr_massE[999], gr_width[999], gr_widthE[999];
  Double_t gry_true[999], gry_true_eff[999];
  TH1::AddDirectory(kFALSE);

  TFile::SetCacheFileDir(gSystem->HomeDirectory()); // direct "$HOME" or "~" not working
  TFile *f = TFile::Open(fname.Data(), "CACHEREAD");
  if (!f) return;
  TList *l; f->GetObject(lname.Data(), l);
  if (!l) return;
  Int_t bf[999], bl[999];
  Int_t nn = FindExactRange(((THnSparse *)(l->FindObject(s1name.Data())))->
                            Projection(1), del_step, bf, bl);
  //  Int_t nn = FindRange5(bf, bl);
  Bool_t binhaluska = kFALSE;
  //  binhaluska = kTRUE;
  if (binAnders) {
    nn = 8;
    bf[0] = 6;bf[1] =  9;bf[2] = 11;bf[3] = 16;bf[4] = 21;bf[5] = 26;
    bl[0] = 8;bl[1] = 10;bl[2] = 15;bl[3] = 20;bl[4] = 25;bl[5] = 30;

    bf[6] = 31;bf[7] = 41;
    bl[6] = 40;bl[7] = 50;
  }

  Printf("number of intervals = %d", nn);
  Int_t count = 0;
  Double_t ptmean = 0, value = 0;
  Double_t bwidth[999] = {0};
  Int_t fitStatus = -1;
  gStyle->SetOptStat(0);
  gStyle->SetOptFit();
  TCanvas *c = new TCanvas();
  c->SetTitle(graph_name.Data());
  c->Divide(6, 4, 0.0005, 0.0005); c->Modified(); c->Draw();
  TCanvas *c2 = (TCanvas *)c->DrawClone("c2");
  c2->Modified(); c2->Draw();
  TCanvas *c3, *c4;
  if (mc) {
    c3 = (TCanvas *)c->DrawClone("c3");
    c3->SetTitle("Phi mesons (gen)"); c3->Modified(); c3->Draw();
    c4 = (TCanvas *)c->DrawClone("c4");
    c4->SetTitle("Phi mesons (true)"); c4->Modified(); c4->Draw();
  }

  for (Int_t i = 0; i < nn; i++) {
    c->cd(count + 1)->SetGrid();
    h1 = (TH1D *)PullHisto(l, s1name.Data(), bf[i], bl[i], ptmean, bwidth[i]);
    h1->SetLineColor(kRed);
    h1->GetXaxis()->SetTitle("M_{ inv} [GeV/c^{2}]");
    //    h1->GetXaxis()->SetRangeUser(0.995, 1.095);
    h1->GetXaxis()->SetLabelSize(0.05);
    h1->GetYaxis()->SetLabelSize(0.05);
    h1->GetYaxis()->SetNdivisions(409);
    h1->Draw("hist");

    if (!mixing) {
      h3_p = (TH1D *)PullHisto(l, s3name_p.Data(), bf[i], bl[i], ptmean, bwidth[i]);
      h3_m = (TH1D *)PullHisto(l, s3name_m.Data(), bf[i], bl[i], ptmean, bwidth[i]);
      h3_p->Add(h3_m);
    }
    else {
      //      h3_m = (TH1D *)PullHisto(l, smixpp.Data(), bf[i], bl[i], ptmean, bwidth[i]);
      //      h3_p->Add(h3_m);
      h3_p = (TH1D *)PullHisto(l, smixpp.Data(), bf[i], bl[i], ptmean, bwidth[i]);
      h3_m = (TH1D *)PullHisto(l, smixmm.Data(), bf[i], bl[i], ptmean, bwidth[i]);
      h3_p->Add(h3_m);
      h3_m = (TH1D *)PullHisto(l, smix.Data(), bf[i], bl[i], ptmean, bwidth[i]);
      h3_p->Add(h3_m);
    }

    Norm(h1, h3_p, norm[0], norm[1]);
    if (h3_p->Integral() != h3_p->Integral()) // isnan
      h3_p->Reset();
    h3_p->SetLineColor(kBlue);
    h3_p->Draw("hist, same");

    if (mc) {
      c3->cd(count + 1)->SetGrid();
      hg = (TH1D *)PullHisto(l, s1namegen.Data(), bf[i], bl[i], ptmean, bwidth[i]);
      hg->SetLineColor(kMagenta);
      hg->GetXaxis()->SetTitle("M_{ inv} [GeV/c^{2}]");
      hg->Draw("hist");

      c4->cd(count + 1)->SetGrid();
      ht = (TH1D *)PullHisto(l, s1nametrue.Data(), bf[i], bl[i], ptmean, bwidth[i]);
      ht->SetLineColor(kMagenta-5);
      ht->GetXaxis()->SetTitle("M_{ inv} [GeV/c^{2}]");
      ht->Draw("hist");
    }

    c2->cd(count + 1)->SetGrid();
    TH1 *hh = (TH1 *)h1->Clone("hh");
    hh->SetLineColor(kRed+1);
    hh->Add(h3_p, -1);
    hh->Draw("hist");
    //    hh->GetXaxis()->SetRangeUser(0.995, 1.095);

    // !!!!!!!!!!!!!!!!!!
    TF1 *ff = 0;
    Int_t del = 0;
    if (isVoig) {
      ff = new TF1("ff", fun_s2, 0.9, 1.2, 8);
      del = 1; // change only from parametger[3]
    }
    else {
      ff = new TF1("ff", fun_s, 0.9, 1.2, 7);
      del = 0;
    }

    ff->SetParameters(2.0, 1.02, 0.004, 0., 0., 0., 0., 0.);
    //    if (noSigma) ff->SetParameters(200.0, 1.02, 0.004, 0., 0., 0., 0., 0.);
    //    ff->SetLineColor(hh->GetLineColor()-3);
    ff->SetLineColor(kGreen-3);
    ff->SetLineWidth(2);
    if (isVoig) {
      if (isTPC) ff->FixParameter(3, 0.00118398);
      else       ff->FixParameter(3, 0.00115599);
    }

    // where fit
    //    Double_t fmin = 0.995;
    //    Double_t fmax = 1.185;
    // change from 2013-01-28
    //    fmin = 1.00999;
    //    fmax = 1.0678000000800000000165;
    // !!!!!!!!!!!!!!!!!!
    Bool_t hisfun = kFALSE; // kFALSE = integral from function
    Double_t       hisfun_k = 1.0/hh->GetBinWidth(10);
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (binhaluska)
      if (count > 9) hisfun_k = 0.50/hh->GetBinWidth(10);

    // !!!!!!!!!!!!!!!!!!
    // wehere integral (his or fun)
    Double_t fmini = 1.02-2*0.004;
    Double_t fmaxi = 1.02+2*0.004;
    hh->Fit(ff, "Q", "", fmin, fmax);
    hh->Fit(ff, "Q", "", fmin, fmax);
    fitStatus = hh->Fit(ff, "Q", "", fmin, fmax);
    // if (ff->GetParameter(1)!=TMath::Range(fmini, fmaxi, ff->GetParameter(1))) {
    //   // peak is around 1.02
    //   ff->SetParameters(0.5, 1.02, 0.0001, -300., 250., 300., -250.);
    //   hh->Fit(ff, "Q", "", fmin, fmax);
    //   hh->Fit(ff, "Q", "", fmin, fmax);
    //   fitStatus = hh->Fit(ff, "Q", "", fmin, fmax);
    // }
    // if (ff->GetParameter(1)!=TMath::Range(fmini, fmaxi, ff->GetParameter(1))) {
    //   // peak is around 1.02
    //   ff->SetParameters(50, 1.02, 0.0004,-2000000.,5000000.,-4000000.,1000000.);
    //   fmin = 1.00;
    //   fmax = 1.05;
    //   hh->Fit(ff, "Q", "", fmin, fmax);
    //   hh->Fit(ff, "Q", "", fmin, fmax);
    //   fitStatus = hh->Fit(ff, "Q", "", fmin, fmax);
    // }
    // ///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // // W - parameter
    // hh->Fit(ff, "QW", "", fmin, fmax);
    // hh->Fit(ff, "QW", "", fmin, fmax);
    // fitStatus = hh->Fit(ff, "QW", "", fmin, fmax);
    // //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    Double_t drawfmin = fmin; //0.990;
    Double_t drawfmax = fmax; //1.150;
    TF1 *pp3 = new TF1("pp3", "[0]+x*[1]+x*x*[2]+x*x*x*[3]",drawfmin,drawfmax);
    pp3->SetParameters(ff->GetParameter(3+del), ff->GetParameter(4+del),
                       ff->GetParameter(5+del), ff->GetParameter(6+del));
    pp3->SetLineWidth(1);
    pp3->SetLineColor(h3_p->GetLineColor());
    pp3->Draw("same");
    hh->GetListOfFunctions()->Add(pp3);

    gr_mass[count]   = ff->GetParameter(1);
    gr_massE[count]  = ff->GetParError(1);
    gr_width[count]  = ff->GetParameter(2);
    gr_widthE[count] = ff->GetParError(2);

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    fmini = ff->GetParameter(1) - fipm*ff->GetParameter(2);
    fmaxi = ff->GetParameter(1) + fipm*ff->GetParameter(2);
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    value              = hh->Integral(hh->FindBin(fmini), hh->FindBin(fmaxi));
    // pre histo doimplementovat odcitavanie BKG, pre fun uz je hotove
    if (!hisfun) value = ff->Integral(fmini, fmaxi)*hisfun_k -
                   pp3->Integral(fmini, fmaxi)*hisfun_k;
    if (value < 0) value = 0;

    // if (ff->GetNDF() < (hh->GetNbinsX()*0.50)) {
    //   printf(" SKIP 0 Data");
    //   pp3->SetLineWidth(3);
    //   value = 0;
    // }
    if ((fitStatus != 0) || (TMath::Abs(ff->GetParameter(2)) > (0.004*25))) {
      printf(" SKIP 1 Data");
      pp3->SetLineWidth(3);
      value = 0;
    }
    if ((ff->GetParameter(2) < 0.0) ||
        (TMath::Abs(ff->GetParameter(2)) < (0.004/5.0)) ||
        (TMath::Abs(ff->GetParameter(2)) > (0.004*5.0))) {
      printf(" SKIP 2 Data");
      pp3->SetLineWidth(3);
      value = 0;
    }
    Double_t ppp1 = ff->GetParameter(1);
    if (ppp1 != TMath::Range(fmini, fmaxi, ppp1)) { // peak is around 1.02
      printf(" SKIP 3 Data");
      pp3->SetLineWidth(3);
      value = 0;
    }

    grx[count] = ptmean;
    if (binhaluska) {
      if (count < 10) grxE[count] = 0.25; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      else            grxE[count] = 0.50; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
    else
      //      grxE[count] = (3.30-3.10)/2.0; // !!!!!!!!!!!!!!!!!!!!!!!!!!
      // 2013-06-10
      grxE[count] = (bwidth[i]/2.0);
    gry[count] = value;
    gryE[count] = TMath::Sqrt(gry[count]); // !!!!!!!!!!!!!!!!

    // Signal, Background
    Double_t tmp1 = h1->Integral(h1->FindBin(fmini), h1->FindBin(fmaxi));
    Double_t tmp2 = h3_p->Integral(h3_p->FindBin(fmini), h3_p->FindBin(fmaxi));

    tmp1 = TMath::Abs(tmp1);
    tmp2 = TMath::Abs(tmp2);
    Double_t tmp_sg = tmp1 - tmp2;
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!
    tmp_sg = value;
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!
    Double_t tmp_bg = tmp2;

    // SB, S, B
    Printf("%6d, %6d, %6d", (Int_t)tmp1, (Int_t)tmp_sg, (Int_t)tmp2);

    if ((tmp_sg <= -tmp_bg) || (tmp_bg < 1.0)) {
      gry3[count] = 0.0;
      gry4[count] = 0.0;
    }
    else {
      gry3[count] = tmp_sg/tmp_bg;
      gry4[count] = tmp_sg/TMath::Sqrt(tmp_sg + tmp_bg);
    }

    if (mc) {
      c3->cd(count + 1);
      // !!!!!!!!!!!!!!!!
      ff->SetParameters(200, 1.02, 0.004, 0., 0., 0., 0.);
      hg->Fit(ff, "Q", "", fmin, fmax);
      hg->Fit(ff, "Q", "", fmin, fmax);
      fitStatus = hg->Fit(ff, "Q", "", fmin, fmax);
      TF1 *pp3 = new TF1("pp3", "[0]+x*[1]+x*x*[2]+x*x*x*[3]", fmin, fmax);
      pp3->SetParameters(ff->GetParameter(3), ff->GetParameter(4),
                         ff->GetParameter(5), ff->GetParameter(6));
      pp3->SetLineWidth(1);
      pp3->SetLineColor(h3_p->GetLineColor());
      pp3->Draw("same");

      value              = hg->Integral(hg->FindBin(fmini), hg->FindBin(fmaxi));
      if (!hisfun) value = ff->Integral(fmini, fmaxi)*hisfun_k;
      if (value <= 0) value = -1;
      if ((fitStatus != 0) || (TMath::Abs(ff->GetParameter(2)) > (0.004*25))) {
        printf(" SKIP MC");
        value = -1;
      }
      if ((ff->GetParameter(2) < 0.0) ||
          (TMath::Abs(ff->GetParameter(2)) < (0.004/2.0))) {
        printf(" SKIP MC");
        pp3->SetLineWidth(3);
        value = -1;
      }
      gry2[count]    = value;
      gry_eff[count] = gry[count]/gry2[count];

      c4->cd(count + 1);
      // !!!!!!!!!!!!!!!!
      ff->SetParameters(200, 1.02, 0.004, 0., 0., 0., 0.);
      ht->Fit(ff, "Q", "", fmin, fmax);
      ht->Fit(ff, "Q", "", fmin, fmax);
      fitStatus = ht->Fit(ff, "Q", "", fmin, fmax);
      TF1 *pp3 = new TF1("pp3", "[0]+x*[1]+x*x*[2]+x*x*x*[3]", fmin, fmax);
      pp3->SetParameters(ff->GetParameter(3), ff->GetParameter(4),
                         ff->GetParameter(5), ff->GetParameter(6));
      pp3->SetLineWidth(1);
      pp3->SetLineColor(h3_p->GetLineColor());
      pp3->Draw("same");

      value              = ht->Integral(ht->FindBin(fmini), ht->FindBin(fmaxi));
      if (!hisfun) value = ff->Integral(fmini, fmaxi)*hisfun_k;
      if (value <= 0) value = -1;
      if ((fitStatus != 0) || (TMath::Abs(ff->GetParameter(2)) > (0.004*25))) {
        printf(" SKIP true");
        value = -1;
      }
      if ((ff->GetParameter(2) < 0.0) ||
          (TMath::Abs(ff->GetParameter(2)) < (0.004/2.0))) {
        printf(" SKIP true");
        pp3->SetLineWidth(3);
        value = -1;
      }

      gry_true[count]     = value;
      gry_true_eff[count] = gry_true[count]/gry2[count];
    }
    Printf("=> %4.2f\n", ptmean);
    count++;
  }
  // save peaks pictures
  //  c->SaveAs(Form("%s_a.pdf", lname.Data()));
  //  c2->SaveAs(Form("%s_b.pdf", lname.Data()));

  TGraphErrors *gr_raw = new TGraphErrors(count, grx, gry, grxE, gryE);
  G2F(gr_raw, TString::Format("%s_%02d_%s", lname.Data(), combi, "R"));

  // applying super macro
  Double_t superfactor;
  for (Int_t i = 0; i < count; i++) {
    // SuperMacro
    superfactor = CalculateFactor(l, bwidth[i]);
    gry[i] = gry[i]*superfactor;
    gryE[i] = gryE[i]*superfactor;
  }

  //  new TCanvas();
  TGraphErrors *gr = new TGraphErrors(count, grx, gry, grxE, gryE);
  gr->SetMarkerStyle(8);
  gr->SetMarkerColor(hh->GetLineColor());
  gr->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  gr->SetTitle(Form("raw phi, %s", gtitle.Data()));
  gr->SetName(Form("%s_raw", gtitle.Data()));
  m_gr->Add(gr);
  gr->Draw("AP");
  gPad->Update();
  gPad->Modified();
  G2F(gr, TString::Format("%s_%02d_%s", lname.Data(), combi, "RF"));

  new TCanvas();
  TGraphErrors *grm = new TGraphErrors(count, grx, gr_mass, grxE, gr_massE);
  grm->SetMarkerStyle(8);
  grm->SetMarkerColor(kGray+2);
  if (lcolor > 0) grm->SetMarkerColor(lcolor);
  grm->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  grm->SetTitle(Form("mass, %s", gtitle.Data()));
  grm->SetName(Form("%s_mass", gtitle.Data()));
  grm->SetMinimum(1.016);
  grm->SetMaximum(1.022);
  m_gr_mass->SetMinimum(grm->GetMinimum());
  m_gr_mass->SetMaximum(grm->GetMaximum());
  m_gr_mass->Add(grm);
  grm->Draw("AP");
  new TCanvas();
  TGraphErrors *grw = new TGraphErrors(count, grx, gr_width, grxE, gr_widthE);
  grw->SetMarkerStyle(8);
  grw->SetMarkerColor(kGray);
  grw->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  grw->SetTitle(Form("width, %s", gtitle.Data()));
  grw->SetName(Form("%s_width", gtitle.Data()));
  grw->SetMinimum(0.0);
  grw->SetMaximum(0.015);
  grw->Draw("AP");

  //  cc3 = new TCanvas();
  TGraph *gr3 = new TGraph(count, grx, gry3);
  gr3->SetMarkerStyle(22);
  gr3->SetMarkerColor(kBlue+1);
  gr3->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  gr3->SetTitle(Form("SIG / BKG, %s", gtitle.Data()));
  gr3->SetMinimum(0);
  //  gr3->Draw("AP");

  //  cc4 = new TCanvas();
  TGraph *gr4 = new TGraph(count, grx, gry4);
  gr4->SetMarkerStyle(23);
  gr4->SetMarkerColor(kBlue-1);
  gr4->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  gr4->SetTitle(Form("Significance, %s", gtitle.Data()));
  gr4->SetMinimum(0);
  //  gr4->Draw("AP");

  ccc = new TCanvas();
  ccc->SetWindowSize(900, 300);
  ccc->Divide(2, 1, 0.001, 0.001);
  ccc->cd(1); gr3->Draw("AP");
  ccc->cd(2); gr4->Draw("AP");

  TString blabla = "mc";
  if (!mc) blabla = "data";
  // gr3->SaveAs(Form("SB_%s_%s.C", blabla.Data(), graph_name.Data()));
  // gr4->SaveAs(Form("Sig_%s_%s.C", blabla.Data(), graph_name.Data()));
  // ccc->SaveAs(Form("%s_%s_2.eps", blabla.Data(), graph_name.Data()));
  // c->SaveAs(Form("%s_%s_0.eps", blabla.Data(), graph_name.Data()));
  // c2->SaveAs(Form("%s_%s_1.eps", blabla.Data(), graph_name.Data()));
  //  cc3->SaveAs(Form("%s_%s_2.eps", blabla.Data(), graph_name.Data()));
  //  gr3->SaveAs(Form("sig_bck_%s_%s.C", blabla.Data(), graph_name.Data()));

  if (mc) {
    new TCanvas();
    TGraph *gr2 = new TGraph(count, grx, gry2);
    gr2->SetMarkerStyle(8);
    gr2->SetMarkerColor(hg->GetLineColor());
    gr2->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    gr2->SetTitle(Form("gen phi, %s", gtitle.Data()));
    gr2->Draw("AP");
    c = new TCanvas();
    c->SetGrid();
    TGraph *gr_e = new TGraph(count, grx, gry_eff);
    gr_e->SetMarkerStyle(22);
    gr_e->SetMarkerColor(kBlack);
    gr_e->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    gr_e->SetTitle(Form("efficiency (raw), %s", graph_name.Data()));
    gr_e->Draw("AP");
    Printf("Save as '\033[1meffi_raw_%s\033[0m' file", graph_name.Data());
    for (Int_t i = 0; i < gr_e->GetN(); i++)
      Printf("%f %f", gr_e->GetX()[i], gr_e->GetY()[i]);

    new TCanvas();
    TGraph *gr_true = new TGraph(count, grx, gry_true);
    gr_true->SetMarkerStyle(8);
    gr_true->SetMarkerColor(ht->GetLineColor());
    gr_true->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    gr_true->SetTitle(Form("true phi, %s", gtitle.Data()));
    gr_true->Draw("AP");
    c = new TCanvas();
    c->SetGrid();
    TGraph *gr_true_eff = new TGraph(count, grx, gry_true_eff);
    gr_true_eff->SetMarkerStyle(23);
    gr_true_eff->SetMarkerColor(kBlack);
    gr_true_eff->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    gr_true_eff->SetTitle(Form("efficiency (true), %s", graph_name.Data()));
    gr_true_eff->Draw("AP");
    Printf("Save as '\033[1meffi_true_%s\033[0m' file", graph_name.Data());
    for (Int_t i = 0; i < gr_true_eff->GetN(); i++)
      Printf("%f %f", gr_true_eff->GetX()[i], gr_true_eff->GetY()[i]);

    // ------------------
    c = new TCanvas("cfinal", "mc_effi", 1200, 450);
    c->Divide(2, 1, 0.001, 0.001); c->Modified(); c->Draw();
    c->cd(1);
    gr_true->SetMinimum(0);
    gr_true->SetTitle(Form("phi (true & raw), %s", gtitle.Data()));
    gr_true->SetMarkerColor(kGreen+1);
    gr_true->Draw("AP");
    gr->SetMarkerColor(kRed+1);
    gr->Draw("P");
    c->cd(2)->SetGrid();
    gr_true_eff->SetMinimum(0);
    gr_true_eff->SetTitle(Form("efficiency, %s", graph_name.Data()));
    gr_true_eff->SetMarkerColor(kGreen+1);
    gr_true_eff->Draw("AP");
    gr_e->SetMarkerColor(kRed+1);
    gr_e->Draw("P");
    //    c->SaveAs(Form("%s_%s.eps", blabla.Data(), graph_name.Data()));
    return;
  }

  // //  TF1 *fun = new TF1("fun",
  // //                     "[2]+([3]*TMath::Erfc((x-[0])/([1]*TMath::Sqrt2())))");
  // TGraphErrors *geff;
  // if (isTPC) {
  //   geff = new TGraphErrors("effiE_TPC");
  //   //    fun->SetParameters(-1.5863940, -2.1219190, -2.2516170,  1.3835046);
  //   Printf("TPC effi");
  // }
  // else {
  //   geff = new TGraphErrors("effiE_TOF");
  //   //    fun->SetParameters( 1.1204853, -1.8916342, -0.1814699,  0.2136834);
  //   Printf("TOF effi_TOF");
  // }

  //  TGraphErrors *geff = new TGraphErrors(Form("EFFI_OK/effi_%s",
  //                                             graph_name.Data()));

  if (effiTPC) {
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    graphee_name =  graph_name;
    noSigma = kTRUE;
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  }
  TGraphErrors *geff = new TGraphErrors(Form("EFFI_OK/effi_%s",
                                             graphee_name.Data()));
  if (binAnders) {
    delete geff;
    geff = new TGraphErrors(Form("EFFI_Anders/effi_%s", graphee_name.Data()));
  }
  Printf("Open EFFI_OK/%s", graphee_name.Data());
  if (geff->IsZombie()) return;
  geff->SetMarkerStyle(20);
  geff->SetMarkerColor(kBlack);
  geff->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  geff->SetTitle(Form("efficiency, %s", graphee_name.Data()));
  //  c = new TCanvas();
  //  c->SetGrid();
  //  geff->Draw("AP");
  Double_t sss = TMath::Erf(sigma/TMath::Sqrt(2.0));
  if (noSigma) sss = 1.0;
  Printf("sigma = %3.2f => %10f", sigma, sss);
  TGraph *tmp = new TGraph(geff->GetN(), geff->GetX(), geff->GetEY()); // graph with error

  for (Int_t i = 0; i < count; i++) {
    Double_t deno = geff->Eval(grx[i])*(sss*sss);
    //    Double_t deno = geff->Eval(grx[i])*(sss*sss*sss*sss);
    if (deno < 0.0000001) deno = 1;
    gry_fix[i] = gry[i]/deno;
    // SuperMacro
    // Double_t superfactor = CalculateFactor(l, bwidth[i]);
    // if (useCF) gry_fix[i] = gry_fix[i]*superfactor;

    Double_t AAA  = gry[i];
    Double_t AAAE = gryE[i];
    Double_t BBB  = deno;
    Double_t BBBE = tmp->Eval(grx[i]);
    BBBE          = (sss*sss)*BBBE;       // constant*error of var
    //    BBBE          = (sss*sss*sss*sss)*BBBE;
    Double_t EEE  = TMath::Sqrt((AAAE/AAA)*(AAAE/AAA)+(BBBE/BBB)*(BBBE/BBB));
    EEE = EEE*gry_fix[i];
    gry_fixE[i] = EEE;
    if (binAnders)
      Printf("%16.2f %8.2f %.6f", gry_fix[i], gry_fixE[i], 0.123456);
  }

  if (binAnders) {
    Printf("Save as '\033[1mpp_2.76/pt.txt\033[0m'");
    for (Int_t i = 0; i < count; i++)
      Printf("%f %f %f", gry_fix[i], gry_fixE[i], gry_fixE[i]*1.10);
  }

  //  new TCanvas;
  TGraphErrors *gr_fix = new TGraphErrors(count, grx, gry_fix, grxE, gry_fixE);
  gr_fix->SetMarkerStyle(20);
  gr_fix->SetMarkerColor(hh->GetLineColor());
  gr_fix->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  gr_fix->SetTitle(Form("corrected phi * #sigma^{2}, %s", gtitle.Data()));
  if (noSigma)
    gr_fix->SetTitle(Form("corrected phi (no #sigma), %s", gtitle.Data()));
  m_gr_fix->Add(gr_fix);
  if (superfactor > 0.9) m_gr_fix->SetMinimum(1);
  //  gr_fix->Draw("AP");
  G2F(gr_fix, TString::Format("%s_%02d_%s", lname.Data(), combi, "RFE"));

  c = new TCanvas();
  c->SetWindowSize(1200, 450);
  c->Divide(2, 1, 0.001, 0.001); c->Modified(); c->Draw();
  c->cd(1);
  if (superfactor > 0.9) gr->SetMinimum(1);
  gr->SetMarkerColor(kBlack);
  gr->Draw("AP");
  c->cd(2);
  if (superfactor > 0.9) gr_fix->SetMinimum(1);
  gr_fix->SetMarkerColor(kGreen+3);
  if (lcolor > 0) {
    gr->SetMarkerColor(lcolor);
    gr_fix->SetMarkerColor(lcolor);
  }
  gr_fix->Draw("AP");
  TString bla9 = Form("qualityonly_PID2_%s", graph_name.Data());
  if (noSigma) bla9 = Form("%s_noSig.C", bla9.Data());
  else         bla9 = Form("%s.C", bla9.Data());
  //  gr_fix->SaveAs(bla9.Data());
  //  TPad *cp = new TPad("cpf", "", 0.45,0.45,0.99,0.92);
  TPad *cp = new TPad("cpf", "", 0.60,0.55,0.99,0.93);
  cp->SetLogy(); cp->Draw(); cp->cd();
  TGraphErrors *cloneg = ((TGraphErrors *)gr_fix->Clone());
  cloneg->SetTitle(); cloneg->SetMarkerSize(0.8);
  cloneg->SetMaximum(cloneg->GetHistogram()->GetMaximum()*2.00);
  cloneg->Draw("AP");
  c->cd();
  //  c->SaveAs(Form("%s_%s.eps", blabla.Data(), graph_name.Data()));
  f->Close();
}

TH1 *PullHisto(const TList *list, const char *name, Int_t min, Int_t max,
               Double_t &mean, Double_t &bw)
{
  THnSparse *hs = list->FindObject(name);
  if (!hs) return 0;
  TAxis *atmp = hs->GetAxis(1);
  atmp->SetRange(min, max);
  // !!!!!!!!!!!!!!!!!!!!
  hs->GetAxis(2)->SetRangeUser(-0.5, 0.5);
  TH1 *hfin = hs->Projection(0);
  hfin->SetTitle(Form("p_{T} #in (%4.2f, %4.2f) GeV/c",
                      atmp->GetBinLowEdge(min),
                      atmp->GetBinLowEdge(max) + atmp->GetBinWidth(max)));
  mean = atmp->GetBinLowEdge(min) +
    (atmp->GetBinLowEdge(max) + atmp->GetBinWidth(max) -
     atmp->GetBinLowEdge(min))/2.0;
  bw = (atmp->GetBinLowEdge(max) + atmp->GetBinWidth(max) -
        atmp->GetBinLowEdge(min));
  // !!!!!!!!!!!!!!!!!!!!
  Printf("binwidth = %.5f", bw);
  return hfin;//->Rebin();
}

void Norm(const TH1 *h0, TH1 *hn, Double_t min, Double_t max)
{
  hn->Scale(h0->Integral(h0->FindFixBin(min), h0->FindFixBin(max))/
            hn->Integral(hn->FindFixBin(min), hn->FindFixBin(max)));

}

Int_t FindExactRange(const TH1 *h, Double_t step, Int_t *fb, Int_t *lb)
{
  Double_t bin_step =  step/h->GetBinWidth(1);
  Int_t count = 0;
  // !!!!!!!!!!!!!!!!!!!!
  Double_t maxp = 3.4; // maxp = 5.0 => 24 intervarls
  maxp=5.0;
  for (Int_t i = 1; i <= h->GetNbinsX(); i+=bin_step) {
    fb[count] = i;
    lb[count] = i+bin_step-1;
    if (h->GetBinCenter(i) > maxp) {
      lb[count] = h->GetNbinsX()-1;
      break;
    }
    count++;
  }

  return count-1;
}

Int_t FindRange5(Int_t *fb, Int_t *lb)
{
  Int_t count = 0;
  fb[count] = 1;
  lb[count] = 5;
  count++;
  for (Int_t i = 1; i <= 9; i++) {
    fb[count] = fb[count-1]+5;
    lb[count] = lb[count-1]+5;

    Printf("%d,  1  %d, %d", count, fb[count], lb[count]);
    count++;
  }

  fb[count] = 51;
  lb[count] = 60;
  Printf("%d,  m  %d, %d", count, fb[count], lb[count]);
  count++;
  for (Int_t i = 1; i <= 6; i++) {   // MC ma az do i <= 15
    fb[count] = fb[count-1]+10;
    lb[count] = lb[count-1]+10;

    Printf("%d,  2  %d, %d", count, fb[count], lb[count]);
    count++;
  }
  return count;
}
//______________________________________________________________________________
TGraphErrors *GraphRatio(TGraphErrors *g1,TGraphErrors *g2,
                         Bool_t binomial=kFALSE)
{
  Int_t n = g1->GetN();
  if (n != g2->GetN()) {
    Printf("!!!!!!!!!!!!!!!!");
    return 0;
  }

  /////////////////////////
  //  n = 14;
  /////////////////////////
  TGraphErrors *g = new TGraphErrors(n);
  g->SetMarkerStyle(20);
  g->SetMarkerSize(0.75);
  g->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  g->SetTitle(Form("#frac{%s}{%s}", g1->GetTitle(), g2->GetTitle()));
  g->SetMinimum(0.76);
  g->SetMaximum(1.24);
  Double_t x, y1, y2;
  Double_t c0, c1, c12, e0, e1;
  Double_t *ex = g1->GetEX();
  Double_t *ey = g1->GetEY();
  for (Int_t i = 0; i < n; i++) {
    g1->GetPoint(i, x, y1);
    g2->GetPoint(i, x, y2);
    //if (TMath::Abs(y2) < 1.E-12) y2 = 1.0;
    c0 = y1;
    c1 = y2;
    c12 = c1*c1;
    e0  = g1->GetErrorY(i);
    e1  = g2->GetErrorY(i);
    g->SetPoint(i, x, y1/y2);
    if ((g1->GetEY()[i] < 0.00001) || (g2->GetEY()[i] < 0.00001)) {
      g->SetPoint(i, x, 9999);
      Printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    }
    Double_t ttt = (e0*e0*c1*c1 + e1*e1*c0*c0)/(c12*c12);
    if (binomial) {
      Double_t w = y1/y2;
      ttt = TMath::Abs( ( (1.-2.*w)*e0*e0 + w*w*e1*e1 )/(y2*y2) );
    }
    ttt = TMath::Sqrt(ttt); // !!!!!!!!!!!!!!!!!!!!!!!!!!!
    g->SetPointError(i, ex[i], ttt);
  }

  g->GetXaxis()->SetRangeUser(-1, 5);
  return g;
}

Double_t NanCheck(Double_t value, Double_t retvalue = 0.000000001)
{
  if (TMath::IsNaN(value)) return retvalue;
  return value;
}

void G2F(const TGraphErrors *g, const char *name, Bool_t info = kTRUE)
{
  if (info) Printf("Creating file %s", name);
  ofstream myfile;
  myfile.open(name);
  TString str_tmp;
  Double_t x, y, ex, ey;
  for (Int_t i = 0; i < g->GetN(); i++) {
    g->GetPoint(i, x, y);
    ex = g->GetErrorX(i);
    ey = g->GetErrorY(i);
    str_tmp = TString::Format("%g %g %g %g", x, y, NanCheck(ex), NanCheck(ey));
    if (info) Printf("%s", str_tmp.Data());
    myfile << str_tmp.Data() << endl;
  }
  myfile.close();
}

Double_t CalculateFactor(TList *list, Double_t delta_pt)
{
  TH1F *hEventStat = (TH1F *)list->FindObject("hEventStat");
  if (!hEventStat) return 1.0;

  // events All
  Double_t nEvents    = hEventStat->GetBinContent(1);
  // events Selected
  Double_t nEventsSel = hEventStat->GetBinContent(4);

  // deltaY Eta is 1.0 (-0.5, 0.5)
  Double_t delta_y    = 1.0;
  // trigger eff 0.851 +7% - 3.5%
  Double_t effTrigger = 0.851;
  // Phi loss because of VertexCut We have to check
  // (for now 0.01 -> 1% as in 7TeV paper
  Double_t effVert    = 0.99;
  // Branching ratio
  Double_t br = 0.489;

  Double_t fac = effTrigger / (delta_y * delta_pt * effVert * br * nEventsSel);

  Printf("NumEvents    = %.0f", nEvents);
  Printf("NumEventsSel = %.0f", nEventsSel);
  Printf("factor = %.10f", fac);

  return fac;
}
