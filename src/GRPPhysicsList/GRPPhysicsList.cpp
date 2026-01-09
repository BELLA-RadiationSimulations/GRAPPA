
#include <G4EmLivermorePhysics.hh>
#include <G4EmStandardPhysics.hh>
#include <G4EmStandardPhysics_option1.hh>
#include <G4EmStandardPhysics_option2.hh>
#include <G4EmStandardPhysics_option3.hh>
#include <G4EmStandardPhysics_option4.hh>
#include <G4GenericBiasingPhysics.hh>
#include <G4StepLimiterPhysics.hh>
#include <G4SystemOfUnits.hh>
#include <G4ProcessTable.hh>

#include <GRPEmPhysics.hpp>
#include <GRPPhysicsList.hpp>

GRPPhysicsList::GRPPhysicsList() : G4VModularPhysicsList()
{

    G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(
        250 * eV, 1 * GeV);
    SetDefaultCutValue(0.7 * mm);
    DumpCutValuesTable();

    // Add the electromagnetic physics list
    // The default list is the default in Geant4 physics lists (option 0)
    RegisterPhysics(new GRPEmPhysics());

    // Add the step limiter physics
    RegisterPhysics(new G4StepLimiterPhysics());

    // // Introduce biasing
    // G4GenericBiasingPhysics *biasingPhysics = new G4GenericBiasingPhysics();
    // // We need to bias GammaGeneralProc because it's the only process explicitly
    // // declared. Selection is then performed in the biasing operation.
    // std::vector<G4String> processToBias{"GammaGeneralProc"};
    // biasingPhysics->PhysicsBias("gamma", processToBias);
    // // Bias pion decay
    // // processToBias = {"Decay"};
    // // biasingPhysics->PhysicsBias("pi+", processToBias);
    // // biasingPhysics->PhysicsBias("pi-", processToBias);

    // RegisterPhysics(biasingPhysics);

    // Add the physics list messenger to have UI commands
    m_pl_messenger = std::make_unique<GRPPhysicsListMessenger>(this);
}

GRPPhysicsList::~GRPPhysicsList() = default;

void GRPPhysicsList::AddPhysicsList(const G4String &name)
{

    if (name == "emstandard_opt0")
    {
        ReplacePhysics(new G4EmStandardPhysics());
        G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
    }
    else if (name == "emstandard_opt1")
    {
        ReplacePhysics(new G4EmStandardPhysics_option1());
        G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
    }
    else if (name == "emstandard_opt2")
    {
        ReplacePhysics(new G4EmStandardPhysics_option2());
        G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
    }
    else if (name == "emstandard_opt3")
    {
        ReplacePhysics(new G4EmStandardPhysics_option3());
        G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
    }
    else if (name == "emstandard_opt4")
    {
        ReplacePhysics(new G4EmStandardPhysics_option4());
        G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
    }
    else if (name == "emlivermore")
    {
        ReplacePhysics(new G4EmLivermorePhysics());
        G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
    }
    else
    {

        G4cout << "PhysicsList::AddPhysicsList: <" << name << ">"
               << " is not defined" << G4endl;
    }
    G4cout << "PhysicsList::AddPhysicsList: <" << name << ">"
           << " is activated" << G4endl;
}
