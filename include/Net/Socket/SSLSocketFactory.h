#ifndef NET_SSL_SOCKET_FACTORY_H
#define NET_SSL_SOCKET_FACTORY_H

#include "Net/Socket/ISocket.h"
#include "Net/Socket/IListenerSocket.h"
#include "Net/Socket/ISocketFactory.h"

#if defined(__linux__) || defined(linux) || defined(__GNUG__)
#include "Net/Socket/Linux/LinuxSSLSocket.h"
#include "Net/Socket/Linux/LinuxSSLListenerSocket.h"
#elif defined(_WIN32) || defined(_WIN64)
#include "Net/Socket/Windows/WindowsSSLSocket.h"
#include "Net/Socket/Windows/WindowsSSLListenerSocket.h"
#else
#error "Unsupported platform"   
#endif

#include <memory>
#include <string>

namespace Xale::Net
{
    class SSLSocketFactory : public ISocketFactory
    {
        public:
            SSLSocketFactory(const std::string& SSLCert, const std::string& SSLKey);
            std::unique_ptr<ISocket> createSocket() override;
            std::unique_ptr<IListenerSocket> createListenerSocket() override;
        private:
            std::string _SSLCert;
            std::string _SSLKey;
    };
}

#endif // NET_SSL_SOCKET_FACTORY_H