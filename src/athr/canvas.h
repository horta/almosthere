#ifndef ATHR_CANVAS_H
#define ATHR_CANVAS_H

#define ATHR_CANVAS_MAX_SIZE 512

struct athr_canvas
{
    unsigned size;
    unsigned min_size;
    char buff[ATHR_CANVAS_MAX_SIZE];
};

#define ATHR_CANVAS_INIT                                                       \
    (struct athr_canvas)                                                       \
    {                                                                          \
        0, 0, { '\0' }                                                         \
    }

/**
 * Create a canvas.
 * It also sets the canvas size according to terminal width and
 * minimum size provided.
 * @param canvas Canvas object.
 * @param min_size Required minimum canvas size.
 */
void athr_canvas_create(struct athr_canvas *canvas, unsigned min_size);
/**
 * Assign ASCII character ' ' to every canvas character.
 * @param canvas Canvas object.
 */
void athr_canvas_draw(struct athr_canvas const *canvas);
void athr_canvas_clean(struct athr_canvas *canvas);
/**
 * Resize canvas according to terminal width and minimum size.
 * @param canvas Canvas object.
 */
void athr_canvas_resize(struct athr_canvas *canvas);

#endif
