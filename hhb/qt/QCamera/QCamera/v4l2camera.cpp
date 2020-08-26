#include "v4l2camera.h"
#include <QDebug>

#define	CLEAR(x)	memset (&x, 0, sizeof(x))
char *mpeg[] = {"./1.mpeg", "./2.mpeg", "./3.mpeg", "./4.mpeg", "./5.mpeg"};

V4l2Camera::V4l2Camera()
{

}

int V4l2Camera::init_dev(pass_data *s, int width, int height)
{
    int flag = open_device(s);
    if(flag != 0)
        return -1;

    if(init_device(s, width, height) < 0)
        return -2;
    init_mmap(s);
    start_capturing(s);
    qDebug("%s initialize finish ...\n",s->dev_name);
    return 0;
}

void V4l2Camera::stop_dev(pass_data *s)
{
    stop_capturing(s);  /* 停止捕获 */
    stop_device(s);     /* 停止设备，释放内存映射 */
    close_device(s);    /* 关闭设备文件 */
}

void V4l2Camera::process_image(void *p, pass_data *s, int i)
{
    fputc('.', stdout);
    fflush(stdout);
    fprintf(stderr, "%s", mpeg[i]);
    int fd;
    if((fd = open(mpeg[i], O_RDWR | O_CREAT | O_TRUNC, 0644)) == -1)
        errno_exit("open");
    if((write(fd, (struct v4l2_buffer *)p, s->fmt.fmt.pix.sizeimage)) == -1)
        errno_exit("write");
    close(fd);
}

int V4l2Camera::read_frame(pass_data *s)
{
    int ret = 0;
    CLEAR(s->buf);
    s->buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    s->buf.memory = V4L2_MEMORY_MMAP;
    if(xioctl(s->fd, VIDIOC_DQBUF, &s->buf) == -1)
    {
        switch (errno) {
            case EAGAIN:
                ret = -1;
                //errno_exit("VIDIOC_DQBUF");
            case EIO:
            default:
                ret = -2;
            //errno_exit("VIDIOC_DQBUF");
        }
    }
    //assert(s->buf.index < s->n_buffers);
    return ret;
}

int V4l2Camera::return_data(pass_data *s)
{
    //xioctl(s->fd, VIDIOC_QBUF, &s->buf);
    if(xioctl(s->fd, VIDIOC_QBUF, &s->buf) == -1)
    {
        return -1;
    }
      //errno_exit("VIDIOC_QBUF");
}

int V4l2Camera::init_device(pass_data *s, int width, int height)
{
    struct v4l2_capability cap;     /*  设备属性*/
    struct v4l2_cropcap cropcap;    /* 设置摄像头捕获能力 */
    struct v4l2_crop   crop;
    unsigned int min;

    if(xioctl(s->fd, VIDIOC_QUERYCAP, &cap) == -1) /* 查询摄像头参数 */
    {
        if(EINVAL == errno)
        {
            fprintf(stderr, "%s is no v4l2 device\n", s->dev_name);\
            return -1;
            //exit(EXIT_FAILURE);
        }
        else
        {
            return -2;
            //errno_exit("VIDIOC_QUERYCAP");
        }
    }
    if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) /* 不支持图像获取 */
    {
        fprintf(stderr,"%s is no video capture device\n", s->dev_name);
        return -3;
        //exit(EXIT_FAILURE);
    }

    if(! (cap.capabilities & V4L2_CAP_STREAMING)) /* 不支持流I/O */
    {
        fprintf(stderr, "%s does not support straming I/O\n", s->dev_name);
        return -4;
        //exit(EXIT_FAILURE);
    }

    qDebug("%s driver: %s",s->dev_name, cap.driver);
    qDebug("%s card %s:",s->dev_name, cap.card);
    qDebug("%s bus %s:",s->dev_name, cap.bus_info);
    qDebug("%s version:%u.%u.%u",s->dev_name, ((cap.version >> 16)&0xff),
                                               ((cap.version >> 8)&0xff),
                                               ((cap.version)&0xff));

    /* 设置摄像头捕获能力 */
    CLEAR(cropcap);
    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;     /* 视频预览 */
    cropcap.bounds.width = width;
    cropcap.bounds.height = height;
    cropcap.defrect.width = width;
    cropcap.defrect.height = height;
    if(xioctl(s->fd, VIDIOC_CROPCAP, &cropcap) == 0) /* 设置 */
    {
//       qDebug("bounds.left:%d", cropcap.bounds.left);
//       qDebug("bounds.top:%d", cropcap.bounds.top);
//       qDebug("bounds.width:%d", cropcap.bounds.width);
//       qDebug("bounds.height:%d", cropcap.bounds.height);

       crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
       crop.c = cropcap.defrect;
       if(xioctl(s->fd, VIDIOC_S_CROP, &crop))
       {
           switch (errno) {
               case EINVAL:
                    break;
               default:
                break;
           }
       }
       else
       {
           /* Errors ignored */
       }
    }

    /* 获取当前帧格式 */
    CLEAR(s->fmt);
    s->fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(xioctl(s->fd, VIDIOC_G_FMT, &s->fmt) == -1)
        return -5;
        //errno_exit("VIDIOC_G_FMT");

    /* 设置当前格式 */
    CLEAR(s->fmt);
    s->fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  /* 视频捕获 */
    s->fmt.fmt.pix.width = width;
    s->fmt.fmt.pix.height = height;
    s->fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV; /* 像素格式为YUYV */
    s->fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
    unsigned   char* ch = cap.driver;
  /* 设置帧率,5640需要这样设置 */
    if(ch[0]== 'm'){//通过设备的首字母判断是不是mx6s-csi驱动
    struct v4l2_frmsizeenum fszenum ;
    struct v4l2_streamparm stream_Parm;

    fszenum.index = 0;
    int capture_mode = -1;
    fszenum.pixel_format = V4L2_PIX_FMT_YUYV;

    if(-1 == ioctl(s->fd, VIDIOC_ENUM_FRAMESIZES, &fszenum))
    {
        // emit display_error(tr("fszenum").arg(QString(strerror(errno))));
    }
    while (ioctl (s->fd, VIDIOC_ENUM_FRAMESIZES, &fszenum)== 0 ){
        //if(fszenum.type == V4L2_FRMSIZE_TYPE_DISCRETE)
        //判断设置帧的大小，查询5640的配置模式
       if (fszenum.discrete.width ==  s->fmt.fmt.pix.width && fszenum.discrete.height ==  s->fmt.fmt.pix.height) {
         capture_mode = fszenum.index;
         break;
       }
       fszenum.index ++;
     }

    if(capture_mode < 0)
    {
        //emit display_error(tr("can't support resolution").arg(QString(strerror(errno))));
        return -1;
    }

    memset(&stream_Parm, 0, sizeof(struct v4l2_streamparm));
    stream_Parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    stream_Parm.parm.capture.capturemode = capture_mode;//设置驱动里面配置模式

    stream_Parm.parm.capture.timeperframe.denominator =15; //帧率
    stream_Parm.parm.capture.timeperframe.numerator = 1;

    if(-1 == ioctl(s->fd, VIDIOC_S_PARM, &stream_Parm))
    {
        //emit display_error(tr("VIDIOC_S_PARM").arg(QString(strerror(errno))));
        return -1;
    }
/* 设置成功 */
 }

    if(xioctl(s->fd, VIDIOC_S_FMT, &s->fmt) == -1)
        return -6;
        //errno_exit("VIDIOC_S_FMT");

    min = s->fmt.fmt.pix.width * 2;
    if(s->fmt.fmt.pix.bytesperline < min)
        s->fmt.fmt.pix.bytesperline = min;

    min = s->fmt.fmt.pix.bytesperline * s->fmt.fmt.pix.height;
    if(s->fmt.fmt.pix.sizeimage < min)
        s->fmt.fmt.pix.sizeimage = min;
}

void V4l2Camera::init_mmap(pass_data *s)
{
    struct v4l2_requestbuffers req;
    CLEAR(req);

    //req.count = 20; /* 申请20个缓冲帧的缓冲区 */
    req.count = 3; /* 申请20个缓冲帧的缓冲区 */
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; /* 缓冲区类型 */
    req.memory = V4L2_MEMORY_MMAP;  /* mmap映射*/
    if(xioctl(s->fd, VIDIOC_REQBUFS, &req))
    {
        if(EINVAL == errno)
        {
            fprintf(stderr, "%s does not support 'memeory mapping'\n", s->dev_name);
            exit(EXIT_FAILURE);
        }
        else
        {
            errno_exit("VIDIOC_REQBUFS");
        }
    }

    if(req.count < 2)
    {
        fprintf(stderr, "Insufficient buffer memory on %s\n", s->dev_name);
        exit(EXIT_FAILURE);
    }

    if((s->buffers = (buffer *)calloc(req.count, sizeof (*s->buffers))) == NULL)
    {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }

    for(s->n_buffers = 0; s->n_buffers < req.count; ++s->n_buffers)
    {
        struct v4l2_buffer buf;
        CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = s->n_buffers;
        if(xioctl(s->fd, VIDIOC_QUERYBUF, &buf) == -1) /* 获取缓冲区帧长度 */
            errno_exit("VIDIOC_QUERYBUF");

        s->buffers[s->n_buffers].length = buf.length;
        /* mmap映射 */
        s->buffers[s->n_buffers].start = mmap(NULL,                     /* 映射起始地址，一般为NULL，让内核自己选择 */
                                              buf.length,               /* 被映射的内存块长度 */
                                              PROT_READ | PROT_WRITE,   /* 映射后是否能被读写 */
                                              MAP_SHARED,               /* 确定此内存能否与其他进程恭共享 */
                                              s->fd,                    /* 文件 */
                                              buf.m.offset);
        if(s->buffers[s->n_buffers].start == MAP_FAILED)
            errno_exit("mmap");
    }
}


void V4l2Camera::start_capturing(pass_data *s)
{
    unsigned  int i;
    enum v4l2_buf_type type;
    for(i=0; i<s->n_buffers; i++)   //将缓冲帧放到队列中
    {
        struct v4l2_buffer buf;
        CLEAR(buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        if(xioctl(s->fd, VIDIOC_QBUF, &buf) == -1)
            errno_exit("VIDIOC_QBUF");
    }

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(xioctl(s->fd, VIDIOC_STREAMON,&type)) //启动数据流
        errno_exit("VIDIOC_STREAMON");
}

int V4l2Camera::stop_capturing(pass_data *s)
{
    enum v4l2_buf_type type;
    type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(xioctl(s->fd, VIDIOC_STREAMOFF,&type))
        return -1;
}

int V4l2Camera::stop_device(pass_data *s)
{
    unsigned i;
    for(i = 0; i < s->n_buffers; ++i)
    {
        if(munmap(s->buffers[i].start, s->buffers[i].length) == -1)
            return -1;
            //errno_exit("munmap");
    }
}

int V4l2Camera::open_device(pass_data *s)
{
    struct stat st;
    if(stat(s->dev_name, &st) == -1) /* 判断设备文件是否存在 */
    {
        fprintf(stderr, "Can't identify '%s':[%d] %s\n", s->dev_name, errno, strerror(errno));
        return -1;
    }
    if(!S_ISCHR(st.st_mode))        /* 判断文件是不是字符文件 */
    {
        fprintf(stderr, "%s is no device\n", s->dev_name);
        return -2;
    }

    if((s->fd = open(s->dev_name, O_RDWR, 0)) == -1) /* 打开设备文件 */
    {
        fprintf(stderr, "Can't open '%s':error %d, %s\n", s->dev_name,errno, strerror(errno));
        return -2;
    }
    return 0;
}

void V4l2Camera::close_device(pass_data *s)
{
    close(s->fd);
}


int V4l2Camera::xioctl(int fd, int request, void *arg)
{
    int r;
    do r = ioctl(fd, request, arg);
    while (r == -1 && EINTR == errno);
    return r;
}

void V4l2Camera::errno_exit(const char *s)
{
    fprintf(stderr, "%s error %d, %s\n", s, errno, strerror(errno));
    exit(EXIT_FAILURE);
}

int V4l2Camera::convert_yuv_to_rgb_pixel(int y, int u, int v)
{
    unsigned int pixel32 = 0;
    unsigned char *pixel = (unsigned char *)&pixel32;
    int r, g, b;
    r = y + (1.370705 * (v-128));
    g = y - (0.698001 * (v-128)) - (0.337633 * (u-128));
    b = y + (1.732446 * (u-128));
    if(r > 255) r = 255;
    if(g > 255) g = 255;
    if(b > 255) b = 255;
    if(r < 0) r = 0;
    if(g < 0) g = 0;
    if(b < 0) b = 0;
    pixel[0] = r * 220 / 256;
    pixel[1] = g * 220 / 256;
    pixel[2] = b * 220 / 256;
    return pixel32;
}

int V4l2Camera::convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height)
{
    unsigned int in, out = 0;
    unsigned int pixel_16;
    unsigned char pixel_24[3];
    unsigned int pixel32;
    int y0, u, y1, v;
    for(in = 0; in < width * height * 2; in += 4) {
        pixel_16 = yuv[in + 3] << 24 |
                   yuv[in + 2] << 16 |
                   yuv[in + 1] <<  8 |
                   yuv[in + 0];
        y0 = (pixel_16 & 0x000000ff);
        u  = (pixel_16 & 0x0000ff00) >>  8;
        y1 = (pixel_16 & 0x00ff0000) >> 16;
        v  = (pixel_16 & 0xff000000) >> 24;
        pixel32 = convert_yuv_to_rgb_pixel(y0, u, v);
        pixel_24[0] = (pixel32 & 0x000000ff);
        pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
        pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
        rgb[out++] = pixel_24[0];
        rgb[out++] = pixel_24[1];
        rgb[out++] = pixel_24[2];
        pixel32 = convert_yuv_to_rgb_pixel(y1, u, v);
        pixel_24[0] = (pixel32 & 0x000000ff);
        pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
        pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
        rgb[out++] = pixel_24[0];
        rgb[out++] = pixel_24[1];
        rgb[out++] = pixel_24[2];
    }
    return 0;
}
