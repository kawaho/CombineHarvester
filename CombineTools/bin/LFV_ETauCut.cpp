#include <string>
#include <map>
#include <set>
#include <iostream>
#include <utility>
#include <vector>
#include <cstdlib>
#include "CombineHarvester/CombineTools/interface/CombineHarvester.h"
#include "CombineHarvester/CombineTools/interface/Observation.h"
#include "CombineHarvester/CombineTools/interface/Process.h"
#include "CombineHarvester/CombineTools/interface/Utilities.h"
#include "CombineHarvester/CombineTools/interface/Systematics.h"
#include "CombineHarvester/CombineTools/interface/BinByBin.h"

using namespace std;

int main(int argc, char* argv[]){

  printf ("Creating LFV MuTau_had datacards \n ");

  string folder="shapes_et_CutBased";
  string lumi="";
  string inputFile="shapesETau";
  string outputFile="lfv_et_13TeV.input";
  string dirInput="";
  bool binbybin=false;
  string name="def";

  if (argc > 1)
    { 
      int count=0;
      for (count = 1; count < argc; count++)
	{
	  if(strcmp(argv[count] ,"--i")==0) inputFile=argv[count+1];
	  if(strcmp(argv[count] ,"--o")==0) outputFile=argv[count+1];
	  if(strcmp(argv[count] ,"--l")==0) lumi=argv[count+1];
	  if(strcmp(argv[count] ,"--d")==0) dirInput=argv[count+1];
	  if(strcmp(argv[count] ,"--f")==0) folder=argv[count+1];
	  if(strcmp(argv[count] ,"--b")==0) binbybin=true; 
	  if(strcmp(argv[count] ,"--name")==0) name=argv[count+1];
	}
    }

  string aux_shapes = string(getenv("CMSSW_BASE")) + "/src/auxiliaries/shapesForLimit";

  ch::CombineHarvester cb;
  // cb.SetVerbosity(3);

  ch::Categories cats = {
    {1, "0Jet"},
    {2, "1Jet"},
    {3, "2Jet"},
    {4, "2JetVBF"}
  };

  vector<string> masses = ch::MassesFromRange("125");

  cb.AddObservations({"*"}, {"lfv"}, {"13TeV"}, {"et"}, cats);

  vector<string> bkg_procs = {"GluGluH", "VBFH", "VH", "Zll", "TT", "ST", "EWKDiboson", "embed", "Fakes", "EWK"}; //"EWK"
  cb.AddProcesses({"*"}, {"lfv"}, {"13TeV"}, {"et"}, bkg_procs, cats, false);

  vector<string> sig_procs = {"GluGlu", "VBF"};
  cb.AddProcesses(masses, {"lfv"}, {"13TeV"}, {"et"}, sig_procs, cats, true);
  
  using ch::syst::SystMap;
  using ch::syst::era;
  using ch::syst::bin_id;
  using ch::syst::process;

  cb.cp().process(ch::JoinStr({sig_procs, {"GluGluH", "VBFH", "VH", "Zll", "TT", "ST", "EWKDiboson"}}))
    .AddSyst(cb, "CMS_lumi_13TeV", "lnN", SystMap<>::init(1.023));
  
  cb.cp().AddSyst(cb, "QCDscale_gg", "lnN", SystMap<process,bin_id>::init
		  ({"GluGlu", "GluGluH"}, {1}, 1.039)
		  ({"GluGlu", "GluGluH"}, {2}, 1.039)
		  ({"GluGlu", "GluGluH"}, {3}, 1.039)
		  ({"GluGlu", "GluGluH"}, {4}, 1.039)
		  );

  cb.cp().AddSyst(cb, "QCDscale_vbf", "lnN", SystMap<process,bin_id>::init
		  ({"VBF", "VBFH"}, {1}, 1.005)
		  ({"VBF", "VBFH"}, {2}, 1.005)
		  ({"VBF", "VBFH"}, {3}, 1.005)
		  ({"VBF", "VBFH"}, {4}, 1.005)
		  );

  cb.cp().AddSyst(cb, "pdf_gg", "lnN", SystMap<process,bin_id>::init
		  ({"GluGlu", "GluGluH"}, {1}, 1.032)
		  ({"GluGlu", "GluGluH"}, {2}, 1.032)
		  ({"GluGlu", "GluGluH"}, {3}, 1.032)
		  ({"GluGlu", "GluGluH"}, {4}, 1.032)
		  );

  cb.cp().AddSyst(cb, "pdf_vbf", "lnN", SystMap<process,bin_id>::init
		  ({"VBF", "VBFH"}, {1}, 1.021)
		  ({"VBF", "VBFH"}, {2}, 1.021)
		  ({"VBF", "VBFH"}, {3}, 1.021)
		  ({"VBF", "VBFH"}, {4}, 1.021)
		  );
  
  cb.cp().process({"VH"}).AddSyst(cb, "QCDscale_vH", "lnN", SystMap<>::init(1.009));
  cb.cp().process({"VH"}).AddSyst(cb, "pdf_vH", "lnN", SystMap<>::init(1.013));

  //cb.cp().process({"ttH"}).AddSyst(cb, "QCDscale_ttH", "lnN", SystMap<>::init(1.008));
  //cb.cp().process({"ttH"}).AddSyst(cb, "pdf_ttH", "lnN", SystMap<>::init(1.019));

  cb.cp().process(ch::JoinStr({sig_procs, {"GluGluH", "VBFH", "VH", "Zll", "TT", "ST", "EWKDiboson", "embed", "EWK"}})) //"EWK"
    .AddSyst(cb, "CMS_eff_e", "lnN", SystMap<>::init(1.02));
  cb.cp().process(ch::JoinStr({sig_procs, {"GluGluH", "VBFH", "VH", "Zll", "TT", "ST", "EWKDiboson", "embed", "EWK"}})) //"EWK"
    .AddSyst(cb, "CMS_Trigger_13TeV", "lnN", SystMap<>::init(1.02));
  cb.cp().process(ch::JoinStr({sig_procs, {"GluGluH", "VBFH", "VH", "Zll", "TT", "ST", "EWKDiboson", "EWK"}})) //"EWK"
    .AddSyst(cb, "CMS_eff_tau", "shape", SystMap<>::init(1.0));

  cb.cp().process({"Fakes"})
    .AddSyst(cb, "norm_fakes_etauh", "lnN", SystMap<>::init(1.30));
  cb.cp().process({"Fakes"})
    .AddSyst(cb, "norm_fakes_etauh_uncor_etauh_$BIN", "lnN", SystMap<>::init(1.10));

  cb.cp().process({"Zll"})
    .AddSyst(cb, "norm_Zothers", "lnN", SystMap<>::init(1.1));
  cb.cp().process({"Zll"})
    .AddSyst(cb, "norm_Zothers_$BIN", "lnN", SystMap<>::init(1.05));

  cb.cp().process({"EWKDiboson"})
    .AddSyst(cb, "norm_Diboson", "lnN", SystMap<>::init(1.05));
  cb.cp().process({"EWKDiboson"})
    .AddSyst(cb, "norm_Diboson_$BIN", "lnN", SystMap<>::init(1.05));

  cb.cp().process({"TT"})
    .AddSyst(cb, "norm_TT ", "lnN", SystMap<>::init(1.06));
  cb.cp().process({"TT"})
    .AddSyst(cb, "norm_TT_$BIN", "lnN", SystMap<>::init(1.05));

  cb.cp().process({"ST"})
    .AddSyst(cb, "norm_T", "lnN", SystMap<>::init(1.05));
  //cb.cp().process({"ST"})
  //  .AddSyst(cb, "norm_T_$BIN", "lnN", SystMap<>::init(1.05));

  cb.cp().process({"EWK"})
    .AddSyst(cb, "norm_EWK", "lnN", SystMap<>::init(1.04));
  //cb.cp().process({"EWK"})
  //  .AddSyst(cb, "norm_EWK_$BIN", "lnN", SystMap<>::init(1.05));
  
  cb.cp().process(ch::JoinStr({sig_procs, {"GluGluH", "VBFH", "VH", "TT", "ST", "EWKDiboson", "EWK"}})) //"Zll"
    .AddSyst(cb, "CMS_Pileup_13TeV", "shape", SystMap<>::init(1.));
  cb.cp().process(ch::JoinStr({{"embed"}}))
    .AddSyst(cb, "CMS_tracking_tau", "shape", SystMap<>::init(1.));
  //cb.cp().process(ch::JoinStr({{"embed"}}))
  //  .AddSyst(cb, "CMS_selection_m", "shape", SystMap<>::init(1.));

  cb.cp().process(ch::JoinStr({sig_procs, {"GluGluH", "VBFH", "VH", "TT", "ST", "EWKDiboson", "embed", "EWK"}})) //"EWK"
    .AddSyst(cb, "CMS_scale_t_1prong_13TeV", "shape", SystMap<>::init(1.));
  cb.cp().process(ch::JoinStr({sig_procs, {"GluGluH", "VBFH", "VH", "TT", "ST", "EWKDiboson", "embed", "EWK"}})) //"EWK"
    .AddSyst(cb, "CMS_scale_t_1prong1pizero_13TeV", "shape", SystMap<>::init(1.));
  cb.cp().process(ch::JoinStr({sig_procs, {"GluGluH", "VBFH", "VH", "TT", "ST", "EWKDiboson", "embed", "EWK"}})) //"EWK"
    .AddSyst(cb, "CMS_scale_t_3prong_13TeV", "shape", SystMap<>::init(1.));

  cb.cp().process(ch::JoinStr({sig_procs, {"Zll", "GluGluH", "VBFH", "EWK"}})) //"EWK"
    .AddSyst(cb, "CMS_RecoilResponse_13TeV", "shape", SystMap<>::init(1.));
  cb.cp().process(ch::JoinStr({sig_procs, {"Zll", "GluGluH", "VBFH", "EWK"}})) //"EWK"
    .AddSyst(cb, "CMS_RecoilResolution_13TeV", "shape", SystMap<>::init(1.));

  cb.cp().process(ch::JoinStr({{"Zll"}}))
    .AddSyst(cb, "CMS_DYpTreweight_13TeV", "lnN", SystMap<>::init(1.1));

  cb.cp().process(ch::JoinStr({{"TT", "ST"}}))
    .AddSyst(cb, "CMS_eff_btag_13TeV", "shape", SystMap<>::init(1.));

  cb.cp().process(ch::JoinStr({{"TT"}}))
    .AddSyst(cb, "CMS_ToppTreweight_13TeV", "shape", SystMap<>::init(1.));

  cb.cp().process(ch::JoinStr({sig_procs, {"GluGluH", "VBFH", "VH", "Zll", "TT", "ST", "EWKDiboson", "EWK"}})) //"EWK"
    .AddSyst(cb, "CMS_scale_mfaketau_13TeV", "shape", SystMap<>::init(1.));
  cb.cp().process(ch::JoinStr({sig_procs, {"GluGluH", "VBFH", "VH", "Zll", "TT", "ST", "EWKDiboson", "EWK"}})) //"EWK"
    .AddSyst(cb, "CMS_scale_efaketau_13TeV", "shape", SystMap<>::init(1.));
  cb.cp().process(ch::JoinStr({sig_procs, {"GluGluH", "VBFH", "VH", "Zll", "TT", "ST", "EWKDiboson", "EWK"}})) //"EWK"
    .AddSyst(cb, "CMS_scale_efaketaues_13TeV", "shape", SystMap<>::init(1.));

  cb.cp().process(ch::JoinStr({sig_procs, {"GluGluH", "VBFH", "VH", "Zll", "TT", "ST", "EWKDiboson", "EWK"}})) //"EWK"
    .AddSyst(cb, "CMS_EEScale_13TeV", "shape", SystMap<>::init(1.));

  cb.cp().process(ch::JoinStr({sig_procs, {"GluGluH", "VBFH", "VH", "Zll", "TT", "ST", "EWKDiboson", "EWK"}})) //"EWK"
    .AddSyst(cb, "CMS_EESigma_13TeV", "shape", SystMap<>::init(1.));

  //cb.cp().process(ch::JoinStr({{"Fakes", "embed", "GluGluH", "VBFH", "VH", "Zll", "TT", "ST", "EWKDiboson", "EWK"}}))
  //  .AddSyst(cb, "CMS_TauFakeRate_13TeV", "shape", SystMap<>::init(1.));

  cb.cp().process(ch::JoinStr({{"Fakes", "embed", "GluGluH", "VBFH", "VH", "Zll", "TT", "ST", "EWKDiboson", "EWK"}})) //"EWK"
    .AddSyst(cb, "CMS_TauFakeRate_EB_DM0_13TeV", "shape", SystMap<>::init(1.));
  cb.cp().process(ch::JoinStr({{"Fakes", "embed", "GluGluH", "VBFH", "VH", "Zll", "TT", "ST", "EWKDiboson", "EWK"}})) //"EWK"
    .AddSyst(cb, "CMS_TauFakeRate_EB_DM1_13TeV", "shape", SystMap<>::init(1.));
  cb.cp().process(ch::JoinStr({{"Fakes", "embed", "GluGluH", "VBFH", "VH", "Zll", "TT", "ST", "EWKDiboson", "EWK"}})) //"EWK"
    .AddSyst(cb, "CMS_TauFakeRate_EB_DM10_13TeV", "shape", SystMap<>::init(1.));
  cb.cp().process(ch::JoinStr({{"Fakes", "embed", "GluGluH", "VBFH", "VH", "Zll", "TT", "ST", "EWKDiboson", "EWK"}})) //"EWK"
    .AddSyst(cb, "CMS_TauFakeRate_EE_DM0_13TeV", "shape", SystMap<>::init(1.));
  cb.cp().process(ch::JoinStr({{"Fakes", "embed", "GluGluH", "VBFH", "VH", "Zll", "TT", "ST", "EWKDiboson", "EWK"}})) //"EWK"
    .AddSyst(cb, "CMS_TauFakeRate_EE_DM1_13TeV", "shape", SystMap<>::init(1.));
  cb.cp().process(ch::JoinStr({{"Fakes", "embed", "GluGluH", "VBFH", "VH", "Zll", "TT", "ST", "EWKDiboson", "EWK"}})) //"EWK"
    .AddSyst(cb, "CMS_TauFakeRate_EE_DM10_13TeV", "shape", SystMap<>::init(1.));

  cb.cp().process(ch::JoinStr({{"Fakes", "embed", "GluGluH", "VBFH", "VH", "Zll", "TT", "ST", "EWKDiboson", "EWK"}})) //"EWK"
    .AddSyst(cb, "CMS_EleFakeRate_13TeV", "shape", SystMap<>::init(1.));

  TString JESNAMES[28] = {"CMS_Jes_JetAbsoluteFlavMap_13TeV", "CMS_Jes_JetAbsoluteMPFBias_13TeV", "CMS_Jes_JetAbsoluteScale_13TeV", "CMS_Jes_JetAbsoluteStat_13TeV", "CMS_Jes_JetFlavorQCD_13TeV", "CMS_Jes_JetFragmentation_13TeV", "CMS_Jes_JetPileUpDataMC_13TeV", "CMS_Jes_JetPileUpPtBB_13TeV", "CMS_Jes_JetPileUpPtEC1_13TeV", "CMS_Jes_JetPileUpPtEC2_13TeV", "CMS_Jes_JetPileUpPtHF_13TeV", "CMS_Jes_JetPileUpPtRef_13TeV", "CMS_Jes_JetRelativeFSR_13TeV", "CMS_Jes_JetRelativeJEREC1_13TeV", "CMS_Jes_JetRelativeJEREC2_13TeV", "CMS_Jes_JetRelativeJERHF_13TeV", "CMS_Jes_JetRelativePtBB_13TeV", "CMS_Jes_JetRelativePtEC1_13TeV", "CMS_Jes_JetRelativePtEC2_13TeV", "CMS_Jes_JetRelativePtHF_13TeV", "CMS_Jes_JetRelativeStatEC_13TeV", "CMS_Jes_JetRelativeStatFSR_13TeV", "CMS_Jes_JetRelativeStatHF_13TeV", "CMS_Jes_JetSinglePionECAL_13TeV", "CMS_Jes_JetSinglePionHCAL_13TeV", "CMS_Jes_JetTimePtEta_13TeV", "CMS_Jes_JetRelativeBal_13TeV", "CMS_Jes_JetRelativeSample_13TeV"};
  for (int i = 0; i < 28; i++){
    cb.cp().process(ch::JoinStr({{"TT", "ST", "EWKDiboson", "VH"}}))
      .AddSyst(cb, JESNAMES[i].Data(), "shape", SystMap<>::init(1.));
  }

  cb.cp().process(ch::JoinStr({{"TT", "ST", "EWKDiboson", "VH"}}))
    .AddSyst(cb, "CMS_MET_Ues_13TeV", "shape", SystMap<>::init(1.));  

  cb.cp().backgrounds().ExtractShapes(aux_shapes + dirInput+"/"+inputFile+".root", "$BIN/$PROCESS", "$BIN/$PROCESS_$SYSTEMATIC");
  cb.cp().signals().ExtractShapes(aux_shapes + dirInput+"/"+inputFile+".root", "$BIN/$PROCESS$MASS", "$BIN/$PROCESS$MASS_$SYSTEMATIC");

  if(binbybin){
    auto bbb = ch::BinByBinFactory()
      .SetAddThreshold(0.5)
      .SetMergeThreshold(0.5)
      .SetFixNorm(false);
    //bbb.MergeBinErrors(cb.cp().backgrounds());
    bbb.MergeBinErrors(cb.cp().process({"Fakes", "embed", "GluGluH", "VBFH", "VH", "Zll", "TT", "ST", "EWKDiboson", "EWK"})); //"EWK"
    bbb.AddBinByBin(cb.cp().backgrounds(), cb);
  }

  // This function modifies every entry to have a standardised bin name of
  // the form: {analysis}_{channel}_{bin_id}_{era}
  // which is commonly used in the htt analyses
  ch::SetStandardBinNames(cb);

  boost::filesystem::create_directories(folder);

  set<string> bins = cb.bin_set();

  string textbinbybin="";
  if(binbybin) textbinbybin="_bbb";

  TFile output((folder + "/"+outputFile+".root").c_str(), "RECREATE");

  for (auto b : bins) {
    for (auto m : masses) {
      cout << ">> Writing datacard for bin: " << b << " and mass: " << m
           << "\n";
      cb.cp().bin({b}).mass({m, "*"}).WriteDatacard(folder + "/" + b + "_" + m + ".txt", output);
    }
  }
}
