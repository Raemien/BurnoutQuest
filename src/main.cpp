#include "UI/BurnoutFlowCoordinator.hpp"
#include "Helpers/ColorHelper.hpp"
#include "config.hpp"
using namespace Burnout;

#include <unordered_set>
#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp" 
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"

#include "questui/shared/QuestUI.hpp"
#include "custom-types/shared/register.hpp"

#include "UnityEngine/Quaternion.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Renderer.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/LineRenderer.hpp"
#include "UnityEngine/Color.hpp"


#include "GlobalNamespace/SaberBurnMarkArea.hpp"

static ModInfo modInfo;

Logger& getLogger() {
    static Logger* logger = new Logger(modInfo, LoggerOptions(false, true));
    return *logger;
}

Configuration& getConfig() {
    static Configuration config(modInfo);
    return config;
}

MAKE_HOOK_MATCH(SaberBurnMarkArea_Start, &GlobalNamespace::SaberBurnMarkArea::Start, void, GlobalNamespace::SaberBurnMarkArea* instance)
{
    auto& modcfg = getConfig().config;
    if (modcfg["EnableTweaks"].GetBool())
    {
        if (modcfg["BurnLifetime"].GetFloat() < 0.001f) instance->set_enabled(false);
        SaberBurnMarkArea_Start(instance);
        float clampedOpacity = std::max(0.4f, modcfg["BurnOpacity"].GetFloat());
        instance->burnMarksFadeOutStrength = (5.0f / clampedOpacity) * modcfg["BurnLifetime"].GetFloat();
        Array<UnityEngine::LineRenderer*>* burnRenderers = instance->lineRenderers;
        for (size_t i = 0; i < burnRenderers->Length(); i++)
        {
            UnityEngine::LineRenderer* renderer = burnRenderers->values[i];
            static auto lr_GetStartColor = reinterpret_cast<function_ptr_t<void, UnityEngine::LineRenderer*, ByRef<UnityEngine::Color>>>(il2cpp_functions::resolve_icall("UnityEngine.LineRenderer::get_startColor_Injected"));
            UnityEngine::Color burnColor = UnityEngine::Color::get_white();
            lr_GetStartColor(renderer, burnColor);

            if (modcfg["OverrideColors"].GetBool())
            {
                if (i == 0) burnColor = Helpers::ColorHelper::HexToColor(modcfg["LeftColor"].GetString());
                else burnColor = Helpers::ColorHelper::HexToColor(modcfg["RightColor"].GetString());
            }
            renderer->get_transform()->set_localScale(UnityEngine::Vector3::get_one() * modcfg["BurnScale"].GetFloat());

            burnColor.a = clampedOpacity;
            renderer->set_startColor(burnColor);
            renderer->set_endColor(burnColor);
        }
    }
    else SaberBurnMarkArea_Start(instance);
}


extern "C" void setup(ModInfo& info) {
    info.id = "Burnout";
    info.version = "1.0.0";
    modInfo = info;
    getConfig().Load();
}

extern "C" void load() {
    if (!LoadConfig()) SetupConfig();
    il2cpp_functions::Init();
    QuestUI::Init();
    INSTALL_HOOK(getLogger(), SaberBurnMarkArea_Start); 
    custom_types::Register::AutoRegister();
    QuestUI::Register::RegisterModSettingsFlowCoordinator<::BurnoutFlowCoordinator*>(modInfo);
}
