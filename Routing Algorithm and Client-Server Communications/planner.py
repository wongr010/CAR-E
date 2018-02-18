import googlemaps
import json
from datetime import datetime
from pprint import pprint

import bluetooth

server_sock=bluetooth.BluetoothSocket( bluetooth.RFCOMM )

port = 1
server_sock.bind(("",port))
server_sock.listen(1)

client_sock,address = server_sock.accept()
print "Accepted connection from ",address

destination = client_sock.recv(1024)
print "received [%s]" % data

client_sock.close()
server_sock.close()

source=raw_input("Enter source address: ")
#destination=raw_input("Enter destination address: ")


gmaps = googlemaps.Client(key='AIzaSyBxDlttyi4kDWNiCuq-5ZB7Rz0v4Qfn0cI')

# Geocoding an address
geocode_result = gmaps.geocode('1600 Amphitheatre Parkway, Mountain View, CA')

# Look up an address with reverse geocoding
reverse_geocode_result = gmaps.reverse_geocode((40.714224, -73.961452))

# Request directions via public transit
now = datetime.now()
directions_result = gmaps.directions(source,
                                     destination,
                                     mode="driving",
                                     departure_time=now)




#pprint(directions_result)

with open('sample.json', 'w') as f:
    json.dump(directions_result, f)

#clear old input

y = open('dank.txt', 'w')
y.write('')
y.close()

data=json.load(open('sample.json'))

for p in data[0]["legs"][0]["steps"]:
	if 'maneuver' not in p:
		continue
	if 'distance' in p:	
	    f = open('dank.txt', 'a')
            f.write(p["distance"]["text"]+"\n")
           # pprint(p["distance"]["text"])
            f.close()
	# pprint(p["maneuver"])
        f = open('dank.txt','a')
        f.write(p["maneuver"] + "\n")
        f.close()

f = open('dank.txt','a')
f.write("end" + "\n")
f.close()



