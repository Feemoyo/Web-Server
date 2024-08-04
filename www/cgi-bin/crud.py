import os
import json
import cgi
import cgitb

cgitb.enable()

DATA_FILE = os.environ.get('DATA_BASE')

if not DATA_FILE:
	print("404")
	exit(1)

def read_data():
	if not os.path.exists(DATA_FILE):
		return {}
	with open(DATA_FILE, 'r') as file:
		return json.load(file)

def write_data(data):
	with open(DATA_FILE, 'w') as file:
		json.dump(data, file, indent=4)

def create_entry(key, value):
	data = read_data()
	data[key] = value
	write_data(data)
	return f"Created entry: {key} = {value}"

def read_entry(key):
	data = read_data()
	if key in data:
		return f"Read entry: {key} = {data[key]}"
	else:
		return f"Entry '{key}' not found."

def update_entry(key, value):
	data = read_data()
	if key in data:
		data[key] = value
		write_data(data)
		return f"Updated entry: {key} = {value}"
	else:
		return f"Entry '{key}' not found."

def delete_entry(key):
	data = read_data()
	if key in data:
		del data[key]
		write_data(data)
		return f"Deleted entry: {key}"
	else:
		return f"Entry '{key}' not found."

def generate_html_response(message):
	return f"""
	<!DOCTYPE html>
	<html lang="en">
	<head>
		<meta charset="UTF-8">
		<title>Operation Result</title>
	</head>
	<body>
		<h1>{message}</h1>
	</body>
	</html>
	"""

def main():
	form = cgi.FieldStorage()
	operation = form.getvalue('operation')
	key = form.getvalue('key')
	value = form.getvalue('value')

	if operation == 'create':
		message = create_entry(key, value)
	elif operation == 'read':
		message = read_entry(key)
	elif operation == 'update':
		message = update_entry(key, value)
	elif operation == 'delete':
		message = delete_entry(key)
	else:
		message = "Invalid operation."

	print(generate_html_response(message))

if __name__ == "__main__":
	main()
