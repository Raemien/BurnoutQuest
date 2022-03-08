#pragma once
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "beatsaber-hook/shared/utils/utils.h"
#include "UnityEngine/Color.hpp"
#include <map>


namespace Burnout::Helpers
{

            struct TrailPreset
            {
                TrailPreset(float duration, float intensity, float size, bool overridecols, UnityEngine::Color col_l, UnityEngine::Color col_r)
                {
                    Duration = duration;
                    Intensity = intensity;
                    Size = size;
                    LeftColor = col_l;
                    RightColor = col_r;
                    OverrideColors = overridecols;
                }
                TrailPreset()
                {
                    Duration = 0.5f;
                    Intensity = 0.5f;
                    Size = 0.5f;
                    LeftColor = UnityEngine::Color::get_red();
                    RightColor = UnityEngine::Color::get_blue();
                    OverrideColors = false;
                }


                float Duration;
                float Intensity;
                float Size;
                bool OverrideColors;
                UnityEngine::Color LeftColor;
                UnityEngine::Color RightColor;
            };

    class PresetHelper
    {
        public:
            static void ApplyPreset(TrailPreset preset);
    };
}
