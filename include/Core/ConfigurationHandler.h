#ifndef CORE_CONFIGURATION_HANDLER_H
#define CORE_CONFIGURATION_HANDLER_H

#include "Core/ConfigurationPath.h"

#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <ctime>

namespace Xale::Core 
{
    class ConfigurationHandler 
    {
        public:
            static ConfigurationHandler& getInstance();
            bool loadFromFile(const std::string& path, std::string& outError) noexcept;
            bool isLoaded() const noexcept;
            const std::string& getBuildType() const noexcept;
            const std::string& getDefaultLogLevel() const noexcept;
            const std::string& getExceptionLogLevel() const noexcept;
            const std::string& getLogOutputDirectory() const noexcept;
            const std::string& getLogFileNameFormat() const noexcept;
            const std::string& getDataFilePath() const noexcept;

        private:
            static std::unique_ptr<ConfigurationHandler> instance;
            ConfigurationHandler() = default;
            ConfigurationHandler(const ConfigurationHandler&) = delete;
            ConfigurationHandler& operator=(const ConfigurationHandler&) = delete;
            bool extractStringField(const std::string& text, const std::string& key, std::string& outValue);
            std::tm* getLocaltime();
            std::string getLocaltimeDay();
            std::string getLocaltimeMonth();
            std::string getLocaltimeYear();
            std::string _buildType;
            std::string _defaultLogLevel;
            std::string _exceptionLogLevel;
            std::string _outputFilePath;
            std::string _fileNameFormat;
            std::string _dataFilePath;
            bool _loaded = false;
    };
}

#endif //CORE_CONFIGURATION_HANDLER_H
