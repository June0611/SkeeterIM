//
// Created by Nile on 2018/5/7.
//

#include <iostream>
#include "ChatServerDelegate.h"
#include "ChatConnection.h"

ChatServerDelegate::ChatServerDelegate(boost::asio::io_service &io_service,
                                       boost::asio::ip::tcp::endpoint &endpoint) : m_acceptor_(io_service, endpoint),
                                                                                   m_socket_(io_service) {}

void ChatServerDelegate::do_accept() {
    m_acceptor_.async_accept(m_socket_, [this](boost::system::error_code ec) {
        if (!ec) {
            //处理事件
            std::cout << "accept client" <<  std::endl;
            std::make_shared<ChatConnection>(std::move(m_socket_),m_messageHandler_)->start();
        }
        printf("ChatServerDelegate ========  %p\n",&m_messageHandler_);
        //继续监听
        listen();
    });
}

void ChatServerDelegate::listen() {
    std::cout << "continue listen" <<  std::endl;
    do_accept();
}
