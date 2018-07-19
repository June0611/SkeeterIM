//
// Created by Nile on 2018/5/7.
//

#include "ChatConnection.h"

void ChatConnection::start() {
    do_read_header();
}

void ChatConnection::deliver(const AutoBuffer &msg) {

    printf("write length is    %zu\n",msg.Length());
    msg.Ptr();
    printf("%p",msg.Ptr());
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if (!write_in_progress) {
        do_write();
    }
}

void ChatConnection::do_read_header() {
    auto self(shared_from_this());
    //1.读取消息头,失败后返回失败,移动端会断开重新连接,重发消息
    boost::asio::async_read(socket_, boost::asio::buffer(&m_header, sizeof(m_header)),
                            [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                                if (!ec) {
                                    do_read_body();
                                }
                                else {
                                    //掉线
                                    printf("close\n");
                                    // room_.leave(shared_from_this());
                                }
                            });
}

void ChatConnection::do_read_body() {

    auto self(shared_from_this());
    //根据头读取消息体 -- 可能会导致多次开辟空间
    uint32_t head_length = ntohl(m_header.head_length);
    uint32_t client_version = ntohl(m_header.client_version);
    uint32_t cmdid = ntohl(m_header.cmdid);
    uint32_t seq = ntohl(m_header.seq);
    uint32_t body_length_ = ntohl(m_header.body_length);
    if (m_recv_buffer.Capacity() - m_recv_buffer.Length() < body_length_) {
        m_recv_buffer.AddCapacity(body_length_);
    }
    boost::asio::async_read(socket_,
                            boost::asio::buffer((char*)m_recv_buffer.Ptr() + m_recv_buffer.Length(), body_length_),
                            [this, self](boost::system::error_code ec, std::size_t length) {
                                if (!ec) {
                                    //进行消息分发
                                    m_recv_buffer.Length(m_recv_buffer.Pos(),length);
                                    m_messageHandler_.handlerMessage(*this,m_header,m_recv_buffer);
                                    //继续监听
                                    do_read_header();
                                }
                                else {
                                    printf("close\n");
//                                    room_.leave(shared_from_this());
                                }
                            });

}

void ChatConnection::do_write() {
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(write_msgs_.front().get().Ptr(), write_msgs_.front().get().Length()),
                             [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                                 if (!ec) {
                                     write_msgs_.pop_front();
                                     if (!write_msgs_.empty()) {
                                         do_write();
                                     }
                                 }
                                 else {
                                     printf("close");
//                                     room_.leave(shared_from_this());
                                 }
                             });

}


ChatConnection::ChatConnection(boost::asio::ip::tcp::socket socket,MessageHandler &messageHandler)
        : socket_(std::move(socket)),m_messageHandler_(messageHandler){
    printf("ChatConnection    ------   %p\n",&m_messageHandler_);
}

