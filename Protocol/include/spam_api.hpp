#include <json/json.h>
#include <map>

namespace spam_api {

// Message Generation
namespace gen {

namespace request {

// Create a message to request a connection
std::string connect(std::string address, std::string port);

// Create a message to request to join
std::string join(std::string username);

// Create a message to post a new message to the board
std::string post(std::string message_id, std::string sender, std::string post_date, std::string subject, std::string content);

// Create a message to request a posted message
std::string message(std::string requested_message_id);

// Create a message to request leaving the board
std::string leave(std::string username);

// Create a message to request the current users
std::string getusers(std::string group_id);

} // namespace request

namespace respond {

// Create a message to respond to a connection request
std::string connect(bool success);

// Create a message to respond to a join request
std::string join(bool success);

// Create a message to respond to a post request
std::string post(bool success);

// Create a message to respond to a message request
std::string message(std::string message_id, std::string sender, std::string post_date, std::string subject, std::string content);

// Create a message to respond to a leave request
std::string leave(bool success);

// Create a message to respond to a getusers request
std::string getusers(std::string users[]);

} // namespace respond
} // namespace gen

namespace parse {

namespace request {

std::map<std::string, std::string> connect(std::string& json_message);
std::map<std::string, std::string> join(std::string& json_message);
std::map<std::string, std::string> post(std::string& json_message);
std::map<std::string, std::string> message(std::string& json_message);
std::map<std::string, std::string> leave(std::string& json_message);
std::map<std::string, std::string> getusers(std::string& json_message);

} // namespace request
} // namespace parse
} // namespace spam_api
