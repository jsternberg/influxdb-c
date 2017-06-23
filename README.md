# InfluxDB C Library

This is a C library for using InfluxDB. The library is written primarily in C
and the tests are written in C++. Since the library is written primarily in C,
it should not require the C++ standard library when linking.

This library's only dependency is `libcurl`.

## Using the Library

Here is an example of creating a client, pinging the server, and then freeing the client.

```c
#include <stdio.h>
#include <influxdb.h>

int main() {
  // Initialize curl.
  influxdb_global_init();

  // Initialize a new client.
  influxdb_client_t *influxdb = influxdb_client_init();

  // Use the /ping endpoint to find the status and print the version.
  influxdb_status_t *status = influxdb_status(influxdb);
  printf("%s\n", status->version);

  // Cleanup resources.
  influxdb_status_cleanup(status);
  influxdb_client_cleanup(influxdb);
  influxdb_global_cleanup();
  return 0;
}
```

## Compiling the Library

The library uses CMake to generate Makefiles.

```bash
$ cmake .
$ make
$ make test
```

## Running the Tests

At the moment, tests require an instance of InfluxDB to be running locally. In
the future, unit tests will use mock objects and integration tests will be able
to use the Docker images. Neither of these are currently done.

To run the tests, call `make test` after generating the Makefiles with CMake.
