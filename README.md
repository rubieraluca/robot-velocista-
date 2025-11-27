# 🤖 El Velocista – El Topollobot

INTEGRANTES:
Alfredo Sánchez 
Luca Rubiera
Damian Robledo
Thiago Amaya
Limachi Rubiera

Fecha: 09/10/25

> Robot seguidor de línea que busca completar circuitos a altas velocidades, con un diseño mecanico y electronico.


## 🎯 Visión general y Objetivos

El proyecto "El topollo" consiste  diseñar y construir un robot velocista capaz de:

- Seguir una línea negra trazada sobre una pista con precisión y rapidez  
- Optimizar el rendimiento mediante criterios aerodinámicos, estructurales y de control    
- Superar velocidades como referencia: en proyectos similares se han alcanzado hasta *2,23 m/s*  

Este robot combina tres áreas clave:

| Módulo | Descripción |
|-------|-------------|
| Mecánica / estructura | Chasis ligero (por ejemplo, placas PCB ajustables)
| Electrónica / sensores | Uso de sensores QRE1113 (QTR8A) para detección de la línea
| Control / software | Algoritmo PID


## 📌 Datos técnicos de referencia


- Nuestro proyecto "El Topollobot" quiere alcanzar una velocidad máxima de 2,23 m/s 

- En el proyecto “el topollo" se usaron placas PCB como estructura, sensores QTR8A, regulación con TA6586 para control de motores

- Guía de diseño señala que para lograr mayores velocidades se debe optimizar la relación peso-potencia, el coeficiente de fricción de las llantas, la velocidad de los motores y el algoritmo de control 



Componentes Utilizados

-Arduino Nano
-TA6586
-QTR8A
-Capacitores de 100nF y 470uF
-Resistencias de 1k y 10k 
-Motores POLOLU de 2000 RPM
-Placa de cobre 10x10
-Percloruro
-Led
-Soldador
-Pineras macho y hembra
-ETC

DESCRIPCIONES DE QUE HIZO CADA UNO:

Alfredo Sanchez: AYUDO A AMAYA CON EL PCB Y EL ESQUEMATICO

Thiago Amaya: ESQUEMATICO Y PCB

DAMIAN Robledo: SOLDADURA Y ARMADO DE LA PLACA, DISEÑO LOGISTICO DEL ROBOT

Luca Rubiera: SOLDADURA Y ARMADO DE LA PLACA, HIZO EL 3D

Limachi Lautaro: Soldaduras, codigo y montaje de la placa











<img width="668" height="536" alt="image" src="https://github.com/user-attachments/assets/e52d0821-a702-4e8b-bea2-362a55beacd8" />












<img width="940" height="533" alt="image" src="https://github.com/user-attachments/assets/78c224ac-41c8-41b2-a2e9-bd4ba3b0d4bd" />







PASOS PARA ARMAR EL PROYECTO

1)Armado del esquematico y PCB

2)Comprar componentes segun necesidades y analizar cual comprar

3)imprimir el PCB y plancharlo en la placa y sacarle el papel al enfriarse

4)Tirar la placa en percloruro y sacarlo hasta que se coma el cobre

5)Hacer los agujeros en los lugares correspondientes

6)Probar que los componentes anden

7)Empezamos a montar los componentes en la placa soldandolos 

8)Hacer el 3D de la carroceria del robot

9)Hacer el codigo para el funcionamiento del velocista

10)Poner la placa en la carroceria

11)Cargar el codigo en el arduino

12)Probar que funcione

13)Fin


Herramientas necesarias


-pinza y alicate

-chupa estaño

-soldador de estaño

-estaño

-percloruro

-birulana

-agua

-las manos y cabeza

-computadora 

-impresora y papel

-plancha


-agujeriadora y mechas 

-destornilladores 

-impresora 3d 

-flux

Precauciones de seguridad 


-no estas boludiando

-no tocar acido

-estar atentos

-no ser down 
y vas a andar bien 


soldamos con lapiz soldador 
y las manos

datasheet en carpeta "los topollos"






Codigo del velocista

#include <Arduino.h>

// CLASE MOTOR

class Motor
{
private:
  int _inA;
  int _inB;

public:
  Motor(int inA, int inB) : _inA(inA), _inB(inB)
  {
    pinMode(_inA, OUTPUT);
    pinMode(_inB, OUTPUT);
  }

  void mover(int velocidad)
  {
    // Limitar velocidad entre -255 y 255
    velocidad = constrain(velocidad, -255, 255);

    if (velocidad > 0)
    {
      ledcWrite(_inA, velocidad);
      ledcWrite(_inB, 0);
    }
    else if (velocidad < 0)
    {
      ledcWrite(_inA, 0);
      ledcWrite(_inB, -velocidad);
    }
    else
    {
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
  int _pinesSensores;
  int _numSensores;
  // PID
  float Kp = 0.15;
  float Ki = 0.0;
  float Kd = 0.8;
  // UMBRAL
  int umbral;
  int error = 0;
  int ultimoError = 0;

public:
  Robot_Velocista(int inA_izq, int inB_izq, int inA_der, int inB_der,
                  int const pinesSensores[8],
                  int numSensores, int velocidadBase = 150,
                  float Kp = 0.15, float Ki = 0.0, float Kd = 0.8,
                  int umbral = 500)

      : _motorIzq(inA_izq, inB_izq),
        _motorDer(inA_der, inB_der),
        _velocidadBase(velocidadBase),
        _pinesSensores(pinesSensores),
        _numSensores(numSensores),
        _Kp(Kp), _Ki(Ki), _Kd(Kd),
        _error(error),
        _ultimoError(ultimoError),
        _umbral(umbral)
  {
    for (int i = 0; i < _numSensores; i++)
    {
      _pinesSensores[i] = pinesSensores[i];
      pinMode(_pinesSensores[i], INPUT);
    }

    void setUmbral(int u)
    {
      _umbral = constrain(u, 0, 1023);
    }
  }
  //////////////////////////////////////////////////
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
    int valores[NUM_SENSORES];
    int suma = 0;
    int sumaPonderada = 0;

    // Leer sensores
    for (int i = 0; i < NUM_SENSORES; i++)
    {
      valores[i] = digitalRead(pinesSensores[i]);
      suma += valores[i];
      sumaPonderada += valores[i] * i * 1000;
    }

    // Calcular posición (-3500 a 3500 para 8 sensores)
    if (suma == 0)
    {
      return ultimoError; // Mantener último error si no detecta línea
    }

    int posicion = (sumaPonderada / suma) - ((NUM_SENSORES - 1) * 500);
    return posicion;
  }

  void seguirLinea()
  {
    int posicion = leerLinea();
    int error = posicion; // El error es la posición (0 = centrado)

    // Cálculo PID
    int proporcional = error;
    int derivativo = error - _ultimoError;

    int correccion = (Kp * proporcional) + (Ki * _integral) + (Kd * derivativo);

    int velIzq = _velocidadBase + correccion;
    int velDer = _velocidadBase - correccion;

    _motorIzq.mover(velIzq);
    _motorDer.mover(velDer);
    // Aplicar corrección
    // robot.girar(correccion);

    ultimoError = error;
  }

  void info()
  {
    Serial.println("=== CONFIGURACIÓN ROBOT ===");
    Serial.print("Velocidad base: ");
    Serial.println(_velocidadBase);
    Serial.print("Kp: ");
    Serial.println(_Kp);
    Serial.print("Ki: ");
    Serial.println(_Ki);
    Serial.print("Kd: ");
    Serial.println(_Kd);
    Serial.print("Umbral: ");
    Serial.println(_umbral);
  }
};

// CONFIGURACIÓN DE PINES

#define MOTOR_IZQ_A 17
#define MOTOR_IZQ_B 18
#define MOTOR_DER_A 16
#define MOTOR_DER_B 4

// Pines del sensor QTR
#define NUM_SENSORES 8
const int pinesSensores[NUM_SENSORES] = {2, 3, 4, 5, 6, 7, 8, A0};

// VARIABLES GLOBALES

Robot Robot_Velocista(MOTOR_IZQ_A, MOTOR_IZQ_B, MOTOR_DER_A, MOTOR_DER_B, &pinesSensores, 150);

void setup()
{
  Serial.begin(9600);
  robot.info();
  Serial.println("Robot listo. Esperando 2 segundos...");


  
  delay(2000);
}

void loop()
{
  seguirLinea();
}
