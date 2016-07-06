#include "CommandLineOptions.h"

CommandLineOptions::CommandLineOptions() :
        cmd("dnp3proxy", ' ', "0.1"),
        hostPort("p", "port", "Port the client connects on", false, 20000, "tcp port"),
        hostAddress("a", "hostAddress", "IPv4 address of the remote host", false, "127.0.0.1", "ipv4 address"),
        masterAddress("m", "masterAddress", "DNP3 address of the local master", false, 1, "master address"),
        outstationAddress("o", "outstationAddress", "DNP3 address of the remote outstation", false, 1024, "outstation address")
{
    cmd.add(hostPort);
    cmd.add(hostAddress);
    cmd.add(masterAddress);
    cmd.add(outstationAddress);
}

void CommandLineOptions::Parse(int argc, char** argv)
{
    cmd.parse(argc, argv);
}


