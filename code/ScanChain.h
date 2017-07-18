// ROOT
#include "Math/VectorUtil.h"

// SNT
#include "tasutil.cc"
#include "LeptonTree.cc"
#include "pu_weights.h"

using namespace std;
using namespace tas;

float computePtRel( LorentzVector lepp4, LorentzVector jetp4, bool subtractLep );
double calculateMt( const LorentzVector p4, double met, double met_phi );
float getPt( float pt, bool extrPtRel = false );
float getEta( float eta, float ht, bool extrPtRel = false );
bool passIsolatedFO( int id, float eta, float disc, float pt );
float mvacut( float A, float B, float C, float pt_ );
void fill( TasUtil::AutoHist& hists, TString suffix, float& evt_mt, float& evt_met, float& weight);
void fillFakeRateHistograms( TasUtil::AutoHist& hists, TString label, float& evt_met, float& evt_mt, float& weight );
