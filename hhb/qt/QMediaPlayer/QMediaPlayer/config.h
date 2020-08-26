#ifndef CONFIG_H
#define CONFIG_H

//开机默认音量
#define APP_CONFIG_PLAYER_DEFAULT_VOLUME         50      // range: 0~100

#define APP_CONFIG_AUDIO_FILE_PATH               "./Resource/movie"

//#define PLAYBACK_MODE_CURRENT_ITEM_ONCE    0               // 单曲播放
//#define APP_CONFIG_PLAYER_DEFAULT_PLAYBACK_MODE  PLAYBACK_MODE_CURRENT_ITEM_ONCE

//主Widget默认背景1
#define MAINWIDGET_STYLE_1                      "QWidget{border-image:url(Resource/background/QBack2.png);}"

//主Widget默认背景2
#define MAINWIDGET_STYLE_2                      "QWidget{border-image:transparent;background-color:transparent;}"

#define APP_CONFIG_MAINWINDOW_BACKGROUND_STYLE  "QWidget{border-image:transparent;background-color:transparent;}"
//底部播放面板
#define TOOLWIDGETPANEL_STYLE                   "QWidget{background-color:transparent;border-image:transparent}"

//前一曲按钮样式
#define PREVIOUSPUSHBUTTON_STYLE                "QPushButton {border-image:url(:icon/Resource/icon/btn_previous_1.png);}"\
                                                "QPushButton:hover{border-image:url(:icon/Resource/icon/btn_previous_2.png);}"\
                                                "QPushButton:pressed{border-image:url(:icon/Resource/icon/btn_previous_1.png);}"
//下一曲按钮样式
#define NEXTPUSHBUTTON_STYLE                    "QPushButton {border-image:url(:icon/Resource/icon/btn_next_1.png);}"\
                                                "QPushButton:hover{border-image:url(:icon/Resource/icon/btn_next_2.png);}"\
                                                "QPushButton:pressed{border-image:url(:icon/Resource/icon/btn_next_1.png);}"

//播放或暂停按钮样式一
#define playOrPausePushButton_STYLE_1           "QPushButton {border-image:url(:icon/Resource/icon/btn_play_1.png);}"\
                                                "QPushButton:hover{border-image:url(:icon/Resource/icon/btn_play_2.png);}"\
                                                "QPushButton:pressed{border-image:url(:icon/Resource/icon/btn_play_1.png);}"

//播放或暂停按钮样式二
#define playOrPausePushButton_STYLE_2           "QPushButton {border-image:url(:icon/Resource/icon/btn_pause_1.png);}"\
                                                "QPushButton:hover{border-image:url(:icon/Resource/icon/btn_pause_2.png);}"\
                                                "QPushButton:pressed{border-image:url(:icon/Resource/icon/btn_pause_1.png);}"
//打开文件
#define OPENFILEPUSHBUTTON_STYLE                "QPushButton {border-image:url(:icon/Resource/icon/openfile_1.png);}"\
                                                "QPushButton:hover{border-image:url(:icon/Resource/icon/openfile_2.png);}"\
                                                "QPushButton:pressed{border-image:url(:icon/Resource/icon/openfile_1.png);}"

//静音按钮样式1
#define MUTEPUSHBUTTON_STYLE_1                  "QPushButton {border-image:url(:icon/Resource/icon/btn_vol_1.png);}"\
                                                "QPushButton:hover{border-image:url(:icon/Resource/icon/btn_vol_2.png);}"\
                                                "QPushButton:pressed{border-image:url(:icon/Resource/icon/btn_vol_1.png);}"
//静音按钮样式2
#define MUTEPUSHBUTTON_STYLE_2                  "QPushButton {border-image:url(:icon/Resource/icon/btn_mute_1.png);}"\
                                                "QPushButton:hover{border-image:url(:icon/Resource/icon/btn_mute_2.png);}"\
                                                "QPushButton:pressed{border-image:url(:icon/Resource/icon/btn_mute_1.png);}"
//歌曲播放进度条样式
#define MEDIASLIDER_STYLE                       "QSlider::groove:horizontal {\
                                                border: 1px solid #4A708B;\
                                                background: #C0C0C0;\
                                                height: 2px;\
                                                border-radius: 1px;\
                                                padding-left:-10px;\
                                                padding-right:-10px;\
                                                }\
                                                QSlider {\
                                                background-color: transparent;\
                                                padding-left: 10px;\
                                                padding-right: 10px;\
                                                }\
                                                QSlider::sub-page:horizontal {\
                                                background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,\
                                                stop: 0 #5DCCFF, stop: 1 #f19dbd);\
                                                border: 1px solid #f1f3a0;\
                                                height: 10px;\
                                                border-radius: 2px;\
                                                }\
                                                QSlider::add-page:horizontal {\
                                                background: #575757;\
                                                border: 1px solid #777;\
                                                height: 10px;\
                                                border-radius: 2px;\
                                                }\
                                                QSlider::handle:horizontal \
                                                {\
                                                background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.3, fx:0.4, fy:0.4, \
                                                stop:0.6 #f1f3a0, stop:0.778409 rgba(255, 255, 255, 0));\
                                                width: 25px;\
                                                margin-top: -12px;\
                                                margin-bottom: -12px;\
                                                border-radius: 8px;\
                                                }\
                                                QSlider::handle:horizontal:hover {\
                                                background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.4, fx:0.5, fy:0.5, stop:0.6 #f1f3a0, \
                                                stop:0.778409 rgba(255, 255, 255, 0));\
                                                width: 25px;\
                                                margin-top: -12px;\
                                                margin-bottom: -12px;\
                                                border-radius: 8px;\
                                                }\
                                                QSlider::sub-page:horizontal:disabled {\
                                                background: #00009C;\
                                                border-color: #999;\
                                                }\
                                                QSlider::add-page:horizontal:disabled {\
                                                background: #eee;\
                                                border-color: #999;\
                                                }\
                                                QSlider::handle:horizontal:disabled {\
                                                background: #eee;\
                                                border: 1px solid #aaa;\
                                                border-radius: 4px;\
                                                }"
//音量条样式
#define VOLUMESLIDER_STYLE                      "QSlider {background-color: rgb(25, 38, 58); \
                                                 min-width:5px; max-width:5px; border:15px solid rgb(25, 38, 58);}\
                                                 QSlider::add-page:vertical{background-color: #ba89dc;width:4px;}\
                                                 QSlider::groove:vertical {\
                                                 border: 1px solid #4A708B;\
                                                 background: #C0C0C0;\
                                                 width: 5px;\
                                                 border-radius: 1px;\
                                                 padding-left:-1px;\
                                                 padding-right:-1px;\
                                                 padding-top:-10px;\
                                                 padding-bottom:-10px;\
                                                 }\
                                                 QSlider::sub-page:vertical {\
                                                 background: #575757;\
                                                 border: 0px solid #f1f3a0;\
                                                 border-radius: 2px;\
                                                 }\
                                                 QSlider::add-page:vertical\
                                                 {\
                                                 background: qlineargradient(x1: 0, y1: 1, x2: 0, y2: 0.2,\
                                                 stop: 0 #5DCCFF, stop: 1 #f1f3a0);\
                                                 border: 1px solid #777;\
                                                 width: 10px;\
                                                 border-radius: 2px;\
                                                 }\
                                                 QSlider::handle:vertical\
                                                 {\
                                                 background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.4, fx:0.5, fy:0.5, stop:0.6 #f1f3a0,\
                                                 stop:0.778409 rgba(255, 255, 255, 0));\
                                                 height: 25px;\
                                                 margin-left: -10px;\
                                                 margin-right: -10px;\
                                                 border-radius: 8px;\
                                                 }\
                                                 QSlider::handle:vertical:hover {\
                                                 background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.6, fx:0.5, fy:0.5, stop:0.6 #f1f3a0, \
                                                 stop:0.778409 rgba(255, 255, 255, 0));\
                                                 height: 25px;\
                                                 margin-left: -10px;\
                                                 margin-right: -10px;\
                                                 border-radius: 8px;\
                                                 }\
                                                 QSlider::sub-page:vertical:disabled {\
                                                 background: #00009C;\
                                                 border-color: #999;\
                                                 }\
                                                 QSlider::add-page:vertical:disabled {\
                                                 background: #eee;\
                                                 border-color: #999;\
                                                 }\
                                                 QSlider::handle:vertical:disabled {\
                                                 background: #eee;\
                                                 border: 1px solid #aaa;\
                                                 border-radius: 4px;\
                                                 }"
//视频列表的样式
#define SONGSLIST_STYLE                         "QListWidget{background-color: rgba(255, 255, 255, 10%);outline:none;border-image:transparent}"\
                                                "QListWidget::item:selected:active{background: rgb(0, 106, 181);}"\
                                                "QListWidget::item:hover:active{background: qlineargradient(spread:repeat, x2:0, y2:1, x2:1, y2:0, stop:0 #a1a112, stop:0.25 #a1a112, stop:0.5 #ba89dc, stop:1 #ba89dc);}"


#endif // CONFIG_H

