// Copyright 2021-2024
//
// Authors:
// Stanimir Kisyov, Sarah Schröder, Davide Terzani
//
// Creator: Davide Terzani
//
// This file is part of GRAPPA.
//
// License: BSD-3-Clause

#pragma once

// C++
#include <csignal>

// Geant4
#include <G4RunManager.hh>
#include <G4UImanager.hh>

// GRAPPA
#include <GRPMessages.hpp>

// Utility class that enables signal handling
void RegisterSignals();

class SignalHandler
{
public:
    // Static function that returns the pointer to the thread-local instance
    static SignalHandler *GetSignalHandler();

public:
    static void HandleSignals(G4int signum);

private:
    SignalHandler() = default;
    ~SignalHandler() = default;
    SignalHandler(const SignalHandler &) = delete;
    SignalHandler &operator=(const SignalHandler &) = delete;
    static void CleanTerminationSigInt();
    static void CleanTerminationSigTerm();

    static G4ThreadLocal SignalHandler *m_SHpointer;
};
