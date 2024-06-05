#ifndef __AUTO_SPLITTER_H__
#define __AUTO_SPLITTER_H__

#include <linux/limits.h>
#include <stdatomic.h>
#include <stdbool.h>

extern atomic_bool auto_splitter_enabled;
extern atomic_bool call_start;
extern atomic_bool call_split;
extern atomic_bool toggle_loading;
extern atomic_bool call_reset;
extern char auto_splitter_file[PATH_MAX];
extern int maps_cache_cycles_value;

extern char desc_gamename[50];
extern char desc_authors[100];
extern char desc_notes[500];
extern char desc_version[10];
extern bool desc_experimental;

void check_directories();
void run_auto_splitter();

#endif /* __AUTO_SPLITTER_H__ */
