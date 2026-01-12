
#include <G4DecayPhysics.hh>
#include <G4GenericBiasingPhysics.hh>
#include <G4HadronElasticPhysicsHP.hh>
#include <G4HadronicParameters.hh>
#include <G4IonPhysics.hh>
#include <G4MuonicAtomDecayPhysics.hh>
#include <G4NeutronTrackingCut.hh>
#include <G4StepLimiterPhysics.hh>
#include <G4StoppingPhysics.hh>
#include <G4SystemOfUnits.hh>
#include <G4ProcessTable.hh>

#include <GRPEmPhysics.hpp>
#include <GRPEmExtraPhysics.hpp>
#include <GRPPhysicsList.hpp>

GRPPhysicsList::GRPPhysicsList() : G4VModularPhysicsList()
{

    G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(
        250 * eV, 1 * GeV);
    SetDefaultCutValue(0.7 * mm);
    DumpCutValuesTable();

    // Add the electromagnetic physics list
    // The default list is the default in Geant4 physics lists (option 0)
    m_em_physics = new GRPEmPhysics();
    RegisterPhysics(m_em_physics);

    GRPEmExtraPhysics *extraphysics = new GRPEmExtraPhysics();
    RegisterPhysics(extraphysics);

    // Register decay of muonic atoms
    RegisterPhysics(new G4MuonicAtomDecayPhysics());

    // Decays
    RegisterPhysics(new G4DecayPhysics());

    // Hadron Elastic scattering
    RegisterPhysics(new G4HadronElasticPhysicsHP());
#if defined (GRAPPA_ENABLED_BERTINI_ROLLBACK)
    G4HadronicParameters::Instance()->SetBertiniAs11_2(true);
#endif
    // Stopping Physics
    // This include the capture of muon minus (mu- + p -> n + nu_mu)
    // We set it to false because it otherwise shadows the more detailed muonic atom decay physics
    G4StoppingPhysics * stoppingphysics= new G4StoppingPhysics();
    stoppingphysics->SetMuonMinusCapture(false);
    RegisterPhysics(stoppingphysics);

    // Ion Physics
    RegisterPhysics(new G4IonPhysics());
    
    // Neutron tracking cut
    RegisterPhysics(new G4NeutronTrackingCut());

    // Add the step limiter physics
    RegisterPhysics(new G4StepLimiterPhysics());

    // // Introduce biasing
    // G4GenericBiasingPhysics *biasingPhysics = new G4GenericBiasingPhysics();
    // // We need to bias GammaGeneralProc because it's the only process
    // explicitly
    // // declared. Selection is then performed in the biasing operation.
    // std::vector<G4String> processToBias{"GammaGeneralProc"};
    // biasingPhysics->PhysicsBias("gamma", processToBias);
    // // Bias pion decay
    // // processToBias = {"Decay"};
    // // biasingPhysics->PhysicsBias("pi+", processToBias);
    // // biasingPhysics->PhysicsBias("pi-", processToBias);

    // RegisterPhysics(biasingPhysics);

    // Add the physics list messenger to have UI commands
    m_pl_messenger =
        std::make_unique<GRPPhysicsListMessenger>(this, m_em_physics);
}

GRPPhysicsList::~GRPPhysicsList() = default;

// void GRPPhysicsList::AddPhysicsList(const G4String &name)
// {

//     if (name == "emstandard_opt0")
//     {
//         ReplacePhysics(new G4EmStandardPhysics());
//         G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
//     }
//     else if (name == "emstandard_opt1")
//     {
//         ReplacePhysics(new G4EmStandardPhysics_option1());
//         G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
//     }
//     else if (name == "emstandard_opt2")
//     {
//         ReplacePhysics(new G4EmStandardPhysics_option2());
//         G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
//     }
//     else if (name == "emstandard_opt3")
//     {
//         ReplacePhysics(new G4EmStandardPhysics_option3());
//         G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
//     }
//     else if (name == "emstandard_opt4")
//     {
//         ReplacePhysics(new G4EmStandardPhysics_option4());
//         G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
//     }
//     else if (name == "emlivermore")
//     {
//         ReplacePhysics(new G4EmLivermorePhysics());
//         G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
//     }
//     else
//     {

//         G4cout << "PhysicsList::AddPhysicsList: <" << name << ">"
//                << " is not defined" << G4endl;
//     }
//     G4cout << "PhysicsList::AddPhysicsList: <" << name << ">"
//            << " is activated" << G4endl;
// }

