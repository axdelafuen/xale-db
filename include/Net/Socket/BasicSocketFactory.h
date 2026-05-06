#ifndef NET_SOCKET_FACTORY_H
#define NET_SOCKET_FACTORY_H

#include "Net/Socket/ISocket.h"
#include "Net/Socket/IListenerSocket.h"
#include "Net/Socket/ISocketFactory.h"

#if defined(__linux__) || defined(linux) || defined(__GNUG__)
#include "Net/Socket/Linux/LinuxSocket.h"
#include "Net/Socket/Linux/LinuxListenerSocket.h"
#elif defined(_WIN32) || defined(_WIN64)
#include "Net/Socket/Windows/WindowsSocket.h"
#include "Net/Socket/Windows/WindowsListenerSocket.h"
#else
#error "Unsupported platform"   
#endif

#include <memory>

namespace Xale::Net
{
    class BasicSocketFactory : public ISocketFactory
    {
        public:
            std::unique_ptr<ISocket> createSocket() override;
            std::unique_ptr<IListenerSocket> createListenerSocket() override;
    };
}

#endif // NET_SOCKET_FACTORY_H