#include <QBuffer>
#include <QDebug>
#include <QDir>
#include <git2.h>
#include <git2/refs.h>
#include <git2/repository.h>
#include <git2/types.h>
#include <git2/worktree.h>
#include <util.hpp>

int main() {
  qDebug() << "hacking";

  git_libgit2_init();
  git_repository *repo = nullptr;
  git_reference *master_branch = nullptr;
  git_worktree *master_worktree = nullptr;

  int ret = 0;
  QString err_msg;

  if ((ret = git_repository_open(&repo, ".")) != 0) {
    err_msg = "open repository failed.";
    goto clean;
  }

  if ((ret = git_reference_lookup(&master_branch, repo, "refs/heads/master")) !=
      0) {
    err_msg = "can't find master branch.";
    goto clean;
  }

  if (git_worktree_lookup(&master_worktree, repo, "master") != 0) {
    git_worktree_add_options opts = {GIT_WORKTREE_ADD_OPTIONS_VERSION, 0, true,
                                     master_branch, GIT_CHECKOUT_OPTIONS_INIT};
    if ((ret = git_worktree_add(&master_worktree, repo, "master",
                                "build/master", &opts)) != 0) {
      err_msg = "can't add master worktree.";
      goto clean;
    }
  }

clean:
  if (repo != nullptr) {
    git_repository_free(repo);
  }
  if (master_branch != nullptr) {
    git_reference_free(master_branch);
  }
  if (master_worktree != nullptr) {
    git_worktree_free(master_worktree);
  }
  if (!err_msg.isEmpty()) {
    qFatal() << err_msg;
  }

  QBuffer flist;
  {
    flist.open(QBuffer::WriteOnly);
    QDir root("build/master/");
    writeDirMD5(flist, root, "build/master/lua", "*.lua");
    writeDirMD5(flist, root, "build/master/Fk", "*.qml");
    writeDirMD5(flist, root, "build/master/Fk", "*.js");
    flist.close();
  }
  qDebug() << flist.data();
  QFile file("build/hacker_flist.txt");
  {
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
      qFatal("Cannot open flist.txt. Quitting.");
    }
    QTextStream(&file) << "R\"(" << flist.data() << ")\"";
  }

  qDebug() << "hacked";
  return 0;
}