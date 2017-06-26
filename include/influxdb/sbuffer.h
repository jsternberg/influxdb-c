#ifndef _INFLUXDB_SBUFFER_H
#define _INFLUXDB_SBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct influxdb_sbuffer_t {
  char *data;
  size_t len;
  size_t cap;
} influxdb_sbuffer_t;

void influxdb_sbuffer_init(influxdb_sbuffer_t *self);

void influxdb_sbuffer_destroy(influxdb_sbuffer_t *self);

int influxdb_sbuffer_write(void *data, const char *buf, size_t len);

int influxdb_sbuffer_printf(influxdb_sbuffer_t *self, const char * format, ...);

#ifdef __cplusplus
}
#endif
#endif
