#ifndef NET_I_LISTENER_SOCKET_H
#define NET_I_LISTENER_SOCKET_H

#include <string>

namespace Xale::Net
{
    class IListenerSocket
    {
        public:
            virtual bool open(int port) = 0;
            virtual int listen(std::string& buffer, size_t size) = 0;
            virtual int respond(const std::string* data, size_t size) = 0; // should be SecurePaquet in the future
            virtual void close() = 0;
    };
}

#endif // NET_I_LISTENER_SOCKET_H
