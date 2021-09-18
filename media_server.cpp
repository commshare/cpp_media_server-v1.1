#include "rtmp_server.hpp"
#include "ws_server.hpp"
#include "httpflv_server.hpp"
#include "logger.hpp"
#include <stdint.h>
#include <stddef.h>

websocket_server* wss_p = nullptr;
websocket_server* ws_p  = nullptr;

void create_wss_server(boost::asio::io_context& io_context, uint16_t ws_port) {
    const std::string ssl_pem_file = "../certs/server.pem";
    const std::string cert_file = "../certs/server.pem";

    log_infof("websocket https server is starting, port:%d", ws_port);
    wss_p = new websocket_server(io_context, ws_port, WEBSOCKET_IMPLEMENT_PROTOO_TYPE, cert_file, ssl_pem_file);
}

void create_ws_server(boost::asio::io_context& io_context, uint16_t ws_port) {
    ws_p = new websocket_server(io_context, ws_port, WEBSOCKET_IMPLEMENT_PROTOO_TYPE);
    log_infof("websocket http server is starting, port:%d", ws_port);
}

int main(int argn, char** argv) {
    const uint16_t rtmp_def_port = 1935;
    const uint16_t ws_def_port = 1900;
    const uint16_t ws_webrtc_port = 9110;
    const uint16_t httpflv_port = 8080;

    boost::asio::io_context io_context;
    boost::asio::io_service::work work(io_context);

    //Logger::get_instance()->set_filename("server.log");

    try {
        rtmp_server server(io_context, rtmp_def_port);
        websocket_server ws(io_context, ws_def_port, WEBSOCKET_IMPLEMENT_FLV_TYPE);
        httpflv_server httpflv_serv(io_context, httpflv_port);

        create_ws_server(io_context, ws_webrtc_port);
        log_infof("rtmp server start:%d", rtmp_def_port);
        log_infof("websocket server start:%d", ws_def_port);
        io_context.run();
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    return 0;
}