#include "Net/Socket/SSLSocketFactory.h"

namespace Xale::Net
{
    SSLSocketFactory::SSLSocketFactory(const std::string& SSLCert, const std::string& SSLKey)
        : _SSLCert(SSLCert), _SSLKey(SSLKey)
    {}
    
#if defined(__linux__) || defined(linux) || defined(__GNUG__)

    std::unique_ptr<ISocket> SSLSocketFactory::createSocket()
    {
        return std::make_unique<LinuxSSLSocket>();
    }

    std::unique_ptr<IListenerSocket> SSLSocketFactory::createListenerSocket()
    {
        return std::make_unique<LinuxSSLListenerSocket>(_SSLCert, _SSLKey);
    }

#elif defined(_WIN32) || defined(_WIN64)

    std::unique_ptr<ISocket> SSLSocketFactory::createSocket()
    {
        return std::make_unique<WindowsSSLSocket>();
    }

    std::unique_ptr<IListenerSocket> SSLSocketFactory::createListenerSocket()
    {
        return std::make_unique<WindowsSSLListenerSocket>(_SSLCert, _SSLKey);
    }

#else
    #error "Unsupported platform"
#endif
}