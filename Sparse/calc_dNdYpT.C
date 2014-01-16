// Jan Musinsky, Martin Vala
// 2014-01-16

#ifndef __CINT__
#include <TGraphErrors.h>
#include <TF1.h>
#include <TFitResult.h>
#include <TAxis.h>
#include <TPad.h>
#include <TStyle.h>
#include <TMath.h>
#include <TLine.h>
#endif

Double_t LevyTsallis(const Double_t *pt, const Double_t *par);

TF1 *calc_dNdYpT(const char *data_file = "pt/out.txt", Bool_t show = kTRUE)
{
  TGraphErrors *g = new TGraphErrors(data_file);
  if (g->IsZombie()) return 0;
  // reject points < 0.5
  while (g->GetX()[0] < 0.49)
    g->RemovePoint(0);

  TF1 *flt = new TF1("flt", LevyTsallis, 0., 5., 4);
  flt->SetParameters(0.02, 0.3, 7, 1.019445);
  flt->FixParameter(3, 1.019445);        // !!!

  Double_t fitmin = 0.25, fitmax = 5.25; // !!!
  g->Fit(flt, "QS", "", fitmin, fitmax);
  g->Fit(flt, "QS", "", fitmin, fitmax);
  TFitResultPtr res = g->Fit(flt, "S", "", fitmin, fitmax);

  Double_t first, last;
  // !!! bining sensibility !!!
  // first two points (< 0.5) was rejected
  Int_t graph_range[2] = {2-2, 23-2}; // [2-2] = 0.05 - 0.01, [23-2] = 4.70 + 0.01

  // interval 1
  first = 0.0;   // !!!
  last  = g->GetX()[graph_range[0]] - g->GetErrorX(graph_range[0]);
  Double_t fi1  = flt->Integral(first, last);
  Double_t fi1e = flt->IntegralError(first, last, res->GetParams(), res->GetCovarianceMatrix().GetMatrixArray());
  Double_t fpt1 = flt->Mean(first, last, res->GetParams());

  // interval 2
  first = last;
  last  = g->GetX()[graph_range[1]] + g->GetErrorX(graph_range[1]);
  Double_t fi2  = flt->Integral(first, last);
  Double_t fi2e = flt->IntegralError(first, last, res->GetParams(), res->GetCovarianceMatrix().GetMatrixArray());
  Double_t fpt2 = flt->Mean(first, last, res->GetParams());
  Double_t gi2   = 0.0;
  Double_t gi2e  = 0.0;
  Double_t gpt2  = 0.0;
  Double_t gpt2e = 0.0;
  for (Int_t ip = graph_range[0]; ip <= graph_range[1]; ip++) {
    gi2  += g->GetY()[ip]*g->GetErrorX(ip)*2.0;
    gi2e += g->GetEY()[ip]*g->GetErrorX(ip)*2.0 * g->GetEY()[ip]*g->GetErrorX(ip)*2.0;
    gpt2  += g->GetY()[ip]*g->GetX()[ip]*g->GetErrorX(ip)*2.0;
    gpt2e += g->GetEY()[ip]*g->GetX()[ip]*g->GetErrorX(ip)*2.0 * g->GetEY()[ip]*g->GetX()[ip]*g->GetErrorX(ip)*2.0;
  }
  gi2e = TMath::Sqrt(gi2e);
  gpt2e = TMath::Sqrt(gpt2e);

  // interval 3
  first = last;
  last  = 30.0;  // !!!
  Double_t fi3  = flt->Integral(first, last);
  Double_t fi3e = flt->IntegralError(first, last, res->GetParams(), res->GetCovarianceMatrix().GetMatrixArray());
  Double_t fpt3 = flt->Mean(first, last, res->GetParams());


  Printf("------------------------------");
  Printf("dN/dY fun : %f \t %f \t %f \t=> %f", fi1, fi2, fi3, fi1 + fi2 + fi3);
  Printf("dN/dY funE: %f \t %f \t %f \t=> %f", fi1e, fi2e, fi3e, TMath::Sqrt(fi1e*fi1e + fi2e*fi2e + fi3e*fi3e));
  Printf("dN/dY gra : \t \t %f", gi2);
  Printf("dN/dY graE: \t \t %f", gi2e);
  Double_t dNdY  = fi1 + gi2 + fi3;
  Double_t dNdYe = TMath::Sqrt(fi1e*fi1e + gi2e*gi2e + fi3e*fi3e);
  Printf("\ndN/dY       => %f +- %f", dNdY, dNdYe);
  Printf("dN/dY (fit) => %f +- %f", flt->GetParameter(0), flt->GetParError(0));
  Printf("------------------------------");
  Double_t pT  = (fpt1*fi1 + gpt2 + fpt3*fi3)/(fi1 + gi2 +fi3);
  Double_t pTe = (fpt1*fi1e + gpt2e + fpt3*fi3e)/(fi1 + gi2 +fi3);
  Printf("pT        => %f +- %f", pT, pTe);
  Double_t fpT  = (fpt1*fi1 + fpt2*fi2 + fpt3*fi3)/(fi1 + fi2 +fi3);
  Double_t fpTe = (fpt1*fi1e + fpt2*fi2e + fpt3*fi3e)/(fi1 + fi2 +fi3);
  Printf("pT (fun)  => %f +- %f", fpT, fpTe);
  Printf("pT (mean) => %f", flt->Moment(1.0, 0.0, 30.0, res->GetParams())); // !!! interval


  TF1 *fret = new TF1("dummy", "pol9", 0., 0.);
  fret->SetParameters(flt->GetParameters());
  fret->SetParErrors(flt->GetParErrors());
  fret->SetParameter(4, dNdY);
  fret->SetParError(4, dNdYe);
  fret->SetParameter(5, pT);
  fret->SetParError(5, pTe);

  if (!show) {
    delete g;
    delete flt;
    return fret;
  }

  g->SetTitle();
  g->GetXaxis()->SetTitle("p_{T}, GeV/c");
  g->GetYaxis()->SetTitle("d^{ 2}N / dY dp_{T},  (GeV/c)^{ -1}");
  g->GetYaxis()->SetTitleOffset(1.25);
  g->SetMarkerStyle(20);
  g->SetMarkerSize(0.75);
  g->SetMaximum(TMath::MaxElement(g->GetN(), g->GetY())*1.50);
  g->SetMinimum(TMath::MinElement(g->GetN(), g->GetY())*0.50);
  g->Draw("AP");
  flt->SetRange(0., 6.);
  flt->Draw("same");
  TLine l;
  l.SetLineStyle(2);
  l.DrawLine(g->GetX()[graph_range[0]] - g->GetErrorX(graph_range[0]), 0,
             g->GetX()[graph_range[0]] - g->GetErrorX(graph_range[0]), g->GetMaximum());
  l.DrawLine(g->GetX()[graph_range[1]] + g->GetErrorX(graph_range[1]), 0,
             g->GetX()[graph_range[1]] + g->GetErrorX(graph_range[1]), g->GetMaximum()*0.05);
  gPad->SetLogy();
  gPad->SetGridx();
  gStyle->SetGridColor(kGray);

  return fret;
}

Double_t LevyTsallis(const Double_t *pt, const Double_t *par)
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
