#include "questui/shared/BeatSaberUI.hpp"
#include "custom-types/shared/macros.hpp"
#include "questui/shared/CustomTypes/Components/ModalColorPicker.hpp"
#include "HMUI/ViewController.hpp"

#include "UnityEngine/UI/Button.hpp"

DECLARE_CLASS_CODEGEN(Burnout, BurnoutMainView, HMUI::ViewController,

    // Parented to ViewController
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Toggle*, masterEnabledToggle);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::VerticalLayoutGroup*, colorContainer);
    
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Toggle*, colorOverrideToggle);
    DECLARE_INSTANCE_FIELD(QuestUI::ModalColorPicker*, LeftColorModal);
    DECLARE_INSTANCE_FIELD(QuestUI::ModalColorPicker*, RightColorModal);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, leftColorButton);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, rightColorButton);

    // Methods
    DECLARE_INSTANCE_METHOD(void, ApplyInitialValues);
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, DidDeactivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidDeactivate", 2), bool removedFromHierarchy, bool systemScreenDisabling);
)