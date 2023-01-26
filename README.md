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

## Brief

## How to use

- Compile the project with `make`
- Run the server with `./bin/webserv [config_file]`
- Execute a curl request to it with `curl -v localhost:8000`

### Examples

- `curl -v localhost:8000`
- `curl -d '{"key1":"value1", "key2":"value2"}' -H "Content-Type: application/json" -X POST localhost:8000`
- `curl -d "param1=value1&param2=value2" -X DELETE localhost:8000`

## References

- [How to server-client sockets in C++](https://www.bogotobogo.com/cplusplus/sockets_server_client.php)

## License

This project is licensed under the Affero General Public License v3.0 - see the [LICENSE](LICENSE) file for details.
