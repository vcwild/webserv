#!/usr/bin/python3

def handle_req():
    import cgi
    import cgitb

    cgitb.enable()

    form = cgi.FieldStorage()
    searchterm = form.getvalue('searchbox')
    if len(searchterm) != len(searchterm.encode()):
        print("<H1>Error</H1>")
        print("Please fill in the name and addr fields.")
        return

    print(f"<H1>Hello there {searchterm}!<H1>")

if __name__ == "__main__":
    handle_req()
