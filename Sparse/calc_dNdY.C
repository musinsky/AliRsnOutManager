Double_t calc_dNdY(const char *data_file = "pt_RFE", Bool_t show = kFALSE)
{
  TGraphErrors *g = new TGraphErrors(data_file);
  if (g->IsZombie()) return;

  TF1 *flt = new TF1("flt", LevyTsallis, 0., 5., 4);
  flt->SetParameters(2.93483e-02, 2.80382e-01, 8.10224e+00, 1.01944e+00);
  flt->FixParameter(3, 1.019445);

  Double_t fitmin = 0.25, fitmax = 5.25; // !!!
  g->Fit(flt, "Q", "", fitmin, fitmax);
  g->Fit(flt, "Q", "", fitmin, fitmax);
  g->Fit(flt, "Q", "", fitmin, fitmax);

  Double_t first, last;
  // !!! bining sensibility !!!
  Int_t graph_range[2] = {2, 23}; // [2] = 0.05 - 0.01, [23] = 4.70 + 0.01

  // interval 1
  first = 0.0;   // !!!
  last  = g->GetX()[graph_range[0]] - g->GetErrorX(graph_range[0]);
  Double_t fi1 = flt->Integral(first, last);

  // interval 2
  first = last;
  last  = g->GetX()[graph_range[1]] + g->GetErrorX(graph_range[1]);
  Double_t fi2 = flt->Integral(first, last);
  Double_t gi2 = 0.0;
  for (Int_t ip = graph_range[0]; ip <= graph_range[1]; ip++)
    gi2 += g->GetY()[ip]*g->GetErrorX(ip)*2.0;

  // interval 3
  first = last;
  last  = 30.0;   // !!!
  Double_t fi3 = flt->Integral(first, last);

  Double_t result = fi1 + gi2 + fi3;

  if (!show) {
    delete g;
    delete flt;
    return result;
  }

  Printf("function: %f \t %f \t %f", fi1, fi2, fi3);
  Printf("graph: \t \t \t %f", gi2);
  Printf("result => %f", result);
  g->Draw("AP");
  gPad->SetLogy();
  return result;
}

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
