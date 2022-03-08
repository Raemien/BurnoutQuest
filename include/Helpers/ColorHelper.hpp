#pragma once
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "UnityEngine/Color.hpp"

namespace Burnout::Helpers
{
    class ColorHelper
    {
        public:
            static UnityEngine::Color HexToColor(std::string hex);
            static std::string ColorToHex(UnityEngine::Color col);

            static UnityEngine::Color Default_Red;
            static UnityEngine::Color Default_Blue;
    };
}
