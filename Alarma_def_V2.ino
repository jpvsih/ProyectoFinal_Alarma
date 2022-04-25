#include <LiquidCrystal.h>                                  //IMPORTAMOS LA LIBRERIA PARA CONTROLAR UN LCD
#define buzzer 5                                            //DEFINIMOS EL PIN AL QUE ESTA CONECTADO EL BUZZER DE LA ALARMA
#define tempPin A5                                          //DEFINIMOS EL PIN AL QUE ESTA CONECTADO EL THERMISTOR DE LA ALARMA
#define led 6                                               //DEFINIMOS EL PIN AL QUE ESTA CONECTADO EL LED DE LA ALARMA
#define check_led 3                                         //DEFINIMOS EL PIN AL QUE ESTA CONECTADO EL LED DE CONEXION DE LA ALARMA

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);                     //DEFINIMOS PINES DE CONEXION DE LA PANTALLA LCD


void setup() {
  Serial.begin(57600);                                      //INICIALIZAMOS EL SERIAL MONITOR PARA PERMITIR COMUNICACION CON LA PLACA
  pinMode(check_led, OUTPUT);                               //DEFINIMOS LOS DISTINTOS PINES COMO SALIDA
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
}

void loop() {
  lcd.begin(16, 2);                                         //INICIALIZAMOS PANTALLA LCD
  float temp, tc;                                           //DEFINIMOS VARIABLES FLOAT PARA EL CALCULO DE TEMPERATURAS
  int chk, alarm, tround;                                   //DEFINIMOS LAS VARIABLES INT DEL PROGRAMA
  chk = 0;                                                  //ASIGNAMOS VALORES INICIALES
  alarm = 0;
  String verify = "connect";                                //DEFINIMOS LAS VARIABLES STRING PARA EL USO DE COMANDOS DESDE SERIAL
  String bzz = "alarm test";                                //MONITOR Y ASIGNAMOS LOS COMANDOS CORRESPONDIENTES
  String aloff = "off";
  String tread = "temperature";
  
  while(Serial.available()==0);                             //PUEDE SER PRESCINDIBLE, HAY QUE PROBAR
    String check = Serial.readString();                     //COMPROBAMOS INPUT DEL SERIAL MONITOR
    
  if ( verify == check){                                    //COMPROBAMOS SI EL INPUT SE CORRESPONDE CON connect 
    chk = 1;                                                //SI COINCIDEN, chk TOMA VALOR 1
    lcd.print("Connected");                                 //SE IMPRIME MENSAJE DE CONFIRMACION EN EL LCD
    Serial.print("Connected successfully");                 //SE IMPRIME MENSAJE DE CONFIRMACION EN EL SERIAL MONITOR
    Serial.print("\n");
    delay(2000);
    lcd.clear();                                            //SE LIMPIA EL OUTPUT DEL LCD
    
    while (chk == 1){                                       //MIENTRAS chk SEA 1, LA ALARMA ESTA CONECTADA AL SISTEMA Y SE EJECUTA
      digitalWrite(check_led,HIGH);                         //SE ENCIENDE EL LED VERDE DE CONFIRMACIÓN DE CONEXION
      String check = Serial.readString();                   //SE COMPRUEBA EL INPUT DEL SERIAL MONITOR
      temp = analogRead(tempPin);                           //LEEMOS EL VALOR DEL THERMISTOR temp VA DE 0-1023
      tc = (79.319)-(0.1064)*temp ;                         //CONVERTIMOS DICHO VALOR A CELSIUS
      
      if (tread == check){                                  //MODULO PARA MOSTRAR LA TEMPERATURA CUANDO ESCRIBES "temperatura"
        Serial.print("Temperature: ");                      //EN EL SERIAL MONITOR
        Serial.print(tc);
        Serial.print("\n");
      }                                                     //MOSTRAMOS TEMPERATURA EN TIEMPO REAL
      lcd.setCursor(0,0);                                   //COLOCAMOS EL CURSOR EN EL LCD
      lcd.print("TEMPERATURE: ");                           //COMENZAMOS A IMPRIMIR EN LA PRIMERA FILA DEL LCD
      lcd.setCursor(0,1);                                   //COLOCAMOS EL CURSOR EN EL LCD
      lcd.print(tc);                                        //IMPRIMIMOS LA TEMPERATURA EN TIEMPO REAL EN LA SEGUNDA FILA DEL LCD
      
//MODULO PARA COMPROBAR EL CORRECTO FUNCIONAMIENTO DE LAS SEÑALES LUMINOSAS Y ACUSTICAS DE LA ALARMA USANDO SERIAL MONITOR

      if (bzz == check){                                    //SE COMPRUEBA SI EL VALOR DE check COINCIDE CON "alarm test"
        alarm = 1;                                          //DE SER ASI SE CAMBIA EL VALOR DE alarm PARA ENTRAR EN while
        
        while (alarm == 1){                                 //MIENTRAS alarm TOME VALOR 1 SE PERMANECERA EN EL MODO DE alarm test
          lcd.begin(16, 2);                                 //SE INICIALIZA LA PANTALLA LCD
          Serial.print("ALARM CHECK");                      //IMPRIMIMOS "ALARM CHECK" EN EL SERIAL MONITOR PARA INFORMAR AL USUARIO
          Serial.print("\n");
          lcd.print("WARNING");                             //IMPRIMIMOS TAMBIEN "WARNING" EN EL LCD DE LA ALARMA
          digitalWrite(led, HIGH);                          //COMPROBACION DE SEÑAL LUMINOSA
          tone(buzzer, 700, 500);                           //COMPROBACION DE SEÑAL ACUSTICA
          delay(500);
          digitalWrite(led, HIGH);
          delay(500);
          String check = Serial.readString();               //COMPROBAMOS INPUT DEL SERIAL MONITOR
          
          if (aloff == check){                              //SI DICHO INPUT COINCIDE CON "off"
            alarm = 0;                                      //alarm TOMA VALOR 0 Y SE SALE DEL while
            lcd.clear();                                    //SE LIMPIA EL OUTPUT DEL LCD
            }
          }
        }
        
//EL SIGUIENTE MODULO SE REPITE PARA LOS DISTINTOS VALORES DE temp (se comenta en detalle el primero de los modulos)

        while(temp >= 463 &&  temp < 511){                  //TEMPERATURA ENTRE 25 Y 30 GRADOS CELSIUS
          String check = Serial.readString();               //DEFINIMOS check PARA LECTURA DE COMANDOS DESDE SERIAL MONITOR
          temp = analogRead(tempPin);                       //LEEMOS EL VALOR DEL THERMISTOR temp VA DE 0-1023
          tc = (79.319)-(0.1064)*temp ;                     //CONVERTIMOS DICHO VALOR A CELSIUS
          
          if (tread == check){                              //MODULO PARA MOSTRAR LA TEMPERATURA CUANDO ESCRIBES "temperatura"
            Serial.print("Temperature: ");                  //EN EL SERIAL MONITOR
            Serial.print(tc);
            Serial.print("\n");
          }
          Serial.print("CAUTION, TEMPERATURE OVER 25ºC");   //ADVERTENCIA DE QUE SE HA SOBREPASADO EL LIMITE DE TEMPERATURA
          Serial.print("\n");
                                                            //MOSTRAMOS TEMPERATURA EN TIEMPO REAL
          lcd.setCursor(0,0);                               //COLOCAMOS EL CURSOR EN EL LCD
          lcd.print("TEMPERATURE: ");                       //COMENZAMOS A IMPRIMIR EN LA PRIMERA FILA DEL LCD
          lcd.setCursor(0,1);                               //COLOCAMOS EL CURSOR EN EL LCD
          lcd.print(tc);                                    //IMPRIMIMOS LA TEMPERATURA EN TIEMPO REAL EN LA SEGUNDA FILA DEL LCD
          
          digitalWrite(led, HIGH);                          //SEÑAL LUMINOSA DE LED ROJO
          delay(100);                                       //EN LOS SIGUIENTES MODULOS SE ACOMPAÑA DE UNA SEÑAL ACUSTICA
          digitalWrite(led, LOW);
          delay(500);
        }
        
        while(temp >= 417 &&  temp < 463){                  //TEMPERATURA POR ENCIMA DE 35ºC
          String check = Serial.readString();               
          temp = analogRead(tempPin);                       
          tc = (79.319)-(0.1064)*temp ;                     
          
          if (tread == check){                              
            Serial.print("Temperature: ");                  
            Serial.print(tc);                               
            Serial.print("\n");                             
          }
          Serial.print("WARNING TEMPERATURE, OVER 30ºC");   
          Serial.print("\n");
          
          lcd.setCursor(0,0);                               
          lcd.print("TEMPERATURE: ");                       
          lcd.setCursor(0,1);
          lcd.print(tc);
          
          digitalWrite(led, HIGH);                            
          tone(buzzer, 700, 300);                           //SEÑAL ACUSTICA A MISMA FRECUENCIA QUE LED
          delay(300);
          digitalWrite(led, LOW);
          delay(300);
        
      }
      while(temp < 417){                                    //TEMPERATURA POR ENCIMA DE 35ºC
          String check = Serial.readString();               
          temp = analogRead(tempPin);                       
          tc = (79.319)-(0.1064)*temp ;                     

          if (tread == check){                              
            Serial.print("Temperature: ");                  
            Serial.print(tc);
            Serial.print("\n");
          }
          Serial.print("CRITICAL TEMPERATURE, OVER 35ºC");  
          Serial.print("\n");
          
          lcd.setCursor(0,0);                               
          lcd.print("TEMPERATURE: ");                       
          lcd.setCursor(0,1);
          lcd.print(tc);
          
          digitalWrite(led, HIGH);                            
          tone(buzzer, 1400, 150);                          //SEÑAL ACUSTICA A MISMA FRECUENCIA QUE LED
          delay(150);
          digitalWrite(led, LOW);
          delay(150);
        
      }
    }
  }
  
  else{                                                     //EN CASO DE NO INTRODUCIR EL COMANDO connect ADECUADAMENTE
    lcd.print("ERROR CONNECTING");                          //SE MUESTRA UN ERROR EN PANTALLA LCD
    Serial.print("Connection unsuccessful");                //SE MUESTRA UN MENSAJE DE ERROR EN SERIAL MONITOR
    Serial.print("\n");
    delay(2000);
    lcd.clear();                                            //SE LIMPIA EL OUTPUT DEL LCD

  }
}
