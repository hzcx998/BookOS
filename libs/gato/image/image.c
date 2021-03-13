#include "surface.h"
#include <assert.h>
#include <xtk_image.h>
#include <string.h>

surface_t *surface_image_load(char const *filename, int w, int h)
{
    int iw, ih, channels_in_file;
    unsigned char *image =  xtk_load_image_raw(filename, &iw, &ih, &channels_in_file);
    assert(image);

    surface_t *s = surface_alloc(w, h);
    assert(s);
    xtk_resize_image_raw(image, iw, ih, (unsigned char *) s->pixels, w, h, 4);

    if (image)
        free(image);
    return s;
}

surface_t *surface_image_resize(surface_t *image, int w, int h)
{
    assert(image && image->pixels);
    surface_t *s = surface_alloc(w, h);
    assert(s);
    xtk_resize_image_raw((unsigned char *) image->pixels, image->width, image->height,
            (unsigned char *) s->pixels, w, h, 4);
    return s;
}
