﻿#ifndef TEXTSETTINGPROPS_H
#define TEXTSETTINGPROPS_H

#include <QFont>
#include <QColor>
#include <QMargins>
#include <QJsonObject>
#include "../IJsonSerializable.h"

class TextSettingProps : public IJsonSerializable
{
public:
    TextSettingProps();
    TextSettingProps(bool splitView,
                 bool textView,
                 const QFont &font,
                 const QColor &textColor,
                 const QColor &backgroundColor,
                 const QMargins &padding,
                 double lineSpacing);

    // getter, setter
    bool isSplitView() const;
    void setSplitView(bool value);

    bool isTextView() const;
    void setTextView(bool value);

    QFont getFont() const;
    void setFont(const QFont &font);

    QColor getTextColor() const;
    void setTextColor(const QColor &color);

    QColor getBackgroundColor() const;
    void setBackgroundColor(const QColor &color);

    QMargins getPadding() const;
    void setPadding(const QMargins &margins);

    double getLineSpacing() const;
    void setLineSpacing(double spacing);

    // JSON 직렬화/역직렬화 메서드
    QJsonObject toJson() const override;
    void fromJson(const QJsonObject &json) override;
	QString getFileName() override;

private:
    bool splitView;
    bool textView;
    QFont font;
    QColor textColor;
    QColor backgroundColor;
    QMargins padding;
    double lineSpacing;
};

#endif // TEXTSETTINGS_H
