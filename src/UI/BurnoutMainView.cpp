#include "Helpers/PresetHelper.hpp"
#include "Helpers/ColorHelper.hpp"
#include "UI/SharedUIManager.hpp"
#include "UI/BurnoutMainView.hpp"
#include "config.hpp"
#include "main.hpp"

#include "UnityEngine/Color.hpp"

#include "modloader/shared/modloader.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"
#include "custom-types/shared/coroutine.hpp"

#include "GlobalNamespace/ColorPickerButtonController.hpp"
#include "GlobalNamespace/ColorChangeUIEventType.hpp"

#include "HMUI/Screen.hpp"
#include "HMUI/ImageView.hpp"
#include "HMUI/ModalView.hpp"
#include "HMUI/FlowCoordinator.hpp"
#include "HMUI/ViewController.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/TextAnchor.hpp"
#include "UnityEngine/TextAlignment.hpp"
#include "UnityEngine/WaitForSeconds.hpp"
#include "HMUI/SimpleTextDropdown.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "TMPro/FontStyles.hpp"

#include "System/Collections/IEnumerator.hpp"

#include <chrono>
#include <map>

using namespace Burnout;
DEFINE_TYPE(Burnout, BurnoutMainView);

std::vector<std::u16string> PresetOptions {u"Custom", u"Default", u"Classic", u"Mono-Blue"};

BurnoutMainView* BurnMainView;

custom_types::Helpers::Coroutine DisplayPinkCoreWarning(HMUI::ModalView* modal)
{
    while (BurnMainView->isInTransition)
    {
        co_yield reinterpret_cast<System::Collections::IEnumerator*>(CRASH_UNLESS(UnityEngine::WaitForSeconds::New_ctor(0.1f)));
    }
    modal->Show(true, true, nullptr);
    ApplyPanelVisibility(false);
    co_return;
}

void OnChangeEnabled(bool newval)
{
    auto& modcfg = getConfig().config;
    modcfg["EnableTweaks"].SetBool(newval);
    ApplyPanelVisibility(newval);
}

void OnChangeOverrideColors(bool newval)
{
    auto& modcfg = getConfig().config;
    modcfg["OverrideColors"].SetBool(newval);
    BurnMainView->colorContainer->get_gameObject()->SetActive(newval);
}

void SetButtonColor(UnityEngine::UI::Button* button, UnityEngine::Color color) {
    button->get_transform()->Find(il2cpp_utils::newcsstr("Underline"))->GetComponent<HMUI::ImageView*>()->set_color(color + UnityEngine::Color(0,0,0,1));
}

void OnChangeLeftColor(UnityEngine::Color newval)
{
    auto& modcfg = getConfig().config;
    std::string hexcol = Helpers::ColorHelper::ColorToHex(newval);
    modcfg["LeftColor"].SetString(hexcol, modcfg.GetAllocator());
    getConfig().Write();
    if (BurnMainView->leftColorButton) SetButtonColor(BurnMainView->leftColorButton, newval);
}

void OnChangeRightColor(UnityEngine::Color newval)
{
    auto& modcfg = getConfig().config;
    std::string hexcol = Helpers::ColorHelper::ColorToHex(newval);
    modcfg["RightColor"].SetString(hexcol, modcfg.GetAllocator());
    getConfig().Write();
    if (BurnMainView->rightColorButton) SetButtonColor(BurnMainView->rightColorButton, newval);
}

void OnSelectPreset(std::u16string_view newval_u16)
{
    std::string newval_u8 = to_utf8(newval_u16);
    using namespace Burnout::Helpers;
    std::map<std::string, TrailPreset> ConfigMap = 
    { 
        {"Default", TrailPreset(0.5f, 1.0f, 1.0f, false, ColorHelper::Default_Red, ColorHelper::Default_Blue)},
        {"Classic", TrailPreset(0.5f, 1.0f, 1.0f, true, ColorHelper::Default_Red, ColorHelper::Default_Red)},
        {"Mono-Blue", TrailPreset(0.5f, 1.0f, 1.0f, true, ColorHelper::Default_Blue, ColorHelper::Default_Blue)},
        {"X-mas", TrailPreset(0.5f, 1.0f, 1.0f, true, ColorHelper::HexToColor("#aa0000"), ColorHelper::HexToColor("#00a000"))}
    };

    if (ConfigMap.contains(newval_u8)){
        TrailPreset preset = ConfigMap[newval_u8];
        PresetHelper::ApplyPreset(preset);
    }
    if (BurnMainView) BurnMainView->ApplyInitialValues();
    ReloadAllElements();
}

void BurnoutMainView::ApplyInitialValues()
{
    auto& modcfg = getConfig().config;
    this->colorOverrideToggle->set_isOn(modcfg["OverrideColors"].GetBool());
    SetButtonColor(leftColorButton, Helpers::ColorHelper::HexToColor(modcfg["LeftColor"].GetString()));
    SetButtonColor(rightColorButton, Helpers::ColorHelper::HexToColor(modcfg["RightColor"].GetString()));
}

void BurnoutMainView::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    BurnMainView = this;
    if (firstActivation && addedToHierarchy)
    {
        auto& modcfg = getConfig().config;
        bool enabled_initval = modcfg["EnableTweaks"].GetBool();
        bool overridecols_initval = modcfg["OverrideColors"].GetBool();
        UnityEngine::Color leftcol_initval = Helpers::ColorHelper::HexToColor(modcfg["LeftColor"].GetString());
        UnityEngine::Color rightcol_initval = Helpers::ColorHelper::HexToColor(modcfg["RightColor"].GetString());

        const std::time_t curtime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        tm loctime = *localtime(&curtime);
        if (loctime.tm_mon == 11 && PresetOptions.size() < 5) PresetOptions.push_back(u"X-mas");

        UnityEngine::UI::VerticalLayoutGroup* container = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_rectTransform());
        container->set_childAlignment(UnityEngine::TextAnchor::UpperLeft);
        container->set_childForceExpandHeight(true);
        container->set_childControlHeight(false);
        
        // Enable button
        UnityEngine::UI::VerticalLayoutGroup* maincontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(container->get_rectTransform());
        maincontainer->set_padding(UnityEngine::RectOffset::New_ctor(2, 2, 2, 2));
        maincontainer->set_childAlignment(UnityEngine::TextAnchor::MiddleLeft);
        maincontainer->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_horizontalFit(2);
        maincontainer->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_verticalFit(2);
        maincontainer->GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredWidth(110);

        QuestUI::BeatSaberUI::CreateText(maincontainer->get_rectTransform(), "Main", false)->set_fontSize(8);
        this->masterEnabledToggle = QuestUI::BeatSaberUI::CreateToggle(maincontainer->get_rectTransform(), "Enable Burnout", enabled_initval, OnChangeEnabled);
        HMUI::SimpleTextDropdown* presetDropdown = QuestUI::BeatSaberUI::CreateDropdown(maincontainer->get_transform(), u"Preset", to_utf16("Custom"), PresetOptions,
        [](std::u16string_view newval) {OnSelectPreset(newval);});

        UnityEngine::UI::VerticalLayoutGroup* subContainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(maincontainer->get_rectTransform());
        subContainer->GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredWidth(110);
        subContainer->GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredHeight(33);
        subContainer->set_childAlignment(UnityEngine::TextAnchor::UpperLeft);
        subContainer->set_childForceExpandHeight(false);
        subContainer->set_childControlHeight(false);
        QuestUI::BeatSaberUI::CreateText(subContainer->get_rectTransform(), "Colors", false)->set_fontSize(8);
        this->colorOverrideToggle = QuestUI::BeatSaberUI::CreateToggle(subContainer->get_rectTransform(), "Override Colors", overridecols_initval, OnChangeOverrideColors);

        this->colorContainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(subContainer->get_rectTransform());
        colorContainer->set_childAlignment(UnityEngine::TextAnchor::UpperLeft);
    
        // UnityEngine::GameObject* leftcol = QuestUI::BeatSaberUI::CreateColorPicker(colorContainer->get_transform(), "Left Saber", leftcol_initval, OnChangeColor);
        this->LeftColorModal = QuestUI::BeatSaberUI::CreateColorPickerModal(colorContainer->get_transform(), "Left Saber", leftcol_initval, OnChangeLeftColor, nullptr, nullptr);
        this->RightColorModal = QuestUI::BeatSaberUI::CreateColorPickerModal(colorContainer->get_transform(), "Right Saber", rightcol_initval, OnChangeRightColor, nullptr, nullptr);

        this->leftColorButton = QuestUI::BeatSaberUI::CreateUIButton(colorContainer->get_transform(), "Left Saber", UnityEngine::Vector2(0, 0), UnityEngine::Vector2(80.0f, 10.0f), [this]{LeftColorModal->Show();});
        this->rightColorButton = QuestUI::BeatSaberUI::CreateUIButton(colorContainer->get_transform(), "Right Saber", UnityEngine::Vector2(0, 0), UnityEngine::Vector2(80.0f, 10.0f), [this]{RightColorModal->Show();});

        leftColorButton->get_transform()->Find(il2cpp_utils::newcsstr("Underline"))->GetComponent<HMUI::ImageView*>()->set_color(leftcol_initval + UnityEngine::Color(0,0,0,1));
        rightColorButton->get_transform()->Find(il2cpp_utils::newcsstr("Underline"))->GetComponent<HMUI::ImageView*>()->set_color(rightcol_initval+ UnityEngine::Color(0,0,0,1));
        leftColorButton->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->set_fontStyle(2);
        rightColorButton->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->set_fontStyle(2);

        // QuestUI::BeatSaberUI::CreateUIButton(colorContainer->get_transform(), "Left Saber", "", []()
        // {
        //     // if (leftcol->)
        //     // if (leftcol) leftcol->Show();
        // });

        // col->GetComponentInChildren<QuestUI::ExternalComponents*>()->Get<UnityEngine::RectTransform*>()->GetChild(0)->get_gameObject()->GetComponent<UnityEngine::RectTransform*>()->set_pivot(UnityEngine::Vector2(0.8f, 0.77f));
        // rightcol->GetComponentInChildren<QuestUI::ExternalComponents*>()->Get<UnityEngine::RectTransform*>()->GetChild(0)->get_gameObject()->GetComponent<UnityEngine::RectTransform*>()->set_pivot(UnityEngine::Vector2(0.8f, 0.77f));

        colorContainer->get_gameObject()->SetActive(overridecols_initval);
    }
    if (!Modloader::getMods().contains("PinkCore"))
    {
        HMUI::ModalView* warnmodal = QuestUI::BeatSaberUI::CreateModal(this->get_transform(), UnityEngine::Vector2(75.0f, 35.0f), nullptr, false);
        
        auto* maincontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(warnmodal->get_transform());
        maincontainer->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);

        auto* titlecontainer = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(maincontainer->get_transform());
        titlecontainer->get_gameObject()->AddComponent<QuestUI::Backgroundable*>()->ApplyBackground(il2cpp_utils::newcsstr("panel-top"));
        titlecontainer->set_padding(UnityEngine::RectOffset::New_ctor(2, 2, 1, 1));
        titlecontainer->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_horizontalFit(2);

        auto* titletext = QuestUI::BeatSaberUI::CreateText(titlecontainer->get_transform(), "PinkCore not detected!");
        titletext->set_alignment(TMPro::TextAlignmentOptions::Center);
        titletext->set_fontSize(7);

        auto* subtext = QuestUI::BeatSaberUI::CreateText(maincontainer->get_transform(), "To use this mod, run a 'Quick Fix' via BMBF.\nThis will reinstall all core mods.");
        subtext->set_fontSize(4.2f);
        subtext->set_alignment(TMPro::TextAlignmentOptions::Center);

        this->StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(custom_types::Helpers::CoroutineHelper::New(DisplayPinkCoreWarning(warnmodal))));
    }
}

void BurnoutMainView::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling)
{
    getConfig().Write();
}
