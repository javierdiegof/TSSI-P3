/*
  This program blinks pin 13 of the Arduino (the
  built-in LED)
*/

int ena1 = 2;
int ena2 = 3;
int dir1 = 5;
int dir2 = 6;

//String lec_motor_actual;
//String lec_stat_actual;
//String lec_dir_actual;

int motor_actual = -1;
int stat_actual = -1;
int dir_actual = -1;
int check = -1;

int DIR_DERECHA   = 1;
int DIR_IZQUIERDA = 0;

int PRENDIDO   = 1;
int APAGADO    = 0;
int ERROR      = -1;  

String lectura;
void setup()
{
  Serial.begin(9600);
  pinMode(ena1, OUTPUT);
  pinMode(ena2, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);
}

void loop(){
   if(Serial.available() > 0){
        lectura = Serial.readString();
      //lec_motor_actual = lectura.substring(0,2);
      //lec_stat_actual  = lectura.substring(3,6);
    
      //Serial.print("stat:" + lec_stat_actual + "\n");
  
      // Procesamiento del motor a escoger
      motor_actual = motor_actualf(lectura);
    
      // Procesamiento del status del motor (on/off)
      stat_actual = stat_actualf(lectura);
    
      // En caso de on, procesamiento de la dirección de 
      dir_actual = dir_actualf(lectura, stat_actual);

      Serial.print("valores: " + String(motor_actual) + String(stat_actual) + String(dir_actual) + "\n");
      // Checando que todos los datos se hayan insertado correctamente
      check = checar(motor_actual, stat_actual, dir_actual);

      // Si el chequeo es incorrecto, mostramos error al usuario
      if(check == -1){
         Serial.print("Error de sintaxis!\n");
      }
      // Operaciones correctas prendemos las salidas del Arduino
      else if (check == 1){
         salida_motor(motor_actual, stat_actual, dir_actual);
      }
  }
}


int motor_actualf(String lectura){
    String lec_motor_actual;
    lec_motor_actual = lectura.substring(0,2);
    if(lec_motor_actual.substring(0,1).equals("A")){
      return lec_motor_actual.substring(1).toInt() + 1; // Incremento para pin
    }
    return ERROR;
}

int stat_actualf(String lectura){
    String lec_stat_actual;
    lec_stat_actual  = lectura.substring(3,6);
    if(lec_stat_actual.equals("on ")){
      return PRENDIDO;  
    }
    else if(lec_stat_actual.equals("off")){
      return APAGADO;  
    }
    return ERROR;
}

int dir_actualf(String lectura, int stat_actual){
  String lec_dir_actual;
  if(stat_actual == PRENDIDO){
      lec_dir_actual = lectura.substring(6);
        //Serial.print("Direccion actual: " + lec_dir_actual + "\n");
        if(lec_dir_actual.equals("left")){
          return DIR_IZQUIERDA;
        }
        else if(lec_dir_actual.equals("right")){
          return DIR_DERECHA; 
        }
   }
   else if(stat_actual == APAGADO){
      if(lectura.substring(6).equals("")){ // Checando que no se pongan más cosas
         return APAGADO;
      }
   }
   return -ERROR;
}

int checar(int motor_actual, int stat_actual, int dir_actual){
   if(((motor_actual != 2) && (motor_actual != 3)) || (stat_actual == -1) || (dir_actual == -1)){
      return -ERROR;
   }
   return 1;
}

int salida_motor(int motor_actual, int stat_actual, int dir_actual){
   for(int i = 0; i < 500; i++){
      // Motor prendido
     if(stat_actual == PRENDIDO){
        digitalWrite(motor_actual, PRENDIDO);
        digitalWrite(motor_actual+3, dir_actual);
      }
      else{
         digitalWrite(motor_actual, APAGADO);
      }
   }
}
