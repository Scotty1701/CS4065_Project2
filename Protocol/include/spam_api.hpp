#ifndef SPAM_API_HPP
#define SPAM_API_HPP

#include <json/json.h>
#include <vector>
#include <map>
#include <variant>

typedef std::map<std::string, std::variant<std::string, std::vector<std::string>>> parsedMessage;

namespace spam_api {

// Message Generation
namespace gen {

namespace request {

// Create a message to request a connection
std::string connect(std::string address, std::string port);

// Create a message to request to join
std::string join(std::string username);

// Create a message to post a new message to the board
std::string post(std::string sender, std::string post_date, std::string subject, std::string content);

// Create a message to request a posted message
std::string message(std::string requested_message_id);

// Create a message to request leaving the board
std::string leave(std::string username);

// Create a message to request the current users
std::string getusers(std::string group_id);

} // namespace request

namespace respond {

// Create a message to respond to a connection request
std::string connect(bool success, std::string message);

// Create a message to respond to a join request
std::string join(bool success, std::string message);

// Create a message to respond to a post request
std::string post(bool success, std::string message);

// Create a message to respond to a message request
std::string message(std::string message_id, std::string sender, std::string post_date, std::string subject, std::string content);
std::string message(bool success, std::string message);

// Create a message to respond to a leave request
std::string leave(bool success, std::string message);

// Create a message to respond to a getusers request
std::string getusers(std::vector<std::string> users);
std::string getusers(bool success, std::string message);

} // namespace respond
} // namespace gen

parsedMessage parse(std::string& json_message);

} // namespace spam_api

#endif
