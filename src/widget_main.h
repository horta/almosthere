#ifndef WIDGET_MAIN_H
#define WIDGET_MAIN_H

struct athr_widget_main;

void widget_main_create(struct athr_widget_main *);
void widget_main_setup(struct athr_widget_main *);

struct athr_widget_bar *widget_main_add_bar(struct athr_widget_main *main);
struct athr_widget_eta *widget_main_add_eta(struct athr_widget_main *main);
struct athr_widget_perc *widget_main_add_perc(struct athr_widget_main *main);
struct athr_widget_text *widget_main_add_text(struct athr_widget_main *main);

#endif
