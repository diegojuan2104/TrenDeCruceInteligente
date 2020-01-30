#include <Servo.h>
Servo servo1;
Servo servo2;

#include<SoftwareSerial.h> 
String data="";
SoftwareSerial serialArd(51,50);//rx D8/tx D7 

//VARIABLES DE SENSORES
long duracion, distancia; 

int sinVal;
int toneVal; 

int tiempoDeCruce;

//iTERADOR DE SEMAFOROS 
int iterador;

//CONTADOR DE TIEMPO
int segundos;

//ESCENARIOS 
boolean trenPasando;
boolean enRojo;
boolean horaPico;
boolean enAmarillo,enAmarilloComp;
boolean horaPicoS1,horaPicoS2,horaPicoS3,horaPicoS4;

String flujoCalle1,flujoCalle2,flujoCalle3,flujoCalle4;
String flu1,flu2,flu3,flu4;


//PINES SEMAFORO 1
int verde1 = 2;
int amarillo1 = 3;
int rojo1 = 4;

//PINES SEMAFORO 2
int verde2 = 5;
int amarillo2 = 6;
int rojo2 = 7;

//PINES SEMAFORO 3
int verde3 = 8;
int amarillo3 = 9;
int rojo3 = 10;

//PINES SEMAFORO 4
int verde4 = 11;
int amarillo4 = 12;
int rojo4 = 13;

//PINES SEMAFORO 5;
int verde5678 = 30;
int rojo5678 =  31;

//PINES SEMAFORO 6;
int verde9101112 = 32;
int rojo9101112 =  33;

//PETICION DE PASO NORMAL S5
int pn5 = 22;
//PETICION DE PASO NORMAL S6
int pn6 = 23;
//PETICION DE PASO NORMAL S7
int pn7 = 24;
//PETICION DE PASO NORMAL S8
int pn8 =  25;
//PETICION DE PASO NORMAL S9
int pn9 = 26;
//PETICION DE PASO NORMAL S10
int pn10 = 27;
//PETICION DE PASO NORMAL S11
int pn11 = 28;
//PETICION DE PASO NORMAL S12
int pn12 = 29;

//SEMEFOROS DISCPACITADOS
int sd = 42;

//TIEMPOS SEMAFOROS 
int tiempoEnRojo1 = 3;
int tiempoEnRojo2 = 3;
int tiempoEnRojo3 = 3;
int tiempoEnRojo4 = 3;


int tiempoEnVerde1 = 6;
int tiempoEnVerde2 = 6;
int tiempoEnVerde3 = 6;
int tiempoEnVerde4 = 6;


#define PechoCalle1 40
#define PtrigCalle1 41
#define PechoCalle2 34
#define PtrigCalle2 35
#define PechoCalle3 38
#define PtrigCalle3 39
#define PechoCalle4 36
#define PtrigCalle4 37
#define PechoTren 47
#define PtrigTren 48

String strFireBase;

int contNuevo = 250;

int contCarrosCalle1 = 0;
int contCarrosCalle2 = 0;
int contCarrosCalle3 = 0;
int contCarrosCalle4 = 0;

int contCarrosHoraP1 = 0;
int contCarrosHoraP2 = 0;
int contCarrosHoraP3 = 0;
int contCarrosHoraP4 = 0;

int tiempoDeSemaforoA = tiempoEnVerde4;

//PETICIONES DE CRUCE DE CALLE
boolean peticionNormal;
boolean peticionEspecial;

boolean pasarCarro;

//DEFINICION DE PINES DE ENTRADA Y SALIDA
void entradasYSalidas(){
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);   
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(46,OUTPUT);
    
  pinMode(22,INPUT);  
  pinMode(23,INPUT);
  pinMode(24,INPUT);  
  pinMode(25,INPUT);
  pinMode(26,INPUT);  
  pinMode(27,INPUT);
  pinMode(28,INPUT);  
  pinMode(29,INPUT);
  pinMode(sd,INPUT);
  
  pinMode(30,OUTPUT);
  pinMode(31,OUTPUT);
  pinMode(32,OUTPUT);
  pinMode(33,OUTPUT);

  servo1.attach(44);
  servo2.attach(45);
  servo1.write(90);
  servo2.write(90);
  
  pinMode (PechoCalle1, INPUT);
  pinMode (PtrigCalle1, OUTPUT);
  pinMode (PechoCalle2, INPUT);
  pinMode (PtrigCalle2, OUTPUT);
  pinMode (PechoCalle3, INPUT);
  pinMode (PtrigCalle3, OUTPUT);
  pinMode (PechoCalle4, INPUT);
  pinMode (PtrigCalle4, OUTPUT);
  pinMode (PechoTren, INPUT);
  pinMode (PtrigTren, OUTPUT);
}
//CONDICIONES INICIALES DEL SISTEMA 
void inicioDelSistema(){
  stringFirebase(); 
  serialArd.begin(9600);
  //CONFIGURACION INICIAL DEL SISTEMA 
  digitalWrite(rojo1,HIGH);
  digitalWrite(rojo2,HIGH);
  digitalWrite(rojo3,HIGH);
  digitalWrite(verde4,HIGH);

  digitalWrite(verde5678,LOW);
  digitalWrite(verde9101112,LOW);
  digitalWrite(rojo5678,HIGH);
  digitalWrite(rojo9101112,HIGH);  

  //CONTADORES
  iterador = 0;
  segundos = 0;
  int tiempoDeCruce = 5;
  //CONDICIONES 
  trenPasando = false;
  peticionEspecial = false;
  peticionNormal = false;
  enRojo = false;
  enAmarillo = false;
  enAmarilloComp = false;

  flujoCalle1 = "NORMAL";
  flujoCalle2 = "NORMAL";
  flujoCalle3 = "NORMAL";
  flujoCalle4 = "NORMAL";

  flu1 = "N";
  flu2 = "N";
  flu3 = "N";
  flu4 = "N";
}

int contFireBase = 0;
int contDia = 0;
int contSegDia = 0;

void contarTiempo(){
    if(!trenPasando){
      if(contNuevo%250 == 0){
        detectarPeticion(); 
      }
      if(contNuevo == 500){
        contSegDia++;
        segundos++;
        contNuevo = 250;
      }else{
        contNuevo = contNuevo + 250;
      }
      contFireBase++;
      if(contFireBase == 4){
                
        contadorCarros();
        firebase();
        contadorCarros();
        
        contFireBase = 0;
      }

      if(contSegDia == 180){
        contDia++;  
        contSegDia = 0;
        contCarrosCalle1 = 0;
        contCarrosCalle2 = 0;
        contCarrosCalle3 = 0;
        contCarrosCalle4 = 0;
      }
      delay(250);
    }
}

//ME DETERMINA EL TIEMPO ESTABLECIDO DE CADA SEMAFORO 
int tiempoSemaforoActual(int iterador){
  int tiempo;
  if(iterador == 0){
    if(horaPicoS4){
      tiempoEnVerde4 = 15;
      horaPicoS4 = false;
      flujoCalle4 = "ELEVADO";
      flu4 = "P";
    }else if(contCarrosHoraP4 == 2){
      flujoCalle4 = "NORMAL";
      flu4 = "N";
    }else if(contCarrosHoraP4 < 2){
      flujoCalle4 = "EXCELENTE";
      tiempoEnVerde4 = 6;
      flu4 = "E";
    }
    contCarrosHoraP4 = 0;
    tiempo = tiempoEnVerde1;  
    
  } else if(iterador == 1){
      
     if(horaPicoS1){
      tiempoEnVerde1 = 15;
      horaPicoS1 = false;
      flujoCalle1 = "ELEVADO";
      flu1 = "P";
    }else if(contCarrosHoraP1 == 2){
      flujoCalle1 = "NORMAL";
      flu1 = "N";
    }else if(contCarrosHoraP1 < 2){
      flujoCalle1 = "EXCELENTE";
      tiempoEnVerde1 = 6;
      flu1 = "E";
    }
      tiempo = tiempoEnVerde2; 
      contCarrosHoraP1 = 0; 

  } if(iterador == 2){
     
     if(horaPicoS2){
      tiempoEnVerde2 = 15;
      horaPicoS2 = false;
      flujoCalle2 = "ELEVADO";
      flu2 = "P";
    }else if(contCarrosHoraP2 == 2){
      flujoCalle2 = "NORMAL";
      flu2 = "N";
    }else if(contCarrosHoraP2 < 2){
      flujoCalle2 = "EXCELENTE";
      tiempoEnVerde2 = 6;
      flu2 = "E";
    }
      tiempo = tiempoEnVerde3;
      contCarrosHoraP2 = 0;
  } if(iterador == 3){
     
    if(horaPicoS3){
      tiempoEnVerde3 = 15;
      horaPicoS3 = false;
      flujoCalle3 = "ELEVADO";
      flu3 = "P";
    }else if(contCarrosHoraP3 == 2){
      flujoCalle3 = "NORMAL";
      flu3 = "N";
    }else if(contCarrosHoraP3 < 2){
      flujoCalle3 = "EXCELENTE";
      tiempoEnVerde3 = 6;
      flu3 = "E";
    }
     contCarrosHoraP3 = 0;
     tiempo = tiempoEnVerde4;    
  } 
  stringFirebase();
  return tiempo;
}


//METODO MÁS IMPORTANTE DEL SISTEMA, ESTE CONTROLA TODOS LOS SEMAFOROS 
void cambiarSemaforo(){
  if(!trenPasando){  
    if(!peticionNormal && !peticionEspecial && !enRojo && tiempoDeSemaforoA == segundos){  

      if(enAmarilloComp){
        tiempoDeSemaforoA = tiempoSemaforoActual(iterador);
      }
      
      digitalWrite(verde5678,LOW);
      digitalWrite(verde9101112,LOW);
      digitalWrite(rojo5678,HIGH);
      digitalWrite(rojo9101112,HIGH);  
      if(iterador == 0){
        if(!enAmarilloComp){
           enAmarillo = true; 
           segundos = 0;
         }else{
          digitalWrite(verde1,HIGH);
          digitalWrite(rojo1,LOW); 
          iterador = iterador +1;
          segundos = 0;  
          enAmarilloComp = false;
         }
      }else if (iterador == 1){
        if(!enAmarilloComp){
           enAmarillo = true; 
           segundos = 0;
         }else{
          digitalWrite(verde2,HIGH); 
          digitalWrite(rojo2,LOW);
          iterador = iterador +1;
          segundos = 0;
          enAmarillo = false;
          enAmarilloComp = false;
         }
      }
      else if (iterador == 2){
        if(!enAmarilloComp){
           enAmarillo = true; 
           segundos = 0;
         }else{
          digitalWrite(verde3,HIGH); 
          digitalWrite(rojo3,LOW);
          segundos = 0;
          iterador = iterador +1;
          enAmarilloComp = false;
          enAmarillo = false;
         } 
      }
      else if (iterador == 3){
        if(!enAmarilloComp){
           enAmarillo = true; 
           segundos = 0;
        }else{
          digitalWrite(verde4,HIGH); 
          digitalWrite(rojo4,LOW);
          iterador = 0;
          segundos = 0; 
          enAmarilloComp = false;
          enAmarillo = false;
        }   
      }  
    }else{
    if((enRojo == false) && tiempoDeSemaforoA == segundos){
         if(!enAmarilloComp){
           enAmarillo = true; 
           segundos = 0;
         }else{ 
           if(peticionEspecial){
            tiempoDeCruce = 20;
           }else{
            tiempoDeCruce = 5;
           }
           estadoRojo(); 
         }
      }
    }   
    
    if(enRojo){
      if(segundos == tiempoDeCruce){
        enRojo = false; 
        peticionEspecial = false;
        peticionNormal = false;
        segundos = tiempoDeSemaforoA;
        enAmarilloComp = true;
      }
    }
    estadoAmarillo();  
  }
}

//ESTABLECE TODOS LOS SEMAFOROS EN ROJO
void estadoRojo(){
  
   digitalWrite(verde1,LOW); 
   digitalWrite(rojo1,HIGH);
   digitalWrite(verde2,LOW);
   digitalWrite(rojo2,HIGH);
   digitalWrite(verde3,LOW); 
   digitalWrite(rojo3,HIGH);
   digitalWrite(verde4,LOW);
   digitalWrite(rojo4,HIGH);

   digitalWrite(verde5678,HIGH);
   digitalWrite(verde9101112,HIGH);
   digitalWrite(rojo5678,LOW);
   digitalWrite(rojo9101112,LOW);  
   segundos = 0;
   enRojo = true;  
}

int tiempoAmarillo = 4;

//HACE LA TRANSICION DE VERDE A AMARILLO Y ROJO DEPENDIENDO DEL ITERADOR
void estadoAmarillo(){
 if((enAmarillo == true) && (!enAmarilloComp)){ 
  int var1;
  int var2;
  int var3;
  if(iterador == 1){
      digitalWrite(verde1,LOW);
      var1 = verde1;
      var2 = amarillo1;
      var3 = rojo1;
  }else if(iterador == 2){
      digitalWrite(verde2,LOW);
      var1 = verde2;
      var2 = amarillo2;
      var3 = rojo2;
  }else if(iterador == 3){
      digitalWrite(verde3,LOW);
      var1 = verde3;
      var2 = amarillo3;
      var3 = rojo3;
  }else if(iterador == 0){
      digitalWrite(verde4,LOW);                                                                                                                                                                                                                           
      var1 = verde4;
      var2 = amarillo4;
      var3 = rojo4;
  }
  if(segundos == 0){
    digitalWrite(var1,LOW); 
  }
  if(segundos == 1){
    digitalWrite(var1,HIGH); 
  }
  if(segundos == 2){
    digitalWrite(var1,LOW); 
    digitalWrite(var2,HIGH); 
  }
  if(segundos == 4){
    digitalWrite(var2,LOW); 
    digitalWrite(var3,HIGH); 
    enAmarilloComp = true;
    enAmarillo = false;
    segundos = tiempoDeSemaforoA;
    cambiarSemaforo();
  }
 }
}
void setup(){ 
  Serial.begin (9600);
  entradasYSalidas();
  inicioDelSistema();
}
//EMITE EL SONIDO CORRESPONDIENTE AL PASO DEL TREN
void sonar(){
  for(int x=0; x<180; x++){
    int sinVal = (sin(x*(3.1412/180)));
    int toneVal = 2000+(int(sinVal*1000));
    tone(46,toneVal);
    delay(3);
  }
}

//CONTADOR DEL PASO DEL TREN
int contTren = 0;

void accionDeTren(){
  if(trenPasando == true){
    sonar();
    int var1;
    int var2;
    int var3;
    contTren++;
    if(!enRojo){
      if(iterador!= 2){ 
        if(iterador == 1){
          var1 = verde1;
          var2 = amarillo1;
          var3 = rojo1;
        }else if(iterador == 3){
          var1 = verde3;
          var2 = amarillo3;
          var3 = rojo3;
        }else if(iterador == 0){
          var1 = verde4;
          var2 = amarillo4;
          var3 = rojo4;
        }
     if(contTren == 1){
          digitalWrite(var1,HIGH);   
      }
      if(contTren == 2){
          digitalWrite(var1,LOW);
           
      }
      if(contTren == 3){
          digitalWrite(var1,HIGH);   
      }
      if(contTren == 4){
          digitalWrite(var1,LOW);
          digitalWrite(var2,HIGH);
      }
     
      if(contTren == 8){
          digitalWrite(var2,LOW); 
          digitalWrite(var3,HIGH); 
          digitalWrite(rojo2,LOW);
          digitalWrite(verde2,HIGH);
          servo1.write(0);  
     }
     }else{
       digitalWrite(rojo2,LOW);
      digitalWrite(amarillo2,LOW);
      digitalWrite(verde2,HIGH);
      if(contTren == 8){
        servo1.write(0);
      }
     }
    }else{
      if(contTren == 5){
        digitalWrite(verde5678,LOW);
        digitalWrite(rojo5678,HIGH);
        digitalWrite(verde9101112,LOW);
        digitalWrite(rojo9101112,HIGH);
        digitalWrite(verde2,HIGH);
        digitalWrite(rojo2,LOW);
        digitalWrite(amarillo2,LOW);      
      }
      if(contTren == 8){
           servo1.write(0);
         }  
     
      }
        if(contTren == 13){
          servo2.write(180);
        }
        //Serial.print(contTren);
        if(contTren == 19){ 
           noTone(46);
           trenPasando = false;
           iterador = 2;
           digitalWrite(amarillo2,LOW);
           digitalWrite(rojo2,LOW);
           segundos = 5; 
           servo1.write(90);
           servo2.write(90);
           contTren = 0;
           enRojo = false;
           peticionNormal = false;
           peticionEspecial = false;
           enAmarilloComp = false;
           contSegDia = contSegDia + 15;
     } 
  }
}

void sensorTren(){
  digitalWrite(PtrigTren, LOW);
  delayMicroseconds(2);
  digitalWrite(PtrigTren, HIGH);
  delayMicroseconds(10);
  digitalWrite(PtrigTren, LOW);

  duracion = pulseIn(PechoTren, HIGH);
  distancia = (duracion/2)/29;
  //Serial.print(" ");
  if (distancia < 5)
  { 
     trenPasando = true;
  }  
}

void contarCarros(int Ptrig,int Pecho, int contCarros){ 
  digitalWrite(Ptrig, LOW);
  delayMicroseconds(2);
  digitalWrite(Ptrig, HIGH);   // genera el pulso de triger por 10ms
  delayMicroseconds(10);
  digitalWrite(Ptrig, LOW);
  
  duracion = pulseIn(Pecho, HIGH);
  distancia = (duracion/2) / 29;       // calcula la distancia en centimetros
  if (distancia <= 10 && pasarCarro)
  {
     contCarros = contCarros + 1;
     pasarCarro = false;
  }
  if(distancia > 10){
    pasarCarro = true;
  }
}

void contadorCarros(){
  if(iterador == 0){
    /*contarCarros(PtrigCalle4,PechoCalle4,contCarrosCalle4);
    Serial.print(contCarrosCalle4);
    Serial.print("calle4  ");*/
  digitalWrite(PtrigCalle4, LOW);
  delayMicroseconds(2);
  digitalWrite(PtrigCalle4, HIGH);   // genera el pulso de triger por 10ms
  delayMicroseconds(10);
  digitalWrite(PtrigCalle4, LOW);
  
  duracion = pulseIn(PechoCalle4, HIGH);
  distancia = (duracion/2) / 29;       // calcula la distancia en centimetros
  if (distancia <= 10 && pasarCarro)
  {
     contCarrosCalle4 = contCarrosCalle4 + 1;
     contCarrosHoraP4 = contCarrosHoraP4 +1;
     //Serial.println("CALLE 4");
     //Serial.println(contCarrosHoraP4);
     pasarCarro = false;
  }
  if(distancia > 10){
    pasarCarro = true;
  }
  }
  if(iterador == 1){
      digitalWrite(PtrigCalle1, LOW);
      delayMicroseconds(2);
      digitalWrite(PtrigCalle1, HIGH);   // genera el pulso de triger por 10ms
      delayMicroseconds(10);
      digitalWrite(PtrigCalle1, LOW);   
      duracion = pulseIn(PechoCalle1, HIGH);
      distancia = (duracion/2) / 29;       // calcula la distancia en centimetros
      
  if (distancia <= 10 && pasarCarro)
  {
      contCarrosCalle1 = contCarrosCalle1 +1;
      contCarrosHoraP1 = contCarrosHoraP1 +1;
      //Serial.println("CALLE 1");
      //Serial.println(contCarrosHoraP1);
      pasarCarro = false;
  }
  if(distancia > 10){
      pasarCarro = true;
  }
  }
  if(iterador == 2){
  digitalWrite(PtrigCalle2, LOW);
  delayMicroseconds(2);
  digitalWrite(PtrigCalle2, HIGH);   // genera el pulso de triger por 10ms
  delayMicroseconds(10);
  digitalWrite(PtrigCalle2, LOW);
  duracion = pulseIn(PechoCalle2, HIGH);
  distancia = (duracion/2) / 29;       // calcula la distancia en centimetros
  if (distancia <= 10 && pasarCarro)
  {
     contCarrosCalle2 = contCarrosCalle2 + 1;
     contCarrosHoraP2 = contCarrosHoraP2 +1;

     //Serial.println("CALLE 2");
     //Serial.println(contCarrosHoraP2);
     
     pasarCarro = false;
  }
  if(distancia > 10){
    pasarCarro = true;
  }
  }
  if(iterador == 3){
  digitalWrite(PtrigCalle3, LOW);
  delayMicroseconds(2);
  digitalWrite(PtrigCalle3, HIGH);   // genera el pulso de triger por 10ms
  delayMicroseconds(10);
  digitalWrite(PtrigCalle3, LOW);
  
  duracion = pulseIn(PechoCalle3, HIGH);
  distancia = (duracion/2) / 29;       // calcula la distancia en centimetros
  if (distancia <= 10 && pasarCarro)
  {
     contCarrosCalle3 = contCarrosCalle3 + 1;
     contCarrosHoraP3= contCarrosHoraP3 +1;

     //Serial.println("CALLE 3");
     //Serial.println(contCarrosHoraP3);
     
     pasarCarro = false; 
  }
  if(distancia > 10){
    pasarCarro = true;
  }
  }
}

int contadorPeticionesN = 0;


void detectarPeticion(){
  if(!trenPasando){
    if(!peticionNormal){
      if(digitalRead(pn5)==HIGH){
        contadorPeticionesN++;
      } 
      if(digitalRead(pn6)==HIGH){
        contadorPeticionesN++;
      }
      if(digitalRead(pn7)==HIGH){
        contadorPeticionesN++;
      }  
      if(digitalRead(pn8)==HIGH){
        contadorPeticionesN++;
      }
      if(digitalRead(pn9)==HIGH){
        contadorPeticionesN++;
      } 
      if(digitalRead(pn10)==HIGH){
        contadorPeticionesN++;
      }  
      //if(digitalRead(pn11)==HIGH){
        //contadorPeticionesN++;
      //}      
      if(digitalRead(pn12)==HIGH){
        contadorPeticionesN++;
      }   
      if(!(horaPicoS1 || horaPicoS2 || horaPicoS3 || horaPicoS4)){
        if(contadorPeticionesN>1){
          peticionNormal = true;
          contadorPeticionesN = 0;
        }
      }else{
        if(contadorPeticionesN>4){
          peticionNormal = true;
          contadorPeticionesN = 0;
        }
       }  
    }
  if(!peticionEspecial){
      if(digitalRead(sd)== HIGH){
        peticionEspecial = true;
      }  
   }
  }
}


void detectarTrafico(){
  if(contCarrosHoraP1 > 2){
    horaPicoS1 = true;  
  }
  if(contCarrosHoraP2 > 2){
    horaPicoS2 = true;  
  }
  if(contCarrosHoraP3 > 2){
    horaPicoS3 = true;  
  }
  if(contCarrosHoraP4 > 2){
    horaPicoS4 = true;  
  }
} 


void firebase(){
   data =  Serial.readString();
   if(data.equals("\"P1\"")){
      peticionNormal = true;
   }
   serialArd.print(strFireBase);
}

void stringFirebase(){  
  strFireBase = flu1+"_"+flu2+"_"+flu3+"_"+flu4+"_"+(String)contCarrosCalle1+"_"+(String)contCarrosCalle2+"_"+(String)contCarrosCalle3+"_"+(String)contCarrosCalle4+"_"+(String)contDia+"_"; 
}

void loop() {
 if(!trenPasando){ 
  contarTiempo();
  cambiarSemaforo();
  contadorCarros();
  detectarTrafico();
  //Serial.println(flu4);
 }
 sensorTren();
 accionDeTren(); 
}
