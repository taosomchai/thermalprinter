/*******************************************
 * see header-file for further informations
 ********************************************/

#include "Arduino.h"
#include "thermalprinter.h"

static const char LF = 0xA; // print buffer and line feed  
    
    
Epson::Epson(int rxPin, int txPin)
{
  this->_rxPin = rxPin;
  this->_txPin = txPin;
  this->start();
}

void Epson::start(){

  pinMode(this->_txPin, OUTPUT);
  pinMode(this->_rxPin, INPUT);  
  this->_printer = new SoftwareSerial (this->_rxPin, this->_txPin);
  this->_printer->begin(38400);
}

// query status of printer. when online returns value 22.
int Epson::getStatus(){
  //this->write(0x10);    
  //this->write(0x04);  
  //this->write(1);
  this->write(0x1B);    
  this->write(0x76);  
  this->write(0);
//bit0==0 Print is Ready 
//bit0==1 Print is Not Ready  
//bit2==0 There are paper 
//bit2==4 Out of Paper 
//bit3==0 Voltage is normal 
//bit3==8 Voltage higher than 9.5V  
//bit6==0 Temperature is normal 
//bit6==64 Temperature over 60 degrees  

  int result;
  result = this->_printer->read();
  return result;
}

int Epson::read(){
    int result;
    result = this->_printer->read();
    return result;
}

// Print and feed n lines
// prints the data in the print buffer and feeds n lines
void Epson::feed(uint8_t n){
  this->write(0x1B);  
  this->write(0x64);
  this->write(n);    
}

// Print one line
void Epson::feed(){
  this->feed(1);    
}


// Set line spacing
// sets the line spacing to n/180-inch
void Epson::lineSpacing(uint8_t n){
  this->write(0x1B);  
  this->write(0x33);
  this->write(n);  
}

// Select default line spacing
// sets the line spacing to 1/6 inch (n=60). This is equivalent to 30 dots.
void Epson::defaultLineSpacing(){
  this->write(0x1B);  
  this->write(0x32);
}

// Select an international character set
//  0 = U.S.A. 
//  1 = France 
//  2 = Germany 
//  3 = U.K. 
//  4 = Denmark I 
//  5 = Sweden 
//  6 = Italy 
//  7 = Spain 
//  8 = Japan 
//  9 = Norway 
// 10 = Denmark II 
// see reference for Details! 
void Epson::characterSet(uint8_t n){
  this->write(0x1B);  
  this->write(0x52);
  this->write(n);  
}


void Epson::doubleHeightOn(){
  this->write(0x1B);    
  this->write(0x21);  
  this->write(16);
}

void Epson::doubleHeightOff(){
  this->write(0x1B);  
  this->write(0x21);    
  this->write(0);
}

void Epson::boldOn(){
  this->write(0x1B);  
  this->write(0x21);    
  this->write(8);
}

void Epson::boldOff(){
  this->write(0x1B);  
  this->write(0x21);    
  this->write(0);
}

void Epson::underlineOff() {
  this->write(0x1B);  
  this->write(0x21);    
  this->write(0);
}
void Epson::underlineOn() {
  this->write(0x1B);  
  this->write(0x21);    
  this->write(128);
}


// Turn white/black reverse printing mode on/off
void Epson::reverseOn() {
  this->write(0x1D);  
  this->write(0x42);    
  this->write(1);
}
  
void Epson::reverseOff() {
  this->write(0x1D);  
  this->write(0x42);    
  this->write(0);
}

void Epson::justifyLeft() {
  this->write(0x1B);  
  this->write(0x61);    
  this->write(0);
}

void Epson::justifyCenter() {
  this->write(0x1B);  
  this->write(0x61);    
  this->write(1);
}

void Epson::justifyRight() {
  this->write(0x1B);  
  this->write(0x61);    
  this->write(2);
}
//n range 1-255
void Epson::barcodeHeight(uint8_t n) {
  this->write(0x1D);  
  this->write(0x68);    
  this->write(n);
}
//n range 2-6
void Epson::barcodeWidth(uint8_t n) {
  this->write(0x1D);  
  this->write(0x77);    
  this->write(n);
}
//n range 0-3
void Epson::barcodeNumberPosition(uint8_t n) {
  this->write(0x1D);  
  this->write(0x48);    
  this->write(n);
}
//m range 65-73 (code type)
//n (digit length)
void Epson::printBarcode(uint8_t m, uint8_t n) {
  this->write(0x1D);  
  this->write(0x6B);    
  this->write(m);
  this->write(n);
}

// Two-Dimensional Code Commands
// Gs ( k pL pH cn fn [parameters]
//      Store, prints symbol data, or configure the settings.
//		cn =	48: PDF417
//				49: QR Code
//				50: MatricCode
//Select the model
//qz.append(chr(29)+chr(40)+chr(107)+chr(4)+chr(0)+chr(49)+chr(65)+chr(50)+chr(0));
//Set the size
//qz.append(chr(29)+chr(40)+chr(107)+chr(3)+chr(0)+chr(49)+chr(67)+chr(32));
//Error correction M
//qz.append(chr(29)+chr(40)+chr(107)+chr(3)+chr(0)+chr(49)+chr(69)+chr(49));
//Store the data
//qz.append(chr(29)+chr(40)+chr(107)+chr(5)+chr(0)+chr(49)+chr(80)+chr(48)+"01");
//Print
//qz.append(chr(29)+chr(40)+chr(107)+chr(3)+chr(0)+chr(49)+chr(81)+chr(48));
void Epson::qrSelectModel() {
  this->write(0x1D);	//Gs
  this->write(0x28);	//(
  this->write(0x6B);	//k
  this->write(0x04);	//pL 0x04
  this->write(0x00);	//pH
  this->write(49);	//cn = QR Code
  this->write(65);	//fn 65= columns
  this->write(50);  //n1 50
  this->write(0);	//n2
}
void Epson::qrSetSize(uint8_t n) {
  this->write(0x1D);	//Gs
  this->write(0x28);	//(
  this->write(0x6B);	//k
  this->write(3);	//
  this->write(0);	//
  this->write(49);	//cn = QR Code
  this->write(67);	//fn 67= module
  this->write(n);  //n1
}
void Epson::qrErrorM() {
  this->write(29);	//Gs
  this->write(40);	//(
  this->write(107);	//k
  this->write(3);	//
  this->write(0);	//
  this->write(49);	//cn = QR Code
  this->write(69);	//fn error correction
//  this->write(49);  //n1
  this->write(48);  //n1  48
}
void Epson::qrStoreData(uint8_t size1) {
  this->write(29);	//Gs
  this->write(40);	//(
  this->write(107);	//k
  this->write(size1+3);	//pL
  this->write(0);	//pH
  this->write(49);	//cn = QR Code
  this->write(80);	//fn store symbol data in memory
  this->write(48);  //n1
}
void Epson::qrPrint() {
  this->write(29);	//Gs
  this->write(40);	//(
  this->write(107);	//k
  this->write(3);	//pL
  this->write(0);	//pH
  this->write(49);	//cn = QR Code
  this->write(81);	//fn print 2d sysmblo
  this->write(48);  //n1
}


void Epson::cut() {
  this->write(0x1D);
  this->write('V');
  this->write(66);
  this->write(0xA); // print buffer and line feed
}

//Select page mode
void Epson::PageMode() {
  this->write(0x1B);
  this->write('L');
}

//Select standard mode
void Epson::StandatdMode() {
  this->write(0x1B);
  this->write('S');
}

void Epson::SetXPos(int pos){
  this->write(0x1B);  
  this->write(0x24);
  this->write(lowByte(pos));//nL
  this->write(highByte(pos));//nH
}

void Epson::SetYPos(int pos){
  this->write(0x1D);  
  this->write(0x24);
  this->write(lowByte(pos));//nL
  this->write(highByte(pos));//nH
}

void Epson::SetPrnArea(){
  this->write(0x1B);  
  this->write('W');
  //horizontal logical origin
  this->write(0);//xL
  this->write(0);//xH
  this->write(0);//yL
  this->write(0);//yH
  //print area width and print area height
  this->write(0);//dxL 180
  this->write(2);//dxH
  this->write(44);//dyL
  this->write(1);//dyH
}

void Epson::SetMotion(){
  this->write(0x1D);  
  this->write('P');
  this->write(180);//x
  this->write(180);//y
}

void Epson::BatchPrint(){
  this->write(0x1B);  
  this->write(0x0C);  
}

size_t Epson::write(uint8_t c) {
  this->_printer->write(c);
  return 1;
}



