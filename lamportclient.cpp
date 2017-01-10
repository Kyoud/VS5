#include <cstdlib>
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>
#include <sys/time.h>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "gen-cpp/lamportservice.h"// service

using std::ios;
using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace std::chrono;
/*
 * 
 */

int main(int argc, char** argv) {
	string command;
   	boost::shared_ptr<TSocket> socket(new TSocket("127.0.0.1", 9090));
    	boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
    	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    
    	lamportserviceClient client(protocol);
	try {
		// Client loop
		while(1) {
			// Set current cmd
			string cmd;
			cout << "Enter cmd: ";
			cin >> cmd;
			if(cmd == "r")
			{
				// Start connection
				transport->open();
				// Lock
				client.lock();
				// Unlock
				client.unlock();
				// Close connection
				transport->close();
			}
		}

    	} catch (TException &tx) {
        cout << "error calling server: " << tx.what() << endl;
    	}
    	return 0;
}

