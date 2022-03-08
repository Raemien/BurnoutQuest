#include "questui/shared/BeatSaberUI.hpp"
#include "custom-types/shared/macros.hpp"
#include "questui/shared/CustomTypes/Components/Settings/IncrementSetting.hpp"
#include "HMUI/ViewController.hpp"

DECLARE_CLASS_CODEGEN(Burnout, BurnoutSideView, HMUI::ViewController,

    // Parented to ViewController
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Toggle*, masterEnabledToggle);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::VerticalLayoutGroup*, container);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::VerticalLayoutGroup*, configcontainer);

    // Increments
    DECLARE_INSTANCE_FIELD(QuestUI::IncrementSetting*, burnScaleSetting);
    DECLARE_INSTANCE_FIELD(QuestUI::IncrementSetting*, burnLifetimeSetting);
    DECLARE_INSTANCE_FIELD(QuestUI::IncrementSetting*, burnIntensitySetting);

    // Methods
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, DidDeactivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidDeactivate", 2), bool removedFromHierarchy, bool systemScreenDisabling);
    DECLARE_INSTANCE_METHOD(void, ReloadValues);
    DECLARE_INSTANCE_METHOD(void, OnChangeVisibility, bool);
)