#ifndef WIDGET_MAIN_H
#define WIDGET_MAIN_H

struct athr_widget_main;

void __athr_widget_main_create(struct athr_widget_main *);
void __athr_widget_main_setup(struct athr_widget_main *);

struct athr_widget_bar *
__athr_widget_main_add_bar(struct athr_widget_main *main);
struct athr_widget_eta *
__athr_widget_main_add_eta(struct athr_widget_main *main);
struct athr_widget_perc *
__athr_widget_main_add_perc(struct athr_widget_main *main);
struct athr_widget_text *
__athr_widget_main_add_text(struct athr_widget_main *main);

#endif
