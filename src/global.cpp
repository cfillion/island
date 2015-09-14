#include "global.hpp"

#include <QRegularExpression>

QStringList Island::InputToBuffer(const QString &input)
{
  QStringList output;

  const QRegularExpression regex("(\\<[^\\>]+\\>|.)");
  auto matches = regex.globalMatch(input);

  while(matches.hasNext()) {
    auto match = matches.next();
    output << match.captured();
  }

  return output;
}
