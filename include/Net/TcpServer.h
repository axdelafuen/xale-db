#ifndef NET_TCP_SERVER_H
#define NET_TCP_SERVER_H

#include <Logger.h>

#include "Net/Socket/ISocketFactory.h"
#include "Net/Socket/IListenerSocket.h"

#include "Net/Packet/Packet.h"
#include "Net/Packet/PacketConstants.h"

#include "Engine/QueryEngine.h" // TODO: remove when injecting from outside

#include <string>
#include <memory>

namespace Xale::Net
{
    class TcpServer
    {
        public:
            TcpServer(Xale::Engine::QueryEngine& queryEngine, std::unique_ptr<Xale::Net::ISocketFactory> socketFactory);
            ~TcpServer();
            bool start(int port);
            void stop();
        private:
            Xale::Logger::Logger<TcpServer>& _logger;
            std::unique_ptr<Xale::Net::IListenerSocket> _serverSocket;
            std::unique_ptr<Xale::Net::ISocketFactory> _socketFactory;

            Xale::Engine::QueryEngine& _queryEngine; // TODO: inject from outside
    };
}

#endif // NET_TCP_SERVER_H