#ifndef ALMOSTHERE_CANVAS_H_
#define ALMOSTHERE_CANVAS_H_

struct canvas {
    char *buff;
    int length;
    int min_length;
};

void canvas_create(struct canvas *canvas, int min_length);
void canvas_draw(struct canvas *canvas);
void canvas_clean(struct canvas *canvas);
void canvas_resize(struct canvas *canvas);
void canvas_finish(struct canvas *canvas);

#endif /* end of include guard: ALMOSTHERE_CANVAS_H_ */
