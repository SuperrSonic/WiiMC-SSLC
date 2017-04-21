/************************************************************/
/*                   USB ios58 test                         */
/*                                                          */
/*                                                          */
/************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ogcsys.h>
#include <gccore.h>
#include <sys/dir.h>
#include <unistd.h>
#include <wiiuse/wpad.h>
#include <fcntl.h>
#include <debug.h>

#include <ogc/usbstorage.h>
#include <sdcard/wiisd_io.h>
#include <sdcard/gcsd.h>

#include <ogc/lwp_watchdog.h>

#include <fat.h>


#include <ogc/usbstorage.h>

const DISC_INTERFACE* usb = &__io_usbstorage;
const DISC_INTERFACE* sd = &__io_wiisd;


static bool reset_pressed = false;
static bool power_pressed = false;

#define USB_TEST_VERSION "1.12"

static int method=0;
static u64 timer_init=0;
void usb_log(char* format, ...);
void reset_usb_log();




static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

static void initialise_video() 
{
    VIDEO_Init();
    rmode = VIDEO_GetPreferredMode(NULL);
    xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
    console_init(xfb, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);
    VIDEO_Configure(rmode);
    VIDEO_SetNextFramebuffer(xfb);
    VIDEO_SetBlack(FALSE);
    VIDEO_Flush();
    VIDEO_WaitVSync(); 
    if (rmode->viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync(); 
    printf("\x1b[2;0H");
    VIDEO_WaitVSync();
}
/*
static void showdir(char *path)
{
  char filename[MAXPATHLEN];
  DIR_ITER *dp;
  struct stat fstat;

  dp = diropen(path);

  if(dp==NULL) 
  {
    printf("Error diropen\n");
    return;
  }
  else printf("Ok diropen\n");
  
  int cnt=0;
  while( dirnext( dp, filename, &fstat ) == 0 )
  {
    //if(cnt==20) break;
    cnt++;

    if( fstat.st_mode & S_IFDIR ) printf("/"); 
    printf("%s\n",filename);	  
    	
  }
  dirclose(dp);
  printf("\nEnd show dirrectory (%d).\n",cnt);
}

static void dump_hex(char *ptr, int len)
{
	int i,pos=0;
	char c;
	do
	{
		for(i=0;(i<16)&&(pos<len);i++)
		{
			c=ptr[pos];
			if(c<' ')c='.';
			//printf(" %02X (%c)",ptr[pos],c);
			printf("%c",c);
			pos++;
		}
		printf("\n");
	}
	while(pos<len);
	printf("----------------------------------------------------\n");
}
*/

static void reset_cb (void) {
	reset_pressed = true;
}

static void power_cb (void) {
	power_pressed = true;
}

void writelog()
{
	FILE *fp;
	char *log;
	char * getusblog();


	
	log=getusblog();
	
	

		fp=fopen("sd:/log_usb.txt","wb");

		if(fp!=0)
		{
			fprintf(fp,"USB2 device test version: %s\n",USB_TEST_VERSION);
			fprintf(fp,"=============================\n");		
			fwrite(log,1, strlen(log) ,fp);
				
			fclose(fp);
		}
	

}


static bool device_ok;

void test(int _method)
{
	printf("Testing...\n");
	usleep(5000000);
	
	void set_usb_method(int _method);

	if(timer_init>0)
	{
		printf ("\x1b[%d;%dH", 15, 1 );
		
		printf("Wake up test aborted\n");
	}

	timer_init=0;

	method=_method;

	set_usb_method(method);

	usb_log("USB2 device test method: %i\n------------------------------\n",method);
	

	if(usb->isInserted())
	{
		usb_log("USB Device Compatible!!!\n");
		device_ok=true;
	}
	else
	{
		usb_log("USB Device NOT Compatible!!!\n");
		device_ok=false;
	}
}

void InitialScreen()
{
	printf ("\x1b[2J"); //clear screen
	
	printf("\n");
	printf("USB2 device test version: %s\n",USB_TEST_VERSION);
	printf("=============================\n");

	printf("8 tests will be run.\n");
	printf("After each test, please unplug and replug your device when requested.\n");
	printf("The final one tests wakeup and will take 20 minutes to run.\n");
	printf("On exit application log will be saved to sd:/log_usb.txt\n");
	printf("Please upload this file to wiimc.org\n");
}

void enable_wakeup()
{
	timer_init=gettime();
	printf ("\x1b[2J"); //clear screen
	printf("\n\n\n\nWake up test in progress - press HOME to cancel test.\n\n");
}

void check_wakeup()
{
	if(timer_init==0) return;
	if(ticks_to_secs(gettime() - timer_init) > 20*60) //20 mins
	{
		char buf[2048];

		timer_init=0;
		printf("\n\nTesting wake up\n");
	
		if(usb->readSectors(512,1,buf)==false)
			usb_log("Error reading sector. Device wake up fail!!\n");
		else
			usb_log("OK reading sector. Device wake up OK!!\n");
		
	}
	else 
	{		
		printf ("\x1b[%d;%dH", 15, 1 ); //cursor position 
		printf("Wake up counter: %i                           ", (20*60)-(int)ticks_to_secs(gettime() - timer_init));
		fflush(stdout);
	}

}

void wait_a()
{
	u32 pressed = 0;
	
	while(!pressed && !reset_pressed)
	{	
		WPAD_ScanPads();
		pressed = WPAD_ButtonsDown(0);

		if ( (pressed & WPAD_BUTTON_A)) return;

		usleep(5000);		
	}
	reset_pressed=false;
}

void change_dev()
{
	printf("Unplug your USB device now. Press A when ready.\n");
	wait_a();
	printf("Plug in your USB device now. Press A when ready.\n");
	wait_a();
}

int main(int argc, char **argv) 
{
	void __exception_setreload(int t);	

	__exception_setreload(1);
	if(IOS_GetVersion()!=58) IOS_ReloadIOS(58);
	usleep(50000);
	
	initialise_video(); 
	
	CON_EnableGecko(1,0);
	SYS_SetResetCallback (reset_cb);
	SYS_SetPowerCallback(power_cb);

	if(IOS_GetVersion()!=58) 
	{
		printf("\n\nYou need ios58 installed.");
		sleep(4);
		return 0;
	}

	sd->startup();
	if(!fatMount("sd",sd,0,2,128) )
	{
		printf("\n\nYou need an SD card to run this test.");
		sleep(4);
		return 0;
	}

	WPAD_Init();
	
	InitialScreen();

	usleep(1000000);
	reset_usb_log();
	usb->startup();
		
	usleep(1000000);
	
	printf("Plug in your USB device now. Press A when ready.\n");
	wait_a();
	test(0);
	
	change_dev();
	test(1);
	
	change_dev();
	test(2);
	
	change_dev();
	test(3);
	
	change_dev();
	test(4);

	change_dev();
	test(5);
	
	change_dev();
#if 1
{
	void USBStorage_Deinitialize();
	usb_log("Doing IOSReload.\n");
	fatUnmount("sd");
	__io_wiisd.shutdown();
	__io_wiisd.shutdown();
	__io_usbstorage.shutdown();
	USBStorage_Deinitialize();
		
	WPAD_Shutdown();
	usleep(500*1000);
	USB_Deinitialize();
	usleep(50*1000);
	IOS_ReloadIOS(58);
	sleep(1);
	WPAD_Init();

	sd->startup();

	fatMount("sd",sd,0,2,128);
	usb->startup();
	usleep(500*1000);
}
#endif
	
	test(6);

	if(device_ok)
	{
		printf("DONT unplug your USB device. Press A when ready.\n");
		wait_a();
		enable_wakeup();
	}
	else printf("\nWake up test cancelled, device not compatible. Press HOME to exit.\n");
	while(!reset_pressed)
	{	
		WPAD_ScanPads();
		u32 pressed = WPAD_ButtonsDown(0);

		if ( pressed & WPAD_BUTTON_HOME ) break;

		check_wakeup();

		usleep(5000);
	}
	writelog();
	fatUnmount("sd:");
	printf("Exiting...\n");
	usleep(5000);

	return 0;
}
