#ifndef ATTACKIQ_DNP3_CLIENT_COMMANDLINEOPTIONS_H
#define ATTACKIQ_DNP3_CLIENT_COMMANDLINEOPTIONS_H

#include <tclap/CmdLine.h>

class CommandLineOptions
{

public:
    CommandLineOptions();

    void Parse(int argc, char** argv);

    TCLAP::CmdLine cmd;

    TCLAP::ValueArg<uint16_t> connectPort;
    TCLAP::ValueArg<std::string> connectAddress;
};


#endif //ATTACKIQ_DNP3_CLIENT_COMMANDLINEOPTIONS_H
