#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include "influxdb.h"

int influxdb_global_init(void) {
  curl_global_init(CURL_GLOBAL_DEFAULT);
  return 0;
}

void influxdb_global_cleanup(void) {
  curl_global_cleanup();
}

struct influxdb_client_t {
  CURL *handle;
};

influxdb_client_t *influxdb_client_init() {
  influxdb_client_t *self = (influxdb_client_t *) malloc(sizeof(influxdb_client_t));
  self->handle = curl_easy_init();
  return self;
}

void influxdb_client_cleanup(influxdb_client_t *self) {
  curl_easy_cleanup(self->handle);
  free(self);
}

int influxdb_ping(influxdb_client_t *self) {
  curl_easy_setopt(self->handle, CURLOPT_URL, "http://127.0.0.1:8086/ping");

  CURLcode res = curl_easy_perform(self->handle);
  if (res != CURLE_OK) {
    curl_easy_reset(self->handle);
    return res;
  }

  // Verify the return code is 2xx.
  long http_code = 0;
  curl_easy_getinfo(self->handle, CURLINFO_RESPONSE_CODE, &http_code);
  curl_easy_reset(self->handle);
  return http_code / 100 != 2;
}

static size_t influxdb_status_callback(char *buffer, size_t size, size_t nitems, void *userdata) {
  influxdb_status_t *status = (influxdb_status_t *) userdata;
  if (!status->version && nitems > 20) { // "X-Influxdb-Version: " size.
    if (memcmp(buffer, "X-Influxdb-Version: ", 20) == 0) {
      size_t len = nitems - 20 - 2; // \r\n is the extra 2.
      char *version = (char *) calloc(len + 1, sizeof(char));
      memcpy(version, buffer+20, len);
      status->version = version;
    }
  }
  return size * nitems;
}

influxdb_status_t *influxdb_status(influxdb_client_t *self) {
  influxdb_status_t *status = (influxdb_status_t *) calloc(1, sizeof(influxdb_status_t));
  if (!status) {
    return NULL;
  }

  curl_easy_setopt(self->handle, CURLOPT_URL, "http://127.0.0.1:8086/ping");
  curl_easy_setopt(self->handle, CURLOPT_HEADERFUNCTION, influxdb_status_callback);
  curl_easy_setopt(self->handle, CURLOPT_HEADERDATA, status);

  CURLcode res = curl_easy_perform(self->handle);
  if (res != CURLE_OK) {
    influxdb_status_cleanup(status);
    free(status);
    curl_easy_reset(self->handle);
    return NULL;
  }

  // Verify the return code is 2xx.
  long http_code = 0;
  curl_easy_getinfo(self->handle, CURLINFO_RESPONSE_CODE, &http_code);
  curl_easy_reset(self->handle);

  if (http_code / 100 != 2) {
    influxdb_status_cleanup(status);
    free(status);
    return NULL;
  }
  return status;
}

void influxdb_status_cleanup(influxdb_status_t *self) {
  free(self->version);
}
