//
// Created by Nile on 2018/5/9.
//

#include "MessageHandler.h"
#include "ChatConnection.h"
#include "chat.pb.h"

void MessageHandler::handlerMessage(ChatConnection &connection,const __STNetMsgXpHeader &header,const AutoBuffer &body) {
    uint32_t head_length = ntohl(header.head_length);
    uint32_t client_version = ntohl(header.client_version);
    uint32_t cmdid = ntohl(header.cmdid);
    uint32_t seq = ntohl(header.seq);
    uint32_t body_length_ = ntohl(header.body_length);
    printf("handler the msg of comid is %d the body length is %d\n",cmdid,body_length_);
    switch (cmdid) {
        case 6:{
            //处理心跳检测
            AutoBuffer m_resp_buffer(sizeof(__STNetMsgXpHeader));
            m_resp_buffer.Write(&header, sizeof(header));
            connection.deliver(m_resp_buffer);
            break;
        }
        case 1:{
            //say hello
            chat::proto::SendMessageRequest request;
            request.ParseFromArray(body.Ptr(),body_length_);
            AutoBuffer resp_buffer;
            __STNetMsgXpHeader resp_header;
            memcpy(&resp_header, &header, sizeof(__STNetMsgXpHeader));
            chat::proto::SendMessageResponse response;
            response.set_from("I Received your msg!");
            response.set_err_code(0);
            response.set_topic("aaa");
            response.set_err_msg("bbb");
            response.set_text("ccc");
            resp_header.body_length = htonl(response.ByteSize());
            resp_buffer.AllocWrite(sizeof(resp_header) + response.ByteSize());
            resp_buffer.Write(&resp_header,sizeof(resp_header));
            response.SerializeToArray(resp_buffer.PosPtr(),response.ByteSize());
            printf("send body size is %d\n",response.ByteSize());
//            connection.deliver(resp_buffer);
            break;
        }

        default:{
            break;
        }
    }
}
