//
// Created by Nile on 2018/5/7.
//

#include <iostream>
#include <boost/asio.hpp>
#include "ChatServerApplication.h"
#include "ChatServerDelegate.h"

int ChatServerApplication::run() {
    try {
        boost::asio::io_service io_service;
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 9999);
        ChatServerDelegate chatServerDelegate(io_service,endpoint);
        chatServerDelegate.listen();
        io_service.run();
    }
    catch (std::exception &e) {
        std::cerr << "Server Run Exception: " << e.what() << "\n";
        return -1;
    }

    return 0;
}
