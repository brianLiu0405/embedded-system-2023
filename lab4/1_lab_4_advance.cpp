#include <fcntl.h> 
#include <fstream>
#include <linux/fb.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
int go = 100;
int start = 576000;
pthread_mutex_t mutex;
struct framebuffer_info
{
    uint32_t bits_per_pixel;    // framebuffer depth
    uint32_t xres_virtual;      // how many pixel in a row in virtual screen
    uint32_t yres_virtual;
};

struct framebuffer_info get_framebuffer_info(const char *framebuffer_device_path);
void* _getChar(void* argv);
int main(int argc, const char *argv[])
{
    cv::Mat image;
    cv::Size2f image_size;
    
    framebuffer_info fb_info = get_framebuffer_info("/dev/fb2");
    std::ofstream ofs("/dev/fb2");

	pthread_t t1;
	pthread_create(&t1, NULL, _getChar, NULL);
    
	
	// printf("height %d, width %d\n",image.size().height, image.size().width);
	printf("fb_info.xres_virtual : %d\n", fb_info.xres_virtual);
	printf("fb_info.yres_virtual : %d\n", fb_info.yres_virtual);
	printf("fb_info.bits_per_pixel : %d\n", fb_info.bits_per_pixel);
    
	int pixel = fb_info.bits_per_pixel;
	int x_max = fb_info.xres_virtual;
	image = cv::imread("picture.png");
    image_size = image.size();

	
	//cv::cvtColor(image, image, cv::COLOR_BGR2BGR565);

	while(1){
		for (int y = 350; y < image_size.height-300; y++)
		{
			if(start < 1920)
				ofs.seekp(y * x_max * 4 + (1920-start)*4);
			else
				ofs.seekp(y * x_max * 4);
			
			if(start < 1920){
				ofs.write(reinterpret_cast<char*>(image.ptr(y)), sizeof(uchar)*start*4);
			}
			else{
				ofs.write(reinterpret_cast<char*>(image.ptr(y))+(start%3840), sizeof(uchar)*1920*4);
			}
		}
		printf("%d\n", start);
		//pthread_mutex_lock(&mutex);
		start += go;
		//pthread_mutex_unlock(&mutex);
		
	}
    return 0;
}

struct framebuffer_info get_framebuffer_info(const char *framebuffer_device_path)
{
    struct framebuffer_info fb_info;        // Used to return the required attrs.
    struct fb_var_screeninfo screen_info;   // Used to get attributes of the device from OS kernel.

    // open device with linux system call "open()"
    // https://man7.org/linux/man-pages/man2/open.2.html

    // get attributes of the framebuffer device thorugh linux system call "ioctl()".
    // the command you would need is "FBIOGET_VSCREENINFO"
    // https://man7.org/linux/man-pages/man2/ioctl.2.html
    // https://www.kernel.org/doc/Documentation/fb/api.txt

    // put the required attributes in variable "fb_info" you found with "ioctl() and return it."
    // fb_info.xres_virtual =       // 8
    // fb_info.bits_per_pixel =     // 16

    int fd = -1;
    fd = open(framebuffer_device_path, O_RDWR);
    if (fd >= 0) {
        if (!ioctl(fd, FBIOGET_VSCREENINFO, &screen_info)) {
            fb_info.xres_virtual = screen_info.xres_virtual;
			fb_info.yres_virtual = screen_info.yres_virtual;
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
		if(a == 'j'){
			if(go != 100){
				go = 100;
				//pthread_mutex_lock(&mutex);
				start += 200;
				//pthread_mutex_unlock(&mutex);
			}
		}
		if(a == 'l'){
			if(go != -100){
				go = -100;
				//pthread_mutex_lock(&mutex);
				start -= 200;
				//pthread_mutex_unlock(&mutex);
			}
		}
		if(a == 'a'){
			abort();
		}
	}
}

