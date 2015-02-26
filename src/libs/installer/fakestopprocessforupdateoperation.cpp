/**************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Qt Installer Framework.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
**
** $QT_END_LICENSE$
**
**************************************************************************/

#include "fakestopprocessforupdateoperation.h"

#include "messageboxhandler.h"
#include "packagemanagercore.h"

using namespace KDUpdater;
using namespace QInstaller;

FakeStopProcessForUpdateOperation::FakeStopProcessForUpdateOperation()
{
    setName(QLatin1String("FakeStopProcessForUpdate"));
}

void FakeStopProcessForUpdateOperation::backup()
{
}

bool FakeStopProcessForUpdateOperation::performOperation()
{
    return true;
}

bool FakeStopProcessForUpdateOperation::undoOperation()
{
    setError(KDUpdater::UpdateOperation::NoError);
    if (!checkArgumentCount(1))
        return false;

    PackageManagerCore *const core = value(QLatin1String("installer")).value<PackageManagerCore*>();
    if (!core) {
        setError(KDUpdater::UpdateOperation::UserDefinedError, tr("Could not get package manager "
            "core."));
        return false;
    }

    QStringList processes = arguments().at(0).split(QLatin1Char(','), QString::SkipEmptyParts);
    for (int i = processes.count() - 1; i >= 0; --i) {
        if (!core->isProcessRunning(processes.at(i)))
            processes.removeAt(i);
    }

    if (processes.isEmpty())
        return true;

    if (processes.count() == 1) {
        setError(UpdateOperation::UserDefinedError, tr("This process should be stopped before "
            "continuing: %1").arg(processes.first()));
    } else {
        const QString sep = QString::fromWCharArray(L"\n   \u2022 ");   // Unicode bullet
        setError(UpdateOperation::UserDefinedError, tr("These processes should be stopped before "
            "continuing: %1").arg(sep + processes.join(sep)));
    }
    return false;
}

bool FakeStopProcessForUpdateOperation::testOperation()
{
    return true;
}

Operation *FakeStopProcessForUpdateOperation::clone() const
{
    return new FakeStopProcessForUpdateOperation();
}
