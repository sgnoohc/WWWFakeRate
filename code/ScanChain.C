//  .
// ..: P. Chang, philip@physics.ucsd.edu

#include "ScanChain.h"

int nptbins = 9;
double ptbins[10] = {10., 15., 20., 25., 30., 35., 50., 70., 120., 170.};

int nptbins_coarse = 6;
double ptbins_coarse[7] = {10., 15., 20., 25., 30., 35., 70.};

int netabins = 3;
double etabins_mu[4] = {0., 1.2, 2.1, 2.4};
double etabins_el[4] = {0., 0.8, 1.479, 2.5};

bool isData = false;

int evt_event_to_print = 94151444;

// Rebuilding event level variables

int prev_evt_event = 0;
std::vector<Lepton> leptons;

//========================================================================================
//
//
// Main Looper
//
//
//========================================================================================
//________________________________________________________________________________________
void ScanChain( TChain* chain, TString outputname, int nEvents = -1 )
{

    //------------------------------------------------------------------------------------
    // Identifying which sample I am running on based on the output name.
    // Generally not a good idea to have the output name dictate the behavior of the code.
    // But for simplicity I am treating the "outputname" as "options".
    //------------------------------------------------------------------------------------
    isData = outputname.Contains( "2016" )
        || outputname.Contains( "Double" ); // made it global so i can use in other functions. (Not pretty though.)
    bool isDoubleMuon = outputname.Contains( "DoubleMuon" ) || outputname.Contains( "doublemuon" );
    bool isEWK = ( outputname.Contains( "WJets" ) || outputname.Contains( "DY" ) || outputname.Contains( "TTbar_" ) );
    bool isQCD = outputname.Contains( "QCD" ) || outputname.Contains( "TTbarFake" );
    bool noMCMatch = ( isData || isEWK );

    //------------------------------------------------------------------------------------
    // Constants in the analysis
    //------------------------------------------------------------------------------------
    float lumi = 35.87;
    // Hardcoded prescale values for the trigger.
    // Something went wrong with prescales for data.
    float e8i = 4682;
    float e17i = 604;
    float m8i = 3464;
    float m17i = 170;

    //------------------------------------------------------------------------------------
    // Histograms in the analysis
    //------------------------------------------------------------------------------------
    TasUtil::AutoHist hists;

    //------------------------------------------------------------------------------------
    //
    // Main iLoop
    //
    //------------------------------------------------------------------------------------
    TasUtil::Looper<LeptonTree> looper( chain, &lepton_tree, nEvents );

    while ( looper.nextEvent() )
    {

        //--------------------------------
        // Prescale calculation via Z-peak
        //--------------------------------

        if ( passes_VVV_cutbased_tight() && p4().pt() > 25. && tag_p4().pt() > 30. )
        {
            float wgt = isData ? 1 : scale1fb() * lumi;

            if ( abs( id() ) == 11 )
            {
                if ( HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30() > 0 )
                {
                    hists.fill( dilep_mass(), "mll_e8i", wgt, 80, 0, 200 );
                }

                if ( HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30() > 0 )
                {
                    hists.fill( dilep_mass(), "mll_e17i", wgt, 80, 0, 200 );
                }
            }
            else if ( abs( id() ) == 13 )
            {
                if ( HLT_Mu8_TrkIsoVVL() > 0 )
                    hists.fill( dilep_mass(), "mll_m8i", wgt, 80, 0, 200 );

                if ( HLT_Mu17_TrkIsoVVL() > 0 )
                {
                    hists.fill( dilep_mass(), "mll_m17i", wgt, 80, 0, 200 );
                }
            }
        }

        //----------------------
        // Compute jet variables
        //----------------------
        int njets40 = 0;

        for ( unsigned int i = 0; i < jets().size(); i++ )
        {
            if ( ROOT::Math::VectorUtil::DeltaR( jets()[i], p4() ) < 1. )
                continue;

            if ( jets()[i].pt() > 40. && fabs( jets()[i].eta() ) < 2.4 )
                njets40++;
        }

        //--------------------------
        // Compute trigger variables
        //--------------------------

        // Compute prescales for QCD and Data.
        // QCD will take prescale from the LeptonTree directly.
        // For data, the prescale values were messed up.
        // Therefore it will use hardcoded values.
        int prescale = 0;
        bool pass_trig = 0; // logical AND with whether it matched

        if ( abs( id() ) == 11 )
        {
            // Check the trigger for our isolated single lepton trigger fired.
            // The trigger has positive value for lepton leg matched.
            // The HLT_* branch value will be negative for leg not matched.
            // (See setHLTBranch() function in CORE for more info.)
            if ( p4().pt() >= 10 && p4().pt() < 25 )
            {
                prescale = e8i;
                pass_trig = true;

                if ( HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30() <= 0 )
                {
                    hists.fill( __COUNTER__, "counter",  1.,  20, 0, 20 );
                    pass_trig = false;
                }
            }
            else if ( p4().pt() >= 25 )
            {
                prescale = e17i;
                pass_trig = true;

                if ( HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30() <= 0 )
                {
                    hists.fill( __COUNTER__, "counter",  1.,  20, 0, 20 );
                    pass_trig = false;
                }
            }
        }
        else if ( abs( id() ) == 13 )
        {
            // Check the trigger for our isolated single lepton trigger fired.
            // The trigger has positive value for lepton leg matched.
            // The HLT_* branch value will be negative for leg not matched.
            // (See setHLTBranch() function in CORE for more info.)
            if ( p4().pt() >= 10 && p4().pt() < 25 )
            {
                prescale = m8i;
                pass_trig = true;

                if ( HLT_Mu8_TrkIsoVVL() <= 0 )
                {
                    hists.fill( __COUNTER__, "counter",  1.,  20, 0, 20 );
                    pass_trig = false;
                }
            }
            else if ( p4().pt() >= 25 )
            {
                prescale = m17i;
                pass_trig = true;

                if ( HLT_Mu17_TrkIsoVVL() <= 0 )
                {
                    hists.fill( __COUNTER__, "counter",  1.,  20, 0, 20 );
                    pass_trig = false;
                }
            }
        }

        //---------------------
        // Compute event weight
        //---------------------
        float weight = isData ? prescale : scale1fb() * lumi * getTruePUw_Moriond( nvtx() );

        //-------------------
        // compute MT and MET
        //-------------------
        float evt_met = evt_corrMET();
        float evt_metPhi = evt_corrMETPhi();
        float evt_mt = calculateMt( p4(), evt_met, evt_metPhi );

        //------------------------------------------------------------
        // Lepton ID selection (most important selection in fake rate)
        //------------------------------------------------------------
        bool passId = passes_VVV_cutbased_tight();
        bool passFO = passes_VVV_cutbased_fo_noiso();

        float relIso = RelIso03EA();

        if ( abs( id() ) == 11 )
            passFO = passFO && ( relIso < 0.2 ) && ( exp_innerlayers() == 0 );
        else if ( abs( id() ) == 13 )
            passFO = passFO && ( relIso < 0.4 );

        // If electron, there are some trigger cuts necessary
        // to stay within the trigger online lepton ID
        if ( abs( id() ) == 11 )
        {
            bool isEB = fabs( etaSC() ) > 1.479 ? false : true;
            float sIeIe = sigmaIEtaIEta_full5x5();
            float hoe = hOverE();
            float deta = fabs( dEtaIn() );
            float dphi = fabs( dPhiIn() );
            float invep = fabs( 1. / ecalEnergy() - 1. / p4().P() );
            float cut_sIeIe = isEB ? 0.011 : 0.031;
            float cut_hoe   = 0.08;
            float cut_deta  = 0.01;
            float cut_dphi  = isEB ? 0.04 : 0.08;
            float cut_invep = 0.01;
            bool passHltCuts = ( sIeIe < cut_sIeIe && hoe < cut_hoe && deta < cut_deta
                    && dphi < cut_dphi && invep < cut_invep );

            if ( !passIsolatedFO( id(), etaSC(), mva_25ns(), p4().pt() ) )
            {
                hists.fill( __COUNTER__, "counter",  1.,  20, 0, 20 );
                continue;
            }

            float ePFIso = ecalPFClusterIso() / p4().pt();
            float hPFIso = hcalPFClusterIso() / p4().pt();
            float trkIso = tkIso() / p4().pt();
            float cut_ePFIso = 0.45;
            float cut_hPFIso = 0.25;
            float cut_trkIso  = 0.2;
            passHltCuts = passHltCuts && ePFIso < cut_ePFIso && hPFIso < cut_hPFIso
                && trkIso < cut_trkIso;
            passFO = passHltCuts && passFO;
        }

        //---------------------------------------------------------
        // Compute cone correction variable for VVV tight isolation
        //---------------------------------------------------------
        float coneptcorr = 0;

        if ( abs( id() ) == 11 )
        {
            if ( abs( etaSC() ) <= 1.479 )
                coneptcorr = std::max( 0., relIso - 0.0588 );
            else
                coneptcorr = std::max( 0., relIso - 0.0571 );
        }

        if ( abs( id() ) == 13 )
            coneptcorr = std::max( 0., relIso - 0.06 );

        // If the lepton passed the tight ID no correction is needed.
        if ( passId )
            coneptcorr = 0;

        //==========================================================================================
        //
        //
        // Done computing variables
        //
        //
        //==========================================================================================

        //--------------------------
        // Instantiate Lepton object
        //--------------------------
        Lepton lepton;
        lepton.p4 = p4();
        lepton.njets40 = njets40;
        lepton.prescale = prescale;
        lepton.pass_trig = pass_trig;
        lepton.weight = weight;
        lepton.evt_met = evt_met;
        lepton.evt_metPhi = evt_metPhi;
        lepton.evt_mt = evt_mt;
        lepton.passId = passId;
        lepton.passFO = passFO;
        lepton.coneptcorr = coneptcorr;

        //-------------------------------
        // Debug print relevant variables
        //-------------------------------

        if ( lepton_tree.evt_event() == evt_event_to_print )
        {
            lepton.print();
        }

        //--------------------------------------------------------------------
        // Rebuiliding data at the per event level instead of per lepton level
        //--------------------------------------------------------------------
        if ( prev_evt_event != lepton_tree.evt_event() )
        {
            // If it is a new event, it means it's time to take care of previous event data
            // Do something with it.
            fillEventLevelHistograms( lepton );

            // After doing something with it, clear it out.
            leptons.clear();
        }

        // add the lepton to the leptons vector
        leptons.push_back( lepton );

        //==========================================================================================
        //
        //
        // Event selections
        //
        //
        //==========================================================================================

        //----------------------
        // Event level selection
        //----------------------
        // if prompt lepton with QCD sample reject.
        if ( isQCD && motherID() > 0 )
        {
            hists.fill( __COUNTER__, "counter",  1.,  20, 0, 20 );
            continue;
        }

        //-----------------------------------
        // Skip events with trigger not fired
        //-----------------------------------
        if ( !pass_trig ) continue;

        //-----------------------------------------------------------------------------------------------
        // Skip events in data when the candidate we're looking at is not from the dataset that we expect
        //-----------------------------------------------------------------------------------------------
        if ( isData )
        {
            if ( isDoubleMuon && abs( id() ) != 13 )
            {
                hists.fill( __COUNTER__, "counter",  1.,  20, 0, 20 );
                continue;
            }

            if ( !isDoubleMuon && abs( id() ) != 11 )
            {
                hists.fill( __COUNTER__, "counter",  1.,  20, 0, 20 );
                continue;
            }
        }

        //-----------------------------------------------
        // Skip events without at least one jet of 40 GeV
        //-----------------------------------------------
        if ( !( njets40 > 0 ) )
        {
            hists.fill( __COUNTER__, "counter",  1.,  20, 0, 20 );
            continue;
        }

        //----------------------------------
        // Skip events with more than one FO
        //----------------------------------
        if ( nFOs_VVV() > 1 )
        {
            hists.fill( __COUNTER__, "counter",  1.,  20, 0, 20 );
            continue;
        }

        //----------------------------------
        // Kinematic selection on the lepton
        //----------------------------------
        if ( p4().pt() < 10. )
        {
            hists.fill( __COUNTER__, "counter",  1.,  20, 0, 20 );
            continue;
        }

        if ( abs( id() ) == 11 && fabs( p4().eta() ) > 2.5 )
        {
            hists.fill( __COUNTER__, "counter",  1.,  20, 0, 20 );
            continue;
        }

        if ( abs( id() ) == 13 && fabs( p4().eta() ) > 2.4 )
        {
            hists.fill( __COUNTER__, "counter",  1.,  20, 0, 20 );
            continue;
        }

        hists.fill( njets40, "njets40", weight, 5, 0, 5 );

        //==========================================================================================
        //
        //
        // fill histograms
        //
        //
        //==========================================================================================
        
        //-----------------------------------------
        // Fill the main fake rate study histograms
        //-----------------------------------------
        if ( passId )
            fillFakeRateHistograms( hists, "", evt_met, evt_mt, weight );

        if ( passFO )
            fillFakeRateHistograms( hists, "loose_", evt_met, evt_mt, weight );

        //        if ( passFO && !passId )
        //            fillFakeRateHistograms( hists, "lbnt_", evt_met, evt_mt, weight );

        if ( !( evt_met < 20. && evt_mt < 20 ) )
        {
            hists.fill( __COUNTER__, "counter",  1.,  20, 0, 20 );
            continue;
        }

        //-------------------
        // Verifying prescale
        //-------------------
        if ( abs( id() ) == 11 )
        {
            hists.fill( p4().pt(), "histo_pt_el", weight, 20000, 0, 200 );
            hists.fill( p4().pt(), "histo_nowgt_pt_el",
                    isData ? 1 : scale1fb() * lumi * getTruePUw_Moriond( nvtx() )
                    , 20000, 0, 200 );
        }
        else if ( abs( id() ) == 13 )
        {
            hists.fill( p4().pt(), "histo_pt_mu", weight, 20000, 0, 200 );
            hists.fill( p4().pt(), "histo_nowgt_pt_mu",
                    isData ? 1 : scale1fb() * lumi * getTruePUw_Moriond( nvtx() )
                    , 20000, 0, 200 );
        }

        // For MC, motherID's can have various values.
        // For data, it's always -1.
        // For MC, we only do matching for QCD sample.
        // For W/Z samples, noMCMatch is true. (See above)
        // For data, we accept all events.
        if ( noMCMatch || motherID() <= 0 )
        {

            if ( passId )
            {
                hists.fill( 0, "Nt", weight, 1, 0, 1 );

                if ( abs( id() ) == 11 )
                {
                    hists.fill( 0, "Nt_e", weight, 1, 0, 1 );
                    hists.fill( p4().pt(), abs( p4().eta() ), "Nt_histo_e",
                            weight, nptbins, ptbins, netabins, etabins_el );
                }

                if ( abs( id() ) == 13 )
                {
                    hists.fill( 0, "Nt_mu", weight, 1, 0, 1 );
                    hists.fill( p4().pt(), abs( p4().eta() ), "Nt_histo_mu",
                            weight, nptbins, ptbins, netabins, etabins_mu );
                }
            }

            if ( passFO )
            {
                hists.fill( 0, "Nl", weight, 1, 0, 1 );

                if ( abs( id() ) == 11 )
                {
                    hists.fill( 0, "Nl_e", weight, 1, 0, 1 );
                    hists.fill( p4().pt(), abs( p4().eta() ), "Nl_histo_e",
                            weight, nptbins, ptbins, netabins, etabins_el );
                    hists.fill( p4().pt() * ( 1 + coneptcorr ), abs( p4().eta() ),
                            "Nl_cone_histo_e", weight, nptbins, ptbins, netabins, etabins_el );
                }

                if ( abs( id() ) == 13 )
                {
                    hists.fill( 0, "Nl_mu", weight, 1, 0, 1 );
                    hists.fill( p4().pt(), abs( p4().eta() ), "Nl_histo_mu",
                            weight, nptbins, ptbins, netabins, etabins_mu );
                    hists.fill( p4().pt() * ( 1 + coneptcorr ), abs( p4().eta() ),
                            "Nl_cone_histo_mu", weight, nptbins, ptbins, netabins, etabins_mu );
                }
            }

        }


    }

    TH1F* h_Nt    = ( TH1F* ) hists.get( "Nt" );
    TH1F* h_Nl    = ( TH1F* ) hists.get( "Nl" );
    TH1F* h_Nt_e  = ( TH1F* ) hists.get( "Nt_e" );
    TH1F* h_Nl_e  = ( TH1F* ) hists.get( "Nl_e" );
    TH1F* h_Nt_mu = ( TH1F* ) hists.get( "Nt_mu" );
    TH1F* h_Nl_mu = ( TH1F* ) hists.get( "Nl_mu" );

    float Nt    = h_Nt    ? h_Nt    -> GetBinContent( 1 ) : 0.;
    float Nl    = h_Nl    ? h_Nl    -> GetBinContent( 1 ) : 0.;
    float Nt_e  = h_Nt_e  ? h_Nt_e  -> GetBinContent( 1 ) : 0.;
    float Nl_e  = h_Nl_e  ? h_Nl_e  -> GetBinContent( 1 ) : 0.;
    float Nt_mu = h_Nt_mu ? h_Nt_mu -> GetBinContent( 1 ) : 0.;
    float Nl_mu = h_Nl_mu ? h_Nl_mu -> GetBinContent( 1 ) : 0.;

    float e = Nt / ( Nl );
    float e_e = Nt_e / ( Nl_e );
    float e_mu = Nt_mu / ( Nl_mu );

    cout << "\nReco: " << "Nt = " << Nt << ", Nl = " << Nl << ", e = " << e << endl;
    cout << "\nReco (el): " << "Nt = " << Nt_e << ", Nl = " << Nl_e << ", e = " << e_e << endl;
    cout << "\nReco (mu): " << "Nt = " << Nt_mu << ", Nl = " << Nl_mu << ", e = " << e_mu <<
        endl;

    hists.save( outputname );
}

//________________________________________________________________________________________
void fillEventLevelHistograms( Lepton& lepton )
{
}

//________________________________________________________________________________________
float computePtRel( LorentzVector lepp4, LorentzVector jetp4, bool subtractLep )
{
    if ( jetp4.pt() == 0 )
        return 0.;

    if ( subtractLep )
        jetp4 -= lepp4;

    float dot = lepp4.Vect().Dot( jetp4.Vect() );
    float ptrel = lepp4.P2() - dot * dot / jetp4.P2();
    ptrel = ptrel > 0 ? sqrt( ptrel ) : 0.0;
    return ptrel;
}

//________________________________________________________________________________________
double calculateMt( const LorentzVector p4, double met, double met_phi )
{
    float phi1 = p4.Phi();
    float phi2 = met_phi;
    float Et1  = p4.Et();
    float Et2  = met;
    return sqrt( 2 * Et1 * Et2 * ( 1.0 - cos( phi1 - phi2 ) ) );
}

//________________________________________________________________________________________
float getPt( float pt, bool extrPtRel )
{
    if ( !extrPtRel && pt >= 70. )
        return 69.;

    if ( extrPtRel && pt >= 150. )
        return 149.;

    if ( pt < 10. )
    {
        return 11.;    //use this if lower FR histo bound is 10.
    }

    return pt;
}

//________________________________________________________________________________________
float getEta( float eta, float ht, bool extrPtRel )
{
    if ( extrPtRel )
    {
        if ( ht >= 800 )
            return 799;

        return ht;
    }

    if ( fabs( eta ) >= 2.4 )
        return 2.3;

    return fabs( eta );
}

//________________________________________________________________________________________
bool passIsolatedFO( int id, float eta, float disc, float pt )
{
    if ( abs( id ) == 13 )
        return true;

    float aeta = fabs( eta );

    // if (aeta < 0.8) return mva > -0.155;
    // if ((aeta >= 0.8 && aeta <= 1.479)) return mva > -0.56;
    // if (aeta > 1.479) return mva > -0.76;
    if ( aeta < 0.8 )
        return disc > mvacut( -0.86, -0.96, -0.3, pt );

    if ( ( aeta >= 0.8 && aeta <= 1.479 ) )
        return disc > mvacut( -0.85, -0.96, -0.36, pt );

    if ( aeta > 1.479 )
        return disc > mvacut( -0.81, -0.95, -0.63, pt );

    return false;
}

//________________________________________________________________________________________
// returns A if pt<ptmin, B if pt>ptmax, and linear interpolation between. if pt<10, use C
float mvacut( float A, float B, float C, float pt_ )
{
    float ptmin = 15;
    float ptmax = 25;
    return pt_ > 10 ? std::min( A, std::max( B,
                A + ( B - A ) / ( ptmax - ptmin ) * ( pt_ - ptmin ) ) ) : C;
}

//________________________________________________________________________________________
void fill( TasUtil::AutoHist& hists, TString suffix, float& evt_mt, float& evt_met, float& weight )
{
    float coneptcorr = 0;
    float relIso = RelIso03EA();

    if ( abs( id() ) == 11 )
    {
        if ( abs( etaSC() ) <= 1.479 )
            coneptcorr = std::max( 0., relIso - 0.0588 );
        else
            coneptcorr = std::max( 0., relIso - 0.0571 );
    }

    if ( abs( id() ) == 13 )
        coneptcorr = std::max( 0., relIso - 0.06 );

    hists.fill( evt_mt, "histo_mt_" + suffix, weight, 20, 0, 200 );
    hists.fill( evt_met, "histo_met_" + suffix, weight, 20, 0, 200 );

    if ( abs( id() ) == 11 )
    {
        hists.fill( nvtx(), "histo_nvtx_" + suffix + "_el", weight, 50, 0, 50 );
        hists.fill( evt_mt, "histo_mt_" + suffix + "_el", weight, 20, 0, 200 );
        hists.fill( evt_met, "histo_met_" + suffix + "_el", weight, 20, 0, 200 );
        hists.fill( p4().pt(), "histo_ptvarbin_" + suffix + "_el", weight, nptbins, ptbins );
        hists.fill( p4().pt(), "histo_ptvarbin_coarse_" + suffix + "_el", weight, nptbins_coarse, ptbins_coarse );
        hists.fill( p4().pt() * ( 1 + coneptcorr ), "histo_conecorrptvarbin_" + suffix + "_el", weight, nptbins, ptbins );
        hists.fill( p4().eta(), "histo_etavarbin_" + suffix + "_el", weight, netabins, etabins_el );
        hists.fill( p4().pt(), p4().eta(), "histo_ptvarbin_etavarbin_" + suffix + "_el", weight, nptbins, ptbins, netabins,
                etabins_el );
        hists.fill( p4().pt() * ( 1 + coneptcorr ), p4().eta(), "histo_conecorrptvarbin_etavarbin_" + suffix + "_el", weight,
                nptbins, ptbins, netabins, etabins_el );
        hists.fill( p4().pt(), "histo_pt_" + suffix + "_el", weight, 40, 0, 200 );
        hists.fill( p4().eta(), "histo_eta_" + suffix + "_el", weight, 20, -3, 3 );
        hists.fill( p4().phi(), "histo_phi_" + suffix + "_el", weight, 20, -3.15, 3.15 );
    }
    else if ( abs( id() ) == 13 )
    {
        hists.fill( nvtx(), "histo_nvtx_" + suffix + "_mu", weight, 50, 0, 50 );
        hists.fill( evt_mt, "histo_mt_" + suffix + "_mu", weight, 20, 0, 200 );
        hists.fill( evt_met, "histo_met_" + suffix + "_mu", weight, 20, 0, 200 );
        hists.fill( p4().pt(), "histo_ptvarbin_" + suffix + "_mu", weight, nptbins, ptbins );
        hists.fill( p4().pt(), "histo_ptvarbin_coarse_" + suffix + "_mu", weight, nptbins_coarse, ptbins_coarse );
        hists.fill( p4().pt() * ( 1 + coneptcorr ), "histo_conecorrptvarbin_" + suffix + "_mu", weight, nptbins, ptbins );
        hists.fill( p4().eta(), "histo_etavarbin_" + suffix + "_mu", weight, netabins, etabins_mu );
        hists.fill( p4().pt(), p4().eta(), "histo_ptvarbin_etavarbin_" + suffix + "_mu", weight, nptbins, ptbins, netabins,
                etabins_mu );
        hists.fill( p4().pt() * ( 1 + coneptcorr ), p4().eta(), "histo_conecorrptvarbin_etavarbin_" + suffix + "_mu", weight,
                nptbins, ptbins, netabins, etabins_mu );
        hists.fill( p4().pt(), "histo_pt_" + suffix + "_mu", weight, 40, 0, 200 );
        hists.fill( p4().eta(), "histo_eta_" + suffix + "_mu", weight, 20, -3, 3 );
        hists.fill( p4().phi(), "histo_phi_" + suffix + "_mu", weight, 20, -3.15, 3.15 );

        //        float nvtxrewgt = isData ? 1. : nvtxRewgt( nvtx() );
        //
        //        hists.fill( nvtx(), "histo_nvtxrewgt_nvtx_" + suffix + "_mu", weight * nvtxrewgt, 50, 0, 50 );
        //        hists.fill( evt_mt, "histo_nvtxrewgt_mt_" + suffix + "_mu", weight * nvtxrewgt, 20, 0, 200 );
        //        hists.fill( evt_met, "histo_nvtxrewgt_met_" + suffix + "_mu", weight * nvtxrewgt, 20, 0, 200 );
        //        hists.fill( p4().pt(), "histo_nvtxrewgt_ptvarbin_" + suffix + "_mu", weight * nvtxrewgt, nptbins, ptbins );
        //        hists.fill( p4().pt(), "histo_nvtxrewgt_ptvarbin_coarse_" + suffix + "_mu", weight * nvtxrewgt, nptbins_coarse,
        //                    ptbins_coarse );
        //        hists.fill( p4().pt() * ( 1 + coneptcorr ), "histo_nvtxrewgt_conecorrptvarbin_" + suffix + "_mu", weight * nvtxrewgt,
        //                    nptbins, ptbins );
        //        hists.fill( p4().eta(), "histo_nvtxrewgt_etavarbin_" + suffix + "_mu", weight * nvtxrewgt, netabins, etabins_mu );
        //        hists.fill( p4().pt(), p4().eta(), "histo_nvtxrewgt_ptvarbin_etavarbin_" + suffix + "_mu", weight * nvtxrewgt, nptbins,
        //                    ptbins, netabins,
        //                    etabins_mu );
        //        hists.fill( p4().pt() * ( 1 + coneptcorr ), p4().eta(), "histo_nvtxrewgt_conecorrptvarbin_etavarbin_" + suffix + "_mu",
        //                    weight * nvtxrewgt,
        //                    nptbins, ptbins, netabins, etabins_mu );
        //        hists.fill( p4().pt(), "histo_nvtxrewgt_pt_" + suffix + "_mu", weight * nvtxrewgt, 40, 0, 200 );
        //        hists.fill( p4().eta(), "histo_nvtxrewgt_eta_" + suffix + "_mu", weight * nvtxrewgt, 20, -3, 3 );
        //        hists.fill( p4().phi(), "histo_nvtxrewgt_phi_" + suffix + "_mu", weight * nvtxrewgt, 20, -3.15, 3.15 );
    }

    //    if ( abs( id() ) == 11 )
    //    {
    //        if ( p4().pt() < 10. )
    //            return;
    //        else if ( p4().pt() < 15. )
    //        {
    //            hists.fill( evt_mt, "histo_pt15_mt_" + suffix + "_el", weight, 20, 0, 200 );
    //            hists.fill( evt_met, "histo_pt15_met_" + suffix + "_el", weight, 20, 0, 200 );
    //        }
    //        else if ( p4().pt() < 20. )
    //        {
    //            hists.fill( evt_mt, "histo_pt20_mt_" + suffix + "_el", weight, 20, 0, 200 );
    //            hists.fill( evt_met, "histo_pt20_met_" + suffix + "_el", weight, 20, 0, 200 );
    //        }
    //        else if ( p4().pt() < 25. )
    //        {
    //            hists.fill( evt_mt, "histo_pt25_mt_" + suffix + "_el", weight, 20, 0, 200 );
    //            hists.fill( evt_met, "histo_pt25_met_" + suffix + "_el", weight, 20, 0, 200 );
    //        }
    //        else if ( p4().pt() < 30. )
    //        {
    //            hists.fill( evt_mt, "histo_pt30_mt_" + suffix + "_el", weight, 20, 0, 200 );
    //            hists.fill( evt_met, "histo_pt30_met_" + suffix + "_el", weight, 20, 0, 200 );
    //        }
    //        else if ( p4().pt() < 35. )
    //        {
    //            hists.fill( evt_mt, "histo_pt35_mt_" + suffix + "_el", weight, 20, 0, 200 );
    //            hists.fill( evt_met, "histo_pt35_met_" + suffix + "_el", weight, 20, 0, 200 );
    //        }
    //        else if ( p4().pt() < 50. )
    //        {
    //            hists.fill( evt_mt, "histo_pt50_mt_" + suffix + "_el", weight, 20, 0, 200 );
    //            hists.fill( evt_met, "histo_pt50_met_" + suffix + "_el", weight, 20, 0, 200 );
    //        }
    //        else if ( p4().pt() < 70. )
    //        {
    //            hists.fill( evt_mt, "histo_pt70_mt_" + suffix + "_el", weight, 20, 0, 200 );
    //            hists.fill( evt_met, "histo_pt70_met_" + suffix + "_el", weight, 20, 0, 200 );
    //        }
    //    }
    //    else if ( abs( id() ) == 13 )
    //    {
    //        if ( p4().pt() < 10. )
    //            return;
    //        else if ( p4().pt() < 15. )
    //        {
    //            hists.fill( evt_mt, "histo_pt15_mt_" + suffix + "_mu", weight, 20, 0, 200 );
    //            hists.fill( evt_met, "histo_pt15_met_" + suffix + "_mu", weight, 20, 0, 200 );
    //        }
    //        else if ( p4().pt() < 20. )
    //        {
    //            hists.fill( evt_mt, "histo_pt20_mt_" + suffix + "_mu", weight, 20, 0, 200 );
    //            hists.fill( evt_met, "histo_pt20_met_" + suffix + "_mu", weight, 20, 0, 200 );
    //        }
    //        else if ( p4().pt() < 25. )
    //        {
    //            hists.fill( evt_mt, "histo_pt25_mt_" + suffix + "_mu", weight, 20, 0, 200 );
    //            hists.fill( evt_met, "histo_pt25_met_" + suffix + "_mu", weight, 20, 0, 200 );
    //        }
    //        else if ( p4().pt() < 30. )
    //        {
    //            hists.fill( evt_mt, "histo_pt30_mt_" + suffix + "_mu", weight, 20, 0, 200 );
    //            hists.fill( evt_met, "histo_pt30_met_" + suffix + "_mu", weight, 20, 0, 200 );
    //        }
    //        else if ( p4().pt() < 35. )
    //        {
    //            hists.fill( evt_mt, "histo_pt35_mt_" + suffix + "_mu", weight, 20, 0, 200 );
    //            hists.fill( evt_met, "histo_pt35_met_" + suffix + "_mu", weight, 20, 0, 200 );
    //        }
    //        else if ( p4().pt() < 50. )
    //        {
    //            hists.fill( evt_mt, "histo_pt50_mt_" + suffix + "_mu", weight, 20, 0, 200 );
    //            hists.fill( evt_met, "histo_pt50_met_" + suffix + "_mu", weight, 20, 0, 200 );
    //        }
    //        else if ( p4().pt() < 70. )
    //        {
    //            hists.fill( evt_mt, "histo_pt70_mt_" + suffix + "_mu", weight, 20, 0, 200 );
    //            hists.fill( evt_met, "histo_pt70_met_" + suffix + "_mu", weight, 20, 0, 200 );
    //        }
    //    }



}

//________________________________________________________________________________________
void fillFakeRateHistograms( TasUtil::AutoHist& hists, TString label, float& evt_met, float& evt_mt, float& weight )
{
    //mt control region
    if ( evt_met > 30. && evt_mt > 80. && evt_mt < 120. )
        fill( hists, label + "ewk_cr", evt_mt, evt_met, weight );

    //    if ( evt_met < 30. && evt_mt > 80. && evt_mt < 120. )
    //        fill( hists, label + "lowmet_ewk_cr", evt_mt, evt_met, weight );
    //
    //    if ( evt_met > 30. && p4().pt() > 50 )
    //        fill( hists, label + "highpt_cr", evt_mt, evt_met, weight );
    //
    //    if ( evt_met > 30. && p4().pt() <= 50 && p4().pt() > 30 )
    //        fill( hists, label + "medpt_cr", evt_mt, evt_met, weight );
    //
    //    if ( evt_met > 30. && p4().pt() <= 30 )
    //        fill( hists, label + "lowpt_cr", evt_mt, evt_met, weight );
    //
    //    if ( evt_met > 30. )
    //        fill( hists, label + "cr", evt_mt, evt_met, weight );
    //
    //    if ( evt_met > 50. && p4().pt() > 30 )
    //        fill( hists, label + "highpt_tightcr", evt_mt, evt_met, weight );
    //
    //    if ( evt_met > 50. && p4().pt() <= 30 )
    //        fill( hists, label + "lowpt_tightcr", evt_mt, evt_met, weight );
    //
    //    if ( evt_met > 50. )
    //        fill( hists, label + "tightcr", evt_mt, evt_met, weight );
    //
    //    if ( evt_met < 20. )
    //        fill( hists, label + "lm", evt_mt, evt_met, weight );
    //
    //    if ( p4().pt() > 30 )
    //        fill( hists, label + "highpt", evt_mt, evt_met, weight );
    //
    //    if ( p4().pt() > 50 )
    //        fill( hists, label + "highpt50", evt_mt, evt_met, weight );
    //
    //    if ( p4().pt() > 30. &&  p4().pt() <= 50 )
    //        fill( hists, label + "medpt", evt_mt, evt_met, weight );
    //
    //    if ( evt_mt < 20. )
    //        fill( hists, label + "lmt", evt_mt, evt_met, weight );
    //
    //    if ( evt_mt > 30. )
    //        fill( hists, label + "hmt", evt_mt, evt_met, weight );
    //
    //    if ( evt_mt > 50. )
    //        fill( hists, label + "hmt50", evt_mt, evt_met, weight );
    //
    //    if ( evt_mt > 70. )
    //        fill( hists, label + "hmt70", evt_mt, evt_met, weight );
    //
    //    if ( evt_mt > 80. )
    //        fill( hists, label + "hmt80", evt_mt, evt_met, weight );
    //
    //    if ( evt_mt > 120. )
    //        fill( hists, label + "hmt120", evt_mt, evt_met, weight );
    //
    //    if ( evt_mt > 50. && p4().pt() > 30. )
    //        fill( hists, label + "hmt50pt30", evt_mt, evt_met, weight );
    //
    //    if ( evt_mt > 70. && p4().pt() > 30. )
    //        fill( hists, label + "hmt70pt30", evt_mt, evt_met, weight );
    //
    //    if ( evt_mt > 80. && p4().pt() > 30. )
    //        fill( hists, label + "hmt80pt30", evt_mt, evt_met, weight );
    //
    //    if ( evt_mt > 120. && p4().pt() > 30. )
    //        fill( hists, label + "hmt120pt30", evt_mt, evt_met, weight );
    //
    //    if ( evt_mt > 50. && p4().pt() > 50. )
    //        fill( hists, label + "hmt50pt50", evt_mt, evt_met, weight );
    //
    //    if ( evt_mt > 70. && p4().pt() > 50. )
    //        fill( hists, label + "hmt70pt50", evt_mt, evt_met, weight );
    //
    //    if ( evt_mt > 80. && p4().pt() > 50. )
    //        fill( hists, label + "hmt80pt50", evt_mt, evt_met, weight );
    //
    //    if ( evt_mt > 120. && p4().pt() > 50. )
    //        fill( hists, label + "hmt120pt50", evt_mt, evt_met, weight );

    if ( evt_mt < 20. && evt_met < 20 )
        fill( hists, label + "meas", evt_mt, evt_met, weight );
}


//________________________________________________________________________________________
double nvtxRewgt( int nvtx )
{
    if ( nvtx == 0 )
        return  0         ;

    if ( nvtx == 1 )
        return  12.0748   ;

    if ( nvtx == 2 )
        return  8.84592   ;

    if ( nvtx == 3 )
        return  8.70355   ;

    if ( nvtx == 4 )
        return  5.31321   ;

    if ( nvtx == 5 )
        return  3.78796   ;

    if ( nvtx == 6 )
        return  7.01165   ;

    if ( nvtx == 7 )
        return  8.41857   ;

    if ( nvtx == 8 )
        return  4.82683   ;

    if ( nvtx == 9 )
        return  3.58198   ;

    if ( nvtx == 10 )
        return  2.9254    ;

    if ( nvtx == 11 )
        return  2.19347   ;

    if ( nvtx == 12 )
        return  1.81965   ;

    if ( nvtx == 13 )
        return  1.51107   ;

    if ( nvtx == 14 )
        return  1.26134   ;

    if ( nvtx == 15 )
        return  1.07211   ;

    if ( nvtx == 16 )
        return  0.943931  ;

    if ( nvtx == 17 )
        return  0.804781  ;

    if ( nvtx == 18 )
        return  0.749968  ;

    if ( nvtx == 19 )
        return  0.671684  ;

    if ( nvtx == 20 )
        return  0.586063  ;

    if ( nvtx == 21 )
        return  0.543424  ;

    if ( nvtx == 22 )
        return  0.443598  ;

    if ( nvtx == 23 )
        return  0.397247  ;

    if ( nvtx == 24 )
        return  0.333098  ;

    if ( nvtx == 25 )
        return  0.306724  ;

    if ( nvtx == 26 )
        return  0.274128  ;

    if ( nvtx == 27 )
        return  0.230995  ;

    if ( nvtx == 28 )
        return  0.189151  ;

    if ( nvtx == 29 )
        return  0.173443  ;

    if ( nvtx == 30 )
        return  0.13398   ;

    if ( nvtx == 31 )
        return  0.140405  ;

    if ( nvtx == 32 )
        return  0.0990381 ;

    if ( nvtx == 33 )
        return  0.0925015 ;

    if ( nvtx == 34 )
        return  0.0998464 ;

    if ( nvtx == 35 )
        return  0.0981454 ;

    if ( nvtx == 36 )
        return  0.0707519 ;

    if ( nvtx == 37 )
        return  0.0952367 ;

    if ( nvtx >= 38 )
        return  0.112327  ;

    return 0;
}

//        //------------------------------
//        // Rebuild event level variables
//        //------------------------------
//
//        if ( lepton_tree.evt_event() == evt_event_to_print )
//        {
//            std::cout << "=====meas====" << std::endl;
//            std::cout << "printing event info for evt_event = " << lepton_tree.evt_event() << std::endl;
//            std::cout << " passId " <<  passId << std::endl;
//            std::cout << " passFO " <<  passFO << std::endl;
//            std::cout << " pt " <<  p4().pt() << std::endl;
//            std::cout << " nFOs_VVV " <<  nFOs_VVV() << std::endl;
//            std::cout << " nFOs_SS " <<  nFOs_SS() << std::endl;
//            std::cout << " evt_met " <<  evt_met << std::endl;
//            std::cout << " evt_mt " <<  evt_mt << std::endl;
//            std::cout << " prescale " << prescale << std::endl;
//            std::cout << " pass_trig " << pass_trig << std::endl;
//            std::cout << " njets40 " << njets40 << std::endl;
//            std::cout << "=====" << std::endl;
//
//        }
//        
//        // New event!
//        if ( prev_evt_event != lepton_tree.evt_event() )
//        {
//        
//        
////            std::cout << " prev_evt_event " << prev_evt_event << std::endl;
////            std::cout << " local_nFOs_VVV " <<  evt_lvl_var_passFO_p4.size() << std::endl;
////            std::cout << " local_nIds_VVV " <<  evt_lvl_var_passId_p4.size() << std::endl;
////            std::cout << " evt_met " <<  evt_met << std::endl;
////            std::cout << " lepton_tree.evt_event() " << lepton_tree.evt_event() << std::endl;
//
//            // We got a new event.
//            // Deal with what's already in the vectors and
//            // then later push this even'ts variable into the vector.
//            if ( prev_evt_event == evt_event_to_print ) printEvtLvlVar();
//        
//            int local_nFOs_VVV = evt_lvl_var_passFO_p4.size();
//            int local_nIds_VVV = evt_lvl_var_passId_p4.size();
//        
//            bool pass = false;
//            int idx_nlep2 = -1;
//            float pt = -999;
//            float met = -999;
//            float mt = -999;
//            float this_weight = 1;
//        
//            // Now if it's a one lepton bin do the usual
//            if ( local_nIds_VVV == 1 && local_nFOs_VVV == 1 )
//            {
//                if ( prev_evt_event == evt_event_to_print )
//                {
//                    printEvtLvlVar();
//                    std::cout << "diagnose" << std::endl;
//                    std::cout << evt_lvl_var_evt_met[0] << std::endl;
//                    std::cout << evt_lvl_var_evt_mt[0] << std::endl;
//                    std::cout << local_nIds_VVV << std::endl;
//                    std::cout << fabs( evt_lvl_var_passId_id[0] ) << std::endl;
//                    std::cout << fabs( evt_lvl_var_passId_p4[0].eta() ) << std::endl;
//                    std::cout << fabs( evt_lvl_var_passId_p4[0].pt() ) << std::endl;
//                    std::cout << evt_lvl_var_njets[0] << std::endl;
//                    std::cout << evt_lvl_var_pass_trig[0] << std::endl;
//                }
//                // Since I want to study the purity in the tight Id
//                if ( evt_lvl_var_evt_met[0] < 20. && evt_lvl_var_evt_mt[0] < 20. && local_nIds_VVV == 1 && fabs( evt_lvl_var_passId_id[0] ) == 13
//                        && fabs( evt_lvl_var_passId_p4[0].eta() ) < 2.4 && fabs( evt_lvl_var_passId_p4[0].pt() ) > 10. && evt_lvl_var_njets[0] > 0 
//                        && evt_lvl_var_pass_trig[0] == 1 )
//                {
//                    if ( prev_evt_event == evt_event_to_print )
//                        std::cout << "here" << std::endl;
//                    pass = true;
//                    pt = evt_lvl_var_passId_p4[0].pt();
//                    met = evt_lvl_var_evt_met[0];
//                    mt = evt_lvl_var_evt_mt[0];
//                }
//            }
//            // This is what I am trying to compare to
//            else if ( local_nFOs_VVV == 2 )
//            {
//                // First I need to construct dilepton mass
//                float mll = dilep_mass_from_FOs();
//        
//                float Mt0 = calculateMt( evt_lvl_var_passFO_p4[0], evt_lvl_var_evt_met[0], evt_lvl_var_evt_metPhi[0] );
//                float Mt1 = calculateMt( evt_lvl_var_passFO_p4[1], evt_lvl_var_evt_met[1], evt_lvl_var_evt_metPhi[1] );
//                hists.fill( Mt0, "Mt0" ,  this_weight,  50, 0, 150 );
//                hists.fill( Mt1, "Mt1" ,  this_weight,  50, 0, 150 );
//                // Tag W via one closer to MT = 80
//                idx_nlep2 = abs( Mt0 - 80. ) < abs( Mt1 - 80. ) ? 0 : 1;
//                pass = abs( evt_lvl_var_passId_id[idx_nlep2] ) == 13;
//                pt = evt_lvl_var_passId_p4[idx_nlep2].pt();
//                met = evt_lvl_var_evt_met[idx_nlep2];
//                mt = evt_lvl_var_evt_mt[idx_nlep2];
//
////                if ( abs( mll - 91.1876 ) > 15. )
////                {
////                    hists.fill( mll, "mll_nIds_VVV_eq2", this_weight, 80, 0, 200 );
////        
////                    // Now of the two leptons let's tag W
////                    if ( local_nIds_VVV == 2 )
////                    {
////                        float Mt0 = calculateMt( evt_lvl_var_passId_p4[0], evt_lvl_var_evt_met[0], evt_lvl_var_evt_metPhi[0] );
////                        float Mt1 = calculateMt( evt_lvl_var_passId_p4[1], evt_lvl_var_evt_met[1], evt_lvl_var_evt_metPhi[1] );
////                        hists.fill( Mt0, "Mt0" ,  this_weight,  50, 0, 150 );
////                        hists.fill( Mt1, "Mt1" ,  this_weight,  50, 0, 150 );
////                        // Tag W via one closer to MT = 80
////                        idx_nlep2 = abs( Mt0 - 80. ) < abs( Mt1 - 80. ) ? 0 : 1;
////                        pass = abs( evt_lvl_var_passId_id[idx_nlep2] ) == 13;
////                        pt = evt_lvl_var_passId_p4[idx_nlep2].pt();
////                        met = evt_lvl_var_evt_met[idx_nlep2];
////                        mt = evt_lvl_var_evt_mt[idx_nlep2];
////                    }
////                }
//            }
//        
//            if ( pass )
//            {
//                if ( pass && local_nIds_VVV == 1 )
//                {
//                    std::cout << std::endl;
//                    std::cout << " evt " << prev_evt_event << " pt = " << pt << " met = " << met << " mt = " << mt << " wgt = " << evt_lvl_var_weight[0] << std::endl;
//                }
//                hists.fill( local_nIds_VVV, "nIds_VVV",  evt_lvl_var_weight[0],  2, 1, 3 );
//                if ( pt > 170. ) hists.fill( local_nIds_VVV, "nIds_VVV_pt170",  evt_lvl_var_weight[0],  2, 1, 3 );
//                else if ( pt > 120. ) hists.fill( local_nIds_VVV, "nIds_VVV_pt120",  evt_lvl_var_weight[0],  2, 1, 3 );
//                else if ( pt >  70. ) hists.fill( local_nIds_VVV, "nIds_VVV_pt70" ,  evt_lvl_var_weight[0],  2, 1, 3 );
//                else if ( pt >  50. ) hists.fill( local_nIds_VVV, "nIds_VVV_pt50" ,  evt_lvl_var_weight[0],  2, 1, 3 );
//        
//                if ( local_nFOs_VVV == 1 || ( local_nFOs_VVV == 2 && abs( dilep_mass_from_FOs() - 91.1876 ) > 25. ) )
//                {
//                    hists.fill( local_nIds_VVV, "nIds_VVV_ZvetoTight",  this_weight,  2, 1, 3 );
//                    if ( pt > 170. ) hists.fill( local_nIds_VVV, "nIds_VVV_pt170_ZvetoTight",  this_weight,  2, 1, 3 );
//                    else if ( pt > 120. ) hists.fill( local_nIds_VVV, "nIds_VVV_pt120_ZvetoTight",  this_weight,  2, 1, 3 );
//                    else if ( pt >  70. ) hists.fill( local_nIds_VVV, "nIds_VVV_pt70_ZvetoTight" ,  this_weight,  2, 1, 3 );
//                    else if ( pt >  50. ) hists.fill( local_nIds_VVV, "nIds_VVV_pt50_ZvetoTight" ,  this_weight,  2, 1, 3 );
//                }
//            }
//        
//            // Now the previous event is dealt with clear the vector
//            clearEvtLvlVar();
//        
//            // And save this round of event
//            saveEvtLvlVar( passFO, passId, evt_met, evt_metPhi, evt_mt, njets40, pass_trig, prescale, weight );
//        
//            prev_evt_event = lepton_tree.evt_event();
//        }
//        // Old event!
//        else
//        {
//            // If it's an old event just save
//            saveEvtLvlVar( passFO, passId, evt_met, evt_metPhi, evt_mt, njets40, pass_trig, prescale, weight );
//        }

//        if ( lepton_tree.evt_event() == evt_event_to_print )
//        {
//            std::cout << "=====meas2====" << std::endl;
//            std::cout << "printing event info for evt_event = " << lepton_tree.evt_event() << std::endl;
//            std::cout << " passId " <<  passId << std::endl;
//            std::cout << " passFO " <<  passFO << std::endl;
//            std::cout << " pt " <<  p4().pt() << std::endl;
//            std::cout << " nFOs_VVV " <<  nFOs_VVV() << std::endl;
//            std::cout << " nFOs_SS " <<  nFOs_SS() << std::endl;
//            std::cout << " evt_met " <<  evt_met << std::endl;
//            std::cout << " evt_mt " <<  evt_mt << std::endl;
//            std::cout << " prescale " << prescale << std::endl;
//            std::cout << " pass_trig " << pass_trig << std::endl;
//            std::cout << " njets40 " << njets40 << std::endl;
//            std::cout << "=====" << std::endl;
//        }

////========================================================================================
//// Lepton Baby Event Level Variable Rebuilding Tool
////========================================================================================
//
//std::vector<int> evt_lvl_var_passFOs;
//std::vector<int> evt_lvl_var_passIds;
//std::vector<int> evt_lvl_var_njets;
//std::vector<LV> evt_lvl_var_p4;
//std::vector<LV> evt_lvl_var_passFO_p4;
//std::vector<int> evt_lvl_var_passFO_id;
//std::vector<LV> evt_lvl_var_passId_p4;
//std::vector<int> evt_lvl_var_passId_id;
//std::vector<int> evt_lvl_var_pass_trig;
//std::vector<int> evt_lvl_var_prescale;
//std::vector<int> evt_lvl_var_dilep_mass;
//std::vector<float> evt_lvl_var_evt_met;
//std::vector<float> evt_lvl_var_evt_metPhi;
//std::vector<float> evt_lvl_var_evt_mt;
//std::vector<float> evt_lvl_var_weight;
//std::vector<int>   evt_lvl_var_FO_pass_trig;
//std::vector<int>   evt_lvl_var_FO_prescale;
//std::vector<int>   evt_lvl_var_FO_dilep_mass;
//std::vector<float> evt_lvl_var_FO_evt_met;
//std::vector<float> evt_lvl_var_FO_evt_metPhi;
//std::vector<float> evt_lvl_var_FO_evt_mt;
//std::vector<float> evt_lvl_var_FO_weight;
//
////________________________________________________________________________________________
//void printEvtLvlVar()
//{
//    std::cout << "evt_lvl_var_passFOs   : "; for ( auto& i : evt_lvl_var_passFOs )     std::cout << i      << " "; std::cout << std::endl;
//    std::cout << "evt_lvl_var_passIds   : "; for ( auto& i : evt_lvl_var_passIds )     std::cout << i      << " "; std::cout << std::endl;
//    std::cout << "evt_lvl_var_p4        : "; for ( auto& i : evt_lvl_var_p4 )          std::cout << i.pt() << " "; std::cout << std::endl;
//    std::cout << "evt_lvl_var_p4 eta    : "; for ( auto& i : evt_lvl_var_p4 )          std::cout << i.eta()<< " "; std::cout << std::endl;
//    std::cout << "evt_lvl_var_passFO_p4 : "; for ( auto& i : evt_lvl_var_passFO_p4 )   std::cout << i.pt() << " "; std::cout << std::endl;
//    std::cout << "evt_lvl_var_passFO_id : "; for ( auto& i : evt_lvl_var_passFO_id )   std::cout << i      << " "; std::cout << std::endl;
//    std::cout << "evt_lvl_var_passId_p4 : "; for ( auto& i : evt_lvl_var_passId_p4 )   std::cout << i.pt() << " "; std::cout << std::endl;
//    std::cout << "evt_lvl_var_passId_id : "; for ( auto& i : evt_lvl_var_passId_id )   std::cout << i      << " "; std::cout << std::endl;
//    std::cout << "evt_lvl_var_pass_trig : "; for ( auto& i : evt_lvl_var_pass_trig )   std::cout << i      << " "; std::cout << std::endl;
//    std::cout << "evt_lvl_var_prescale  : "; for ( auto& i : evt_lvl_var_prescale  )   std::cout << i      << " "; std::cout << std::endl;
//    std::cout << "evt_lvl_var_njets     : "; for ( auto& i : evt_lvl_var_njets     )   std::cout << i      << " "; std::cout << std::endl;
//    std::cout << "evt_lvl_var_dilep_mass: "; for ( auto& i : evt_lvl_var_dilep_mass)   std::cout << i      << " "; std::cout << std::endl;
//    std::cout << "evt_lvl_var_evt_met   : "; for ( auto& i : evt_lvl_var_evt_met  )    std::cout << i      << " "; std::cout << std::endl;
//    std::cout << "evt_lvl_var_evt_metPhi: "; for ( auto& i : evt_lvl_var_evt_metPhi  ) std::cout << i      << " "; std::cout << std::endl;
//    std::cout << "evt_lvl_var_evt_mt    : "; for ( auto& i : evt_lvl_var_evt_mt  )     std::cout << i      << " "; std::cout << std::endl;
//    std::cout << "evt_lvl_var_FO_pass_trig : "; for ( auto& i : evt_lvl_var_FO_pass_trig )   std::cout << i      << " "; std::cout << std::endl;
//    std::cout << "evt_lvl_var_FO_prescale  : "; for ( auto& i : evt_lvl_var_FO_prescale  )   std::cout << i      << " "; std::cout << std::endl;
//    std::cout << "evt_lvl_var_FO_njets     : "; for ( auto& i : evt_lvl_var_FO_njets     )   std::cout << i      << " "; std::cout << std::endl;
//    std::cout << "evt_lvl_var_FO_dilep_mass: "; for ( auto& i : evt_lvl_var_FO_dilep_mass)   std::cout << i      << " "; std::cout << std::endl;
//    std::cout << "evt_lvl_var_FO_evt_met   : "; for ( auto& i : evt_lvl_var_FO_evt_met  )    std::cout << i      << " "; std::cout << std::endl;
//    std::cout << "evt_lvl_var_FO_evt_metPhi: "; for ( auto& i : evt_lvl_var_FO_evt_metPhi  ) std::cout << i      << " "; std::cout << std::endl;
//    std::cout << "evt_lvl_var_FO_evt_mt    : "; for ( auto& i : evt_lvl_var_FO_evt_mt  )     std::cout << i      << " "; std::cout << std::endl;
//}
//
////________________________________________________________________________________________
//void clearEvtLvlVar()
//{
//    // Now the previous event is dealt with clear the vector
//    evt_lvl_var_passFOs.clear();
//    evt_lvl_var_passIds.clear();
//    evt_lvl_var_njets.clear();
//    evt_lvl_var_p4.clear();
//    evt_lvl_var_passFO_p4.clear();
//    evt_lvl_var_passFO_id.clear();
//    evt_lvl_var_passId_p4.clear();
//    evt_lvl_var_passId_id.clear();
//    evt_lvl_var_pass_trig.clear();
//    evt_lvl_var_prescale.clear();
//    evt_lvl_var_dilep_mass.clear();
//    evt_lvl_var_evt_met.clear();
//    evt_lvl_var_evt_metPhi.clear();
//    evt_lvl_var_evt_mt.clear();
//    evt_lvl_var_weight.clear();
//    evt_lvl_var_FO_pass_trig.clear();
//    evt_lvl_var_FO_prescale.clear();
//    evt_lvl_var_FO_dilep_mass.clear();
//    evt_lvl_var_FO_evt_met.clear();
//    evt_lvl_var_FO_evt_metPhi.clear();
//    evt_lvl_var_FO_evt_mt.clear();
//    evt_lvl_var_FO_weight.clear();
//}
//
////________________________________________________________________________________________
//void saveEvtLvlVar( bool passFO, bool passId, float evt_met, float evt_metPhi, float evt_mt, int njets, bool pass_trig, int prescale, float weight )
//{
//    evt_lvl_var_passFOs.push_back( passFO );
//    evt_lvl_var_passIds.push_back( passId );
//    evt_lvl_var_p4.push_back( p4() );
//
//    passFO &= p4().pt() > 10.;
//    passId &= p4().pt() > 10.;
//
//    if ( passFO )
//        evt_lvl_var_passFO_p4.push_back( p4() );
//
//    if ( passFO )
//        evt_lvl_var_passFO_id.push_back( id() );
//
//    if ( passId )
//        evt_lvl_var_passId_p4.push_back( p4() );
//
//    if ( passId )
//        evt_lvl_var_passId_id.push_back( id() );
//
//    if ( passId )
//    {
//        evt_lvl_var_pass_trig.push_back( pass_trig );
//        evt_lvl_var_prescale.push_back( prescale );
//        evt_lvl_var_njets.push_back( njets );
//        evt_lvl_var_dilep_mass.push_back( dilep_mass() );
//        evt_lvl_var_evt_met.push_back( evt_met );
//        evt_lvl_var_evt_metPhi.push_back( evt_metPhi );
//        evt_lvl_var_evt_mt.push_back( evt_mt );
//        evt_lvl_var_weight.push_back( weight );
//    }
//    if ( passFO )
//    {
//        evt_lvl_var_FO_pass_trig.push_back( pass_trig );
//        evt_lvl_var_FO_prescale.push_back( prescale );
//        evt_lvl_var_FO_njets.push_back( njets );
//        evt_lvl_var_FO_dilep_mass.push_back( dilep_mass() );
//        evt_lvl_var_FO_evt_met.push_back( evt_met );
//        evt_lvl_var_FO_evt_metPhi.push_back( evt_metPhi );
//        evt_lvl_var_FO_evt_mt.push_back( evt_mt );
//        evt_lvl_var_FO_weight.push_back( weight );
//    }
//}
//
////________________________________________________________________________________________
//int nIds_VVV()
//{
//    int nIds_VVV_rtn = 0;
//
//    for ( auto& passId : evt_lvl_var_passIds )
//    {
//        if ( passId != 0 )
//            nIds_VVV_rtn++;
//    }
//
//    return nIds_VVV_rtn;
//}
//
////________________________________________________________________________________________
//float dilep_mass_from_FOs()
//{
//    // I will assume when I call this function I already have two FOs
//    if ( evt_lvl_var_passFO_p4.size() != 2 )
//        TasUtil::error( "I assumed I had two and only two fakeable object. This is not true!" );
//
//    return ( evt_lvl_var_passFO_p4[0] + evt_lvl_var_passFO_p4[1] ).mass();
//}

//    TH1F* histo_ptvarbin_meas_mu = ( TH1F*) hists.get( "histo_ptvarbin_meas_mu" );
//    TH1F* nIds_VVV_pt50 = ( TH1F*) hists.get( "nIds_VVV_pt50" );
//    TH1F* nIds_VVV_pt70 = ( TH1F*) hists.get( "nIds_VVV_pt70" );
//    TH1F* nIds_VVV_pt120 = ( TH1F*) hists.get( "nIds_VVV_pt120" );
//    TH1F* nIds_VVV_pt170 = ( TH1F*) hists.get( "nIds_VVV_pt170" );
//
//    if ( histo_ptvarbin_meas_mu && nIds_VVV_pt50 && nIds_VVV_pt70 && nIds_VVV_pt120 )
//    {
//        double ref50 = histo_ptvarbin_meas_mu->GetBinContent( 7 );
//        double ref70 = histo_ptvarbin_meas_mu->GetBinContent( 8 );
//        double ref120 = histo_ptvarbin_meas_mu->GetBinContent( 9 );
//        double val50 = nIds_VVV_pt50->GetBinContent( 1 );
//        double val70 = nIds_VVV_pt70->GetBinContent( 1 );
//        double val120 = nIds_VVV_pt120->GetBinContent( 1 );
//
//        std::cout << " pt 50 - 70 bin : " << ref50 << " " << val50 << std::endl;
//        std::cout << " pt 70 -120 bin : " << ref70 << " " << val70 << std::endl;
//        std::cout << " pt120 -170 bin : " << ref120 << " " << val120 << std::endl;
//    }
