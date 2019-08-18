combineTool.py -M T2W -i HMuTau_mutauhad_3_2016_bbb_m125_Sig_Limit.txt -o HMuTau_mutauhad_3_2016_bbb_m125_Sig_Limit.root --parallel 4 -m 125
combineTool.py -M Impacts -d HMuTau_mutauhad_3_2016_bbb_m125_Sig_Limit.root -m 125 --doInitialFit --robustFit 1 --rMin=-5 --rMax=5 -t -1
combineTool.py -M Impacts -d HMuTau_mutauhad_3_2016_bbb_m125_Sig_Limit.root -m 125 --robustFit 1 --doFits --rMin=-5 --rMax=5 -t -1 --parallel 4
combineTool.py -M Impacts -d HMuTau_mutauhad_3_2016_bbb_m125_Sig_Limit.root -m 125 -o impacts.json -t -1
plotImpacts.py -i impacts.json -o impacts
