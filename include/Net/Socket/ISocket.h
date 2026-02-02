#ifndef NET_I_SOCKET_H
#define NET_I_SOCKET_H

namespace Xale::Net
{
    class ISocket
    {
        public:
            virtual int listen(int port) = 0;
            virtual bool connect(const std::string hostAddress, int port) = 0;
            virtual int send(const std::string* data, size_t size) = 0; // should be SecurePaquet in the future
            virtual int receive(std::string* buffer, size_t size) = 0;
            virtual void close() = 0;
    }
}

#endif // NET_I_SOCKET_H
