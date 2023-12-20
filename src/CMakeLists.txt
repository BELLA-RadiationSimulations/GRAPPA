# Copyright 2021-2023
#
# Authors:
# Stanimir Kisyov, Davide Terzani
#
# Creator: Davide Terzani
#
# This file is part of GRAPPA.
#
# License: BSD-3-Clause

target_sources(
    GRAPPA
    PRIVATE GRPActionInitialization.cpp
            GRPDetectorConstruction.cpp
            GRPHistandNTupleManager.cpp
            GRPParticleSource.cpp
            GRPPrimaryGeneratorAction.cpp
            GRPRunAction.cpp
            GRPSDAbsorber.cpp
            GRPTrackingAction.cpp
            ReadASCII.cpp
            grappa.cpp)