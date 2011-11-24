#include <stdio.h>
#include <stdlib.h>
#include <libfreenect_sync.h>
#include <SDL.h>
#include <GL/gl.h>

void draw_depth(short *buffer, float *space){
   int i,x,y;
   float depth;

   glClear(GL_COLOR_BUFFER_BIT);

   glBegin(GL_POINTS);
   for(i = 0; i < 640*480; i++){
      y = i / 640;
      x = i % 640;
      depth = (float)buffer[i] / 2047.0;
      if(depth > 0.3) depth = 0.0;
      space[i] = depth;
      glColor3f(depth, depth, depth);
      glVertex2f((float)x / 320.0 - 1.0, (float)y / -240.0 + 1.0);
   }
   glEnd();
   SDL_GL_SwapBuffers( );

}

void process_kinect(){
   SDL_Event event;
   uint32_t time;
   short *buffer = NULL;
   int done = 0;
   float **space;
   int frame = 0;
   int i,j;

  space = malloc(sizeof(float*) * 1000);

   while(!done){
      /* Event handling */
      while( SDL_PollEvent( &event) ){
         switch(event.type){
            case SDL_KEYDOWN:
               if (event.key.keysym.sym == SDLK_ESCAPE) done=1;
               break;
         }
      }

      space[frame] = malloc(sizeof(float) * 640 * 480);

      freenect_sync_get_depth((void**)&buffer, &time, 0, FREENECT_DEPTH_11BIT);

      draw_depth(buffer, space[frame]);

      frame++;
      if(frame == 1000) break;
   }

   /* Write out to file */
   for(i = 0; i < frame; i++){
      for(j = 0; j < 640*480; j++){
         printf("%f ", space[i][j]);
      }
      printf("\n");
      free(space[i]);
   }

   free(space);

}

int main(int argc, char **argv){
   freenect_context *fctx;
   freenect_device  *fdev;

   /* Init Kinect */
   if(freenect_init(&fctx, NULL) < 0){
      printf("No Kinect :-(\n");
      return 1;
   }

   freenect_set_log_level(fctx, FREENECT_LOG_ERROR);
   freenect_select_subdevices(fctx, FREENECT_DEVICE_CAMERA);

   if(freenect_open_device(fctx, &fdev, 0) < 0){
      printf("No device :-(\n");
      return 2;
   }

   SDL_Init(SDL_INIT_VIDEO);
   SDL_SetVideoMode(640,480,32, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_HWPALETTE);

   process_kinect();

   SDL_Quit();

   /* Close Kinect */
   freenect_close_device(fdev);
   freenect_shutdown(fctx);

   return EXIT_SUCCESS;
}
