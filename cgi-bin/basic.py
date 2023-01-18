#!/usr/bin/env python

import datetime

print("Content-type: text/html")
print()
print("<html>")
print("<head>")
print("<title>Python CGI Script</title>")
print("</head>")
print("<body>")
print("<h1>Current Date and Time</h1>")
print("<p>", datetime.datetime.now(), "</p>")
print("</body>")
print("</html>")

