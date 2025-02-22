#include "ImageSettingProps.h"
#include <QJsonObject>

ImageSettingProps::ImageSettingProps()
    : splitView(false),
      scaleMode(1)
{
}

ImageSettingProps::ImageSettingProps(bool splitView,
                           int scaleMode)
    : splitView(splitView),
        scaleMode(scaleMode)
{
}

bool ImageSettingProps::isSplitView() const { return splitView; }
void ImageSettingProps::setSplitView(bool value) { splitView = value; }

int ImageSettingProps::getScaleMode() const { return scaleMode; }
void ImageSettingProps::setScaleMode(int value) { scaleMode = value; }

QJsonObject ImageSettingProps::toJson() const {
    QJsonObject json;
    json["splitView"] = splitView;
    json["scaleMode"]  = scaleMode;
    return json;
}

void ImageSettingProps::fromJson(const QJsonObject &json) {
    if (json.contains("splitView"))
        splitView = json["splitView"].toBool();
    if (json.contains("scaleMode"))
        scaleMode = json.contains("paddingLeft") ?  json["scaleMode"].toInt() : 1;

}
QString ImageSettingProps::getFileName() {
	return "ImageSettingProps.json";
}