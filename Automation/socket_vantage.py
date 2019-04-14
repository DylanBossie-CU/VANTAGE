import socket
import sys
import time

# create TCP sockets 
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sockdata = socket.socket(socket.AF_INET, socket.SOCK_STREAM)


# sets of signals
wokeSig = 'NUC is Woke'
manifestSig = 'Manifest is Sent'
standbySig = 'NUC is Standby'
initiateSig = 'CubeSat is Deploying'
resultsSig = 'Results is Done'


# binding sockets to their portals
server_address = ('169.254.146.34', 4013)
server_data=('169.254.146.34',4014)
print('starting up on %s port %s' % server_address)
print('starting up on %s port %s' % server_data)


sock.bind(server_address)
sockdata.bind(server_data)

# Here is where the Mothership sends the Pi the manifesto



# listen to incoming connections
sock.listen(1)
sockdata.listen(1)



# This is NUC to PI communication
# Pepega

if True:
	# Wait for a connection
	print('waiting for a Pepega' )
	connection, client_address = sock.accept()
	connection_data, client_data = sockdata.accept()
	
	try:
		while True:
			data_ws = connection.recv(32768)
			print('received %s' % data_ws)
			if wokeSig in data_ws:
				print('NUC is awake')
				break
		
		manifest = open('manifest.txt','r')
		print('PepeHands Manifest is opened')
		
		manifest_data = manifest.read()
		connection.send(manifestSig)
		time.sleep(0.2)
		connection_data.sendall(manifest_data)
		print('Pepega Manifesto is sent')
		time.sleep(0.2)
		while True:
			data_sb = connection.recv(32768)
			if standbySig in data_sb:
				print('NUC is ready')
				break
		
		# when someone tells the pi to start
		print('Starting simulated 10 seconds of not starting')
		time.sleep(10)
		print('CUBES are Launching right now')
		connection.send(initiateSig)
		
		while True:
			data_rs = connection.recv(32768)
			if resultsSig in data_rs:
				print('NUC sending results')
				break
		
		# Recieve the data in chunks
		results = open('Results.txt','w')
		resultsC=0
		# C for count to 100 before stopping
		# W for wait for word to showup
		resultsW=0
		while True:
			data = connection_data.recv(32768)
			
			if data:
				results.write(data)
				resultsC=0
				resultsW=1
			else:
				resultsC=resultsC+1
			if resultsC>64:
				if resultsW==1:
					break
				
		# Return results to mothership
		
		# Clean up connections
		connection.close()
		connection_data.close()
		# code to send results back to mothership
		print('Mothership has received the package')

	finally:
		# Clean up the connection
		connection.close()
		connection_data.close()
else:
	print('If you see this, that means true does not mean true')
