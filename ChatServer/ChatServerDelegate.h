//
// Created by Nile on 2018/5/7.
//

#ifndef CHATSERVER_CHATSERVERDELEGATE_H
#define CHATSERVER_CHATSERVERDELEGATE_H


#include <boost/asio.hpp>
#include "MessageHandler.h"

class ChatServerDelegate {
public:
    ChatServerDelegate(boost::asio::io_service &io_service,boost::asio::ip::tcp::endpoint &endpoint);
    void listen();
private:
    void do_accept();
    boost::asio::ip::tcp::socket m_socket_;
    boost::asio::ip::tcp::acceptor m_acceptor_;
    MessageHandler m_messageHandler_;
};


#endif //CHATSERVER_CHATSERVERDELEGATE_H
