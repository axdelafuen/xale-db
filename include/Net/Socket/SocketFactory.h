#ifndef NET_SOCKET_FACTORY_H
#define NET_SOCKET_FACTORY_H

#include "Net/Socket/ISocket.h"
#include "Net/Socket/IListenerSocket.h"

#if defined(__linux__) || defined(linux) || defined(__GNUG__)
#include "Net/Socket/LinuxSocket.h"
#include "Net/Socket/LinuxListenerSocket.h"
#elif defined(_WIN32) || defined(_WIN64)
#include "Net/Socket/WindowsSocket.h"
#include "Net/Socket/WindowsListenerSocket.h"
#else
#error "Unsupported platform"   
#endif

#include <memory>

namespace Xale::Net
{
    class SocketFactory
    {
        public:
            static std::unique_ptr<ISocket> createSocket();
            static std::unique_ptr<IListenerSocket> createListenerSocket();
    };
}

#endif // NET_SOCKET_FACTORY_H