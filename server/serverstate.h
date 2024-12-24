#pragma once

#include "gamepad.h"

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/TCPServer.h>

#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <vector>

class ServerState
{
public:
    ServerState();

public:
    int count() const;
    bool is_running() const;
    
    bool is_connected(int index) const;
    bool is_locked(int index) const;
    std::string client_name(int index) const;
    const Snapshot& latest_snapshot(int index) const;

    void add_gamepad();
    void remove_gamepad();

    void toggle_locked(int index);

    void connect_gamepad(int index, const std::string& client_name);
    void disconnect_gamepad(int index);
    bool update_gamepad(int index, const Snapshot& ss);

    int next_free_gamepad();

    std::string server_host() const;
    uint16_t server_port() const;

    void start_server_async();
    void stop_server();

private:
    struct GamepadState
    {
        Gamepad gamepad;
        bool is_connected;
        bool is_locked;
        std::string client_name = "Samsoong Noot Nein";
        Snapshot latest_snapshot;
    };

private:
    std::atomic<bool> _running;
    std::shared_mutex _mutex;
    std::vector<GamepadState> _gamepad_states;
    Poco::Net::ServerSocket _socket;
    Poco::Net::TCPServer _server;
};