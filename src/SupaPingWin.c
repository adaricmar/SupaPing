

#define _WIN32_WINNT 0x0500
#include <stdio.h>
#include <windows.h>
#include <pthread.h>
#include <assert.h>
   
int altKeyPressed = 0;

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
   
   
   
   
   
LRESULT CALLBACK keyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
      
    //printf("Got into the hook.");
  
  
    PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT) (lParam);
    
    if (wParam == WM_SYSKEYDOWN && p->vkCode == VK_LMENU) {
      
	  
	  altKeyPressed = 1;
    }
    else if (wParam == WM_KEYDOWN && p->vkCode == VK_LCONTROL) {
      altKeyPressed = altKeyPressed;
    }
    else {
      altKeyPressed = 0;
    }
    
    
    pthread_mutex_lock(&mut);
    if (altKeyPressed) pthread_cond_broadcast(&cond);    
    pthread_mutex_unlock(&mut);
    
    
    
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}
    
    
void LeftClick()
{  
	INPUT    Input={0};													// Create our input.

	Input.type        = INPUT_MOUSE;									// Let input know we are using the mouse.
	Input.mi.dwFlags  = MOUSEEVENTF_LEFTDOWN;							// We are setting left mouse button down.
	SendInput( 1, &Input, sizeof(INPUT) );								// Send the input.

	ZeroMemory(&Input,sizeof(INPUT));									// Fills a block of memory with zeros.
	Input.type        = INPUT_MOUSE;									// Let input know we are using the mouse.
	Input.mi.dwFlags  = MOUSEEVENTF_LEFTUP;								// We are setting left mouse button up.
	SendInput( 1, &Input, sizeof(INPUT) );								// Send the input.
}
    
void * doWork(void *) {
   
    int pingCount = 0;
 
   
    while(1) {
     
      //printf("Output: %d \n",output);
   
  
	pthread_mutex_lock(&mut);
	while (!altKeyPressed) {
	  pthread_cond_wait(&cond,&mut);
	}
	pthread_mutex_unlock(&mut);
	
	LeftClick();
	
	++pingCount;
	
	printf("Ping #%d \n",pingCount);
	
	Sleep(1);
   
    }
    

  
}



    
    

    
    
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    
      
      
    printf("DOTA 2 SupaPing by Adam Martinez! \n");
      
      
    printf("Setting hook...");
    HHOOK keyboardHook = SetWindowsHookEx(
    WH_KEYBOARD_LL,
    keyboardHookProc,
    hInstance,
    0);
    
    printf("done!\n");
    
    pthread_t dw;
    
    int thread_args = 1;
    
    int NUM_THREADS = 1;
    
    int rc;
    
    printf("Initializing work thread...");
    pthread_create(&dw, NULL, &doWork, (void *) &thread_args);
    printf("done!\n");
    
    
    printf("You can now hold ALT to SupaPing!  \n");
    
    
    MessageBox(NULL, "Press OK to stop the pinging.", "Dota 2 SupaPing!", MB_OK);
    
    
    //rc = pthread_join(dw, NULL);
    //assert(0 == rc);
    
    printf("\nExiting...\n");
    
    
    
    return 0;
}

