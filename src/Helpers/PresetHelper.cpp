#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "Helpers/ColorHelper.hpp"
#include "Helpers/PresetHelper.hpp"
#include "UnityEngine/Color.hpp"
#include "config.hpp"
#include "main.hpp"
#include <map>

namespace Burnout::Helpers
{
    void PresetHelper::ApplyPreset(TrailPreset preset)
    {
        auto& config = getConfig().config;
        config["BurnLifetime"].SetFloat(preset.Duration);
        config["BurnOpacity"].SetFloat(preset.Intensity);
        config["BurnScale"].SetFloat(preset.Size);
        config["OverrideColors"].SetBool(preset.OverrideColors);
        config["LeftColor"].SetString(ColorHelper::ColorToHex(preset.LeftColor), config.GetAllocator());
        config["RightColor"].SetString(ColorHelper::ColorToHex(preset.RightColor), config.GetAllocator());
        getConfig().Write();
    }

};
