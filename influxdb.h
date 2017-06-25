#ifndef _INFLUXDB_H
#define _INFLUXDB_H

#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * NAME influxdb_global_init()
 *
 * DESCRIPTION
 *
 * influxdb_global_init() should be invoked before any other functions. It
 * initializes the curl subsystem by calling curl_global_init(). If your
 * application uses libcurl itself and needs to use anything other than the
 * default options, you should call curl_global_init() yourself before invoking
 * this function.
 */
int influxdb_global_init(void);

/*
 * NAME influxdb_global_cleanup()
 *
 * DESCRIPTION
 *
 * influxdb_global_cleanup() should be invoked exactly once for each
 * application that uses influxdb.
 */
void influxdb_global_cleanup(void);

/*
 * NAME influxdb_client_t
 *
 * DESCRIPTION
 *
 * influxdb_client_t holds the client handle to be used for interacting with
 * the server over the HTTP connection. This object is not thread-safe.
 */
typedef struct influxdb_client_t influxdb_client_t;

/*
 * NAME influxdb_client_init()
 */
influxdb_client_t *influxdb_client_init(void);

/*
 * NAME influxdb_client_cleanup()
 */
void influxdb_client_cleanup(influxdb_client_t *self);

// influxdb_status_t contains the status information returned from influxdb_ping.
typedef struct influxdb_status_t {
  char *version;
} influxdb_status_t;

/*
 * NAME influxdb_ping()
 */
int influxdb_ping(influxdb_client_t *self);

/*
 * NAME influxdb_status()
 */
influxdb_status_t *influxdb_status(influxdb_client_t *self);

/*
 * NAME influxdb_status_cleanup()
 */
void influxdb_status_cleanup(influxdb_status_t *self);

typedef struct influxdb_kv_pair {
  const char *key;
  enum { UNKNOWN = 0, FLOAT, INTEGER, STRING, BOOL } vtype;
  union {
    int i; // This is reused for bool.
    double d;
    const char *s;
  } value;
} influxdb_kv_pair;

/*
 * NAME influxdb_kv_list_t
 *
 * DESCRIPTION
 *
 * influxdb_kv_list_t contains a list of key/value pairs.
 */
typedef struct influxdb_kv_list_t {
  influxdb_kv_pair *data;
  size_t len;
} influxdb_kv_list_t;

/*
 * NAME influxdb_kv_list()
 *
 * DESCRIPTION
 *
 * This function takes a pointer to an array of influxdb_kv_pair_t structs and
 * wraps it in an influxdb_kv_list_t with the length of the array so it can be
 * used with an influxdb_point_t.
 */
influxdb_kv_list_t influxdb_kv_list(influxdb_kv_pair *data, size_t len);

/*
 * NAME influxdb_point_t
 *
 * DESCRIPTION
 *
 * influxdb_point_t contains the definition of a point that can be encoded.
 */
typedef struct influxdb_point_t {
  const char *name;
  influxdb_kv_list_t tags;
  influxdb_kv_list_t fields;
} influxdb_point_t;

/*
 * NAME influxdb_protocol_t
 *
 * DESCRIPTION
 *
 * influxdb_protocol_t holds a handle to a line protocol implementation.
 */
typedef struct influxdb_protocol_t influxdb_protocol_t;

/*
 * NAME influxdb_protocol_v1()
 */
influxdb_protocol_t *influxdb_protocol_v1(void);

/*
 * NAME influxdb_protocol_cleanup()
 */
void influxdb_protocol_cleanup(influxdb_protocol_t *self);

typedef struct influxdb_sbuffer_t {
  char *data;
  size_t len;
  size_t cap;
} influxdb_sbuffer_t;

void influxdb_sbuffer_init(influxdb_sbuffer_t *self);

void influxdb_sbuffer_destroy(influxdb_sbuffer_t *self);

int influxdb_sbuffer_printf(influxdb_sbuffer_t *self, const char * format, ...);

typedef int (*influxdb_write)(void *data, const char *buf, size_t len);

typedef struct influxdb_writer_t {
  void *data;
  influxdb_write write;
} influxdb_writer_t;

/*
 * NAME influxdb_protocol_encode()
 */
int influxdb_protocol_encode(influxdb_protocol_t *self, influxdb_writer_t *w, influxdb_point_t *pt);

/*
 * NAME influxdb_protocol_content_type()
 */
const char *influxdb_protocol_content_type(influxdb_protocol_t *self);

#ifdef __cplusplus
}
#endif
#endif
