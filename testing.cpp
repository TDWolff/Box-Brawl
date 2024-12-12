#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ts/internet.hpp>

int main()
{
    boost::system::error_code ec;

    //create a "context" - essentially the platform specific interface
    boost::asio::io_context context;

    // get the address of the server we are connecting to
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address("127.0.0.1", ec), 80);

    // create a socket, the context will deliver the implementation
    boost::asio::ip::tcp::socket socket(context);

    // tell socket to try and connect
    socket.connect(endpoint, ec);

    if (!ec)
    {
        std::cout << "Connected!" << std::endl;
    }
    else
    {
        std::cout << "Failed to connect to address:\n" << ec.message() << std::endl;
    }

    return 0;
}