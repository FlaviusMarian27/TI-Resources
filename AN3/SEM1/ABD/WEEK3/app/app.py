from flask import Flask
import mysql.connector

app = Flask(__name__)

@app.route('/')
def hello_world():
    return 'Hello, World! This is connected to MySQL.'

if __name__ == '__main__':
    app.run(host='0.0.0.0')