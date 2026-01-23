#ifndef XALE_INIT_SETUP_H
#define XALE_INIT_SETUP_H

#include "Logger.h"
#include "Core/ConfigurationHandler.h"

#include <string>
#include <iostream>

namespace Xale::Core::Init 
{
    int setup(std::string& error)
    {
        auto& configHandler = Xale::Core::ConfigurationHandler::getInstance();
        if (!configHandler.loadFromFile("appconfig.json", error))
            return  false;
        else
        {
            auto& logger = Xale::Logger::Logger<void>::getInstance();

            std::string buildType = configHandler.getBuildType();
            if (buildType != "Debug" && buildType != "Release")
            {
                error = "Invalid build type \'" + buildType + "\', in configuration. Expected \'Debug\' or \'Release\'";
                return false;
            }
            else if (buildType == "Debug")
                logger.setIsDebugEnable(true);
            else // Release
                logger.setIsDebugEnable(false);

            std::string logOutputDir = configHandler.getLogOutputDirectory();
            std::string logFileName = configHandler.getLogFileNameFormat();
            if (!logOutputDir.empty() && !logFileName.empty())
            {
                if (logOutputDir.back() == '/' || logOutputDir.back() == '\\')
                    logOutputDir.pop_back();

                std::cout << logOutputDir << "/" << logFileName << std::endl;
                logger.setLogFilePath(logOutputDir + "/" + logFileName);
                logger.setLogToFile(true);
            }
            else
                logger.setLogToFile(false);

            logger.debug("Configuration loaded successfully");
            logger.debug("Build Type: " + buildType);
            logger.debug("Default Log Level: " + configHandler.getDefaultLogLevel());
            logger.debug("Exception Log Level: " + configHandler.getExceptionLogLevel());
            logger.debug("Log Output Directory: " + logOutputDir);
            logger.debug("Log File Name Format: " + logFileName);

            return true;
        }
    }
}

#endif // XALE_INIT_SETUP_H
