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

#include "gen-cpp/fileservice.h"// service

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
    	int n1 = 2, n2 = 3;
   	boost::shared_ptr<TSocket> socket(new TSocket("127.0.0.1", 9090));
    	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    
    	fileserviceClient client(protocol);
	try {
		while(1) {
			cout << "ENTER download OR upload: ";
			cin >> command;

			if(command == "download"){
				// Initial Download Variables
				FileChunk resultStruct;
				resultStruct.newOffset = 0;
				resultStruct.totalFileSize = 0;
				int mytotalFileSize = 0;
				int myOffset = 0;
				string fileName;
				// Enter Filename
				cout << "Enter filename: ";
				cin >> fileName;
				// Open Connection
				
				
				// Start first download
				transport->open();
				client.download(resultStruct, fileName, 0);
				transport->close();
				cout << "Size of requested file: " << resultStruct.totalFileSize << endl;
				
				// Add first download to localdata
				std::vector<char> localdata;
				localdata.insert(std::end(localdata), std::begin(resultStruct.data), std::end(resultStruct.data));
				mytotalFileSize = resultStruct.totalFileSize;
				myOffset += resultStruct.newOffset;

				cout << "\nFirst DL Local Data Size: " << localdata.size() << " bytes" << endl;
				// Download till localdata's size is >= totalFileSize
				while(mytotalFileSize > localdata.size()) {
					try {
						cout << "\nSIZE: " << localdata.size() << endl;
						// Start nth download
						transport->open();
						client.download(resultStruct, fileName, myOffset);
						transport->close();
						// Add downloaded data to localdata
						localdata.insert(std::end(localdata), std::begin(resultStruct.data), std::end(resultStruct.data));
						myOffset += resultStruct.newOffset;
						//cout << "\nOFFSET: " << resultStruct.newOffset << endl;
					} catch (...) {
						cout << "Failed to continue the download process." << endl;
    					}
					sleep(1);
				}

				cout << "Total Local Data: " << localdata.size() << " bytes" << endl;

				// Write localdata to file
			        ofstream output_file("../local_files/"+fileName);
			        ostream_iterator<char> output_iterator(output_file);
			        copy(localdata.begin(), localdata.end(), output_iterator);				
			} else if(command == "upload") {
				// Initial Download Variables
				FileChunk chunk;
				int fileSize = 0;
				int sendSize = 0;
				int offset = 0;
				string fileName = "";
				// Enter Filename
				cout << "Enter filename: ";
				cin >> fileName;
				// Check if file exists locally
				if (std::ifstream("../local_files/" + fileName))
				{
					// Open the file
					std::ifstream file;
					file.open("../local_files/" + fileName);
					//if(!file.is_open())
					//return;

					// Get the length of the file
					file.seekg(0, ios::end);
					size_t fileSize = file.tellg();

					// Set cursor to current offset
					file.seekg(0, ios::beg);

					// Create a vector to hold all the bytes in this file
					std::vector<char> data(fileSize, 0);

					// Read and copy file to data vector
					file.read(&data[0], fileSize);

					// Chunk Vector
					std::vector<char> chunkData;

					// Call upload till
					while(sendSize < fileSize){
						// Chunk Vector
						std::vector<char> chunkData;
						cout << "DATA SIZE " << data.size() << endl;
						// Create chunk
						for(int i=sendSize; i<data.size(); i++){
							if(offset+100000 > data.size() && i==data.size()) {
								// Rest of file or file too small for chunk
								cout << "rest or too small" << endl;
								chunk.data.insert(end(chunk.data), begin(chunkData), end(chunkData));
								break;
							} else if(i%100000==0 && i!=0) {
								// Chunk of size 100.000 Bytes
								cout << "Chunk complete" << endl;
								chunk.data.insert(end(chunk.data), begin(chunkData), end(chunkData));
								break;
							}
							chunkData.push_back(data.at(i));
						}

						// Attempt to send chunk
						transport->open();
						if(client.upload(fileName, offset, fileSize, chunk)){
							cout << "Upload returned true, continue." << endl;
						} else {
							cout << "Upload returned false, stop" << endl;
							break;
						}
						transport->close();

						// Increase sendSize by chunksize
						sendSize += chunkData.size();
						offset += chunkData.size();
						cout << "Send Size: " << sendSize << endl;
						// Wait to allow other connections
						sleep(1);						
					}
				} else {// if
					std::cout << "Input file does not exist" << std::endl;
				}
			} else {
				break;
			}
		}
        // Output response
	cout << "Connection closed" << endl;

    	} catch (TException &tx) {
        cout << "error calling server: " << tx.what() << endl;
    	}
    	return 0;
}

