#include "Network.h"
#include "iostream"
#include "fstream"
#include "string"
#include <sstream>
Network::Network() {

}

void Network::process_commands(vector<Client> &clients, vector<string> &commands, int message_limit,
                      const string &sender_port, const string &receiver_port) {
    // TODO: Execute the commands given as a vector of strings while utilizing the remaining arguments.

    for (const string &command : commands) {
        if (!command.find("MESSAGE")) {

            std::vector<std::string> args;
            std::string word;
            std::string message;
            std::stringstream ss(command);

            size_t messageStartPos = command.find('#');
            size_t messageEndPos = command.rfind('#');

            message = command.substr(messageStartPos + 1, messageEndPos - 1);
            while (ss >> word && word.find("#")) // reading our ss and adding it to our vector
            {
                args.push_back(word);
            }

            ApplicationLayerPacket app_packet(3, args[1], args[2], message);
              
        }
    }
    /* Don't use any static variables, assume this method will be called over and over during testing.
     Don't forget to update the necessary member variables after processing each command. For example,
     after the MESSAGE command, the outgoing queue of the sender must have the expected frames ready to send. */
}

vector<Client> Network::read_clients(const string &filename) {
    vector<Client> clients;
    // TODO: Read clients from the given input file and return a vector of Client instances.

    fstream clientFile; // read from file
    clientFile.open(filename,ios::in); // open input file

    if (clientFile.is_open())
    {
        std::string line; // read line
        std::string word; // read word
        std::getline(clientFile, line); // skip first line ?

        while (getline(clientFile,line))
        {
            std::vector<std::string> words; // vector of words

            std::stringstream ss(line); 


            while (ss >> word) // reading our ss and adding it to our vector
            {
                words.push_back(word);
            }

            clients.emplace_back(words[0], words[1], words[2]); // Creating our clients and adding them to our vector
        }

    }

    return clients;
}

void Network::read_routing_tables(vector<Client> &clients, const string &filename) {
    // TODO: Read the routing tables from the given input file and populate the clients' routing_table member variable.

    fstream routingFile;                 // read from file
    routingFile.open(filename, ios::in); // open input file

    if (routingFile.is_open()) 
    {
        std::string line; // read line
        std::string client; // add client
        int count = 0; // number of current client;

        while (getline(routingFile, line))
        {
            if (line != "-") {

                std::vector<std::string> routes;
                std::stringstream ss(line);

                while (ss >> client) // reading our ss and adding it to our vector
                {
                    routes.push_back(client);
                }

                clients[count].routing_table.insert(std::make_pair(routes[0], routes[1]));
            }
            else {

                count++;
            }

        }
    }
}

// Returns a list of token lists for each command
vector<string> Network::read_commands(const string &filename) {
    vector<string> commands;
    // TODO: Read commands from the given input file and return them as a vector of strings.

    fstream commandFile;                 // read from file
    commandFile.open(filename, ios::in); // open input file

    if (commandFile.is_open())
    {


        std::string line;

        std::getline(commandFile, line); // skip first line ?

        while (getline(commandFile, line))
        {
            commands.push_back(line);
        }
        
    }
    return commands;
}

Network::~Network() {
    // TODO: Free any dynamically allocated memory if necessary.
}
