#!/usr/bin/env python                                                                                                      
import copy
import subprocess 
import ROOT
import re
from array import array
import argparse
import os

parser = argparse.ArgumentParser(
    "Create pre/post-fit plots for LFV H analysis")
parser.add_argument(
    "--filelist",
    type=str,
    action="store",
    dest="filelist",
    required=True,
    #default="datacards.txt",
    help="text file with names of datacards")
parser.add_argument(
    "--skipLimitCal",
    action="store_true",
    help="Skips limit calculation, assumes appropiately named root files with limits already exist ")
parser.add_argument(
    "--noBBB",
    action="store_true",
    help="indicates if no bin by bin used")
parser.add_argument(
    "--lumilist",
    type=str,
    action="store",
    dest="lumilist",
    default="75,100,125,175,225,300",
    help="list of lumivalues in fb to project to")
parser.add_argument(
    "--channel",
    type=str,
    action="store",
    dest="channel",
    default="et",
    help="Which channel to run over? (et, mt, em, me)")

args = parser.parse_args()

filename=args.filelist
channel=args.channel
lumilist=[float(lumi) for lumi in args.lumilist.split(',')]
lumilist.sort(reverse=True)

# Category map for the LaTeX naming of histograms                                                                                                             
catMap = {
    "em" : "e#tau_{#mu}",
    "et" : "e#tau_{h}",
    "mt" : "#mu#tau_{h}",
    "me" : "#mu#tau_{e}",
}

bbb=""

def add_nobbb():
    lowX=0.10
    lowY=0.835
    lumi  = ROOT.TPaveText(lowX, lowY+0.06, lowX+0.30, lowY+0.16, "NDC")
    lumi.SetBorderSize(   0 )
    lumi.SetFillStyle(    0 )
    lumi.SetTextAlign(   12 )
    lumi.SetTextColor(    1 )
    lumi.SetTextSize(0.04)
    lumi.SetTextFont (   42 )
    lumi.AddText("No bin-by-bin")
    return lumi

def add_channel(channel='me'):
    lowX=0.65
    lowY=0.835
    lumi  = ROOT.TPaveText(lowX, lowY+0.06, lowX+0.30, lowY+0.16, "NDC")
    lumi.SetBorderSize(   0 )
    lumi.SetFillStyle(    0 )
    lumi.SetTextAlign(   12 )
    lumi.SetTextColor(    1 )
    lumi.SetTextSize(0.04)
    lumi.SetTextFont (   42 )
    lumi.AddText(catMap[channel]+"-channel"+bbb+" (13 TeV)")
    return lumi

def add_CMS():
    lowX=0.28
    lowY=0.745
    lumi  = ROOT.TPaveText(lowX, lowY+0.06, lowX+0.15, lowY+0.16, "NDC")
    lumi.SetTextFont(61)
    lumi.SetTextSize(0.05)
    lumi.SetBorderSize(   0 )
    lumi.SetFillStyle(    0 )
    lumi.SetTextAlign(   12 )
    lumi.SetTextColor(    1 )
    lumi.AddText("CMS")
    return lumi

def add_Preliminary():
    lowX=0.28
    lowY=0.695
    lumi  = ROOT.TPaveText(lowX, lowY+0.06, lowX+0.15, lowY+0.16, "NDC")
    lumi.SetTextFont(52)
    lumi.SetTextSize(0.04)
    lumi.SetBorderSize(   0 )
    lumi.SetFillStyle(    0 )
    lumi.SetTextAlign(   12 )
    lumi.SetTextColor(    1 )
    lumi.AddText("Preliminary")
    return lumi

#filename='HMuTau_mutaue_combined_2016_bbb_m125_BDT_goldenjson_36fb.txt'
def add_rate_params(filename):
    if args.skipLimitCal:
        return
    file_handle=open(filename,'a')
    file_handle.write('\r\n')
    file_handle.write('lumiscale rateParam * * 1 \r\n')
    file_handle.write('nuisance edit freeze lumiscale \r\n')
    file_handle.write('\r\n')
    file_handle.close()




def produce_limits(datacard,category="combined",lumivalue=35.862,defaultlumi=35.862):
    if args.skipLimitCal:
        return
    
    lumiscale=lumivalue/defaultlumi

    if lumivalue==35.862:
        luminame='35p9'
    else:
        luminame=str(int(lumivalue))
        
    command="combine -M Asymptotic --run expected -C 0.95 -t -1 --minimizerStrategy 0 -n "+str(category+luminame)+" -m 125 --setPhysicsModelParameters lumiscale="+str(lumiscale)+"  "+datacard
    print command
    subprocess.call(command.split())


#produce_limits(args.filelist,lumivalue=300,luminame='300')


def extract_limit_values(category="combined",luminame='35p9'):
    limitfile=ROOT.TFile('higgsCombine'+category+luminame+'.Asymptotic.mH125.root')
    my_limits=[]
    for event in limitfile.limit:
        my_limits.append(event.limit)
    return my_limits


def doallLimits(cardfile,lumilist=[150,100],cat='combined'):
    ret_dict={}
    produce_limits(cardfile,category=cat)
    ret_dict[35.862]=extract_limit_values(category=cat)
    for lumi in lumilist:
        produce_limits(cardfile,category=cat,lumivalue=lumi)
        ret_dict[lumi]=extract_limit_values(category=cat,luminame=str(int(lumi)))
    return ret_dict

def make_tgraph(my_dict):
    ret_TGraph=ROOT.TGraph()
    count=0
    lumi_keys=my_dict.keys()
    lumi_keys.sort()
    for lumi in lumi_keys:
        ret_TGraph.SetPoint(count,lumi,my_dict[lumi][2])
        count+=1
    return ret_TGraph

cards_list=open('datacards.lst','r')
graph_list=[]



for line in cards_list.readlines():
    current_category=line.rstrip('\n').split(',')
    add_rate_params(current_category[0])
    current_dict=doallLimits(current_category[0],lumilist=lumilist,cat=current_category[1])
#    print current_dict
    current_graph=make_tgraph(current_dict)
    new_current_graph=copy.copy(current_graph)
    graph_list.append([new_current_graph,current_category[1]])
    

#print graph_list



canvas=ROOT.TCanvas("c1","A Simple Graph Example",200,10,700,500)
#canvas.SetGrid()
canvas.DrawFrame(0.2,0,320,2)


#canvas.cd()

my_legend=ROOT.TLegend(0.6,0.7,0.9,0.9)


color=1
#graph_list[0][0].SetTitle(graph_list[0][1])
#graph_list[0][0].SetName(graph_list[0][1])
#graph_list[0][0].SetLineColor(color)

my_mg=ROOT.TMultiGraph("mg","mg")
for j in range(len(graph_list)):
    graph_list[j][0].SetTitle(graph_list[j][1])
    graph_list[j][0].SetName(graph_list[j][1])
    graph_list[j][0].SetLineColor(color)
    graph_list[j][0].SetLineWidth(3)
    graph_list[j][0].SetMarkerStyle(21)
    my_legend.AddEntry(graph_list[j][0],graph_list[j][1],"lp")
    my_mg.Add(graph_list[j][0])
    color+=1
    canvas.Update()

#graph_list[0][0].Draw("C*")
my_mg.SetTitle("#mu#tau_e")
my_mg.Draw("CP")
my_legend.Draw('same')
prelim=add_Preliminary()
cms=add_CMS()
chan=add_channel()
prelim.Draw('same')
cms.Draw('same')
chan.Draw('same')
if args.noBBB:
    nobbb=add_nobbb()
    nobbb.Draw('same')
my_mg.GetXaxis().SetTitle("Luminosity [fb^{-1}]");
my_mg.GetYaxis().SetTitle("Median expected limit");

canvas.SaveAs("try.pdf")

"""
canvas2=ROOT.TCanvas("c2","A Simple Graph Example",200,10,700,500)
canvas2.cd()
graph_list[0][0].Draw()
canvas2.SaveAs("asd.pdf")


canvas3=ROOT.TCanvas("c3","A Simple Graph Example",200,10,700,500)
canvas3.cd()
graph_list[1][0].Draw()
canvas3.SaveAs("aasasd.pdf")
"""
