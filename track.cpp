
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
 
using namespace cv;
using namespace std;
 
// Convert to string
#define SSTR( x ) static_cast< std::ostringstream & >( \
( std::ostringstream() << std::dec << x ) ).str()
 
int main(int argc, char **argv)
{

	if(argc < 2)
	{
		cout<<"Usage : "<<argv[0]<<" [1 - 8]  [avi_file]"<<endl;
		cout<<" [1 - 8]    : Choose tracker"<<endl;
		cout<<"              1 : BOOSTING"<<endl;
		cout<<"              2 : MIL"<<endl;
		cout<<"              3 : KCF"<<endl;
		cout<<"              4 : TLD"<<endl;
		cout<<"              5 : MEDIANFLOW"<<endl;
		cout<<"              6 : GOTURN"<<endl;
		cout<<"              7 : CSRT"<<endl;
		cout<<"              8 : MOSSE"<<endl;
		cout<<" [avi_file] : file name of video"<<endl;
		cout<<"              no file name to open camera"<<endl;
		cout<<"Note : Tracker 6 need caffe prototxt and model"<<endl;
		return -1;
	}

    string trackerType = argv[1];
 
    Ptr<Tracker> tracker;
 
        if ((trackerType == "BOOSTING") || (trackerType=="1"))
	{
	    trackerType = "BOOSTING";
            tracker = TrackerBoosting::create();
	}
        if ((trackerType == "MIL") || (trackerType=="2"))
	{
	    trackerType = "MIL";
            tracker = TrackerMIL::create();
	}
        if ((trackerType == "KCF") || (trackerType=="3"))
	{
	    trackerType = "KCF";
            tracker = TrackerKCF::create();
	}
        if ((trackerType == "TLD") || (trackerType=="4"))
	{
	    trackerType = "TLD";	
            tracker = TrackerTLD::create();
	}
        if ((trackerType == "MEDIANFLOW") || (trackerType=="5"))
	{
	    trackerType = "MEDIANFLOW";
            tracker = TrackerMedianFlow::create();
	}
        if ((trackerType == "GOTURN") || (trackerType=="6"))
	{
	    trackerType = "GOTURN";
            tracker = TrackerGOTURN::create();
	}
	if ((trackerType == "CSRT") || (trackerType=="7"))
	{
	    trackerType = "CSRT";
	    tracker = TrackerCSRT::create();
	}
	if ((trackerType == "MOSSE") || (trackerType=="8"))
	{
	    trackerType = "MOSSE";
	    tracker = TrackerMOSSE::create();
	}

    cout<<"Select Tracker : "<<trackerType<<endl;

    VideoCapture video;
    if( argc <= 2) video.open(0);
    else video.open(argv[2]);
    
    if(!video.isOpened())
    {
        cout << "Could not read video file" << endl;
        return 1;
    }
     
    Mat frame;
    video.read(frame);
    Rect2d bbox(1, 1, 2, 2);
     
ROI:
    bbox = selectROI("SELECT RIO", frame, true, false);
    if(bbox.width==0 || bbox.height==0)
    {
        video.read(frame); //Maybe choose from another frame
        goto ROI;
    }

    rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 );
    imshow("Tracking", frame);
    destroyWindow("SELECT RIO");

    bool initialized = tracker->init(frame, bbox);
    if( !initialized )
    {
	cout<< "Could not init tracker!" <<endl;
	return -1;
    } 
	
    float avg_fps = 0.0;
	
    while(video.read(frame))
    {     
        double timer = (double)getTickCount();
        bool up = tracker->update(frame, bbox);
        float fps = getTickFrequency() / ((double)getTickCount() - timer);
        if(avg_fps==0.0)avg_fps = fps;
	else avg_fps = (avg_fps + fps)/2.0;
	    
        if (up)
        {
            rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 );
        }
        else
        {
            putText(frame, "LOST!", Point(10,80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
        }
         
        putText(frame, "Tracker : " + trackerType , Point(10,20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(100,255,50),2);
         
        putText(frame, "FPS : " + SSTR(int(fps)), Point(10,50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(100,255,50), 2);
 
        imshow("Tracking", frame);
         
        int k = waitKey(1);
        if((k == 27) || (k == 113) || (k ==81))
        {
            break;
        }
 
    }

    printf("Average FPS: %.3f\r\n", avg_fps);
    video.release();
    destroyAllWindows();
}
