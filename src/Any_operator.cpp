#include "Any_operator.h"
#include "Evalvisitor.h"
#include "int2048.h"
using std::string;
using namespace sjtu;
typedef sjtu::int2048 num_type;
void Turn_Int_To_Double(antlrcpp::Any &obj)
{
	// if (obj.is<num_type>())
	// 	obj = double(obj.as<num_type>());
	return;
}
antlrcpp::Any operator-(antlrcpp::Any obj)
{
	if (obj.is<num_type>())
		return -obj.as<num_type>();
	if (obj.is<double>())
		return -obj.as<double>();
	throw("INVAILD -()operator");
	return obj;
}
antlrcpp::Any &operator+=(antlrcpp::Any &lhs,antlrcpp::Any rhs)
{
	if (lhs.is<num_type>() && rhs.is<num_type>())
	{
		lhs = lhs.as<num_type>() + rhs.as<num_type>();
		return lhs;
	}
	Turn_Int_To_Double(lhs);
	Turn_Int_To_Double(rhs);
	if (lhs.is<double>() && rhs.is<double>())
	{
		lhs = lhs.as<double>() + rhs.as<double>();
		return lhs;
	}
	if (lhs.is<string>() && rhs.is<string>())
	{
		lhs = lhs.as<string>() + rhs.as<string>();
		return lhs;
	}
	throw("INVAILD +operator");
	return lhs;
}

antlrcpp::Any &operator-=(antlrcpp::Any &lhs,antlrcpp::Any rhs)
{
	if (lhs.is<num_type>() && rhs.is<num_type>())
	{
		lhs = lhs.as<num_type>() - rhs.as<num_type>();
		return lhs;
	}
	Turn_Int_To_Double(lhs);
	Turn_Int_To_Double(rhs);
	if (lhs.is<double>() && rhs.is<double>())
	{
		lhs = lhs.as<double>() - rhs.as<double>();
		return lhs;
	}
	throw("INVAILD -operator");
	return lhs;
}
antlrcpp::Any &operator*=(antlrcpp::Any &lhs,antlrcpp::Any rhs)
{
	if (lhs.is<num_type>() && rhs.is<num_type>())
	{
		lhs = lhs.as<num_type>() * rhs.as<num_type>();
		return lhs;
	}
	Turn_Int_To_Double(lhs);
	Turn_Int_To_Double(rhs);
	if (lhs.is<double>() && rhs.is<double>())
	{
		lhs = lhs.as<double>() * rhs.as<double>();
		return lhs;
	}
	if (lhs.is<string>() && rhs.is<num_type>()) // repeating string
	{
		string original_string = lhs.as<string>();
		num_type temp_time = rhs.as<num_type>();
		string ans;
		while (!temp_time.Is_Zero())
		{
			ans.append(original_string);
			temp_time-=1;
		}
		lhs = ans;
		return lhs;
	}
	throw("INVAILD *operator");
	return lhs;
}
antlrcpp::Any &operator/=(antlrcpp::Any &lhs,antlrcpp::Any rhs)
{
	Turn_Int_To_Double(lhs);
	Turn_Int_To_Double(rhs);
	if (lhs.is<double>() && rhs.is<double>())
	{
		lhs = lhs.as<double>() * rhs.as<double>();
		return lhs;
	}
	throw("INVAILD /operator");
	return lhs;
}
antlrcpp::Any &operator<<=(antlrcpp::Any &lhs,antlrcpp::Any rhs) // used by //operator.
{
	if (lhs.is<num_type>() && rhs.is<num_type>())
	{
		lhs = lhs.as<num_type>() / rhs.as<num_type>();
		return lhs;
	}
	throw("INVAILD //operator");
	return lhs;
}
antlrcpp::Any &operator%=(antlrcpp::Any &lhs,antlrcpp::Any rhs) // a%b=a-(a//b)*b
{
	if (lhs.is<num_type>() && rhs.is<num_type>())
	{
		lhs = lhs.as<num_type>() % rhs.as<num_type>();
		return lhs;
	}
	std::cerr<<"fuck";
	if (rhs.is<ReturnVal>() || lhs.is<ReturnVal>())
		std::cout<<"isreturn val";
	if (rhs.is<ValName>() || lhs.is<ValName>())
		std::cout<<"isnameval";
	if(rhs.isNull() || lhs.isNull())
		std::cout<<"isnull";
	throw("INVAILD %operator");
	return lhs;
}

bool operator<(antlrcpp::Any lhs,antlrcpp::Any rhs)
{
	if (lhs.is<num_type>() && rhs.is<num_type>())
		return lhs.as<num_type>() < rhs.as<num_type>();
	Turn_Int_To_Double(lhs);
	Turn_Int_To_Double(rhs);
	if (lhs.is<double>() && rhs.is<double>())
		return lhs.as<double>() < rhs.as<double>();
	if (lhs.is<string>() && rhs.is<string>())
		return lhs.as<string>() < rhs.as<string>();
	if (lhs.is<bool>() && rhs.is<bool>())
		return !lhs.as<bool>() && rhs.as<bool>();
	throw("INVAILD comp operator");
	return false;
}
bool operator>(antlrcpp::Any lhs,antlrcpp::Any rhs)
{
	return rhs<lhs;
}
bool operator==(antlrcpp::Any lhs,antlrcpp::Any rhs)
{
	if (lhs.is<num_type>() && rhs.is<num_type>())
		return lhs.as<num_type>() == rhs.as<num_type>();
	Turn_Int_To_Double(lhs);
	Turn_Int_To_Double(rhs);
	if (lhs.is<double>() && rhs.is<double>())
		return lhs.as<double>() == rhs.as<double>();
	if (lhs.is<string>() && rhs.is<string>())
		return lhs.as<string>() == rhs.as<string>();
	if (lhs.is<bool>() && rhs.is<bool>())
		return lhs.as<bool>() == rhs.as<bool>();
	return false;
	}
bool operator<=(antlrcpp::Any lhs,antlrcpp::Any rhs)
{
	return (lhs < rhs || lhs == rhs);
}
bool operator>=(antlrcpp::Any lhs,antlrcpp::Any rhs)
{
	return rhs <= lhs;
}
bool operator!=(antlrcpp::Any lhs,antlrcpp::Any rhs)
{
	return !(lhs == rhs);
}