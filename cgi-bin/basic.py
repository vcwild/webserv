#!/usr/bin/python3

import os

def set_params(script_name: str, path_info: str):
    print("Content-type: text/html")
    print()

    print("<html>")
    print("<head>")
    print("<title>CGI Script</title>")
    print("</head>")
    print("<body>")
    print("<h1>CGI Script</h1>")
    print("<p>SCRIPT_NAME: " + script_name + "</p>")
    print("<p>PATH_INFO: " + path_info + "</p>")
    print("</body>")
    print("</html>")

def exec():
    try:
        set_params(os.environ["SCRIPT_NAME"], os.environ["PATH_INFO"])
    except KeyError:
        set_params("basic.py", "no path info")

if __name__ == "__main__":
    exec()
