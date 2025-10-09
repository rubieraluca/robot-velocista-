# 🤖 El Velocista – El Topollo

> Robot seguidor de línea que busca completar circuitos a altas velocidades, con un diseño mecanico y electronico.


## 🎯 Visión general

El proyecto "El topollo" consiste  diseñar y construir un robot velocista capaz de:

- Seguir una línea negra trazada sobre una pista con precisión y rapidez  
- Optimizar el rendimiento mediante criterios aerodinámicos, estructurales y de control    
- Superar velocidades como referencia: en proyectos similares se han alcanzado hasta *2,23 m/s*  

Este robot combina tres áreas clave:

| Módulo | Descripción |
|-------|-------------|
| Mecánica / estructura | Chasis ligero (por ejemplo, placas PCB ajustables)
| Electrónica / sensores | Uso de sensores infrarrojos (CNY70 u otros) para detección de la línea
| Control / software | Algoritmo PID, filtros (Kalman o similares) y control de motores independientes.


## 📌 Datos técnicos de referencia

- Proyecto *Diseño e Implementación de un Robot Velocista sobre FPGA* alcanzó velocidad máxima de 2,23 m/s 
- En el proyecto “el topollo" se usaron placas PCB como estructura, sensores CNY70, regulación con LM317 para control de motores 
- En tutorial “Robot Velocista Lycan” se propone usar Arduino Nano, motores DC, driver TA6586 y batería LiPo para una versión educativa del velocista   
- Guía de diseño señala que para lograr mayores velocidades se debe optimizar la relación peso-potencia, el coeficiente de fricción de las llantas, la velocidad de los motores y el algoritmo de control 


