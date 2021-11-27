#include "Python3BaseVisitor.h"
// antlrcpp::Any operator+(antlrcpp::Any &a,antlrcpp::Anyb);
// antlrcpp::Any operator-(antlrcpp::Any &a,antlrcpp::Anyb);
// antlrcpp::Any operator*(antlrcpp::Any &a,antlrcpp::Anyb);
// antlrcpp::Any operator/(antlrcpp::Any &a,antlrcpp::Anyb);
// antlrcpp::Any operator<<(antlrcpp::Any &a,antlrcpp::Anyb); // used by operator "//"(IDIV)
void Turn_Int_To_Double(antlrcpp::Any &); // turn any Any to double if possible.
antlrcpp::Any operator-(antlrcpp::Any);
antlrcpp::Any &operator+=(antlrcpp::Any &,antlrcpp::Any);
antlrcpp::Any &operator-=(antlrcpp::Any &,antlrcpp::Any);
antlrcpp::Any &operator*=(antlrcpp::Any &,antlrcpp::Any);
antlrcpp::Any &operator/=(antlrcpp::Any &,antlrcpp::Any);
antlrcpp::Any &operator<<=(antlrcpp::Any &,antlrcpp::Any); // used by operator "//"(IDIV).
antlrcpp::Any &operator%=(antlrcpp::Any &,antlrcpp::Any); // a%b=a-(a//b)*b
bool operator<(antlrcpp::Any,antlrcpp::Any);
bool operator>(antlrcpp::Any,antlrcpp::Any);
bool operator==(antlrcpp::Any,antlrcpp::Any);
bool operator<=(antlrcpp::Any,antlrcpp::Any);
bool operator>=(antlrcpp::Any,antlrcpp::Any);
bool operator!=(antlrcpp::Any,antlrcpp::Any);