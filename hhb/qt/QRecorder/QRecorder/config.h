#ifndef CONFIG_H
#define CONFIG_H

#define APP_CONFIG_AUDIO_FILE_PATH "./Resource/audio"
#define USER_VOLUME 50

#define RECORDERBUTTON_STYLE_1      "QPushButton {border-image:url(:/icon/Resource/icons/recorder_1);}"\
                                    "QPushButton:hover {border-image:url(:/icon/Resource/icons/recorder_2);}"\
                                    "QPushButton:pressed {border-image:url(:/icon/Resource/icons/recorder_1);}"

#define RECORDERBUTTON_STYLE_2      "QPushButton {border-image:url(:/icon/Resource/icons/recorder_stop_1);}"\
                                    "QPushButton:hover {border-image:url(:/icon/Resource/icons/recorder_stop_2);}"\
                                    "QPushButton:pressed {border-image:url(:/icon/Resource/icons/recorder_stop_1);}"

#define RECORDERBUTTON_STYLE        "QPushButton {border-image:url(:/icon/Resource/icons/recorder_1);}"\
                                    "QPushButton:hover {border-image:url(:/icon/Resource/icons/recorder_2);}"\
                                    "QPushButton:pressed {border-image:url(:/icon/Resource/icons/recorder_1);}"

#define RECORDERLISTWIDGET_STYLE    "QListWidget{background-color: rgba(255, 255, 255, 10%);outline:none;border-image:transparent;}"\
                                    "QListWidget::item:selected:active{background: rgb(0, 106, 181);}"\
                                    "QListWidget::item:hover:active{background: qlineargradient(spread:repeat, x2:0, y2:1, x2:1, y2:0,"\
                                    "stop:0 #1d2437, stop:0.25 #1d2437, stop:0.5 #ff5b53, stop:1 #ff5b53);}"

#define PREVIOUSPUSHBUTTON_STYLE    "QPushButton {border-image:url(:/icon/Resource/icons/btn_previous_1);}"\
                                    "QPushButton:hover {border-image:url(:/icon/Resource/icons/btn_previous_2);}"\
                                    "QPushButton:pressed {border-image:url(:/icon/Resource/icons/btn_previous_1);}"

#define NEXTPUSHBUTTON_STYLE        "QPushButton {border-image:url(:/icon/Resource/icons/btn_next_1);}"\
                                    "QPushButton:hover {border-image:url(:/icon/Resource/icons/btn_next_2);}"\
                                    "QPushButton:pressed {border-image:url(:/icon/Resource/icons/btn_next_1);}"

#define PLAYORPAUSEPUSHBUTTON_STYLE_1 "QPushButton {border-image:url(:/icon/Resource/icons/btn_play_1);}"\
                                    "QPushButton:hover {border-image:url(:/icon/Resource/icons/btn_play_2);}"\
                                    "QPushButton:pressed {border-image:url(:/icon/Resource/icons/btn_pause_1);}"

#define PLAYORPAUSEPUSHBUTTON_STYLE_2 "QPushButton {border-image:url(:/icon/Resource/icons/btn_pause_1);}"\
                                    "QPushButton:hover {border-image:url(:/icon/Resource/icons/btn_pause_2);}"\
                                    "QPushButton:pressed {border-image:url(:/icon/Resource/icons/btn_pause_1);}"

#define MEDIASLIDER_STYLE           "QSlider::groove:horizontal {\
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
                                    stop: 0 #5DCCFF, stop: 1 #f73d31);\
                                    border: 1px solid #f73d31;\
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
                                    stop:0.6 #f73d31, stop:0.778409 rgba(255, 255, 255, 0));\
                                    width: 25px;\
                                    margin-top: -12px;\
                                    margin-bottom: -12px;\
                                    border-radius: 8px;\
                                    }\
                                    QSlider::handle:horizontal:hover {\
                                    background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.4, fx:0.5, fy:0.5, stop:0.6 #f73d31, \
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

#define MUTEPUSHBUTTON_STYLE_1      "QPushButton {border-image:url(:/icon/Resource/icons/btn_vol_1);}"\
                                    "QPushButton:hover {border-image:url(:/icon/Resource/icons/btn_vol_2);}"\
                                    "QPushButton:pressed {border-image:url(:/icon/Resource/icons/btn_vol_1);}"

#define MUTEPUSHBUTTON_STYLE_2       "QPushButton {border-image:url(:/icon/Resource/icons/btn_mute_1);}"\
                                    "QPushButton:hover {border-image:url(:/icon/Resource/icons/btn_mute_2);}"\
                                    "QPushButton:pressed {border-image:url(:/icon/Resource/icons/btn_mute_1);}"

#define DELETEPUSHBUTTON_STYLE      "QPushButton {border-image:url(:/icon/Resource/icons/delete_1);}"\
                                    "QPushButton:hover {border-image:url(:/icon/Resource/icons/delete_2);}"\
                                    "QPushButton:pressed {border-image:url(:/icon/Resource/icons/delete_1);}"

#define VOLUMESLIDER_STYLE          "QSlider {background-color: rgb(40, 40, 85); \
                                    min-width:5px; max-width:5px; border:15px solid rgb(60, 60, 60);}\
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
                                    border: 1px solid #4A708B;\
                                    border-radius: 2px;\
                                    }\
                                    QSlider::add-page:vertical\
                                    {\
                                    background: qlineargradient(x1: 1, y1: 0.5, x2: 1, y2: 0,\
                                    stop: 0 #d62c20, stop: 1 #f73d31);\
                                    border: 0px solid #777;\
                                    width: 10px;\
                                    border-radius: 2px;\
                                    }\
                                    QSlider::handle:vertical\
                                    {\
                                    background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.4, fx:0.5, fy:0.5, stop:0.6 #f73d31,\
                                    stop:0.778409 rgba(255, 255, 255, 0));\
                                    height: 25px;\
                                    margin-left: -10px;\
                                    margin-right: -10px;\
                                    border-radius: 8px;\
                                    }\
                                    QSlider::handle:vertical:hover {\
                                    background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.6, fx:0.5, fy:0.5, stop:0.6 #f73d31, \
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
/* 窗口话询问按钮确认键样式表 */
#define BTN_YES_STYLE               "background-color:#89AFDE;"\
                                    "border-style: outset;"\
                                    "border-width: 2px;"\
                                    "border-radius: 10px;"\
                                    "border-color: beige;"\
                                    "font: bold 15px;"\
                                    "min-width: 3em;"\
                                    "min-height: 1em;"
/* 窗口话询问按钮取消键样式表 */
#define BTN_CANCEL_STYLE            "background-color:#89AFDE;"\
                                    "border-style: outset;"\
                                    "border-width: 2px;"\
                                    "border-radius: 10px;"\
                                    "border-color: beige;"\
                                    "font: bold 15px;"\
                                    "min-width: 3em;"\
                                    "min-height: 1em;"
#endif // CONFIG_H

