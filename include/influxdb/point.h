#ifndef _INFLUXDB_POINT_H
#define _INFLUXDB_POINT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct influxdb_tag_t {
  const char *key;
  const char *value;
} influxdb_tag_t;

static inline influxdb_tag_t influxdb_tag(const char *key, const char *value) {
  influxdb_tag_t kv = { key, value };
  return kv;
}

typedef struct influxdb_tags_t {
  influxdb_tag_t *data;
  size_t len;
} influxdb_tags_t;

static inline influxdb_tags_t influxdb_tags(influxdb_tag_t *data, size_t len) {
  influxdb_tags_t tags = { data, len };
  return tags;
}

typedef enum {
  INFLUXDB_TYPE_UNKNOWN = 0,
  INFLUXDB_TYPE_FLOAT,
  INFLUXDB_TYPE_INTEGER,
  INFLUXDB_TYPE_STRING,
  INFLUXDB_TYPE_BOOL,
} influxdb_field_type_t;

typedef struct influxdb_field_t {
  const char *key;
  influxdb_field_type_t vtype;
  union {
    int i; // This is reused for bool.
    double d;
    const char *s;
  } value;
} influxdb_field_t;

static inline influxdb_field_t influxdb_float(const char *key, double value) {
  influxdb_field_t kv;
  kv.key = key;
  kv.vtype = INFLUXDB_TYPE_FLOAT;
  kv.value.d = value;
  return kv;
}

static inline influxdb_field_t influxdb_integer(const char *key, int64_t value) {
  influxdb_field_t kv;
  kv.key = key;
  kv.vtype = INFLUXDB_TYPE_INTEGER;
  kv.value.i = value;
  return kv;
}

static inline influxdb_field_t influxdb_string(const char *key, const char *value) {
  influxdb_field_t kv;
  kv.key = key;
  kv.vtype = INFLUXDB_TYPE_STRING;
  kv.value.s = value;
  return kv;
}

static inline influxdb_field_t influxdb_boolean(const char *key, bool value) {
  influxdb_field_t kv;
  kv.key = key;
  kv.vtype = INFLUXDB_TYPE_BOOL;
  kv.value.i = value;
  return kv;
}

/*
 * NAME influxdb_kv_list_t
 *
 * DESCRIPTION
 *
 * influxdb_kv_list_t contains a list of key/value pairs.
 */
typedef struct influxdb_fields_t {
  influxdb_field_t *data;
  size_t len;
} influxdb_fields_t;

/*
 * NAME influxdb_fields()
 *
 * DESCRIPTION
 *
 * This function takes a pointer to an array of influxdb_field_t structs and
 * wraps it in an influxdb_fields_t with the length of the array so it can be
 * used with an influxdb_point_t.
 */
static inline influxdb_fields_t influxdb_fields(influxdb_field_t *data, size_t len) {
  influxdb_fields_t fields = { data, len };
  return fields;
}

/*
 * NAME influxdb_point_t
 *
 * DESCRIPTION
 *
 * influxdb_point_t contains the definition of a point that can be encoded.
 */
typedef struct influxdb_point_t {
  const char *name;
  influxdb_tags_t tags;
  influxdb_fields_t fields;
} influxdb_point_t;

#ifdef __cplusplus
}
#endif
#endif
