#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <GL/gl.h>

void draw_depth(float *space){
   int i,x,y;

   glClear(GL_COLOR_BUFFER_BIT);

   glBegin(GL_POINTS);
   for(i = 0; i < 640*480; i++){
      y = i / 640;
      x = i % 640;
      glColor3f(space[i], space[i], space[i]);
      glVertex2f((float)x / 320.0 - 1.0, (float)y / -240.0 + 1.0);
   }
   glEnd();
   SDL_GL_SwapBuffers( );

}

void process_file(char *filename){
   SDL_Event event;
   int done = 0;
   float **space;
   int frame = 0;
   int i;
   FILE *depthfile;
   char *line = NULL;
   size_t size;
   int playing = 1;

  space = malloc(sizeof(float*) * 1000);

  depthfile = fopen(filename,"r");
  while(getline(&line, &size, depthfile) != -1){
     space[frame] = malloc(sizeof(float) * 640 * 480);

     space[frame][0] = atof(strtok(line, " "));
     for(i = 1; i < 640*480; i++){
        space[frame][i] = atof(strtok(NULL," \n"));
     }

     frame++;
  }
  fclose(depthfile);
  frame = 0;

   while(!done){
      /* Event handling */
      while( SDL_PollEvent( &event) ){
         switch(event.type){
            case SDL_KEYDOWN:
               if (event.key.keysym.sym == SDLK_ESCAPE) done=1;
               break;
            case SDL_KEYUP:
               if (event.key.keysym.sym == SDLK_SPACE) playing = !playing;
               if (event.key.keysym.sym == SDLK_LEFT)  frame--;
               if (event.key.keysym.sym == SDLK_RIGHT) frame++;
               break;
         }
      }

      draw_depth(space[frame]);

      if(playing){
printf("%d\n", frame);
         frame++;
      }

      if(frame == 1000) break;
   }

   /* Free resources */
   for(i = 0; i < frame; i++){
      free(space[i]);
   }

   free(space);

}

int main(int argc, char **argv){

   if(argc < 2){
      printf("Give me a file name\n");
      return(1);
   }

   SDL_Init(SDL_INIT_VIDEO);
   SDL_SetVideoMode(640,480,32, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_HWPALETTE);

   process_file(argv[1]);

   SDL_Quit();

   return EXIT_SUCCESS;
}
