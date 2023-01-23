#!/usr/bin/python3

import os

print("Content-type: text/html")
print()

print("<html>")
print("<head>")
print("<title>CGI Script</title>")
print("</head>")
print("<body>")
print("<h1>CGI Script</h1>")
print("<p>SCRIPT_NAME: " + os.environ["SCRIPT_NAME"] + "</p>")
print("<p>PATH_INFO: " + os.environ["PATH_INFO"] + "</p>")
print("</body>")
print("</html>")