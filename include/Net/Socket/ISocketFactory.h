#ifndef NET_I_SOCKET_FACTORY_H
#define NET_I_SOCKET_FACTORY_H

#include "Net/Socket/ISocket.h"
#include "Net/Socket/IListenerSocket.h"

#include <memory>

namespace Xale::Net
{
    class ISocketFactory
    {
        public:
            virtual std::unique_ptr<ISocket> createSocket() = 0;
            virtual std::unique_ptr<IListenerSocket> createListenerSocket() = 0;
    };
}

#endif // NET_I_SOCKET_FACTORY_H