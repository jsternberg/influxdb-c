#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include "influxdb.h"

TEST_CASE( "create and then free a client", "[client]" ) {
  influxdb_client_t *influxdb = influxdb_client_init();
  REQUIRE( influxdb != NULL );
  influxdb_client_cleanup(influxdb);
}

TEST_CASE( "test the ping response", "[client]" ) {
  influxdb_client_t *influxdb = influxdb_client_init();
  int res = influxdb_ping(influxdb);
  REQUIRE( res == 0 );
  influxdb_client_cleanup(influxdb);
}

TEST_CASE( "test the status response", "[client]" ) {
  influxdb_client_t *influxdb = influxdb_client_init();
  influxdb_status_t *status = influxdb_status(influxdb);
  REQUIRE( status != NULL );
  REQUIRE( status->version != NULL );
  influxdb_status_cleanup(status);
  influxdb_client_cleanup(influxdb);
}

int main(int argc, char *argv[]) {
  Catch::Session session;
  int returnCode = session.applyCommandLine(argc, argv);
  if (returnCode != 0)
    return returnCode;

  influxdb_global_init();
  int numFailed = session.run();
  influxdb_global_cleanup();
  return (numFailed < 0xff ? numFailed : 0xff);
}
