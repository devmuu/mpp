#ifndef MPP_CLIENT_H
#define MPP_CLIENT_H

#include <mpd/client.h>
#include <stdio.h>
#include <stdlib.h>

#define AUDIO_DIR getenv("AUDIO_DIR")

typedef enum {
    mpd_tag_type,
} MetadataType;

typedef enum {
    TOGGLE,
    PLAY,
    STOP,
    PREVIOUS,
    NEXT,
} MPDControl;

typedef enum {
    RANDOM,
} MPDStatus;

struct mpd_connection *get_conn();
const char *mpd_get_path_song_current(struct mpd_song *song);
const char *mpd_get_path_cover_artist(struct mpd_song *song);
unsigned mpd_get_tag_interator(struct mpd_song *song, MetadataType mpd_tag);
const char *mpd_get_tag(struct mpd_song *song, MetadataType mpd_tag);
void *mpd_print_tag(struct mpd_song *song, MetadataType mpd_tag);
void *mpd_ctrl_song(struct mpd_connection *conn, MPDControl ctrl_key);
void *mpd_toggle_status(struct mpd_connection *conn, MPDStatus status_key);

#endif
