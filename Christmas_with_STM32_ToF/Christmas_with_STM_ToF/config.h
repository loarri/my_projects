
#define ENABLE_SERIAL 1


#define SOGLIA 2000  //threshold
#define TRIGGER_PIN D5  //blu
#define ECHO_PIN D4  //verde

#define RELE D7
#define REVERSE_RELE 1

#define VOLUME 15 // 28?
#define CONTEGGIO 55000

#define LED_ON  digitalWrite(LED_BUILTIN, HIGH)
#define LED_OFF digitalWrite(LED_BUILTIN, LOW)

#if REVERSE_RELE
#define RELE_ON digitalWrite(RELE, LOW)
#define RELE_OFF digitalWrite(RELE, HIGH)
#else
#define RELE_ON digitalWrite(RELE, HIGH)
#define RELE_OFF digitalWrite(RELE, LOW)
#endif
