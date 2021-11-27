#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H

#include "Any_operator.h"
#include "int2048.h"
typedef sjtu::int2048 num_type;
using std::string;
#include "Python3BaseVisitor.h"
#include <map>
// typedef int num_type;

static std::map<string, antlrcpp::Any> val_map;
class ValName {
private:
    string name;

public:
    ValName() {};
    ValName(const string& other)
        : name(other) {};
    ValName(const ValName& other)
        : name(other.name) {};
    operator string() const
    {
        return name;
    }
    antlrcpp::Any Get_Val()
    {
        if (val_map.find(name) == val_map.end())
            throw((val_map.size()));
        return val_map[name];
    }
    ValName& operator=(const ValName& other)
    {
        name = other.name;
        return *this;
    }
};

static void Get_Val(antlrcpp::Any& obj)
{
    if (obj.is<ValName>())
        obj = obj.as<ValName>().Get_Val();
    return;
}

static void Edit_Val(const antlrcpp::Any& index, const antlrcpp::Any& value)
{
    val_map[string(index.as<ValName>())] = value;
    return;
}
static bool Get_Bool(antlrcpp::Any obj) // turn ANY Any to bool 
{
	Get_Val(obj);
	if (obj.is<bool>())
		return obj.as<bool>();
	if (obj.is<num_type>())
		return !obj.as<num_type>().Is_Zero();
	if (obj.is<double>())
		return obj.as<double>() != double(0);
	if (obj.is<string>())
		return obj.as<string>().size();
	if (obj.isNull())
		return false;
	throw("INVAILD CONDITION");
	return false;
}

class EvalVisitor : public Python3BaseVisitor {

    virtual antlrcpp::Any visitAtom_expr(Python3Parser::Atom_exprContext* ctx) override
    {
        if (!ctx->trailer())
            return visitAtom(ctx->atom());
        string function_name = ctx->atom()->getText();
        auto argu_list = ctx->trailer()->arglist()->argument();
        if (function_name == "print") {
            for (auto argu_iter = argu_list.begin(); argu_iter != argu_list.end();
                 argu_iter++) {
                if (argu_iter != argu_list.begin())
                    std::cout << ' ';
                auto ans = visitArgument(*argu_iter);
                Get_Val(ans);
                if (ans.is<num_type>())
                    std::cout << ans.as<num_type>();
                if (ans.is<string>())
                    std::cout << ans.as<string>();
                if (ans.is<bool>()) {
                    if (ans.as<bool>())
                        std::cout << "True";
                    else
                        std::cout << "False";
                }
                if (ans.isNull())
                    std::cout << "None";
            }
            std::cout << std::endl;
            return 0;
        }
        return visitChildren(ctx);
    }
    virtual antlrcpp::Any visitArgument(Python3Parser::ArgumentContext* ctx) override
    {
        return visitTest(ctx->test()[0]);
        // return visitChildren(ctx);
    }
    virtual antlrcpp::Any visitAtom(Python3Parser::AtomContext* ctx) override
    {
        if (!ctx->STRING().empty()) {
            string ans;
            auto string_list_to_treat = ctx->STRING();
            for (auto& temp_ctx : string_list_to_treat) {
                string temp_string = temp_ctx->getText();
                temp_string = temp_string.substr(1, temp_string.length() - 2);
                // std::cout<<x;
                ans.append(temp_string);
            }
            return ans;
        }
        if (ctx->NUMBER()) {
            num_type ans = ctx->NUMBER()->getText();
            return ans;
        }
        if (ctx->TRUE()) {
            return true;
        }
        if (ctx->FALSE()) {
            return false;
        }
        if (ctx->NONE()) {
            return antlrcpp::Any();
        }
        if (ctx->NAME()) {
            string val_name = ctx->NAME()->getText();
            return ValName(val_name);
        }
        throw("UB");
    }

    virtual antlrcpp::Any visitExpr_stmt(Python3Parser::Expr_stmtContext* ctx) override
    {
        auto test_list = ctx->testlist();
        if (test_list.size() == 1)
            return visitChildren(ctx);
        if (!ctx->ASSIGN().empty()) {
            auto now = test_list.rbegin();
            antlrcpp::Any ans = visitTestlist(*now);
            Get_Val(ans);
            for (now++; now != test_list.rend(); now++) {
                antlrcpp::Any now_stmt = visitTestlist(*now);
                if (now_stmt.is<ValName>())
                    Edit_Val(now_stmt, ans);
                else
                    throw("fuck exprstmt");
            }
            return ans;
        }
        if (!ctx->augassign()->isEmpty()) {
            if (test_list.size() > 2)
                throw("INVAILD ?=");
            auto temp_augassign = ctx->augassign();
            antlrcpp::Any lhs = visitTestlist(*test_list.begin());
            antlrcpp::Any temp_ans = lhs;
            Get_Val(temp_ans);
            antlrcpp::Any rhs = visitTestlist(*test_list.rbegin());
            Get_Val(rhs);
            if (temp_augassign->ADD_ASSIGN()) // +=
                temp_ans += rhs;
            if (temp_augassign->SUB_ASSIGN()) // -=
                temp_ans -= rhs;
            if (temp_augassign->MULT_ASSIGN()) // *=
                temp_ans *= rhs;
            if (temp_augassign->DIV_ASSIGN()) // /=
                temp_ans /= rhs;
            if (temp_augassign->IDIV_ASSIGN()) // //=
                temp_ans <<= rhs;
            if (temp_augassign->MOD_ASSIGN()) // %=
                temp_ans %= rhs;
            Edit_Val(lhs, temp_ans);
            return temp_ans;
        }
        throw("INVAILD expr_stmt");
        return 0;
    }
    virtual antlrcpp::Any visitArith_expr(Python3Parser::Arith_exprContext* ctx) override
    {
        auto term_list = ctx->term();
        auto op_list = ctx->addorsub_op();
        if (term_list.size() == 1)
            return visitChildren(ctx);
        auto term_iter = term_list.begin();
        auto op_iter = op_list.begin();
        antlrcpp::Any ans = visitTerm(*term_iter);
        Get_Val(ans);
        for (term_iter++; term_iter != term_list.end(); term_iter++, op_iter++) {
            auto temp_ans = visitTerm(*term_iter);
            Get_Val(temp_ans);
            if ((*op_iter)->ADD())
                ans += temp_ans;
            if ((*op_iter)->MINUS())
                ans -= temp_ans;
        }
        return ans;
    }
    virtual antlrcpp::Any visitTerm(Python3Parser::TermContext* ctx) override
    {
        auto factor_list = ctx->factor();
        auto op_list = ctx->muldivmod_op();
        if (ctx->factor().size() == 1)
            return visitChildren(ctx);
        auto factor_iter = factor_list.begin();
        auto op_iter = op_list.begin();
        auto ans = visitFactor(*factor_iter);
        Get_Val(ans);
        for (factor_iter++; factor_iter != factor_list.end(); factor_iter++, op_iter++) {
            antlrcpp::Any temp_ans = visitFactor(*factor_iter);
            Get_Val(temp_ans);
            if ((*op_iter)->STAR())
                ans *= temp_ans;
            if ((*op_iter)->DIV())
                ans /= temp_ans;
            if ((*op_iter)->IDIV())
                ans <<= temp_ans;
            if ((*op_iter)->MOD())
                ans %= temp_ans;
        }
        return ans;
    }
    virtual antlrcpp::Any visitFactor(Python3Parser::FactorContext* ctx) override
    {
        if (ctx->MINUS())
            return -visitChildren(ctx);
        return visitChildren(ctx);
    }
    virtual antlrcpp::Any visitComparison(Python3Parser::ComparisonContext* ctx) override
    {
        auto arith_list = ctx->arith_expr();
        if (arith_list.size() == 1)
            return visitChildren(ctx);
        auto op_list = ctx->comp_op();
        auto arith_iter = arith_list.begin();
        auto op_iter = op_list.begin();
        antlrcpp::Any lst_val = visitArith_expr(*arith_iter);
        Get_Val(lst_val);
        for (arith_iter++; arith_iter != arith_list.end(); arith_iter++, op_iter++) {
            auto now_val = visitArith_expr(*arith_iter);
            Get_Val(now_val);
            if ((*op_iter)->LESS_THAN()) // "<"
                if (!(lst_val < now_val))
                    return false;
            if ((*op_iter)->GREATER_THAN()) // ">"
                if (!(lst_val > now_val))
                    return false;
            if ((*op_iter)->EQUALS()) // "=="
                if (!(lst_val == now_val))
                    return false;
            if ((*op_iter)->GT_EQ()) // ">="
                if (!(lst_val >= now_val))
                    return false;
            if ((*op_iter)->LT_EQ()) // "<="
                if (!(lst_val <= now_val))
                    return false;
            if ((*op_iter)->NOT_EQ_2()) // "!="
                if (!(lst_val != now_val))
                    return false;
            lst_val = now_val;
        }
        return true;
    }
    virtual antlrcpp::Any visitIf_stmt(Python3Parser::If_stmtContext* ctx) override
    {
        auto test_list = ctx->test();
        auto test_iter = test_list.begin();
        auto suite_list = ctx->suite();
        auto suite_iter = suite_list.begin();
        bool flag = false;
        for (; test_iter != test_list.end(); test_iter++, suite_iter++) {
            antlrcpp::Any test_val = visitTest(*test_iter);
            Get_Val(test_val);
            if (Get_Bool(test_val)) {
                flag = true;
                visitSuite(*suite_iter);
                break;
            }
        }
        if (!flag && ctx->ELSE())
            visitSuite(*suite_list.rbegin());
        return 0;
        // antlrcpp::Any condition_val = visitTest(*test_list.begin());
        // Get_Val(condition_val);
        // if (!Get_Bool(condition_val.is<bool>()))
        // {
        // }
    }

    virtual antlrcpp::Any visitOr_test(Python3Parser::Or_testContext* ctx) override
    {
        if (ctx->and_test().size() == 1)
            return visitChildren(ctx);
		auto test_list = ctx->and_test();
		for (auto test_iter = test_list.begin();test_iter !=test_list.end();test_iter++)
			if (Get_Bool(visitAnd_test(*test_iter)))
				return true;
		return false;
    }

    virtual antlrcpp::Any visitAnd_test(Python3Parser::And_testContext* ctx) override
    {
        if (ctx->not_test().size() == 1)
            return visitChildren(ctx);
		auto test_list = ctx->not_test();
		for (auto test_iter = test_list.begin();test_iter !=test_list.end();test_iter++)
			if (!Get_Bool(visitNot_test(*test_iter)))
				return false;
		return true;
    }

    virtual antlrcpp::Any visitNot_test(Python3Parser::Not_testContext* ctx) override
    {
        if (ctx->NOT())
            return !(Get_Bool(visitChildren(ctx)));
        else
            return visitChildren(ctx);
    }
};

#endif //PYTHON_INTERPRETER_EVALVISITOR_H
