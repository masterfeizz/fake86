/*
  Fake86: A portable, open-source 8086 PC emulator.
  Copyright (C)2010-2013 Mike Chambers

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

/* input.c: functions for translation of SDL scancodes to BIOS scancodes,
   and handling of SDL events in general. */

//#include <SDL/SDL.h>
#include <stdint.h>
#include "sermouse.h"

#include <3ds.h>
#include <stdio.h>
uint8_t keydown[0x100], keyboardwaitack = 0;
extern uint32_t usegrabmode;

extern void doirq (uint8_t irqnum);
extern uint8_t running, portram[0x10000];
//extern SDL_Surface *screen;

uint8_t translatescancode (uint16_t keyval) {
	switch (keyval) {
			case 0x1B:
				return (1);
				break; //Esc
			case 0x30:
				return (0xB);
				break; //zero
			case 0x31:
			case 0x32:
			case 0x33:
			case 0x34:
			case 0x35:
			case 0x36:
			case 0x37:
			case 0x38:
			case 0x39:
				return (keyval - 0x2F);
				break; //other number keys
			case 0x2D:
				return (0xC);
				break; //-_
			case 0x3D:
				return (0xD);
				break; //=+
			case 0x8:
				return (0xE);
				break; //backspace
			case 0x9:
				return (0xF);
				break; //tab
			case 0x71:
				return (0x10);
				break;
			case 0x77:
				return (0x11);
				break;
			case 0x65:
				return (0x12);
				break;
			case 0x72:
				return (0x13);
				break;
			case 0x74:
				return (0x14);
				break;
			case 0x79:
				return (0x15);
				break;
			case 0x75:
				return (0x16);
				break;
			case 0x69:
				return (0x17);
				break;
			case 0x6F:
				return (0x18);
				break;
			case 0x70:
				return (0x19);
				break;
			case 0x5B:
				return (0x1A);
				break;
			case 0x5D:
				return (0x1B);
				break;
			case 0xD:
			case 0x10F:
				return (0x1C);
				break; //enter
			case 0x131:
			case 0x132:
				return (0x1D);
				break; //ctrl
			case 0x61:
				return (0x1E);
				break;
			case 0x73:
				return (0x1F);
				break;
			case 0x64:
				return (0x20);
				break;
			case 0x66:
				return (0x21);
				break;
			case 0x67:
				return (0x22);
				break;
			case 0x68:
				return (0x23);
				break;
			case 0x6A:
				return (0x24);
				break;
			case 0x6B:
				return (0x25);
				break;
			case 0x6C:
				return (0x26);
				break;
			case 0x3B:
				return (0x27);
				break;
			case 0x27:
				return (0x28);
				break;
			case 0x60:
				return (0x29);
				break;
			case 0x130:
				return (0x2A);
				break; //left shift
			case 0x5C:
				return (0x2B);
				break;
			case 0x7A:
				return (0x2C);
				break;
			case 0x78:
				return (0x2D);
				break;
			case 0x63:
				return (0x2E);
				break;
			case 0x76:
				return (0x2F);
				break;
			case 0x62:
				return (0x30);
				break;
			case 0x6E:
				return (0x31);
				break;
			case 0x6D:
				return (0x32);
				break;
			case 0x2C:
				return (0x33);
				break;
			case 0x2E:
				return (0x34);
				break;
			case 0x2F:
				return (0x35);
				break;
			case 0x12F:
				return (0x36);
				break; //right shift
			case 0x13C:
				return (0x37);
				break; //print screen
			case 0x133:
			case 0x134:
				return (0x38);
				break; //alt
			case 0x20:
				return (0x39);
				break; //space
			case 0x12D:
				return (0x3A);
				break; //caps lock
			case 0x11A:
			case 0x11B:
			case 0x11C:
			case 0x11D:
			case 0x11E:
			case 0x11F:
			case 0x120:
			case 0x121:
			case 0x122:
			case 0x123:
				return (keyval - 0x11A + 0x3B);
				break; //F1 to F10
			case 0x12C:
				return (0x45);
				break; //num lock
			case 0x12E:
				return (0x46);
				break; //scroll lock
			case 0x116:
			case 0x107:
				return (0x47);
				break; //home
			case 0x111:
			case 0x108:
				return (0x48);
				break; //up
			case 0x118:
			case 0x109:
				return (0x49);
				break; //pgup
			case 0x10D:
				return (0x4A);
				break; //keypad -
			case 0x114:
			case 0x104:
				return (0x4B);
				break; //left
			case 0x105:
				return (0x4C);
				break; //center
			case 0x113:
			case 0x106:
				return (0x4D);
				break; //right
			case 0x10E:
				return (0x4E);
				break; //keypad +
			case 0x117:
			case 0x101:
				return (0x4F);
				break; //end
			case 0x112:
			case 0x102:
				return (0x50);
				break; //down
			case 0x119:
			case 0x103:
				return (0x51);
				break; //pgdn
			case 0x115:
			case 0x100:
				return (0x52);
				break; //ins
			case 0x7F:
			case 0x10A:
				return (0x53);
				break; //del
			default:
				return (0);
		}
}

uint8_t buttons = 0;
extern void sermouseevent (uint8_t buttons, int8_t xrel, int8_t yrel);
extern struct sermouse_s sermouse;
extern void setwindowtitle (uint8_t *extra);

void mousegrabtoggle() {
	/*if (usegrabmode == SDL_GRAB_ON) {
			usegrabmode = SDL_GRAB_OFF;
			SDL_WM_GrabInput (SDL_GRAB_OFF);
			SDL_ShowCursor (SDL_ENABLE);
			setwindowtitle ("");
		}
	else {
			usegrabmode = SDL_GRAB_ON;
			SDL_WM_GrabInput (SDL_GRAB_ON);
			SDL_ShowCursor (SDL_DISABLE);
			setwindowtitle (" (press Ctrl + Alt to release mouse)");
		}*/
}

extern uint8_t scrmodechange;
extern uint32_t usefullscreen;
void handleinput() {
	//SDL_Event event;
	int mx = 0, my = 0;
	uint8_t tempbuttons;
	uint32_t kDown = hidKeysDown();
	/*if(kDown)
	{
		if(kDown & KEY_START)
			portram[0x60] = 0x1C; portram[0x64] |= 2;

		doirq (1);
	}*/
	uint32_t kUp = hidKeysDown();
	/*if(kUp)
	{
		if(kUp & KEY_START)
			portram[0x60] = 0x1C | 0x80; portram[0x64] |= 2;

		doirq (1);
	}*/
	/*if (SDL_PollEvent (&event) ) {
			switch (event.type) {
					case SDL_KEYDOWN:
						portram[0x60] = translatescancode (event.key.keysym.sym);
						portram[0x64] |= 2;
						doirq (1);
						//printf("%02X\n", translatescancode(event.key.keysym.sym));
						keydown[translatescancode (event.key.keysym.sym) ] = 1;
						if (keydown[0x38] && keydown[0x1D] && (SDL_WM_GrabInput (SDL_GRAB_QUERY) == SDL_GRAB_ON) ) {
								keydown[0x1D] = 0;
								keydown[0x32] = 0;
								mousegrabtoggle();
								break;
							}
						if (keydown[0x38] && keydown[0x1C]) {
								keydown[0x1D] = 0;
								keydown[0x38] = 0;
								if (usefullscreen) usefullscreen = 0;
								else usefullscreen = SDL_FULLSCREEN;
								scrmodechange = 1;
								break;
							}
						break;
					case SDL_KEYUP:
						portram[0x60] = translatescancode (event.key.keysym.sym) | 0x80;
						portram[0x64] |= 2;
						doirq (1);
						keydown[translatescancode (event.key.keysym.sym) ] = 0;
						break;
					case SDL_MOUSEBUTTONDOWN:
						if (SDL_WM_GrabInput (SDL_GRAB_QUERY) == SDL_GRAB_OFF) {
								mousegrabtoggle();
								break;
							}
						tempbuttons = SDL_GetMouseState (NULL, NULL);
						if (tempbuttons & 1) buttons = 2;
						else buttons = 0;
						if (tempbuttons & 4) buttons |= 1;
						sermouseevent (buttons, 0, 0);
						break;
					case SDL_MOUSEBUTTONUP:
						if (SDL_WM_GrabInput (SDL_GRAB_QUERY) == SDL_GRAB_OFF) break;
						tempbuttons = SDL_GetMouseState (NULL, NULL);
						if (tempbuttons & 1) buttons = 2;
						else buttons = 0;
						if (tempbuttons & 4) buttons |= 1;
						sermouseevent (buttons, 0, 0);
						break;
					case SDL_MOUSEMOTION:
						if (SDL_WM_GrabInput (SDL_GRAB_QUERY) == SDL_GRAB_OFF) break;
						SDL_GetRelativeMouseState (&mx, &my);
						sermouseevent (buttons, (int8_t) mx, (int8_t) my);
						SDL_WarpMouse (screen->w / 2, screen->h / 2);
						while (1) {
								SDL_PollEvent (&event);
								SDL_GetRelativeMouseState (&mx, &my);
								if ( (mx == 0) && (my == 0) ) break;
							}
						break;
					case SDL_QUIT:
						running = 0;
						break;
					default:
						break;
				}
		}*/
}


u16* touchpadOverlay;
u16* keyboardOverlay;
char lastKey = 0;
int tmode;
u16* tfb;
touchPosition oldtouch, touch;
u64 tick;

//Touchscreen mode identifiers
#define TMODE_TOUCHPAD 1
#define TMODE_KEYBOARD 2

//Keyboard is currently laid out on a 14*4 grid of 20px*20px boxes for lazy implementation
char keymap[14 * 6] = {
  0x01, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 0,
  41, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
  15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 43,
  0, 30 , 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 28, 28,
  42, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 0, 72, 0,
  0, 0 , 0, 0, 57, 57, 57, 57, 57, 57, 0, 75, 80, 77
};

void Touch_Init(){
    tmode = TMODE_TOUCHPAD; //Start in touchpad Mode
    tfb = (u16*)gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);

    //Load overlay files from sdmc for easier testing
    FILE *texture = fopen("touchpadOverlay.bin", "rb");
    if(!texture){
        printf("Could not open touchpadOverlay.bin\n");
        goto ERROR;
    }
    fseek(texture, 0, SEEK_END);
    int size = ftell(texture);
    fseek(texture, 0, SEEK_SET);
    touchpadOverlay = malloc(size);
    fread(touchpadOverlay, 1, size, texture);
    fclose(texture);

    texture = fopen("keyboardOverlay.bin", "rb");
    if(!texture){
        printf("Could not open keyboardOverlay.bin\n");
        goto ERROR;
    }
    fseek(texture, 0, SEEK_END);
    size = ftell(texture);
    fseek(texture, 0, SEEK_SET);
    keyboardOverlay = malloc(size);
    fread(keyboardOverlay, 1, size, texture);
    fclose(texture);

    return;

    ERROR:
    printf("Press START to exit\n");
    while(1){
        hidScanInput();
        uint32_t kDown = hidKeysDown();
        if (kDown & KEY_START)
            break;
    }
    gfxExit();
    exit(1);
}

void Touch_TouchpadUpdate(){
    if(hidKeysDown() & KEY_TOUCH){
        hidTouchRead(&oldtouch);
    }

    if(hidKeysHeld() & KEY_TOUCH){
        hidTouchRead(&touch);
        //mousex += (touch.px - oldtouch.px)*10;
        //mousey += (touch.py - oldtouch.py)*10;
        oldtouch = touch;
    }

    if(hidKeysUp() & KEY_TOUCH){
        if(touch.py > 195 && touch.py < 240 && touch.px > 270 && touch.px < 320){
            tmode = 2;
            Touch_DrawOverlay();
        }
    }
}

int shiftToggle = 0;

void Touch_KeyboardUpdate(){

    if(hidKeysUp() & KEY_TOUCH){
        if(touch.py > 195 && touch.py < 240 && touch.px > 270 && touch.px < 320){
            tmode = 1;
            Touch_DrawOverlay();
        }
    }

    char key = 0;
    if(hidKeysHeld() & KEY_TOUCH){

        hidTouchRead(&touch);
        if(touch.py > 5 && touch.py < 138 && touch.px > 5 && touch.px < 315){
            key = keymap[((touch.py - 6) / 22) * 14 + (touch.px - 6)/22];
        }
    }

    if(lastKey != key && lastKey != 0){
        portram[0x60] = lastKey | 0x80; portram[0x64] |= 2;
        lastKey = 0;
        doirq(1);
        return;
    }

    if(key == 42 && lastKey != key)
    {
    	shiftToggle = !shiftToggle;
    	if(shiftToggle){
    		portram[0x60] = key; portram[0x64] |= 2;
    	}

    	else{
    		portram[0x60] = key | 0x80; portram[0x64] |= 2;
    	}
    }

    else if(key !=0 && lastKey != key){
        portram[0x60] = key; portram[0x64] |= 2;
        lastKey = key;
        doirq(1);
    }
}

void Touch_Update(){
    if(tmode == TMODE_TOUCHPAD)
        Touch_TouchpadUpdate();
    else
        Touch_KeyboardUpdate();
}

void Touch_DrawOverlay(){
    u16* overlay = 0;
    if(tmode == TMODE_TOUCHPAD)
        overlay = touchpadOverlay;
    else
        overlay = keyboardOverlay;

    if(!overlay) return;
  
    int x,y;

    for(x=0; x<320; x++){
        for(y=0; y<240;y++){
            tfb[(x*240 + (239 - y))] = overlay[(y*320 + x)];
        }
    }

    if(tmode == TMODE_KEYBOARD && shiftToggle == 1){
        for(x=20; x<24; x++){
            for(y=98; y<102;y++){
                tfb[(x*240 + (239 - y))] = RGB8_to_565(0,255,0);
            }
        }
    }
}