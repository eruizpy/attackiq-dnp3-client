#include "CommandLineOptions.h"

CommandLineOptions::CommandLineOptions() :
        cmd("dnp3proxy", ' ', "0.1"),
        hostPort("p", "port", "Port the client connects on", false, 20000, "tcp port"),
        hostAddress("a", "hostAddress", "IPv4 address of the remote host", false, "127.0.0.1", "ipv4 address")
{
    cmd.add(hostPort);
    cmd.add(hostAddress);
}

void CommandLineOptions::Parse(int argc, char** argv)
{
    cmd.parse(argc, argv);
}


