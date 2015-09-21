# 3D-Sails  
Nintendo 3ds Canvas (Get &amp; Set, RecRegions indivial Pixil) 

it passed the bar 

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
