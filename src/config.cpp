#include "modloader/shared/modloader.hpp"
#include "Helpers/ColorHelper.hpp"
#include "main.hpp"

#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"

using namespace Burnout::Helpers;

struct Config_t
{
    bool EnableTweaks = false;
    bool OverrideColors = false;

    float BurnLifetime = 2.0f;
    float BurnOpacity = 1.0f;
    float BurnScale = 1.0f;

    std::string Color_LSaber = ColorHelper::ColorToHex(ColorHelper::Default_Red);
    std::string Color_RSaber = ColorHelper::ColorToHex(ColorHelper::Default_Blue);
}
Settings;

void SetupConfig()
{
    getConfig().config.RemoveAllMembers();
    getConfig().config.SetObject();
    auto& allocator = getConfig().config.GetAllocator();
    getConfig().config.AddMember("EnableTweaks", Settings.EnableTweaks, allocator);
    getConfig().config.AddMember("BurnLifetime", Settings.BurnLifetime, allocator);
    getConfig().config.AddMember("BurnOpacity", Settings.BurnOpacity, allocator);
    getConfig().config.AddMember("BurnScale", Settings.BurnScale, allocator);
    getConfig().config.AddMember("OverrideColors", Settings.OverrideColors, allocator);
    getConfig().config.AddMember("LeftColor", Settings.Color_LSaber, allocator);
    getConfig().config.AddMember("RightColor", Settings.Color_RSaber, allocator);
    getConfig().Write();
}

bool LoadConfig()
{
    getConfig().Load();
    if(!getConfig().config.HasMember("EnableTweaks") || !getConfig().config["EnableTweaks"].IsBool()) return false;
    if(!getConfig().config.HasMember("BurnLifetime") || !getConfig().config["BurnLifetime"].IsFloat()) return false;
    if(!getConfig().config.HasMember("BurnOpacity") || !getConfig().config["BurnOpacity"].IsFloat()) return false;
    if(!getConfig().config.HasMember("BurnScale") || !getConfig().config["BurnScale"].IsFloat()) return false;
    if(!getConfig().config.HasMember("OverrideColors") || !getConfig().config["OverrideColors"].IsBool()) return false;
    if(!getConfig().config.HasMember("LeftColor") || !getConfig().config["LeftColor"].IsString()) return false;
    if(!getConfig().config.HasMember("RightColor") || !getConfig().config["RightColor"].IsString()) return false;
    return true;
}