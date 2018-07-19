//
// Created by Nile on 2018/5/7.
//

#include <iostream>
#include "ChatMessage.h"

void ChatMessage::encode_header() {
    __STNetMsgXpHeader st = {0};
    st.seq = htonl(seq);
    st.cmdid = htonl(cmdid);
    st.head_length = htonl(head_length);
    st.body_length = htonl(body_length());
    st.client_version = htonl(client_version);
    std::memcpy(data_, &st, header_length);
}

bool ChatMessage::decode_header() {
    __STNetMsgXpHeader st = {0};
    memcpy(&st, data_, sizeof(__STNetMsgXpHeader));
    head_length = ntohl(st.head_length);
    client_version = ntohl(st.client_version);
    cmdid = ntohl(st.cmdid);
    seq = ntohl(st.seq);
    body_length_ = ntohl(st.body_length);
    printf("body_length_ is : %d\n",body_length_);
    if (body_length_ > max_body_length) {
        body_length_ = 0;
        printf("decode the header error case the body_length is too long\n");
        return false;
    }
    printf("decode the header successful\n");
    return true;
}

void ChatMessage::body_length(uint32_t new_length) {
    body_length_ = new_length;
    if (body_length_ > max_body_length)
        body_length_ = max_body_length;

}

uint32_t ChatMessage::body_length() const {
    return body_length_;
}

char *ChatMessage::body() {
    return data_ + header_length;
}

const char *ChatMessage::body() const {
    return data_ + header_length;
}

uint32_t ChatMessage::length() const {
    return header_length + body_length_;
}

char *ChatMessage::data() {
    return data_;
}

const char *ChatMessage::data() const {
    return data_;
}



ChatMessage::ChatMessage() : body_length_(0){}

ChatMessage::ChatMessage(__STNetMsgXpHeader &stNetMsgXpHeader) : body_length_(stNetMsgXpHeader.body_length) {}

ChatMessage::~ChatMessage() {
    std::cout << "~ChatMessage()  , cmdid is " << cmdid << std::endl;
}

