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


#include <X11/extensions/XInput.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "input.h"


#define INVALID_EVENT_TYPE      -1


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

char *device = "11";

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

      

	
      if (output) pthread_cond_broadcast(&cond);
	    
      pthread_mutex_unlock(&mut);
	


	
      if (Event.type == key_release_type) {
	output = -1*output;  
	//std::cout << "Key released.\n";
      }


      //printf("%d\n",output);

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

void *
getNextKey(void *)
{
    
  XDeviceInfo		*info;
    
  Display *display;
  display = XOpenDisplay(NULL);
	
    
    
  Bool	handle_proximity = False;
	

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
  //return output;
	
}    
    









//Input Class Functions


input::input(char *chosenDevice) {

  if(chosenDevice != NULL) {
    device = chosenDevice;
  }
  else
    printf("Using default device 12.\n");

  int thread_args = 1;

  pthread_create(&cm, NULL, &getNextKey, (void *) &thread_args);

  newOutput = output;
}


input::~input(void) {

  pthread_cancel(cm);

  pthread_join(cm,NULL);

  printf("Deconstructing input object......\n");
}



void input::waitUntilChange(void) {

    pthread_mutex_lock(&mut);  
    
    while(newOutput == output)
      pthread_cond_wait(&cond,&mut);

    pthread_mutex_unlock(&mut);

    newOutput = output;

}


void input::waitUntilKey(int key) {

  pthread_mutex_lock(&mut);

  while(output != key || output == (-1 * output))
    pthread_cond_wait(&cond, &mut);

  pthread_mutex_unlock(&mut);

}

int input::returnLastKey(void) {

  return output;

}



  

  
      

    

      
