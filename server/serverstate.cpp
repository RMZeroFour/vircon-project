#include "serverstate.h"

#include "connection.h"

#include <Poco/Net/IPAddress.h>
#include <Poco/Net/NetworkInterface.h>
#include <Poco/Net/TCPServerConnectionFactory.h>
#include <Poco/Net/TCPServerParams.h>

#include <array>

using namespace Poco::Net;

namespace
{
class GamepadConnectionFactory
    : public TCPServerConnectionFactory
{
public:
    GamepadConnectionFactory(ServerState& server_state)
        : _server_state{ server_state }
    { }

public:
	TCPServerConnection* createConnection(const StreamSocket& socket)
	{
		return new GamepadConnection(socket, _server_state);
	}

private:
    ServerState& _server_state;
};

SocketAddress get_socket_endpoint()
{
    IPAddress address{};
    for (const NetworkInterface& iface : NetworkInterface::list())
    {
        if (iface.isRunning() && !iface.isLoopback())
        {
            iface.firstAddress(address);
            break;
        }
    }
    return { address, 0 };
}

TCPServer create_server(const ServerSocket& socket, ServerState& server_state)
{
    auto factory{ new GamepadConnectionFactory{ server_state } };

    auto params{ new TCPServerParams{} };
    params->setMaxThreads(4);  
    params->setMaxQueued(4);
    params->setThreadIdleTime(100);

    return { factory, socket, params };
}
}

ServerState::ServerState()
    : _running{}
    , _mutex{}
    , _gamepad_states{}
    , _socket{ get_socket_endpoint() }
    , _server{ create_server(_socket, *this) }
{}

int ServerState::count() const
{
    return _gamepad_states.size();
}

bool ServerState::is_running() const
{
    return _running.load();
}

bool ServerState::is_connected(int index) const
{
    return _gamepad_states.at(index).is_connected;
}

bool ServerState::is_locked(int index) const
{
    return _gamepad_states.at(index).is_locked;
}

std::string ServerState::client_name(int index) const
{
    return _gamepad_states.at(index).client_name;
}

const Snapshot& ServerState::latest_snapshot(int index) const
{
    return _gamepad_states.at(index).latest_snapshot;
}

void ServerState::add_gamepad()
{
    std::unique_lock ul{ _mutex }; 
    _gamepad_states.emplace_back();
}

void ServerState::remove_gamepad()
{
    std::unique_lock ul{ _mutex }; 
    _gamepad_states.pop_back();
}

void ServerState::toggle_locked(int index)
{
    std::shared_lock sl{ _mutex };
    _gamepad_states[index].is_locked = !_gamepad_states[index].is_locked;
}

void ServerState::connect_gamepad(int index, const std::string& client_name)
{
    std::shared_lock sl{ _mutex }; 
    _gamepad_states[index].is_connected = true;
    _gamepad_states[index].client_name = client_name;
}

void ServerState::disconnect_gamepad(int index)
{
    std::shared_lock sl{ _mutex };
    _gamepad_states[index].is_connected = false;
    _gamepad_states[index].client_name = "";
}

bool ServerState::update_gamepad(int index, const Snapshot& ss)
{
    std::shared_lock sl{ _mutex };

    if (index >= _gamepad_states.size())
    {
        return false;
    }

    GamepadState& gps{ _gamepad_states[index] };
    if (!gps.is_locked)
    {
        gps.latest_snapshot = ss;
        gps.gamepad.send_input(ss);
    }

    return true;
}

int ServerState::next_free_gamepad()
{
    std::unique_lock ul{ _mutex };

    for (int i = 0; i < _gamepad_states.size(); ++i)
    {
        if (!_gamepad_states[i].is_connected)
        {
            return i;
        }
    }

    return -1;
}

std::string ServerState::server_host() const
{
    return _socket.address().host().toString();
}

uint16_t ServerState::server_port() const
{
    return _socket.address().port();
}

void ServerState::start_server_async()
{
    _running.store(true);
    _server.start();
}

void ServerState::stop_server()
{
    _server.stop();
    _running.store(false);
}
