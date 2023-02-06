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
      printf("backend called\n");
      const char *file_path = "data.txt";

      char buffer[64];
      time_t raw_time;
      struct tm *time_info;

      time(&raw_time);
      time_info = localtime(&raw_time);

      strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", time_info);

      // Open the file
      FILE *fp = fopen(file_path, "r");

      // Read the data from the file and store it in a buffer
      char data[1024];
      fread(data, sizeof(char), 1024, fp);

      // Close the file
      fclose(fp);

      // Return the data in JSON format
      mg_http_reply(c, 200, "Content-Type: application/json\r\n",
                    "{\"time\":\"%s\",\"signals\":%s}", buffer, data);
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
