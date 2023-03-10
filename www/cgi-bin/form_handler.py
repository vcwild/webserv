#!/usr/bin/python3


def clean_string(s):
    import string
    valid_chars = string.ascii_letters + string.digits + " "
    return "".join(c for c in s if c in valid_chars).strip()

def handle_req():
    import cgi
    import cgitb

    cgitb.enable()

    form = cgi.FieldStorage()
    s = str(form.getvalue('name'))
    if len(s) <= 4:
        print("<H1>Error</H1>")
        print("Please fill in a name with at least 5 characters.")
        return

    print(f"<h1>Hello there {clean_string(s)}!<h1>")

if __name__ == "__main__":
    handle_req()
