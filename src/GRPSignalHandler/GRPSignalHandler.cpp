// Copyright 2021-2026
//
// Authors:
// Stanimir Kisyov, Sarah Schröder, Davide Terzani
//
// Creator: Davide Terzani
//
// This file is part of GRAPPA.
//
// License: BSD-3-Clause

#include <GRPSignalHandler.hpp>

G4ThreadLocal SignalHandler *SignalHandler::m_SHpointer = nullptr;

void RegisterSignals()
{
    static_cast<void>(std::signal(SIGCONT, SignalHandler::HandleSignals));
    static_cast<void>(std::signal(SIGHUP, SignalHandler::HandleSignals));
    static_cast<void>(std::signal(SIGINT, SignalHandler::HandleSignals));
    static_cast<void>(std::signal(SIGTERM, SignalHandler::HandleSignals));
}

void SignalHandler::HandleSignals(G4int signum)
{
    G4cout << G4endl;
    G4cout << " Interrupt signal (" << signum << ") received" << G4endl;
    switch (signum)
    {
    case SIGINT:
        CleanTerminationSigInt();
        break;
    case SIGHUP:
        CleanTerminationSigHup();
        break;
    case SIGTERM:
        CleanTerminationSigTerm();
        break;
    case SIGKILL:
        G4cout << " Received signal SIGKILL. Program is killed." << G4endl;
    case SIGCONT:
        G4cout << " Received signal SIGCONT. Simulation will continue."
               << G4endl;
        break;

    default:
        G4cout << " No instructions for signal " << signum << " are provided."
               << G4endl;
        exit(signum);
        break;
    }
}

void SignalHandler::CleanTerminationSigInt()
{
    G4cout << " Received SIGINT. Interrupting run after finishing the current "
              "events and saving "
              "data "
           << G4endl;
    G4RunManager *rman = G4RunManager::GetRunManager();
    rman->AbortRun(true);
}

void SignalHandler::CleanTerminationSigHup()
{
    G4cout << " Received SIGHUP. Interrupting run after finishing the current "
              "events and saving "
              "data "
           << G4endl;
    G4RunManager *rman = G4RunManager::GetRunManager();
    rman->AbortRun(true);
}

void SignalHandler::CleanTerminationSigTerm()
{
    G4cout << " Received SIGTERM. Aborting the current event, interrupting run "
              "and saving data "
           << G4endl;
    G4RunManager *rman = G4RunManager::GetRunManager();
    rman->AbortRun(false);
}

SignalHandler *SignalHandler::GetSignalHandler()
{
    if (m_SHpointer == nullptr)
    {
        m_SHpointer = new SignalHandler;
    }

    return m_SHpointer;
}
