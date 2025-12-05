cmake_minimum_required(VERSION 3.23)

project(ws_server)

add_executable(WsServer)

target_sources(WsServer
        PRIVATE
            advanced_server.cpp)