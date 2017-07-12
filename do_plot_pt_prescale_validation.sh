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
    --datahist 'outputs/fakerate_data.root:::histo_pt_mu:::SetMarkerStyle=>19,SetLineColor=>1,SetName=>data%ep,Scale=>1' \
    --nbinx 200 \
    --plottype plot1d \
    --plotname plots/plot_pt_mu \
    --canvas_logy \
    --ratio_maximum 1.7 \
    --ratio_minimum 0.3
python ~/ditto/python/makeplot.py \
    --datahist 'outputs/fakerate_data.root:::histo_nowgt_pt_mu:::SetMarkerStyle=>19,SetLineColor=>1,SetName=>data%ep,Scale=>1' \
    --nbinx 200 \
    --plottype plot1d \
    --plotname plots/plot_nowgt_pt_mu \
    --canvas_logy \
    --ratio_maximum 1.7 \
    --ratio_minimum 0.3

python ~/ditto/python/makeplot.py \
    --datahist 'outputs/fakerate_data.root:::histo_pt_el:::SetMarkerStyle=>19,SetLineColor=>1,SetName=>data%ep,Scale=>1' \
    --nbinx 200 \
    --plottype plot1d \
    --plotname plots/plot_pt_el \
    --ratio_maximum 1.7 \
    --ratio_minimum 0.3
python ~/ditto/python/makeplot.py \
    --datahist 'outputs/fakerate_data.root:::histo_nowgt_pt_el:::SetMarkerStyle=>19,SetLineColor=>1,SetName=>data%ep,Scale=>1' \
    --nbinx 200 \
    --plottype plot1d \
    --plotname plots/plot_nowgt_pt_el \
    --canvas_logy \
    --ratio_maximum 1.7 \
    --ratio_minimum 0.3
