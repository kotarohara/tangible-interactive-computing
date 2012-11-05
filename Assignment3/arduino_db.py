import serial
from db.SQLite import SQLite
from datetime import datetime
from numpy import median
# from tempodb import Client, DataPoint

SERIAL_PORT = "/dev/tty.usbmodem1411"
BAUD_RATE = 115200
LIST_SIZE = 10



# Set up a serial network with Arduino
ser = serial.Serial(SERIAL_PORT, BAUD_RATE)

# TempoDB client
# client = Client('0a6583452be342c59f0ffac08f7c9063', 'eb8368811e7e4bd3b89084fbc06f50db')

# Keep reading a sensor data
def loop():
	i = 0
	fsr_values = [0] * LIST_SIZE
	rfid_values = []
	while 1:
		fsr_value = -1
		rfid_value = "0"
		data = ser.readline().strip().split(",")
		
		# Check if it is properly reading data from Arduino
		# Split a FSR value and a RFID value
		if len(data) == 2:
			fsr = data[0].split(":")
			rfid = data[1].split(":")
			
			if len(fsr) == 2:
				fsr_value = int(fsr[1])
				
			if len(rfid) == 2:
				rfid_value = rfid[1]
	
		# print "fsr:", fsr_value, "rfid:", rfid_value
		
		if fsr_value != -1:
			fsr_values[i] = fsr_value
		else:
			fsr_values[i] = 0
			
		if rfid_value != "0":
			rfid_values.append(rfid_value)

		# Submit rfid values every time you get LIST_SIZE items
		if i == LIST_SIZE - 1:
			date = datetime.now()
			submit_fsr_tempodb(date, int(median(fsr_values)))
			submit_rfid_tempodb(date, rfid_values)
			rfid_values = []
					
		i += 1
		i %= LIST_SIZE


# param: an integer median value of fsr_values
# This function sends a median value of fsr_values to
# TempoDB
def submit_fsr_tempodb(date, fsr_median):
	#print date
	#print fsr_median
	db = SQLite('db/coffee.db')
	db.insert_coffee(date, fsr_median)
	#data = [DataPoint(date, fsr_median)]
	#client.write_key('coffee', data)
	return


# param: a list of rfid values
# This function sends all rfid values read within
# a cycle in the loop
def submit_rfid_tempodb(date, rfid_values):	
	# print date
	# print rfid_values
	
	# submit all the rfid values read
	#for rfid_value in rfid_values:
	consumers = []
	for consumer in rfid_values:
		if not consumer in consumers:
			consumers.append(consumer)
	
	db = SQLite('db/coffee.db')
	for consumer in consumers:
		db.insert_coffee_consumer(date, consumer)
		#db.insert_coffee_consumer(date, "ME")
	return	


if __name__=="__main__":
	loop()