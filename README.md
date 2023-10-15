# IRC

HOW TO RUN PROGRAM AND TEST IT:
a) Check Number of Arguments can only receive two, a Port and A password (password bot Yet defined)

b) Build Server object you can pass any port within the limits. I have been using 6667
	i) Calls the function Init which will do the primary set ups of the server

c) StartConection and open sockets, this is made by 4 steps:
	1. Set Server Info => setServerInfo() function to do that
	2. Bind the socket to the server => bindSocketFD() for that
	3. After binding and getting server dada we start connection (StartConection() for that in main)
	4. Listen which will wait for clients to connect to the channel => In order to optimize this we use it combined with Select()

d) Connection is set and now we handle it with HandleConnection(), which will do the following actions in its compossition:
	i) Checks if any user as connected to the server and reports back
	ii ) Checks if any user sent a message with the recev() function
	iii) Keeps user data which we need to understand how to extrapulate that and create clients from There
	iV) Checks if any user disconected from the server

e) To run the program and test it without the UGI its pretty simple, just follow the following steps:
	1. Open two terminals side by side
	2. In the first terminal run the program with 
		i) port: 6667 (or one of your choice as long as its valid) 
		ii) password: not relevant for now just insert any thing you want.
	3. In second terminal run the following: nc 127.0.0.1 6667 
	4. Type a message on terminal 2

	Congratz!! you just did your first IRC connection and trasmited information trough a TPC protocol :D

HOW TO RUN PROGRAM AND TEST IT WITH IRC CLIENT HEXCHAT:
a) make install => it will automatically install HEXCHAT in your machine
b) make config => It will make necessary configurations in your HEXCHAT and set it ready to run
c) make run	=> Will run the program with a pre-defined set of input and a port, it will automatically
	open your HEXCHAT interface. It will prompt you to open HEXCHAT
	I) Do you want to start an instance of Hexchat: "y" 
	II) In HEXCHAT check box: Show Favorites Only
	III) Select ft_irc (should be the only option available)
	IV) Click connect => It will open the Hexchat interface and connect it to your terminal
	V) In terminal again: Do you want to start an instance of Hexchat: "n" (unless you wwant to open many HEXCHAT interfaces, which you can)
	VI) In HEXCHAT input the following command for testing purpuses: /MESSAGE "Your Message"
		
Notes:
1. The terminal will only display messages from HEXCHAT that follow a valid command like so:
	a) /MESSAGE "Your Message"
	b) /JOIN "Your message"
	C) /NICK "Your message"
2. Check the HEXCHAT manual to see which inputs are valid. Subject also refers to some
3. We need to handle the messages by the first input we receive on our server


Hexchat configurations:
1. In order to make HEXCHAT work with our server we need to make some configurations
2. We need to add a new server to HEXCHAT, we do that by:
	a) Open HEXCHAT
	b) Click on the Hexchat icon on the top left corner
	c) Click on Network List
	d) Click on Add
	e) Fill the following fields:
		i) Network: ft_irc
		ii) Server: Add: localhost/6667 (or any other port you want to use)
		iii) Password: (define one)
		iv) Username: (define one)
		v) Real Name: (define one)
		vi) Nick Name: (define one)
		vii) Uncheck all boxes in configurations except for the last one
		viii) Login Method: Challenge Auto (username + password)
		ix) Character Set: UTF-8(Unicode)
