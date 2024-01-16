#include "yolov5_tflite.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string>
#include <signal.h>
#include <pthread.h>
#include <linux/fb.h>
using namespace std;

cv::Mat frame;
cv::Mat pic;

struct framebuffer_info
{
    uint32_t bits_per_pixel;    // depth of framebuffer
    uint32_t xres_virtual;      // how many pixel in a row in virtual screen
};

struct framebuffer_info get_framebuffer_info ( const char *framebuffer_device_path );
void* _getChar(void* argv);

int main(int argc, char *argv[])
{
	pthread_t t1;
	pthread_create(&t1, NULL, _getChar, NULL);
	cv::Size2f frame_size;
	cv::VideoCapture camera(2);
	std::ofstream ofs("/dev/fb0");
	framebuffer_info fb_info = get_framebuffer_info("/dev/fb0");

	camera.set(cv::CAP_PROP_FRAME_WIDTH, 600);
    camera.set(cv::CAP_PROP_FRAME_HEIGHT, 600);
	camera.set(cv::CAP_PROP_FPS, 30);

    Prediction out_pred;
    const std::string model_path = argv[1];
    const std::string names_path = argv[2];

    std::vector<std::string> labelNames;

    YOLOV5 model;

    // conf
    model.confThreshold = 0.30;
    model.nmsThreshold = 0.40;
    model.nthreads = 4;
    // Load the saved_model
    model.loadModel(model_path);
    // Load names
    model.getLabelsName(names_path, labelNames);

    // save video config
    bool save = true;
    while ( true )
    {
        camera >> frame;
		int pixel = fb_info.bits_per_pixel;
		int x_max = fb_info.xres_virtual;

        // Predict on the input image
        model.run(frame, out_pred);

        // add the bbox to the image and save it
        auto boxes = out_pred.boxes;
        auto scores = out_pred.scores;
        auto labels = out_pred.labels;

        for (int i = 0; i < boxes.size(); i++)
        {
            auto box = boxes[i];
            auto score = scores[i];
            auto label = labels[i];
            cv::rectangle(frame, box, cv::Scalar(255, 0, 0), 2);
            cv::putText(frame, labelNames[label], cv::Point(box.x, box.y), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
        }
		pic = frame;
		frame_size = pic.size();
		cv::cvtColor(pic, pic, cv::COLOR_RGB2BGR565);
		
        out_pred = {};

        for ( int y = 0; y < frame_size.height; y++ )
        {
			
        	ofs.seekp(y * x_max*2 + ((x_max-frame_size.width)/2)*2 );
			ofs.write(reinterpret_cast<char*>(pic.ptr(y)), frame_size.width*2);
        }
    }
	camera.release ( );
    return 0;
}

struct framebuffer_info get_framebuffer_info(const char *framebuffer_device_path)
{
    struct framebuffer_info fb_info;        
    struct fb_var_screeninfo screen_info;

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
		if(a == 'a'){
			abort();
		}
	}
}
