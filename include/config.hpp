#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"

struct Config_t
{
    bool EnableTweaks;
    bool OverrideColors;

    float BurnLifespan;
    float BurnOpacity;
    float BurnScale;

    std::string Color_LSaber;
    std::string Color_RSaber;
};

bool LoadConfig();
void SetupConfig();