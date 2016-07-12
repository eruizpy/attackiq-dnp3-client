#include <asiodnp3/DNP3Manager.h>
#include <asiodnp3/PrintingSOEHandler.h>
#include <asiodnp3/ConsoleLogger.h>
#include <asiodnp3/DefaultMasterApplication.h>

#include <opendnp3/LogLevels.h>

#include <future>

#include "SyncVar.h"
#include "CommandLineOptions.h"

using namespace std;
using namespace openpal;
using namespace asiopal;
using namespace asiodnp3;
using namespace opendnp3;

MasterStackConfig GetMasterConfig(uint16_t masterAddr, uint16_t outstationAddr);

int main(int argc, char *argv[]) {

    CommandLineOptions options;
    options.Parse(argc, argv);


    // Specify what log levels to use. NORMAL is warning and above
    // You can add all the comms logging by uncommenting below
    const uint32_t FILTERS = levels::NORMAL | levels::ALL_APP_COMMS;

    // we'll use this synchronized variable to monitor the state of the channel
    SyncVar<ChannelState> channelstate(ChannelState::CLOSED);

    // This is the main point of interaction with the stack
    DNP3Manager manager(1, ConsoleLogger::Create());

    // Connect via a TCPClient socket to a outstation
    auto pChannel = manager.AddTCPClient("tcpclient", FILTERS, ChannelRetry::Default(), options.hostAddress.getValue(), "0.0.0.0", options.hostPort.getValue());

    // update the synchronized variable whenever the channel state changes
    pChannel->AddStateListener([&channelstate](ChannelState state) {
        channelstate.Set(state);
    });

    // Create a new master on a previously declared port, with a
    // name, log level, command acceptor, and config info. This
    // returns a thread-safe interface used for sending commands.
    auto master = pChannel->AddMaster(
            "master",                                         // id for logging
            PrintingSOEHandler::Instance(),                   // callback for data processing
            DefaultMasterApplication::Instance(),             // master application instance
            GetMasterConfig(                                  // stack configuration
                    options.masterAddress.getValue(),
                    options.outstationAddress.getValue()
            )
    );

    // Enable the master. This will start communications.
    master->Enable();

    // Wait for a connection for a period of time
    if (!channelstate.WaitFor(ChannelState::OPEN, std::chrono::seconds(5))) {
        std::cerr << "Unable to connect within timeout" << std::endl;
        return 1;
    }

    std::promise<CommandPointResult> p;
    auto callback = [&p](const ICommandTaskResult & results) -> void
    {
        CommandPointResult result(0,0, CommandPointState::INIT, CommandStatus::UNDEFINED);
        results.ReadOnlyValue(result);
        p.set_value(result);
    };

    master->SelectAndOperate(ControlRelayOutputBlock(ControlCode::LATCH_ON), 0, callback);

    // wait for the operation to complete
    auto result = p.get_future().get();

    if(result.state != CommandPointState::SUCCESS)
    {
        std::cerr << "Command operation unsuccessful: " << CommandPointStateToString(result.state) << std::endl;
        return 2;
    }

    if(result.status != CommandStatus::SUCCESS)
    {
        std::cerr << "Outstation reported failure: " << CommandStatusToString(result.status) << std::endl;
        return 3;
    }

    std::cout << "Command operation completed successfully. Exiting progam" << std::endl;

    return 0;
}

MasterStackConfig GetMasterConfig(uint16_t masterAddr, uint16_t outstationAddr) {

    MasterStackConfig config;

    config.master.ignoreRestartIIN = true;
    config.master.startupIntegrityClassMask = ClassField::None();
    config.master.unsolClassMask = ClassField::None();
    config.master.disableUnsolOnStartup = false;

    // You can override the default link layer settings here
    // in this example we've changed the default link layer addressing
    config.link.LocalAddr = masterAddr;
    config.link.RemoteAddr = outstationAddr;

    return config;

}
