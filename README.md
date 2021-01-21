Name: Abel Aguilar
What I did in this assignment:
	In this assignment we created 4 files, server A, server B, servermain and client.
	each the server A and B files read in data1 and data2 files and create a social map index. 
	server A and B also have the algorithm to find the recommendation.
	Server A and B send the country list to servermain
	Client file sends user and country to servermain
	Servermain check if the country is in server A or B, if it isnt it sends country not found back to client
	if it is it sends user and country to server A or B and then server A or B sends the recommendation/User not found to servermain
	servmain sends those results to client
	client-servermain uses tcp
	serverA/B-sermain uses udp

Code files
Server A: has recommendation algorithm, takes in data1 file creates social map. sends the country list to servermain with udp. It later puts inputs from the servermain into the algorithm and outputs a recommendation/user not in country.

Server B: has recommendation algorithm, takes in data2 file creates social map. sends the country list to servermain with udp. It later puts inputs from the servermain into the algorithm and outputs a recommendation/user not in country.

Client: Sends inputs from user to servermain using TCP. it later recieves the a recommendation/user not found/country not found and prints it for the user to see.

Servermain: Takes in country list form server A and B and creates a country list using UDP. it then takes input form client using tcp and sends that to server A or B using udp. server A or B send back results and servermain forwards it back to client 

client output:
	The client is up and running
	Please enter the User ID: 
	51 
	Please enter Country Name: 
	UKs
	The client has sent User 51 and UKs to Main Server using TCP
	The client has received results from Main Server: User None is/are possible friend(s)of User 51 in UKs
	Please enter the User ID: 
	27
	Please enter Country Name: 
	SI
	The client has sent User 27 and SI to Main Server using TCP
	The client has received results from Main Server: User None is/are possible friend(s)of User 27 in SI
	Please enter the User ID: 

Servermain output:
	The Main server is up and running using UDP on port 32412.
	The Main server has received the country list from server A using UDP over port 30412
	The Main server has received the country list from server B using UDP over port 31412
	Server A       |Server B
	UKs            | SI
	The Main server has received the request on User 51 in UKs from the client using TCP over port 33412
	UKs shows up in server A
	The Main Server has sent request from User 51 to server A using UDP over port 30412
	The Main server has received searching result(s) of User None from server A
	The Main Server has sent searching result(s) to client using TCP over port 33412
	The Main server has received the request on User 27 in SI from the client using TCP over port 33412
	SI shows up in server B
	The Main Server has sent request from User 27 to server B using UDP over port 31412
	The Main server has received searching result(s) of User None from server B
	The Main Server has sent searching result(s) to client using TCP over port 33412

Server A output:
	The Server A is up and running using UDP on port 30412.
	The server A has sent a country list to Main Server
	The server A has received request for finding possible friends of User 51 in UKs
	The server A is searching possible friends for User 51 ...
	Here are the results: User None...
	The server A has sent the result(s) to Main Server

Server B output:
	The Server B is up and running using UDP on port 31412.
	The server B has sent a country list to Main Server
	The server B has received request for finding possible friends of User 27 in SI
	The server B is searching possible friends for User 27 ...
	Here are the results: User None...
	The server B has sent the result(s) to Main Server

If the country name/User ID is too large it fails

took code form geeks for geeks to make tcp and udp files
links:
https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/?ref=lbp
https://www.geeksforgeeks.org/udp-server-client-implementation-c/?ref=lbp