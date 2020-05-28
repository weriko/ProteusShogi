// import the libraries that we'll use
#include "stdio.h"
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

//variables
SoftwareSerial mySerial(2,3);  //define other inputs/outputs 
LiquidCrystal lcd_blue (53,51,49,47,45,43); //define lcd pins
LiquidCrystal lcd_red (35,33,31,29,27,25); //define lcd pins

String pieces_list = "KGRSCBLPDAOOOOOO";  // pieces and transformations 
int positions_column[] = {14,41,68,95,122,149,176,203,230}; //column separation in pixeles 
int positions_row[]={15,45,80,115,150,185,220,255,299};  //row separation in pixeles 
size_t pieces_color[]={ILI9341_RED,ILI9341_BLACK};  // pieces colors 
String mover = "";
String pieces;
String lcd_pieces_row1 ="";
String lcd_pieces_row2 ="";
Adafruit_ILI9341 tft = Adafruit_ILI9341(8,9,11,13, 10, 12);  // display configuration

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  lcd_blue.begin(16,2);
  lcd_red.begin(16,2);
  mySerial.println("SHOGI GAME");  
  tft.begin();
  pinMode(2,INPUT);  //RX pin for terminal
  pinMode(3,OUTPUT); //TX pin for terminal
  pinMode(7,OUTPUT); //blue led pin 
  pinMode(4,OUTPUT); //red led pin
  pinMode(12,OUTPUT); //speaker pin 
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  mySerial.print("Display Power Mode: 0x"); mySerial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDMADCTL);
  mySerial.print("MADCTL Mode: 0x"); mySerial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDPIXFMT);
  mySerial.print("Pixel Format: 0x"); mySerial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDIMGFMT);
  mySerial.print("Image Format: 0x"); mySerial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDSELFDIAG);
  mySerial.print("Self Diagnostic: 0x"); mySerial.println(x, HEX); 
  delay(1000);
  
  mySerial.println("Loading...");
  Serial.println(board());
  song_dbz();  //play dragon ball song in a speaker
  Serial.println(DrawPieces());
  delay(500);
  mySerial.print("Start game");
  noTone(9);
  digitalWrite(7,HIGH);
}


void loop(void) {
  int choice;
  int player=0;
  Serial.flush();   //delete buffer
  if(Serial.available()>0){  //detects if there are inputs
    mover = Serial.readString(); //saves the data that send you
    mySerial.println(mover);
    choice = int (String(mover[0]).toInt());
    lcd_pieces_row1 = String(mover[8])+String(mover[9])+String(mover[10])+String(mover[11])+String(mover[12])+String(mover[13])+String(mover[14])+String(mover[15])+String(mover[16])+String(mover[17])+String(mover[18])+String(mover[19]);
    mySerial.println(lcd_pieces_row1);
    lcd_pieces_row2 = String(mover[20])+String(mover[21])+String(mover[22])+String(mover[23])+String(mover[24])+String(mover[25])+String(mover[26])+String(mover[27])+String(mover[28]);
    mySerial.println(lcd_pieces_row2);
    
    if(choice==1){
      if(mover[5]!='x'){  //define if is a one digit number or two digits
        pieces = String(mover[5])+String(mover[6]);
      }else{
        pieces = String(mover[6]);
      }
      lcd_print(lcd_pieces_row1, lcd_pieces_row2,int( String(mover[7]).toInt()) );
      pieces_movement(int (String(mover[2]).toInt()),int (String(mover[1]).toInt()),positions_column[int (String(mover[4]).toInt())],positions_row[int(String(mover[3]).toInt())],int(pieces.toInt()) ,int( String(mover[7]).toInt()));
      delay(500);
    }else{
      lcd_print(lcd_pieces_row1, lcd_pieces_row2,int( String(mover[5]).toInt()) );
      put_piece(positions_column[int (String(mover[2]).toInt())],positions_row[int(String(mover[1]).toInt())],int(String(mover[4]).toInt()) ,int( String(mover[5]).toInt()));
      delay(500);
    }
  }
}

unsigned long board(){ // create a shogi board
  unsigned long start;
  int line_x, line_y, line_width = tft.width(), line_height = tft.height();
  start = micros();
  for(line_y=0; line_y<line_height; line_y+=35) tft.drawFastHLine(0, line_y, line_width, ILI9341_BLUE);  //draw horizontal lines
  for(line_x=0; line_x<line_width; line_x+=27) tft.drawFastVLine(line_x, 0, line_height, ILI9341_BLACK);  //draw vertical lines
  return micros() - start;
}

unsigned long DrawPieces(){  //Draw the default positions of a piece
  unsigned long start = micros();
  pawns();  //call pawns function
  rook();  //call rook function
  bishop();  //call bishop function
  lances();  //call lances function
  knights();  //call knights function
  silver_generals();  //call silver generals function
  golds_generals();  //call glold generals function
  kings();  //call kings function
  return micros() - start;
}
unsigned long pawns(){  // default pawns
  tft.setCursor(positions_column[0],positions_row[2]);
  tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(1);
  tft.println("P");
  tft.setCursor(positions_column[1],positions_row[2]);
  tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(1);
  tft.println("P");
  tft.setCursor(positions_column[2],positions_row[2]);
  tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(1);
  tft.println("P");
  tft.setCursor(positions_column[3],positions_row[2]);
  tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(1);
  tft.println("P");
  tft.setCursor(positions_column[4],positions_row[2]);
  tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(1);
  tft.println("P");
  tft.setCursor(positions_column[5],positions_row[2]);
  tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(1);
  tft.println("P");
  tft.setCursor(positions_column[6],positions_row[2]);
  tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(1);
  tft.println("P");
  tft.setCursor(positions_column[7],positions_row[2]);
  tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(1);
  tft.println("P");
  tft.setCursor(positions_column[8],positions_row[2]);
  tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(1);
  tft.println("P");
  tft.setCursor(positions_column[0],positions_row[6]);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(1);
  tft.println("P");
  tft.setCursor(positions_column[1],positions_row[6]);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(1);
  tft.println("P");
  tft.setCursor(positions_column[2],positions_row[6]);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(1);
  tft.println("P");
  tft.setCursor(positions_column[3],positions_row[6]);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(1);
  tft.println("P");
  tft.setCursor(positions_column[4],positions_row[6]);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(1);
  tft.println("P");
  tft.setCursor(positions_column[5],positions_row[6]);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(1);
  tft.println("P");
  tft.setCursor(positions_column[6],positions_row[6]);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(1);
  tft.println("P");
  tft.setCursor(positions_column[7],positions_row[6]);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(1);
  tft.println("P");
  tft.setCursor(positions_column[8],positions_row[6]);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(1);
  tft.println("P");
}

unsigned long rook(){  //default rook
  tft.setCursor(positions_column[1],positions_row[1]);
  tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(1);
  tft.println("R");
  tft.setCursor(positions_column[7],positions_row[7]);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(1);
  tft.println("R");
}

unsigned long bishop(){  //default bihop
  tft.setCursor(positions_column[7],positions_row[1]);
  tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(1);
  tft.println("B");
  tft.setCursor(positions_column[1],positions_row[7]);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(1);
  tft.println("B");
}

unsigned long lances(){  //default lances
  tft.setCursor(positions_column[0],positions_row[0]);
  tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(1);
  tft.println("L");
  tft.setCursor(positions_column[8],positions_row[0]);
  tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(1);
  tft.println("L");
  tft.setCursor(positions_column[0],positions_row[8]);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(1);
  tft.println("L");
  tft.setCursor(positions_column[8],positions_row[8]);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(1);
  tft.println("L"); 
}

unsigned long knights(){  //default  knights
  tft.setCursor(positions_column[1],positions_row[0]);
  tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(1);
  tft.println("C");
  tft.setCursor(positions_column[7],positions_row[0]);
  tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(1);
  tft.println("C");
  tft.setCursor(positions_column[1],positions_row[8]);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(1);
  tft.println("C");
  tft.setCursor(positions_column[7],positions_row[8]);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(1);
  tft.println("C");
}
unsigned long silver_generals(){  //default silver generals
  tft.setCursor(positions_column[2],positions_row[0]);
  tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(1);
  tft.println("S");
  tft.setCursor(positions_column[6],positions_row[0]);
  tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(1);
  tft.println("S");
  tft.setCursor(positions_column[2],positions_row[8]);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(1);
  tft.println("S");
  tft.setCursor(positions_column[6],positions_row[8]);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(1);
  tft.println("S");
}
unsigned long golds_generals(){  //default gold generals 
  tft.setCursor(positions_column[3],positions_row[0]);
  tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(1);
  tft.println("G");
  tft.setCursor(positions_column[5],positions_row[0]);
  tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(1);
  tft.println("G");
  tft.setCursor(positions_column[3],positions_row[8]);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(1);
  tft.println("G");
  tft.setCursor(positions_column[5],positions_row[8]);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(1);
  tft.println("G");
}
unsigned long kings(){  //default kings
  tft.setCursor(positions_column[4],positions_row[0]);
  tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(1);
  tft.println("K");
  tft.setCursor(positions_column[4],positions_row[8]);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(1);
  tft.println("K");
}

unsigned change_led(int player_color){ //led on or off
  if (player_color == 0){ //define the player
      mySerial.println("1");
      digitalWrite(7,HIGH);
      digitalWrite(4,LOW);
    }else{
      mySerial.println("2");
      digitalWrite(4,HIGH);
      digitalWrite(7,LOW);
    }
}

unsigned pieces_movement(int old_x,int old_y, int new_x,int new_y, int move_piece,int player_color){  //move piece 
  unsigned long start = micros();
  change_led(player_color);
  tft.fillRect(positions_column[old_x],positions_row[old_y],10,10,ILI9341_WHITE);  //put a white rectangle in the old positions
  delay(200);
  tft.fillRect(new_x,new_y,10,10,ILI9341_WHITE);  //put a white rectangle in the new positions
  delay(200);
  tft.setCursor(new_x,new_y);  //put cursor in the new positions
  tft.setTextColor(pieces_color[player_color]);  tft.setTextSize(1);  //define the color and size text
  tft.println(pieces_list[move_piece-1]);  //shows the piece to place 
  return micros() - start;
}

unsigned put_piece(int put_x,int put_y,int put_piece, int player_color){  //put piece
  unsigned long start = micros();
  change_led(player_color);
  tft.setCursor(put_x,put_y);  //put cursor in the positions
  tft.setTextColor(pieces_color[player_color]);  tft.setTextSize(1);  //define the color and size text
  tft.println(pieces_list[put_piece-1]);  //shows the piece to place
  return micros() - start;
}
unsigned lcd_print(String row1, String row2, int p){
  if( p == 1){ //define lcds 
    lcd_blue.setCursor(0,0);
    lcd_blue.println(row1);
    lcd_blue.setCursor(0,1);
    lcd_blue.println(row2);
  }else{
    lcd_red.setCursor(0,0);
    lcd_red.println(row1);
    lcd_red.setCursor(0,1);
    lcd_red.println(row2);
  }
}
void song_dbz(){  //function with a tones, rhythm and frequency of dragon ball song
  tone(12, 698, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 698, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 622, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 523, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 698, 231.907546875);
    delay(257.675052083);
    delay(794.954947917);
    tone(12, 698, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 698, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 622, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 523, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 698, 231.907546875);
    delay(257.675052083);
    delay(794.954947917);
    tone(12, 466, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 466, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 466, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 415, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 466, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 466, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 466, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 415, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 349, 828.946125);
    delay(921.05125);
    delay(1184.20875);
    tone(12, 698, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 698, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 622, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 523, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 698, 231.907546875);
    delay(257.675052083);
    delay(794.954947917);
    tone(12, 698, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 698, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 622, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 523, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 698, 231.907546875);
    delay(257.675052083);
    delay(794.954947917);
    tone(12, 466, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 466, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 466, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 415, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 466, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 466, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 466, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 415, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 523, 828.946125);
    delay(921.05125);
    delay(1184.20875);
    tone(12, 523, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 622, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 698, 394.73625);
    delay(438.595833333);
    delay(87.7191666667);
    tone(12, 698, 394.73625);
    delay(438.595833333);
    delay(87.7191666667);
    tone(12, 698, 394.73625);
    delay(438.595833333);
    delay(87.7191666667);
    tone(12, 622, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 523, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 622, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 523, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 622, 78.94725);
    delay(87.7191666667);
    delay(175.438333333);
    tone(12, 622, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 698, 231.907546875);
    delay(257.675052083);
    delay(794.954947917);
    tone(12, 523, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 622, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 698, 828.946125);
    delay(921.05125);
    delay(131.57875);
    tone(12, 830, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 783, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 698, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 622, 394.73625);
    delay(438.595833333);
    delay(87.7191666667);
    tone(12, 830, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 783, 231.907546875);
    delay(257.675052083);
    delay(5.48244791667);
    tone(12, 698, 394.73625);
    delay(438.595833333);
    delay(87.7191666667);
  } 
