#include "main.hpp"
#include "UI/SharedUIManager.hpp"
#include "UI/BurnoutSideView.hpp"

#include <string>

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "questui/shared/CustomTypes/Components/Settings/IncrementSetting.hpp"

#include "UnityEngine/Object.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "HMUI/ViewController.hpp"
#include "UnityEngine/UI/Selectable.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "TMPro/TextMeshProUGUI.hpp"

using namespace Burnout;
DEFINE_TYPE(Burnout, BurnoutSideView);

BurnoutSideView* SideView;

void OnChangeScale(float newval)
{
    newval = std::round(newval * 100) / 100;
    auto& modcfg = getConfig().config;
    auto* element = SideView->burnScaleSetting;
    auto* decrButton = element->GetComponentsInChildren<UnityEngine::UI::Button*>()->values[0];
    auto* incrButton = element->GetComponentsInChildren<UnityEngine::UI::Button*>()->values[1];
    decrButton->set_interactable(true);
    incrButton->set_interactable(true);
    if (newval < -3 + element->Increment)
    {
        decrButton->set_interactable(false);
    }
    if (newval > 3 - element->Increment)
    {
        incrButton->set_interactable(false);
    }
    modcfg["BurnScale"].SetFloat(newval);
}

void OnChangeLifetime(float newval)
{
    newval = std::round(newval * 100) / 100;
    auto& modcfg = getConfig().config;
    auto* element = SideView->burnLifetimeSetting;
    auto* decrButton = element->GetComponentsInChildren<UnityEngine::UI::Button*>()->values[0];
    decrButton->set_interactable(true);
    if (newval < 0 + element->Increment * 0.8)
    {
        decrButton->set_interactable(false);
    }
    modcfg["BurnLifetime"].SetFloat(newval);
}

void OnChangeIntensity(float newval)
{
    newval = std::round(newval * 100) / 100;
    auto& modcfg = getConfig().config;
    auto* element = SideView->burnIntensitySetting;
    auto* decrButton = element->GetComponentsInChildren<UnityEngine::UI::Button*>()->values[0];
    decrButton->set_interactable(true);
    if (newval < 0 + element->Increment * 0.8)
    {
        decrButton->set_interactable(false);
    }
    modcfg["BurnOpacity"].SetFloat(newval);
}


void BurnoutSideView::OnChangeVisibility(bool newval)
{
    this->container->get_gameObject()->SetActive(newval);
}

void BurnoutSideView::ReloadValues()
{
    auto& modcfg = getConfig().config;
    burnScaleSetting->CurrentValue = modcfg["BurnScale"].GetFloat();
    burnLifetimeSetting->CurrentValue = modcfg["BurnLifetime"].GetFloat();
    burnIntensitySetting->CurrentValue = modcfg["BurnOpacity"].GetFloat();
    burnScaleSetting->UpdateValue();
    burnLifetimeSetting->UpdateValue();
    burnIntensitySetting->UpdateValue();
}

void BurnoutSideView::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    SideView = this;
    auto& modcfg = getConfig().config;
    if (firstActivation && addedToHierarchy) 
    {
        float scale_initval = modcfg["BurnScale"].GetFloat();
        float lifetime_initval = modcfg["BurnLifetime"].GetFloat();
        float intensity_initval = modcfg["BurnOpacity"].GetFloat();

        this->container = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_rectTransform());
        container->set_spacing(0.2f);
        container->set_childControlHeight(true);
        container->GetComponent<UnityEngine::UI::LayoutElement*>()->set_minWidth(25.0);

        auto* titlecontainer = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(container->get_rectTransform());
        titlecontainer->get_gameObject()->AddComponent<QuestUI::Backgroundable*>()->ApplyBackground(il2cpp_utils::createcsstr("panel-top"));
        titlecontainer->set_padding(UnityEngine::RectOffset::New_ctor(10, 10, 0, 0));
        titlecontainer->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_horizontalFit(2);

        auto* titletext = QuestUI::BeatSaberUI::CreateText(titlecontainer->get_rectTransform(), "Burn Effects", false);
        titletext->set_alignment(TMPro::TextAlignmentOptions::Center);
        titletext->set_fontSize(10);

        this->configcontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(container->get_rectTransform());
        configcontainer->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_verticalFit(2);

        this->burnScaleSetting = QuestUI::BeatSaberUI::CreateIncrementSetting(configcontainer->get_rectTransform(), "Scale", 2, 0.1f, scale_initval, 0.2f, 3.0f, OnChangeScale);
        this->burnLifetimeSetting = QuestUI::BeatSaberUI::CreateIncrementSetting(configcontainer->get_rectTransform(), "Burn Lifetime", 2, 0.5f, lifetime_initval, 0.0f, 10.0f, OnChangeLifetime);
        this->burnIntensitySetting = QuestUI::BeatSaberUI::CreateIncrementSetting(configcontainer->get_rectTransform(), "Intensity", 2, 0.2f, intensity_initval, 0.2f, 1.0f, OnChangeIntensity);
    }
    this->ReloadValues();
    this->container->get_gameObject()->SetActive(modcfg["EnableTweaks"].GetBool());
}

void BurnoutSideView::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling)
{
    getConfig().Write();
}