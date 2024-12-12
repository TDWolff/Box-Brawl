#include <iostream>
#include <chrono>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ts/internet.hpp>
#include <thread>

std::vector<char> vBuffer(20 * 1024);

void GrabSomeData(boost::asio::ip::tcp::socket& socket)
{
    socket.async_read_some(boost::asio::buffer(vBuffer.data(), vBuffer.size()),
        [&](std::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                std::cout << "\n\nRead " << length << " bytes\n\n";

                for (int i = 0; i < length; i++)
                    std::cout << vBuffer[i];

                GrabSomeData(socket);
            }
            else
            {
                std::cout << "Error during read: " << ec.message() << std::endl;
            }
        }
    );
}

int main()
{
    boost::system::error_code ec;

    // Create a "context" - platform-specific interface for Boost.Asio
    boost::asio::io_context context;

    // Work object to keep io_context running
    boost::asio::io_context::work idleWork(context);

    // Run the io_context in a separate thread
    std::thread thrContext = std::thread([&]() { context.run(); });

    // Get the address of the server we are connecting to
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address("51.38.81.49", ec), 80);

    // Create a socket, the context will deliver the implementation
    boost::asio::ip::tcp::socket socket(context);

    // Attempt to connect
    socket.connect(endpoint, ec);

    if (!ec)
    {
        std::cout << "Connected!" << std::endl;
    }
    else
    {
        std::cout << "Failed to connect to address:\n" << ec.message() << std::endl;
        return 1;
    }

    if (socket.is_open())
    {
        // Start reading data asynchronously
        GrabSomeData(socket);

        // Send an HTTP GET request
        std::string sRequest = 
            "GET /index.html HTTP/1.1\r\n"
            "Host: example.com\r\n"
            "Connection: close\r\n\r\n";
        socket.write_some(boost::asio::buffer(sRequest.data(), sRequest.size()), ec);

        // Wait for the asynchronous operations to complete (use proper condition to avoid blocking indefinitely)
        std::this_thread::sleep_for(std::chrono::seconds(5));  // Reduce the sleep time to 5 seconds for testing

        context.stop();
        if (thrContext.joinable()) thrContext.join();
    }

    return 0;
}
