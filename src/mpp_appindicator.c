#include "mpp_appindicator.h"

// INFO: in implementation, only to test.
// STATUS: Not complete

static AppIndicator *indicator = NULL;

// callback functions to control mpd
static void cb_play_pause(GtkMenuItem *item, gpointer data) {
    // toggle play
}

static void cb_next(GtkMenuItem *item, gpointer data) {
    // next
}

static void cb_prev(GtkMenuItem *item, gpointer data) {
    // previous
}

static void cb_stop(GtkMenuItem *item, gpointer data) {
    // stop
}

static void cb_toggle_notify(GtkCheckMenuItem *item, gpointer data) {
    gboolean enabled = gtk_check_menu_item_get_active(item);
    // TODO: test if notify enable in change music
}

static void cb_reconnect(GtkMenuItem *item, gpointer data) {
    // TODO: reconnect with mpd
}

// close appindicator
static void cb_quit(GtkMenuItem *item, gpointer data) {
    gtk_main_quit();
}

// main appindicator menu
GtkWidget *create_menu(void) {
    GtkWidget *menu = gtk_menu_new();

    GtkWidget *play = gtk_menu_item_new_with_label("Toggle Play");
    GtkWidget *next = gtk_menu_item_new_with_label("Next");
    GtkWidget *prev = gtk_menu_item_new_with_label("Previous");
    GtkWidget *stop = gtk_menu_item_new_with_label("Stop");

    GtkWidget *notify = gtk_check_menu_item_new_with_label("Enable Notifications");
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(notify), TRUE);

    GtkWidget *reconnect = gtk_menu_item_new_with_label("Reconnect MPD");
    GtkWidget *quit = gtk_menu_item_new_with_label("Quit");

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), play);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), next);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), prev);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), stop);

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), gtk_separator_menu_item_new());

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), notify);

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), gtk_separator_menu_item_new());

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), reconnect);

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), gtk_separator_menu_item_new());

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), quit);

    // callbacks
    g_signal_connect(play, "activate", G_CALLBACK(cb_play_pause), NULL);
    g_signal_connect(next, "activate", G_CALLBACK(cb_next), NULL);
    g_signal_connect(prev, "activate", G_CALLBACK(cb_prev), NULL);
    g_signal_connect(stop, "activate", G_CALLBACK(cb_stop), NULL);
    g_signal_connect(notify, "toggled", G_CALLBACK(cb_toggle_notify), NULL);
    g_signal_connect(reconnect, "activate", G_CALLBACK(cb_reconnect), NULL);
    g_signal_connect(quit, "activate", G_CALLBACK(cb_quit), NULL);

    // show
    gtk_widget_show_all(menu);

    return menu;
}

// init appindicator
void mpp_indicator_init(void) {
    indicator = app_indicator_new(
        "mpp",
        "multimedia-player",
        APP_INDICATOR_CATEGORY_APPLICATION_STATUS
    );

    app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);
    app_indicator_set_menu(indicator, GTK_MENU(create_menu()));
}

