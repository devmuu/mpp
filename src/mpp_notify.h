#ifndef MPP_NOTIFY_H
#define MPP_NOTIFY_H

#define TMP_DIR "/tmp"
#define NOTIFY_ID_FILE "/tmp/mpd_notify_id"

// send notification
void send_dbus_notification(const char *artist, const char *title, const char *icon_path, const char *img_uri);

#endif
