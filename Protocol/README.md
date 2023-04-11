## Overview
The protocol simply consists of a JSON formatted message with three fields: the type of message, whether it is a request or a response, and a payload for that message. The application layer is responsible for performing the proper actions for each message type.

## Format
```
{
    "message_type": "keyword",
    "is_request": true,
    "payload": "whatever is needed"
}

```

## Message Types
### Connect
Used to connect to a server. Request payload contains the address and port of the server, formatted as a string `{"address", "port"}`. Response payload is a bool indicating whether the connection was accepted.
### Join
Used to join the message board. Request payload contains the desired username as a string `"username"`. Response payload is a bool indicating whether the user was added to the board.
### Post
Used to post a new message to the board. Request payload contains the following information formatted as follows a string array `{"message_id", "sender", "post_date", "subject", "content"}`. Response payload is a bool indicating whether the message was posted to the board.
### Message
Used to get the content of a message from the server. Request payload contains the message ID of the desired message as a number `123`. Response payload is a string matching the payload format for the post request, `{"message_id", "sender", "post_date", "subject", "content"}`.
### Leave
Used to leave the group. Request payload contains the username of the user leaving as a string `"username"`. Response payload is a bool indicating whether the user successfully left the group.
### GetUsers
Used to get a list of users currently in the group. Request payload contains the group ID as a number `345`. Response payload is a string array of the usernames in the group `{"user1", "user2", "user3"}`. 

## Building
This project requires cmake to build.
Starting in the root directory:
1. `mkdir build`
1. `cd build`
1. `cmake ..`
1. `cmake --build . --config Release`
1. `cmake --install . --prefix ../install`

To build the python bindings:
1. `cd python`
1. `mkdir build`
1. `cd build`
1. `cmake ..`
1. `cmake --build . --config Release`
1. `cmake --install . --prefix ../install`
