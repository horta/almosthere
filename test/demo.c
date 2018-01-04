int main() {

  struct almosthere *at = almosthere_create(150);

  sleep_ms(400);
  almosthere_consume(at, 5);
  sleep_ms(500);
  almosthere_consume(at, 5);
  sleep_ms(500);
  almosthere_consume(at, 5);
  sleep_ms(400);
  almosthere_consume(at, 5);
  sleep_ms(300);
  almosthere_consume(at, 5);
  sleep_ms(200);
  almosthere_consume(at, 5);
  sleep_ms(100);
  almosthere_consume(at, 5);
  sleep_ms(50);
  almosthere_consume(at, 5);
  sleep_ms(50);
  almosthere_consume(at, 5);

  sleep_ms(400);
  almosthere_consume(at, 10);
  sleep_ms(400);
  almosthere_consume(at, 10);
  sleep_ms(400);
  almosthere_consume(at, 10);
  sleep_ms(40);
  almosthere_consume(at, 10);
  sleep_ms(40);
  almosthere_consume(at, 10);

  sleep_ms(800);
  almosthere_consume(at, 55);

  cline_finish(at);

  return 0;
}
