/*
 * A lot of this code was repurposed from the xinput project. 
 * Thanks and copyright notice to the authors! :
 *
 * Copyright 1996 by Frederic Lepied, France. <Frederic.Lepied@sugix.frmug.org>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is  hereby granted without fee, provided that
 * the  above copyright   notice appear  in   all  copies and  that both  that
 * copyright  notice   and   this  permission   notice  appear  in  supporting
 * documentation, and that   the  name of  the authors  not  be  used  in
 * advertising or publicity pertaining to distribution of the software without
 * specific,  written      prior  permission.     The authors  make  no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.
 *
 * THE AUTHORS DISCLAIM ALL   WARRANTIES WITH REGARD  TO  THIS SOFTWARE,
 * INCLUDING ALL IMPLIED   WARRANTIES OF MERCHANTABILITY  AND   FITNESS, IN NO
 * EVENT  SHALL THE AUTHORS  BE   LIABLE   FOR ANY  SPECIAL, INDIRECT   OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA  OR PROFITS, WHETHER  IN  AN ACTION OF  CONTRACT,  NEGLIGENCE OR OTHER
 * TORTIOUS  ACTION, ARISING    OUT OF OR   IN  CONNECTION  WITH THE USE    OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 */


#include <string.h>
#include <X11/Xlib.h>
#include <X11/extensions/XInput.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <time.h>

#define INVALID_EVENT_TYPE	-1

static int           motion_type = INVALID_EVENT_TYPE;
static int           button_press_type = INVALID_EVENT_TYPE;
static int           button_release_type = INVALID_EVENT_TYPE;
static int           key_press_type = INVALID_EVENT_TYPE;
static int           key_release_type = INVALID_EVENT_TYPE;
static int           proximity_in_type = INVALID_EVENT_TYPE;
static int           proximity_out_type = INVALID_EVENT_TYPE;

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
  
int output = 0;

int randomMode = 0;

char *device = "12";



static int
register_events(Display		*dpy,
		XDeviceInfo	*info,
		char		*dev_name,
		Bool		handle_proximity)
{
    int			number = 0;	/* number of events registered */
    XEventClass		event_list[7];
    int			i;
    XDevice		*device;
    Window		root_win;
    unsigned long	screen;
    XInputClassInfo	*ip;

    screen = DefaultScreen(dpy);
    root_win = RootWindow(dpy, screen);

    device = XOpenDevice(dpy, info->id);

    if (!device) {
	fprintf(stderr, "unable to open device '%s'\n", dev_name);
	return 0;
    }

    if (device->num_classes > 0) {
	for (ip = device->classes, i=0; i<info->num_classes; ip++, i++) {
	    switch (ip->input_class) {
	    case KeyClass:
		DeviceKeyPress(device, key_press_type, event_list[number]); number++;
		DeviceKeyRelease(device, key_release_type, event_list[number]); number++;
		break;

	    case ButtonClass:
		DeviceButtonPress(device, button_press_type, event_list[number]); number++;
		DeviceButtonRelease(device, button_release_type, event_list[number]); number++;
		break;

	    case ValuatorClass:
		DeviceMotionNotify(device, motion_type, event_list[number]); number++;
		if (handle_proximity) {
		    ProximityIn(device, proximity_in_type, event_list[number]); number++;
		    ProximityOut(device, proximity_out_type, event_list[number]); number++;
		}
		break;

	    default:
		fprintf(stderr, "unknown class\n");
		break;
	    }
	}

	if (XSelectExtensionEvent(dpy, root_win, event_list, number)) {
	    fprintf(stderr, "error selecting extended events\n");
	    return 0;
	}
    }
    return number;
}

static void
print_events(Display	*dpy)
{
    XEvent        Event;

    while(1) {
        
	XNextEvent(dpy, &Event);
	

	 
	  
	// detect Keyboard events only
	
	if ((Event.type == key_press_type) ||
		   (Event.type == key_release_type)) {
	    int	loop;
	    XDeviceKeyEvent *key = (XDeviceKeyEvent *) &Event;
	
	
	
	
	    pthread_mutex_lock(&mut);
	  
	    output = key->keycode;
	
	    if (output == 64) pthread_cond_broadcast(&cond);
	    
	    pthread_mutex_unlock(&mut);
	


	
	    if (Event.type == key_release_type) output = -1*output;  
	    

	    for(loop=0; loop<key->axes_count; loop++) {
		printf("a[%d]=%d ", key->first_axis + loop, key->axis_data[loop]);
	    }

	} 
	
    }
}


XDeviceInfo*
find_device_info(Display	*display,
		 char		*name,
		 Bool		only_extended)
{
    XDeviceInfo	*devices;
    XDeviceInfo *found = NULL;
    int		loop;
    int		num_devices;
    int		len = strlen(name);
    Bool	is_id = True;
    XID		id = (XID)-1;

    for(loop=0; loop<len; loop++) {
	if (!isdigit(name[loop])) {
	    is_id = False;
	    break;
	}
    }

    if (is_id) {
	id = atoi(name);
    }

    devices = XListInputDevices(display, &num_devices);

    for(loop=0; loop<num_devices; loop++) {
	if ((!only_extended || (devices[loop].use >= IsXExtensionDevice)) &&
	    ((!is_id && strcmp(devices[loop].name, name) == 0) ||
	     (is_id && devices[loop].id == id))) {
	    if (found) {
	        fprintf(stderr,
	                "Warning: There are multiple devices named '%s'.\n"
	                "To ensure the correct one is selected, please use "
	                "the device ID instead.\n\n", name);
		return NULL;
	    } else {
		found = &devices[loop];
	    }
	}
    }
    return found;
}

int
getNextKey(void)
{
    
    XDeviceInfo		*info;
    
    Display *display;
    display = XOpenDisplay(NULL);
	
    
       
       

    
	Bool	handle_proximity = False;
	int	idx = 0;

	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    

	info = find_device_info(display, device, True);

	if (!info) {
	    //fprintf(stderr, "unable to find device '%s'\n", argv[idx]);
	    //return EXIT_FAILURE;
	} else {
	    if (register_events(display, info, device, handle_proximity)) {
		print_events(display);
	    }
	    else {
		fprintf(stderr, "no event registered...\n");
		//return EXIT_FAILURE;
	    }
	}
	return output;
	
    
    
}


void doWork(void) {
  
  int pingCount = 0;
  
  
  double randomNum = 0.0;
  char buffer[2];
  srand((unsigned)time(NULL));
  
  char command[10];


  
  while(1) {
    
  //printf("Output: %d \n",output);
  
 
  pthread_mutex_lock(&mut);
  while (output != 64) {
    pthread_cond_wait(&cond,&mut);
  }
  pthread_mutex_unlock(&mut);
  
  //output is 64 now, do work!
  
  ++pingCount;
  
  printf("Ping #%d \n",pingCount);
  
  system("xdotool click 1");
  
  if (randomMode) {

	strcpy(command,"sleep ");

	randomNum = rand()%150 + 1;

	//printf("randomNum: %f \n",randomNum);

	sprintf(buffer,"%f",randomNum/1000);

	printf("Delay: %s seconds. \n",buffer);

	strcat(command,buffer);

	//printf("Command: %s \n", command);
	
	system(command);
  }
  
  
  
  }

  
  
  

}


int main(int argc, char *argv[]) {
  
  
  printf("DOTA 2 SupaPing by Adam Martinez! \n");

  if (argc > 3) {
	printf("Too many command line arguments! \n");
	exit(2);
  }
  
  if (argc >= 2) {
	if (strcmp(argv[1],"-r")) {
		device = argv[1];
		printf("Device specified; using device %s. \n",device);
	}
	else {
		printf("Random mode enabled. \n");
		randomMode = 1;
	}
	if (argc == 3) {
		if (strcmp(argv[2],"-r")) {
			device = argv[2];
			printf("Device specified; using device %s. \n",device);
		}
		else {
				printf("Random mode enabled. \n");
				randomMode = 1;
		}
	}
		
  }
  


  else {
	printf("No device specified; using device %s. \n",device);
  }
  	
  
	
  
  pthread_t cm, dw;
  
  int thread_args = 1;
  
  int NUM_THREADS = 2;
  
  int rc,i;
  
  
  printf("Initializing input thread... \n");
  pthread_create(&cm, NULL, (void *) &getNextKey, (void *) &thread_args);
  
  printf("Initializing work thread... \n");
  pthread_create(&dw, NULL, (void *) &doWork, (void *) &thread_args);
  
  
  printf("Done! You can now hold ALT to SupaPing!  \n");
  
 
  
  
 
  rc = pthread_join(cm, NULL);
  assert(0 == rc);
  
  rc = pthread_join(dw,NULL);
  assert(0 == rc);
   
  
  
  
  
  
  return 0;
  
}
    
