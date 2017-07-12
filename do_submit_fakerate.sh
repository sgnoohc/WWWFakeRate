sh do_create_condor_tarball.sh -o condor_package.tgz code/ CORE/
~/public_html/tasutil/condor_concierge submit -e do_run_fakerate.sh -p condor_package.tgz -j joblists/joblist_ttbar.txt,joblists/joblist_2016doublemuon.txt,joblists/joblist_2016doubleeg.txt,joblists/joblist_dy.txt,joblists/joblist_wj.txt,joblists/joblist_qcd_mu.txt,joblists/joblist_qcd_el.txt
