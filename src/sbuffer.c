#include "influxdb.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define INFLUXDB_SBUFFER_INIT_SIZE 256

void influxdb_sbuffer_init(influxdb_sbuffer_t *self) {
  memset(self, 0, sizeof(influxdb_sbuffer_t));
}

void influxdb_sbuffer_destroy(influxdb_sbuffer_t *self) {
  free(self->data);
}

int influxdb_sbuffer_write(void *data, const char *buf, size_t len) {
  influxdb_sbuffer_t *self = (influxdb_sbuffer_t *) data;
  if (self->cap < self->len + len) {
    // Must allocate more space so this write actually succeeds.
    size_t nsize = (self->cap) ? self->cap * 2 : INFLUXDB_SBUFFER_INIT_SIZE;
    while (nsize < self->len + len) {
      size_t tmp_nsize = nsize * 2;
      if (tmp_nsize <= nsize) {
        nsize = self->len + len;
        break;
      }
      nsize = tmp_nsize;
    }

    void *tmp = realloc(self->data, nsize);
    if (!tmp) { return -1; }

    self->data = (char *) tmp;
    self->cap = nsize;
  }
  memcpy(self->data + self->len, buf, len);
  self->len += len;
  return 0;
}

int influxdb_sbuffer_printf(influxdb_sbuffer_t *self, const char * format, ...) {
  char *p = self->data + self->len;
  size_t cap = self->cap - self->len;
  int len = 0;
  {
    va_list ap;
    va_start(ap, format);
    len = vsnprintf(p, cap, format, ap);
    va_end(ap);
  }

  if (len < cap) {
    self->len += len;
    return len;
  }

  // Must allocate more space so this write actually succeeds.
  size_t nsize = (self->cap) ? self->cap * 2 : INFLUXDB_SBUFFER_INIT_SIZE;
  while (nsize < self->len + len) {
    size_t tmp_nsize = nsize * 2;
    if (tmp_nsize <= nsize) {
      nsize = self->len + len;
      break;
    }
    nsize = tmp_nsize;
  }

  void *tmp = realloc(self->data, nsize);
  if (!tmp) { return -1; }

  self->data = (char *) tmp;
  self->cap = nsize;

  p = self->data + self->len;
  cap = self->cap - self->len;

  va_list ap;
  va_start(ap, format);
  self->len += vsnprintf(p, cap, format, ap);
  va_end(ap);
  return 0;
}
