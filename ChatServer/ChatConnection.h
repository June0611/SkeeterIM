//
// Created by Nile on 2018/5/7.
//

#ifndef CHATSERVER_CHATCONNECTION_H
#define CHATSERVER_CHATCONNECTION_H

#include <boost/asio.hpp>
#include <deque>
#include "ChatMessage.h"
#include "MessageHandler.h"
#include "autobuffer.h"
#include "move_wrapper.h"

/*
 * 1.负责接收消息
 * 2.负责发送消息
 *
 * */

typedef std::deque<move_wrapper<AutoBuffer>> ChatMessageQueue;
class ChatConnection : public std::enable_shared_from_this<ChatConnection>{
public:
    ChatConnection(boost::asio::ip::tcp::socket socket,MessageHandler &messageHandler);
    void start();
    void deliver(const AutoBuffer &msg);//发送消息
private:
    void do_read_header() ;
    void do_read_body() ;
    void do_write() ;
    boost::asio::ip::tcp::socket socket_;
    __STNetMsgXpHeader m_header;
    AutoBuffer m_recv_buffer;
    ChatMessageQueue write_msgs_;
    MessageHandler &m_messageHandler_;
};


#endif //CHATSERVER_CHATCONNECTION_H
