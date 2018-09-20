int PIN_ENA1 = 2;
int PIN_ENA2 = 3;
int PIN_DIR1 = 5;
int PIN_DIR2 = 6;

//String lec_motor_actual;
//String lec_stat_actual;
//String lec_dir_actual;

int pin_enable = -1;
int pin_dir    = -1;
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
  pinMode(PIN_ENA1, OUTPUT);
  pinMode(PIN_ENA2, OUTPUT);
  pinMode(PIN_DIR1, OUTPUT);
  pinMode(PIN_DIR2, OUTPUT);
}

void loop(){
   if(Serial.available() > 0){
        lectura = Serial.readString();
      //lec_motor_actual = lectura.substring(0,2);
      //lec_stat_actual  = lectura.substring(3,6);
    
      //Serial.print("stat:" + lec_stat_actual + "\n");
  
      // Procesamiento del motor a escoger
      motor_actualf(lectura);
    
      // Procesamiento del status del motor (on/off)
      stat_actual = stat_actualf(lectura);
    
      // En caso de on, procesamiento de la dirección de 
      dir_actual = dir_actualf(lectura, stat_actual);

      Serial.println("Valores a la tarjeta: ");
      Serial.println("Pin_enable: " + String(pin_enable));
      Serial.println("Pin_direccion: " + String(pin_dir));
      Serial.println("Estatus: " + String(stat_actual));
      Serial.println("Direccion: " + String(dir_actual));
      
      // Checando que todos los datos se hayan insertado correctamente
      check = checar(pin_enable, pin_dir, stat_actual, dir_actual);

      // Si el chequeo es incorrecto, mostramos error al usuario
      if(check == -1){
         Serial.print("Error de sintaxis!\n");
      }
      // Operaciones correctas prendemos las salidas del Arduino
      else if (check == 1){
         salida_motor(pin_enable, pin_dir, stat_actual, dir_actual);
      }
  }
}


void motor_actualf(String lectura){
    char letra;
    char num;
    letra = lectura.charAt(0);
    num   = lectura.charAt(1);
    if(letra == 'A'){
      if(num == '1'){
        pin_enable = PIN_ENA1;
        pin_dir    = PIN_DIR1;
      }
      else if (num == '2'){
        pin_enable = PIN_ENA2;
        pin_dir    = PIN_DIR2;  
      }
    }
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

int checar(int pin_enable,  int pin_dir, int stat_actual, int dir_actual){
   if(((pin_enable != PIN_ENA1) && (pin_enable != PIN_ENA2)) || (stat_actual == -1) || (dir_actual == -1)){
      return -ERROR;
   }
   return 1;
}

int salida_motor(int pin_enable, int pin_dir, int stat_actual, int dir_actual){
   for(int i = 0; i < 500; i++){
      // Motor prendido
     if(stat_actual == PRENDIDO){
        digitalWrite(pin_enable, PRENDIDO);
        digitalWrite(pin_dir, dir_actual);
      }
      else{
        digitalWrite(pin_enable, APAGADO);
      }
   }
}
