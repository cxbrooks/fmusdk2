/* ------------------------------------------------------------------------- 
 * fmi.h
 * Struct with the corresponding function pointers for FMI 2.0.
 * Copyright QTronic GmbH. All rights reserved.
 * -------------------------------------------------------------------------*/

#ifndef FMI_H
#define FMI_H

#include <windows.h>
#include "fmiFunctions.h"

#include "XmlParserCApi.h"

typedef struct {
    ModelDescription* modelDescription;

    HMODULE dllHandle; // fmu.dll handle
    /***************************************************
    Common Functions
    ****************************************************/
    fmiGetTypesPlatformTYPE         *getTypesPlatform;
    fmiGetVersionTYPE               *getVersion;
    fmiSetDebugLoggingTYPE          *setDebugLogging;
    fmiInstantiateTYPE              *instantiate;
    fmiFreeInstanceTYPE             *freeInstance;
    fmiSetupExperimentTYPE          *setupExperiment;
    fmiEnterInitializationModeTYPE  *enterInitializationMode;
    fmiExitInitializationModeTYPE   *exitInitializationMode;
    fmiTerminateTYPE                *terminate;
    fmiResetTYPE                    *reset;
    fmiGetRealTYPE                  *getReal;
    fmiGetIntegerTYPE               *getInteger;
    fmiGetBooleanTYPE               *getBoolean;
    fmiGetStringTYPE                *getString;
    fmiSetRealTYPE                  *setReal;
    fmiSetIntegerTYPE               *setInteger;
    fmiSetBooleanTYPE               *setBoolean;
    fmiSetStringTYPE                *setString;
    fmiGetFMUstateTYPE              *getFMUstate;
    fmiSetFMUstateTYPE              *setFMUstate;
    fmiFreeFMUstateTYPE             *freeFMUstate;
    fmiSerializedFMUstateSizeTYPE   *serializedFMUstateSize;
    fmiSerializeFMUstateTYPE        *serializeFMUstate;
    fmiDeSerializeFMUstateTYPE      *deSerializeFMUstate;
    fmiGetDirectionalDerivativeTYPE *getDirectionalDerivative;
    /***************************************************
    Functions for FMI for Co-Simulation
    ****************************************************/
    fmiSetRealInputDerivativesTYPE  *setRealInputDerivatives;
    fmiGetRealOutputDerivativesTYPE *getRealOutputDerivatives;
    fmiDoStepTYPE                   *doStep;
    fmiCancelStepTYPE               *cancelStep;
    fmiGetStatusTYPE                *getStatus;
    fmiGetRealStatusTYPE            *getRealStatus;
    fmiGetIntegerStatusTYPE         *getIntegerStatus;
    fmiGetBooleanStatusTYPE         *getBooleanStatus;
    fmiGetStringStatusTYPE          *getStringStatus;
    /***************************************************
    Functions for FMI for Model Exchange
    ****************************************************/
    fmiEnterEventModeTYPE                *enterEventMode;
    fmiNewDiscreteStatesTYPE             *newDiscreteStates;
    fmiEnterContinuousTimeModeTYPE       *enterContinuousTimeMode;
    fmiCompletedIntegratorStepTYPE       *completedIntegratorStep;
    fmiSetTimeTYPE                       *setTime;
    fmiSetContinuousStatesTYPE           *setContinuousStates;
    fmiGetDerivativesTYPE                *getDerivatives;
    fmiGetEventIndicatorsTYPE            *getEventIndicators;
    fmiGetContinuousStatesTYPE           *getContinuousStates;
    fmiGetNominalsOfContinuousStatesTYPE *getNominalsOfContinuousStates;
} FMU;

#endif // FMI_H

