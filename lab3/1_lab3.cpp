#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <linux/fb.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <sys/ioctl.h>
#include <string>
#include <pthread.h>
using namespace std;

char buf[100]; 
const char picName[37] = "/run/media/mmcblk1p1/screenshot/pic_";
const char bmp[5] = ".bmp";
int cnt = 0;
cv::Mat frame;
cv::Mat pic;
struct framebuffer_info
{
    uint32_t bits_per_pixel;    // depth of framebuffer
    uint32_t xres_virtual;      // how many pixel in a row in virtual screen
};

struct framebuffer_info get_framebuffer_info ( const char *framebuffer_device_path );
void* _getChar(void* argv);
int main ( int argc, const char *argv[] )
{
	pthread_t t1;
	pthread_create(&t1, NULL, _getChar, NULL);
    
	cv::Size2f frame_size;
	
    cv::VideoCapture camera(2);

	framebuffer_info fb_info = get_framebuffer_info("/dev/fb0");
	
    std::ofstream ofs("/dev/fb0");

    
    if( !camera.isOpened() )
    {
        std::cerr << "Could not open video device." << std::endl;
        return 1;
    }

	camera.set(cv::CAP_PROP_FRAME_WIDTH, 600);
    camera.set(cv::CAP_PROP_FRAME_HEIGHT, 600);
	camera.set(cv::CAP_PROP_FPS, 30);
	
	
    while ( true )
    {
		camera >> frame;
		frame_size = frame.size();
		
		pic = frame;
		cv::cvtColor(frame, frame, cv::COLOR_BGR2BGR565);
	
		
		int pixel = fb_info.bits_per_pixel;
		int x_max = fb_info.xres_virtual;
        // output the video frame to framebufer row by row
        for ( int y = 0; y < frame_size.height; y++ )
        {
        	ofs.seekp(y * x_max*2 + ((x_max-frame_size.width)/2)*2 );
			for(int x = 0; x < frame_size.width; x++){
				uchar *pixel_ptr = &frame.ptr<uchar>(y)[x*2];
				ofs.write((const char*)(pixel_ptr), sizeof(uchar)*2);
			}
        }
		
		

    }
	
    camera.release ( );

    return 0;
}

struct framebuffer_info get_framebuffer_info(const char *framebuffer_device_path)
{
    struct framebuffer_info fb_info;        // Used to return the required attrs.
    struct fb_var_screeninfo screen_info;   // Used to get attributes of the device from OS kernel.

    int fd = -1;
    fd = open(framebuffer_device_path, O_RDWR);
    if (fd >= 0) {
        if (!ioctl(fd, FBIOGET_VSCREENINFO, &screen_info)) {
            fb_info.xres_virtual = screen_info.xres_virtual;
            fb_info.bits_per_pixel = screen_info.bits_per_pixel;
        }
    }

    return fb_info;
};

void* _getChar(void* argv){
	char a;
	system("/bin/stty raw");
	while(1){
		a = getc(stdin);
		if(a == 'c'){
			sprintf(buf,"%s%d%s", picName, cnt, bmp);
			cv::imwrite(buf, pic);
			cnt++;
		}
		if(a == 'a'){
			abort();
		}
	}
}

