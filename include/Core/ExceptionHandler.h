#ifndef CORE_EXCEPTION_HANDLER_H
#define CORE_EXCEPTION_HANDLER_H

#include <string>
#include <exception>
#include <iostream>
#include <utility>

namespace Xale::Core
{
    enum class ExceptionCode // Should be reviewed
    {
        None = 0,
        Unknown,
        Startup,
        StorageOpen,
        StorageClose,
        ReadFile,
        WriteFile,
        SyncFile,
        DataStruct,
        ParseError,
		ExecutionError,
    };

    class DbException : public std::exception 
    {
        public:
            DbException(ExceptionCode code, std::string msg, const char* file, int line);
            const char* what() const noexcept override;
            ExceptionCode getCode() const;

        private:
            ExceptionCode _code;
            std::string _msg;
            std::string _fullMessage;
            const char* _file;
            int _line;
    };
}

#define THROW_DB_EXCEPTION(code, msg) \
    throw Xale::Core::DbException(code, msg, __FILE__, __LINE__)

#endif // CORE_EXCEPTION_HANDLER_H
