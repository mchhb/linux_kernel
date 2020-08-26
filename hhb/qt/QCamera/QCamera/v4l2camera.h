#ifndef V4L2CAMERA_H
#define V4L2CAMERA_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/videodev2.h>
/*
 * 参考至博客:
 * https://blog.csdn.net/sunny_hu92/article/details/79985660
 * https://blog.csdn.net/taiyang1987912/article/details/50541775
 */

/* buffer结构体 */
typedef struct
{
    void *start;
    size_t length;
}buffer;

/* 传输数据结构体 */
typedef struct
{
    int fd;
    int n_buffers;
    char *dev_name;
    buffer *buffers;
    struct v4l2_buffer buf;
    struct v4l2_format fmt;
}pass_data;


/* V4L2操作类 */
class V4l2Camera
{
public:
    V4l2Camera();

    //int init_dev(pass_data *);
    int init_dev(pass_data *s, int width, int height);
    void stop_dev(pass_data *);


    int open_device(pass_data *);
    //int init_device(pass_data *);
    int init_device(pass_data *s, int width, int height);
    int xioctl(int, int, void *);
    void errno_exit(const char *s);
    void init_mmap(pass_data *);
    void start_capturing(pass_data *);
    int stop_capturing(pass_data *);
    int stop_device(pass_data *);
    void close_device(pass_data *);
    void process_image(void *p, pass_data *s, int i);
    int read_frame(pass_data *s);
    int return_data(pass_data *s);
    int convert_yuv_to_rgb_pixel(int , int , int );
    int convert_yuv_to_rgb_buffer(unsigned char *, unsigned char *, unsigned int , unsigned int);

};

#endif // V4L2CAMERA_H
