//
// Created by Arne Gockeln on 04.08.17.
//

#ifndef INC_2FA_SERVICEITEM_H
#define INC_2FA_SERVICEITEM_H

#include <QString>

namespace TwoFA {
  class ServiceItem {
  private:
    QString m_name;
    QString m_secret;
    int m_interval;

  public:
    ServiceItem() {
      m_name = " ";
      m_secret = " ";
      m_interval = 30;
    };

    ServiceItem(QString& t_name, QString& t_secret, int& t_interval):
        m_name(t_name), m_secret(t_secret), m_interval(t_interval) {

    }

    // copy constructor
    ServiceItem(const ServiceItem& other): m_name(other.m_name), m_secret(other.m_secret), m_interval(other.m_interval){

    }

    // assignment operator
    ServiceItem& operator=(const ServiceItem& other) {
      setName(other.getName());
      setSecret(other.getSecret());
      setInterval(other.getInterval());

      return *this;
    }

    // compare operator !=
    bool operator!=(const ServiceItem& item1){
      return (item1.getName() != getName() || item1.getSecret() != getSecret() || item1.getInterval() != getInterval());
    }
    // compare operator ==
    bool operator==(const ServiceItem& item1){
      return (item1.getName() == getName() && item1.getSecret() == getSecret() && item1.getInterval() == getInterval());
    }

    ~ServiceItem(){}

    QString getName() const {
      return m_name;
    }
    void setName(const QString& t_name){
      m_name = t_name;
    }

    QString getSecret() const {
      return m_secret;
    }
    void setSecret(const QString& t_secret){
      m_secret = t_secret;
    }

    int getInterval() const {
      return m_interval;
    }
    void setInterval(const int& t_interval){
      m_interval = t_interval;
    }
  };
}

#endif //INC_2FA_SERVICEITEM_H
