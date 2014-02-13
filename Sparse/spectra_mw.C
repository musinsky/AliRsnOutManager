// Jan Musinsky, Martin Vala
// 2014-01-31

#ifndef __CINT__
#include <TF1.h>
#include <TFitResult.h>
#include <TPad.h>
#include <TMultiGraph.h>
#include <TBox.h>
#include <TLatex.h>
#endif

void spectra_mw(const char *prefix = "pt/PhiNsigma_KTPCnsig30_STD2010_00_DEFAULT_00")
{
  TGraphErrors *g = ReadMWGraph(Form("%s/mass", prefix), 1);
  g->Draw("ACP");

  TLatex latex;
  latex.SetTextFont(42);
  latex.SetTextAlign(12);
  latex.SetTextSize(0.03);
  latex.SetTextColor(kBlue+1);
  latex.DrawLatex(1.2, 1.01877, "MC generated");
  latex.SetTextColor(kGreen+1);
  latex.DrawLatex(1.2, 1.01877-0.00015, "MC reconstructed");
  latex.SetTextColor(kRed+1);
  latex.DrawLatex(1.2, 1.01877-2*0.000155, "Real data (uncorrected)");
  latex.SetTextColor(kBlack);
  latex.DrawLatex(1.2, 1.01877-3*0.00016, "Real data (corrected)");
  gPad->Print(Form("%s.pdf", g->GetName()));

  gPad->Clear();
  gPad->SetLogy(0);
  TGraphErrors *g = ReadMWGraph(Form("%s/mass", prefix), 3);
  g->Draw("AP");
  latex.SetTextAlign(22);
  latex.SetTextSize(0.04);
  latex.SetTextColor(kBlack);
  latex.DrawLatex(2.0, 1.0207, "pp @ #sqrt{s} = 2.76 TeV");
  latex.DrawLatex(2.0, 1.0205, "TPC 3.0 #sigma");
  latex.DrawLatex(2.0, 1.0202, "#phi #rightarrow K^{ +}  K^{ -}");
  latex.SetTextSize(0.035);
  latex.DrawLatex(1.7, 1.0182, "uncertainties:  stat. (bars) & syst. (boxes)");
  gPad->Print(Form("%s.pdf", g->GetName()));

  gPad->Clear();
  TGraphErrors *g = ReadMWGraph(Form("%s/width", prefix), 2);
  g->Draw("AP");
  TLatex latex;
  latex.SetTextAlign(22);
  latex.SetTextSize(0.04);
  latex.SetTextColor(kBlack);
  latex.DrawLatex(1.7, 0.0090, "pp @ #sqrt{s} = 2.76 TeV");
  latex.DrawLatex(1.7, 0.0085, "TPC 3.0 #sigma");
  latex.DrawLatex(1.7, 0.0077, "#phi #rightarrow K^{ +}  K^{ -}");
  latex.SetTextSize(0.035);
  latex.DrawLatex(1.7, 0.0007, "uncertainties:  stat. (bars) & syst. (boxes)");
  gPad->Print(Form("%s.pdf", g->GetName()));

  //  gPad->Clear();
  //  gPad->SetLogy();
  //  TGraphErrors *g = ReadPtGraphSys("pt/out.txt");
  //  if (!g) return;
  //  g->Draw("AP");
  //  gPad->Print(Form("%s.pdf", g->GetName()));
}


//if (hisfun)  value = CalculateYield(valueError, bcmin, bcmax, hh, ff, r.Get(), pp3);
//else         value = CalculateYield(valueError, 1, 0, hh, ff,(TFitResult*)r.Get(), pp3);







TGraphErrors *ReadMWGraph(const char *name, Int_t flag)
{
  Double_t xreject = 0.49;

  TGraphErrors *g = new TGraphErrors(name);
  if (g->IsZombie()) return 0;
  while (g->GetX()[0] < xreject)
    g->RemovePoint(0);

  TGraphErrors *g2 = new TGraphErrors(name);
  if (g2->IsZombie()) return 0;
  while (g2->GetX()[0] < xreject)
    g2->RemovePoint(0);
  g2->SetMarkerStyle(4);
  g2->SetMarkerSize(1.00);
  g2->SetMarkerColor(kBlack);
  g2->SetLineColor(kBlack);

  TGraphErrors *gsys = new TGraphErrors(name, "%lg %lg %lg %*lg %lg");
  if (gsys->IsZombie()) return 0;
  while (gsys->GetX()[0] < xreject)
    gsys->RemovePoint(0);
  for (Int_t i = 0; i < gsys->GetN(); i++)
    gsys->SetPointError(i, gsys->GetErrorX(i)*0.75, gsys->GetErrorY(i));
  gsys->SetFillColor(kGray+2);
  gsys->SetLineColor(kGray+2);
  gsys->SetFillStyle(3000);

  if (flag == 1 || flag == 3) {
    TGraphErrors *gt = new TGraphErrors(Form("%s_%s", name, "trues"));
    if (gt->IsZombie()) return 0;
    while (gt->GetX()[0] < xreject)
      gt->RemovePoint(0);
    gt->SetMarkerStyle(20);
    gt->SetMarkerSize(0.75);
    gt->SetMarkerColor(kGreen+1);
    gt->SetLineColor(kGreen+1);

    TGraphErrors *gbw = new TGraphErrors(Form("%s_%s", name, "gen"));
    if (gbw->IsZombie()) return 0;
    while (gbw->GetX()[0] < xreject)
      gbw->RemovePoint(0);
    gbw->SetMarkerStyle(20);
    gbw->SetMarkerSize(0.75);
    gbw->SetMarkerColor(kBlue+1);
    gbw->SetLineColor(kBlue+1);

    for (Int_t i = 0; i < g->GetN(); i++) {
      g->SetPointError(i, g->GetEX()[i], 0.);
      gt->SetPointError(i, gt->GetEX()[i], 0.);
      gbw->SetPointError(i, gbw->GetEX()[i], 0.);
    }

    for (Int_t i = 0; i < g2->GetN(); i++) {
      g2->SetPoint(i, g2->GetX()[i], g2->GetY()[i] - gt->GetY()[i] + gbw->GetY()[i]);
      g2->SetPointError(i, g2->GetEX()[i], TMath::Sqrt(g2->GetEY()[i]*g2->GetEY()[i] + gt->GetEY()[i]*gt->GetEY()[i] +
                                                       gbw->GetEY()[i]*gbw->GetEY()[i]));

      //      g2->SetPoint(i, g2->GetX()[i], g2->GetY()[i] - gt->GetY()[i] + 1.01947);
      //      g2->SetPointError(i, g2->GetEX()[i], TMath::Sqrt(g2->GetEY()[i]*g2->GetEY()[i] + gt->GetEY()[i]*gt->GetEY()[i] +
      //                                                       7.78680e-06*7.78680e-06));

      gsys->SetPoint(i, gsys->GetX()[i], g2->GetY()[i]);
    }
  }

  g->SetTitle();
  g->SetName(name);
  g->GetXaxis()->SetTitle("p_{T}, GeV/c");
  g->SetMarkerStyle(20);
  g->SetMarkerSize(0.95);
  g->SetMarkerColor(kRed+1);
  g->SetLineColor(kRed+1);

  const Double_t mass = 1.019455;
  const Double_t mass_delta = 0.000020;
  const Double_t width = 0.00426;
  const Double_t width_delta = 0.00004;
  if (flag == 1) { // mass
    g->GetYaxis()->SetTitleOffset(1.50);
    g->GetYaxis()->SetTitle("mass, GeV/c^{2}");
    g->SetMaximum(mass+0.0015);
    g->SetMinimum(mass-0.0015);
    TBox *box = new TBox(g->GetXaxis()->GetXmin(), mass - mass_delta, g->GetXaxis()->GetXmax(), mass + mass_delta);
    box->SetFillColor(kGray+1);
    box->SetFillStyle(3001);
    g->GetListOfFunctions()->Add(box);
    g->GetListOfFunctions()->Add(g2, "CP");
    g->GetListOfFunctions()->Add(gt, "CP");
    g->GetListOfFunctions()->Add(gbw, "CP");
  }
  else if (flag == 3) { // mass simple
    g2->SetTitle();
    g2->SetName(Form("%s_only", name));
    g2->GetXaxis()->SetTitle("p_{T}, GeV/c");
    g2->SetMarkerStyle(20);
    g2->SetMarkerSize(0.75);
    g2->SetMarkerColor(kBlack);
    g2->SetLineColor(kBlack);
    g2->GetYaxis()->SetTitleOffset(1.50);
    g2->GetYaxis()->SetTitle("mass, GeV/c^{2}");
    g2->SetMaximum(mass+0.0015);
    g2->SetMinimum(mass-0.0015);
    TBox *box = new TBox(g->GetXaxis()->GetXmin(), mass - mass_delta, g2->GetXaxis()->GetXmax(), mass + mass_delta);
    box->SetFillColor(kGray+1);
    box->SetFillStyle(3001);
    g2->GetListOfFunctions()->Add(box);
    g2->GetListOfFunctions()->Add(gsys, "E5");
    return g2;
  }
  else if (flag == 2) { // width
    g->SetTitle();
    g->SetName(name);
    g->GetXaxis()->SetTitle("p_{T}, GeV/c");
    g->SetMarkerStyle(20);
    g->SetMarkerSize(0.75);
    g->SetMarkerColor(kBlack);
    g->SetLineColor(kBlack);
    g->GetYaxis()->SetTitleOffset(1.50);
    g->GetYaxis()->SetTitle("width, GeV/c^{2}");
    g->SetMaximum(0.01);
    g->SetMinimum(0.0);
    TBox *box = new TBox(g->GetXaxis()->GetXmin(), width - width_delta, g->GetXaxis()->GetXmax(), width + width_delta);
    box->SetFillColor(kGray+1);
    box->SetFillStyle(3001);
    g->GetListOfFunctions()->Add(box);
    g->GetListOfFunctions()->Add(gsys, "E5");
  }

  return g;
}
