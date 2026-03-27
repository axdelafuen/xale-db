#include "Core/Setup.h"

namespace Xale::Core 
{
    std::unique_ptr<Setup> Setup::_instance = nullptr;

    Setup::Setup() 
        : _logger(Xale::Logger::Logger<Setup>::getInstance())
    {
    }

    Setup::~Setup()
    {
        shutdown();
    }

    Setup& Setup::getInstance()
    {
        if (!_instance)
            _instance = std::unique_ptr<Setup>(new Setup());
        return *_instance;
    }

    bool Setup::initialize()
    {
        if (_isSetupDone)
            return true;
        else
        {
            std::string error;
            auto& configHandler = Xale::Core::ConfigurationHandler::getInstance();
            if (!configHandler.loadFromFile(CONFIG_FILE_PATH, error))
            {
                _logger.error("Failed to load configuration: " + error);
                return false;
            }

            std::string buildType = configHandler.getBuildType();
            if (buildType != "Debug" && buildType != "Release")
            {
                _logger.error("Invalid build type \'" + buildType + "\', in configuration. Expected \'Debug\' or \'Release\'");
                return false;
            }
            else if (buildType == "Debug")
                Xale::Logger::Logger<void>::setIsDebugEnable(true);

            // Setup logger
            std::string logOutputDir = configHandler.getLogOutputDirectory();
            std::string logFileName = configHandler.getLogFileNameFormat();
            if (!logOutputDir.empty() && !logFileName.empty())
            {
                if (logOutputDir.back() == '/' || logOutputDir.back() == '\\')
                    logOutputDir.pop_back();

                std::cout << logOutputDir << "/" << logFileName << std::endl;
                Xale::Logger::Logger<void>::setLogFilePath(logOutputDir + "/" + logFileName);
                Xale::Logger::Logger<void>::setLogToFile(true);
            }
            else
            {
                Xale::Logger::Logger<void>::setLogToFile(false);
            }

            if (configHandler.useSSL())
            {
                const std::string& SSLCert = configHandler.getServerSSLCert();
                const std::string& SSLKey = configHandler.getServerSSLKey();

                _socketFactory = std::make_unique<Xale::Net::SSLSocketFactory>(SSLCert, SSLKey);
                _logger.debug("SSL enabled for network communication");
            }
            else
            {
                _socketFactory = std::make_unique<Xale::Net::BasicSocketFactory>();
                _logger.debug("SSL disabled for network communication");
            }
            
            _logger.debug("Configuration loaded successfully");
            _logger.debug("Build Type: " + buildType);
            _logger.debug("Default Log Level: " + configHandler.getDefaultLogLevel());
            _logger.debug("Exception Log Level: " + configHandler.getExceptionLogLevel());
            _logger.debug("Log Output Directory: " + logOutputDir);
            _logger.debug("Log File Name Format: " + logFileName);
            _logger.debug("Data File Path: " + configHandler.getDataFilePath());
            _logger.debug("Use SSL: " + std::string(configHandler.useSSL() ? "true" : "false"));
            _logger.debug("SSL Cert File: " + configHandler.getServerSSLCert());
            _logger.debug("SSL Key File: " + configHandler.getServerSSLKey());

            // Setup engines

            const std::string dataFilePath = configHandler.getDataFilePath();
            if (dataFilePath.empty())
            {
                _logger.error("Engine storage file path is not set in configuration file (" + CONFIG_FILE_PATH + ")");
                return false;
            }

            _execFm = std::make_unique<Xale::Storage::BinaryFileManager>();
            _fileStorageEngine = std::make_unique<Xale::Storage::FileStorageEngine>(*_execFm, dataFilePath);;
            if (!_fileStorageEngine->startup())
            {
                _logger.error("Executor StorageEngine startup failed");
                return false;
            }
            _parserTokenizer =  std::make_unique<Xale::Query::BasicTokenizer>();
            _parser = std::make_unique<Xale::Query::BasicParser>(_parserTokenizer.get());
            _tableManager = std::make_unique<Xale::Execution::TableManager>(*_fileStorageEngine, *_execFm);
            _executor = std::make_unique<Xale::Execution::BasicExecutor>(*_tableManager);
            _queryEngine = std::make_unique<Xale::Engine::QueryEngine>(_parser.get(), _executor.get());
            _isSetupDone = true;

            return true;
        }
    }

    Xale::Engine::QueryEngine& Setup::getQueryEngine()
    {
        return *_queryEngine;
    }

    std::unique_ptr<Xale::Net::ISocketFactory>& Setup::getSocketFactory()
    {
        return _socketFactory;
    }

    bool Setup::isInitialized() const
    {
        return _isSetupDone;
    }

    void Setup::shutdown()
    {
        if (_isSetupDone)
        {
            _queryEngine.reset();
            _executor.reset();
            _tableManager.reset();
            _parser.reset();
            _parserTokenizer.reset();
            if (_fileStorageEngine)
            {
                _fileStorageEngine->shutdown();
                _fileStorageEngine.reset();
            }
            if (_execFm)
            {
                _execFm.reset();
            }
            if (_socketFactory)
            {
                _socketFactory.reset();
            }
            _isSetupDone = false;
        }
    }
}