#include <common.h>

extern uint64_t g_nr_guest_inst;

#ifndef CONFIG_TARGET_AM
FILE *log_fp = NULL;
FILE *ftrace_fp = NULL;

void init_log(const char *log_file) {
  log_fp = stdout;
  if (log_file != NULL) {
    FILE *fp = fopen(log_file, "w");
    Assert(fp, "Can not open '%s'", log_file);
    log_fp = fp;
  }
  Log("Log is written to %s", log_file ? log_file : "stdout");
}

bool log_enable() {
  return MUXDEF(CONFIG_TRACE, (g_nr_guest_inst >= CONFIG_TRACE_START) &&
         (g_nr_guest_inst <= CONFIG_TRACE_END), false);
}

void init_ftrace(const char *trace_file) {
  ftrace_fp = stdout;
  if (trace_file != NULL) {
    FILE *fp = fopen(trace_file, "w");
    Assert(fp, "Can not open '%s'", trace_file);
    ftrace_fp = fp;
  }
  Log("Ftrace is written to %s", trace_file ? trace_file : "stdout");
}

bool ftrace_enable() {
  bool enable = MUXDEF(CONFIG_TRACE, (g_nr_guest_inst >= CONFIG_TRACE_START) &&
         (g_nr_guest_inst <= CONFIG_TRACE_END), false);
  return enable;
}

#endif
