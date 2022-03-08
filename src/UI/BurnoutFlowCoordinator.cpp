#include "main.hpp"
#include "config.hpp"
#include "UI/SharedUIManager.hpp"
#include "UI/BurnoutMainView.hpp"
#include "UI/BurnoutSideView.hpp"
#include "UI/BurnoutFlowCoordinator.hpp"

#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp" 
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "beatsaber-hook/shared/config/config-utils.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"

#include "HMUI/ViewController.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/FlowCoordinator.hpp"
#include "UnityEngine/Object.hpp"
#include "GlobalNamespace/MainFlowCoordinator.hpp"

DEFINE_TYPE(Burnout, BurnoutFlowCoordinator);

namespace Burnout
{
    void BurnoutFlowCoordinator::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        BurnoutFC = (HMUI::FlowCoordinator*)this;
        if (!this->mainView) this->mainView = QuestUI::BeatSaberUI::CreateViewController<BurnoutMainView*>();
        if (!this->sideView) this->sideView = QuestUI::BeatSaberUI::CreateViewController<BurnoutSideView*>();
        if (firstActivation) 
        {
            this->SetTitle(il2cpp_utils::createcsstr("Burnout"), (int)1);
            this->showBackButton = true;
            // SetInitialValues();
            BurnoutFlowCoordinator::ProvideInitialViewControllers(mainView, sideView, nullptr, nullptr, nullptr);
        }
    }

    void BurnoutFlowCoordinator::ReloadUIValues()
    {
        BurnoutSideView* sideVC = (BurnoutSideView*)this->sideView;
        sideVC->ReloadValues();
    }

    void BurnoutFlowCoordinator::UpdateSettingsVisibility(bool visible)
    {
        BurnoutSideView* sideVC = (BurnoutSideView*)this->sideView;
        sideVC->OnChangeVisibility(visible);
    }

    void BurnoutFlowCoordinator::BackButtonWasPressed(HMUI::ViewController* topView)
    {
        getConfig().Write();
        HMUI::FlowCoordinator* settingsFC = QuestUI::GetModSettingsFlowCoordinator();
        settingsFC->DismissFlowCoordinator(this, (int)0, nullptr, false);
    }
}
