#include <time.h>
#include "mongoose.h"

int i = 0;

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  struct mg_http_serve_opts opts = {.root_dir = "."};
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;

    if (mg_http_match_uri(hm, "/api/sum")) {
      // Attempt to fetch parameters from the body, hm->body
      struct mg_str params = hm->body;
      double num1, num2;
      if (mg_json_get_num(params, "$[0]", &num1) &&
          mg_json_get_num(params, "$[1]", &num2)) {
        // Success! create a JSON response
        mg_http_reply(c, 200, "Content-Type: application/json\r\n", "{%Q:%d}\n",
                      "result", num1 + num2);
      } else {
        mg_http_reply(c, 500, NULL, "%s", "Parameters missing");
      }
    } else if (mg_http_match_uri(hm, "/api/test")) {
      printf("backend was called\n");
      time_t rawtime;
      time(&rawtime);
      struct tm *timeinfo;
      timeinfo = localtime(&rawtime);
      char buffer[80];
      strftime(buffer, sizeof(buffer), " %H:%M:%S", timeinfo);
      mg_http_reply(c, 200, "Content-Type: application/json\r\n",
                    "{\"time\":\"%s\", \"signals\": [{\"name\": "
                    "\"signal1\", \"value\" : \"10\", \"unit\" : "
                    "\"celcius\"}, {\"name\": "
                    "\"signal2\", \"value\" : \"20\", \"unit\": "
                    "\"celcius\"}, {\"name\": "
                    "\"signal3\", \"value\" : \"30\", \"unit\": "
                    "\"celcius\"}, {\"name\": "
                    "\"signal4\", \"value\" : \"35\", \"unit\": "
                    "\"celcius\"} ]}\n",
                    buffer);
    } else {
      // mg_http_reply(c, 500, NULL, "%s", "Invalid URI");
      mg_http_serve_dir(c, ev_data, &opts);
    }
  }
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;
  printf("Howdy\n");
  mg_mgr_init(&mgr);  // Init manager
  mg_http_listen(&mgr, "http://localhost:8002", fn,
                 &mgr);              // Setup listener
  for (;;) mg_mgr_poll(&mgr, 1000);  // Event loop
  mg_mgr_free(&mgr);                 // Cleanup
  return 0;
}
