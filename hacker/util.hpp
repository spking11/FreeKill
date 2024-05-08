#pragma once

#include <QCryptographicHash>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>

static void writeFileMD5(QFile &dest, const QDir &root, const QString &fname) {
  QFile f(fname);
  if (!f.open(QIODevice::ReadOnly)) {
    return;
  }

  auto data = f.readAll();
  f.close();
  data.replace(QByteArray("\r\n"), QByteArray("\n"));
  auto hash = QCryptographicHash::hash(data, QCryptographicHash::Md5).toHex();
  dest.write(
      root.relativeFilePath(QFileInfo(fname).absoluteFilePath()).toUtf8() +
      '=' + hash + ';');
}

static void writeDirMD5(QFile &dest, const QDir &root, const QString &dir,
                        const QString &filter) {
  QDir d(dir);
  auto entries = d.entryInfoList(
      QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
  auto re = QRegularExpression::fromWildcard(filter);
  foreach (QFileInfo info, entries) {
    if (info.isDir()) {
      writeDirMD5(dest, root, info.filePath(), filter);
    } else {
      if (re.match(info.fileName()).hasMatch()) {
        writeFileMD5(dest, root, info.filePath());
      }
    }
  }
}
