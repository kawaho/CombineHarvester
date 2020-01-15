combineTool.py -M T2W -i HMuTau_mutauhad_2_2017_m125.txt -o HMuTau_mutauhad_2_2017_m125.root --parallel 4 -m 125
combineTool.py -M Impacts -d HMuTau_mutauhad_2_2017_m125.root -m 125 --doInitialFit --robustFit 1 --rMin=-5 --rMax=5 -t -1
combineTool.py -M Impacts -d HMuTau_mutauhad_2_2017_m125.root -m 125 --robustFit 1 --doFits --rMin=-5 --rMax=5 -t -1 --parallel 4
combineTool.py -M Impacts -d HMuTau_mutauhad_2_2017_m125.root -m 125 -o impacts.json -t -1
plotImpacts.py -i impacts.json -o impacts
