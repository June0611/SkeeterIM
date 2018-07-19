//
// Created by Nile on 2018/5/7.
//

#ifndef CHATSERVER_CHATMESSAGE_H
#define CHATSERVER_CHATMESSAGE_H
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/dtrace.h>


static uint32_t sg_client_version = 200;
#pragma pack(push, 1)
struct __STNetMsgXpHeader {
    uint32_t    head_length;
    uint32_t    client_version;
    uint32_t    cmdid;
    uint32_t    seq;
    uint32_t	body_length;
};
#pragma pack(pop)

class ChatMessage {
public:
    enum { header_length = 20 };
    enum { max_body_length = 1024 * 64 };

    //Constructor
    ChatMessage();
    //Constructor
    ChatMessage(__STNetMsgXpHeader &stNetMsgXpHeader);
    ~ChatMessage();

    const char* data() const ;

    char* data() ;

    uint32_t length() const ;

    const char* body() const ;

    char* body() ;

    uint32_t body_length() const ;

    void body_length(uint32_t new_length) ;

    bool decode_header();

    void encode_header();

private:
    //dynamic alloc memory
    char *data_;
    uint32_t body_length_;
    uint32_t head_length;
    uint32_t client_version;
    uint32_t cmdid;
    uint32_t seq;
public:

};



#endif //CHATSERVER_CHATMESSAGE_H
