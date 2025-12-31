#ifndef MPP_CONFIG_H
#define MPP_CONFIG_H

#include "src/mpp_client.h"

typedef enum {
    MPP_DESKTOP_UNKNOWN = 0,
    MPP_DESKTOP_GNOME,
    MPP_DESKTOP_KDE,
    MPP_DESKTOP_HYPRLAND
} MppDesktop;

MppDesktop mpp_desktop_detect(void);
const char *mpp_desktop_to_string(MppDesktop d);

void mpp_notify(struct mpd_song *song);
void mpp_save_thumb(struct mpd_song *song);
void mpp_help();
int mpp_test_conn();

#endif
