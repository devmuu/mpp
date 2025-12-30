#include "mpp_client.h"

// initialize a mpd connection
struct mpd_connection *get_conn() {
    struct mpd_connection *conn;
    // mpd_connection_new(host, port, timeout_ms )
    // if tcp: host:NULL to get default host
    conn = mpd_connection_new("/run/user/1000/mpd/socket", 0, 0);

    if (conn == NULL) {
        fprintf(stderr, "Out of memory\n");
        return NULL;
    }

    if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS) {
        fprintf(stderr, "%s\n", mpd_connection_get_error_message(conn));
        mpd_connection_free(conn);

        return NULL;
    }

    return conn;
}

// concat filename with default music path
const char *mpd_get_path_cover_artist(struct mpd_song *song) {
    char *output = "";
    const char *artist = mpd_get_tag(song, MPD_TAG_ARTIST);

    asprintf(&output, "%s/%s%s%s", AUDIO_DIR, "metadata/Artists/", artist, "/folder.jpg");

    return(output);
}

// get current song path
const char *mpd_get_path_song_current(struct mpd_song *song) {
    const char *filename = "";
    char *output = "";

    filename = mpd_song_get_uri(song);
    asprintf(&output, "%s/%s", AUDIO_DIR, filename);

    return(output);
}

// get number of elements in tag
unsigned mpd_get_tag_interator(struct mpd_song *song, MetadataType mpd_tag) {
    unsigned i = 0;

    do i++; while (mpd_song_get_tag(song, mpd_tag, i) != NULL);
    return i;
}

// get song tag
const char *mpd_get_tag(struct mpd_song *song, MetadataType mpd_tag) {
    const char *output = "";

    output = mpd_song_get_tag(song, mpd_tag, 0);
    return output;
}

void *mpd_print_tag(struct mpd_song *song, MetadataType mpd_tag) {
    unsigned i = 0;
    unsigned j = 0;
    const char *output = "";

    do i++; while (mpd_song_get_tag(song, mpd_tag, i) != NULL);

    while ((output = mpd_song_get_tag(song, mpd_tag, j++)) != NULL) {
        if (j == i) {
            printf("%s\n", output);
        } else {
            printf("%s, ", output);
        }
    }
}

void *mpd_ctrl_song(struct mpd_connection *conn, MPDControl ctrl_key) {
    switch(ctrl_key){
        case PLAY:
            mpd_run_play(conn);
            break;
        case STOP:
            mpd_run_stop(conn);
            break;
        case TOGGLE:
            mpd_run_toggle_pause(conn);
            break;
        case PREVIOUS:
            mpd_run_previous(conn);
            break;
        case NEXT:
            mpd_run_next(conn);
            break;

        default:
            puts("Not valid control.");
    }
}

void *mpd_toggle_status(struct mpd_connection *conn, MPDStatus status_key) {

    struct mpd_status *status;
    status = mpd_run_status(conn);

    switch(status_key){
        case RANDOM:
            bool C_RANDOM = mpd_status_get_random(status);
            printf("Random mode is %s.\n", !C_RANDOM ? "on" : "off");
            mpd_run_random(conn, !C_RANDOM);
            break;

        default:
            puts("Not valid control.");
    }

    mpd_status_free(status);
}
