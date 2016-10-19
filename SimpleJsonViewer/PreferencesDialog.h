#pragma once

namespace Ui
{
    class PreferencesDialog;
}

class Preferences;

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    PreferencesDialog(QWidget *parent = Q_NULLPTR);
    ~PreferencesDialog() {}

private:
    Ui::PreferencesDialog* _ui;
};
