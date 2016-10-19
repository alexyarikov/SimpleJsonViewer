#include "stdafx.h"
#include "PreferencesDialog.h"
#include "Preferences.h"
#include "ui_PreferencesDialog.h"

PreferencesDialog::PreferencesDialog(QWidget *parent/* = Q_NULLPTR*/) : QDialog(parent)
{
    _ui = new Ui::PreferencesDialog();
    _ui->setupUi(this);

    _ui->chbShowRawJson->setChecked(Preferences::getInstance().showRawJson());
    QObject::connect(_ui->chbShowRawJson, &QAbstractButton::toggled, [](bool value) { Preferences::getInstance().setShowRawJson(value); });
}
