Double_t sigma = 1.0;
Double_t del_step = 0.25;
Bool_t binAnders = kFALSE;
Bool_t useCF = kTRUE;
Int_t rsn_data   = 20130106; // 20130411
TString mv_colon = "_";      // ":";

const char *suf[12] = {"00_DEFAULT", "CHI2ITS036", "CHI2ITS100", "CHI2TPC04",
                       "CHI2TPC06", "DCAXY035", "DCAXY140", "DCAZ01", "DCAZ20",
                       "NCLSTTPC50", "NCLSTTPC70", "NCLSTTPC80"};
const char *what   = "";
TString fname, grapht;
TString lname,s1name,s1namegen,s1nametrue,s3name_p,s3name_m,smix,smixpp,smixmm,
  graph_name, dir_prefix;
Int_t ilist = 0;

Bool_t noSigma = kFALSE;
Double_t norm[2]  = {1.045, 1.085};

TH1D *hg, *h1, *h3_p, *h3_m, *ht;
TMultiGraph *m_gr = new TMultiGraph();



void SetNameBordel(Int_t fsuf, Int_t qc, Int_t std10or11, Bool_t info=kFALSE,
                   const char *my_fname="AnalysisResults.root")
{
  if (rsn_data == 20130106) {
    fname = Form("root://eos.saske.sk//eos/saske.sk/scratch/ALICE/RSN/RESULTS/Rsn_Phi/pp_2.76/2013-01-06/MC_LHC12i6_146805_ESD/%s/%s", suf[fsuf], my_fname);
    mv_colon = "_";
  }
  else {
    Printf("Wrong input Rsn data !!!");
    return;
  }

  const char *tmp_qc = "";
  if      (qc == 0)  { tmp_qc = "qualityonly"; ilist = 0; sigma = 0.0;
    noSigma = kTRUE; }
  else if (qc == 10) { tmp_qc = "KTPCnsig10";  ilist = 2; sigma = 1.0;
    noSigma = kFALSE; }
  else if (qc == 15) { tmp_qc = "KTPCnsig15";  ilist = 2; sigma = 1.5;
    noSigma = kFALSE; }
  else if (qc == 20) { tmp_qc = "KTPCnsig20";  ilist = 3; sigma = 2.0;
    noSigma = kFALSE; }
  else if (qc == 25) { tmp_qc = "KTPCnsig25";  ilist = 4; sigma = 2.5;
    noSigma = kFALSE; }
  else if (qc == 30) { tmp_qc = "KTPCnsig30";  ilist = 5; sigma = 3.0;
    noSigma = kFALSE; }
  else {
    Printf("Wrong TPC cut !!!");
    return;
  }
  const char *tmp_10or11 = "";
  if      (std10or11 == 2010) tmp_10or11 = "STD2010_PRIMARY";
  else if (std10or11 == 2011) tmp_10or11 = "STD2011_PRIMARY_NCLSTTPC";
  else {
    Printf("Wrong 2010 or 2011 cut !!!");
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

  dir_prefix = "EFFI2";
  if (binAnders) dir_prefix += "_ANDERS";
  dir_prefix += Form("_%s", suf[fsuf]);
  //  graphee_name =
  //    Form("PhiNsigma_qualityonly_%s_%s", tmp_10or11, suf[fsuf]);

  s1name     = Form("RsnMini_phi.%s_Unlike",   what);
  s1namegen  = Form("RsnMini_phi_MCGenphi.%s", what);
  s1nametrue = Form("RsnMini_phi.%s_Trues",    what);
  s3name_p   = Form("RsnMini_phi.%s_LikePP",   what);
  s3name_m   = Form("RsnMini_phi.%s_LikeMM",   what);
  smix       = Form("RsnMini_phi.%s_Mixing",   what);
  smixpp     = Form("RsnMini_phi.%s_MixingPP", what);
  smixmm     = Form("RsnMini_phi.%s_MixingMM", what);

  if (!info) return;
  Printf("file name: %s", fname.Data());
  Printf("list name: %s", lname.Data());
  Printf("\n1 - %s\n2 - %s\n3 - %s\n4 - %s\n5 - %s\n6 - %s\n7 - %s\n8 - %s\n",
         s1name.Data(), s3name_p.Data(), s3name_m.Data(),
         smix.Data(), smixpp.Data(), smixmm.Data(),
         s1nametrue.Data(), s1namegen.Data());
}

Double_t fun_s(double *m, double *par) const
{
  Double_t val = 0.0;
  double x     = m[0];
  val = TMath::BreitWigner(x, par[1], par[2]);
  return par[0]*val ;//+ par[3] + x*par[4] + x*x*par[5] + x*x*x*par[6];
}

TF1 *ff = new TF1("ff", fun_s, 0.9, 1.2, 7);
// TF1 *ff = new TF1("ff", fun_s, 1.02-0.1, 1.02+0.1, 3);
Bool_t mc = kTRUE;

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

void AnalysisSparse(Bool_t save_output = kFALSE)
{
  gStyle->SetGridColor(kGray);
  //  TString tmpstr(fname);
  //  if (tmpstr.Contains("data")) {
  //    Printf("!!! Real Data !!!");
  //    mc = kFALSE;
  //  }
  TString gtitle = Form("Monte Carlo, %s", graph_name.Data());
  grapht = graph_name.Data();
  Double_t grx[999], gry[999], gry2[999], gry3[999], gry4[999],
    gry_eff[999], gry_fix[999], grxE[999];
  Double_t gry22[999], gry22E[999], grx22E[999];
  Double_t gry_true[999], gry_true_eff[999], gry_true_effE[999];
  TH1::AddDirectory(kFALSE);
  TFile::SetCacheFileDir(gSystem->HomeDirectory());
  TFile *f = TFile::Open(fname.Data(), "CACHEREAD");
  if (!f) return;
  TList *l; f->GetObject(lname.Data(), l);
  if (!l) return;
  Int_t bf[999], bl[999];
  Int_t nn = FindExactRange(((THnSparse *)(l->FindObject(s1name.Data())))->
                            Projection(1), del_step, bf, bl);
  //  Int_t nn = FindRange5(bf, bl);
  Bool_t binhaluska = kFALSE;
  if (binAnders) {
    nn = 8;
    bf[0] = 6;bf[1] =  9;bf[2] = 11;bf[3] = 16;bf[4] = 21;bf[5] = 26;
    bl[0] = 8;bl[1] = 10;bl[2] = 15;bl[3] = 20;bl[4] = 25;bl[5] = 30;

    bf[6] = 31;bf[7] = 41;
    bl[6] = 40;bl[7] = 50;
  }
  Printf("number of intervals = %d =>", nn);

  Int_t count = 0;
  Double_t ptmean = 0, value = 0;
  Int_t fitStatus = -1;
  gStyle->SetOptStat(0);
  TCanvas *c = new TCanvas("c", "Signal & Background");
  c->Divide(5, 5); c->Modified(); c->Draw();
  TCanvas *c2 = (TCanvas *)c->DrawClone("c2");
  c2->SetTitle("Phi mesons (raw)"); c2->Modified(); c2->Draw();
  TCanvas *c3, *c4;
  if (mc) {
    c3 = (TCanvas *)c->DrawClone("c3");
    c3->SetTitle("Phi mesons (gen)"); c3->Modified(); c3->Draw();
    c4 = (TCanvas *)c->DrawClone("c4");
    c4->SetTitle("Phi mesons (true)"); c4->Modified(); c4->Draw();
  }

  for (Int_t i = 0; i < nn; i++) {
    c->cd(count + 1)->SetGrid();
    h1 = (TH1D *)PullHisto(l, s1name.Data(), bf[i], bl[i], ptmean);
    h1->SetLineColor(kRed);
    h1->GetXaxis()->SetTitle("inv. mass, GeV/c^2");
    h1->Draw("hist");

    h3_p = (TH1D *)PullHisto(l, s3name_p.Data(), bf[i], bl[i], ptmean);
    h3_m = (TH1D *)PullHisto(l, s3name_m.Data(), bf[i], bl[i], ptmean);
    // !!!!!!!!!!!!!!!!!!!!!!!!
    if (count==0) h3_p = h1;
    // !!!!!!!!!!!!!!!!!!!!!!!!
    else {
      h3_p->Add(h3_m);
      //      h3_p->Add((TH1D *)PullHisto(l, smix.Data(), bf[i], bl[i], ptmean));
      //      h3_p->Add((TH1D *)PullHisto(l, smixpp.Data(), bf[i], bl[i], ptmean));
      //      h3_p->Add((TH1D *)PullHisto(l, smixmm.Data(), bf[i], bl[i], ptmean));
      Norm(h1, h3_p, norm[0], norm[1]);
    }
    h3_p->SetLineColor(kBlue);
    h3_p->Draw("hist, same");

    if (mc) {
      c3->cd(count + 1)->SetGrid();
      Printf("%s", s1namegen.Data());
      hg = (TH1D *)PullHisto(l, s1namegen.Data(), bf[i], bl[i], ptmean);
      hg->SetLineColor(kMagenta);
      hg->GetXaxis()->SetTitle("inv. mass, GeV/c^2");
      hg->Draw("hist");
      c4->cd(count + 1)->SetGrid();
      ht = (TH1D *)PullHisto(l, s1nametrue.Data(), bf[i], bl[i], ptmean);
      ht->SetLineColor(kMagenta-5);
      ht->GetXaxis()->SetTitle("inv. mass, GeV/c^2");
      ht->Draw("hist");
    }
    c2->cd(count + 1)->SetGrid();
    TH1 *hh = (TH1 *)h1->Clone("hh");
    hh->SetLineColor(kRed+1);
    hh->Add(h3_p, -1);
    /// !!!!!!!!!!!!!!!!!!!!!!
    //////////    if ((ilist == 3) && (count < 2)) hh->Reset();
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    hh->Draw("hist");

    // !!!!!!!!!!!!!!!!!!
    ff->SetParameters(0.1, 1.02, 0.004, -25000., 0., 0., 0.);
    ff->SetLineColor(hh->GetLineColor());
    ff->SetLineWidth(1);
    //    ff->SetLineStyle(kDashed);
    // where fit
    Double_t fmin = 1.02-2*0.004;
    Double_t fmax = 1.02+2*0.004;
    //    Double_t fmin = 0.995;
    //    Double_t fmax = 1.185;
    // !!!!!!!!!!!!!!!!!!
    Bool_t hisfun = kFALSE; // kFALSE = integral from function
    Double_t   hisfun_k = 1.0/hh->GetBinWidth(10);
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (binhaluska)
      if (i > 9) hisfun_k = 0.5/hh->GetBinWidth(10);
    Printf("======= %f", hisfun_k);
    // !!!!!!!!!!!!!!!!!!
    // wehere integral (his or fun)
    Double_t fmini = 1.02-2*0.004;
    Double_t fmaxi = 1.02+2*0.004;
    hh->Fit(ff, "Q", "", fmin, fmax);
    hh->Fit(ff, "Q", "", fmin, fmax);
    fitStatus = hh->Fit(ff, "Q", "", fmin, fmax);
    TF1 *pp3 = new TF1("pp3", "[0]+x*[1]+x*x*[2]+x*x*x*[3]", fmin, fmax);
    pp3->SetParameters(ff->GetParameter(3), ff->GetParameter(4),
                       ff->GetParameter(5), ff->GetParameter(6));
    pp3->SetLineWidth(1);
    pp3->SetLineColor(h3_p->GetLineColor());
    pp3->Draw("same");
    // ff->SetRange(fmin, fmax);
    // ff->DrawCopy("same");

    value              = hh->Integral(hh->FindBin(fmini), hh->FindBin(fmaxi));
    if (!hisfun) value = ff->Integral(fmini, fmaxi)*hisfun_k -
                   pp3->Integral(fmini, fmaxi)*hisfun_k;
    if (value < 0) value = 0;

    if ((fitStatus != 0) || (ff->GetParameter(2) > 0.1)) {
      printf(" SKIP Data");
      value = 0;
    }
    grx[count] = ptmean;
    if (binhaluska) {
      if (count < 10) grxE[count] = 0.25; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      else            grxE[count] = 0.50; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
    else
      //      grxE[count] = (1.30-1.10)/2.0; // !!!!!!!!!!!!!!!!!!!!!!!!!!
      grxE[count] = 0.05;
    gry[count] = value;

    Double_t tmp1 = h1->Integral(h1->FindBin(fmini), h1->FindBin(fmaxi));
    Double_t tmp2 = h3_p->Integral(h3_p->FindBin(fmini), h3_p->FindBin(fmaxi));
    Double_t tmp_sg = tmp1 - tmp2;
    Double_t tmp_bg = tmp2;

    // if ((tmp_sg <= -tmp_bg) || (tmp_bg < 33.0)) {
    //   gry3[count] = 0.0;
    //   gry4[count] = 0.0;
    // }
    // else {
    gry3[count] = tmp_sg/tmp_bg;
    gry4[count] = tmp_sg/TMath::Sqrt(tmp_sg + tmp_bg);
    // }

    //    Printf("%4.2f, %10f, %10f, %10f", ptmean, tmp1, tmp2, gry3[count]);


    if (mc) {
      c3->cd(count + 1);
      // !!!!!!!!!!!!!!!!
      ff->SetParameters(1, 1.02, 0.004, 0., 0., 0., 0.);
      hg->Fit(ff, "Q", "", fmin, fmax);
      hg->Fit(ff, "Q", "", fmin, fmax);
      fitStatus = hg->Fit(ff, "Q", "", fmin, fmax);
      /*      TF1 *pp3 = new TF1("pp3", "[0]+x*[1]+x*x*[2]+x*x*x*[3]", fmin, fmax);
              pp3->SetParameters(ff->GetParameter(3), ff->GetParameter(4),
              ff->GetParameter(5), ff->GetParameter(6));
              pp3->SetLineWidth(1);
              pp3->SetLineColor(h3_p->GetLineColor());
              pp3->Draw("same");
      */

      value              = hg->Integral(hg->FindBin(fmini), hg->FindBin(fmaxi));
      if (!hisfun) value = ff->Integral(fmini, fmaxi)*hisfun_k;
      //!!!!!!!!!!!!!!!!!!!pp3->Integral(fmini, fmaxi)*hisfun_k;
      if (value <= 0) value = -1;

      if ((fitStatus != 0) || (ff->GetParameter(2) > 0.1)) {
        printf(" SKIP MC");
        value = -1;
      }
      gry2[count]    = value;
      Double_t superfactor = CalculateFactor(l, 0.1);
      if (useCF) {
        gry22E[i] = TMath::Sqrt(gry2[i])*superfactor;
        //        gry22E[i] = 0.0001;
        gry22[i]  = gry2[i]*superfactor;
        grx22E[i] = 0.05;
      }
      gry_eff[count] = gry[count]/gry2[count];

      c4->cd(count + 1);
      // !!!!!!!!!!!!!!!!
      ff->SetParameters(1, 1.02, 0.004, 0., 0., 0., 0.);
      ht->Fit(ff, "Q", "", fmin, fmax);
      ht->Fit(ff, "Q", "", fmin, fmax);
      fitStatus = ht->Fit(ff, "Q", "", fmin, fmax);
      /*      TF1 *pp3 = new TF1("pp3", "[0]+x*[1]+x*x*[2]+x*x*x*[3]", fmin, fmax);
              pp3->SetParameters(ff->GetParameter(3), ff->GetParameter(4),
              ff->GetParameter(5), ff->GetParameter(6));
              pp3->SetLineWidth(1);
              pp3->SetLineColor(h3_p->GetLineColor());
              pp3->Draw("same");
      */

      value              = ht->Integral(ht->FindBin(fmini), ht->FindBin(fmaxi));
      if (!hisfun) value = ff->Integral(fmini, fmaxi)*hisfun_k;
      //!!!!!!!!!!!!!!!!!!!pp3->Integral(fmini, fmaxi)*hisfun_k;
      if (value <= 0) value = -1;

      if ((fitStatus != 0) || (ff->GetParameter(2) > 0.1)) {
        printf(" SKIP true");
        value = -1;
      }
      gry_true[count]     = value;
      gry_true_eff[count] = gry_true[count]/gry2[count];
      // Propagation of uncertainty (A/B)
      Double_t AAA  = gry_true[count];
      Double_t AAAE = TMath::Sqrt(AAA);
      Double_t BBB  = gry2[count];
      Double_t BBBE = TMath::Sqrt(BBB);
      Double_t EEE  = TMath::Sqrt((AAAE/AAA)*(AAAE/AAA)+(BBBE/BBB)*(BBBE/BBB));
      EEE = EEE*gry_true_eff[count];
      gry_true_effE[count] = EEE;
    }

    Printf("=> %6.4f", ptmean);
    count++;
  }

  new TCanvas();
  TGraph *gr = new TGraph(count, grx, gry);
  gr->SetMarkerStyle(8);
  gr->SetMarkerColor(hh->GetLineColor());
  gr->GetXaxis()->SetTitle("p_{t}, GeV/c");
  gr->SetTitle(Form("raw phi, %s", gtitle.Data()));
  gr->Draw("AP");

  cc3 = new TCanvas();
  TGraph *gr3 = new TGraph(count, grx, gry3);
  gr3->SetMarkerStyle(22);
  gr3->SetMarkerColor(kBlue+1);
  gr3->GetXaxis()->SetTitle("p_{t}, GeV/c");
  gr3->SetTitle(Form("SIG / BKG, %s", gtitle.Data()));
  gr3->SetMinimum(0);
  gr3->Draw("AP");

  cc4 = new TCanvas();
  TGraph *gr4 = new TGraph(count, grx, gry4);
  gr4->SetMarkerStyle(23);
  gr4->SetMarkerColor(kBlue-1);
  gr4->GetXaxis()->SetTitle("p_{t}, GeV/c");
  gr4->SetTitle(Form("Significance, %s", gtitle.Data()));
  gr4->SetMinimum(0);
  gr4->Draw("AP");

  ccc = new TCanvas("ccc","ccc",0,0,900,300);
  ccc->Divide(2, 1, 0.001, 0.001);
  ccc->cd(1); gr3->Draw("AP");
  ccc->cd(2); gr4->Draw("AP");

  TString blabla = "mc";
  if (!mc) blabla = "data";
  // gr3->SaveAs(Form("SB_%s_%s.C", blabla.Data(), grapht.Data()));
  // gr4->SaveAs(Form("Sig_%s_%s.C", blabla.Data(), grapht.Data()));
  // ccc->SaveAs(Form("%s_%s_2.eps", blabla.Data(), grapht.Data()));
  // c->SaveAs(Form("%s_%s_0.eps", blabla.Data(), grapht.Data()));
  // c2->SaveAs(Form("%s_%s_1.eps", blabla.Data(), grapht.Data()));

  //  cc3->SaveAs(Form("%s_%s_2.eps", blabla.Data(), grapht.Data()));
  //  gr3->SaveAs(Form("sig_bck_%s_%s.C", blabla.Data(), grapht.Data()));

  if (mc) {
    new TCanvas();
    TGraph *gr2 = new TGraph(count, grx, gry2);
    gr2->SetMarkerStyle(8);
    gr2->SetMarkerColor(hg->GetLineColor());
    gr2->GetXaxis()->SetTitle("p_{t}, GeV/c");
    gr2->SetTitle(Form("gen phi, %s", gtitle.Data()));
    gr2->Draw("AP");

    new TCanvas();
    TGraphErrors *gr22 = new TGraphErrors(count, grx, gry22, grx22E, gry22E);
    gr22->SetMarkerStyle(8);
    gr22->SetMarkerColor(kCyan);
    gr22->GetXaxis()->SetTitle("p_{t}, GeV/c");
    gr22->SetTitle(Form("gen phi, %s", gtitle.Data()));
    gr22->Draw("AP");


    c = new TCanvas();
    c->SetGrid();
    TGraph *gr_e = new TGraph(count, grx, gry_eff);
    gr_e->SetMarkerStyle(22);
    gr_e->SetMarkerColor(kBlack);
    gr_e->GetXaxis()->SetTitle("p_{t}, GeV/c");
    gr_e->SetTitle(Form("efficiency (raw), %s", grapht.Data()));
    gr_e->Draw("AP");
    Printf("Save as '\033[1meffi_raw_%s\033[0m' file", grapht.Data());
    for (Int_t i = 0; i < gr_e->GetN(); i++)
      Printf("%f %f", gr_e->GetX()[i], gr_e->GetY()[i]);

    cvb = new TCanvas();
    cvb->cd();
    TGraph *gr_true = new TGraph(count, grx, gry_true);
    gr_true->SetMarkerStyle(8);
    gr_true->SetMarkerColor(ht->GetLineColor());
    gr_true->GetXaxis()->SetTitle("p_{t}, GeV/c");
    gr_true->SetTitle(Form("true phi, %s", gtitle.Data()));
    gr_true->Draw("AP");
    c = new TCanvas();
    c->cd();
    c->SetGrid();
    TGraphErrors *gr_true_eff = new TGraphErrors(count, grx, gry_true_eff,
                                                 grxE, gry_true_effE);
    gr_true_eff->SetMarkerStyle(20);
    //    gr_true_eff->SetMarkerSize(0.75);
    gr_true_eff->SetMarkerColor(kBlack);
    gr_true_eff->GetXaxis()->SetTitle("p_{t}, GeV/c");
    gr_true_eff->SetTitle(Form("efficiency (true), %s", grapht.Data()));
    gr_true_eff->Draw("AEP");
    m_gr->Add(gr_true_eff);
    Printf("Save as '\033[1meffi_true_%s\033[0m' file", grapht.Data());
    TString tout;
    Double_t oux, ouy, ouxe, ouye;
    for (Int_t i = 0; i < gr_true_eff->GetN(); i++) {
      oux = gr_true_eff->GetX()[i];
      ouy = gr_true_eff->GetY()[i];
      ouy = MinusCheck(ouy);
      ouxe = gr_true_eff->GetErrorX(i);
      ouye = gr_true_eff->GetErrorY(i);
      ouye = NanCheck(ouye);
      Printf("%f %f %f %f", gr_true_eff->GetX()[i], gr_true_eff->GetY()[i],
             gr_true_eff->GetErrorX(i), gr_true_eff->GetErrorY(i));
      if (!save_output) continue;
      gSystem->mkdir(dir_prefix.Data());
      tout = Form("%f %f %f %f", oux, ouy, ouxe, ouye);
      if (i == 0)
        tout = Form("Printf(\"%s\"); > %s/effi_%s", tout.Data(),
                    dir_prefix.Data(), grapht.Data());
      else
        tout = Form("Printf(\"%s\"); >> %s/effi_%s", tout.Data(),
                    dir_prefix.Data(), grapht.Data());
      //      Printf(":::::: %s", tout.Data());
      gROOT->ProcessLine(tout.Data());
    }
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
    gr_true_eff->SetTitle(Form("efficiency, %s", grapht.Data()));
    gr_true_eff->SetMarkerColor(kGreen+1);
    gr_true_eff->Draw("AP");
    gr_e->SetMarkerColor(kRed+1);
    gr_e->Draw("P");
    //    c->SaveAs(Form("%s_%s.eps", blabla.Data(), grapht.Data()));
    return;
  }

  //  TGraph *geff = new TGraph(Form("effi_raw_%s", grapht.Data()));
  //  TGraph *geff = new TGraph(Form("effi_true_%s", grapht.Data()));
  //  TGraph *geff = new TGraph("effi_true_Phi2010_qualityonly");
  TGraph *geff = new TGraph("effi_true_PhiNsigma_qualityonly");
  if (geff->IsZombie()) return;
  geff->SetMarkerStyle(22);
  geff->SetMarkerColor(kBlack);
  geff->GetXaxis()->SetTitle("p_{t}, GeV/c");
  geff->SetTitle(Form("efficiency, %s", grapht.Data()));
  c = new TCanvas();
  c->SetGrid();
  geff->Draw("AP");
  Double_t tpcsigma = 9999.9;
  if (ilist == 1) tpcsigma = 1.0;
  if (ilist == 2) tpcsigma = 1.5;
  if (ilist == 3) tpcsigma = 2.0;
  if (ilist == 4) tpcsigma = 2.5;
  if (ilist == 5) tpcsigma = 3.0;
  Double_t sss = TMath::Erf(tpcsigma/TMath::Sqrt(2.0));
  if (noSigma) sss = 1.0;
  Printf("sigma = %10f", sss);

  // for (Int_t i = 0; i < count; i++)
  //   geff->GetY()[i] = (sss*sss)/(geff->GetY()[i]);
  // geff->SetMaximum(1.0);
  // geff->Draw("AP");

  for (Int_t i = 0; i < count; i++) {
    Double_t deno = geff->Eval(grx[i])*sss*sss;
    if (deno < 0.00001) deno = 1;
    gry_fix[i] = gry[i]/deno;
  }
  new TCanvas;
  TGraph *gr_fix = new TGraph(count, grx, gry_fix);
  gr_fix->SetMarkerStyle(21);
  gr_fix->SetMarkerColor(hh->GetLineColor());
  gr_fix->GetXaxis()->SetTitle("p_{t}, GeV/c");
  gr_fix->SetTitle(Form("corrected phi * #sigma^{2}, %s", gtitle.Data()));
  if (noSigma)
    gr_fix->SetTitle(Form("corrected phi (no #sigma), %s", gtitle.Data()));
  gr_fix->Draw("AP");

  //---------------------
  c = new TCanvas("cfinald", "data_correct", 1200, 450);
  c->Divide(2, 1, 0.001, 0.001); c->Modified(); c->Draw();
  c->cd(1);
  gr->SetMinimum(0);
  gr->SetMarkerColor(kBlack);
  gr->Draw("AP");
  c->cd(2);
  gr_fix->SetMinimum(0);
  gr_fix->SetMarkerColor(kGreen+3);
  gr_fix->Draw("AP");
  TString bla9 = Form("qualityonly_PID2_%s", grapht.Data());
  if (noSigma) bla9 = Form("%s_noSig.C", bla9.Data());
  else         bla9 = Form("%s.C", bla9.Data());
  //  gr_fix->SaveAs(bla9.Data());
  //  TPad *cp = new TPad("cpf", "", 0.45,0.45,0.99,0.92);
  TPad *cp = new TPad("cpf", "", 0.60,0.55,0.99,0.93);
  cp->SetLogy(); cp->Draw(); cp->cd();
  TGraph *cloneg = ((TGraph *)gr_fix->Clone());
  cloneg->SetTitle(); cloneg->SetMarkerSize(0.8);
  cloneg->Draw("AP");
  //  c->SaveAs(Form("%s_%s.eps", blabla.Data(), grapht.Data()));
  f->Close();
}

Double_t NanCheck(Double_t value, Double_t retvalue = 0.0)
{
  if (TMath::IsNaN(value)) return retvalue;
  return value;
}
Double_t MinusCheck(Double_t value, Double_t retvalue = 0.0)
{
  if (value < 0.000001) return retvalue;
  return value;
}

TH1 *PullHisto(const TList *list, const char *name, Int_t min, Int_t max,
               Double_t &mean)
{
  THnSparse *hs = list->FindObject(name);
  if (!hs) return 0;
  TAxis *atmp = hs->GetAxis(1);
  atmp->SetRange(min, max);
  // !!!!!!!!!!!!!!!!!!!!
  hs->GetAxis(2)->SetRangeUser(-0.5, 0.5);
  TH1 *hfin = hs->Projection(0);
  hfin->SetTitle(Form("p_{t} #in (%4.2f, %4.2f) GeV/c",
                      atmp->GetBinLowEdge(min),
                      atmp->GetBinLowEdge(max) + atmp->GetBinWidth(max)));
  mean = atmp->GetBinLowEdge(min) +
    (atmp->GetBinLowEdge(max) + atmp->GetBinWidth(max) -
     atmp->GetBinLowEdge(min))/2.0;
  // !!!!!!!!!!!!!!!!!!!!
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
  Double_t maxp = 3.4;
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
  for (Int_t i = 1; i <= 15; i++) {
    fb[count] = fb[count-1]+10;
    lb[count] = lb[count-1]+10;

    Printf("%d,  2  %d, %d", count, fb[count], lb[count]);
    count++;
  }
  return count;
}

//______________________________________________________________________________
TGraphErrors *GraphRatio(TGraphErrors *g1,TGraphErrors *g2)
{
  Int_t n = g1->GetN();
  if (n != g2->GetN()) {
    Printf("!!!!!!!!!!!!!!!!");
    return 0;
  }

  TGraphErrors *g = new TGraphErrors(n);
  g->SetMarkerStyle(20);
  g->GetXaxis()->SetTitle("p_{t}, GeV/c");
  g->SetTitle(Form("#frac{%s}{%s}", g1->GetTitle(), g2->GetTitle()));
  Double_t x, y1, y2;
  Double_t c0, c1, c12, e0, e1;
  Double_t *ex = g1->GetEX();
  Double_t *ey = g1->GetEY();
  for (Int_t i = 0; i < n; i++) {
    g1->GetPoint(i, x, y1);
    g2->GetPoint(i, x, y2);
    if (TMath::Abs(y2) < 1.E-12) y2 = 1.0;
    c0 = y1;
    c1 = y2;
    c12 = c1*c1;
    e0  = g1->GetErrorY(i);
    e1  = g2->GetErrorY(i);
    g->SetPoint(i, x, y1/y2);
    g->SetPointError(i, ex[i], (e0*e0*c1*c1 + e1*e1*c0*c0)/(c12*c12));
  }

  return g;
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

  effTrigger = 1.0;

  Double_t fac = effTrigger / (delta_y * delta_pt * effVert * br * nEventsSel);

  Printf("NumEvents    = %.0f", nEvents);
  Printf("NumEventsSel = %.0f", nEventsSel);
  Printf("factor = %.10f", fac);

  return fac;
}
