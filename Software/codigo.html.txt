#include <Arduino.h>

// --- DEFINICIONES GLOBALES (ANTES DE LAS CLASES) ---
// Para que NUM_SENSORES sea conocido en la clase Robot_Velocista
#define NUM_SENSORES 8

// CLASE MOTOR

class Motor
{
private:
  int _inA;
  int _inB;

public:
  Motor(int inA, int inB) : _inA(inA), _inB(inB)
  {
    // Es mejor usar ledcAttachPin() para asociar el pin con un canal PWM en ESP32
    // Pero para simplificar y mantener la lógica original, asumiremos que
    // ledcWrite() está manejando implícitamente la configuración si los pines son PWM.
    // Sin embargo, en un entorno ESP32 real, deberías usar ledcSetup y ledcAttachPin
    pinMode(_inA, OUTPUT);
    pinMode(_inB, OUTPUT);
    // Para simplificar, asumimos que ledcWrite() es la forma correcta para mover
    // el motor con PWM en ESP32 para este ejemplo.
  }

  void mover(int velocidad)
  {
    // Limitar velocidad entre -255 y 255
    velocidad = constrain(velocidad, -255, 255);

    if (velocidad > 0)
    {
      // Adelante (Motor A en HIGH/PWM, Motor B en LOW/0)
      ledcWrite(_inA, velocidad);
      ledcWrite(_inB, 0);
    }
    else if (velocidad < 0)
    {
      // Atrás (Motor A en LOW/0, Motor B en HIGH/PWM)
      // Usamos el valor absoluto de la velocidad para el PWM
      ledcWrite(_inA, 0);
      ledcWrite(_inB, -velocidad);
    }
    else
    {
      // Detener
      ledcWrite(_inA, 0);
      ledcWrite(_inB, 0);
    }
  }

  void detener()
  {
    ledcWrite(_inA, 0);
    ledcWrite(_inB, 0);
  }
};

// CLASE ROBOT (VELOCISTA)

class Robot_Velocista
{
private:
  Motor _motorIzq;
  Motor _motorDer;
  int _velocidadBase;
  // El array de pines debe ser almacenado como un puntero o un array estático
  const int* _pinesSensores; 
  int _numSensores;

  // PID
  float _Kp; // Añadir el prefijo '_' para variables miembro
  float _Ki;
  float _Kd;

  // Integral (falta en tu código original, necesaria para Ki)
  float _integral = 0.0;

  // UMBRAL y ERRORES
  int _umbral; // Añadir el prefijo '_'
  // int _error = 0; // Se calcula en seguirLinea(), no es necesario guardarlo como miembro
  int _ultimoError = 0; // Añadir el prefijo '_'

public:
  // Constructor: Se modifican los parámetros y la lista de inicialización
  Robot_Velocista(int inA_izq, int inB_izq, int inA_der, int inB_der,
                  const int pinesSensores[], // Recibe el array como puntero
                  int numSensores,
                  int velocidadBase = 150,
                  float Kp = 0.15, float Ki = 0.0, float Kd = 0.8,
                  int umbral = 500)

      // Lista de inicialización corregida: se usan los prefijos '_'
      : _motorIzq(inA_izq, inB_izq),
        _motorDer(inA_der, inB_der),
        _velocidadBase(velocidadBase),
        _pinesSensores(pinesSensores), // Inicializa el puntero con el puntero recibido
        _numSensores(numSensores),
        _Kp(Kp), _Ki(Ki), _Kd(Kd),
        // No inicializamos _ultimoError en la lista, ya tiene valor por defecto
        _umbral(umbral)
  {
    // En el constructor: configuramos los pines como INPUT
    // Ya que _pinesSensores ahora apunta al array pinesSensores global/pasado,
    // usamos ese puntero para configurar los pines.
    for (int i = 0; i < _numSensores; i++)
    {
      // pinMode() necesita el valor del pin, que está en la posición i del array
      pinMode(_pinesSensores[i], INPUT); 
    }
    
    // La función 'setUmbral' estaba definida DENTRO del constructor, lo que es un error
    // Se ha movido fuera del constructor como un método público.
  }
  
  // --- MÉTODOS PÚBLICOS ---

  void setUmbral(int u)
  {
    _umbral = constrain(u, 0, 1023);
  }

  void setVelocidadBase(int vel)
  {
    _velocidadBase = constrain(vel, 0, 255);
  }

  // Mover con velocidades independientes
  void mover(int velIzq, int velDer)
  {
    _motorIzq.mover(velIzq);
    _motorDer.mover(velDer);
  }

  // Avanzar recto
  void avanzar()
  {
    mover(_velocidadBase, _velocidadBase);
  }

  // Girar con corrección
  void girar(int correccion)
  {
    int velIzq = _velocidadBase + correccion;
    int velDer = _velocidadBase - correccion;
    mover(velIzq, velDer);
  }

  void detener()
  {
    _motorIzq.detener();
    _motorDer.detener();
  }

  int leerLinea()
  {
    // NUM_SENSORES está definido como macro al inicio del archivo
    int valores[NUM_SENSORES];
    long suma = 0; // Usar long para evitar desbordamiento en sumas grandes
    long sumaPonderada = 0;

    // Leer sensores
    for (int i = 0; i < NUM_SENSORES; i++)
    {
      // Usamos el puntero a los pines miembro _pinesSensores
      // Leemos los pines como digitales (0 o 1). Si fueran analógicos, sería analogRead().
      valores[i] = digitalRead(_pinesSensores[i]); 
      suma += valores[i];
      sumaPonderada += (long)valores[i] * i * 1000;
    }

    // Calcular posición (-3500 a 3500 para 8 sensores, si se usa 0-7 como peso)
    if (suma == 0)
    {
      return _ultimoError; // Usar _ultimoError (variable miembro)
    }

    // Posición centrada es: (NUM_SENSORES - 1) * 1000 / 2 = 7 * 1000 / 2 = 3500
    // La posición retornada será (sumaPonderada / suma) - 3500
    int posicion = (int)(sumaPonderada / suma) - ((NUM_SENSORES - 1) * 500); 
    return posicion;
  }

  void seguirLinea()
  {
    int posicion = leerLinea();
    int error = posicion; // El error es la posición (0 = centrado)

    // Cálculo PID
    float proporcional = error;

    // Cálculo de la integral (si se usa Ki)
    _integral += error; 

    float derivativo = error - _ultimoError; // Usar _ultimoError

    // Se usa float para el cálculo y luego se convierte a int para la corrección
    int correccion = (int)((_Kp * proporcional) + (_Ki * _integral) + (_Kd * derivativo));

    int velIzq = _velocidadBase + correccion;
    int velDer = _velocidadBase - correccion;

    _motorIzq.mover(velIzq);
    _motorDer.mover(velDer);

    _ultimoError = error; // Guardar el error actual para la próxima iteración
  }

  void info()
  {
    Serial.println("=== CONFIGURACIÓN ROBOT ===");
    Serial.print("Velocidad base: ");
    Serial.println(_velocidadBase);
    Serial.print("Kp: ");
    Serial.println(_Kp); // Usar _Kp (variable miembro)
    Serial.print("Ki: ");
    Serial.println(_Ki); // Usar _Ki (variable miembro)
    Serial.print("Kd: ");
    Serial.println(_Kd); // Usar _Kd (variable miembro)
    Serial.print("Umbral: ");
    Serial.println(_umbral); // Usar _umbral (variable miembro)
    Serial.print("Numero de Sensores: ");
    Serial.println(_numSensores);
  }
};

// CONFIGURACIÓN DE PINES

#define MOTOR_IZQ_A 17
#define MOTOR_IZQ_B 18
#define MOTOR_DER_A 16
#define MOTOR_DER_B 4

// Pines del sensor QTR
const int pinesSensores[NUM_SENSORES] = {34, 35, 32, 33, 25, 26, 28, 14}; 
// NUM_SENSORES debe estar definido ANTES de su uso aquí

Robot_Velocista robot(MOTOR_IZQ_A, MOTOR_IZQ_B, MOTOR_DER_A, MOTOR_DER_B, 
                      pinesSensores, 
                      NUM_SENSORES, 
                      150);

void setup()
{
  Serial.begin(9600);
  robot.info();
  Serial.println("Robot listo. Esperando 2 segundos...");
  delay(2000);
}

void loop()
{
  robot.seguirLinea(); // 'seguirLinea' es un método de la instancia 'robot'
}
