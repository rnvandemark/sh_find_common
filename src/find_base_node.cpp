#include "sh_find_common/find_base_node.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <jsoncpp/json/json.h>

namespace sh {

// Our REST API implementation to communicate with the FIND server. This uses
// Boost's beast to perform the actual REST calls and JsonCpp to parse the
// responses.
class FindBaseNode::CommImpl
{
protected:
    // The parameters of the FIND server
    const int http_version;
    const std::string server_host;
    const int server_port;

    // Utilities that could or must stay in scope
    boost::asio::io_context ioc;
    boost::beast::tcp_stream stream;
    boost::beast::flat_buffer buffer;
    Json::Reader json_reader;

public:
    // All of the server parameters are requried on creation
    CommImpl(const int http_version,
             const std::string server_host,
             const int server_port) :
        http_version(http_version),
        server_host(server_host),
        server_port(server_port),
        ioc(),
        stream(ioc)
    {
        // Connect our data stream to the server
        stream.connect(
            boost::asio::ip::tcp::resolver(ioc).resolve(
                server_host,
                std::to_string(server_port)
            )
        );
    }

    // Place a GET request and capture the response as a string
    void get(const std::string& target, std::string& rv)
    {
        namespace bbh = boost::beast::http;

        // Create the request and send it
        bbh::request<bbh::string_body> req {bbh::verb::get, target, http_version};
        req.set(bbh::field::host, server_host);
        req.set(bbh::field::user_agent, BOOST_BEAST_VERSION_STRING);
        bbh::write(stream, req);

        // Capture the response
        bbh::response<bbh::dynamic_body> res;
        bbh::read(stream, buffer, res);

        // Convert the captured response to a string
        rv = boost::beast::buffers_to_string(res.body().data());
    }

    // Place a GET request and capture the response into a JSON object
    void get(const std::string& target, Json::Value& json_value)
    {
        std::string rv;
        get(target, rv);
        json_reader.parse(rv, json_value);
    }

    // Ping the FIND server
    bool ping()
    {
        std::string rv;
        get("/ping", rv);
        return "pong" == rv;
    }
};

FindBaseNode::FindBaseNode(const std::string& name) :
    HeartbeatNode(name)
{
}
FindBaseNode::~FindBaseNode()
{
}

bool FindBaseNode::init_communication(
    const std::string& http_version,
    const std::string& server_host,
    int server_port
)
{
    RCLCPP_INFO(
        get_logger(),
        "Interfacing w/ FIND server at '%s:%d' (HTTP version %s)",
        server_host.c_str(),
        server_port,
        http_version.c_str()
    );
    comm.reset(new CommImpl(
        ("1.0" == http_version) ? 10 : 11,
        server_host,
        server_port
    ));
    return comm && comm->ping();
}

}
