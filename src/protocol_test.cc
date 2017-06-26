#include "catch.hpp"

#include "influxdb.h"

TEST_CASE( "check the content type - line protocol v1", "[line-protocol]" ) {
  influxdb_protocol_t *protocol = influxdb_protocol_v1();
  REQUIRE( protocol != NULL );

  const char *content_type = influxdb_protocol_content_type(protocol);
  REQUIRE( content_type == "application/x-influxdb-line-protocol-v1" );
  influxdb_protocol_cleanup(protocol);
}

static int write_to_stream(void *data, const char *buf, size_t len) {
  FILE *stream = (FILE *) data;
  fwrite(buf, 1, len, stream);
  fflush(stream);
  return 0;
}

TEST_CASE( "write a point - line protocol v1", "[line-protocol]" ) {
  influxdb_protocol_t *protocol = influxdb_protocol_v1();
  REQUIRE( protocol != NULL );

  influxdb_writer_t writer;
  writer.data = stdout;
  writer.write = write_to_stream;

  influxdb_point_t point;
  memset(&point, 0, sizeof(influxdb_point_t));
  point.name = "cpu";

  influxdb_field_t fields[1];
  fields[0] = influxdb_float("value", 2.5);
  point.fields = influxdb_fields(fields, 1);

  influxdb_protocol_encode(protocol, &writer, &point);
  influxdb_protocol_cleanup(protocol);
}

TEST_CASE( "write a point with tags - line protocol v1", "[line-protocol]" ) {
  influxdb_protocol_t *protocol = influxdb_protocol_v1();
  REQUIRE( protocol != NULL );

  influxdb_writer_t writer;
  writer.data = stdout;
  writer.write = write_to_stream;

  influxdb_point_t point;
  memset(&point, 0, sizeof(influxdb_point_t));
  point.name = "cpu";

  influxdb_tag_t tags[2];
  tags[0] = influxdb_tag("host", "server01");
  tags[1] = influxdb_tag("region", "uswest");
  point.tags = influxdb_tags(tags, 2);

  influxdb_field_t fields[1];
  fields[0] = influxdb_float("value", 2.5);
  point.fields = influxdb_fields(fields, 1);

  influxdb_protocol_encode(protocol, &writer, &point);
  influxdb_protocol_cleanup(protocol);
}
