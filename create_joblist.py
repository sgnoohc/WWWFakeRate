#!/bin/env python

import os
import sys
import glob

def splitList(lst, n):
    return [lst[i:i+n] for i in range(0,len(lst),n)]

def make_joblist(pattern):

    lepbabyrootfiles = glob.glob("/hadoop/cms/store/user/phchang/condor/forCommissioningv9/*"+pattern+"*.root")



    combinedlist = splitList(lepbabyrootfiles, 40)

    for index, dataset in enumerate(combinedlist):
        print ",".join(dataset), "fakerate_"+pattern+"_"+str(index)+".root"

make_joblist("DoubleMuon")
make_joblist("DoubleEG")
make_joblist("DY_madgraph")
make_joblist("TTbar")
make_joblist("WJets_madgraph")
