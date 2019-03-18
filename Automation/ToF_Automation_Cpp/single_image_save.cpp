//
// ex-file_io.cpp
//
// Capture a frame from the camera, and write the data out to files. For
// exemplary purposes, we will write the amplitdue and radial distance images
// to PNG files and the point cloud to a PCD file.
//

#include <iostream>
#include <memory>
#include <opencv2/opencv.hpp>
#include <pcl/io/pcd_io.h>
#include <pcl/point_cloud.h>
#include <ifm3d/camera.h>
#include <ifm3d/fg.h>
#include <ifm3d/image.h>

std::string formatTimestamp(ifm3d::TimePointT timestamp)
{
    using namespace std::chrono;
    std::time_t time = std::chrono::system_clock::to_time_t(
        std::chrono::time_point_cast<std::chrono::system_clock::duration>(
          timestamp));

    milliseconds milli = duration_cast<milliseconds>(
          timestamp.time_since_epoch() - duration_cast<seconds>(
            timestamp.time_since_epoch()));

    std::ostringstrea/m s;
    s << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S")
      << ":" << std::setw(3) << std::setfill('0') << milli.count();

    return s.str();
}


int main(int argc, const char **argv)
{
  auto cam = ifm3d::Camera::MakeShared();

  ifm3d::ImageBuffer::Ptr img = std::make_shared<ifm3d::ImageBuffer>();
  ifm3d::FrameGrabber::Ptr fg =
    std::make_shared<ifm3d::FrameGrabber>(
      cam, ifm3d::IMG_AMP|ifm3d::IMG_CART|ifm3d::IMG_RDIS);

  if (! fg->WaitForFrame(img.get(), 1000))
    {
      std::cerr << "Timeout waiting for camera!" << std::endl;
      return -1;
    }

    ifm3d::TimePointT timestamp = img->TimeStamp();
  stringstream filename
  
    filename << "filename_"
              << formatTimestamp(timestamp)
              << ".pcd";
  
  pcl::io::savePCDFileASCII(filename, *(img->Cloud()));

  return 0;
}