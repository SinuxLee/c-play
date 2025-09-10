typedef struct {
    void *ctx;
    void (*handle)(void *);
} Closure;

void handler(void *ctx){

}

void loop(){
    Closure c = {.ctx = (void*)123, .handle = handler};
    c.handle(c.ctx);
}