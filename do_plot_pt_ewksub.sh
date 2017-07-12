#!/bin/bash

if $(type root &> /dev/null); then
    :
else
    echo "Setting up ROOT"
    export SCRAM_ARCH=slc6_amd64_gcc491   # or whatever scram_arch you need for your desired CMSSW release
    export CMSSW_VERSION=CMSSW_7_4_2
    source /cvmfs/cms.cern.ch/cmsset_default.sh
    cd /cvmfs/cms.cern.ch/$SCRAM_ARCH/cms/cmssw/$CMSSW_VERSION/src
    eval `scramv1 runtime -sh`
    cd -
fi

echo 'which root'
which root

python ~/ditto/python/makeplot.py \
    --bkghist 'outputs/fakerate_data_mu.root  :::histo_pt_meas_mu:::SetMarkerStyle=>19,SetFillColor=>920,SetLineColor=>1,SetName=>Tight%f,Scale=>1' \
    --bkghist 'outputs/fakerate_wj.root    :::histo_pt_meas_mu:::SetFillColor=>7006,SetLineColor=>7006,SetName=>W%f%skip,Scale=>-1.02' \
    --bkghist 'outputs/fakerate_dy.root    :::histo_pt_meas_mu:::SetFillColor=>7002,SetLineColor=>7002,SetName=>DY%f%skip,Scale=>-1.02' \
    --bkghist 'outputs/fakerate_ttbar.root :::histo_pt_meas_mu:::SetFillColor=>7004,SetLineColor=>7004,SetName=>t#bar{t}%f%skip,Scale=>-1.02' \
    --datahist 'outputs/fakerate_data_mu.root :::histo_pt_loose_meas_mu:::SetMarkerStyle=>19,SetLineColor=>1,SetName=>Loose%ep,Scale=>1' \
    --datahist 'outputs/fakerate_wj.root   :::histo_pt_loose_meas_mu:::SetFillColor=>7006,SetLineColor=>7006,SetName=>W%f%skip,Scale=>-1.02' \
    --datahist 'outputs/fakerate_dy.root   :::histo_pt_loose_meas_mu:::SetFillColor=>7002,SetLineColor=>7002,SetName=>DY%f%skip,Scale=>-1.02' \
    --datahist 'outputs/fakerate_ttbar.root:::histo_pt_loose_meas_mu:::SetFillColor=>7004,SetLineColor=>7004,SetName=>t#bar{t}%f%skip,Scale=>-1.02' \
    --maximum_scale 0.2 \
    --XTitle '#it{p}_{T,#mu} [GeV]' \
    --YTitle '# of leptons' \
    --autostack \
    --reverse_ratio \
    --show_overflow \
    --legendYlow 0.5 \
    --plottype plot1dratio \
    --plotname plots/plot_pt_ewksub_mu \
    --ratio_draw_style ep \
    --ratio_pane_on_top \
    --ratio_maximum 0.7 \
    --ratio_minimum 0.0

python ~/ditto/python/makeplot.py \
    --bkghist 'outputs/fakerate_data.root:::histo_ptvarbin_meas_mu:::SetMarkerStyle=>19,SetFillColor=>920,SetLineColor=>1,SetName=>Tight%f,Scale=>1' \
    --bkghist 'outputs/fakerate_wj.root  :::histo_ptvarbin_meas_mu:::SetFillColor=>7006,SetLineColor=>7006,SetName=>W%f%skip,Scale=>-1.02' \
    --bkghist 'outputs/fakerate_dy.root  :::histo_ptvarbin_meas_mu:::SetFillColor=>7002,SetLineColor=>7002,SetName=>DY%f%skip,Scale=>-1.02' \
    --bkghist 'outputs/fakerate_ttbar.root :::histo_ptvarbin_meas_mu:::SetFillColor=>7004,SetLineColor=>7004,SetName=>t#bar{t}%f%skip,Scale=>-1.02' \
    --datahist 'outputs/fakerate_data.root:::histo_ptvarbin_loose_meas_mu:::SetMarkerStyle=>19,SetLineColor=>1,SetName=>Loose%ep,Scale=>1' \
    --datahist 'outputs/fakerate_wj.root  :::histo_ptvarbin_loose_meas_mu:::SetFillColor=>7006,SetLineColor=>7006,SetName=>W%f%skip,Scale=>-1.02' \
    --datahist 'outputs/fakerate_dy.root  :::histo_ptvarbin_loose_meas_mu:::SetFillColor=>7002,SetLineColor=>7002,SetName=>DY%f%skip,Scale=>-1.02' \
    --datahist 'outputs/fakerate_ttbar.root:::histo_ptvarbin_loose_meas_mu:::SetFillColor=>7004,SetLineColor=>7004,SetName=>t#bar{t}%f%skip,Scale=>-1.02' \
    --maximum_scale 5 \
    --XTitle '#it{p}_{T,#mu} [GeV]' \
    --YTitle '# of leptons' \
    --autostack \
    --reverse_ratio \
    --show_overflow \
    --legendYlow 0.5 \
    --plottype plot1dratio \
    --plotname plots/plot_ptvarbin_ewksub_mu \
    --ratio_draw_style ep \
    --ratio_pane_on_top \
    --ratio_maximum 0.7 \
    --ratio_minimum 0.0

python ~/ditto/python/makeplot.py \
    --bkghist 'outputs/fakerate_data.root:::histo_pt_meas_el:::SetMarkerStyle=>19,SetFillColor=>920,SetLineColor=>1,SetName=>Tight%f,Scale=>1' \
    --bkghist 'outputs/fakerate_wj.root  :::histo_pt_meas_el:::SetFillColor=>7006,SetLineColor=>7006,SetName=>W%f%skip,Scale=>-1.02' \
    --bkghist 'outputs/fakerate_dy.root  :::histo_pt_meas_el:::SetFillColor=>7002,SetLineColor=>7002,SetName=>DY%f%skip,Scale=>-1.02' \
    --datahist 'outputs/fakerate_data.root:::histo_pt_loose_meas_el:::SetMarkerStyle=>19,SetLineColor=>1,SetName=>Loose%ep,Scale=>1' \
    --datahist 'outputs/fakerate_wj.root  :::histo_pt_loose_meas_el:::SetFillColor=>7006,SetLineColor=>7006,SetName=>W%f%skip,Scale=>-1.02' \
    --datahist 'outputs/fakerate_dy.root  :::histo_pt_loose_meas_el:::SetFillColor=>7002,SetLineColor=>7002,SetName=>DY%f%skip,Scale=>-1.02' \
    --maximum_scale 5 \
    --XTitle '#it{p}_{T,#mu} [GeV]' \
    --YTitle '# of leptons' \
    --autostack \
    --reverse_ratio \
    --show_overflow \
    --legendYlow 0.5 \
    --plottype plot1dratio \
    --plotname plots/plot_pt_ewksub_el \
    --ratio_draw_style ep \
    --ratio_pane_on_top \
    --ratio_maximum 0.7 \
    --ratio_minimum 0.0

python ~/ditto/python/makeplot.py \
    --bkghist 'outputs/fakerate_data.root:::histo_ptvarbin_meas_el:::SetMarkerStyle=>19,SetFillColor=>920,SetLineColor=>1,SetName=>Tight%f,Scale=>1' \
    --bkghist 'outputs/fakerate_wj.root  :::histo_ptvarbin_meas_el:::SetFillColor=>7006,SetLineColor=>7006,SetName=>W%f%skip,Scale=>-1.02' \
    --bkghist 'outputs/fakerate_dy.root  :::histo_ptvarbin_meas_el:::SetFillColor=>7002,SetLineColor=>7002,SetName=>DY%f%skip,Scale=>-1.02' \
    --datahist 'outputs/fakerate_data.root:::histo_ptvarbin_loose_meas_el:::SetMarkerStyle=>19,SetLineColor=>1,SetName=>Loose%ep,Scale=>1' \
    --datahist 'outputs/fakerate_wj.root  :::histo_ptvarbin_loose_meas_el:::SetFillColor=>7006,SetLineColor=>7006,SetName=>W%f%skip,Scale=>-1.02' \
    --datahist 'outputs/fakerate_dy.root  :::histo_ptvarbin_loose_meas_el:::SetFillColor=>7002,SetLineColor=>7002,SetName=>DY%f%skip,Scale=>-1.02' \
    --maximum_scale 5 \
    --XTitle '#it{p}_{T,#mu} [GeV]' \
    --YTitle '# of leptons' \
    --autostack \
    --reverse_ratio \
    --show_overflow \
    --legendYlow 0.5 \
    --plottype plot1dratio \
    --plotname plots/plot_ptvarbin_ewksub_el \
    --ratio_draw_style ep \
    --ratio_pane_on_top \
    --ratio_maximum 0.7 \
    --ratio_minimum 0.0

