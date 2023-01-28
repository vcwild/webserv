<div align="center">
  <img src=".github/assets/image.svg" width="230px" />
</div>
<br/>
<div align="center">
  <img src="https://img.shields.io/github/languages/count/vcwild/webserv?color=%234466fb&style=flat-square" alt="languages" />
  <img src="https://img.shields.io/github/license/vcwild/webserv?color=%234466fb&style=flat-square" alt="license" />
  <img src="https://img.shields.io/github/repo-size/vcwild/webserv?color=%234466fb&style=flat-square" alt="repo size" />
  <img src="https://img.shields.io/github/actions/workflow/status/vcwild/webserv/build.yml?branch=main&style=flat-square&color=%234466fb" alt="build" />
</div>

# Webserv

This project is about writing your ow HTTP server.

You will be able to test it with an actual browser.

HTTP is one of the most used protocols on the internet.

Knowing its arcane will be useful, even if you won’t be working on a website

## Brief

Webserv is a project at 42 which requires us to build a HTTP server using I/O Multiplexing built on top of a event loop.

We start the server by reading a configuration file based on nginx conf, containing information like hostname(ip/port), servername, index file, directory listing, URL redirect, location and so on... The server is then able to handle connections, and serve static files, CGI scripts, and handle HTTP methods like GET, POST and DELETE.

## How to use

- Compile the project with `make`
- Run the server with `./bin/webserv [config_file]`
- Execute a curl request to it with `curl -v localhost:8000`

### Examples

- `curl -v localhost:8000`
- `curl -d '{"key1":"value1", "key2":"value2"}' -H "Content-Type: application/json" -X POST localhost:8000`
- `curl -d "param1=value1&param2=value2" -X DELETE localhost:8000`

## References

- [Web Server Concepts and Examples](https://www.youtube.com/watch?v=9J1nJOivdyw)
- [What is a Server? (Deepdive)](https://www.youtube.com/watch?v=VXmvM2QtuMU)
- [How to server-client sockets in C++](https://www.bogotobogo.com/cplusplus/sockets_server_client.php)
- [Sockets - Server & Client - 2020](https://www.bogotobogo.com/cplusplus/sockets_server_client.php)
- [Building a simple server with C++](https://ncona.com/2019/04/building-a-simple-server-with-cpp/)

## License

This project is licensed under the Affero General Public License v3.0 - see the [LICENSE](LICENSE) file for details.
