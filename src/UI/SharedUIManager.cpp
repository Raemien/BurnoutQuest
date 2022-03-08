#include "main.hpp"
#include "UI/SharedUIManager.hpp"
#include "UI/BurnoutFlowCoordinator.hpp"

using namespace Burnout;

HMUI::FlowCoordinator* BurnoutFC;

void ReloadAllElements()
{
    if (BurnoutFC != nullptr)
    {
        BurnoutFlowCoordinator* targetFC = (BurnoutFlowCoordinator*)BurnoutFC;
        targetFC->ReloadUIValues();
    }
}

void ApplyPanelVisibility(bool visible)
{
    if (BurnoutFC != nullptr)
    {
        BurnoutFlowCoordinator* targetFC = (BurnoutFlowCoordinator*)BurnoutFC;
        targetFC->UpdateSettingsVisibility(visible);
    }
}