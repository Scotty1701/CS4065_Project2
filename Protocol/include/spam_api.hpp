#include <json/json.h>

namespace spam_api {

// Message Generation

// Create a message to request a connection
Json::Value gen_request_connect(std::string address, std::string port);

// Create a message to respond to a connection request
Json::Value gen_respond_connect(bool success);

// Create a message to request to join
Json::Value gen_request_join(std::string username);

// Create a message to respond to a join request
Json::Value gen_respond_join(bool success);

// Create a message to post a new message to the board
Json::Value gen_request_post(std::string message_id, std::string sender, std::string post_date, std::string subject, std::string content);

// Create a message to respond to a post request
Json::Value gen_respond_post(bool success);

// Create a message to request a posted message
Json::Value gen_request_message(std::string requested_message_id);

// Create a message to respond to a message request
Json::Value gen_respond_message(std::string message_id, std::string sender, std::string post_date, std::string subject, std::string content);

// Create a message to request leaving the board
Json::Value gen_request_leave(std::string username);

// Create a message to respond to a leave request
Json::Value gen_respond_leave(bool success);

// Create a message to request the current users
Json::Value gen_request_getusers(std::string group_id);

// Create a message to respond to a getusers request
Json::Value gen_respond_getusers(std::string users[]);

} // namespace spam_api
