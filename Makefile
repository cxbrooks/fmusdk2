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
