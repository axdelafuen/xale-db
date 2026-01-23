#ifndef CORE_CONFIGURATION_PATH_H
#define CORE_CONFIGURATION_PATH_H

#include <string>
#include <filesystem>

#define XALE_CONFIG_DEFAULT_FILE_NAME "appconfig.json"

namespace Xale::Core::Helper
{
    inline std::string getExecutableFolderPath()
    {
        namespace fs = std::filesystem;
        std::error_code ec;

#if defined(__linux__) || defined(linux) || defined(__GNUG__)
        fs::path exe = fs::canonical("/proc/self/exe", ec);
#else
        fs::path exe = fs::current_path(ec);
#endif
        if (ec)
            return {};

        return exe.parent_path().string();
    }

    inline std::string getConfigPath(std::string configFileName = XALE_CONFIG_DEFAULT_FILE_NAME)
    {
        return (getExecutableFolderPath() + "/" + configFileName);
    }
}

#endif // CORE_CONFIGURATION_PATH_H
