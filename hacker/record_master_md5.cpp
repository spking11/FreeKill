#include <util.hpp>

int main() {
  QFile flist("hacker_flist.txt");
  if (!flist.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    qFatal("Cannot open flist.txt. Quitting.");
  }
  writeDirMD5(flist, "lua", "*.lua");
  writeDirMD5(flist, "Fk", "*.qml");
  writeDirMD5(flist, "Fk", "*.js");
  flist.close();
  return 0;
}