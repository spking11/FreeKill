#pragma once

#include <QJsonArray>
#include <QJsonObject>
#include <QStringList>
#include <sqlite3.h>

static int callback(void *jsonDoc, int argc, char **argv, char **cols) {
  QJsonObject obj;
  for (int i = 0; i < argc; i++) {
    obj[QString(cols[i])] = QString(argv[i] ? argv[i] : "#null");
  }
  ((QJsonArray *)jsonDoc)->append(obj);
  return 0;
}

static inline QJsonArray SelectFromDatabase(sqlite3 *db, const QString &sql) {
  QJsonArray arr;
  auto bytes = sql.toUtf8();
  sqlite3_exec(db, bytes.data(), callback, (void *)&arr, nullptr);
  return arr;
}

class DB {
public:
  static DB *getInstance() {
    static DB *instance = new DB();
    return instance;
  }
  QStringList getDisabledPacks() { return disabled_packs; }

private:
  DB() {
    foreach (auto e,
             SelectFromDatabase(db, "SELECT name, enabled FROM packages;")) {
      auto obj = e.toObject();
      auto pack = obj["name"].toString();
      auto enabled = obj["enabled"].toString().toInt() == 1;

      if (!enabled) {
        disabled_packs << pack;
      }
    }
  }

  sqlite3 *db;
  QStringList disabled_packs;
};

