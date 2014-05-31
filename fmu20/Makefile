all:
	(cd src; $(MAKE))

clean:
	(cd src; $(MAKE) clean)

distclean: clean
	rm -f bin/fmusim_cs* bin/fmusim_me*
	rm -rf fmu
	find . -name "*~" -exec rm {} \;
	find . -name "#*~" -exec rm {} \;

test: test_cs test_me
test_cs:
	bin/fmusim_cs fmu/cs/bouncingBall.fmu
	bin/fmusim_cs fmu/cs/dq.fmu
	bin/fmusim_cs fmu/cs/inc.fmu
	bin/fmusim_cs fmu/cs/values.fmu
	bin/fmusim_cs fmu/cs/vanDerPol.fmu

test_me:
	bin/fmusim_me fmu/me/bouncingBall.fmu
	bin/fmusim_me fmu/me/dq.fmu
	bin/fmusim_me fmu/me/inc.fmu
	bin/fmusim_me fmu/me/values.fmu
	bin/fmusim_me fmu/me/vanDerPol.fmu

VALGRIND = valgrind
valgrind_test: valgrind_test_cs valgrind_test_me
valgrind_test_cs:
	$(VALGRIND) bin/fmusim_cs fmu/cs/bouncingBall.fmu
	$(VALGRIND) bin/fmusim_cs fmu/cs/dq.fmu
	$(VALGRIND) bin/fmusim_cs fmu/cs/inc.fmu
	$(VALGRIND) bin/fmusim_cs fmu/cs/values.fmu
	$(VALGRIND) bin/fmusim_cs fmu/cs/vanDerPol.fmu

valgrind_test_me:
	$(VALGRIND) bin/fmusim_me fmu/me/bouncingBall.fmu
	$(VALGRIND) bin/fmusim_me fmu/me/dq.fmu
	$(VALGRIND) bin/fmusim_me fmu/me/inc.fmu
	$(VALGRIND) bin/fmusim_me fmu/me/values.fmu
	$(VALGRIND) bin/fmusim_me fmu/me/vanDerPol.fmu
