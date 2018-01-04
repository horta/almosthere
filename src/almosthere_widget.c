#include "almosthere_widget.h"

void almosthere_widget_create(struct almosthere_widget **widget) {

  *widget = malloc(sizeof(struct almosthere_widget_bar));
  // (*widget)->drawn = 0;
}

void almosthere_widget_create(struct almosthere_widget *widget) {
  free(widget);
}
