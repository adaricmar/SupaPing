
#include <iostream>
#include <string>
#include "input.h"
#include <unistd.h>
#include <stdlib.h>



extern "C" {
#include <xdo.h>
}



int main(int argc, char* argv[]) {

  bool legacyClick = false;


  if (argc == 1) {
    std::cout << "No arguments passed.\nUSAGE: Sp2 [device ID] [-r]\n";
    exit(2);
  }
  else if (argc >= 2) 
    std::cout << "Choosing device: " << argv[1] << std::endl;
  
  if (argc == 3) {
    
    std::string arg = argv[2];


    if (arg == "-r") {
      std::cout << "Legacy right-click enabled.\n";
      legacyClick = true;
    }
    else {
      std::cout << "Unknown argument passed.\nUSAGE: Sp2 [device ID] [-r]\n";
      exit(2);
    }

  }
    
  
  
    



  int pings = 0;
  int rightClicks = 0;

  int Xpos;
  int Ypos;
  int screen;

  input keys(argv[1]); // Create input structure son

  int delay = 10000;

  xdo_t *xdo = xdo_new(NULL);

  bool sp = true;
  


  while(1) {

    keys.waitUntilChange(); //Wait until a new key is pressed (or depressed)

    
    while(keys.returnLastKey() == 64) { //While L_ALT is pressed

      if(sp) {
	xdo_click_window(xdo,CURRENTWINDOW,1);
	pings++;
	std::cout << "Ping #" << pings << "\n";
      }

      else {

	if (legacyClick == false) {

	  xdo_move_mouse(xdo,1500,950,0);
	  xdo_mouse_down(xdo,CURRENTWINDOW,1);
	  xdo_move_mouse(xdo,1920/2,1080/2,0);
	  xdo_mouse_up(xdo,CURRENTWINDOW,1);
	}
	
	else {
	  xdo_click_window(xdo,CURRENTWINDOW,2);
	}

	rightClicks++;

	if(legacyClick) 
	  std::cout << "Legacy click #" << rightClicks << std::endl;
	else
	  std::cout << "Right click #" << rightClicks << std::endl;


      }

      usleep(delay);
    }



    while(keys.returnLastKey() == 86) { //While + is pressed
      std::cout << "Delay = " << delay << std::endl;
      delay = delay + 100;
      usleep(5000);
    }



    while(keys.returnLastKey() == 82) { // While - is pressed

      if ((delay-100) >= 0) {
	std::cout << "Delay = " << delay << std::endl;
	delay = delay - 100;
      }
      else
	std::cout << "Cant have a negative delay!\n";

      usleep(5000);
    }



    if (keys.returnLastKey() == 63) {

      if(sp) {
	sp = false;
	std::cout << "Switched to KillCour.\n";
      }
      else
	std::cout << "Already switched to KillCour\n";

    }



    if (keys.returnLastKey() == 106) {

      if(sp)
	std::cout << "Already switched to SupaPing.\n";
      else {
	sp = true;
	std::cout << "Switched to SupaPing.\n";
      }
    }	

    if(keys.returnLastKey() == 84) { // NUM_5

      std::cout << "Popping Glyph!\n";

      xdo_get_mouse_location(xdo,&Xpos,&Ypos,&screen);

      xdo_move_mouse(xdo, 1885, 1000 + 80/2, screen);

      xdo_click_window(xdo,CURRENTWINDOW,1);

      xdo_move_mouse(xdo, Xpos, Ypos, screen);

    }

      


    //std::cout << keys.returnLastKey() << "\n";

    
  }
    
  return 0;
}


  
