//
// Created by Arne Gockeln on 30.07.17.
//

#include <QDebug>
#include "Auth.h"
#include <QRegularExpression>
#include "include/libcppotp/otp.h"
#include <ctime>

using namespace CppTotp;
using namespace std;

Auth* Auth::m_instance = 0;
Auth* Auth::getInstance() {
  if(m_instance == 0){
    m_instance = new Auth();
  }
  return m_instance;
}

Auth::Auth(){

}

/**
 * Generate time based one time token with provided secret
 * @param t_secret QString
 * @return uint32_t
 */
uint32_t Auth::generateToken(const QString t_secret, const int t_interval) {
  const Bytes::ByteString key = Bytes::fromBase32(t_secret.toStdString());
  uint32_t token = totp(key, time(nullptr), 0, t_interval, 6);
  //qDebug() << QString("%1 (%2 remain)").arg(token).arg(t_interval - (time(nullptr) % t_interval));

  return token;
}

/**
 * Decode base32 string and return a binary representation
 *
 * @param t_base32str
 * @return QString
 */
QString Auth::base32_decode(const QString t_base32str){
  QString base32_string = t_base32str.toUpper();
  QString allowedChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

  // check for invalid characters
  QRegularExpression regEx("^[ABCDEFGHIJKLMNOPQRSTUVWXYZ234567]+$");
  QRegularExpressionMatch match = regEx.match(base32_string);
  if(!match.hasMatch()){
    return "no match";
  }

  const int strLen = base32_string.length();
  int j = 0, n = 0;
  QString binary;
  for(int i = 0; i < strLen; i++){
    // move buffer left by 5 to make room
    n = n << 5;
    // add value into buffer
    n = n + allowedChars.indexOf(base32_string.at(i));
    // keep track of number of bits in buffer
    j += 5;

    if(j >= 8){
      j -= 8;
      // chr( ( $n & ( 0xFF << $j ) ) >> $j );
      char c = (n & ( 0xFF << j ) ) >> j;
      binary.append(c);
    }
  }

  return binary;
}