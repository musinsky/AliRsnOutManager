// Jan Musinsky, Martin Vala
// 2014-01-31

Float_t x, y, dx, dy, dy2;
ofstream outfile;

void toLaTeX(const char *fname = "pt_RFE", Bool_t useSysErr=kFALSE)
{
  TTree *t = new TTree();
  if (useSysErr) t->ReadFile(fname, "x:y:dx:dy:dy2"); // default read as float
  else t->ReadFile(fname, "x:y:dx:dy"); // default read as float

  t->SetBranchAddress("x", &x);
  t->SetBranchAddress("y", &y);
  t->SetBranchAddress("dx", &dx);
  t->SetBranchAddress("dy", &dy);
  if (useSysErr) t->SetBranchAddress("dy2", &dy2);

  outfile.open(Form("%s_TeX.tex", fname));
  PrintHeader(useSysErr);

  for (Int_t i = 0; i < t->GetEntries(); i++) {
    t->GetEntry(i);
    PrintRow(useSysErr);
  }

  PrintFooter();

  outfile.close();
  delete t;
}

void PrintHeader(Bool_t useSysErr)
{
  outfile << "\\documentclass{article}" << endl;
  outfile << "\\usepackage[table]{xcolor}\n" << endl;
  outfile << "\\begin{document}" << endl;
  outfile << "\\definecolor{grayKE}{gray}{0.75}" << endl;
  outfile << "\\rowcolors{2}{grayKE}{white}\n" << endl;
  if (useSysErr)
    outfile << "\\begin{tabular}{| c | c | c | c | c | c |}" << endl;
  else
    outfile << "\\begin{tabular}{| c | c | c | c | c |}" << endl;


  outfile << "  \\hline"<< endl;
  if (useSysErr)
    outfile << "  $p_{T, min}$ & $p_{T, max}$ & value & stat. & sys. \\\\ \\hline" << endl;
  else
    outfile << "  $p_{T, min}$ & $p_{T, max}$ & value & stat. \\\\ \\hline" << endl;
  outfile << "  \\hline" << endl;
}

void PrintFooter()
{
  outfile << "\\end{tabular}" << endl;
  outfile << "\n\\end{document}"<< endl;
}

void PrintRow(Bool_t useSysErr)
{
  TString row;
  if (useSysErr)
    row = Form("  %3.2f & %3.2f & %f & %f & %f \\\\ \\hline",
	       x - dx, x + dx, y, dy, dy2);		
  else
    row = Form("  %3.2f & %3.2f & %f & %f \\\\ \\hline",
	       x - dx, x + dx, y, dy);		

  outfile << row.Data() << endl;
}
