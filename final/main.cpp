#include "yolov5_tflite.h"
#include <pthread.h>
std::vector<cv::Rect> boxes;
std::vector<float> scores;
std::vector<int> labels;
std::vector<std::string> labelNames;
cv::Mat frame;

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        std::cout << "\nError! Usage: <path to tflite model> <path to classes names> <path to input video> <path to output video>\n"
                  << std::endl;
        return 1;
    }

    Prediction out_pred;
    const std::string model_path = argv[1];
    const std::string names_path = argv[2];
    const std::string pic_path = argv[3];
    const std::string save_path = argv[4];

    YOLOV5 model;

    // conf
    model.confThreshold = 0.20;
    model.nmsThreshold = 0.40;
    model.nthreads = 4;
    // Load the saved_model
    model.loadModel(model_path);
    // Load names
    model.getLabelsName(names_path, labelNames);

    frame = cv::imread(pic_path);
    // Predict on the input image
    model.run(frame, out_pred);

    // add the bbox to the image and save it
    boxes = out_pred.boxes;
    scores = out_pred.scores;
    labels = out_pred.labels;

    for (int i = 0; i < boxes.size(); i++)
    {
        auto box = boxes[i];
        auto score = scores[i];
        auto label = labels[i];
        cv::rectangle(frame, box, cv::Scalar(255, 0, 0), 2);
        cv::putText(frame, labelNames[label], cv::Point(box.x, box.y), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
    }

    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
    cv::imwrite(save_path,frame);
}

