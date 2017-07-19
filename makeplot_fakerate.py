#!/bin/env python

import sys
sys.path.append("/home/users/phchang/public_html/tasutil")
import plotmaker
import ROOT as r

tfile_data   = r.TFile("outputs/fakerate_data.root")
tfile_dy     = r.TFile("outputs/fakerate_dy.root")
tfile_wj     = r.TFile("outputs/fakerate_wj.root")
tfile_ttbar  = r.TFile("outputs/fakerate_ttbar.root")
tfile_qcd_mu = r.TFile("outputs/fakerate_qcd_mu.root")
tfile_qcd_el = r.TFile("outputs/fakerate_qcd_el.root")

from array import *

norebin = True

def rebin(hist, name):
    if norebin:
        return hist
    if name.find("varbin") != -1:
#        bins = array('d', [10., 15., 20., 25., 30., 35., 50., 170.])
#        tmphist = hist.Rebin(7, hist.GetName(), bins)
        bins = array('d', [10., 15., 20., 25., 30., 35., 170.])
        tmphist = hist.Rebin(6, hist.GetName(), bins)
        tmphist.SetDirectory( 0 )
        return tmphist
    if name.find("histo_met_") != -1:
        bins = array('d', [0., 20., 30., 200.])
        tmphist = hist.Rebin(3, hist.GetName(), bins)
        tmphist.SetDirectory( 0 )
        return tmphist
    return hist

def draw(histname, additional_optstring="", include_ttbar=False, include_QCD=False, rewgthist=None, suffix=""):

    print histname

    histo__data = rebin( tfile_data .Get(histname).Clone("Data"), histname )
    histo__dy   = rebin( tfile_dy   .Get(histname).Clone("DY"), histname )
    histo__wj   = rebin( tfile_wj   .Get(histname).Clone("W"), histname )
    histo__data .SetDirectory( 0 )
    histo__dy   .SetDirectory( 0 )
    histo__wj   .SetDirectory( 0 )
    histo__data.SetLineColor( 1 )
    histo__dy.SetFillColor( 7002 )
    histo__wj.SetFillColor( 7006 )
    histo__dy.SetLineColor( 7002 )
    histo__wj.SetLineColor( 7006 )

    if rewgthist:
        try:
            for ibin in xrange( 0, histo__dy.GetNbinsX() + 1 ):
                if ibin == 0 or ibin == 1:
                    continue
                histo__dy.SetBinContent( ibin + 1, histo__dy.GetBinContent( ibin + 1 ) * rewgthist.GetBinContent( ibin + 1 ) )
                histo__wj.SetBinContent( ibin + 1, histo__wj.GetBinContent( ibin + 1 ) * rewgthist.GetBinContent( ibin + 1 ) )
        except:
            histo__dy.Scale( rewgthist )
            histo__wj.Scale( rewgthist )

    v_bkg_hists = r.vector("TH1*")()
    v_bkg_hists.push_back( histo__dy )
    v_bkg_hists.push_back( histo__wj )
    if include_ttbar:
        histo__ttbar = rebin( tfile_ttbar.Get(histname).Clone("t#bar{t}"), histname)
        histo__ttbar.SetDirectory( 0 )
        histo__ttbar.SetFillColor( 7004 )
        histo__ttbar.SetLineColor( 7004 )
        if rewgthist:
            for ibin in xrange( 0, histo__dy.GetNbinsX() + 1 ):
                histo__ttbar.SetBinContent( ibin + 1, histo__ttbar.GetBinContent( ibin + 1 ) * rewgthist.GetBinContent( ibin + 1 ) )
        v_bkg_hists.push_back( histo__ttbar )

    if include_QCD:
        histo__qcd_mu= rebin( tfile_qcd_mu.Get(histname).Clone("QCD"), histname )
        histo__qcd_mu.SetDirectory( 0 )
        histo__qcd_mu.SetFillColor( 7003 )
        histo__qcd_mu.SetLineColor( 7003 )
        if rewgthist:
            for ibin in xrange( 0, histo__dy.GetNbinsX() + 1 ):
                histo__qcd_mu.SetBinContent( ibin + 1, histo__qcd_mu.GetBinContent( ibin + 1 ) * rewgthist.GetBinContent( ibin + 1 ) )
        v_bkg_hists.push_back( histo__qcd_mu )

    if histname.find( "varbin" ):
        additional_optstring += "--divideByBinWidth"

    for bkg_hist in v_bkg_hists:
        bkg_hist.Print("all")

    histo__data.Print("all")


    return r.plotmaker( """
                  --yTitle N leptons
                  --xTitle m_{T,#mu} [GeV]
                  --plotOutputName %s
                  --ratio_DrawOpt ep
                  --ratio_Maximum 1.3
                  --ratio_Minimum 0.
                  --showOverflow
                  --ratioPaneAtBottom
                  --autoStack
                  %s
                  """%("plots/" + histname + suffix, additional_optstring),
                  histo__data, v_bkg_hists )

def draw_ewk_subtr(num_histname, den_histname, plotname, additional_optstring="", include_ttbar=False, rewgthist=None, suffix=""):

    histo_num__data = rebin( tfile_data .Get(num_histname).Clone("Tight Data") , num_histname)
    histo_num__dy   = rebin( tfile_dy   .Get(num_histname).Clone("DYskip") , num_histname)
    histo_num__wj   = rebin( tfile_wj   .Get(num_histname).Clone("Wskip") , num_histname)
    histo_den__data = rebin( tfile_data .Get(den_histname).Clone("Loose Data") , den_histname)
    histo_den__dy   = rebin( tfile_dy   .Get(den_histname).Clone("DYskip") , den_histname)
    histo_den__wj   = rebin( tfile_wj   .Get(den_histname).Clone("Wskip") , den_histname)

    histo_num__data .SetDirectory( 0 )
    histo_num__dy   .SetDirectory( 0 )
    histo_num__wj   .SetDirectory( 0 )
    histo_den__data .SetDirectory( 0 )
    histo_den__dy   .SetDirectory( 0 )
    histo_den__wj   .SetDirectory( 0 )

    histo_num__data .SetLineColor( 1 )
    histo_num__data .SetFillColor( r.kGray + 1 )

    histo_den__data .SetLineColor( 1 )

    histo_num__dy.SetFillColor( 7002 )
    histo_num__wj.SetFillColor( 7006 )
    histo_num__dy.SetLineColor( 7002 )
    histo_num__wj.SetLineColor( 7006 )
    histo_den__dy.SetFillColor( 7002 )
    histo_den__wj.SetFillColor( 7006 )
    histo_den__dy.SetLineColor( 7002 )
    histo_den__wj.SetLineColor( 7006 )

    if rewgthist:
        if rewgthist[0]:
            num_rewgthist = rewgthist[0]
            for ibin in xrange( 0, histo_num__dy.GetNbinsX() + 1 ):
                print ibin + 1, num_rewgthist.GetBinContent( ibin + 1 )
                histo_num__dy.SetBinContent( ibin + 1, histo_num__dy.GetBinContent( ibin + 1 ) * num_rewgthist.GetBinContent( ibin + 1 ) )
                histo_num__wj.SetBinContent( ibin + 1, histo_num__wj.GetBinContent( ibin + 1 ) * num_rewgthist.GetBinContent( ibin + 1 ) )

        if rewgthist[1]:
            den_rewgthist = rewgthist[1]
            for ibin in xrange( 0, histo_den__dy.GetNbinsX() + 1 ):
                print ibin + 1, den_rewgthist.GetBinContent( ibin + 1 )
                histo_den__dy.SetBinContent( ibin + 1, histo_den__dy.GetBinContent( ibin + 1 ) * den_rewgthist.GetBinContent( ibin + 1 ) )
                histo_den__wj.SetBinContent( ibin + 1, histo_den__wj.GetBinContent( ibin + 1 ) * den_rewgthist.GetBinContent( ibin + 1 ) )

        if rewgthist[2]:
            flatrewgt = rewgthist[2]
            histo_num__dy.Scale( flatrewgt )
            histo_num__wj.Scale( flatrewgt )

        if rewgthist[3]:
            flatrewgt = rewgthist[3]
            histo_den__dy.Scale( flatrewgt )
            histo_den__wj.Scale( flatrewgt )

    histo_num__dy.Scale( -1 )
    histo_num__wj.Scale( -1 )
    histo_den__dy.Scale( -1 )
    histo_den__wj.Scale( -1 )

    v_bkg_hists = r.vector("TH1*")()
    v_bkg_hists.push_back( histo_num__data )
    v_bkg_hists.push_back( histo_num__dy )
    v_bkg_hists.push_back( histo_num__wj )
    v_data_hists = r.vector("TH1*")()
    v_data_hists.push_back( histo_den__data )
    v_data_hists.push_back( histo_den__dy )
    v_data_hists.push_back( histo_den__wj )
    if include_ttbar:
        histo_num__ttbar= rebin( tfile_ttbar.Get(num_histname).Clone("t#bar{t}skip") , num_histname)
        histo_den__ttbar= rebin( tfile_ttbar.Get(den_histname).Clone("t#bar{t}skip") , den_histname)
        histo_num__ttbar.SetDirectory( 0 )
        histo_num__ttbar.SetFillColor( 7004 )
        histo_num__ttbar.SetLineColor( 7004 )
        histo_den__ttbar.SetDirectory( 0 )
        histo_den__ttbar.SetFillColor( 7004 )
        histo_den__ttbar.SetLineColor( 7004 )
        if rewgthist:
            if rewgthist[0]:
                num_rewgthist = rewgthist[0]
                for ibin in xrange( 0, histo_num__dy.GetNbinsX() + 1 ):
                    print ibin + 1, num_rewgthist.GetBinContent( ibin + 1 )
                    histo_num__ttbar.SetBinContent( ibin + 1, histo_num__ttbar.GetBinContent( ibin + 1 ) * num_rewgthist.GetBinContent( ibin + 1 ) )

            if rewgthist[1]:
                den_rewgthist = rewgthist[1]
                for ibin in xrange( 0, histo_den__dy.GetNbinsX() + 1 ):
                    print ibin + 1, den_rewgthist.GetBinContent( ibin + 1 )
                    histo_den__ttbar.SetBinContent( ibin + 1, histo_den__ttbar.GetBinContent( ibin + 1 ) * den_rewgthist.GetBinContent( ibin + 1 ) )

            if rewgthist[2]:
                flatrewgt = rewgthist[2]
                histo_num__ttbar.Scale( flatrewgt )

            if rewgthist[3]:
                flatrewgt = rewgthist[3]
                histo_den__ttbar.Scale( flatrewgt )

        histo_num__ttbar.Scale( -1 )
        histo_den__ttbar.Scale( -1 )
        v_bkg_hists.push_back( histo_num__ttbar )
        v_data_hists.push_back( histo_den__ttbar )

    if num_histname.find( "varbin" ):
        additional_optstring += "--divideByBinWidth"

    return r.plotmaker( """
                  --yTitle N leptons
                  --xTitle m_{T,#mu} [GeV]
                  --plotOutputName %s
                  --ratio_DrawOpt ep
                  --sumDataHists
                  --autoStack
                  --showOverflow
                  --ratioPaneAtBottom
                  --ratio_Maximum 1.7
                  %s
                  """%("plots/" + plotname + suffix, additional_optstring),
                  v_data_hists, v_bkg_hists )

def draw_qcd(num_histname, den_histname, plotname, additional_optstring="", include_ttbar=False, rewgthist=None, suffix=""):

    if num_histname.find( "_mu" ) != -1:

        histo_num__qcd_mu = rebin( tfile_qcd_mu.Get(num_histname).Clone("Tight QCD") , num_histname)
        histo_den__qcd_mu = rebin( tfile_qcd_mu.Get(den_histname).Clone("Loose QCD") , den_histname)

        histo_den__qcd_mu.SetLineColor( 2 )
        histo_den__qcd_mu.SetMarkerColor( 2 )
        histo_num__qcd_mu.SetLineColor( 1 )
        histo_num__qcd_mu.SetFillColor( 7003 )

        v_bkg_hists = r.vector("TH1*")()
        v_bkg_hists.push_back( histo_num__qcd_mu )
        v_data_hists = r.vector("TH1*")()
        v_data_hists.push_back( histo_den__qcd_mu )

        if num_histname.find( "varbin" ):
            additional_optstring += "--divideByBinWidth"

        return r.plotmaker( """
                      --yTitle N leptons
                      --xTitle m_{T,#mu} [GeV]
                      --plotOutputName %s
                      --ratio_DrawOpt ep
                      --autoStack
                      --ratioPaneAtBottom
                      --showOverflow
                      --ratio_Maximum 1.7
                      %s
                      """%("plots/" + plotname + suffix, additional_optstring),
                      v_data_hists, v_bkg_hists )

    else:
        histo_num__qcd_el = rebin( tfile_qcd_el.Get(num_histname).Clone("Tight QCD") , num_histname)
        histo_den__qcd_el = rebin( tfile_qcd_el.Get(den_histname).Clone("Loose QCD") , den_histname)

        histo_den__qcd_el.SetLineColor( 2 )
        histo_den__qcd_el.SetMarkerColor( 2 )
        histo_num__qcd_el.SetLineColor( 1 )
        histo_num__qcd_el.SetFillColor( 7003 )

        v_bkg_hists = r.vector("TH1*")()
        v_bkg_hists.push_back( histo_num__qcd_el )
        v_data_hists = r.vector("TH1*")()
        v_data_hists.push_back( histo_den__qcd_el )

        if num_histname.find( "varbin" ):
            additional_optstring += "--divideByBinWidth"

        return r.plotmaker( """
                      --yTitle N leptons
                      --xTitle m_{T,#mu} [GeV]
                      --plotOutputName %s
                      --ratio_DrawOpt ep
                      --autoStack
                      --ratioPaneAtBottom
                      --showOverflow
                      --ratio_Maximum 1.7
                      %s
                      """%("plots/" + plotname + suffix, additional_optstring),
                      v_data_hists, v_bkg_hists )


def draw_ewk_v_qcd(histname, plotname, additional_optstring="", include_ttbar=False, rewgthist=None, flatrewgt=1.0, suffix=""):

    histo_num__data = rebin( tfile_data .Get(histname).Clone("Data") , histname)
    histo_num__dy   = rebin( tfile_dy   .Get(histname).Clone("DYskip") , histname)
    histo_num__wj   = rebin( tfile_wj   .Get(histname).Clone("Wskip") , histname)

    histo_num__data .SetDirectory( 0 )
    histo_num__dy   .SetDirectory( 0 )
    histo_num__wj   .SetDirectory( 0 )

    histo_num__data .SetLineColor( 1 )
    histo_num__data .SetFillColor( r.kGray + 1 )

    histo_num__dy.SetFillColor( 7002 )
    histo_num__wj.SetFillColor( 7006 )
    histo_num__dy.SetLineColor( 7002 )
    histo_num__wj.SetLineColor( 7006 )

    if rewgthist:
        num_rewgthist = rewgthist[0]
        for ibin in xrange( 0, histo_num__dy.GetNbinsX() + 1 ):
            histo_num__dy.SetBinContent( ibin + 1, histo_num__dy.GetBinContent( ibin + 1 ) * num_rewgthist.GetBinContent( ibin + 1 ) )
            histo_num__wj.SetBinContent( ibin + 1, histo_num__wj.GetBinContent( ibin + 1 ) * num_rewgthist.GetBinContent( ibin + 1 ) )

    histo_num__dy.Scale( -1 * flatrewgt )
    histo_num__wj.Scale( -1 * flatrewgt )

    v_data_hists = r.vector("TH1*")()
    v_data_hists.push_back( histo_num__data )
    v_data_hists.push_back( histo_num__dy )
    v_data_hists.push_back( histo_num__wj )

    histo_den__qcd_mu = tfile_qcd_mu.Get(histname).Clone("QCD")

    histo_den__qcd_mu .SetLineColor( 1 )
    histo_den__qcd_mu .SetFillColor( r.kGray + 1 )

    v_bkg_hists = r.vector("TH1*")()
    v_bkg_hists.push_back( histo_den__qcd_mu )

    if histname.find( "varbin" ):
        additional_optstring += "--divideByBinWidth"

    return r.plotmaker( """
                  --yTitle N leptons
                  --xTitle m_{T,#mu} [GeV]
                  --plotOutputName %s
                  --ratio_DrawOpt ep
                  --sumDataHists
                  --showOverflow
                  --autoStack
                  --ratioPaneAtBottom
                  --ratio_Maximum 1.7
                  %s
                  """%("plots/" + plotname + suffix, additional_optstring),
                  v_data_hists, v_bkg_hists )

import sys



def run_fakerate(var, lepflav, doflat, dottbar, magicsf, runlabel):

    #
    # Muon
    #

    if lepflav == "mu":
        nicename = "#mu"
    elif lepflav =="e":
        nicename = "e"
    
    
    print draw     ( "histo_mt_ewk_cr_"+lepflav               , """ --ratio_Maximum 1.9 --xTitle p_{T,%s} [GeV] --xNbin 1 """%nicename  , dottbar ,   False , None  , "")[0].GetBinContent(1)
    draw           ( "histo_mt_cr_"+lepflav                   , """ --ratio_Maximum 1.9 --xTitle p_{T,%s} [GeV]           """%nicename  , dottbar ,   False , None  , "")[0].GetBinContent(1)
    flat_sf  = draw( "histo_"+var+"_ewk_cr_"+lepflav          , """ --ratio_Maximum 1.9 --xTitle p_{T,%s} [GeV] --xNbin 1 """%nicename  , dottbar ,   False , None  , "")[0].GetBinContent(1)
    tight_sf = draw( "histo_"+var+"_ewk_cr_"+lepflav          , """ --ratio_Maximum 1.5 --ratio_Minimum 0.7 --xTitle p_{T,%s} [GeV] """%nicename            , dottbar ,   False , None  , "")
    loose_sf = draw( "histo_"+var+"_loose_ewk_cr_"+lepflav    , """ --ratio_Maximum 1.9 --xTitle p_{T,%s} [GeV] """%nicename            , dottbar ,   False , None  , "")
#    lbnt_sf  = draw( "histo_"+var+"_lbnt_highmet_cr_"+lepflav , """ --ratio_Maximum 1.9 --xTitle p_{T,%s} [GeV] """%nicename            , dottbar , dottbar , None  , "")
#    lbnt_sf  = draw( "histo_mt_lbnt_highmet_cr_"+lepflav      , """ --ratio_Maximum 1.9 --xTitle p_{T,%s} [GeV] """%nicename            , dottbar , dottbar , None  , "")
#    lbnt_sf  = draw( "histo_met_lbnt_highmet_cr_"+lepflav     , """ --ratio_Maximum 1.9 --xTitle p_{T,%s} [GeV] """%nicename  , dottbar , dottbar , None  , "")
#    lbnt_sf  = draw( "histo_"+var+"_lbnt_ewk_cr_"+lepflav     , """ --ratio_Maximum 1.9 --xTitle p_{T,%s} [GeV] """%nicename            , dottbar , dottbar , None  , "")
    draw           ( "histo_"+var+"_meas_"+lepflav            , """ --Maximum 30000 --reverseRatio --xTitle p_{T,%s} [GeV] """%nicename , dottbar ,   False , None  , "")
    draw           ( "histo_"+var+"_loose_meas_"+lepflav      , """ --Maximum 30000 --reverseRatio --xTitle p_{T,%s} [GeV] """%nicename , dottbar ,   False , None  , "")
    draw           ( "histo_"+var+"_meas_"+lepflav            , """ --Maximum 30000 --reverseRatio --xTitle p_{T,%s} [GeV] """%nicename , dottbar , dottbar , None  , "_wqcd")
    draw           ( "histo_"+var+"_loose_meas_"+lepflav      , """ --Maximum 30000 --reverseRatio --xTitle p_{T,%s} [GeV] --printData --printTotalBkg """%nicename , dottbar , dottbar , None  , "_wqcd")
    draw           ( "histo_"+var+"_ewk_cr_"+lepflav          , """ --ratio_Maximum 1.5 --ratio_Minimum 0.7 --xTitle p_{T,%s} [GeV] """%nicename            , dottbar , dottbar , None  , "_wqcd")
    draw           ( "histo_mt_highpt_"+lepflav               , """ --ratio_Maximum 1.9 --xTitle m_{T,%s} [GeV] """%nicename            , dottbar , dottbar , None  , "_wqcd")
    draw           ( "histo_met_hmt_"+lepflav                 , """ --ratio_Maximum 1.9 --xTitle MET [GeV] """                          , dottbar , dottbar , None  , "_wqcd")
    draw           ( "histo_"+var+"_hmt_"+lepflav             , """ --ratio_Maximum 1.9 --xTitle p_{T,%s} [GeV] """%nicename            , dottbar , dottbar , None  , "_wqcd")
    draw           ( "histo_pt_hmt_"+lepflav                  , """ --ratio_Maximum 1.9 --xTitle p_{T,%s} [GeV] """%nicename            , dottbar , dottbar , None  , "_wqcd")
    draw           ( "histo_met_highpt_"+lepflav              , """ --ratio_Minimum 0.7 --ratio_Maximum 1.3 --xTitle MET [GeV] """      , dottbar , dottbar , None  , "_wqcd")
    draw           ( "histo_met_medpt_"+lepflav               , """ --ratio_Minimum 0.7 --ratio_Maximum 1.3 --xTitle MET [GeV] """      , dottbar , dottbar , None  , "_wqcd")
    draw           ( "histo_met_highpt50_"+lepflav            , """ --ratio_Minimum 0.7 --ratio_Maximum 1.3 --xTitle MET [GeV] """      , dottbar , dottbar , None  , "_wqcd")[0].Print("all")
    draw           ( "histo_nvtx_highpt50_"+lepflav           , """ --ratio_Maximum 1.9 --xTitle N_{Vtx}           """                  , dottbar , dottbar , None  , "_wqcd")
    draw           ( "histo_met_hmt80pt50_"+lepflav           , """ --ratio_Minimum 0.7 --ratio_Maximum 1.3 --xTitle MET [GeV] """      , dottbar , dottbar , None  , "_wqcd")
    draw           ( "histo_met_hmt70pt50_"+lepflav           , """ --ratio_Minimum 0.7 --ratio_Maximum 1.3 --xTitle MET [GeV] """      , dottbar , dottbar , None  , "_wqcd")
    draw           ( "histo_met_hmt50pt50_"+lepflav           , """ --ratio_Minimum 0.7 --ratio_Maximum 1.3 --xTitle MET [GeV] """      , dottbar , dottbar , None  , "_wqcd")
    draw           ( "histo_met_hmt80pt30_"+lepflav           , """ --ratio_Minimum 0.7 --ratio_Maximum 1.3 --xTitle MET [GeV] """      , dottbar , dottbar , None  , "_wqcd")
    draw           ( "histo_met_hmt70pt30_"+lepflav           , """ --ratio_Minimum 0.7 --ratio_Maximum 1.3 --xTitle MET [GeV] """      , dottbar , dottbar , None  , "_wqcd")
    draw           ( "histo_met_hmt50pt30_"+lepflav           , """ --ratio_Minimum 0.7 --ratio_Maximum 1.3 --xTitle MET [GeV] """      , dottbar , dottbar , None  , "_wqcd")
    
    if doflat:
        print flat_sf
        fakerate_data = draw_ewk_subtr( "histo_"+var+"_meas_"+lepflav+"" , "histo_"+var+"_loose_meas_"+lepflav+"" , "histo_"+var+"_fakerate_data_"+lepflav+"" ,
                """ --ratio_yTitle Fake Rate --reverseRatio --ratio_Minimum 0.0 --ratio_Maximum 0.7 --MaximumMultiplier 3.0 --xTitle p_{T,%s} [GeV] """%nicename ,
                dottbar, None )
    else:
        tight_sf[0].Print("all")
        fakerate_data = draw_ewk_subtr( "histo_"+var+"_meas_"+lepflav+"" , "histo_"+var+"_loose_meas_"+lepflav+"" , "histo_"+var+"_fakerate_data_"+lepflav+"" ,
                """ --Minimum 1 --Maximum 20000000 --ratio_yTitle Fake Rate --reverseRatio --ratio_Minimum 0.0 --ratio_Maximum 0.7 --MaximumMultiplier 3.0 --xTitle p_{T,%s} [GeV] """%nicename ,
                dottbar , [None, None, magicsf, magicsf] )
                #dottbar , [tight_sf[0], tight_sf[0], 1.18, 1.18] )
                #dottbar , [None, None, 1.00, 1.00] )
                #dottbar , [tight_sf[0] , tight_sf[0], 1.10, 0.85] )
    fakerate_data_qcd = draw_qcd( "histo_"+var+"_meas_"+lepflav+""   , "histo_"+var+"_loose_meas_"+lepflav+"" , "histo_"+var+"_fakerate_qcd_"+lepflav+""  ,
            """ --Minimum 1 --Maximum 20000000 --ratio_yTitle Fake Rate --reverseRatio --ratio_Minimum 0.0 --ratio_Maximum 0.7 --xTitle p_{T,%s} [GeV] --MaximumMultiplier 3.0 """%nicename ,
            dottbar )
    
    fakerate_data[0].SetName( "EWK Subtr. Data" )
    fakerate_data[0].SetLineColor( 1 )
    fakerate_data[0].SetMarkerColor( 1 )
    fakerate_data_qcd[0].SetName( "QCD MC" )
    fakerate_data_qcd[0].SetLineColor( 2 )
    fakerate_data_qcd[0].SetMarkerColor( 2 )
    finalratio = r.plotmaker( """ --ratioPaneAtBottom --legend_bkgDrawOpt e2p --ratio_DrawOpt ep --data_DrawOpt ep --ratio_Maximum 3.3 --Maximum 0.3 --stack_DrawOpt ep --yTitle Fake rate --xTitle p_{T,%s} [GeV] --plotOutputName plots/fakerate_%s_%s """%(nicename,lepflav, runlabel), fakerate_data[0], fakerate_data_qcd[0])[0].Print("all")
    
    draw_ewk_v_qcd( "histo_"+var+"_meas_"+lepflav+"",       "histo_"+var+"_ewk_v_qcd_"+lepflav+"",       """ --ratio_Minimum 0.0 --ratio_Maximum 1.7 --Minimum 1 --Maximum 20000000 --MaximumMultiplier 2.0 --xTitle p_{T,%s} [GeV] """%nicename, True, [tight_sf[0],tight_sf[0]] )
    draw_ewk_v_qcd( "histo_"+var+"_loose_meas_"+lepflav+"", "histo_"+var+"_loose_ewk_v_qcd_"+lepflav+"", """ --ratio_Minimum 0.0 --ratio_Maximum 1.7 --Minimum 1 --Maximum 20000000 --MaximumMultiplier 2.0 --xTitle p_{T,%s} [GeV] """%nicename, True, [tight_sf[0],tight_sf[0]] )
    draw_ewk_v_qcd( "histo_"+var+"_lbnt_meas_"+lepflav+"", "histo_"+var+"_lbnt_ewk_v_qcd_"+lepflav+"", """ --ratio_Minimum 0.0 --ratio_Maximum 1.7 --MaximumMultiplier 2.0 --xTitle p_{T,%s} [GeV] """%nicename, True, [tight_sf[0],tight_sf[0]] )

    return finalratio

#run_fakerate( "ptvarbin", "mu", True )
nominal_result = run_fakerate( "ptvarbin", "mu", False, True, 1.18, "nominal" )
#syst_result    = run_fakerate( "ptvarbin", "mu", False, True, 1.00, "syst" )

sys.exit()
