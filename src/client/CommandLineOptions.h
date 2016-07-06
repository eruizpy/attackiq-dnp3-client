#ifndef ATTACKIQ_DNP3_CLIENT_COMMANDLINEOPTIONS_H
#define ATTACKIQ_DNP3_CLIENT_COMMANDLINEOPTIONS_H

#include <tclap/CmdLine.h>

class CommandLineOptions
{

public:
    CommandLineOptions();

    void Parse(int argc, char** argv);

private:
    TCLAP::CmdLine cmd;

public:
    TCLAP::ValueArg<uint16_t> hostPort;
    TCLAP::ValueArg<std::string> hostAddress;

    TCLAP::ValueArg<uint16_t> masterAddress;
    TCLAP::ValueArg<uint16_t> outstationAddress;
};


#endif //ATTACKIQ_DNP3_CLIENT_COMMANDLINEOPTIONS_H

