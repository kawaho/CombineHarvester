text2workspace.py HETau_etauhad_comb_2017_m125.txt -m 125 --X-nuisance-function 'CMS*' 'expr::lumisyst("1/sqrt(@0)",lumiscale[1])'
mv HETau_etauhad_comb_2017_m125.root HETau_etauhad_comb41.8_2017_m125.root
text2workspace.py HETau_etauhad_comb_2017_m125.txt -m 125 --X-nuisance-function 'CMS*' 'expr::lumisyst("1/sqrt(@0)",lumiscale[1.41])'
mv HETau_etauhad_comb_2017_m125.root HETau_etauhad_comb59_2017_m125.root
text2workspace.py HETau_etauhad_comb_2017_m125.txt -m 125 --X-nuisance-function 'CMS*' 'expr::lumisyst("1/sqrt(@0)",lumiscale[1.84])'
mv HETau_etauhad_comb_2017_m125.root HETau_etauhad_comb77_2017_m125.root
text2workspace.py HETau_etauhad_comb_2017_m125.txt -m 125 --X-nuisance-function 'CMS*' 'expr::lumisyst("1/sqrt(@0)",lumiscale[3.25])'
mv HETau_etauhad_comb_2017_m125.root HETau_etauhad_comb136_2017_m125.root

combine -M AsymptoticLimits -m 125 HETau_etauhad_comb41.8_2017_m125.root --run blind
mv higgsCombineTest.AsymptoticLimits.mH125.root higgsCombinecombined42.AsymptoticLimits.mH125.root
combine -M AsymptoticLimits -m 125 HETau_etauhad_comb59_2017_m125.root --run blind
mv higgsCombineTest.AsymptoticLimits.mH125.root higgsCombinecombined59.AsymptoticLimits.mH125.root
combine -M AsymptoticLimits -m 125 HETau_etauhad_comb77_2017_m125.root --run blind
mv higgsCombineTest.AsymptoticLimits.mH125.root higgsCombinecombined77.AsymptoticLimits.mH125.root
combine -M AsymptoticLimits -m 125 HETau_etauhad_comb136_2017_m125.root --run blind
mv higgsCombineTest.AsymptoticLimits.mH125.root higgsCombinecombined136.AsymptoticLimits.mH125.root
