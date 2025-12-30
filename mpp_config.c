#include "mpp_config.h"
#include "src/mpp_notify.h"
#include "src/mpp_ffmpeg.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int mpp_test_conn(){
    struct mpd_connection *conn;

    printf("Testing connection...\n");
    conn = get_conn();

    if (conn == NULL) {
        return 1;
    }
    else {
        printf("MPD connection it's ok.\n");
    }

    printf("Closing connection...\n");
    mpd_connection_free(conn);
    return 0;
}

void mpp_save_thumb(struct mpd_song *song){
    ffmpeg_get_cover(mpd_get_path_song_current(song));
}

void mpp_notify(struct mpd_song *song){
    const char *artist = mpd_get_tag(song, MPD_TAG_ARTIST);
    const char *title = mpd_get_tag(song, MPD_TAG_TITLE);
    unsigned n = mpd_get_tag_interator(song, MPD_TAG_ARTIST);
    char *artist_name;

    system("rm -f /tmp/thumb*");

    if (n > 1){
        asprintf(&artist_name, "%s (+%d)", artist, n-1);
    } else {
        asprintf(&artist_name, "%s", artist);
    }

    ffmpeg_get_cover(mpd_get_path_song_current(song));

    time_t ts = time(NULL);
    char img_path[512];
    snprintf(img_path, sizeof(img_path), "/tmp/thumb_%ld.jpg", ts);
    rename("/tmp/thumb.jpg", img_path);

    // concatena o valor de img_path e o padrão em img_uri
    char img_uri[1024];
    snprintf(img_uri, sizeof(img_uri), "file://%s", img_path);

    // Envia notificação
    send_dbus_notification(artist_name, title, "music-app", img_uri);
}

void mpp_help(){
    puts("- Usage -");
    puts("\t<Command> \t\t<Description>");
    puts("\tmpp notify\t\tsend current mpd song notification");
    puts("\tmpp play\t\tsend play to mpd server");
    puts("\tmpp stop\t\tsend stop to mpd server");
    puts("\tmpp toggle\t\ttoggle song state (play/pause)");
    puts("\tmpp prev\t\tgo to previous song");
    puts("\tmpp next\t\tgo to next song");
    puts("\tmpp random\t\ttoggle random mode");
    puts("\tmpp tag artist\t\tshow artist tag from current song");
    puts("\tmpp tag album-artist\tshow album artist tag from current song");
    puts("\tmpp tag album\t\tshow album tag from current song");
    puts("\tmpp tag year\t\tshow year tag from current song");
    puts("\tmpp tag title\t\tshow title tag from current song");
    puts("\tmpp tag track\t\tshow track tag from current song");
    puts("\tmpp tag disc\t\tshow disc tag from current song");
    puts("\tmpp tag genre\t\tshow genre tag from current song");
    puts("\tmpp path song\t\tshow current song filepath");
    puts("\tmpp path folder\t\tshow current artist folder art path");
    puts("\tmpp save-thumb\t\tsave thumb from current song");
}
