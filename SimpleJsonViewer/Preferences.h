#pragma once

// default values
#define DEFAULT_SHOW_RAW_JSON false

class Preferences : public QObject
{
    Q_OBJECT

public:
    // get config instance
    static Preferences &getInstance() { static Preferences _instance; return _instance; };

    // preferences getters/setters
    bool showRawJson() const              { return _settings.value("ShowRawJson", qVariantFromValue<bool>(_defaultShowRawJson)).toBool();      }
    void setShowRawJson(const bool value) { _settings.setValue("ShowRawJson", qVariantFromValue<bool>(value)); emit showRawJsonChanged(value); }

signals:
    void showRawJsonChanged(const bool value);

private:
    Preferences() = default;
    ~Preferences() = default;
    Preferences(const Preferences&) = delete;
    Preferences(Preferences&&) = delete;
    Preferences &operator = (const Preferences&) = delete;
    Preferences &operator = (Preferences&&) = delete;

    const bool _defaultShowRawJson = false;
    QSettings _settings;
};
