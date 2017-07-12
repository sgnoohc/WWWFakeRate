#python ~/ditto/python/makeplot.py \
#    --datahist 'haddoutput/SingleMuon.root:::HLT_Mu50_v_onZ__h_mll_zoomedcoarse_on_Zpeak_mm:::SetMarkerStyle=>19,SetLineColor=>1,SetName=>data%ep,Scale=>1' \
#    --bkghist  'haddoutput/dy.root        :::HLT_Mu50_v_onZ__h_mll_zoomedcoarse_on_Zpeak_mm:::SetFillColor=>2003,SetLineColor=>1,SetName=>DY%f,Scale=>0.26' \
#    --plottype plot1dratio \
#    --plotname plot \
#    --ratio_maximum 1.7 \
#    --ratio_minimum 0.3

python ~/ditto/python/makeplot.py \
    --datahist 'haddoutput/SinglePhoton.root:::SinglePh_h_nmu_npheq1:::SetMarkerStyle=>19,SetLineColor=>1,SetName=>data%ep,Scale=>1' \
    --plottype plot1d \
    --plotname plot \
    --ratio_maximum 1.7 \
    --ratio_minimum 0.3
