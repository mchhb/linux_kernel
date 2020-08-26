#include "videodevice.h"
#include "config.h"
#include <unistd.h>
#include <stdio.h>
#include <QProcess>
#include <QDebug>
/*
参考开源中国社区https://www.oschina.net/code/snippet_124925_3789
*/
VideoDevice::VideoDevice(QString dev_name)
{
    this->dev_name = dev_name;
    this->fd = -1;
    this->buffers = NULL;
    this->n_buffers = 0;
    this->index = -1;

}

//打开设备
int VideoDevice::open_device()
{
    //读写方式打开
    fd = open(dev_name.toStdString().c_str(), O_RDWR/*|O_NONBLOCK*/, 0);

    if(-1 == fd)
    {
        emit display_error(tr("open: %1").arg(QString(strerror(errno))));
        return -1;
    }
    return 0;
}

//关闭设备
int VideoDevice::close_device()
{
    if(-1 == close(fd))
    {
        // emit display_error(tr("close: %1").arg(QString(strerror(errno))));
        return -1;
    }
    return 0;
}

//初始化设备
int VideoDevice::init_device()
{
    struct v4l2_capability cap;//设备能力
    struct v4l2_cropcap cropcap;
    struct v4l2_crop crop;
    struct v4l2_format fmt;//分辨率、图像采集格式

    enum v4l2_buf_type  v4l2type;

    v4l2type = V4L2_BUF_TYPE_VIDEO_CAPTURE;//v4l2格式

    //VIDIOC_STREAMOFF:关闭视频流
    if(-1 == ioctl(fd, VIDIOC_STREAMOFF, &v4l2type))
    {
        emit display_error(tr("fail"));
    }

    //VIDIOC_QUERYCAP:获取设备支持的操作/能力
    if(-1 == ioctl(fd, VIDIOC_QUERYCAP, &cap))
    {
        if(EINVAL == errno)
        {
            emit display_error(tr("%1 is no V4l2 device").arg(dev_name));
        }
        else
        {
            emit display_error(tr("VIDIOC_QUERYCAP: %1").arg(QString(strerror(errno))));
        }
        return -1;
    }
    //打印摄像头信息
    printf("Driver Name:%s\n Card Name:%s\n Bus info:%s\n version:%d\n capabilities:%x\n \n ", cap.driver, cap.card, cap.bus_info,cap.version,cap.capabilities);

    //判断是否具有video capture能力
    if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
    {
        emit display_error(tr("%1 is no video capture device").arg(dev_name));
        return -1;
    }

    //判断是否具有视频流能力
    if(!(cap.capabilities & V4L2_CAP_STREAMING))
    {
        emit display_error(tr("%1 does not support streaming i/o").arg(dev_name));
        return -1;
    }

    //判断是否支持捕获
    if ((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == V4L2_CAP_VIDEO_CAPTURE)
    {
        printf("Device %s: supports capture.\n",NULL);
    }

    //判断是否支持视频流
    if ((cap.capabilities & V4L2_CAP_STREAMING) == V4L2_CAP_STREAMING)
    {
        printf("Device %s: supports streaming.\n",NULL);

    }

    CLEAR(cropcap);

    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if(0 == ioctl(fd, VIDIOC_CROPCAP, &cropcap))
    {
        CLEAR(crop);
        crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        crop.c = cropcap.defrect;

        if(-1 == ioctl(fd, VIDIOC_S_CROP, &crop))
        {
            if(EINVAL == errno)
            {
                //emit display_error(tr("VIDIOC_S_CROP not supported"));
            }
            else
            {
                emit display_error(tr("VIDIOC_S_CROP: %1").arg(QString(strerror(errno))));
                return -1;
            }
        }
    }
    else
    {
        emit display_error(tr("VIDIOC_CROPCAP: %1").arg(QString(strerror(errno))));
        return -1;
    }

    CLEAR(fmt);//清除格式

    memset(&fmt, 0, sizeof(struct v4l2_format));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = CAPTURE_WIDTH;//设置摄像头的采集的宽
    fmt.fmt.pix.height = CAPTURE_HIGHT;//设置摄像头的采集的高
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

    //设置fmt格式，不成功则返回-1
    if(-1 == ioctl(fd, VIDIOC_S_FMT, &fmt))
    {
        emit display_error(tr("VIDIOC_S_FMT").arg(QString(strerror(errno))));
        return -1;
    }

    //获取设置后的捕获分辨率格式信息
    v4l2_format Format;
    Format.type= V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(ioctl(fd,VIDIOC_G_FMT,&Format)==-1)
    {
        perror("ioctl");
        exit(EXIT_FAILURE);
    }
    printf(">>:  %d * %d\n",Format.fmt.pix.width,Format.fmt.pix.height);
    printf("pix.pixelformat:%c%c%c%c\n", \
           Format.fmt.pix.pixelformat & 0xFF,\
           (Format.fmt.pix.pixelformat >> 8) & 0xFF, \
           (Format.fmt.pix.pixelformat >> 16) & 0xFF,\
           (Format.fmt.pix.pixelformat >> 24) & 0xFF);


    //帧大小枚举结构体与流相关（帧）结构体
    struct v4l2_frmsizeenum fszenum;
    struct v4l2_streamparm stream_parm;

    fszenum.index = 0;
    int capture_mode = -1;
    fszenum.pixel_format = V4L2_PIX_FMT_YUYV;

    //VIDIOC_ENUM_FRAMESIZES:列举所有支持帧大小
    if(-1 == ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &fszenum))
    {
        emit display_error(tr("fszenum").arg(QString(strerror(errno))));
    }
    while (ioctl (fd, VIDIOC_ENUM_FRAMESIZES, &fszenum)== 0 ){
        //if(fszenum.type == V4L2_FRMSIZE_TYPE_DISCRETE)
        //判断设置帧的大小，设置驱动摄像头的采集像素大小
        if (fszenum.discrete.width == fmt.fmt.pix.width && fszenum.discrete.height == fmt.fmt.pix.height) {
            capture_mode = fszenum.index;
            break;
        }
        fszenum.index ++;//下标加1
    }

    if(capture_mode < 0)
    {
        emit display_error(tr("can't support resolution").arg(QString(strerror(errno))));
        return -1;
    }

    //将stream_parm清零
    memset(&stream_parm, 0, sizeof(struct v4l2_streamparm));

    stream_parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    stream_parm.parm.capture.capturemode = capture_mode;

    stream_parm.parm.capture.timeperframe.denominator = 30; /* 1/30帧 */
    stream_parm.parm.capture.timeperframe.numerator = 1;

    if(-1 == ioctl(fd, VIDIOC_S_PARM, &stream_parm))//设置格式
    {
        emit display_error(tr("VIDIOC_S_PARM").arg(QString(strerror(errno))));
        return -1;
    }



    if(-1 == init_mmap())
    {
        return -1;
    }

    return 0;
}

//初始化buffer
int VideoDevice::init_mmap()
{
    v4l2_requestbuffers req;//申请buffer
    CLEAR(req);

    req.count = 4;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if(-1 == ioctl(fd, VIDIOC_REQBUFS, &req))//向驱动提出申请内存的请求
    {
        if(EINVAL == errno)
        {
            emit display_error(tr("%1 does not support memory mapping").arg(dev_name));
            return -1;
        }
        else
        {
            emit display_error(tr("VIDIOC_REQBUFS %1").arg(QString(strerror(errno))));
            return -1;
        }
    }

    if(req.count < 2)
    {
        emit display_error(tr("Insufficient buffer memory on %1").arg(dev_name));
        return -1;
    }

    buffers = (buffer*)calloc(req.count, sizeof(*buffers));

    if(!buffers)
    {
        emit display_error(tr("out of memory"));
        return -1;
    }

    for(n_buffers = 0; n_buffers < req.count; ++n_buffers)
    {
        v4l2_buffer buf;
        CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = n_buffers;

        if(-1 == ioctl(fd, VIDIOC_QUERYBUF, &buf))
        {
            emit display_error(tr("VIDIOC_QUERYBUF: %1").arg(QString(strerror(errno))));
            return -1;
        }

        buffers[n_buffers].length = buf.length;
        buffers[n_buffers].start =
                mmap(NULL, // start anywhere
                     buf.length,
                     PROT_READ | PROT_WRITE,
                     MAP_SHARED,
                     fd, buf.m.offset);

        if(MAP_FAILED == buffers[n_buffers].start)
        {
            emit display_error(tr("mmap %1").arg(QString(strerror(errno))));
            return -1;
        }
    }
    return 0;

}

//开始捕获
int VideoDevice::start_capturing()
{
    unsigned int i;
    for(i = 0; i < n_buffers; ++i)
    {
        v4l2_buffer buf;
        CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory =V4L2_MEMORY_MMAP;
        buf.index = i;
        //        fprintf(stderr, "n_buffers: %d\n", i);

        if(-1 == ioctl(fd, VIDIOC_QBUF, &buf))
        {
            emit display_error(tr("VIDIOC_QBUF: %1").arg(QString(strerror(errno))));
            return -1;
        }
    }

    v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if(-1 == ioctl(fd, VIDIOC_STREAMON, &type))
    {
        emit display_error(tr("VIDIOC_STREAMON: %1").arg(QString(strerror(errno))));
        return -1;
    }
    return 0;
}

//停止捕获
int VideoDevice::stop_capturing()
{
    v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if(-1 == ioctl(fd, VIDIOC_STREAMOFF, &type))
    {
        emit display_error(tr("VIDIOC_STREAMOFF: %1").arg(QString(strerror(errno))));
        return -1;
    }
    return 0;
}

//释放内存
int VideoDevice::uninit_device()
{
    unsigned int i;
    for(i = 0; i < n_buffers; ++i)
    {
        if(-1 == munmap(buffers[i].start, buffers[i].length))
        {
            emit display_error(tr("munmap: %1").arg(QString(strerror(errno))));
            return -1;
        }

    }
    free(buffers);
    return 0;
}

//获取帧
int VideoDevice::get_frame(void **frame_buf, size_t* len)
{
    v4l2_buffer queue_buf;
    CLEAR(queue_buf);

    queue_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    queue_buf.memory = V4L2_MEMORY_MMAP;

    if(-1 == ioctl(fd, VIDIOC_DQBUF, &queue_buf))
    {
        switch(errno)
        {
        case EAGAIN:
            //           perror("dqbuf");
            return -1;
        case EIO:
            return -1 ;
        default:
            emit display_error(tr("VIDIOC_DQBUF: %1").arg(QString(strerror(errno))));
            return -1;
        }
    }

    *frame_buf = buffers[queue_buf.index].start;
    *len = buffers[queue_buf.index].length;
    index = queue_buf.index;

    return 0;

}

//清除帧
int VideoDevice::unget_frame()
{
    if(index != -1)
    {
        v4l2_buffer queue_buf;
        CLEAR(queue_buf);

        queue_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        queue_buf.memory = V4L2_MEMORY_MMAP;
        queue_buf.index = index;

        if(-1 == ioctl(fd, VIDIOC_QBUF, &queue_buf))
        {
            emit display_error(tr("VIDIOC_QBUF: %1").arg(QString(strerror(errno))));
            return -1;
        }
        return 0;
    }
    return -1;
}


