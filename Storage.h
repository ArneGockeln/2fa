//
// Created by Arne Gockeln on 29.07.17.
//

#ifndef INC_2FA_STORAGE_H
#define INC_2FA_STORAGE_H

#include <QString>
#include <QFile>
#include <QDomDocument>
#include <QXmlStreamWriter>
#include "TableModel.h"

class Storage {
private:
  /**
   * Here will be the instance stored
   */
  static Storage* m_instance;

  /*
   * Private constructor to prevent instancing
   */
  Storage();

  /**
   * The complete storage path with filename
   */
  QString m_filename;

public:
  /**
   * Static access method
   * @return
   */
  static Storage* getInstance();
  void checkStorage();
  void write(TableModel *t_tableModel);
  void read(TableModel *t_tableModel);
};


#endif //INC_2FA_STORAGE_H
