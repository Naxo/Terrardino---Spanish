void aboutScreen()
{ 
  dispScreen = 8;
  printHead("Terrardino Terrarium Controller v.0.1");
  
  setFont(SMALL, blackColor, greenLightColor);
  myGLCD.print("Written by Nacho Lafuente", CENTER, 32);
  
  myGLCD.print("Main code based on Stilo", 5, 52);
  myGLCD.print("http://code.google.com/p/stilo/", 5, 64);
  
  myGLCD.print("Free icons from", 5, 84);
  myGLCD.print("http://icons.mysitemyway.com/", 5, 96);

  myGLCD.print("Contact: terrardino@gmail.com", 5, 120); 
  myGLCD.print("License: GNU General Public License ", 5, 144);
  myGLCD.print("This program is distributed in the hope that it will be useful", 5, 156);
  myGLCD.print("but WITHOUT ANY WARRANTY;", 5, 168);
  
  printLineSeparator();
  
  cancelButton.print();
} 
