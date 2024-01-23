//Initialisation of display segments, buttons and mosfets
int SegA = 2;
int SegB = 3;
int SegC = 4;
int SegD = 5;
int SegE = 6;
int SegF = 7;
int SegG = 8;
int Mosfet1 = 9;
int Mosfet2 = 10;
int Mosfet3 = 11;
int Mosfet4 = 12;
int Bouton1 = A0;
int Bouton2 = A1;
unsigned long previousMillis; //Initialisation of a previousMillis variable
//Matrix to know the display segment to switch on for each number
int MatriceNumero[10][7] = {
  { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW }, //number 0
  { LOW, HIGH, HIGH, LOW, LOW, LOW, LOW }, //number 1
  { HIGH, HIGH, LOW, HIGH, HIGH, LOW, HIGH }, //number 2
  { HIGH, HIGH, HIGH, HIGH, LOW, LOW, HIGH }, //number 3
  { LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH }, //number 4
  { HIGH, LOW, HIGH, HIGH, LOW, HIGH, HIGH }, //number 5
  { HIGH, LOW, HIGH, HIGH, HIGH, HIGH, HIGH }, //number 6
  { HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW }, //number 7
  { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH }, //number 8
  { HIGH, HIGH, HIGH, HIGH, LOW, HIGH, HIGH }, //number 9
};
//Matrix to control mosfet
int MatriceAfficheur[4][4] = {
  { HIGH, LOW, LOW, LOW, },
  { LOW, HIGH, LOW, LOW, },
  { LOW, LOW, HIGH, LOW, },
  { LOW, LOW, LOW, HIGH, },
};
//Table to write and read each variable of the hour 
//ex : [1, 8, 3, 4] for 18h34
int TableauHeure[4];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  TableauHeure[0] = 0;
  TableauHeure[1] = 0;
  TableauHeure[2] = 0;
  TableauHeure[3] = 0;
}

void loop(){
  setHeure();
  previousMillis = millis();
  Serial.println(millis());
  afficher();
  Serial.println(millis());
  while(true){
    calculTemps();
    afficher();
  }

}

//Every 60 seconds, we add 1minutes and we verify if we need to change the other variable
void calculTemps(){
  if(millis() >= previousMillis + 60000){
    previousMillis = millis();
    TableauHeure[3] += 1;
    if(TableauHeure[3] == 10){
      TableauHeure[3] = 0;
      TableauHeure[2] += 1;
      if(TableauHeure[2] == 6){
        TableauHeure[2] = 0;
        TableauHeure[1] += 1;
        if(TableauHeure[0] == 2 && TableauHeure[1] == 4){
          TableauHeure[0] = 0;
          TableauHeure[1] = 0;
        }
        else if(TableauHeure[1] == 10){
          TableauHeure[1] = 0;
          TableauHeure[0] += 1;
        }
      }
    }
  }
}

//We need this function to initialise the hour with the button
void setHeure() {
  int start = 0;
  while(start != 4){
    bool verif = false;
    bool verif1 = false;
    while(digitalRead(Bouton1) == 0){
      verif = true;
      afficher();
    }
    if(verif == true){
      if(start == 0){
        if(TableauHeure[0] == 2){
          TableauHeure[0] = -1;
        }
      }
      else if(start == 1){
        if(TableauHeure[0] == 2 && TableauHeure[1] == 3){
          TableauHeure[1] = -1;
        }
        if(TableauHeure[1] == 9){
          TableauHeure[1] = -1;
        }
      }
      else if(start == 2){
        if(TableauHeure[2] == 5){
          TableauHeure[2] = -1;
        }
      }
      else if(start == 3){
        if(TableauHeure[3] == 9){
          TableauHeure[3] = -1;
        }
      }
      TableauHeure[start] += 1;
    }
    while(digitalRead(Bouton2) == 0){
      verif1 = true;
      afficher();
    }
    if(verif1 == true){
      start += 1;
    }
    afficher();
  }
}

//"afficher()" is the function to display the good variable on each 7 segments display
void afficher() {
  int state1 = LOW;
  int state2 = LOW;
  for(int i = 0; i < 4; i++){ 
    for(int e = 0; e < 4; e++){
      digitalWrite(e+9, MatriceAfficheur[i][e]);
    }
    for(int y = 0; y < 7; y++){
      state2 = MatriceNumero[TableauHeure[i]][y];
      digitalWrite(y+2, state2);
    }
    delay(4);
  }
}


