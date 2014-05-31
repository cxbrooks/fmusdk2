/* ------------------------------------------------------------------------- 
 * main.c
 * Implements simulation of a single FMU instance using the forward Euler
 * method for numerical integration.
 * Command syntax: see printHelp()
 * Simulates the given FMU from t = 0 .. tEnd with fixed step size h and 
 * writes the computed solution to file 'result.csv'.
 * The CSV file (comma-separated values) may e.g. be plotted using 
 * OpenOffice Calc or Microsoft Excel. 
 * This program demonstrates basic use of an FMU.
 * Real applications may use advanced numerical solvers instead, means to 
 * exactly locate state events in time, graphical plotting utilities, support 
 * for co-execution of many FMUs, stepping and debug support, user control
 * of parameter and start values etc. 
 * All this is missing here.
 *
 * Revision history
 *  07.03.2014 initial version released in FMU SDK 2.0.0
 *
 * Free libraries and tools used to implement this simulator:
 *  - header files from the FMU specification
 *  - libxml2 XML parser, see http://xmlsoft.org
 *  - 7z.exe 4.57 zip and unzip tool, see http://www.7-zip.org
 * Author: Adrian Tirea
 * Copyright QTronic GmbH. All rights reserved.
 * -------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include "fmi.h"
#include "sim_support.h"

FMU fmu; // the fmu to simulate

// simulate the given FMU using the forward euler method.
// time events are processed by reducing step size to exactly hit tNext.
// state events are checked and fired only at the end of an Euler step. 
// the simulator may therefore miss state events and fires state events typically too late.
static int simulate(FMU* fmu, double tEnd, double h, fmiBoolean loggingOn, char separator,
                    int nCategories, char **categories) {
    int i, n;
    double dt, tPre;
    fmiBoolean timeEvent, stateEvent, stepEvent, terminateSimulation;
    double time;
    int nx;                          // number of state variables
    int nz;                          // number of state event indicators
    double *x;                       // continuous states
    double *xdot;                    // the corresponding derivatives in same order
    double *z = NULL;                // state event indicators
    double *prez = NULL;             // previous values of state event indicators
    fmiEventInfo eventInfo;          // updated by calls to initialize and eventUpdate
    ModelDescription* md;            // handle to the parsed XML file
    const char* guid;                // global unique id of the fmu
    fmiCallbackFunctions callbacks;  // called by the model during simulation
    fmiComponent c;                  // instance of the fmu
    fmiStatus fmiFlag;               // return code of the fmu functions
    fmiReal tStart = 0;              // start time
    fmiBoolean toleranceDefined = fmiFalse; // true if model description define tolerance
    fmiReal tolerance = 0;           // used in setting up the experiment
    fmiBoolean visible = fmiFalse;   // no simulator user interface
    const char *instanceName;        // instance name
    char *fmuResourceLocation = getTempResourcesLocation(); // path to the fmu resources as URL, "file://C:\QTronic\sales"
    int nSteps = 0;
    int nTimeEvents = 0;
    int nStepEvents = 0;
    int nStateEvents = 0;
    FILE* file;
    ValueStatus vs;

    // instantiate the fmu
    md = fmu->modelDescription;
    guid = getAttributeValue((Element *)md, att_guid);
    instanceName = getAttributeValue((Element *)getModelExchange(md), att_modelIdentifier);
    callbacks.logger = fmuLogger;
    callbacks.allocateMemory = calloc;
    callbacks.freeMemory = free;
    callbacks.stepFinished = NULL; // not needed for co-execution
    callbacks.componentEnvironment = fmu; // pointer to current fmu from the environment.
    c = fmu->instantiate(instanceName, fmiModelExchange, guid, fmuResourceLocation,
                        &callbacks, visible, loggingOn);
    free(fmuResourceLocation);
    if (!c) return error("could not instantiate model");

    if (nCategories > 0) {
        fmiFlag = fmu->setDebugLogging(c, fmiTrue, nCategories, categories);
        if (fmiFlag > fmiWarning) {
            return error("could not initialize model; failed FMI set debug logging");
        }
    }

    // allocate memory
    nx = getDerivativesSize(getModelStructure(md)); // number of continuous states is number of derivatives
                                                    // declared in model structure
    nz = getAttributeInt((Element *)md, att_numberOfEventIndicators, &vs); // number of event indicators
    x    = (double *) calloc(nx, sizeof(double));
    xdot = (double *) calloc(nx, sizeof(double));
    if (nz>0) {
        z    =  (double *) calloc(nz, sizeof(double));
        prez =  (double *) calloc(nz, sizeof(double));
    }
    if (!x || !xdot || nz>0 && (!z || !prez)) return error("out of memory");

    // open result file
    if (!(file = fopen(RESULT_FILE, "w"))) {
        printf("could not write %s because:\n", RESULT_FILE);
        printf("    %s\n", strerror(errno));
        return 0; // failure
    }

    // setup the experiment, set the start time
    time = tStart;
    fmiFlag = fmu->setupExperiment(c, toleranceDefined, tolerance, tStart, fmiTrue, tEnd);
    if (fmiFlag > fmiWarning) {
        return error("could not initialize model; failed FMI setup experiment");
    }
    fmiFlag = fmu->setTime(c, time);
    if (fmiFlag > fmiWarning) {
        return error("could not set time");
    }

    // initialize
    fmiFlag = fmu->enterInitializationMode(c);
    if (fmiFlag > fmiWarning) {
        return error("could not initialize model; failed FMI enter initialization mode");
    }
    fmiFlag = fmu->exitInitializationMode(c);
    if (fmiFlag > fmiWarning) {
        return error("could not initialize model; failed FMI exit initialization mode");
    }

    // event iteration
    eventInfo.newDiscreteStatesNeeded = fmiTrue;
    eventInfo.terminateSimulation = fmiFalse;
    while (eventInfo.newDiscreteStatesNeeded && !eventInfo.terminateSimulation) {
        // update discrete states
        fmiFlag = fmu->newDiscreteStates(c, &eventInfo);
        if (fmiFlag > fmiWarning) return error("could not set a new discrete state");
    }

    if (eventInfo.terminateSimulation) {
        printf("model requested termination at t=%.16g\n", time);
    } else {
        // enter Continuous-Time Mode
        fmu->enterContinuousTimeMode(c);
        // output solution for time tStart
        outputRow(fmu, c, tStart, file, separator, TRUE);  // output column names
        outputRow(fmu, c, tStart, file, separator, FALSE); // output values

        // enter the simulation loop
        while (time < tEnd) {
            // get current state and derivatives
            fmiFlag = fmu->getContinuousStates(c, x, nx);
            if (fmiFlag > fmiWarning) return error("could not retrieve states");
            fmiFlag = fmu->getDerivatives(c, xdot, nx);
            if (fmiFlag > fmiWarning) return error("could not retrieve derivatives");

            // advance time
            tPre = time;
            time = min(time+h, tEnd);
            timeEvent = eventInfo.nextEventTimeDefined && eventInfo.nextEventTime < time;
            if (timeEvent) time = eventInfo.nextEventTime;
            dt = time - tPre;
            fmiFlag = fmu->setTime(c, time);
            if (fmiFlag > fmiWarning) error("could not set time");

            // perform one step
            for (i = 0; i < nx; i++) x[i] += dt * xdot[i]; // forward Euler method
            fmiFlag = fmu->setContinuousStates(c, x, nx);
            if (fmiFlag > fmiWarning) return error("could not set states");
            if (loggingOn) printf("Step %d to t=%.16g\n", nSteps, time);

            // check for state event
            for (i = 0; i < nz; i++) prez[i] = z[i];
            fmiFlag = fmu->getEventIndicators(c, z, nz);
            if (fmiFlag > fmiWarning) return error("could not retrieve event indicators");
            stateEvent = FALSE;
            for (i=0; i<nz; i++)
                stateEvent = stateEvent || (prez[i] * z[i] < 0);

            // check for step event, e.g. dynamic state selection
            fmiFlag = fmu->completedIntegratorStep(c, fmiTrue, &stepEvent, &terminateSimulation);
            if (fmiFlag > fmiWarning) return error("could not complete intgrator step");
            if (terminateSimulation) {
                printf("model requested termination at t=%.16g\n", time);
                break; // success
            }

            // handle events
            if (timeEvent || stateEvent || stepEvent) {
                fmu->enterEventMode(c);
                if (timeEvent) {
                    nTimeEvents++;
                    if (loggingOn) printf("time event at t=%.16g\n", time);
                }
                if (stateEvent) {
                    nStateEvents++;
                    if (loggingOn) for (i=0; i<nz; i++)
                        printf("state event %s z[%d] at t=%.16g\n",
                               (prez[i]>0 && z[i]<0) ? "-\\-" : "-/-", i, time);
                }
                if (stepEvent) {
                    nStepEvents++;
                    if (loggingOn) printf("step event at t=%.16g\n", time);
                }

                // event iteration in one step, ignoring intermediate results
                eventInfo.newDiscreteStatesNeeded = fmiTrue;
                eventInfo.terminateSimulation = fmiFalse;
                while (eventInfo.newDiscreteStatesNeeded && !eventInfo.terminateSimulation) {
                    // update discrete states
                    fmiFlag = fmu->newDiscreteStates(c, &eventInfo);
                    if (fmiFlag > fmiWarning) return error("could not set a new discrete state");
                }
                if (eventInfo.terminateSimulation) {
                    printf("model requested termination at t=%.16g\n", time);
                    break; // success
                }

                // enter Continuous-Time Mode
                fmu->enterContinuousTimeMode(c);

                // check for change of value of states
                if (eventInfo.valuesOfContinuousStatesChanged && loggingOn) {
                    printf("continuous state values changed at t=%.16g\n", time);
                }

                if (eventInfo.nominalsOfContinuousStatesChanged && loggingOn){
                    printf("nominals of continuous state changed  at t=%.16g\n", time);
                }

            } // if event
            outputRow(fmu, c, time, file, separator, FALSE); // output values for this step
            nSteps++;
        } // while
    }
    // cleanup
    fmu->terminate(c);
    fmu->freeInstance(c);
    fclose(file);
    if (x != NULL) free(x);
    if (xdot != NULL) free(xdot);
    if (z != NULL) free(z);
    if (prez != NULL) free(prez);

    // print simulation summary
    printf("Simulation from %g to %g terminated successful\n", tStart, tEnd);
    printf("  steps ............ %d\n", nSteps);
    printf("  fixed step size .. %g\n", h);
    printf("  time events ...... %d\n", nTimeEvents);
    printf("  state events ..... %d\n", nStateEvents);
    printf("  step events ...... %d\n", nStepEvents);

    return 1; // success
}

int main(int argc, char *argv[]) {
    const char* fmuFileName;
    int i;

    // parse command line arguments and load the FMU
    // default arguments value
    double tEnd = 1.0;
    double h=0.1;
    int loggingOn = 0;
    char csv_separator = ',';
    char **categories = NULL;
    int nCategories = 0;

    parseArguments(argc, argv, &fmuFileName, &tEnd, &h, &loggingOn, &csv_separator, &nCategories, &categories);
    loadFMU(fmuFileName);

        // run the simulation
    printf("FMU Simulator: run '%s' from t=0..%g with step size h=%g, loggingOn=%d, csv separator='%c' ",
            fmuFileName, tEnd, h, loggingOn, csv_separator);
    printf("log categories={ ");
    for (i = 0; i < nCategories; i++) printf("%s ", categories[i]);
    printf("}\n");

    simulate(&fmu, tEnd, h, loggingOn, csv_separator, nCategories, categories);
    printf("CSV file '%s' written\n", RESULT_FILE);

    // release FMU
    FreeLibrary(fmu.dllHandle);
    freeModelDescription(fmu.modelDescription);
    if (categories) free(categories);

    return EXIT_SUCCESS;
}
