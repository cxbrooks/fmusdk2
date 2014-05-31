# fmusim makefile for Mac OS X

EXECS = \
	fmusim_cs \
	fmusim_me

# Build simulators for co_simulation and model_exchange and then build the .fmu files.
all: $(EXECS)
	(cd models; $(MAKE))

clean:
	rm -f $(EXECS)
	rm -rf  *.dSYM
	rm -f cosimulation/*.o
	rm -f model_exchange/*.o
	(cd models; $(MAKE) clean)

# Sources shared between co-simulation and model exchange
SHARED_SRCS = \
	shared/sim_support.c \

# Dependencies for only fmusim_cs
CO_SIMULATION_DEPS = \
	co_simulation/main.c

# Dependencies for only fmusim_me
MODEL_EXCHANGE_DEPS = \
	model_exchange/main.c

# Dependencies shared between both fmusim_cs and fmusim_me
SHARED_DEPS = \
	shared/fmi.h \
	shared/include/fmiFunctions.h \
	shared/include/fmiFunctionTypes.h \
	shared/include/fmiTypesPlatform.h \
	shared/parser/XmlElement.h \
	shared/parser/XmlParser.h \
	shared/parser/XmlParserCApi.h \
	shared/parser/XmlParserException.h

# Set CFLAGS to -m32 to build for linux32
#CFLAGS=-m32
# See also models/build_fmu

# Create the binaries in the current directory because co_simulation already has
# a directory named "fmusim_cs"
fmusim_cs: $(CO_SIMULATION_DEPS) $(SHARED_DEPS) ../bin/
	$(CC) $(CFLAGS) -g -Wall -DFMI_COSIMULATION \
		-Ishared/include -Ishared/parser/libxml -Ishared/parser -Ishared \
		co_simulation/main.c $(SHARED_SRCS) \
		-o $@ -lexpat -ldl
	cp fmusim_cs ../bin/

fmusim_me: $(MODEL_EXCHANGE_DEPS) $(SHARED_DEPS) ../bin/
	$(CC) $(CFLAGS) -g -Wall \
		-Ishared/include -Ishared/parser/libxml -Ishared/parser -Ishared \
		model_exchange/main.c $(SHARED_SRCS) \
		-o $@ -lexpat -ldl
	cp fmusim_me ../bin/

../bin/:
	if [ ! -d ../bin ]; then \
		echo "Creating ../bin/"; \
		mkdir ../bin/; \
	fi