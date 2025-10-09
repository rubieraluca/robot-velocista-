# 🤖 El Velocista – Robot Velocista de Competencia

> Robot autónomo seguidor de línea que busca completar circuitos a la máxima velocidad posible, integrando diseño mecánico, electrónico y algoritmos de control.

---

## 🎯 Visión general

El proyecto *El Velocista* consiste en diseñar y construir un robot velocista capaz de:

- Seguir una línea negra trazada sobre una pista con precisión y rapidez  
- Optimizar el rendimiento mediante criterios aerodinámicos, estructurales y de control  
- Participar en competencias de robótica (como Runibot, RoboManiacs, etc.)  
- Superar velocidades como referencia: en proyectos similares se han alcanzado hasta *2,23 m/s* con plataformas FPGA 0  

Este robot combina tres áreas clave:

| Módulo | Descripción |
|-------|-------------|
| Mecánica / estructura | Chasis ligero (por ejemplo, placas PCB ajustables) 1 |
| Electrónica / sensores | Uso de sensores infrarrojos (CNY70 u otros) para detección de la línea 2 |
| Control / software | Algoritmo PID, filtros (Kalman o similares) y control de motores independientes 3 |

---

## 📌 Datos técnicos de referencia

- Proyecto *Diseño e Implementación de un Robot Velocista sobre FPGA* alcanzó velocidad máxima de 2,23 m/s 4  
- En el proyecto “Robot Velocista” se usaron placas PCB como estructura, sensores CNY70, regulación con LM317 para control de motores 5  
- En tutorial “Robot Velocista Lycan” se propone usar Arduino Nano, motores DC, driver TB6612 y batería LiPo para una versión educativa del velocista 6  
- Guía de diseño señala que para lograr mayores velocidades se debe optimizar la relación peso-potencia, el coeficiente de fricción de las llantas, la velocidad de los motores y el algoritmo de control 7  

---

## 🧱 Estructura del repositorio sugerida

/ ├── docs/                # Información técnica, simulaciones, reglamentos ├── hardware/            # Placas, esquemáticos, modelos CAD ├── firmware/            # Código fuente del microcontrolador / FPGA ├── tests/               # Bancos de prueba y mediciones ├── README.md            # Este documento └── LICENSE
# robot-velocista-
robot seguidor de linea  velocista
