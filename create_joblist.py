#!/bin/env python

import os
import sys
import glob

def splitList(lst, n):
    return [lst[i:i+n] for i in range(0,len(lst),n)]

def make_joblist(pattern):
    lepbabyrootfiles = glob.glob("/hadoop/cms/store/user/phchang/condor/forCommissioningv9/*"+pattern+"*.root")
    combinedlist = splitList(lepbabyrootfiles, 30)
    for index, dataset in enumerate(combinedlist):
        print ",".join(dataset), "fakerate_"+pattern+"_"+str(index)+".root"

def make_joblist_v7(pattern):
    lepbabyrootfiles = glob.glob("/hadoop/cms/store/user/phchang/condor/forCommissioningv7/*"+pattern+"*.root")
    combinedlist = splitList(lepbabyrootfiles, 30)
    for index, dataset in enumerate(combinedlist):
        print ",".join(dataset), "fakerate_"+pattern+"_"+str(index)+".root"

make_joblist("DoubleMuon")
make_joblist("DoubleEG")
make_joblist("DY_madgraph")
make_joblist("TTbar")
make_joblist("WJets_madgraph")
make_joblist("WW")
make_joblist("WZ")
make_joblist("ZZ")
make_joblist_v7("QCD_EM")
make_joblist_v7("QCD_bcToE")
make_joblist_v7("QCD_Mu")
