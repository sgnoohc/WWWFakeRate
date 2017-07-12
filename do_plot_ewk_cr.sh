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
    --datahist 'outputs/fakerate_data.root:::histo_mt_cr_mu:::SetMarkerStyle=>19,SetLineColor=>1,SetName=>data%ep,Scale=>1' \
    --bkghist  'outputs/fakerate_wj.root  :::histo_mt_cr_mu:::SetFillColor=>7006,SetLineColor=>7006,SetName=>W%f,Scale=>1' \
    --bkghist  'outputs/fakerate_dy.root  :::histo_mt_cr_mu:::SetFillColor=>7002,SetLineColor=>7002,SetName=>DY%f,Scale=>1' \
    --bkghist  'outputs/fakerate_ttbar.root  :::histo_mt_cr_mu:::SetFillColor=>7004,SetLineColor=>7004,SetName=>t#bar{t}%f,Scale=>1' \
    --autostack \
    --maximum_scale 2 \
    --no_legend \
    --plottype plot1d \
    --plotname plots/plot_mt_cr_mu \
    --ratio_maximum 1.7 \
    --ratio_minimum 0.3

python ~/ditto/python/makeplot.py \
    --datahist 'outputs/fakerate_data.root:::histo_mt_cr_el:::SetMarkerStyle=>19,SetLineColor=>1,SetName=>data%ep,Scale=>1' \
    --bkghist  'outputs/fakerate_wj.root  :::histo_mt_cr_el:::SetFillColor=>7006,SetLineColor=>7006,SetName=>W%f,Scale=>1' \
    --bkghist  'outputs/fakerate_dy.root  :::histo_mt_cr_el:::SetFillColor=>7002,SetLineColor=>7002,SetName=>DY%f,Scale=>1' \
    --bkghist  'outputs/fakerate_ttbar.root  :::histo_mt_cr_mu:::SetFillColor=>7004,SetLineColor=>7004,SetName=>t#bar{t}%f,Scale=>1' \
    --autostack \
    --maximum_scale 2 \
    --no_legend \
    --plottype plot1d \
    --plotname plots/plot_mt_cr_el \
    --ratio_maximum 1.7 \
    --ratio_minimum 0.3
