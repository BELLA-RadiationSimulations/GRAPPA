// Copyright 2021-2026
//
// Authors:
// Stanimir Kisyov, Sarah Schröder, Davide Terzani
//
// Creator: Davide Terzani
//
// This file is part of GRAPPA.
//
// License: BSD-3-Clause-LBNL

#pragma once

#include <G4ParticleChange.hh>
#include <G4VBiasingOperation.hh>

class GRPMuonPairSplittingOperation : public G4VBiasingOperation
{

public:
    GRPMuonPairSplittingOperation(G4String name);
    virtual ~GRPMuonPairSplittingOperation();

    //
    // Interface to sub-classes :
    //
    // Methods for physics-based biasing:
    //
    // - I. Biasing of the process occurrence:
    //
    // The biasing of the process occurrence regards the occurrence of the
    // PostStepDoIt behavior. But the weight is manipulated by both AlongStep
    // methods (weight for non-interaction) and PostStep methods (weight for
    // interaction). For this reason, occurrence biasing is handled by both
    // AlongStep and PostStep methods.
    //
    // If the operation is returned to the G4BiasingProcessInterface process by
    // the ProposeOccurenceBiasingOperation(...) or
    // GetProposedOccurenceBiasingOperation(...) method of the biasing operator,
    // all methods below will be called for this operation.
    //
    // - I.1) Methods called in at the PostStepGetPhysicalInteractionLength(...)
    // level :
    //
    // Main and mandatory method for biasing of the PostStep process
    // biasing occurrence :
    //  - propose an interaction law to be substituted to the process
    // that is biased
    //  - the operation is told which is the G4BiasingProcessInterface
    // calling it with callingProcess argument.
    //  - the returned law will have to have been sampled prior to be
    // returned as it will be asked for its GetSampledInteractionLength() by the
    // callingProcess.
    //  - the operation can propose a force condition in the PostStepGPIL (the
    //  passed value to the operation is the one of the wrapped process, if
    //  proposeForceCondition is unchanged, this same value will be used as the
    //  biasing foroce condition)
    // --
    // ProvideOccurenceBiasingInteractionLaw must be implemented because is an
    // pure virtual method, but it's not used
    // --
    virtual const G4VBiasingInteractionLaw *
    ProvideOccurenceBiasingInteractionLaw(
        const G4BiasingProcessInterface *, G4ForceCondition &)
    {
        return 0;
    }

    // - II. Biasing of the process post step final state:
    //
    // Mandatory method for biasing of the PostStepDoIt of the wrapped process
    // holds by the G4BiasingProcessInterface callingProcess.
    // User has full freedom for the particle change returned, and is reponsible
    // for the correctness of weights set to tracks. The forcedBiasedFinalState
    // should be left as is (ie false) in general. In this way, if an occurrence
    // biasing is also applied in the step, the weight correction for it will be
    // applied. If returned forceBiasedFinalState is returned true, then the
    // returned particle change will be returned as is to the stepping. Full
    // responsibility of the weight correctness is taken by the biasing
    // operation. The wrappedProcess can be accessed through the
    // G4BiasingProcessInterface if needed. This can be used in conjunction with
    // an occurrence biasing, provided this final state biasing is uncorrelated
    // with the occurrence biasing (as single multiplication of weights occur
    // between these two biasings).
    virtual G4VParticleChange *ApplyFinalStateBiasing(
        const G4BiasingProcessInterface *,
        const G4Track *,
        const G4Step *,
        G4bool &);

    // - III. Biasing of the process along step final state:
    //
    // ***************************************************
    // -- Methods for non-physics-based biasing operation:
    // ***************************************************
    // If the operation is returned to the G4BiasingProcessInterface
    // process by the ProposeNonPhysicsBiasingOperation(...) or
    // GetProposedNonPhysicsBiasingOperation(...) method of the biasing
    // operator, all methods below will be called for this operation. 1) Method
    // called in at the PostStepGetPhysicalInteractionLength(...) level : o
    // Return to the distance at which the operation should be applied, or may
    // play with the force condition flags.
    // --
    // DistanceToApplyOperation must be
    // implemented because is an pure virtual method, but it's not used
    // --
    virtual G4double
    DistanceToApplyOperation(const G4Track *, G4double, G4ForceCondition *)
    {
        return DBL_MAX;
    }

    //
    // - 2) Method called in at the PostStepDoIt(...) level :
    // o Generate the final state for biasing (eg: splitting, killing, etc.)
    // --
    // GenerateBiasingFinalState must be implemented because is an pure virtual
    // method, but it's not used
    // --
    virtual G4VParticleChange *
    GenerateBiasingFinalState(const G4Track *, const G4Step *)
    {
        return 0;
    }

    G4int GetSplittingFactor() const { return m_SplittingFactor; }
    void SetSplittingFactor(G4int splittingFactor)
    {
        m_SplittingFactor = splittingFactor;
    }
    G4bool GetScaleWeightByCrossSection() const
    {
        return m_scaleMuonWeightByCrossSection;
    }
    void SetScaleWeightByCrossSection(G4bool ifScaleWeight)
    {
        m_scaleMuonWeightByCrossSection = ifScaleWeight;
    }

private:
    G4int m_SplittingFactor;
    G4bool m_scaleMuonWeightByCrossSection;
    G4ParticleChange m_ParticleChange;
};
