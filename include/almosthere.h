struct almosthere *almosthere_create(int64_t volume);
void almosthere_update(struct almosthere *at, int64_t consume);
struct almosthere *almosthere_finish(struct almosthere *at);
