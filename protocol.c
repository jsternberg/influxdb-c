#include <stdlib.h>
#include "influxdb.h"

typedef struct influxdb_protocol_t {
  void *data;
  int (*encode)(void *, influxdb_writer_t *, influxdb_point_t *);
  void (*free)(void *);
  const char *content_type;
} influxdb_protocol_t;

static int influxdb_protocol_v1_encode(void *, influxdb_writer_t *, influxdb_point_t *);
static void influxdb_protocol_v1_free(void *);

influxdb_protocol_t *influxdb_protocol_v1(void) {
  influxdb_protocol_t *self = (influxdb_protocol_t *) malloc(sizeof(influxdb_protocol_t));
  if (!self) {
    return NULL;
  }
  self->data = NULL; // Do not fill with any implementation specifics on initialization.
  self->encode = influxdb_protocol_v1_encode;
  self->free = influxdb_protocol_v1_free;
  self->content_type = "application/x-influxdb-line-protocol-v1";
  return self;
}

void influxdb_protocol_cleanup(influxdb_protocol_t *self) {
  if (self->free) {
    self->free(self->data);
  }
  free(self);
}

int influxdb_protocol_encode(influxdb_protocol_t *self, influxdb_writer_t *w, influxdb_point_t *pt) {
  return self->encode(self->data, w, pt);
}

const char *influxdb_protocol_content_type(influxdb_protocol_t *self) {
  return self->content_type;
}

static int influxdb_protocol_v1_encode(void *data, influxdb_writer_t *w, influxdb_point_t *pt) {
  if (pt->fields.len == 0) {
    return -1;
  }

  influxdb_sbuffer_t buf;
  influxdb_sbuffer_init(&buf);

  influxdb_sbuffer_printf(&buf, "%s", pt->name);
  if (pt->tags.len > 0) {
    size_t len = pt->tags.len;
    for (int i = 0; i < len; i++) {
      influxdb_tag_t *kv = &pt->tags.data[i];
      influxdb_sbuffer_printf(&buf, ",%s=%s", kv->key, kv->value);
    }
  }
  influxdb_sbuffer_printf(&buf, " ");

  size_t len = pt->fields.len;
  for (int i = 0; i < len; i++) {
    if (i > 0) {
      influxdb_sbuffer_printf(&buf, ",");
    }

    influxdb_field_t *kv = &pt->fields.data[i];
    switch (kv->vtype) {
      case INFLUXDB_TYPE_FLOAT:
        influxdb_sbuffer_printf(&buf, "%s=%f", kv->key, kv->value.d);
        break;
      case INFLUXDB_TYPE_INTEGER:
        influxdb_sbuffer_printf(&buf, "%s=%di", kv->key, kv->value.i);
        break;
      case INFLUXDB_TYPE_STRING:
        influxdb_sbuffer_printf(&buf, "%s=\"%s\"", kv->key, kv->value.s);
        break;
      case INFLUXDB_TYPE_BOOL:
        if (kv->value.i) {
          influxdb_sbuffer_printf(&buf, "%s=t", kv->key);
        } else {
          influxdb_sbuffer_printf(&buf, "%s=f", kv->key);
        }
        break;
      default:
        return -1;
    }
  }
  influxdb_sbuffer_printf(&buf, "\n");
  int ret = w->write(w->data, buf.data, buf.len);
  influxdb_sbuffer_destroy(&buf);
  return ret;
}

static void influxdb_protocol_v1_free(void *data) {
  free(data);
}
