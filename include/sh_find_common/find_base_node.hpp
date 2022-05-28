#pragma once

#include <sh_common/heartbeat_node.hpp>

namespace sh {

class FindBaseNode : public HeartbeatNode
{
protected:
    // Forward declare
    class CommImpl;
    // The implementation for our communication with the FIND database
    std::unique_ptr<CommImpl> comm;

public:
    FindBaseNode(const std::string& name);
    ~FindBaseNode();

    // Create /a comm to establish communication with the FIND server
    bool init_communication(
        const std::string& http_version,
        const std::string& server_host,
        int server_port
    );
};

}
