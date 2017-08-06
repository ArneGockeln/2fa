//
// Created by Arne Gockeln on 29.07.17.
//

#include <QIODevice>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include <QMessageBox>
#include "Storage.h"
#include "ServiceItem.h"

using namespace TwoFA;

// will append to /Users/<username>/Documents/
#ifndef kSTORAGE_DIR
#define kSTORAGE_DIR ".twofa"
#endif

// will append to /Users/<username>/Documents/<kSTORAGE_DIR>/
#ifndef kSTORAGE_FILENAME
#define kSTORAGE_FILENAME "twofa.xml"
#endif

Storage* Storage::m_instance = 0;

Storage* Storage::getInstance() {
  if(m_instance == 0){
    m_instance = new Storage();
  }

  return m_instance;
}

Storage::Storage() {
  checkStorage();
}

/**
 * Check if storage file exists,
 * if not create it
 */
void Storage::checkStorage() {
  QString storagePath = QString(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
  storagePath.append("/");
  storagePath.append(kSTORAGE_DIR);

  m_filename = storagePath;
  m_filename.append("/");
  m_filename.append(kSTORAGE_FILENAME);

  QDir dir;
  if(!dir.exists(storagePath)){
    // create directory
    dir.mkpath(storagePath);
    // check if its read write possible
    // if the file does not exist, it will be created
    QFile file(m_filename);
    if(file.open(QIODevice::ReadWrite)){
      qDebug() << "file exists" << m_filename;
    }
  }
}

/**
 * Write tableModel data to xml file
 *
 * @param t_tableModel
 */
void Storage::write(TableModel *t_tableModel) {
  if (m_filename.isEmpty()) {
    QMessageBox::critical(Q_NULLPTR, QObject::tr("Error"), QObject::tr("Storage filename is empty!"));
    return;
  }

  QFile file(m_filename);

  if (!file.open(QFile::WriteOnly | QFile::Text)) {
    QMessageBox::critical(Q_NULLPTR, QObject::tr("Error"),
                          QObject::tr("Cannot write to file %1:\n%2.")
                              .arg(m_filename)
                              .arg(file.errorString())
    );
    return;
  }

  QXmlStreamWriter xmlStreamWriter(&file);
  xmlStreamWriter.setAutoFormatting(true);

  xmlStreamWriter.writeStartDocument();
  xmlStreamWriter.writeDTD("<!DOCTYPE twofa>");
  xmlStreamWriter.writeStartElement("twofa");
  xmlStreamWriter.writeAttribute("version", "1.0");
  xmlStreamWriter.writeStartElement("sites");

  QList<ServiceItem> serviceList = t_tableModel->getList();

  if (serviceList.isEmpty()){
    return;
  }

  for (int i = 0; i < serviceList.size(); i++) {
    ServiceItem serviceItem(serviceList.at(i));
    xmlStreamWriter.writeStartElement("site");
    xmlStreamWriter.writeAttribute("name", serviceItem.getName());
    xmlStreamWriter.writeAttribute("secret", serviceItem.getSecret());
    xmlStreamWriter.writeAttribute("interval", QString::number(serviceItem.getInterval()));
    xmlStreamWriter.writeEndElement();
  }

  xmlStreamWriter.writeEndElement();
  xmlStreamWriter.writeEndDocument();

  file.close();
}

/**
 * Read data from xml file and populate tableModel
 * @param t_tableModel
 */
void Storage::read(TableModel *t_tableModel){
  if(m_filename.isEmpty()){
    QMessageBox::critical(Q_NULLPTR, QObject::tr("Error"), QObject::tr("Storage filename is empty!"));
    return;
  }

  QFile file(m_filename);

  if(!file.open(QFile::ReadOnly)){
    QMessageBox::critical(Q_NULLPTR, QObject::tr("Error"),
                          QObject::tr("Cannot read file %1:\n%2.")
                              .arg(m_filename)
                              .arg(file.errorString())
    );
    return;
  }

  QXmlStreamReader xmlStreamReader(&file);

  while(!xmlStreamReader.atEnd() && !xmlStreamReader.hasError()){
    // read next element
    QXmlStreamReader::TokenType token = xmlStreamReader.readNext();
    // if this is the start document token, continue
    if(token == QXmlStreamReader::StartDocument){
      continue;
    }

    // if this is the start of an element, try to read it
    if(token == QXmlStreamReader::StartElement){
      if(xmlStreamReader.name() == "site"){

        ServiceItem serviceItem;
        serviceItem.setName(xmlStreamReader.attributes().value("name").toString());
        serviceItem.setSecret(xmlStreamReader.attributes().value("secret").toString());
        serviceItem.setInterval(xmlStreamReader.attributes().value("interval").toInt());

        if(!t_tableModel->getList().contains(serviceItem)){
          t_tableModel->insertRows(0, 1, QModelIndex());

          QModelIndex index = t_tableModel->index(0, 0, QModelIndex());
          t_tableModel->setData(index, serviceItem.getName(), Qt::EditRole);
          index = t_tableModel->index(0, 1, QModelIndex());
          t_tableModel->setData(index, serviceItem.getSecret(), Qt::EditRole);
          index = t_tableModel->index(0, 2, QModelIndex());
          t_tableModel->setData(index, serviceItem.getInterval(), Qt::EditRole);
        }
      }
    }
  }

  xmlStreamReader.clear();
  file.close();
}