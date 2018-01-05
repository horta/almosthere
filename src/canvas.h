#ifndef ALMOSTHERE_CANVAS_H_
#define ALMOSTHERE_CANVAS_H_

struct canvas {
    char *buff;
    int length;
    int min_length;
};

/**
 * Create a canvas.
 * It also sets the canvas length according to terminal width and
 * minimum length provided.
 * @param canvas Canvas object.
 * @param min_length Required minimum canvas length.
 */
void canvas_create(struct canvas *canvas, int min_length);
/**
 * Assign ASCII character ' ' to every canvas character.
 * @param canvas Canvas object.
 */
void canvas_draw(struct canvas *canvas);
void canvas_clean(struct canvas *canvas);
/**
 * Resize canvas according to terminal width and minimum length.
 * @param canvas Canvas object.
 */
void canvas_resize(struct canvas *canvas);
void canvas_finish(struct canvas *canvas);

#endif /* end of include guard: ALMOSTHERE_CANVAS_H_ */
