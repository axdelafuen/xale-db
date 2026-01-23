#ifndef CORE_EXCEPTION_HANDLER_H
#define CORE_EXCEPTION_HANDLER_H

#include <string>
#include <exception>
#include <iostream>

namespace Xale::Core
{
    enum class ExceptionCode // Should be reviewed
    {
        None = 0,
        Unknown,
        StartupFailed,
        WindowInitFailed,
        VulkanInitFailed,
        OpenGLInitFailed,
        AssetNotFound,
        ShaderCompileError,
    };

    class DbException : public std::exception 
    {
        public:
            DbException(ExceptionCode code, std::string msg, const char* file, int line);
            const char* what() const noexcept override;
            ExceptionCode getCode() const;

        private:
            ExceptionCode code;
            std::string msg;
            std::string fullMessage;
            const char* file;
            int line;
    };
}

#endif // CORE_EXCEPTION_HANDLER_H
