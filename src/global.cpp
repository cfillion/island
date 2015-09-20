#include "global.hpp"

#include <QRegularExpression>

Island::Buffer Island::InputToBuffer(const QString &input)
{
  Buffer output;

  const QRegularExpression regex("(\\<[^\\>]+\\>|.)");
  auto matches = regex.globalMatch(input);

  while(matches.hasNext()) {
    auto match = matches.next();
    output << match.captured();
  }

  return output;
}
