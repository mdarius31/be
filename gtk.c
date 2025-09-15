// The following line is needed to build the program on some systems.
// For example, when using GCC, compile with:
// gcc main.c -o pixel_display `pkg-config --cflags --libs gtk+-3.0`

#include <gtk/gtk.h>
#include <cairo.h>
#include <stdlib.h>
#include <stdio.h>

// Your custom pixel structure
typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Pixel;

// Global variables for the pixel data and its dimensions
#define WIDTH 256
#define HEIGHT 256
Pixel pixel_array[WIDTH * HEIGHT];

// A static function to clean up the Cairo surface data.
// This is a C callback function and must be defined this way.
// It's called when the Cairo surface is destroyed.
static void free_cairo_data(void *data) {
    if (data) {
        free(data);
    }
}

// The main drawing function for the GtkDrawingArea.
// This function is connected to the "draw" signal.
static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer data) {
    int widget_width, widget_height;
    gtk_window_get_size(GTK_WINDOW(gtk_widget_get_toplevel(widget)), &widget_width, &widget_height);

    // Create a new unsigned char array for Cairo.
    // We use calloc to ensure the memory is initialized to zero.
    // Cairo's CAIRO_FORMAT_ARGB32 is a big-endian format, meaning the byte order is
    // A, R, G, B in memory.
    unsigned char *cairo_data = (unsigned char *)calloc(WIDTH * HEIGHT * 4, sizeof(unsigned char));
    if (!cairo_data) {
        perror("Failed to allocate memory for Cairo data");
        return FALSE;
    }

    // Convert our Pixel array to Cairo's ARGB32 format.
    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        int index = i * 4;
        cairo_data[index + 0] = 255; // A (Alpha)
        cairo_data[index + 1] = pixel_array[i].r; // R (Red)
        cairo_data[index + 2] = pixel_array[i].g; // G (Green)
        cairo_data[index + 3] = pixel_array[i].b; // B (Blue)
    }

    // Create a Cairo image surface from our pixel data.
    // The `stride` is the number of bytes per row, which is `WIDTH * 4` for ARGB32.
    // The `free_cairo_data` callback will be called when the surface is destroyed.
    cairo_surface_t *surface = cairo_image_surface_create_for_data(
        cairo_data,
        CAIRO_FORMAT_ARGB32,
        WIDTH,
        HEIGHT,
        WIDTH * 4);

    if (cairo_surface_status(surface) != CAIRO_STATUS_SUCCESS) {
        fprintf(stderr, "Could not create cairo surface: %s\n", cairo_status_to_string(cairo_surface_status(surface)));
        free(cairo_data);
        return FALSE;
    }

    // Set the source of the Cairo context to our new surface
    cairo_set_source_surface(cr, surface, 0, 0);

    // Paint the surface onto the drawing area
    cairo_paint(cr);

    // Clean up the surface. This will also call our free_cairo_data callback to free the memory.
    cairo_surface_destroy(surface);

    return FALSE; // Return FALSE to indicate we are not handling further drawing
}

// Function to initialize the pixel_array with some data.
// In a real application, you would load an image here.
void initialize_pixel_data() {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            int index = y * WIDTH + x;
            pixel_array[index].r = x;
            pixel_array[index].g = y;
            pixel_array[index].b = 255 - x;
        }
    }
}

int main(int argc, char *argv[]) {
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Initialize our custom pixel data
    initialize_pixel_data();

    // Create a new window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GTK Pixel Display");
    gtk_container_set_border_width(GTK_CONTAINER(window), 0);
    gtk_widget_set_size_request(window, WIDTH, HEIGHT);
    
    // Connect the "destroy" signal to quit the GTK main loop
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a GtkDrawingArea
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, WIDTH, HEIGHT);
    
    // Connect the "draw" signal to our custom drawing function
    g_signal_connect(drawing_area, "draw", G_CALLBACK(on_draw_event), NULL);

    // Add the drawing area to the window
    gtk_container_add(GTK_CONTAINER(window), drawing_area);

    // Show all widgets and start the GTK main loop
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
