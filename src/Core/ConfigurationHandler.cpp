#include "Core/ConfigurationHandler.h"

namespace Xale::Core 
{
    std::unique_ptr<ConfigurationHandler> ConfigurationHandler::instance = nullptr;

    bool ConfigurationHandler::loadFromFile(const std::string& path, std::string& outError) noexcept
    {
        outError.clear();
        _loaded = false;

        std::string fullPath = Xale::Core::Helper::getConfigPath(path);

        std::ifstream in(fullPath);
        if (!in)
        {
            outError = "Failed to open config file: " + fullPath;
            return false;
        }

        std::ostringstream buffer;
        buffer << in.rdbuf();
        const std::string content = buffer.str();

        if (!extractStringField(content, "Build", _buildType))
        {
            outError = "Missing or invalid 'Build' in config";
            return false;
        }

        if (!extractStringField(content, "Default", _defaultLogLevel))
        {
            outError = "Missing or invalid 'Logging.LogLevel.Default' in config";
            return false;
        }

        if (!extractStringField(content, "Exception", _exceptionLogLevel))
        {
            outError = "Missing or invalid 'Logging.LogLevel.Exception' in config";
            return false;
        }

        if (!extractStringField(content, "OutputFilePath", _outputFilePath))
        {
            outError = "Missing or invalid 'Logging.OutputFilePath' in config";
            return false;
        }
        else
        {
            const std::string root = Xale::Core::Helper::getExecutableFolderPath();
            if (!root.empty())
            {
                size_t pos = 0;
                while ((pos = _outputFilePath.find("__ROOT__", pos)) != std::string::npos)
                {
                    _outputFilePath.replace(pos, 8, root);
                    pos += root.length();
                }
            }
        }

        if (!extractStringField(content, "DataFilePath", _dataFilePath))
        {
            outError = "Missing or invalid 'DataFilePath' in config";
            return false;
        }
        else
        {
            const std::string root = Xale::Core::Helper::getExecutableFolderPath();
            if (!root.empty())
            {
                size_t pos = 0;
                while ((pos = _dataFilePath.find("__ROOT__", pos)) != std::string::npos)
                {
                    _dataFilePath.replace(pos, 8, root);
                    pos += root.length();
                }
            }
        }

        if (!extractStringField(content, "FileNameFormat", _fileNameFormat))
        {
            outError = "Missing or invalid 'Logging.FileNameFormat' in config";
            return false;
        }
        else
        {
            const std::string placeholderDD = "__DD__";
            const std::string placeholderMM = "__MM__";
            const std::string placeholderYYYY = "__YYYY__";
            size_t pos = 0;

            while ((pos = _fileNameFormat.find(placeholderDD, pos)) != std::string::npos)
            {
                _fileNameFormat.replace(pos, placeholderDD.length(), getLocaltimeDay());
                pos += 2;
            }
            pos = 0;
            while ((pos = _fileNameFormat.find(placeholderMM, pos)) != std::string::npos)
            {
                _fileNameFormat.replace(pos, placeholderMM.length(), getLocaltimeMonth());
                pos += 2;
            }
            pos = 0;
            while ((pos = _fileNameFormat.find(placeholderYYYY, pos)) != std::string::npos)
            {
                _fileNameFormat.replace(pos, placeholderYYYY.length(), getLocaltimeYear());
                pos += 4;
            }
        }

        _loaded = true;
        return true;
    }

    bool ConfigurationHandler::isLoaded() const noexcept
    { 
        return _loaded; 
    }

    const std::string& ConfigurationHandler::getBuildType() const noexcept 
    { 
        return _buildType;
    }

    const std::string& ConfigurationHandler::getDefaultLogLevel() const noexcept 
    { 
        return _defaultLogLevel;
    }

    const std::string& ConfigurationHandler::getExceptionLogLevel() const noexcept 
    { 
        return _exceptionLogLevel; 
    }

    const std::string& ConfigurationHandler::getLogOutputDirectory() const noexcept 
    { 
        return _outputFilePath; 
    }

    const std::string& ConfigurationHandler::getLogFileNameFormat() const noexcept 
    { 
        return _fileNameFormat; 
    }

    const std::string& ConfigurationHandler::getDataFilePath() const noexcept 
    { 
        return _dataFilePath; 
    }

    bool ConfigurationHandler::extractStringField(const std::string& text, const std::string& key, std::string& outValue)
    {
        const std::string pattern = "\"" + key + "\"";
        std::size_t pos = text.find(pattern);
        if (pos == std::string::npos)
            return false;

        pos = text.find(':', pos + pattern.size());
        if (pos == std::string::npos)
            return false;

        pos = text.find('"', pos);
        if (pos == std::string::npos)
            return false;

        std::size_t end = text.find('"', pos + 1);
        if (end == std::string::npos)
            return false;

        outValue = text.substr(pos + 1, end - (pos + 1));
        return true;
    }

    ConfigurationHandler& ConfigurationHandler::getInstance()
    {
        if (!instance) {
            instance.reset(new ConfigurationHandler());
        }
        return *instance;
    }

    std::tm* ConfigurationHandler::getLocaltime()
    {
        std::time_t t = std::time(0);
        std::tm* now = std::localtime(&t);
        return now;
    }

    std::string ConfigurationHandler::getLocaltimeDay() 
    {
        std::tm* now = getLocaltime();
        const int intDay = now->tm_mday;
        std::string stringDay = std::to_string(intDay);

        if (stringDay.length() == 0)
            return "01";

        if (stringDay.length() == 1)
            stringDay.insert(0, "0");

        return stringDay;
    }
    
    std::string ConfigurationHandler::getLocaltimeMonth() 
    {
        std::tm* now = getLocaltime();
        const int intMonth = now->tm_mon + 1;
        std::string stringMonth = std::to_string(intMonth);

        if (stringMonth.length() == 0)
            return "01";

        if (stringMonth.length() == 1)
            stringMonth.insert(0, "0");

        return stringMonth;
    }

    std::string ConfigurationHandler::getLocaltimeYear() 
    {
        std::tm* now = getLocaltime();
        const int intYear = now->tm_year + 1900;
        std::string stringYear = std::to_string(intYear);

        if (stringYear.length() == 0)
            return "1900";

        return stringYear;
    }
}
