#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "Helpers/ColorHelper.hpp"
#include "UnityEngine/Color.hpp"
#include <sstream>
#include <vector>
#include "main.hpp"

namespace Burnout::Helpers
{
    UnityEngine::Color ColorHelper::Default_Red = HexToColor("AF1313");
    UnityEngine::Color ColorHelper::Default_Blue = HexToColor("0766B4");

    UnityEngine::Color ColorHelper::HexToColor(std::string hexstr)
    {
        // Please ignore the overengineeriness, it felt appropriate at the time.
        if (hexstr.starts_with('#')) {
            hexstr.erase(0, 1);
            hexstr.shrink_to_fit();
        }
        while (hexstr.length() < 6) hexstr = "0" + hexstr;
        std::vector<float> channels = {0.0f, 0.0f, 0.0f};
        for (size_t i = 0; i < 3; i++)
        {
            unsigned int output;
            std::stringstream sstream;
            sstream << std::hex << hexstr.substr(i*2, 2);
            sstream >> output;
            channels[i] = (float)output / 255.0f;
        }
        return UnityEngine::Color(channels[0], channels[1], channels[2]);
    }

    std::string ColorHelper::ColorToHex(UnityEngine::Color col)
    {
        int red = (int)((col.r * 255.0f) - 0.5f);
        int green = (int)((col.g * 255.0f) - 0.5f);
        int blue = (int)((col.b * 255.0f) - 0.5f);
        std::stringstream total;
        total << std::hex << std::hex << (red << 16 | green << 8 | blue);
        return "#" + std::string(total.str());
    }
};
