#include "LeptonTree.h"
LeptonTree lepton_tree;

void LeptonTree::Init(TTree *tree) {
  jets_branch = tree->GetBranch("jets");
  if (jets_branch) jets_branch->SetAddress(&jets_);
  p4_branch = tree->GetBranch("p4");
  if (p4_branch) p4_branch->SetAddress(&p4_);
  tag_p4_branch = tree->GetBranch("tag_p4");
  if (tag_p4_branch) tag_p4_branch->SetAddress(&tag_p4_);
  dilep_p4_branch = tree->GetBranch("dilep_p4");
  if (dilep_p4_branch) dilep_p4_branch->SetAddress(&dilep_p4_);
  jet_close_lep_branch = tree->GetBranch("jet_close_lep");
  if (jet_close_lep_branch) jet_close_lep_branch->SetAddress(&jet_close_lep_);

  tree->SetMakeClass(1);

  evt_corrMET_branch = tree->GetBranch("evt_corrMET");
  if (evt_corrMET_branch) evt_corrMET_branch->SetAddress(&evt_corrMET_);
  evt_corrMETPhi_branch = tree->GetBranch("evt_corrMETPhi");
  if (evt_corrMETPhi_branch) evt_corrMETPhi_branch->SetAddress(&evt_corrMETPhi_);
  evt_isRealData_branch = tree->GetBranch("evt_isRealData");
  if (evt_isRealData_branch) evt_isRealData_branch->SetAddress(&evt_isRealData_);
  scale1fb_branch = tree->GetBranch("scale1fb");
  if (scale1fb_branch) scale1fb_branch->SetAddress(&scale1fb_);
  ht_branch = tree->GetBranch("ht");
  if (ht_branch) ht_branch->SetAddress(&ht_);
  ht_SS_branch = tree->GetBranch("ht_SS");
  if (ht_SS_branch) ht_SS_branch->SetAddress(&ht_SS_);
  jets_disc_branch = tree->GetBranch("jets_disc");
  if (jets_disc_branch) jets_disc_branch->SetAddress(&jets_disc_);
  jets_area_branch = tree->GetBranch("jets_area");
  if (jets_area_branch) jets_area_branch->SetAddress(&jets_area_);
  jets_undoJEC_branch = tree->GetBranch("jets_undoJEC");
  if (jets_undoJEC_branch) jets_undoJEC_branch->SetAddress(&jets_undoJEC_);
  nFOs_SS_branch = tree->GetBranch("nFOs_SS");
  if (nFOs_SS_branch) nFOs_SS_branch->SetAddress(&nFOs_SS_);
  nvtx_branch = tree->GetBranch("nvtx");
  if (nvtx_branch) nvtx_branch->SetAddress(&nvtx_);
  id_branch = tree->GetBranch("id");
  if (id_branch) id_branch->SetAddress(&id_);
  idx_branch = tree->GetBranch("idx");
  if (idx_branch) idx_branch->SetAddress(&idx_);
  dxyPV_branch = tree->GetBranch("dxyPV");
  if (dxyPV_branch) dxyPV_branch->SetAddress(&dxyPV_);
  dZ_branch = tree->GetBranch("dZ");
  if (dZ_branch) dZ_branch->SetAddress(&dZ_);
  dxyPV_err_branch = tree->GetBranch("dxyPV_err");
  if (dxyPV_err_branch) dxyPV_err_branch->SetAddress(&dxyPV_err_);
  motherID_branch = tree->GetBranch("motherID");
  if (motherID_branch) motherID_branch->SetAddress(&motherID_);
  mc_id_branch = tree->GetBranch("mc_id");
  if (mc_id_branch) mc_id_branch->SetAddress(&mc_id_);
  RelIso03_branch = tree->GetBranch("RelIso03");
  if (RelIso03_branch) RelIso03_branch->SetAddress(&RelIso03_);
  RelIso03EA_branch = tree->GetBranch("RelIso03EA");
  if (RelIso03EA_branch) RelIso03EA_branch->SetAddress(&RelIso03EA_);
  RelIso03DB_branch = tree->GetBranch("RelIso03DB");
  if (RelIso03DB_branch) RelIso03DB_branch->SetAddress(&RelIso03DB_);
  tkIso_branch = tree->GetBranch("tkIso");
  if (tkIso_branch) tkIso_branch->SetAddress(&tkIso_);
  passes_VVV_cutbased_veto_branch = tree->GetBranch("passes_VVV_cutbased_veto");
  if (passes_VVV_cutbased_veto_branch) passes_VVV_cutbased_veto_branch->SetAddress(&passes_VVV_cutbased_veto_);
  passes_VVV_cutbased_veto_noiso_branch = tree->GetBranch("passes_VVV_cutbased_veto_noiso");
  if (passes_VVV_cutbased_veto_noiso_branch) passes_VVV_cutbased_veto_noiso_branch->SetAddress(&passes_VVV_cutbased_veto_noiso_);
  passes_VVV_cutbased_veto_noiso_noip_branch = tree->GetBranch("passes_VVV_cutbased_veto_noiso_noip");
  if (passes_VVV_cutbased_veto_noiso_noip_branch) passes_VVV_cutbased_veto_noiso_noip_branch->SetAddress(&passes_VVV_cutbased_veto_noiso_noip_);
  passes_VVV_cutbased_fo_branch = tree->GetBranch("passes_VVV_cutbased_fo");
  if (passes_VVV_cutbased_fo_branch) passes_VVV_cutbased_fo_branch->SetAddress(&passes_VVV_cutbased_fo_);
  passes_VVV_cutbased_fo_noiso_branch = tree->GetBranch("passes_VVV_cutbased_fo_noiso");
  if (passes_VVV_cutbased_fo_noiso_branch) passes_VVV_cutbased_fo_noiso_branch->SetAddress(&passes_VVV_cutbased_fo_noiso_);
  passes_VVV_cutbased_tight_noiso_branch = tree->GetBranch("passes_VVV_cutbased_tight_noiso");
  if (passes_VVV_cutbased_tight_noiso_branch) passes_VVV_cutbased_tight_noiso_branch->SetAddress(&passes_VVV_cutbased_tight_noiso_);
  passes_VVV_cutbased_tight_branch = tree->GetBranch("passes_VVV_cutbased_tight");
  if (passes_VVV_cutbased_tight_branch) passes_VVV_cutbased_tight_branch->SetAddress(&passes_VVV_cutbased_tight_);
  passes_VVV_MVAbased_tight_noiso_branch = tree->GetBranch("passes_VVV_MVAbased_tight_noiso");
  if (passes_VVV_MVAbased_tight_noiso_branch) passes_VVV_MVAbased_tight_noiso_branch->SetAddress(&passes_VVV_MVAbased_tight_noiso_);
  passes_VVV_MVAbased_tight_branch = tree->GetBranch("passes_VVV_MVAbased_tight");
  if (passes_VVV_MVAbased_tight_branch) passes_VVV_MVAbased_tight_branch->SetAddress(&passes_VVV_MVAbased_tight_);
  passes_VVV_baseline_branch = tree->GetBranch("passes_VVV_baseline");
  if (passes_VVV_baseline_branch) passes_VVV_baseline_branch->SetAddress(&passes_VVV_baseline_);
  ip3d_branch = tree->GetBranch("ip3d");
  if (ip3d_branch) ip3d_branch->SetAddress(&ip3d_);
  ip3derr_branch = tree->GetBranch("ip3derr");
  if (ip3derr_branch) ip3derr_branch->SetAddress(&ip3derr_);
  miniiso_branch = tree->GetBranch("miniiso");
  if (miniiso_branch) miniiso_branch->SetAddress(&miniiso_);
  miniisoDB_branch = tree->GetBranch("miniisoDB");
  if (miniisoDB_branch) miniisoDB_branch->SetAddress(&miniisoDB_);
  reliso04_branch = tree->GetBranch("reliso04");
  if (reliso04_branch) reliso04_branch->SetAddress(&reliso04_);
  annulus04_branch = tree->GetBranch("annulus04");
  if (annulus04_branch) annulus04_branch->SetAddress(&annulus04_);
  AbsTrkIso_branch = tree->GetBranch("AbsTrkIso");
  if (AbsTrkIso_branch) AbsTrkIso_branch->SetAddress(&AbsTrkIso_);
  jet_close_lep_undoJEC_branch = tree->GetBranch("jet_close_lep_undoJEC");
  if (jet_close_lep_undoJEC_branch) jet_close_lep_undoJEC_branch->SetAddress(&jet_close_lep_undoJEC_);
  jet_close_L1_branch = tree->GetBranch("jet_close_L1");
  if (jet_close_L1_branch) jet_close_L1_branch->SetAddress(&jet_close_L1_);
  jet_close_L2L3_branch = tree->GetBranch("jet_close_L2L3");
  if (jet_close_L2L3_branch) jet_close_L2L3_branch->SetAddress(&jet_close_L2L3_);
  ptratio_branch = tree->GetBranch("ptratio");
  if (ptratio_branch) ptratio_branch->SetAddress(&ptratio_);
  dilep_mass_branch = tree->GetBranch("dilep_mass");
  if (dilep_mass_branch) dilep_mass_branch->SetAddress(&dilep_mass_);
  isRandom_branch = tree->GetBranch("isRandom");
  if (isRandom_branch) isRandom_branch->SetAddress(&isRandom_);
  sigmaIEtaIEta_full5x5_branch = tree->GetBranch("sigmaIEtaIEta_full5x5");
  if (sigmaIEtaIEta_full5x5_branch) sigmaIEtaIEta_full5x5_branch->SetAddress(&sigmaIEtaIEta_full5x5_);
  etaSC_branch = tree->GetBranch("etaSC");
  if (etaSC_branch) etaSC_branch->SetAddress(&etaSC_);
  dEtaIn_branch = tree->GetBranch("dEtaIn");
  if (dEtaIn_branch) dEtaIn_branch->SetAddress(&dEtaIn_);
  dPhiIn_branch = tree->GetBranch("dPhiIn");
  if (dPhiIn_branch) dPhiIn_branch->SetAddress(&dPhiIn_);
  hOverE_branch = tree->GetBranch("hOverE");
  if (hOverE_branch) hOverE_branch->SetAddress(&hOverE_);
  scSeedEta_branch = tree->GetBranch("scSeedEta");
  if (scSeedEta_branch) scSeedEta_branch->SetAddress(&scSeedEta_);
  ecalEnergy_branch = tree->GetBranch("ecalEnergy");
  if (ecalEnergy_branch) ecalEnergy_branch->SetAddress(&ecalEnergy_);
  conv_vtx_flag_branch = tree->GetBranch("conv_vtx_flag");
  if (conv_vtx_flag_branch) conv_vtx_flag_branch->SetAddress(&conv_vtx_flag_);
  charge_branch = tree->GetBranch("charge");
  if (charge_branch) charge_branch->SetAddress(&charge_);
  mva_25ns_branch = tree->GetBranch("mva_25ns");
  if (mva_25ns_branch) mva_25ns_branch->SetAddress(&mva_25ns_);
  tag_mva_25ns_branch = tree->GetBranch("tag_mva_25ns");
  if (tag_mva_25ns_branch) tag_mva_25ns_branch->SetAddress(&tag_mva_25ns_);
  ecalPFClusterIso_branch = tree->GetBranch("ecalPFClusterIso");
  if (ecalPFClusterIso_branch) ecalPFClusterIso_branch->SetAddress(&ecalPFClusterIso_);
  hcalPFClusterIso_branch = tree->GetBranch("hcalPFClusterIso");
  if (hcalPFClusterIso_branch) hcalPFClusterIso_branch->SetAddress(&hcalPFClusterIso_);
  HLT_Mu8_branch = tree->GetBranch("HLT_Mu8");
  if (HLT_Mu8_branch) HLT_Mu8_branch->SetAddress(&HLT_Mu8_);
  HLT_Mu17_branch = tree->GetBranch("HLT_Mu17");
  if (HLT_Mu17_branch) HLT_Mu17_branch->SetAddress(&HLT_Mu17_);
  HLT_Mu8_TrkIsoVVL_branch = tree->GetBranch("HLT_Mu8_TrkIsoVVL");
  if (HLT_Mu8_TrkIsoVVL_branch) HLT_Mu8_TrkIsoVVL_branch->SetAddress(&HLT_Mu8_TrkIsoVVL_);
  HLT_Mu17_TrkIsoVVL_branch = tree->GetBranch("HLT_Mu17_TrkIsoVVL");
  if (HLT_Mu17_TrkIsoVVL_branch) HLT_Mu17_TrkIsoVVL_branch->SetAddress(&HLT_Mu17_TrkIsoVVL_);
  HLT_IsoMu24_branch = tree->GetBranch("HLT_IsoMu24");
  if (HLT_IsoMu24_branch) HLT_IsoMu24_branch->SetAddress(&HLT_IsoMu24_);
  HLT_IsoTkMu24_branch = tree->GetBranch("HLT_IsoTkMu24");
  if (HLT_IsoTkMu24_branch) HLT_IsoTkMu24_branch->SetAddress(&HLT_IsoTkMu24_);
  HLT_Ele8_CaloIdM_TrackIdM_PFJet30_branch = tree->GetBranch("HLT_Ele8_CaloIdM_TrackIdM_PFJet30");
  if (HLT_Ele8_CaloIdM_TrackIdM_PFJet30_branch) HLT_Ele8_CaloIdM_TrackIdM_PFJet30_branch->SetAddress(&HLT_Ele8_CaloIdM_TrackIdM_PFJet30_);
  HLT_Ele17_CaloIdM_TrackIdM_PFJet30_branch = tree->GetBranch("HLT_Ele17_CaloIdM_TrackIdM_PFJet30");
  if (HLT_Ele17_CaloIdM_TrackIdM_PFJet30_branch) HLT_Ele17_CaloIdM_TrackIdM_PFJet30_branch->SetAddress(&HLT_Ele17_CaloIdM_TrackIdM_PFJet30_);
  HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_branch = tree->GetBranch("HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30");
  if (HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_branch) HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_branch->SetAddress(&HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_);
  HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_branch = tree->GetBranch("HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30");
  if (HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_branch) HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_branch->SetAddress(&HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_);

  tree->SetMakeClass(0);
}

void LeptonTree::GetEntry(unsigned int idx) {
  // this only marks branches as not loaded, saving a lot of time
  index = idx;
  evt_corrMET_isLoaded = false;
  evt_corrMETPhi_isLoaded = false;
  evt_isRealData_isLoaded = false;
  scale1fb_isLoaded = false;
  ht_isLoaded = false;
  ht_SS_isLoaded = false;
  jets_isLoaded = false;
  jets_disc_isLoaded = false;
  jets_area_isLoaded = false;
  jets_undoJEC_isLoaded = false;
  nFOs_SS_isLoaded = false;
  nvtx_isLoaded = false;
  p4_isLoaded = false;
  tag_p4_isLoaded = false;
  dilep_p4_isLoaded = false;
  id_isLoaded = false;
  idx_isLoaded = false;
  dxyPV_isLoaded = false;
  dZ_isLoaded = false;
  dxyPV_err_isLoaded = false;
  motherID_isLoaded = false;
  mc_id_isLoaded = false;
  RelIso03_isLoaded = false;
  RelIso03EA_isLoaded = false;
  RelIso03DB_isLoaded = false;
  tkIso_isLoaded = false;
  passes_VVV_cutbased_veto_isLoaded = false;
  passes_VVV_cutbased_veto_noiso_isLoaded = false;
  passes_VVV_cutbased_veto_noiso_noip_isLoaded = false;
  passes_VVV_cutbased_fo_isLoaded = false;
  passes_VVV_cutbased_fo_noiso_isLoaded = false;
  passes_VVV_cutbased_tight_noiso_isLoaded = false;
  passes_VVV_cutbased_tight_isLoaded = false;
  passes_VVV_MVAbased_tight_noiso_isLoaded = false;
  passes_VVV_MVAbased_tight_isLoaded = false;
  passes_VVV_baseline_isLoaded = false;
  ip3d_isLoaded = false;
  ip3derr_isLoaded = false;
  miniiso_isLoaded = false;
  miniisoDB_isLoaded = false;
  reliso04_isLoaded = false;
  annulus04_isLoaded = false;
  AbsTrkIso_isLoaded = false;
  jet_close_lep_isLoaded = false;
  jet_close_lep_undoJEC_isLoaded = false;
  jet_close_L1_isLoaded = false;
  jet_close_L2L3_isLoaded = false;
  ptratio_isLoaded = false;
  dilep_mass_isLoaded = false;
  isRandom_isLoaded = false;
  sigmaIEtaIEta_full5x5_isLoaded = false;
  etaSC_isLoaded = false;
  dEtaIn_isLoaded = false;
  dPhiIn_isLoaded = false;
  hOverE_isLoaded = false;
  scSeedEta_isLoaded = false;
  ecalEnergy_isLoaded = false;
  conv_vtx_flag_isLoaded = false;
  charge_isLoaded = false;
  mva_25ns_isLoaded = false;
  tag_mva_25ns_isLoaded = false;
  ecalPFClusterIso_isLoaded = false;
  hcalPFClusterIso_isLoaded = false;
  HLT_Mu8_isLoaded = false;
  HLT_Mu17_isLoaded = false;
  HLT_Mu8_TrkIsoVVL_isLoaded = false;
  HLT_Mu17_TrkIsoVVL_isLoaded = false;
  HLT_IsoMu24_isLoaded = false;
  HLT_IsoTkMu24_isLoaded = false;
  HLT_Ele8_CaloIdM_TrackIdM_PFJet30_isLoaded = false;
  HLT_Ele17_CaloIdM_TrackIdM_PFJet30_isLoaded = false;
  HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_isLoaded = false;
  HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_isLoaded = false;
}

void LeptonTree::LoadAllBranches() {
  // load all branches
  if (evt_corrMET_branch != 0) evt_corrMET();
  if (evt_corrMETPhi_branch != 0) evt_corrMETPhi();
  if (evt_isRealData_branch != 0) evt_isRealData();
  if (scale1fb_branch != 0) scale1fb();
  if (ht_branch != 0) ht();
  if (ht_SS_branch != 0) ht_SS();
  if (jets_branch != 0) jets();
  if (jets_disc_branch != 0) jets_disc();
  if (jets_area_branch != 0) jets_area();
  if (jets_undoJEC_branch != 0) jets_undoJEC();
  if (nFOs_SS_branch != 0) nFOs_SS();
  if (nvtx_branch != 0) nvtx();
  if (p4_branch != 0) p4();
  if (tag_p4_branch != 0) tag_p4();
  if (dilep_p4_branch != 0) dilep_p4();
  if (id_branch != 0) id();
  if (idx_branch != 0) idx();
  if (dxyPV_branch != 0) dxyPV();
  if (dZ_branch != 0) dZ();
  if (dxyPV_err_branch != 0) dxyPV_err();
  if (motherID_branch != 0) motherID();
  if (mc_id_branch != 0) mc_id();
  if (RelIso03_branch != 0) RelIso03();
  if (RelIso03EA_branch != 0) RelIso03EA();
  if (RelIso03DB_branch != 0) RelIso03DB();
  if (tkIso_branch != 0) tkIso();
  if (passes_VVV_cutbased_veto_branch != 0) passes_VVV_cutbased_veto();
  if (passes_VVV_cutbased_veto_noiso_branch != 0) passes_VVV_cutbased_veto_noiso();
  if (passes_VVV_cutbased_veto_noiso_noip_branch != 0) passes_VVV_cutbased_veto_noiso_noip();
  if (passes_VVV_cutbased_fo_branch != 0) passes_VVV_cutbased_fo();
  if (passes_VVV_cutbased_fo_noiso_branch != 0) passes_VVV_cutbased_fo_noiso();
  if (passes_VVV_cutbased_tight_noiso_branch != 0) passes_VVV_cutbased_tight_noiso();
  if (passes_VVV_cutbased_tight_branch != 0) passes_VVV_cutbased_tight();
  if (passes_VVV_MVAbased_tight_noiso_branch != 0) passes_VVV_MVAbased_tight_noiso();
  if (passes_VVV_MVAbased_tight_branch != 0) passes_VVV_MVAbased_tight();
  if (passes_VVV_baseline_branch != 0) passes_VVV_baseline();
  if (ip3d_branch != 0) ip3d();
  if (ip3derr_branch != 0) ip3derr();
  if (miniiso_branch != 0) miniiso();
  if (miniisoDB_branch != 0) miniisoDB();
  if (reliso04_branch != 0) reliso04();
  if (annulus04_branch != 0) annulus04();
  if (AbsTrkIso_branch != 0) AbsTrkIso();
  if (jet_close_lep_branch != 0) jet_close_lep();
  if (jet_close_lep_undoJEC_branch != 0) jet_close_lep_undoJEC();
  if (jet_close_L1_branch != 0) jet_close_L1();
  if (jet_close_L2L3_branch != 0) jet_close_L2L3();
  if (ptratio_branch != 0) ptratio();
  if (dilep_mass_branch != 0) dilep_mass();
  if (isRandom_branch != 0) isRandom();
  if (sigmaIEtaIEta_full5x5_branch != 0) sigmaIEtaIEta_full5x5();
  if (etaSC_branch != 0) etaSC();
  if (dEtaIn_branch != 0) dEtaIn();
  if (dPhiIn_branch != 0) dPhiIn();
  if (hOverE_branch != 0) hOverE();
  if (scSeedEta_branch != 0) scSeedEta();
  if (ecalEnergy_branch != 0) ecalEnergy();
  if (conv_vtx_flag_branch != 0) conv_vtx_flag();
  if (charge_branch != 0) charge();
  if (mva_25ns_branch != 0) mva_25ns();
  if (tag_mva_25ns_branch != 0) tag_mva_25ns();
  if (ecalPFClusterIso_branch != 0) ecalPFClusterIso();
  if (hcalPFClusterIso_branch != 0) hcalPFClusterIso();
  if (HLT_Mu8_branch != 0) HLT_Mu8();
  if (HLT_Mu17_branch != 0) HLT_Mu17();
  if (HLT_Mu8_TrkIsoVVL_branch != 0) HLT_Mu8_TrkIsoVVL();
  if (HLT_Mu17_TrkIsoVVL_branch != 0) HLT_Mu17_TrkIsoVVL();
  if (HLT_IsoMu24_branch != 0) HLT_IsoMu24();
  if (HLT_IsoTkMu24_branch != 0) HLT_IsoTkMu24();
  if (HLT_Ele8_CaloIdM_TrackIdM_PFJet30_branch != 0) HLT_Ele8_CaloIdM_TrackIdM_PFJet30();
  if (HLT_Ele17_CaloIdM_TrackIdM_PFJet30_branch != 0) HLT_Ele17_CaloIdM_TrackIdM_PFJet30();
  if (HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_branch != 0) HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30();
  if (HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_branch != 0) HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30();
}

const float &LeptonTree::evt_corrMET() {
  if (not evt_corrMET_isLoaded) {
    if (evt_corrMET_branch != 0) {
      evt_corrMET_branch->GetEntry(index);
    } else {
      printf("branch evt_corrMET_branch does not exist!\n");
      exit(1);
    }
    evt_corrMET_isLoaded = true;
  }
  return evt_corrMET_;
}

const float &LeptonTree::evt_corrMETPhi() {
  if (not evt_corrMETPhi_isLoaded) {
    if (evt_corrMETPhi_branch != 0) {
      evt_corrMETPhi_branch->GetEntry(index);
    } else {
      printf("branch evt_corrMETPhi_branch does not exist!\n");
      exit(1);
    }
    evt_corrMETPhi_isLoaded = true;
  }
  return evt_corrMETPhi_;
}

const bool &LeptonTree::evt_isRealData() {
  if (not evt_isRealData_isLoaded) {
    if (evt_isRealData_branch != 0) {
      evt_isRealData_branch->GetEntry(index);
    } else {
      printf("branch evt_isRealData_branch does not exist!\n");
      exit(1);
    }
    evt_isRealData_isLoaded = true;
  }
  return evt_isRealData_;
}

const float &LeptonTree::scale1fb() {
  if (not scale1fb_isLoaded) {
    if (scale1fb_branch != 0) {
      scale1fb_branch->GetEntry(index);
    } else {
      printf("branch scale1fb_branch does not exist!\n");
      exit(1);
    }
    scale1fb_isLoaded = true;
  }
  return scale1fb_;
}

const float &LeptonTree::ht() {
  if (not ht_isLoaded) {
    if (ht_branch != 0) {
      ht_branch->GetEntry(index);
    } else {
      printf("branch ht_branch does not exist!\n");
      exit(1);
    }
    ht_isLoaded = true;
  }
  return ht_;
}

const float &LeptonTree::ht_SS() {
  if (not ht_SS_isLoaded) {
    if (ht_SS_branch != 0) {
      ht_SS_branch->GetEntry(index);
    } else {
      printf("branch ht_SS_branch does not exist!\n");
      exit(1);
    }
    ht_SS_isLoaded = true;
  }
  return ht_SS_;
}

const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &LeptonTree::jets() {
  if (not jets_isLoaded) {
    if (jets_branch != 0) {
      jets_branch->GetEntry(index);
    } else {
      printf("branch jets_branch does not exist!\n");
      exit(1);
    }
    jets_isLoaded = true;
  }
  return *jets_;
}

const vector<float> &LeptonTree::jets_disc() {
  if (not jets_disc_isLoaded) {
    if (jets_disc_branch != 0) {
      jets_disc_branch->GetEntry(index);
    } else {
      printf("branch jets_disc_branch does not exist!\n");
      exit(1);
    }
    jets_disc_isLoaded = true;
  }
  return *jets_disc_;
}

const vector<float> &LeptonTree::jets_area() {
  if (not jets_area_isLoaded) {
    if (jets_area_branch != 0) {
      jets_area_branch->GetEntry(index);
    } else {
      printf("branch jets_area_branch does not exist!\n");
      exit(1);
    }
    jets_area_isLoaded = true;
  }
  return *jets_area_;
}

const vector<float> &LeptonTree::jets_undoJEC() {
  if (not jets_undoJEC_isLoaded) {
    if (jets_undoJEC_branch != 0) {
      jets_undoJEC_branch->GetEntry(index);
    } else {
      printf("branch jets_undoJEC_branch does not exist!\n");
      exit(1);
    }
    jets_undoJEC_isLoaded = true;
  }
  return *jets_undoJEC_;
}

const int &LeptonTree::nFOs_SS() {
  if (not nFOs_SS_isLoaded) {
    if (nFOs_SS_branch != 0) {
      nFOs_SS_branch->GetEntry(index);
    } else {
      printf("branch nFOs_SS_branch does not exist!\n");
      exit(1);
    }
    nFOs_SS_isLoaded = true;
  }
  return nFOs_SS_;
}

const int &LeptonTree::nvtx() {
  if (not nvtx_isLoaded) {
    if (nvtx_branch != 0) {
      nvtx_branch->GetEntry(index);
    } else {
      printf("branch nvtx_branch does not exist!\n");
      exit(1);
    }
    nvtx_isLoaded = true;
  }
  return nvtx_;
}

const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &LeptonTree::p4() {
  if (not p4_isLoaded) {
    if (p4_branch != 0) {
      p4_branch->GetEntry(index);
    } else {
      printf("branch p4_branch does not exist!\n");
      exit(1);
    }
    p4_isLoaded = true;
  }
  return *p4_;
}

const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &LeptonTree::tag_p4() {
  if (not tag_p4_isLoaded) {
    if (tag_p4_branch != 0) {
      tag_p4_branch->GetEntry(index);
    } else {
      printf("branch tag_p4_branch does not exist!\n");
      exit(1);
    }
    tag_p4_isLoaded = true;
  }
  return *tag_p4_;
}

const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &LeptonTree::dilep_p4() {
  if (not dilep_p4_isLoaded) {
    if (dilep_p4_branch != 0) {
      dilep_p4_branch->GetEntry(index);
    } else {
      printf("branch dilep_p4_branch does not exist!\n");
      exit(1);
    }
    dilep_p4_isLoaded = true;
  }
  return *dilep_p4_;
}

const int &LeptonTree::id() {
  if (not id_isLoaded) {
    if (id_branch != 0) {
      id_branch->GetEntry(index);
    } else {
      printf("branch id_branch does not exist!\n");
      exit(1);
    }
    id_isLoaded = true;
  }
  return id_;
}

const int &LeptonTree::idx() {
  if (not idx_isLoaded) {
    if (idx_branch != 0) {
      idx_branch->GetEntry(index);
    } else {
      printf("branch idx_branch does not exist!\n");
      exit(1);
    }
    idx_isLoaded = true;
  }
  return idx_;
}

const float &LeptonTree::dxyPV() {
  if (not dxyPV_isLoaded) {
    if (dxyPV_branch != 0) {
      dxyPV_branch->GetEntry(index);
    } else {
      printf("branch dxyPV_branch does not exist!\n");
      exit(1);
    }
    dxyPV_isLoaded = true;
  }
  return dxyPV_;
}

const float &LeptonTree::dZ() {
  if (not dZ_isLoaded) {
    if (dZ_branch != 0) {
      dZ_branch->GetEntry(index);
    } else {
      printf("branch dZ_branch does not exist!\n");
      exit(1);
    }
    dZ_isLoaded = true;
  }
  return dZ_;
}

const float &LeptonTree::dxyPV_err() {
  if (not dxyPV_err_isLoaded) {
    if (dxyPV_err_branch != 0) {
      dxyPV_err_branch->GetEntry(index);
    } else {
      printf("branch dxyPV_err_branch does not exist!\n");
      exit(1);
    }
    dxyPV_err_isLoaded = true;
  }
  return dxyPV_err_;
}

const int &LeptonTree::motherID() {
  if (not motherID_isLoaded) {
    if (motherID_branch != 0) {
      motherID_branch->GetEntry(index);
    } else {
      printf("branch motherID_branch does not exist!\n");
      exit(1);
    }
    motherID_isLoaded = true;
  }
  return motherID_;
}

const int &LeptonTree::mc_id() {
  if (not mc_id_isLoaded) {
    if (mc_id_branch != 0) {
      mc_id_branch->GetEntry(index);
    } else {
      printf("branch mc_id_branch does not exist!\n");
      exit(1);
    }
    mc_id_isLoaded = true;
  }
  return mc_id_;
}

const float &LeptonTree::RelIso03() {
  if (not RelIso03_isLoaded) {
    if (RelIso03_branch != 0) {
      RelIso03_branch->GetEntry(index);
    } else {
      printf("branch RelIso03_branch does not exist!\n");
      exit(1);
    }
    RelIso03_isLoaded = true;
  }
  return RelIso03_;
}

const float &LeptonTree::RelIso03EA() {
  if (not RelIso03EA_isLoaded) {
    if (RelIso03EA_branch != 0) {
      RelIso03EA_branch->GetEntry(index);
    } else {
      printf("branch RelIso03EA_branch does not exist!\n");
      exit(1);
    }
    RelIso03EA_isLoaded = true;
  }
  return RelIso03EA_;
}

const float &LeptonTree::RelIso03DB() {
  if (not RelIso03DB_isLoaded) {
    if (RelIso03DB_branch != 0) {
      RelIso03DB_branch->GetEntry(index);
    } else {
      printf("branch RelIso03DB_branch does not exist!\n");
      exit(1);
    }
    RelIso03DB_isLoaded = true;
  }
  return RelIso03DB_;
}

const float &LeptonTree::tkIso() {
  if (not tkIso_isLoaded) {
    if (tkIso_branch != 0) {
      tkIso_branch->GetEntry(index);
    } else {
      printf("branch tkIso_branch does not exist!\n");
      exit(1);
    }
    tkIso_isLoaded = true;
  }
  return tkIso_;
}

const bool &LeptonTree::passes_VVV_cutbased_veto() {
  if (not passes_VVV_cutbased_veto_isLoaded) {
    if (passes_VVV_cutbased_veto_branch != 0) {
      passes_VVV_cutbased_veto_branch->GetEntry(index);
    } else {
      printf("branch passes_VVV_cutbased_veto_branch does not exist!\n");
      exit(1);
    }
    passes_VVV_cutbased_veto_isLoaded = true;
  }
  return passes_VVV_cutbased_veto_;
}

const bool &LeptonTree::passes_VVV_cutbased_veto_noiso() {
  if (not passes_VVV_cutbased_veto_noiso_isLoaded) {
    if (passes_VVV_cutbased_veto_noiso_branch != 0) {
      passes_VVV_cutbased_veto_noiso_branch->GetEntry(index);
    } else {
      printf("branch passes_VVV_cutbased_veto_noiso_branch does not exist!\n");
      exit(1);
    }
    passes_VVV_cutbased_veto_noiso_isLoaded = true;
  }
  return passes_VVV_cutbased_veto_noiso_;
}

const bool &LeptonTree::passes_VVV_cutbased_veto_noiso_noip() {
  if (not passes_VVV_cutbased_veto_noiso_noip_isLoaded) {
    if (passes_VVV_cutbased_veto_noiso_noip_branch != 0) {
      passes_VVV_cutbased_veto_noiso_noip_branch->GetEntry(index);
    } else {
      printf("branch passes_VVV_cutbased_veto_noiso_noip_branch does not exist!\n");
      exit(1);
    }
    passes_VVV_cutbased_veto_noiso_noip_isLoaded = true;
  }
  return passes_VVV_cutbased_veto_noiso_noip_;
}

const bool &LeptonTree::passes_VVV_cutbased_fo() {
  if (not passes_VVV_cutbased_fo_isLoaded) {
    if (passes_VVV_cutbased_fo_branch != 0) {
      passes_VVV_cutbased_fo_branch->GetEntry(index);
    } else {
      printf("branch passes_VVV_cutbased_fo_branch does not exist!\n");
      exit(1);
    }
    passes_VVV_cutbased_fo_isLoaded = true;
  }
  return passes_VVV_cutbased_fo_;
}

const bool &LeptonTree::passes_VVV_cutbased_fo_noiso() {
  if (not passes_VVV_cutbased_fo_noiso_isLoaded) {
    if (passes_VVV_cutbased_fo_noiso_branch != 0) {
      passes_VVV_cutbased_fo_noiso_branch->GetEntry(index);
    } else {
      printf("branch passes_VVV_cutbased_fo_noiso_branch does not exist!\n");
      exit(1);
    }
    passes_VVV_cutbased_fo_noiso_isLoaded = true;
  }
  return passes_VVV_cutbased_fo_noiso_;
}

const bool &LeptonTree::passes_VVV_cutbased_tight_noiso() {
  if (not passes_VVV_cutbased_tight_noiso_isLoaded) {
    if (passes_VVV_cutbased_tight_noiso_branch != 0) {
      passes_VVV_cutbased_tight_noiso_branch->GetEntry(index);
    } else {
      printf("branch passes_VVV_cutbased_tight_noiso_branch does not exist!\n");
      exit(1);
    }
    passes_VVV_cutbased_tight_noiso_isLoaded = true;
  }
  return passes_VVV_cutbased_tight_noiso_;
}

const bool &LeptonTree::passes_VVV_cutbased_tight() {
  if (not passes_VVV_cutbased_tight_isLoaded) {
    if (passes_VVV_cutbased_tight_branch != 0) {
      passes_VVV_cutbased_tight_branch->GetEntry(index);
    } else {
      printf("branch passes_VVV_cutbased_tight_branch does not exist!\n");
      exit(1);
    }
    passes_VVV_cutbased_tight_isLoaded = true;
  }
  return passes_VVV_cutbased_tight_;
}

const bool &LeptonTree::passes_VVV_MVAbased_tight_noiso() {
  if (not passes_VVV_MVAbased_tight_noiso_isLoaded) {
    if (passes_VVV_MVAbased_tight_noiso_branch != 0) {
      passes_VVV_MVAbased_tight_noiso_branch->GetEntry(index);
    } else {
      printf("branch passes_VVV_MVAbased_tight_noiso_branch does not exist!\n");
      exit(1);
    }
    passes_VVV_MVAbased_tight_noiso_isLoaded = true;
  }
  return passes_VVV_MVAbased_tight_noiso_;
}

const bool &LeptonTree::passes_VVV_MVAbased_tight() {
  if (not passes_VVV_MVAbased_tight_isLoaded) {
    if (passes_VVV_MVAbased_tight_branch != 0) {
      passes_VVV_MVAbased_tight_branch->GetEntry(index);
    } else {
      printf("branch passes_VVV_MVAbased_tight_branch does not exist!\n");
      exit(1);
    }
    passes_VVV_MVAbased_tight_isLoaded = true;
  }
  return passes_VVV_MVAbased_tight_;
}

const bool &LeptonTree::passes_VVV_baseline() {
  if (not passes_VVV_baseline_isLoaded) {
    if (passes_VVV_baseline_branch != 0) {
      passes_VVV_baseline_branch->GetEntry(index);
    } else {
      printf("branch passes_VVV_baseline_branch does not exist!\n");
      exit(1);
    }
    passes_VVV_baseline_isLoaded = true;
  }
  return passes_VVV_baseline_;
}

const float &LeptonTree::ip3d() {
  if (not ip3d_isLoaded) {
    if (ip3d_branch != 0) {
      ip3d_branch->GetEntry(index);
    } else {
      printf("branch ip3d_branch does not exist!\n");
      exit(1);
    }
    ip3d_isLoaded = true;
  }
  return ip3d_;
}

const float &LeptonTree::ip3derr() {
  if (not ip3derr_isLoaded) {
    if (ip3derr_branch != 0) {
      ip3derr_branch->GetEntry(index);
    } else {
      printf("branch ip3derr_branch does not exist!\n");
      exit(1);
    }
    ip3derr_isLoaded = true;
  }
  return ip3derr_;
}

const float &LeptonTree::miniiso() {
  if (not miniiso_isLoaded) {
    if (miniiso_branch != 0) {
      miniiso_branch->GetEntry(index);
    } else {
      printf("branch miniiso_branch does not exist!\n");
      exit(1);
    }
    miniiso_isLoaded = true;
  }
  return miniiso_;
}

const float &LeptonTree::miniisoDB() {
  if (not miniisoDB_isLoaded) {
    if (miniisoDB_branch != 0) {
      miniisoDB_branch->GetEntry(index);
    } else {
      printf("branch miniisoDB_branch does not exist!\n");
      exit(1);
    }
    miniisoDB_isLoaded = true;
  }
  return miniisoDB_;
}

const float &LeptonTree::reliso04() {
  if (not reliso04_isLoaded) {
    if (reliso04_branch != 0) {
      reliso04_branch->GetEntry(index);
    } else {
      printf("branch reliso04_branch does not exist!\n");
      exit(1);
    }
    reliso04_isLoaded = true;
  }
  return reliso04_;
}

const float &LeptonTree::annulus04() {
  if (not annulus04_isLoaded) {
    if (annulus04_branch != 0) {
      annulus04_branch->GetEntry(index);
    } else {
      printf("branch annulus04_branch does not exist!\n");
      exit(1);
    }
    annulus04_isLoaded = true;
  }
  return annulus04_;
}

const float &LeptonTree::AbsTrkIso() {
  if (not AbsTrkIso_isLoaded) {
    if (AbsTrkIso_branch != 0) {
      AbsTrkIso_branch->GetEntry(index);
    } else {
      printf("branch AbsTrkIso_branch does not exist!\n");
      exit(1);
    }
    AbsTrkIso_isLoaded = true;
  }
  return AbsTrkIso_;
}

const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &LeptonTree::jet_close_lep() {
  if (not jet_close_lep_isLoaded) {
    if (jet_close_lep_branch != 0) {
      jet_close_lep_branch->GetEntry(index);
    } else {
      printf("branch jet_close_lep_branch does not exist!\n");
      exit(1);
    }
    jet_close_lep_isLoaded = true;
  }
  return *jet_close_lep_;
}

const float &LeptonTree::jet_close_lep_undoJEC() {
  if (not jet_close_lep_undoJEC_isLoaded) {
    if (jet_close_lep_undoJEC_branch != 0) {
      jet_close_lep_undoJEC_branch->GetEntry(index);
    } else {
      printf("branch jet_close_lep_undoJEC_branch does not exist!\n");
      exit(1);
    }
    jet_close_lep_undoJEC_isLoaded = true;
  }
  return jet_close_lep_undoJEC_;
}

const float &LeptonTree::jet_close_L1() {
  if (not jet_close_L1_isLoaded) {
    if (jet_close_L1_branch != 0) {
      jet_close_L1_branch->GetEntry(index);
    } else {
      printf("branch jet_close_L1_branch does not exist!\n");
      exit(1);
    }
    jet_close_L1_isLoaded = true;
  }
  return jet_close_L1_;
}

const float &LeptonTree::jet_close_L2L3() {
  if (not jet_close_L2L3_isLoaded) {
    if (jet_close_L2L3_branch != 0) {
      jet_close_L2L3_branch->GetEntry(index);
    } else {
      printf("branch jet_close_L2L3_branch does not exist!\n");
      exit(1);
    }
    jet_close_L2L3_isLoaded = true;
  }
  return jet_close_L2L3_;
}

const float &LeptonTree::ptratio() {
  if (not ptratio_isLoaded) {
    if (ptratio_branch != 0) {
      ptratio_branch->GetEntry(index);
    } else {
      printf("branch ptratio_branch does not exist!\n");
      exit(1);
    }
    ptratio_isLoaded = true;
  }
  return ptratio_;
}

const float &LeptonTree::dilep_mass() {
  if (not dilep_mass_isLoaded) {
    if (dilep_mass_branch != 0) {
      dilep_mass_branch->GetEntry(index);
    } else {
      printf("branch dilep_mass_branch does not exist!\n");
      exit(1);
    }
    dilep_mass_isLoaded = true;
  }
  return dilep_mass_;
}

const bool &LeptonTree::isRandom() {
  if (not isRandom_isLoaded) {
    if (isRandom_branch != 0) {
      isRandom_branch->GetEntry(index);
    } else {
      printf("branch isRandom_branch does not exist!\n");
      exit(1);
    }
    isRandom_isLoaded = true;
  }
  return isRandom_;
}

const float &LeptonTree::sigmaIEtaIEta_full5x5() {
  if (not sigmaIEtaIEta_full5x5_isLoaded) {
    if (sigmaIEtaIEta_full5x5_branch != 0) {
      sigmaIEtaIEta_full5x5_branch->GetEntry(index);
    } else {
      printf("branch sigmaIEtaIEta_full5x5_branch does not exist!\n");
      exit(1);
    }
    sigmaIEtaIEta_full5x5_isLoaded = true;
  }
  return sigmaIEtaIEta_full5x5_;
}

const float &LeptonTree::etaSC() {
  if (not etaSC_isLoaded) {
    if (etaSC_branch != 0) {
      etaSC_branch->GetEntry(index);
    } else {
      printf("branch etaSC_branch does not exist!\n");
      exit(1);
    }
    etaSC_isLoaded = true;
  }
  return etaSC_;
}

const float &LeptonTree::dEtaIn() {
  if (not dEtaIn_isLoaded) {
    if (dEtaIn_branch != 0) {
      dEtaIn_branch->GetEntry(index);
    } else {
      printf("branch dEtaIn_branch does not exist!\n");
      exit(1);
    }
    dEtaIn_isLoaded = true;
  }
  return dEtaIn_;
}

const float &LeptonTree::dPhiIn() {
  if (not dPhiIn_isLoaded) {
    if (dPhiIn_branch != 0) {
      dPhiIn_branch->GetEntry(index);
    } else {
      printf("branch dPhiIn_branch does not exist!\n");
      exit(1);
    }
    dPhiIn_isLoaded = true;
  }
  return dPhiIn_;
}

const float &LeptonTree::hOverE() {
  if (not hOverE_isLoaded) {
    if (hOverE_branch != 0) {
      hOverE_branch->GetEntry(index);
    } else {
      printf("branch hOverE_branch does not exist!\n");
      exit(1);
    }
    hOverE_isLoaded = true;
  }
  return hOverE_;
}

const float &LeptonTree::scSeedEta() {
  if (not scSeedEta_isLoaded) {
    if (scSeedEta_branch != 0) {
      scSeedEta_branch->GetEntry(index);
    } else {
      printf("branch scSeedEta_branch does not exist!\n");
      exit(1);
    }
    scSeedEta_isLoaded = true;
  }
  return scSeedEta_;
}

const float &LeptonTree::ecalEnergy() {
  if (not ecalEnergy_isLoaded) {
    if (ecalEnergy_branch != 0) {
      ecalEnergy_branch->GetEntry(index);
    } else {
      printf("branch ecalEnergy_branch does not exist!\n");
      exit(1);
    }
    ecalEnergy_isLoaded = true;
  }
  return ecalEnergy_;
}

const bool &LeptonTree::conv_vtx_flag() {
  if (not conv_vtx_flag_isLoaded) {
    if (conv_vtx_flag_branch != 0) {
      conv_vtx_flag_branch->GetEntry(index);
    } else {
      printf("branch conv_vtx_flag_branch does not exist!\n");
      exit(1);
    }
    conv_vtx_flag_isLoaded = true;
  }
  return conv_vtx_flag_;
}

const int &LeptonTree::charge() {
  if (not charge_isLoaded) {
    if (charge_branch != 0) {
      charge_branch->GetEntry(index);
    } else {
      printf("branch charge_branch does not exist!\n");
      exit(1);
    }
    charge_isLoaded = true;
  }
  return charge_;
}

const float &LeptonTree::mva_25ns() {
  if (not mva_25ns_isLoaded) {
    if (mva_25ns_branch != 0) {
      mva_25ns_branch->GetEntry(index);
    } else {
      printf("branch mva_25ns_branch does not exist!\n");
      exit(1);
    }
    mva_25ns_isLoaded = true;
  }
  return mva_25ns_;
}

const float &LeptonTree::tag_mva_25ns() {
  if (not tag_mva_25ns_isLoaded) {
    if (tag_mva_25ns_branch != 0) {
      tag_mva_25ns_branch->GetEntry(index);
    } else {
      printf("branch tag_mva_25ns_branch does not exist!\n");
      exit(1);
    }
    tag_mva_25ns_isLoaded = true;
  }
  return tag_mva_25ns_;
}

const float &LeptonTree::ecalPFClusterIso() {
  if (not ecalPFClusterIso_isLoaded) {
    if (ecalPFClusterIso_branch != 0) {
      ecalPFClusterIso_branch->GetEntry(index);
    } else {
      printf("branch ecalPFClusterIso_branch does not exist!\n");
      exit(1);
    }
    ecalPFClusterIso_isLoaded = true;
  }
  return ecalPFClusterIso_;
}

const float &LeptonTree::hcalPFClusterIso() {
  if (not hcalPFClusterIso_isLoaded) {
    if (hcalPFClusterIso_branch != 0) {
      hcalPFClusterIso_branch->GetEntry(index);
    } else {
      printf("branch hcalPFClusterIso_branch does not exist!\n");
      exit(1);
    }
    hcalPFClusterIso_isLoaded = true;
  }
  return hcalPFClusterIso_;
}

const int &LeptonTree::HLT_Mu8() {
  if (not HLT_Mu8_isLoaded) {
    if (HLT_Mu8_branch != 0) {
      HLT_Mu8_branch->GetEntry(index);
    } else {
      printf("branch HLT_Mu8_branch does not exist!\n");
      exit(1);
    }
    HLT_Mu8_isLoaded = true;
  }
  return HLT_Mu8_;
}

const int &LeptonTree::HLT_Mu17() {
  if (not HLT_Mu17_isLoaded) {
    if (HLT_Mu17_branch != 0) {
      HLT_Mu17_branch->GetEntry(index);
    } else {
      printf("branch HLT_Mu17_branch does not exist!\n");
      exit(1);
    }
    HLT_Mu17_isLoaded = true;
  }
  return HLT_Mu17_;
}

const int &LeptonTree::HLT_Mu8_TrkIsoVVL() {
  if (not HLT_Mu8_TrkIsoVVL_isLoaded) {
    if (HLT_Mu8_TrkIsoVVL_branch != 0) {
      HLT_Mu8_TrkIsoVVL_branch->GetEntry(index);
    } else {
      printf("branch HLT_Mu8_TrkIsoVVL_branch does not exist!\n");
      exit(1);
    }
    HLT_Mu8_TrkIsoVVL_isLoaded = true;
  }
  return HLT_Mu8_TrkIsoVVL_;
}

const int &LeptonTree::HLT_Mu17_TrkIsoVVL() {
  if (not HLT_Mu17_TrkIsoVVL_isLoaded) {
    if (HLT_Mu17_TrkIsoVVL_branch != 0) {
      HLT_Mu17_TrkIsoVVL_branch->GetEntry(index);
    } else {
      printf("branch HLT_Mu17_TrkIsoVVL_branch does not exist!\n");
      exit(1);
    }
    HLT_Mu17_TrkIsoVVL_isLoaded = true;
  }
  return HLT_Mu17_TrkIsoVVL_;
}

const int &LeptonTree::HLT_IsoMu24() {
  if (not HLT_IsoMu24_isLoaded) {
    if (HLT_IsoMu24_branch != 0) {
      HLT_IsoMu24_branch->GetEntry(index);
    } else {
      printf("branch HLT_IsoMu24_branch does not exist!\n");
      exit(1);
    }
    HLT_IsoMu24_isLoaded = true;
  }
  return HLT_IsoMu24_;
}

const int &LeptonTree::HLT_IsoTkMu24() {
  if (not HLT_IsoTkMu24_isLoaded) {
    if (HLT_IsoTkMu24_branch != 0) {
      HLT_IsoTkMu24_branch->GetEntry(index);
    } else {
      printf("branch HLT_IsoTkMu24_branch does not exist!\n");
      exit(1);
    }
    HLT_IsoTkMu24_isLoaded = true;
  }
  return HLT_IsoTkMu24_;
}

const int &LeptonTree::HLT_Ele8_CaloIdM_TrackIdM_PFJet30() {
  if (not HLT_Ele8_CaloIdM_TrackIdM_PFJet30_isLoaded) {
    if (HLT_Ele8_CaloIdM_TrackIdM_PFJet30_branch != 0) {
      HLT_Ele8_CaloIdM_TrackIdM_PFJet30_branch->GetEntry(index);
    } else {
      printf("branch HLT_Ele8_CaloIdM_TrackIdM_PFJet30_branch does not exist!\n");
      exit(1);
    }
    HLT_Ele8_CaloIdM_TrackIdM_PFJet30_isLoaded = true;
  }
  return HLT_Ele8_CaloIdM_TrackIdM_PFJet30_;
}

const int &LeptonTree::HLT_Ele17_CaloIdM_TrackIdM_PFJet30() {
  if (not HLT_Ele17_CaloIdM_TrackIdM_PFJet30_isLoaded) {
    if (HLT_Ele17_CaloIdM_TrackIdM_PFJet30_branch != 0) {
      HLT_Ele17_CaloIdM_TrackIdM_PFJet30_branch->GetEntry(index);
    } else {
      printf("branch HLT_Ele17_CaloIdM_TrackIdM_PFJet30_branch does not exist!\n");
      exit(1);
    }
    HLT_Ele17_CaloIdM_TrackIdM_PFJet30_isLoaded = true;
  }
  return HLT_Ele17_CaloIdM_TrackIdM_PFJet30_;
}

const int &LeptonTree::HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30() {
  if (not HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_isLoaded) {
    if (HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_branch != 0) {
      HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_branch->GetEntry(index);
    } else {
      printf("branch HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_branch does not exist!\n");
      exit(1);
    }
    HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_isLoaded = true;
  }
  return HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_;
}

const int &LeptonTree::HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30() {
  if (not HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_isLoaded) {
    if (HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_branch != 0) {
      HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_branch->GetEntry(index);
    } else {
      printf("branch HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_branch does not exist!\n");
      exit(1);
    }
    HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_isLoaded = true;
  }
  return HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_;
}


void LeptonTree::progress( int nEventsTotal, int nEventsChain ){
  int period = 1000;
  if (nEventsTotal%1000 == 0) {
    // xterm magic from L. Vacavant and A. Cerri
    if (isatty(1)) {
      if ((nEventsChain - nEventsTotal) > period) {
        float frac = (float)nEventsTotal/(nEventsChain*0.01);
        printf("\015\033[32m ---> \033[1m\033[31m%4.1f%%"
             "\033[0m\033[32m <---\033[0m\015", frac);
        fflush(stdout);
      }
      else {
        printf("\015\033[32m ---> \033[1m\033[31m%4.1f%%"
               "\033[0m\033[32m <---\033[0m\015", 100.);
        cout << endl;
      }
    }
  }
}

namespace tas {

const float &evt_corrMET() { return lepton_tree.evt_corrMET(); }
const float &evt_corrMETPhi() { return lepton_tree.evt_corrMETPhi(); }
const bool &evt_isRealData() { return lepton_tree.evt_isRealData(); }
const float &scale1fb() { return lepton_tree.scale1fb(); }
const float &ht() { return lepton_tree.ht(); }
const float &ht_SS() { return lepton_tree.ht_SS(); }
const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets() { return lepton_tree.jets(); }
const vector<float> &jets_disc() { return lepton_tree.jets_disc(); }
const vector<float> &jets_area() { return lepton_tree.jets_area(); }
const vector<float> &jets_undoJEC() { return lepton_tree.jets_undoJEC(); }
const int &nFOs_SS() { return lepton_tree.nFOs_SS(); }
const int &nvtx() { return lepton_tree.nvtx(); }
const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &p4() { return lepton_tree.p4(); }
const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &tag_p4() { return lepton_tree.tag_p4(); }
const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &dilep_p4() { return lepton_tree.dilep_p4(); }
const int &id() { return lepton_tree.id(); }
const int &idx() { return lepton_tree.idx(); }
const float &dxyPV() { return lepton_tree.dxyPV(); }
const float &dZ() { return lepton_tree.dZ(); }
const float &dxyPV_err() { return lepton_tree.dxyPV_err(); }
const int &motherID() { return lepton_tree.motherID(); }
const int &mc_id() { return lepton_tree.mc_id(); }
const float &RelIso03() { return lepton_tree.RelIso03(); }
const float &RelIso03EA() { return lepton_tree.RelIso03EA(); }
const float &RelIso03DB() { return lepton_tree.RelIso03DB(); }
const float &tkIso() { return lepton_tree.tkIso(); }
const bool &passes_VVV_cutbased_veto() { return lepton_tree.passes_VVV_cutbased_veto(); }
const bool &passes_VVV_cutbased_veto_noiso() { return lepton_tree.passes_VVV_cutbased_veto_noiso(); }
const bool &passes_VVV_cutbased_veto_noiso_noip() { return lepton_tree.passes_VVV_cutbased_veto_noiso_noip(); }
const bool &passes_VVV_cutbased_fo() { return lepton_tree.passes_VVV_cutbased_fo(); }
const bool &passes_VVV_cutbased_fo_noiso() { return lepton_tree.passes_VVV_cutbased_fo_noiso(); }
const bool &passes_VVV_cutbased_tight_noiso() { return lepton_tree.passes_VVV_cutbased_tight_noiso(); }
const bool &passes_VVV_cutbased_tight() { return lepton_tree.passes_VVV_cutbased_tight(); }
const bool &passes_VVV_MVAbased_tight_noiso() { return lepton_tree.passes_VVV_MVAbased_tight_noiso(); }
const bool &passes_VVV_MVAbased_tight() { return lepton_tree.passes_VVV_MVAbased_tight(); }
const bool &passes_VVV_baseline() { return lepton_tree.passes_VVV_baseline(); }
const float &ip3d() { return lepton_tree.ip3d(); }
const float &ip3derr() { return lepton_tree.ip3derr(); }
const float &miniiso() { return lepton_tree.miniiso(); }
const float &miniisoDB() { return lepton_tree.miniisoDB(); }
const float &reliso04() { return lepton_tree.reliso04(); }
const float &annulus04() { return lepton_tree.annulus04(); }
const float &AbsTrkIso() { return lepton_tree.AbsTrkIso(); }
const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &jet_close_lep() { return lepton_tree.jet_close_lep(); }
const float &jet_close_lep_undoJEC() { return lepton_tree.jet_close_lep_undoJEC(); }
const float &jet_close_L1() { return lepton_tree.jet_close_L1(); }
const float &jet_close_L2L3() { return lepton_tree.jet_close_L2L3(); }
const float &ptratio() { return lepton_tree.ptratio(); }
const float &dilep_mass() { return lepton_tree.dilep_mass(); }
const bool &isRandom() { return lepton_tree.isRandom(); }
const float &sigmaIEtaIEta_full5x5() { return lepton_tree.sigmaIEtaIEta_full5x5(); }
const float &etaSC() { return lepton_tree.etaSC(); }
const float &dEtaIn() { return lepton_tree.dEtaIn(); }
const float &dPhiIn() { return lepton_tree.dPhiIn(); }
const float &hOverE() { return lepton_tree.hOverE(); }
const float &scSeedEta() { return lepton_tree.scSeedEta(); }
const float &ecalEnergy() { return lepton_tree.ecalEnergy(); }
const bool &conv_vtx_flag() { return lepton_tree.conv_vtx_flag(); }
const int &charge() { return lepton_tree.charge(); }
const float &mva_25ns() { return lepton_tree.mva_25ns(); }
const float &tag_mva_25ns() { return lepton_tree.tag_mva_25ns(); }
const float &ecalPFClusterIso() { return lepton_tree.ecalPFClusterIso(); }
const float &hcalPFClusterIso() { return lepton_tree.hcalPFClusterIso(); }
const int &HLT_Mu8() { return lepton_tree.HLT_Mu8(); }
const int &HLT_Mu17() { return lepton_tree.HLT_Mu17(); }
const int &HLT_Mu8_TrkIsoVVL() { return lepton_tree.HLT_Mu8_TrkIsoVVL(); }
const int &HLT_Mu17_TrkIsoVVL() { return lepton_tree.HLT_Mu17_TrkIsoVVL(); }
const int &HLT_IsoMu24() { return lepton_tree.HLT_IsoMu24(); }
const int &HLT_IsoTkMu24() { return lepton_tree.HLT_IsoTkMu24(); }
const int &HLT_Ele8_CaloIdM_TrackIdM_PFJet30() { return lepton_tree.HLT_Ele8_CaloIdM_TrackIdM_PFJet30(); }
const int &HLT_Ele17_CaloIdM_TrackIdM_PFJet30() { return lepton_tree.HLT_Ele17_CaloIdM_TrackIdM_PFJet30(); }
const int &HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30() { return lepton_tree.HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30(); }
const int &HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30() { return lepton_tree.HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30(); }

}
