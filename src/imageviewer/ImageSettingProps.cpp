#include "ImageSettingProps.h"
#include <QJsonObject>

ImageSettingProps::ImageSettingProps()
	: m_splitView(false),
	m_scaleMode(1),
	m_autoNext(true)
{
}

ImageSettingProps::ImageSettingProps(bool splitView,
	int scaleMode, bool isAutoNext)
	: m_splitView(splitView),
	m_scaleMode(scaleMode),
	m_autoNext(isAutoNext)
{
}

bool ImageSettingProps::isSplitView() const { return m_splitView; }
void ImageSettingProps::setSplitView(bool value) { m_splitView = value; }

int ImageSettingProps::getScaleMode() const { return m_scaleMode; }
void ImageSettingProps::setScaleMode(int value) { m_scaleMode = value; }

bool ImageSettingProps::isAutoNext() const { return m_autoNext; }
void ImageSettingProps::setAutoNext(bool value) { m_autoNext = value; }

QJsonObject ImageSettingProps::toJson() const {
	QJsonObject json;
	json["splitView"] = m_splitView;
	json["scaleMode"] = m_scaleMode;
	json["autoNext"] = m_autoNext;
	return json;
}

void ImageSettingProps::fromJson(const QJsonObject& json) {
	if (json.contains("splitView"))
		m_splitView = json["splitView"].toBool();
	if (json.contains("scaleMode"))
		m_scaleMode = json.contains("paddingLeft") ? json["scaleMode"].toInt() : 1;
	if (json.contains("autoNext"))
		m_autoNext = json["autoNext"].toBool();

}
QString ImageSettingProps::getFileName() {
	return "ImageSettingProps.json";
}