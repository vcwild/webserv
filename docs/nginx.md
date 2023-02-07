# Creating Docker NGINX environment

This is a simple example of how to create a Docker environment with NGINX.

## Prerequisites

- [Docker](https://docs.docker.com/install/)

## Running

Fetch the NGINX image from Docker Hub:

```bash
docker pull nginx
```

Set the port to run the container:

```bash
PORT=7000
```

Run the container:

```bash
docker run --name nginx -p $PORT:80 nginx
```

## Testing

Methods

### Using curl

GET request:

```bash
curl localhost:$PORT
```

POST request:

```bash
curl -X POST localhost:$PORT
```

### Using browser

Open the browser and type `localhost:$PORT` in the address bar.

## Stopping

Stop the container:

```bash
docker stop nginx
```

Remove the container:

```bash
docker rm nginx
```

## Updating NGINX configuration to allow POST requests

Edit the `nginx.conf` file:

```bash
docker exec -it nginx bash
```

```bash
vi /etc/nginx/nginx.conf
```

Add the following lines to the `http` section:

```bash
http {
    ...
    server {
        location / {
            if ($request_method = 'POST') {
                add_header Access-Control-Allow-Origin *;
                add_header Access-Control-Allow-Methods 'GET, POST, OPTIONS';
                add_header Access-Control-Allow-Headers 'DNT,User-Agent,X-Requested-With,If-Modified-Since,Cache-Control,Content-Type,Range';
                add_header Access-Control-Expose-Headers 'Content-Length,Content-Range';
            }
        }
    }
    ...
}
```

Restart the container:

```bash
docker restart nginx
```

## References

- [NGINX Docker Hub](https://hub.docker.com/_/nginx)
