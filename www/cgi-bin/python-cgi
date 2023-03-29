#!/usr/bin/python3

import os

def set_params(script_name: str, path_info: str):
    print(f"""\
<html>
<head>
<title>CGI Script</title>
</head>
<body>
<h1>CGI Script</h1>
<p>SCRIPT_NAME: " + {script_name} + "</p>
<p>PATH_INFO: " + {path_info} + "</p>
</body>
</html>""")

def exec():
    try:
        set_params(os.environ["SCRIPT_NAME"], os.environ["PATH_INFO"])
    except KeyError:
        set_params("basic.py", "no path info")

if __name__ == "__main__":
    exec()
