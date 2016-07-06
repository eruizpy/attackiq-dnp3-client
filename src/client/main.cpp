#include <asiodnp3/DNP3Manager.h>
#include <asiodnp3/PrintingSOEHandler.h>
#include <asiodnp3/ConsoleLogger.h>
#include <asiodnp3/DefaultMasterApplication.h>
#include <asiodnp3/PrintingCommandCallback.h>

#include <opendnp3/LogLevels.h>

#include "SyncVar.h"

using namespace std;
using namespace openpal;
using namespace asiopal;
using namespace asiodnp3;
using namespace opendnp3;

MasterStackConfig GetMasterConfig();

int main(int argc, char* argv[])
{

	// Specify what log levels to use. NORMAL is warning and above
	// You can add all the comms logging by uncommenting below
	const uint32_t FILTERS = levels::NORMAL | levels::ALL_APP_COMMS;

    // we'll use this synchronized variable to monitor the state of the channel
    SyncVar<ChannelState> channelstate(ChannelState::CLOSED);

	// This is the main point of interaction with the stack
	DNP3Manager manager(1, ConsoleLogger::Create());

	// Connect via a TCPClient socket to a outstation
	auto pChannel = manager.AddTCPClient("tcpclient", FILTERS, ChannelRetry::Default(), "127.0.0.1", "0.0.0.0", 20000);

	// update the synchronized variable whenever the channel state changes
	pChannel->AddStateListener([&channelstate](ChannelState state)
	{
        channelstate.Set(state);
	});

	// Create a new master on a previously declared port, with a
	// name, log level, command acceptor, and config info. This
	// returns a thread-safe interface used for sending commands.
	auto master = pChannel->AddMaster(
	                  "master",										    // id for logging
	                  PrintingSOEHandler::Instance(),					// callback for data processing
	                  DefaultMasterApplication::Instance(),             // master application instance
	                  GetMasterConfig()									// stack configuration
	);

	// Enable the master. This will start communications.
	master->Enable();

    // Wait for a connection for a period of time
    if(!channelstate.WaitFor(ChannelState::OPEN, std::chrono::seconds(5)))
    {
        std::cerr << "Unable to connect within timeout" << std::endl;
        return 1;
    }

	//ControlRelayOutputBlock crob(ControlCode::LATCH_ON);
	//master->SelectAndOperate(crob, 0, PrintingCommandCallback::Get());

	return 0;
}

MasterStackConfig GetMasterConfig()
{
    MasterStackConfig config;

    config.master.disableUnsolOnStartup = false;

    // You can override the default link layer settings here
    // in this example we've changed the default link layer addressing
    config.link.LocalAddr = 1;
    config.link.RemoteAddr = 10;

    return config;

}
