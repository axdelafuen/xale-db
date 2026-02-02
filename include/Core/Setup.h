#ifndef CORE_XALE_SETUP_H
#define CORE_XALE_SETUP_H

#include <Logger.h>
#include "Core/ConfigurationHandler.h"
#include "Engine/QueryEngine.h"
#include "Storage/BinaryFileManager.h"
#include "Storage/FileStorageEngine.h"
#include "Query/BasicTokenizer.h"
#include "Query/BasicParser.h"
#include "Execution/TableManager.h"
#include "Execution/BasicExecutor.h"

#include <string>
#include <iostream>
#include <memory>

const std::string CONFIG_FILE_PATH = "appconfig.json";

namespace Xale::Core
{
    class Setup {
        public:
            ~Setup() = default;
            static Setup& getInstance();
            bool initialize();
            void shutdown();
            Xale::Engine::QueryEngine& getQueryEngine();
            bool isInitialized() const;
        private:
            static std::unique_ptr<Setup> _instance;
            Setup();
            Setup(const Setup&) = delete;
            Setup& operator=(const Setup&) = delete;
            bool _isSetupDone = false;
            Xale::Logger::Logger<Setup>& _logger;
            std::unique_ptr<Xale::Storage::BinaryFileManager> _execFm;
            std::unique_ptr<Xale::Storage::FileStorageEngine> _fileStorageEngine;
            std::unique_ptr<Xale::Query::BasicTokenizer> _parserTokenizer;
            std::unique_ptr<Xale::Query::BasicParser> _parser;
            std::unique_ptr<Xale::Execution::TableManager> _tableManager;
            std::unique_ptr<Xale::Execution::BasicExecutor> _executor;
            std::unique_ptr<Xale::Engine::QueryEngine> _queryEngine;
    };
}

#endif // CORE_XALE_SETUP_H
