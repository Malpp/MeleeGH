/*
*  Project     Nintendo Extension Controller Library
*  @author     David Madison
*  @link       github.com/dmadison/NintendoExtensionCtrl
*  @license    LGPLv3 - Copyright (c) 2018 David Madison
*
*  This file is part of the Nintendo Extension Controller Library.
*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*  Example:      Guitar_DebugPrint
*  Description:  Connect to a Guitar Hero controller and continuously print
*                its control data, nicely formatted for debugging, over
*                the serial bus.
*/

#include <NintendoExtensionCtrl.h>
#include "Nintendo.h"

GuitarController guitar;
CGamecubeConsole GamecubeConsole(4);
CGamecubeController GamecubeController1(2);

Gamecube_Data_t gcc = defaultGamecubeData;
Gamecube_Data_t gcc_empty = defaultGamecubeData;

void setup() {
  GamecubeController1.read();
  GamecubeConsole.write(gcc_empty);
  
	//Serial.begin(115200);
	guitar.begin();

	while (!guitar.connect()) {
		//Serial.println("Guitar controller not detected!");
		delay(100);
	}
}

void loop() {
	boolean success = guitar.update();  // Get new data from the controller

	if (success == true) {  // We've got data!
		//guitar.printDebug();  // Print all of the values!

    gcc.report.a = guitar.fretGreen();
    gcc.report.b = guitar.fretRed();
    gcc.report.x = guitar.fretYellow();
    gcc.report.y = guitar.fretBlue();
    gcc.report.z = guitar.fretOrange();
    gcc.report.start = guitar.buttonPlus();
    //gcc.report.r = pinR;
    //gcc.report.l = false ? 1 : 0;
    gcc.report.left = constrain(guitar.whammyBar() - 16, 0, 15) * 15;
    int x = 255 - constrain(guitar.joyX() * 4, 0, 255);
    int y = constrain(guitar.joyY() * 4, 0, 255);
    gcc.report.cxAxis = 128;
    gcc.report.cyAxis = 128;
    gcc.report.xAxis = 128;
    gcc.report.yAxis = 128;
    gcc.report.dup = 0;
    gcc.report.ddown = 0;
    gcc.report.dleft = 0;
    gcc.report.dright = 0;
    if(guitar.strumDown()){
      gcc.report.cxAxis = x;
      gcc.report.cyAxis = y;  
    } else if(guitar.strumDown()){
      gcc.report.dup = y > 200;
      gcc.report.ddown = y < 50;
      gcc.report.dleft = x < 50;
      gcc.report.dright = x > 200;
    } else {
      gcc.report.xAxis = x;
      gcc.report.yAxis = y;  
    }
    gcc.report.ddown = guitar.buttonMinus();
    
    //gcc.report.cxAxis = 128 + constrain(0 * 3, -128, 127);
    //gcc.report.cyAxis = 128 + constrain(0, -128, 127);
    //sends the complied data to console when console polls for the input
    GamecubeConsole.write(gcc);
	}
	else {  // Data is bad :(
		//Serial.println("Controller Disconnected!");
		delay(1000);
		guitar.connect();
	}
}
