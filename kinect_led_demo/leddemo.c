#include <stdio.h>
#include <stdlib.h>
#include <libfreenect_sync.h>

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

   /* Set led color */
   if(freenect_sync_set_led(LED_GREEN, 0) != 0){
      printf("Setting led color failed :-(\n");
   }

   /* Close Kinect */
   freenect_close_device(fdev);
   freenect_shutdown(fctx);

   return EXIT_SUCCESS;
}
