#ifndef _INFLUXDB_H
#define _INFLUXDB_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct influxdb_client_t influxdb_client_t;

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

#ifdef __cplusplus
}
#endif
#endif
