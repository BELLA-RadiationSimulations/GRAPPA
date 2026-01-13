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

#pragma once

#include <memory>

#include <G4UIdirectory.hh>
#include <G4UImessenger.hh>
#include <G4UIcmdWithoutParameter.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithoutParameter.hh>
#include <G4UIcmdWithABool.hh>

// Forward delacration to enable access to the base class
class GRPSplittingOperator;

class GRPSplittingOperatorMessenger : public G4UImessenger
{
public:
    explicit GRPSplittingOperatorMessenger(GRPSplittingOperator *);
    ~GRPSplittingOperatorMessenger();

    virtual void SetNewValue(G4UIcommand *, G4String) override;

private:
    GRPSplittingOperator *m_splitting_operator;
    std::unique_ptr<G4UIdirectory> m_splitting_directory;
    std::unique_ptr<G4UIdirectory> m_splitting_directory_pions;
    std::unique_ptr<G4UIdirectory> m_splitting_directory_muons;
    std::unique_ptr<G4UIcmdWithAnInteger> m_set_muon_factor;
    std::unique_ptr<G4UIcmdWithAnInteger> m_set_pion_factor;
    std::unique_ptr<G4UIcmdWithoutParameter> m_get_muon_factor;
    std::unique_ptr<G4UIcmdWithoutParameter> m_get_pion_factor;
    std::unique_ptr<G4UIcmdWithABool> m_set_muon_weight_scale;
    std::unique_ptr<G4UIcmdWithoutParameter> m_get_muon_weight_scale;
};
