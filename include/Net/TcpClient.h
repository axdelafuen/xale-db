#ifndef NET_TCP_CLIENT_H
#define NET_TCP_CLIENT_H

#include "Net/Socket/SocketFactory.h"

#include <string>
#include <memory>

namespace Xale::Net
{
    class TcpClient
    {
        public:
            TcpClient();
            ~TcpClient();
            bool connect(const std::string& ip, int port);
            int send(const std::string* data, size_t size); // should be SecurePaquet in the future
            int receive(std::string* buffer, size_t size);
            void close();
        private:
            std::unique_ptr<Xale::Net::ISocket> _socket;

    };
}

#endif // NET_TCP_CLIENT_H
