#include "mpp_config.h"
#include "src/mpp_notify.h"
#include <stdio.h>
#include <string.h>
// #include <unistd.h>

// main function
int main(int argc, char **argv) {
    // struct to connection and song entities
    struct mpd_connection *conn;
    struct mpd_song *song;

    // init connection and create song
    conn = get_conn();
    song = mpd_run_current_song(conn);

    // test if song exists
    if (song == NULL || conn == NULL){
        puts("No song playing or waiting.");
        return 1;
    }

    // string to put option from user argument
    char *opt;
    opt = argv[1];

    if (opt == NULL || strcmp(opt, "help") == 0) {
        mpp_help();
    }
    else if (argc > 3) {
        printf("You pass more than two argument.\n");
        return 1;
    }
    else if (argc <= 3) {
        char *opt2;
        opt2 = argv[2];

        /* show paths */
        if (strcmp(opt, "path") == 0) {
            if (opt2 == NULL){
                puts("path option requires <song> or <folder> argument.");
            } else if (strcmp(opt2, "song") == 0) {
                const char *songpath = mpd_get_path_song_current(song);
                if (songpath == NULL) {
                    puts("No one song playing.");
                } else {
                    puts(songpath);
                }
            }
            else if (strcmp(opt2, "folder") == 0) {
                puts(mpd_get_path_cover_artist(song));
            } else {
                puts("Invalid option. Choose <song> or <folder>.");
            }
        }

        /* show tags */
        else if (strcmp(opt, "tag") == 0) {
            if (opt2 == NULL || strcmp(opt2, "help") == 0){
                puts("tag option requires <tag> argument.");
                puts("\talbum-artist");
                puts("\tartist");
                puts("\ttitle");
                puts("\talbum");
                puts("\tyear");
                puts("\ttrack");
                puts("\tdisc");
                puts("\tgenre");
            } else if (strcmp(opt2, "album-artist") == 0) {
                puts(mpd_get_tag(song, MPD_TAG_ALBUM_ARTIST));
            }
            else if (strcmp(opt2, "artist") == 0) {
                mpd_print_tag(song, MPD_TAG_ARTIST);
            }
            else if (strcmp(opt2, "album") == 0) {
                puts(mpd_get_tag(song, MPD_TAG_ALBUM));
            }
            else if (strcmp(opt2, "year") == 0) {
                puts(mpd_get_tag(song, MPD_TAG_DATE));
            }
            else if (strcmp(opt2, "title") == 0) {
                puts(mpd_get_tag(song, MPD_TAG_TITLE));
            }
            else if (strcmp(opt2, "track") == 0) {
                puts(mpd_get_tag(song, MPD_TAG_TRACK));
            }
            else if (strcmp(opt2, "disc") == 0) {
                puts(mpd_get_tag(song, MPD_TAG_DISC));
            }
            else if (strcmp(opt2, "genre") == 0) {
                puts(mpd_get_tag(song, MPD_TAG_GENRE));
            } else {
                puts("Invalid option. Choose a valid <tag>.");
            }
        }

        /* send notification */
        else if (strcmp(opt, "notify") == 0) {
            mpp_notify(song);
        }

        else if (strcmp(opt, "save-thumb") == 0) {
            mpp_save_thumb(song);
        }

        /* music control */
        else if (strcmp(opt, "play") == 0) {
            mpd_ctrl_song(conn, PLAY);
        }
        else if (strcmp(opt, "stop") == 0) {
            mpd_ctrl_song(conn, STOP);
        }
        else if (strcmp(opt, "toggle") == 0) {
            mpd_ctrl_song(conn, TOGGLE);
        }
        else if (strcmp(opt, "prev") == 0) {
            mpd_ctrl_song(conn, PREVIOUS);
        }
        else if (strcmp(opt, "next") == 0) {
            mpd_ctrl_song(conn, NEXT);
        }
        else if (strcmp(opt, "random") == 0) {
            mpd_toggle_status(conn, RANDOM);
        }

        // get desktop
        else if (strcmp(opt, "desktop") == 0) {
            MppDesktop d = mpp_desktop_detect();
            printf("Desktop: %s\n", mpp_desktop_to_string(d));
        }

        // end
        else {
            printf("%s it's not a valid option.\n", opt);
        }
    }

    // close connection andd free memory
    mpd_song_free(song);
    mpd_connection_free(conn);

    return 0;
}
