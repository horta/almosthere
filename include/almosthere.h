struct almosthere *almosthere_create(long volume);
void almosthere_update(struct almosthere *at, long consume);
struct almosthere *almosthere_finish(struct almosthere *at);
void almosthere_sleep(long milliseconds);
