#include <Servo.h>

Servo servoHorizontal;  // Servo para el eje horizontal
Servo servoVertical;    // Servo para el eje vertical
int posHorizontal = 45;  // Posición inicial del servo horizontal (centro)
int posVertical = 0;    // Posición inicial del servo vertical (centro)

const int joyX = A2;    // Eje X del joystick
const int joyY = A3;    // Eje Y del joystick
const int servoPinHorizontal = 4;
const int servoPinVertical = 5;

int speedHorizontal = 1; // Velocidad de rotación (grados por paso)
int speedVertical = 4;   // Velocidad de rotación (grados por paso)
unsigned long lastUpdateTime = 0; // Tiempo de la última actualización
const int updateInterval = 30;     // Intervalo de actualización en milisegundos

void setup() {
  servoHorizontal.attach(servoPinHorizontal);
  servoVertical.attach(servoPinVertical);
  // Inicializa los servos en la posición inicial
  servoHorizontal.write(posHorizontal);
  servoVertical.write(posVertical);
}

void loop() {
  int valX = analogRead(joyX); // Lee el valor del eje X
  int valY = analogRead(joyY); // Lee el valor del eje Y

  // Mapear los valores del joystick a los rangos del servo
  int mappedPosHorizontal = map(valX, 0, 1023, 0, 180);
  int mappedPosVertical = map(valY, 0, 1023, 0, 180);

  // Ajustar la velocidad en función de la diferencia de posición
  int diffHorizontal = abs(mappedPosHorizontal - posHorizontal);
  int diffVertical = abs(mappedPosVertical - posVertical);
  speedHorizontal = map(diffHorizontal, 0, 180, 1, 10); // Velocidad de 1 a 10 grados por paso
  speedVertical = map(diffVertical, 0, 180, 1, 10);     // Velocidad de 1 a 10 grados por paso

  // Verifica si es tiempo de actualizar la posición del servo
  if (millis() - lastUpdateTime >= updateInterval) {
    lastUpdateTime = millis(); // Actualiza el tiempo

    // Suavizar el movimiento del servo horizontal
    if (posHorizontal < mappedPosHorizontal) {
      posHorizontal += speedHorizontal;
      if (posHorizontal > mappedPosHorizontal) {
        posHorizontal = mappedPosHorizontal;
      }
    } else if (posHorizontal > mappedPosHorizontal) {
      posHorizontal -= speedHorizontal;
      if (posHorizontal < mappedPosHorizontal) {
        posHorizontal = mappedPosHorizontal;
      }
    }

    // Suavizar el movimiento del servo vertical
    if (posVertical < mappedPosVertical) {
      posVertical += speedVertical;
      if (posVertical > mappedPosVertical) {
        posVertical = mappedPosVertical;
      }
    } else if (posVertical > mappedPosVertical) {
      posVertical -= speedVertical;
      if (posVertical < mappedPosVertical) {
        posVertical = mappedPosVertical;
      }
    }

    // Limitar el rango de movimiento
    posHorizontal = constrain(posHorizontal, 0, 180);
    posVertical = constrain(posVertical, 0, 180);

    // Mover los servos según la posición del joystick
    servoHorizontal.write(posHorizontal);
    servoVertical.write(posVertical);
  }

  delay(15); // Pequeña pausa para estabilizar el movimiento
}
