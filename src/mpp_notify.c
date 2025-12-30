#include "mpp_notify.h"
#include <libnotify/notify.h>
#include <gio/gio.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

// GVariant *create_image_data_from_file(const char *filename);

void send_dbus_notification(const char *artist, const char *title, const char *icon_path, const char *img_uri) {
    static guint replaces_id = 0;

    // Tenta ler ID anterior
    FILE *f = fopen(NOTIFY_ID_FILE, "r");
    if (f) {
        fscanf(f, "%u", &replaces_id);
        fclose(f);
    }

    GDBusConnection *conn = g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, NULL);
    if (!conn) {
        fprintf(stderr, "Erro: não foi possível conectar ao D-Bus.\n");
        return;
    }

    const char *safe_artist = artist && *artist ? artist : " ";
    const char *safe_title  = title  && *title  ? title  : " ";
    const char *safe_icon   = icon_path && *icon_path ? icon_path : "music-app";

    GVariantBuilder tuple;
    g_variant_builder_init(&tuple, G_VARIANT_TYPE_TUPLE);

    g_variant_builder_add(&tuple, "s", "mpd");
    g_variant_builder_add(&tuple, "u", replaces_id);
    g_variant_builder_add(&tuple, "s", safe_icon);
    g_variant_builder_add(&tuple, "s", safe_artist);
    g_variant_builder_add(&tuple, "s", safe_title);

    GVariantBuilder actions_builder;
    g_variant_builder_init(&actions_builder, G_VARIANT_TYPE("as"));
    g_variant_builder_add_value(&tuple, g_variant_builder_end(&actions_builder));

    GVariantBuilder hints_builder;
    g_variant_builder_init(&hints_builder, G_VARIANT_TYPE("a{sv}"));

    // urgency notify
    g_variant_builder_add(
        &hints_builder,
        "{sv}",
        "urgency",
        g_variant_new_byte(1)
    );

    // category notify
    g_variant_builder_add(
        &hints_builder,
        "{sv}",
        "category",
        g_variant_new_string("mpd")
    );

    // image
    g_variant_builder_add(
        &hints_builder,
        "{sv}",
        "image-path",
        g_variant_new_string(img_uri)
    );

    // verificar desktop
    // const char *desktop = g_getenv("XDG_CURRENT_DESKTOP");
    // if (desktop && g_strrstr(desktop, "GNOME"))
    // printf("Estou no %s", desktop);

    // não funciona no gnome
    // if (img_uri && *img_uri) {
    //     GVariant *img_var = create_image_data_from_file(img_uri);
    //     if (img_var) {
    //         g_variant_builder_add(
    //             &hints_builder,
    //             "{sv}",
    //             "image-path",
    //             img_var
    //         );
    //     }
    // }

    g_variant_builder_add_value(&tuple, g_variant_builder_end(&hints_builder));

    // timeout
    g_variant_builder_add(&tuple, "i", 2000);

    GVariant *params = g_variant_builder_end(&tuple);

    GError *error = NULL;

    GVariant *result = g_dbus_connection_call_sync(
            conn,
            "org.freedesktop.Notifications",
            "/org/freedesktop/Notifications",
            "org.freedesktop.Notifications",
            "Notify",
            params,
            G_VARIANT_TYPE("(u)"),
            G_DBUS_CALL_FLAGS_NONE,
            2000,
            NULL,
            &error
    );

    if (error) {
        fprintf(stderr, "Erro D-Bus: %s\n", error->message);
        g_error_free(error);
    } else if (result) {
        g_variant_get(result, "(u)", &replaces_id);
        g_variant_unref(result);

        // Salva ID para próxima atualização
        f = fopen(NOTIFY_ID_FILE, "w");
        if (f) {
            fprintf(f, "%u\n", replaces_id);
            fclose(f);
        }
    }

    g_object_unref(conn);
}

// GVariant *create_image_data_from_file(const char *filename) {
//     GError *error = NULL;
//
//     GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, &error);
//     if (!pixbuf) {
//         fprintf(stderr, "Erro ao carregar imagem: %s\n", error->message);
//         g_error_free(error);
//         return NULL;
//     }
//
//     int width           = gdk_pixbuf_get_width(pixbuf);
//     int height          = gdk_pixbuf_get_height(pixbuf);
//     int rowstride       = gdk_pixbuf_get_rowstride(pixbuf);
//     int channels        = gdk_pixbuf_get_n_channels(pixbuf);
//     int has_alpha       = gdk_pixbuf_get_has_alpha(pixbuf);
//     int bits_per_sample = gdk_pixbuf_get_bits_per_sample(pixbuf);
//
//     int data_len;
//     const guchar *pixels = gdk_pixbuf_get_pixels_with_length(pixbuf, &data_len);
//
//     // Constrói o array AY
//     GVariantBuilder data_builder;
//     g_variant_builder_init(&data_builder, G_VARIANT_TYPE("ay"));
//
//     for (int i = 0; i < data_len; i++) {
//         g_variant_builder_add(&data_builder, "y", pixels[i]);
//     }
//
//     // Finaliza o array ay
//     GVariant *data_array = g_variant_builder_end(&data_builder);
//
//     // Constrói a tupla (iiiibiiay)
//
//     GVariant *variant = g_variant_new(
//         "(iiiibii@ay)",
//         width,
//         height,
//         rowstride,
//         has_alpha,
//         bits_per_sample,
//         channels,
//         data_len,
//         data_array
//     );
//
//     g_object_unref(pixbuf);
//     return variant;
// }
