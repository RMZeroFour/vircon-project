#include "connection.h"

#include <Poco/BinaryReader.h>
#include <Poco/BinaryWriter.h>
#include <Poco/Net/SocketStream.h>

#include <string>

using namespace Poco::Net;
using Poco::BinaryReader;
using Poco::BinaryWriter;

GamepadConnection::GamepadConnection(const StreamSocket& socket, ServerState& server_state)
    : TCPServerConnection{ socket }
    , _server_state{ server_state }
{ }

void GamepadConnection::run()
{
    try
    {
        SocketStream stream{ socket() };
        BinaryReader reader{ stream };
        BinaryWriter writer{ stream };
            
        perform_handshake(reader, writer);
    }
    catch (const Poco::Exception& e)
    {
    }
}

void GamepadConnection::perform_handshake(BinaryReader& reader, BinaryWriter& writer)
{
    enum ResponseCode : uint8_t
    {
        VersionSupported = 0,
        VersionNotSupported = 1,
    };

    uint8_t version{};
    reader >> version;

    switch (version)
    {
    case 1:
        writer << VersionSupported;
        writer.flush();
        handle_version_1(reader, writer);
        break;
    
    default:
        writer << VersionNotSupported;
        writer.flush();
        break;
    }
}

void GamepadConnection::handle_version_1(BinaryReader& reader, BinaryWriter& writer)
{
    enum ResponseCode : uint8_t
    {
        GamepadAvailable = 0,
        NoGamepadAvailable = 1,
    };

    int index{ _server_state.next_free_gamepad() };
    if (index != -1)
    {
        writer << GamepadAvailable;
        writer.flush();

        uint8_t name_length{};
        reader >> name_length;
        if (!reader.fail())
        {
            std::string name{};
            reader.readRaw(name_length, name);
            if (!reader.fail())
            {
                _server_state.connect_gamepad(index, name);
                
                Snapshot ss{};
                uint16_t buttons{ 0 };
                uint64_t axes{ 0 };

                while (_server_state.is_running() && _server_state.update_gamepad(index, ss))
                {
                    reader >> buttons >> axes;
                    if (reader.fail())
                    {
                        break;
                    }

                    ss.a = (buttons >> 0) & 1;
                    ss.b = (buttons >> 1) & 1;
                    ss.x = (buttons >> 2) & 1;
                    ss.y = (buttons >> 3) & 1;
                    ss.l1 = (buttons >> 4) & 1;
                    ss.r1 = (buttons >> 5) & 1;
                    ss.l2 = (buttons >> 6) & 1;
                    ss.r2 = (buttons >> 7) & 1;
                    ss.up = (buttons >> 8) & 1;
                    ss.down = (buttons >> 9) & 1;
                    ss.left = (buttons >> 10) & 1;
                    ss.right = (buttons >> 11) & 1;
                    ss.select = (buttons >> 12) & 1;
                    ss.start = (buttons >> 13) & 1;

                    ss.lx = (axes >> 0) & 0xFFFF;
                    ss.ly = (axes >> 16) & 0xFFFF;
                    ss.rx = (axes >> 32) & 0xFFFF;
                    ss.ry = (axes >> 48) & 0xFFFF;
                }

                _server_state.disconnect_gamepad(index);
            }
        }
    }
    else
    {
        writer << NoGamepadAvailable;
        writer.flush();
    }
}
