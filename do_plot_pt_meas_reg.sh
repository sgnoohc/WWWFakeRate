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
    --datahist 'outputs/fakerate_data_mu.root:::histo_pt_meas_mu:::SetMarkerStyle=>19,SetLineColor=>1,SetName=>data%ep,Scale=>1' \
    --bkghist  'outputs/fakerate_wj.root     :::histo_pt_meas_mu:::SetFillColor=>7006,SetLineColor=>7006,SetName=>W%f,Scale=>1.02' \
    --bkghist  'outputs/fakerate_dy.root     :::histo_pt_meas_mu:::SetFillColor=>7002,SetLineColor=>7002,SetName=>DY%f,Scale=>1.02' \
    --bkghist  'outputs/fakerate_ttbar.root  :::histo_pt_meas_mu:::SetFillColor=>7004,SetLineColor=>7004,SetName=>t#bar{t}%f,Scale=>1.02' \
    --bkghist  'outputs/fakerate_qcd_mu.root :::histo_pt_meas_mu:::SetFillColor=>7003,SetLineColor=>7003,SetName=>QCD%f,Scale=>1.02' \
    --autostack \
    --maximum 3e6 \
    --minimum_logy 1e2 \
    --legendYlow 0.6 \
    --plottype plot1d \
    --plotname plots/plot_pt_meas_mu \
    --ratio_maximum 1.7 \
    --ratio_minimum 0.3

exit

python ~/ditto/python/makeplot.py \
    --datahist 'outputs/fakerate_data.root:::histo_pt_loose_meas_mu:::SetMarkerStyle=>19,SetLineColor=>1,SetName=>data%ep,Scale=>1' \
    --bkghist  'outputs/fakerate_wj.root  :::histo_pt_loose_meas_mu:::SetFillColor=>7006,SetLineColor=>7006,SetName=>W%f,Scale=>1.02' \
    --bkghist  'outputs/fakerate_dy.root  :::histo_pt_loose_meas_mu:::SetFillColor=>7002,SetLineColor=>7002,SetName=>DY%f,Scale=>1.02' \
    --autostack \
    --maximum_scale 20 \
    --no_legend \
    --plottype plot1d \
    --plotname plots/plot_pt_loose_meas_mu \
    --ratio_maximum 1.7 \
    --ratio_minimum 0.3

python ~/ditto/python/makeplot.py \
    --datahist 'outputs/fakerate_data.root:::histo_ptvarbin_meas_mu:::SetMarkerStyle=>19,SetLineColor=>1,SetName=>data%ep,Scale=>1' \
    --bkghist  'outputs/fakerate_wj.root  :::histo_ptvarbin_meas_mu:::SetFillColor=>7006,SetLineColor=>7006,SetName=>W%f,Scale=>1.02' \
    --bkghist  'outputs/fakerate_dy.root  :::histo_ptvarbin_meas_mu:::SetFillColor=>7002,SetLineColor=>7002,SetName=>DY%f,Scale=>1.02' \
    --autostack \
    --maximum_scale 14 \
    --no_legend \
    --plottype plot1d \
    --plotname plots/plot_ptvarbin_meas_mu \
    --ratio_maximum 1.7 \
    --ratio_minimum 0.3

python ~/ditto/python/makeplot.py \
    --datahist 'outputs/fakerate_data.root:::histo_ptvarbin_loose_meas_mu:::SetMarkerStyle=>19,SetLineColor=>1,SetName=>data%ep,Scale=>1' \
    --bkghist  'outputs/fakerate_wj.root  :::histo_ptvarbin_loose_meas_mu:::SetFillColor=>7006,SetLineColor=>7006,SetName=>W%f,Scale=>1.02' \
    --bkghist  'outputs/fakerate_dy.root  :::histo_ptvarbin_loose_meas_mu:::SetFillColor=>7002,SetLineColor=>7002,SetName=>DY%f,Scale=>1.02' \
    --autostack \
    --maximum_scale 20 \
    --no_legend \
    --plottype plot1d \
    --plotname plots/plot_ptvarbin_loose_meas_mu \
    --ratio_maximum 1.7 \
    --ratio_minimum 0.3

python ~/ditto/python/makeplot.py \
    --datahist 'outputs/fakerate_data.root:::histo_pt_meas_el:::SetMarkerStyle=>19,SetLineColor=>1,SetName=>data%ep,Scale=>1' \
    --bkghist  'outputs/fakerate_wj.root  :::histo_pt_meas_el:::SetFillColor=>7006,SetLineColor=>7006,SetName=>W%f,Scale=>1.10' \
    --bkghist  'outputs/fakerate_dy.root  :::histo_pt_meas_el:::SetFillColor=>7002,SetLineColor=>7002,SetName=>DY%f,Scale=>1.10' \
    --bkghist  'outputs/fakerate_qcd_el.root :::histo_pt_meas_el:::SetFillColor=>7003,SetLineColor=>7003,SetName=>QCD%f,Scale=>1.0' \
    --autostack \
    --maximum_scale 14 \
    --no_legend \
    --plottype plot1d \
    --plotname plots/plot_pt_meas_el \
    --ratio_maximum 1.7 \
    --ratio_minimum 0.3

python ~/ditto/python/makeplot.py \
    --datahist 'outputs/fakerate_data.root:::histo_pt_loose_meas_el:::SetMarkerStyle=>19,SetLineColor=>1,SetName=>data%ep,Scale=>1' \
    --bkghist  'outputs/fakerate_wj.root  :::histo_pt_loose_meas_el:::SetFillColor=>7006,SetLineColor=>7006,SetName=>W%f,Scale=>1.10' \
    --bkghist  'outputs/fakerate_dy.root  :::histo_pt_loose_meas_el:::SetFillColor=>7002,SetLineColor=>7002,SetName=>DY%f,Scale=>1.10' \
    --bkghist  'outputs/fakerate_qcd_el.root :::histo_pt_loose_meas_el:::SetFillColor=>7003,SetLineColor=>7003,SetName=>QCD%f,Scale=>1.0' \
    --autostack \
    --autostack \
    --maximum_scale 20 \
    --no_legend \
    --plottype plot1d \
    --plotname plots/plot_pt_loose_meas_el \
    --ratio_maximum 1.7 \
    --ratio_minimum 0.3

python ~/ditto/python/makeplot.py \
    --datahist 'outputs/fakerate_data.root:::histo_ptvarbin_meas_el:::SetMarkerStyle=>19,SetLineColor=>1,SetName=>data%ep,Scale=>1' \
    --bkghist  'outputs/fakerate_wj.root  :::histo_ptvarbin_meas_el:::SetFillColor=>7006,SetLineColor=>7006,SetName=>W%f,Scale=>1.10' \
    --bkghist  'outputs/fakerate_dy.root  :::histo_ptvarbin_meas_el:::SetFillColor=>7002,SetLineColor=>7002,SetName=>DY%f,Scale=>1.10' \
    --autostack \
    --maximum_scale 14 \
    --no_legend \
    --plottype plot1d \
    --plotname plots/plot_ptvarbin_meas_el \
    --ratio_maximum 1.7 \
    --ratio_minimum 0.3

python ~/ditto/python/makeplot.py \
    --datahist 'outputs/fakerate_data.root:::histo_ptvarbin_loose_meas_el:::SetMarkerStyle=>19,SetLineColor=>1,SetName=>data%ep,Scale=>1' \
    --bkghist  'outputs/fakerate_wj.root  :::histo_ptvarbin_loose_meas_el:::SetFillColor=>7006,SetLineColor=>7006,SetName=>W%f,Scale=>1.10' \
    --bkghist  'outputs/fakerate_dy.root  :::histo_ptvarbin_loose_meas_el:::SetFillColor=>7002,SetLineColor=>7002,SetName=>DY%f,Scale=>1.10' \
    --autostack \
    --maximum_scale 20 \
    --no_legend \
    --plottype plot1d \
    --plotname plots/plot_ptvarbin_loose_meas_el \
    --ratio_maximum 1.7 \
    --ratio_minimum 0.3
