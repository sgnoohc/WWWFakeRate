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

//________________________________________________________________________________________
void ScanChain( TChain* chain, TString outputname, int nEvents = -1 )
{

    //------------------------------------------------------------------------------------
    // Identifying which sample I am running on based on the output name.
    // Generally not a good idea to have the output name dictate the behavior of the code.
    // But for simplicity I am treating the "outputname" as "options".
    //------------------------------------------------------------------------------------
    bool isData = outputname.Contains( "2016" );
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
    
        //----------------------
        // Jet related selection
        //----------------------
        int njets40 = 0;
        
        for ( unsigned int i = 0; i < jets().size(); i++ )
        {
            if ( ROOT::Math::VectorUtil::DeltaR( jets()[i], p4() ) < 1. )
                continue;
                
            if ( jets()[i].pt() > 40. && fabs( jets()[i].eta() ) < 2.4 )
                njets40++;
        }
        
        //--------------------------------
        // Prescale calculation via Z-peak
        //--------------------------------
        
        if ( passes_VVV_cutbased_tight() && p4().pt() > 25. && tag_p4().pt() > 30. )
        {
            float wgt = isData ? 1 : scale1fb() * lumi;
            
            if ( abs( id() ) == 11 )
            {
                if ( HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30() > 0 )
                    hists.fill( dilep_mass(), "mll_e8i", wgt, 80, 0, 200 );
                    
                if ( HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30() > 0 )
                    hists.fill( dilep_mass(), "mll_e17i", wgt, 80, 0, 200 );
            }
            else if ( abs( id() ) == 13 )
            {
                if ( HLT_Mu8_TrkIsoVVL() > 0 )
                    hists.fill( dilep_mass(), "mll_m8i", wgt, 80, 0, 200 );
                    
                if ( HLT_Mu17_TrkIsoVVL() > 0 )
                    hists.fill( dilep_mass(), "mll_m17i", wgt, 80, 0, 200 );
            }
        }
        
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
        
        //----------------------
        // Event level selection
        //----------------------
        // if prompt lepton with QCD sample reject.
        if ( isQCD && motherID() > 0 )
        {
            hists.fill( __COUNTER__, "counter",  1.,  20, 0, 20 );
            continue;
        }
        
//        // if prompt lepton with QCD sample reject.
//        if ( isEWK && motherID() < 0 )
//        {
//            hists.fill( __COUNTER__, "counter",  1.,  20, 0, 20 );
//            continue;
//        }
        
        // Skip events without at least one jet of 40 GeV
        if ( !( njets40 > 0 ) )
        {
            hists.fill( __COUNTER__, "counter",  1.,  20, 0, 20 );
            continue;
        }
        
        // Skip events with more than one FO
        if ( nFOs_SS() > 1 )
        {
            hists.fill( __COUNTER__, "counter",  1.,  20, 0, 20 );
            continue;
        }
        
        float relIso = RelIso03EA();
        
        //--------------------------
        // Trigger related selection
        //--------------------------
        
        // Compute prescales for QCD and Data.
        // QCD will take prescale from the LeptonTree directly.
        // For data, the prescale values were messed up.
        // Therefore it will use hardcoded values.
        int prescale = 0;
        
        
        if ( abs( id() ) == 11 )
        {
            // Check the trigger for our isolated single lepton trigger fired.
            // The trigger has positive value for lepton leg matched.
            // The HLT_* branch value will be negative for leg not matched.
            // (See setHLTBranch() function in CORE for more info.)
            if ( p4().pt() >= 10 && p4().pt() < 25 )
            {
                if ( HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30() <= 0 )
                {
                    hists.fill( __COUNTER__, "counter",  1.,  20, 0, 20 );
                    continue;
                }
                
                prescale = e8i;
            }
            else if ( p4().pt() >= 25 )
            {
                if ( HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30() <= 0 )
                {
                    hists.fill( __COUNTER__, "counter",  1.,  20, 0, 20 );
                    continue;
                }
                
                prescale = e17i;
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
                if ( HLT_Mu8_TrkIsoVVL() <= 0 )
                {
                    hists.fill( __COUNTER__, "counter",  1.,  20, 0, 20 );
                    continue;
                }
                
                prescale = m8i;
            }
            else if ( p4().pt() >= 25 )
            {
                if ( HLT_Mu17_TrkIsoVVL() <= 0 )
                {
                    hists.fill( __COUNTER__, "counter",  1.,  20, 0, 20 );
                    continue;
                }
                
                prescale = m17i;
            }
        }
        
        //---------------------
        // Compute event weight
        //---------------------
        float weight = isData ? prescale : scale1fb() * lumi * getTruePUw_Moriond( nvtx() );
        
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
        
        //------------------------------------------------------------
        // Lepton ID selection (most important selection in fake rate)
        //------------------------------------------------------------
        bool passId = passes_VVV_cutbased_tight();
        bool passFO = passes_VVV_cutbased_fo();
//        bool passFO = passes_VVV_cutbased_fo_noiso();
//        if ( abs( id() ) == 11 )
//        {
//            passFO = passFO && (relIso < 0.2);
//        }
//        else if ( abs( id() ) == 13 )
//        {
//            passFO = passFO && (relIso < 0.8);
//        }

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
        
        //------------------------------------------------------------
        // compute MT and MET
        //------------------------------------------------------------
        float evt_met = evt_corrMET();
        float evt_metPhi = evt_corrMETPhi();
        float evt_mt = calculateMt( p4(), evt_met, evt_metPhi );
        
        //------------------------------------
        // VVV tight isolation cone correction
        //------------------------------------
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
        
        //------------------------------------------------------------
        // fill histograms
        //------------------------------------------------------------
        if ( passId )
        {

            fillFakeRateHistograms( hists, "", evt_met, evt_mt, weight);;
//            //mt control region
//            if ( evt_met > 30. && evt_mt > 80. && evt_mt < 120. )
//                fill( hists, "ewk_cr", evt_mt, evt_met, weight );
//                
//            if ( evt_met < 30. && evt_mt > 80. && evt_mt < 120. )
//                fill( hists, "lowmet_ewk_cr", evt_mt, evt_met, weight );
//                
//            if ( evt_met > 30. && p4().pt() > 50 )
//                fill( hists, "highpt_cr", evt_mt, evt_met, weight );
//                
//            if ( evt_met > 30. && p4().pt() <= 50 && p4().pt() > 30 )
//                fill( hists, "medpt_cr", evt_mt, evt_met, weight );
//                
//            if ( evt_met > 30. && p4().pt() <= 30 )
//                fill( hists, "lowpt_cr", evt_mt, evt_met, weight );
//                
//            if ( evt_met > 30. )
//                fill( hists, "cr", evt_mt, evt_met, weight );
//                
//            if ( evt_met > 50. && p4().pt() > 30 )
//                fill( hists, "highpt_tightcr", evt_mt, evt_met, weight );
//                
//            if ( evt_met > 50. && p4().pt() <= 30 )
//                fill( hists, "lowpt_tightcr", evt_mt, evt_met, weight );
//                
//            if ( evt_met > 50. )
//                fill( hists, "tightcr", evt_mt, evt_met, weight );
//                
//            if ( evt_met < 20. )
//                fill( hists, "lm", evt_mt, evt_met, weight );
//                
//            if ( p4().pt() > 30 )
//                fill( hists, "highpt", evt_mt, evt_met, weight );
//                
//            if ( evt_mt < 20. )
//                fill( hists, "lmt", evt_mt, evt_met, weight );
//                
//            if ( evt_mt > 30. )
//                fill( hists, "hmt", evt_mt, evt_met, weight );
//                
//            if ( evt_mt < 20. && evt_met < 20 )
//                fill( hists, "meas", evt_mt, evt_met, weight );
                
        }
        
        if ( passFO )
        {
            fillFakeRateHistograms( hists, "loose_", evt_met, evt_mt, weight);;
//            //mt control region
//            if ( evt_met > 30. && evt_mt > 80. && evt_mt < 120. )
//                fill( hists, "loose_ewk_cr", evt_mt, evt_met, weight );
//                
//            if ( evt_met < 30. && evt_mt > 80. && evt_mt < 120. )
//                fill( hists, "loose_lowmet_ewk_cr", evt_mt, evt_met, weight );
//                
//            if ( evt_met > 30. && p4().pt() > 50 )
//                fill( hists, "loose_highpt_cr", evt_mt, evt_met, weight );
//                
//            if ( evt_met > 30. && p4().pt() <= 50 && p4().pt() > 30 )
//                fill( hists, "loose_medpt_cr", evt_mt, evt_met, weight );
//                
//            if ( evt_met > 30. && p4().pt() <= 30 )
//                fill( hists, "loose_lowpt_cr", evt_mt, evt_met, weight );
//                
//            if ( evt_met > 30. )
//                fill( hists, "loose_cr", evt_mt, evt_met, weight );
//                
//            if ( evt_met > 30. && p4().pt() > 30 )
//                fill( hists, "loose_highpt_tightcr", evt_mt, evt_met, weight );
//                
//            if ( evt_met > 30. && p4().pt() <= 30 )
//                fill( hists, "loose_lowpt_tightcr", evt_mt, evt_met, weight );
//                
//            if ( evt_met > 30. )
//                fill( hists, "loose_tightcr", evt_mt, evt_met, weight );
//                
//            if ( evt_met < 20. )
//                fill( hists, "loose_lm", evt_mt, evt_met, weight );
//                
//            if ( p4().pt() > 30 )
//                fill( hists, "loose_highpt", evt_mt, evt_met, weight );
//                
//            if ( evt_mt < 20. )
//                fill( hists, "loose_lmt", evt_mt, evt_met, weight );
//                
//            if ( evt_mt > 30. )
//                fill( hists, "loose_hmt", evt_mt, evt_met, weight );
//                
//            if ( evt_mt < 20. && evt_met < 20 )
//                fill( hists, "loose_meas", evt_mt, evt_met, weight );
        }
        
        if ( passFO && !passId )
        {
            fillFakeRateHistograms( hists, "lbnt_", evt_met, evt_mt, weight);;
//            if ( evt_met > 50. && evt_mt > 80. )
//                fill( hists, "lbnt_highmet_cr", evt_mt, evt_met, weight );
//
//            if ( evt_met < 30. && evt_mt > 80. && evt_mt < 120. )
//                fill( hists, "lbnt_lowmet_ewk_cr", evt_mt, evt_met, weight );
//                
//            if ( evt_met > 50. && evt_mt > 80. && p4().pt() > 30. )
//                fill( hists, "lbnt_highmet_highpt_cr", evt_mt, evt_met, weight );
//
//            if ( evt_met > 30. && evt_mt > 80. && evt_mt < 120. )
//                fill( hists, "lbnt_ewk_cr", evt_mt, evt_met, weight );
//                
//            if ( evt_met > 30. && p4().pt() > 50 )
//                fill( hists, "lbnt_highpt_cr", evt_mt, evt_met, weight );
//                
//            if ( evt_met > 30. && p4().pt() <= 50 && p4().pt() > 30 )
//                fill( hists, "lbnt_medpt_cr", evt_mt, evt_met, weight );
//                
//            if ( evt_met > 30. && p4().pt() <= 30 )
//                fill( hists, "lbnt_lowpt_cr", evt_mt, evt_met, weight );
//                
//            if ( evt_met > 30. )
//                fill( hists, "lbnt_cr", evt_mt, evt_met, weight );
//                
//            if ( evt_met > 30. && p4().pt() > 30 )
//                fill( hists, "lbnt_highpt_tightcr", evt_mt, evt_met, weight );
//                
//            if ( evt_met > 30. && p4().pt() <= 30 )
//                fill( hists, "lbnt_lowpt_tightcr", evt_mt, evt_met, weight );
//                
//            if ( evt_met > 30. )
//                fill( hists, "lbnt_tightcr", evt_mt, evt_met, weight );
//                
//            if ( evt_met < 20. )
//                fill( hists, "lbnt_lm", evt_mt, evt_met, weight );
//                
//            if ( p4().pt() > 30 )
//                fill( hists, "lbnt_highpt", evt_mt, evt_met, weight );
//                
//            if ( evt_mt < 20. )
//                fill( hists, "lbnt_lmt", evt_mt, evt_met, weight );
//                
//            if ( evt_mt > 30. )
//                fill( hists, "lbnt_hmt", evt_mt, evt_met, weight );
//                
//            if ( evt_mt < 20. && evt_met < 20 )
//                fill( hists, "lbnt_meas", evt_mt, evt_met, weight );
        }

        if ( !( evt_met < 20. && evt_mt < 20 ) )
        {
            hists.fill( __COUNTER__, "counter",  1.,  20, 0, 20 );
            continue;
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
                hists.fill( 0, "Nt", weight );
                
                if ( abs( id() ) == 11 )
                {
                    hists.fill( 0, "Nt_e", weight );
                    hists.fill( p4().pt(), abs( p4().eta() ), "Nt_histo_e",
                                weight, nptbins, ptbins, netabins, etabins_el );
                }
                
                if ( abs( id() ) == 13 )
                {
                    hists.fill( 0, "Nt_mu", weight );
                    hists.fill( p4().pt(), abs( p4().eta() ), "Nt_histo_mu",
                                weight, nptbins, ptbins, netabins, etabins_mu );
                }
            }
            
            if ( passFO )
            {
                hists.fill( 0, "Nl", weight );
                
                if ( abs( id() ) == 11 )
                {
                    hists.fill( 0, "Nl_e", weight );
                    hists.fill( p4().pt(), abs( p4().eta() ), "Nl_histo_e",
                                weight, nptbins, ptbins, netabins, etabins_el );
                    hists.fill( p4().pt() * ( 1 + coneptcorr ), abs( p4().eta() ),
                                "Nl_cone_histo_e", weight, nptbins, ptbins, netabins, etabins_el );
                }
                
                if ( abs( id() ) == 13 )
                {
                    hists.fill( 0, "Nl_mu", weight );
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
    }
    
    if ( abs( id() ) == 11 )
    {
        if ( p4().pt() < 10. )
            return;
        else if ( p4().pt() < 15. )
        {
            hists.fill( evt_mt, "histo_pt15_mt_" + suffix + "_el", weight, 20, 0, 200 );
            hists.fill( evt_met, "histo_pt15_met_" + suffix + "_el", weight, 20, 0, 200 );
        }
        else if ( p4().pt() < 20. )
        {
            hists.fill( evt_mt, "histo_pt20_mt_" + suffix + "_el", weight, 20, 0, 200 );
            hists.fill( evt_met, "histo_pt20_met_" + suffix + "_el", weight, 20, 0, 200 );
        }
        else if ( p4().pt() < 25. )
        {
            hists.fill( evt_mt, "histo_pt25_mt_" + suffix + "_el", weight, 20, 0, 200 );
            hists.fill( evt_met, "histo_pt25_met_" + suffix + "_el", weight, 20, 0, 200 );
        }
        else if ( p4().pt() < 30. )
        {
            hists.fill( evt_mt, "histo_pt30_mt_" + suffix + "_el", weight, 20, 0, 200 );
            hists.fill( evt_met, "histo_pt30_met_" + suffix + "_el", weight, 20, 0, 200 );
        }
        else if ( p4().pt() < 35. )
        {
            hists.fill( evt_mt, "histo_pt35_mt_" + suffix + "_el", weight, 20, 0, 200 );
            hists.fill( evt_met, "histo_pt35_met_" + suffix + "_el", weight, 20, 0, 200 );
        }
        else if ( p4().pt() < 50. )
        {
            hists.fill( evt_mt, "histo_pt50_mt_" + suffix + "_el", weight, 20, 0, 200 );
            hists.fill( evt_met, "histo_pt50_met_" + suffix + "_el", weight, 20, 0, 200 );
        }
        else if ( p4().pt() < 70. )
        {
            hists.fill( evt_mt, "histo_pt70_mt_" + suffix + "_el", weight, 20, 0, 200 );
            hists.fill( evt_met, "histo_pt70_met_" + suffix + "_el", weight, 20, 0, 200 );
        }
    }
    else if ( abs( id() ) == 13 )
    {
        if ( p4().pt() < 10. )
            return;
        else if ( p4().pt() < 15. )
        {
            hists.fill( evt_mt, "histo_pt15_mt_" + suffix + "_mu", weight, 20, 0, 200 );
            hists.fill( evt_met, "histo_pt15_met_" + suffix + "_mu", weight, 20, 0, 200 );
        }
        else if ( p4().pt() < 20. )
        {
            hists.fill( evt_mt, "histo_pt20_mt_" + suffix + "_mu", weight, 20, 0, 200 );
            hists.fill( evt_met, "histo_pt20_met_" + suffix + "_mu", weight, 20, 0, 200 );
        }
        else if ( p4().pt() < 25. )
        {
            hists.fill( evt_mt, "histo_pt25_mt_" + suffix + "_mu", weight, 20, 0, 200 );
            hists.fill( evt_met, "histo_pt25_met_" + suffix + "_mu", weight, 20, 0, 200 );
        }
        else if ( p4().pt() < 30. )
        {
            hists.fill( evt_mt, "histo_pt30_mt_" + suffix + "_mu", weight, 20, 0, 200 );
            hists.fill( evt_met, "histo_pt30_met_" + suffix + "_mu", weight, 20, 0, 200 );
        }
        else if ( p4().pt() < 35. )
        {
            hists.fill( evt_mt, "histo_pt35_mt_" + suffix + "_mu", weight, 20, 0, 200 );
            hists.fill( evt_met, "histo_pt35_met_" + suffix + "_mu", weight, 20, 0, 200 );
        }
        else if ( p4().pt() < 50. )
        {
            hists.fill( evt_mt, "histo_pt50_mt_" + suffix + "_mu", weight, 20, 0, 200 );
            hists.fill( evt_met, "histo_pt50_met_" + suffix + "_mu", weight, 20, 0, 200 );
        }
        else if ( p4().pt() < 70. )
        {
            hists.fill( evt_mt, "histo_pt70_mt_" + suffix + "_mu", weight, 20, 0, 200 );
            hists.fill( evt_met, "histo_pt70_met_" + suffix + "_mu", weight, 20, 0, 200 );
        }
    }



}

//________________________________________________________________________________________
void fillFakeRateHistograms( TasUtil::AutoHist& hists, TString label, float& evt_met, float& evt_mt, float& weight )
{
    //mt control region
    if ( evt_met > 30. && evt_mt > 80. && evt_mt < 120. )
        fill( hists, label + "ewk_cr", evt_mt, evt_met, weight );
        
    if ( evt_met < 30. && evt_mt > 80. && evt_mt < 120. )
        fill( hists, label + "lowmet_ewk_cr", evt_mt, evt_met, weight );
        
    if ( evt_met > 30. && p4().pt() > 50 )
        fill( hists, label + "highpt_cr", evt_mt, evt_met, weight );
        
    if ( evt_met > 30. && p4().pt() <= 50 && p4().pt() > 30 )
        fill( hists, label + "medpt_cr", evt_mt, evt_met, weight );
        
    if ( evt_met > 30. && p4().pt() <= 30 )
        fill( hists, label + "lowpt_cr", evt_mt, evt_met, weight );
        
    if ( evt_met > 30. )
        fill( hists, label + "cr", evt_mt, evt_met, weight );
        
    if ( evt_met > 50. && p4().pt() > 30 )
        fill( hists, label + "highpt_tightcr", evt_mt, evt_met, weight );
        
    if ( evt_met > 50. && p4().pt() <= 30 )
        fill( hists, label + "lowpt_tightcr", evt_mt, evt_met, weight );
        
    if ( evt_met > 50. )
        fill( hists, label + "tightcr", evt_mt, evt_met, weight );
        
    if ( evt_met < 20. )
        fill( hists, label + "lm", evt_mt, evt_met, weight );
        
    if ( p4().pt() > 30 )
        fill( hists, label + "highpt", evt_mt, evt_met, weight );
        
    if ( p4().pt() > 50 )
        fill( hists, label + "highpt50", evt_mt, evt_met, weight );
        
    if ( p4().pt() > 30. &&  p4().pt() <= 50 )
        fill( hists, label + "medpt", evt_mt, evt_met, weight );
        
    if ( evt_mt < 20. )
        fill( hists, label + "lmt", evt_mt, evt_met, weight );
        
    if ( evt_mt > 30. )
        fill( hists, label + "hmt", evt_mt, evt_met, weight );
        
    if ( evt_mt > 50. )
        fill( hists, label + "hmt50", evt_mt, evt_met, weight );
        
    if ( evt_mt > 70. )
        fill( hists, label + "hmt70", evt_mt, evt_met, weight );
        
    if ( evt_mt > 80. )
        fill( hists, label + "hmt80", evt_mt, evt_met, weight );
        
    if ( evt_mt > 120. )
        fill( hists, label + "hmt120", evt_mt, evt_met, weight );
        
    if ( evt_mt > 50. && p4().pt() > 30. )
        fill( hists, label + "hmt50pt30", evt_mt, evt_met, weight );
        
    if ( evt_mt > 70. && p4().pt() > 30. )
        fill( hists, label + "hmt70pt30", evt_mt, evt_met, weight );
        
    if ( evt_mt > 80. && p4().pt() > 30. )
        fill( hists, label + "hmt80pt30", evt_mt, evt_met, weight );
        
    if ( evt_mt > 120. && p4().pt() > 30. )
        fill( hists, label + "hmt120pt30", evt_mt, evt_met, weight );
        
    if ( evt_mt > 50. && p4().pt() > 50. )
        fill( hists, label + "hmt50pt50", evt_mt, evt_met, weight );
        
    if ( evt_mt > 70. && p4().pt() > 50. )
        fill( hists, label + "hmt70pt50", evt_mt, evt_met, weight );
        
    if ( evt_mt > 80. && p4().pt() > 50. )
        fill( hists, label + "hmt80pt50", evt_mt, evt_met, weight );
        
    if ( evt_mt > 120. && p4().pt() > 50. )
        fill( hists, label + "hmt120pt50", evt_mt, evt_met, weight );
        
    if ( evt_mt < 20. && evt_met < 20 )
        fill( hists, label + "meas", evt_mt, evt_met, weight );
}
