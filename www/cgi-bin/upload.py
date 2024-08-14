import os
import re
import sys

UPLOAD_DIR = './www/cgi-bin/uploads/'

def save_file(filename, file_content):
	filepath = os.path.join(UPLOAD_DIR, filename)
	
	os.makedirs(UPLOAD_DIR, exist_ok=True)
	
	with open(filepath, 'wb') as f:
		f.write(file_content)
	
	return filepath

def parse_multipart_form_data(header, body):
	first_line = header.find('\n')
	boundary = re.search(r'--([^-]*)', header[0:first_line])
	print(boundary)
	if not boundary:
		raise ValueError("Boundary not found.")
	boundary = boundary[0].encode()

	parts = body.split(b'--' + boundary)
	
	parts = parts[1:-1]
	
	file_data = {}
	
	for part in parts:
		if b'Content-Disposition' not in part:
			continue
		
		header_end = part.index(b'\r\n\r\n') + 4
		headers = part[:header_end].decode()
		body = part[header_end:]
		
		match = re.search(r'filename="([^"]+)"', headers)
		if match:
			filename = match.group(1)
			file_data[filename] = body.rstrip(b'\r\n--')
	
	return file_data

def create_response(message):
    return f"""
    <!DOCTYPE html>
    <html>
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Operation Result</title>
    </head>
    <style>
        body {{
            font-family: 'Raleway', sans-serif;
            background-image: url('../imgs/200_OK.png');
            background-size: cover;
            background-position: center;
            background-repeat: no-repeat;
            height: 100vh;
        }}
    </style>
    <body>
        <h1>{message}</h1>
    </body>
    </html>
    """

def main():
	header = os.environ.get('DATA')
	header = header[0:header.find('\r\n\r\n')]

	try:
		body = os.environ.get('DATA').encode()
	except (UnicodeEncodeError, KeyError):
		print("415")
		return
	
	if not body:
		print("404")
		return
	
	file_data = parse_multipart_form_data(header, body)
	
	for filename, content in file_data.items():
		filepath = save_file(filename, content)
		msg = (f"File saved at: {filepath}")

	print(create_response(msg))

if __name__ == "__main__":
	main()