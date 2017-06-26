#ifndef _INFLUXDB_PROTOCOL_H
#define _INFLUXDB_PROTOCOL_H

#include <influxdb/writer.h>

#ifdef __cplusplus
extern "C" {
#endif

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
