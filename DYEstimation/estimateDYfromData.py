#! /usr/bin/env python

import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.Reset()
from cp3_llbb.CommonTools.HistogramTools import getHistogramsFromFileRegex

#import os, sys, inspect
#scriptDir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
#sys.path.append(scriptDir)
from subtractMCfromData import performSubtraction, addHistoDicos

import argparse

parser = argparse.ArgumentParser(description="Subtract MC from data for all histograms in a file.")
parser.add_argument("-d", "--data", nargs='+', help="Input files for data", required=True)
parser.add_argument("--dy", nargs='+', help="Input files for DY", required=True)
parser.add_argument("-m", "--mc", nargs='+', help="Input files for MC", required=True)
parser.add_argument("-o", "--output", help="Output file", required=True)
parser.add_argument("-s", "--syst", help="Take care of systematics", action="store_true")
options = parser.parse_args()

LUMI = 36809

# Get SF histograms and perform the subtraction
print "Subtracting MuMu histograms"
histos_mumu = performSubtraction(options.data, options.mc, R".*MuMu.*_with_nobtag_to_btagM_reweighting$", LUMI)
print "Subtracting ElEl histograms"
histos_ee = performSubtraction(options.data, options.mc, R".*ElEl.*_with_nobtag_to_btagM_reweighting$", LUMI)

if options.syst:
    def build_syst(_list):
        _ret = []
        for _s in _list:
            _ret.append(_s + "up")
            _ret.append(_s + "down")
    def build_scale(_s):
        return [ _s + str(i) for i in range(6) ]

    # Systematics applied on both Data and MC
    systematics = ["jec", "jer", "jjbtaglight", "jjbtagheavy", "pu", "dyStat"]
    for _s in build_syst(systematics) + build_scale("dyScaleUncorr"):
        histos_mumu.append(performSubtraction(options.data, options.mc, R".*MuMu.*_with_nobtag_to_btagM_reweighting__" + _s + "$" , LUMI)
        histos_e.append(performSubtraction(options.data, options.mc, R".*ElEl.*_with_nobtag_to_btagM_reweighting__" + _s + "$" , LUMI)

    # Systematics for which only MC is affected, not the reweighting
    systematics = ["elidiso", "muid", "muiso", "trigeff", "pdf"]
    for _s in build_syst(systematic) + build_scale("scaleUncorr"):
        histos_mumu.append(performSubtraction(options.data, options.mc, R".*MuMu.*_with_nobtag_to_btagM_reweighting$" , LUMI, mc_regexp=R".*MuMu.*_with_nobtag_to_btagM_reweighting__" + _s + "$", translation=(R"$", R"__" + _s)))
        histos_ee.append(performSubtraction(options.data, options.mc, R".*ElEl.*_with_nobtag_to_btagM_reweighting$" , LUMI, mc_regexp=R".*ElEl.*_with_nobtag_to_btagM_reweighting__" + _s + "$", translation=(R"$", R"__" + _s)))
        

# For MuE, get histograms from MC
print "Copying MuEl histograms"
histos_emu = getHistogramsFromFileRegex(options.dy[0], R".*MuEl.*btagM.*")
for file in options.dy[1:]:
    this_histos = getHistogramsFromFileRegex(file, R".*MuEl.*btagM.*")
    addHistoDicos(histos_emu, this_histos)

## Rename to have the same name as the SF ones
#for h in histos_emu.values():
#    old_name = h.GetName()
#    new_name = old_name.replace("btagM", "nobtag").replace("cut", "cut_with_nobtag_to_btagM_reweighting")
#    h.SetName(new_name)

# Rename to have the same name as the other MC and data
for h in histos_ee.values() + histos_mumu.values():
    old_name = h.GetName()
    new_name = old_name.replace("_with_nobtag_to_btagM_reweighting", "").replace("nobtag", "btagM")
    h.SetName(new_name)

print "Writing output"
# Write output
r_file = ROOT.TFile.Open(options.output, "recreate")
for hist in histos_mumu.values() + histos_ee.values() + histos_emu.values():
    hist.Write()
r_file.Close()
