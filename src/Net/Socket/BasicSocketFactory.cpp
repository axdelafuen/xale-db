#include "Net/Socket/BasicSocketFactory.h"

namespace Xale::Net
{
#if defined(__linux__) || defined(linux) || defined(__GNUG__)

    std::unique_ptr<ISocket> BasicSocketFactory::createSocket()
    {
        return std::make_unique<LinuxSocket>();
    }

    std::unique_ptr<IListenerSocket> BasicSocketFactory::createListenerSocket()
    {
        return std::make_unique<LinuxListenerSocket>();
    }

#elif defined(_WIN32) || defined(_WIN64)

    std::unique_ptr<ISocket> BasicSocketFactory::createSocket()
    {
        return std::make_unique<WindowsSocket>();
    }

    std::unique_ptr<IListenerSocket> BasicSocketFactory::createListenerSocket()
    {
        return std::make_unique<WindowsListenerSocket>();
    }

#else
    #error "Unsupported platform"
#endif
}