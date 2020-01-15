#!/bin/bash

#$DIR="MyDir/"
#$CARD="mycard.txt"

#combineCards.py -S $DIR$CARD > card.txt



text2workspace.py  HMuTau_mutaue_combined_2016_bbb_m125_BDT_goldenjson_36fb.txt -m 125

combineTool.py -M Impacts -d HMuTau_mutaue_combined_2016_bbb_m125_BDT_goldenjson_36fb.root -m 125 --doInitialFit --robustFit 1 --rMax 5 --rMin -5

combineTool.py -M Impacts -d HMuTau_mutaue_combined_2016_bbb_m125_BDT_goldenjson_36fb.root -m 125 --robustFit 1 --doFits   --rMax 5 --rMin -5  --parallel 30 

combineTool.py -M Impacts -d HMuTau_mutaue_combined_2016_bbb_m125_BDT_goldenjson_36fb.root -m 125 -o "myimpactsbbb_combined_BDT.json"
plotImpacts.py -i "myimpactsbbb_combined_BDT.json" --transparent -o myimpactsbbb_combined_BDT
