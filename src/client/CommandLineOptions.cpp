#include "CommandLineOptions.h"

CommandLineOptions::CommandLineOptions() :
        cmd("dnp3proxy", ' ', "0.1"),
        connectPort("l", "listenPort", "Port the server listens on", false, 20000, "tcp port"),
        connectAddress("a", "listenAdapter", "Adapter on which the server listens for connection", false, "127.0.0.1", "ipv4 address")
{
    cmd.add(connectPort);
    cmd.add(connectAddress);
}

void CommandLineOptions::Parse(int argc, char** argv)
{
    cmd.parse(argc, argv);
}


