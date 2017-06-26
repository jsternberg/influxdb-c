#ifndef _INFLUXDB_WRITER_H
#define _INFLUXDB_WRITER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*influxdb_write)(void *data, const char *buf, size_t len);

typedef struct influxdb_writer_t {
  void *data;
  influxdb_write write;
} influxdb_writer_t;

#ifdef __cplusplus
}
#endif
#endif
