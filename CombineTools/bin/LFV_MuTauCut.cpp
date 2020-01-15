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

  string folder="shapes_mt_CutBased";
  string lumi="";
  string inputFile="shapesMT";
  string outputFile="HMuTau_mutauhad_2017_m125.input";
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
    {1, "0jet"},
    {2, "1jet"},
    {3, "2jet_gg"},
    {4, "2jet_vbf"}
  };

  vector<string> masses = ch::MassesFromRange("125");

  cb.AddObservations({"*"}, {"HMuTau"}, {"2017"}, {"mutauhad"}, cats);

  vector<string> bkg_procs = {"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "vH_htt", "Zothers", "TT", "T", "EWK", "Diboson", "Fakes", "ZTauTau"};
  cb.AddProcesses({"*"}, {"HMuTau"}, {"2017"}, {"mutauhad"}, bkg_procs, cats, false);

  vector<string> sig_procs = {"LFVGG", "LFVVBF"};
  cb.AddProcesses(masses, {"HMuTau"}, {"2017"}, {"mutauhad"}, sig_procs, cats, true);

  using ch::syst::SystMap;
  using ch::syst::era;
  using ch::syst::bin_id;
  using ch::syst::process;

  // QCD Scale and PDF uncertainty
  cb.cp().AddSyst(cb, "QCDscale_ggH", "lnN", SystMap<process,bin_id>::init
		  ({"LFVGG", "ggH_htt", "ggH_hww"}, {1}, 1.039)
		  ({"LFVGG", "ggH_htt", "ggH_hww"}, {2}, 1.039)
		  ({"LFVGG", "ggH_htt", "ggH_hww"}, {3}, 1.039)
		  ({"LFVGG", "ggH_htt", "ggH_hww"}, {4}, 1.039)
		  );

  cb.cp().AddSyst(cb, "QCDscale_qqH", "lnN", SystMap<process,bin_id>::init
		  ({"LFVVBF", "qqH_htt", "qqH_hww"}, {1}, 1.005)
		  ({"LFVVBF", "qqH_htt", "qqH_hww"}, {2}, 1.005)
		  ({"LFVVBF", "qqH_htt", "qqH_hww"}, {3}, 1.005)
		  ({"LFVVBF", "qqH_htt", "qqH_hww"}, {4}, 1.005)
		  );

  cb.cp().AddSyst(cb, "pdf_Higgs_gg", "lnN", SystMap<process,bin_id>::init
		  ({"LFVGG", "ggH_htt", "ggH_hww"}, {1}, 1.032)
		  ({"LFVGG", "ggH_htt", "ggH_hww"}, {2}, 1.032)
		  ({"LFVGG", "ggH_htt", "ggH_hww"}, {3}, 1.032)
		  ({"LFVGG", "ggH_htt", "ggH_hww"}, {4}, 1.032)
		  );

  cb.cp().AddSyst(cb, "pdf_Higgs_qq", "lnN", SystMap<process,bin_id>::init
		  ({"LFVVBF", "qqH_htt", "qqH_hww"}, {1}, 1.021)
		  ({"LFVVBF", "qqH_htt", "qqH_hww"}, {2}, 1.021)
		  ({"LFVVBF", "qqH_htt", "qqH_hww"}, {3}, 1.021)
		  ({"LFVVBF", "qqH_htt", "qqH_hww"}, {4}, 1.021)
		  );

  cb.cp().process({"vH_htt"}).AddSyst(cb, "QCDscale_vH", "lnN", SystMap<>::init(1.012));//(ZH,WH)->(0.9%,0.8%)
  cb.cp().process({"vH_htt"}).AddSyst(cb, "pdf_vH", "lnN", SystMap<>::init(1.023));//(ZH,WH)->(1.3%,1.9%)

  // Acceptance
  cb.cp().AddSyst(cb,"acceptance_pdf_gg", "lnN", SystMap<process,bin_id>::init
		  ({"LFVGG", "ggH_htt"}, {1}, 1.005)
		  ({"LFVGG", "ggH_htt"}, {2}, 1.005)
		  ({"LFVGG", "ggH_htt"}, {3}, 0.995)
		  ({"LFVGG", "ggH_htt"}, {4}, 0.995)
		  ({"ggH_hww"}, {1}, 1.005)
		  ({"ggH_hww"}, {2}, 0.998)
		  ({"ggH_hww"}, {3}, 0.985)
		  ({"ggH_hww"}, {4}, 0.985)
		  );

  cb.cp().AddSyst(cb,"acceptance_pdf_vbf", "lnN", SystMap<process,bin_id>::init
		  ({"LFVVBF", "qqH_htt"}, {1}, 1.005)
		  ({"LFVVBF", "qqH_htt"}, {2}, 0.99)
		  ({"LFVVBF", "qqH_htt"}, {3}, 0.985)
		  ({"LFVVBF", "qqH_htt"}, {4}, 0.985)
		  ({"qqH_hww"}, {1}, 1.01)
		  ({"qqH_hww"}, {2}, 1.005)
		  ({"qqH_hww"}, {3}, 0.998)
		  ({"qqH_hww"}, {4}, 0.998)
		  );

  cb.cp().AddSyst(cb,"acceptance_scale_gg", "lnN", SystMap<process,bin_id>::init
		  ({"LFVGG", "ggH_htt"}, {1}, 1.02)
		  ({"LFVGG", "ggH_htt"}, {2}, 0.996)
		  ({"LFVGG", "ggH_htt"}, {3}, 0.97)
		  ({"LFVGG", "ggH_htt"}, {4}, 0.97)
		  ({"ggH_hww"}, {1}, 1.01)
		  ({"ggH_hww"}, {2}, 0.996)
		  ({"ggH_hww"}, {3}, 0.97)
		  ({"ggH_hww"}, {4}, 0.97)
		  );

  cb.cp().AddSyst(cb,"acceptance_scale_vbf", "lnN", SystMap<process,bin_id>::init
		  ({"LFVVBF", "qqH_htt"}, {1}, 1.01)
		  ({"LFVVBF", "qqH_htt"}, {2}, 1.006)
		  ({"LFVVBF", "qqH_htt"}, {3}, 0.999)
		  ({"LFVVBF", "qqH_htt"}, {4}, 0.999)
		  ({"qqH_hww"}, {1}, 1.01)
		  ({"qqH_hww"}, {2}, 1.003)
		  ({"qqH_hww"}, {3}, 0.997)
		  ({"qqH_hww"}, {4}, 0.997)
		  );

  // Uncertainty on BR for HTT @ 125 GeV
  cb.cp().process({"ggH_htt", "qqH_htt"}).AddSyst(cb, "BR_htt_THU", "lnN", SystMap<>::init(1.017));
  cb.cp().process({"ggH_htt", "qqH_htt"}).AddSyst(cb, "BR_htt_PU_mq", "lnN", SystMap<>::init(1.0099));
  cb.cp().process({"ggH_htt", "qqH_htt"}).AddSyst(cb, "BR_htt_PU_alphas", "lnN", SystMap<>::init(1.0062));

  // Uncertainty on BR of HWW @ 125 GeV
  cb.cp().process({"ggH_hww", "qqH_hww"}).AddSyst(cb, "BR_hww_THU", "lnN", SystMap<>::init(1.0099));
  cb.cp().process({"ggH_hww", "qqH_hww"}).AddSyst(cb, "BR_hww_PU_mq", "lnN", SystMap<>::init(1.0099));
  cb.cp().process({"ggH_hww", "qqH_hww"}).AddSyst(cb, "BR_hww_PU_alphas", "lnN", SystMap<>::init(1.0066));

  // Normalization
  cb.cp().process({"Zothers"})
    .AddSyst(cb, "norm_Zothers", "lnN", SystMap<>::init(1.04));
  cb.cp().process({"Zothers"})
    .AddSyst(cb, "norm_Zothers_mutauh_$BIN", "lnN", SystMap<>::init(1.04));

  cb.cp().process({"Fakes"})
    .AddSyst(cb, "norm_fakes_mutauh", "lnN", SystMap<>::init(1.30));
  cb.cp().process({"Fakes"})
    .AddSyst(cb, "norm_fakes_mutauh_uncor_mutauh_$BIN", "lnN", SystMap<>::init(1.10));

  cb.cp().process({"Diboson"})
    .AddSyst(cb, "norm_Diboson", "lnN", SystMap<>::init(1.05));
  cb.cp().process({"Diboson"})
    .AddSyst(cb, "norm_Diboson_mutauh_$BIN", "lnN", SystMap<>::init(1.05));

  cb.cp().process({"TT"})
    .AddSyst(cb, "norm_TT ", "lnN", SystMap<>::init(1.06));
  cb.cp().process({"TT"})
    .AddSyst(cb, "norm_TT_mutauh_$BIN", "lnN", SystMap<>::init(1.05));

  cb.cp().process({"T"})
    .AddSyst(cb, "norm_T", "lnN", SystMap<>::init(1.05));
  cb.cp().process({"T"})
    .AddSyst(cb, "norm_T_mutauh_$BIN", "lnN", SystMap<>::init(1.05));

  cb.cp().process({"EWK"})
    .AddSyst(cb, "norm_EWK", "lnN", SystMap<>::init(1.04));
  cb.cp().process({"EWK"})
    .AddSyst(cb, "norm_EWK_mutauh_$BIN", "lnN", SystMap<>::init(1.04));

  // Luminosity, Pileup, Prefiring
  cb.cp().process(ch::JoinStr({sig_procs, {"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "vH_htt", "Zothers", "TT", "T", "EWK", "Diboson"}}))
    .AddSyst(cb, "CMS_lumi_13TeV", "lnN", SystMap<>::init(1.023));
  cb.cp().process(ch::JoinStr({sig_procs, {"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "vH_htt", "Zothers", "TT", "T", "EWK", "Diboson"}}))
    .AddSyst(cb, "CMS_Pileup_13TeV", "shape", SystMap<>::init(1.0));
  cb.cp().process(ch::JoinStr({sig_procs, {"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "vH_htt", "Zothers", "TT", "T", "EWK", "Diboson"}}))
    .AddSyst(cb, "CMS_Prefiring_13TeV", "shape", SystMap<>::init(1.0));

  // Trigger and lepton efficiency
  cb.cp().process(ch::JoinStr({sig_procs, {"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "vH_htt", "Zothers", "TT", "T", "EWK", "Diboson"}}))
    .AddSyst(cb, "CMS_Trigger_13TeV", "lnN", SystMap<>::init(1.02));
  cb.cp().process(ch::JoinStr({{"ZTauTau"}}))
    .AddSyst(cb, "CMS_Trigger_emb_13TeV", "lnN", SystMap<>::init(1.02));
  cb.cp().process(ch::JoinStr({{"ZTauTau"}}))
    .AddSyst(cb, "CMS_selection_muon_emb_13TeV", "lnN", SystMap<>::init(1.04));

  cb.cp().process(ch::JoinStr({sig_procs, {"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "vH_htt", "Zothers", "TT", "T", "EWK", "Diboson"}}))
    .AddSyst(cb, "CMS_eff_m", "lnN", SystMap<>::init(1.02));
  cb.cp().process({{"ZTauTau"}})
    .AddSyst(cb, "CMS_eff_m", "lnN", SystMap<>::init(1.01));
  cb.cp().process({{"ZTauTau"}})
    .AddSyst(cb, "CMS_eff_emb_m", "lnN", SystMap<>::init(1.01732));

  cb.cp().process(ch::JoinStr({sig_procs, {"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "vH_htt", "Zothers", "TT", "T", "EWK", "Diboson"}}))
    .AddSyst(cb, "CMS_eff_tau", "lnN", SystMap<>::init(1.03));
  cb.cp().process({{"ZTauTau"}})
    .AddSyst(cb, "CMS_eff_tau", "lnN", SystMap<>::init(1.015));
  cb.cp().process({{"ZTauTau"}})
    .AddSyst(cb, "CMS_eff_emb_tau", "lnN", SystMap<>::init(1.02598));

  cb.cp().process(ch::JoinStr({{"ZTauTau"}}))
    .AddSyst(cb, "CMS_tracking_tau", "shape", SystMap<>::init(1.0));

  // Recoil Uncertainty
  cb.cp().process(ch::JoinStr({sig_procs, {"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "Zothers", "EWK"}}))
    .AddSyst(cb, "CMS_RecoilResponse_13TeV", "shape", SystMap<>::init(1.0));
  cb.cp().process(ch::JoinStr({sig_procs, {"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "Zothers", "EWK"}}))
    .AddSyst(cb, "CMS_RecoilResolution_13TeV", "shape", SystMap<>::init(1.0));

  // Reweighting and b-tagging uncertainty
  cb.cp().process(ch::JoinStr({{"Zothers"}}))
    .AddSyst(cb, "CMS_DYpTreweight_13TeV", "lnN", SystMap<>::init(1.1));
  cb.cp().process(ch::JoinStr({{"TT"}}))
    .AddSyst(cb, "CMS_TTpTreweight_13TeV", "shape", SystMap<>::init(1.0));
  cb.cp().process(ch::JoinStr({{"TT", "T"}}))//.bin_id({2})
    .AddSyst(cb, "CMS_eff_btag_13TeV", "shape", SystMap<>::init(1.0));

  // Leptons faking tau
  cb.cp().process(ch::JoinStr({sig_procs, {"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "vH_htt", "Zothers", "TT", "T", "EWK", "Diboson"}}))
    .AddSyst(cb, "CMS_scale_mfaketau_13TeV", "shape", SystMap<>::init(1.0));
  cb.cp().process(ch::JoinStr({sig_procs, {"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "vH_htt", "Zothers", "TT", "T", "EWK", "Diboson"}}))
    .AddSyst(cb, "CMS_scale_efaketau_13TeV", "shape", SystMap<>::init(1.0));
  cb.cp().process(ch::JoinStr({sig_procs, {"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "vH_htt", "Zothers", "TT", "T", "EWK", "Diboson"}}))
    .AddSyst(cb, "CMS_scale_efaketaues_13TeV", "shape", SystMap<>::init(1.0));

  // Fake background Uncertainty
  cb.cp().process(ch::JoinStr({{"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "vH_htt", "Zothers", "TT", "T", "EWK", "Diboson", "Fakes", "ZTauTau"}}))
    .AddSyst(cb, "CMS_TauFakeRate_p0_dm0_B_13TeV", "shape", SystMap<>::init(1.0));
  cb.cp().process(ch::JoinStr({{"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "vH_htt", "Zothers", "TT", "T", "EWK", "Diboson", "Fakes", "ZTauTau"}}))
    .AddSyst(cb, "CMS_TauFakeRate_p0_dm1_B_13TeV", "shape", SystMap<>::init(1.0));
  cb.cp().process(ch::JoinStr({{"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "vH_htt", "Zothers", "TT", "T", "EWK", "Diboson", "Fakes", "ZTauTau"}}))
    .AddSyst(cb, "CMS_TauFakeRate_p0_dm10_B_13TeV", "shape", SystMap<>::init(1.0));
  cb.cp().process(ch::JoinStr({{"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "vH_htt", "Zothers", "TT", "T", "EWK", "Diboson", "Fakes", "ZTauTau"}}))
    .AddSyst(cb, "CMS_TauFakeRate_p0_dm0_E_13TeV", "shape", SystMap<>::init(1.0));
  cb.cp().process(ch::JoinStr({{"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "vH_htt", "Zothers", "TT", "T", "EWK", "Diboson", "Fakes", "ZTauTau"}}))
    .AddSyst(cb, "CMS_TauFakeRate_p0_dm1_E_13TeV", "shape", SystMap<>::init(1.0));
  cb.cp().process(ch::JoinStr({{"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "vH_htt", "Zothers", "TT", "T", "EWK", "Diboson", "Fakes", "ZTauTau"}}))
    .AddSyst(cb, "CMS_TauFakeRate_p0_dm10_E_13TeV", "shape", SystMap<>::init(1.0));

  cb.cp().process(ch::JoinStr({{"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "vH_htt", "Zothers", "TT", "T", "EWK", "Diboson", "Fakes", "ZTauTau"}}))
    .AddSyst(cb, "CMS_TauFakeRate_p1_dm0_B_13TeV", "shape", SystMap<>::init(1.0));
  cb.cp().process(ch::JoinStr({{"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "vH_htt", "Zothers", "TT", "T", "EWK", "Diboson", "Fakes", "ZTauTau"}}))
    .AddSyst(cb, "CMS_TauFakeRate_p1_dm1_B_13TeV", "shape", SystMap<>::init(1.0));
  cb.cp().process(ch::JoinStr({{"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "vH_htt", "Zothers", "TT", "T", "EWK", "Diboson", "Fakes", "ZTauTau"}}))
    .AddSyst(cb, "CMS_TauFakeRate_p1_dm10_B_13TeV", "shape", SystMap<>::init(1.0));
  cb.cp().process(ch::JoinStr({{"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "vH_htt", "Zothers", "TT", "T", "EWK", "Diboson", "Fakes", "ZTauTau"}}))
    .AddSyst(cb, "CMS_TauFakeRate_p1_dm0_E_13TeV", "shape", SystMap<>::init(1.0));
  cb.cp().process(ch::JoinStr({{"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "vH_htt", "Zothers", "TT", "T", "EWK", "Diboson", "Fakes", "ZTauTau"}}))
    .AddSyst(cb, "CMS_TauFakeRate_p1_dm1_E_13TeV", "shape", SystMap<>::init(1.0));
  cb.cp().process(ch::JoinStr({{"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "vH_htt", "Zothers", "TT", "T", "EWK", "Diboson", "Fakes", "ZTauTau"}}))
    .AddSyst(cb, "CMS_TauFakeRate_p1_dm10_E_13TeV", "shape", SystMap<>::init(1.0));

  cb.cp().process(ch::JoinStr({{"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "vH_htt", "Zothers", "TT", "T", "EWK", "Diboson", "Fakes", "ZTauTau"}}))
    .AddSyst(cb, "CMS_MuonFakeRate_p0_13TeV", "shape", SystMap<>::init(1.0));
  cb.cp().process(ch::JoinStr({{"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "vH_htt", "Zothers", "TT", "T", "EWK", "Diboson", "Fakes", "ZTauTau"}}))
    .AddSyst(cb, "CMS_MuonFakeRate_p1_13TeV", "shape", SystMap<>::init(1.0));

  // Energy Scale Uncertainty
  cb.cp().process(ch::JoinStr({sig_procs, {"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "vH_htt", "Zothers", "TT", "T", "EWK", "Diboson"}}))
    .AddSyst(cb, "CMS_MES_13TeV", "shape", SystMap<>::init(1.0));

  cb.cp().process(ch::JoinStr({sig_procs, {"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "vH_htt", "TT", "T", "EWK", "Diboson"}}))
    .AddSyst(cb, "CMS_scale_t_1prong_13TeV", "shape", SystMap<>::init(1.0));
  cb.cp().process({{"ZTauTau"}})
    .AddSyst(cb, "CMS_scale_t_1prong_13TeV", "shape", SystMap<>::init(0.5));
  cb.cp().process({{"ZTauTau"}})
    .AddSyst(cb, "CMS_scale_emb_t_1prong_13TeV", "shape", SystMap<>::init(0.866));

  cb.cp().process(ch::JoinStr({sig_procs, {"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "vH_htt", "TT", "T", "EWK", "Diboson"}}))
    .AddSyst(cb, "CMS_scale_t_1prong1pizero_13TeV", "shape", SystMap<>::init(1.0));
  cb.cp().process({{"ZTauTau"}})
    .AddSyst(cb, "CMS_scale_t_1prong1pizero_13TeV", "shape", SystMap<>::init(0.5));
  cb.cp().process({{"ZTauTau"}})
    .AddSyst(cb, "CMS_scale_emb_t_1prong1pizero_13TeV", "shape", SystMap<>::init(0.866));

  cb.cp().process(ch::JoinStr({sig_procs, {"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "vH_htt", "TT", "T", "EWK", "Diboson"}}))
    .AddSyst(cb, "CMS_scale_t_3prong_13TeV", "shape", SystMap<>::init(1.0));
  cb.cp().process({{"ZTauTau"}})
    .AddSyst(cb, "CMS_scale_t_3prong_13TeV", "shape", SystMap<>::init(0.5));
  cb.cp().process({{"ZTauTau"}})
    .AddSyst(cb, "CMS_scale_emb_t_3prong_13TeV", "shape", SystMap<>::init(0.866));

  TString JESNAMES[28] = {"CMS_Jes_JetAbsoluteFlavMap_13TeV", "CMS_Jes_JetAbsoluteMPFBias_13TeV", "CMS_Jes_JetAbsoluteScale_13TeV", "CMS_Jes_JetAbsoluteStat_13TeV", "CMS_Jes_JetFlavorQCD_13TeV", "CMS_Jes_JetFragmentation_13TeV", "CMS_Jes_JetPileUpDataMC_13TeV", "CMS_Jes_JetPileUpPtBB_13TeV", "CMS_Jes_JetPileUpPtEC1_13TeV", "CMS_Jes_JetPileUpPtEC2_13TeV", "CMS_Jes_JetPileUpPtHF_13TeV", "CMS_Jes_JetPileUpPtRef_13TeV", "CMS_Jes_JetRelativeFSR_13TeV", "CMS_Jes_JetRelativeJEREC1_13TeV", "CMS_Jes_JetRelativeJEREC2_13TeV", "CMS_Jes_JetRelativeJERHF_13TeV", "CMS_Jes_JetRelativePtBB_13TeV", "CMS_Jes_JetRelativePtEC1_13TeV", "CMS_Jes_JetRelativePtEC2_13TeV", "CMS_Jes_JetRelativePtHF_13TeV", "CMS_Jes_JetRelativeStatEC_13TeV", "CMS_Jes_JetRelativeStatFSR_13TeV", "CMS_Jes_JetRelativeStatHF_13TeV", "CMS_Jes_JetSinglePionECAL_13TeV", "CMS_Jes_JetSinglePionHCAL_13TeV", "CMS_Jes_JetTimePtEta_13TeV", "CMS_Jes_JetRelativeBal_13TeV", "CMS_Jes_JetRelativeSample_13TeV"};
  for (int i = 0; i < 28; i++){
    cb.cp().process(ch::JoinStr({{"TT", "T", "Diboson", "vH_htt"}}))
      .AddSyst(cb, JESNAMES[i].Data(), "shape", SystMap<>::init(1.0));
  }

  cb.cp().process(ch::JoinStr({{"TT", "T", "Diboson", "vH_htt"}}))
    .AddSyst(cb, "CMS_MET_Ues_13TeV", "shape", SystMap<>::init(1.0));

  cb.cp().backgrounds().ExtractShapes(aux_shapes + dirInput+"/"+inputFile+".root", "$BIN/$PROCESS", "$BIN/$PROCESS_$SYSTEMATIC");
  cb.cp().signals().ExtractShapes(aux_shapes + dirInput+"/"+inputFile+".root", "$BIN/$PROCESS$MASS", "$BIN/$PROCESS$MASS_$SYSTEMATIC");

  // Bin By Bin Uncertainty
  if(binbybin){
    auto bbb = ch::BinByBinFactory()
      .SetAddThreshold(0.5)
      .SetMergeThreshold(0.5)
      .SetFixNorm(false);
    //bbb.MergeBinErrors(cb.cp().backgrounds());
    bbb.MergeBinErrors(cb.cp().process({"ggH_htt", "qqH_htt", "ggH_hww", "qqH_hww", "vH_htt", "Zothers", "TT", "T", "EWK", "Diboson", "Fakes", "ZTauTau"}));
    bbb.AddBinByBin(cb.cp().backgrounds(), cb);
  }

  // This function modifies every entry to have a standardised bin name of the form: {analysis}_{channel}_{bin_id}_{era} which is commonly used in the htt analyses
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
      cb.cp().bin({b}).mass({m, "*"}).WriteDatacard(folder + "/" + b + "_m" + m + ".txt", output);
    }
  }
}
