#pragma once

#include "db.hpp"
#include <QCryptographicHash>
#include <QDir>
#include <QFile>
#include <QRegularExpression>

static void writeFileMD5(QFile &dest, const QString &fname) {
  QFile f(fname);
  if (!f.open(QIODevice::ReadOnly)) {
    return;
  }

  auto data = f.readAll();
  f.close();
  data.replace(QByteArray("\r\n"), QByteArray("\n"));
  auto hash = QCryptographicHash::hash(data, QCryptographicHash::Md5).toHex();
  dest.write(fname.toUtf8() + '=' + hash + ';');
}

static void writeDirMD5(QFile &dest, const QString &dir,
                        const QString &filter) {
  QDir d(dir);
  auto entries = d.entryInfoList(
      QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
  auto re = QRegularExpression::fromWildcard(filter);
  const auto disabled = DB::getInstance()->getDisabledPacks();
  foreach (QFileInfo info, entries) {
    if (info.isDir() && !info.fileName().endsWith(".disabled") &&
        !disabled.contains(info.fileName())) {
      writeDirMD5(dest, info.filePath(), filter);
    } else {
      if (re.match(info.fileName()).hasMatch()) {
        writeFileMD5(dest, info.filePath());
      }
    }
  }
}
