echo "NOM"
combine -M AsymptoticLimits -m 125 HETau_etauhad_comb_new_2017_m125.txt --run blind
echo "BBB"
combine -M AsymptoticLimits -m 125 HETau_etauhad_comb_new_2017_m125.txt --run blind --freezeNuisanceGroups bbb
echo "THEO"
combine -M AsymptoticLimits -m 125 HETau_etauhad_comb_new_2017_m125.txt --run blind --freezeNuisanceGroups theo
echo "JES"
combine -M AsymptoticLimits -m 125 HETau_etauhad_comb_new_2017_m125.txt --run blind --freezeNuisanceGroups jes
echo "LES"
combine -M AsymptoticLimits -m 125 HETau_etauhad_comb_new_2017_m125.txt --run blind --freezeNuisanceGroups les
echo "BGNORM"
combine -M AsymptoticLimits -m 125 HETau_etauhad_comb_new_2017_m125.txt --run blind --freezeNuisanceGroups bgnorm
echo "FAKE"
combine -M AsymptoticLimits -m 125 HETau_etauhad_comb_new_2017_m125.txt --run blind --freezeNuisanceGroups fake
echo "REC"
combine -M AsymptoticLimits -m 125 HETau_etauhad_comb_new_2017_m125.txt --run blind --freezeNuisanceGroups rec
echo "EFF"
combine -M AsymptoticLimits -m 125 HETau_etauhad_comb_new_2017_m125.txt --run blind --freezeNuisanceGroups eff
echo "PU"
combine -M AsymptoticLimits -m 125 HETau_etauhad_comb_new_2017_m125.txt --run blind --freezeParameters CMS_Pileup_13TeV
echo "PF"
combine -M AsymptoticLimits -m 125 HETau_etauhad_comb_new_2017_m125.txt --run blind --freezeParameters CMS_Prefiring_13TeV
echo "UES"
combine -M AsymptoticLimits -m 125 HETau_etauhad_comb_new_2017_m125.txt --run blind --freezeParameters CMS_MET_Ues_13TeV
echo "LUMI"
combine -M AsymptoticLimits -m 125 HETau_etauhad_comb_new_2017_m125.txt --run blind --freezeParameters CMS_lumi_13TeV
echo "STAT"
combine -M AsymptoticLimits -m 125 HETau_etauhad_comb_2017_m125.txt --run blind -S 0

