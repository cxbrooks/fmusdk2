all:
	(cd fmu10; $(MAKE))
	(cd fmu20; $(MAKE))

clean:
	(cd fmu10; $(MAKE) clean)
	(cd fmu20; $(MAKE) clean)

distclean: clean
	rm -f fmu10/bin/fmusim_cs* fmu10/bin/fmusim_me*
	rm -f fmu20/bin/fmusim_cs* fmu20/bin/fmusim_me*
	rm -rf fmu10/fmu
	rm -rf fmu20/fmu
	find . -name "*~" -exec rm {} \;
	find . -name "#*~" -exec rm {} \;

run_all: run_all_fmu10 run_all_fmu20

#VALGRIND=valgrind
VALGRIND=

# Run all the fmu10 fmus.  Args are from run_all.bat
run_all_fmu10:
	$(VALGRIND) fmu10/bin/fmusim_me fmu10/fmu/me/bouncingBall.fmu 4 0.01 0 c
	mv result.csv result_me10_bouncingBall.csv
	$(VALGRIND) fmu10/bin/fmusim_cs fmu10/fmu/cs/bouncingBall.fmu 4 0.01 0 c
	mv result.csv result_cs10_bouncingBall.csv
	#
	$(VALGRIND) fmu10/bin/fmusim_me fmu10/fmu/me/vanDerPol.fmu 5 0.1 0 c
	mv result.csv result_me10_vanDerPol.csv
	$(VALGRIND) fmu10/bin/fmusim_cs fmu10/fmu/cs/vanDerPol.fmu 5 0.1 0 c
	mv result.csv result_cs10_vanDerPol.csv
	#
	$(VALGRIND) fmu10/bin/fmusim_me fmu10/fmu/me/dq.fmu 1 0.1 0 c
	mv result.csv result_me10_dq.csv
	$(VALGRIND) fmu10/bin/fmusim_cs fmu10/fmu/cs/dq.fmu 1 0.1 0 c
	mv result.csv result_cs10_dq.csv
	#
	$(VALGRIND) fmu10/bin/fmusim_me fmu10/fmu/me/inc.fmu 15 15 0 c
	mv result.csv result_me10_inc.csv
	$(VALGRIND) fmu10/bin/fmusim_cs fmu10/fmu/cs/inc.fmu 15 15 0 c
	mv result.csv result_cs10_inc.csv
	#
	$(VALGRIND) fmu10/bin/fmusim_me fmu10/fmu/me/values.fmu 12 0.3 0 c
	mv result.csv result_me10_values.csv
	$(VALGRIND) fmu10/bin/fmusim_cs fmu10/fmu/cs/values.fmu 12 0.3 0 c
	mv result.csv result_cs10_values.csv

# Run all the fmu20 fmus.  Args are from run_all.bat
run_all_fmu20:
	$(VALGRIND) fmu20/bin/fmusim_me fmu20/fmu/me/bouncingBall.fmu 4 0.01 0 c
	mv result.csv result_me20_bouncingBall.csv
	$(VALGRIND) fmu20/bin/fmusim_cs fmu20/fmu/cs/bouncingBall.fmu 4 0.01 0 c
	mv result.csv result_cs20_bouncingBall.csv
	#
	$(VALGRIND) fmu20/bin/fmusim_me fmu20/fmu/me/vanDerPol.fmu 5 0.1 0 c
	mv result.csv result_me20_vanDerPol.csv
	$(VALGRIND) fmu20/bin/fmusim_cs fmu20/fmu/cs/vanDerPol.fmu 5 0.1 0 c
	mv result.csv result_cs20_vanDerPol.csv
	#
	$(VALGRIND) fmu20/bin/fmusim_me fmu20/fmu/me/dq.fmu 1 0.1 0 c
	mv result.csv result_me20_dq.csv
	$(VALGRIND) fmu20/bin/fmusim_cs fmu20/fmu/cs/dq.fmu 1 0.1 0 c
	mv result.csv result_cs20_dq.csv
	#
	$(VALGRIND) fmu20/bin/fmusim_me fmu20/fmu/me/inc.fmu 15 15 0 c
	mv result.csv result_me20_inc.csv
	$(VALGRIND) fmu20/bin/fmusim_cs fmu20/fmu/cs/inc.fmu 15 15 0 c
	mv result.csv result_cs20_inc.csv
	#
	$(VALGRIND) fmu20/bin/fmusim_me fmu20/fmu/me/values.fmu 12 0.3 0 c
	mv result.csv result_me20_values.csv
	$(VALGRIND) fmu20/bin/fmusim_cs fmu20/fmu/cs/values.fmu 12 0.3 0 c
	mv result.csv result_cs20_values.csv

FMUCHECK_LOG = 3
fmuCheck: fmuCheck_fmu10 fmuCheck_fmu20
fmuCheck_fmu10:
	fmuCheck -o result_cs10_fmuCheck_bouncingBall.csv -s 4 -h 0.01 -l $(FMUCHECK_LOG) fmu10/fmu/cs/bouncingBall.fmu 
	fmuCheck -o result_me10_fmuCheck_bouncingBall.csv -s 4 -h 0.01 -l $(FMUCHECK_LOG) fmu10/fmu/me/bouncingBall.fmu 
	fmuCheck -o result_cs10_fmuCheck_vanDerPol.csv -s 5 -h 0.1 -l $(FMUCHECK_LOG) fmu10/fmu/cs/vanDerPol.fmu 
	fmuCheck -o result_me10_fmuCheck_vanDerPol.csv -s 5 -h 0.1 -l $(FMUCHECK_LOG) fmu10/fmu/me/vanDerPol.fmu 
	fmuCheck -o result_cs10_fmuCheck_dq.csv -s 1 -h 0.1 -l $(FMUCHECK_LOG) fmu10/fmu/cs/dq.fmu 
	fmuCheck -o result_me10_fmuCheck_dq.csv -s 1 -h 0.1 -l $(FMUCHECK_LOG) fmu10/fmu/me/dq.fmu 
	fmuCheck -o result_cs10_fmuCheck_inc.csv -s 15 -h 0.5 -l $(FMUCHECK_LOG) fmu10/fmu/cs/inc.fmu 
	fmuCheck -o result_me10_fmuCheck_inc.csv -s 15 -h 15 -l $(FMUCHECK_LOG) fmu10/fmu/me/inc.fmu 
	fmuCheck -o result_cs10_fmuCheck_values.csv -s 12 -h 0.3 -l $(FMUCHECK_LOG) fmu10/fmu/cs/values.fmu 
	fmuCheck -o result_me10_fmuCheck_values.csv -s 12 -h 12 -l $(FMUCHECK_LOG) fmu10/fmu/me/values.fmu 

fmuCheck_fmu20:
	fmuCheck -o result_cs20_fmuCheck_bouncingBall.csv -s 4 -h 0.01 -l $(FMUCHECK_LOG) fmu20/fmu/cs/bouncingBall.fmu 
	fmuCheck -o result_me20_fmuCheck_bouncingBall.csv -s 4 -h 0.01 -l $(FMUCHECK_LOG) fmu20/fmu/me/bouncingBall.fmu 
	fmuCheck -o result_cs20_fmuCheck_vanDerPol.csv -s 5 -h 0.1 -l $(FMUCHECK_LOG) fmu20/fmu/cs/vanDerPol.fmu 
	fmuCheck -o result_me20_fmuCheck_vanDerPol.csv -s 5 -h 0.1 -l $(FMUCHECK_LOG) fmu20/fmu/me/vanDerPol.fmu 
	fmuCheck -o result_cs20_fmuCheck_dq.csv -s 1 -h 0.1 -l $(FMUCHECK_LOG) fmu20/fmu/cs/dq.fmu 
	fmuCheck -o result_me20_fmuCheck_dq.csv -s 1 -h 0.1 -l $(FMUCHECK_LOG) fmu20/fmu/me/dq.fmu 
	fmuCheck -o result_cs20_fmuCheck_inc.csv -s 15 -h 0.5 -l $(FMUCHECK_LOG) fmu20/fmu/cs/inc.fmu 
	fmuCheck -o result_me20_fmuCheck_inc.csv -s 15 -h 15 -l $(FMUCHECK_LOG) fmu20/fmu/me/inc.fmu 
	fmuCheck -o result_cs20_fmuCheck_values.csv -s 12 -h 0.3 -l $(FMUCHECK_LOG) fmu20/fmu/cs/values.fmu 
	fmuCheck -o result_me20_fmuCheck_values.csv -s 12 -h 12 -l $(FMUCHECK_LOG) fmu20/fmu/me/values.fmu 

