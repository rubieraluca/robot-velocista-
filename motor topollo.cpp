class Motor {

  private:

    int _inA;
    int _inB;

    void avanzar(int velocidad) {
      analogWrite(_inA, velocidad);
      analogWrite(_inB, 0);
    }

    void retroceder(int velocidad) {
      analogWrite(_inA, 0);
      analogWrite(_inB, velocidad);

    }
    void detener() {
      analogWrite(_inA, 0);
      analogWrite(_inB, 0);
    }

  public:

    Motor(int inA, int inB) {

      _inA = inA;
      _inB = inB;

      pinMode(_inA, OUTPUT);
      pinMode(_inB, OUTPUT);
    }
    void mover(int velocidad) {

      if (velocidad > 0) {
        avanzar(velocidad);
      }
      else if (velocidad < 0) {
        retroceder(-velocidad);
      }

      else {
        detener();
      }

    } void info () {

      Serial.println("Pines asignados a los motores:");
      Serial.print("inA :");
      Serial.println(_inA);
      Serial.print("inB :");
      Serial.println(_inB);

    }
};

class Robot{
private:
Motor _motorIzq;
Motor _motorDer;
int velocidadBase;



}
public:

Robot(int inA, int inB, int inA2, int inB2){

_motorIzq = Motor(inA,inB);
_motorDer = Motor(inA2,inB2);

}


void mover(int velocidad, int velocidad2){
_motorIzq.mover(velocidad);
_motorDer.mover(velocidad2);
}

void info(){
  _motorIzq.info();
  _moorDer.info();

}
Robot robot = Robot(9,10,11,12);

void setup() {
  Serial.begin(9600);
  robot.info();
}
void loop() {

}
