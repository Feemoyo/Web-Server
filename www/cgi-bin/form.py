#!/usr/bin/env python3

import os
import json

DATA_FILE = "./www/cgi-bin/data/form.json"

if not os.path.exists(DATA_FILE):
	with open(DATA_FILE, 'w') as file:
		json.dump({}, file)
	return

def	get_data(void):
	with open(DATA_FILE, 'r') as file:
		data = json.load(file)
		if not data:
			return ("204")
		return json.dump(data, indent=4)

def	delete_data(key_1, key_2):
	with open(DATA_FILE, 'r+') as file:
		file_data = json.load(file)
		item_to_delete = None
	for item in data:
		if item.get('nome') == key_1 and item.get('email') == key_2:
			item_to_delete = item
			break
	if item_to_delete:
		data.remove(item_to_delete)
		file.seek(0)
		file.truncate()
		json.dump(data, file, indent=4)
		return (f"Deleted entry: {key_1}, {key_2}")
	else
		return (f"Entry: {key_1}, {key_2} not found.")

def	post_data(data):
	with open(DATA_FILE, 'r+') as file:
		file_data = json.load(file)
		file_data.append(data)
		file.seek(0)
		json.dump(file_data. file, indent=4)

def	main(void):
	form = os.environ.get('DATA')
	method = os.environ.get('METHOD')
	data = json.loads(form)
	key_1 = data['nome']
	key_2 = data['email']

	if method == 'get'
		message = get_data()
	elif method == 'delete'
		message = delete_data(key_1, key_2)
	elif method == 'post'
		message = post_data(data)
	else
		message = "405"

if __name__ == "__main__":
	main()