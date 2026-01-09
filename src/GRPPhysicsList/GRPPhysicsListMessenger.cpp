

#include <GRPPhysicsListMessenger.hpp>
#include <GRPPhysicsList.hpp>

GRPPhysicsListMessenger::GRPPhysicsListMessenger(GRPPhysicsList *physics_list)
    : G4UImessenger()
    , m_physics_list(physics_list)
    , m_physics_directory(nullptr)
    , m_add_cmd(nullptr)
{

    m_physics_directory =
        std::make_unique<G4UIdirectory>("/GRAPPA/physics_list/");
    m_physics_directory->SetGuidance("Personalize the GRAPPA physics list");

    m_add_cmd = std::make_unique<G4UIcmdWithAString>(
        "/GRAPPA/physics_list/addPhysics", this);
    m_add_cmd->SetGuidance("Add physics list.");
    m_add_cmd->SetParameterName("Plist", false);
    m_add_cmd->AvailableForStates(G4State_PreInit);
}

GRPPhysicsListMessenger::~GRPPhysicsListMessenger() = default;

void GRPPhysicsListMessenger::SetNewValue(
    G4UIcommand *command, G4String newValue)
{
    if (command == m_add_cmd.get())
    {
        if (!m_physics_list)
        {
            G4ExceptionDescription msg;
            msg << "No physics list is defined!";
            G4Exception(
                "GRPPhysicsListMessenger::SetNewValue()",
                "GRAPPA::PHYSICS_LIST_UNDEFINED",
                FatalException,
                msg);
        }
        m_physics_list->AddPhysicsList(newValue);
    }
}
