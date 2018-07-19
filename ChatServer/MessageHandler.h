//
// Created by Nile on 2018/5/9.
//

#ifndef CHATSERVER_MESSAGEHANDLER_H
#define CHATSERVER_MESSAGEHANDLER_H

#include "autobuffer.h"
#include "ChatMessage.h"
class ChatConnection; // avoid the header file circulation include ,
class MessageHandler {

public:
    //handler the msg did the connection object received
    void handlerMessage(ChatConnection &connection,const __STNetMsgXpHeader &header,const AutoBuffer &body);
//private:
//    AutoBuffer m_resp_buffer;

};


#endif //CHATSERVER_MESSAGEHANDLER_H
