#ifndef NET_TCP_SERVER_H
#define NET_TCP_SERVER_H

#include <Logger.h>

#include "Net/Socket/ISocketFactory.h"
#include "Net/Socket/IListenerSocket.h"
#include "Net/Socket/IClientConnection.h"

#include "Net/Packet/Packet.h"
#include "Net/Packet/PacketConstants.h"

#include "Engine/QueryEngine.h" // TODO: remove when injecting from outside

#include <string>
#include <memory>
#include <mutex>

namespace Xale::Net
{
    class TcpServer
    {
        public:
            TcpServer(Xale::Engine::QueryEngine& queryEngine, std::unique_ptr<Xale::Net::ISocketFactory> socketFactory);
            ~TcpServer();

            /**
             * @brief Start the server on the given port (blocking accept loop)
             * @param port TCP port to listen on
             */
            bool start(int port);

            /**
             * @brief Stop the server and close the listening socket
             */
            void stop();

        private:
            Xale::Logger::Logger<TcpServer>& _logger;
            std::unique_ptr<Xale::Net::IListenerSocket> _serverSocket;
            std::unique_ptr<Xale::Net::ISocketFactory>  _socketFactory;
            Xale::Engine::QueryEngine& _queryEngine; // TODO: inject from outside
            std::mutex _queryMutex; ///< Protects QueryEngine from concurrent access

            /**
             * @brief Handle a single client connection in a dedicated thread
             * @param conn The client connection (takes ownership)
             */
            void handleClient(std::unique_ptr<IClientConnection> conn);
    };
}

#endif // NET_TCP_SERVER_H
