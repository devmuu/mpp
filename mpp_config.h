#ifndef MPP_CONFIG_H
#define MPP_CONFIG_H

#include "src/mpp_client.h"

void mpp_notify(struct mpd_song *song);
void mpp_save_thumb(struct mpd_song *song);
void mpp_help();
int mpp_test_conn();

#endif
