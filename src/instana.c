#include "instana.h"
#include "util.h"
#include "http.h"
#include <time.h>
#include <unistd.h>

static const char INSTANA_REST_API_ENDPOINT[]="http://localhost:42699/com.instana.plugin.generic.trace";
static uint64_t parentId = NULL;
static uint64_t traceId  = NULL;

uint64_t epoch_time_in_ms()
{
  time_t seconds; time(&seconds);
  return (unsigned long long)seconds * 1000;
}

uint64_t elapse_time()
{
  struct timeb now;
  ftime(&now);
  uint64_t elapse = now.millitm - start.millitm;
printf("\n************Elapsed time:%ld", elapse);
  start = now;
  return elapse;
}

void init_trace()
{
  srand((unsigned) time(NULL));
  ftime(&start);
}

void populate_metadata(char* json, kv* data, int howmany)
{

  // Populate meta data
  for (int i=0;i<howmany;i++)
  {
      // Add key value pair
      sprintf(json, "%s%s\"%s\":\"%s\"",json, (i > 0)?",":"",data[i].key, data[i].value);
  }

  // Stupid syntax
  sprintf(json, "%s}}",json);
}

void entry_point(char* name, kv* data)
{

    // Start timer
    init_trace();

    // Build json
    char *json = (char*)calloc(4094, sizeof(char));
    assert (json != NULL);

    uint64_t key = rand();

    // Pushs ID to use in subsequent spans
    traceId = parentId = key;
    sprintf(json, "{\"traceId\":%ld,\"spanId\":%ld,\"timestamp\":%ld,\"duration\":%ld,\"name\":\"%s\",\"type\":\"%s\",\"data\":{",
                    key, key, epoch_time_in_ms(), elapse_time(), name,"ENTRY"
    );

    populate_metadata(json, data, 2);
    post((char*)INSTANA_REST_API_ENDPOINT, json);

    free(json);
}

void intermidiate_point(char* name, kv* data)
{
  // Build json
  char *json = (char*)calloc(4094, sizeof(char));
  assert (json != NULL);
  uint64_t key = rand();
  sprintf(json, "{\"traceId\":%ld,\"parentId\":%ld,\"spanId\":%ld, \"timestamp\":%ld,\"duration\":%ld,\"name\":\"%s\",\"type\":\"%s\",\"data\":{",
                  traceId, parentId, key, epoch_time_in_ms(), elapse_time(), name,"INTERMEDIATE"
  );

  // Pushes parentId
  parentId = key;

  populate_metadata(json, data, 0);
  post((char*)INSTANA_REST_API_ENDPOINT, json);
  free(json);
}

void exit_point(char* name, kv* data)
{
  // Build json
  sleep(5);
  char *json = (char*)calloc(4094, sizeof(char));
  assert (json != NULL);
  uint64_t key = rand();
  sprintf(json, "{\"traceId\":%ld,\"parentId\":%ld,\"spanId\":%ld, \"timestamp\":%ld,\"duration\":%ld,\"name\":\"%s\",\"type\":\"%s\",\"data\":{",
                  traceId, parentId, key, epoch_time_in_ms(), elapse_time(), name,"EXIT"
  );

  populate_metadata(json, data, 1);
  post((char*)INSTANA_REST_API_ENDPOINT, json);
  free(json);
}
