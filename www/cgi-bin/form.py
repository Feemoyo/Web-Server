#!/usr/bin/env python3

import urllib.parse
import os
import json

DATA_FILE = "./www/cgi-bin/data/form.json"

if not os.path.exists(os.path.dirname(DATA_FILE)):
    os.makedirs(os.path.dirname(DATA_FILE))
    with open(DATA_FILE, 'w') as file:
        json.dump([], file)

def read_data():
    if os.path.getsize(DATA_FILE) == 0:
        return []
    with open(DATA_FILE, 'r') as file:
        try:
            data = json.load(file)
            if not isinstance(data, list):
                return []
            return data
        except json.JSONDecodeError:
            return []

def get_data():
    data = read_data()
    if not data:
        return ""
    return json.dumps(data, indent=4)

def post_data(data):
    file_data = read_data()
    file_data.append(data)
    with open(DATA_FILE, 'w') as file:
        json.dump(file_data, file, indent=4)
    return f"Created entry: {data['nome']}, {data['email']}, {data['mensagem']}"

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
            background-image: url('../imgs/banana_server.png');
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
    encoded_data = os.environ.get('DATA')

    decoded_data = urllib.parse.parse_qs(encoded_data)
    decoded_data = {k: v[0].strip() if len(v) == 1 else v for k, v in decoded_data.items()}
    form = json.dumps(decoded_data, indent=4)
    
    method = os.environ.get('METHOD')
    data = json.loads(form) if form else {}
    key_1 = data.get('nome')
    key_2 = data.get('email')

    if method == 'GET':
        message = get_data()
    elif method == 'POST':
        if key_1 and key_2:
            message = post_data(data)
        else:
            message = "404 - Not Found"
    else:
        message = "405 - Method Not Allowed"

    print(create_response(message))

if __name__ == "__main__":
    main()
