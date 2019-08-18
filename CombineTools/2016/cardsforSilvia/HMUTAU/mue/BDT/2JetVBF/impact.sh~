combineTool.py -M T2W -i HMuTau_mutaue_2_2016_bbb_m125_BDT_goldenjson_36fb.txt -o lfv_me_2_13TeV.root --parallel 4 -m 125
combineTool.py -M Impacts -d lfv_me_2_13TeV.root -m 125 --doInitialFit --robustFit 1 --rMin=-5 --rMax=5 -t -1
combineTool.py -M Impacts -d lfv_me_2_13TeV.root -m 125 --robustFit 1 --doFits --rMin=-5 --rMax=5 -t -1 --parallel 4
combineTool.py -M Impacts -d lfv_me_2_13TeV.root -m 125 -o impacts.json -t -1
plotImpacts.py -i impacts.json -o impacts
