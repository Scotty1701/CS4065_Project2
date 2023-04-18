# A list of possible events that a client instance can send a cli/gui instance

- `log`, prints to debug area
- `message`, prints to message area, value must include username, subject, and content
- `getgroups`, prints to group area, adds group to list
- `message`, prints to message area,
- `exit`, kills client safely,
- `post`, post reply,
- `join`, join reply
- `connect`, connect reply
- `leave`, leave reply
