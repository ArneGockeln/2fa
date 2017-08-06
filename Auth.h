//
// Created by Arne Gockeln on 30.07.17.
//

#ifndef INC_2FA_AUTH_H
#define INC_2FA_AUTH_H


#include <QString>

class Auth {
private:
  static Auth* m_instance;
  Auth();

public:
  static Auth* getInstance();
  uint32_t generateToken(const QString t_secret, const int t_interval = 30);
  QString base32_decode(const QString t_base32str);
  time_t getUTCTime();
};


#endif //INC_2FA_AUTH_H
