#include <fcntl.h> 
#include <fstream>
#include <linux/fb.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdint.h>

struct framebuffer_info
{
    uint32_t bits_per_pixel;    // framebuffer depth
    uint32_t xres_virtual;      // how many pixel in a row in virtual screen
};

struct framebuffer_info get_framebuffer_info(const char *framebuffer_device_path);

int main(int argc, const char *argv[])
{
    cv::Mat image;
    cv::Size2f image_size;
    
    framebuffer_info fb_info = get_framebuffer_info("/dev/fb0");
    std::ofstream ofs("/dev/fb0");

    // read image file (sample.bmp) from opencv libs.
    // https://docs.opencv.org/3.4.7/d4/da8/group__imgcodecs.html#ga288b8b3da0892bd651fce07b3bbd3a56
    // image = .......
    image = cv::imread("NYCU_logo.bmp");

    // get image size of the image.
    // https://docs.opencv.org/3.4.7/d3/d63/classcv_1_1Mat.html#a146f8e8dda07d1365a575ab83d9828d1
    // image_size = ......
    image_size = image.size();

	
    // transfer color space from BGR to BGR565 (16-bit image) to fit the requirement of the LCD
    // https://docs.opencv.org/3.4.7/d8/d01/group__imgproc__color__conversions.html#ga397ae87e1288a81d2363b61574eb8cab
    // https://docs.opencv.org/3.4.7/d8/d01/group__imgproc__color__conversions.html#ga4e0972be5de079fed4e3a10e24ef5ef0
	
	
	cv::cvtColor(image, image, cv::COLOR_BGR2BGR565);
	
	printf("height %d, width %d\n",image.size().height, image.size().width);
	printf("fb_info.xres_virtual : %d\n", fb_info.xres_virtual);
	printf("fb_info.bits_per_pixel : %d\n", fb_info.bits_per_pixel);
    
	int pixel = fb_info.bits_per_pixel;
	int x_max = fb_info.xres_virtual;

    for (int y = 0; y < image_size.height; y++)
    {
		ofs.seekp(y * x_max*2);
		for(int x = 0; x < image_size.width; x++){
			uchar *pixel_ptr = &image.ptr<uchar>(y)[x*2];
			ofs.write((const char*)(pixel_ptr), sizeof(uchar)*2);
		}
        // move to the next written position of output device framebuffer by "std::ostream::seekp()".
        // posisiotn can be calcluated by "y", "fb_info.xres_virtual", and "fb_info.bits_per_pixel".
        // http://www.cplusplus.com/reference/ostream/ostream/seekp/

        // write to the framebuffer by "std::ostream::write()".
        // you could use "cv::Mat::ptr()" to get the pointer of the corresponding row.
        // you also have to count how many bytes to write to the buffer
        // http://www.cplusplus.com/reference/ostream/ostream/write/
        // https://docs.opencv.org/3.4.7/d3/d63/classcv_1_1Mat.html#a13acd320291229615ef15f96ff1ff738
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
            fb_info.bits_per_pixel = screen_info.bits_per_pixel;
        }
    }

    return fb_info;
};
