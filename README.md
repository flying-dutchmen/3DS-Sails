# 3DS Sails 
  
####24bit "true-colour" Canvas for the Nintendo 3DS platform  
####versatile get & set for both pixil or rectangular-regions. With Text, Lines, Boxes, Poly, Circles & tilted  Ellipse.
####"Elegant" use of left, right, bottom & ... gfxFramebuffer's as your blank canvas.

All source Code with exception of "Dual_for" presented has undergone debuging & passed Eg. is functional/usable.  
The *.c Sources & snippets presented how ever broken, assume an intermediate++ knowledge "C" for thire usage!  
To steno-graph, laymen others a "glips &or new appreciation for the work involved in building an implemention" and not scarcely fathom just the *.h header/interface's! & if you don't like; Well then; it's walk the plank! 
  
& too many other authours && engineers && pro-liter-riot's mentioned here or not through out 
"A very sincere Thank-You"!  

S.V.P. Any Questions, requests &or proposals can be sent via email!   

to: kennyd.lee@outlook.com  
or Bcc undead.kennyd@gmail.com, KennyD@mail.com   
Subject: Nintendo 3DS Sails ...  

Etymology--> https://en.wikipedia.org/wiki/Canvas#Etymology   
The word "canvas" is derived from the 13th century Anglo-French canevaz and the Old French canevas. Both may be derivatives of the Vulgar Latin cannapaceus for "made of hemp," originating from the Greek κάνναβις (cannabis)

##Pixil usage/demo

	screenTopLeft = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL); 
	screenTopRight = gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL); 

//test pattern 1st pass  
for (i = 0; i < 79; ++i)  
 for (j = 0; j < 200; ++j)  
	{  
        SetCanvasPixel(screenTopLeft,i,j,RED);  
                                SetCanvasPixel(screenTopRight,i,j,RED);  
                                SetCanvasPixel(screenTopLeft,i+80,j,CYAN);  
                                SetCanvasPixel(screenTopRight,i+80,j,CYAN);  
                                SetCanvasPixel(screenTopLeft,i+160,j,YELLOW);  
                                SetCanvasPixel(screenTopRight,i+160,j,YELLOW);  
                                SetCanvasPixel(screenTopLeft,i+240,j,BLACK);  
                                SetCanvasPixel(screenTopRight,i+240,j,BLACK);  
                                SetCanvasPixel(screenTopLeft,i+320,j,BLUE);  
                                SetCanvasPixel(screenTopRight,i+320,j,BLUE);  
				};  
//test pattern 2nd pass   
for (i = 0; i < 99; ++i)  
 for (j = 201; j < 239; ++j)  
	{  
                                SetCanvasPixel(screenTopLeft,i,j,PINK);  
                                SetCanvasPixel(screenTopRight,i,j,PINK);  
                                SetCanvasPixel(screenTopLeft,i+100,j,LIGHT_GREEN);  
                                SetCanvasPixel(screenTopRight,i+100,j,LIGHT_GREEN);  
                                SetCanvasPixel(screenTopLeft,i+200,j,WHITE);  
                                SetCanvasPixel(screenTopRight,i+200,j,WHITE);  
                                SetCanvasPixel(screenTopLeft,i+300,j,ORANGE);  
                                SetCanvasPixel(screenTopRight,i+300,j,ORANGE);  
				}  

				gfxFlushBuffers();
				//gfxSwapBuffers();

As scene here --> https://www.facebook.com/1596037147331761/photos/a.1629176367351172.1073741828.1596037147331761/1636255289976613/?type=1&theater

##Pixil usage/demo II  
   switch(GetCanvasPixel(screenTopLeft,x,y))  
  {   
 	case RED: text_print(screenBottom, "Sampled a RED",100,120); break;  
	case GREEN: text_print(screenBottom, "Sampled a GREEN",100,120); break;  
 	case BLUE: text_print(screenBottom, "Sampled a BLUE",100,120); break;  
 	case CYAN: text_print(screenBottom, "Sampled a CYAN",100,120); break;  
 	case PINK: text_print(screenBottom, "Sampled a PINK",100,120); break;  
 	case YELLOW: text_print(screenBottom, "Sampled a YELLOW",100,120); break;  
 	case BLACK: text_print(screenBottom, "Sampled a BLACK",100,120); break;  
 	case WHITE: text_print(screenBottom, "Sampled a WHITE",100,120); break;   
 	case ORANGE: text_print(screenBottom, "Sampled a ORANGE",100,120); break;  
 	case LIGHT_GREEN: text_print(screenBottom, "Sampled a LIGHT GREEN",100,120); break;   
 	case PURPLE: text_print(screenBottom, "Sampled a PURPLE",100,120); break;  
	default:   
                ClearCanvas(screenBottom, BLACK);  
		break;  
  }  
  
##Primitive shapes usage/demo  
//Cracker; and the world is full of them; want a polygon?  

  				line(screenTopLeft, 0, 239, 399, 0, PINK);  
			  	line(screenTopRight, 0, 0, 399, 239, PINK);  
				Circle(screenTopLeft, 50, 50, 10, BLUE); //needs slight overhall    
				Circle(screenTopRight, 349, 50, 10, BLUE); //& finalized     
				Ellipse(screenTopLeft, 50, 150, 20, 10, GREEN); //macro is too-be revised    
				Ellipse(screenTopRight, 349, 150, 20, 10, GREEN); //& placed back in finaldraft   

				gfxFlushBuffers();  

As scene here -->https://www.facebook.com/1596037147331761/photos/a.1629176367351172.1073741828.1596037147331761/1636836319918510/?type=1&theater

##Primitive shapes usage/demo II

   int * shape[6];   /* our polygon array */

    shape[0] = 20;           /* first vertex */
    shape[1] = 20;
    shape[2] = 20;    /* second vertex */
    shape[3] = 60;
    shape[4] = 140;    /* third vertex */
    shape[5] = 80;
  
//   shape[4] = shape[0];      /* our's automatically close */  
 //  shape[5] = shape[1];      /* the polygon, so we don't need bother*/  
 
    poly(screenTopLeft, shape, 5, LIGHT_GREEN);    
    poly(screenTopRight, shape, 5, LIGHT_GREEN);  
    
   box(screenTopLeft, 70, 90, 260, 40, WHITE);   
   box(screenTopRight, 70, 90, 260, 40, WHITE);   

As scene here -->https://www.facebook.com/1596037147331761/photos/a.1629176367351172.1073741828.1596037147331761/1638563179745824/?type=3&theater

##Rectangular-Region usage/demo

//Proforms memory clean-up all cases. What if wii need in 1/2 time repeated usage of ...
SetRecRe(screenBottom,10,10,80,80,GetRecRe(screenTopLeft,10,10,80,80));

##Rectangular-Region extended usage/demo II 
//still in debug

//u8* Recbufl = (u8*)linearMemAlign(bitmapsize, 0x8);  
Recbufl= (u8*)GetRecEx(screenTopLeft,10,10,80,80,linear); // patterns & tiles ?  
//& about args 7th boolean cleanup, 8th RamType (1=vram or 0=linear)    
SetRecEx(screenBottom,10,10,80,80, Recbufl, 0, linear);    
//looks like wii Have to GetRecEx(buff) over & ... anyways? vram !work && freeze 3ds  
//SetRecEx(screenBottom,10,100,80,80, Recbufl, 0, linear); repeated or subsiquent calls have no-eval  
linearFree(Recbufl); //manual CleanUp

#Ps. Mysterio is nothing more than pawn,    
#So tell your %^#@ing Spider-Woman!!!
