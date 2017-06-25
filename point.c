#include <stdlib.h>
#include "influxdb.h"

influxdb_kv_list_t influxdb_kv_list(influxdb_kv_pair *data, size_t len) {
  influxdb_kv_list_t list;
  list.data = data;
  list.len = len;
  return list;
}
