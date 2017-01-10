// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "lamportservice.h"
// Protocol includes
#include <thrift/protocol/TBinaryProtocol.h>
// Server includes
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TNonblockingServer.h>
#include <thrift/server/TThreadedServer.h>
// Transport includes
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TTransportUtils.h>
// Concurrency includes
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
// Other includes
#include <vector>
#include <mutex> 
#include <algorithm>

// Namespaces
using namespace ::apache::thrift;
using namespace apache::thrift::concurrency;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;


bool myfunction (TimeStamp i,TimeStamp j) { return (i.time<j.time); }

class lamportserviceHandler : virtual public lamportserviceIf {

	private:
	struct TimeStamp timestamp;
	std::vector<TimeStamp> requestsqueue;

	std::vector<lamportserviceClient> servers;
	std::string name = "";
	bool locktry =false;
 public:
  	lamportserviceHandler(int own_port) {
	timestamp.id = own_port;
	timestamp.time =0;

    	std::cout <<"connection of server "<<own_port<<std::endl;
	name = "Server"+own_port;
	// Your initialization goes here
	//connect to resource server 9091 res1
	for (int i = 9090; i < 9093; i++) {
	std::cout<<i<<std::endl;
	if (own_port != i) {
	std::cout << "connect to server with port: "<<i<<std::endl;

	boost::shared_ptr<TSocket> socket(new TSocket("127.0.0.1", i));
    	boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
    	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	lamportserviceClient res1(protocol);
	servers.push_back(res1);
}
}

  }

  void request(TimeStamp& _return, const TimeStamp& timestampc) {    
	std::cout<<"request"<<std::endl;
	requestsqueue.push_back(timestampc);
	this->browse();
	std::sort(requestsqueue.begin(),requestsqueue.end(),myfunction);
	
	if(this->timestamp.time<timestampc.time)
	{
		this->timestamp.time = timestampc.time +1;
	}
	else{
		this->timestamp.time++;
	}
	_return.time = this->timestamp.time;
	_return.id = timestampc.id;
  }

  void release(const TimeStamp& timestampc) {
    // Your implementation goes here
	requestsqueue.pop_back();
	if(this->timestamp.time<timestampc.time)
	{
		this->timestamp.time = timestampc.time +1;
	}
	else{
		this->timestamp.time++;
	}
    printf("release\n");
  }

  void lock() {
	timestamp.time++;
	requestsqueue.push_back(timestamp);
    for (int i = 0; i < servers.size(); i++) {
	try {
	servers.at(i).getOutputProtocol()->getTransport()->open();
	std::cout << "request server" << std::endl;
	servers.at(i).request(timestamp, timestamp);
	servers.at(i).getOutputProtocol()->getTransport()->close();
	} catch (TException &tx) {
	std::cout << "error calling server: " << tx.what() << std::endl;
	}
	}
	browse();
	printf("lock\n");
	sleep(5);
    	for (int i = 0; i < servers.size(); i++) {
	try {
	servers.at(i).getOutputProtocol()->getTransport()->open();
	std::cout << "release server" << std::endl;
	servers.at(i).release(this->timestamp);
	servers.at(i).getOutputProtocol()->getTransport()->close();
	} catch (TException &tx) {
	std::cout << "error calling server: " << tx.what() << std::endl;
	}
	}
	release(timestamp);
  }

  void unlock() {
    // Your implementation goes here
    printf("unlock\n");
  }
 void browse(){
	for (int i = 0; i < requestsqueue.size(); i++)
		{
		std::cout<<"Server: " <<requestsqueue.at(i).id <<"time: " <<requestsqueue.at(i).time<<std::endl;
		}
 }

};

int main(int argc, char **argv) {
  int port = 9090;
  int numThreads = 10;
  std::cout << "Please enter the port number for this server (4 Nubers)" << std::endl;
  std::cin>>port;
  shared_ptr<lamportserviceHandler> handler(new lamportserviceHandler(port));
  shared_ptr<TProcessor> processor(new lamportserviceProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
  shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(numThreads);
  shared_ptr<ThreadFactory> threadFactory(new PosixThreadFactory()); 

  threadManager->threadFactory(threadFactory);
  threadManager->start(); 

  TServer* server = NULL;
  server =  new TNonblockingServer(processor, protocolFactory, port, threadManager);

  server->serve();
  return 0;
}

