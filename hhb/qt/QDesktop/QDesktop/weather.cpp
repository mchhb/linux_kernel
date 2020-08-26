#include "weather.h"

void MainWindow::networkAccessManagerInit(void) {
    /* 实例化 */
    NAManager = new QNetworkAccessManager(this);

    /* 绑定URL */
    QNetworkRequest  ipGetNRequest;
    ipGetNRequest.setUrl(QUrl(IP_GET_API_URL));

    /* get */
    QNetworkReply *reply = NAManager->get(ipGetNRequest);

    /* 连接信号、槽函数 */
    connect(reply, SIGNAL(finished()), this, SLOT(slotFinishedIpGetReply()));
}

void MainWindow::weatherDisInit(void) {
    QFont font;
    /* 区域背景显示 */
    weatherDisAreaBG = new QWidget(this);
    weatherDisAreaBG->setGeometry((float)20/800*this->geometry().width(), (float)185/480*this->geometry().height(),
                                  (float)220/800*this->geometry().width(), (float)120/480*this->geometry().height());
    weatherDisAreaBG->setStyleSheet("QWidget {border-image: url(Resource/background/bgWeather);"
                                    "background-color: rgba(0, 0, 0, 0%);}");

    /* 城市 */
    displayCity = new QLabel(weatherDisAreaBG);
    displayCity->setGeometry((float)10/800*this->geometry().width(), 0,
                             (float)150/800*this->geometry().width(), (float)40/480*this->geometry().height());
    displayCity->setStyleSheet("QLabel {color: white;border-image: transparent}");
    displayCity->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    displayCity->setText("正在定位...");
    font.setPointSize((float)17/800*this->geometry().width());
    displayCity->setFont(font);

    /* 当前温度 */
    displayTemp = new QLabel(weatherDisAreaBG);
    displayTemp->setGeometry((float)10/800*this->geometry().width(), (float)40/480*this->geometry().height(),
                             (float)150/800*this->geometry().width(), (float)40/480*this->geometry().height());
    displayTemp->setStyleSheet("QLabel { color: white;border-image: transparent}");
    displayTemp->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    displayTemp->setText("26℃");
    font.setPointSize((float)26/800*this->geometry().width());
    displayTemp->setFont(font);

    /* 当日温度最大值、最小值 */
    displayMinMaxTemp = new QLabel(weatherDisAreaBG);
    displayMinMaxTemp->setGeometry((float)10/800*this->geometry().width(), (float)80/480*this->geometry().height(),
                                   (float)100/800*this->geometry().width(), (float)40/480*this->geometry().height());
    displayMinMaxTemp->setStyleSheet("QLabel { color: white;border-image: transparent}");
    displayMinMaxTemp->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    displayMinMaxTemp->setText("20℃~28℃");
    font.setPointSize((float)12/800*this->geometry().width());
    displayMinMaxTemp->setFont(font);

    /* 天气图标 */
    weatherIcon = new QLabel(weatherDisAreaBG);
    weatherIcon->setGeometry((float)90/800*this->geometry().width(), (float)20/480*this->geometry().height(),
                             (float)120/800*this->geometry().width(), (float)60/480*this->geometry().height());
    weatherIcon->setAlignment(Qt::AlignCenter);
    weatherIcon->setStyleSheet("QLabel{border-image:url(./Resource/weatherIcon/晴.png);}");
    weatherIcon->installEventFilter(this);

    /* 天气类型 */
    displayWeather = new QLabel(weatherDisAreaBG);
    displayWeather->setGeometry((float)105/800*this->geometry().width(), (float)80/480*this->geometry().height(),
                                (float)100/800*this->geometry().width(), (float)40/480*this->geometry().height());
    displayWeather->setStyleSheet("QLabel {color: white;border-image: transparent}");
    displayWeather->setAlignment(Qt::AlignCenter);
    displayWeather->setText("晴");
    displayWeather->setFont(font);
}

/*
 * 槽函数
 */
void MainWindow::slotFinishedIpGetReply(void) {
    QNetworkReply *reply = (QNetworkReply *)sender();    // 获取信号发送者
    QTextCodec *codec = QTextCodec::codecForName("gbk"); // 该接口返回的字符串采用gbk编码
    QString temp = codec->toUnicode(reply->readAll());   // 读取所有返回数据并将其转换为Unicode编码
    reply->deleteLater();                                // 这个要加上 避免发生内存泄漏

    if (temp.isEmpty() || temp.isNull()) {
        displayCity->setText("定位失败");
        return;
    }

    /* 获取ip地址 发送请求 */
    QNetworkRequest  ipQueryNRequest;
    ipQueryNRequest.setUrl(QUrl(IP_QUERY_API_URL + temp));
    QNetworkReply *newReply = NAManager->get(ipQueryNRequest);
    connect(newReply, SIGNAL(finished()), this, SLOT(slotFinishedIpQueryReply()));
}

void MainWindow::slotFinishedIpQueryReply(void) {
    QNetworkReply *reply = (QNetworkReply *)sender();
    QTextCodec *codec = QTextCodec::codecForName("gbk");
    QString temp = codec->toUnicode(reply->readAll());
    reply->deleteLater();

    /* 判断字符串是否为空 */
    if (temp.isEmpty() ||
            temp.isNull()) {
        displayCity->setText("定位失败");
        return;
    }

    /* 提取city字段 */
    int length = temp.length(); // 得到字符串长度
    int startIndex = temp.indexOf("city:") + 6;  // city:广州市,
    int endIndex = -1;

    if (5 == startIndex) { // 表示不存在city字段 -1
        displayCity->setText("定位失败");
        return;
    }

    for (int i = startIndex; i < length; i++) {

        if (',' == temp.at(i)) {
            endIndex = i;
            break;
        }
    }

    if (-1 == endIndex) {
        displayCity->setText("定位失败");
        return;
    }

    /* 获取城市名 发送请求 */
    QString cityName = temp.mid(startIndex, endIndex - startIndex - 1);
    QNetworkRequest  weatherGetNRequest;
    weatherGetNRequest.setUrl(QUrl(WEATHER_GET_API_URL + cityName));
    QNetworkReply *newReply = NAManager->get(weatherGetNRequest);
    connect(newReply, SIGNAL(finished()), this, SLOT(slotFinishedWeatherGetReply()));
}

void MainWindow::slotFinishedWeatherGetReply(void) {
    QNetworkReply *reply = (QNetworkReply *)sender();
    QString temp = reply->readAll();
    reply->deleteLater();
    //qDebug() << temp << endl;

    if (temp.isEmpty() ||
            temp.isNull() ) {
        displayCity->setText("定位失败");
        return;
    }

    /* 创建Json相关对象 */
    QJsonParseError error;
    QJsonObject jsonObj;
    QJsonDocument jsonDocument;
    QString data;

    /* 解析Jason字符串 获取天气预告信息 */
    jsonDocument = QJsonDocument::fromJson(temp.toUtf8(), &error);
    if (error.error == QJsonParseError::NoError) {

        if (jsonDocument.isNull() || jsonDocument.isEmpty()) {
            displayCity->setText("定位失败");
        }
        else {
            if (jsonDocument.isObject()) {
                jsonObj = jsonDocument.object();
                jsonObj = jsonObj.value("data").toObject();

                data = jsonObj.value("city").toString();
                displayCity->setText(data);

                data = jsonObj.value("wendu").toString() + "℃";
                displayTemp->setText(data);

                QJsonArray forecast1 = jsonObj.value("forecast").toArray();
                QJsonObject object = forecast1.at(0).toObject();
                data = object.value("low").toString().mid(3, 3) + "~";
                data += object.value("high").toString().mid(3, 3);
                displayMinMaxTemp->setText(data);

                data = object.value("type").toString();
                displayWeather->setText(data);
                data = "./Resource/weatherIcon/" + data + ".png";
                QFileInfo file(data);
                if(file.exists() == false){
                    weatherIcon->setStyleSheet("QLabel{border-image:url(./Resource/weatherIcon/未定义.png);}");
                }else {
                    weatherIcon->setStyleSheet("QLabel{border-image:url("  + data + ");}");
                }
            }
            else {
                displayCity->setText("定位失败");
            }
        }
    }
    else {
        displayCity->setText("定位失败");
    }
}

void MainWindow::updateWeather(void) {
    if (!flagUpdateWeather)
        return;

    /* 标志位置位 */
    flagUpdateWeather = false;
}

