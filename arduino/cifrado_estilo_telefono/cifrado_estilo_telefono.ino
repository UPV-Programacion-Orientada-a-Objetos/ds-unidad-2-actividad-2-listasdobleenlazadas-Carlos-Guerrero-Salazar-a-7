const char* tramas[] = {
    "L,H",
    "L,O", 
    "L,L",
    "M,2",
    "L,A",
    "L, ",
    "L,W",
    "M,-2",
    "L,O",
    "L,R",
    "L,L",
    "L,D",
    "FIN"     // Señal de fin
};

const int numTramas = 13;

void setup() {
    Serial.begin(9600);
    
    // Esperar a que se abra el puerto serial
    while (!Serial) {
        ; // Esperar conexión
    }
    
    // Pequeña pausa para asegurar la conexión
    delay(2000);
    
    Serial.println("[ARDUINO] Transmisor PRT-7 iniciado");
    Serial.println("[ARDUINO] Enviando tramas...");
    delay(1000);
}

void loop() {
    // Enviar todas las tramas
    for (int i = 0; i < numTramas; i++) {
        Serial.println(tramas[i]);
        delay(1000); // Esperar 1 segundo entre tramas
    }
}