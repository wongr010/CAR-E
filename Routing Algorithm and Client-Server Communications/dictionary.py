import json
from pprint import pprint

data=json.load(open('sample.json'))

for p in data["routes"][0]["legs"][0]["steps"]:
	if 'maneuver' not in p:
		continue
	if 'distance' in p:
		pprint(p["distance"]["text"])
	pprint(p["maneuver"])