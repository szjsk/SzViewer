#include "TextSettingProps.h"
#include <QJsonObject>

TextSettingProps::TextSettingProps()
    : splitView(false),
      textView(true),
      font(QFont("맑은 고딕", 16, QFont::Bold)),
      textColor(Qt::black),
      backgroundColor(QColor("#e8e4d2")), //#e8e4d2
      padding(10, 10, 10, 10),
      lineSpacing(1.0)
{
}

TextSettingProps::TextSettingProps(bool splitView,
                           bool textView,
                           const QFont &font,
                           const QColor &textColor,
                           const QColor &backgroundColor,
                           const QMargins &padding,
                           double lineSpacing)
    : splitView(splitView),
      textView(textView),
      font(font),
      textColor(textColor),
      backgroundColor(backgroundColor),
      padding(padding),
      lineSpacing(lineSpacing)
{
}

bool TextSettingProps::isSplitView() const { return splitView; }
void TextSettingProps::setSplitView(bool value) { splitView = value; }

bool TextSettingProps::isTextView() const { return textView; }
void TextSettingProps::setTextView(bool value) { textView = value; }

QFont TextSettingProps::getFont() const { return font; }
void TextSettingProps::setFont(const QFont &font) { this->font = font; }

QColor TextSettingProps::getTextColor() const { return textColor; }
void TextSettingProps::setTextColor(const QColor &color) { textColor = color; }

QColor TextSettingProps::getBackgroundColor() const { return backgroundColor; }
void TextSettingProps::setBackgroundColor(const QColor &color) { backgroundColor = color; }

QMargins TextSettingProps::getPadding() const { return padding; }
void TextSettingProps::setPadding(const QMargins &margins) { padding = margins; }

double TextSettingProps::getLineSpacing() const { return lineSpacing; }
void TextSettingProps::setLineSpacing(double spacing) { lineSpacing = spacing; }

QJsonObject TextSettingProps::toJson() const {
    QJsonObject json;
    json["splitView"] = splitView;
    json["textView"]  = textView;

    // QFont를 string으로 변환 (예: QFont::toString 활용)
    json["font"] = font.toString();

    json["textColor"] = textColor.name();
    json["backgroundColor"] = backgroundColor.name();

    json["paddingLeft"] = padding.left();
    json["paddingTop"] = padding.top();
    json["paddingRight"] = padding.right();
    json["paddingBottom"] = padding.bottom();

    json["lineSpacing"] = lineSpacing;
    return json;
}

void TextSettingProps::fromJson(const QJsonObject &json) {
    if (json.contains("splitView"))
        splitView = json["splitView"].toBool();
    if (json.contains("textView"))
        textView = json["textView"].toBool();

    if (json.contains("font"))
        font.fromString(json["font"].toString());

    if (json.contains("textColor"))
        textColor = QColor(json["textColor"].toString());
    if (json.contains("backgroundColor"))
        backgroundColor = QColor(json["backgroundColor"].toString());

    int left = json.contains("paddingLeft") ? json["paddingLeft"].toInt() : 10;
    int top = json.contains("paddingTop") ? json["paddingTop"].toInt() : 10;
    int right = json.contains("paddingRight") ? json["paddingRight"].toInt() : 10;
    int bottom = json.contains("paddingBottom") ? json["paddingBottom"].toInt() : 10;
    padding = QMargins(left, top, right, bottom);

    if (json.contains("lineSpacing"))
        lineSpacing = json["lineSpacing"].toDouble();
}
QString TextSettingProps::getFileName() {
	return "TextSettingProps.json";
}